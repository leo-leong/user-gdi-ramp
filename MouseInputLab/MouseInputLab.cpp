#include <windows.h>
#include <tchar.h>

static POINTS ptsBegin;
static POINTS ptsEnd;

//Part 1: draw line when mouse down
void DrawLine(HWND hWnd, LPARAM lParam, int width, COLORREF color)
{
    HDC hdc;
    HPEN pen{};
    HPEN penColorNew{};

    hdc = GetDC(hWnd);

    penColorNew = CreatePen(PS_SOLID, width, color);
    pen = (HPEN)GetCurrentObject(hdc, OBJ_PEN);

    SelectObject(hdc, penColorNew);

    ptsEnd = MAKEPOINTS(lParam);
    MoveToEx(hdc, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
    LineTo(hdc, ptsEnd.x, ptsEnd.y);

    SelectObject(hdc, pen);
    ptsBegin = ptsEnd;

    ReleaseDC(hWnd, hdc);
}

//Part 1: draw line when mouse down
void HandleMouse(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    RECT rcClient;
    COLORREF colorRed = RGB(255, 51, 0);   //red-ish
    COLORREF colorBlue = RGB(0, 0, 255);   //red-ish

    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        GetClientRect(hWnd, &rcClient);
        ClipCursor(&rcClient);
        ptsBegin = MAKEPOINTS(lParam);

        //Part 2: capture mouse input
        SetCapture(hWnd);
        return;

    case WM_RBUTTONDOWN:
        GetClientRect(hWnd, &rcClient);
        ClipCursor(&rcClient);
        ptsBegin = MAKEPOINTS(lParam);

        //Part 2: capture mouse input
        SetCapture(hWnd);
        return;

    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON)
        {
            DrawLine(hWnd, lParam, 3, colorRed);
        }
        else if (wParam & MK_RBUTTON)
        {
            DrawLine(hWnd, lParam, 3, colorBlue);
        }
        break;

    case WM_LBUTTONUP:
        ClipCursor(NULL);

        //Part 2: capture mouse input
        ReleaseCapture();
        break;


    case WM_RBUTTONUP:
        ClipCursor(NULL);

        //Part 2: capture mouse input
        ReleaseCapture();
        break;

    }

}

LRESULT CALLBACK MainWindowProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    //Part 1: draw line when mouse down
    if (uMsg <= WM_MOUSELAST && uMsg >= WM_MOUSEFIRST)
    {
        HandleMouse(hWnd, uMsg, wParam, lParam);
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