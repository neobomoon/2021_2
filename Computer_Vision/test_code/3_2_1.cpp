#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image = imread("Lena.png") ;
    Rect rect(100, 30, 250, 300) ;
    Mat rect_roi = image(rect) ;
    imshow("rectROI", rect_roi) ;

    waitKey(0) ;

    return 0 ;
}