#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image = imread("Lena.png") ;
    int value, value_B, value_G, value_R, channels ;
    channels = image.channels() ;
    cout << "channels: " << channels <<  endl ;
    uchar *p ;
    p = image.ptr<uchar>(50) ; // row: 50
    value_B = p[ 100 * channels + 0 ] ; // BGRBGRBGR...순으로 저장되기 때문에
    value_G = p[ 100 * channels + 1 ] ;
    value_R = p[ 100 * channels + 2 ] ;

    cout << "value at (100, 50): " <<  value_B << " " << value_G << " " <<  value_R << endl ;

    return  0 ;
}