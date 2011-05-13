// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include <Lmcons.h>
#include "xkeymacs.h"
#include "Profile.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_USER_NTFYICON (WM_USER+100)

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(IDC_ABOUT, OnAbout)
	ON_COMMAND(IDC_QUIT, OnQuit)
	ON_COMMAND(IDC_DISABLE, OnDisable)
	ON_COMMAND(IDC_PROPERTIES, OnProperties)
	ON_COMMAND(IDC_OPTIONS, OnOptions)
	ON_COMMAND(IDC_KEYBOARD, OnKeyboard)
	ON_COMMAND(IDC_IMPORT, OnImport)
	ON_COMMAND(IDC_EXPORT, OnExport)
	ON_COMMAND(IDC_RESET, OnReset)
	ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
	ON_COMMAND(IDC_KEYBOARD_ANY_USER, OnKeyboardAnyUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_pXkeymacsDll			= new CXkeymacsDll;
	m_pPropertiesDlg		= new CProperties;
	m_nResultPropertiesDlg	= -1;
	m_bPropertiesDlgExist	= FALSE;
	m_pAboutDlg				= new CAboutDlg;
	m_nResultAboutDlg		= -1;
	m_bAboutDlgExist		= FALSE;
	m_pOptionsDlg			= new COptionsDlg;
	m_nResultOptionsDlg		= -1;
	m_bOptionsDlgExist		= FALSE;
	memset(m_p104KeyboardDlg, 0, sizeof(m_p104KeyboardDlg));
	memset(m_p109KeyboardDlg, 0, sizeof(m_p109KeyboardDlg));
	for (int i = 0; i < MAX_HKEY_TYPE; ++i) {
		m_p104KeyboardDlg[i] = new C104Keyboard((HKEY_TYPE) i);
		m_p109KeyboardDlg[i] = new C109Keyboard((HKEY_TYPE) i);
	}
	memset(m_nResultKeyboardDlg, -1, sizeof(m_nResultKeyboardDlg));
	memset(m_bKeyboardDlgExist, 0, sizeof(m_bKeyboardDlgExist));

	// register window class
	WNDCLASS	stWndClass;
	CString szClassName(MAKEINTRESOURCE(AFX_IDS_APP_TITLE));

	stWndClass.style			= CS_BYTEALIGNWINDOW;
	stWndClass.lpfnWndProc		= &AfxWndProc;
	stWndClass.cbClsExtra		= 0;
	stWndClass.cbWndExtra		= 0;
	stWndClass.hInstance		= AfxGetInstanceHandle();
	stWndClass.hIcon			= NULL;
	stWndClass.hCursor			= NULL;
	stWndClass.hbrBackground	= (HBRUSH)COLOR_BACKGROUND;
	stWndClass.lpszMenuName		= NULL;
	stWndClass.lpszClassName	= szClassName;

	AfxRegisterClass(&stWndClass);

	// make window
	CRect	rect(0, 0, 100, 100);

	Create(szClassName, CString(MAKEINTRESOURCE(AFX_IDS_APP_TITLE)), WS_OVERLAPPEDWINDOW | WS_MINIMIZE, rect);
}

CMainFrame::~CMainFrame()
{
	delete m_pXkeymacsDll;
	m_pXkeymacsDll = NULL;

	delete m_pPropertiesDlg;
	m_pPropertiesDlg = NULL;

	delete m_pAboutDlg;
	m_pAboutDlg = NULL;

	delete m_pOptionsDlg;
	m_pOptionsDlg = NULL;

	for (int i = 0; i < MAX_HKEY_TYPE; ++i) {
		delete m_p104KeyboardDlg[i];
		m_p104KeyboardDlg[i] = NULL;

		delete m_p109KeyboardDlg[i];
		m_p109KeyboardDlg[i] = NULL;
	}
}

