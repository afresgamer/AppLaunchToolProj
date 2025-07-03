#include "AppLaunchToolProj.h"
#include "SystemManager.h"
#include "CSVReader.h"

// �O���[�o���ϐ�:
HINSTANCE hInst;                      // ���݂̃C���^�[�t�F�C�X
WCHAR szTitle[MAX_LOADSTRING];        // �^�C�g���o�[�̃e�L�X�g
WCHAR szWindowClass[MAX_LOADSTRING];  // ���C���E�B���h�E �N���X��
vector<CSVData::CSVDataInfo> csvInfoList;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

SystemManager::SystemManager(HINSTANCE hInstance, vector<CSVData::CSVDataInfo> dataList)
{
    hInst = hInstance;
    csvInfoList = dataList;
}

bool SystemManager::Init(HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // �O���[�o�������������������
    LoadStringW(hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInst, IDC_APPLAUNCHTOOLPROJ, szWindowClass, MAX_LOADSTRING);
    RegisterApp();

    // �A�v���P�[�V�����������̎��s:
    if (!InitInstance(nCmdShow)) return FALSE;

    return TRUE;
}

int SystemManager::Update()
{
    HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_APPLAUNCHTOOLPROJ));
    MSG msg;

    // ���C�� ���b�Z�[�W ���[�v:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM SystemManager::RegisterApp()
{
    WNDCLASSEXW wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    
    wcex.style =          CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc =    WndProc;
    wcex.cbClsExtra =     0;
    wcex.cbWndExtra =     0;
    wcex.hInstance =      hInst;
    wcex.hIcon =          LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLAUNCHTOOLPROJ));
    wcex.hCursor =        LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground =  (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName =   MAKEINTRESOURCEW(IDI_APPLAUNCHTOOLPROJ);
    wcex.lpszClassName =  szWindowClass;
    wcex.hIconSm =        LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    
    return RegisterClassExW(&wcex);
}

BOOL SystemManager::InitInstance(int nCmdShow)
{
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst, nullptr);
    if (!hWnd) return FALSE;

    for (int i = 0; i < csvInfoList.size(); i++)
    {
        int btnId = (ID_BUTTON * (i + 1));
        HWND hWndButton = CreateWindowEx(
            0, TEXT("BUTTON"), TEXT("�{�^��"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            200 * (i + 1), 300, // X-coordinate, Y-coordinate 
            100, 100, // Width, Height
            hWnd, (HMENU)btnId, hInst, NULL
        );
        if (!hWndButton) return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        //�{�^�������ʏ���
        for (int i = 0; i < csvInfoList.size(); i++)
        {
            int btnId = (ID_BUTTON * (i + 1));
            if (wmId == btnId)
            {
                if (!csvInfoList[i].filePath.empty()) 
                    SystemManager::SetProcess(hWnd, csvInfoList[i].filePath.c_str(), csvInfoList[i].appName.c_str());
            }
        }

        // �I�����ꂽ���j���[�̉��:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

BOOL SystemManager::SetProcess(HWND hWnd, const wchar_t* commandLine, const wchar_t* appName)
{
    STARTUPINFO si{};
    PROCESS_INFORMATION pi{};

    si.cb = sizeof(si);

    // �R�}���h���C���̎��s(���������0�ȊO���߂�)
    if (CreateProcess(nullptr, (LPWSTR)commandLine, nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi))
    {
        // �N������
        MessageBox(hWnd, TEXT("�N������"), (LPCWSTR)appName, MB_OK);

        // �A�v���P�[�V�����̏I���܂ő҂�
        // (�҂����ɏ������ɐi�߂�ꍇ�AWaitForSingleObject��GetExitCodeProcess�͕s�v)
        WaitForSingleObject(pi.hProcess, INFINITE);

        // �A�v���P�[�V�����̏I���R�[�h�̎擾
        // (�I���R�[�h���g�p���Ȃ��ꍇ�͕s�v)
        unsigned long exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);

        // �I���R�[�h�����̒l�ɂȂ�ꍇ������̂ŁAsigned�ɃL���X�g����
        long ec = static_cast<long>(exitCode);

        // �n���h�������
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        // �N�����s
        MessageBox(hWnd, TEXT("�N�����s"), (LPCWSTR)appName, MB_OK);
        return FALSE;
    }

    return TRUE;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
