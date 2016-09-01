#include "parameter.h"

Parameter::Parameter(){
    // parameters for line detection
    skip_step = 2;
    horizon_th = 30;
    black_th = 140;
    homography_matrix << -4.440892098500626e-16, 22.22222817189583, 2.842170943040401e-14,
      -0.7500000000000013, 7.333335284009966, 479.9999999999999,
        -1.951563910473908e-18, 0.03055556485192055, 1;


    // radius iteration to find intersection point
    r_itr = 10;

    // parameters for line following
    v_linear = 1;
    v_angular_max = 1;

    // parameter for transforming between real length and image length
    image_to_real_ratio = 1 / 100.;
    real_to_image_ratio = 100;
}
