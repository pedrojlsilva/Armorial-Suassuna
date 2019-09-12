#include "samico.h"
#include <thread>

Samico::Samico()
{
    // fundo do samico
    fundoSamico = new sf::RectangleShape(sf::Vector2f(7400.f, 10400.f));
    fundoSamico->setFillColor(sf::Color(0, 100, 0, 255));

    // fonte dos numeros nos jogadores

    if(!font.loadFromFile("utils/arial.ttf")){
        printf("Error loading font.\n");
        exit(-1);
    }

    /* circulo central */
    circuloCentral = new sf::CircleShape(centralCirleRadius);
    circuloCentral->setFillColor(sf::Color(0, 0, 0, 0));
    circuloCentral->setOutlineThickness(10.f);
    circuloCentral->setOutlineColor(sf::Color(255, 255, 255, 255));
    circuloCentral->setPointCount(circlePrecision);
    circuloCentral->setPosition((max_x/2.0) - centralCirleRadius, (max_y/2.0) - centralCirleRadius);

    // texto e sprite do time azul
    for(int x = 0; x < maxRobots; x++){
        blueText[x].setFont(font);
        char teste[5];
        sprintf(teste, "%d", x);
        blueText[x].setString(teste);
        blueText[x].setCharacterSize(160);
        blueText[x].setFillColor(sf::Color::Black);

        blueRobots_shape[x].setRadius(robotRadius);
        blueRobots_shape[x].setFillColor(sf::Color::Blue);
        blueRobots_shape[x].setOutlineColor(sf::Color::Black);
        blueRobots_shape[x].setOutlineThickness(12.f);
        blueRobots_shape[x].setPointCount(100.f);
    }

    // texto e sprite do time amarelo
    for(int x = 0; x < maxRobots; x++){
        yellowText[x].setFont(font);
        char teste[5];
        sprintf(teste, "%d", x);
        yellowText[x].setString(teste);
        yellowText[x].setCharacterSize(160);
        yellowText[x].setFillColor(sf::Color::Black);

        yellowRobots_shape[x].setRadius(robotRadius);
        yellowRobots_shape[x].setFillColor(sf::Color::Yellow);
        yellowRobots_shape[x].setOutlineColor(sf::Color::Black);
        yellowRobots_shape[x].setOutlineThickness(12.f);
        yellowRobots_shape[x].setPointCount(100.f);
    }

    // bola
    ;
    ball->setFillColor(sf::Color(255, 69, 0, 255));
    ball->setPointCount(circlePrecision);

    // configurações e criação da tela principal
    settings.antialiasingLevel = 8;

    window = new sf::RenderWindow(sf::VideoMode(592, 832), "Armorial Samico", sf::Style::Default, settings);
    window->setFramerateLimit(60);

    zoomViewAt(sf::Vector2i(0, 0), window, 12.5);

}

void Samico::drawBall(){
    if(frame_received->_ball.getBallPosition().isValid()){
        double ballx = abs(((frame_received->_ball.getPosition().getX())+6000.0)/(6000.0/5200.0));
        double bally = abs(((frame_received->_ball.getPosition().getY())-4700)/(4700.0/3700.0));
        ball->setPosition(bally - ballRadius, ballx - ballRadius);
        ball->setOutlineColor(sf::Color::Black);
        ball->setOutlineThickness(12.f);
        window->draw(*ball);
    }
}

void Samico::drawRobots(){

    for(int x = 0; x < frame_received->_blueRobots.size(); x++){
        double t = frame_received->_blueRobots[x].getOrientation().value(), newx, newy;
        char robotNumber[2];
        sprintf(robotNumber, "%d", frame_received->_blueRobots[x].robotId());
        blueText[x].setString(robotNumber);

        newx = abs(frame_received->_blueRobots[x].getPosition().getX() + 6000)/(6000.0/5200.0);
        newy = abs(frame_received->_blueRobots[x].getPosition().getY() - 4700)/(4700.0/3700.0);

        blueRobots_shape[x].setPosition(newy - robotRadius, newx - robotRadius);
        blueText[x].setPosition(newy - 40, newx - 85);

        if(frame_received->_blueRobots[x].getPosition().isValid()){
            window->draw(blueRobots_shape[x]);
            window->draw(blueText[x]);
        }
    }

    for(int x = 0; x < frame_received->_yellowRobots.size(); x++){
        double t = frame_received->_yellowRobots[x].getOrientation().value(), newx, newy;
        char robotNumber[2];
        sprintf(robotNumber, "%d", frame_received->_yellowRobots[x].robotId());
        yellowText[x].setString(robotNumber);

        newx = abs(frame_received->_yellowRobots[x].getPosition().getX() + 6000)/(6000.0/5200.0);
        newy = abs(frame_received->_yellowRobots[x].getPosition().getY() - 4700)/(4700.0/3700.0);

        yellowRobots_shape[x].setPosition(newy - robotRadius, newx - robotRadius);
        yellowText[x].setPosition(newy - 40, newx - 85);

        if(frame_received->_yellowRobots[x].getPosition().isValid()){
            window->draw(yellowRobots_shape[x]);
            window->draw(yellowText[x]);
        }
    }
}

sf::RenderWindow *Samico::getWindow(){
    return this->window;
}

void Samico::zoomViewAt(sf::Vector2i pixel, sf::RenderWindow *window, double zoom){
    const sf::Vector2f beforeCoord{ window->mapPixelToCoords(pixel) };
    sf::View view{ window->getView() };
    view.zoom(zoom);
    window->setView(view);
    const sf::Vector2f afterCoord{ window->mapPixelToCoords(pixel) };
    const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
    view.move(offsetCoords);
    window->setView(view);
}

void Samico::drawWindow(){
    bool moving = false;
    sf::Vector2f oldPos;
    while(window->isOpen()){
        sf::Event event;
        while(window->pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window->close();
            }
            if (event.type == sf::Event::MouseWheelScrolled){
                if (event.mouseWheelScroll.delta > 0)
                    zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, (1.f / zoomAmount));
                else if (event.mouseWheelScroll.delta < 0)
                    zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, zoomAmount);
            }
            if(event.type == sf::Event::MouseButtonPressed){
                if(event.mouseButton.button == 0){
                    moving = true;
                    oldPos = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
            }
            if(event.type == sf::Event::MouseButtonReleased){
                if(event.mouseButton.button == 0){
                    moving = false;
                }
            }
            if(event.type == sf::Event::MouseMoved){
                if(moving){
                    const sf::Vector2f newPos = window->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                    const sf::Vector2f deltaPos = oldPos - newPos;
                    sf::View view = window->getView();

                    view.setCenter(view.getCenter() + deltaPos);
                    window->setView(view);

                    oldPos = window->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                }
            }
        }

        window->clear();

        // desenho das bordas do campo e do fundo
        window->draw(*fundoSamico);
        window->draw(linhasExternas, 10, sf::Lines);
        window->draw(golEsquerdo, 6, sf::Lines);
        window->draw(golDireito, 6, sf::Lines);
        window->draw(*circuloCentral);

        drawBall();
        drawRobots();
        window->display();
    }

}

void Samico::setFrame(Frame *newFrame){
       cout<<frame_received->_qt_robosTime<<std::endl;
       frame_received = newFrame;


}


