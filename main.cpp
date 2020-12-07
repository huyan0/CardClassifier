#include <iostream>

int main() {
  IplImage* img =  cvLoadImage("Cards.bmp");
  
	//show the original image
	cvNamedWindow("Raw");
	cvShowImage("Raw",img);

	//converting the original image into grayscale
	IplImage* imgGrayScale = cvCreateImage(cvGetSize(img), 8, 1); 
	cvCvtColor(img,imgGrayScale,CV_BGR2GRAY);

	//thresholding the grayscale image to get better results
	cvThreshold(imgGrayScale,imgGrayScale,128,255,CV_THRESH_BINARY);  
 
	CvSeq* contours;  //hold the pointer to a contour in the memory block
	CvSeq* result;   //hold sequence of points of a contour
	CvMemStorage *storage = cvCreateMemStorage(0); //storage area for all contours
 
	//finding all contours in the image
	cvFindContours(imgGrayScale, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
  
	//iterating through each contour
	while(contours){
		//obtain a sequence of points of contour, pointed by the variable 'contour'
		result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);
           

		if(result->total == 4){ // Square o Rectangle
			//iterating through each point
			CvPoint *pt[4];
			for(int i=0;i<4;i++){
			 pt[i] = (CvPoint*)cvGetSeqElem(result, i);
			}
   
			//drawing lines around the triangle
			cvLine(img, *pt[0], *pt[1], cvScalar(0,0,255),4);
			cvLine(img, *pt[1], *pt[2], cvScalar(0,0,255),4);
			cvLine(img, *pt[2], *pt[3], cvScalar(0,0,255),4);
			cvLine(img, *pt[3], *pt[0], cvScalar(0,0,255),4);
		}

		//obtain the next contour
		contours = contours->h_next; 
	}

	//show the image in which identified shapes are marked   
	cvNamedWindow("Tracked");
	cvShowImage("Tracked",img);
   
	cvWaitKey(0); //wait for a key press

	//cleaning up
	cvDestroyAllWindows(); 
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&img);
	cvReleaseImage(&imgGrayScale);

	return 0;
}