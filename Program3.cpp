//// Program3.cpp
//// This code demonstrates creating of color histograms, 
//// calculating most common color, and using that to overlay 
//// a foreground image from a background image.
//// This code also executes some additional OpenCV methods on a 
//// test file called "test3.jpg"
//// Author : Ali Jahangirnezhad
//// date : 11/9/2020
//
//#include <opencv2/core.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
//#include <stdio.h>
//#include <iostream>
//#include <cmath>

//
//using namespace cv;
//
//// number of hist buckets in each dimension
//const int size = 4;
//// size of each bucket for histogram
//const int bucketSize = 256 / size;
//
//// createHist - Creates a color histogram matrix from the input image
//// Preconditions:	The image is in color.
//// Postcondition:	A color histogram is constructed and returned
//// Parameters:		image - The input color image
//Mat createHist(const Mat &image) {
//	int dims[] = { size, size, size };
//	Mat hist(3, dims, CV_32S, Scalar::all(0));
//	int r = 0, g = 0, b = 0;
//	for (int row = 0; row < image.rows; row++) {
//		for (int col = 0; col < image.cols; col++) {
//			// get RGB values of pixel at (row, col) 
//			b = static_cast<int>(image.at<Vec3b>(row, col)[0]);
//			g = static_cast<int>(image.at<Vec3b>(row, col)[1]);
//			r = static_cast<int>(image.at<Vec3b>(row, col)[2]);
//			// calculate the bucket to increment
//			b /= bucketSize;
//			g /= bucketSize;
//			r /= bucketSize;
//			// increment bucket
//			hist.at<int>(r, g, b) += 1;
//		}
//	}
//	// return the computed histogram
//	return hist;
//}
//
//// mostCommonColor - Calculates the most common color of an input color histogram matrix
//// Preconditions:	The input matrix has RGB channels
//// Postconditions:	A Vec3b object of the most common color in the input color histogram
////					is created and returned.
//// Parameters:		matrix - The color histogram matrix
//Vec3b mostCommonColor(const Mat& matrix) {
//	// int array [r, g, b] of the max voted bin
//	int maxVotedBin[3] = { 0, 0, 0 };
//	// max value to use for finding the most voted bin
//	int max = 0;
//	// the number of votes at a given bucket
//	int votes = 0;
//	// loop through the histogram to find the bucket with most votes
//	for (int r = 0; r < matrix.size[0]; r++) {
//		for (int g = 0; g < matrix.size[1]; g++) {
//			for (int b = 0; b < matrix.size[2]; b++) {
//				// get the number of votes at this bucket
//				votes = matrix.at<int>(r, g, b);
//				if (matrix.at<int>(r, g, b) > max) {
//					max = votes;
//					maxVotedBin[0] = r;
//					maxVotedBin[1] = g;
//					maxVotedBin[2] = b;
//				}
//			}
//		}
//	}
//	// calculate most common color
//	int cRed = maxVotedBin[0] * bucketSize + bucketSize / 2;
//	int cGreen = maxVotedBin[1] * bucketSize + bucketSize / 2;
//	int cBlue = maxVotedBin[2] * bucketSize + bucketSize / 2;
//	// Vec3b object with most common color
//	Vec3b mostCommon(saturate_cast<uchar>(cBlue), saturate_cast<uchar>(cGreen), \
//		saturate_cast<uchar>(cRed));
//
//	return mostCommon;
//}
//
//
//
//// exploreOpenCVMethods - Perform 
//// PreConditions : an image file named "test3.jpg" exists in program directory
//// PostCondition : a file named "myoutput.jpg" is saved to disk, which is 
////				   the result of performing the mentioned operations on 
////				   "test3.jpg"
//void exploreOpenCVMethods() {
//	// read the test3.jpg file
//	Mat test = imread("test3.jpg");
//	// make a clone, in order to manipulate
//	Mat output = test.clone();
//	// photo negative
//	bitwise_not(output, output);
//	// draw a red line from (0, 0) to center on the image
//	Point2i a(output.cols / 2, output.rows / 2);
//	Point2i b(0, 0);
//	line(output, a, b, Scalar(0, 0, 250), 2);
//	// draw a blue circle in the middle of the image
//	circle(output, a, 50, Scalar(250, 0, 0), -1);
//	// write the word "test3" in the middle of the circle
//	putText(output, "Test3", Point2i(output.cols / 2 - 30, output.rows / 2), \
//		FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(0, 0, 250), 2);
//	// write the output image 
//	imwrite("myoutput.jpg", output);
//}
//
//// createOverlay - a method to create overlay output from background and foreground
//// Preconditions:	The foreground is the same image used to get the most common color
//// Postconditions:	An image is produced and returned that is the result of replacing
////					pixels in the foreground with RGB values close to the most common color
////					with corresponding pixels from background color
//// Parameters:		foreGround - The foreground image
////					backGround - The background image
////					mostCommon - The most common color of foreGround image
//Mat createOverlay(const Mat& foreGround, const Mat& backGround, const Vec3b& mostCommon) {
//	Mat output = foreGround.clone();
//	// difference of RGB from the value in foreground image
//	unsigned char rDiff = 0, bDiff = 0, gDiff = 0;
//	// looping through the foreGround clone and replacing
//	// pixels that are close to the most common color
//	for (int row = 0; row < output.rows; row++) {
//		for (int col = 0; col < output.cols; col++) {
//			// calculate R,G,B difference between foreground and most common
//			bDiff = std::abs(mostCommon[0] - output.at<Vec3b>(row, col)[0]);
//			gDiff = std::abs(mostCommon[1] - output.at<Vec3b>(row, col)[1]);
//			rDiff = std::abs(mostCommon[2] - output.at<Vec3b>(row, col)[2]);
//			if (bDiff <= bucketSize && gDiff <= bucketSize && rDiff <= bucketSize) {
//				// calculate the background row and column
//				int bRow = row % backGround.rows;
//				int bCol = col % backGround.cols;
//				// replace values at foreground color
//				output.at<Vec3b>(row, col) = backGround.at<Vec3b>(bRow, bCol);
//			}
//		}
//	}
//	return output;
//}
//
//// main -	The main driver of the program.
////			It reads the foreground and background images, and 
////			performs the overlay.
////			Also performs required openCV methods on the background file,
////			and additional openCV methods on test3 
//int main() {
//	// read foreground and background images
//	Mat test = imread("test.jpg");
//	// calculate the color histogram
//	Mat hist = createHist(test);
//	// get the most common colors
//	Vec3b common = mostCommonColor(hist);
//	// output
//	Mat output = test.clone();
//
//	unsigned char r = 0, b = 0, g = 0, rnew = 0, bnew = 0, gnew = 0;
//	b = common[0];
//	g = common[1];
//	r = common[2];
//
//	for (int row = 0; row < test.rows; row++) {
//		for (int col = 0; col < test.cols; col++) {
//			bnew = static_cast<unsigned char>(floor(b + 1.5 * (test.at<Vec3b>(row, col)[0] - b)));
//			gnew = static_cast<unsigned char>(floor(g + 1.5 * (test.at<Vec3b>(row, col)[1] - g)));
//			rnew = static_cast<unsigned char>(floor(r + 1.5 * (test.at<Vec3b>(row, col)[2] - r)));
//
//			// check for out of bounds
//			bnew = bnew > 255 ? 255 : bnew;
//			bnew = bnew < 0 ? 0 : bnew;
//
//			gnew = gnew > 255 ? 255 : gnew;
//			gnew = gnew < 0 ? 0 : gnew;
//
//			rnew = rnew > 255 ? 255 : rnew;
//			rnew = rnew < 0 ? 0 : rnew;
//
//			output.at<Vec3b>(row, col)[0] = bnew;
//			output.at<Vec3b>(row, col)[1] = gnew;
//			output.at<Vec3b>(row, col)[2] = rnew;
//		}
//	}
//	std::cout << output.size << std::endl;
//	setBreakOnError(true);
//	// save to disk
//	imwrite("output.jpg", output);
//
//	return 0;
//}

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <opencv2/videoio.hpp>
#include "extractor.h"
#include <map>

