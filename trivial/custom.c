/* File custom.c
 * (custom control implementation)
 */

#include "custom.h"


static void
CustomPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect;

    GetClientRect(hwnd, &rect);

    hdc = BeginPaint(hwnd, &ps);
    SetTextColor(hdc, RGB(0,0,0));
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, _T("Hello World!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    EndPaint(hwnd, &ps);
}


static LRESULT CALLBACK
CustomProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
        case WM_PAINT:
            CustomPaint(hwnd);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void
CustomRegister(void)
{
    WNDCLASS wc = { 0 };

    wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = CustomProc;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = CUSTOM_WC;
    RegisterClass(&wc);
}

void
CustomUnregister(void)
{
    UnregisterClass(CUSTOM_WC, NULL);
}


// //////////////////////////////////////////////////////////////////
// 
// Retrieve and show the system error message for the last-error code
//
DWORD MsgBoxLastError(HWND hwnd, LPCTSTR lpszMessage)
{
    const DWORD dwErrID = GetLastError();

    LPVOID lpszMsgBuf = NULL;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwErrID,
        0,
        (LPTSTR)&lpszMsgBuf, // LocalAlloc()
        0, NULL);

    const TCHAR* const lpszMsgFormat = _T("%s - GetLastError(%i):'%s'!");

    if (lpszMsgBuf) {
        // Display the error message and exit the process
        size_t const len = _tcslen(lpszMsgFormat) + _tcslen((LPCTSTR)lpszMsgBuf) + _tcslen(lpszMessage) + 64;
        LPTSTR lpszDisplayBuf = (LPTSTR)LocalAlloc(LPTR, (len * sizeof(TCHAR)));
        if (lpszDisplayBuf) {
            _stprintf_s(lpszDisplayBuf, len, lpszMsgFormat, lpszMessage, dwErrID, (LPCTSTR)lpszMsgBuf);
            MessageBoxExW(hwnd, lpszDisplayBuf, _T("ERROR"), MB_ICONERROR | MB_TOPMOST, 0);
            LocalFree(lpszDisplayBuf);
        }
        LocalFree(lpszMsgBuf);
        lpszMsgBuf = NULL;
    }
    return dwErrID;
}

