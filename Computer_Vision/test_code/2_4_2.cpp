#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image = imread("Lena.png") ;
    Mat mask = Mat::zeros(image.size(), image.type()) ;
    Mat copied ;
    Rect rect = Rect(10, 10, 100, 100) ;
    rectangle(mask, rect, Scalar::all(255), -1, 8, 0) ; // (img, Rect, Scalar(b, g, r), 선굵기, 라인 타입, shift) shift는 보통 0
    image.copyTo(copied, mask) ; // (output, input)
    imshow("copied", copied) ;

    waitKey(0) ;

    return 0 ;
}