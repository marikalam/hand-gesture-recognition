//
//  main.cpp
//  P1
//
//  Created by Masaya Ando, Marika Lee, and Jungwoo Shin.
//  Copyright (c) 2015 Masaya Ando, Marika Lee, Jungwoo Shin. All rights reserved.
//
//  Code Reference:
//      Title:      Hand Detection using Color Recognition
//      Author:     Simen Andresen (simena86)
//      URL:        https://github.com/simena86/handDetectionCV
//
//  Description:
//      Programming Assignment #1 for BU CS440: Artificial Intelligence. Our program
//      scans the user's palm from camera input and computes the average of skin color.
//      Using this color information, the program can detect hand gestures or count the
//      number of fingers open. This program extends the source code of "Hand Detection
//      using Color Recognition" by Simen Andresen.

#include <cmath>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/opencv.hpp>

#include "gesture.hpp"
#include "image.hpp"
#include "main.hpp"
#include "myroi.hpp"

using namespace cv;
using namespace std;

#define SAMPLESIZE 7



/**************** Global Variables ****************/

int fontFace = FONT_HERSHEY_PLAIN;
int squareLength;
int averageColor[SAMPLESIZE][3];
int cLower[SAMPLESIZE][3];
int cUpper[SAMPLESIZE][3];
int numDefects;
int kfFilterInit;

struct dimension {
    int height;
    int width;
} boundingDimension;

VideoWriter out;

Mat edges;

vector<MyROI> roi;
vector<KalmanFilter> kf;
vector<Mat_<float>> measurement;

MyROI roi1, roi2, roi3, roi4, roi5, roi6;


// initialize()
// Initializes the squareLength and kfFilterInit for Image img
void initialize(Image *img) {
    squareLength = 20;
    kfFilterInit = 0;
}


// changeColor()
// Changes a color from one spce to another
void changeColor(int hsv[3], int bgr[3], Mat source) {
    Mat averageBGR = source.clone();
    for (int i = 0; i < 3; i++) {
        averageBGR.data[i] = hsv[i];
    }
    cvtColor(averageBGR, averageBGR, CV_BGR2HLS);
    for (int i = 0; i < 3; i++) {
        bgr[i] = averageBGR.data[i];
    }
}

// printText()
// Prints texts into the image source
void printText(Mat source, string str) {
    int fontFace = FONT_HERSHEY_PLAIN;
    float fontSize = 1.5f;
    putText(source, str, Point(source.cols/2, source.rows/10), fontFace, fontSize, Scalar(200, 0, 0), 2);
}

// getMedian()
// Returns the median value of a vector
int getMedian(vector<int> value) {
    int median;
    size_t size = value.size();
    sort(value.begin(), value.end());
    
    if (size % 2 == 0) {
        median = value[size / 2 - 1];
    } else {
        median = value[size / 2];
    }
    return median;
}

// getAverageColor()
// Stores the average HSV values into a vector
void getAverageColor(Image *img, MyROI roi, int avg[3]) {
    Mat temp;
    roi.roiPointer.copyTo(temp);
    
    vector<int> hm;
    vector<int> sm;
    vector<int> vm;
    
    for (int i = 2; i < temp.rows - 2; i++) {
        for (int j = 2; j < temp.cols - 2; j++) {
            hm.push_back(temp.data[temp.channels() * (temp.cols * i + j) + 0]);
            sm.push_back(temp.data[temp.channels() * (temp.cols * i + j) + 1]);
            vm.push_back(temp.data[temp.channels() * (temp.cols * i + j) + 2]);
        }
    }
    
    avg[0] = getMedian(hm);
    avg[1] = getMedian(sm);
    avg[2] = getMedian(vm);
}

// readPalm()
// Scans the user's hand
void readPalm(Image *img) {
    img->capture >> img->source;
    flip(img->source, img->source, 1);
    roi.push_back(MyROI(Point(img->source.cols/3,   img->source.rows/6),    Point(img->source.cols/3+squareLength,      img->source.rows/6+squareLength),   img->source));
    roi.push_back(MyROI(Point(img->source.cols/4,   img->source.rows/2),    Point(img->source.cols/4+squareLength,      img->source.rows/2+squareLength),   img->source));
    roi.push_back(MyROI(Point(img->source.cols/3,   img->source.rows/1.5),  Point(img->source.cols/3+squareLength,      img->source.rows/1.5+squareLength), img->source));
    roi.push_back(MyROI(Point(img->source.cols/2,   img->source.rows/2),    Point(img->source.cols/2+squareLength,      img->source.rows/2+squareLength),   img->source));
    roi.push_back(MyROI(Point(img->source.cols/2.5, img->source.rows/2.5),  Point(img->source.cols/2.5+squareLength,    img->source.rows/2.5+squareLength), img->source));
    roi.push_back(MyROI(Point(img->source.cols/2,   img->source.rows/1.5),  Point(img->source.cols/2+squareLength,      img->source.rows/1.5+squareLength), img->source));
    roi.push_back(MyROI(Point(img->source.cols/2.5, img->source.rows/1.8),  Point(img->source.cols/2.5+squareLength,    img->source.rows/1.8+squareLength), img->source));
    
    for (int i = 0; i < 50; i++) {
        img->capture >> img->source;
        flip(img->source, img->source, 1);
        for (int j = 0; j < SAMPLESIZE; j++) {
            roi[j].drawRectangle(img->source);
        }
        string inst = string("Cover the rectangles with your palm");
        printText(img->source, inst);
        
        imshow("Output", img->source);
        out << img->source;
        if (waitKey(30) > - 0) {
            break;
        }
    }
}

