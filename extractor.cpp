// extractor.cpp - implementations of the Extractor class.
// Author: Yang Hu
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "extractor.h"

using namespace cv;
using namespace std;

// Extractor - given an isolated image of a card (i.e., the image height and width is 
// approximately the same as the height and width of the card itself), extracts two 
// subimages representing the rank and the suit of the card. The subimages are taken
// from the top left corner of the card.
Extractor::Extractor(bool enableShowResult) {
    this->enableShowResult = enableShowResult;
    RNG rng(12345);
}

bool Extractor::extract(Mat input) {
    src = Mat(input, Rect(0, 0, input.cols / 4, input.rows / 3.5));
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));
    findSubImages();
    return !(rank.empty() || suit.empty());
}

void Extractor::showResult() {
    namedWindow(source_window);
    imshow(source_window, src);
    namedWindow(contour_window);
    imshow(contour_window, drawing);
    namedWindow(rank_window);
    imshow(rank_window, rank);
    namedWindow(suit_window);
    imshow(suit_window, suit);
    waitKey();
}
void Extractor::findSubImages()
{
    Mat canny_output;
    // adaptive threshold
    Canny(src_gray, canny_output, THRESH_OTSU, THRESH_OTSU);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(canny_output, contours, hierarchy, RETR_LIST, CHAIN_APPROX_NONE);

    vector<vector<Point> > contours_poly(contours.size());
    vector<Rect> boundRect(contours.size());
   
    for (size_t i = 0; i < contours.size(); i++)
    {
        approxPolyDP(contours[i], contours_poly[i], 3, true);
        boundRect[i] = boundingRect(contours_poly[i]);
    }

    drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    Mat extractMask = Mat::zeros(src.size(), CV_8UC3);
    
    int area = src.cols * src.rows;

    // find the contour representing rank
    for (size_t i = 0; i < contours.size(); i++)
    {

        int height = -boundRect[i].tl().y + boundRect[i].br().y;
        int width = -boundRect[i].tl().x + boundRect[i].br().x;
        
        // filter out the contour representing rank and suit by checking the dimensions
        // ratio, area, and the position of the contour
        if (height * width < area / 10 || height / width > 4 || height / width  < 1 
            || boundRect[i].br().x > (3 * src.cols / 4) 
            || boundRect[i].br().y > 3 * src.rows / 4) {
            continue;
        }
        else {
            // update rank br position
            rankBrYPosition = boundRect[i].br().y;
            rankBrXPosition = boundRect[i].br().x;
            // create img from contour
            drawContours(extractMask, contours_poly, i, Scalar(255), 0);

            Mat imageROI = src.clone();
            src.copyTo(imageROI, extractMask); 
            rank = imageROI(boundRect[i]);

            // draw contour for window
            Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
            drawContours(drawing, contours_poly, (int)i, color);
            rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2);
            break;
        }
    }
    // return if no contour representing rank is found
    if (rankBrYPosition < 0) {
        cout << "did not find number";
        return;
    }
    // find suit
    for (size_t i = 0; i < contours.size(); i++) {
        int height = -boundRect[i].tl().y + boundRect[i].br().y;
        int width = -boundRect[i].tl().x + boundRect[i].br().x;

        // filter out the contour representing suit by checking the dimensions and the bottom right position
        // of the contour
        if (height * width < area / 25 ||height / width > 4 || height / width < 1 || boundRect[i].br().y <= rankBrYPosition + error || boundRect[i].br().x >= rankBrXPosition + src.cols / 6) {
            continue;
        }
        else {
           
            // create img from contour
            drawContours(extractMask, contours_poly, i, Scalar(255), 0); // This is a OpenCV function

            Mat imageROI = src.clone();
            src.copyTo(imageROI, extractMask); // 'src' is the image you used to compute the contours.
            suit = imageROI(boundRect[i]);

            // draw contour for window
            Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
            drawContours(drawing, contours_poly, (int)i, color);
            rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2);
            break;
        }
    }
    // only show results if both rank and suit sub-images are found
    if (!rank.empty() && !suit.empty() && enableShowResult) {
        showResult();
    }
}
