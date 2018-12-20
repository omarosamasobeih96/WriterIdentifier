#pragma once
#include <fstream>
#include "Constants.h"
template <typename T>
class Matrix {

	T ** matrix;
public:

	int height, width;
	Matrix<T>(int height, int width) {
		this->height = height;
		this->width = width;
		matrix = new T*[height];
		for (int i = 0; i < height; ++i) {
			matrix[i] = new T[width];
			for (int j = 0; j < width; ++j)
				matrix[i][j] = 0;
		}
	}
	Matrix(const Matrix<T> &cpy) {
		height = cpy.height;
		width = cpy.width;
		matrix = new T*[height];
		for (int i = 0; i < height; ++i) {
			matrix[i] = new T[width];
			for (int j = 0; j < width; ++j)
				matrix[i][j] = cpy(i, j);
		}
	}
	Matrix() {
		height = width = 0;
		matrix = new T*[0];
	}
	Matrix& operator = (const Matrix<T> &rhs) {
		for (int i = 0; i < height; ++i)
			delete[] matrix[i];
		delete[] matrix;
		height = rhs.height;
		width = rhs.width;
		matrix = new T*[height];
		for (int i = 0; i < height; ++i) {
			matrix[i] = new T[width];
			for (int j = 0; j < width; ++j)
				matrix[i][j] = rhs(i, j);
		}
		return *this;
	}
	T& operator() (int row, int col) const {
		return matrix[row][col];
	}
	Matrix GetSubMatrix(int strtRow, int endRow, int strtCol, int endCol) const {
		Matrix<T> ret(endRow - strtRow + 1, endCol - strtCol + 1);
		for (int i = strtRow, j = 0; i <= endRow; ++i, ++j)
			for (int k = strtCol, l = 0; k <= endCol; ++k, ++l) {
				if (i < height && k < width)
					ret(j, l) = matrix[i][k];
			}
		return ret;
	}
	Matrix GetTranspose() const {
		Matrix ret(width, height);
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				ret(j, i) = matrix[i][j];
		return ret;
	}

	~Matrix() {
		for (int i = 0; i < height; ++i)
			delete[] matrix[i];
		delete[] matrix;
	}
};
