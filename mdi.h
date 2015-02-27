#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;


Mat getRoi(cv::Mat &src);
void roiCallBack(int event, int x, int y, int flags, void* ptr);
Mat findLargestContour(cv::Mat& im, Point target_centre);

void Dilation(cv::Mat& im, int iterations, int elem, int size);
void Erosion(cv::Mat& im, int iterations, int elem, int size);


class indicator {

public:
	cv::vector<cv::Point> shots;

	bool debug;

	int h_min, h_max,
		s_min, s_max,
		v_min, v_max,
		canny_low, canny_high,
		canny_low_max, canny_high_max;

	/* Grouping information */
	Point mpi;
	double dist_to_mpi, accuracy;
	double standard_dev_x, standard_dev_y;
	int shots_on_target, total_shots;
	
	////////////////////////////////

	indicator();

	void createDebugTools();

	cv::Mat findTarget(cv::Mat &src);
	void correctShots(cv::Mat &src);
	void getShots(cv::Mat &src, vector<Point> &shots); 
	void process(cv::Mat &src);

	cv::Point getShot(cv::Mat &src, cv::Mat &ref);// for 2 image comparison. redundant


};