// g++ $(pkg-config --cflags --libs opencv) 1_3_1.cpp -o ./exe/1_3_1
#include <opencv2/opencv.hpp> 
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat gray_image, color_image ;

    // 0 = gray
    gray_image = imread("./Lena.png", 0) ;

    // 1 = default, color
    color_image = imread("./Lena.png") ;

    imshow("gray_image", gray_image) ;
    imshow("color_image", color_image) ;

    waitKey(0) ;
    return 0 ;
}