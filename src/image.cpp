//
//  image.cpp
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


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "image.hpp"

using namespace cv;



Image::Image() {
}

Image::Image(int camera) {
    source = camera;
    capture = VideoCapture(camera);
}