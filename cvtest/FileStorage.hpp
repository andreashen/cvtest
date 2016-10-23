#ifndef INCLUDE_FLAG
#define INCLUDE_FLAG
#include<iostream>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;
#endif

void fsWrite(vector<KeyPoint> keyPoint, Mat src, string filename);
void fsRead(vector<KeyPoint> &keyPoint, Mat &dst, string filename);

void fsWrite(Mat src, string filename);
void fsRead(Mat &dst, string filename);

void fsWrite(vector<KeyPoint> keyPoint, string filename);
void fsRead(vector<KeyPoint> &keyPoint, string filename);