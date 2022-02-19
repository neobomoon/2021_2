// g++ $(pkg-config --cflags --libs opencv) 9_5_1.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image, erosion, dilation ;
    Mat element = getStructuringElement( MORPH_ELLIPSE, Size(10, 10) ) ;

    //Other options:MORPH_RECT, MORPH_CROSS
    image = imread( "./resources/water_coins.jpg", 0 ) ;
    threshold( image, image, 128, 255, THRESH_BINARY ) ;

    erode( image, erosion, element) ;
    dilate( image, dilation, element ) ;

    imshow( "Binary image", image ) ;
    imshow( "Erosion", erosion ) ;
    imshow( "Dilation", dilation ) ;

    waitKey( 0 ) ;
    return 0 ;
}