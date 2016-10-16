#include<iostream>
#include<opencv2\opencv.hpp>

//#include"extr_sift.cpp"

using namespace cv;
using namespace std;
Mat PrePro(const string &filename);
Mat PrePro(Mat I);
Mat ColorDeconv(Mat I);
void SwapRow13(Mat mIn, Mat &mOut){
	Mat M = mIn.clone();
	Mat rowData[3];
	for (int i = 0; i < 3; i++){
		rowData[i] = M.rowRange(i, i + 1).clone();
	}
	Mat swapM;
	for (int j = 2; j >-1; j--){
		swapM.push_back(rowData[j]);
	}
	mOut = swapM.clone();
}

void multi2(Mat I){
	Mat g =I;
	I = g * 3;
	g = g * 2;
	g.convertTo(g, CV_8UC1);
	g = g * 2;
	I = g*3;
}
void ClearBorder(Mat src, Mat &dst){
	// I <- src image
	Mat I = src.clone();
	// bigI <- I with borders added
	Mat bigI;
	// add 4 white borders to image
	copyMakeBorder(I, bigI, 1, 1, 1, 1, BORDER_CONSTANT, Scalar(255));
	// floodfill from borders
	floodFill(bigI, Point(0, 0), Scalar(0), 0, Scalar(0), Scalar(0), 8);
	// output
	dst = bigI;
}

int main(){
	//cout << (1 << 3) << endl;
	////cout << "CV_8UC3=" << CV_8UC3 << endl;  //16
	////cout << "CV_32FC3=" << CV_32FC3 << endl;//21
	////cout << "CV_64FC3=" << CV_64FC3 << endl;//22
	//getchar();
	//return 2;
	Mat I = imread("lumens.jpg");
	cvtColor(I, I, CV_BGR2GRAY);
	threshold(I, I, 128, 255, CV_THRESH_BINARY);
	Mat dst;
	ClearBorder(I, dst);
	imwrite("lumens_cb.tif", dst);
	return 1;
	Mat M = (Mat_<float>(3, 3) <<
		1, 2, 3, 4, 5, 6, 7.2, 8, 9);
	cout << "M.type()=" << M.type() << endl;
	cout << "M=" << endl << M << endl;
	multi2(M);
	cout << "M.type()=" << M.type() << endl;
	cout << "M=" << endl << M << endl;
	getchar();
	return 1;
}

//color deconv
/*
int main()
{
	Mat I = imread("test.tif");
	cvtColor(I, I, CV_BGR2RGB);
	//=======STEP 1=======
	// rgbI <-- RGB channels of (I) with 3 channels & 64-bit precision
	Mat rgbI = I.clone();
	switch (rgbI.type())
	{
	case CV_8UC3:
		//rgbI.convertTo(rgbI, CV_64FC3);
		//rgbI /= 255.0;
		rgbI.convertTo(rgbI, CV_64FC3, 1.0 / 255);
		break;
	case CV_64FC3:break;
	case CV_32FC3:
		rgbI.convertTo(rgbI, CV_64FC3);
		break;
	default:
		cout << "Type of image doesn't belong to {8U,32F,64F}" << endl;
		break;
	}
	//=======STEP 2=======
	// odRGB <-- optical density of (I)
	Mat odRGB;
	odRGB = rgbI + 1 / 255.0;
	log(odRGB, odRGB);
	odRGB = -(255 * odRGB) / log(255);
	//=======STEP 3=======
	// M <-- stain matrix of HE
	Mat M = (Mat_<double>(3, 3) <<
		0.644211, 0.716556, 0.266844,
		0.092789, 0.954111, 0.283111,
		0.759199, 0, 0.921218);
	//SwapRow13(M, M);

	// normalize M by L2-NORM in every row
	// rowData: used to extract each row of (M)
	Mat rowData;
	for (int i = 0; i < 3; i++){
		rowData = M.rowRange(i, i + 1);
		normalize(rowData, rowData);
	}
	// D <-- M^-1
	Mat D = M.inv();
	//======STEP 4=======
	// odHEB <-- optical density of HEB image
	//		 <-- D^T * odRGB
	Mat odHEB;
	D = D.t();
	//Mat rowD[3];
	//for (int i = 0; i < 3; i++){
	//	rowD[i] = D.rowRange(i, i + 1).clone();
	//}
	//// swapD <-- BGR order of D (swap Bchannel & Rchannel)
	//Mat swapD;
	//swapD.push_back(rowD[2]);
	//swapD.push_back(rowD[1]);
	//swapD.push_back(rowD[0]);
	//cout << swapD << endl;
	transform(odRGB, odHEB, D);
	Mat c_odHEB[3];
	split(odHEB, c_odHEB);
	imshow("H", c_odHEB[0]);
	waitKey(0);


	// ************ HERE IS ANOTHER BETA VERSION***********************
	//// odHEB <-- optical density of HEB image
	//Mat odHEB;

	//// c_odRGB <-- 3-element array of 3 channels of optical density of RGB image
	//Mat c_odRGB[3];
	//split(odRGB, c_odRGB);
	//// c_odHEB <-- 3-element array of 3 channels of optical density of HEB image
	//Mat c_odHEB[3];
	//vector<Mat> c_odHEB;
	//for (int c = 0; c < 3; c++){
	//	c_odHEB.push_back(D.at<double>(0, c)*c_odRGB[0]
	//		+ D.at<double>(1, c)*c_odRGB[1]
	//		+ D.at<double>(2, c)*c_odRGB[2]);
	//}
	//merge(c_odHEB, odHEB);
	//imshow("hava a go", c_odHEB[0]/255.0);
	//waitKey(0);
	//return 4;
	//======STEP 5=======
	// hebI <-- HEB channels of image (I)
	Mat hebI;
	hebI = -(odHEB - 255)*log(255) / 255.0;
	exp(hebI, hebI);
	//=======STEP 6=======
	// Before thresholding, convert (hebI) to 32-bit matrix
	// because cv::threshold can handle only 8-bit & 32-bit
	hebI.convertTo(hebI, CV_32FC3);
	threshold(hebI, hebI, 255, 255, THRESH_TRUNC);
	hebI = hebI + 0.5;
	Mat output;
	// here is no need to multiply 255
	hebI.convertTo(output, CV_8UC3);

	Mat HEB = output.clone();
	Mat cHEB[3];
	split(HEB, cHEB);
	imshow("H channel", cHEB[0]);
	imwrite("H_1012.tif", cHEB[0]);
	waitKey(0);
	imshow("E channel", cHEB[1]);
	imwrite("E_1012.tif", cHEB[1]);
	waitKey(0);
	imshow("Back", cHEB[2]);
	imwrite("B_1012.tif", cHEB[2]);
	waitKey(0);
	return 1;

}
**/

