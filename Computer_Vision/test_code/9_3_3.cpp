// g++ $(pkg-config --cflags --libs opencv) 9_3_3.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv ;
using namespace std ;

int
main () {
    Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2() ;
    VideoCapture cap("./resources/Faces.mp4") ;

    Mat image, foregroundMask, backgroundImg, foregroundImg ;
    while ( true ) {
        cap >> image ;
        // resize( image, image, Size(640, 480)) ;

        if ( foregroundMask.empty() )
            foregroundMask.create( image.size(), image.type() ) ;
        
        bg_model->apply( image, foregroundMask ) ;


        // Mat element = getStructuringElement(MORPH_ELLIPSE, Size(1, 1) ) ;
        // // morphologyEx( foregroundMask,foregroundMask, MORPH_CLOSE, element ) ;
        // dilate(foregroundMask, foregroundMask, element) ;


        imshow( "foreground mask", foregroundMask ) ;
        GaussianBlur( foregroundMask, foregroundMask, Size(11, 11), 3.5, 3.5 ) ;
        threshold( foregroundMask, foregroundMask, 30, 255, THRESH_BINARY ) ;
        foregroundImg = Scalar::all( 0 ) ;
        image.copyTo( foregroundImg, foregroundMask ) ;

        bg_model->getBackgroundImage( backgroundImg ) ;

        // imshow( "foreground mask", foregroundMask ) ;
        imshow( "foreground image", foregroundImg ) ;

        if ( !backgroundImg.empty() ) {
            imshow( "mean background image", backgroundImg ) ;
        }
        waitKey(33) ;

    }

    return 0 ;
}