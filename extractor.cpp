// Extractor - given an isolated image of a card (i.e., the image height and width is 
// approximately the same as the height and width of the card itself), extracts two 
// subimages representing the number and the suit of the card. The subimages are taken
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
    int thresh = 100;
    void thresh_callback(int, void*);
public:
    Mat number, suit;
    Extractor(const Mat input);
};
Extractor::Extractor(const Mat input) {
    RNG rng(12345);
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));
    //const char* source_window = "Source";
    //namedWindow(source_window);
    //  imshow(source_window, src);
    const int max_thresh = 255;
    // createTrackbar("Canny thresh:", source_window, &thresh, max_thresh, thresh_callback);
    thresh_callback(0, 0);
    // waitKey();
    // imwrite("results/" + to_string(i) + ".png", number);

}
void Extractor::thresh_callback(int, void*)
{
    Mat canny_output;
    Canny(src_gray, canny_output, thresh, thresh * 2);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(canny_output, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

    vector<vector<Point> > contours_poly(contours.size());
    vector<Rect> boundRect(contours.size());

    for (size_t i = 0; i < contours.size(); i++)
    {
        approxPolyDP(contours[i], contours_poly[i], 3, true);
        boundRect[i] = boundingRect(contours_poly[i]);
    }
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    Mat extractMask = Mat::zeros(src.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        int height = -boundRect[i].tl().y + boundRect[i].br().y;
        int width = -boundRect[i].tl().x + boundRect[i].br().x;

        // filter out the contour representing number and suit by checking the dimensions
        // of the contour
        if (!(height < src.rows / 7 && height > src.rows / 15) || boundRect[i].br().y > src.rows / 5) {
            continue;
        }
        else {
            // create img from contour
            drawContours(extractMask, contours_poly, i, Scalar(255), 0); // This is a OpenCV function

            Mat imageROI = src.clone();
            src.copyTo(imageROI, extractMask); // 'src' is the image you used to compute the contours.
            number = imageROI(boundRect[i]);

            // draw contour for window
            Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
            drawContours(drawing, contours_poly, (int)i, color);
            rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2);
        }
    }
    imshow("Contours", drawing);
}
