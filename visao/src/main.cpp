#include <bits/stdc++.h>
#include "robocup_ssl_client.h"
#include "timer.h"

#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"

#include "Robot.hpp"
#include "Ball.hpp"
#include "Filters.hpp"

#define qt_robosTime 8
#define tempoFiltro 500
using namespace std;
using namespace chrono;

void setRobotsInfo(SSL_DetectionFrame &detection, vector<Robot> &robosAzuis, vector<Robot> &robosAmarelos){
    uint8_t qt_robosAzuis = detection.robots_blue_size();
    uint8_t qt_robosAmarelos = detection.robots_yellow_size();
    
    static Filters *filteredRobsAz;
    filteredRobsAz = (Filters *) malloc(qt_robosAzuis * sizeof(Filters));
    static Filters *filteredRobsAm;
    filteredRobsAm = (Filters *) malloc(qt_robosAmarelos * sizeof(Filters));
    
    for(uint8_t x = 0; x < qt_robosAzuis; x++){
        robosAzuis[x].setId(detection.robots_blue(x).robot_id());
        robosAzuis[x].setHeight(detection.robots_blue(x).height());
        robosAzuis[x].setCoordinates(detection.robots_blue(x).x(), detection.robots_blue(x).y());
        robosAzuis[x].setAngle(detection.robots_blue(x).orientation());
        filteredRobsAz[x].setFilterTime(tempoFiltro);
        filteredRobsAz[x].noiseFilter(robosAzuis[x]);
        filteredRobsAz[x].lossFilter(robosAzuis[x]);
        robosAzuis[x].printRobotInfo();
    }
    for(uint8_t x = 0; x < qt_robosAmarelos; x++){
        robosAmarelos[x].setId(detection.robots_yellow(x).robot_id());
        robosAmarelos[x].setHeight(detection.robots_yellow(x).height());
        robosAmarelos[x].setCoordinates(detection.robots_yellow(x).x(), detection.robots_yellow(x).y());
        robosAmarelos[x].setAngle(detection.robots_yellow(x).orientation());
        filteredRobsAm[x].setFilterTime(tempoFiltro);
        filteredRobsAm[x].noiseFilter(robosAmarelos[x]);
        filteredRobsAm[x].lossFilter(robosAmarelos[x]);
        robosAmarelos[x].printRobotInfo();
    }
}

void setBallInfo(SSL_DetectionFrame &detection, Ball &ball){
    if(detection.balls_size() > 0){
        static Filters filteredBall;
        ball.setCoordinates(detection.balls(0).x(), detection.balls(0).y());
        filteredBall.setFilterTime(tempoFiltro);
        filteredBall.noiseFilter(ball);
        filteredBall.lossFilter(ball);
        ball.printBallInfo();
    }
}

int main(){
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