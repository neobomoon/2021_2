#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

Mat
drawHistogram (Mat src) {
    Mat hist ;
    int histSize = 256 ;
    float range[] = { 0, 256 } ; // 입력값 범위 정하기
    const float* histRange = { range } ;
    
    // 히스토그램을 계산하는 부분
    // src: input image 
    // 1: image 갯수
    // 0: channel 갯수 (0 ~ channels() - 1)
    // Mat(): 특정 part위해 (mask)
    // hist: output histogram
    // 1: histogram dimension
    // &histSize: array of histogram size (= bin)
    // &histRange: array of histogram's boundaries
    calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &histRange) ;


    int hist_w = 512 ;
    int hist_h = 400 ;
    // hist_h: histogram 높이
    // hist_w: histogram 넓이
    // CV_8UC3: type
    // 바탕 색깔
    Mat histImage = Mat(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255)) ;

    // 캔버스 높이를 이용해서 계산한 히스토그램을 정규화 시킴.
    // 0: lower range boundary of range normalization
    // histImage.rows: upper range boundary
    // NORM_MINMAX: type
    // -1: when negative, the output array has the same type as src
    // Mat(): 특정 part위해 (mask)
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat()) ;

    // 캔버스 크기 대비 bin의 개수로 나눠주면 하나의 bin의 width 알 수 있다.
    int bin_w = cvRound((double)hist_w / histSize) ;

    for ( int i = 0 ; i < histSize; i++ ) {
        rectangle(histImage, Point(bin_w * i, hist_h), Point(bin_w * i + hist_w / histSize, hist_h - cvRound(hist.at<float>(i))), Scalar(0, 0, 0), -1) ;
    }
    
    return histImage ;
}

int
main () {
    Mat image;
    Mat hist_equalized_image ;
    Mat hist_graph ;
    Mat hist_equalized_graph ;

    image = imread("./resources/Lena.png", 0) ;

    if ( !image.data)
        exit(1) ;

    equalizeHist(image, hist_equalized_image) ;

    hist_graph = drawHistogram(image) ;
    hist_equalized_graph = drawHistogram(hist_equalized_image) ;

    imshow("input", image) ;
    imshow("hist equalized image", hist_equalized_image) ;
    imshow("hist graph", hist_graph) ;
    imshow("hist equalized graph", hist_equalized_graph) ;

    waitKey(0) ;
    return 0 ;
}
