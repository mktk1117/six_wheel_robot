#ifndef  BOUNDARY_
#define  BOUNDARY_

#include <opencv2/opencv.hpp>
#include <iostream>
#include <Eigen/Core>
#include "parameter.h"

using namespace Eigen;
using namespace cv;

class LineDetector{
public:
    // how many pixels to skip in extract_rgb
    int skip_step_;
    int horizon_th_;
    int black_th_;
    Matrix3d homography_matrix_;

    Parameter param_;


    LineDetector();

    // extract black or gray from rgb image
    void extract_black(Mat* src, Mat* dst, double th_g);

    // calculate variance of image
    // direction 0 -> x axis, 1 -> y axis 
    void calc_variance(Mat *src, vector<int> *variance, int th, int direction);

    // detect the horizon 
    int detect_horizon(Mat *filtered);

    // get the sobel filtered image
    void get_filtered(Mat *src, Mat *filtered);

    // get transformed point with homography matrix
    Vector2d transform_point_homography(Vector2d point);

    // detect the line and the circle is crossed or not
    bool isCrossed(Vector2d p1, Vector2d p2, double r);

    // calculate intersection points with line and circle
    vector<Vector2d> calc_intersection(Vector2d p1, Vector2d p2, double r);

    // calculate the goal point to control
    Vector2d get_goal_point(Mat *src, double r);

};

#endif 
