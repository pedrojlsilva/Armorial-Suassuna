#ifndef RRT_MODIFIED_H
#define RRT_MODIFIED_H

#include<bits/stdc++.h>
#include <QList>
#include <QPair>

namespace ArmorialPath 
{
    class RRTModified{
        public:
        RRTModified(bool **world, float timeLimit);
        ~RRTModified();
        void iterate();
        QList<QPair<int,int>> getPath();



        private:

        bool **_world;
        float _timeLimit;
        QList<QPair<int,int>> _path;
        QPair<int, int> getPointOutRobot(QPair<int, int> obstacle, QPair<int, int> vector, bool angle);
        QPair<int, int> hasObstacle(QPair<int, int> initialPoint, QPair<int, int> finalPoint);
        
    };
} 



#endif
