//
//  gesture.cpp
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



#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "gesture.hpp"

using namespace cv;
using namespace std;

#define PI 3.14159



/*************** Public Functions ****************/

Gesture::Gesture() {
    idxFrame = 0;
    numNoFingers = 0;
    fontFace = FONT_HERSHEY_PLAIN;
}

// detectHand()
// Detects hand by analyzing the dimension of the contours
bool Gesture::detectHand() {
    analyzeContours();
    double height = b_rect_height;
    double width = b_rect_width;
    
    isHand = true;
    
    if (fingerTips.size() > 5) {
        isHand = false;
    } else if (height == 0 || width == 0) {
        isHand = false;
    } else if (height / width > 4 || width / height > 4) {
        isHand = false;
    } else if (b_rectangle.x < 20) {
        isHand = false;
    }
    
    return isHand;
}

// drawFingerTips()
// Draws circles around the detected finger tips
void Gesture::drawFingerTips(Image *img) {
    Point p;
    float fontSize = 1.2f;
    for (int i = 0; i < fingerTips.size(); i++) {
        p = fingerTips[i];
        
        cout << "Detected: " << i << endl;
        
        putText(img->source, int_to_string(i), p - Point(0, 30), fontFace, fontSize, Scalar(200, 200, 200), 2);
        circle(img->source, p, 5, Scalar(100, 255, 100), 4);
    }
}

// deetectGesture()
// Detects the number of fingers and recognize gestures
void Gesture::detectHandGesture(Image *img) {
    Point p;
    float fontSize = 1.2f;
    
    for (int i = 0; i < fingerTips.size(); i++) {
        p = fingerTips[i];
        
        string gestureStr;
        
        if (i == 0) {
            gestureStr = "Fist";
            cout << "Fist" << endl;
        } else if (i == 2) {
            gestureStr = "Peace";
            cout << "Peace" << endl;
        } else if (i == 5) {
            gestureStr = "Hi-Five";
            cout << "Hi-Five" << endl;
        }
    }
}

// getFingerNum()
// Calculates the most frequent number of fingers over 20 frames
void Gesture::getFingerNum(Image *img) {
    removeFingerTips();
    
    if (b_rectangle.height > img->source.rows/2 && numNoFingers > 12 && isHand) {
        colorNum = Scalar(0, 200, 0);
        insertNumberToVector();
        
        if (idxFrame > 12) {
            numNoFingers = 0;
            idxFrame = 0;
            computeNumFingers();
            numbersOnDisplay.push_back(maxFreqFinger);
            fingerNum.clear();
        } else {
            idxFrame++;
        }
    } else {
        numNoFingers++;
        colorNum = Scalar(200, 200, 200);
    }
    //drawNumberToImg(img);
    detectHandGesture(img);
}

// getFingerTips()
// Gets the positions of the finger tips
void Gesture::getFingersTips(Image *img) {
    fingerTips.clear();
    
    int i = 0;
    vector<Vec4i>::iterator d = defects[idxC].begin();
    
    while (d != defects[idxC].end()) {
        Vec4i& v = (*d);
        int idxStart = v[0];
        Point pStart(contours[idxC][idxStart]);
        
        int idxEnd = v[1];
        Point pEnd(contours[idxC][idxEnd]);
        
        int idxFar = v[2];
        Point pFar(contours[idxC][idxFar]);
        
        if (i == 0) {
            fingerTips.push_back(pStart);
            i++;
        }
        fingerTips.push_back(pEnd);
        d++;
        i++;
    }
    if (fingerTips.size() == 0) {
        checkOneFinger(img);
    }
}

