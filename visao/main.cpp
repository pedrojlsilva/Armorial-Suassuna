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

#define PORT 8881
#define IP "127.0.0.1"
#define qt_robosTime 8
#define tempoFiltro 500
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

mutex info_mutex;
unsigned int filterTime;
bool proc1 = false;

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
        noiseBlueRobots[x].noiseRobotFilter(robosAzuis[x]);
        lossBlueRobots[x].lossRobotFilter(robosAzuis[x]);
        robot_aux_coord.id = robosAzuis[x].robot_id;
        robot_aux_coord.x = robosAzuis[x].getRobotX();
        robot_aux_coord.y = robosAzuis[x].getRobotY();
        robot_aux_coord.angle = robosAzuis[x].getAngle();
        (*robotsInfo).robots_blue[x] = robot_aux_coord;
        //robosAzuis[x].printRobotInfo();
    }
    for(uint8_t x = 0; x < qt_robosAmarelos; x++){
        robosAmarelos[x].setId(detection.robots_yellow(x).robot_id());
        robosAmarelos[x].setHeight(detection.robots_yellow(x).height());
        robosAmarelos[x].setCoordinates(detection.robots_yellow(x).x(), detection.robots_yellow(x).y());
        robosAmarelos[x].setAngle(detection.robots_yellow(x).orientation());
        noiseYellowRobots[x].noiseRobotFilter(robosAmarelos[x]);
        lossYellowRobots[x].lossRobotFilter(robosAmarelos[x]);
        robot_aux_coord.id = robosAmarelos[x].robot_id;
        robot_aux_coord.x = robosAmarelos[x].getRobotX();
        robot_aux_coord.y = robosAmarelos[x].getRobotY();
        robot_aux_coord.angle = robosAmarelos[x].getAngle();
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
        (*robotsInfo).ball = make_pair(ball.getBallX(), ball.getBallY());
        //ball.printBallInfo();
    }
}

int main(){
    setFilterTime(tempoFiltro);
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
    address.sin_port = htons( PORT ); 
       
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