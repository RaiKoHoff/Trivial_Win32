/* File main.c
 * (application doing actually nothing but creating a main window and
 *  the custom control as its only child)
 */

#define USE_COMCTL32_LIB 1


#include <tchar.h>
#include <windows.h>

#if defined(_DEBUG) && !defined(NDEBUG)
#include <assert.h>
#endif

#include "resource.h"
#include "custom.h"

#ifdef USE_COMCTL32_LIB
#pragma warning(push)
#pragma warning(disable:4210)
#include <commctrl.h>
#pragma warning(pop)
#pragma comment(lib, "comctl32.lib")
#endif


static const TCHAR* const lpszMWCName = _T("MainWindowClass");

static HINSTANCE s_hInstance;
static HWND s_hwndCustom;

#define CUSTOM_ID     101
#define MARGIN          7


static LRESULT CALLBACK
MainProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
        case WM_SIZE:
            if(wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED) {
                WORD cx = LOWORD(lParam);
                WORD cy = HIWORD(lParam);
                SetWindowPos(s_hwndCustom, NULL, MARGIN, MARGIN,
                             cx-2*MARGIN, cy-2*MARGIN, SWP_NOZORDER);
            }
            break;

        case WM_CREATE:
            s_hwndCustom = CreateWindow(CUSTOM_WC, NULL, WS_CHILD | WS_VISIBLE,
                                  0, 0, 0, 0, hwnd, (HMENU) CUSTOM_ID, s_hInstance, NULL);
            break;

        case WM_CLOSE:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


HWND InitInstance(HINSTANCE hInst, int iCmdShow)
{
    s_hInstance = hInst;

    WNDCLASS wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainProc;
    wc.hInstance = s_hInstance;
    //~wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDR_MAINWNDICO));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = lpszMWCName;
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(lpszMWCName, _T("App Name"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 350, 250, NULL, NULL, s_hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);

    HICON hIcon = NULL;


#ifdef USE_COMCTL32_LIB

    const INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX), ICC_BAR_CLASSES | ICC_STANDARD_CLASSES };
    InitCommonControlsEx(&icc);

    if (FAILED(LoadIconWithScaleDown(s_hInstance, MAKEINTRESOURCE(IDR_MAINWNDICO), 256, 256, &hIcon))) {
        hIcon = (HICON)LoadImage(s_hInstance, MAKEINTRESOURCE(IDR_MAINWNDICO), IMAGE_ICON, 256, 256, LR_DEFAULTCOLOR | LR_SHARED);
    }

#else

    hIcon = LoadIcon(s_hInstance, MAKEINTRESOURCE(IDR_MAINWNDICO));
    //hIcon = (HICON)LoadImage(s_hInstance, MAKEINTRESOURCE(IDR_MAINWNDICO), IMAGE_ICON, 256, 256, LR_DEFAULTCOLOR | LR_SHARED);

#endif


    if (!hIcon) {
        MsgBoxLastError(hwnd, _T("Load Icon failed"));
    }
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

    return hwnd;
}


int APIENTRY
_tWinMain(HINSTANCE hInstance, HINSTANCE hInstPrev, TCHAR* lpszCmdLine, int iCmdShow)
{
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(lpszCmdLine);

    CustomRegister();

    HWND hwnd = InitInstance(hInstance, iCmdShow);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)) {
        if(IsDialogMessage(hwnd, &msg))
            continue;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CustomUnregister();

    return (int)msg.wParam;
}
