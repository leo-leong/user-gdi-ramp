#include <windows.h>
#include <tchar.h>
#include <CommCtrl.h>

//Part 1: print keystroke to debug output
void HandleKeystrokes(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    PCTSTR pszMessage = NULL;
    switch (uMsg)
    {
    case WM_SYSKEYDOWN:
        pszMessage = TEXT("WM_SYSKEYDOWN");
        break;
    case WM_SYSKEYUP:
        pszMessage = TEXT("WM_SYSKEYUP");
        break;
    case WM_KEYDOWN:
        pszMessage = TEXT("WM_KEYDOWN");
        break;
    case WM_KEYUP:
        pszMessage = TEXT("WM_KEYUP");
        break;
    default:
        return;
    }

    WORD wVk = (WORD)wParam;
    WORD wRepeatCount = LOWORD(lParam);
    WORD wScanCode = (WORD)((lParam >> 16) & 0xFF);
    BOOL fExtendedKey = KF_EXTENDED == (HIWORD(lParam) & KF_EXTENDED);
    BOOL fAltDown = KF_ALTDOWN == (HIWORD(lParam) & KF_ALTDOWN);
    BOOL fRepeat = KF_REPEAT == (HIWORD(lParam) & KF_REPEAT);
    BOOL fKeyUp = KF_UP == (HIWORD(lParam) & KF_UP);

    TCHAR szBuffer[100];
    int result = _stprintf_s(szBuffer, TEXT("%-13s VK:%-3d SC: %-3d RepeatCount:%-3d Extended:%d AltDown:%d Repeat:%d KeyUp:%d\r\n"),
        pszMessage,
        wVk,
        wScanCode,
        wRepeatCount,
        fExtendedKey,
        fAltDown,
        fRepeat,
        fKeyUp
    );
    if (-1 != result)
    {
        OutputDebugString(szBuffer);
    }
}

void HandleCharacters(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    RECT rect{};
    HFONT hfont{};
    HBRUSH hbrush{};
    HGDIOBJ originfont{};
    COLORREF colorbackground = RGB(255, 255, 255);    //white
    COLORREF colortext = RGB(0, 0, 0);   //black

    HDC hdc = GetWindowDC(hWnd);
    if (hdc)
    {
        if (GetClientRect(hWnd, &rect))
        {
            // Brush to wipe out existing content and reset the canvas
            hbrush = CreateSolidBrush(colorbackground);
            if (hbrush)
            {
                FillRect(hdc, &rect, hbrush);
                originfont = SelectObject(hdc, GetStockObject(DC_PEN));

                //Part 2: draw keystroke within window
                //Create arial font with height of 48
                hfont = CreateFont(48, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                    CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));

                //Adding font to context
                SelectObject(hdc, hfont);

                //Part 3: set text color to red if SHIFT key is down
                bool bShiftKeyDown = ((unsigned short) GetKeyState(VK_SHIFT)) >> 15;
                if (bShiftKeyDown) { colortext = RGB(255, 0, 0); }

                //Adding text and background color, then draw it
                SetTextColor(hdc, colortext);
                SetBkColor(hdc, colorbackground);
                TCHAR text[] = { wParam };
                int result = DrawText(hdc, text, 1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                
                ////Part 1: print keystroke to debug output
                //TCHAR szBuffer[100];
                //result = _stprintf_s(szBuffer, TEXT("Character: %c\r\n"), wParam);
                //if (-1 != result)
                //{
                //    OutputDebugString(szBuffer);
                //}

                //Restore original font
                SelectObject(hdc, originfont);

                DeleteObject(hfont);
            }
        }
        else
        {
            MessageBox(hWnd, L"Error calling GetClientRect, exiting now... :)", L"Error", MB_OK);
            return;
        }

        ReleaseDC(hWnd, hdc);
    }

}

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
        //Part 4.1: send window message to set hot key
        SendMessage(hWnd, WM_SETHOTKEY, NULL, 0);

        PostQuitMessage(1);
        break;

    case WM_SYSKEYDOWN:
        HandleKeystrokes(hWnd, uMsg, wParam, lParam);
        break;
    case WM_SYSKEYUP:
        HandleKeystrokes(hWnd, uMsg, wParam, lParam);
        break;
    case WM_KEYDOWN:
        HandleKeystrokes(hWnd, uMsg, wParam, lParam);
        break;
    case WM_KEYUP:
        HandleKeystrokes(hWnd, uMsg, wParam, lParam);
        break;

    //Lab: Handling Character Messages
    case WM_CHAR:
        HandleCharacters(hWnd, uMsg, wParam, lParam);

    //Part 4.1: send window message to set hot key
    case WM_CREATE:
        SendMessage(hWnd, WM_SETHOTKEY, MAKEWORD('A', HOTKEYF_CONTROL | HOTKEYF_SHIFT), 0);

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
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }



    return 0;
}