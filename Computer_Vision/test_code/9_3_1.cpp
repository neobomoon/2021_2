// g++ $(pkg-config --cflags --libs opencv) 9_3_1.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv ;
using namespace std ;


int
main () {
    VideoCapture capture("./resources/background.mp4") ;
    int cnt = 2 ;

    Mat avg ;
    capture >> avg ;

    Mat image ;
    Mat sum ;
    while ( true ) {
        if ( !capture.read( image ) )
            waitKey(0) ;
        add(image / cnt, avg * ( cnt - 1) / cnt, avg) ;

        imshow("avg", avg) ;
        cnt++ ;
        waitKey( 33 ) ;
    }

    return 0 ;
}