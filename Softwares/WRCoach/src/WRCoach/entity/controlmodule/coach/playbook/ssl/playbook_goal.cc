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

#include "playbook_goal.hh"
#include <WRCoach/entity/controlmodule/coach/playbook/wrplaybook.hh>

#define GK_RADIUS 0.40
#define GK_TAKEOUT_FACTOR 1.2
#define GK_TAKEOUT_EXTENDED_FACTOR 1.25

#define GKA_RADIUS 1.15

#define GKA_BALLDIST_REDUCE_D 2.5
#define GKA_D 0.17
#define GKA_D_REDUCED 0.12
#define GKA_D_EXTENDED 2.0*GKA_D

QString Playbook_Goal::name() {
    return "Playbook_Goal";
}

Playbook_Goal::Playbook_Goal() {
    setGK(-1);

    _bh_gk = NULL;
    _bh_gka_uni = NULL;
    _bh_gka_multiR = NULL;
    _bh_gka_multiL = NULL;
    _bh_gka_multiC = NULL;

    _state = STATE_POS;
    _gka_uni = 0;
    _gka_multiR = 0;
    _gka_multiL = 0;
    _gka_multiC = 0;
}

int Playbook_Goal::maxNumPlayer() {
    return 4;
}

void Playbook_Goal::configure(int numPlayers) {

    // GK
    usesBehavior(_bh_gk = new Behavior_GK());

    // GK + GKA
    if(numPlayers==2) {
        usesBehavior(_bh_gka_uni = new Behavior_GKA());
        _bh_gka_uni->setSide(Sides::UNDEFINED);
    }

    // GK + 2 GKA
    if(numPlayers>=3) {
        usesBehavior(_bh_gka_multiR = new Behavior_GKA());
        _bh_gka_multiR->setSide(Sides::RIGHT);
        usesBehavior(_bh_gka_multiL = new Behavior_GKA());
        _bh_gka_multiL->setSide(Sides::LEFT);
    }

    // GK + 3 GKA
    if(numPlayers>=4) {
        usesBehavior(_bh_gka_multiC = new Behavior_GKA());
        _bh_gka_multiC->setSide(Sides::CENTER);
    }
}

void Playbook_Goal::run(int numPlayers) {
    if(_gk == -1) {
        std::cout << "[WARNING] Playbook_Goal: GK not set, returning!\n";
        return;
    }

    // Set behaviors
    // GK (1)
    setPlayerBehavior(_gk, _bh_gk);
    dist()->removePlayer(_gk);

    // GKA Uni (1)
    if(numPlayers==2) {
        _gka_uni = dist()->getOneKNN(loc()->ourGoal());
        setPlayerBehavior(_gka_uni, _bh_gka_uni);
    }

    // GKA Multi (2)
    if(numPlayers==3) {
        QList<quint8> knn = dist()->getKNNandY(2, loc()->ourGoal());
        if(loc()->ourSide().isRight()) {
            _gka_multiL = knn.takeFirst();
            _gka_multiR = knn.takeFirst();
        } else {
            _gka_multiR = knn.takeFirst();
            _gka_multiL = knn.takeFirst();
        }

        // GKAs
        setPlayerBehavior(_gka_multiL, _bh_gka_multiL);
        setPlayerBehavior(_gka_multiR, _bh_gka_multiR);
    }

    // GKA Multi (3)
    if(numPlayers>=4) {
        QList<quint8> knn = dist()->getKNNandY(3, loc()->ourGoal());
        if(loc()->ourSide().isRight()) {
            _gka_multiL = knn.takeFirst();
            _gka_multiC = knn.takeFirst();
            _gka_multiR = knn.takeFirst();
        } else {
            _gka_multiR = knn.takeFirst();
            _gka_multiC = knn.takeFirst();
            _gka_multiL = knn.takeFirst();
        }

        // GKAs
        setPlayerBehavior(_gka_multiL, _bh_gka_multiL);
        setPlayerBehavior(_gka_multiR, _bh_gka_multiR);
        setPlayerBehavior(_gka_multiC, _bh_gka_multiC);
    }

    // Configure behaviors
    switch(_state) {
        default:
        case STATE_POS: {
            // GK
            _bh_gk->setRadius(GK_RADIUS);
            _bh_gk->setTakeoutEnabled(false);
            _bh_gk->setTakeoutFactor(GK_TAKEOUT_FACTOR);

            // GKA Uni
            if(numPlayers==2) {
                _bh_gka_uni->setRadius(GKA_RADIUS);
                _bh_gka_uni->setD(loc()->distBallOurGoal()<GKA_BALLDIST_REDUCE_D? GKA_D_REDUCED : GKA_D);
            }

            // GKA Multi (2)
            if(numPlayers==3) {
                // GKA_L
                _bh_gka_multiL->setRadius(GKA_RADIUS);
                _bh_gka_multiL->setD(loc()->distBallOurGoal()<GKA_BALLDIST_REDUCE_D? GKA_D_REDUCED : GKA_D);

                // GKA_R
                _bh_gka_multiR->setRadius(GKA_RADIUS);
                _bh_gka_multiR->setD(loc()->distBallOurGoal()<GKA_BALLDIST_REDUCE_D? GKA_D_REDUCED : GKA_D);
            }

            // GKA Multi (3)
            if(numPlayers>=4) {
                // GKA_L
                _bh_gka_multiL->setRadius(GKA_RADIUS);
                _bh_gka_multiL->setD(GKA_D+0.10f);

                // GKA_R
                _bh_gka_multiR->setRadius(GKA_RADIUS);
                _bh_gka_multiR->setD(GKA_D+0.10f);

                // GKA_C
                _bh_gka_multiC->setRadius(GKA_RADIUS);
            }

            // Switch state condition
            if(canKickBall() &&  loc()->isInsideOurArea(loc()->ball(), GK_TAKEOUT_FACTOR))
                _state = STATE_TAKEOUT;

        } break;

        case STATE_TAKEOUT: {

            // Enable GK takeout
            _bh_gk->setTakeoutEnabled(true);

            // GKA Uni
            if(numPlayers==2) {
                _bh_gka_uni->setD(GKA_D_EXTENDED);
            }

            // GKA Multi (2)
            if(numPlayers>=3) {
                    // GKA R
                    _bh_gka_multiR->setD(GKA_D_EXTENDED);

                    // GKA L
                    _bh_gka_multiL->setD(GKA_D_EXTENDED);
            }

            // GKA Multi (3)
            if(numPlayers>=4) {
                // GKA C
                _bh_gka_multiC->setD(GKA_D_EXTENDED);
                _bh_gka_multiC->setRadius(1.20*GKA_RADIUS);
            }

            // Switch state condition
            if(canKickBall()==false ||  loc()->isInsideOurArea(loc()->ball(), GK_TAKEOUT_EXTENDED_FACTOR)==false) // hysteresis applied
                _state = STATE_POS;

        } break;
    }
}
