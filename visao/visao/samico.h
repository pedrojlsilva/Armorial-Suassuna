#ifndef SAMICO_H
#define SAMICO_H

#include <bits/stdc++.h>
#include <stdlib.h>
#include <thread>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <SFML/Graphics.hpp>
#include "Ball/ball.h"
#include "Robot/robot.h"
#include "types/frame.h"

using namespace std;
using namespace sf;

#define maxRobots 8
#define circlePrecision 100
#define ballRadius 6.f
#define centralCirleRadius 50.f
#define robotRadius 9.f


class Samico
{
public:
    Samico();
    void setPacket();
    void drawWindow();
    void setFrame(Frame newFrame){
        *frame_received = newFrame;
    }

private:
    void drawBall();
    void drawRobots();


private:
    Frame *frame_received = new Frame();
    RectangleShape *fundoSamico;
    Font font;
    CircleShape *circuloCentral;
    Image blueRobots;
    Image yellowRobots;
    Texture blueTexture;
    Texture yellowTexture;
    Text blueText[maxRobots];
    Sprite blueSprite[maxRobots];


    Text yellowText[maxRobots];
    Sprite yellowSprite[maxRobots];
    ContextSettings settings;


    RenderWindow *window;

    Vertex linhasExternas[10] =
    {
        Vertex(Vector2f(10.f, 10.f)),
        Vertex(Vector2f(1070.f, 10.f)),
        Vertex(Vector2f(1070.f, 10.f)),
        Vertex(Vector2f(1070.f, 710.f)),
        Vertex(Vector2f(1070.f, 710.f)),
        Vertex(Vector2f(10.f, 710.f)),
        Vertex(Vector2f(10.f, 710.f)),
        Vertex(Vector2f(10.f, 10.f)),
        Vertex(Vector2f(10.f, 360.f)),
        Vertex(Vector2f(1070.f, 360.f))
    };

    Vertex linhaMeio[2] =
    {
        Vertex(Vector2f(540.f, 10.f)),
        Vertex(Vector2f(540.f, 710.f))
    };

    Vertex golEsquerdo[6] =
    {
        Vertex(Vector2f(10.f, 280.f)),
        Vertex(Vector2f(110.f, 280.f)),
        Vertex(Vector2f(10.f, 430.f)),
        Vertex(Vector2f(110.f, 430.f)),
        Vertex(Vector2f(110.f, 280.f)),
        Vertex(Vector2f(110.f, 430.f))
    };

    Vertex golDireito[6] =
    {
        Vertex(Vector2f(1070.f, 280.f)),
        Vertex(Vector2f(960.f, 280.f)),
        Vertex(Vector2f(1070.f, 430.f)),
        Vertex(Vector2f(960.f, 430.f)),
        Vertex(Vector2f(960.f, 280.f)),
        Vertex(Vector2f(960.f, 430.f))
    };
};

#endif // SAMICO_H
