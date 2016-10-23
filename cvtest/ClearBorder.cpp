///*
//* function name: ClearBorder()
//* description: 
//*		Fill light structures connected to the image border like imclearboder() in Matlab.
//*		Help yourself with parameters of floodFill().
//*		Be aware that dst img has 4 more borders than src img.
//* author: Eswai
//* github: loper-eswai
//*/
//#include<iostream>
//#include<opencv2\opencv.hpp>
//using namespace std;
//using namespace cv;
//
//void ClearBorder(Mat src, Mat &dst){
//	// I <- src image
//	Mat I = src.clone();
//	// bigI <- I with borders added
//	Mat bigI;
//	// add 4 white borders to image
//	copyMakeBorder(I, bigI, 1, 1, 1, 1, BORDER_CONSTANT, Scalar(255));
//	// floodfill from borders
//	floodFill(bigI, Point(0, 0), Scalar(0), 0, Scalar(0), Scalar(0), 8);
//	// output
//	dst = bigI;
//}