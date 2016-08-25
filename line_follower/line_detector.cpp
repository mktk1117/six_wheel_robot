#include <opencv2/opencv.hpp>
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry> 
#include "line_detector.h"
#include <time.h>
#include <sys/time.h>

#define print(var)  \
      std::cout<<#var"= "<<std::endl<<(var)<<std::endl

using namespace std;
using namespace cv;
using namespace Eigen;

LineDetector::LineDetector(){
    skip_step_ = 2;
    horizon_th_ = 30;
    black_th_ = 130;
    // homography_matrix_ << 0.0, -13.3, 3200.0,
    //                      1.5, -3.0, -240.0,
    //                      0, -0.0125, 1;
    // homography_matrix_ << -7.320533068622126e-16, -13.33333333333329, 3199.99999999999,
    //                        1.499999999999994, -2.999999999999994, -239.9999999999988,
    //                        -1.219727444046192e-18, -0.01249999999999997, 1;
    homography_matrix_ << -1.609823385706477e-15, 6.666666666666675, 2.842170943040401e-14,
                           -0.7500000000000022, 1.500000000000004, 480.0000000000001,
                           -2.005774019098183e-18, 0.006250000000000012, 1;



}

void LineDetector::extract_black(Mat* src, Mat* dst, double th_g){
    // loop for each pixel
    // skip some pixels to calculate fast
    for(int y = 0; y < src->rows; y=y+skip_step_){
        for(int x = 0; x < src->cols; x=x+skip_step_){
                    double r = src->data[ y * src->step + x * src->elemSize()];
                    double g = src->data[ y * src->step + x * src->elemSize() + 1];
                    double b = src->data[ y * src->step + x * src->elemSize() + 2];

                    // calculate the distance between R, B and G
                    double distance = sqrt((r - g) * (r - g) + (r - b) * (r - b) + (g - b) * (g - b));

                    // black_ratio = (1 - distance / 411)^4
                    double black_ratio = 1 - (distance / 411);
                    if(black_ratio < 0){
                        black_ratio = 0;
                    }
                    black_ratio = black_ratio * black_ratio;
                    black_ratio = black_ratio * black_ratio;

                    // grayscale 
                    int gray = 0.3 * r + 0.59 * g + 0.11 * b;
                    int p = (255 - gray) * black_ratio;

                    // binarize
                    if(p > th_g){
                        dst->data[ y / skip_step_ * dst->step + x / skip_step_ * dst->elemSize()] = 255;
                    }
            }
    }
    return;
}

void LineDetector::calc_variance(Mat *src, vector<int> *variance, int th, int direction){
    // direction 0 -> x axis, 1 -> y axis 
    if(direction == 0){
        // loop for each pixel
        // skip some pixels to calculate fast
        for(int y = 0; y < src->rows; y=y+skip_step_){
            int mx = 0;
            int mx2 = 0;
            int n = 0;
            // sum for x axis
            for(int x = 0; x < src->cols; x=x+skip_step_){
                double p = src->data[ y * src->step + x * src->elemSize()];
                if(p > th){
                    mx += x;
                    mx2 += x * x;
                }
                n++;
            }
            // calculate variance 
            // V = E(X^2) - E(X)^2
            int v = mx2 / n - (mx / n) * (mx / n);
            variance->push_back(v);
        }
    }
    else{
        for(int x = 0; x < src->cols; x=x+skip_step_){
            int my = 0;
            int my2 = 0;
            int n = 0;
            // sum for y axis
            for(int y = 0; y < src->rows; y=y+skip_step_){
                double p = src->data[ y * src->step + x * src->elemSize()];
                if(p > th){
                    my += y;
                    my2 += y * y;
                }
                n++;
            }
            // calculate variance 
            // V = E(X^2) - E(X)^2
            int v = my2 / n - (my / n) * (my / n);
            variance->push_back(v);
        }
    }
}

// detect the horizon 
int LineDetector::detect_horizon(Mat *filtered){

    // calculate the variance
    vector<int> variance;
    calc_variance(filtered, &variance, 100, 0);

    int y_border = 0;
    // if the variance > threshold it is horizon
    for(int i = variance.size(); i > 0; i--){
        if(variance[i] > horizon_th_){
            y_border = i;
            break;
        }
    }
    return y_border;
}

