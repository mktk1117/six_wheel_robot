#include <opencv2/opencv.hpp>
#include <iostream>
#include <Eigen/Core>
#include <sstream>
#include <string>
#include "line_detector.h"
#define print(var)  \
      std::cout<<#var"= "<<std::endl<<(var)<<std::endl

using namespace std;
using namespace cv;
using namespace Eigen;

int main(int argc, char *argv[]){
    //カメラの設定
    VideoCapture cap(0);
    if(!cap.isOpened()){
        cout << "Camera not Found!" << endl;
        return -1;
    }

    //画像を入れるMatの宣言
    Mat org;    //カメラ画像
    // Mat dst = Mat::zeros(Size(org.cols, org.rows), CV_8U);

    LineDetector ld;

    org = imread(argv[1]);

    Vector2d intersect_point;
    double r = 250;
    intersect_point = ld.get_goal_point(&org, r);

    Mat dst(org.cols, org.rows, CV_8U, Scalar(0, 0, 0));

    Mat filtered, colordst;
    ld.get_filtered(&org, &filtered);
    int y_border = ld.detect_horizon(&filtered);

    cvtColor(filtered, colordst, CV_GRAY2RGB);
    // line(org, Point(0, y_border * 2), Point((int)org.cols, y_border * 2), Scalar(0,0,255), 3, 8);
    line(colordst, Point(0, y_border), Point((int)colordst.cols, y_border), Scalar(0,0,255), 3, 8);

    Mat cutted(filtered, Rect(0, y_border, filtered.cols, (filtered.rows - y_border)));

    vector<Vec4i> lines;
    HoughLinesP(cutted, lines, 1, CV_PI/180, 80, 30, 8 );
    for(int i = 0; i < lines.size(); i++){
        Vec4i l = lines[i];
        double dx = l[2] - l[0];
        double dy = l[3] - l[1];
        double d = sqrt(dx * dx + dy * dy);
        if(d > 0){
            line(org, Point(l[0] * 2, (l[1] + y_border) * 2), Point(l[2] * 2, (l[3] + y_border) * 2), Scalar(255,0,0), 3, 8);
            line(colordst, Point(l[0], l[1] + y_border), Point(l[2], l[3] + y_border), Scalar(255,0,0), 3, 8);
        }
    }
    Vec4i line = lines[0];
    Vector2d p1(line[0] * 2, line[1] * 2);
    Vector2d p2(line[2] * 2, line[3] * 2);

    Mat cutted_org(org, Rect(0, y_border * ld.skip_step_, org.cols, (org.rows - y_border * ld.skip_step_)));


    // stringstream filename;
    // imwrite("org.jpg", org);
    // imwrite("black.jpg", black_image);
    // imwrite("filteredx.jpg", filteredx);
    // imwrite("filteredy.jpg", filteredy);
    // imwrite("colordst1.jpg", colordst);
    // cout << "saved image" << endl;
    // waitKey(0);
    const int    DST_WIDTH = 640;
    const int    DST_HEIGHT = 480;
    //
    // const cv::Point2f src_pt[]={
    //             cv::Point2f(0, y_border * ld.skip_step_),
    //             cv::Point2f(org.cols , y_border * ld.skip_step_),
    //             cv::Point2f(org.cols , org.rows),
    //             cv::Point2f(0, org.rows)};
    // const cv::Point2f dst_pt[]={
    //             cv::Point2f(0, org.rows),
    //             cv::Point2f(0.0, 0.0),
    //             cv::Point2f(org.cols / 1, 1.5 * org.rows / 5),
    //             cv::Point2f(org.cols / 1, 3.5 * org.rows / 5 )};

    const cv::Point2f src_pt[]={
                cv::Point2f(0, 0),
                cv::Point2f(cutted_org.cols , 0),
                cv::Point2f(cutted_org.cols , cutted_org.rows),
                cv::Point2f(0, cutted_org.rows)};
    const cv::Point2f dst_pt[]={
                cv::Point2f(0, org.rows),
                cv::Point2f(0.0, 0.0),
                cv::Point2f(org.cols / 1, 1.5 * org.rows / 5),
                cv::Point2f(org.cols / 1, 3.5 * org.rows / 5 )};
    
    const cv::Mat homography_matrix = cv::getPerspectiveTransform(src_pt,dst_pt);
    // double theta = 10 * 3.14 / 180;
    // double f = 100;
    // double h = 0.6;
    //
    // const cv::Mat homography_matrix = (cv::Mat_<double>(3,3) << h, 0, 0, 0, 0, h * f / cos(theta), 0, -cos(theta), f * sin(theta));

    // cout << "homography_matrix = " << homography_matrix << endl;
    Mat homodst;
    cv::warpPerspective(cutted_org, homodst, homography_matrix, org.size());
    print(p1);
    print(p2);
    Vector2d l_start = ld.transform_point_homography(p1);
    Vector2d l_end = ld.transform_point_homography(p2);
    print(l_start);
    print(l_end);
    cv::line(dst, Point((dst.cols - (int)l_start.y()), (int)l_start.x()), Point((dst.cols - (int)l_end.y()), (int)l_end.x()), Scalar(255,255,0), 3, 8);
    print(intersect_point);
    cv::circle(dst, Point(dst.cols / 2 + intersect_point.x(), dst.rows - intersect_point.y()), 10, Scalar(255, 255, 0), 5);
    cv::circle(dst, Point(dst.cols / 2, dst.rows), r, Scalar(255, 255, 0), 5);
    // cv::line(cutted_org, Point(p1.x(), p1.y()), Point(p2.x(), p2.y()), Scalar(255,255,0), 3, 8);

    // Vector2d p(org.cols , y_border * ld.skip_step_);
    // Vector2d pp(0, y_border * ld.skip_step_);
    // print(ld.transform_point_homography(p));
    // print(ld.transform_point_homography(pp));

    imshow("Original", org);
    // imshow("black", black_image);
    // imshow("filteredx", filteredx);
    imshow("colordst", colordst);
    imshow("cutted_org", cutted_org);
    imshow("homography", homodst);
    imshow("transformed", dst);
    // imwrite("before_transformed.jpg", org);
    // imwrite("after_transformed.jpg", dst);
    waitKey(0);

    int cnt = 0;
    // while(waitKey(1)%256 != 'q'){
    //     cap >> org;
    //     cv::warpPerspective(org, dst, homography_matrix, org.size());
    //     imshow("Original", org);
    //     imshow("transformed", dst);
    //     stringstream filename;
    //     filename << "image_" << cnt << ".jpg";
    //     // imwrite(filename.str(), org);
    //     cnt++;
    // }
}
