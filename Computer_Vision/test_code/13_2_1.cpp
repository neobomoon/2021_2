// g++ $(pkg-config --cflags --libs opencv) 13_2_1.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv ;
using namespace std ;

struct MouseParams {
    Mat img ;
    vector<Point2f> in, out ;
} ;

static void
onMouse ( int event, int x, int y, int, void * param ) {
    MouseParams* mp = (MouseParams*) param ;
    Mat img = mp->img ;
    if ( event == EVENT_LBUTTONDOWN ) {
        Mat result ;

        // insert position from LT. Direction is clock-wise
        mp->in.push_back( Point2f(x, y) ) ;

        if ( mp->in.size() == 4 ) {
            // calculate perspective transform matrix(=homo_mat) from 4 matching pairs of points
            Mat homo_mat = getPerspectiveTransform( mp->in, mp->out ) ;

            // apply perspedtive transformation to img using homo_mat
            // result will have the same size of Size(200, 200) and the same type of img
            warpPerspective( img, result, homo_mat, Size(200, 200) ) ;
            imshow( "output", result ) ;
        }
        else {
            result = img.clone() ;
            for (size_t i = 0 ; i < mp->in.size() ; i++ ) {
                circle(result, mp->in[i], 3, Scalar(0, 0, 255), 5 ) ;
            }
            imshow( "input", result ) ;
        }
    }

    if ( event == EVENT_RBUTTONDOWN ) {
        mp->in.clear() ;
        imshow( "input", img ) ;
    }
} 

int
main () {
    Mat input = imread( "./resources/book.jpg" ) ;
    imshow( "input", input) ;
    
    MouseParams mp ;
    mp.out.push_back(Point2f(0, 0) ) ;
    mp.out.push_back(Point2f(200, 0) ) ;
    mp.out.push_back(Point2f(200, 200) ) ;
    mp.out.push_back(Point2f(0, 200) ) ;

    mp.img = input; 

    setMouseCallback( "input", onMouse, (void*) &mp ) ;
    waitKey(0) ;
    return 0 ;
}