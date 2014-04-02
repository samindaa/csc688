/*
 * KalmanFiltering.h
 *
 *  Created on: Apr 1, 2014
 *      Author: sam
 */

#ifndef KALMANFILTERING_H_
#define KALMANFILTERING_H_

#include <string.h>
#include "Matrix.h"
/**
 * http://www.cs.unc.edu/~welch/kalman/kalmanIntro.html
 *
 * See also:
 * http://en.wikipedia.org/wiki/Kalman_filter
 *
 * TODO: Not implemented yet
 *
 */
class KalmanFiltering
{
  public:
    class KFBase
    {
      protected:
        int rowsM;
        int colsN;
        float* dataKF;

      public:
        KFBase(const int& rows, const int& cols) :
            rowsM(rows), colsN(cols), dataKF(new float[rowsM * colsN])
        {
          memset(dataKF, 0, sizeof(float) * rowsM * colsN);
        }

        ~KFBase()
        {
          delete[] dataKF;
        }

        virtual float* data() const
        {
          return dataKF;
        }

        virtual int rows() const
        {
          return rowsM;
        }

        virtual int cols() const
        {
          return colsN;
        }

        virtual void set(const float& value, const int& i, const int& j = 1)
        {
          dataKF[colsN * i + j] = value;
        }

        virtual float get(const int& i, const int& j = 1) const
        {
          return dataKF[colsN * i + j];
        }
    };

    class KFMatrix: public KFBase
    {
      public:
        KFMatrix(const int& m, const int& n) :
            KFBase(m, n)
        {
        }
        virtual ~KFMatrix()
        {
        }
    };

    class KFVector: public KFBase
    {
      public:
        KFVector(const int& m) :
            KFBase(m, 1)
        {
        }
        virtual ~KFVector()
        {
        }
    };

  protected:
    KFMatrix* A;        // n x n
    KFMatrix* B;        // n * l
    KFMatrix* H;        // m * n

    KFMatrix* Q;        // n * n
    KFMatrix* R;        // m * m

    KFMatrix* Pk;       // n * n
    KFMatrix* Pk_;      // n * n
    KFMatrix* Kk;       // n * m

    KFVector* uk;       // l * 1
    KFVector* zk;       // m * 1
    KFVector* xk_hat;   // n * 1
    KFVector* xk_hat_;  // n * 1
    KFVector* xk_hat1_;
    KFVector* xk_hat2_;

  public:
    KalmanFiltering(const int& x_n, const int& z_m, const int& u_l = 1);
    ~KalmanFiltering();

    void predict();
    void correct();

    KFVector* getUk() const;
    KFVector* getZk() const;

    KFMatrix* getQ() const;
    KFMatrix* getR() const;
    KFMatrix* getP() const;

    KFVector* getMu() const;
    KFMatrix* getSigma() const;
};

#endif /* KALMANFILTERING_H_ */
