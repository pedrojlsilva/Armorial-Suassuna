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

#ifndef PLAYBOOK_STOPGAME_MARK_HH
#define PLAYBOOK_STOPGAME_MARK_HH

#include <WRCoach/entity/controlmodule/coach/playbook/playbook.hh>
#include <WRCoach/utils/knn/knn.hh>

class Playbook_StopGame_Mark : public Playbook{
private:
    void configure(int numPlayers);
    void run(int numPlayers);

    // Behaviors
    Behavior_MarkBall *_bh_markBall;
    QList<Behavior_MarkPlayer *>  _bh_markPlayer;
    QList<Behavior_MarkBallA *> _bh_markBallA;

    // Parameters
    bool _disableMarkBall;
    bool _canGoToOpTeamFieldSide;
    bool _markBetweenBall;
    float _markDistance;

    // Internal
    bool isMarkable(quint8 opPlayer);
    QList<quint8> getThreatsByGoalAngle(QList<quint8> oppsToMark, QList<quint8> markers);
    static bool angleCompare(std::pair<quint8, float> &a1, std::pair<quint8, float> &a2);

public:
    Playbook_StopGame_Mark();
    QString name();
    int maxNumPlayer();

    void setCanGoToOpTeamFieldSide(bool canGoToAttackArea) { _canGoToOpTeamFieldSide = canGoToAttackArea; }
    void setMarkDistance(float markDistance){ _markDistance = markDistance; }
    void setMarkBetweenBall(bool markBetweenBall) { _markBetweenBall = markBetweenBall; }
    void setDisableMarkBall(bool disableMarkBall) { _disableMarkBall = disableMarkBall; }
};

#endif // PLAYBOOK_STOPGAME_MARK_HH
