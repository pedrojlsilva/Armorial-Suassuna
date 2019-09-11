#include "position.h"


Position::Position() {
    _x = 0;
    _y = 0;
    setInvalid();

}

Position::~Position(){

}

Position::Position(bool known, float newX, float newY) {

    (known == true) ? setPosition(newX, newY) : setUnknown();

}

// Set/get functions

float Position::getX() { return(_x); }

float Position::getY() { return(_y); }


void  Position::setPosition(float newX, float newY) {

    // Sets the coordinates
    _x = newX;
    _y = newY;

    // Sets as a valid known position
    _isValid   = true;
    _unknown = false;

}

// Info functions

bool Position::isUnknown() const { return _unknown; }

bool Position::isValid()   { return _isValid;   }

void Position::setInvalid() { _isValid = false; }

void Position::setValid() { _isValid = true; }

void Position::setUnknown() {

    _unknown = true;
    _isValid   = true;

}
