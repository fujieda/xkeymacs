// MainFrm.cpp : implementation of the CMainFrame class
//

#include "mainfrm.h"
#include "xkeymacs.h"
#include "profile.h"
#include "../xkeymacsdll/xkeymacsdll.h"
#include "../xkeymacsdll/Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_USER_NTFYICON (WM_USER+100)

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

BOOL CMainFrame::m_bIcon[MAX_ICON_TYPE];
HICON CMainFrame::m_hIcon[MAX_ICON_TYPE][MAX_STATUS];
DWORD CMainFrame::m_dwOldMessage[MAX_ICON_TYPE];
NOTIFYICONDATA CMainFrame::m_stNtfyIcon[MAX_ICON_TYPE];
NOTIFYICONDATA CMainFrame::m_stOldNtfyIcon[MAX_ICON_TYPE];

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
	ON_MESSAGE(WM_POWERBROADCAST, OnPowerBroadCast)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
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
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// set notify icon data
	m_hIcon[MAIN_ICON][STATUS_ENABLE] = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, LR_SHARED),
	m_hIcon[MAIN_ICON][STATUS_DISABLE_TMP] = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAIN_DISABLE_TMP), IMAGE_ICON, 16, 16, LR_SHARED);
	m_hIcon[MAIN_ICON][STATUS_DISABLE_WOCQ] = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAIN_DISABLE_WOCQ), IMAGE_ICON, 16, 16, LR_SHARED);
	m_hIcon[MAIN_ICON][STATUS_DISABLE] = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAIN_DISABLE), IMAGE_ICON, 16, 16, LR_SHARED);
	m_bIcon[MAIN_ICON] = AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_MAIN)), 1);
	NOTIFYICONDATA main = { sizeof(NOTIFYICONDATA), m_hWnd, MAIN_ICON, NIF_MESSAGE | NIF_ICON | NIF_TIP, WM_USER_NTFYICON,
		  (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, LR_SHARED) };
	LoadString(AfxGetInstanceHandle(), AFX_IDS_APP_TITLE, main.szTip, _countof(main.szTip));
	m_stNtfyIcon[MAIN_ICON] = main;
	AddShell_NotifyIcon(MAIN_ICON);

	// set notify ^X icon data
	SetIconData(CX_ICON, IDS_TIP_CX, IDI_CX_ON, IDI_CX_OFF, IDS_REG_ENTRY_CX);
	// set notify M-x icon data
	SetIconData(MX_ICON, IDS_TIP_MX, IDI_MX_ON, IDI_MX_OFF, IDS_REG_ENTRY_MX);
	// set notify Meta icon data
	SetIconData(META_ICON, IDS_TIP_META, IDI_META_ON, IDI_META_OFF, IDS_REG_ENTRY_META);
	// set notify Shift icon data
	SetIconData(SHIFT_ICON, IDS_TIP_SHIFT, IDI_SHIFT_ON, IDI_SHIFT_OFF, IDS_REG_ENTRY_SHIFT);
	// set notify Ctrl icon data
	SetIconData(CTRL_ICON, IDS_TIP_CTRL, IDI_CTRL_ON, IDI_CTRL_OFF, IDS_REG_ENTRY_CTRL);
	// set notify Alt icon data
	SetIconData(ALT_ICON, IDS_TIP_ALT, IDI_ALT_ON, IDI_ALT_OFF, IDS_REG_ENTRY_ALT);
	
	CXkeymacsDll::SetKeyboardSpeed(CProfile::GetKeyboardSpeed());
	CXkeymacsDll::SetAccelerate(AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_ACCELERATE)), 1));

	CXkeymacsDll::SetCursorData((HCURSOR)::LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
								  (HCURSOR)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_DISABLE_TMP_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
								  (HCURSOR)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_DISABLE_WOCQ_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
								  (HCURSOR)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_DISABLE_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
								  AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(IDS_REG_ENTRY_CHANGE_CURSOR)), 0));

	CXkeymacsDll::SetHooks();
	StartPollThread();

	return 0;
}