using namespace cv;
using namespace std;

void preprocessFrame(Mat& image, Mat& foreGround, vector<vector<Point>>& contours, vector<Vec4i>& hierarchy) {
	// convert to grayscale
	cvtColor(image, foreGround, COLOR_BGR2GRAY);
	// blurr
	GaussianBlur(foreGround, foreGround, Size(5, 5), 0);
	// to black and white
	threshold(foreGround, foreGround, 128, 255, THRESH_BINARY);
	// Canny ED
	Canny(foreGround, foreGround, THRESH_OTSU, THRESH_OTSU);
	// find contours
	findContours(foreGround, contours, hierarchy, RETR_LIST, CHAIN_APPROX_NONE);
}

Mat ranks[13];
Mat suits[4];

String NAMES[] = { "Zero", "Ace", "Two", "Three",
				   "Four", "Five", "Six", "Seven",
				   "Eight", "Nine", "Jack",
				   "Queen", "King" };

map<int, string> suitMap = {
	{0, "Hearts"},
	{1, "Diamonds"},
	{2, "Spades"},
	{3, "Clubs"}
};


// max difference values for rank and suit images
// used in comparison.
int DIFF_RANK = 2500;
int DIFF_SUIT = 750;
// threshold for converting to binary
int BW_THRESHOLD = 128;

