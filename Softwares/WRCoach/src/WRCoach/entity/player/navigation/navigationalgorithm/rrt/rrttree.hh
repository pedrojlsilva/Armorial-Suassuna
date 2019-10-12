#ifndef RRTTREE_H
#define RRTTREE_H

#include "WRCoach/entity/player/navigation/navigationalgorithm/rrt/rrtnode.hh"

class RRTTree {
public:
    RRTTree();
    ~RRTTree();

    // Methods
    void append(RRTNode *leaf);
    RRTNode* getNN(Position myPos);
    QList<Position> getPath();

private:
    RRTNode* _root;
    QList<RRTNode*> _tree;
    QList<RRTNode*> _stackOfNodes;

    // Method for optimize distance comparison
    float squareDistance(Position posA, Position posB) { return pow(posA.x()-posB.x(), 2) + pow(posA.y()-posB.y(), 2); }
};
#endif // RRTTREE_H
