#include <windows.h>
#include <tchar.h>

void OnPaint(HWND hWnd)
{
    RECT rect{};
    HBRUSH hbrush{}, hbrush2{};
    PAINTSTRUCT paintStruct{};
    HPEN pen{};
    HFONT hfont{};

    COLORREF color = RGB(255, 51, 0);   //red-ish
    COLORREF color2 = RGB(51, 153, 255);    //blue-ish
    COLORREF color3 = RGB(255, 255, 255);   //white
    int width{ 3 };
    HDC hdc = BeginPaint(hWnd, &paintStruct);

    if (hdc)
    {
        if (GetClientRect(hWnd, &rect))
        {
            //Lab: Handling WM_PAINT Message
            //Part 2:
            rect.left = rect.left + 10;
            rect.right = rect.right - 10;
            rect.top = rect.top + 10;
            rect.bottom = rect.bottom - 10;

            hbrush = CreateSolidBrush(color);
            if (hbrush)
            {
                FillRect(hdc, &rect, hbrush);

                //Part 3:
                hbrush2 = CreateSolidBrush(color2);
                if (hbrush2)
                {
                    pen = CreatePen(PS_SOLID, width, color2);

                    if (pen)
                    {
                        //Adding new pen and brush to context
                        SelectObject(hdc, pen);
                        SelectObject(hdc, hbrush2);

                        //Draw ellipse within rectangle
                        Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

                        //Reset brush back to original in context
                        SelectObject(hdc, hbrush);
                    }

                    DeleteObject(hbrush2);
                }

                //Part 4:
                //TODO: SelectObject to save original fonts
                HGDIOBJ originfont{};
                originfont = SelectObject(hdc, GetStockObject(DC_PEN));

                //Create arial font with height of 48
                hfont = CreateFont(48, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                    CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));

                //Adding font to context
                SelectObject(hdc, hfont);

                //Adding text and background color, then draw it
                SetTextColor(hdc, color3);
                SetBkColor(hdc, color2);
                DrawText(hdc, TEXT("Hello Fonts!"), 12, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

                //Restore original font
                SelectObject(hdc, originfont);

                DeleteObject(hfont);
                DeleteObject(hbrush);
            }
        }
        else
        {
            MessageBox(hWnd, L"Error calling GetClientRect, exiting now... :)", L"Error", MB_OK);
            return;
        }

        ReleaseDC(hWnd, hdc);
        EndPaint(hWnd, &paintStruct);
    }



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

    case WM_PAINT:
        OnPaint(hWnd);
        break;

    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

}

int APIENTRY _tWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine,
    int nShowCmd
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