// skinColorAvg()
// Computes the average skin color of the user's hand
void skinColorAvg(Image *img) {
    img->capture >> img->source;
    flip(img->source, img->source, 1);
    for (int i =0; i < 30; i++) {
        img->capture >> img->source;
        flip(img->source, img->source, 1);
        cvtColor(img->source, img->source, CV_BGR2HLS);
        for (int j = 0; j < SAMPLESIZE; j++) {
            getAverageColor(img, roi[j], averageColor[j]);
            roi[j].drawRectangle(img->source);
        }
        cvtColor(img->source, img->source, CV_HLS2BGR);
        string inst = string("Finding average color of your hand. Please hold...");
        printText(img->source, inst);
        imshow("Output", img->source);
        if (waitKey(30) >= 0) {
            break;
        }
    }
}

// initializeTrackbar()
// Initializes the trackbar
void initializeTrackbar() {
    for (int i = 0; i < SAMPLESIZE; i++) {
        cLower[i][0] = 12;
        cUpper[i][0] = 7;
        cLower[i][1] = 30;
        cUpper[i][1] = 40;
        cLower[i][2] = 80;
        cUpper[i][2] = 80;
    }
    
    createTrackbar("lower1", "Trackbar", &cLower[0][0], 255);
    createTrackbar("lower2", "Trackbar", &cLower[0][1], 255);
    createTrackbar("lower3", "Trackbar", &cLower[0][2], 255);
    createTrackbar("upper1", "Trackbar", &cUpper[0][0], 255);
    createTrackbar("upper2", "Trackbar", &cUpper[0][1], 255);
    createTrackbar("upper3", "Trackbar", &cUpper[0][2], 255);
}

// initializeWindows()
// Initializes the window for video output
void initalizeWindow(Image img) {
    namedWindow("Trackbars", CV_WINDOW_KEEPRATIO);
    namedWindow("Output", CV_WINDOW_FULLSCREEN);
}

// showWindows()
// Opens the windows and displays the output to user
void showWindows(Image img) {
    pyrDown(img.imgProcess, img.imgProcess);
    pyrDown(img.imgProcess, img.imgProcess);
    
    Rect roi(Point(3 * img.source.cols/4, 0), img.imgProcess.size());
    vector<Mat> channels;
    Mat result;
    
    for (int i = 0; i < 3; i++) {
        channels.push_back(img.imgProcess);
    }
    
    merge(channels, result);
    result.copyTo(img.source(roi));
    imshow("Output", img.source);
}

// normalizeColor()
// Normalizes the scanned skin color
void normalizeColor(Image *img) {
    for (int i = 1; i < SAMPLESIZE; i++) {
        for (int j = 0; j < 3; j++) {
            cLower[i][j] = cLower[0][j];
            cUpper[i][j] = cUpper[0][j];
        }
    }
    
    for (int i = 0 ; i < SAMPLESIZE; i++) {
        if ((averageColor[i][0] - cLower[i][0]) < 0) {
            cLower[i][0] = averageColor[i][0];
        }
        if ((averageColor[i][1] - cLower[i][1]) < 0) {
            cLower[i][1] = averageColor[i][1];
        }
        if ((averageColor[i][2] - cLower[i][2]) < 0) {
            cLower[i][2] = averageColor[i][2];
        }
        if ((averageColor[i][0] + cUpper[i][0]) > 255) {
            cUpper[i][0] = averageColor[i][0];
        }
        if ((averageColor[i][1] + cUpper[i][1]) > 255) {
            cUpper[i][1] = averageColor[i][1];
        }
        if ((averageColor[i][2] + cUpper[i][2]) < 255) {
            cUpper[i][2] = averageColor[i][2];
        }
    }
}

// produceBinary()
// Gets the loewr and upper bound of the skin coor and copy to imgProcessList
void produceBinary(Image *img) {
    Scalar lowBound;
    Scalar upBound;
    
    for (int i = 0; i < SAMPLESIZE; i++) {
        normalizeColor(img);
        lowBound = Scalar(averageColor[i][0] - cLower[i][0], averageColor[i][1] - cLower[i][1], averageColor[i][2] - cLower[i][2]);
        upBound  = Scalar(averageColor[i][0] + cUpper[i][0], averageColor[i][1] + cUpper[i][1], averageColor[i][2] + cUpper[i][2]);
     
        img->imgProcessList.push_back(Mat(img->sourceLR.rows, img->sourceLR.cols, CV_8U));
        inRange(img->sourceLR, lowBound, upBound, img->imgProcessList[i]);
    }
    
    img->imgProcessList[0].copyTo(img->imgProcess);
    for (int i = 1; i < SAMPLESIZE; i++) {
        img->imgProcess += img->imgProcessList[i];
    }
    medianBlur(img->imgProcess, img->imgProcess, 7);
}

