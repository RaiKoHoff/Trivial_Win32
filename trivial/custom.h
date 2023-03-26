/* File custom.h
 * (custom control interface)
 */

#ifndef CUSTOM_H
#define CUSTOM_H

#include <tchar.h>
#include <windows.h>


/* Window class */
#define CUSTOM_WC   _T("CustomControl")

/* Register/unregister the window class */
void CustomRegister(void);
void CustomUnregister(void);

DWORD MsgBoxLastError(HWND hwnd, LPCTSTR lpszMessage);


#endif  /* CUSTOM_H */
