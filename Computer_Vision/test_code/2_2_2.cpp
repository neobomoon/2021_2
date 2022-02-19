// g++ $(pkg-config --cflags --libs opencv) 2_2_2.cpp -o ./exe/2_2_2

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    int w = 150 ;
    int h = 100 ;
    Mat image(h, w, CV_8UC1, Scalar(255)) ;
    cout << "Size: " << image.size().height << "," << image.size().width << endl ;
    imshow("image", image) ;
    waitKey(0) ;

    return 0 ;
}
