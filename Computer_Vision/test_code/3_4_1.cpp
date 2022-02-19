#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image, avgimg, gaussianimg ;
    image = imread("Lena.png") ;
    
    blur(image, avgimg, Size(5, 5)) ;

    GaussianBlur(image, gaussianimg, Size(5, 5), 1,5) ;

    imshow("Input image", image) ;
    imshow("Average image", avgimg) ;
    imshow("Gaussian blurred image", gaussianimg) ;

    waitKey(0) ;

    return 0 ;
}