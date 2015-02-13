//
//  roi.hpp
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

#ifndef __P1__roi__
#define __P1__roi__

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;



class MyROI {
    public:
        MyROI();
        MyROI(Point upCorner, Point lowCorner, Mat soruce);

        Point upCorner;
        Point lowCorner;
        
        Mat roiPointer;
        Scalar color;
        int borderThick;
        void drawRectangle(Mat source);
};


#endif /* defined(__P1__roi__) */
