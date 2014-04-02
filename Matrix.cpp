/*
 *  MatrixMath.cpp Library for MatrixMath Math
 *
 *  Created by Charlie Matlack on 12/18/10.
 *  Modified from code by RobH45345 on Arduino Forums, taken from unknown source.
 *
 *  Code is modified by Sam Abyeruwan to fit to Energia.
 *  http://playground.arduino.cc/Code/MatrixMath#.Uzs2-nWx3t7
 *
 *  MatrixMath.cpp
 *
 *  Created on: Apr 1, 2014
 *      Author: sam
 *
 *
 */

#include "Matrix.h"

MatrixMath Matrix;          // Pre-instantiate

MatrixMath::MatrixMath()
{
}

MatrixMath::~MatrixMath()
{
}

// MatrixMath Printing Routine
// Uses tabs to separate numbers under assumption printed float width won't cause problems
void MatrixMath::print(float* A, int m, int n, MatrixString label)
{
  // A = input matrix (m x n)
  int i, j;
#if defined(ENERGIA)
  Serial.println();
  Serial.println(label);
#else
  std::cout << label << " ";
#endif
  for (i = 0; i < m; i++)
  {
    for (j = 0; j < n; j++)
    {
#if defined(ENERGIA)
      Serial.print(A[n * i + j]);
      Serial.print("\t");
#else
      std::cout << A[n * i + j] << "\t";
#endif
    }
#if defined(ENERGIA)
    Serial.println();
#else
    std::cout << std::endl;
#endif
  }
}

void MatrixMath::copy(float* A, int n, int m, float* B)
{
  int i, j;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++)
    {
      B[n * i + j] = A[n * i + j];
    }
}

//MatrixMath Multiplication Routine
// C = A*B
void MatrixMath::multiply(float* A, float* B, int m, int p, int n, float* C)
{
  // A = input matrix (m x p)
  // B = input matrix (p x n)
  // m = number of rows in A
  // p = number of columns in A = number of rows in B
  // n = number of columns in B
  // C = output matrix = A*B (m x n)
  int i, j, k;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++)
    {
      C[n * i + j] = 0;
      for (k = 0; k < p; k++)
        C[n * i + j] = C[n * i + j] + A[p * i + k] * B[n * k + j];
    }
}

//MatrixMath Addition Routine
void MatrixMath::add(float* A, float* B, int m, int n, float* C)
{
  // A = input matrix (m x n)
  // B = input matrix (m x n)
  // m = number of rows in A = number of rows in B
  // n = number of columns in A = number of columns in B
  // C = output matrix = A+B (m x n)
  int i, j;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++)
      C[n * i + j] = A[n * i + j] + B[n * i + j];
}

//MatrixMath Subtraction Routine
void MatrixMath::subtract(float* A, float* B, int m, int n, float* C)
{
  // A = input matrix (m x n)
  // B = input matrix (m x n)
  // m = number of rows in A = number of rows in B
  // n = number of columns in A = number of columns in B
  // C = output matrix = A-B (m x n)
  int i, j;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++)
      C[n * i + j] = A[n * i + j] - B[n * i + j];
}

//MatrixMath Transpose Routine
void MatrixMath::transpose(float* A, int m, int n, float* C)
{
  // A = input matrix (m x n)
  // m = number of rows in A
  // n = number of columns in A
  // C = output matrix = the transpose of A (n x m)
  int i, j;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++)
      C[m * j + i] = A[n * i + j];
}

void MatrixMath::scale(float* A, int m, int n, float k)
{
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
      A[n * i + j] = A[n * i + j] * k;
}

// The trace of an n-by-n square matrix A is defined to be the sum of the elements on the
// main diagonal.
float MatrixMath::trace(float* A, int m, int n)
{
  if (m != n)
    return 0.0f;
  float tr = 0.0f;
  for (int i = 0; i < m; i++)
    tr += A[n * i + i];
  return tr;
}

//MatrixMath Inversion Routine
// * This function inverts a matrix based on the Gauss Jordan method.
// * Specifically, it uses partial pivoting to improve numeric stability.
// * The algorithm is drawn from those presented in
//   NUMERICAL RECIPES: The Art of Scientific Computing.
// * The function returns 1 on success, 0 on failure.
// * NOTE: The argument is ALSO the result matrix, meaning the input matrix is REPLACED
int MatrixMath::invert(float* A, int n)
{
  // A = input matrix AND result matrix
  // n = number of rows = number of columns in A (n x n)
  int pivrow = 0;   // keeps track of current pivot row
  int k, i, j;    // k: overall index along diagonal; i: row index; j: col index
  int pivrows[n]; // keeps track of rows swaps to undo at end
  float tmp;    // used for finding max value and making column swaps
  //memset(pivrows, 0, sizeof(pivrows));
  for (k = 0; k < n; k++)
  {
    // find pivot row, the row with biggest entry in current column
    tmp = 0;
    for (i = k; i < n; i++)
    {
      if (fabs(A[i * n + k]) >= tmp) // 'Avoid using other functions inside abs()?'
      {
        tmp = fabs(A[i * n + k]);
        pivrow = i;
      }
    }

    // check for singular matrix
    if (A[pivrow * n + k] == 0.0f)
    {
#if defined(ENERGIA)
      Serial.println("Inversion failed due to singular matrix");
#else
      std::cout << "Inversion failed due to singular matrix" << std::endl;
#endif
      return 0;
    }

    // Execute pivot (row swap) if needed
    if (pivrow != k)
    {
      // swap row k with pivrow
      for (j = 0; j < n; j++)
      {
        tmp = A[k * n + j];
        A[k * n + j] = A[pivrow * n + j];
        A[pivrow * n + j] = tmp;
      }
    }
    pivrows[k] = pivrow;  // record row swap (even if no swap happened)

    tmp = 1.0f / A[k * n + k];  // invert pivot element
    A[k * n + k] = 1.0f;    // This element of input matrix becomes result matrix

    // Perform row reduction (divide every element by pivot)
    for (j = 0; j < n; j++)
    {
      A[k * n + j] = A[k * n + j] * tmp;
    }

    // Now eliminate all other entries in this column
    for (i = 0; i < n; i++)
    {
      if (i != k)
      {
        tmp = A[i * n + k];
        A[i * n + k] = 0.0f;  // The other place where in matrix becomes result mat
        for (j = 0; j < n; j++)
        {
          A[i * n + j] = A[i * n + j] - A[k * n + j] * tmp;
        }
      }
    }
  }

  // Done, now need to undo pivot row swaps by doing column swaps in reverse order
  for (k = n - 1; k >= 0; k--)
  {
    if (pivrows[k] != k)
    {
      for (i = 0; i < n; i++)
      {
        tmp = A[i * n + k];
        A[i * n + k] = A[i * n + pivrows[k]];
        A[i * n + pivrows[k]] = tmp;
      }
    }
  }
  return 1;
}

