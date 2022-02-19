// g++ $(pkg-config --cflags --libs opencv) 8_2_7.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    Mat image = imread("./resources/dog.png") ;

    if ( image.empty() ) {
        cout << "no such file" << endl ;
        return 0 ;
    }

    Rect rectangle(65, 0, 300, 600) ;
    Mat result, bgModel, fgModel ;

    grabCut(image, result, rectangle, bgModel, fgModel, 10, GC_INIT_WITH_RECT) ;

    compare(result, GC_PR_FGD, result, CMP_EQ) ;


    

    Mat foreground = Mat(image.size(), CV_8UC3, Scalar(0, 0, 0)) ;

    // image에 result(사람만 흰색)인 것을 덮는다. 이때 foreground는 복사 안 된다.
    image.copyTo(foreground, result) ; // (output, mask)
    imshow("original", image) ;
    imshow("rectangle", image(rectangle)) ;
    imshow("result", result) ;
    imshow("Foreground", foreground) ;

    waitKey(0) ;
    return 0 ;
}