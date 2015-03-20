#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <Windows.h>
#include <stdlib.h>
#include <ShObjIdl.h>
#include <Commdlg.h>
#include <fstream>
#include <sys/stat.h>


using namespace cv;
using namespace std;

Mat getRoi(cv::Mat &src);
void roiCallBack(int event, int x, int y, int flags, void* ptr);
void ptCallBack(int event, int x, int y, int flags, void* ptr);
Mat findLargestContour(cv::Mat& im, std::vector<std::vector<cv::Point>> &contours, int &largest_contour_index);
void Dilation(cv::Mat& im, int iterations, int elem, int size);
void Erosion(cv::Mat& im, int iterations, int elem, int size);

Mat load_image();
bool fileExists(const std::string& filename);

class indicator {

private:

	int h_min, h_max,
		s_min, s_max,
		v_min, v_max,
		canny_low, canny_high,
		canny_low_max, canny_high_max;

public:
	cv::vector<cv::Point> shots;

	bool debug;
	
	////* Grouping information */////

	Point mpi;
	double accuracy;
	double mean_x, mean_y;
	double standard_dev_x, standard_dev_y;
	int shots_on_target, total_shots;
	double miss_distance_mean, miss_distance_std; // mean and std deviation of miss distance. 
	Rect target_rect;
	Point target_centre;
	double ratio_x, ratio_y; // ratio between pixels to metres.
	
	////////////////////////////////

	indicator();

	void createDebugTools();

	Point findTargetCentre(cv::Mat &src);
	void correctShots(cv::Mat &src);
	void getShots(cv::Mat &src, vector<Point> &shots); 
	void process(cv::Mat &src);

	cv::Point getShot(cv::Mat &src, cv::Mat &ref);// for 2 image comparison. redundant

	void save_output();

};