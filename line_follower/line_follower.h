#ifndef  LINEFOLLOWER_
#define  LINEFOLLOWER_

#include <opencv2/opencv.hpp>
#include <iostream>
#include <Eigen/Core>
#include "parameter.h"
#include "line_detector.h"

using namespace Eigen;
using namespace cv;

class LineFollower{
public:

    // constant value for linear velocity
    double v_linear_;
    // max angular velocity
    double v_angular_max_;

    // parameter for transforming between real length and image length
    double image_to_real_ratio_;
    double real_to_image_ratio_;

    Parameter param_;
    LineDetector ld_;
    LineFollower();

    // calculate control velocity with pure pursuit
    void pure_pursuit(Vector2d point, double *linear, double *angular);

    // get control velocity
    void get_control_velocity(Mat *src, double r, double *linear, double *angular);


};

#endif 
