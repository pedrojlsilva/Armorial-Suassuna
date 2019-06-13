#include <bits/stdc++.h>
#include "robocup_ssl_client.h"
#include "robocup_ssl_server.h"
#include "timer.h"
#include <thread>
#include <mutex>

#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"

#include "Robot.hpp"
#include "Ball.hpp"
#include "Loss.hpp"
#include "Noise.hpp"
#include "kalman.hpp"

#define PORT 8881
#define IP "127.0.0.1"
#define qt_robosTime 8
#define tempoFiltro 200
#define maxRobots 8
using namespace std;
using namespace chrono;

typedef struct{
    int id;
    double x, y, angle;
} robot_coords;

typedef struct{
    int qt_blue;
    int qt_yellow;
    int camera_id;
    unsigned char battery[2][maxRobots]; // 1byte (0 a 100)
    robot_coords robots_blue[maxRobots];
    robot_coords robots_yellow[maxRobots];
    pair<double, double> ball;
} pacote;

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

mutex info_mutex;
unsigned int filterTime;
bool proc1 = false;

vector<KalmanFilter> kFiltersXB;
vector<KalmanFilter> kFiltersYB;

vector<KalmanFilter> kFiltersXY;
vector<KalmanFilter> kFiltersYY;
KalmanFilter filterBallX;
KalmanFilter filterBallY;


bool firstKalmanInputB[maxRobots] = {false};
bool firstKalmanInputY[maxRobots] = {false};
bool firstKalmanBall = false;

void initializeKalman(){
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

    std::cout << "A: \n" << A << std::endl;
    std::cout << "C: \n" << C << std::endl;
    std::cout << "Q: \n" << Q << std::endl;
    std::cout << "R: \n" << R << std::endl;
    std::cout << "P: \n" << P << std::endl;

    for(int x = 0; x < maxRobots; x++){
        firstKalmanInputB[x] = false;
        firstKalmanInputY[x] = false;
        kFiltersXB.push_back(KalmanFilter(dt,A, C, Q, R, P));
        kFiltersYB.push_back(KalmanFilter(dt,A, C, Q, R, P));
        kFiltersXY.push_back(KalmanFilter(dt,A, C, Q, R, P));
        kFiltersYY.push_back(KalmanFilter(dt,A, C, Q, R, P));
    }

    filterBallX = KalmanFilter(dt, A, C, Q, R, P);
    filterBallY = KalmanFilter(dt, A, C, Q, R, P);

}

/* loss filter methods */

bool Loss::lossRobotFilter(Robot &newCoord){
	static double iniTime = clock();
	bool ret = (clock() - iniTime >= filterTime);
	if(ret) newCoord.~Robot();
	return ret;
}

bool Loss::lossBallFilter(Ball &newCoord){
	static double iniTime = clock();
	bool ret = (clock() - iniTime >= filterTime);
	if(ret) newCoord.~Ball();
	return ret;
}
 
/* end of loss filter methods */

/* noise filter methods */

void Noise::noiseRobotFilter(Robot &newCoord){
	static double initTime = clock();
	static Robot filtCoord = newCoord;
	
	if(clock() - initTime > filterTime){
		initTime = clock();
		filtCoord = newCoord;
	}
}

void Noise::noiseBallFilter(Ball &newCoord){	
	static double initTimeB = clock();
	static Ball filtCoordB = newCoord;

	if(clock() - initTimeB > filterTime){
		initTimeB = clock();
		filtCoordB = newCoord;
	}
}

/* end of noise filter methods */

void setFilterTime(unsigned int time){
    filterTime = time;
}

int glob = 0;

