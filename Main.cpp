#include "mdi.h"

// This is the entry point to the program
int main (int argc, char* argv[])
{
	Mat img = imread("MDItarget.jpg");

	indicator mdi;
	Mat roi = getRoi(img); // Find the ROI aka our target. 
	Mat copy = roi.clone();
	mdi.getShots(roi, mdi.shots); // Find all bullet holes and fill 'shots' vector with their positions

	// Would you like to modify existing shots

	mdi.correctShots(roi); // Manually add & delete shots
	return 0;
}

