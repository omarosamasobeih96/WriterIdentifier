#pragma once

#include "Matrix.h"
#include "BoundingBox.h"
#include "Image.h"
#include "Constants.h"
#include <queue>
using namespace std;
class Preprocessing {
public:

	static vector<BoundingBox> GetComponentsBoundingBoxes(Matrix<bool> matrix) {
		int height = matrix.height;
		int width = matrix.width;
		Matrix<bool> visited(height, width);
		vector<pair<int, int>> dirs = {
			{ 1, 0 },{ -1, 0 },{ 0, 1 },{ 0, -1 }
		};
		vector<BoundingBox> ret;
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j) {
				if (visited(i, j)) continue;
				if (!matrix(i, j)) continue;
				queue<pair<int, int>> q;
				q.push({ i, j });
				BoundingBox tmp;
				visited(i, j) = 1;
				while (q.size()) {
					int x = q.front().first, y = q.front().second;
					tmp.Add(q.front());
					q.pop();
					for (auto dir : dirs) {
						int nx = x + dir.first;
						int ny = y + dir.second;
						if (nx < 0 || nx >= matrix.height) continue;
						if (ny < 0 || ny >= matrix.width) continue;
						if (visited(nx, ny)) continue;
						if (!matrix(nx, ny)) continue;
						q.push({ nx, ny });
						visited(nx, ny) = 1;
					}
				}
				ret.push_back(tmp);
			}
		return ret;
	}

	static vector<Matrix<bool>> GetComponents(Matrix<bool> matrix) {
		int height = matrix.height;
		int width = matrix.width;
		Matrix<bool> visited(height, width);
		vector<pair<int, int>> dirs = {
			{ 1, 0 },{ -1, 0 },{ 0, 1 },{ 0, -1 }
		};
		vector<Matrix<bool>> ret;
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j) {
				if (visited(i, j)) continue;
				if (!matrix(i, j)) continue;
				queue<pair<int, int>> q;
				q.push({ i, j });
				BoundingBox tmp;
				vector<pair<int, int>> pos;
				visited(i, j) = 1;
				while (q.size()) {
					int x = q.front().first, y = q.front().second;
					pos.push_back(q.front());
					tmp.Add(q.front());
					q.pop();
					for (auto dir : dirs) {
						int nx = x + dir.first;
						int ny = y + dir.second;
						if (nx < 0 || nx >= matrix.height) continue;
						if (ny < 0 || ny >= matrix.width) continue;
						if (visited(nx, ny)) continue;
						if (!matrix(nx, ny)) continue;
						q.push({ nx, ny });
						visited(nx, ny) = 1;
					}
				}
				Matrix<bool> cur(tmp.buttom - tmp.top + 1, tmp.right - tmp.left + 1);
				for (auto p : pos) cur(p.first - tmp.top, p.second - tmp.left) = 1;
				ret.push_back(cur);
			}
		return ret;
	}

	static pair<int, int> GetHandWritten(const string &imagePath, Matrix<bool> &image) {
		image = Image::ReadImage(imagePath);
		vector<BoundingBox> boxes = GetComponentsBoundingBoxes(image);
		sort(boxes.rbegin(), boxes.rend());
		if (boxes.size() < 3) return{ -1, -1 };
		vector<int> xs;
		vector<int>allUpperXs;
		vector<int>allLowerXs;
		for (int i = 0; i < boxes.size(); ++i)
		{
			int w = boxes[i].right - boxes[i].left;
			if (w < 20) break;
			allUpperXs.push_back(boxes[i].top);
			allLowerXs.push_back(boxes[i].buttom);
		}
		for (int i = 0; i < 3; ++i) {
			xs.push_back(boxes[i].top);
			xs.push_back(boxes[i].buttom);
		}
		sort(xs.begin(), xs.end());
		sort(allUpperXs.begin(), allUpperXs.end());
		sort(allLowerXs.begin(), allLowerXs.end());

		int maximumStart = upper_bound(allUpperXs.begin(), allUpperXs.end(), xs[3]) - allUpperXs.begin();
		int minimumEnd = lower_bound(allLowerXs.begin(), allLowerXs.end(), xs[4]) - allLowerXs.begin() - 1;

		if (maximumStart == allUpperXs.size()) maximumStart = xs[3] + 1;
		else maximumStart = max(xs[3] + 1, allUpperXs[maximumStart] - 100);

		if (minimumEnd < 0) minimumEnd = xs[4] - 1;
		else minimumEnd = min(xs[4] - 1, allLowerXs[minimumEnd] + 100);

		return{ maximumStart, minimumEnd };
	}

	static Matrix<bool> CompressMatrices(vector<Matrix<bool>> matrices) {
		int add = 100;
		int width = 0, height = 0;
		for (Matrix<bool> &m : matrices) {
			width = max(width, m.width);
			height += m.height + add;
		}
		Matrix<bool> ret(height, width);
		height = 0;
		for (Matrix<bool> &m : matrices) {
			{
				for (int i = 0; i < m.height; ++i)
					for (int j = 0; j < m.width; ++j)
						ret(height + i, j) = m(i, j);
				height += m.height + add;
			}
		}
		return ret;
	}

	static void DrawWithBoundingBoxes(Matrix<bool> &image, vector<BoundingBox> &vec, const string &path) {
		for (auto &box : vec) {
			box.left = max(0, box.left);
			box.top = max(0, box.top);
			box.right = min(box.right, image.width - 1);
			box.buttom = min(box.buttom, image.height - 1);

			for (int i = box.left; i <= box.right; ++i)
				image(box.top, i) = 1, image(box.buttom, i) = 1;
			for (int i = box.top; i <= box.buttom; ++i)
				image(i, box.left) = 1, image(i, box.right) = 1;
		}
		int r = rand() % 10000;
		Image::WriteImage(path, image);
	}

	static void GetWindows(const Matrix<bool> &image, vector<Matrix<bool>> & ret) {
		int rows = (image.height + window_size - 1) / window_size;
		vector<BoundingBox> boxes;
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < image.width; ++j) {
				for (int k = 0; k < window_size; ++k) {
					int row = i * window_size + k;
					if (row >= image.height) break;
					if (image(row, j)) {
						boxes.push_back(BoundingBox(i * window_size, (i + 1) * window_size - 1, j, j + window_size - 1));
						ret.push_back(image.GetSubMatrix(i * window_size, (i + 1) * window_size - 1, j, j + window_size - 1));
						j += window_size - 1;
						break;
					}
				}
			}
		}
	}

	static void CompressImages(vector<string> &readfileNames, const string &writeFileName) {
		vector<Matrix<bool>> images;
		for (string &s : readfileNames) {
			Matrix<bool> cur;
			pair<int, int> rows = GetHandWritten(s, cur);
			images.push_back(cur.GetSubMatrix(rows.first, rows.second, left_margin, cur.width - 1));
		}
		Matrix<bool> CompressedMatrix = CompressMatrices(images);
		Image::WriteImage(writeFileName, CompressedMatrix);
	}

	static void Preprocess(string out, vector<Matrix<bool>> &winds) {
		Matrix<bool> image = Image::ReadImage(out);
		vector<Matrix<bool>> comps = Preprocessing::GetComponents(image);
		for (auto &comp : comps)
			Preprocessing::GetWindows(comp, winds);
	}

};
