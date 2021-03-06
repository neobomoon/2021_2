#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image, image_YUV, dst ;
    Mat yuv_channels[3] ;

    image = imread("./resources/Lena.png") ;

    cvtColor(image, image_YUV, CV_BGR2YUV) ;

    split(image_YUV, yuv_channels) ;

    merge(yuv_channels, 3, dst) ;

    imshow("input image", image) ;
    imshow("Y", yuv_channels[0]) ;
    imshow("U", yuv_channels[1]) ;
    imshow("V", yuv_channels[2]) ;
    imshow("YUV image", dst) ;

    waitKey(0) ;

    return 0 ;
}