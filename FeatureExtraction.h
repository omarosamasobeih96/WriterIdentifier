#pragma once
#include "Matrix.h"
#include "ImageClass.h"
#include "Image.h"
#include "Constants.h"
#include "Preprocessing.h"
#include <bitset>
using namespace std;

typedef bitset<area> BS;
typedef pair<BS, BS> MatE;
bitset<area> zeroBitSet(0);
class FeatureExtraction
{
public:
	static MatE GetBitset(const Matrix<bool> &mx) {
		MatE mte;
		int height = mx.height, width = mx.width;
		for (int i = 0, k = 0; i < height; ++i)
			for (int j = 0; j < width; ++j, ++k)
				mte.first[k] = mx(i, j), mte.second[k] = !mx(i, j);
		return mte;
	}
	static double CalculateSimilarity(const MatE& lhs, const MatE& rhs) {
		double n00 = 0, n11 = 0, n10 = 0, n01 = 0;
		n00 = area - (lhs.first | rhs.first).count();
		n10 = (lhs.first & rhs.second).count();
		n01 = (lhs.second & rhs.first).count();
		n11 = (lhs.first & rhs.first).count();
		double ret = (n11*n00 - n10*n01) / sqrt(1.0 * (n11 + n10) * (n01 + n00) * (n11 + n01) * (n10 + n00));
		return ret;
	}
	static bool IsIdentical(const MatE& lhs, const MatE& rhs) {
		return (zeroBitSet == (lhs.first ^ rhs.first));
	}
	static vector<ImageClass> AssignWindows(const vector<pair<vector<Matrix<bool> >, string>> & allWinds)
	{
		vector<ImageClass>ret;
		vector<MatE> cor;
		int cnt = 0;
		for (auto &writerWinds : allWinds) {
			string writerId = writerWinds.second;
			const vector<Matrix<bool> >& winds = writerWinds.first;
			for (int i = 0; i < winds.size(); ++i)
			{
				cnt++;
				int idx = -1;
				double maxi = -1;
				MatE equi = GetBitset(winds[i]);
				for (int j = 0; j < ret.size(); ++j)
				{
					double s = CalculateSimilarity(cor[j], equi);
					if (s < similarity_threshold) continue;
					if (s > maxi)
					{
						maxi = s;
						idx = j;
					}
				}

				if (cnt % 4000 == 0) 
					cout << ret.size() << " image classes have been created\n";

				if (idx != -1)
				{
					ret[idx].Add(winds[i], writerId , 1.0/winds.size());
					cor[idx] = GetBitset(ret[idx].Mean);
				}
				else
				{
					ret.push_back(ImageClass(winds[i], writerId , 1.0 / winds.size()));
					cor.push_back(GetBitset(ret.back().Mean));
				}
			}
		}
		cout << ret.size() << " image classes have been created\n";
		for (int i = 0; i < ret.size(); ++i)
		{
			ret[i].CalcProp(cnt);
		}

		return ret;
	}

	static vector<ImageClass> ExtractFeatures(const vector<pair<string, vector<string>>> &writers) {
		vector<pair<vector<Matrix<bool>>, string>> winds;
		int cntt = 0;
		for(auto &writer : writers) {
			winds.push_back({ {}, writer.first });
			for (auto &image : writer.second) {				
				Preprocessing::Preprocess(image, winds.back().first);				
				cout << "Exctracting Features from " << image << endl;
			}
			cntt += winds.back().first.size();
		}
		cout << "Training Windows Count: " << cntt << endl;
		return AssignWindows(winds);
	}
};

