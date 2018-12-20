#pragma once
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
class PerformanceAnalysis
{
public:

	static void AnalyzePerformance(const string &output, const string &correct, const string &results) {
		cout << "Analyzing Performance..." << endl;
		ifstream out(output);
		ifstream cor(correct);
		ofstream res(results);
		string str1, str2;
		double all = 0, corect = 0;
		while (getline(cor, str1)) {
			all++;
			if (getline(out, str2)) {
				if (str1 == str2) corect++;
				cout << "correct: " << str1 << "   predicted: " << str2 << endl;
				res << "correct: " << str1 << "   predicted: " << str2 << endl;
			}
		}
		cout << endl;
		res << endl;
		cout << "correctly classified " << corect << " / " << all << endl;
		cout << "Accuracy = " << 100.0 * corect / all << "%" << endl;
		res << "correctly classified " << corect << " / " << all << endl;
		res << "Accuracy = " << 100.0 * corect / all << "%" << endl;
	}



};

