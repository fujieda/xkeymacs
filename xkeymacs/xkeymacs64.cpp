#include "xkeymacs64.h"
#include "mainfrm64.h"
#include "../xkeymacsdll/xkeymacsdll.h"

CXkeymacsApp::CXkeymacsApp()
{
	m_hMutex = NULL;
}

CXkeymacsApp theApp;

static UINT PollIPCMessage(LPVOID lpParam);
static void Start32bitProcess();

BOOL CXkeymacsApp::InitInstance()
{
	m_hMutex = CreateMutex(NULL, FALSE, _T("XKeymacs64"));
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
		return FALSE;
	}
	bool start32bit = false;
	HANDLE h32 = CreateMutex(NULL, FALSE, _T("XKeymacs"));
	if (GetLastError() != ERROR_ALREADY_EXISTS)
		start32bit = true;
	CloseHandle(h32);

	m_pMainWnd = new CMainFrame;
	m_pMainWnd->ShowWindow(SW_HIDE);
	m_pMainWnd->UpdateWindow();

	AfxBeginThread(PollIPCMessage, NULL);
	if (start32bit)
		Start32bitProcess();
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
			CXkeymacsDll::LoadConfig();
			break;
		case XKEYMACS_RESET:
			CXkeymacsDll::ResetHooks();
			break;
		}
	}
exit:
	CloseHandle(hPipe);
	CXkeymacsDll::ReleaseHooks();
	ExitProcess(0);
	return 0;
}

void Start32bitProcess()
{
	TCHAR buf[MAX_PATH];
	if (!GetModuleFileName(NULL, buf, MAX_PATH))
		return;
	CString path = buf;
	if (!path.Replace(_T("xkeymacs64.exe"), _T("xkeymacs.exe")))
		return;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	if (!CreateProcess(path, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		return;
	// close unused handles
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
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
