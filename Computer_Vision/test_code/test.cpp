#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat frame ;
    VideoCapture cap ;

    if ( cap.open("background.mp4") == 0) {
        cout << "such no file" << endl ;
        waitKey(0) ;
    }

    int play_time = 0 ;
    while( play_time == 3000 ) {
        cap >> frame ;

        // exception 처리
        if ( frame.empty() ) {
            cout << "end of video" << endl ;
            break ;
        }

        // 재생
        imshow("video", frame) ;
        waitKey(33) ;

        // 현재 프레임
        int current_frame = cap.get(CAP_PROP_POS_FRAMES) ;
        int total_frame = cap.get(CAP_PROP_FRAME_COUNT) ;
        cout << current_frame << " / " << total_frame << endl ;

        //시간 측정
        play_time = cap.get(CAP_PROP_POS_MSEC) ;
    }

    return 0 ;
}