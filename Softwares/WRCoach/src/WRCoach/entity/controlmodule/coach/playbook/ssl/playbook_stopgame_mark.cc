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

#include "playbook_stopgame_mark.hh"
#include <WRCoach/entity/controlmodule/coach/playbook/wrplaybook.hh>
#include <QStack>

#pragma GCC diagnostic ignored "-Wunused-parameter"

QString Playbook_StopGame_Mark::name() {
    return "Playbook_StopGame_Mark";
}

int Playbook_StopGame_Mark::maxNumPlayer() {
    return 3;
}

Playbook_StopGame_Mark::Playbook_StopGame_Mark() {
    _bh_markBall = NULL;

    setCanGoToOpTeamFieldSide(true);
    setDisableMarkBall(false);
    setMarkDistance(0.60);
    setMarkBetweenBall(true);
}

void Playbook_StopGame_Mark::configure(int numPlayers) {
    usesBehavior(_bh_markBall = new Behavior_MarkBall());
    for (int i = 0; i < numPlayers; i++){
        Behavior_MarkPlayer *bh_mp=NULL;
        Behavior_MarkBallA *bh_mba=NULL;

        _bh_markPlayer.push_back(bh_mp = new Behavior_MarkPlayer());
        usesBehavior(bh_mp);
        _bh_markBallA.push_back(bh_mba = new Behavior_MarkBallA());
        usesBehavior(bh_mba);
    }
}

void Playbook_StopGame_Mark::run(int numPlayers) {

    // Set MarkBall
    if(_disableMarkBall == false) {
        quint8 markBall = dist()->getOneKNN(loc()->ball());
        _bh_markBall->setCanGoToOpTeamFieldSide(_canGoToOpTeamFieldSide);
        _bh_markBall->setAdvanceToBall(false);
        _bh_markBall->setDistToMark(_markDistance);
        setPlayerBehavior(markBall, _bh_markBall);
    }

    // Generate opponents to mark list
    QList<quint8> oppsToMark;
    QList<kNN::element> opponents = utils()->getOpponentKNN(6, loc()->ourGoal());
    for(int i = 0; i < opponents.size(); i++){
        if(isMarkable(opponents.at(i).id)) {
          oppsToMark.append(opponents.at(i).id);
        }
    }

    // Calc num of MBA's
    int markBallANum = (oppsToMark.size() < dist()->playersAvailable())? (dist()->playersAvailable() - oppsToMark.size()) : 0;
    QList<quint8> markBallAssistants = dist()->getKNN(markBallANum, loc()->ball());

    // MarkBallA's (other players)
    int mba=0;
    Sides::SIDE side = (loc()->ourSide().isRight()? Sides::LEFT : Sides::RIGHT);
    if(markBallAssistants.size()==1)
        side = Sides::UNDEFINED;
    while(markBallAssistants.empty()==false) {
        // Get marker
        const quint8 markerA = markBallAssistants.takeFirst();

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
                side = (markBallAssistants.size()==1? Sides::RIGHT : Sides::CENTER);
            } else if(side==Sides::CENTER)
                side = Sides::RIGHT;
            else
                side = Sides::LEFT;
        } else {
            if(side==Sides::RIGHT)
                side = (markBallAssistants.size()==1? Sides::LEFT : Sides::CENTER);
            else if(side==Sides::CENTER)
                side = Sides::LEFT;
            else
                side = Sides::RIGHT;
        }
    }

    QList<quint8> markers = dist()->getAllKNN(loc()->ourGoal());
    QList<quint8> threats = getThreatsByGoalAngle(oppsToMark, markers);

    // Used to navigate throught the lists
    int i = 0;
    // Iterates throught the list of threats
    while(threats.empty()==false && markers.size() > 0) {

        // Variable to store the closest distance that an marker has to the iteration threat
        float closestDistance = GlobalConstants::highDistance();

        // Stores the position on the list of the marker that is currently closer to the threat
        int closestMarkersListPosition = 0;

        // Stores the iteration threat position
        Position threatPos = PlayerBus::theirPlayer(threats.first())->position();

        // Iterates throught all our availables markers to find the one that is closest to the threat
        for(int j = 0; j < markers.size(); j++) {
            float distance = PlayerBus::ourPlayer(markers.at(j))->distanceTo(threatPos);
            if(distance < closestDistance) {
                closestDistance = distance;
                closestMarkersListPosition = j;
            }
        }

        // Set the behavior parameters and the iteration threat
        _bh_markPlayer.at(i)->setCanGoToOpTeamFieldSide(_canGoToOpTeamFieldSide);
        _bh_markPlayer.at(i)->setDirectOrIndirect(true);
        _bh_markPlayer.at(i)->setMarkBetweenBall(_markBetweenBall);
        _bh_markPlayer.at(i)->setPlayerToMarkID(threats.first());

        // Sets the markPlayer behavior to the player that has the shortest distance to the currently threat
        setPlayerBehavior(markers.at(closestMarkersListPosition), _bh_markPlayer.at(i));

        // Removes the already chosen player from the list of markers
        markers.removeAt(closestMarkersListPosition);
        threats.removeFirst();

        i++;
    }
}

