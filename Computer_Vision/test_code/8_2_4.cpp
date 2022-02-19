// g++ $(pkg-config --cflags --libs opencv) 8_2_4.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

// basic
int
main () {
    int thresh_T = 200 ;
    int th = 10 ;
    int low_cnt = 0 ;
    int high_cnt = 0 ;
    int low_sum = 0 ;
    int high_sum = 0 ;

    Mat image = imread("./resources/Lena.png", 0) ;

    cout << "threshold value: " << thresh_T << endl ;

    while(1) {
        for( int j = 0 ; j < image.rows ; j++ ) {
            for( int i = 0 ; i < image.cols ; i++ ) {
                if( image.at<uchar>(j, i) < thresh_T) {
                    low_sum += image.at<uchar>(j, i) ;
                    low_cnt++ ;
                }
                else {
                    high_sum += image.at<uchar>(j, i) ;
                    high_cnt++ ;
                }
            }
        }
        if( abs(thresh_T - (low_sum / low_cnt + high_sum / high_cnt) / 2.0f) < th) {
            break ;
        }
        else {
            thresh_T = (low_sum / low_cnt + high_sum / high_cnt) / 2.0f ;
            cout << "threshold value: " << thresh_T << endl ;
            low_cnt = high_cnt = low_sum = high_sum = 0 ;
        }
    }

    Mat thresh ;
    threshold(image, thresh, thresh_T, 255, THRESH_BINARY) ;

    imshow("input image", image) ;
    imshow("thresholding", thresh) ;
    waitKey(0) ;
    return 0 ;
}