# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

[//]: # (Image References)

[image1]: ./img/pid-comp.png
[image2]: ./img/v1.png
[image3]: ./img/v2.png

---

*I'm going thru all the rubric points here.*

## Compilation
### Your code should compile.

It does.

![alt text][image1]

## Implementation
### The PID procedure follows what was taught in the lessons.

The PID Controller is implemented in PID.cpp. In `init()` it initializes tau hyperparameters for both proportional, integral, and derivative parts. Then it updates error values in `updateError(const double& cte)`. And lastly it computes the total error in `totalError()`.

## Reflection
### Describe the effect each of the P, I, D components had in your implementation.

I utilized two instances of the PID Controller, namely one for steering and an aother one for throttling. The hyperparameters were different due to the different nature of the problems handled by each controller.

#### The `P` hyperparameter

Since the `P` term is proportional to the current value of the CTE it causes the vehicle to be steered very aggressively: e.g., if the error is large and positive, it steers the vehicle with maximal angle to the left, this causes a massive overshooting when crossing the zero-CTE line (the lane center) and a compensation kicks in causing a yo-yo effect. Keeping this in mind I used a fairly small value of `0.22`. In the PID controller of throttling this value is somewhat larger `0.8` so that in "emergency" situation the model can decelerate more quickly.

#### The `D` hyperparameter

The `D` term is a best estimate of the future trend of the CTE based on its current rate of change. In my setup this parameter has the largest effect on the model output. It helped keeping the trajectory of the vehicle straighter since it effectively reduces the effect of the proportional part by exerting a control influence produced by the rate of error change. The quicker the change, the larger the regulatory or inhibiting effect. In the PID controller of steering this value was bumped up to `10.0`, meanwhile it plays a less essetntial part in the PID controller of throttling - only `0.1`.

#### The `I` hyperparameter

I deliberately left the `I` term last: in both my controllers it play negligable part. Normally it accounts for past values of the CTE and integrates them over time. But as discussed in the videos it is needed when the control "baseline" is off by a constant or a slowly changing value. Simply put it would eliminate the residual error. In our case this is not really significant.

### Describe how the final hyperparameters were chosen.

When choosing the final hyperparameters I considered applying `twiddling` but due to the difficulity of automating test result evaluation I threw out the idea.

Instead I played with them around while keeping in mind the effects detailed in the section above. To make the process easier first I set all three parameters to 0 and only antered on of them at a time - just like twiddling would do.

Some other features of my code:

* The total error produced by the throttling PID controller is taken as an absolute value using `fabs()` since it doesent matter whether the car is on the far right or on the far left side of the road - in both cases action has to be taken fairly effectively.
* The total error produced by the steering PID controller is limited to the `[-0.56, +0.65]` range in order to prevent oversteering.
* The final `steer_value` is calculated by weighted average of the previos steering angle and the total error - weight of the total error is proportional to the actual CTE.
* The final trottle is the "inverse" of the total error (i.e. `0.9 - err2`)

## Simulation
### The vehicle must successfully drive a lap around the track.

I run several tests in the simulator. I recorded two full laps and uploaded the results to Youtube:

[![alt text][image2]](https://youtu.be/mgS2_w3o0k8)

[![alt text][image3]](https://youtu.be/xdTi8_vWC9g)

When recording the second video the CPU load was much higher due to the greater capture area and the encodning of the debug printouts.
This resulted in a longer video, the car speed was noticably lower - however exactly the same binary was used.
