#include "mdi.h"

bool draw = false;
bool select_flag = false;

// Callback function for finding ROI of target
void roiCallBack(int event, int x, int y, int flags, void* ptr)
{
	vector<Point> *p = static_cast<vector<Point> *>(ptr);

	if ( event == EVENT_LBUTTONDOWN )
	{
		select_flag = false;
		p->clear();
		p->push_back(Point(x,y));
		p->push_back(Point(x,y)); // This is duplicated so as not to draw a random box immediately, and to fill p[1]
		draw = true; 
	}
	if ( event == EVENT_MOUSEMOVE && draw)
	{
		(*p)[1] = Point(x,y);
	}
	if ( event == EVENT_LBUTTONUP )
	{
		draw = false;
		select_flag = true;
	}
}

// Select ROI by clicking & dragging on an image. 
Mat getRoi(cv::Mat &src)
{
	Rect roi;
	vector<Point> pts;
	Mat temp;
	namedWindow("Target", CV_WINDOW_NORMAL);
	setMouseCallback("Target", roiCallBack, &pts);

	while(1)
	{
		if(!select_flag) temp = src.clone();
		if(draw)
		{
			rectangle(temp, pts[0], pts[1], Scalar(255,255,255), 3, 8);
			circle(temp, pts[0], 10, Scalar(255,255,255), -3, 8);
			circle(temp, pts[1], 10, Scalar(255,255,255), -3, 8);
			roi = Rect(pts[0], pts[1]);
			
		}
		imshow("Target", temp);
		if(select_flag)
		{
			imshow("Target", src);
			if(roi.area() > 500) imshow("ROI", src(roi));
		}
		if(waitKey(15) == 32 && select_flag) break; // Exit loop if space is pressed
	}
	destroyAllWindows();
	return src(roi);
}