#ifndef INCLUDE_FLAG
#define INCLUDE_FLAG
#include<iostream>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;
#endif

// Write & Read for [keyPoint,Mat]
void fsWrite(vector<KeyPoint> keyPoint, Mat src, string filename){
	FileStorage fs(filename, FileStorage::WRITE);
	write(fs, "KeyPoint", keyPoint);
	write(fs, "Mat", src);
	fs.release();
}

void fsRead(vector<KeyPoint> &keyPoint, Mat &dst, string filename){
	FileStorage fs(filename, FileStorage::READ);
	read(fs["KeyPoint"], keyPoint);
	read(fs["Mat"], dst);
	fs.release();
}




// Write and Raed for KeyPoint
void fsWrite(vector<KeyPoint> keyPoint, string filename){
	FileStorage fs(filename, FileStorage::WRITE);
	write(fs, "KeyPoint", keyPoint);
	fs.release();
}
void fsRead(vector<KeyPoint> &keyPoint, string filename){
	FileStorage fs(filename, FileStorage::READ);
	read(fs["KeyPoint"], keyPoint);
	fs.release();
}

// Write and Read for Mat
void fsWrite(Mat src, string filename){
	FileStorage fs(filename, FileStorage::WRITE);
	fs << "Mat" << src;
	fs.release();
}
void fsRead(Mat &dst, string filename){
	FileStorage fs(filename, FileStorage::READ);
	fs["Mat"] >> dst;
	fs.release();
}