Mat PrePro(const string &filename){
	Mat I = imread(filename);
	return PrePro(I);
}

Mat PrePro(Mat I){
	// mblur <-- dst image after median blur
	Mat mblur;
	medianBlur(I, mblur, 3);
	// res_64F <-- result with 64-bit precision to be output
	Mat res_64F;
	mblur.convertTo(res_64F, CV_64FC3, 1.0 / 255);

	return res_64F;
}

//
//Mat ColorDeconv(Mat I){
//	//=======STEP 1=======
//	// rgbI <-- RGB channels of (I) with 3 channels & 64-bit precision
//	Mat rgbI = I.clone();
//	normalize(rgbI, rgbI, 0.0, 1.0, NORM_MINMAX, CV_64FC3);
//	//=======STEP 2=======
//	// odRGB <-- optical density of (I)
//	Mat odRGB;
//	odRGB = rgbI + 1 / 255.0;
//	log(odRGB, odRGB);
//	odRGB = -(255 * odRGB) / log(255);
//	//=======STEP 3=======
//	// M <-- stain matrix of HE
//	Mat M = (Mat_<double>(3, 3) <<
//		0.644211, 0.716556, 0.266844,
//		0.092789, 0.954111, 0.283111,
//		0.759199, 0, 0.921218);
//	// normalize M by L2-NORM in every row
//	// rowData: used to extract each row of (M)
//	Mat rowData;
//	for (int i = 0; i < 3; i++){
//		rowData = M.rowRange(i, i + 1);
//		normalize(rowData, rowData);
//	}
//	rowData.~Mat();
//	// D <-- M^-1
//	Mat D = M.inv();
//	//======STEP 4=======
//	// odHEB <-- optical density of HEB image
//	Mat odHEB;
//	// odHEB <-- D^T * odRGB
//	transform(odRGB, odHEB, D.t());
//	//======STEP 5=======
//	// hebI <-- HEB channels of image (I)
//	Mat hebI;
//	hebI = -(odHEB - 255)*log(255) / 255.0;
//	exp(hebI, hebI);
//	//=======STEP 6=======
//	threshold(hebI, hebI, 255, 255, THRESH_TRUNC);
//	hebI = hebI + 0.5;
//	Mat output;
//	hebI.convertTo(output, CV_8UC3, 255.0);// FIX LATER!!!!
//	//======RETURN=======
//	return output;
//}