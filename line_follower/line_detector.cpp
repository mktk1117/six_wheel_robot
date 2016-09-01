#include <opencv2/opencv.hpp>
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry> 
#include "line_detector.h"
#include <time.h>
#include <sys/time.h>
#include "parameter.h"

#define print(var)  \
      std::cout<<#var"= "<<std::endl<<(var)<<std::endl

using namespace std;
using namespace cv;
using namespace Eigen;

LineDetector::LineDetector(){
    skip_step_ = param_.skip_step;
    horizon_th_ = param_.horizon_th;
    black_th_ = param_.black_th;
    homography_matrix_ = param_.homography_matrix;
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

    // first, get the black image
    extract_black(src, &black_image, black_th_);
    Mat filteredx = black_image.clone();
    // apply sobel filter
    Sobel(black_image, filteredx, CV_8U, 1, 0);
    // Laplacian(black_image, filteredx, CV_8U, 3);
    // Canny(black_image, filteredx, 50, 200);
    *filtered = filteredx;
}

// transform the point with homography matrix
Vector2d LineDetector::transform_point_homography(Vector2d point){
    Vector3d p(point.x(), point.y(), 1);
    Vector3d t = homography_matrix_ * p;
    Vector2d result(t.x() / t.z(), t.y() / t.z());
    return result;
}

// detect whether the line from p1 to p2 is crossed with the circle(center(0, 0), radius r)
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

// calculate the intersection point with line from point p1 to p2 and the circle(center (0, 0), radius r)
vector<Vector2d> LineDetector::calc_intersection(Vector2d p1, Vector2d p2, double r){
    vector<Vector2d> result;
    // if x is the same, intersection point's x = x, y = sqrt(r^2 - x^2)
    if(p2.x() - p1.x() == 0){
        result.push_back(Vector2d(p1.x(), sqrt(r * r - p1.x() * p1.x())));
    }
    else{
        //-------------------------------------------------------------------
        //calculation
        //line equation 
        // y = ax + b ----(1)
        // a = (y2 - y1) / (x2 - x1)
        // b = y1 - a * x1
        // circle equation
        // x^2 + y^2 = r^2 ----(2)
        // solve these two equataion
        // x = (-ab +- sqrt(a^2b^2 - (1 + a^2)(b^2 - r^2))) / (1 + a^2)
        // y = ax + b
        //-------------------------------------------------------------------
        double a = (p2.y() - p1.y()) / (p2.x() - p1.x());
        double b = p1.y() - a * p1.x();
        double sq = sqrt(a * a * b * b - (1 + a * a) * (b * b - r * r));
        double interx1 = (-a * b + sq) / (1 + a * a);
        double interx2 = (-a * b - sq) / (1 + a * a);

        // if the calculated point is really intersection points, x should be
        //  x0 <= x <= x1

        // smaller point should be x0
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


// get goal point for control from an original image
Vector2d LineDetector::get_goal_point(Mat *src, double r){
    // first filter the image
    Mat filtered;
    int rows = src->cols;
    int cols = src->rows;
    get_filtered(src, &filtered);

    // then get the horizon line
    int y_border = detect_horizon(&filtered);

    // cut the image at the horizon
    Mat cutted(filtered, Rect(0, y_border, filtered.cols, (filtered.rows - y_border)));

    // get lines by Hough transformation
    vector<Vec4i> lines;
    HoughLinesP(cutted, lines, 1, CV_PI/180, 80, 30, 8 );

    // for each lines, calculate the intersection points
    // if there is a intersection point, add to the vector
    vector<Vector2d> intersect_points;
    for(unsigned int i = 0; i < lines.size(); i++){
        Vec4i l = lines[i];
        Vector2d p1(l[0] * skip_step_, l[1] * skip_step_);
        Vector2d p2(l[2] * skip_step_, l[3] * skip_step_);
        // transform the point to the floor coordinate
        Vector2d pstart = transform_point_homography(p1);
        Vector2d pend = transform_point_homography(p2);
        // calculate the line point to the coordinate where center is x=0, bottom is y=0
        Vector2d l_start(cols / 2. - pstart.y(), rows - pstart.x());
        Vector2d l_end(cols / 2. - pend.y(), rows - pend.x());
        if(isCrossed(l_start, l_end, r)){
            vector<Vector2d> intersect = calc_intersection(l_start, l_end, r);
            for(unsigned int j = 0; j < intersect.size(); j++){
                intersect_points.push_back(intersect[j]);
            }
        }
    }
    // then, use the point that is nearest to the center
    int center_num = 0;
    int min_d = 10000;
    for(unsigned int i = 0; i < intersect_points.size(); i++){
        int d = intersect_points[i].x();
        if(fabs(d) < min_d){
            min_d = d;
            center_num = i;
        }
    }
    if(intersect_points.size() > 0){
        return intersect_points[center_num];
    }
    // if there is no intersection point, return (0, 0)
    else{
        return Vector2d(0, 0);
    }
}




