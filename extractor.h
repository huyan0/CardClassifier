#pragma once
// Extractor - given an isolated image of a card (i.e., the image height and width is 
// approximately the same as the height and width of the card itself), extracts two 
// subimages representing the rank and the suit of the card. The subimages are taken
// from the top left corner of the card.
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;
class Extractor {
private:
    Mat src_gray, src;
    RNG rng;
    int thresh = 50;
    string rank_window = "rank";
    string suit_window = "suit";
    string source_window = "source";
    string contour_window = "contour";
    bool enableShowResult;
    float rankBrYPosition = -1;
    float rankBrXPosition = -1;
    int error = 5;
    void thresh_callback(int, void*);
    void showResult();

public:
    Mat rank, suit, drawing;
    Extractor( bool showResult);
    bool extract(Mat input);
}; 
