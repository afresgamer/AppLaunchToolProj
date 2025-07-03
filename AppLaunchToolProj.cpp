// AppLaunchToolProj.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "AppLaunchToolProj.h"
#include "SystemManager.h"
#include "CSVReader.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    CSVReader* csvReader = new CSVReader("Test.csv", ios::in);
    auto csvDataList = csvReader->Read();

    SystemManager* sm = new SystemManager(hInstance, csvDataList);
    if (!sm->Init(hPrevInstance, lpCmdLine, nCmdShow))
    {
        std::vector<CSVData::CSVDataInfo>().swap(csvDataList);
        delete sm;
        delete csvReader;
        return FALSE;
    }

    return sm->Update();
}

