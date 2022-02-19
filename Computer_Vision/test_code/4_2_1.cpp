#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image = imread("./resources/Lena.png") ;
    Rect rect = Rect(10, 10, 100, 100) ;
    rectangle(image, rect, Scalar(255, 0, 0), 4, 8, 0) ;
    imshow("image", image) ;

    waitKey(0) ;
    return 0 ;
}