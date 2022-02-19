// g++ $(pkg-config --cflags --libs opencv) 1_3_2.cpp -o ./exe/1_3_2
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat frame ;
    VideoCapture cap ;

    // check if file exists. if none program ends
    if (cap.open("background.mp4") == 0) {
        cout << "no such file!" << endl ;
        waitKey(0) ;
    }

    double fps = cap.get(CAP_PROP_FPS) ; // frame rate
    double time_in_sec = cap.get(CAP_PROP_POS_MSEC) ; // current position of the video file in msec or video capture timestamp
    int total_frames = cap.get(CAP_PROP_FRAME_COUNT) ; // number of frames in the video file
    cout << "fps: " << fps << endl ;
    cout << "time_in_sec: " << time_in_sec << endl ;
    cout << "total_frames: " << total_frames << endl ;
}