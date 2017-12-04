#include "stdafx.h"
#include "MainWnd.h"
#include "resource.h"

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()
CComModule _Module;

BOOL InitInstance(HINSTANCE hInst);
BOOL ExitInstance();


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	MSG		msg;
	HACCEL	hAccelTable;

	if (!InitInstance (hInstance)) 
	{
		return FALSE;
	}

	CMainWnd main;
	
	if(main.Create(__wargv, __argc)==FALSE) return 1;

	hAccelTable = LoadAccelerators(_Module.m_hInstResource, (LPCTSTR)IDC_ACCEL);

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if(main.PreTranslateMessage(&msg)==FALSE)
		{
			if (!TranslateAccelerator(main.m_hWnd, hAccelTable, &msg)) 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	ExitInstance();

	return 0;
}


BOOL InitInstance(HINSTANCE hInstance)
{
	_Module.Init(ObjectMap, hInstance);

	::CoInitialize(NULL);

	return TRUE;
}

BOOL ExitInstance()
{
	::CoUninitialize();

	_Module.Term();

	return TRUE;
}



