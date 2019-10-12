/***
 * Warthog Robotics
 * University of Sao Paulo (USP) at Sao Carlos
 * http://www.warthog.sc.usp.br/
 *
 * This file is part of WRCoach project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef NAVIGATION_HH
#define NAVIGATION_HH

#include <WRCoach/entity/player/baseplayer.hh>
#include <WRCoach/entity/player/navigation/navigationalgorithm/navigationalgorithm.hh>
#include <WRCoach/entity/player/navigation/controlalgorithm/controlalgorithm.hh>

class Navigation {
public:
    Navigation(Player *player, NavigationAlgorithm *navAlg, ControlAlgorithm *linCtrAlg, ControlAlgorithm *angCtrAlg);

    virtual ~Navigation();

    // Setters
    void setGoal(const Position &destination, const Angle &orientation, bool avoidTeammates, bool avoidOpponents, bool avoidBall, bool avoidGoalArea, bool avoidTheirGoalArea);
    void setLError(float lError) { _lError = lError; }
    void setAError(float aError) { _aError = aError; }
    void setMaxLSpeed(float maxLSpeed) { _maxLSpeed = fabs(maxLSpeed); }
    void setMaxASpeed(float maxASpeed) { _maxASpeed = fabs(maxASpeed); }
    void setMaxLAcceleration(float maxLAcceleration) { _maxLAcceleration = maxLAcceleration; }

    // Getters
    Angle getDirection() const;
    float getDistance() const;
    QLinkedList<Position> getPath() const;
    float getLError() const { return _lError; }
    float getAError() const { return _aError; }
    float getMaxLSpeed() const { return _maxLSpeed; }
    float getMaxASpeed() const { return _maxASpeed; }
    float getmaxLAcceleration() const { return _maxLAcceleration; }

    // Interface functions
    void reset();
    double getLinearSpeed(float distError, Velocity velocity);
    double getAngularSpeed(float angularError, Velocity velocity);

    // PID
    void setLinearPIDParameters(double kp, double ki, double kd, double ilimit);
    void setAngularPIDParameters(double kp, double ki, double kd, double ilimit);

    // Discrete PID
    void setLinearDiscretePIDParameters(double kp, double ki, double kd);
    void setAngularDiscretePIDParameters(double kp, double ki, double kd);

    // Cascade PID
    void setLinearCascadePIDParameters(double dkp, double dkd, double skp, double ski, double skd, double siLimit, float maxSpeed);
    void setAngularCascadePIDParameters(double dkp, double dkd, double skp, double ski, double skd, double siLimit, float maxSpeed);
private:
    // General access
    Player* _player;
    NavigationAlgorithm *_navAlg;

    // Secondary access
    WRTeam *_ourTeam;
    WRTeam *_opTeam;
    Locations *_loc;

    // Internal
    ControlAlgorithm *_linCtrAlg;
    ControlAlgorithm *_angCtrAlg;

    Angle _direction;
    float _distance;

    // Errors
    float _lError;
    float _aError;

    // Maximum speed
    float _maxLSpeed;
    float _maxASpeed;

    // Maximum acceleration
    float _maxLAcceleration;

    float _calculateLinearSpeed(float distError, Velocity velocity);
    float _calculateAngularSpeed(float angGoal, Velocity velocity);
};

#endif // NAVIGATION_HH
