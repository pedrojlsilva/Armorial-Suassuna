#include <bits/stdc++.h>
#include <math.h>
#include <iostream>
#include <netinet/in.h>
#include <vector>
#include <thread>
#include <algorithm>
#include "types/types.h"
// #include "types/position.h"
// #include "types/angle.h"
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

bool isInLimit(int x, int y){
    return ((x >= -6000 && x <= 6000) && (y >= -4500 && y <= 4500));
}

double getSpeedRotateToPoint(double robot_x, double robot_y, double point_x, double point_y, double angleOrigin2Robot){
    long double vectorRobot2BallX = (point_x - robot_x);
    long double vectorRobot2BallY = (point_y - robot_y);
    long double modVectorRobot2Ball = sqrt(pow(vectorRobot2BallX, 2) + pow(vectorRobot2BallY, 2));

    vectorRobot2BallX = vectorRobot2BallX / modVectorRobot2Ball;
    vectorRobot2BallY = vectorRobot2BallY / modVectorRobot2Ball;

    long double vectorOriginX = 1;

    long double angleOrigin2ball;
    long double angleRobot2Ball;

    angleOrigin2ball = acos((vectorRobot2BallX * vectorOriginX)); //angulo que a bola faz com o eixo x em relação ao robo

    if(vectorRobot2BallY < 0){ //terceiro e quadrante
        angleOrigin2ball = 2*M_PI - acos((vectorRobot2BallX * vectorOriginX)); //angulo que a bola faz com o eixo x em relação ao robo
    }else{ //primeiro e segundo quadrante
        angleOrigin2ball = acos((vectorRobot2BallX * vectorOriginX)); //angulo que a bola faz com o eixo x em relação ao robo
    }

    long double minValue = 1.5;
    long double maxValue = 2.0;

    long double speed;

    angleRobot2Ball = angleOrigin2Robot - angleOrigin2ball;

    if(fabs(angleRobot2Ball) >= M_PI / 60.0){
        if(abs(angleRobot2Ball) < minValue){
            if(angleRobot2Ball < 0.0) speed = minValue;
            else speed = -minValue;
        }else{
            if(angleRobot2Ball < 0.0){
                if(angleRobot2Ball < -M_PI) speed = -maxValue;
                else speed = maxValue;
            }else{
                if(angleRobot2Ball < M_PI) speed = -maxValue;
                else speed = maxValue;
            }
        }
    }else{
        speed = 0;
    }

    return speed;
}

Velocity getSpeed(double robot_x, double robot_y, double point_x, double point_y, double robotAngle){
    Velocity velocity(true, 0.0, 0.0);
    float Vx = (point_x - robot_x);
    float Vy = (point_y - robot_y);
    float theta = robotAngle;
    float moduloDistancia = sqrt(pow(Vx,2)+pow(Vy,2));
    float vxSaida = (Vx * cos(theta) + Vy * sin(theta))/1000.0;
    float vySaida = (Vx * cos(theta) - Vy * sin(theta))/1000.0;
    float sinalX = 1;
    float sinalY = 1;
    if(vxSaida < 0) sinalX = -1;
    if(vySaida < 0) sinalY = -1;
     if(moduloDistancia > 30.0 ){ 
        vxSaida = std::min(fabs(vxSaida)*0.7, 1.0);
        vySaida = std::min(fabs(vySaida)*0.7, 1.0);
    } else {
        vxSaida = 0;
        vySaida = 0;
    }
    velocity.setVelocityX(fabs(vxSaida) * sinalX);
    velocity.setVelocityY(fabs(vySaida) * sinalY);
    return velocity;
}


Position findBehindBall(float alvoX, float alvoY, float bolaX, float bolaY, float finalDistToPoint){
    Position position(true, 0.0, 0.0);
    float newX = fabs(finalDistToPoint);
    float deltaY=alvoY-bolaY;
    float deltaX=alvoX-bolaX;
    float proporcao = fabs(deltaY/deltaX);
    float newY = proporcao*newX;

    if(bolaX<alvoX) position.setPositionX(bolaX - newX);
    if(bolaX==alvoX) position.setPositionX(bolaX);
    if(bolaX>alvoX) position.setPositionX(bolaX + newX);


    if(bolaY<alvoY) position.setPositionY(bolaY-newY);
    if(bolaY==alvoY) position.setPositionY(bolaY);
    if(bolaY>alvoY) position.setPositionY(bolaY + newY);



    return position;
}

int main(){
    // opening ssl vision client
    RoboCupSSLClient client;
    client.open(true);
    SSL_WrapperPacket packet;

    std::pair<long double, long double> destino;

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

                if(detection.robots_blue_size() != 0){
                        int x = 0;
                    for(int x = 0; x < 8; x++){
                        grSim_robot.id = x;
                        grSim_robot.isYellow = false;
                        grSim_robot.angle = getSpeedRotateToPoint(robotsInfo->_blueRobots[x].getPosition().getX(), robotsInfo->_blueRobots[x].getPosition().getY(),
                                                                   robotsInfo->_blueRobots[1].getPosition().getX(), robotsInfo->_blueRobots[1].getPosition().getY(), robotsInfo->_blueRobots[x].getOrientation().value());
                        
                        // destino = getDestPoint(robotsInfo->_blueRobots[1].getPosition().getX(), robotsInfo->_blueRobots[1].getPosition().getY(),
                        // robotsInfo->_ball.getPosition().getX(), robotsInfo->_ball.getPosition().getY());
                        

                        // grSim_robot.vx = getSpeedX(robotsInfo->_blueRobots[x].getPosition().getX(), robotsInfo->_blueRobots[x].getPosition().getY(),
                        //                                           destino.first, destino.second, robotsInfo->_blueRobots[x].getOrientation().value());
                        // grSim_robot.vy = getSpeedY(robotsInfo->_blueRobots[x].getPosition().getX(), robotsInfo->_blueRobots[x].getPosition().getY(),
                        //                                           destino.first, destino.second, robotsInfo->_blueRobots[x].getOrientation().value());
                        
                        //grSim->sendPacket(grSim_robot);
                    }
                }
                samico->setFrame(robotsInfo);
                //grSim->sendPacket(grSim_robot);
            }
        }
    }
}





