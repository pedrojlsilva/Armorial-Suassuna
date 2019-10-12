/***
 * Warthog Robotics
 * University of Sao Paulo (USP) at Sao Carlos
 * http://www.warthog.sc.usp.br/
 *
 * ThD file is part of WRCoach project.
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

#include "sslstrategy_wr2015_theirdirectkick.hh"
#include <WRCoach/entity/controlmodule/coach/strategy/wrstrategystate.hh>

#pragma GCC diagnostic ignored "-Wunused-parameter"

QString SSLStrategy_WR2015_TheirDirectKick::name() {
    return "SSLStrategy_WR2015_TheirDirectKick";
}

SSLStrategy_WR2015_TheirDirectKick::SSLStrategy_WR2015_TheirDirectKick() {
	_pb_goal = NULL;
	_pb_defense = NULL;
    _pb_stopGame_mark = NULL;

    _setDisableMarkBall = false;
}

void SSLStrategy_WR2015_TheirDirectKick::configure(int numOurPlayers) {
    usesPlaybook(_pb_goal = new Playbook_Goal());
    usesPlaybook(_pb_defense = new Playbook_Defense());
    usesPlaybook(_pb_stopGame_mark = new Playbook_StopGame_Mark());
}

void SSLStrategy_WR2015_TheirDirectKick::run(int numOurPlayers) {
    // Obs: The order in which the players are put in their playbooks is important in the matter of preventing oscillation

    //Configure Playbook Defense
    _pb_stopGame_mark->setMarkBetweenBall(true);
    _pb_stopGame_mark->setCanGoToOpTeamFieldSide(true);
    _pb_stopGame_mark->setDisableMarkBall(_setDisableMarkBall);
    _pb_stopGame_mark->setMarkDistance(0.6);


    // ADD PLAYERS
    // Goal (1, GK)
    quint8 gk = dist()->getGK();
    _pb_goal->addPlayer(gk);
    _pb_goal->setGK(gk);

    if(_setDisableMarkBall == false) {
        // StopGame_Mark (1, MarkBall)
        quint8 mb = dist()->getOneKNN(loc()->ball());
        _pb_stopGame_mark->addPlayer(mb);
    }

    // This structure below must be in this order to avoid oscilation between GKA and Markers because of the used kNN parameters
    // At this point my available players in the "dist()" are the total number of players minus the two added above
    if(dist()->playersAvailable() > 2) {
        // Here I want GKA only if I have two players already marking
        if(dist()->playersAvailable() == (_setDisableMarkBall == false? 4 : 5)) {
            // Goal (1, GKA)
            quint8 gka1 = dist()->getOneKNN(loc()->ourGoal());
            _pb_goal->addPlayer(gka1);
        }

        // Goal (1, GKA)
        quint8 gka2 = dist()->getOneKNN(loc()->ourGoal());
        _pb_goal->addPlayer(gka2);
    }

    // MarkPlayer (1)
    quint8 a = dist()->getOneKNN(loc()->ball());
    _pb_stopGame_mark->addPlayer(a);

    // MarkPlayer (1)
    quint8 b = dist()->getOneKNN(loc()->ball());
    _pb_stopGame_mark->addPlayer(b);

    if(_setDisableMarkBall == true) {
        // MarkPlayer (1)
        quint8 c = dist()->getOneKNN(loc()->ball());
        _pb_stopGame_mark->addPlayer(c);
    }
}
