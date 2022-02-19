// g++ $(pkg-config --cflags --libs opencv) 5_4_2.cpp -o ./exe/5_4_2

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat gray = imread("./resources/xray.jpg") ;
    Mat color ;

    applyColorMap(gray, color, COLORMAP_JET) ;
    imshow("gray", gray) ;
    imshow("image", color) ;
    waitKey(0) ;

    return 0 ;
}