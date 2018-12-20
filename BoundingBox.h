#pragma once
#include <algorithm>
using namespace std;

class BoundingBox {
	
public:
	int left, right;
	int top, buttom;
	BoundingBox() {
		left = top = (int)1e9;
		right = buttom = (int)-1e9;
	}
	BoundingBox(int top, int buttom, int left, int right) {
		this->left = left;
		this->top = top;
		this->right = right;
		this->buttom = buttom;
	}
	void Add(pair<int, int> pos) {
		top = min(pos.first, top);
		buttom = max(pos.first, buttom);
		left = min(pos.second, left);
		right = max(pos.second, right);
	}
	bool operator < (const BoundingBox& rhs) const {
		int width = right - left;
		int wridth = rhs.right - rhs.left;
		return width < wridth;
	}	
};