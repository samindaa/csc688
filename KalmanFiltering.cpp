/*
 * KalmanFiltering.cpp
 *
 *  Created on: Apr 1, 2014
 *      Author: sam
 */

#include "KalmanFiltering.h"

KalmanFiltering::KalmanFiltering(const int& x_n, const int& z_m, const int& u_l) :
    A(new KFMatrix(x_n, x_n)), B(new KFMatrix(x_n, u_l)), H(new KFMatrix(z_m, x_n)), Q(
        new KFMatrix(x_n, x_n)), R(new KFMatrix(z_m, z_m)), Pk(new KFMatrix(x_n, x_n)), Pk_(
        new KFMatrix(x_n, x_n)), Kk(new KFMatrix(x_n, z_m)), uk(new KFVector(u_l)), zk(
        new KFVector(z_m)), xk_hat(new KFVector(x_n)), xk_hat_(new KFVector(x_n)), xk_hat1_(
        new KFVector(x_n)), xk_hat2_(new KFVector(x_n))
{
}

KalmanFiltering::~KalmanFiltering()
{
  delete A;
  delete B;
  delete H;
  delete Q;
  delete R;
  delete Pk;
  delete Pk_;
  delete Kk;
  delete uk;
  delete zk;
  delete xk_hat;
  delete xk_hat_;
  delete xk_hat1_;
  delete xk_hat2_;
}

void KalmanFiltering::predict()
{
  // (1) Project the state ahead
  Matrix.multiply(A->data(), xk_hat->data(), A->rows(), A->cols(), xk_hat->cols(), xk_hat1_->data());
  Matrix.multiply(B->data(), uk->data(), B->rows(), B->cols(), uk->cols(), xk_hat2_->data());
  Matrix.add(xk_hat1_->data(), xk_hat2_->data(), xk_hat_->rows(), xk_hat_->cols(), xk_hat_->data());

  // (2) Project the error covariance ahead
  // TODO
}

void KalmanFiltering::correct()
{
  // TODO:
  // (1) Compute the Kalman gain
  // (2) Update estimate with measurement zk
  // (3) Update the error covariance
}

KalmanFiltering::KFVector* KalmanFiltering::getUk() const
{
  return uk;
}

KalmanFiltering::KFVector* KalmanFiltering::getZk() const
{
  return zk;
}

KalmanFiltering::KFMatrix* KalmanFiltering::getQ() const
{
  return Q;
}

KalmanFiltering::KFMatrix* KalmanFiltering::getR() const
{
  return R;
}

KalmanFiltering::KFMatrix* KalmanFiltering::getP() const
{
  return Pk;
}

KalmanFiltering::KFVector* KalmanFiltering::getMu() const
{
  return xk_hat;
}

KalmanFiltering::KFMatrix* KalmanFiltering::getSigma() const
{
  return Pk;
}

