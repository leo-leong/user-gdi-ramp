#include <windows.h>
#include <tchar.h>

UINT colorindex{ COLOR_WINDOW };
const UINT_PTR timer1id{ 100 };
const UINT_PTR timer2id{ 200 };
const UINT_PTR timer3id{ 300 };

void OnPaint(HWND hWnd)
{
    RECT rect{};
    HBRUSH hbrush{};
    PAINTSTRUCT paintStruct{};

    COLORREF color = colorindex;
    HDC hdc = BeginPaint(hWnd, &paintStruct);
    if (hdc)
    {
        if (GetClientRect(hWnd, &rect))
        {
            //TODO: not sure why this doesn't work
            //hbrush = CreateSolidBrush(color);
            //FillRect(hdc, &rect, hbrush);
            //DeleteObject(hbrush);
            
            FillRect(hdc, &rect, (HBRUSH) colorindex);
        }
        EndPaint(hWnd, &paintStruct);
    }
}

void OnTimer(HWND hWnd, UINT uMsg, UINT_PTR nID, DWORD dwTime)
{
    RECT rect{};

    if (100 == nID)
    {
        ++colorindex;

        //Part 1 & 2: invalidates region to trigger paint
        //NULL invalidates the entire client area
        //InvalidateRect(hWnd, NULL, true);

        //Part 3: draw immediately
        HDC hdc = GetDC(hWnd);
        if (GetClientRect(hWnd, &rect))
        {
            FillRect(hdc, &rect, (HBRUSH)colorindex);
        }
        ReleaseDC(hWnd, hdc);
    }
    else
    {
        colorindex = COLOR_HOTLIGHT;
        InvalidateRect(hWnd, NULL, true);
        KillTimer(hWnd, timer1id);
        KillTimer(hWnd, timer2id);
    }
}

void OnCreate(HWND hWnd)
{
    ////Part 1: 2sec and 30sec timers
    //SetTimer(hWnd, timer1id, 2000, (TIMERPROC)NULL);
    //SetTimer(hWnd, timer2id, 15000, (TIMERPROC)NULL);

    ////Part 2: 2sec and 30sec timers
    //SetTimer(hWnd, timer1id, 2000, (TIMERPROC)OnTimer);
    //SetTimer(hWnd, timer2id, 30000, (TIMERPROC)OnTimer);

    //Part 3: 1sec timer
    SetTimer(nullptr, timer3id, 1000, (TIMERPROC)OnTimer);
}

LRESULT CALLBACK MainWindowProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(1);
        break;

    ////Part 2:
    //case WM_PAINT:
    //    OnPaint(hWnd);
    //    break;

    case WM_CREATE:
        OnCreate(hWnd);
        break;

    ////Part 1:
    //case WM_TIMER:
    //    if (100 == wParam)
    //    {
    //        ++colorindex;

    //        //NULL invalidates the entire client area
    //        InvalidateRect(hWnd, NULL, true);
    //    }
    //    else
    //    {
    //        colorindex = COLOR_HOTLIGHT;
    //        InvalidateRect(hWnd, NULL, true);
    //        KillTimer(hWnd, timer1id);
    //        KillTimer(hWnd, timer2id);
    //    }
    //    break;

    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
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