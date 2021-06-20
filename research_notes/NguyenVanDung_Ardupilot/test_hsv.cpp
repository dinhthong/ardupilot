#include <cv.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

/*
int blur_val = 4;
int blur_max = 255;

int H_val = 163;
int H_max = 185;

int S_val = 115;
int S_max = 164;

int V_val = 142;
int V_max = 222;
*/

 /*int blur_val = 9;
 int blur_max = 255;

 int H_val = 0;
 int H_max = 195;

 int S_val = 66;
 int S_max = 202;

 int V_val = 79;
 int V_max = 198;
*/
/*
int blur_val = 4;
 int blur_max = 255;

 int H_val = 0;
 int H_max = 241;

 int S_val = 95;
 int S_max = 234;

 int V_val = 117;
 int V_max = 203;
*/

 int blur_val = 0;
 int blur_max = 255;

 int H_val = 0;
 int H_max = 255;

 int S_val = 0;
 int S_max = 255;

 int V_val = 0;
 int V_max = 255;

/*
int blur_val = 4;
int blur_max = 109;

int H_val = 90;
int H_max = 122;

int S_val = 135;
int S_max = 223;

int V_val = 90;
int V_max = 173;*/

/*
int blur_val = 4;
 int blur_max = 255;

 int H_val = 0;
 int H_max = 209;

 int S_val = 81;
 int S_max = 173;

 int V_val = 46;
 int V_max = 201;
*/
/*
int blur_val = 3;
int blur_max = 255;

int H_val = 0;
int H_max = 12;

int S_val = 145;
int S_max = 209;

int V_val = 121;
int V_max = 158;
*/

void init(){
    namedWindow("Trackbar",0);
    createTrackbar("Blur", "Trackbar", &blur_val, blur_max);
    
    createTrackbar("H value: ", "Trackbar", &H_val, H_max);
    createTrackbar("H max: ", "Trackbar", &H_max, H_max);
    
    createTrackbar("S value: ", "Trackbar", &S_val, S_max);
    createTrackbar("S max: ", "Trackbar", &S_max, S_max);
    
    createTrackbar("V value: ", "Trackbar", &V_val, V_max);
    createTrackbar("V max: ", "Trackbar", &V_max, V_max);
}

int main(){
    init();
    //VideoCapture capture("Video 1.wmv");
    VideoCapture stream(0);
    if (!stream.isOpened()) { //check if video device has been initialised
        cout << "cannot open camera";
    }
    Mat frame;
    //namedWindow( "video", 1);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat img_blur, img_raw, img_HSV, threshold, img_result;
    stream.read(frame);
	imshow("abc",frame);    
    img_result = frame.clone();
    bool flag = false;
    while (1){
        stream.read(frame);
        
        // if (waitKey(33) != 'a'){
        //     //img_raw = imread("Video 1.wmv",1);
        //     flag = !flag;
        // }
        // if (flag == false){
        //     capture >> frame;
        //     img_result = frame.clone();
        // }
        
        //capture >> frame;
        
        //imshow("Video 1", img_raw);
        imshow("video", frame);
        blur(frame,img_blur,Size(blur_val*2+1,blur_val*2+1), Point(-1,-1), BORDER_DEFAULT);
        imshow("Blur Image", img_blur);
        cvtColor(img_blur, img_HSV, COLOR_BGR2HSV);
        imshow("HSV image", img_HSV);
        inRange(img_HSV,Scalar(H_val,S_val,V_val),Scalar(H_max, S_max, V_max), threshold);
        imshow("Threshold",threshold);
        //findContours(threshold, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
        // for (int i=0;i<contours.size();i++){
        //     int area = contourArea(contours.at(i));
        //     if (area > 7000){
        //         RotatedRect rect = minAreaRect(contours.at(i));
        //         putText(img_result,"Object",rect.center,FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(255,255,255),1);
        //         drawContours(img_result,contours,i,Scalar(255,255,255),1);
        //     }
        // }
        // imshow("Object detect",img_result);
        
        if (waitKey(33) == 'q')
            break;
    }
    return 0;
}
//g++ main.cpp -o make `pkg-config --libs opencv` `pkg-config --cflags opencv`
