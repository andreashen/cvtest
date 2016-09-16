#include<opencv2\opencv.hpp>

// change it if necessary £¡£¡
#define MAX_IMAGE_SIZE 1360

using namespace std;
extern void CalculateSiftDescriptor(
	const string &img_dir,
	const string &data_dir,
	int gridSpacing,
	int patchSize,
	int maxImSize,
	int nrml_threshold);

void extr_sift(const string &img_dir, const string &data_dir){
	int gridSpacing = 8;
	int patchSize = 16;
	int maxImSize = MAX_IMAGE_SIZE;
	int nrml_threshold = 1;
	CalculateSiftDescriptor(img_dir, data_dir, gridSpacing, patchSize, maxImSize, nrml_threshold);
}

