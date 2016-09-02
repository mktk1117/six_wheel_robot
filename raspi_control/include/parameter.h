#ifndef  PARAMETER_
#define  PARAMETER_

#include <Eigen/Core>

using namespace Eigen;

class Parameter{
public:
    // how many pixels to skip in extract_rgb
    int skip_step;
    // threshold to detect horizon line
    int horizon_th;
    // threshold to binarization
    int black_th;
    // matrix used at homography transformation
    Matrix3d homography_matrix;

    // radius iteration to find intersection point
    int r_itr;

    // parameters for line following
    // linear velocity
    double v_linear;
    // max angular velocity
    double v_angular_max;

    double linear_v_ratio;
    double angular_v_ratio;
    double left_right_ratio;

    // parameter for transforming between real length and image length
    double image_to_real_ratio;
    double real_to_image_ratio;

    Parameter();
};

#endif 
