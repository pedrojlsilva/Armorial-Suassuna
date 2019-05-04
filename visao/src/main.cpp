#include <bits/stdc++.h>
#include "robocup_ssl_client.h"
#include "timer.h"

#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"

#include "Robot.hpp"
#include "Ball.hpp"

#define qt_robosTime 8
// Robot and Ball coordinates
using namespace std;
using namespace chrono;

// Filters to use 
class Filters{


	public:
    void noiseFilter(Robot &newCoord){
	
		static double initTime = clock();
		static Robot filtCoord = newCoord;
		
		if(clock() - initTime > 500){
		
			initTime = clock();
			filtCoord = newCoord;
		}
		
	}
	
	
	void noiseFilter(Ball &newCoord){
	
		static double initTimeB = clock();
		static Ball filtCoordB = newCoord;
		
		if(clock() - initTimeB > 500){
		
			initTimeB = clock();
			filtCoordB = newCoord;
		}

	}
	
	/*
	
	bool lossFilter(void){
	
		static double iniTime = clock();
		static double filterTime=200;
	
		bool ret = clock()-iniTime>=200;
		if(ret){
		
			iniTime=clock();
		
		}
		
		return ret;
	
	
	}
*/
	/*blablalbalblaa*/
};



// globais
vector<Robot> robosAzuis;
vector<Robot> robosAmarelos;



/* ball methods */
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

/* end of ball methods */

/* robot methods */
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

/* end of robot methods */

void setRobotsInfo(SSL_DetectionFrame &detection, vector<Robot> &robosAzuis, vector<Robot> &robosAmarelos){
    uint8_t qt_robosAzuis = detection.robots_blue_size();
    uint8_t qt_robosAmarelos = detection.robots_yellow_size();
    
    static Filters *filteredRobsAz;
    filteredRobsAz = (Filters *) malloc(qt_robosAzuis * sizeof(Filters));
    static Filters *filteredRobsAm;
    filteredRobsAm = (Filters *) malloc(qt_robosAmarelos * sizeof(Filters));
    
    for(uint8_t x = 0; x < qt_robosAzuis; x++){
        robosAzuis[x].setId(detection.robots(x).robot_id());
        robosAzuis[x].setHeight(detection.robots_blue(x).height());
        robosAzuis[x].setCoordinates(detection.robots_blue(x).x(), detection.robots_blue(x).y());
        robosAzuis[x].setAngle(detection.robots_blue(x).orientation());
        filteredRobsAz[x].noiseFilter(robosAzuis[x]);
        robosAzuis[x].printRobotInfo();
    }
    for(uint8_t x = 0; x < qt_robosAmarelos; x++){
        robosAmarelos[x].setId(detection.robots(x).robot_id());
        robosAmarelos[x].setHeight(detection.robots_yellow(x).height());
        robosAmarelos[x].setCoordinates(detection.robots_yellow(x).x(), detection.robots_yellow(x).y());
        robosAmarelos[x].setAngle(detection.robots_yellow(x).orientation());
        filteredRobsAm[x].noiseFilter(robosAmarelos[x]);
        robosAmarelos[x].printRobotInfo();
    }
}

void setBallInfo(SSL_DetectionFrame &detection, Ball &ball){
    if(detection.balls_size() > 0){
        static Filters filteredBall;
        ball.setCoordinates(detection.balls(0).x(), detection.balls(0).y());
        filteredBall.noiseFilter(ball);
        ball.printBallInfo();
    }
}

int main(){
    /* criação do vetor dinamico de objetos */
    vector<Robot> robosAzuis;
    vector<Robot> robosAmarelos;
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