#include <bits/stdc++.h>
#include <math.h>
#include <iostream>
#include <netinet/in.h>
#include <vector>
#include <thread>
#include <algorithm>
#include "types/types.h"
#include "Ball/ball.h"
#include "Robot/robot.h"
#include "types/frame.h"
#include "grsSimulator/grsSimulator.h"
#include "samico.h"


#include "include/3rd_party/robocup_ssl_client.h"
#include "include/3rd_party/messages_robocup_ssl_wrapper.pb.h"

#define qt_robosTime 8
#define tempoFiltros 300.0
#define maxRobots 8

#define printarInfoRobos false
#define printarInfoBola false
#define bateriasRandomicas true

Frame *robotsInfo = new Frame(qt_robosTime, tempoFiltros);
Position newPosition = Position();
Angle newOrientation = Angle();

/* simulador do grsim */
grsSimulator *grSim = new grsSimulator();
grs_robot grSim_robot;

/* variables */
vector<Robot> blueRobots;
vector<Robot> yellowRobots;
Ball *ball;
Robot *aux;
Samico *samico = new Samico();

void setRobotsInfo(SSL_DetectionFrame &detection){
    bool visited[maxRobots];
    int qt_blueRobots = detection.robots_blue_size();
    int qt_yellowRobots = detection.robots_yellow_size();
    quint32 camera_id = detection.camera_id();

    if(qt_blueRobots != 0) robotsInfo->_qt_blue=qt_blueRobots;
    if(qt_yellowRobots != 0) robotsInfo->_qt_yellow=qt_yellowRobots;

    robotsInfo->_camera_id=camera_id;

    memset(visited, false, sizeof(visited)); // clear visited robots

    for(int x = 0; x < qt_blueRobots; x++){
        quint32 id = detection.robots_blue(x).robot_id();

        if(!(id < maxRobots)){
            throw std::runtime_error("ID error, check setRobotsInfo");
        }

        Position *pos_aux = new Position(true, detection.robots_blue(x).x(), detection.robots_blue(x).y());
        Angle *angle_aux = new Angle(true, detection.robots_blue(x).orientation());

        robotsInfo->_blueRobots[id].setRobotId(id);
        robotsInfo->_blueRobots[id].update(100, *pos_aux, *angle_aux);

        visited[id] = true; // mark as visited
    }

    for(int x = 0; x < maxRobots && qt_blueRobots != 0; x++){
        if(!visited[x] && !robotsInfo->_blueRobots[x].checkLoss()){
            // se nao tiver sido visitado pelo frame anterior mas ainda estiver rodando loss, da predict
            robotsInfo->_blueRobots[x].predict();
        }
    }

    for(int x = 0; x < qt_yellowRobots; x++){
        quint32 id = detection.robots_yellow(x).robot_id();

        if(!(id < maxRobots)){
            throw std::runtime_error("ID error, check setRobotsInfo");
        }
        Position *pos_aux = new Position(true, detection.robots_yellow(x).x(), detection.robots_yellow(x).y());
        Angle *angle_aux = new Angle(true, detection.robots_yellow(x).orientation());

        robotsInfo->_yellowRobots[id].setRobotId(id);
        robotsInfo->_yellowRobots[id].update(100, *pos_aux, *angle_aux);
    }

    for(int x = 0; x < maxRobots && qt_yellowRobots != 0; x++){
        if(!visited[x] && !robotsInfo->_yellowRobots[x].checkLoss()){
            // se nao tiver sido visitado pelo frame anterior mas ainda estiver rodando loss, da predict
            robotsInfo->_yellowRobots[x].predict();
        }
    }

}

void setBallInfo(SSL_DetectionFrame &detection){
    if(detection.balls_size() > 0){ // potencial gigante de erro aqui, verificar depois
        Position *pos_aux = new Position(true, detection.balls(0).x(), detection.balls(0).y());
        Angle *angle_aux = new Angle(true, 0);
        robotsInfo->_ball.update(100, *pos_aux, *angle_aux);
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

    while(true){ 
        if(client.receive(packet)){
            if(packet.has_detection()){
               SSL_DetectionFrame detection = packet.detection();
                setRobotsInfo(detection);
                setBallInfo(detection);

                
            }
        }
    }
}





