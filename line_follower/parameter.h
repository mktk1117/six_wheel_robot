#ifndef  PARAMETER_
#define  PARAMETER_

#include <Eigen/Core>

using namespace Eigen;

class Parameter{
public:
    // how many pixels to skip in extract_rgb
    int skip_step_;
    // threshold to detect horizon line
    int horizon_th_;
    // threshold to binarization
    int black_th_;
    // matrix used at homography transformation
    Matrix3d homography_matrix_;

    // parameters for line following
    // linear velocity
    double v_linear;
    // max angular velocity
    double v_angular_max;

    // parameter for transforming between real length and image length
    double image_to_real_ratio;
    double real_to_image_ratio;

    Parameter();
};

#endif 
