#include <bits/stdc++.h>
#include <iostream>
#include <netinet/in.h>
#include <vector>
#include "types/position.h"
#include "types/angle.h"
#include "Ball/ball.h"
#include "Robot/robot.h"
#include "types/frame.h"
#include "samico.h"

#include "include/3rd_party/robocup_ssl_client.h"
#include "include/3rd_party/messages_robocup_ssl_wrapper.pb.h"


#define PORT 8881
#define IP "127.0.0.1"
#define qt_robosTime 8
#define tempoFiltros 300
#define maxRobots 8

#define printarInfoRobos false
#define printarInfoBola false
#define bateriasRandomicas true

Frame *robotsInfo = new Frame();
Position newPosition = Position();
Angle newOrientation = Angle();


using namespace std;
using namespace chrono;


/* variables */

vector<Robot> blueRobots;
vector<Robot> yellowRobots;
Ball *ball;
Robot *aux;
Samico *samico;


void initRobots(){
   aux = new Robot(0, 1, 1, true, true,true,true);

    for(uint8_t x = 0; x < qt_robosTime; x++){

        blueRobots.push_back(*aux);
        blueRobots[x].setKalman(true);
        blueRobots[x].setLoss(true);
        blueRobots[x].setNoise(true);
        blueRobots[x].setFilterTimes(tempoFiltros);

        yellowRobots.push_back(*aux);
        yellowRobots[x].setKalman(true);
        yellowRobots[x].setLoss(true);
        yellowRobots[x].setNoise(true);
        yellowRobots[x].setFilterTimes(tempoFiltros);
    }
}

void initBall(){
    ball = new Ball();
    ball->setKalman(true);;
    ball->setLoss(true);;
    ball->setNoise(true);;
    ball->setFilterTimes(tempoFiltros);

}



void setRobotsInfo(SSL_DetectionFrame &detection, vector<Robot> &blueRobots, vector<Robot> &yellowRobots){
    int qt_blueRobots = detection.robots_blue_size();
    int qt_yellowRobots = detection.robots_yellow_size();
    quint32 camera_id = detection.camera_id();
    robotsInfo->_qt_blue=qt_blueRobots;
    robotsInfo->_qt_yellow=qt_yellowRobots;
    robotsInfo->_camera_id=camera_id;




    for(uint8_t x = 0; x < qt_blueRobots; x++){
        quint32 id = detection.robots_blue(x).robot_id();
        if(!( id < maxRobots))
            throw std::runtime_error("ID error, check setRobotsInfo");
        Position *pos_aux = new Position(true, detection.robots_blue(x).x(), detection.robots_blue(x).y());
        Angle *angle_aux = new Angle(true, detection.robots_blue(x).orientation());
        blueRobots[id].update(100, *pos_aux, *angle_aux);
        if(blueRobots[id]._position.isValid()){// se a posição for valida, ele passa para frame
            robotsInfo->_blueRobots[id].setRobotId(id);
            robotsInfo->_blueRobots[id].update(100, *pos_aux, *angle_aux);

        }
    }

    for(uint8_t x = 0; x < qt_yellowRobots; x++){
        quint32 id = detection.robots_yellow(x).robot_id();
        if(!(id < maxRobots))
            throw std::runtime_error("ID error, check setRobotsInfo");
        Position *pos_aux = new Position(true, detection.robots_yellow(x).x(), detection.robots_yellow(x).y());
        Angle *angle_aux = new Angle(true, detection.robots_yellow(x).orientation());
        yellowRobots[id].update(100, *pos_aux, *angle_aux);
        if(yellowRobots[id]._position.isValid()){// se a posição for valida, ele passa para frame
            robotsInfo->_yellowRobots[id].setRobotId(id);
            robotsInfo->_yellowRobots[id].update(100, *pos_aux, *angle_aux);
        }
    }

}

void setBallInfo(SSL_DetectionFrame &detection){
    if(detection.balls_size() > 0){
        Position *pos_aux = new Position(true, detection.balls(0).x(), detection.balls(0).y());
        Angle *angle_aux = new Angle(true, 0);
        robotsInfo->_ball.update(100, *pos_aux, *angle_aux);

    }
}

void gerarBaterias(){
    srand(time(NULL));
    for(int y = 0; y < maxRobots; y++){
        robotsInfo->_blueRobots[y].setBattery(rand()%100);
        robotsInfo->_yellowRobots[y].setBattery(rand()%100);
    }
}

//void debugPacket(Frame robotsInfo){
//    printf("pacote debug:\n");
//    printf("Blue Robots:\n");
//    for(int x = 0; x < robosInfo.qt_blue; x++){
//        printf("ID: %d\n", robosInfo.robots_blue[x].id);
//        printf("X: %lf\n", robosInfo.robots_blue[x].x);
//        printf("Y: %lf\n", robosInfo.robots_blue[x].y);
//        printf("ANGLE: %lf\n", robosInfo.robots_blue[x].angle);
//    }
//    printf("\nYellow Robots:\n");
//    for(int x = 0; x < robosInfo.qt_yellow; x++){
//        printf("ID: %d\n", robosInfo.robots_yellow[x].id);
//        printf("X: %lf\n", robosInfo.robots_yellow[x].x);
//        printf("Y: %lf\n", robosInfo.robots_yellow[x].y);
//        printf("ANGLE: %lf\n\n", robosInfo.robots_yellow[x].angle);
//    }
//}

int main(){
    // opening ssl vision client
    RoboCupSSLClient client;
    client.open(true);
    SSL_WrapperPacket packet;

    //samico packet
//    pacote robotsInfo;
    initRobots();
    initBall();
//    initSamicoSocket();
    if(bateriasRandomicas) gerarBaterias();

    while(true){
        if(client.receive(packet)){
            if(packet.has_detection()){
               SSL_DetectionFrame detection = packet.detection();
                setRobotsInfo(detection, blueRobots, yellowRobots);
                setBallInfo(detection);
                samico->setPacket();
                samico->drawWindow();
//                send(new_socket, &robotsInfo, sizeof(pacote), 0);
            }
        }
    }
}





