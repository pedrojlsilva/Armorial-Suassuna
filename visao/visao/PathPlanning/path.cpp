#include "path.h"
using namespace std;

#define INF 9999999999

using namespace std;

pathPlanner::pathPlanner(int tam_row, int tam_col){
    weight = tam_row;
    height = tam_col;

    closedList = (bool **) malloc(weight * sizeof(bool *));
    for(int x = 0; x < weight; x++){
        closedList[x] = (bool *) malloc(height * sizeof(bool));
    }

    memset(closedList, false, sizeof(closedList));

    cellDetails = (cell **) malloc(weight * sizeof(cell *));
    for(int x = 0; x < weight; x++){
        cellDetails[x] = (cell *) malloc(height * sizeof(cell));
        for(int y = 0; y < height; y++){
            cellDetails[x][y].f = INF;
            cellDetails[x][y].g = INF;
            cellDetails[x][y].h = INF;
            cellDetails[x][y].parent_i = -1;
            cellDetails[x][y].parent_j = -1;
        }
    }

    havePath = false;
}

bool pathPlanner::isValid(int row, int col){
    return (row >= 0) && (row < weight) && (col >= 0) && (col < height);
}

bool pathPlanner::isUnblocked(bool **grid, int row, int col){
    return grid[row][col];
}

bool pathPlanner::isDestination(int row, int col, pair<int, int> &dest){
    return (row == dest.first && col == dest.second);
}

double pathPlanner::calculateHValue(int row, int col, pair<int, int> &dest){
    return ((double) sqrt(pow((row - dest.first), 2) + pow((col - dest.second), 2)));
}

void pathPlanner::tracePath(cell **cellDetails, pair<int, int> &dest){
    int row = dest.first;
    int col = dest.second;

    stack<pair<int, int>> path;

    while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col)){ 
		path.push(make_pair(row, col));
		int temp_row = cellDetails[row][col].parent_i; 
        int temp_col = cellDetails[row][col].parent_j; 
        row = temp_row;
		col = temp_col;
	}

    path.push(make_pair(row, col));

    while(!path.empty()){
        pair<int, int> p = path.top();
        path.pop();
        path_vector.push_back(make_pair(p.first, p.second));
    }

    return ;
}

