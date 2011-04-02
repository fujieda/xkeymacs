// xkeymacs.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "xkeymacs.h"
#include "Profile.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXkeymacsApp

BEGIN_MESSAGE_MAP(CXkeymacsApp, CWinApp)
	//{{AFX_MSG_MAP(CXkeymacsApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXkeymacsApp construction

CXkeymacsApp::CXkeymacsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	m_Instance = FirstInstance;
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

    if (::GetLastError() == ERROR_ALREADY_EXISTS) {
		m_Instance = SecondInstance;
        CloseHandle( m_hMutex );
        return FALSE;
    }

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
//	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	SetRegistryKey(IDS_REGISTRY_KEY);
	free((void*)m_pszProfileName);
	m_pszProfileName=_tcsdup(_T("XKeymacs2"));
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
	SetClassLong(m_pMainWnd->m_hWnd, GCL_HICON, (LONG)LoadIcon(IDR_MAINFRAME));

	// set registry key
//	CMainFrame *pMainWnd = (CMainFrame*)m_pMainWnd;
	CProfile::InitDllData();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_szVersionInformation = _T("");
	m_szLegalCopyright = _T("");
	//}}AFX_DATA_INIT

	m_szLegalCopyright.Format(_T("Copyright (C) 2001-2006"));	// rough

	WORD wMajorVersion		= 0;
	WORD wMinorVersion		= 0;
	WORD wBuildVersion		= 0;
	WORD wRevisionVersion	= 0;

	TCHAR szFileName[MAX_PATH] = {'\0'};
	GetModuleFileName(NULL, szFileName, sizeof(szFileName));
	DWORD dwHandle = 0;

	DWORD dwLen = GetFileVersionInfoSize(szFileName, &dwHandle);
	if (dwLen) {
		LPVOID lpData = new BYTE[dwLen];
		if (GetFileVersionInfo(szFileName, dwHandle, dwLen, lpData)) {
			VS_FIXEDFILEINFO *pInfo;
			UINT uLen;
			if (VerQueryValue(lpData, _T("\\"), (LPVOID *)&pInfo, &uLen)) {
				wMajorVersion		= (WORD)((pInfo->dwProductVersionMS >> 16) & 0xffff);
				wMinorVersion		= (WORD)((pInfo->dwProductVersionMS      ) & 0xffff);
				wBuildVersion		= (WORD)((pInfo->dwProductVersionLS >> 16) & 0xffff);
				wRevisionVersion	= (WORD)((pInfo->dwProductVersionLS      ) & 0xffff);
			}

			Translate_t *lpTranslate = NULL;
			UINT cbTranslate = 0;
			if (VerQueryValue(lpData, _T("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &cbTranslate) && sizeof(Translate_t) <= cbTranslate) {
				LPVOID lpLegalCopyright = NULL;
				UINT uLen = 0;
				CString SubBlock;

				SubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\LegalCopyright"), lpTranslate->wLanguage, lpTranslate->wCodePage);
				if (VerQueryValue(lpData, SubBlock.GetBuffer(SubBlock.GetLength()), (LPVOID *)&lpLegalCopyright, &uLen) && uLen) {
					m_szLegalCopyright.Format(_T("%s"), lpLegalCopyright);
				}
			}
		}
		delete[] lpData;
		lpData = NULL;
	}

	m_szVersionInformation.Format(_T("%s Version %d.%d"), CString(MAKEINTRESOURCE(AFX_IDS_APP_TITLE)), wMajorVersion, wMinorVersion);

	if (wBuildVersion) {
		CString sz;
		sz.Format(_T(".%d"), wBuildVersion);
		m_szVersionInformation += sz;

		if (wRevisionVersion) {
			CString sz;
			sz.Format(_T(".%d"), wRevisionVersion);
			m_szVersionInformation += sz;
		}
	}
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_URL, m_cURL);
	DDX_Text(pDX, IDC_VERSION_INFORMATION, m_szVersionInformation);
	DDX_Text(pDX, IDC_LEGAL_COPYRIGHT, m_szLegalCopyright);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_URL, OnUrl)
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CXkeymacsApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CXkeymacsApp message handlers


int CXkeymacsApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	if (m_Instance == FirstInstance) {
	    ReleaseMutex( m_hMutex );
		CloseHandle( m_hMutex );

		m_pMainWnd->DestroyWindow();
		delete m_pMainWnd;
		m_pMainWnd = NULL;
	}

	return CWinApp::ExitInstance();
}

void CAboutDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}


void CAboutDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}

void CAboutDlg::OnUrl() 
{
	// TODO: Add your message handler code here and/or call default
	CString szURL;
	m_cURL.GetWindowText(szURL);

	ShellExecute(m_hWnd, NULL, szURL, NULL, NULL, SW_SHOWNORMAL);
}

HBRUSH CAboutDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	COLORREF crBlue = RGB(0, 0, 0xff);

	if (pWnd == &m_cURL) {
		pDC->SetTextColor(crBlue);
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CAboutDlg::OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	m_cURL.GetWindowRect(rect);

	POINT p;
	::GetCursorPos(&p);

	if (rect.PtInRect(p)) {
		HCURSOR hCursor = (HCURSOR)LoadImage(theApp.m_hInstance, MAKEINTRESOURCE(IDC_HAND_CURSOR),
											 IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
		SetCursor(hCursor);
		return TRUE;	
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
