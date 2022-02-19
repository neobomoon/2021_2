#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat frame ;
    VideoCapture cap ;

    // open mp4 file
    if ( cap.open("background.mp4") == 0 ) {
        cout << "no such file" << endl ;
        waitKey(0) ;
    }

    int play_time = 0 ;
    // play the video for 3sec
    while ( play_time != 3000 ) {

        // exception. (end of video)
        cap >> frame ; // pixel data -> Mat (Mat can have pixel data)
        if ( frame.empty() ) {
            cout << "end of video" << endl ;
            break ;
        }

        // show image
        imshow("video", frame) ;
        waitKey(33) ;

        // print current frame / total frame
        int current_frame = cap.get(CAP_PROP_POS_FRAMES) ;
        int total_frames = cap.get(CAP_PROP_FRAME_COUNT) ;
        cout << current_frame << " / "<< total_frames << endl ;

        // update play time
        play_time = cap.get(CAP_PROP_POS_MSEC) ;
    }

    return 0 ;
}