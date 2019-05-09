#include <bits/stdc++.h>
#include "robocup_ssl_client.h"
#include "timer.h"

#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"

#include "Robot.hpp"
#include "Ball.hpp"
#include "Loss.hpp"
#include "Noise.hpp"

#define qt_robosTime 8
#define tempoFiltro 500
using namespace std;
using namespace chrono;

unsigned int filterTime;

/* Robot methods */

Robot::Robot(uint8_t id, bool teamBlue){
    this->robot_id = id;
    this->active = true;
    this->teamBlue = teamBlue;
}

Robot::~Robot(){
    this->active = false;
}

void Robot::printRobotInfo(){
    if(this->getTeam() == 0) printf("Yellow Robot |");
    else printf("Blue Robot |");

    printf("ID: %d\t", this->robot_id);
    printf("Altura: %6.2lf | Posicao: <%9.2lf,%9.2lf |", this->getHeight(), this->getRobotX(), this->getRobotY());
    printf("Angulo: %6.2lf |\n", this->getAngle());
}

void Robot::setCoordinates(double coordX, double coordY){
    this->coordX = coordX;
    this->coordY = coordY;
}

void Robot::setId(uint8_t id){
    this->robot_id = id;
}

void Robot::setAngle(double angle){
    this->angle = angle;
}

void Robot::setHeight(double height){
    this->height = height;
}

double Robot::getRobotX(){
    return this->coordX;
}

double Robot::getRobotY(){
    return this->coordY;
}

double Robot::getHeight(){
    return this->height;
}

double Robot::getAngle(){
    return this->angle;
}

bool Robot::isActive(){
    return this->active;
}

bool Robot::getTeam(){
    return this->teamBlue;
}

/* end of Robot methods */

/* Ball methods */

Ball::Ball(){
    this->active = true;
    this->coordX = 0;
    this->coordY = 0;
}   

Ball::~Ball(){
    this->active = false;
    this->coordX = 0;
    this->coordY = 0;
}

void Ball::setCoordinates(double coordX, double coordY){
    this->coordX = coordX;
    this->coordY = coordY;
}

double Ball::getBallX(){
    return this->coordX;
}

double Ball::getBallY(){
    return this->coordY;
}

void Ball::printBallInfo(){
    printf("Bola\tPosicao: <%9.2lf, %9.2lf>\n", this->getBallX(), this->getBallY());
}

bool Ball::isActive(){
    return this->active;
}

/* end of Ball methods */

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

void setRobotsInfo(SSL_DetectionFrame &detection, vector<Robot> &robosAzuis, vector<Robot> &robosAmarelos){
    uint8_t qt_robosAzuis = detection.robots_blue_size();
    uint8_t qt_robosAmarelos = detection.robots_yellow_size();
    
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
        robosAzuis[x].printRobotInfo();
    }
    for(uint8_t x = 0; x < qt_robosAmarelos; x++){
        robosAmarelos[x].setId(detection.robots_yellow(x).robot_id());
        robosAmarelos[x].setHeight(detection.robots_yellow(x).height());
        robosAmarelos[x].setCoordinates(detection.robots_yellow(x).x(), detection.robots_yellow(x).y());
        robosAmarelos[x].setAngle(detection.robots_yellow(x).orientation());
        noiseYellowRobots[x].noiseRobotFilter(robosAmarelos[x]);
        lossYellowRobots[x].lossRobotFilter(robosAmarelos[x]);
        robosAmarelos[x].printRobotInfo();
    }
}

void setBallInfo(SSL_DetectionFrame &detection, Ball &ball){
    if(detection.balls_size() > 0){
        static Loss lossBall;
        static Noise noiseBall;
        ball.setCoordinates(detection.balls(0).x(), detection.balls(0).y());
        noiseBall.noiseBallFilter(ball);
        lossBall.lossBallFilter(ball);
        ball.printBallInfo();
    }
}

int main(){
    setFilterTime(500);
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

    while(true){
        if(client.receive(packet)){
            if(packet.has_detection()){
                SSL_DetectionFrame detection = packet.detection();
                setBallInfo(detection, *ball);
                setRobotsInfo(detection, robosAzuis, robosAmarelos);
            }
        }
    }
}