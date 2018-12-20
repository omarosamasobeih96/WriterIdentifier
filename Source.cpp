#define _CRT_SECURE_NO_WARNINGS
#include "Testing.h"
#include "TestCasesGenerator.h"
#include "PerformanceAnalysis.h"
#include "Constants.h"
int main() {
	TestCasesGenerator::GenerateTestCases(correctPath, testcasesPath);
	Testing::RunTestCases(testcasesPath, outputPath);
	PerformanceAnalysis::AnalyzePerformance(outputPath, correctPath, resultsPath);
}

