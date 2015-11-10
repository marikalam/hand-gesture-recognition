# Hand-Gesture-Recognition-With-OpenCV

The goal of this assignment is to design and implement algorithms that delineate objects in video images
and analyze their shapes.  The program designs and implements algorithms that delineate hand shapes or gestures and create a graphical display that responds to the recognition of the hand shapes or gestures.

####Method and Implementation

#####Skin Detection

Skin color detection is a crucial component of our hand gesture detector because it allows the program to read the user's skin color at runtime and determine the skin color via color recognition. Because skin colors varies from person to person, it would be difficult to hard-code presets of colors into the code to let the program determine which part of the image is skin or not. 

Our skin color detection component is constructed using these functions in main.cpp: readPalm(), skinColorAvg(), getAverageColor(), getMedian(), produceBinary(), and normalizeColor(). 

When main() calls readPalm() to begin the skin color detection process, it outputs seven rectangles onto the screen which takes sample from the region and stores the colors inside a wrapper to be used by skinColorAvg(). A color profile is extracted from the scanning process by getting the median of HSV value at each rectangle. 

Using this color profile created from readPalm() and a threshold, we produce seven binary images from the seven samples to subtract the hand from the background by calling produceBinary(). More specifically, this function sums these binary images and applies a non-linear filter medianBlur() to remove noises and smoothen the image. 

At the end of this process, the program should output an embedded screen on the right corner of the main output that shows the processed scene from the operations above.

#####Convex Hull

After the program scanned the user's skin color and extracted the binary representation of the scene, a contour line that surrounds the general silhouette of the hand is drawn by function makeContours() using OpenCV's convex hull utility. A convex hull is a set of convexes such that if we take nay two points inside the region and join them to form a line, it lies entirely inside the set. Because other objects in the scene could also generate contour, we filter out unwanted contours by calling findMaxContour() which -as the function name implies- finds the contour with the biggest area which would usually be the hand because it is the closest to the camera. In the image below, the blue line surrounding the hand is the convex hull. 

Because the convex hull's vertexes (known as convex points) lies on the finger tips of the hand, we can find the 'valley' between the fingers by getting the points (known as convex defects) furthest away from each convex points. We use OpenCV's convexityDefects() function to get the points. 

Essentially, the program looks for the deepest point of deviation on the contour to determine its defects. Although the program was able to capture the convex defects between the valleys of the fingers, it also produced undesired defects at the wrist (third image from the left shown above). We must filter out these unnecessary points. How? 

We used two factors to determine the dismissal of unnecessary defects: 
                <ul>
                	<li>Angle between fingers is more than 80 degrees</li>
                	<li>Length of finger is less than 0.4 * Bounding Box</li>
                </ul>

If you take a look at your hand at static position, the angles between each finger never exceeds 80-90 degrees. Also the length of the finger must be at least 40% of the bounding box surrounding the hand. The function removeDefects() in gesture.cpp does such operation in our program. 
