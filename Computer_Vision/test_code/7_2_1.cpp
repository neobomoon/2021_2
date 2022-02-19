// g++ $(pkg-config --cflags --libs opencv) 7_2_1.cpp -o ./exe/7_2_1

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

    vector<Vec2f> lines ;
    HoughLines(edge, lines, 1, CV_PI / 180, 150) ;

    for ( int i = 0 ; i < lines.size() ; i++ ) {
        float rho = lines[i][0] ; 
        float theta = lines[i][1] ;
        float a = cos(theta) ;
        float b = sin(theta) ;

        float x0 = a * rho ;
        float y0 = b * rho ;

        Point p1 = Point(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a)) ;
        Point p2 = Point(cvRound(x0-1000 * (-b)), cvRound(y0 - 1000 * a)) ;

        line(result, p1, p2, Scalar(0, 0, 255), 3, 8) ;
    }

    imshow("input image", image) ;
    imshow("edge", edge) ;
    imshow("Hough Transform", result) ;

    waitKey(0) ;
    return 0 ;
}