int CMainFrame::OnCreate(const LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	m_pXkeymacsDll->Set106Keyboard(CProfile::Is106Keyboard());

	// init notify icon data
	NOTIFYICONDATA notifyIconData[MAX_ICON_TYPE] = {
		{ sizeof(NOTIFYICONDATA), m_hWnd, MAIN_ICON, NIF_MESSAGE | NIF_ICON | NIF_TIP, WM_USER_NTFYICON,
		  (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, LR_SHARED) },
		{ sizeof(NOTIFYICONDATA), m_hWnd, CX_ICON, NIF_MESSAGE | NIF_ICON | NIF_TIP, WM_USER_NTFYICON,
		  AfxGetApp()->LoadIcon(IDI_CX_OFF) },
		{ sizeof(NOTIFYICONDATA), m_hWnd, MX_ICON, NIF_MESSAGE | NIF_ICON | NIF_TIP, WM_USER_NTFYICON,
		  AfxGetApp()->LoadIcon(IDI_MX_OFF) },
		{ sizeof(NOTIFYICONDATA), m_hWnd, META_ICON, NIF_MESSAGE | NIF_ICON | NIF_TIP, WM_USER_NTFYICON,
		  AfxGetApp()->LoadIcon(IDI_META_OFF) },
		{ sizeof(NOTIFYICONDATA), m_hWnd, SHIFT_ICON, NIF_MESSAGE | NIF_ICON | NIF_TIP, WM_USER_NTFYICON,
		  AfxGetApp()->LoadIcon(IDI_SHIFT_OFF) },
		{ sizeof(NOTIFYICONDATA), m_hWnd, CTRL_ICON, NIF_MESSAGE | NIF_ICON | NIF_TIP, WM_USER_NTFYICON,
		  AfxGetApp()->LoadIcon(IDI_CTRL_OFF) },
		{ sizeof(NOTIFYICONDATA), m_hWnd, ALT_ICON, NIF_MESSAGE | NIF_ICON | NIF_TIP, WM_USER_NTFYICON,
		  AfxGetApp()->LoadIcon(IDI_ALT_OFF) },
	};
	LoadString(AfxGetInstanceHandle(), AFX_IDS_APP_TITLE, notifyIconData[MAIN_ICON].szTip,
			   sizeof(notifyIconData[MAIN_ICON].szTip));
	LoadString(AfxGetInstanceHandle(), IDS_TIP_CX, notifyIconData[CX_ICON].szTip,
			   sizeof(notifyIconData[CX_ICON].szTip));
	LoadString(AfxGetInstanceHandle(), IDS_TIP_MX, notifyIconData[MX_ICON].szTip,
			   sizeof(notifyIconData[MX_ICON].szTip));
	LoadString(AfxGetInstanceHandle(), IDS_TIP_META, notifyIconData[META_ICON].szTip,
			   sizeof(notifyIconData[META_ICON].szTip));
	LoadString(AfxGetInstanceHandle(), IDS_TIP_SHIFT, notifyIconData[SHIFT_ICON].szTip,
			   sizeof(notifyIconData[SHIFT_ICON].szTip));
	LoadString(AfxGetInstanceHandle(), IDS_TIP_CTRL, notifyIconData[CTRL_ICON].szTip,
			   sizeof(notifyIconData[CTRL_ICON].szTip));
	LoadString(AfxGetInstanceHandle(), IDS_TIP_ALT, notifyIconData[ALT_ICON].szTip,
			   sizeof(notifyIconData[ALT_ICON].szTip));

	// set notify icon data
	m_pXkeymacsDll->SetNotifyIconData(MAIN_ICON, notifyIconData[MAIN_ICON],
		(HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, LR_SHARED),
		(HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAIN_DISABLE_TMP), IMAGE_ICON, 16, 16, LR_SHARED),
		(HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAIN_DISABLE_WOCQ), IMAGE_ICON, 16, 16, LR_SHARED),		// disable without C-q
		(HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAIN_DISABLE), IMAGE_ICON, 16, 16, LR_SHARED),
		AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_MAIN)), 1));

	// set notify ^X icon data
	m_pXkeymacsDll->SetNotifyIconData(CX_ICON, notifyIconData[CX_ICON], AfxGetApp()->LoadIcon(IDI_CX_ON),
									  AfxGetApp()->LoadIcon(IDI_CX_OFF),
									  AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_CX)), 0));

	// set notify M-x icon data
	m_pXkeymacsDll->SetNotifyIconData(MX_ICON, notifyIconData[MX_ICON], AfxGetApp()->LoadIcon(IDI_MX_ON),
									  AfxGetApp()->LoadIcon(IDI_MX_OFF),
									  AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_MX)), 0));

	// set notify Meta icon data
	m_pXkeymacsDll->SetNotifyIconData(META_ICON, notifyIconData[META_ICON], AfxGetApp()->LoadIcon(IDI_META_ON),
									  AfxGetApp()->LoadIcon(IDI_META_OFF),
									  AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_META)), 0));

	// set notify Shift icon data
	m_pXkeymacsDll->SetNotifyIconData(SHIFT_ICON, notifyIconData[SHIFT_ICON], AfxGetApp()->LoadIcon(IDI_SHIFT_ON),
									  AfxGetApp()->LoadIcon(IDI_SHIFT_OFF),
									  AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_SHIFT)), 0));

	// set notify Ctrl icon data
	m_pXkeymacsDll->SetNotifyIconData(CTRL_ICON, notifyIconData[CTRL_ICON], AfxGetApp()->LoadIcon(IDI_CTRL_ON),
									  AfxGetApp()->LoadIcon(IDI_CTRL_OFF),
									  AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_CTRL)), 0));

	// set notify Alt icon data
	m_pXkeymacsDll->SetNotifyIconData(ALT_ICON, notifyIconData[ALT_ICON], AfxGetApp()->LoadIcon(IDI_ALT_ON),
									  AfxGetApp()->LoadIcon(IDI_ALT_OFF),
									  AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_ALT)), 0));

	m_pXkeymacsDll->SetKeyboardSpeed(CProfile::GetKeyboardSpeed());
	m_pXkeymacsDll->SetAccelerate(AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_ACCELERATE)), 1));

	m_pXkeymacsDll->SetCursorData((HCURSOR)::LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
								  (HCURSOR)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_DISABLE_TMP_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
								  (HCURSOR)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_DISABLE_WOCQ_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
								  (HCURSOR)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_DISABLE_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
								  AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_CHANGE_CURSOR)), 0));

	m_pXkeymacsDll->SetHooks();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::SetForegroundTheDialog(CDialog *const pDialog)
{
	if (pDialog) {
		pDialog->SetForegroundWindow();
	}
}

