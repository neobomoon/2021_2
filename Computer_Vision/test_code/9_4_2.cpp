// g++ $(pkg-config --cflags --libs opencv) 9_4_2.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv ;
using namespace std ;

int
main () {
    Mat gray = imread( "./resources/contours.png", 0 ) ;
    Mat result ;

    threshold( gray, result, 180, 255, THRESH_BINARY_INV ) ;
    vector<vector<Point> > contours ;
    vector<Vec4i>hierarchy ;
    findContours( result, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE ) ;
    

    // defining bounding rectangle
    vector<Rect> boundRect(contours.size()) ;
    for ( int i = 0 ; i < contours.size() ; i++ ) {
        boundRect[i] = boundingRect(Mat(contours[i])) ;
    }
    // draw rectangles on the contours
    for ( int i = 0 ; i < contours.size() ; i++ ) {
        cout << boundRect[i].tl() << endl ;
        cout << boundRect[i].br() << endl ;
        rectangle( result, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 0, 255), 2, 8 ,0) ;
    }

    imshow("contours", result) ;
    waitKey(0) ;

    return 0 ;
}