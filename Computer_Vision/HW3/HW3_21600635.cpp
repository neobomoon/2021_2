#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv ;
using namespace std ;

int
main () {
    // 1. Read "Lena.png"
    Mat lena = imread("./resources/Lena.png") ;
    
    if ( lena.empty() ) {
        cout << "no such file" << endl ;
        return 0 ;
    }

    Mat lena_filtered = lena.clone() ;
    int height = lena.rows ;
    int width = lena.cols ;

    // To perform filtering on the left half of the image
    Rect rect_1(0, 0, width/2, height) ;

    // Set the mask size as (7, 7)
    blur(lena(rect_1), lena_filtered(rect_1), Size(7, 7)) ;
    
    imshow("lena", lena) ;
    imshow("lena_Filtered", lena_filtered) ;


    // 2. Read "moon.png"
    Mat moon = imread("./resources/moon.png") ;

    if ( moon.empty() ) {
        cout << "no such file" << endl ;
        return 0 ;
    }

    Mat laplacian ;
    Mat abs_laplacian ;
    height = moon.rows ;
    width = moon.cols ;

    // To perform filtering on the right half of the image
    Rect rect_2(width/2, 0, width/2, height) ;

    // Unsharping masking
    GaussianBlur(moon, moon, Size(3, 3), 0, 0, BORDER_DEFAULT) ;
    Mat moon_filtered = moon.clone() ;

    // Second devation
    Laplacian(moon_filtered, laplacian, CV_16SC3, 1, 1, 0) ;
    convertScaleAbs(laplacian, abs_laplacian) ;
    
    // add second devation to origin image
    moon_filtered(rect_2) += abs_laplacian(rect_2) ;

    imshow("moon", moon) ;
    imshow("moon_filtered", moon_filtered) ;


    // 3. Read "saltnpepper.png"
    Mat saltnpepper = imread("./resources/saltnpepper.png") ;
    Mat saltnpepper_filtered ;

    if ( moon.empty() ) {
        cout << "no such file" << endl ;
        return 0 ;
    }

    // Perform median filtering(Set aperture size as 9)
    medianBlur(saltnpepper, saltnpepper_filtered, 9) ;

    imshow("saltnpepper", saltnpepper) ;
    imshow("saltnpepper_filtered", saltnpepper_filtered) ;

    waitKey(0) ;

    return 0 ;
}