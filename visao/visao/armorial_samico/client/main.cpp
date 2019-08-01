#include <bits/stdc++.h>
#include "robocup_ssl_client.h"
#include "robocup_ssl_server.h"
#include "timer.h"
#include <thread>
#include <mutex>
#include <condition_variable>

#include "Robot.hpp"
#include "Ball.hpp"
#include "Loss.hpp"
#include "Noise.hpp"
#include "kalman.hpp"

#define PORT 8881
#define IP "127.0.0.1"
#define qt_robosTime 8
#define tempoFiltroRuido 0.1
#define tempoFiltroPerda 0.1
#define constanteCapirotica 50.0
#define maxRobots 8

#define printarInfoRobos false
#define printarInfoBola false
#define bateriasRandomicas true

using namespace std;
using namespace chrono;

/* funções do kalman */
KalmanFilter::KalmanFilter(
    double dt,
    const Eigen::MatrixXd& A,
    const Eigen::MatrixXd& C,
    const Eigen::MatrixXd& Q,
    const Eigen::MatrixXd& R,
    const Eigen::MatrixXd& P)
  : A(A), C(C), Q(Q), R(R), P0(P),
    m(C.rows()), n(A.rows()), dt(dt), initialized(false),
    I(n, n), x_hat(n), x_hat_new(n)
{
    I.setIdentity();
}

KalmanFilter::KalmanFilter() {}

void KalmanFilter::init(double t0, const Eigen::VectorXd& x0) {
    x_hat = x0;
    P = P0;
    this->t0 = t0;
    t = t0;
    initialized = true;
}

void KalmanFilter::init() {
    x_hat.setZero();
    P = P0;
    t0 = 0;
    t = t0;
    initialized = true;
}

bool KalmanFilter::checkInit(){
    return initialized;
}

void KalmanFilter::update(const Eigen::VectorXd& y) {

    if(!initialized)
        throw std::runtime_error("Filter is not initialized!");

    x_hat_new = A * x_hat;
    P = A*P*A.transpose() + Q;
    K = P*C.transpose()*(C*P*C.transpose() + R).inverse();
    x_hat_new += K * (y - C*x_hat_new);
    P = (I - K*C)*P;
    x_hat = x_hat_new;

    t += dt;
}

void KalmanFilter::update(const Eigen::VectorXd& y, double dt, const Eigen::MatrixXd A) {
    this->A = A;
    this->dt = dt;
    update(y);
}

void KalmanFilter::update(const Eigen::VectorXd& y, double dt, const Eigen::MatrixXd A, const Eigen::MatrixXd Q) {
    this->A = A;
    this->Q = Q;
    this->dt = dt;
    update(y);
}

/* funções do kalman */

/* estrutura de coordenadas do robo */
typedef struct{
    int id;
    double x, y, angle;
    bool isLoss;
} robot_coords;

/* pacote enviado para o samico */
typedef struct{
    char qt_blue;
    char qt_yellow;
    char camera_id;
    unsigned char battery[2][maxRobots];
    robot_coords robots_blue[maxRobots];
    robot_coords robots_yellow[maxRobots];
    pair<double, double> ball;
    bool isBallLoss;
} pacote;

/* estrutura que contem o robo e os filtros a ele associados */
typedef struct{
    Robot *robot = new Robot(0, true);
    KalmanFilter kalmanx;
    KalmanFilter kalmany;
    Loss lossFilter;
    Noise noiseFilter;
} roboFiltrado;

/* variaveis usadas para o socket com o samico*/
int server_fd, new_socket, opt = 1; 
struct sockaddr_in address;
int addrlen = sizeof(address);

/* variaveis dos filtros */
unsigned int filterTime;
bool firstKalmanInput[2][maxRobots] = {false};
bool firstKalmanBall = false;
KalmanFilter filterBallX;
KalmanFilter filterBallY;

/* vetores dos robos e bola */
vector<roboFiltrado> robosAzuis;
vector<roboFiltrado> robosAmarelos;
Ball *ball;

/* funções para inicializações do sistema */

