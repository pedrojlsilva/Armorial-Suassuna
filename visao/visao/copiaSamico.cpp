#include <bits/stdc++.h>
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <thread>
#include <unistd.h> 
#include <stdio.h>   
#include <string.h> 
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

#define maxRobots 8
#define circlePrecision 100
#define ballRadius 6.f
#define centralCirleRadius 50.f
#define robotRadius 9.f

#define PORT 8881
#define ssl_port 10006
#define IP "127.0.0.1"

// g++ -c samico.cpp
// g++ samico.o -o samico -pthread -lsfml-graphics -lsfml-window -lsfml-system -Ofast

/* pacotes enviados pelo cliente */

// armazenar informacoes sobre os jogadores (id, x, y, angulo)
typedef struct{
    int id;
    double x, y, angle;
    bool isLoss;
} robot_coords;

// pacote como um todo
typedef struct{
    char qt_blue;
    char qt_yellow;
    char camera_id;
    unsigned char battery[2][maxRobots]; // 1byte (0 a 100)
    robot_coords robots_blue[maxRobots];
    robot_coords robots_yellow[maxRobots];
    pair<double, double> ball;
    bool isBallLoss;
} pacote;

Vertex linhasExternas[] =
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

Vertex linhaMeio[] =
{
    Vertex(Vector2f(540.f, 10.f)),
    Vertex(Vector2f(540.f, 710.f))
};

Vertex golEsquerdo[] =
{
    Vertex(Vector2f(10.f, 280.f)),
    Vertex(Vector2f(110.f, 280.f)),
    Vertex(Vector2f(10.f, 430.f)),
    Vertex(Vector2f(110.f, 430.f)),
    Vertex(Vector2f(110.f, 280.f)),
    Vertex(Vector2f(110.f, 430.f))
};

Vertex golDireito[] =
{
    Vertex(Vector2f(1070.f, 280.f)),
    Vertex(Vector2f(960.f, 280.f)),
    Vertex(Vector2f(1070.f, 430.f)),
    Vertex(Vector2f(960.f, 430.f)),
    Vertex(Vector2f(960.f, 280.f)),
    Vertex(Vector2f(960.f, 430.f))
};

bool draw = false;

void ball_players(pacote *info_packet){

	// criacao do socket
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
        cout << "Erro durante a criacao do socket" << endl; 
        exit(-1);
    }
    // zerando o address e setando logo em seguida
	memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    
	// verificando endereco do socket
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){ 
        cout << "Endereco invalido" << endl;
        exit(-1);
    }
	
	// esperando recepção das mensagens
    while(true){
    	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) >= 0){ 
    		valread = read(sock, info_packet, sizeof(pacote)); 
    		while(valread){
        		valread = read(sock, info_packet, sizeof(pacote));
                draw = true;
        	}
    	}
    }
}

