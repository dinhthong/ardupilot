/*
 * udpserver.c
 * http://www.pythonprasanna.com/Papers%20and%20Articles/Sockets/udpserver.c
 */

#include <cv.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <mutex>

using namespace std;
using namespace cv;

#define MAX_BUFFER_SIZE 1024

// variables

const int truhao = 25;

bool DetectObject = false;
Mat scale_img;

VideoCapture capture(0);
vector<vector<Point>> contours;
vector<Vec4i> hierarchy;

bool start = true;
bool notfound = true;
Rect roi;
int offset_x = 0;
int offset_y = 0;

int blur_val = 4;
int blur_max = 255;

int H_val = 41;
int H_max = 64;

int S_val = 78;
int S_max = 215;

int V_val = 78;
int V_max = 185;

Mat frame;
Mat img_blur, img_raw, img_HSV, threshol, img_result;

int iLastX = -1;
int iLastY = -1;

int err_x, err_y;

char text[50];

pthread_t server;
pthread_mutex_t mlock; 
struct sockaddr_in server_addr, client_addr;
int sock;

void init()
{
    /*namedWindow("Trackbar",0);
    createTrackbar("Blur", "Trackbar", &blur_val, blur_max);

    createTrackbar("H value: ", "Trackbar", &H_val, H_max);
    createTrackbar("H max: ", "Trackbar", &H_max, H_max);

    createTrackbar("S value: ", "Trackbar", &S_val, S_max);
    createTrackbar("S max: ", "Trackbar", &S_max, S_max);

    createTrackbar("V value: ", "Trackbar", &V_val, V_max);
    createTrackbar("V max: ", "Trackbar", &V_max, V_max);
    */
    //capture >> frame;
    // cv::resize(frame, scale_img, cv::Size(), 0.5, 0.5);
    img_result = scale_img.clone();
    // imshow("video", scale_img);
    blur(scale_img, img_blur, Size(blur_val * 2 + 1, blur_val * 2 + 1), Point(-1, -1), BORDER_DEFAULT);
    //imshow("Blur Image", img_blur);
    cvtColor(img_blur, img_HSV, COLOR_BGR2HSV);
    //imshow("HSV image", img_HSV);
    inRange(img_HSV, Scalar(H_val, S_val, V_val), Scalar(H_max, S_max, V_max), threshol);
    // imshow("Threshold",threshol);
    findContours(threshol, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    int area = 0;
    int index_max = 0;
    for (int i = 0; i < contours.size(); i++)
    {
        int area_i = contourArea(contours.at(i));
        if (area < area_i)
        {
            area = area_i;
            index_max = i;
        }
    }

    if (contours.size() > 0)
    {
        notfound = false;
        RotatedRect rect = minAreaRect(contours.at(index_max));
        putText(img_result,".",rect.center,FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(255,255,255),1);
        drawContours(img_result,contours,index_max,Scalar(255,255,255),1);
        cout << "(x,y): " << rect.center.x << " " << rect.center.y << " height: " << rect.size.height << " weight: " << rect.size.width << endl;
        cout << "(x,y) = " << rect.center.x << " " << rect.center.y << endl;

        // imshow("Object detect",img_result);

        offset_x = rect.center.x - rect.size.width / 2 - truhao;
        if (offset_x < 0)
            offset_x = 0;
        offset_y = rect.center.y - rect.size.height / 2 - truhao;
        if (offset_y < 0)
            offset_y = 0;

        //Rect roi;
        roi.x = offset_x;
        roi.y = offset_y;
        roi.width = rect.size.width + truhao * 2;
        if (roi.width > img_result.size().width - roi.x - 1)
            roi.width = img_result.size().width - roi.x - 1;

        roi.height = rect.size.height + truhao * 2;
        if (roi.height > img_result.size().height - roi.y - 1)
            roi.height = img_result.size().height - roi.y - 1;

        /* Crop the original image to the defined ROI */
        // cout << "x: " << roi.x << ", y: " << roi.y << ", width: " << roi.width << ", height: " << roi.height << ",image width: " << img_result.size().width << ", image height: " << img_result.size().height << "\n";
        cv::Mat crop = img_result(roi);
        // cv::imshow("crop", crop);
    }
    else
    {
        roi.x = 0;
        roi.y = 0;
        roi.width = scale_img.size().width;
        roi.height = scale_img.size().height;
    }
}

bool DetectMaker()
{
    cv::resize(scale_img, scale_img, cv::Size(), 0.5, 0.5);
    // if (roi.width + roi.x > scale_img.size().width)
    //     roi.width = scale_img.size().width - roi.x;
    // if (roi.height + roi.y > scale_img.size().height)
    //     roi.height = scale_img.size().height - roi.y;
    // Mat crop = scale_img(roi);
    img_result = scale_img.clone();
    // imshow("video", scale_img);
    blur(scale_img, scale_img, Size(blur_val * 2 + 1, blur_val * 2 + 1), Point(-1, -1), BORDER_DEFAULT);
    //imshow("Blur Image", img_blur);
    cvtColor(scale_img, scale_img, COLOR_BGR2HSV);
    //imshow("HSV image", img_HSV);
    inRange(scale_img, Scalar(H_val, S_val, V_val), Scalar(H_max, S_max, V_max), threshol);
    // imshow("Threshold",threshol);

    findContours(threshol, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    if (contours.size() == 0)
    {
        cout << "Not found object" << endl;
        imshow("Object",img_result);
        return false;
    }
    int area = 0;
    int index_max = 0;
    for (int i = 0; i < contours.size(); i++)
    {
        int area_i = contourArea(contours.at(i));
        if (area < area_i)
        {
            area = area_i;
            index_max = i;
        }
    }

    // Mat img_result_crop = scale_img.clone();

    RotatedRect rect = minAreaRect(contours.at(index_max));
    // putText(scale_img, ".", rect.center, FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255, 255, 255), 1);
    drawContours(img_result, contours, index_max, Scalar(255, 255, 255), 5);
    Rect bound = boundingRect(contours.at(index_max));
    rectangle(img_result, bound, Scalar(0, 255, 0), 5, 8, 0);
    //cout << "(x,y): " << rect.center.x+roi.x << " " << rect.center.y+roi.y << " height: " << rect.size.height << " weight: " << rect.size.width << endl;
    // cout << "(x,y) = " << 2 * (-scale_img.size().width / 2 + (int)(rect.center.x + roi.x)) << " " << 2 * (scale_img.size().height / 2 - (int)(rect.center.y + roi.y)) << endl;
    // err_x = (int)(2 * (-scale_img.size().width / 2 + (int)(rect.center.x + roi.x)));
    // err_y = (int)(2 * (scale_img.size().height / 2 - (int)(rect.center.y + roi.y)));
    // imshow("Object",img_result_crop);

    err_x = (int)(-scale_img.size().width / 2 + rect.center.x);
    // if (offset_x < 0)
    //     offset_x = 0;
    err_y = (int)(-scale_img.size().height / 2 + rect.center.y);
    circle(img_result, rect.center, 5, Scalar(0, 255, 0), 5, 8, 0);
    imshow("Object",img_result);
    // if (offset_y < 0)
    //     offset_y = 0;

    // //Rect roi;
    // roi.x = offset_x;
    // roi.y = offset_y;
    // roi.width = rect.size.width + truhao * 2;

    // roi.height = rect.size.height + truhao * 2;

    // imshow("Object detect",img_result);

    //return img_result;
    return true;
}

void *server_listen_thread(void *)
{
    struct sockaddr_in client;
    int addr_len = 0, bytes_read;
    char recv_data[1024];
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    server_addr.sin_addr.s_addr = inet_addr("10.42.0.231");

    if (bind(sock, (struct sockaddr *)&server_addr,
             sizeof(struct sockaddr)) == -1)
    {
        perror("Bind");
        exit(1);
    }

    addr_len = sizeof(struct sockaddr);

    client_addr.sin_addr.s_addr = -1;
    printf("\nUDPServer Waiting for client on port 8000\n");
    while (1) {
        bytes_read = recvfrom(sock, recv_data, 1024, 0,
                                (struct sockaddr *)&client, (socklen_t *)&addr_len);
        if (bytes_read > 0)
        {
            cout << "Someone connected: " << client.sin_addr.s_addr << endl;
            pthread_mutex_lock(&mlock);
            memcpy(&client_addr, &client, sizeof(struct sockaddr_in));
            pthread_mutex_unlock(&mlock);
        }
    }
}

int main()
{
    int frame_counter = 0;
    std::chrono::time_point<std::chrono::system_clock> start, end;	
    start = std::chrono::system_clock::now();
    fflush(stdout);

    if (pthread_mutex_init(&mlock, NULL) != 0) {
        cout << "Init mutex fail\n";
        return 1;
    }
    pthread_create(&server, NULL, &server_listen_thread, NULL);
    VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'),10, Size(320, 240)); 
    char send_data[MAX_BUFFER_SIZE];
    while (1)
    {
        capture >> scale_img;
        // imshow("cam", frame);
        // if (waitKey(1) == 'q') {
        //     break;
        // }
        // continue;
        if (scale_img.empty())
            continue;
        frame_counter++;
        // video.write(scale_img);
        // imshow("cam", scale_img);
        // if (notfound)
        // {
        //     init();
        //     continue;
        // }
        if (DetectMaker())
        {
            int len = sprintf(send_data, "s%d %d %d\n",1 , err_x, err_y);
            std::cout << send_data << std::endl;
            pthread_mutex_lock(&mlock);
            if (client_addr.sin_addr.s_addr != -1) {
                sendto(sock, send_data, len, 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));
            }
            pthread_mutex_unlock(&mlock);
        } else {
            int len = sprintf(send_data, "s%d %d %d\n", 0, err_x, err_y);
            std::cout << send_data << std::endl;
            pthread_mutex_lock(&mlock);
            if (client_addr.sin_addr.s_addr != -1) {
                sendto(sock, send_data, len, 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));
            }
            pthread_mutex_unlock(&mlock);
        }
        video.write(scale_img);
		// if (frame_counter >= 100)
		// {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        printf("fps: %.2f\n", 1.0f / elapsed_seconds.count());
        start = std::chrono::system_clock::now();
            // frame_counter = 0;
		// }
        waitKey(1);
    }
    video.release();
    pthread_mutex_destroy(&mlock); 
    return 0;
}
