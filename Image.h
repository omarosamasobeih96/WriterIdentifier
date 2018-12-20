#pragma once
#include <string>
using namespace std;
#include "Matrix.h"

#include <opencv2/opencv.hpp>
using namespace cv;

class Image
{

	static const int blk_wit_thr = 160;

public:
	static void WriteImage(const string &imagePath, Matrix<bool> &matrix) {
		int height = matrix.height;
		int width = matrix.width;
		cv::Mat Image(height, width, CV_8UC1, 255);
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				Image.at<uchar>(i, j) = matrix(i, j) ? 0 : 255;
		imwrite(imagePath, Image);
	}
	static Matrix<bool> ReadImage(const string &imagePath) {
		cv::Mat image = imread(imagePath, CV_LOAD_IMAGE_GRAYSCALE);
		int height = image.rows;
		int width = image.cols;
		Matrix<bool> matrix(height, width);
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j)
				matrix(i, j) = (int)image.at<uchar>(i, j) < blk_wit_thr;
		}
		return matrix;
	}
};