// findMaxContour()
// Finds the largest contour in the source
int findMaxContour(vector<vector<Point>> contours) {
    int idxMaxContour = -1;
    int sizeMaxContour = 0;
    for (int i = 0; i < contours.size(); i++) {
        if (contours[i].size() > sizeMaxContour) {
            idxMaxContour = i;
            sizeMaxContour = int(contours[i].size());
        }
    }
    return idxMaxContour;
}

// myDrawContours()
// Draws the contour of the hand
void myDrawContours(Image *img, Gesture *gesture) {
    drawContours(img->source, gesture->pointHull, gesture->idxC, Scalar(200, 0, 0), 2, 8, vector<Vec4i>(), 0, Point());
    
    rectangle(img->source, gesture->b_rectangle.tl(), gesture->b_rectangle.br(), Scalar(0, 0, 200));
    vector<Vec4i>::iterator d = gesture->defects[gesture->idxC].begin();
    
    vector<Mat> channels;
    Mat result;
    
    for (int i = 0; i < 3; i++) {
        channels.push_back(img->imgProcess);
    }
    merge(channels, result);
    drawContours(result, gesture->pointHull, gesture->idxC, Scalar(0, 0, 250), 10, 8, vector<Vec4i>(), 0, Point());
    
    while (d != gesture->defects[gesture->idxC].end()) {
        Vec4i& v = (*d);
        
        int idxStart = v[0];
        Point pStart(gesture->contours[gesture->idxC][idxStart]);
        int idxEnd = v[1];
        Point pEnd(gesture->contours[gesture->idxC][idxEnd]);
        int idxFar = v[2];
        Point pFar(gesture->contours[gesture->idxC][idxFar]);
        
        circle(result, pFar, 9, Scalar(0, 205, 0), 5);
        d++;
    }
}


// makeContours()
// Creates the contour of the hand using Convex hull
void makeContours(Image *img, Gesture *gesture) {
    Mat _imgProcess;
    pyrUp(img->imgProcess, img->imgProcess);
    img->imgProcess.copyTo(_imgProcess);
    
    findContours(_imgProcess, gesture->contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    
    gesture->initVectors();
    gesture->idxC = findMaxContour(gesture->contours);
    
    if (gesture->idxC != -1) {
        gesture->b_rectangle = boundingRect(Mat(gesture->contours[gesture->idxC]));
        convexHull(Mat(gesture->contours[gesture->idxC]), gesture->pointHull[gesture->idxC], false, true);
        convexHull(Mat(gesture->contours[gesture->idxC]), gesture->intHull[gesture->idxC], false, false);
        approxPolyDP(Mat(gesture->pointHull[gesture->idxC]), gesture->pointHull[gesture->idxC], 18, true);
        
        if (gesture->contours[gesture->idxC].size() > 3) {
            convexityDefects(gesture->contours[gesture->idxC], gesture->intHull[gesture->idxC], gesture->defects[gesture->idxC]);
            gesture->removeDefects(img);
        }
        
        bool isHand = gesture->detectHand();
        gesture->printInfo(img->source);
        
        if (isHand) {
            gesture->getFingersTips(img);
            gesture->drawFingerTips(img);
            myDrawContours(img, gesture);
        }
    }
}

// main()
// Entry point into Hand Gesture Recognition
int main(int argc, const char * argv[]) {
    
    // Initialize Image class
    Image img(0);
    
    // Initialize handgesture class
    Gesture gesture;
    
    // Populate img
    initialize(&img);
    
    // Create window and open video
    img.capture >> img.source;
    namedWindow("Ouput", CV_WINDOW_KEEPRATIO);
    out.open("output.avi", CV_FOURCC('M', 'J', 'P', 'G'), 15, img.source.size(), true);
    
    // Scan user's palm
    readPalm(&img);
    skinColorAvg(&img);
    destroyWindow("Output");
    
    // Re-initialize Windows and Trackbars
    initalizeWindow(img);
    initializeTrackbar();
    
    // Enter main loop for hand gesture detection
    for (;;) {
        gesture.idxFrame++;
        img.capture >> img.source;
        flip(img.source, img.source, 1);
        pyrDown(img.source, img.sourceLR);
        blur(img.sourceLR, img.sourceLR, Size(3,3));
        cvtColor(img.sourceLR, img.sourceLR, CV_BGR2HLS);
        
        // Detect lower and upper bound of skin color
        produceBinary(&img);
        
        cvtColor(img.sourceLR, img.sourceLR, CV_HLS2BGR);
        
        // Draw contours around the hand
        makeContours(&img, &gesture);
        
        // Find the number of fingers
        gesture.getFingerNum(&img);
        
        // Launch the window with the image
        showWindows(img);
        out << img.source;
        
        if (waitKey(30) == char('q')) {
            break;
        }
    }
    destroyAllWindows();
    out.release();
    img.capture.release();
    return 0;
}
