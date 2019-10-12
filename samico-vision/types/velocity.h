#ifndef VELOCITY_H
#define VELOCITY_H

#include "angle.h"
#include <cmath>

class Velocity {

    private:

        // Velocity components

        float _x;
        float _y;
        float _abs;
        Angle _arg;

        // Info flags

        bool _unknown;

        bool _valid;

    public:

        /*** Constructor

          ** Description: Creates an invalid velocity

          ** Recieves:    Nothing

          ***/

        Velocity();

        /*** Constructor

          ** Description: Creates a valid velocity

          ** Recieves:    [known] Indicates if the velocity is known.

                                    If not, 'x' and 'y' are ignored

                          [x]     The x component

                          [y]     The y component

          ***/

        Velocity(bool known, float x, float y);

        /*** Constructor

          ** Description: Creates a valid velocity

          ** Recieves:    [known] Indicates if the velocity is known.

                                    If not, 'x' and 'y' are ignored

                          [abs]   The velocity absolute value

                          [arg]   The velocity argument

          ***/

        Velocity(bool known, float abs, const Angle& arg);



    public:

        /*** '=' operator

          ** Description: Copies a velocity

          ** Receives:    [other] The velocity that will be copied

          ** Returns:     This velocity

          ***/

        Velocity& operator =(const Velocity& other);


    public:

        /*** 'setVelocity' function

          ** Description: Validates the velocity and sets its components

          ** Receives:    [x] The x component

                          [y] The y component

          ** Returns:     Nothing

          ***/

        void setVelocity(float x, float y);
        void setVelocityX(float x);
        void setVelocityY(float y);

        /*** 'setVelocity' function

          ** Description: Validates the velocity and sets its components

          ** Receives:    [abs] The velocity absolute value

                          [arg] The velocity argument

          ** Returns:     Nothing

          ***/

        void setVelocity(float abs, const Angle& arg);

        /*** Get functions

          ** Description: Gets the velocity component

          ** Receives:    Nothing

          ** Returns:     The wanted component

          ***/

        float x() const;

        float y() const;

        float abs() const;

        const Angle& arg() const;

    public:

        /*** 'isUnknown' function

          ** Description: Verifies if the velocity is unknown

          ** Receives:    Nothing

          ** Returns:     'false' if the velocity value is known, 'true' otherwise

          ***/

        bool isUnknown() const;

        /*** 'isValid' function

          ** Description: Verifies if the velocity is valid

          ** Receives:    Nothing

          ** Returns:     'false' if the velocity is invalid, 'true' otherwise

          ***/

        bool isValid() const;

        /*** 'setUnknown' function

          ** Description: Sets the velocity as unknown

          ** Receives:    Nothing

          ** Returns:     Nothing

          ***/

        void setUnknown();

        /*** 'setInvalid' function

          ** Description: Sets the velocity as invalid

          ** Receives:    Nothing

          ** Returns:     Nothing

          ***/

        void setInvalid();

    private:

        /*** 'setCartesianComponents' function

          ** Description: Sets the velocity cartesian components (x and y)

          ** Receives:    Nothing

          ** Returns:     Nothing

          ***/

        void setCartesianComponents();

        /*** 'setPolarComponents' function

          ** Description: Sets the velocity polar components (abs and arg)

          ** Receives:    Nothing

          ** Returns:     Nothing

          ***/

        void setPolarComponents();

};



#endif // VELOCITY_H
