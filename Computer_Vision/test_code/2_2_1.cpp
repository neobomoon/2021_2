// g++ $(pkg-config --cflags --libs opencv) 2_2_1.cpp -o ./exe/2_2_1

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    //  make a 3x3 floating--point matrix
    Mat mtx(3, 3, CV_32F) ;

    // make a  10x1 2-channel floating-point matrix
    Mat mtx(10, 1, CV_64F) ;

    // make a 3-channel (color) image of 1920 columns and 1080 rows.
    Mat img(1080, 1920, CV_8UC3) ;

    // make a 3-channel (color) image of 1920 columns  and 1080 rows.
    Mat img(Size(1920, 1080), CV_8UC3) ;

    return 0 ;
}