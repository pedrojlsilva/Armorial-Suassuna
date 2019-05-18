#ifndef matrix

#define MATRIX_H

#include <iostream>
#include <assert.h>

class Matrix{
    

private:
    unsigned _lines;
    unsigned _columns;

    float **_matrix;
    void allocMat();
    void deallocMat();
    void init();

public:
    //Constructors and Destructors 
    Matrix(unsigned lines, unsigned columns);
    ~Matrix();


    //Basics Functions
    void setSize(unsigned lines, unsigned columns);
    float getInfo(unsigned line, unsigned column);
    void setInfo(unsigned line, unsigned column, float value);
    //escrever função que retorna _lines e _columns


    // Mat functions 
    Matrix transposed() const;
    Matrix diag(unsigned size, float diagValue);
    Matrix identity(unsigned size);
    unsigned lines()const{ return _lines; }
    unsigned columns() const { return _lines; }
    void copyFrom(const Matrix &M);
    void print();

};

#endif