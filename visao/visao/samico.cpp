#include "samico.h"

using namespace sf;

Samico::Samico()
{
    // fundo do samico
    fundoSamico = new RectangleShape(Vector2f(1080.f, 720.f));
    fundoSamico->setFillColor(Color(0, 100, 0, 255));

    // fonte dos numeros nos jogadores

    if(!font.loadFromFile("utils/arial.ttf")){
        printf("Error loading font.\n");
        exit(-1);
    }

    /* circulo central */
    circuloCentral = new CircleShape(centralCirleRadius);
    circuloCentral->setFillColor(Color(0, 0, 0, 0));
    circuloCentral->setOutlineThickness(1.f);
    circuloCentral->setOutlineColor(Color(255, 255, 255, 255));
    circuloCentral->setPointCount(circlePrecision);
    circuloCentral->setPosition(540.f - centralCirleRadius, 360.f - centralCirleRadius);

    /* sprites e textura dos times */

    if(!blueRobots.loadFromFile("utils/bluerobot.png")){
        printf("Error loading image!\n");
        exit(-1);
    }

    blueTexture.loadFromImage(blueRobots);

    if(!yellowRobots.loadFromFile("utils/yellowrobot.png")){
        printf("Error loading image!\n");
        exit(-1);
    }


    yellowTexture.loadFromImage(yellowRobots);

    // texto e sprite do time azul
    Vector2<unsigned int> bSize = blueRobots.getSize();
    for(int x = 0; x < maxRobots; x++){
        blueSprite[x].setOrigin((bSize.x)/2.0, (bSize.y)/2.0);
        blueText[x].setFont(font);
        char teste[5];
        sprintf(teste, "%d", x);
        blueText[x].setString(teste);
        blueText[x].setCharacterSize(12);
        blueText[x].setFillColor(Color::Black);

        blueSprite[x].setTexture(blueTexture, true);
    }

    // texto e sprite do time amarelo

    Vector2<unsigned int> ySize = yellowRobots.getSize();
    for(int x = 0; x < maxRobots; x++){
        yellowSprite[x].setOrigin((ySize.x)/2.0, (ySize.y)/2.0);
        yellowText[x].setFont(font);
        char teste[5];
        sprintf(teste, "%d", x);
        yellowText[x].setString(teste);
        yellowText[x].setCharacterSize(12);
        yellowText[x].setFillColor(Color::Black);

        yellowSprite[x].setTexture(yellowTexture, true);
    }

    // bola
    ;
    ball->setFillColor(Color(255, 69, 0, 255));
    ball->setPointCount(circlePrecision);

    // configurações e criação da tela principal
    settings.antialiasingLevel = 8;

    window = new RenderWindow(VideoMode(1080, 720), "Armorial Samico", Style::Default, settings);
    window->setFramerateLimit(60);
}

void Samico::drawBall(){
    if(frame_received->_ball.getBallPosition().isValid()){
        double ballx = abs(((frame_received->_ball.getBallPosition().getX())+6000.0)/(6000.0/540.0));
        double bally = abs(((frame_received->_ball.getBallPosition().getY())-4700)/(4700.0/360.0));
        ball->setPosition(ballx - ballRadius, bally - ballRadius);
        window->draw(*ball);
    }
}

void Samico::drawRobots(){
    for(int x = 0; x < frame_received->_qt_blue; x++){
        double t = frame_received->_blueRobots[x].getOrientation().value(), newx, newy;
        char robotNumber[2];
        sprintf(robotNumber, "%d", frame_received->_blueRobots[x].robotId());
        blueText[x].setString(robotNumber);

        newx = abs(frame_received->_blueRobots[x].getRobotPosition().getX() + 6000)/(6000.0/540.0);
        newy = abs(frame_received->_blueRobots[x].getRobotPosition().getY() - 4700)/(4700.0/360.0);

        Vertex robotTriangle[] =
        {
            Vertex(sf::Vector2f(newx, newy - (2 * robotRadius))),
            Vertex(sf::Vector2f(newx - (sqrt(3)*robotRadius), newy + robotRadius)),
            Vertex(sf::Vector2f(newx, newy - (2 * robotRadius))),
            Vertex(sf::Vector2f(newx + (sqrt(3)*robotRadius), newy + robotRadius)),
            Vertex(sf::Vector2f(newx - (sqrt(3)*robotRadius), newy + robotRadius)),
            Vertex(sf::Vector2f(newx + (sqrt(3)*robotRadius), newy + robotRadius))
        };
        for(int x = 0; x < 6; x++) robotTriangle[x].color = Color::White;

        blueSprite[x].setRotation((-t) * 57.2958); // conversao de rad para degree
        blueSprite[x].setPosition(newx, newy);

        blueText[x].setPosition(newx - 3, newy - 7);

        if(!frame_received->_blueRobots[x].getRobotPosition().isValid()){
            window->draw(blueSprite[x]);
            window->draw(blueText[x]);
            window->draw(robotTriangle, 6, Lines);
        }
    }

    for(int x = 0; x < frame_received->_qt_yellow; x++){
        double t = frame_received->_yellowRobots[x].getOrientation().value(), newx, newy;
        char robotNumber[2];
        sprintf(robotNumber, "%d", frame_received->_yellowRobots[x].robotId());
        yellowText[x].setString(robotNumber);

        newx = abs(frame_received->_yellowRobots[x].getRobotPosition().getX() + 6000)/(6000.0/540.0);
        newy = abs(frame_received->_yellowRobots[x].getRobotPosition().getY() - 4700)/(4700.0/360.0);

        Vertex robotTriangle[] =
        {
            Vertex(sf::Vector2f(newx, newy - (2 * robotRadius))),
            Vertex(sf::Vector2f(newx - (sqrt(3)*robotRadius), newy + robotRadius)),
            Vertex(sf::Vector2f(newx, newy - (2 * robotRadius))),
            Vertex(sf::Vector2f(newx + (sqrt(3)*robotRadius), newy + robotRadius)),
            Vertex(sf::Vector2f(newx - (sqrt(3)*robotRadius), newy + robotRadius)),
            Vertex(sf::Vector2f(newx + (sqrt(3)*robotRadius), newy + robotRadius))
        };
        for(int x = 0; x < 6; x++) robotTriangle[x].color = Color::White;

        yellowSprite[x].setRotation((-t) * 57.2958); // conversao de rad para degree
        yellowSprite[x].setPosition(newx, newy);

        yellowText[x].setPosition(newx - 3, newy - 7);

        if(!frame_received->_yellowRobots[x].getRobotPosition().isValid()){
            window->draw(yellowSprite[x]);
            window->draw(yellowText[x]);
            window->draw(robotTriangle, 6, Lines);
        }
    }
}



void Samico::drawWindow(){
    while(window->isOpen()){
        Event event;
        while(window->pollEvent(event)){
            if(event.type == Event::Closed){
                window->close();
            }
        }

        window->clear();

        // desenho das bordas do campo e do fundo
        window->draw(*fundoSamico);
        window->draw(linhasExternas, 10, Lines);
        window->draw(linhaMeio, 2, Lines);
        window->draw(golEsquerdo, 6, Lines);
        window->draw(golDireito, 6, Lines);
        window->draw(*circuloCentral);

        drawBall();
        drawRobots();
        window->display();
    }

}


