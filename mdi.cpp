#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "mdi.h"

using namespace cv;
using namespace std;

indicator::indicator()
{
	debug = true;
	draw = false;
	h_min = 0;
	h_max = 255;
	s_min = 0;
	s_max = 255;
	v_min = 0;
	v_max = 255;

	if(debug) createDebugTools();

}

void indicator::createDebugTools()
{
	namedWindow  ("Trackbars", CV_WINDOW_NORMAL);
	resizeWindow("Trackbars", 420, 200);

	createTrackbar( "h_min", "Trackbars", &h_min, h_max );
	createTrackbar( "h_max", "Trackbars", &h_max, h_max );
	createTrackbar( "s_min", "Trackbars", &s_min, s_max );
	createTrackbar( "s_max", "Trackbars", &s_max, s_max );
	createTrackbar( "v_min", "Trackbars", &v_min, v_max );
	createTrackbar( "v_max", "Trackbars", &v_max, v_max );

}

// Callback function for finding ROI of target
void indicator::targetCallBack(int event, int x, int y, int flags, void* ptr)
{
	Rect *r = static_cast <Rect*>(ptr);

	if ( event == EVENT_LBUTTONDOWN )
	{
		r->tl() = Point(x,y);
		draw = true; 
	}
	if ( event == EVENT_MOUSEMOVE )
	{
		// not sure
	}
	if ( event == EVENT_LBUTTONUP )
	{
		r->br() = Point(x,y);
		draw = false;
	}
}

Mat indicator::findTarget(cv::Mat &src)
{
	Rect roi;
	namedWindow("Target", CV_WINDOW_NORMAL);
	setMouseCallback("Target", targetCallBack, &roi);

	while(1)
	{
		if(draw)
		{
			cout << "Drawing~!!!" << endl;
		}
		imshow("Target", src);

	}
	Mat dst = src.clone();
	return dst;
}


/* Find next shot function - called each frame and will compare to the reference image
   to find the impact point. The x,y location of the shot w.r.t the target centre is returned 
   and the shot is appended to the vector 'shots' containing each shot location */
Point indicator::getShot(cv::Mat &src, cv::Mat &ref)
{
	vector<vector<Point> > contours;
	Rect rect;
	Point pos = Point(0,0);
	
	cv::Mat diff, temp;
	cv::Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1) );
	
	absdiff(src, ref, diff);
	cvtColor(diff, temp, CV_BGR2GRAY);
	dilate(temp, temp, element, Point(-1, -1), 3, 1 );
	cv::threshold(temp,temp,20,255,THRESH_BINARY);

	if(debug) {
		namedWindow("diff", CV_WINDOW_NORMAL);
		imshow("diff", temp);
	}

	findContours(temp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	if(contours.size() == 1)
	{
		vector< vector<Point> > largestContourVec;
		largestContourVec.push_back(contours.at(contours.size()-1));
		//make a bounding rectangle around the largest contour then find its centroid
		//this will be the object's final estimated position.
		rect = boundingRect(largestContourVec.at(0));
		pos.x = rect.x+rect.width/2;
		pos.y = rect.y+rect.height/2;
		shots.push_back(pos);
	}
	
	return pos;
}
