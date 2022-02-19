// g++ $(pkg-config --cflags --libs opencv) HW5_21600635.cpp -o ./a

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv ;
using namespace std ;

Mat
negativeTransformation ( Mat src ) {

    Mat negative_img = src.clone() ;
    Mat HSV ;
    vector<Mat> hsv(3) ;

    cvtColor(negative_img, HSV, CV_BGR2HSV) ; // 에러 나옴

    split(HSV, hsv) ;

    int pixel_size = hsv[2].rows * hsv[2].cols * hsv[2].channels() ;

    for ( int i = 0 ; i < pixel_size ; i++) {
        hsv[2].data[i] = 255 - hsv[2].data[i] ;
    }

    merge(hsv, negative_img) ;
    cvtColor(negative_img, negative_img, CV_HSV2BGR) ;

    return negative_img ;
}

Mat
gammaTransformation ( Mat src ) {
    Mat gamma_img = src.clone() ;
    Mat HSV ;
    vector<Mat> hsv(3) ;

    cvtColor(gamma_img, HSV, CV_BGR2HSV) ;
    split(HSV, hsv) ;
    
    int pixel_size = hsv[2].rows * hsv[2].cols * hsv[2].channels() ;

    unsigned char look_up[256] ;
    int arr_size = sizeof(look_up) / sizeof(unsigned char) ;
    float gamma = 2.5 ;

    for ( int i = 0 ; i < arr_size ; i++ ) {
        look_up[i] = saturate_cast<uchar>( pow( i / 255.0, gamma) * 255.0) ;
    }

    for (int i = 0 ; i < pixel_size ; i++ ) {
        hsv[2].data[i] = look_up[ hsv[2].data[i] ] ;
    }

    merge(hsv, gamma_img) ;
    cvtColor(gamma_img, gamma_img, CV_HSV2BGR) ;

    return gamma_img ;
}

Mat
equalizeHistogram( Mat src ) {
    Mat equal_img = src.clone() ;
    Mat HSV ;
    vector<Mat> hsv(3) ;

    cvtColor(equal_img, HSV, CV_BGR2HSV) ;
    split(HSV, hsv) ;

    equalizeHist(hsv[2], hsv[2]) ;

    merge(hsv, equal_img) ;
    cvtColor(equal_img, equal_img, CV_HSV2BGR) ;

    return equal_img ;
}

Mat
lena_transformation ( Mat src ) {
    Mat image ;


    while ( true ) {
        char option ;
        cout << "Please press the option for lena image." << endl ;
        cout << "'n' is Negative transformation, 'g' is Gamma transformation, 'h' is Histogram equalization, 'r' is reset." << endl ;
        option = waitKey(0) ;

        if ( option == 'n' ) { // n == 110
            image = negativeTransformation( src ) ;
            break ;
        }
        else if ( option == 'g' ) { // g == 103
            image = gammaTransformation( src ) ;
            break ;
        }
        else if ( option == 'h' ) { // h == 104
            image = equalizeHistogram( src ) ;
            break ;
        }
        else if ( option == 'r' ) { // r == 114
            image = src.clone() ;
            break ;
        }
        else { // wrong option
            cout << "Wrong option. please press option key.1" << endl ;
            continue ;
        }
    }
    
    return image ;
}

Mat
colorSlicing( Mat src ) {
    Mat slicing_img = src.clone() ;
    Mat HSV ;
    vector<Mat> hsv(3) ;

    cvtColor(slicing_img, HSV, CV_BGR2HSV) ;

    split(HSV, hsv) ;

    // equalizeHist(hsv[2], hsv[2]) ;

    int pixel_size = hsv[2].rows * hsv[2].cols * hsv[2].channels() ;

    for( int i = 0 ; i < pixel_size ; i++ ) {
        if( hsv[0].data[i] > 9 && hsv[0].data[i] < 23 )
            hsv[1].data[i] = hsv[1].data[i] ;
        else
            hsv[1].data[i] = 0 ;
    } 

    merge(hsv, slicing_img) ;
    cvtColor(slicing_img, slicing_img, CV_HSV2BGR) ;


    return slicing_img ;
}