// assumes the train files are named as presented
void readTrainFiles() {
	// read rank samples
	Mat zero = imread("0.png");
	Mat ace = imread("A.png");
	Mat two = imread("2.png");
	Mat three = imread("3.png");
	Mat four = imread("4.png");
	Mat five = imread("5.png");
	Mat six = imread("6.png");
	Mat seven = imread("7.png");
	Mat eight = imread("8.png");
	Mat nine = imread("9.png");
	Mat jack = imread("J.png");
	Mat queen = imread("Q.png");
	Mat king = imread("K.png");

	// save to ranks array
	ranks[0] = zero;
	ranks[1] = ace;
	ranks[2] = two;
	ranks[3] = three;
	ranks[4] = four;
	ranks[5] = five;
	ranks[6] = six;
	ranks[7] = seven;
	ranks[8] = eight;
	ranks[9] = nine;
	ranks[10] = jack;
	ranks[11] = queen;
	ranks[12] = king;

	// save the suits 
	suits[0] = imread("hearts.png");
	suits[1] = imread("diamonds.png");
	suits[2] = imread("spades.png");
	suits[3] = imread("clubs.png");
}

double isEqual(const Mat& test, const Mat& input) {
	Mat test_clone = test.clone();
	Mat input_clone = input.clone();
	if (input.rows < 5 || input.cols < 5) {
		return INT_MIN;
	}
	// resize so both have the same size, in order to calculate difference
	resize(input_clone, input_clone, test_clone.size());
	// convert to greyScale
	cvtColor(test_clone, test_clone, COLOR_BGR2GRAY);
	cvtColor(input_clone, input_clone, COLOR_BGR2GRAY);
	// blurr images for easier distinguishing
	GaussianBlur(input_clone, input_clone, Size(5, 5), 0);
	GaussianBlur(test_clone, test_clone, Size(5, 5), 0);
	// threshold to convert to binary (black and white)
	threshold(test_clone, test_clone, BW_THRESHOLD, 255, THRESH_BINARY);
	threshold(input_clone, input_clone, BW_THRESHOLD, 255, THRESH_BINARY);
	// temporary matrix to hold the difference matrix
	Mat temp;
	//// calculates the difference between two matrices, saves results in temp
	//absdiff(test_clone, input_clone, temp);	
	//// different pixels have the value 255, the rest are 0
	//double value = sum(temp)[0]/ 255;
	double value = norm(test_clone, input_clone);
	/*imwrite("test_clone.jpg", test_clone);
	imwrite("input_clone.jpg", input_clone);
	imwrite("class_test.jpg", temp);
	imwrite("class_test.jpg", temp);*/

	return value;
}

