#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

// Please note that the Eigen library does not initialize 
// VectorXd or MatrixXd objects with zeros upon creation.

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
    x_ = F_ * x_;
    MatrixXd Ft = F_.transpose();
    P_ = F_ * P_ * Ft + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
    VectorXd z_pred = H_ * x_;
    VectorXd y = z - z_pred;
    update(y);
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
    * update the state by using Extended Kalman Filter equations
  */
    
    float px = x_[0];
    float py = x_[1];
    float vx = x_[2];
    float vy = x_[3];
    if(fabs(px) < 0.0001) px = 0.0001;
    float phi = atan2(py, px);
    float r = sqrt(px * px + py * py);
    if(fabs(r) < 0.0001) r = 0.0001;
    float r1 = (px * vx + py * vy) / r;
    VectorXd hx = VectorXd(3);
    hx << r, phi, r1;
    VectorXd y = z - hx;
    while ( y(1) > M_PI || y(1) < -M_PI ) {
        if ( y(1) > M_PI ) {
          y(1) -= M_PI;
        } else {
          y(1) += M_PI;
        }
      }
    update(y);
}

void KalmanFilter::update(const VectorXd &y){
	 MatrixXd Ht = H_.transpose();
	    MatrixXd S = H_ * P_ * Ht + R_;
	    MatrixXd K = P_ * Ht * S.inverse();
	    x_ = x_ + (K * y);
	    long x_size = x_.size();
	    MatrixXd I = MatrixXd::Identity(x_size, x_size);
	    P_ = (I - K * H_) * P_;
}

