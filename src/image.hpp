//
//  image.hpp
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



#ifndef __P1__image__
#define __P1__image__

#include <vector>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;



class Image {
    public:
        Image();
        Image(int camera);

        Mat imgProcess;
        Mat source;
        Mat sourceLR;

        vector<Mat> imgProcessList;
        VideoCapture capture;
        int cameraSource;
        void initCamera(int i);
};


#endif /* defined(__P1__image__) */
