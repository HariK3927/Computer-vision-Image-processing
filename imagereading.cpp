#include <opencv2/opencv.hpp> 
#include <iostream> 

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	// Read the image file 
	Mat image = imread("lena.jpg");
	// Check for failure 
	if (image.empty())
	{
		cout << "Image Not Found!!!" << endl;
		cin.get(); //wait for any key press 
		return -1;
	}
	int rows = image.rows;
	int cols = image.cols;
	int channels = image.channels();
	int totalSize = rows * cols * channels;
	cout << "rows" << rows << endl;
	cout << "cols" << cols << endl;
	cout << "xhannels" << channels << endl;
	cout << "total size" << totalSize << endl;
	// Show our image inside a window. 
	imshow("Image Window Name here", image);

	// Wait for any keystroke in the window 
	waitKey(0);
	return 0;
}
