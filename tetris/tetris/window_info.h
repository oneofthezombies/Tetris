#pragma once

#include <Windows.h>

struct Window
{
    static const int kOriginX = 100;
    static const int kOriginY = 100;
    static const int kWidth = 480;
    static const int kHeight = 670;
    static const int kStyle = WS_OVERLAPPEDWINDOW;
};

struct Timer
{
private:
    enum class Id
    {
        kRender
    };

    enum class Fps
    {
        k60 = 17,
        k30 = 33
    };

public:
    static const Id kId = Id::kRender;
    static const Fps kFps = Fps::k60;
};

struct WindowResizer
{
    void operator()(HWND const& hWnd)
    {
        RECT rc { 0, 0, Window::kWidth, Window::kHeight };
        AdjustWindowRect(&rc, Window::kStyle, true);
        SetWindowPos(hWnd, NULL,
                     Window::kOriginX, Window::kOriginY,
                     Window::kWidth, Window::kHeight,
                     SWP_NOZORDER | SWP_NOMOVE);
    }
};

/*
HWND hWnd;
unique_ptr<ootz::GameMain, ootz::GameMainDeleter> gameMain;

void    CALLBACK    RenderProc(HWND, UINT, UINT, DWORD);

gameMain = GameMainBuilder {}();
if (FAILED(gameMain->init()))
return FALSE;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
hInst = hInstance; // Store instance handle in our global variable

hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

if (!hWnd)
return FALSE;

WindowResizer {}(hWnd);
ShowWindow(hWnd, nCmdShow);
UpdateWindow(hWnd);

return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
switch (message)
{
case WM_CREATE:
{
SetTimer(hWnd,
static_cast<UINT_PTR>(Timer::kId),
static_cast<UINT>(Timer::kFps),
static_cast<TIMERPROC>(RenderProc));
}
break;
case WM_COMMAND:
{
int wmId = LOWORD(wParam);
// Parse the menu selections:
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
{
KillTimer(hWnd, static_cast<UINT_PTR>(Timer::kId));
PostQuitMessage(0);
}
break;
default:
return DefWindowProc(hWnd, message, wParam, lParam);
}
return 0;
}

void CALLBACK RenderProc(HWND hWnd, UINT message, UINT idEvent, DWORD time)
{
gameMain->renderProc(hWnd, message, idEvent, time);
}

*/
