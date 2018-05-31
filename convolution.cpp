// g++ convolution.cpp `pkg-config --cflags --libs opencv` -std=c++11 && ./a.out example.png sharpen
// ./a.out example.png sharpen

#include <cstdio>
#include <iostream>
#include <algorithm>
#include <opencv2/highgui.hpp>

#define FMAX 47

using namespace std;

cv::Mat img;

int fsize;
float mult;
short int f[FMAX][FMAX];

int stride = 1;

cv::Mat res;

int ceil(int a, int b){
	return a / b + !!(a % b);
}

int imsafe(int row, int col){
	// corners
	if(row < 0 && col < 0){ row = 0; col = 0; } else
	if(row < 0 && col >= img.cols){ row = 0; col = img.cols - 1; } else
	if(row >= img.rows && col >= img.cols){ row = img.rows - 1; col = img.cols - 1; } else
	if(row >= img.rows && col < 0){ row = img.rows - 1; col = 0; } else
	// edges
	if(row < 0){ row = 0; } else
	if(col >= img.cols){ col = img.cols - 1; } else
	if(row >= img.rows){ row = img.rows - 1; } else
	if(col < 0){ col = 0; }

	return img.at<unsigned char>(row, col);
}

void applyf(int row, int col){
	int offset = fsize / 2;

	int sum = 0;
	for(int frow = 0, imrow = row - offset; frow < fsize; frow++, imrow++){
		for(int fcol = 0, imcol = col - offset; fcol < fsize; fcol++, imcol++){
			sum += f[frow][fcol] * imsafe(imrow, imcol);
		}
	}

	res.at<unsigned char>(row / stride, col / stride) = max(0, min(255, (int) (mult * sum)));
}

void conv(int stride){
	for(int row = 0; row < img.rows; row += stride){
		for(int col = 0; col < img.cols; col += stride){
			applyf(row, col);
		}
	}
}

int main(int argc, char** argv){
	img = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
	
	FILE *filter = fopen(argv[2], "r");
	fscanf(filter, "%d %f", &fsize, &mult);
	for(int row = 0; row < fsize; row++){
		for(int col = 0; col < fsize; col++){
			fscanf(filter, "%hd", &f[row][col]);
		}
	}

	if(argc == 4){
		stride = atoi(argv[3]);
	}
	
	res = cv::Mat(ceil(img.rows, stride), ceil(img.cols, stride), img.type());
	conv(stride);
	
	char outname[128];
	sprintf(outname, "convolution_%s%s_%s", argv[2], argc == 4 ? argv[3] : "", argv[1]);
	cv::imwrite(outname, res);

	return 0;
}
