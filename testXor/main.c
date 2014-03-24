/*
 * read two image file in gray and do xor operation.
 *
 * author: shan jiejing
 *
 * show two images in gray, and their threshold images,
 * user can change the slide and this will change the threshold 
 * images, and then, user input 'o' to do or operation, input
 * 'x' to do xor operation, input 'a' to do and operation.
 *
 * 2014.3.23
 */

#include "opencv/cv.h"
#include "opencv/highgui.h"

#include <stdio.h>
#include <string.h>

const char* WINDOW1NAME = "image 1";
const char* WINDOW2NAME = "image 2";
const char* WINDOW1BINARYNAME = "image 1 binary";
const char* WINDOW2BINARYNAME = "image 2 binary";
const char* WINDOWXORNAME = "xor";
const char* WINDOWANDNAME = "and";
const char* WINDOWORNAME = "or";
const char* TRACKNAME = "binary";

IplImage* pBinaryImage1 = NULL;
IplImage* pBinaryImage2 = NULL;
IplImage* pGrayImage1 = NULL;
IplImage* pGrayImage2 = NULL;


void help ()
{
	printf("use case:\n"
		"\t xor image1_name image2_name\n");
}

void on_trackbar(int pos)
{ 
	cvThreshold(pGrayImage1, pBinaryImage1, pos, 255, CV_THRESH_BINARY);
	cvThreshold(pGrayImage2, pBinaryImage2, pos, 255, CV_THRESH_BINARY);
	cvShowImage(WINDOW1BINARYNAME, pBinaryImage1);
	cvShowImage(WINDOW2BINARYNAME, pBinaryImage2);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		help();
		return 0;
	}

	char *image1name = argv[1];
	char *image2name = argv[2];

	//create two windows
	cvNamedWindow(WINDOW1NAME, CV_WINDOW_NORMAL);
	cvNamedWindow(WINDOW2NAME, CV_WINDOW_NORMAL);

	//read two images with gray 
	pGrayImage1 = cvLoadImage(image1name, CV_LOAD_IMAGE_GRAYSCALE);
	pGrayImage2 = cvLoadImage(image2name, CV_LOAD_IMAGE_GRAYSCALE);

	//show two gray images
	cvShowImage(WINDOW1NAME, pGrayImage1);
	cvShowImage(WINDOW2NAME, pGrayImage2);

	cvWaitKey(0);

	//show image1 and with a track slider
	cvNamedWindow(WINDOW1BINARYNAME, CV_WINDOW_NORMAL);
	cvNamedWindow(WINDOW2BINARYNAME, CV_WINDOW_NORMAL);
	pBinaryImage1 = cvCreateImage(cvGetSize(pGrayImage1), IPL_DEPTH_8U, 1);
	pBinaryImage2 = cvCreateImage(cvGetSize(pGrayImage2), IPL_DEPTH_8U, 1);
	int threshold = 0;
	cvCreateTrackbar(TRACKNAME, WINDOW1BINARYNAME, &threshold, 254, on_trackbar);
	on_trackbar(1);
	
	int c = cvWaitKey(0);
	printf("%c, %d, %x, %x, %x, %x\n", c, c, c, 'a', 'o', 'x');
	IplImage * pImageXor = cvCreateImage(cvGetSize(pGrayImage1), IPL_DEPTH_8U, 1);
	if (c == 0x100078) {
		cvXor(pBinaryImage1, pBinaryImage2, pImageXor, NULL);
		cvNamedWindow(WINDOWXORNAME, CV_WINDOW_NORMAL);
		cvShowImage(WINDOWXORNAME, pImageXor);
	} else if (c == 0x100061) {
		cvAnd(pBinaryImage1, pBinaryImage2, pImageXor, NULL);
		cvNamedWindow(WINDOWANDNAME, CV_WINDOW_NORMAL);
		cvShowImage(WINDOWANDNAME, pImageXor);
	} else if (c == 0x10006f) {
		cvOr(pBinaryImage1, pBinaryImage2, pImageXor, NULL);
		cvNamedWindow(WINDOWORNAME, CV_WINDOW_NORMAL);
		cvShowImage(WINDOWORNAME, pImageXor);
	} else {
		goto err_input;
	}

	int s = cvWaitKey(0);
	char* filename = NULL;
	if (s == 0x100073) {
		if (c == 0x100078) {
			filename = "xor.jpg";
		} else if (c == 0x10006f) {
			filename = "or.jpg";
		} else if (c == 0x100061) {
			filename = "and.jpg";
		}
		cvSaveImage(filename, pImageXor, 0);
	}

	//xor two images
	/*cvOr(pGrayImage1, pGrayImage2, pImageXor, NULL);
	cvNamedWindow("xor", CV_WINDOW_NORMAL);
	cvShowImage("xor", pImageXor);
	cvWaitKey(0);
	cvReleaseImage(&pImageXor);
	cvDestroyWindow("xor");*/

err_input:
	cvReleaseImage(&pImageXor);
	cvReleaseImage(&pGrayImage1);
	cvReleaseImage(&pGrayImage2);
	cvReleaseImage(&pBinaryImage1);
	cvReleaseImage(&pBinaryImage2);
	cvDestroyWindow(WINDOW1NAME);
	cvDestroyWindow(WINDOW2NAME);
	cvDestroyWindow(WINDOW1BINARYNAME);
	cvDestroyWindow(WINDOW2BINARYNAME);
	return 0;
}