void CMainFrame::SetForegroundSomeDialog()
{
	CDialog *pDialog = NULL;

	if (m_bPropertiesDlgExist) {
		pDialog = m_pPropertiesDlg;
	} else if (m_bOptionsDlgExist) {
		pDialog = m_pOptionsDlg;
	} else if (m_bKeyboardDlgExist[ENGLISH_KEYBOARD][CURRENT_USER]) {
		pDialog = m_p104KeyboardDlg[CURRENT_USER];
	} else if (m_bKeyboardDlgExist[ENGLISH_KEYBOARD][LOCAL_MACHINE]) {
		pDialog = m_p104KeyboardDlg[LOCAL_MACHINE];
	} else if (m_bKeyboardDlgExist[JAPANESE_KEYBOARD][CURRENT_USER]) {
		pDialog = m_p109KeyboardDlg[CURRENT_USER];
	} else if (m_bKeyboardDlgExist[JAPANESE_KEYBOARD][LOCAL_MACHINE]) {
		pDialog = m_p109KeyboardDlg[LOCAL_MACHINE];
	} else if (m_bAboutDlgExist) {
		pDialog = m_pAboutDlg;
	} else {
		EnableWindow(FALSE);
		return;
	}

	SetForegroundTheDialog(pDialog);
	return;
}

void CMainFrame::PopUpDialog(CDialog *const pDialog, int *const pnResultDialog, BOOL *const pbDialogExist)
{
	if (pDialog			== NULL
	 || pnResultDialog	== NULL
	 || pbDialogExist	== NULL) {
		return;
	}

	if (!*pbDialogExist) {
		*pbDialogExist = TRUE;
		*pnResultDialog = pDialog->DoModal();
		*pbDialogExist = FALSE;
		*pnResultDialog = -1;

		SetForegroundSomeDialog();
	} else {
		SetForegroundTheDialog(pDialog);
	}
}

