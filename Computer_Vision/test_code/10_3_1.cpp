// g++ $(pkg-config --cflags --libs opencv) 10_3_1.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv ;
using namespace std ;

int
main () {
    CascadeClassifier face_classifier ;
    Mat frame, grayframe ;
    vector<Rect> faces ;
    
    // oepn the webcam
    VideoCapture cap(0) ;

    // check if we succeeded
    if ( !cap.isOpened() ) {
        cout << "Could not open camera" << endl ;
        return -1 ;
    }

    //face detection configuration
    face_classifier.load( "resources/haarcascade_frontalface_alt.xml" ) ;

    while ( true ) {
        // get a new frame from webcam
        cap >> frame ;

        // convert captured frame to gray scale
        cvtColor( frame, grayframe, COLOR_BGR2GRAY ) ;
        face_classifier.detectMultiScale( grayframe, faces, 1.1, // increase search scale by 10% each pass
                                                            3, // merge groups of three detections
                                                            0, // not used for a new cascade
                                                            Size(30, 30) ) ; // min size

        // draw the results
        for ( int i = 0 ; i < faces.size() ; i++ ) {
            Point lb( faces[i].x + faces[i].width, faces[i].y + faces[i].height ) ;
            Point tr( faces[i].x, faces[i].y ) ;
            rectangle( frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0 ) ;
        }

        //print the output
        imshow( "Face Detection", frame ) ;
        if ( waitKey(33) == 27 ) 
            break ;

    }


    waitKey( 0 ) ;
    return 0 ;
}