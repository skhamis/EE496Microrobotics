int * Offset(int curr_xaxis, int curr_yaxis, Mat watershed, Mat tracking) {
	int xaxis, yaxis;

	// empty function for sammy's comparison algorithm which shall determine if correction is needed
	int * compare = Compare(watershed, tracking);

	// after comparison of two Mats occur, we then should know the pixel offset between the tracked 
	// microrobot and the watershed detected paths. Thus we want to apply an apporipriate voltage correction

	// compare should be an array that holds the x and y pixel offsets


}