void initRobots(pacote *robotsInfo){
    roboFiltrado aux;
    /* criação do vetor dinamico de objetos */
    for(uint8_t x = 0; x < qt_robosTime; x++){
        robosAzuis.push_back(aux);
        robosAzuis[x].robot->setId(x);
        robosAzuis[x].robot->setTeam(true);
        robosAzuis[x].noiseFilter.setFilterTime((tempoFiltroRuido)/constanteCapirotica);
        robosAzuis[x].lossFilter.setFilterTime((tempoFiltroPerda)/constanteCapirotica);
        (*robotsInfo).robots_blue[x].isLoss = true;

        robosAmarelos.push_back(aux);
        robosAmarelos[x].robot->setId(x);
        robosAmarelos[x].robot->setTeam(false);
        robosAmarelos[x].noiseFilter.setFilterTime((tempoFiltroRuido)/constanteCapirotica);
        robosAmarelos[x].lossFilter.setFilterTime((tempoFiltroPerda)/constanteCapirotica);
        (*robotsInfo).robots_yellow[x].isLoss = true;
    }
}

void initBall(pacote *robotsInfo){
    ball = new Ball();
    (*robotsInfo).isBallLoss = true;
}

void initSamicoSocket(){
    // criação do socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){ 
        cout << "Erro na criação do socket" << endl;
        exit(-1);
    } 
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){ 
        cout << "Erro na setsockopt" << endl;
        exit(-1); 
    }
    // setando o endereco e as portas
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT); 

    // tentando bindar o endereco (pegar prioridade) 
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){ 
        printf("Bind error.\n");
        exit(-1);
    } 
    // verificando se foi possivel dar o listen
    if(listen(server_fd, 3) < 0){
        printf("Listen error.\n");
        exit(-1);
    }
    if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){ 
        printf("Connection accept error.\n");
        exit(-1);
    }
}

void initKalman(){
    double dt = 1.0/30; // Time step

    Eigen::MatrixXd A(3, 3); // System dynamics matrix
    Eigen::MatrixXd C(1, 3); // Output matrix
    Eigen::MatrixXd Q(3, 3); // Process noise covariance
    Eigen::MatrixXd R(1, 1); // Measurement noise covariance
    Eigen::MatrixXd P(3, 3); // Estimate error covariance

    // Discrete LTI projectile motion, measuring position only
    A << 1, dt, dt*dt/2, 0, 1, dt, 0, 0, 1;
    C << 1, 0, 0;

    // Reasonable covariance matrices
    Q << dt*dt*dt*dt/4, dt*dt*dt/2, dt*dt/2, dt*dt*dt/2, dt*dt, dt, dt*dt/2, dt, 1;
    R << 0.001;
    P << 1, 0, 0, 0, 1, 0, 0, 0, 1;

    cout << "A: \n" << A << endl;
    cout << "C: \n" << C << endl;
    cout << "Q: \n" << Q << endl;
    cout << "R: \n" << R << endl;
    cout << "P: \n" << P << endl;

    for(int x = 0; x < maxRobots; x++){
        firstKalmanInput[0][x] = false;
        firstKalmanInput[1][x] = false;
        robosAzuis[x].kalmanx = KalmanFilter(dt, A, C, Q, R, P);
        robosAzuis[x].kalmany = KalmanFilter(dt, A, C, Q, R, P);
        robosAmarelos[x].kalmanx = KalmanFilter(dt, A, C, Q, R, P);
        robosAmarelos[x].kalmany = KalmanFilter(dt, A, C, Q, R, P);
    }
    
    filterBallX = KalmanFilter(dt, A, C, Q, R, P);
    filterBallY = KalmanFilter(dt, A, C, Q, R, P);
}

