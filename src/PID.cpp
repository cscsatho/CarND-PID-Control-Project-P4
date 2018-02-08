#include "PID.h"

//using namespace std;

void PID::init(double kp, double ki, double kd)
{
  kp_ = kp;
  ki_ = ki;
  kd_ = kd;

  i_error_ = 0.0;
  p_error_ = 0.0;
  d_error_ = 0.0;
}

void PID::updateError(const double& cte)
{
  i_error_ += cte;
  d_error_ = cte - p_error_;
  p_error_ = cte;
}

double PID::totalError() const
{
  return kp_ * p_error_ + kd_ * d_error_ + ki_ * i_error_;
}