Mat
colorConversion( Mat src ) {
    Mat conv_img = src.clone() ;
    Mat HSV ;
    vector<Mat> hsv(3) ;

    cvtColor(conv_img, HSV, CV_BGR2HSV) ;

    split(HSV, hsv) ;

    int pixel_size = hsv[2].rows * hsv[2].cols * hsv[2].channels() ;

    for( int i = 0 ; i < pixel_size ; i++ ) {
        if ( hsv[0].data[i] > 129)
            hsv[0].data[i] = hsv[0].data[i] + 50 - 129 ;
        else
            hsv[0].data[i] = hsv[0].data[i] + 50 ;
    }

    merge(hsv, conv_img) ;
    cvtColor(conv_img, conv_img, CV_HSV2BGR) ;

    return conv_img ;
}

Mat
colorful_transformation( Mat src ){
    Mat image ;

    while ( true ) {
        char option ;
        cout << "Please press the option for colorful image." << endl ;
        cout << "'s' is Color slicing, 'c' is Color conversion, 'r' is reset." << endl ;
        option = waitKey(0) ;

        if ( option == 's' ) {
            image = colorSlicing( src ) ;
            break ;
        }
        else if ( option == 'c' ) {
            image = colorConversion( src ) ;
            break ;
        }
        else if ( option == 'r' ) {
            image = src.clone() ;
            break ;
        }
        else { // wrong option
            cout << "Wrong option. please press option key." << endl ;
            continue ;
        }
    }

    return image ;
}

Mat
averageFiltering( Mat src ) {
    Mat ave_img = src.clone() ;

    blur(ave_img, ave_img, Size(9, 9)) ;

    return ave_img ;
}

Mat
whiteBalancing( Mat src ) {
    Mat white_img = src.clone() ;
    Mat BGR[3] ;

    split(white_img, BGR) ;

    int pixel_size = white_img.rows * white_img.cols ;


    for( int c = 0 ; c < white_img.channels() ; c++ ) {
        int sum = 0 ;
        for( int i = 0 ; i < pixel_size ; i++ ) {
            sum += BGR[c].data[i] ;
        }

        double avg = sum / pixel_size ;

        for( int i = 0 ; i < pixel_size ; i++ ) {
            int temp = (128 / avg) * BGR[c].data[i] ;
            if( temp > 255)
                BGR[c].data[i] = 255 ;
            else 
                BGR[c].data[i] = temp ;
        }
    }
    merge(BGR, 3, white_img) ;

    return white_img ;
}


Mat
balancing_transformation( Mat src ) {
    Mat image ;

    while ( true ) {
        char option ;
        cout << "Please press the option for balancing image." << endl ;
        cout << "'a' is Average filtering, 'w' is White balancing, 'r' is reset." << endl ;
        option = waitKey(0) ;

        if ( option == 'a' ) {
            image = averageFiltering( src ) ;
            break ;
        }
        else if ( option == 'w' ) {
            image = whiteBalancing( src ) ;
            break ;
        }
        else if ( option == 'r' ) {
            image = src.clone() ;
            break ;
        }
        else { // wrong option
            cout << "Wrong option. please press option key." << endl ;
            continue ;
        }
    }

    return image ;
}


int
main () {
    Mat lena = imread("./resources/lena.png") ;
    Mat colorful = imread("./resources/colorful.jpg") ;
    Mat balancing = imread("./resources/balancing.jpg") ;

    // exception 
    if ( lena.empty() ||  colorful.empty() || balancing.empty()) {
        cout << "no such file" << endl ;
        return 0 ;
    }
    imshow("lena", lena) ;
    imshow("colorful", colorful) ;
    imshow("balancing", balancing) ;

    while ( 1 ) {
        // lena
        Mat lena_ = lena_transformation( lena ) ;

        // colorful
        Mat colorful_ = colorful_transformation( colorful ) ;

        // balancing
        Mat balancing_ = balancing_transformation( balancing ) ;

        // show the image
        destroyAllWindows() ;
        imshow("lena", lena_) ;
        imshow("colorful", colorful_) ;
        imshow("balancing", balancing_) ;
        // terminate case
        cout << "If you want to quit, press 'ESC' to terminate. If you want to continue, press any other key" << endl ;
        if ( waitKey(0) == 27 ) {
            break ;
        }
    }



    return 0 ;
}