void setRobotsInfo(SSL_DetectionFrame &detection, vector<Robot> &robosAzuis, vector<Robot> &robosAmarelos, pacote *robotsInfo){
    uint8_t qt_robosAzuis = detection.robots_blue_size();
    uint8_t qt_robosAmarelos = detection.robots_yellow_size();

    (*robotsInfo).qt_blue = qt_robosAzuis;
    (*robotsInfo).qt_yellow = qt_robosAmarelos;
    (*robotsInfo).camera_id = detection.camera_id();
    robot_coords robot_aux_coord;

    static Noise *noiseBlueRobots;
    noiseBlueRobots = (Noise *) malloc(qt_robosAzuis * sizeof(Noise));
    static Noise *noiseYellowRobots;
    noiseYellowRobots = (Noise *) malloc(qt_robosAmarelos * sizeof(Noise));
    static Loss *lossBlueRobots;
    lossBlueRobots = (Loss *) malloc(qt_robosAzuis * sizeof(Loss));
    static Loss *lossYellowRobots;
    lossBlueRobots = (Loss *) malloc(qt_robosAmarelos * sizeof(Loss));
    
    for(uint8_t x = 0; x < qt_robosAzuis; x++){
        robosAzuis[x].setId(detection.robots_blue(x).robot_id());
        robosAzuis[x].setHeight(detection.robots_blue(x).height());
        robosAzuis[x].setCoordinates(detection.robots_blue(x).x(), detection.robots_blue(x).y());
        robosAzuis[x].setAngle(detection.robots_blue(x).orientation());
        robot_aux_coord.id = robosAzuis[x].robot_id;
        robot_aux_coord.angle = robosAzuis[x].getAngle();
        noiseBlueRobots[x].noiseRobotFilter(robosAzuis[x]);
        lossBlueRobots[x].lossRobotFilter(robosAzuis[x]);
        if(firstKalmanInputB[robosAzuis[x].robot_id] == false){
            firstKalmanInputB[robosAzuis[x].robot_id] = true;
            Eigen::VectorXd x0(3);
            x0 << robosAzuis[x].getRobotX(),0,0;
            kFiltersXB[robosAzuis[x].robot_id].init(0,x0);
            Eigen::VectorXd x1(3);
            x1 << robosAzuis[x].getRobotY(),0,0;
            kFiltersYB[robosAzuis[x].robot_id].init(0,x1);
        }else{
            Eigen::VectorXd zz(1);
            zz << robosAzuis[x].getRobotX();
            kFiltersXB[robosAzuis[x].robot_id].update(zz);
            Eigen::VectorXd zy(1);
            zy << robosAzuis[x].getRobotY();
            kFiltersYB[robosAzuis[x].robot_id].update(zy);
        }
        robot_aux_coord.x = kFiltersXB[robosAzuis[x].robot_id].state().transpose()(0);
        robot_aux_coord.y = kFiltersYB[robosAzuis[x].robot_id].state().transpose()(0);
        //robosAzuis[x].printRobotInfo();
        //cout << "chegou: " << detection.robots_blue(x).x() << " " << detection.robots_blue(x).y() << endl;
        //cout << robot_aux_coord.x << " " << robot_aux_coord.y << endl;
        (*robotsInfo).robots_blue[x] = robot_aux_coord;
    }
    for(uint8_t x = 0; x < qt_robosAmarelos; x++){
        robosAmarelos[x].setId(detection.robots_yellow(x).robot_id());
        robosAmarelos[x].setHeight(detection.robots_yellow(x).height());
        robosAmarelos[x].setCoordinates(detection.robots_yellow(x).x(), detection.robots_yellow(x).y());
        robosAmarelos[x].setAngle(detection.robots_yellow(x).orientation());
        robot_aux_coord.id = robosAmarelos[x].robot_id;
        robot_aux_coord.angle = robosAmarelos[x].getAngle();
        noiseYellowRobots[x].noiseRobotFilter(robosAmarelos[x]);
        lossYellowRobots[x].lossRobotFilter(robosAmarelos[x]);
        if(firstKalmanInputY[robosAmarelos[x].robot_id] == false){
            firstKalmanInputY[robosAmarelos[x].robot_id] = true;
            Eigen::VectorXd x0(3);
            x0 << robosAmarelos[x].getRobotX(),0,0;
            kFiltersXY[robosAmarelos[x].robot_id].init(0,x0);
            Eigen::VectorXd x1(3);
            x1 << robosAmarelos[x].getRobotY(),0,0;
            kFiltersYY[robosAmarelos[x].robot_id].init(0,x1);
        }else{
            Eigen::VectorXd zz(1);
            zz << robosAmarelos[x].getRobotX();
            kFiltersXY[robosAmarelos[x].robot_id].update(zz);
            Eigen::VectorXd zy(1);
            zy << robosAmarelos[x].getRobotY();
            kFiltersYY[robosAmarelos[x].robot_id].update(zy);
        }
        robot_aux_coord.x = kFiltersXY[robosAmarelos[x].robot_id].state().transpose()(0);
        robot_aux_coord.y = kFiltersYY[robosAmarelos[x].robot_id].state().transpose()(0);
        (*robotsInfo).robots_yellow[x] = robot_aux_coord;
        //robosAmarelos[x].printRobotInfo();
    }
}

void setBallInfo(SSL_DetectionFrame &detection, Ball &ball, pacote *robotsInfo){
    if(detection.balls_size() > 0){
        static Loss lossBall;
        static Noise noiseBall;
        ball.setCoordinates(detection.balls(0).x(), detection.balls(0).y());
        noiseBall.noiseBallFilter(ball);
        lossBall.lossBallFilter(ball);
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
        ball.printBallInfo();
    }
}

int main(){
    setFilterTime(tempoFiltro);
    initializeKalman();
    vector<Robot> robosAzuis;
    vector<Robot> robosAmarelos;
    /* criação do vetor dinamico de objetos */
    for(uint8_t x = 0; x < qt_robosTime; x++){
        Robot roboAzul(x, true);
        Robot roboAmarelo(x, false);
        robosAzuis.push_back(roboAzul);
        robosAmarelos.push_back(roboAmarelo);
    }
    Ball *ball = new Ball();

    RoboCupSSLClient client;
    client.open(true);
    SSL_WrapperPacket packet;
    pacote robosInfo;

    // criação do socket
    int server_fd, new_socket, opt = 1; 
    struct sockaddr_in address;
    int addrlen = sizeof(address);
     
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){ 
        cout << "Erro na criação do socket" << endl;
        exit(-1);
    } 
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){ 
        cout << "Erro na setsockopt" << endl;
        exit(-1); 
    }
    // setando o endereco e as portas
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT); 
       
    // tentando bindar o endereco (pegar prioridade) 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){ 
        cout << "Erro no bind" << endl;
        exit(-1);
    } 
    
    // verificando se foi possivel dar o listen
    if (listen(server_fd, 3) < 0){ 
        cout << "Erro no listen" << endl;
        exit(-1);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){ 
        cout << "Erro ao aceitar a conexao" << endl;
        exit(-1);
    }

    /* gerando umas baterias randomicamente */
    for(int x = 0; x < 2; x++){
        for(int y = 0; y < maxRobots; y++){
            robosInfo.battery[x][y] = rand()%100;
            cout << "randomizei: " << (int)robosInfo.battery[x][y] << endl;
        }
    }
    
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