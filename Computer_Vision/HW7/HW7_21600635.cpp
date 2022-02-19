// g++ $(pkg-config --cflags --libs opencv) HW7_21600635.cpp -o ./a

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv ;
using namespace std ;

Mat
basic (Mat src) {
    int thresh_T = 200 ;
    int th = 10 ;
    int low_cnt = 0 ;
    int high_cnt = 0 ;
    int low_sum = 0 ;
    int high_sum = 0 ;

    Mat image = imread("./resources/Lena.png", 0) ;

    while(1) {
        for( int j = 0 ; j < src.rows ; j++ ) {
            for( int i = 0 ; i < src.cols ; i++ ) {
                if( src.at<uchar>(j, i) < thresh_T) {
                    low_sum += src.at<uchar>(j, i) ;
                    low_cnt++ ;
                }
                else {
                    high_sum += src.at<uchar>(j, i) ;
                    high_cnt++ ;
                }
            }
        }
        if( abs(thresh_T - (low_sum / low_cnt + high_sum / high_cnt) / 2.0f) < th) {
            break ;
        }
        else {
            thresh_T = (low_sum / low_cnt + high_sum / high_cnt) / 2.0f ;
            low_cnt = high_cnt = low_sum = high_sum = 0 ;
        }
    }

    Mat thresh ;
    threshold(src, thresh, thresh_T, 255, THRESH_BINARY) ;

    return thresh ;
}

Mat
local_adaptive (Mat src) {
    Mat adaptive_binary ;
    adaptiveThreshold(src, adaptive_binary, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 85, 15) ;

    return adaptive_binary ;
}

Mat
local_adaptive_partition (Mat src) {
    int mask = src.rows ;
    for ( int i = 0 ; i < src.cols ; i ++ ) {
        Rect rect(i, 0, 1, src.rows) ;
        adaptiveThreshold(src(rect), src(rect), 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, mask, 15) ;
    }

    return src ;
}

int
main() {
    Mat finger_print = imread("HW7_resources/finger_print.png", 0) ;
    Mat adaptive_1 = imread("HW7_resources/adaptive_1.jpg", 0) ;
    Mat adaptive = imread("HW7_resources/adaptive.png", 0) ;

    if ( finger_print.empty() |  adaptive_1.empty() | adaptive.empty() ) {
        cout << "no such file" << endl ;
        return 0 ;
    }

    finger_print = basic(finger_print) ;
    adaptive_1 = local_adaptive(adaptive_1) ;
    adaptive = local_adaptive_partition(adaptive) ;

    imshow("finger_print", finger_print) ;
    imshow("adaptive_1", adaptive_1) ;
    imshow("adaptive", adaptive) ;
    waitKey(0) ;
    return 0 ;
}