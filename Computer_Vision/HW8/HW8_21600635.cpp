// g++ $(pkg-config --cflags --libs opencv) HW8_21600635.cpp -o ./a

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv ;
using namespace std ;

Mat
background_img ( VideoCapture src ) {
    Mat background ;
    Mat image ;
    Mat gray ;
    Mat avg ;
    src >> avg ;
    cvtColor( avg, avg, CV_RGB2GRAY ) ;

    int cnt = 2 ;
    
    while ( true ) {
        if ( !src.read( image ) )
            break ;
        cvtColor( image, gray, CV_BGR2GRAY ) ;
        add(gray / cnt, avg * (cnt - 1) / cnt, avg) ;
        cnt++ ;

        int current_frame = src.get( CAP_PROP_POS_FRAMES ) ;
    }

    return avg ;
}

Mat
draw_rect ( Mat src, Mat foregroundMask ) {
    vector<vector<Point> > contours ;
    vector<Vec4i>hierarchy ;
    findContours( foregroundMask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE ) ;
    

    // defining bounding rectangle
    vector<Rect> boundRect(contours.size()) ;
    for ( int i = 0 ; i < contours.size() ; i++ ) {
        boundRect[i] = boundingRect(Mat(contours[i])) ;
    }
    // draw rectangles on the contours
    int count = 0 ;
    for ( int i = 0 ; i < contours.size() ; i++ ) {
        if (boundRect[i].width * boundRect[i].height > 400) {
            count++ ;
            rectangle( src, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0), 2, 8 ,0) ;
        }
    }
    putText(src, format("object: %d", count), Point(5, 20), 1, 0.8, Scalar(255, 255, 255), 1, 8) ; 



    return src ;
}

int
main () {
    VideoCapture capture ;
    VideoCapture capture_for_background ;
    // open mp4 file
    if ( capture.open( "Faces.mp4") == 0 || capture_for_background.open("Faces.mp4") == 0 ) {
        cout << "no such file" << endl ;
        return 0 ;
    }

    Mat background = background_img( capture_for_background ) ;
    Mat gray ;
    Mat image ;
    Mat foregroundMask ;
    imshow( "background", background ) ;

    cout << "pass" << endl ;
    while ( true ) {

        if ( capture.grab() == 0 )
            break ;

        capture.retrieve( image ) ;
        cvtColor( image, gray, CV_BGR2GRAY ) ;
        
        absdiff( background, gray, foregroundMask ) ;
        threshold( foregroundMask, foregroundMask, 40, 255, CV_THRESH_BINARY ) ;
        gray = draw_rect ( gray, foregroundMask ) ;
        imshow( "Result(x, y)", foregroundMask ) ;
        imshow( "result", gray) ;


        waitKey( 33 ) ;
    }
    return 0 ;
}