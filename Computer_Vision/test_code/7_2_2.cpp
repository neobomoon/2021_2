// g++ $(pkg-config --cflags --libs opencv) 7_2_2.cpp -o ./exe/7_2_2

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image = imread("./resources/chess_pattern.png") ;
    Mat result = image.clone() ;

    cvtColor(image, image, CV_BGR2GRAY) ;

    Mat edge ;
    Canny(image, edge, 50, 200, 3) ;

    // 1: rho
    // CV_PI / 180: theata
    // 50: threshold
    // 10: minLineLength
    // 300: Maximum allowed gap between points on the same line to link them.
    vector<Vec4i> lines ;
    HoughLinesP(edge, lines, 1, CV_PI / 180, 50, 10, 300) ;

    for ( int i = 0 ; i < lines.size() ; i++ ) {
        Vec4i l = lines[i] ;

        line(result, Point(l[0], l[1]), Point(l[2] , l[3]), Scalar(0, 0, 255), 3, 8) ;
    }

    imshow("input image", image) ;
    imshow("edge", edge) ;
    imshow("Hough Transform", result) ;

    waitKey(0) ;
    return 0 ;
}