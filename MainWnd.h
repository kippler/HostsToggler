////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// 메인 윈도우
/// 
/// @author   parkkh
/// @date     Friday, November 19, 2010  4:15:21 PM
/// 
/// Copyright(C) 2010 Bandisoft, All rights reserved.
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "resource.h"
#include "Global.h"

class CMainWnd  : public CWindowImpl<CMainWnd, CWindow>
{
public:
	CMainWnd();
	virtual ~CMainWnd();

	BOOL			Create(WCHAR* argv[], int argc);
	BOOL			PreTranslateMessage(MSG* pMsg);

private :
	void			SetTrayIcon();
	void			ModifyTrayIcon();


private :
	int				m_state;
	HICON			m_hIcon[4];
	CString			m_folder;
	CString			m_hostsTarget;
	CString			m_hosts[4];



public :
	BEGIN_MSG_MAP(CMainWnd)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_TRAY_NOTIFY, OnTrayNotify);
		MESSAGE_HANDLER(WM_TASKBARCREATED, OnTaskbarCreated);
		
		COMMAND_ID_HANDLER(IDM_EXIT, OnExit)
			COMMAND_ID_HANDLER(IDM_HOSTS_0, OnHosts)
			COMMAND_ID_HANDLER(IDM_HOSTS_1, OnHosts)
			COMMAND_ID_HANDLER(IDM_HOSTS_2, OnHosts)
			COMMAND_ID_HANDLER(IDM_HOSTS_3, OnHosts)
			COMMAND_ID_HANDLER(IDM_NOTEPAD_HOSTS, OnNotepadHosts)
			COMMAND_ID_HANDLER(IDM_NOTEPAD_HOSTS_0, OnNotepadHosts)
			COMMAND_ID_HANDLER(IDM_NOTEPAD_HOSTS_1, OnNotepadHosts)
			COMMAND_ID_HANDLER(IDM_NOTEPAD_HOSTS_2, OnNotepadHosts)
			COMMAND_ID_HANDLER(IDM_NOTEPAD_HOSTS_3, OnNotepadHosts)
			COMMAND_ID_HANDLER(IDM_OPEN_FOLDER, OnOpenFolder)
			COMMAND_ID_HANDLER(IDM_ABOUT, OnAbout)
	END_MSG_MAP()

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTrayNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTaskbarCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	

	LRESULT OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnHosts(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNotepadHosts(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOpenFolder(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


