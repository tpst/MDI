#include "mdi.h"

// This is the entry point to the program
// Note: all grouping information such as miss distance, mean, etc is measured from the centre of the target. 
int main(int argc, char* argv[])
{

	indicator mdi; // create mdi class

	// Display instructions & load image
    Mat img = load_image();

	Mat roi = getRoi(img); // Find the ROI aka our target. 

	mdi.getShots(roi, mdi.shots); // Find all bullet holes and fill 'shots' vector with their positions

	mdi.correctShots(roi); // Manually add & delete shots
	
	mdi.process(roi); // calculates relevant grouping information

	mdi.save_output();

	return 0;
}

