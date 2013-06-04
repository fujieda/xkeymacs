#include "xkeymacs64.h"
#include "mainfrm64.h"
#include "../xkeymacsdll/xkeymacsdll.h"
#include "../xkeymacsdll/Utils.h"
#include "../xkeymacsdll/PipeName.h"

CXkeymacsApp::CXkeymacsApp()
{
	m_hMutex = NULL;
}

CXkeymacsApp theApp;

static UINT PollIPCMessage(LPVOID param);
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

	AfxBeginThread(PollIPCMessage, m_pMainWnd);
	if (start32bit)
		Start32bitProcess();
	return TRUE;
}

bool SendAck(HANDLE pipe)
{
	DWORD written, ack = 0;
	if (!WriteFile(pipe, &ack, sizeof(DWORD), &written, NULL) && written != sizeof(DWORD)) {
#ifdef DEBUG_IPC
		CUtils::Log(_T("SendAck: WriteFile failed. (%d)"), GetLastError());
#endif
		return false;
	}
	if (!FlushFileBuffers(pipe)) {
#ifdef DEBUG_IPC
		CUtils::Log(_T("SendAck: FlushFileBuffers failed. (%d)"), GetLastError());
#endif
		return false;
	}
	if (!DisconnectNamedPipe(pipe)) {
#ifdef DEBUG_IPC
		CUtils::Log(_T("SendAck: DisconnectNamedPipe failed. (%d)"), GetLastError());
#endif
		return false;
	}
	return true;
}

UINT PollIPCMessage(LPVOID param)
{
	HANDLE hPipe = CreateNamedPipe(PipeName(PIPENAME_IPC64).GetName(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 1, 512, 512, 0, NULL);
	if (hPipe == INVALID_HANDLE_VALUE) {
#ifdef DEBUG_IPC
		CUtils::Log(_T("PollIPCMessage: CreateNamedPipe failed. (%d)"), GetLastError());
#endif
		return 1;
	}
	for (; ;) {
		if (ConnectNamedPipe(hPipe, NULL) ? FALSE : (GetLastError() != ERROR_PIPE_CONNECTED)) {
#ifdef DEBUG_IPC
			CUtils::Log(_T("PollIPCMessage: ConnectNamedPipe failed. (%d)"), GetLastError());
#endif
			break;
		}
		DWORD msg;
		DWORD read;
		if (!ReadFile(hPipe, &msg, sizeof(msg), &read, NULL) || read != sizeof(msg)) {
#ifdef DEBUG_IPC
			CUtils::Log(_T("PollIPCMessage: ReadFile failed. (%d)"), GetLastError());
#endif
			break;
		}
		switch (msg)
		{
		case IPC64_EXIT:
			goto exit;
			break;
		case IPC64_RELOAD:
			if (CXkeymacsDll::LoadConfig())
				CXkeymacsDll::ResetHooks();
			break;
		case IPC64_DISABLE:
			CXkeymacsDll::SetHookStateDirect(false);
			break;
		case IPC64_ENABLE:
			CXkeymacsDll::SetHookStateDirect(true);
			break;
		}
		SendAck(hPipe);
	}
exit:
	CloseHandle(hPipe);
	CXkeymacsDll::ReleaseHooks();
	reinterpret_cast<CMainFrame *>(param)->SendMessage(WM_CLOSE);
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
	if (m_hMutex)
		CloseHandle(m_hMutex);
	return CWinApp::ExitInstance();
}
