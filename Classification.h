#pragma once

#include "Preprocessing.h"
#include "FeatureExtraction.h"
#include "time.h"
#include "Constants.h"
using namespace std;

typedef bitset<area> BS;
typedef pair<BS, BS> MatE;


class Classification
{
	static string FindWriter(const vector<ImageClass> & trainingClasses, const vector<Matrix<bool>> & testingWindows, string name = "") {
		time_t curTime; time(&curTime);
		string cons = "trials/console" + to_string(curTime) + ".txt";
		ofstream console_(cons);
		string writer = "";
		map<string, double> writers;
		int cnt = 0;
		vector<MatE> cor;
		for (int j = 0; j < trainingClasses.size(); ++j)
			cor.push_back(FeatureExtraction::GetBitset(trainingClasses[j].Mean));
		for (int i = 0; i < testingWindows.size(); ++i) {
			int idx = -1;
			double mx = -1e9;
			MatE equi = FeatureExtraction::GetBitset(testingWindows[i]);
			for (int j = 0; j < trainingClasses.size(); ++j) {
				double tmp = FeatureExtraction::CalculateSimilarity(equi, cor[j]);

				if (tmp < similarity_threshold2) continue;
				if (tmp > mx) {
					mx = tmp;
					idx = j;
				}
			}
			if (!~idx) {
				cnt++;
				continue;
			}
			for (auto &it : trainingClasses[idx].writersProp) {
				writers[it.first] += it.second * mx;
			}
			if (i % 511 == 510) cout << 100.0 * i / testingWindows.size() << "%\n";
		}
		cout << "Windows Successfully Classified: " << 100.0 - 100.0 * cnt / testingWindows.size() << "%" << endl;
		console_ << "Windows Successfully Classified: " << 100.0 - 100.0 * cnt / testingWindows.size() << "%" << endl;
		double mx = -1e9;
		vector<pair<double, string>>vec;
		double sum = 0;
		for (auto &it : writers) it.second *= it.second;
		for (auto &it : writers) {
			sum += it.second;
			if (it.second > mx) {
				mx = it.second;
				writer = it.first;
			}
			vec.push_back({ it.second, it.first });
		}
		sort(vec.rbegin(), vec.rend());
		cout << "For image: " << name << " ";
		console_ << "For image: " << name << " ";
		for (int i = 0; i < min(2, (int)vec.size()); ++i) {
			auto e = vec[i];
			cout << "Confidence in " << e.second << ": " << 100.0 * e.first/ sum << "%       ";
			console_ << "Confidence in " << e.second << ": " << 100.0 * e.first / sum << "%       ";
		}
		cout << endl;
		console_ << endl;
		return writer;
	}

public:
	

	static string Classify(const vector<pair<string, vector<string>>> &writers, const string& image) {
		vector<Matrix<bool>> winds;
		cout << "     -------     Preprocessing     -------" << endl;
		Preprocessing::Preprocess(image, winds);
		cout << "     -------     Extracting Features     -------" << endl;
		vector<ImageClass> trainingClasses = FeatureExtraction::ExtractFeatures(writers);
		cout << "     -------     Classifying Image     -------" << endl;
		string predicted = FindWriter(trainingClasses, winds, image);
		return predicted;
	}

};

