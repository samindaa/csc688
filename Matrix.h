/*
 *  MatrixMath.h Library for Matrix Math
 *
 *  Created by Charlie Matlack on 12/18/10.
 *  Modified from code by RobH45345 on Arduino Forums, taken from unknown source.
 *
 *  Author: Sam Abeyruwn has modified the code to work with Energia and Tiva C.
 *
 *  Matrix.h
 *
 *  Created on: Apr 1, 2014
 *      Author: sam
 *
 *  @Doc: http://playground.arduino.cc/Code/MatrixMath#.Uzs2-nWx3t7
 */

#ifndef MATRIXMATH_H_
#define MATRIXMATH_H_

#if defined(ENERGIA)
#include "Energia.h"
#else
#include <iostream>
#include <math.h>
//#include <string.h>
#endif

// In order to compile off-line
#if defined(ENERGIA)
typedef String MatrixString;
#else
typedef std::string MatrixString;
#endif

/**
 * Matrices should be stored in row-major arrays, which is fairly standard. The user must keep
 * track of array dimensions and send them to the functions; mistakes on dimensions will not be
 * caught by the library.
 *
 * It's worth pointing out that the MatrixInvert() function uses Gauss-Jordan elimination with
 * partial pivoting. Partial pivoting is a compromise between a numerically unstable algorithm
 * and full pivoting, which involves more searching and swapping matrix elements.
 *
 * Also, the inversion algorithm stores the result matrix on top of the the input matrix, meaning
 * no extra memory is allocated during inversion but your original matrix is gone.
 */
class MatrixMath
{
  public:
    MatrixMath();
    ~MatrixMath();
    void print(float* A, int m, int n, MatrixString label);
    void copy(float* A, int n, int m, float* B);
    void multiply(float* A, float* B, int m, int p, int n, float* C);
    void add(float* A, float* B, int m, int n, float* C);
    void subtract(float* A, float* B, int m, int n, float* C);
    void transpose(float* A, int m, int n, float* C);
    void scale(float* A, int m, int n, float k);
    float trace(float* A, int m, int n);
    int invert(float* A, int n);
};

extern MatrixMath Matrix;

#endif /* MATRIXMATH_H_ */
