// g++ $(pkg-config --cflags --libs opencv) 9_5_1.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image = imread( "./resources/water_coins.jpg", 0 ) ;
    threshold( image, image, 128, 255, THRESH_BINARY ) ;

    Mat element = getStructuringElement( MORPH_ELLIPSE, Size(7, 7) ) ;

    Mat closing ;
    Mat opening ;
    morphologyEx( image, closing, MORPH_CLOSE, element ) ;
    morphologyEx( image, opening, MORPH_OPEN, element ) ;

    imshow( "Binary image", image ) ;
    imshow( "opening", opening ) ;
    imshow( "closing", closing ) ;

    waitKey( 0 ) ;
    return 0 ;
}