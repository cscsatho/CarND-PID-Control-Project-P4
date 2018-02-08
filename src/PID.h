#ifndef PID_H
#define PID_H

class PID
{
public:
  PID() = default;
  virtual ~PID() = default;

  // Initializing PID
  void init(double Kp, double Ki, double Kd);
  // Updating the PID error variables given cross track error
  void updateError(const double& cte);
  // Calculating the total PID error
  double totalError() const;

private:
  // Errors
  double p_error_;
  double i_error_;
  double d_error_;

  // Coefficients
  double kp_;
  double ki_;
  double kd_;
};

#endif /* PID_H */
