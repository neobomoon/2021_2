#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image, image_YUV ;

    image = imread("./resources/Lena.png") ;

    cvtColor(image, image_YUV, CV_BGR2YUV) ;

    return 0 ;
}