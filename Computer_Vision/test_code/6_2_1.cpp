// g++ $(pkg-config --cflags --libs opencv) 6_2_1.cpp -o ./exe/6_2_1

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat blur ;
    Mat image = imread("./resources/Lena.png", 0) ;

    GaussianBlur(image, blur, Size(5, 5), 5, 5, BORDER_DEFAULT) ;

    // 1: order of derivative in x direction
    // 0: order of derivative in y direction
    // 3: size of extended sobel kernel, it must be 1, 3, 5, 7
    Mat grad_x , abs_grad_x ;
    Sobel(blur, grad_x, CV_16S, 1, 0, 3) ;
    convertScaleAbs(grad_x, abs_grad_x) ;

    // 0: order of derivative in x direction
    // 1: order of derivative in y direction
    // 3: size of extended sobel kernel, it must be 1, 3, 5, 7
    Mat grad_y, abs_grad_y ;
    Sobel(blur, grad_y, CV_16S, 0, 1, 3) ;
    convertScaleAbs(grad_y, abs_grad_y) ;

    // 0.5: weight for abs_grad_x or abs_grad_y
    // 0: offset added to weighted sum
    Mat result ;
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, result) ;

    imshow("X", abs_grad_x) ;
    imshow("Y", abs_grad_y) ;
    imshow("input image", image) ;
    imshow("Sobel Edge Detector", result) ;

    waitKey(0) ;
    return 0 ;
}