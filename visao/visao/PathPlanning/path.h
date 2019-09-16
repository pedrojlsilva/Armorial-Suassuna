#include<bits/stdc++.h>
using namespace std;

typedef struct{
    int parent_i, parent_j;
    double f, g, h;
} cell;

class pathPlanner{
    public:
        pathPlanner(int tam_row, int tam_col);
        void aStar(bool **mat, pair<int, int> &ori, pair<int, int> &dest);
        bool isValid(int row, int col);
        bool isUnblocked(bool **mat, int row, int col);
        bool isDestination(int row, int col, pair<int, int> &dest);
        double calculateHValue(int row, int col, pair<int, int> &dest);
        void tracePath(cell **cellDetails, pair<int, int> &dest);
    private:
        int height;
        int weight;
        vector<pair<int, int>> cells;
        vector<pair<int, int>> path_vector;
        bool havePath;
        bool **closedList;
        cell **cellDetails;
};
