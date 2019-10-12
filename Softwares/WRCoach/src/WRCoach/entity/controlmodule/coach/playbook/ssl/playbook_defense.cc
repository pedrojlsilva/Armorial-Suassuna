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

#include "playbook_defense.hh"
#include <WRCoach/entity/controlmodule/coach/playbook/wrplaybook.hh>

#pragma GCC diagnostic ignored "-Wunused-parameter"

QString Playbook_Defense::name() {
    return "Playbook_Defense";
}

Playbook_Defense::Playbook_Defense() {
    setCanGoToOpTeamFieldSide(true);
    setAdvanceToBall(false);
    setMarkBetweenBall(false);
    setMarkDistance(0.60f);
    setDisableMarkBall(false);
    setUseMarkListFromStrategy(false);

    _bh_markBall = NULL;
}

int Playbook_Defense::maxNumPlayer() {
    return INT_MAX;
}

void Playbook_Defense::configure(int numPlayers) {
    _bh_markPlayer.clear();
    _bh_markBallA.clear();

    // MarkBall
    usesBehavior(_bh_markBall = new Behavior_MarkBall());

    // MarkPlayer/MarkBallA's
    for(int i=0; i<numPlayers; i++) {
        Behavior_MarkPlayer *bh_mp=NULL;
        Behavior_MarkBallA *bh_mba=NULL;

        _bh_markPlayer.push_back(bh_mp = new Behavior_MarkPlayer());
        usesBehavior(bh_mp);
        _bh_markBallA.push_back(bh_mba = new Behavior_MarkBallA());
        usesBehavior(bh_mba);
    }
}

void Playbook_Defense::run(int numPlayers) {

    // Get opponents to mark
    if(_useMarkListFromStrategy==false)
        _oppsToMark = utils()->getOpponentKNN(6, loc()->ourGoal());

    // Set MarkPlayer's
    int mp = 0;
    while(_oppsToMark.empty()==false && dist()->playersAvailable() > (_disableMarkBall? 0 : 1)) {

        // Get op
        const quint8 op = _oppsToMark.takeFirst().id;

        // Check "markable"
        if(isMarkable(op)==false)
            continue;

        // Check players available
        if(dist()->playersAvailable()==0)
            return;

        // Get marker
        Position toMarkPos = PlayerBus::theirPlayer(op)->position();
        if(toMarkPos.isUnknown())
            continue;
        const quint8 marker = dist()->getOneKNN(toMarkPos);

        // Set behavior
        Behavior_MarkPlayer *bh_mp = _bh_markPlayer.at(mp++);
        bh_mp->setCanGoToOpTeamFieldSide(_canGoToOpTeamFieldSide);
        bh_mp->setPlayerToMarkID(op);
        bh_mp->setDirectOrIndirect(true);
        if(_isDirectOrIndirect && PlayerBus::theirPlayer(op)->distBall()< 0.9)
            bh_mp->setMarkBetweenBall(!_markBetweenBall);
        else
            bh_mp->setMarkBetweenBall(_markBetweenBall);
        setPlayerBehavior(marker, bh_mp);
    }

    // Set MarkBall
    if(_disableMarkBall==false) {
        quint8 markBall = dist()->getOneKNN(loc()->ball());
        _bh_markBall->setCanGoToOpTeamFieldSide(_canGoToOpTeamFieldSide);
        _bh_markBall->setAdvanceToBall(_advanceToBall && canKickBall());
        _bh_markBall->setDistToMark(_markDistance);
        setPlayerBehavior(markBall, _bh_markBall);
    }

    // MarkBallA's (other players)
    int mba=0;
    Sides::SIDE side = (loc()->ourSide().isRight()? Sides::LEFT : Sides::RIGHT);
    QList<quint8> markers = dist()->getAllY();
    if(markers.size()==1)
        side = Sides::UNDEFINED;
    while(markers.empty()==false) {
        // Get marker
        const quint8 markerA = markers.takeFirst();

        // Set behavior
        Behavior_MarkBallA *bh_mba = _bh_markBallA.at(mba++);
        bh_mba->setD(0.25f);
        bh_mba->setSide(side);
        bh_mba->setCanGoToOpTeamFieldSide(_canGoToOpTeamFieldSide);
        bh_mba->setDistToMark(_markDistance*1.25f);
        setPlayerBehavior(markerA, bh_mba);

        // Change side for next MBA
        if(loc()->ourSide().isRight()) {
            if(side==Sides::LEFT) {
                side = (markers.size()==1? Sides::RIGHT : Sides::CENTER);
            } else if(side==Sides::CENTER)
                side = Sides::RIGHT;
            else
                side = Sides::LEFT;
        } else {
            if(side==Sides::RIGHT)
                side = (markers.size()==1? Sides::LEFT : Sides::CENTER);
            else if(side==Sides::CENTER)
                side = Sides::LEFT;
            else
                side = Sides::RIGHT;
        }
    }

    // Reset list of players to mark
    _oppsToMark.clear();
}

bool Playbook_Defense::isMarkable(quint8 opPlayer) {
    // Check too close to ball
    if(PlayerBus::theirPlayer(opPlayer)->distBall() < loc()->fieldMaxX()*0.25)
        return false;

    // Check op defense player (GK, etc) by distance to their goal
    if(PlayerBus::theirPlayer(opPlayer)->distOurGoal() <= 1.5*loc()->fieldDefenseRadius()){
        return false;
    }

    // Check if  is a direct or indirect kick
   // if(PlayerBus::theirPlayer(opPlayer)->distBall() <5)

    return true;
}
