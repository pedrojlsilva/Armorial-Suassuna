#include <bits/stdc++.h>
#include <math.h>
#include <iostream>
#include <netinet/in.h>
#include <vector>
#include <thread>
#include "types/position.h"
#include "types/angle.h"
#include "Ball/ball.h"
#include "Robot/robot.h"
#include "types/frame.h"
#include "grsSimulator/grsSimulator.h"
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
    int qt_blueRobots = detection.robots_blue_size();
    int qt_yellowRobots = detection.robots_yellow_size();
    quint32 camera_id = detection.camera_id();
    if(qt_blueRobots != 0) robotsInfo->_qt_blue=qt_blueRobots;
    if(qt_yellowRobots != 0) robotsInfo->_qt_yellow=qt_yellowRobots;
    robotsInfo->_camera_id=camera_id;

    // loop para checar se o robo esta perdido ou ruidoso (invalidando a posicao)
    for(int x = 0; x < qt_robosTime; x++){
        if(robotsInfo->_blueRobots[x].checkLoss() || !robotsInfo->_blueRobots[x].checkNoise()) robotsInfo->_blueRobots[x]._position.setInvalid();
    }

    for(int x = 0; x < qt_blueRobots; x++){
        quint32 id = detection.robots_blue(x).robot_id();

        if(!(id < maxRobots)){
            throw std::runtime_error("ID error, check setRobotsInfo");
        }
        Position *pos_aux = new Position(true, detection.robots_blue(x).x(), detection.robots_blue(x).y());
        Angle *angle_aux = new Angle(true, detection.robots_blue(x).orientation());

        if(robotsInfo->_blueRobots[id]._position.isValid() || (robotsInfo->_blueRobots[id].checkNoise() && !robotsInfo->_blueRobots[id].checkLoss())){// se a posição for valida, ele passa para frame
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
        if(robotsInfo->_yellowRobots[id]._position.isValid() || (robotsInfo->_yellowRobots[id].checkNoise() && robotsInfo->_yellowRobots[id].checkLoss())){// se a posição for valida, ele passa para frame
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

double getSpeedRotateToPoint(int index, double robot_x, double robot_y, double point_x, double point_y, double angleOrigin2Robot){
    double vectorRobot2BallX = (point_x - robot_x);
    double vectorRobot2BallY = (point_y - robot_y);
    double modVectorRobot2Ball = sqrt(pow(vectorRobot2BallX, 2) + pow(vectorRobot2BallY, 2));

    vectorRobot2BallX = vectorRobot2BallX / modVectorRobot2Ball;
    vectorRobot2BallY = vectorRobot2BallY / modVectorRobot2Ball;

    double vectorOriginX = 1;
    double vectorOriginY = 0;

    double angleOrigin2ball;
    double angleRobot2Ball;

    angleOrigin2ball = acos((vectorRobot2BallX * vectorOriginX)); //angulo que a bola faz com o eixo x em relação ao robo

    if(vectorRobot2BallY < 0){ //terceiro e quadrante
        angleOrigin2ball = 2*M_PI - acos((vectorRobot2BallX * vectorOriginX)); //angulo que a bola faz com o eixo x em relação ao robo
    }else{ //primeiro e segundo quadrante
        angleOrigin2ball = acos((vectorRobot2BallX * vectorOriginX)); //angulo que a bola faz com o eixo x em relação ao robo
    }

    double minValue = 1.5;
    double maxValue = 2.0;

    double speed;

    angleRobot2Ball = angleOrigin2Robot - angleOrigin2ball;

    if(index == 6){
        cout << angleOrigin2Robot << " <- origem ate robo" << endl;
        cout << angleRobot2Ball << " <- origem ate bola" << endl;
    }

    if(fabs(angleRobot2Ball) >= M_PI / 60.0){
        if(abs(angleRobot2Ball) < minValue){
            if(angleRobot2Ball < 0.0) speed = minValue;
            else speed = -minValue;
        }else{
            if(angleRobot2Ball < 0.0) speed = -maxValue;
            else speed = maxValue;
        }
    }else{
        speed = 0;
    }

    return speed;
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

                if(detection.robots_blue_size() != 0){
                    for(int x = 0; x < 8; x++){
                        grSim_robot.id = x;
                        grSim_robot.isYellow = false;
                        grSim_robot.angle = getSpeedRotateToPoint(x, robotsInfo->_blueRobots[x].getPosition().getX(), robotsInfo->_blueRobots[x].getPosition().getY(),
                                                                  robotsInfo->_ball.getPosition().getX(), robotsInfo->_ball.getPosition().getY(), robotsInfo->_blueRobots[x].getOrientation().value());

                        grSim->sendPacket(grSim_robot);
                    }
                }
                samico->setFrame(robotsInfo);
                //grSim->sendPacket(grSim_robot);
            }
        }
    }
}





