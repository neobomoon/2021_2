// g++ $(pkg-config --cflags --libs opencv) 8_2_6.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

// Local(Adaptive)
int
main () {
    Mat image = imread("./resources/opencv.jpg", 0) ;

    Mat binary ;
    threshold(image, binary, 150, 255, THRESH_BINARY) ;
    
    Mat adaptive_binary ;
    adaptiveThreshold(image, adaptive_binary, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 85, 15) ;

    imshow("image", image) ;
    imshow("binary", binary) ;
    imshow("adaptive binary", adaptive_binary) ;

    waitKey(0) ;
    return 0 ;
}