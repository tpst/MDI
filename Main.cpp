#include "mdi.h"

using namespace cv;
using namespace std;

int main (int argc, char* argv[])
{
	Mat img = imread("MDItarget.jpg");

	indicator mdi;
	Mat roi = getRoi(img); // Find the ROI aka our target. 
	Mat copy = roi.clone();
	mdi.getShots(roi, mdi.shots); // Find all bullet holes and fill 'shots' vector with their positions
	namedWindow("SHOTS", CV_WINDOW_NORMAL);
	namedWindow("TARGET", CV_WINDOW_NORMAL);
	imshow("SHOTS", roi);
	imshow("TARGET", copy);
	waitKey();
	return 0;
}

