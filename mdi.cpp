#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "mdi.h"

using namespace cv;
using namespace std;

Mat removeBlobs(cv::Mat& im, char* val, double size);

indicator::indicator()
{
	debug = false; /* used for debuging/altering various thresholding algorithms */

	h_min = 91;
	h_max = 164;
	s_min = 20;
	s_max = 255;
	v_min = 0;
	v_max = 63;
	canny_low = 380;
	canny_high = 822;
	canny_low_max = 1000;
	canny_high_max = 1000;

	/* salvo info */
	total_shots = 45;

}

void indicator::createDebugTools()
{
	namedWindow  ("Trackbars", CV_WINDOW_NORMAL);
	resizeWindow("Trackbars", 420, 200);

	createTrackbar( "canny_low", "Trackbars", &canny_low, canny_low_max );
	createTrackbar( "canny_high", "Trackbars", &canny_high, canny_high_max );
	//createTrackbar( "h_min", "Trackbars", &h_min, h_max );
	//createTrackbar( "h_max", "Trackbars", &h_max, h_max );
	//createTrackbar( "s_min", "Trackbars", &s_min, s_max );
	//createTrackbar( "s_max", "Trackbars", &s_max, s_max );
	//createTrackbar( "v_min", "Trackbars", &v_min, v_max );
	//createTrackbar( "v_max", "Trackbars", &v_max, v_max );

}

// Callback function for shot correction
void correctionCallback(int event, int x, int y, int flags, void* ptr)
{
	vector<Point> *p = static_cast<vector<Point> *>(ptr);
	
	if( event == EVENT_LBUTTONDOWN )
	{
		p->push_back(Point(x, y)); 
	}
	if( event == EVENT_RBUTTONDOWN ) // This does some wierd stuff
	{
		for( int i = 0; i < p->size(); i++)
		{ 
			double distance = sqrt( ((*p)[i].x-x)*((*p)[i].x-x) + ((*p)[i].y-y)*((*p)[i].y-y));
			if( distance < 10)
			{
				p->erase(p->begin()+i); // Remove this element from the array of shtos
			}
		}
	}

}

// Allows the user to manually add and remove shots
void indicator::correctShots(cv::Mat &src)
{
	namedWindow("Correct Shots", CV_WINDOW_NORMAL);
	setMouseCallback("Correct Shots", correctionCallback, &shots);

	while(1)
	{
		Mat temp = src.clone();
		for( int i = 0; i< shots.size(); i++ )
		{
			circle( temp, shots[i], 4, Scalar(0,0,255), -1, 8, 0 );
		}
		imshow("Correct Shots", temp);
		if(waitKey(1) == 32) break; // exit loop of space is pressed. 
	}

}



