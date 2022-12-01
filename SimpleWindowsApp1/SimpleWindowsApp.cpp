#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK MainWindowProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    PAINTSTRUCT paintStruct;
    HDC hdc = BeginPaint(hWnd, &paintStruct);
    if (hdc)
    {
        EndPaint(hWnd, &paintStruct);
    }

    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(1);
        break;

    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    
}

int APIENTRY _tWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR lpCmdLine,
    _In_ int nShowCmd
)
{

    WNDCLASS wc;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = TEXT("MainWindowClass");


    if (!RegisterClass(&wc))
    {
        return GetLastError();
    }

    HWND hWndMain = CreateWindowEx(
        0,
        TEXT("MainWindowClass"),
        TEXT("Simple Windows Application"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0,
        CW_USEDEFAULT, 0,
        nullptr, nullptr,
        hInstance, 0
    );
    if (!hWndMain)
    {
        return GetLastError();
    }

    ShowWindow(hWndMain, nShowCmd);
    UpdateWindow(hWndMain);

    MSG msg;
    BOOL bRet;

    while ((bRet = GetMessage(&msg, nullptr, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            // handle the error and possibly exit
        }
        else
        {
            switch (msg.message)
            {
            case WM_KEYDOWN:
                if (VK_ESCAPE == msg.wParam)
                {
                    //Popup message to warn exiting
                    MessageBox(hWndMain, L"You've hit the ESCAPE key, exiting now... :)", L"Keyboard Message", MB_OK);
                }
                return 0;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }



    return 0;
}