void CMainFrame::StartPollThread()
{
	m_hThread = CreateThread(NULL, 0, PollMessage, this, 0, NULL);
}

void CMainFrame::TerminatePollThread()
{
	DWORD ack, read;
	IPC32Message msg;
	msg.Type = IPC32_TERMINATE;
	CallNamedPipe(XKEYMACS32_PIPE, &msg, sizeof(msg.Type), &ack, sizeof(DWORD), &read, NMPWAIT_NOWAIT);
	WaitForSingleObject(m_hThread, 5000);
	CloseHandle(m_hThread);
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

DWORD WINAPI CMainFrame::PollMessage(LPVOID)
{
	HANDLE pipe = CreateNamedPipe(XKEYMACS32_PIPE, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 1,
									sizeof(DWORD), sizeof(IPC32Message), 0, NULL);
	if (pipe == INVALID_HANDLE_VALUE) {
#ifdef DEBUG_IPC
		CUtils::Log(_T("PollMessage: CreateNamedPipe failed. (%d)"), GetLastError());
#endif
		return 1;
	}
	for (; ;) {
		if (ConnectNamedPipe(pipe, NULL) ? FALSE : (GetLastError() != ERROR_PIPE_CONNECTED)) {
#ifdef DEBUG_IPC
			CUtils::Log(_T("PollMessage: ConnectNamedPipe failed. (%d)"), GetLastError());
#endif
			break;
		}
		IPC32Message msg;
		DWORD read;
		if (!ReadFile(pipe, &msg, sizeof(msg), &read, NULL)) {
#ifdef DEBUG_IPC
			CUtils::Log(_T("PollMessage: ReadFile failed. (%d)"), GetLastError());
#endif
			break;
		}
		switch (msg.Type) {
		case IPC32_TERMINATE:
			SendAck(pipe);
			goto exit;
		case IPC32_HOOKSTATE:
			CXkeymacsDll::SetHookStateDirect(msg.Enable);
			CXkeymacsApp::SendIPC64Message(msg.Enable ? IPC64_ENABLE : IPC64_DISABLE);
			if (!SendAck(pipe))
				goto exit;
			continue;
		case IPC32_ICON:
			// return ack ASAP to release hooks from blocking state.
			if (!SendAck(pipe))
				goto exit;
			for (DWORD i = 0; i < (read - offsetof(IPC32Message, IconState)) / sizeof(IconState); ++i) {
				m_stNtfyIcon[msg.IconState[i].Type].hIcon = m_hIcon[msg.IconState[i].Type][msg.IconState[i].State];
				if (msg.IconState[i].Type == MX_ICON && msg.IconState[i].Tip[0] != 0)
					memcpy(m_stNtfyIcon[MX_ICON].szTip, msg.IconState[i].Tip, 128);
				DoShell_NotifyIcon(msg.IconState[i].Type, NIM_MODIFY);
			}
		}
	}
exit:
	CloseHandle(pipe);
	return 0;
}

void CMainFrame::SetIconData(ICON_TYPE icon, int tip, int on, int off, int reg)
{
	m_hIcon[icon][ON_ICON] = AfxGetApp()->LoadIcon(on);
	m_hIcon[icon][OFF_ICON] = AfxGetApp()->LoadIcon(off);
	m_bIcon[icon] = AfxGetApp()->GetProfileInt(CString(), CString(MAKEINTRESOURCE(reg)), 0);
	NOTIFYICONDATA data = { sizeof(NOTIFYICONDATA), m_hWnd, icon, NIF_MESSAGE | NIF_ICON | NIF_TIP, WM_USER_NTFYICON, AfxGetApp()->LoadIcon(off) };
	LoadString(AfxGetInstanceHandle(), tip, data.szTip, _countof(data.szTip));
	m_stNtfyIcon[icon] = data;
	AddShell_NotifyIcon(icon);
}

void CMainFrame::DoShell_NotifyIcon(ICON_TYPE icon, DWORD dwMessage)
{
	if (!m_bIcon[icon] ||
			(m_dwOldMessage[icon] == dwMessage &&
			!memcmp(&m_stOldNtfyIcon[icon], &m_stNtfyIcon[icon], sizeof(m_stNtfyIcon[icon]))))
		return;
	m_dwOldMessage[icon] = dwMessage;
	m_stOldNtfyIcon[icon] = m_stNtfyIcon[icon];
	for (int retry_count = 0; retry_count < 20; ++retry_count) { // retry for timeout
		if (Shell_NotifyIcon(dwMessage, &m_stNtfyIcon[icon]))
			return;
		if (dwMessage != NIM_ADD || (GetLastError() != ERROR_TIMEOUT && 5 < retry_count))
			return;
		Sleep(1000); // 1sec
		if (Shell_NotifyIcon(NIM_MODIFY, &m_stNtfyIcon[icon]))
			return; // ERROR_TIMEOUT was returned but the icon was also added.
	}
}

void CMainFrame::EnableShell_NotifyIcon(ICON_TYPE icon, BOOL bEnable)
{
	DeleteShell_NotifyIcon(icon);
	m_bIcon[icon] = bEnable;
	AddShell_NotifyIcon(icon);
}

void CMainFrame::AddShell_NotifyIcon(ICON_TYPE icon)
{
	DoShell_NotifyIcon(icon, NIM_ADD);
}

void CMainFrame::DeleteShell_NotifyIcon(ICON_TYPE icon)
{
	DoShell_NotifyIcon(icon, NIM_DELETE);
}

void CMainFrame::DeleteAllShell_NotifyIcon()
{
	for (int icon = 0; icon < MAX_ICON_TYPE; ++icon) {
		DeleteShell_NotifyIcon((ICON_TYPE)icon);
	}
}

void CMainFrame::AddAllShell_NotifyIcon()
{
	for (int icon = 0; icon < MAX_ICON_TYPE; ++icon) {
		AddShell_NotifyIcon((ICON_TYPE)icon);
	}
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
		if (CXkeymacsDll::GetHookState()) {
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
			AddAllShell_NotifyIcon();
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
	CXkeymacsDll::ToggleHookState();
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

	CXkeymacsDll::ReleaseHooks();
	TerminatePollThread();
	CXkeymacsApp::Terminate64bitProcess();
	DeleteAllShell_NotifyIcon();

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
	CProfile::LoadData();
	CProfile::SetDllData();
}

void CMainFrame::OnExport() 
{
	CProfile::ExportProperties();
}

void CMainFrame::OnReset() 
{
	CXkeymacsDll::ResetHooks();
	TerminatePollThread();
	StartPollThread();
	CXkeymacsApp::Terminate64bitProcess();
	CXkeymacsApp::Start64bitProcess();
	CProfile::SetDllData();
}

void CMainFrame::OnHelpFinder() 
{
	TCHAR szPath[MAX_PATH] = {'\0'};
	const TCHAR szExt[] = _T("txt");
	_tmakepath_s(szPath, NULL, NULL, CString(MAKEINTRESOURCE(IDS_README)), szExt);

	TCHAR szModuleFileName[MAX_PATH] = {'\0'};
	if (GetModuleFileName(NULL, szModuleFileName, sizeof(szModuleFileName))) {
		TCHAR szDrive[_MAX_DRIVE] = {'\0'};
		TCHAR szDir[_MAX_DIR] = {'\0'};
		_tsplitpath_s(szModuleFileName, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, NULL, 0, NULL, 0);
		_tmakepath_s(szPath, szDrive, szDir, CString(MAKEINTRESOURCE(IDS_README)), szExt);
	}

	ShellExecute(NULL, NULL, szPath, NULL, NULL, SW_SHOWNORMAL);
}

LRESULT CMainFrame::OnPowerBroadCast(WPARAM wParam, LPARAM)
{
	switch (wParam) {
	case PBT_APMRESUMECRITICAL:
	case PBT_APMRESUMESUSPEND:
	case PBT_APMRESUMEAUTOMATIC:
		CXkeymacsDll::ResetHooks();
	}
	return TRUE;
}
