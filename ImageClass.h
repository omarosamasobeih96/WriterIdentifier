#pragma once
#include "Matrix.h"
#include "Constants.h"


class ImageClass
{
public:
	Matrix<double> A, B;
	Matrix<bool> Mean;
	map<string, double> writers, writersProp;
	int size;
	ImageClass() {
		Mean = Matrix<bool>(window_size, window_size);
		A = Matrix<double>(window_size, window_size);
		B = Matrix<double>(window_size, window_size);
		size = 0;
	}
	ImageClass(const Matrix<bool> & m, const string & writerId , double  pr)
	{
		Mean = Matrix<bool>(window_size, window_size);
		A = Matrix<double>(window_size, window_size);
		B = Matrix<double>(window_size, window_size);
		size = 0;
		Add(m, writerId , pr);
	}
	void Add(const Matrix<bool> & m, const string &  writerId , double  pr)
	{
		size++;
		UpdateMean(m);
		writers[writerId]+= pr;
	}
	void UpdateMean(const Matrix<bool> & m)
	{
		for (int i = 0; i<window_size; ++i)
			for (int j = 0; j < window_size; ++j)
			{
				A(i, j) += m(i, j);
				B(i, j) += 1;
				Mean(i, j) = (A(i, j) / B(i, j)) >= 0.5;
			}
	}
	void CalcProp(int sum) {
		for (auto &it : writers) {
			writersProp[it.first] = it.second / (1.0 * size);
		}
	}
	bool operator < (const ImageClass & m) const
	{
		return size < m.size;
	}
};

