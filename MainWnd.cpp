#include "stdafx.h"
#include "MainWnd.h"
#include "resource.h"
#include "Global.h"

#define	APP_NAME				_T("HostsToggler")
#define APP_CLASS_NAME			_T("HostsTogglerAppClass")
#define APP_TITLE				_T("HostsToggler")


CMainWnd::CMainWnd(void)
{
	m_state = 0;

	m_hIcon[0] = ::LoadIcon(_Module.m_hInst, MAKEINTRESOURCE(IDI_ICON0));
	m_hIcon[1] = ::LoadIcon(_Module.m_hInst, MAKEINTRESOURCE(IDI_ICON1));
	m_hIcon[2] = ::LoadIcon(_Module.m_hInst, MAKEINTRESOURCE(IDI_ICON2));
	m_hIcon[3] = ::LoadIcon(_Module.m_hInst, MAKEINTRESOURCE(IDI_ICON3));


	//C:\Windows\System32\drivers\etc 
	WCHAR buf[MAX_PATH];
	::GetSystemDirectory(buf, MAX_PATH);
	m_folder = CString(buf) + L"\\drivers\\etc\\";


	m_hostsTarget	= m_folder + L"hosts";
	m_hosts[0]		= m_folder + L"hosts.0";
	m_hosts[1]		= m_folder + L"hosts.1";
	m_hosts[2]		= m_folder + L"hosts.2";
	m_hosts[3]		= m_folder + L"hosts.3";

}


CMainWnd::~CMainWnd(void)
{

}


BOOL CMainWnd::Create(WCHAR* argv[], int argc)
{
	// 윈도우 생성
	CWndClassInfo &winInfo = GetWndClassInfo();
	winInfo.m_wc.lpszClassName = APP_CLASS_NAME;
	winInfo.m_wc.style = 0;
//	__super::Create(HWND_MESSAGE, CWindow::rcDefault,  APP_NAME, 0);
	__super::Create(NULL, CRect(0,0,0,0),  APP_NAME, WS_POPUP);

	// hosts 파일 체크
	if(	IsFile(m_hosts[0])==FALSE ||
		IsFile(m_hosts[1])==FALSE ||
		IsFile(m_hosts[2])==FALSE ||
		IsFile(m_hosts[3])==FALSE )

	{
		BOOL ret0 = ::CopyFile(m_hostsTarget, m_hosts[0], TRUE);
		BOOL ret1 = ::CopyFile(m_hostsTarget, m_hosts[1], TRUE);
		BOOL ret2 = ::CopyFile(m_hostsTarget, m_hosts[2], TRUE);
		BOOL ret3 = ::CopyFile(m_hostsTarget, m_hosts[3], TRUE);

		if(	IsFile(m_hosts[0])==FALSE ||
			IsFile(m_hosts[1])==FALSE ||
			IsFile(m_hosts[2])==FALSE ||
			IsFile(m_hosts[3])==FALSE )
		{
			::MessageBox(NULL, L"어라, hosts 파일이 있는 폴더에 접근할 수가 없네요.\r\n프로그램을 종료합니다.", APP_NAME, MB_OK);
			PostMessage(WM_CLOSE);
			return FALSE;
		}

		::MessageBox(NULL, L"프로그램을 처음 실행시키셨군요.\r\n기존 hosts 파일을 복사해서 hosts.0, hosts.1, hosts.2, hosts.3 파일을 만들었습니다.", APP_NAME, MB_OK);
	}


	// 파일 내용 비교
	DWORD crcTarget;
	DWORD crc[4];

	if(	GetFileCRC(m_hostsTarget, crcTarget)==FALSE ||
		GetFileCRC(m_hosts[0], crc[0])==FALSE ||
		GetFileCRC(m_hosts[1], crc[1])==FALSE ||
		GetFileCRC(m_hosts[2], crc[2])==FALSE ||
		GetFileCRC(m_hosts[3], crc[3])==FALSE)
	{
		::MessageBox(NULL, L"어라, hosts 파일에 접근할 수가 없네요.\r\n프로그램을 종료합니다.", APP_NAME, MB_OK);
		PostMessage(WM_CLOSE);
		return FALSE;
	}

	if(crcTarget==crc[0]) m_state = 0;
	else if(crcTarget==crc[1]) m_state = 1;
	else if(crcTarget==crc[2]) m_state = 2;
	else if(crcTarget==crc[3]) m_state = 3;
	else
	{
		::MessageBox(NULL, L"어라, hosts 파일이 수정되었나 보네요?\nhosts 폴더를 열어줄테니 파일의 내용을 직접 확인해 보세요.", APP_NAME, MB_OK);
		PostMessage(WM_COMMAND, IDM_OPEN_FOLDER);
	}


	SetTrayIcon();

	return TRUE;
}


