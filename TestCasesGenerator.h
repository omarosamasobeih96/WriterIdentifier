#pragma once
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <set>
using namespace std;

class TestCasesGenerator
{
	inline static bool exists_test0(const std::string& name) {
		ifstream f(name.c_str());
		return f.good();
	}

	static vector < pair<string, vector<string> > > GetDataSet(int imagesCnt) {
		ifstream forms(formsPath);
		string str;
		map < string, vector<string>> writers;
		vector < pair<string, vector<string> > > ret;
		while (getline(forms, str)) {
			if (str.size() == 0) continue;
			if (str[0] == '#') continue;
			stringstream ss(str);
			string imageId, writerId;
			ss >> imageId >> writerId;
			if (!exists_test0(datasetPath + imageId + ".png")) continue;
			writers[writerId].push_back(imageId);
		}
		set<pair<int, string>> s;
		for (auto &it : writers) {
			if (it.second.size() >= imagesCnt)
				ret.push_back(it);
		}
		return ret;
	}

public:
	
	static void GenerateTestCases(const string &corretPath, const string &testcasesPath, int casesCnt = 20, int writersCnt = 3, int trainingCnt = 2) {
		cout << "Reading Dataset..." << endl;
		auto writers = GetDataSet(trainingCnt + 5);
		ofstream tc(testcasesPath);
		ofstream cor(corretPath);
		cout << "Generating Testcases..." << endl << endl;
		while (casesCnt--) {
			vector<int> v(writersCnt);
			for (int i = 0; i < writersCnt; ++i)
				v[i] = rand() % writers.size();
			int x = v[rand() % writersCnt];
			int y = rand() % writers[x].second.size();
			tc << writers[x].second[y] << endl;
			cor << writers[x].first << endl;
			for (int i = 0; i < writersCnt; ++i) {
				tc << writers[v[i]].first << " ";
				map<int, int>visited;
				if (x == v[i]) visited[y] = 1;
				for (int j = 0; j < trainingCnt; ++j) {
					int x = rand() % writers[v[i]].second.size();
					while (visited[x]) x = rand() % writers[v[i]].second.size();
					visited[x] = 1;
					tc << writers[v[i]].second[x] << " ";
				}
				tc << endl;
			}
		}
	}
};

