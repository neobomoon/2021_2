#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main ()  {
    Mat image, image_gray ;
    int value, value_B, value_G, value_R, channels ;

    image = imread("Lena.png") ;
    image_gray = imread("Lena.png", 0) ;

    value = image_gray.at<uchar>(50, 100) ; //(row, column)
    cout << "value: " << value ;

    value_B = image.at<Vec3b>(50, 100)[0] ;
    value_G = image.at<Vec3b>(50, 100)[1] ;
    value_R = image.at<Vec3b>(50, 100)[2] ;
    cout << "value at (100, 50): " <<  value_B << " " << value_G << " " <<  value_R << endl ;

    waitKey(0) ;

    return 0 ;
}