LRESULT CMainWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Tray_NotifyDelete(m_hWnd, 0);
	PostQuitMessage(0);
	return 0;
}

BOOL CMainWnd::PreTranslateMessage(MSG* pMsg)
{
	return FALSE;
}


void CMainWnd::SetTrayIcon()
{
	Tray_NotifyDelete(m_hWnd, 0);
	Tray_NotifyAdd(m_hWnd, 0, m_hIcon[m_state], L"");
}

void CMainWnd::ModifyTrayIcon()
{
	Tray_NotifyModify(m_hWnd, 0, m_hIcon[m_state], L"");
}


LRESULT CMainWnd::OnTrayNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(lParam == WM_RBUTTONUP)
	{
		HMENU menu = ::LoadMenu(_Module.m_hInst, MAKEINTRESOURCE(IDR_MENU_TRAY));
		HMENU menuSub = ::GetSubMenu(menu, 0);


		if(m_state==0) ::CheckMenuItem(menuSub, IDM_HOSTS_0, MF_BYCOMMAND | MF_CHECKED);
		if(m_state==1) ::CheckMenuItem(menuSub, IDM_HOSTS_1, MF_BYCOMMAND | MF_CHECKED);
		if(m_state==2) ::CheckMenuItem(menuSub, IDM_HOSTS_2, MF_BYCOMMAND | MF_CHECKED);
		if(m_state==3) ::CheckMenuItem(menuSub, IDM_HOSTS_3, MF_BYCOMMAND | MF_CHECKED);

		CPoint pnt;
		::GetCursorPos(&pnt);
		SetForegroundWindow(m_hWnd);
		int ret = ::TrackPopupMenu(menuSub, TPM_RIGHTBUTTON | TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RETURNCMD, pnt.x, pnt.y, 0, m_hWnd, NULL);
		DestroyMenu(menu);

		if(ret>0)
			PostMessage(WM_COMMAND, ret);
	}
	else if( lParam==WM_LBUTTONUP )
	{
		if(m_state==0) PostMessage(WM_COMMAND, IDM_HOSTS_1);
		if(m_state==1) PostMessage(WM_COMMAND, IDM_HOSTS_2);
		if(m_state==2) PostMessage(WM_COMMAND, IDM_HOSTS_3);
		if(m_state==3) PostMessage(WM_COMMAND, IDM_HOSTS_0);
	}


	return 0;
}

LRESULT CMainWnd::OnTaskbarCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetTrayIcon();
	return 0;
}


LRESULT CMainWnd::OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}



LRESULT CMainWnd::OnHosts(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int id = 0;
	switch(wID)
	{
	case IDM_HOSTS_0 : id = 0; break;
	case IDM_HOSTS_1 : id = 1; break;
	case IDM_HOSTS_2 : id = 2; break;
	case IDM_HOSTS_3 : id = 3; break;
	default : ASSERT(0); return 0;
	}


	if(::CopyFile(m_hosts[id], m_hostsTarget, FALSE)==FALSE)
	{
		MessageBox(NULL, L"어라 파일을 복사하는데 에러가 발생했네요.", MB_OK);
		return 0;
	}

	m_state = id;
	ModifyTrayIcon();

	return 0;
}


LRESULT CMainWnd::OnNotepadHosts(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int id = 0;
	switch(wID)
	{
	case IDM_NOTEPAD_HOSTS:	::ShellExecute(NULL, L"open", L"notepad", m_hostsTarget, m_folder, SW_SHOW); return 0;
	case IDM_NOTEPAD_HOSTS_0 : id = 0; break;
	case IDM_NOTEPAD_HOSTS_1 : id = 1; break;
	case IDM_NOTEPAD_HOSTS_2 : id = 2; break;
	case IDM_NOTEPAD_HOSTS_3 : id = 3; break;
	default : ASSERT(0); return 0;
	}

	::ShellExecute(NULL, L"open", L"notepad", m_hosts[id], m_folder, SW_SHOW);
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///         폴더 열기
/// @param  
/// @return 
/// @date   Friday, May 27, 2011  11:38:30 AM
////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CMainWnd::OnOpenFolder(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	::ShellExecute(NULL, L"open", m_folder, NULL, NULL, SW_SHOW);

	return 0;
}


LRESULT CMainWnd::OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	::ShellExecute(NULL, L"open", L"http://www.kippler.com/hosts_toggler/", NULL, NULL, SW_SHOW);

	return 0;
}
