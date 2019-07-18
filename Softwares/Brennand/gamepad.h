#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <QWidget>
#include <QThread>
#include <thread>
#include <QGamepad>
namespace Ui {
class Gamepad;
}

class Gamepad : public QWidget
{
    Q_OBJECT

public:
    explicit Gamepad(QWidget *parent = nullptr);
    ~Gamepad();
    QGamepad *game;
    void setDirection(void);
private:
    int countA;
    QPixmap arrowPic = QPixmap("C:/Users/rafin/Documents/Armorial-Suassuna/Softwares/Brennand/Arrow.png");
    QMatrix rm;
    Ui::Gamepad *ui;
};

#endif // GAMEPAD_H