// initVectors()
// Initialize intHull, pointHull, defects vectors
void Gesture::initVectors() {
    intHull = vector<vector<int>>(contours.size());
    pointHull = vector<vector<Point>>(contours.size());
    defects = vector<vector<Vec4i>>(contours.size());
}
void Gesture::printText1(Mat src, string text, int x, int y){
    int fontFace = FONT_HERSHEY_PLAIN;
    putText(src,text,Point(x, y),fontFace, 2.3f,Scalar(200,0,0),2);
}
// printInfo()
// Prints useful information about the current img->source
void Gesture::printInfo(Mat src) {
    int fontFace = FONT_HERSHEY_PLAIN;
    Scalar fColor(245,200,200);
    int xpos=src.cols/1.5;
    int ypos=src.rows/1.6;
    float fontSize=0.7f;
    int lineChange=14;
    string info= "Figure info:";
    putText(src,info,Point(ypos,xpos),fontFace,fontSize,fColor);
    xpos+=lineChange;
    info=string("Number of fingers: ") + string(int_to_string((numDefects+1))) ;
    //	putText(src,info,Point(ypos,xpos),fontFace,fontSize ,fColor);
    printText1(src,info,src.cols/2, src.rows/10+200);
    
    
    
    
    
    
    
    //
    //
    //    int nono_count=0;
    //    if (fingerTips.size()==1){
    //
    //
    //        Point p = fingerTips[0];
    //        int k=0;
    //        putText(m->src,intToString(0),p-Point(0,30),fontFace, 1.2f,Scalar(200,200,200),2);
    //        circle( m->src,p,   5, Scalar(100,255,100), 4 );
    //
    //
    //    }
    
    
    //    if (nrOfDefects==1) {
    //        printText1(src,"HEllo it's one finger",src.cols/2, src.rows/10+150);
    //
    //    }  if (nrOfDefects==0) {
    //        printText1(src,"It's a fist",src.cols/2, src.rows/10+150);
    //
    //    }
    //    printText1(src,"fingerTips.size(): " + intToString((int)fingerTips.size()),src.cols/2, src.rows/10+150);
    
    //    if (fingerTips.size()==1) {
    //        printText1(src,"HEllo it's one finger",src.cols/2, src.rows/10+150);
    //
    //    }  if (nrOfDefects==0) {
    //        printText1(src,"It's a fist",src.cols/2, src.rows/10+150);
    //
    //    }
    
    xpos+=lineChange;
    //	info=string("bounding box height, width ") + string(intToString(bRect_height)) + string(" , ") +  string(intToString(bRect_width)) ;
    //	putText(src,info,Point(ypos,xpos),fontFace,fontSize ,fColor);
    //    printText1(src,info,src.cols/2, src.rows/10+50);
    
    xpos+=lineChange;
    //	info=string("Is hand: ") + string(bool2string(isHand));
    //	putText(src,info,Point(ypos,xpos),fontFace,fontSize  ,fColor);
    //    printText1(src,info,src.cols/2, src.rows/10+100);
}

// removeDefects()
// Detects unwanted defects and removes the endpoints from the detection
void Gesture::removeDefects(Image *img) {
    int idxStart, idxEnd, idxFar;
    int valTolerance = b_rect_height / 5;
    float angleTolerance = 100;
    
    vector<Vec4i> newDefects;
    vector<Vec4i>::iterator d = defects[idxC].begin();
    
    while (d != defects[idxC].end()) {
        Vec4i& v = (*d);
        
        idxStart = v[0];
        idxEnd = v[1];
        idxFar = v[2];
        Point pStart(contours[idxC][idxStart]);
        Point pEnd(contours[idxC][idxEnd]);
        Point pFar(contours[idxC][idxFar]);
        
        if (getDistance(pStart, pFar) > valTolerance && getDistance(pEnd, pFar) > valTolerance && getAngle(pStart, pFar, pEnd) < angleTolerance) {
            if (pEnd.y > (b_rectangle.y + b_rectangle.height - b_rectangle.height / 4)) {
                // Skip
            } else if (pStart.y > b_rectangle.y + b_rectangle.height - b_rectangle.height/4) {
                // Skip
            } else {
                newDefects.push_back(v);
            }
        }
        d++;
    }
    numDefects = int(newDefects.size());
    defects[idxC].swap(newDefects);
    removeEndPoints(defects[idxC], img);
}



/***************** Private Functions ****************/

// bool_to_string()
// Converts boolean values into string
string Gesture::bool_to_string(bool flag) {
    if (flag) {
        return "Yes";
    } else {
        return "No";
    }
}

// int_to_string()
// Converts integer values into string
string Gesture::int_to_string(int num) {
    stringstream s;
    s << num;
    return s.str();
}

// analyzeContours()
// Analyzes the contour of the b_rectangle by its dimensions
void Gesture::analyzeContours() {
    b_rect_height = b_rectangle.height;
    b_rect_width = b_rectangle.width;
}

// checkOneFinger()
// Checks for convexity defects on one finger
void Gesture::checkOneFinger(Image *img) {
    int yTolerance = b_rectangle.height / 6;
    Point maxPoint;
    maxPoint.y = img->source.rows;
    vector<Point>::iterator d = contours[idxC].begin();
    
    while (d != contours[idxC].end()) {
        Point v = (*d);
        if (v.y < maxPoint.y) {
            maxPoint = v;
            cout << maxPoint.y << endl;
        }
        d++;
    }
    
    int n = 0;
    d = pointHull[idxC].begin();
    
    while (d != pointHull[idxC].end()) {
        Point v = (*d);
        cout << "x: " << v.x << " y: " << v.y << "maxPoint.y: " << maxPoint.y << " y Tolerance: " << yTolerance << endl;
        
        if (v.y < maxPoint.y + yTolerance && v.y != maxPoint.y && v.x != maxPoint.x) {
            n++;
        }
        d++;
    }
    if (n == 0) {
        fingerTips.push_back(maxPoint);
    }
}

