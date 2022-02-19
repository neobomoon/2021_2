// g++ $(pkg-config --cflags --libs opencv) 10_4_1.cpp -o ./exe/a

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv ;
using namespace std ;

int
main() {
    Mat descriptions1, descriptions2 ;
    Ptr<ORB> orbF = ORB::create(1000) ;
    vector<KeyPoint> keypoints1, keypoints2 ;
    vector<vector<DMatch> > matches ;
    vector<DMatch> goodMatches ;
    BFMatcher matcher(NORM_HAMMING) ;
    
    Mat query = imread( "resources/query.jpg" ) ;
    Mat image = imread( "resources/input.jpg" ) ;

    if ( query.empty() || image.empty() )
        return -1 ;

    // Compute ORB Features
    resize( image, image, Size(640, 480) ) ;
    orbF->detectAndCompute( query, noArray(), keypoints1, descriptions1 ) ;
    orbF->detectAndCompute( image, noArray(), keypoints2, descriptions2 ) ;

    // KNN Matching
    int k = 2 ;
    matcher.knnMatch( descriptions1, descriptions2, matches, k ) ;

    float nndr = 0.6f ;
    for ( int i = 0 ; i < matches.size() ; i++ ) {
        if ( matches.at(i).size() == 2 && 
                matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance ) {
                    goodMatches.push_back( matches[i][0] ) ;
                }
    }

    //Draw matching
    Mat imgMatches ;
    drawMatches( query, keypoints1, image, keypoints2, goodMatches, imgMatches,
    Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS ) ;

    imshow( "imgMatches", imgMatches ) ;

    waitKey( 0 ) ;
    return 0 ;
}