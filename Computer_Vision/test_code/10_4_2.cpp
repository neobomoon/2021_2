// g++ $(pkg-config --cflags --libs opencv) 10_4_2.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv ;
using namespace std ;

struct CallbackParam {
    Mat frame ;
    Point pt1, pt2 ;
    Rect roi ;
    bool drag ;
    bool updated ;
} ;

void
onMouse ( int event, int x, int y, int flags, void* param ) {
    CallbackParam *p = (CallbackParam*) param ;
    if ( event == EVENT_LBUTTONDOWN ) {
        p->pt1.x = x ;
        p->pt1.y = y ;
        p->pt2 =  p->pt1 ;
        p->drag = true ;
    }

    if ( event == EVENT_LBUTTONUP ) {
        int w = x - p->pt1.x ;
        int h = y - p->pt1.y ;
        p->roi.x = p->pt1.x ;
        p->roi.y = p->pt1.y ;
        p->roi.width = w ;
        p->roi.height = h ;
        p->drag = false ;

        if ( w >= 10 && h >= 10 ) {
            p->updated = true ;
        }
    }

    if (p->drag && event == EVENT_MOUSEMOVE ) {
        if (p->pt2.x != x || p->pt2.y != y) {
            Mat img = p->frame.clone() ;
            p->pt2.x = x ;
            p->pt2.y = y ;
            rectangle(img, p->pt1, p->pt2, Scalar(0, 255, 0), 1 ) ;
            imshow( "Tracker", img ) ;
        }
    }
}


int
main ( int argc, char *argv[] ) {
    VideoCapture cap(0) ;
    CallbackParam param ;
    Mat frame, m_backproj, hsv ;
    Mat m_model3d ;
    Rect m_rc ;
    float hrange[] = {0, 180} ;
    float vrange[] = {0, 255} ;
    float srange[] = {0, 255} ;
    const float* ranges[] = { hrange, srange, vrange } ; //hue, satureation, brightness
    int channels[] = { 0, 1, 2 } ;
    int hist_size[] = { 16, 16, 16 } ;

    // check if we succeeded
    if ( !cap.isOpened() ) {
        cout << "can't open video file" << endl ;
        return 0 ;
    }
    
    // click and drag on image to se ROI
    cap >> frame ;
    imshow( "Tracker", frame ) ;
    param.frame = frame ;
    param.drag = false ;
    param.updated = false ;
    setMouseCallback( "Tracker", onMouse, &param ) ;

    bool tracking = false ;
    while ( true ) {
        // image acquisition & target init
        if ( param.drag ) {
            if ( waitKey(33) == 27 ) 
                break ;
            continue ;
        }
        cvtColor(frame, hsv, COLOR_BGR2HSV) ;
        if ( param.updated ) {
            Rect rc = param.roi ;
            Mat mask = Mat::zeros(rc.height, rc.width, CV_8U) ;
            ellipse(mask, Point(rc.width / 2, rc.height / 2), Size(rc.width / 2, rc.height / 2), 0, 0, 360, 255, CV_FILLED) ;
            Mat roi(hsv, rc) ;
            calcHist( &roi, 1, channels, mask, m_model3d, 3, hist_size, ranges ) ;
            m_rc = rc ;
            param.updated = false ;
            tracking = true ;
        }
        cap >> frame ;
        if ( frame.empty()) 
            break ;

        // image processing
        if ( tracking ) {
            // histogram backprojection
            calcBackProject(&hsv, 1, channels, m_model3d, m_backproj, ranges ) ;
            // tracking
            CamShift(m_backproj, m_rc, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1)) ;
            rectangle(frame, m_rc, Scalar(0, 0, 255), 3) ;
        }

        // image display
        imshow( "Tracker", frame ) ;

        // user input
        char ch = waitKey(33) ;
        if ( ch == 27 ) // esc == exit
            break ;
        else if (ch == 32) { // space == pause
            while ( (ch = waitKey(33)) != 32 && ch != 27 ) ;
            if (ch == 27)
                break ;
        }
    }

    return 0 ;
}