// Get the opponents IDs that have the biggest angles module to our goal
QList<quint8> Playbook_StopGame_Mark::getThreatsByGoalAngle(QList<quint8> oppsToMark, QList<quint8> markers) {

    QList<std::pair<quint8, float> > threats;

    // Get obstacles list
    QList<Obstacle> obstacles = FreeAngles::getObstacles();

    // Iterates through the obstacles list to remove the players that will going to mark to avoid oscillation
    for(int j = 0; j < obstacles.size(); j++) {
        Obstacle obst = obstacles.at(j);

        for(int k = 0; k < markers.size(); k++){
            quint8 ourPlayerId = markers.at(k);
            quint8 ourTeam = PlayerBus::ourPlayer(ourPlayerId)->teamId() ;

            if(obst.team() ==  ourTeam && obst.id() == ourPlayerId) {
                obstacles.removeAt(j);
                j--;
                break;
            }
        }
    }

    for(int i = 0; i < oppsToMark.size(); i++) {
        PlayerAccess *theirPlayer = PlayerBus::theirPlayer(oppsToMark.at(i));

        // Get a list contaning the intervals of the freeAngles that an opponent have to our goal
        QList<FreeAngles::Interval> goalFreeAngles = FreeAngles::getFreeAngles(theirPlayer->position(),
                                                                               loc()->ourGoalRightPost(), loc()->ourGoalLeftPost(),
                                                                               obstacles, false);

        float biggerAngleSum = 0.0;
        // Iterates over the list containing the freeAngles to our goal and get the biggest interval
        while(goalFreeAngles.empty()==false){
            float angle = WR::Utils::angleDiff(goalFreeAngles.first().angFinal(), goalFreeAngles.first().angInitial());
            biggerAngleSum += angle;
            goalFreeAngles.removeFirst();
        }

        threats.append(std::make_pair(oppsToMark.at(i), biggerAngleSum));

//        std::cout << "\n id = " << (int)oppsToMark.at(i) << "  ||  angle = " << biggerAngleSum << "\n";

    }

    // Sorts the list of threats based on their module of free angle to ourGoal
    std::sort(threats.begin(), threats.end(), angleCompare);

    QList<quint8> threatsIDs;
    // Creates another QList only with valid threats
    for(int i = 0; i < threats.size(); i++) {
        threatsIDs.append(threats.at(i).first);
    }

    return threatsIDs;

}

bool Playbook_StopGame_Mark::angleCompare(std::pair<quint8, float> &a1, std::pair<quint8, float> &a2) {
    return  (a1.second > a2.second);
}

bool Playbook_StopGame_Mark::isMarkable(quint8 opPlayer) {
    // Check too close to ball
    if(PlayerBus::theirPlayer(opPlayer)->distBall() < loc()->fieldMaxX()*0.25) {
        return false;
    }

    // Check op defense player (GK, etc) by distance to their goal
    if(PlayerBus::theirPlayer(opPlayer)->distOurGoal() <= 1.5*loc()->fieldDefenseRadius()) {
        return false;
    }

    return true;
}


