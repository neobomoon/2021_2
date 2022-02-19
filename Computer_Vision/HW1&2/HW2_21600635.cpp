// g++ $(pkg-config --cflags --libs opencv) HW2_21600635.cpp -o HW2_21600635

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <typeinfo>


using namespace cv ;
using namespace std ;

int 
main () {
    // Loading image.
    Mat image = imread("Lena.png", 0) ;
    // Channels of image = 3. Because image is color.
    int channels = image.channels() ;
    
    //Exception.
    if (image.empty()) {
        cout << "no such file" << endl ;
        return 0 ;
    }


    // 1. Negative transformation.
    Mat negative_img = image.clone() ;

    // Position of pixcel.
    int pixel_size = image.rows * image.cols * channels ;

    for ( int i = 0 ; i < pixel_size ; i++ )
        negative_img.data[ i ] = 255 - 1 - negative_img.data[ i ] ;

    // Result of negative transformation.
    imshow("negative_img", negative_img) ;
    

    // 2. Log transformation.
    Mat log_img ;

    // Convert uchar to float type.
    image.convertTo(log_img, CV_32F) ;

    log(log_img + 1, log_img) ;
    normalize(log_img, log_img, 0, 255, NORM_MINMAX) ;
    convertScaleAbs(log_img, log_img, 1.5) ; // float -> uchar 

    // Result of log transformation.
    imshow("log_img", log_img) ;


    // 3. Gamma transformation.
    Mat gamma_img = image.clone() ;
    unsigned char look_up[256] ;
    int arr_size = sizeof(look_up) / sizeof(unsigned char) ;
    float gamma = 0.5 ;

    // Look_up table.
    for ( int i = 0 ; i < arr_size ; i++ ) {
        look_up[i] = saturate_cast<uchar>( pow( i / 255.0, gamma ) * 255.0 ) ;
    }

    for ( int i = 0 ; i < pixel_size ; i++ ) {
        gamma_img.data[ i ] = look_up[ gamma_img.data[ i ] ] ;
    }

    // Result of gamma transformation.
    imshow("gamma_img", gamma_img) ;

    waitKey(0) ;

    return 0 ;
}