// computeNumFingers()
// Computes the number of fingers
void Gesture::computeNumFingers() {
    int idxFrequency;
    int thisFrequency = 1;
    int maxFrequency = 1;
    
    std::sort(fingerNum.begin(), fingerNum.end());
    
    idxFrequency = fingerNum[0];
    
    for (int i = 1; i < fingerNum.size(); i++) {
        if (fingerNum[i-1] != fingerNum[i]) {
            if (thisFrequency > maxFrequency) {
                idxFrequency = fingerNum[i - 1];
                maxFrequency = thisFrequency;
            }
            thisFrequency = 0;
        }
        thisFrequency++;
    }
    if (thisFrequency > maxFrequency) {
        idxFrequency = fingerNum[fingerNum.size() - 1];
    }
    maxFreqFinger = idxFrequency;
}

// drawNumberToImg()
// Draws the calculated number of fingers to the img->source
void Gesture::drawNumberToImg(Image *img) {
    int x = 10;
    int y = 10;
    int offset = 30;
    int fontFace = FONT_HERSHEY_PLAIN;
    float fontSize = 1.5f;
    
    for (int i = 0; i < numbersOnDisplay.size(); i++) {
        cv::rectangle(img->source, Point(x, y), Point(x+offset, y+offset), colorNum, 2);
        putText(img->source, int_to_string(numbersOnDisplay[i]), Point(x+7, y+offset-3), fontFace, fontSize, colorNum);
        x += 40;
        if (x > (img->source.cols - img->source.cols/3.2)) {
            y += 40;
            x = 10;
        }
    }
}

// insertNumberToVector()
// Pushes the number of detected finger tip to the image source
void Gesture::insertNumberToVector() {
    int size = int(fingerTips.size());
    fingerNum.push_back(size);
}

// removeEndPoints()
// Removes endpoints of convexity defects if the finger tips of overlapped
void Gesture::removeEndPoints(vector<Vec4i> newDefects, Image *img) {
    Vec4i temp;
    int idxStartA, idxEndA;
    int idxStartB, idxEndB;
    
    float valTolerance = b_rect_width / 6;
    
    for (int i = 0; i < newDefects.size(); i++) {
        for (int j = i; j < newDefects.size(); j++) {
            idxStartA = newDefects[i][0];
            Point pStartA(contours[idxC][idxStartA]);
            
            idxEndA = newDefects[i][1];
            Point pEndA(contours[idxC][idxEndA]);
            
            idxStartB = newDefects[j][0];
            Point pStartB(contours[idxC][idxStartB]);
            
            idxEndB = newDefects[j][1];
            Point pEndB(contours[idxC][idxEndB]);
            
            if (getDistance(pStartA, pEndB) < valTolerance) {
                contours[idxC][idxStartA] = pEndB;
                break;
            }
            
            if (getDistance(pEndA, pStartB) < valTolerance) {
                contours[idxC][idxStartB] = pEndA;
            }
        }
    }
}

// removeFingerTips()
// Removes redundant fingertips that are too close to each other
void Gesture::removeFingerTips() {
    vector<Point> newFingers;
    for (int i = 0; i < fingerTips.size(); i++) {
        for (int j = i; j < fingerTips.size(); j++) {
            if (getDistance(fingerTips[i], fingerTips[j]) < 10 && i != j) {
                // Skip
            } else {
                newFingers.push_back(fingerTips[i]);
                break;
            }
        }
    }
    fingerTips.swap(newFingers);
}

// getAngle()
// Returns the angle between three points
float Gesture::getAngle(Point p1, Point p2, Point p3) {
    float a = getDistance(p2, p1);
    float b = getDistance(p2 ,p3);
    float dot = (p1.x - p2.x) * (p3.x - p2.x) + (p1.y - p2.y) * (p3.y - p2.y);
    float angle = acos(dot / (a * b));
    return angle * 180 / PI;
}

// getDistance()
// Returns the distance between two points
float Gesture::getDistance(Point p1, Point p2) {
    return sqrt(fabs(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2)));
}
