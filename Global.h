#pragma once


extern UINT WM_TASKBARCREATED;
extern UINT WM_TRAY_NOTIFY;

BOOL Tray_NotifyDelete(HWND hWnd, UINT uID);
BOOL Tray_NotifyAdd(HWND hWnd, UINT uID, HICON hIcon, LPCTSTR lpszTip);
BOOL Tray_NotifyModify(HWND hWnd, UINT uID, HICON hIcon, LPCTSTR lpszTip);


BOOL IsFile(LPCTSTR szPathName);
BOOL GetFileCRC(LPCTSTR szFileName, DWORD &dwResult,int nStartPos=0);
