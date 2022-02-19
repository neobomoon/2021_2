// g++ $(pkg-config --cflags --libs opencv) 5_4_3.cpp -o ./exe/5_4_3

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

void
white_balancing (Mat img) {
    Mat bgr_channels[3] ;
    split(img, bgr_channels) ;
    
    for ( int c = 0 ; c < img.channels() ; c++ ) {
        int sum = 0 ;
        double avg = 0.0f ;
        for ( int i = 0 ; i < img.rows; i++ ) {
            for (int j = 0 ; j < img.cols; j++ ) {
                sum += bgr_channels[c].at<uchar>(i ,j) ;
            }
        }
        avg = sum / (img.rows * img.cols) ;
        for ( int i = 0 ; i < img.rows ; i++ ){
            for ( int j = 0 ; j < img.cols ; j++ ) {
                int temp = (128 / avg) * bgr_channels[c].at<uchar>(i ,j) ;
                if (temp > 255)
                    bgr_channels[c].at<uchar>(i, j) = 255 ; // 255넘으면 그냥 255
                else
                    bgr_channels[c].at<uchar>(i, j) = temp ;
            }
        }
    }

    merge(bgr_channels, 3, img) ;
}


int
main () {
    Mat balancing = imread("./resources/balancing.jpg") ;
    Mat balancing_result = balancing.clone() ;

    white_balancing(balancing_result) ;
    imshow("image", balancing) ;
    imshow("balancing", balancing_result) ;
    waitKey(0) ;

    return 0 ;
}