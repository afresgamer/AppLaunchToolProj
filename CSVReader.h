#pragma once
#include "framework.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
#include "CSVData.h"

class CSVReader
{
public:
	CSVReader(const string& fileName, ios::openmode mode);
	~CSVReader();
	vector<wstring> Split(const string& input, char delimiter);
	wstring StringToWString(string oString);
	vector<CSVData::CSVDataInfo> Read();
private:
	ifstream file;
};

