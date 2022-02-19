#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main() {
    Mat image = imread("./resources/Lena.png", 0) ;
    Mat hist_equalized_image ;

    if ( image.empty() ) {
        cout << "no such file" << endl ;
        return 0 ;
    }
    
    equalizeHist(image, hist_equalized_image) ;

    imshow("input image", image) ;
    imshow("hist equalized image", hist_equalized_image) ;

    waitKey(0) ;
    return 0 ;
}