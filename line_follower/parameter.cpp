#include "parameter.h"

Parameter::Parameter(){
    // parameters for line detection
    skip_step = 2;
    horizon_th = 30;
    black_th = 130;
    homography_matrix << -1.61e-15, 6.67, 2.84e-14,
                           -0.75, 1.5, 480,
                           -2.0e-18, 0.00625, 1;

    // parameters for line following
    v_linear = 1;
    v_angular_max = 1;

    // parameter for transforming between real length and image length
    image_to_real_ratio = 1 / 100.;
    real_to_image_ratio = 100;
}
