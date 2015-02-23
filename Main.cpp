#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <windows.h>

#include "mdi.h"

using namespace cv;
using namespace std;

int main (int argc, char* argv[])
{
	cv::Mat img = imread("MDItarget.jpg");
	cv::Mat img1 = imread("MDItarget_1.jpg");
	cv::Mat img2 = imread("MDItarget_2.jpg");
	cv::Mat img3 = imread("MDItarget_3.jpg");
	cv::Mat background = img1.clone();
	cv::Mat drawing = Mat::zeros( img1.rows, img1.cols, CV_8UC3 );	Point shot = Point(0,0);

	namedWindow("Feed", CV_WINDOW_NORMAL);
	//namedWindow("Process", CV_WINDOW_NORMAL);

	indicator mdi;
	mdi.findTarget(img);

	for( int i = 0; i < 10; i++)
	{
		switch(i) {
			case 5:
				//shot = getPoint(img2, background);
				shot = mdi.getShot(img2, background);
				background = img2.clone();
				//Draw display on feed and scene
				circle(img2, shot, 10, Scalar(0,255,0), 2, 8, 0);
				circle(drawing, shot, 10, Scalar(0,255,0), 2, 8, 0);
				imshow("Feed", img2);
				break;
			case 7:
				shot = mdi.getShot(img3, background);
				background = img3.clone();
				circle(img3, shot, 10, Scalar(0,255,0), 2, 8, 0);
				circle(drawing, shot, 10, Scalar(0,255,0), 2, 8, 0);
				imshow("Feed", img3);
				break;
			default:
				imshow("Feed", background);
		}
		namedWindow("History", CV_WINDOW_NORMAL);
		imshow("History", drawing);
		waitKey(1);

		Sleep(1000);

	}
	cout << mdi.shots << endl;
	waitKey();
	return 0;
}

