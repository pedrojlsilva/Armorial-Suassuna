#ifndef SAMICO_H
#define SAMICO_H

#include <bits/stdc++.h>
//#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <SFML/Graphics.hpp>
#include "Ball/ball.h"
#include "Robot/robot.h"
#include "types/frame.h"

using namespace std;

#define maxRobots 8
#define circlePrecision 100
#define ballRadius 6.f
#define centralCirleRadius 50.f
#define robotRadius 9.f




class Samico
{
public:
    Samico();
    void drawWindow();
    void setFrame(Frame *newFrame);


private:
    void drawBall();
    void drawRobots();


private:
    Frame *frame_received=new Frame(8,100);
    sf::CircleShape *ball = new sf::CircleShape(ballRadius);
    sf::RectangleShape *fundoSamico;
    sf::Font font;
    sf::CircleShape *circuloCentral;
    sf::Image blueRobots;
    sf::Image yellowRobots;
    sf::Texture blueTexture;
    sf::Texture yellowTexture;
    sf::Text blueText[maxRobots];
    sf::Sprite blueSprite[maxRobots];


    sf::Text yellowText[maxRobots];
    sf::Sprite yellowSprite[maxRobots];
    sf::ContextSettings settings;


    sf::RenderWindow *window;

    sf::Vertex linhasExternas[10] =
    {
        sf::Vertex(sf::Vector2f(10.f, 10.f)),
        sf::Vertex(sf::Vector2f(1070.f, 10.f)),
        sf::Vertex(sf::Vector2f(1070.f, 10.f)),
        sf::Vertex(sf::Vector2f(1070.f, 710.f)),
        sf::Vertex(sf::Vector2f(1070.f, 710.f)),
        sf::Vertex(sf::Vector2f(10.f, 710.f)),
        sf::Vertex(sf::Vector2f(10.f, 710.f)),
        sf::Vertex(sf::Vector2f(10.f, 10.f)),
        sf::Vertex(sf::Vector2f(10.f, 360.f)),
        sf::Vertex(sf::Vector2f(1070.f, 360.f))
    };

    sf::Vertex linhaMeio[2] =
    {
        sf::Vertex(sf::Vector2f(540.f, 10.f)),
        sf::Vertex(sf::Vector2f(540.f, 710.f))
    };

    sf::Vertex golEsquerdo[6] =
    {
        sf::Vertex(sf::Vector2f(10.f, 280.f)),
        sf::Vertex(sf::Vector2f(110.f, 280.f)),
        sf::Vertex(sf::Vector2f(10.f, 430.f)),
        sf::Vertex(sf::Vector2f(110.f, 430.f)),
        sf::Vertex(sf::Vector2f(110.f, 280.f)),
        sf::Vertex(sf::Vector2f(110.f, 430.f))
    };

    sf::Vertex golDireito[6] =
    {
        sf::Vertex(sf::Vector2f(1070.f, 280.f)),
        sf::Vertex(sf::Vector2f(960.f, 280.f)),
        sf::Vertex(sf::Vector2f(1070.f, 430.f)),
        sf::Vertex(sf::Vector2f(960.f, 430.f)),
        sf::Vertex(sf::Vector2f(960.f, 280.f)),
        sf::Vertex(sf::Vector2f(960.f, 430.f))
    };
};
#endif // SAMICO_H