void CMainFrame::PopUpPropertiesDialog()
{
	PopUpDialog(m_pPropertiesDlg, &m_nResultPropertiesDlg, &m_bPropertiesDlgExist);
}

// show right menu on icon in system tray
void CMainFrame::ShowRightMenu(const ICON_TYPE icon)
{
	CMenu rightMenu;
	rightMenu.LoadMenu(IDR_RIGHT_MENU);

	CMenu *popUp = rightMenu.GetSubMenu(0);

	if (icon == MAIN_ICON) {
		popUp->SetDefaultItem(IDC_PROPERTIES);

		CString szDisable(MAKEINTRESOURCE(IDS_ENABLE));
		if (m_pXkeymacsDll->IsKeyboardHook()) {
			szDisable.LoadString(IDS_DISABLE);
		}
		szDisable += _T("\tCtrl+Q");
		popUp->ModifyMenu(IDC_DISABLE, MF_STRING, IDC_DISABLE, szDisable);

		TCHAR szUserName[UNLEN + 1] = {'0'};
		DWORD nSize = UNLEN + 1;
		if (GetUserName(szUserName, &nSize)) {
			CString szFor;
			szFor.Format(IDS_KEYBOARD, szUserName);
			popUp->ModifyMenu(IDC_KEYBOARD, MF_STRING, IDC_KEYBOARD, szFor);
		}

		if (CProfile::IsVistaOrLater()) {
			popUp->DeleteMenu(IDC_KEYBOARD_ANY_USER, MF_BYCOMMAND);
		}
	} else {
		popUp->SetDefaultItem(IDC_OPTIONS);

		popUp->DeleteMenu(IDC_PROPERTIES, MF_BYCOMMAND);
		popUp->DeleteMenu(IDC_IMPORT, MF_BYCOMMAND);
		popUp->DeleteMenu(IDC_EXPORT, MF_BYCOMMAND);
		popUp->DeleteMenu(IDC_KEYBOARD, MF_BYCOMMAND);
		popUp->DeleteMenu(IDC_KEYBOARD_ANY_USER, MF_BYCOMMAND);
		popUp->DeleteMenu(IDC_DISABLE, MF_BYCOMMAND);
		popUp->DeleteMenu(IDC_RESET, MF_BYCOMMAND);
		popUp->DeleteMenu(IDC_QUIT, MF_BYCOMMAND);
		popUp->DeleteMenu(0, MF_BYPOSITION);	// Delete separator
		popUp->DeleteMenu(1, MF_BYPOSITION);	// Delete separator
		popUp->DeleteMenu(2, MF_BYPOSITION);	// Delete popup menu of Keyboard Layout
	}

	SetForegroundWindow();

	CPoint pt;
	GetCursorPos(&pt);
	popUp->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
}

