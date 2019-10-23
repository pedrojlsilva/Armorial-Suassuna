#ifndef POSITION_H
#define POSITION_H


class Position
{
public:
    Position();
    Position(bool known, float newX, float newY);    
    Position(bool known, float newX, float newY, float newZ);
    ~Position();
    void setPosition(float newX, float newY, float newZ);
    float getX();
    float getY();
    float getAngle();
    bool isUnknown() const;
    bool isValid();
    void setUnknown();
    void setInvalid();
    void setValid();
    void  setPositionX(float newX);
    void  setPositionY(float newY);
    void  setPositionZ(float newZ);


private: //parameters
    float _x;
    float _y;
    float _z;
    bool _unknown;
    bool _isValid;
};

#endif // POSITION_H
