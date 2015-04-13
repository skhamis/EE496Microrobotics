#include "Shape_Detection.h"

/*
	Begin Watershed Code
*/



static void help()
{
    cout << "\nThis program demonstrates the famous watershed segmentation algorithm in OpenCV: watershed()\n"
            "Usage:\n"
            "./watershed [image_name -- default is course2.png]\n" << endl;


    cout << "Hot keys: \n"
        "\tESC - quit the program\n"
        "\tr - restore the original image\n"
        "\tw or SPACE - run watershed segmentation algorithm\n"
        "\t\t(before running it, *roughly* mark the areas to segment on the image)\n"
        "\t  (before that, roughly outline several markers on the image)\n";
}
Mat markerMask, img;
Point prevPt(-1, -1);

/* Image Marking Code? */
static void onMouse( int event, int x, int y, int flags, void* )
{
    if( x < 0 || x >= img.cols || y < 0 || y >= img.rows )
        return;
    if( event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON) )
        prevPt = Point(-1,-1);
    else if( event == CV_EVENT_LBUTTONDOWN )
        prevPt = Point(x,y);
    else if( event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) )
    {
        Point pt(x, y);
        if( prevPt.x < 0 )
            prevPt = pt;
        line( markerMask, prevPt, pt, Scalar::all(255), 5, 8, 0 );
        line( img, prevPt, pt, Scalar::all(255), 5, 8, 0 );
        prevPt = pt;
        imshow("image", img);
    }
}

Mat Watershed( Mat frame )
{
    //char* filename = argc >= 2 ? argv[1] : (char*)"course2.png";	// Read image? Here is where we would input the video frame
    //Mat img0 = imread(filename, 1), imgGray;	// This actually reads the image and converts it to greyscale
	Mat img0 = frame;
	Mat wshed;
	int flag = 1;

	// safety code for proper file opening
	/*
	if( img0.empty() )
    {
        cout << "Couldn't open image " << filename << ". Usage: watershed <image_name>\n";
        return 0;
    }
	*/
	// Display the commands for marking the image
    help();
	//waitKey(0);
    namedWindow( "image", 1 );

	// Code for reading mouse input on image?
    img0.copyTo(img);
    cvtColor(img, markerMask, COLOR_BGR2GRAY);		// takes img, converts it to grey, and names it markerMask
    cvtColor(markerMask, frame, COLOR_GRAY2BGR);	//takes markerMask, converts it to BGR? and then names it imgGrey???
    markerMask = Scalar::all(0);	// not sure what this does. Converts all the points in the matrix to scalars?
    imshow( "image", img );		// image display to user
    setMouseCallback( "image", onMouse, 0 );	// Here is where the mouse input is actually read?

	while (flag == 1)	// loop
    {
        int c = waitKey(0);	// wait for the user's key inputs

        if( (char)c == 27 )	// user entered quit key
            break;

        if( (char)c == 'r' )	// user wishes to restore the image to its original
        {
            markerMask = Scalar::all(0);	// deletes current markerMask?
            img0.copyTo(img);				// grabs the original image again
            imshow( "image", img );			// displays the image
        }

        if( (char)c == 'w' || (char)c == ' ' )	// user wishes to run the watershed algorithm
        {
            int i, j, compCount = 0;
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;

			// find contours in binary image markerMask (which will be the edges?
            findContours(markerMask, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

            if( contours.empty() )	// if contours are not detected easily, we shall use the markers?
                continue;
            Mat markers(markerMask.size(), CV_32S);	// matrix holding the markerMask
            markers = Scalar::all(0);	// convert data points to scalars?
            int idx = 0;
            for( ; idx >= 0; idx = hierarchy[idx][0], compCount++ )	// not sure what this loop is doing. drawContours with drawn lines as reference?
                drawContours(markers, contours, idx, Scalar::all(compCount+1), -1, 8, hierarchy, INT_MAX);

            if( compCount == 0 )	// not sure
                continue;

            vector<Vec3b> colorTab;
            for( i = 0; i < compCount; i++ )	// code for choosing the coloring of different regions?
            {
                int b = theRNG().uniform(0, 255);
                int g = theRNG().uniform(0, 255);
                int r = theRNG().uniform(0, 255);

                colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
            }

            double t = (double)getTickCount();
            watershed( img0, markers );
            t = (double)getTickCount() - t;
            printf( "execution time = %gms\n", t*1000./getTickFrequency() );

            Mat wshed(markers.size(), CV_8UC3);

            // paint the watershed image
            for( i = 0; i < markers.rows; i++ )
                for( j = 0; j < markers.cols; j++ )
                {
                    int index = markers.at<int>(i,j);
                    if( index == -1 )
                        wshed.at<Vec3b>(i,j) = Vec3b(255,255,255);
                    else if( index <= 0 || index > compCount )
                        wshed.at<Vec3b>(i,j) = Vec3b(0,0,0);
                    else
                        wshed.at<Vec3b>(i,j) = colorTab[index - 1];
                }

            wshed = wshed*0.5 + frame*0.5;
            imshow( "watershed transform", wshed );
			int flag = 0;
        }
		
    }
    return wshed;
}


/*
	End Watershed Code
*/



int main(int argc, char* argv[])
{
	Mat wshed;
	VideoCapture cap("test2.mp4"); // open the video file for reading (test.wmv or test2.mp4). This needs to change to be read from the camera

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		return -1;
	}

	//cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

	double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video

	cout << "Frame per seconds : " << fps << endl;

	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

	
	// we will want to read one frame here for watershed purposes
	Mat frame;
	bool bSuccess = cap.read(frame);
	if (!bSuccess) {
		cout << "Cannot read a frame" << endl;
	}
	wshed = Watershed(frame);	// 
	int count = 0;
	
	//while (bSuccess = cap.read(frame))
	while (1)
	{
		Mat frame;
		bSuccess = cap.read(frame);
		count++;

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}
		
		// application of the tracking algorithm
		frame = Shape_Detection(frame);
		imshow("MyVideo", frame); //show the frame in "MyVideo" window

		if (waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;

}