// g++ $(pkg-config --cflags --libs opencv) 8_2_3.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image = imread("./resources/hand.png") ;
    cvtColor(image, image, CV_BGR2YCrCb) ;
    Mat dst ;
    // src, lowerbound, upperbound, dst
    inRange(image, Scalar(0, 133, 77), Scalar(255, 173, 127), dst) ;    

    imshow("dst", dst) ;
    imshow("image", image) ;
    waitKey(0) ;
    return 0 ;
}