// get message of icon on system tray
LRESULT CMainFrame::WindowProc(const UINT message, const WPARAM wParam, const LPARAM lParam) 
{
	static UINT s_uTaskbarRestart;

	switch (message) {
	case WM_CREATE:
		s_uTaskbarRestart = RegisterWindowMessage(_T("TaskbarCreated"));
		break;
	case WM_USER_NTFYICON:
		switch (lParam) {
		case WM_LBUTTONUP:
			SetForegroundSomeDialog();
			break;
		case WM_LBUTTONDBLCLK:
			if (wParam == MAIN_ICON) {
				PopUpPropertiesDialog();
			} else {
				PopUpOptionsDialog();
			}
			break;
		case WM_RBUTTONUP:
			ShowRightMenu((ICON_TYPE)wParam);
			break;
		default:
			break;
		}
	default:
		if (message == s_uTaskbarRestart) {
			m_pXkeymacsDll->AddAllShell_NotifyIcon();
		}
		break;
	}
	
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnProperties()
{
	PopUpPropertiesDialog();
}

void CMainFrame::PopUpOptionsDialog()
{
	PopUpDialog(m_pOptionsDlg, &m_nResultOptionsDlg, &m_bOptionsDlgExist);
}

void CMainFrame::OnOptions() 
{
	PopUpOptionsDialog();
}

// stop/start keyboard hook
void CMainFrame::OnDisable() 
{
	if (m_pXkeymacsDll->IsKeyboardHook()) {
		m_pXkeymacsDll->SetKeyboardHookFlag(FALSE);
	} else {
		m_pXkeymacsDll->SetKeyboardHookFlag(TRUE);
	}
}

// Close the Dialog if it is opened.
void CMainFrame::CloseDialog(CDialog *const pDialog, int *const pnResult)
{
	if (*pnResult != -1) {
		pDialog->EndDialog(*pnResult);
		*pnResult = -1;
	}
}

void CMainFrame::OnQuit() 
{
	CloseDialog(m_pAboutDlg, &m_nResultAboutDlg);
	CloseDialog(m_pPropertiesDlg, &m_nResultPropertiesDlg);
	CloseDialog(m_pOptionsDlg, &m_nResultOptionsDlg);
	for (int i = 0; i < MAX_HKEY_TYPE; ++i) {
		CloseDialog(m_p104KeyboardDlg[i], &m_nResultKeyboardDlg[ENGLISH_KEYBOARD][i]);
		CloseDialog(m_p109KeyboardDlg[i], &m_nResultKeyboardDlg[JAPANESE_KEYBOARD][i]);
	}

	m_pXkeymacsDll->ReleaseHooks();
	m_pXkeymacsDll->DeleteAllShell_NotifyIcon();

	PostQuitMessage(0);
}

void CMainFrame::PopUpAboutDialog()
{
	PopUpDialog(m_pAboutDlg, &m_nResultAboutDlg, &m_bAboutDlgExist);
}

void CMainFrame::OnAbout() 
{
	PopUpAboutDialog();
}

void CMainFrame::OnKeyboard() 
{
	PopUpKeyboardDialog(CURRENT_USER);
}

void CMainFrame::OnKeyboardAnyUser() 
{
	PopUpKeyboardDialog(LOCAL_MACHINE);
}

void CMainFrame::PopUpKeyboardDialog(const HKEY_TYPE hkeyType)
{
	if (CProfile::Is106Keyboard()) {
		PopUpDialog(m_p109KeyboardDlg[hkeyType], &m_nResultKeyboardDlg[JAPANESE_KEYBOARD][hkeyType], &m_bKeyboardDlgExist[JAPANESE_KEYBOARD][hkeyType]);
	} else {
		PopUpDialog(m_p104KeyboardDlg[hkeyType], &m_nResultKeyboardDlg[ENGLISH_KEYBOARD][hkeyType], &m_bKeyboardDlgExist[ENGLISH_KEYBOARD][hkeyType]);
	}
}

void CMainFrame::OnImport() 
{
	CProfile::ImportProperties();
	CProfile::InitDllData();
}

void CMainFrame::OnExport() 
{
	CProfile::ExportProperties();
}

void CMainFrame::OnReset() 
{
	m_pXkeymacsDll->ReleaseHooks();
	m_pXkeymacsDll->SetHooks();
}

void CMainFrame::OnHelpFinder() 
{
	TCHAR szPath[MAX_PATH] = {'\0'};
	const TCHAR szExt[] = _T("txt");
	_tmakepath(szPath, NULL, NULL, CString(MAKEINTRESOURCE(IDS_README)), szExt);

	TCHAR szModuleFileName[MAX_PATH] = {'\0'};
	if (GetModuleFileName(NULL, szModuleFileName, sizeof(szModuleFileName))) {
		TCHAR szDrive[_MAX_DRIVE] = {'\0'};
		TCHAR szDir[_MAX_DIR] = {'\0'};
		_tsplitpath(szModuleFileName, szDrive, szDir, NULL, NULL);
		_tmakepath(szPath, szDrive, szDir, CString(MAKEINTRESOURCE(IDS_README)), szExt);
	}

	ShellExecute(NULL, NULL, szPath, NULL, NULL, SW_SHOWNORMAL);
}
