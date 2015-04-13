#include "Shape_Detection.h"

int * Compare(Mat watershed, Mat tracking) {
	int compare[2];
	
	// hardcoded offsets for testing purposes

	compare[0] = 5;
	compare[1] = 6;

	return compare;
}

int * Offset(int curr_xaxis, int curr_yaxis, Mat watershed, Mat tracking) {
	int xaxis = 0, yaxis = 0;
	int VOLT_OFFSET = 0.1;
	int xyaxis[2];

	// empty function for sammy's comparison algorithm which shall determine if correction is needed
	int * compare = Compare(watershed, tracking);

	// after comparison of two Mats occur, we then should know the pixel offset between the tracked 
	// microrobot and the watershed detected paths. Thus we want to apply an apporipriate voltage correction

	if (compare[0] != 0) { // testing for x axis
		xaxis = curr_xaxis + (compare[0] * VOLT_OFFSET);
		}
	
	if (compare[1] != 0) { // testing for y axis
		yaxis = curr_yaxis + (compare[1] * VOLT_OFFSET);
		}

	// setup for the return
	xyaxis[0] = xaxis;
	xyaxis[1] = yaxis;
	return xyaxis;

}