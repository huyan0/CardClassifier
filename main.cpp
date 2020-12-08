// main.cpp
// Author: Yang Hu

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "extractor.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    String folderpath = "archive/";
    vector<String> filenames;
    cv::glob(folderpath, filenames);
    Extractor ext(true);
    for (size_t i = 0; i < filenames.size(); i++)//filenames.size(); i++)
    {
        cout << filenames[i];
        Mat img = imread(filenames[i]);
        if (ext.extract(img)) {
            imwrite("results/" + to_string(i) + "_rank.png", ext.rank);
            imwrite("results/" + to_string(i) + "_suit.png", ext.suit);
        }
        else {
            cout << "Error: cannot extract number and/or suit from image";
            return 1;
        }
    }
    return 0;
}
