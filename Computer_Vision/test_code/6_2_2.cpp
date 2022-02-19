// g++ $(pkg-config --cflags --libs opencv) 6_2_2.cpp -o ./exe/6_2_2

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image = imread("./resources/Lena.png", 0) ;
    Mat canny ;
    Canny(image, canny, 190, 200, 3) ;

    imshow("input image", image) ;
    imshow("canny", canny) ;

    waitKey(0) ;
    return 0 ;
}