#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <zbar.h>  
#include <iostream>  
using namespace cv;
using namespace std;
using namespace zbar;

int main(int argc, char* argv[])
{
	VideoCapture cap(0); // open the video camera no. 0  

	if (!cap.isOpened()) // if not success, exit program  
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}
	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video  
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video  
	cout << "Frame size : " << dWidth << " x " << dHeight << endl;
	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"  
	while (1)
	{
		Mat frame;
		bool bSuccess = cap.read(frame); // read a new frame from video  
		if (!bSuccess) //if not success, break loop  
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
		Mat grey;
		cvtColor(frame, grey, CV_BGR2GRAY);
		int width = frame.cols;
		int height = frame.rows;
		uchar *raw = (uchar *)grey.data;
		// wrap image data   
		Image image(width, height, "Y800", raw, width * height);
		// scan the image for barcodes   
		int n = scanner.scan(image);
		string command = "";
		// extract results   
		for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
			vector<Point> vp;
			// do something useful with results   
			command = symbol->get_data();
			cout << "decoded " << symbol->get_type_name() << " symbol \"" << command << '"' << " " << endl;
			int n = symbol->get_location_size();
			for (int i = 0; i<n; i++) {
				vp.push_back(Point(symbol->get_location_x(i), symbol->get_location_y(i)));
			}
		}

		// Displaying back and white image with command written on the frame
		Mat displayFrame;
		cvtColor(frame, displayFrame, CV_BGR2GRAY);
		putText(displayFrame, command, cvPoint(10, 100), FONT_HERSHEY_COMPLEX_SMALL, 4, cvScalar(0, 0, 0), 1, CV_AA);

		imshow("MyVideo", displayFrame); //show the frame in "MyVideo" window  
		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop  
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	return 0;
}