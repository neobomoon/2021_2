#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat img = imread("Lena.png") ;
    Mat resize_img ;
    resize(img, resize_img, Size(200,200)) ;
    imshow("original image", img) ;
    imshow("resize image", resize_img) ;

    waitKey(0) ;
    return 0 ;
}