// get the sobel filtered image
void LineDetector::get_filtered(Mat *src, Mat *filtered){
    Mat black_image = Mat::zeros(Size(src->cols / skip_step_ + 1, src->rows / skip_step_ + 1), CV_8U);

    extract_black(src, &black_image, black_th_);
    Mat filteredx = black_image.clone();
    Sobel(black_image, filteredx, CV_8U, 1, 0);
    *filtered = filteredx;
}

Vector2d LineDetector::transform_point_homography(Vector2d point){
    Vector3d p(point.x(), point.y(), 1);
    Vector3d t = homography_matrix_ * p;
    Vector2d result(t.x() / t.z(), t.y() / t.z());
    return result;
}

bool LineDetector::isCrossed(Vector2d p1, Vector2d p2, double r){
    Vector2d a = -p1;
    Vector2d b = -p2;
    Vector2d s = b - a;
    if(p1.norm() <= r || p2.norm() <= r){
        return true;
    }
    else if(((a.x() * s.y() - a.y() * s.x()) / s.norm() <= r) && (a.dot(s) * b.dot(s) < 0)){
        return true;
    }
    else{
        return false;
    }
}

vector<Vector2d> LineDetector::calc_intersection(Vector2d p1, Vector2d p2, double r){
    vector<Vector2d> result;
    if(p2.x() - p1.x() == 0){
        result.push_back(Vector2d(p1.x(), sqrt(r * r - p1.x() * p1.x())));
    }
    else{
        double a = (p2.y() - p1.y()) / (p2.x() - p1.x());
        double b = p1.y() - a * p1.x();
        double sq = sqrt(a * a * b * b - (1 + a * a) * (b * b - r * r));
        double interx1 = (-a * b + sq) / (1 + a * a);
        double interx2 = (-a * b - sq) / (1 + a * a);
        double x0, x1;
        if(p1.x() < p2.x()){
            x0 = p1.x();
            x1 = p2.x();
        }
        else{
            x0 = p2.x();
            x1 = p1.x();
        }
        if(interx1 >= x0 && interx1 <= x1){
            result.push_back(Vector2d(interx1, a * interx1 + b));
        }
        if(interx2 >= x0 && interx2 <= x1){
            result.push_back(Vector2d(interx2, a * interx2 + b));
        }
    }
    return result;
}


Vector2d LineDetector::get_goal_point(Mat *src, double r){
    Mat filtered;
    int rows = src->cols;
    int cols = src->rows;
    get_filtered(src, &filtered);
    int y_border = detect_horizon(&filtered);
    Mat cutted(filtered, Rect(0, y_border, filtered.cols, (filtered.rows - y_border)));
    vector<Vec4i> lines;
    HoughLinesP(cutted, lines, 1, CV_PI/180, 80, 30, 8 );
    vector<Vector2d> intersect_points;
    for(unsigned int i = 0; i < lines.size(); i++){
        Vec4i l = lines[i];
        Vector2d p1(l[0] * 2, l[1] * 2);
        Vector2d p2(l[2] * 2, l[3] * 2);
        Vector2d pstart = transform_point_homography(p1);
        Vector2d pend = transform_point_homography(p2);
        Vector2d l_start(cols / 2. - pstart.y(), rows - pstart.x());
        Vector2d l_end(cols / 2. - pend.y(), rows - pend.x());
        if(isCrossed(l_start, l_end, r)){
            vector<Vector2d> intersect = calc_intersection(l_start, l_end, r);
            for(unsigned int j = 0; j < intersect.size(); j++){
                intersect_points.push_back(intersect[j]);
            }
        }
    }
    int center_num = 0;
    int min_d = 10000;
    for(unsigned int i = 0; i < intersect_points.size(); i++){
        int d = intersect_points[i].x();
        if(fabs(d) < min_d){
            min_d = d;
            center_num = i;
        }
    }
    return intersect_points[center_num];
}




