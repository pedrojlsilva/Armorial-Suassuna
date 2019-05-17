//#include <Eigen/Dense> olhar essa bib, pq tem funções para o uso de matrizes

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
    _matrix transpose();
    _matrix identityMatix();

public:
    Matrix(unsigned lines, unsigned columns);
    ~Matrix();
    

};

#endif
