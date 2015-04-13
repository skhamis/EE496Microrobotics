#include "Shape_Detection.h"

int BFILTERVAL = 900;
int lowThreshold = 50; // good at 86
int ratio = 3;


void help()
{
	cout << "\nThis program demonstrates line finding with the Hough transform.\n"
		"Usage:\n"
		"./houghlines <image_name>, Default is course.png\n" << endl;
}

int main(int argc, char** argv)
{
	const char* filename = argc >= 2 ? argv[1] : "course2.png";

	Mat src = imread(filename, 0);
	if (src.empty())
	{
		help();
		cout << "can not open " << filename << endl;
		return -1;
	}

	Mat dst, cdst, src_gray;
	// filter testing
	//bilateralFilter(src, src_gray, 9, BFILTERVAL*2, BFILTERVAL/2); // 
	medianBlur(src, src_gray, 9);
	imshow("Filter Application", src_gray);
	Canny(src_gray, dst, lowThreshold, lowThreshold*ratio, 3);
	//Canny(src, dst, lowThreshold, lowThreshold*ratio, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);
	imshow("Canny edge detection", cdst);

#if 0
	vector<Vec2f> lines;
	HoughLines(dst, lines, 1, CV_PI / 180, 100, 0, 0);

	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
	}
#else
	vector<Vec4i> lines;
	// threshold, minlinelength, max line gap
	HoughLinesP(cdst, lines, 1, CV_PI / 180, 50, 100, 10); // original: 50,50,10 course2: 70,100,10 course1: 20,0,10
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
	}
#endif
	imshow("source", src);
	imshow("detected lines", cdst);

	waitKey();

	return 0;
}