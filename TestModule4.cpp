/*
 * TestModule4.cpp
 *
 *  Created on: Apr 1, 2014
 *      Author: sam
 */

#include "TestModule4.h"
#include "Matrix.h"

#include <stdlib.h>

MAKE_MODULE(TestModule4)

#define N  (2)

float A[N][N];
float B[N][N];
float C[N][N];
float v[N];      // This is a row vector
float w[N];

int max = 10;  // maximum random matrix entry range

TestModule4::TestModule4() :
    random(0)
{
}

TestModule4::~TestModule4()
{
  if (random)
    delete random;
}

void TestModule4::init()
{
  // Initialize matrices
  random = new RLLib::Random<float>;
  for (int i = 0; i < N; i++)
  {
    v[i] = i + 1;                    // vector of sequential numbers
    for (int j = 0; j < N; j++)
    {
      A[i][j] = (float) random->nextInt(max) - ((float) max) / 2.0f;  // A is random
      if (i == j)
      {
        B[i][j] = 1.0f;                  // B is identity
      }
      else
      {
        B[i][j] = 0.0f;
      }
    }
  }
}

void TestModule4::execute()
{
  Matrix.multiply((float*) A, (float*) B, N, N, N, (float*) C);

  //std::cout << "\nAfter multiplying C = A*B:" << std::endl;
  //Matrix.print((float*) A, N, N, "A");
  //std::cout << "\nTrace(C): " << Matrix.trace((float*) A, N, N) << std::endl;

  //Matrix.print((float*) B, N, N, "B");
  //Matrix.print((float*) C, N, N, "C");
  //Matrix.print((float*) v, N, 1, "v");

  Matrix.add((float*) B, (float*) C, N, N, (float*) C);
  //std::cout << "\nC = B+C (addition in-place)" << std::endl;
  //Matrix.print((float*) C, N, N, "C");
  //Matrix.print((float*) B, N, N, "B");

  Matrix.copy((float*) A, N, N, (float*) B);
  //std::cout << "\nCopied A to B:" << std::endl;
  //Matrix.print((float*) B, N, N, "B");

  Matrix.invert((float*) A, N);
  //std::cout << "\nInverted A:" << std::endl;
  //Matrix.print((float*) A, N, N, "A");

  Matrix.multiply((float*) A, (float*) B, N, N, N, (float*) C);
  //std::cout << "\nC = A*B" << std::endl;
  //Matrix.print((float*) C, N, N, "C");

  // Because the library uses pointers and DIY indexing,
  // a 1D vector can be smoothly handled as either a row or col vector
  // depending on the dimensions we specify when calling a function
  Matrix.multiply((float*) C, (float*) v, N, N, 1, (float*) w);
  //std::cout << "\n C*v = w:" << std::endl;
  //Matrix.print((float*) v, N, 1, "v");
  //Matrix.print((float*) w, N, 1, "w");
  //std::cout << "[] [] []" << std::endl;
}
