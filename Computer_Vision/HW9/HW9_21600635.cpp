// g++ $(pkg-config --cflags --libs opencv) HW9_21600635.cpp -o ./a
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <fstream>
#include <string>

using namespace cv;
using namespace std;
using namespace dnn;


Mat
get_foreground ( Mat src, Mat foreground, Rect rectangle) {

    Mat result, bgModel, fgModel, result2 ;

    grabCut(src, result, rectangle, bgModel, fgModel, 4, GC_INIT_WITH_RECT) ;

    compare(result, GC_PR_FGD, result, CMP_EQ) ;

    // image에 result(사람만 흰색)인 것을 덮는다. 이때 foreground는 복사 안 된다.
    src.copyTo(foreground, result) ; // (output, mask)

    return foreground ;
}


Mat
func1 ( Mat src ) {
    cout << "im func1" << endl ;

    String modelConfiguration = "deep_2/yolov2-tiny.cfg" ;
    String modelBinary = "deep_2/yolov2-tiny.weights" ;
    Net net = readNetFromDarknet(modelConfiguration, modelBinary) ;
    vector<String> classNamesVec ;
    ifstream classNamesFile("deep_2/coco.names") ;

    if ( classNamesFile.is_open() ) {
        string className = "" ;
        while (std::getline(classNamesFile, className)) 
            classNamesVec.push_back(className) ;
    }


    Mat result = src.clone() ;
    if ( result.channels() == 4 )
        cvtColor(result, result, COLOR_BGRA2BGR) ;

    // Convert Mat to batch of images
    Mat inputBlob = blobFromImage( result, 1 / 255.F, Size( 416, 416 ), Scalar(), true, false ) ;

    // set the network inputMat detection
    net.setInput( inputBlob, "data" ) ; 

    // compute output
    Mat detectionMat = net.forward( "detection_out" ) ;

    // by default
    float confidenceThreshold = 0.24 ;
    int count_person = 0 ;
    Mat foreground = Mat(result.size(), CV_8UC3, Scalar(0, 0, 0)) ;
    for ( int i = 0 ; i < detectionMat.rows ; i++ ) {
        const int probability_index = 5 ;
        const int probability_size = detectionMat.cols - probability_index ;
        float *prob_array_ptr = &detectionMat.at<float>( i, probability_index ) ;
        size_t objectClass = max_element( prob_array_ptr, prob_array_ptr + probability_size ) - prob_array_ptr ;
        // prediction probability of each class
        float confidence = detectionMat.at<float>( i, (int)objectClass + probability_index ) ;

        // for drawing labels with name and confidence
        if ( confidence > confidenceThreshold ) {
            float x_center = detectionMat.at<float>( i, 0 ) * result.cols ;
            float y_center = detectionMat.at<float>( i, 1 ) * result.rows ;
            float width = detectionMat.at<float>( i, 2 ) * result.cols ;
            float height = detectionMat.at<float>( i, 3 ) * result.rows ;
            Point p1( cvRound( x_center - width / 2 ), cvRound( y_center - height / 2 ) ) ;
            Point p2( cvRound( x_center + width / 2 ), cvRound( y_center + height / 2 ) ) ;

            // object에 box rect
            Rect object( p1, p2 ) ;

            // // classNamesVec[ objectClass ] == 이름
            String className = objectClass < classNamesVec.size() ? classNamesVec[ objectClass ] : cv::format( "unknown(%d)", objectClass ) ;


            if ( className.compare( "person?" ) ) {
                foreground = get_foreground( src, foreground, object ) ;
                count_person++ ;
            }
        }
    }
    putText( foreground, format( "There are %d people.", count_person), Point( 40, 40 ), FONT_HERSHEY_SIMPLEX, 1, Scalar( 255, 255, 255 ), 2 ) ;

    return foreground ;
}