// Attempts to find bulletholes in the target and stores each shot in the vector 'shots'
void indicator::getShots(cv::Mat &src, vector<Point> &shots)
{
	shots.clear();
	vector<vector<Point>> contours;
	Mat temp, dst, bi;
	Mat clone = src.clone();
	cvtColor(src, temp, COLOR_BGR2GRAY);

	if(debug) createDebugTools();

	bilateralFilter(temp, bi, 7, 80, 80); 
	if(debug) {	
		while(1)
		{
			// Thresholding
			Canny(bi, dst, canny_low, canny_high,  5);
			imshow("Canny", dst);
			if(waitKey(1) == 32) break; // Exit loop if space is pressed
		}
	} else {
		Canny(bi, dst, canny_low, canny_high,  5);
	}

	if(debug) imshow("Canny", dst);

	// Morphological operations
	Dilation(dst, 3, 0, 3);
	Erosion(dst, 3, 0, 3);


	if(debug) imshow("morph", dst);
	Mat temp1 = removeBlobs(dst, "small", 500);

	if(debug) imshow("shot_img", dst),	imshow("Result of blob extraction", temp1);
	waitKey();
	findContours(temp1, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	/// Get the moments
	vector<Moments> mu(contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    { 
		 mu[i] = moments( contours[i], false ); 
	}

    ///  Get the mass centers:
    vector<Point> mc( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    {
		mc[i] = Point( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
		shots.push_back(mc[i]); //Copy to shots vector from mdi class
	}
	 for( int i = 0; i< contours.size(); i++ )
     {
       circle( clone, shots[i], 4, Scalar(0,0,255), -1, 8, 0 );
     }
	 namedWindow("SHOTS", CV_WINDOW_NORMAL);
	 imshow("SHOTS", clone);
	 waitKey();
	 destroyAllWindows();
}

// Function called once we have the target and all shots stored. Fills all 
// important grouping information variables in the indicator class.
void indicator::process(cv::Mat& src)
{

	shots_on_target = shots.size();
	accuracy = shots_on_target/total_shots;

	// find centre of target (+)
	Mat temp = src.clone();
	Mat dst_hsv, dst;
	Point target_centre;
	cvtColor(temp, dst_hsv, CV_BGR2HSV);
	
	if(debug) 
	{
		namedWindow  ("Trackbars", CV_WINDOW_NORMAL);
		resizeWindow("Trackbars", 420, 200);

		createTrackbar( "h_min", "Trackbars", &h_min, h_max );
		createTrackbar( "h_max", "Trackbars", &h_max, h_max );
		createTrackbar( "s_min", "Trackbars", &s_min, s_max );
		createTrackbar( "s_max", "Trackbars", &s_max, s_max );
		createTrackbar( "v_min", "Trackbars", &v_min, v_max );
		createTrackbar( "v_max", "Trackbars", &v_max, v_max );

		while(1) 
		{
			inRange(dst_hsv, Scalar(h_min, s_min, v_min), Scalar(h_max, s_max, v_max), dst);
			imshow("thresholding", dst);
			if(waitKey(1) == 32) break; // exit loop of space is pressed. 
		}
	} else {
		inRange(dst_hsv, Scalar(h_min, s_min, v_min), Scalar(h_max, s_max, v_max), dst);
	}

	Erosion(dst, 3, 0, 3);
	Dilation(dst, 5, 0, 3);
	dst = findLargestContour(dst, target_centre);
	if(debug) imshow("DST", dst), waitKey();

	// find mean of all shots

	double mean_x, mean_y;
	double total_x, total_y;
	for (int i = 0; i<shots.size(); i++)
	{
		total_x += shots[i].x;
		total_y += shots[i].y;
	}
	mean_x = total_x/shots.size();
	mean_y = total_y/shots.size();
	mpi = Point(mean_x, mean_y);
	circle(src, mpi, 5, Scalar(255,0,0), 2, 8);
	
	imshow("mpi", src);
	waitKey();
	
	double variance_x, variance_y;
	// find standard deviation
	for( int i = 0; i < shots.size(); i++)
	{
		variance_x += (shots[i].x - mean_x)*(shots[i].x - mean_x);
		variance_y += (shots[i].y - mean_y)*(shots[i].y - mean_y);
	}
	standard_dev_x = sqrt(variance_x/shots.size());
	standard_dev_y = sqrt(variance_y/shots.size());

	cout << "Std_x: " << standard_dev_x << endl;
	cout << "std_y: " << standard_dev_y << endl;
	imshow("mpi", src);
	waitKey();
}

Point findTargetCentre(cv::Mat& src)
{

}




Mat findLargestContour(cv::Mat& im, Point target_centre)
{
	int largest_area=0;
	int largest_contour_index=0;
	vector<vector<Point>> contours; // Vector for storing contour
    vector<Vec4i> hierarchy;
	Mat temp = Mat::zeros( im.size(), CV_8UC3 );

    findContours( im.clone(), contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image
   
    for( int i = 0; i< contours.size(); i++ ) // iterate through each contour.
    {
       double a=contourArea( contours[i],false);  //  Find the area of contour
       if(a>largest_area){
       largest_area=a;
       largest_contour_index=i;                //Store the index of largest contour
       }
    }
	drawContours(temp, contours, largest_contour_index, Scalar(255,255,255), -1, 8);

	/* FIND THE CENTRE OF TARGET */
	/// Get the moments
	Moments mu = moments( contours[largest_contour_index], false);

    ///  Get the mass centers:
    target_centre = Point( mu.m10/mu.m00 , mu.m01/mu.m00 ); 

	return temp;
}

//Remove blobs either above or blow value size
Mat removeBlobs(cv::Mat& im, char* val, double size)
{
	std::vector<std::vector<cv::Point> > contours;
    vector<Vec4i> hierarchy;	

	findContours(im.clone(), contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	
	
	Mat temp = Mat::zeros( im.size(), CV_8UC1 );
	for (size_t i = 0; i < contours.size(); ++i)
	{
		if(hierarchy[i][3]<0)// Only external contours (removes random holes from morph operations) 
		{
			//calculate contour area
			double area = cv::contourArea(contours[i]);

			if (strcmp(val, "small") == 0)
			{
				// Remove small objects by drawing the contour with black color
				if (area > 0 && area <= size)
				{
					cv::drawContours(im, contours, i, Scalar(0,0,0), -1);
					cv::drawContours(temp, contours, i, Scalar(255,255,255), -1);
				}	

			} else {
				if (area >= size)
				{
					cv::drawContours(im, contours, i, Scalar(0,0,0), -1);
				}
			}
		}
	}
	return temp;
}

void Erosion(cv::Mat& im, int iterations, int elem, int size)
{
	int erosion_type;
	if( elem == 0 ){ erosion_type = MORPH_RECT; }
	else if( elem == 1 ){ erosion_type = MORPH_CROSS; }
	else if( elem == 2) { erosion_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement( erosion_type, Size( size, size ), Point(-1,-1) );
	
	erode(im, im, element, Point(-1,-1), iterations, 1);
}

void Dilation(cv::Mat& im, int iterations, int elem, int size)
{
	int erosion_type;
	if( elem == 0 ){ erosion_type = MORPH_RECT; }
	else if( elem == 1 ){ erosion_type = MORPH_CROSS; }
	else if( elem == 2) { erosion_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement( erosion_type, Size( size, size ), Point(-1,-1) );
	
	dilate(im, im, element, Point(-1,-1), iterations, 1);
}


/* NOTE : REDUNDANT */

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
