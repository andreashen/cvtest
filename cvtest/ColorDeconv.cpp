#include<iostream>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;


// Color Deconvolution: 
//------separate the RGB image into specific(HEB) stain channels
Mat ColorDeconv(Mat I){
	//=======STEP 1=======
	// rgbI <-- RGB channels of (I) with 3 channels & 64-bit precision
	Mat rgbI;
	if (I.type() == CV_64FC3)
		I.convertTo(I, CV_32FC3);

	// Careful! Image matrix read in OpenCV is in BGR color.
	cvtColor(I, rgbI, CV_BGR2RGB);
	
	switch (rgbI.type())
	{
	case CV_8UC3:
		rgbI.convertTo(rgbI, CV_64FC3, 1.0 / 255);
		break;
	case CV_64FC3:break;
	case CV_32FC3:
		rgbI.convertTo(rgbI, CV_64FC3);
		break;
	default:
		// just for check in test
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
	Mat M = (Mat_<float>(3, 3) <<
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
	// D <-- M^-1
	Mat D = M.inv();
	//======STEP 4=======
	// odHEB <-- optical density of HEB image
	//		 <-- D^T * odRGB
	Mat odHEB;
	D = D.t();
	transform(odRGB, odHEB, D);

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
	//======RETURN=======
	return output;
}