void filterRobot(SSL_DetectionFrame &detection, roboFiltrado *robo, bool isBlue, int iteration, pacote *robotsInfo){
    int id;
    robot_coords robot_aux_coord;

    if(isBlue){
        id = detection.robots_blue(iteration).robot_id();
        (*robo).robot->setId(detection.robots_blue(iteration).robot_id());
        (*robo).robot->setHeight(detection.robots_blue(iteration).height());
        (*robo).robot->setCoordinates(detection.robots_blue(iteration).x(), detection.robots_blue(iteration).y());
        (*robo).robot->setAngle(detection.robots_blue(iteration).orientation());
        (*robo).robot->setTeam(true);
    }else{
        id = detection.robots_yellow(iteration).robot_id();
        (*robo).robot->setId(detection.robots_yellow(iteration).robot_id());
        (*robo).robot->setHeight(detection.robots_yellow(iteration).height());
        (*robo).robot->setCoordinates(detection.robots_yellow(iteration).x(), detection.robots_yellow(iteration).y());
        (*robo).robot->setAngle(detection.robots_yellow(iteration).orientation());
        (*robo).robot->setTeam(false);
    }
    if(!(*robo).noiseFilter.isInitialized()){
        (*robo).noiseFilter.initCounter();
        robot_aux_coord.isLoss = true;
    }else{
        if((*robo).noiseFilter.noiseRobotFilter((*((*robo).robot)))){
            robot_aux_coord.isLoss = false;
        }else{
            robot_aux_coord.isLoss = true;
        }
    }
    (*robo).lossFilter.lossRobotFilter((*((*robo).robot)), true);

    if(isBlue){
        if(!firstKalmanInput[0][id]){
            firstKalmanInput[0][id] = true;
                
            Eigen::VectorXd x0(3);
            x0 << (*robo).robot->getRobotX(),0,0;
            (*robo).kalmanx.init(0,x0);

            Eigen::VectorXd x1(3); 
            x1 << (*robo).robot->getRobotY(),0,0;
            (*robo).kalmany.init(0,x1);
            
        }else{
            Eigen::VectorXd zz(1);
            zz << (*robo).robot->getRobotX();
            (*robo).kalmanx.update(zz);

            Eigen::VectorXd zy(1);
            zy << (*robo).robot->getRobotY();
            (*robo).kalmany.update(zy);
        }
    }else{
       if(!firstKalmanInput[1][id]){
            firstKalmanInput[1][id] = true;
                
            Eigen::VectorXd x0(3);
            x0 << (*robo).robot->getRobotX(),0,0;
            (*robo).kalmanx.init(0,x0);

            Eigen::VectorXd x1(3);
            x1 << (*robo).robot->getRobotY(),0,0;
            (*robo).kalmany.init(0,x1);
            
        }else{
            Eigen::VectorXd zz(1);
            zz << (*robo).robot->getRobotX();
            (*robo).kalmanx.update(zz);

            Eigen::VectorXd zy(1);
            zy << (*robo).robot->getRobotY();
            (*robo).kalmany.update(zy);
        } 
    }
    (*robo).robot->setCoordinates((*robo).kalmanx.state().transpose()(0), (*robo).kalmany.state().transpose()(0));
    if(printarInfoRobos) (*robo).robot->printRobotInfo();

    if(isBlue){
        robot_aux_coord.id = (*robo).robot->getId();
        robot_aux_coord.angle = (*robo).robot->getAngle();
        robot_aux_coord.x = (*robo).robot->getRobotX();
        robot_aux_coord.y = (*robo).robot->getRobotY();

        (*robotsInfo).robots_blue[id] = robot_aux_coord;
    }else{
        robot_aux_coord.id = (*robo).robot->getId();
        robot_aux_coord.angle = (*robo).robot->getAngle();
        robot_aux_coord.x = (*robo).robot->getRobotX();
        robot_aux_coord.y = (*robo).robot->getRobotY();

        (*robotsInfo).robots_yellow[id] = robot_aux_coord;
    }

}

void setRobotsInfo(SSL_DetectionFrame &detection, vector<roboFiltrado> &robosAzuis, vector<roboFiltrado> &robosAmarelos, pacote *robotsInfo){
    uint8_t qt_robosAzuis = detection.robots_blue_size();
    uint8_t qt_robosAmarelos = detection.robots_yellow_size();

    (*robotsInfo).qt_blue = maxRobots;
    (*robotsInfo).qt_yellow = maxRobots;
    (*robotsInfo).camera_id = detection.camera_id();
    
    for(uint8_t x = 0; x < qt_robosAzuis; x++){
        int id = detection.robots_blue(x).robot_id();
        if(!(id >= 0 && id < maxRobots))
            throw std::runtime_error("ID error, check setRobotsInfo");
        filterRobot(detection, &robosAzuis[id], true, x, robotsInfo);
    }

    for(uint8_t x = 0; x < qt_robosAmarelos; x++){
        int id = detection.robots_yellow(x).robot_id();
        if(!(id >= 0 && id < maxRobots))
            throw std::runtime_error("ID error, check setRobotsInfo");
        filterRobot(detection, &robosAmarelos[id], false, x, robotsInfo);
    }

    for(uint8_t x = 0; x < maxRobots; x++){
        if(robosAzuis[x].lossFilter.lossRobotFilter((*(robosAzuis[x].robot)), false)){
            (*robotsInfo).robots_blue[x].isLoss = true;
            robosAzuis[x].noiseFilter.setOff();
        }
        if(robosAmarelos[x].lossFilter.lossRobotFilter((*(robosAmarelos[x].robot)), false)){
            (*robotsInfo).robots_yellow[x].isLoss = true;
            robosAmarelos[x].noiseFilter.setOff();
        }
    }
}

