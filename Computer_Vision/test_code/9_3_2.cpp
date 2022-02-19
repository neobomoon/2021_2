// g++ $(pkg-config --cflags --libs opencv) 9_3_2.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv ;
using namespace std ;

int
main () {
    VideoCapture capture("./resources/Faces.mp4") ;
    Mat background ;
    capture >> background ;
    cvtColor(background, background, CV_BGR2GRAY) ;

    Mat image, gray, result, foregroundMask, foregroundImg ;
    while ( true ) {
        if ( capture.grab() == 0 )
            break ;
        capture.retrieve( image ) ;
        cvtColor( image, gray, CV_BGR2GRAY ) ;

        absdiff( background, gray, foregroundMask ) ; // gray - foregroundMask
        threshold( foregroundMask, foregroundMask, 50, 255, CV_THRESH_BINARY ) ;
        foregroundMask.copyTo( foregroundImg ) ;
        gray. copyTo(foregroundImg, foregroundMask) ;

        imshow( "foregroundImg", foregroundImg ) ;
        imshow( "foregroundMask", foregroundMask ) ;
        imshow( "background", background ) ;

        waitKey( 33 ) ;
    }

    return 0 ;
}