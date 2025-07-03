#pragma once
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class CSVData
{
private:
public:
	struct CSVDataInfo
	{
		int no;
		wstring appName;
		wstring filePath;
	};
	vector<CSVDataInfo> csvInfoVec;
};

