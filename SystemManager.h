#include "framework.h"
#include "AppLaunchToolProj.h"
#include "CSVReader.h"

#define MAX_LOADSTRING 100
#define ID_BUTTON 100   //’Ç‰Á

class SystemManager
{
public:
	SystemManager(HINSTANCE hInstance, vector<CSVData::CSVDataInfo> dataList);

	bool Init(HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
	int Update();
	ATOM RegisterApp();
	BOOL InitInstance(int);
	static BOOL SetProcess(HWND hWnd, const wchar_t* commandLine, const wchar_t* appName);
};