void pathPlanner::aStar(bool **grid, pair<int, int> &src, pair<int, int> &dest){
    if(!isValid(src.first, src.second)) return ;
    if(!isValid(dest.first, dest.second)) return ;
    if(!isUnblocked(grid, src.first, src.second)) return ;
    if(!isUnblocked(grid, dest.first, dest.second)) return ;
    if(isDestination(src.first, src.second, dest)) return ;

    int i, j;
    
    cellDetails[src.first][src.second].f = 0.0;
    cellDetails[src.first][src.second].g = 0.0;
    cellDetails[src.first][src.second].h = 0.0;
    cellDetails[src.first][src.second].parent_i = src.first;
    cellDetails[src.first][src.second].parent_j = src.second;

    set<pair<double, pair<int, int>>> openList;
    openList.insert(make_pair(0.0, make_pair(i, j)));
    havePath = false;

    while(!openList.empty()){
        pair<double, pair<int, int>> p = *openList.begin();
        openList.erase(openList.begin());

        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        double gNew, hNew, fNew;
        if(isValid(i-1, j)){
            if(isDestination(i-1, j, dest)){
                cellDetails[i-1][j].parent_i = i;
                cellDetails[i-1][j].parent_j = j;
                tracePath(cellDetails, dest);
                havePath = true;
                return ;
            }else if(closedList[i-1][j] == false && isUnblocked(grid, i-1, j) == true){
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i-1, j, dest);
                fNew = gNew + hNew;

                if(cellDetails[i-1][j].f == FLT_MAX || cellDetails[i-1][j].f > fNew){
                    openList.insert(make_pair(fNew, make_pair(i-1, j)));

                    cellDetails[i-1][j].f = fNew;
                    cellDetails[i-1][j].g = gNew;
                    cellDetails[i-1][j].h = hNew;
                    cellDetails[i-1][j].parent_i = i;
                    cellDetails[i-1][j].parent_j = j;
                }
            }
        }

        if(isValid(i+1, j)){
            if(isDestination(i+1, j, dest)){
                cellDetails[i+1][j].parent_i = i;
                cellDetails[i+1][j].parent_j = j;
                tracePath(cellDetails, dest);
                havePath = true;
                return ;
            }else if(closedList[i+1][j] == false && isUnblocked(grid, i+1, j) == true){
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i+1, j, dest);
                fNew = gNew + hNew;

                if(cellDetails[i+1][j].f == FLT_MAX || cellDetails[i+1][j].f > fNew){
                    openList.insert(make_pair(fNew, make_pair(i+1, j)));

                    cellDetails[i+1][j].f = fNew;
                    cellDetails[i+1][j].g = gNew;
                    cellDetails[i+1][j].h = hNew;
                    cellDetails[i+1][j].parent_i = i;
                    cellDetails[i+1][j].parent_j = j;
                }
            }
        }

        if(isValid(i, j+1)){
            if(isDestination(i, j+1, dest)){
                cellDetails[i][j+1].parent_i = i;
                cellDetails[i][j+1].parent_j = j;
                tracePath(cellDetails, dest);
                havePath = true;
                return ;
            }else if(closedList[i][j+1] == false && isUnblocked(grid, i, j+1) == true){
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j+1, dest);
                fNew = gNew + hNew;

                if(cellDetails[i][j+1].f == FLT_MAX || cellDetails[i][j+1].f > fNew){
                    openList.insert(make_pair(fNew, make_pair(i, j+1)));

                    cellDetails[i][j+1].f = fNew;
                    cellDetails[i][j+1].g = gNew;
                    cellDetails[i][j+1].h = hNew;
                    cellDetails[i][j+1].parent_i = i;
                    cellDetails[i][j+1].parent_j = j;
                }
            }
        }

        if(isValid(i, j-1)){
            if(isDestination(i, j-1, dest)){
                cellDetails[i][j-1].parent_i = i;
                cellDetails[i][j-1].parent_j = j;
                tracePath(cellDetails, dest);
                havePath = true;
                return ;
            }else if(closedList[i][j-1] == false && isUnblocked(grid, i, j-1) == true){
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j-1, dest);
                fNew = gNew + hNew;

                if(cellDetails[i][j-1].f == FLT_MAX || cellDetails[i][j-1].f > fNew){
                    openList.insert(make_pair(fNew, make_pair(i, j-1)));

                    cellDetails[i][j-1].f = fNew;
                    cellDetails[i][j-1].g = gNew;
                    cellDetails[i][j-1].h = hNew;
                    cellDetails[i][j-1].parent_i = i;
                    cellDetails[i][j-1].parent_j = j;
                }
            }
        }

        if(isValid(i-1, j+1)){
            if(isDestination(i-1, j+1, dest)){
                cellDetails[i-1][j+1].parent_i = i;
                cellDetails[i-1][j+1].parent_j = j;
                tracePath(cellDetails, dest);
                havePath = true;
                return ;
            }else if(closedList[i-1][j+1] == false && isUnblocked(grid, i-1, j+1) == true){
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i-1, j+1, dest);
                fNew = gNew + hNew;

                if(cellDetails[i-1][j+1].f == FLT_MAX || cellDetails[i-1][j+1].f > fNew){
                    openList.insert(make_pair(fNew, make_pair(i-1, j+1)));

                    cellDetails[i-1][j+1].f = fNew;
                    cellDetails[i-1][j+1].g = gNew;
                    cellDetails[i-1][j+1].h = hNew;
                    cellDetails[i-1][j+1].parent_i = i;
                    cellDetails[i-1][j+1].parent_j = j;
                }
            }
        }

        if(isValid(i-1, j-1)){
            if(isDestination(i-1, j-1, dest)){
                cellDetails[i-1][j-1].parent_i = i;
                cellDetails[i-1][j-1].parent_j = j;
                tracePath(cellDetails, dest);
                havePath = true;
                return ;
            }else if(closedList[i-1][j-1] == false && isUnblocked(grid, i-1, j-1) == true){
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i-1, j-1, dest);
                fNew = gNew + hNew;

                if(cellDetails[i-1][j-1].f == FLT_MAX || cellDetails[i-1][j-1].f > fNew){
                    openList.insert(make_pair(fNew, make_pair(i-1, j-1)));

                    cellDetails[i-1][j-1].f = fNew;
                    cellDetails[i-1][j-1].g = gNew;
                    cellDetails[i-1][j-1].h = hNew;
                    cellDetails[i-1][j-1].parent_i = i;
                    cellDetails[i-1][j-1].parent_j = j;
                }
            }
        }

        if(isValid(i+1, j+1)){
            if(isDestination(i+1, j+1, dest)){
                cellDetails[i+1][j+1].parent_i = i;
                cellDetails[i+1][j+1].parent_j = j;
                tracePath(cellDetails, dest);
                havePath = true;
                return ;
            }else if(closedList[i+1][j+1] == false && isUnblocked(grid, i+1, j+1) == true){
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i+1, j+1, dest);
                fNew = gNew + hNew;

                if(cellDetails[i+1][j+1].f == FLT_MAX || cellDetails[i+1][j+1].f > fNew){
                    openList.insert(make_pair(fNew, make_pair(i+1, j+1)));

                    cellDetails[i+1][j+1].f = fNew;
                    cellDetails[i+1][j+1].g = gNew;
                    cellDetails[i+1][j+1].h = hNew;
                    cellDetails[i+1][j+1].parent_i = i;
                    cellDetails[i+1][j+1].parent_j = j;
                }
            }
        }

        if(isValid(i+1, j-1)){
            if(isDestination(i+1, j-1, dest)){
                cellDetails[i+1][j-1].parent_i = i;
                cellDetails[i+1][j-1].parent_j = j;
                tracePath(cellDetails, dest);
                havePath = true;
                return ;
            }else if(closedList[i+1][j-1] == false && isUnblocked(grid, i+1, j-1) == true){
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i+1, j-1, dest);
                fNew = gNew + hNew;

                if(cellDetails[i+1][j-1].f == FLT_MAX || cellDetails[i+1][j-1].f > fNew){
                    openList.insert(make_pair(fNew, make_pair(i+1, j-1)));

                    cellDetails[i+1][j-1].f = fNew;
                    cellDetails[i+1][j-1].g = gNew;
                    cellDetails[i+1][j-1].h = hNew;
                    cellDetails[i+1][j-1].parent_i = i;
                    cellDetails[i+1][j-1].parent_j = j;
                }
            }
        }
    }
    
    return ;
}

vector<pair<int, int>> pathPlanner::getPath(){
    return path_vector;
}
