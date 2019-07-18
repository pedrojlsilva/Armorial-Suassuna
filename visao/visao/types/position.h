#ifndef POSITION_H
#define POSITION_H


class Position
{
public:
    Position();
    Position(bool known, float newX, float newY);
    ~Position();
    void setPosition(float newX, float newY);
    float getX();
    float getY();
    float getAngle();
    bool isUnknown() const;
    bool isValid();
    void setUnknown();
    void setInvalid();


private: //parameters
    float _x;
    float _y;
    bool _unknown;
    bool _isValid;
};

#endif // POSITION_H
