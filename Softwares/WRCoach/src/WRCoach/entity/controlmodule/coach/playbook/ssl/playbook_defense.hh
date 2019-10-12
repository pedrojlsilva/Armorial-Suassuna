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

#ifndef PLAYBOOK_DEFENSE_HH
#define PLAYBOOK_DEFENSE_HH

#include <WRCoach/entity/controlmodule/coach/playbook/playbook.hh>
#include <WRCoach/utils/knn/knn.hh>

class Playbook_Defense : public Playbook {
private:
    // Parameters
    bool _canGoToOpTeamFieldSide;
    bool _advanceToBall;
    bool _markBetweenBall;
    bool _useMarkListFromStrategy;
    bool _isDirectOrIndirect;
    float _markDistance;
    float _disableMarkBall;

    // External config
    QList<kNN::element> _oppsToMark;

    // Behaviors
    Behavior_MarkBall *_bh_markBall;
    QList<Behavior_MarkBallA*> _bh_markBallA;
    QList<Behavior_MarkPlayer*> _bh_markPlayer;

    // Internal
    bool isMarkable(quint8 opPlayer);

    void configure(int numPlayers);
    void run(int numPlayers);
public:
    Playbook_Defense();
    QString name();
    int maxNumPlayer();

    void setCanGoToOpTeamFieldSide(bool canGoToAttackArea) { _canGoToOpTeamFieldSide = canGoToAttackArea; }
    void setAdvanceToBall(bool advanceToBall) { _advanceToBall = advanceToBall; }
    void setMarkBetweenBall(bool markBetweenBall) { _markBetweenBall = markBetweenBall; }
    void setMarkDistance(float distance) { _markDistance = distance; }
    void setDisableMarkBall(bool disable) { _disableMarkBall = disable; }
    void setOppsToMark(QList<kNN::element> oppsToMark) { _oppsToMark = oppsToMark; }
    void setUseMarkListFromStrategy(bool useList) { _useMarkListFromStrategy = useList; }
    void setDirectOrIndirect(){_isDirectOrIndirect = true;}
};

#endif // PLAYBOOK_DEFENSE_HH