void setBallInfo(SSL_DetectionFrame &detection, Ball &ball, pacote *robotsInfo){
    static Loss lossBall;
    static Noise noiseBall;
    if(detection.balls_size() > 0){
        ball.setCoordinates(detection.balls(0).x(), detection.balls(0).y());
        noiseBall.noiseBallFilter(ball);
        lossBall.lossBallFilter(ball, true);
        if(firstKalmanBall == false){
            firstKalmanBall = true;
            Eigen::VectorXd x0(3);
            x0 << ball.getBallX(),0,0;
            filterBallX.init(0,x0);
            Eigen::VectorXd x1(3);
            x1 << ball.getBallY(),0,0;
            filterBallY.init(0,x1);
        }else{
            Eigen::VectorXd zz(1);
            zz << ball.getBallX();
            filterBallX.update(zz);
            Eigen::VectorXd zy(1);
            zy << ball.getBallY();
            filterBallY.update(zy);
        }
        (*robotsInfo).ball = make_pair(ball.getBallX(), ball.getBallY());
        (*robotsInfo).isBallLoss = false;
        if(printarInfoBola) ball.printBallInfo();
    }else{
        lossBall.lossBallFilter(ball, false);
    }
}

void gerarBaterias(pacote *robotsInfo){
    srand(time(NULL));
    for(int x = 0; x < 2; x++){
        for(int y = 0; y < maxRobots; y++){
            (*robotsInfo).battery[x][y] = rand()%100;
            if(x == 0) cout << "Robo " << y << " do time azul: " << (int)(*robotsInfo).battery[x][y] << " de bateria." << endl;
            else cout << "Robo " << y << " do time amarelo: " << (int)(*robotsInfo).battery[x][y] << " de bateria." << endl;
        }
    }
}

void debugPacket(pacote robosInfo){
    printf("pacote debug:\n");
    printf("Robos azuis:\n");
    for(int x = 0; x < robosInfo.qt_blue; x++){
        printf("ID: %d\n", robosInfo.robots_blue[x].id);
        printf("X: %lf\n", robosInfo.robots_blue[x].x);
        printf("Y: %lf\n", robosInfo.robots_blue[x].y);
        printf("ANGLE: %lf\n", robosInfo.robots_blue[x].angle);
    }
    printf("\nRobos amarelos:\n");
    for(int x = 0; x < robosInfo.qt_yellow; x++){
        printf("ID: %d\n", robosInfo.robots_yellow[x].id);
        printf("X: %lf\n", robosInfo.robots_yellow[x].x);
        printf("Y: %lf\n", robosInfo.robots_yellow[x].y);
        printf("ANGLE: %lf\n\n", robosInfo.robots_yellow[x].angle);
    }
}

int main(){
    // abrindo o cliente do SSL-Vision
    RoboCupSSLClient client;
    client.open(true);
    SSL_WrapperPacket packet;

    // pacote enviado para o samico
    pacote robosInfo;

    initRobots(&robosInfo);
    initBall(&robosInfo);
    initKalman();
    initSamicoSocket();
    if(bateriasRandomicas) gerarBaterias(&robosInfo);
    while(true){
        if(client.receive(packet)){
            if(packet.has_detection()){
                SSL_DetectionFrame detection = packet.detection();
                setRobotsInfo(detection, robosAzuis, robosAmarelos, &robosInfo);
                setBallInfo(detection, *ball, &robosInfo);
                send(new_socket, &robosInfo, sizeof(pacote), 0);
            }
        }
    }
}