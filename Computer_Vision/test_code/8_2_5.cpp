// g++ $(pkg-config --cflags --libs opencv) 8_2_5.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

// Otsu's
int
main () {
    Mat image = imread("./resources/Lena.png", 0) ;
    Mat result ;
    
    // max = 255
    // dst: mean(7x7) - c
    // blocksize = 7
    // c = 10
    threshold(image, result, 0, 255, THRESH_BINARY | THRESH_OTSU) ;

    imshow("image", image) ;
    imshow("dst", result) ;
    waitKey(0) ;
    return 0 ;
}