Mat
func2 ( Mat src ) {
    Mat result = src.clone() ;
    cout << "im func2" << endl ;

    CascadeClassifier face_classifier ;
    Mat grayframe ;
    vector<Rect> faces ;

    //face detection configuration
    face_classifier.load( "./haarcascade_frontalface_alt.xml" ) ;


    // green
    cvtColor( result, grayframe, COLOR_BGR2GRAY ) ;
    face_classifier.detectMultiScale( grayframe, faces, 1.1, // increase search scale by 10% each pass
                                                        3, // merge groups of three detections
                                                        0, // not used for a new cascade
                                                        Size( 20, 20 ), // in size 제일 작은 사람 20
                                                        Size( 50, 50 ) ) ;

    // draw the results
    for ( int i = 0 ; i < faces.size() ; i++ ) {
        Point lb( faces[i].x + faces[i].width, faces[i].y + faces[i].height ) ;
        Point tr( faces[i].x, faces[i].y ) ;
        if ( faces[i].area() < 630 ) {// bgr
            rectangle( result, lb, tr, Scalar(255, 0, 0), 3, 4, 0 ) ; // farthest blue
        }
        else if ( faces[i].area() > 1285 && faces[i].area() < 2000 ) { // nearest green
            rectangle( result, lb, tr, Scalar(0, 255, 0), 3, 4, 0 ) ;
        }
        else { // other
            rectangle( result, lb, tr, Scalar(0, 255, 255), 3, 4, 0 ) ;
        }
    }

    return result ;
}


Mat
change_background ( Mat src, Mat foreground, Rect rectangle) {

    Mat result, bgModel, fgModel, result2 ;

    grabCut(src, result, rectangle, bgModel, fgModel, 4, GC_INIT_WITH_RECT) ;

    compare(result, GC_PR_FGD, result, CMP_EQ) ;

    // image에 result(사람만 흰색)인 것을 덮는다. 이때 foreground는 복사 안 된다.
    src.copyTo(foreground, result) ; // (output, mask)

    return foreground ;
}

Mat
func3 ( Mat src, Mat background ) {
    Mat result = background.clone() ;
    cout << "im func3" << endl ;

    CascadeClassifier face_classifier ;
    Mat grayframe ;
    vector<Rect> faces ;

    //face detection configuration
    face_classifier.load( "./haarcascade_frontalface_alt.xml" ) ;


    // green
    cvtColor( src, grayframe, COLOR_BGR2GRAY ) ;
    face_classifier.detectMultiScale( grayframe, faces, 1.1, // increase search scale by 10% each pass
                                                        3, // merge groups of three detections
                                                        0, // not used for a new cascade
                                                        Size( 20, 20 ), // in size 제일 작은 사람 20
                                                        Size( 50, 50 ) ) ;

    // draw the results
    for ( int i = 0 ; i < faces.size() ; i++ ) {
        Point lb( faces[i].x + faces[i].width, faces[i].y + faces[i].height ) ;
        Point tr( faces[i].x, faces[i].y ) ;

        Rect object( lb, tr ) ;
        result = change_background( src, result, object ) ;
    }

    return result ;
}




int
main () {
    VideoCapture cap ;

    if ( cap.open( "./Faces.mp4") == 0 ) {
        cout << "no such file" << endl ;
        return 0 ;
    }
    
    Mat result ;
    Mat background = imread( "./background.jpg" ) ;
    if ( background.empty() ) {
        cout << "no such file" << endl ;
        return 0 ;
    }
    resize(background, background, Size(640, 480));

    int key ;
    bool b = false, f = false, g = false ;
    while ( true ) {
        if ( cap.grab() == 0 )
            break ;

        // option
        if ( key == 98 ) // 'b'
            b = !b ;
        else if ( key == 102 )
            f = !f ;
        else if ( key == 103 )
            g = !g ;
        
        // operation
        cap.retrieve( result ) ;
        resize(result, result, Size(640, 480));
        if ( b ) {
            // func1
            result = func1( result ) ; 
        }
        else if ( f ) {
            // func2
            result = func2( result ) ;
        }
        else if ( g ) {
            // func3
            result = func3( result, background ) ;
        }

        imshow( "Result", result ) ;

        key = waitKey( 33 ) ;
    }

    cout << "end" << endl ;

    return 0 ;
}