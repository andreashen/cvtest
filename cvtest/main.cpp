#include<iostream>
#include<opencv2\opencv.hpp>
#include <sstream>
#include <string>
#include"extr_sift.cpp"
using namespace cv;
using namespace std;

const std::string num2str(int x){
	char str[25];
	_itoa_s(x, str, 10);
	return str;
}
int main()
{
	// load an image
	//Mat img = imread("lena.jpg");
	//imshow("Lena image", img);
	//Mat dstimg;
	std::cout<<num2str(1) + num2str(2);
	namedWindow()
	// wait for any key pressed
	waitKey(0);
}