int main() {
	// read train files
	readTrainFiles();
	Mat frame;
	// initialize video capture
	VideoCapture cap;
	cap.set(CAP_PROP_FPS, 60);
	int deviceID = 0;             // 0 = open default camera
	int apiID = cv::CAP_ANY;      // 0 = autodetect default API
	// open selected camera using selected API
	cap.open(deviceID, apiID);
	// check if we succeeded
	if (!cap.isOpened()) {
		cerr << "Cannot access camera!\n";
		return -1;
	}
	// rank and suit extractor
	Extractor ext(false);
	// image to hold each frame
	Mat image, foreGround;
	// contours vector
	vector<vector<Point> > contours;
	vector<vector<Point> > largestContours;
	// contour hierarchy
	vector<Vec4i> hierarchy;
	for (;;)
	{

		// wait for a new frame from camera and store it into 'frame'
		cap.read(frame);
		// check if we succeeded
		if (frame.empty()) {
			cerr << "Blank Frame\n";
			break;
		}
		// save frame in image Mat
		image = frame.clone();
		// preprocess this frame
		// refer to the documentation for preprocess
		preprocessFrame(image, foreGround, contours, hierarchy);
		// find the largest contours
		double area_max = 0;
		for (int i = 0; i < contours.size(); i++) {  // get the largest contour
			double area = fabs(contourArea(contours[i]));
			if (area >= area_max) {
				area_max = area;
				largestContours.clear();
				largestContours.push_back(contours[i]);
			}
		}

		// a mask to hold area inside the contour
		Mat mask = Mat::zeros(image.rows, image.cols, CV_8UC1);
		// if the max contour area is bigger than a preset amount
		// to prevent unwanted small contours

		if (area_max > 3000) {
			// draw contours
			drawContours(image, largestContours, 0, Scalar(255, 0, 0), 2);
			drawContours(mask, largestContours, 0, Scalar(255, 0, 0), FILLED);
		}
		/*vector<Point>  rect;
		approxPolyDP(largestContours.at(0), rect, 0.01 * perimeter, true);*/
		vector<vector<Point> > contours_poly(1);
		if (largestContours.size() != 0 && area_max > 3000) {
			approxPolyDP(largestContours.at(0), contours_poly[0], 0.2, true);
			Rect bounds = boundingRect(contours_poly[0]);
			Mat temp = image(bounds);
			if (ext.extract(temp)) {
				namedWindow("Rank", WINDOW_NORMAL);
				imshow("Rank", ext.rank);
				namedWindow("Suit", WINDOW_NORMAL);
				imshow("Suit", ext.suit);
			}
			namedWindow("Card Extracted", WINDOW_AUTOSIZE);
			imshow("Card Extracted", image(bounds));
			// matcher
			double min = INT_MAX;
			int rank = 0, suit = 0;
			for (int i = 0; i < 13; i++) {
				double error = isEqual(ranks[i], ext.rank);
				if (error < min) {
					min = error;
					rank = i;
				}
			}
			if (NAMES[rank] != "Jack") {
				cout << "Rank : " << NAMES[rank] << ". ";
			}
			min = INT_MAX;
			for (int i = 0; i < 4; i++) {
				double error = isEqual(suits[i], ext.suit);
				if (error < min) {
					min = error;
					suit = i;
				}
			}
			cout << "Suit : " << suitMap[suit] << endl;
		}

		// show live and wait for a key with timeout long enough to show images
		imshow("Live", image);
		if (waitKey(5) >= 0) {
			break;
		}
	}
	return 0;
}