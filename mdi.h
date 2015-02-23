class indicator {


public:
	cv::Point last_shot;
	cv::vector<cv::Point> shots;

	bool debug;
	int h_min, h_max,
		s_min, s_max,
		v_min, v_max;

	/* Constructor */
	indicator();

	void createDebugTools();
	cv::Mat findTarget(cv::Mat &src);
	cv::Point getShot(cv::Mat &src, cv::Mat &ref);

	static void targetCallBack(int event, int x, int y, int flags, void* ptr);

private:
	bool draw;

};