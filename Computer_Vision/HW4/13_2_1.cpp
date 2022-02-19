// g++ $(pkg-config --cflags --libs opencv) 13_2_1.cpp -o ./a

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv ;
using namespace std ;

struct MouseParams {
    Mat img ;
    vector<Point2f> in, out ;
} ;

static void onMouse ( int event, int x, int y, void* param ) {
    MouseParams* mp = (MouseParams*) param ;
    Mat img = mp->img ;
    if ( event == EVENT_LBUTTONDOWN ) { // left button
        Mat result ;
        // Insert position from LT. Direction is clock-wise
        mp->in.push_back( Point2f(x, y) ) ;

        if ( mp->in.size() == 4 ) {
            // Calculate perspective transform matrix(=homo_mat) from 4 mactching pairs of points
            Mat homo_mat = getPerspectiveTransform( mp->in, mp->out ) ;
            // apply perspetive transformation to img using homo_mat
            // result will have the same size of Size(200, 200) and the same type of img
            warpPerspective( img, result, homo_mat, Size( 200, 200) ) ;
            imshow( "output", result ) ;
        }
        else {
            result = img.clone() ;
            for ( size_t i = 0 ; i < mp->in.size() ; i ++ ) {
                circle( result, mp->in[i], 3, Scalar( 0, 0, 255 ), 5 ) ;
            }
            imshow( "input", result ) ;
        }
    }

    // Reset positions
    if ( event == EVENT_RBUTTONDOWN ) {
        mp->in.clear() ;
        imshow( "input", img ) ;
    }
}

int
main () {
    Mat input = imread( "./resources/book.PNG" ) ;
    imshow( "input", input ) ;

    MouseParams mp ;
    mp.out.push_back( Point2f(0, 0) ) ;
    mp.out.push_back( Point2f(200, 0) ) ;
    mp.out.push_back( Point2f(200, 200) ) ;
    mp.out.push_back( Point2f(0, 200) ) ;
    mp.img = inpit ;

    setMouseCallback( "input", onMouse, (void*)&mp ) ;
    waitKey() ;
    return 0 ;
}