int main(){
    pacote info_packet;
    thread first(ball_players, &info_packet);
    first.detach();
    // fundo do samico
    RectangleShape fundoSamico(Vector2f(1080.f, 720.f));
    fundoSamico.setFillColor(Color(0, 100, 0, 255));
    
    // fonte dos numeros nos jogadores
    Font font;
    if(!font.loadFromFile("utils/arial.ttf")){
        printf("Error loading font.\n");
        exit(-1);
    }
    
    /* circulo central */
    CircleShape circuloCentral(centralCirleRadius);
    circuloCentral.setFillColor(Color(0, 0, 0, 0));
    circuloCentral.setOutlineThickness(1.f);
    circuloCentral.setOutlineColor(Color(255, 255, 255, 255));
    circuloCentral.setPointCount(circlePrecision);
    circuloCentral.setPosition(540.f - centralCirleRadius, 360.f - centralCirleRadius);

    /* sprites e textura dos times */
    Image blueRobots;
    if(!blueRobots.loadFromFile("utils/bluerobot.png")){
        printf("Error loading image!\n");
        exit(-1);
    }
    Texture blueTexture;
    blueTexture.loadFromImage(blueRobots);

    Image yellowRobots;
    if(!yellowRobots.loadFromFile("utils/yellowrobot.png")){
        printf("Error loading image!\n");
        exit(-1);
    }
    
    Texture yellowTexture;
    yellowTexture.loadFromImage(yellowRobots);

    // texto e sprite do time azul
    Text blueText[maxRobots];
    Sprite blueSprite[maxRobots];
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
    Text yellowText[maxRobots];
    Sprite yellowSprite[maxRobots];
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
    CircleShape ball(ballRadius);
    ball.setFillColor(Color(255, 69, 0, 255));
    ball.setPointCount(circlePrecision);

    // configurações e criação da tela principal
    ContextSettings settings;
    settings.antialiasingLevel = 8;

    RenderWindow window(VideoMode(1080, 720), "Armorial Samico", Style::Default, settings);
    window.setFramerateLimit(60);

    while(window.isOpen()){
        Event event;
        while(window.pollEvent(event)){
            if(event.type == Event::Closed){
                window.close();
            }
        }

        window.clear();

        // desenho das bordas do campo e do fundo
        window.draw(fundoSamico);
        window.draw(linhasExternas, 10, Lines);
        window.draw(linhaMeio, 2, Lines);
        window.draw(golEsquerdo, 6, Lines);
        window.draw(golDireito, 6, Lines);
        window.draw(circuloCentral);

        // desenho da bola e dos robos
        for(int x = 0; x < info_packet.qt_blue; x++){
            double t = info_packet.robots_blue[x].angle, newx, newy;
            char robotNumber[2];
            sprintf(robotNumber, "%d", info_packet.robots_blue[x].id);
            blueText[x].setString(robotNumber);
    
            newx = abs(info_packet.robots_blue[x].x + 6000)/(6000.0/540.0);
            newy = abs(info_packet.robots_blue[x].y - 4700)/(4700.0/360.0);

            Vertex robotTriangle[] =
            {
                Vertex(Vector2f(newx, newy - (2 * robotRadius))),
                Vertex(Vector2f(newx - (sqrt(3)*robotRadius), newy + robotRadius)),
                Vertex(Vector2f(newx, newy - (2 * robotRadius))),
                Vertex(Vector2f(newx + (sqrt(3)*robotRadius), newy + robotRadius)),
                Vertex(Vector2f(newx - (sqrt(3)*robotRadius), newy + robotRadius)),
                Vertex(Vector2f(newx + (sqrt(3)*robotRadius), newy + robotRadius))
            };
            for(int x = 0; x < 6; x++) robotTriangle[x].color = Color::White;

            blueSprite[x].setRotation((-t) * 57.2958); // conversao de rad para degree
            blueSprite[x].setPosition(newx, newy);
            
            blueText[x].setPosition(newx - 3, newy - 7);

            if(!info_packet.robots_blue[x].isLoss){
                window.draw(blueSprite[x]);
                window.draw(blueText[x]);
                window.draw(robotTriangle, 6, Lines);
            }
        }

        for(int x = 0; x < info_packet.qt_yellow; x++){
            double t = info_packet.robots_yellow[x].angle, newx, newy;
            char robotNumber[2];
            sprintf(robotNumber, "%d", info_packet.robots_yellow[x].id);
            yellowText[x].setString(robotNumber);
    
            newx = abs(info_packet.robots_yellow[x].x + 6000)/(6000.0/540.0);
            newy = abs(info_packet.robots_yellow[x].y - 4700)/(4700.0/360.0);

            Vertex robotTriangle[] =
            {
                Vertex(Vector2f(newx, newy - (2 * robotRadius))),
                Vertex(Vector2f(newx - (sqrt(3)*robotRadius), newy + robotRadius)),
                Vertex(Vector2f(newx, newy - (2 * robotRadius))),
                Vertex(Vector2f(newx + (sqrt(3)*robotRadius), newy + robotRadius)),
                Vertex(Vector2f(newx - (sqrt(3)*robotRadius), newy + robotRadius)),
                Vertex(Vector2f(newx + (sqrt(3)*robotRadius), newy + robotRadius))
            };
            for(int x = 0; x < 6; x++) robotTriangle[x].color = Color::White;

            yellowSprite[x].setRotation((-t) * 57.2958); // conversao de rad para degree
            yellowSprite[x].setPosition(newx, newy);
            
            yellowText[x].setPosition(newx - 3, newy - 7);

            if(!info_packet.robots_yellow[x].isLoss){
                window.draw(yellowSprite[x]);
                window.draw(yellowText[x]);
                window.draw(robotTriangle, 6, Lines);
            }
        }

        if(!info_packet.isBallLoss){
            double ballx = abs(((info_packet.ball.first)+6000)/(6000.0/540.0));
            double bally = abs(((info_packet.ball.second)-4700)/(4700.0/360.0));
            ball.setPosition(ballx - ballRadius, bally - ballRadius);

            window.draw(ball);
        }

        window.display();
    }
    
    return 0;
}