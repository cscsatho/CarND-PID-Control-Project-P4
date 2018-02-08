# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

[//]: # (Image References)

[image1]: ./img/pid-comp.png

---

I'm going thru all the rubric points here.


## Compilation
### Your code should compile.

It does.

![alt text][image1]

## Implementation
### The PID procedure follows what was taught in the lessons.

The PID Controller is implemented in PID.cpp. In `init()` it initializes tau hyperparameters for both proportional, integral, and derivative parts. Then it updates error values in `updateError(const double& cte)`. And lastly it computes the total error in `totalError()`.

## Reflection
### Describe the effect each of the P, I, D components had in your implementation.

### Describe how the final hyperparameters were chosen.

## Simulation
### The vehicle must successfully drive a lap around the track.

I run several tests in the simulator. I recorded two full laps and uploaded the results to Youtube:

[CarND T2P4 - PID Controller test](https://youtu.be/mgS2_w3o0k8)

[CarND T2P4 - PID Controller test w/ debug](https://youtu.be/xdTi8_vWC9g)

When recording the second video the CPU load was much higher due to the greater capture area and the encodning of the debug printouts.
This resulted in a longer video, the car speed was noticably lower - however exactly the same binary was used.

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).

