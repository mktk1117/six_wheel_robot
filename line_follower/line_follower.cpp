#include "line_follower.h"

// set the parameters
LineFollower::LineFollower(){
    v_linear_ = param_.v_linear;
    v_angular_max_ = param_.v_angular_max;
    image_to_real_ratio_ = param_.image_to_real_ratio;
    real_to_image_ratio_ = param_.real_to_image_ratio;
}

// calculate the control velocity
void LineFollower::pure_pursuit(Vector2d point, double *linear, double *angular){
    // if the point is in front, just go straight
    if(point.x() == 0){
        *linear = v_linear_;
        *angular = 0;
    }
    // else, calculate the angular velocity to go to the point with circular path
    else{
        // -------------------------------------------------------
        // calculation
        // -------------------------------------------------------
        // r = v_linear / v_angular
        // circle equation
        // (x + r)^2 + y^2 = r^2
        // => r = -(x^2 + y^2) / (2 * x)
        //  => v_angular = v_linear / r
        //               = -2 * x * v_linear / (x^2 + y^2)
        // -------------------------------------------------------
        double angular_temp = (-2 * point.x() * v_linear_) / (point.x() * point.x() + point.y() * point.y());
        if(fabs(angular_temp) < v_angular_max_){
            *linear = v_linear_;
            *angular = angular_temp;
        }
        // if angular velocity is bigger than max angular velocity, reduce the linear velocity
        // angular = angular_max, linear = r * angular_max
        else{
            *angular = v_angular_max_ * (angular_temp / fabs(angular_temp));
            *linear = -(point.x() * point.x() + point.y() * point.y()) / (2 * point.x()) * (*angular);
        }
    }
}


// get control velocity
void LineFollower::get_control_velocity(Mat *src, double r, double *linear, double *angular){
    Vector2d intersect_point, intersect_point_image;
    // transform the radius from real length to image length
    double r_image = r * real_to_image_ratio_;
    // calculate the intersect point of black line and control circle(radius r)
    intersect_point_image = ld_.get_goal_point(src, r_image);
    // transform the point from image lengthto real length 
    intersect_point = image_to_real_ratio_ * intersect_point_image;
    // calculate the control velocity with pure pursuit algorithm
    pure_pursuit(intersect_point, linear, angular);
}
