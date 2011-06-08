#include "stdafx.h"
#include "xkeymacs64.h"
#include "mainfrm64.h"

CXkeymacsApp::CXkeymacsApp()
{
	m_hMutex = NULL;
}

CXkeymacsApp theApp;

static UINT PollIPCMessage(LPVOID lpParam);

BOOL CXkeymacsApp::InitInstance()
{
	HANDLE h32 = CreateMutex(FALSE, 0, CString(MAKEINTRESOURCE(AFX_IDS_APP_TITLE)));
	if (GetLastError() != ERROR_ALREADY_EXISTS) {
		AfxMessageBox(CString(MAKEINTRESOURCE(IDS_ERR_32BIT)));
		CloseHandle(h32);
		return FALSE;
	}
	CloseHandle(h32);
	m_hMutex = CreateMutex(FALSE, 0, CString(MAKEINTRESOURCE(AFX_IDS_APP_TITLE)) + _T("64"));
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle(m_hMutex);
		m_hMutex = NULL;
        return FALSE;
    }

	m_pMainWnd = new CMainFrame;
	m_pMainWnd->ShowWindow(SW_HIDE);
	m_pMainWnd->UpdateWindow();
	SetClassLongPtr(m_pMainWnd->m_hWnd, GCLP_HICON, (LONG_PTR)LoadIcon(IDR_MAINFRAME));

	CUtils::InitCUtils();
	AfxBeginThread(PollIPCMessage, NULL);
	CXkeymacsDll::SetHooks();
	return TRUE;
}

UINT PollIPCMessage(LPVOID lpParam)
{
	HANDLE hPipe = CreateNamedPipe(IPC_PIPE, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 1, 512, 512, 0, NULL);
	if (hPipe == INVALID_HANDLE_VALUE)
		return 1;
	for (; ;) {
		if (ConnectNamedPipe(hPipe, NULL) ? FALSE : (GetLastError() != ERROR_PIPE_CONNECTED))
			break;
		DWORD msg;
		DWORD read;
		if (!ReadFile(hPipe, &msg, sizeof(msg), &read, NULL) || read != sizeof(msg)) 
			break;
		DWORD written, nul = 0;
		if (!WriteFile(hPipe, &nul, sizeof(DWORD), &written, NULL) || written != sizeof(DWORD)
				|| !FlushFileBuffers(hPipe) || !DisconnectNamedPipe(hPipe))
			break;
		switch (msg)
		{
		case XKEYMACS_EXIT:
			goto exit;
			break;
		case XKEYMACS_RELOAD:
			if (CXkeymacsDll::LoadConfig())
				CXkeymacsDll::EnableKeyboardHook();
			break;
		case XKEYMACS_RESET:
			//CXkeymacsDll::ResetHook();
			break;
		}
	}
exit:
	CloseHandle(hPipe);
	CXkeymacsDll::ReleaseHooks();
	ExitProcess(0);
	return 0;
}

int CXkeymacsApp::ExitInstance() 
{
	if (m_hMutex) {
		CloseHandle(m_hMutex);

		m_pMainWnd->DestroyWindow();
		delete m_pMainWnd;
		m_pMainWnd = NULL;
	}

	return CWinApp::ExitInstance();
}
