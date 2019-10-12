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

#include "sslstrategy_wr2015_gameoff.hh"
#include <WRCoach/entity/controlmodule/coach/strategy/wrstrategystate.hh>

#pragma GCC diagnostic ignored "-Wunused-parameter"

QString SSLStrategy_WR2015_GameOff::name() {
    return "SSLStrategy_WR2015_GameOff";
}

SSLStrategy_WR2015_GameOff::SSLStrategy_WR2015_GameOff() {
    _pb_goal = NULL;
    _pb_defense = NULL;
}

void SSLStrategy_WR2015_GameOff::configure(int numOurPlayers) {
    usesPlaybook(_pb_goal = new Playbook_Goal());
    usesPlaybook(_pb_defense = new Playbook_Defense());
}

void SSLStrategy_WR2015_GameOff::run(int numOurPlayers) {

    // ADD PLAYERS
    // GK or MB (1)
    quint8 gk = dist()->getGK();
    reduceSpeed(gk);
    if(numOurPlayers==1) {
        // MB
        _pb_defense->addPlayer(gk);

    } else {
        // GK
        _pb_goal->addPlayer(gk);
        _pb_goal->setGK(gk);
    }

    // GKA or MB (1)
    bool ballNearOurArea = (loc()->distBallOurGoal() <= 2.5*loc()->fieldDefenseRadius());
    if(ballNearOurArea) {
        // Goal (1, GKA)
        quint8 gka = dist()->getOneKNN(loc()->ourGoal());
        _pb_goal->addPlayer(gka);
        reduceSpeed(gka);
    } else {
        // Defense (1, MarkBall)
        quint8 mb = dist()->getOneKNN(loc()->ball());
        _pb_defense->addPlayer(mb);
        reduceSpeed(mb);
    }

    // Goal or Defense (1)
    if(loc()->isInsideOurField(loc()->ball())) {
        // Goal
        quint8 gka = dist()->getOneKNN(loc()->ourGoal());
        _pb_goal->addPlayer(gka);
        reduceSpeed(gka);
    } else {
        // Defense
        quint8 defense = dist()->getOneKNN(loc()->ball());
        _pb_defense->addPlayer(defense);
        reduceSpeed(defense);
    }

    // Rest: Defense (3)
    QList<quint8> allPlayers = dist()->getAllPlayers();
    for(int i=0; i<allPlayers.size(); i++)
        reduceSpeed(allPlayers.at(i));
    _pb_defense->addPlayers(allPlayers);

    // CONFIGURE
    _pb_defense->setAdvanceToBall(false);
    _pb_defense->setCanGoToOpTeamFieldSide(true);
    _pb_defense->setDisableMarkBall(ballNearOurArea);
}

void SSLStrategy_WR2015_GameOff::reduceSpeed(quint8 id) {
    if(PlayerBus::ourPlayerAvailable(id))
        PlayerBus::ourPlayer(id)->setMaxLSpeed(GlobalConstants::gameOffMaxLSpeed());
}
