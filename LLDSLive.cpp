//including header files 
#include <opencv2/opencv.hpp>
#include <iostream>

//namespaces
using namespace cv;
using namespace std;

//main function
//argc =  number of command line arguments
//argv = an array of pointers to the argument strings
int main(int argc, char* argv())
{
	//opens the video file "if user want, he can change it to a live feed"
	VideoCapture cap(0);

	//getting the witdh and height of the video frames
	int width = (int)cap.get(CAP_PROP_FRAME_WIDTH);
	int height = (int)cap.get(CAP_PROP_FRAME_HEIGHT);

	//create a window to display the rsults of the h/w
	namedWindow("LaneLineDetection", WINDOW_AUTOSIZE);

	//loopes the frames of the video 
	while (true)
	{
		//cretes a matrix of the frame
		Mat frame;

		//sotres the frame from the live feed into the matrix frame
		cap.read(frame);

		//cretae a gray matirx 
		Mat gray;

		//Pre-processing by applint graysce and gaussian blur
		cvtColor(frame, gray, COLOR_RGB2GRAY);
		GaussianBlur(frame, gray, Size(5, 5), 0);

		//applies canny edge detection 
		//edges is a 2d array of pixels that represent ht eedge of the map of the input image, 
		//with whie pixels indicating the edges and black pixels indicating 
		//non-edges
		//"edges" is used as an input to the "HoughLines" function, which processes the edge map to detect lines in the image. 
		//detected lines are then stored in a vector of (rho, theta) pairs, 
		//which are used to draw the lines on the original frame using the "line" function
		Mat edges;
		//50 is thge lower thrshold for the canny edge detectore, Edges below this wioll be discarded
		//200 is the upper threshhold for the canny edge detector. Edges with pixel intensity differences 
		//above this threshold will be retained, 
		//while those between the lower and upper thresholds will 
		//be retained only if they are connected to a strong edge
		Canny(gray, edges, 50, 200);

		//uses Hough transform to detect lines 
		//creates a vector to identify 2 points to make a line
		vector<Vec2f> lines;
		//1 is the resolution of the angle parameter "theta: in radians
		//CV_PI/180 is the resoulution of the angle parametre "theta" in radians. 
		//50 is the min. number of intersectins needed to detect a line. 
		//If a line has less than these number of intersections, it will be dicarded
		HoughLines(edges, lines, 1, CV_PI / 180, 50);

		//overlays the detected lines on the frame
		Mat result = frame.clone();
		for (size_t i = 0; i < lines.size(); i++)
		{
			//extracts the rho and thetha values of the current lines vector
			float rho = lines[i][0];
			float theta = lines[i][0];
			//computes the a and b coefficints of the line equation y = ax+b in standard form
			//the cos funtions are in the math library funtions that compute
			//the cosine and sine of an angle
			double a = cos(theta);
			double b = sin(theta);
			//computes the cordinate x0 and y0 of the point where the line intersects the y-axis
			//starting point for the lines in the image
			double x0 = a * rho;
			double y0 = b * rho;
			//creates two point objects the represnt the endpoints of the line 
			//the point classis a template class for storing 2d objects 
			//the cv rouind function is a rounding function 
			//1000 is the distance along a line
			//The direction of movement is determined by the "a" and "b" coefficients. 
			//If "a" is positive, the line has a positive slope and moves upwards, 
			//and if "a" is negative, the line has a negative slope and moves downwards. 
			//If "b" is positive, the line moves to the right, and if "b" is negative, 
			//the line moves to the left.
			Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
			Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
			//line draws the line on teh image using the line funstion 
			//the "Scalar" object represents the color blue 
			//(red value 0, green value 0, blue value 255)
			//3 is the thickness of the line
			//LINE_AA is the rendering type which stands for anti-aliased line
			//which meand the line will be smoothly blended with the background
			//pixels to reduce aliasing(jagged angles)
			line(result, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);

		}
		//display the result
		imshow("Lane Line Detection System", result);

		//if a key is pressed
		if (waitKey(30) == 27)
		{
			//end the program
			break;
		}

	}
	return 0;
}