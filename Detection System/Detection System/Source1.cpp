#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
	VideoCapture cap("test.wmv"); // open the video file
	if (!cap.isOpened()) { // if not success, end program
		cout << "Cannot open the video file" << endl;
		return -1;
	}

	double fps = cap.get(CV_CAP_PROP_FPS); // get the fps of the video

	cout << "Frames per second: " << fps << endl;

	namedWindow("Tracking Test", CV_WINDOW_AUTOSIZE); // create window called Traking Test

	while (1) {
		Mat frame;

		bool bSuccess = cap.read(frame); // read a new frame from video

		if (!bSuccess) { // if not success, break loop
			cout << "Cannot read the frame from the video file" << endl;
			break;
		}

		imshow("Tracking Test", frame); // show the frame in the Tracking Test window

		if (waitKey(30) == 27) { // if escape pressed for 30ms, break
			cout << "escape key pressed" << endl;
			break;
		}
	}
	return 0;
}
