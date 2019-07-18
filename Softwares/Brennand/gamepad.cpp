#include "gamepad.h"
#include "ui_gamepad.h"
#include <stdio.h>
#include <QThread>
#include <qmath.h>
#include <QDebug>
Gamepad::Gamepad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Gamepad)
{
    ui->setupUi(this);
    QPixmap square("C:/Users/rafin/Documents/Armorial-Suassuna/Softwares/Brennand/Square2.png");
    QPixmap ball("C:/Users/rafin/Documents/Armorial-Suassuna/Softwares/Brennand/Ball.png");
    QPixmap r1("C:/Users/rafin/Documents/Armorial-Suassuna/Softwares/Brennand/R1.png");
    QPixmap l1("C:/Users/rafin/Documents/Armorial-Suassuna/Softwares/Brennand/L1.png");
    QPixmap joy("C:/Users/rafin/Documents/Armorial-Suassuna/Softwares/Brennand/Ljoystick.jpg");
    ui->Square->setPixmap(square);
    ui->Ball->setPixmap(ball);
    ui->R1->setPixmap(r1);
    ui->L1->setPixmap(l1);
    ui->Joystick->setPixmap(joy);

    game = new QGamepad(0, parent);
    countA = 0;
}

void Gamepad::setDirection(){
    qreal x = game->axisLeftX();
    qreal y = -game->axisLeftY();
    qreal at;//o arco tangente
    countA++;
    QPixmap aux = arrowPic;
    if((x != 0.0 || y != 0.0) && countA > 10000){
        countA = 0;
        at = qAtan(y/x);
        rm.translate(0,0);
        if((x<0 && y>0) || (x<0 && y<0)){
            rm.rotate(180-at*57.30);
        }else{
            rm.rotate(-at*57.30);
        }
        aux = arrowPic.transformed(rm);
        ui->arrow->setPixmap(aux);
        rm.reset();
    }
}

Gamepad::~Gamepad()
{
    delete ui;
}
