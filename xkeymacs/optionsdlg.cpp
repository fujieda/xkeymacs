// OptionsDlg.cpp : implementation file
//

#include "optionsdlg.h"
#include "mainfrm.h"
#include "../xkeymacsdll/defs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog


COptionsDlg::COptionsDlg(CWnd *pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	//}}AFX_DATA_INIT
}

COptionsDlg::~COptionsDlg()
{
}

void COptionsDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Control(pDX, IDC_M_X, m_cMx);
	DDX_Control(pDX, IDC_M_, m_cM_);
	DDX_Control(pDX, IDC_ALT, m_cAlt);
	DDX_Control(pDX, IDC_SHIFT, m_cShift);
	DDX_Control(pDX, IDC_CTRL, m_cCtrl);
	DDX_Control(pDX, IDC_C_X_, m_cCx);
	DDX_Control(pDX, IDC_FILE_COMPLETION, m_cFileCompletion);
	DDX_Control(pDX, IDC_AUTO_RUN, m_cAutoRun);
	DDX_Control(pDX, IDOK, m_cOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

BOOL COptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// clear all check
	m_cAutoRun.SetCheck(0);
	m_cCx.SetCheck(0);
	m_cMx.SetCheck(0);
	m_cM_.SetCheck(0);
	m_cShift.SetCheck(0);
	m_cCtrl.SetCheck(0);
	m_cAlt.SetCheck(0);

	// for auto run
	{
		TCHAR szFileName[MAX_PATH] = {'\0'};
		GetModuleFileName(NULL, szFileName, sizeof(szFileName));
		HKEY hKey = NULL;

		if (RegOpenKeyEx(HKEY_CURRENT_USER,  CString(MAKEINTRESOURCE(IDS_REGSUBKEY_AUTORUN)), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS
		 || RegOpenKeyEx(HKEY_LOCAL_MACHINE, CString(MAKEINTRESOURCE(IDS_REGSUBKEY_AUTORUN)), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {
			DWORD dwType = REG_SZ;
			TCHAR szSavedFileName[MAX_PATH] = {'\0'};
			DWORD dwBufLen = sizeof(szSavedFileName);
			RegQueryValueEx(hKey, CString(MAKEINTRESOURCE(AFX_IDS_APP_TITLE)), NULL, &dwType,
							(LPBYTE)szSavedFileName, &dwBufLen);
			RegCloseKey(hKey);

			if (!_tcsnicmp(szFileName, szSavedFileName, sizeof(szFileName))) {
				m_cAutoRun.SetCheck(1);
			}
		}
	}

	// for file and directory name completion
	HKEY hKey = NULL;

	if (RegOpenKeyEx(HKEY_CURRENT_USER,  CString(MAKEINTRESOURCE(IDS_REGSUBKEY_FILE_COMPLETION)), 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS
			|| RegOpenKeyEx(HKEY_LOCAL_MACHINE, CString(MAKEINTRESOURCE(IDS_REGSUBKEY_FILE_COMPLETION)), 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
		DWORD dwType = REG_DWORD;
		DWORD dwCompletionChar = 0;
		DWORD dwcbData = sizeof(dwCompletionChar);
		RegQueryValueEx(hKey, CString(MAKEINTRESOURCE(IDS_COMPLETION_CHAR)), NULL, &dwType,
			(LPBYTE)&dwCompletionChar, &dwcbData);
		RegCloseKey(hKey);
		if (dwCompletionChar == 0x9) {
			m_cFileCompletion.SetCheck(1);
		}
	}

	// for icons in the taskbar's status area
	m_cCx.SetCheck(AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_CX)), 0));
	m_cMx.SetCheck(AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_MX)), 0));
	m_cM_.SetCheck(AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_META)), 0));
	m_cShift.SetCheck(AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_SHIFT)), 0));
	m_cCtrl.SetCheck(AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_CTRL)), 0));
	m_cAlt.SetCheck(AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_ALT)), 0));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsDlg::OnOK() 
{
	// for auto run
	{
		TCHAR szFileName[MAX_PATH] = {'\0'};
		GetModuleFileName(NULL, szFileName, sizeof(szFileName));
		HKEY hKey = NULL;

		DWORD dwDisposition;
		if (RegCreateKeyEx(HKEY_CURRENT_USER, CString(MAKEINTRESOURCE(IDS_REGSUBKEY_AUTORUN)), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition) == ERROR_SUCCESS) {
			if (m_cAutoRun.GetCheck() == 1) {
				RegSetValueEx(hKey, CString(MAKEINTRESOURCE(AFX_IDS_APP_TITLE)), 0, REG_SZ, (LPBYTE)szFileName, _tcslen(szFileName));
			} else {
				RegDeleteValue(hKey, CString(MAKEINTRESOURCE(AFX_IDS_APP_TITLE)));
			}
			RegCloseKey(hKey);
		}
	}

	// for file and directory name completion
	HKEY hKey = NULL;

	if (RegCreateKeyEx(HKEY_CURRENT_USER, CString(MAKEINTRESOURCE(IDS_REGSUBKEY_FILE_COMPLETION)), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
		DWORD dwCompletionChar;
		if (m_cFileCompletion.GetCheck() == 1) {
			dwCompletionChar = 0x9;	// use enum
		} else {
			dwCompletionChar = 0x0;
		}
		RegSetValueEx(hKey, CString(MAKEINTRESOURCE(IDS_COMPLETION_CHAR)), 0, REG_DWORD,
			(LPBYTE)&dwCompletionChar, sizeof(dwCompletionChar));
		RegCloseKey(hKey);
	}

	// for icons in the taskbar's status area
	{
		// write data in registry
		AfxGetApp()->WriteProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_CX)), m_cCx.GetCheck());
		AfxGetApp()->WriteProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_MX)), m_cMx.GetCheck());
		AfxGetApp()->WriteProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_META)), m_cM_.GetCheck());
		AfxGetApp()->WriteProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_SHIFT)), m_cShift.GetCheck());
		AfxGetApp()->WriteProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_CTRL)), m_cCtrl.GetCheck());
		AfxGetApp()->WriteProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_ALT)), m_cAlt.GetCheck());

		// set dll data
		CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
		pMainFrame->EnableShell_NotifyIcon(CX_ICON, m_cCx.GetCheck());
		pMainFrame->EnableShell_NotifyIcon(MX_ICON, m_cMx.GetCheck());
		pMainFrame->EnableShell_NotifyIcon(META_ICON, m_cM_.GetCheck());
		pMainFrame->EnableShell_NotifyIcon(SHIFT_ICON, m_cShift.GetCheck());
		pMainFrame->EnableShell_NotifyIcon(CTRL_ICON, m_cCtrl.GetCheck());
		pMainFrame->EnableShell_NotifyIcon(ALT_ICON, m_cAlt.GetCheck());
	}

	CDialog::OnOK();
}
