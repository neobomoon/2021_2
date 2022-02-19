// g++ $(pkg-config --cflags --libs opencv) 8_2_1.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image = imread("./resources/Lena.png") ;
    cvtColor(image, image, CV_BGR2GRAY) ;
    Mat dst ;
    
    // th = 100
    // max = 255
    threshold(image, dst, 100, 255, THRESH_BINARY) ;

    imshow("dst", dst) ;
    imshow("image", image) ;
    waitKey(0) ;
    return 0 ;
}