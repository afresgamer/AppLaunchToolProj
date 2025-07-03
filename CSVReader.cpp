#include "CSVReader.h"

CSVReader::CSVReader(const string& fileName, ios::openmode mode)
{
    // �o�b�t�@�T�C�Y���w�肵�Č�����
    constexpr streamsize BUFFER_SIZE = 1024 * 1024; // 1MB
    char* buffer = new char[BUFFER_SIZE];
    file.rdbuf()->pubsetbuf(buffer, BUFFER_SIZE);
    
    // �t�@�C�����J��
    file.open(fileName, mode);
    if (!file.is_open()) 
    {
        delete[] buffer;
        throw std::runtime_error("�t�@�C�����J���܂���: " + fileName);
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
    string��wstring�֕ϊ�����
*/
wstring CSVReader::StringToWString(string oString)
{
    // SJIS �� wstring
    int iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, (wchar_t*)NULL, 0);

    // �o�b�t�@�̎擾
    wchar_t* cpUCS2 = new wchar_t[iBufferSize];

    // SJIS �� wstring
    MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, cpUCS2, iBufferSize);

    // string�̐���
    std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

    // �o�b�t�@�̔j��
    delete[] cpUCS2;

    // �ϊ����ʂ�Ԃ�
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
