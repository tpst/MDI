#include "mdi.h"

// This is the entry point to the program
// Note: to remove console for release go to properties->linker->system and change
// subsystem to "windows". Then rename main to 'WinMain'
int main(int argc, char* argv[])
{
	// Display instructions & load image
    Mat img = load_image();

	indicator mdi; // create mdi class

	Mat roi = getRoi(img); // Find the ROI aka our target. 

	mdi.getShots(roi, mdi.shots); // Find all bullet holes and fill 'shots' vector with their positions

	// Would you like to modify existing shots

	mdi.correctShots(roi); // Manually add & delete shots
	
	mdi.process(roi); // calculates relevant grouping information

	return 0;
}

