#ifndef ANGLE_H
#define ANGLE_H


class Angle {

    public:

        // Pi constants

        static constexpr double pi    = 3.14159265358979323846;

        static constexpr double twoPi = 6.28318530717958647692;

    private:

        // Conversion constants

        static constexpr double toRad =  0.01745329251994329577;

        static constexpr double toDeg = 57.29577951308232087680;

    private:

        // Angle value

        float _value;

        // Info flags

        bool _unknown;

        bool _valid;

    public:

        /*** 'toRadians' function

          ** Description: Converts an angle from degrees to radians

          ** Receives:    [angle] The angle that will be converted

          ** Returns:     The angle value in radians

          ***/

        static float toRadians(float angle);

        /*** 'toDegrees' function
          ** Description: Converts an angle from radians to degrees
          ** Receives:    [angle] The angle that will be converted
          ** Returns:     The angle value in degrees
          ***/

        static float toDegrees(const Angle& angle);

        /*** 'toDegrees' function
          ** Description: Converts an angle from radians to degrees
          ** Receives:    [angle] The angle that will be converted
          ** Returns:     The angle value in degrees
          ***/

        static float toDegrees(float angle);

        /*** 'difference' function
          ** Description: Calculates the difference of two angles
          ** Receives:    [a] The first angle
                          [b] The second angle
          ** Returns:     The difference of the angles (positive if a > b or negative if a < b)
          ***/

        static float difference(const Angle& a, const Angle& b);

    public:

        /*** Constructor
          ** Description: Creates an invalid angle
          ** Receives:    Nothing
          ***/

        Angle();

        /*** Constructor
          ** Description: Creates a valid angle
          ** Receives:    [known] Indicates if the angle is known.
                                    If not, 'value' is ignored
                          [value] The angle value
          ***/

        Angle(bool known, float value);

    public:

        /*** '=' operator

          ** Description: Copies an angle

          ** Receives:    [other] The angle that will be copied

          ** Returns:     This angle

          ***/

        Angle& operator =(const Angle& other);

        /*** '=' operator
          ** Description: Sets the angle value
          ** Receives:    [value] The new angle value
          ** Returns:     This angle
          ***/
        Angle& operator =(const float value);

    public:

        /*** 'setValue' function

          ** Description: Validates the angle and sets its value

          ** Receives:    [value] The angle value

          ** Returns:     Nothing

          ***/

        void setValue(float value);

        /*** 'value' function

          ** Description: Gets the angle value

          ** Receives:    Nothing

          ** Returns:     The angle value

          ***/

        float value() const;

    public:

        /*** 'isUnknown' function

          ** Description: Verifies if the angle is unknown

          ** Receives:    Nothing

          ** Returns:     'false' if the angle value is known, 'true' otherwise

          ***/

        bool isUnknown() const;

        /*** 'isValid' function

          ** Description: Verifies if the angle is valid

          ** Receives:    Nothing

          ** Returns:     'false' if the angle is invalid, 'true' otherwise

          ***/

        bool isValid() const;

        /*** 'setUnknown' function

          ** Description: Sets the angle as unknown

          ** Receives:    Nothing

          ** Returns:     Nothing

          ***/

        void setUnknown();

        /*** 'setInvalid' function

          ** Description: Sets the angle as invalid

          ** Receives:    Nothing

          ** Returns:     Nothing

          ***/

        void setInvalid();

};


#endif // ANGLE_H
