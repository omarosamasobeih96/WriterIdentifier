#pragma once
#include "Classification.h"

class Testing
{
public:
	static void RunTestCases(const string &testsPath, const string &outputPath, int writersCnt = 3, int trainingCnt = 2) {
		ifstream test(testsPath);
		ofstream out_(outputPath);
		string imageToTest;
		int testCase = 0;
		while (test >> imageToTest) {
			cout << "Running Test Case " << ++testCase << endl;
			string imageToTest_ = datasetPath + imageToTest + ".png";
			vector<pair<string, vector<string>>> writers;
			for (int i = 0; i < writersCnt; ++i) {
				string writer;
				test >> writer;
				writers.push_back({ writer, {} });
				for (int j = 0; j < trainingCnt; ++j) {
					string imageId;
					test >> imageId;
					imageId = datasetPath + imageId + ".png";
					writers.back().second.push_back(imageId);
				}
			}
			string writer = Classification::Classify(writers, imageToTest_);
			out_ << writer << endl;
			cout << endl << endl << endl;
		}
	}


};

