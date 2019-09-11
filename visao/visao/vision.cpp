#include <bits/stdc++.h>
#include <iostream>
#include <netinet/in.h>
#include <vector>
#include <thread>
#include "types/position.h"
#include "types/angle.h"
#include "Ball/ball.h"
#include "Robot/robot.h"
#include "types/frame.h"
#include "samico.h"


#include "include/3rd_party/robocup_ssl_client.h"
#include "include/3rd_party/messages_robocup_ssl_wrapper.pb.h"

#define qt_robosTime 8
#define tempoFiltros 300
#define maxRobots 8

#define printarInfoRobos false
#define printarInfoBola false
#define bateriasRandomicas true

Frame *robotsInfo = new Frame(qt_robosTime, tempoFiltros);
Position newPosition = Position();
Angle newOrientation = Angle();


/* variables */

vector<Robot> blueRobots;
vector<Robot> yellowRobots;
Ball *ball;
Robot *aux;
Samico *samico = new Samico();




void setRobotsInfo(SSL_DetectionFrame &detection){
    int qt_blueRobots = detection.robots_blue_size();
    int qt_yellowRobots = detection.robots_yellow_size();
    quint32 camera_id = detection.camera_id();
    if(qt_blueRobots != 0) robotsInfo->_qt_blue=qt_blueRobots;
    if(qt_yellowRobots != 0) robotsInfo->_qt_yellow=qt_yellowRobots;
    robotsInfo->_camera_id=camera_id;
    cout<<"amarelos:   "<< robotsInfo->_qt_blue<<std::endl;
    cout<<"azuis:      "<< robotsInfo->_qt_yellow<<std::endl;
    cout << "la no samico ta: " << robotsInfo->_qt_blue << " " << robotsInfo->_qt_yellow << endl;
    for(int x = 0; x < qt_blueRobots; x++){
        quint32 id = detection.robots_blue(x).robot_id();

        if(!(id < maxRobots)){
            throw std::runtime_error("ID error, check setRobotsInfo");
        }
        Position *pos_aux = new Position(true, detection.robots_blue(x).x(), detection.robots_blue(x).y());
        Angle *angle_aux = new Angle(true, detection.robots_blue(x).orientation());

        if(robotsInfo->_blueRobots[id]._position.isValid()){// se a posição for valida, ele passa para frame
            robotsInfo->_blueRobots[id].setRobotId(id);
            robotsInfo->_blueRobots[id].update(100, *pos_aux, *angle_aux);
        }
    }

    for(int x = 0; x < qt_yellowRobots; x++){
        quint32 id = detection.robots_yellow(x).robot_id();

        if(!(id < maxRobots)){
            throw std::runtime_error("ID error, check setRobotsInfo");
        }
        Position *pos_aux = new Position(true, detection.robots_yellow(x).x(), detection.robots_yellow(x).y());
        Angle *angle_aux = new Angle(true, detection.robots_yellow(x).orientation());
        if(robotsInfo->_yellowRobots[id]._position.isValid()){// se a posição for valida, ele passa para frame
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


void samico_drawThread(){
    samico->drawWindow();
}

int main(){
    // opening ssl vision client
    RoboCupSSLClient client;
    client.open(true);
    SSL_WrapperPacket packet;

    // samico draw thread
    samico->getWindow()->setActive(false); // deactivating samico in main thread
    thread first (samico_drawThread);
    first.detach();

    if(bateriasRandomicas) {
        gerarBaterias();
    }

    while(true){
        if(client.receive(packet)){
            if(packet.has_detection()){
               SSL_DetectionFrame detection = packet.detection();
                setRobotsInfo(detection);
                setBallInfo(detection);
                samico->setFrame(robotsInfo);
                //samico->drawWindow();
            }
        }
    }
}





