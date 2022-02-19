//
//  assignment#11_21600786.cpp
//  opencv_project
//
//  Created by 홍승훈 on 2020/11/09.
//  Copyright © 2020 홍승훈. All rights reserved.
//
// g++ $(pkg-config --cflags --libs opencv) test.cpp -o ./a
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


#define TRUE 1
#define FALSE 0


/* Algoritm
        Assignment 11 -> optimization
*/
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

int main() {
    VideoCapture capture("Faces.mp4");
    VideoCapture capture_back("Faces.mp4");
    Mat background, image_back, image_diff, gray, result, foregroundMask, fouregroundImg, avg, sum;
    Mat element_1 = getStructuringElement(MORPH_RECT, Size(15,40));
    Mat element_2 = getStructuringElement(MORPH_RECT, Size(8,10));
    Mat element_3 = getStructuringElement(MORPH_ELLIPSE, Size(5,5));
    Mat opening, closing, dilation, erosion;
    int count = 0;
    int time = 0;
    //1.  first frame is background image
    background = background_img( capture_back ) ;
    
    // 2. generate a binary image, Threshold = 20 <- can be changed, i think 50 is better
    while(TRUE){
        if((capture.grab() == 0) )
            break;
        time = capture.get(CAP_PROP_POS_MSEC) / 1000;
        capture.retrieve(image_diff);
        cvtColor(image_diff, gray, CV_BGR2GRAY);
        
        absdiff(background, gray, foregroundMask);
        threshold(foregroundMask, foregroundMask, 80, 255, CV_THRESH_BINARY);
        foregroundMask.copyTo(fouregroundImg);
        gray.copyTo(fouregroundImg, foregroundMask);
        // imshow("test", fouregroundImg) ;
    // 3. morphological operation  -> closing method?
        dilate(foregroundMask, dilation, element_1);
        erode(dilation, erosion, element_2);
        imshow("test", foregroundMask) ;
    // 4. Making a box for moving objects(which is bigger than 400 pixels).
    // 5. print out how many boxes in image
        //threshold(foregroundMask, result, 180, 255, THRESH_BINARY_INV);
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours(erosion, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        vector<Rect> boundRect(contours.size());
        
        for(int i =0; i<contours.size(); i++){
            boundRect[i] = boundingRect(Mat(contours[i]));
        }
        for(int i=0; i<contours.size(); i++)
        {
            if (boundRect[i].width * boundRect[i].height > 10000) {
            count++ ;
            rectangle( image_diff, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0), 2, 8 ,0) ;
        }
            
        }
        putText(image_diff, format("Number of people:  %d", count), Point(50,80), FONT_HERSHEY_SIMPLEX, 1, Scalar(255), 4);

        
    // 6. Display only one video
        imshow("Number of people", image_diff);
        waitKey(33);
        count = 0;
    }
    
}
