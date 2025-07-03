#include "CSVReader.h"

CSVReader::CSVReader(const string& fileName, ios::openmode mode)
{
    // バッファサイズを指定して効率化
    constexpr streamsize BUFFER_SIZE = 1024 * 1024; // 1MB
    char* buffer = new char[BUFFER_SIZE];
    file.rdbuf()->pubsetbuf(buffer, BUFFER_SIZE);
    
    // ファイルを開く
    file.open(fileName, mode);
    if (!file.is_open()) 
    {
        delete[] buffer;
        throw std::runtime_error("ファイルを開けません: " + fileName);
    }
}

CSVReader::~CSVReader()
{
	if (file.is_open())
	{
		file.close();
	}
}

vector<wstring> CSVReader::Split(const string& input, char delimiter)
{
    istringstream stream(input);
    string field;
    vector<wstring> result;

    while (getline(stream, field, delimiter)) {
        auto data = StringToWString(field);
        result.push_back(data);
    }

    return result;
}

/*
    stringをwstringへ変換する
*/
wstring CSVReader::StringToWString(string oString)
{
    // SJIS → wstring
    int iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, (wchar_t*)NULL, 0);

    // バッファの取得
    wchar_t* cpUCS2 = new wchar_t[iBufferSize];

    // SJIS → wstring
    MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, cpUCS2, iBufferSize);

    // stringの生成
    std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

    // バッファの破棄
    delete[] cpUCS2;

    // 変換結果を返す
    return(oRet);
}

vector<CSVData::CSVDataInfo> CSVReader::Read()
{
    CSVData csvData;
    string line;
    const unsigned int first = 0;
    const unsigned int second = 1;
    const unsigned int third = 2;

    unsigned int num = 0;
    while (getline(file, line)) 
    {
        if (num == first) {
            num++;
            continue;
        }
        vector<wstring> strvec = Split(line, ',');
        
        CSVData::CSVDataInfo data {};
        for (int i = 0; i < strvec.size();i++) 
        {
            wprintf(L"%s", strvec.at(i).c_str());
            
            if (i == first)
            {
                int no = stoi(strvec.at(i));
                data.no = no;
            }
            else if (i == second)
            {
                auto& contentName = strvec.at(i);
                data.appName = contentName;
            }
            else if (i == third)
            {
                auto& path = strvec.at(i);
                data.filePath = path;
            }
        }

        csvData.csvInfoVec.push_back(data);
    }

    return csvData.csvInfoVec;
}
