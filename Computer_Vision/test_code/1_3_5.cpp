#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int 
main () {
    Mat frame ;
    VideoCapture cap ;

    if (cap.open("background.mp4") == 0) {
        cout << "no such file" << endl ;
        waitKey(0) ;
    }
    while (1) {
        cap >> frame ; // extract one frame of Video from cap
        if (frame.empty()) {
            cout << "end of video" << endl ;
            break ;
        }
        imshow("video", frame) ;
        waitKey(33) ; // 33msec. If put 0, it is special value that means forever. Therefore, it will wait forever.
        cout << "--" << frame << endl ;
    }
}