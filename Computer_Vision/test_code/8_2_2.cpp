// g++ $(pkg-config --cflags --libs opencv) 8_2_2.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image = imread("./resources/Lena.png") ;
    cvtColor(image, image, CV_BGR2GRAY) ;
    Mat dst ;
    
    // max = 255
    // dst: mean(7x7) - c
    // blocksize = 7
    // c = 10
    adaptiveThreshold(image, dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 10) ;

    imshow("dst", dst) ;
    imshow("image", image) ;
    waitKey(0) ;
    return 0 ;
}