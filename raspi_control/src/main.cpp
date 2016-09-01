#include <opencv2/opencv.hpp>
#include <iostream>
#include <Eigen/Core>
#include <sstream>
#include <string>
#include "line_follower.h"
#include "serial_lib.h"
#include "parameter.h"
#include <raspicam/raspicam_cv.h>
// #include "SioClientWrapper.h"
// #include "Parser.h"
// #include "DataMaker.h"

#define print(var)  \
      std::cout<<#var"= "<<std::endl<<(var)<<std::endl

using namespace std;
using namespace cv;
using namespace Eigen;

int main(int argc, char *argv[]){
    Serial ser;
    ser.init("/dev/ttyUSB0", 115200);
    usleep(1000 * 1000);
    raspicam::RaspiCam_Cv Camera;
    cv::Mat src_image;
    cv::Mat image;
    // parameters
    Parameter param;
    int nCount=100;
    //set camera params
    Camera.set(CV_CAP_PROP_FORMAT, CV_8UC3);
    Camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    Camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    Camera.set(CV_CAP_PROP_FPS, 60);
    //Open camera
    cout<<"Opening Camera..."<<endl;
    if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
    //Start capture
    for ( int i=0; i<nCount; i++ ) {
        // get image from raspi camera module
        Camera.grab();
        Camera.retrieve (src_image);
        // flip to adjust the direction
        flip(src_image, image, -1);

        // line following controller
        LineFollower lf;
        double linear, angular;
        // look ahead point radius for pure pusuit
        double r = 2;
        lf.get_control_velocity(&image, r, &linear, &angular);
        print(linear); print(angular);

        // make control command
        char serial_str[256];
        int right = linear * param.linear_v_ratio + angular * param.angular_v_ratio;
        int left = linear * param.linear_v_ratio - angular * param.angular_v_ratio;
        left *= param.left_right_ratio;
        sprintf(serial_str, "%d,%d,%d,%d,%d,%d*\n", right, left, right, left, right, left);
        // send to Arduino
        ser.serial_write(serial_str);

        // serial read
        char read_buf[256];
        ser.serial_read(read_buf);
        cout << read_buf << endl;

        // save image
        char filename[32];
        sprintf(filename, "image_%03d.jpg", i);
        cv::imwrite(filename, image);
    }
}
