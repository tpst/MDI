#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

void roiCallBack(int event, int x, int y, int flags, void* ptr);

Mat getRoi(cv::Mat &src);

class indicator {

public:
	cv::Point last_shot;
	cv::vector<cv::Point> shots;

	bool debug;
	int h_min, h_max,
		s_min, s_max,
		v_min, v_max,
		canny_low, canny_high,
		canny_low_max, canny_high_max;

	indicator();

	void createDebugTools();
	cv::Mat findTarget(cv::Mat &src);
	cv::Point getShot(cv::Mat &src, cv::Mat &ref);
	
	void getShots(cv::Mat &src, vector<Point> &shots);

	static void targetCallBack(int event, int x, int y, int flags, void* ptr);

private:
	bool draw;

};