#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int 
main () {
    Mat img1 = imread("Lena.png") ;
    Mat img2 = imread("Lena.png") ;
    Mat dst ;
    add(img1, img2, dst) ;
    imshow("dst", dst) ;
    waitKey(0) ;

    return 0 ;
}