#ifndef matrix

#define MATRIX_H

#include <iostream>
#include <assert.h>

class Matrix {


private:
    unsigned _lines;
    unsigned _columns;

    float **_matrix;
    void allocMat();
    void deallocMat();
    void init();

public:
    Matrix(unsigned lines, unsigned columns);
    ~Matrix();
    

};

#endif