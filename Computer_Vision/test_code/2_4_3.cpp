#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image = imread("Lena.png") ;
    Mat after_converTo, after_converScaleAbs ;

    imshow("original", image) ;

    image.convertTo(after_converTo, CV_16SC1) ; // CV_16SC1은 보통 회색 담을 때 쓴다
    imshow("after convertTo", after_converTo) ;

    convertScaleAbs(image, after_converScaleAbs, 2, 3) ;
    imshow("after convertScaleAbs", after_converScaleAbs) ;

    image.setTo(Scalar(0)) ;
    imshow("after setTo", image) ;

    waitKey(0) ;

    return 0 ;
}