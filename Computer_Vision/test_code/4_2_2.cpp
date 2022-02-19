#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image = imread("./resources/Lena.png") ;
    Point p1(25, 25), p2(100, 50) ;

    line(image, p1, p2, Scalar(255, 0, 0), 3, 8, 0) ;
    imshow("image", image) ;

    waitKey(0) ;
    return 0 ;
}