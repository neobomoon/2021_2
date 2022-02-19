#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv ;
using namespace std ;

Mat
drawHistogram (Mat src, int histSize) {
    int width = 512 ;
    int height = 512 ;
    float range[] = { 0, 256 } ;
    const float* hist_Range = { range } ;

    // bin interval
    int bin_w = cvRound((double)width / histSize) ;

    Mat histImage = Mat(width, height, CV_8UC3, Scalar(255, 255, 255)) ;

    Mat hist ;

    calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &hist_Range) ;

    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat()) ;

    for ( int i = 0 ; i < histSize ; i++ ) {
        rectangle(histImage, Point(bin_w * i, height), Point(bin_w * i + width / histSize, height - cvRound(hist.at<float>(i))), Scalar(0, 0, 0), -1) ;
        // putText(histImage, to_string(cvRound(hist.at<float>(i))), Point(bin_w * i + bin_w / 6, height), 1, 0.8, Scalar(100, 150, 200), 1, 8) ; 
    }

    return histImage ;
}

Mat
drawText (Mat src, int histSize) {
    float range[] = { 0, 256 } ;
    const float* hist_Range = { range } ;

    Mat hist ;
    calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &hist_Range) ;
    normalize(hist, hist, 0, 512, NORM_MINMAX, -1, Mat()) ;


    for ( int i = 0 ; i < histSize ; i++ ) {
        putText(src, format("bin %d: %d", i + 1, cvRound(hist.at<float>(i))), Point(5, i * 20 + 20), 1, 0.8, Scalar(255, 255, 255), 1, 8) ; 
    }
    return src ;
}

int
main() {
    // 1. read an image
    Mat moon = imread("./resources/moon.png", 0) ;

    if ( moon.empty() ) {
        cout << "no such file" << endl ;
        return 0 ;
    }

    // 2. Perform histogram equalization on the input image
    Mat before = moon.clone() ;
    Mat after ;

    equalizeHist(before, after) ;

    // // display each image with the window name as 'before', 'after'
    // imshow("before", before) ;
    // imshow("after", after) ;
    // waitKey(0) ;
    // 3. display each histogram of the input and result image
    Mat before_hist_16 = drawHistogram(before, 16) ;
    Mat after_hist_16 = drawHistogram(after, 16) ;
    // display each image with the window name as 'h1', 'h2'
    imshow("h1", before_hist_16) ;
    imshow("h2", after_hist_16) ;
    
    // 4. compute the value of each component of a normalized histogram of the input image;
    //    write all values on the input image; 
    //    and display the result
    // Mat before_hist_8 = drawHistogram(before, 8) ;
    // Mat after_hist_8 = drawHistogram(after, 8) ;
    before = drawText(before, 8) ;
    after = drawText(after, 8) ;
    imshow("before", before) ;
    imshow("after", after) ;
    // imshow("before histogram(bin 8)", before_hist_8) ;
    // imshow("after histogram(bin 8)", after_hist_8) ;

    

    waitKey(0) ;
    return 0 ;
}
