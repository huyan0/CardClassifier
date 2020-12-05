#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <cmath>

using namespace cv;

Mat ranks[13];

String NAMES[] = { "Zero", "Ace", "Two", "Three",
				   "Four", "Five", "Six", "Seven",
				   "Eight", "Nine", "Ten", "Jack",
				   "Queen", "King"};


// max difference values for rank and suit images
// used in comparison.
int DIFF_RANK = 1000;
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
}

bool isEqual(const Mat& test, const Mat& input, bool isRank) {
	Mat test_clone = test.clone();
	Mat input_clone = input.clone();
	resize(test_clone, input_clone, test_clone.size());
	// convert to greyScale
	cv::cvtColor(test_clone, test_clone, COLOR_BGR2GRAY);
	cv::cvtColor(input_clone, input_clone, COLOR_BGR2GRAY);
	// threshold to convert to binary
	threshold(test_clone, test_clone, BW_THRESHOLD, 255, THRESH_BINARY);
	threshold(input_clone, input_clone, BW_THRESHOLD, 255, THRESH_BINARY);
	// calculate the difference between the two
	Mat temp;
	setBreakOnError(true);
	absdiff(test_clone, input_clone, temp);
	double value = sum(temp)[3]/ 255;
	// return true if less than pre-defined value, false otherwise
	if (isRank) {
		return value <= DIFF_RANK;
	}
	else {
		return value <= DIFF_SUIT;
	}
}

// test
int main() {
	readTrainFiles();
	Mat test = imread("2.png");
	for (int i = 0; i < 13; i++) {
		if (isEqual(ranks[i], test, true)) {
			std::cout << "Equal to: " << NAMES[i] << std::endl;
		}
	}
	return 0;
}