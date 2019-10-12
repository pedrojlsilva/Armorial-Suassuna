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

#include "behavior_markplayer.hh"
#include <WRCoach/entity/player/behavior/wrbehavior.hh>

#define AREA_FACTOR 1.2
#define SECURITY_FACTOR 3.0

QString Behavior_MarkPlayer::name() {
    return "Behavior_MarkPlayer";
}

Behavior_MarkPlayer::Behavior_MarkPlayer() {
    _toMarkID = -1;
    setDistToMark(0.30f);
    setCanGoToOpTeamFieldSide(true);
    setMarkBetweenBall(false);
    setDirectOrIndirect(false);

    _sk_goTo = NULL;
}

void Behavior_MarkPlayer::configure() {
    usesSkill(_sk_goTo = new Skill_GoTo());
}

void Behavior_MarkPlayer::run() {
    if(_toMarkID == -1)
        std::cout << "[WARNING]" << name().toStdString() << ": _toMarkID not set!\n";

    // Check if player to mark is available
    if(PlayerBus::theirPlayerAvailable(_toMarkID)==false) {
        std::cout << "[WARNING] Player to mark ID not available!\n";
        return;
    }

    Position desiredPosition;
    Position lookPosition;

    // Position between player to mark and goal
    const Position posToMark = PlayerBus::theirPlayer(_toMarkID)->position();
    if(_markBetweenBall) {
        desiredPosition = WR::Utils::threePoints(posToMark, loc()->ball(), _distToMark, 0.0f);
        lookPosition = WR::Utils::threePoints(loc()->ball(), posToMark, GlobalConstants::highDistance(), GEARSystem::Angle::pi);
    } else {
        desiredPosition = WR::Utils::threePoints(posToMark, loc()->ourGoal(), _distToMark, 0.0f);
        lookPosition = WR::Utils::threePoints(posToMark, loc()->ourGoal(), GlobalConstants::highDistance(), GEARSystem::Angle::pi);
    }

    // Defense area exception
    if(loc()->isInsideOurArea(desiredPosition, AREA_FACTOR)) {
        // Mark between ball
        desiredPosition = WR::Utils::threePoints(posToMark, loc()->ball(), _distToMark, 0.0f);
        lookPosition = WR::Utils::threePoints(loc()->ball(), posToMark, GlobalConstants::highDistance(), GEARSystem::Angle::pi);
        if(loc()->isInsideOurArea(desiredPosition, AREA_FACTOR)){
            desiredPosition = loc()->ourAreaMargin(desiredPosition, player()->robotRadius()*SECURITY_FACTOR);
        }
    }
    // Direct and Indirect exeption
    while(_isDirectOrIndirect && WR::Utils::distance(desiredPosition, loc()->ball())< 0.65)
        desiredPosition = WR::Utils::threePoints(desiredPosition, loc()->ourGoal(),0.1, 0.0);

    // Op. team field side exception
    if(_canGoToOpTeamFieldSide == false)
        if((loc()->ourSide().isRight() && desiredPosition.x()<0.60f) || (loc()->ourSide().isLeft() && desiredPosition.x()>-0.60f))
            desiredPosition = WR::Utils::threePoints(loc()->ourGoal(), posToMark, loc()->fieldMaxX()-0.60f, 0.0f);

    // Always stays in position
    _sk_goTo->setDestination(desiredPosition);
    _sk_goTo->setPositionToLook(lookPosition);
    _sk_goTo->avoidBall(utils()->isBehindBall(desiredPosition));
    _sk_goTo->avoidTeammates(true);
    _sk_goTo->avoidOpponents(true);
    _sk_goTo->avoidGoalArea(false);
}
