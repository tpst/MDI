#include "mdi.h"
#include <fstream>

// This is the entry point to the program
// Note: to remove console for release go to properties->linker->system and change
// subsystem to "windows". Then rename main to 'WinMain'
int main(int argc, char* argv[])
{
	// Display instructions & load image
    Mat img = load_image();

	ofstream mfile;
	mfile.open("mdi_output.txt");

	indicator mdi; // create mdi class

	Mat roi = getRoi(img); // Find the ROI aka our target. 

	mdi.getShots(roi, mdi.shots); // Find all bullet holes and fill 'shots' vector with their positions

	// Would you like to modify existing shots

	mdi.correctShots(roi); // Manually add & delete shots
	
	mdi.process(roi); // calculates relevant grouping information

	mfile << "# Shots On Target = " << mdi.shots.size() << endl;
	mfile << "Mean Point Impact (mpi) = " << Point2f(mdi.mean_x, mdi.mean_y) << endl;
	mfile << "Standard deviation x = " << mdi.standard_dev_x << endl;
	mfile << "Standard deviation y = " << mdi.standard_dev_y << endl;
	mfile << "MISS DISTANCES" << endl;
	mfile << "MD Mean = " << mdi.miss_distance_mean << endl;
	mfile << "MD Std Dev = " << mdi.miss_distance_std << endl;
	mfile << "Shot Locations" << endl;
	for( int i = 0; i < mdi.shots.size(); i++)
	{
		double temp_x, temp_y;
		temp_x = (mdi.shots[i].x - mdi.target_centre.x)/mdi.ratio_x;
		temp_y = -(mdi.shots[i].y - mdi.target_centre.y)/mdi.ratio_y;// Convert to meters, origin is target centre. 
		mfile << Point2f(temp_x, temp_y) << endl;
	}

	return 0;
}

