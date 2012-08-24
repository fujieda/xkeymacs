// xkeymacs.cpp : Defines the class behaviors for the application.
//

#include "xkeymacs.h"
#include "mainfrm.h"
#include "profile.h"
#include "../xkeymacsdll/Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXkeymacsApp

BEGIN_MESSAGE_MAP(CXkeymacsApp, CWinApp)
	//{{AFX_MSG_MAP(CXkeymacsApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXkeymacsApp construction

CXkeymacsApp::CXkeymacsApp()
{
	m_hMutex = NULL;
	m_bIsWow64 = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CXkeymacsApp object

CXkeymacsApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CXkeymacsApp initialization

BOOL CXkeymacsApp::InitInstance()
{
//	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	m_hMutex = CreateMutex(FALSE, 0, CString(MAKEINTRESOURCE(AFX_IDS_APP_TITLE)));
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
		return FALSE;
	}

	// Change the registry key under which our settings are stored.
	SetRegistryKey(IDS_REGISTRY_KEY);
//
//	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Parse command line for standard shell commands, DDE, file open
//	CCommandLineInfo cmdInfo;
//	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
//	if (!ProcessShellCommand(cmdInfo))
//		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd = new CMainFrame;
	m_pMainWnd->ShowWindow(SW_HIDE);
	m_pMainWnd->UpdateWindow();
	SetClassLongPtr(m_pMainWnd->m_hWnd, GCLP_HICON, (LONG_PTR)LoadIcon(IDR_MAINFRAME));

	if (!Start64bitProcess())
		return FALSE;
	CProfile::InitDllData();

	return TRUE;
}

BOOL CXkeymacsApp::IsWow64()
{
	return m_bIsWow64;
}

BOOL CXkeymacsApp::Start64bitProcess()
{
	typedef BOOL (WINAPI *PFIsWow64Process)(HANDLE, PBOOL);
	PFIsWow64Process func = (PFIsWow64Process)GetProcAddress(GetModuleHandle(_T("kernel32")), _T("IsWow64Process"));
	if (!func)
		return TRUE; // IsWow64Process not exists
	if (!func(GetCurrentProcess(), &m_bIsWow64))
		return FALSE; // error
	if (!m_bIsWow64)
		return TRUE; // do nothing
	
	TCHAR buf[MAX_PATH];
	if (!GetModuleFileName(NULL, buf, MAX_PATH))
		return FALSE;
	CString path = buf;
	if (!path.Replace(_T("xkeymacs.exe"), _T("xkeymacs64.exe")))
		return FALSE;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	if (!CreateProcess(path, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		return FALSE;
	// wait until the child process starts.
	if (WaitForInputIdle(pi.hProcess, INFINITE))
		return FALSE;
	// close unused handles
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return TRUE;
}

void CXkeymacsApp::SendIPC64Message(DWORD msg)
{
	if (!m_bIsWow64)
		return;
	DWORD ack, read;
	for (int i = 0; i < 10; Sleep(100), ++i)
		if (CallNamedPipe(XKEYMACS64_PIPE, &msg, sizeof(msg), &ack, sizeof(DWORD), &read, NMPWAIT_WAIT_FOREVER))
			return;
#ifdef DEBUG_IPC
		else
			CUtils::Log(_T("SendIPC64Message: CallNamedPipe failed. (%d)"), GetLastError());
#endif
}

int CXkeymacsApp::ExitInstance() 
{
	if (m_hMutex) {
		ReleaseMutex(m_hMutex);
		CloseHandle(m_hMutex);

		m_pMainWnd->DestroyWindow();
		delete m_pMainWnd;
		m_pMainWnd = NULL;
	}

	return CWinApp::ExitInstance();
}
