//
//  gesture.hpp
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



#ifndef __P1__gesture__
#define __P1__gesture__

#include <vector>
#include <string>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "image.hpp"

using namespace cv;
using namespace std;



class Gesture {
    public:
        Image img;
        Gesture();

        int idxC;
        int idxFrame;
        int maxFreqFinger;
        int numDefects;

        bool isHand;
        bool detectHand();

        void drawFingerTips(Image *img);
        void getFingerNum(Image *img);
        void getFingersTips(Image *img);
        void initVectors();
        void printInfo(Mat src);
        void removeDefects(Image *img);
        void detectHandGesture(Image *img);

        vector<vector<Point>> contours;
        vector<vector<int>> intHull;
        vector<vector<Point>> pointHull;
        vector<vector<Vec4i>> defects;
        vector<Point> fingerTips;

        Rect rectangle;
        Rect b_rectangle;
        double b_rect_height;
        double b_rect_width;
    
    private:
        string bool_to_string(bool flag);
        string int_to_string(int num);
        
        int fontFace;
        int numNoFingers;
        int prevNumFinger;
        
        void analyzeContours();
        void checkOneFinger(Image *img);
        void computeNumFingers();
        void drawNumberToImg(Image *img);
        void insertNumberToVector();
        void removeEndPoints(vector<Vec4i>newDefects, Image *img);
        void removeFingerTips();
        
        float getAngle(Point p1, Point p2, Point p3);
        float getDistance(Point p1, Point p2);

        Scalar colorNum;
        
        vector<int> fingerNum;
        vector<int> numbersOnDisplay;
};


#endif /* defined(__P1__gesture__) */
