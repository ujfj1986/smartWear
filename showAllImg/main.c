/*
 * show all images in a floder.
 *
 * author: shan jiejing
 * using opencv show all images in a floder.
 */

#include "opencv/cv.h"
#include "opencv/highgui.h"

#include <stdio.h>
#include <dirent.h>
#include <string.h>

#define MAX_IMAGE_NUM 20
static const char * WINDOWNAME = "image window";
int g_slider_position = 0;
char* images[MAX_IMAGE_NUM];
IplImage *g_image = NULL;
char path[200] = "\0";

void onTrackerbarSlide(int pos)
{
	char tmp[200] = "\0";
	sprintf(tmp, "%s/%s", path, images[pos]);
	if (g_image != NULL)
		cvReleaseImage(&g_image);
	g_image = cvLoadImage(tmp, 1);
	cvShowImage(WINDOWNAME, g_image);
}
/*static void showImage(IplImage *img)
{
	cvNamedWindow(WINDOWNAME, CV_WINDOW_AUTOSIZE);
	cvShowImage(WINDOWNAME, img);
}*/
static int getAllImages(const char *dir)
{
	int image_num = 0;
	DIR *di = opendir(dir);
	struct dirent *ptr = NULL;

	while((ptr = readdir(di)) != NULL)
	{
		if (ptr->d_name[0] == '.')
			continue;
		if ((strcmp(ptr->d_name + strlen(ptr->d_name) - 4, ".jpg") == 0) ||
			(strcmp(ptr->d_name + strlen(ptr->d_name) - 4, ".JPG") == 0))
		{
			image_num ++;
			if (image_num > MAX_IMAGE_NUM) break;
			images[image_num - 1] = malloc(ptr->d_reclen + 1);
			strcpy(images[image_num - 1], ptr->d_name);
		}
	}
	closedir(di);

	return image_num;
}

static void help()
{
	printf("usage:\n"
		"\t./showAllImage ./\n");
}

int main (int argc, char** argv)
{
	if (argc != 2) {
		help();
		return 0;
	}

	int i = 0;
	int imgnum = getAllImages(argv[1]);
	strcpy(path, argv[1]);
	path[strlen(argv[1]) - 1] = '\0';
	printf("IMG NUM = %d\n", imgnum);

	cvNamedWindow(WINDOWNAME, CV_WINDOW_NORMAL);
	if (imgnum > 1)
	{
		cvCreateTrackbar("pic_num", WINDOWNAME, &g_slider_position, imgnum, onTrackerbarSlide);
	}
	for (i = 0; i < imgnum; i++)
	{
		printf("[%d] image name = %s\n", i, images[i]);
	}
	char tmp[200] = "\0";
	sprintf(tmp, "%s/%s", path, images[0]);
	printf("%s\n", tmp);
	g_image = cvLoadImage(tmp, 1);
	cvShowImage(WINDOWNAME, g_image);

	cvWaitKey(0);
	if (g_image != NULL)
		cvReleaseImage(&g_image);
	//for (i = 0; i < imgnum; i++)
	//	free(images[i]);
	cvDestroyWindow(WINDOWNAME);

	return 0;
}
