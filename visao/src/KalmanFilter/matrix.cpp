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

_matrix Matrix::transpose(){
    static Matrix(3,4) **aux;
    int tamanho  = _lines * _columns;
    *aux = (Matrix **) malloc( tamanho * sizeof(Matrix));

    if(_lines != 0 && _columns != 0){
        for(unsigned i = 0; i < _lines; i++){
            for(unsigned j = 0; j < _columns; j++){
                aux[j][i] = matrix[i][j];
            }
        }
    }

    return **aux; 
}    
    
_matrix Matrix::identityMatrix(){

    if(_lines != 0 && _columns != 0){
    
        for(unsigned i = 0; i < _lines; i++){
            for(unsigned j = 0; j < _columns; j++){
                if(i == j){
                    _matrix[i][j] = 1;
                }else{
                    _matrix[i][j] = 0;

                }
            }
        }
    }

    return _matrix; 
}   
