#include "matrix.h"

Matrix::Matrix(unsigned lines, unsigned columns) {

    _lines = lines;
    _columns = columns;
    _matrix = NULL;

    allocMat();
    init();

}

Matrix::~Matrix() {
    deallocMat();

}

void Matrix::allocMat(){
    if(_lines != 0 && _columns != 0){
        _matrix = new float*[_lines];
        for(unsigned i = 0; i < _lines; i++){
            _matrix[i] = new float[_columns];
        }
    }
}

void Matrix::deallocMat(){
    if(_matrix != NULL){
        for(unsigned i = 0; i < _lines; i++){
            delete[] _matrix[i];
        }
        delete[] _matrix; //The delete [] operator deallocates memory and calls destructors for an array of objects created with new [].
        _matrix=NULL;
    }
    
}


void Matrix::init(){
    for(unsigned i = 0; i < _lines; i++){
        for(unsigned j = 0; j < _columns; j++){
            _matrix[i][j]=0;
        }
    }
} //end init()

void Matrix::setSize(unsigned lines, unsigned columns){
    deallocMat();
    _lines = lines;
    _columns = columns;
    allocMat();
    init();

}//end setSize()

float Matrix::getInfo(unsigned line, unsigned column){
    assert(line < _lines && column < _columns);
    return _matrix[line][column];

}

void Matrix::setInfo(unsigned line, unsigned column, float value){
    assert(line < _lines && column < _columns);
    _matrix[line][column]=value;

}



Matrix Matrix::transposed() const{
    Matrix matTemp(_columns, _lines);
    for(int i=0; i<_lines; i++){
        for(int j=0; j<_columns; j++){
            matTemp.setInfo(j, i, _matrix[i][j]);
        }
    }
    return matTemp;
}


Matrix Matrix::diag(unsigned size, float diagValue){
    Matrix  mat(size, size);
    for(int i=0; i < size; i++){
        mat.setInfo(i,i, diagValue);
    }
    return mat;

}

Matrix Matrix::identity(unsigned size){
    return diag(size, 1);

}

void Matrix::copyFrom(const Matrix &M){
    assert(_lines==M.lines() && _columns == M.columns());
    for(int i=0; i<_lines; i++){
        for(int j=0; j<_columns; j++){
            _matrix[i][j] = M.getInfo(i, j);
        }
    }

    
}