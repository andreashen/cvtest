#include<iostream>
#include<opencv2\opencv.hpp>

//#include"extr_sift.cpp"

using namespace cv;
using namespace std;
Mat PrePro(const string &filename);
Mat PrePro(Mat I);
Mat ColorDeconv(Mat I);


Mat multi2(Mat a){
	Mat b = a;
	b = a * 2;
	return b;
}
int main()
{
	Mat I = imread("test.tif");
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
	// normalize M by L2-NORM in every row
	// rowData: used to extract each row of (M)
	Mat rowData;
	for (int i = 0; i < 3; i++){
		rowData = M.rowRange(i, i + 1);
		normalize(rowData, rowData);
	}
	rowData.~Mat();
	// D <-- M^-1
	Mat D = M.inv();

	//======STEP 4=======  HERE IS UNDER CONSTRUCTION!!!!!!!!========================================
	//======STEP 4=======
	// odHEB <-- optical density of HEB image
	//		 <-- D^T * odI
	Mat odHEB;
	D = D.t();
	cout << "D=" << D << endl;
	getchar();
	Mat rowD[3];
	for (int i = 0; i < 3; i++){
		rowD[i] = D.rowRange(i, i + 1).clone();
	}
	// swapD <-- BGR order of D (swap Bchannel & Rchannel)
	Mat swapD;
	swapD.push_back(rowD[2]);
	swapD.push_back(rowD[1]);
	swapD.push_back(rowD[0]);
	cout << swapD << endl;
	getchar();
	transform(odRGB, odHEB, swapD);
	Mat c_odHEB[3];
	split(odHEB, c_odHEB);
	imshow("H", c_odHEB[0]);
	waitKey(0);
	//// odHEB <-- optical density of HEB image
	//Mat odHEB;

	//// c_odRGB <-- 3-element array of 3 channels of optical density of RGB image
	//Mat c_odRGB[3];
	//split(odRGB, c_odRGB);
	//// c_odHEB <-- 3-element array of 3 channels of optical density of HEB image
	///*Mat c_odHEB[3];*/
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
	waitKey(0);
	imshow("E channel", cHEB[1]);
	waitKey(0);
	imshow("Back", cHEB[2]);
	waitKey(0);
	return 1;

}

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