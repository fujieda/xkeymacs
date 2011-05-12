// xkeymacsdll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "resource.h"
#include "Utils.h"
#include "Commands.h"
#include <afxdllx.h>
#include <math.h>
#include <Imm.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE XkeymacsdllDLL = { NULL, NULL };

static HINSTANCE g_hDllInst = NULL;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	g_hDllInst = hInstance;
	
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH) {
		TRACE0("XKEYMACSDLL.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(XkeymacsdllDLL, hInstance)) {
			return 0;
		}

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		try {
			new CDynLinkLibrary(XkeymacsdllDLL);
		}
		catch (CMemoryException* e) {
			e->Delete();
//			CUtils::Log("DllMain: 'new' threw an exception");
		}
	} else if (dwReason == DLL_PROCESS_DETACH) {
		TRACE0("XKEYMACSDLL.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(XkeymacsdllDLL);
	}
	return 1;   // ok
}

//////////////////////////////////////////////////////////////////////
// CXkeymacsDll Class
//////////////////////////////////////////////////////////////////////

#include "xkeymacsDll.h"
#pragma data_seg(".xkmcs")
	HHOOK	CXkeymacsDll::m_hHookKeyboard = NULL;
	HHOOK	CXkeymacsDll::m_hHookCallWnd = NULL;
	HHOOK	CXkeymacsDll::m_hHookCallWndRet = NULL;
	HHOOK	CXkeymacsDll::m_hHookGetMessage = NULL;
	HHOOK	CXkeymacsDll::m_hHookShell = NULL;
	int		CXkeymacsDll::m_nCommandID[MAX_APP][MAX_COMMAND_TYPE][MAX_KEY] = {'\0'};
	BOOL	CXkeymacsDll::m_bAtIbeamCursorOnly[MAX_APP][MAX_COMMAND_TYPE][MAX_KEY] = {'\0'};
	int		CXkeymacsDll::m_nFunctionID[MAX_APP][MAX_COMMAND_TYPE][MAX_KEY] = {'\0'};
	TCHAR	CXkeymacsDll::m_szSpecialApp[MAX_APP][CLASS_NAME_LENGTH] = {'\0'};
	TCHAR	CXkeymacsDll::m_szWindowText[MAX_APP][WINDOW_TEXT_LENGTH] = {'\0'};
	BOOL	CXkeymacsDll::m_bRightControl	= FALSE;
	BOOL	CXkeymacsDll::m_bRightAlt		= FALSE;
	BOOL	CXkeymacsDll::m_bRightShift		= FALSE;
	BOOL	CXkeymacsDll::m_bHook			= TRUE;
	BOOL	CXkeymacsDll::m_bDefiningMacro	= FALSE;
	DWORD	CXkeymacsDll::m_dwOldMessage[MAX_ICON_TYPE] = {'\0'};
	NOTIFYICONDATA CXkeymacsDll::m_stNtfyIcon[MAX_ICON_TYPE] = {'\0'};
	NOTIFYICONDATA CXkeymacsDll::m_stOldNtfyIcon[MAX_ICON_TYPE] = {'\0'};
	HICON	CXkeymacsDll::m_hIcon[MAX_ICON_TYPE][MAX_STATUS] = {'\0'};
	BOOL	CXkeymacsDll::m_bIcon[MAX_ICON_TYPE] = {'\0'};
	int		CXkeymacsDll::m_nKillRingMax[MAX_APP] = {'\0'};
	BOOL	CXkeymacsDll::m_bUseDialogSetting[MAX_APP] = {'\0'};
	CList<CClipboardSnap *, CClipboardSnap *> CXkeymacsDll::m_oKillRing;
	CObList CXkeymacsDll::m_Macro;
	int		CXkeymacsDll::m_nKillRing = 0;
	int		CXkeymacsDll::m_nOriginal[MAX_COMMAND_TYPE][MAX_KEY] = {'\0'};
	int		CXkeymacsDll::m_nApplicationID = 0;
	int		CXkeymacsDll::m_nSettingStyle[MAX_APP] = {'\0'};
	BOOL	CXkeymacsDll::m_bIgnoreUndefinedMetaCtrl[MAX_APP] = {'\0'};
	BOOL	CXkeymacsDll::m_bIgnoreUndefinedC_x[MAX_APP] = {'\0'};
//	int		CXkeymacsDll::m_nPassThrough = 0;
	TCHAR	CXkeymacsDll::m_szApplicationName[MAX_PATH] = {'\0'};
	TCHAR	CXkeymacsDll::m_szOldApplicationName[MAX_PATH] = {'\0'};
	BOOL	CXkeymacsDll::m_bEnableCUA[MAX_APP] = {'\0'};
	TCHAR	CXkeymacsDll::m_szFunctionDefinition[MAX_FUNCTION][MAX_DEFINITION] = {'\0'};
	int		CXkeymacsDll::m_nAccelerate = 0;
	int		CXkeymacsDll::m_nKeyboardSpeed = 31;
	HCURSOR	CXkeymacsDll::m_hCursor[MAX_STATUS] = {'\0'};
	HCURSOR CXkeymacsDll::m_hCurrentCursor = NULL;
	BOOL	CXkeymacsDll::m_bCursor = FALSE;
	BOOL	CXkeymacsDll::m_b326Compatible[MAX_APP] = {'\0'};

	BOOL	CXkeymacsData::m_b106Keyboard = FALSE;
#pragma data_seg()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXkeymacsDll::CXkeymacsDll()
{
	CUtils::InitCUtils();
}

CXkeymacsDll::~CXkeymacsDll()
{

}

int CXkeymacsDll::ModifyShell_NotifyIcon(ICON_TYPE icon, BOOL bNewStatus, BOOL bForce)
{
	if (m_stNtfyIcon[icon].hIcon == (bNewStatus ? m_hIcon[icon][ON_ICON] : m_hIcon[icon][OFF_ICON])) {
		if (!bForce) {
			return TRUE;
		}
	} else {
		m_stNtfyIcon[icon].hIcon = (bNewStatus ? m_hIcon[icon][ON_ICON] : m_hIcon[icon][OFF_ICON]);
	}
	return DoShell_NotifyIcon(icon, NIM_MODIFY);
}

void CXkeymacsDll::SetNotifyIconData(ICON_TYPE icon, NOTIFYICONDATA stNtfyIcon, HICON hEnable, HICON hDisableTMP, HICON hDisableWOCQ, HICON hDisable, BOOL bEnable)
{
	m_hIcon[icon][STATUS_ENABLE] = hEnable;
	m_hIcon[icon][STATUS_DISABLE_TMP] = hDisableTMP;
	m_hIcon[icon][STATUS_DISABLE_WOCQ] = hDisableWOCQ;
	m_hIcon[icon][STATUS_DISABLE] = hDisable;
	m_bIcon[icon] = bEnable;
	m_stNtfyIcon[icon] = stNtfyIcon;
	AddShell_NotifyIcon(icon);
}

void CXkeymacsDll::SetNotifyIconData(ICON_TYPE icon, NOTIFYICONDATA stNtfyIcon, HICON hOn, HICON hOff, BOOL bEnable)
{
	m_hIcon[icon][ON_ICON] = hOn;
	m_hIcon[icon][OFF_ICON] = hOff;
	m_bIcon[icon] = bEnable;
	m_stNtfyIcon[icon] = stNtfyIcon;
	AddShell_NotifyIcon(icon);
}

void CXkeymacsDll::EnableShell_NotifyIcon(ICON_TYPE icon, BOOL bEnable)
{
	DeleteShell_NotifyIcon(icon);
	m_bIcon[icon] = bEnable;
	AddShell_NotifyIcon(icon);
}


BOOL CXkeymacsDll::DoShell_NotifyIcon(ICON_TYPE icon, DWORD dwMessage)
{
	if (m_bIcon[icon]
	 &&	(m_dwOldMessage[icon] != dwMessage
	  || memcmp(&m_stOldNtfyIcon[icon], &m_stNtfyIcon[icon], sizeof(m_stNtfyIcon[icon])))) {
		m_dwOldMessage[icon] = dwMessage;
		m_stOldNtfyIcon[icon] = m_stNtfyIcon[icon];

		BOOL rc = FALSE;
		for (int retry_count = 0; retry_count < 20; ++retry_count) { // retry for timeout
			rc = Shell_NotifyIcon(dwMessage, &m_stNtfyIcon[icon]);
			if (dwMessage != NIM_ADD || rc || (GetLastError() != ERROR_TIMEOUT && 5 < retry_count)) {
				break;
			}
			Sleep(1000); // 1sec
			if ((rc = Shell_NotifyIcon(NIM_MODIFY, &m_stNtfyIcon[icon])) != FALSE) {
				break; // ERROR_TIMEOUT was returned but the icon was also added.
			}
		}
		return rc;
	} else {
		return TRUE;
	}
}

void CXkeymacsDll::AddShell_NotifyIcon(ICON_TYPE icon)
{
	DoShell_NotifyIcon(icon, NIM_ADD);
}

void CXkeymacsDll::DeleteShell_NotifyIcon(ICON_TYPE icon)
{
	DoShell_NotifyIcon(icon, NIM_DELETE);
}

// set keyboard hook
BOOL CXkeymacsDll::SetKeyboardHook()
{
	m_hHookKeyboard = ::SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, g_hDllInst, 0);
	if (!m_hHookKeyboard) {
		return FALSE;
	}

	m_hHookCallWnd = ::SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)CallWndProc, g_hDllInst, 0);

	m_hHookCallWndRet = ::SetWindowsHookEx(WH_CALLWNDPROCRET, (HOOKPROC)CallWndRetProc, g_hDllInst, 0);

	m_hHookGetMessage = ::SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)GetMsgProc, g_hDllInst, 0);

	m_hHookShell = ::SetWindowsHookEx(WH_SHELL, (HOOKPROC)ShellProc, g_hDllInst, 0);

	AddShell_NotifyIcon(MAIN_ICON);

	return TRUE;
}

// release keyboard hook
BOOL CXkeymacsDll::ReleaseKeyboardHook()
{
	BOOL bKeyboard = TRUE;
	BOOL bCallWnd = TRUE;
	BOOL bCallWndRet = TRUE;
	BOOL bGetMessage = TRUE;
	BOOL bShell = TRUE;

	if (m_hHookKeyboard) {
		bKeyboard = ::UnhookWindowsHookEx(m_hHookKeyboard);
	}
	m_hHookKeyboard = NULL;

	if (m_hHookCallWnd) {
		bCallWnd = ::UnhookWindowsHookEx(m_hHookCallWnd);
	}
	m_hHookCallWnd = NULL;

	if (m_hHookCallWndRet) {
		bCallWndRet = ::UnhookWindowsHookEx(m_hHookCallWndRet);
	}
	m_hHookCallWndRet = NULL;

	if (m_hHookGetMessage) {
		bGetMessage = ::UnhookWindowsHookEx(m_hHookGetMessage);
	}
	m_hHookGetMessage = NULL;

	if (m_hHookShell) {
		bShell = ::UnhookWindowsHookEx(m_hHookShell);
	}
	m_hHookShell = NULL;

	return bKeyboard && bCallWnd && bCallWndRet && bGetMessage && bShell;
}

void CXkeymacsDll::SetKeyboardHookFlag(BOOL bFlag)
{
	m_bHook = bFlag;

	if (m_bHook) {
		if (CCommands::IsTemporarilyDisableXKeymacs()) {
			m_stNtfyIcon[MAIN_ICON].hIcon = m_hIcon[MAIN_ICON][STATUS_DISABLE_TMP];
			m_hCurrentCursor = m_hCursor[STATUS_DISABLE_TMP];
		} else {
			m_stNtfyIcon[MAIN_ICON].hIcon = m_hIcon[MAIN_ICON][STATUS_ENABLE];
			m_hCurrentCursor = m_hCursor[STATUS_ENABLE];
		}
	} else {
		m_stNtfyIcon[MAIN_ICON].hIcon = m_hIcon[MAIN_ICON][STATUS_DISABLE_WOCQ];
		m_hCurrentCursor = m_hCursor[STATUS_DISABLE_WOCQ];
	}
	if (m_nSettingStyle[m_nApplicationID] == SETTING_DISABLE
	 || (!_tcsicmp(m_szSpecialApp[m_nApplicationID], _T("Default"))
	  && CUtils::IsDefaultIgnoreApplication())) {
		m_stNtfyIcon[MAIN_ICON].hIcon = m_hIcon[MAIN_ICON][STATUS_DISABLE];
		m_hCurrentCursor = m_hCursor[STATUS_DISABLE];
	}
	DoShell_NotifyIcon(MAIN_ICON, NIM_MODIFY);
	DoSetCursor();
}

// if be keyboard hook, return TRUE
BOOL CXkeymacsDll::IsKeyboardHook()
{
	if (m_bHook) {
		return TRUE;
	}

	return FALSE;
}

void CXkeymacsDll::LogCallWndProcMessage(WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT &cwps = *(CWPSTRUCT *)lParam;

	switch (cwps.message) {
	case WM_PAINT:					// 0x000F
	case WM_MDIGETACTIVE:			// 0x0229
	case 0x0403:
	case 0x0407:
	case 0x0418:
	case 0x043F:
	case 0x0440:
		break;
	case WM_CREATE:					// 0x0001
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_CREATE"));
		break;
	case WM_DESTROY:				// 0x0002
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_DESTROY"));
		break;
	case WM_MOVE:					// 0x0003
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_MOVE");)
		break;
	case WM_SIZE:					// 0x0005
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_SIZE"));
		break;
	case WM_GETTEXT:				// 0x000D
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_GETTEXT"));
		break;
	case WM_ERASEBKGND:				// 0x0014
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_ERASEBKGND"));
		break;
	case WM_WINDOWPOSCHANGING:		// 0x0046
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_WINDOWPOSCHANGING"));
		break;
	case WM_WINDOWPOSCHANGED:		// 0x0047
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_WINDOWPOSCHANGED"));
		break;
	case WM_COPYDATA:				// 0x004A
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_COPYDATA"));
		break;
	case WM_NCCREATE:				// 0x0081
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_NCCREATE"));
		break;
	case WM_NCDESTROY:				// 0x0082
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_NCDESTROY"));
		break;
	case WM_NCCALCSIZE:				// 0x0083
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_NCCALCSIZE"));
		break;
	case WM_NCPAINT:				// 0x0085
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_NCPAINT"));
		break;
	case WM_IME_STARTCOMPOSITION:	// 0x010D
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_IME_STARTCOMPOSITION"));
		break;
	case WM_IME_ENDCOMPOSITION:		// 0x010E
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_IME_ENDCOMPOSITION"));
		break;
	case WM_IME_KEYLAST:			// 0x010F
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_IME_KEYLAST"));
		break;
	case WM_COMMAND:				// 0x0111
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_COMMAND"));
		break;
	case WM_CTLCOLOREDIT:			// 0x0133
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_CTLCOLOREDIT"));
		break;
	case WM_POWERBROADCAST:			// 0x0218
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_POWERBROADCAST"));
		switch (wParam) {
		case PBT_APMQUERYSUSPEND:		// 0x0000
//			CUtils::Log(_T("PBT_APMQUERYSUSPEND"));
			break;
		case PBT_APMQUERYSTANDBY:		// 0x0001
//			CUtils::Log(_T("PBT_APMQUERYSTANDBY"));
			break;
		case PBT_APMQUERYSUSPENDFAILED:	// 0x0002
//			CUtils::Log(_T("PBT_APMQUERYSUSPENDFAILED"));
			break;
		case PBT_APMQUERYSTANDBYFAILED:	// 0x0003
//			CUtils::Log(_T("PBT_APMQUERYSTANDBYFAILED"));
			break;
		case PBT_APMSUSPEND:			// 0x0004
//			CUtils::Log(_T("PBT_APMSUSPEND"));
			break;
		case PBT_APMSTANDBY:			// 0x0005
//			CUtils::Log(_T("PBT_APMSTANDBY"));
			break;
		case PBT_APMRESUMECRITICAL:		// 0x0006
//			CUtils::Log(_T("PBT_APMRESUMECRITICAL"));
			break;
		case PBT_APMRESUMESUSPEND:		// 0x0007
//			CUtils::Log(_T("PBT_APMRESUMESUSPEND"));
			break;
		case PBT_APMRESUMESTANDBY:		// 0x0008
//			CUtils::Log(_T("PBT_APMRESUMESTANDBY"));
			break;
		case PBT_APMBATTERYLOW:			// 0x0009
//			CUtils::Log(_T("PBT_APMBATTERYLOW"));
			break;
		case PBT_APMPOWERSTATUSCHANGE:	// 0x000A
//			CUtils::Log(_T("PBT_APMPOWERSTATUSCHANGE"));
			break;
		case PBT_APMOEMEVENT:			// 0x000B
//			CUtils::Log(_T("PBT_APMOEMEVENT"));
			break;
		case PBT_APMRESUMEAUTOMATIC:	// 0x0012
//			CUtils::Log(_T("PBT_APMRESUMEAUTOMATIC"));
			break;
		default:
//			CUtils::Log(_T("PBT_OTHERS: %d"), wParam);
			break;
		}
		break;
	case WM_IME_NOTIFY:				// 0x0282
//		CUtils::Log(_T("CallWndProc: cwps.message = WM_IME_NOTIFY"));
		break;
	default:
//		CUtils::Log(_T("CallWndProc: cwps.message = 0x%04X"), cwps.message);
		break;
	}
}

LRESULT CALLBACK CXkeymacsDll::CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
//	LogCallWndProcMessage(wParam, lParam);

	if (0 <= nCode) {
		CWPSTRUCT &cwps = *(CWPSTRUCT *)lParam;
		switch (cwps.message) {
		case WM_IME_STARTCOMPOSITION:
			InitKeyboardProc(TRUE);
			break;
		case WM_IME_ENDCOMPOSITION:
			InitKeyboardProc(FALSE);
			break;
		case WM_SETFOCUS:
			if (cwps.hwnd == GetForegroundWindow()) {
				InitKeyboardProc(FALSE);
				SetKeyboardHookFlag(m_bHook);
			}
			break;
		case WM_NCACTIVATE:
			if (cwps.wParam) {
				if (cwps.hwnd == GetForegroundWindow()) {
					InitKeyboardProc(FALSE);
					SetKeyboardHookFlag(m_bHook);
				}
			}
			break;
		case WM_POWERBROADCAST:
			switch (wParam) {
			case PBT_APMRESUMECRITICAL: // 0x0006
			case PBT_APMRESUMESUSPEND:  // 0x0007
			case PBT_APMRESUMESTANDBY:  // 0x0008
				ReleaseKeyboardHook();
				SetKeyboardHook();
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	return CallNextHookEx(m_hHookCallWnd, nCode, wParam, lParam);
}

LRESULT CALLBACK CXkeymacsDll::CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (0 <= nCode) {
		CWPRETSTRUCT &cwprets = *(CWPRETSTRUCT *)lParam;
		switch (cwprets.message) {
		case WM_SETTEXT:
			if (cwprets.hwnd == GetForegroundWindow()) {
				InitKeyboardProc(FALSE);
			}
			break;
		case WM_SETCURSOR:
			DoSetCursor();
			break;
		default:
			break;
		}
	}
	return CallNextHookEx(m_hHookCallWndRet, nCode, wParam, lParam);
}

LRESULT CALLBACK CXkeymacsDll::GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	MSG &msg = (*(MSG *)lParam);

	switch (msg.message) {
	case WM_IME_STARTCOMPOSITION:
		InitKeyboardProc(TRUE);
		break;
	case WM_IME_ENDCOMPOSITION:
		InitKeyboardProc(FALSE);
		break;
	}
	return CallNextHookEx(m_hHookGetMessage, nCode, wParam, lParam);
}

LRESULT CALLBACK CXkeymacsDll::ShellProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	switch (nCode) {
	case HSHELL_WINDOWACTIVATED:
	{
		TCHAR className[256];
		::GetClassName((HWND)wParam, className, 255);
		if (!_tcsicmp(className, _T("ConsoleWindowClass"))) {
			InitKeyboardProc(FALSE);
			SetKeyboardHookFlag(m_bHook);
		}
		break;
	}
	default:
		break;
	}
	return CallNextHookEx( m_hHookShell, nCode, wParam, lParam );
}

// return true if the key is down
BOOL CXkeymacsDll::IsDown(BYTE bVk, BOOL bPhysicalKey)
{
	if (bPhysicalKey) {
		return GetKeyState(bVk) < 0;
	} else {
		return GetAsyncKeyState(bVk) < 0;
	}
}

// Do keybd_event
void CXkeymacsDll::DoKeybd_event(BYTE bVk, DWORD dwFlags)
{
	// Set KEYEVENTF_EXTENDEDKEY if needed
	switch (bVk) {
	case VK_CONTROL:
		if (m_bRightControl) {		// Right Ctrl
			dwFlags |= KEYEVENTF_EXTENDEDKEY;
		}
		break;

	case VK_MENU:
		if (m_bRightAlt) {			// Right Alt
			dwFlags |= KEYEVENTF_EXTENDEDKEY;
//			CUtils::Log("Right Alt %d", dwFlags);
//		} else {
//			CUtils::Log("Left Alt %d", dwFlags);
		}
		break;

	case VK_SHIFT:
		if (m_bRightShift) {		// Right Shift
			dwFlags |= KEYEVENTF_EXTENDEDKEY;
		}
		break;

	case VK_PAUSE:
		if (IsDown(VK_CONTROL)) {	// Break
			dwFlags |= KEYEVENTF_EXTENDEDKEY;
		}
		break;

	case VK_INSERT:
	case VK_DELETE:
	case VK_HOME:
	case VK_END:
	case VK_NEXT:
	case VK_PRIOR:
	case VK_UP:
	case VK_DOWN:
	case VK_RIGHT:
	case VK_LEFT:
	case VK_NUMLOCK:
	case VK_PRINT:
		dwFlags |= KEYEVENTF_EXTENDEDKEY;
		break;

	default:
		break;
	}

//	CUtils::Log(_T("b: %x, %x, %x, %#x, %#x"), bVk, dwFlags, GetMessageExtraInfo(), GetKeyState(bVk), GetAsyncKeyState(bVk));
	keybd_event(bVk, 0, dwFlags, GetMessageExtraInfo());
//	CUtils::Log(_T("a: %x, %x, %x, %#x, %#x"), bVk, dwFlags, GetMessageExtraInfo(), GetKeyState(bVk), GetAsyncKeyState(bVk));
}

// the key is being depressed
void CXkeymacsDll::DepressKey(BYTE bVk, BOOL bOriginal)	// bVk is virtual-key code, MSDN said
{
	if (bOriginal) {
//		CUtils::Log(_T("i: %x, %d, %d, %d, %d, %d, %d, %d, %d"), bVk,
//			IsDown(VK_CONTROL), IsDown(VK_CONTROL, FALSE), IsDepressedModifier(CCommands::C_), IsDepressedModifier(CCommands::C_, FALSE),
//			IsDown(VK_MENU), IsDown(VK_MENU, FALSE), IsDepressedModifier(CCommands::MetaAlt), IsDepressedModifier(CCommands::MetaAlt, FALSE));

		int nCommandType = NONE;
		if (IsDown(VK_CONTROL, FALSE)) {
			nCommandType |= CONTROL;
		}
		if (IsDown(VK_MENU, FALSE)) {
			nCommandType |= META;
		}
		Original(nCommandType, bVk, 1);
	}

	DoKeybd_event(bVk, 0);
}

// the key is being released
void CXkeymacsDll::ReleaseKey(BYTE bVk)	// bVk is virtual-key code, MSDN said
{
	DoKeybd_event(bVk, KEYEVENTF_KEYUP);
}

// bVk down, bVk up
void CXkeymacsDll::Kdu(BYTE bVk, DWORD n, BOOL bOriginal)
{
	while (n--) {
		DepressKey(bVk, bOriginal);
		ReleaseKey(bVk);
	}
}

void CXkeymacsDll::InitKeyboardProc(BOOL bImeComposition)
{
	if (CUtils::IsFindDialog()) {
		static BOOL bImeCompositionOld = FALSE;
		if (!bImeComposition
		 && bImeCompositionOld) {
			DepressKey(VK_END);
			ReleaseKey(VK_END);
		}
		bImeCompositionOld = bImeComposition;
	}

	CUtils::SetApplicationName(bImeComposition);

	if (_tcsnicmp(m_szSpecialApp[m_nApplicationID], CUtils::GetApplicationName(), 0xF) || !IsMatchWindowText(m_szWindowText[m_nApplicationID])) {	// PROCESSENTRY32 has only 0xF bytes of Name
		m_nApplicationID = -1;

		for (int nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {
			if (!_tcsnicmp(m_szSpecialApp[nApplicationID], CUtils::GetApplicationName(), 0xF) && IsMatchWindowText(m_szWindowText[nApplicationID])) {

				if (m_nApplicationID < 0
				 || CUtils::GetWindowTextType(m_szWindowText[m_nApplicationID]) < CUtils::GetWindowTextType(m_szWindowText[nApplicationID])
				 || CUtils::GetWindowTextType(m_szWindowText[m_nApplicationID]) == CUtils::GetWindowTextType(m_szWindowText[nApplicationID])
				 && _tcscmp(m_szWindowText[m_nApplicationID], m_szWindowText[nApplicationID]) <= 0) {
					m_nApplicationID = nApplicationID;
				}
			}
		}

		if (m_nApplicationID < 0) {
			for (int nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {
				if (!_tcsicmp(m_szSpecialApp[nApplicationID], _T("Default"))) {
					m_nApplicationID = nApplicationID;
					break;
				}
			}

			if (m_nApplicationID < 0) {
				m_nApplicationID = 0;
			}
		}
	}

	if (m_nSettingStyle[m_nApplicationID] != SETTING_DISABLE
	 && (_tcsicmp(m_szSpecialApp[m_nApplicationID], _T("Default")) || !CUtils::IsDefaultIgnoreApplication())
	 && !bImeComposition
	 && CUtils::IsDialog()) {
		// Use Dialog Setting
		if (m_bUseDialogSetting[m_nApplicationID]) {
			int nOriginalApplicationID = m_nApplicationID;
			for (m_nApplicationID = 0; m_nApplicationID < MAX_APP; ++m_nApplicationID) {
				if (!_tcsicmp(m_szSpecialApp[m_nApplicationID], _T("Dialog"))) {
					break;
				}
			}
			if (m_nApplicationID == MAX_APP) {
				m_nApplicationID = nOriginalApplicationID;
			}
		}
	}

	ModifyShell_NotifyIcon(CX_ICON, FALSE);
	ModifyShell_NotifyIcon(MX_ICON, FALSE);
	ModifyShell_NotifyIcon(META_ICON, FALSE);
	CCommands::SetMark(FALSE);
	CCommands::SetTemporarilyDisableXKeymacs(FALSE);
	CCommands::Reset();
}

// emulate emacs	// cf virtual-key code
LRESULT CALLBACK CXkeymacsDll::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	ASSERT(0 <= wParam && wParam <= UCHAR_MAX);

	int nCommandType = NONE;
	BYTE nKey = (BYTE)wParam;

	static BOOL bLocked = FALSE;
	static const BYTE RECURSIVE_KEY = 0x07;
	static int (*fCommand)() = NULL;
	static BYTE nOneShotModifier[MAX_KEY] = {'\0'};
	static BOOL bCherryOneShotModifier = FALSE;

//	CUtils::Log(_T("nCode = %#x, nKey = %#x, lParam = %#x"), nCode, nKey, lParam);

	if (nCode < 0 || nCode == HC_NOREMOVE) {
		goto DO_NOTHING;
	}

	if (nKey == RECURSIVE_KEY) {
		if (lParam & BEING_RELEASED) {
			goto HOOK_RECURSIVE_KEY;
		} else {
			goto RECURSIVE_COMMAND;
		}
	}

	{
		static BOOL bShift = FALSE;
		if (IsDepressedShiftKeyOnly(nKey)) {
			if (lParam & BEING_RELEASED) {
				if (bShift) {
					CCommands::SetMark(FALSE);
				}
			} else {
				bShift = TRUE;
			}
		} else {
			bShift = FALSE;
		}
	}

	if (CUtils::IsNT()) {
		switch (nKey) {
		case VK_CONTROL:
			if (lParam & EXTENDED_KEY) {
				nKey = VK_RCONTROL;
			} else {
				nKey = VK_LCONTROL;
			}
			break;
		case VK_MENU:
			if (lParam & EXTENDED_KEY) {
				nKey = VK_RMENU;
			} else {
				nKey = VK_LMENU;
			}
			break;
		case VK_SHIFT:
			if (lParam & EXTENDED_KEY) {
				nKey = VK_RSHIFT;
			} else {
				nKey = VK_LSHIFT;
			}
			break;
		default:
			break;
		}
	}

	if (lParam & BEING_RELEASED) {
		if (nKey == VK_MENU
		 || nKey == VK_LWIN
		 || nKey == VK_RWIN
		 || nKey == VK_APPS
		 || nKey == VK_LMENU
		 || nKey == VK_RMENU) {
			for (int i = 0; i < MAX_COMMAND_TYPE; ++i) {
				if (Commands[m_nCommandID[m_nApplicationID][i][nKey]].fCommand
				 && (Commands[m_nCommandID[m_nApplicationID][i][nKey]].fCommand != CCommands::MetaAlt
				  || nKey != VK_MENU && nKey != VK_LMENU && nKey != VK_RMENU)) {
					goto HOOK;
				}
			}
		}

		if (nOneShotModifier[nKey]) {
			ReleaseKey(nOneShotModifier[nKey]);
			nOneShotModifier[nKey] = 0;

			if (bCherryOneShotModifier) {
				bCherryOneShotModifier = FALSE;
				Kdu(nKey);
			}
		}

		goto DO_NOTHING;
	}

	if (m_nSettingStyle[m_nApplicationID] == SETTING_DISABLE) {
		goto DO_NOTHING;
	}

	// Do Nothing for Meadow, Mule for Win32, ... if those use default setting.
	if (!_tcsicmp(m_szSpecialApp[m_nApplicationID], _T("Default"))
	 && CUtils::IsDefaultIgnoreApplication()) {
		goto DO_NOTHING;
	}

	switch (IsPassThrough(nKey)) {
	case GOTO_DO_NOTHING:
		goto DO_NOTHING;
	case GOTO_HOOK:
		goto HOOK;
	case CONTINUE:
		break;
	default:
		ASSERT(0);
		break;
	}

	// set command type
	{
		nCommandType = NONE;
		if (IsDown(VK_SHIFT, FALSE)) {
			nCommandType |= SHIFT;
		}
		if (IsControl()) {
			nCommandType |= CONTROL;
		}
		if (IsMeta()) {
			nCommandType |= META;
		}
		if (CCommands::bC_x()) {
			nCommandType |= CONTROLX;
		}

		// Ignore undefined C-x ?
		if (nCommandType & CONTROLX) {
			if (Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == NULL
			 && m_nFunctionID[m_nApplicationID][nCommandType][nKey] < 0) {
				if (m_bIgnoreUndefinedC_x[m_nApplicationID]) {
					CCommands::Reset(GOTO_HOOK);
					goto HOOK;
				}
				nCommandType &= ~CONTROLX;
			}
		}

		// Ignore undefined Meta Ctrl+?
		if (CCommands::bM_() && (nCommandType & CONTROL)) {
			if (Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == NULL
			 && m_nFunctionID[m_nApplicationID][nCommandType][nKey] < 0) {
				if (m_bIgnoreUndefinedMetaCtrl[m_nApplicationID]) {
					if (Original(CONTROL, nKey)) {
						Original(CONTROL, nKey, -1);
						goto DO_NOTHING;
					}
					CCommands::Reset(GOTO_HOOK);
					goto HOOK;
				}
				nCommandType &= ~META;
			}
		}
	}

	{
//		CUtils::Log(_T("o: %x, %d, %d, %d, %d, %d, %d, %d, %d"), (BYTE)wParam,
//			IsDown(VK_CONTROL), IsDown(VK_CONTROL, FALSE), IsDepressedModifier(CCommands::C_), IsDepressedModifier(CCommands::C_, FALSE),
//			IsDown(VK_MENU), IsDown(VK_MENU, FALSE), IsDepressedModifier(CCommands::MetaAlt), IsDepressedModifier(CCommands::MetaAlt, FALSE));

		BYTE nKey = (BYTE)wParam; // VK_CONTROL is needed instead of VK_RCONTROL and VK_LCONTROL in this block just for Original()
		int nVirtualCommandType = NONE;
		if (IsDown(VK_CONTROL) && nKey != VK_CONTROL) {
			nVirtualCommandType |= CONTROL;
		}
		if (IsDown(VK_MENU) && nKey != VK_MENU) {
			nVirtualCommandType |= META;
		}
		if (Original(nVirtualCommandType, nKey)) {
			Original(nVirtualCommandType, nKey, -1);
			goto DO_NOTHING;
		}
	}

	if (Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::EnableOrDisableXKeymacs) {
		SetKeyboardHookFlag(!m_bHook);
		goto HOOK;
	}
	if (Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::EnableXKeymacs) {
		if (!m_bHook) {
			SetKeyboardHookFlag(!m_bHook);
		}
		goto HOOK;
	}
	if (Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::DisableXKeymacs) {
		if (m_bHook) {
			SetKeyboardHookFlag(!m_bHook);
		}
		goto HOOK;
	}
	if (!m_bHook) {
		goto DO_NOTHING;
	}

	if (CCommands::bM_x()) {
		static unsigned int index = 0;
		static TCHAR szPath[MAX_PATH] = {'\0'};

		if (lParam & BEING_RELEASED) {
			// ignore
		} else if (Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::BackwardChar) {
			if (index) {
				--index;
			}
			goto HOOKX;
		} else if (Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::BeginningOfLine) {
			index = 0;
			goto HOOKX;
		} else if (Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::DeleteBackwardChar) {
			if (index) {
				--index;
				memmove(&szPath[index], &szPath[index + 1], _tcslen(szPath) - index);
				ModifyM_xTip(szPath);
			}
			goto HOOKX;
		} else if (Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::DeleteChar) {
			if (index < _tcslen(szPath)) {
				memmove(&szPath[index], &szPath[index + 1], _tcslen(szPath) - index);
				ModifyM_xTip(szPath);
			}
			goto HOOKX;
		} else if (Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::EndOfLine) {
			index = _tcslen(szPath);
			goto HOOKX;
		} else if (Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::ForwardChar) {
			if (index < _tcslen(szPath)) {
				++index;
			}
			goto HOOKX;
		} else if (Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::KeyboardQuit) {
			CCommands::bM_x(FALSE);
			index = 0;
			memset(szPath, 0, sizeof(szPath));
			goto HOOK;
		} else if (nKey == VK_RETURN
				|| Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::Newline) {
			InvokeM_x(szPath);

			CCommands::bM_x(FALSE);
			index = 0;
			memset(szPath, 0, sizeof(szPath));
			goto HOOK;
		} else if (index < MAX_PATH - 1) {
			const BOOL bIsShiftDown = CXkeymacsDll::IsDown(VK_SHIFT);
			for (TCHAR nAscii = 1; nAscii != 0; ++nAscii) { // repeat until overflow
				if (nKey != 0 && a2v(nAscii) == nKey && bIsShiftDown == IsShift(nAscii)) {
//					CUtils::Log("M-x: %#X (%c), %#X (%c)", nKey, nKey, nAscii, nAscii);
					if (index < _tcslen(szPath)) {
						memmove(&szPath[index + 1], &szPath[index], __min(_tcslen(szPath) - index, MAX_PATH - (index + 1) - 1));
					}
					szPath[index++] = nAscii;
//					CUtils::Log("M-x: %c(%#04x)", nAscii, nAscii);
					ModifyM_xTip(szPath);
					goto HOOKX;
				}
			}
		}
	}

	if (CCommands::bC_u()) {
		if ((nCommandType == NONE) && ('0' <= nKey) && (nKey <= '9')) {
			CCommands::NumericArgument(nKey - '0');
			goto HOOK0_9;
		}
		if ((nCommandType == NONE) && (nKey == 0xBD)) {
			CCommands::NumericArgumentMinus();
			goto HOOK0_9;
		}
	}

	if (Commands[m_nCommandID[m_nApplicationID][nCommandType & ~CONTROL][nKey]].fCommand == CCommands::OneShotModifierCtrl) {
		nOneShotModifier[nKey] = VK_LCONTROL;
		DepressKey(nOneShotModifier[nKey]);
		bCherryOneShotModifier = TRUE;
		goto HOOK;
	} else if (Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::OneShotModifierCtrlRepeat) {
		nOneShotModifier[nKey] = VK_LCONTROL;
		DepressKey(nOneShotModifier[nKey]);
		bCherryOneShotModifier = TRUE;
		goto HOOK;
	} else if (Commands[m_nCommandID[m_nApplicationID][nCommandType & ~CONTROL][nKey]].fCommand == CCommands::OneShotModifierCtrlRepeat) {
		ReleaseKey(nOneShotModifier[nKey]);
		bCherryOneShotModifier = FALSE;
		Kdu(nKey);
		goto HOOK;
	} else if (Commands[m_nCommandID[m_nApplicationID][nCommandType & ~META][nKey]].fCommand == CCommands::OneShotModifierAlt) {
		nOneShotModifier[nKey] = VK_LMENU;
		DepressKey(nOneShotModifier[nKey]);
		bCherryOneShotModifier = TRUE;
		goto HOOK;
	} else if (Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::OneShotModifierAltRepeat) {
		nOneShotModifier[nKey] = VK_LMENU;
		DepressKey(nOneShotModifier[nKey]);
		bCherryOneShotModifier = TRUE;
		goto HOOK;
	} else if (Commands[m_nCommandID[m_nApplicationID][nCommandType & ~META][nKey]].fCommand == CCommands::OneShotModifierAltRepeat) {
		ReleaseKey(nOneShotModifier[nKey]);
		bCherryOneShotModifier = FALSE;
		Kdu(nKey);
		goto HOOK;
	} else if (Commands[m_nCommandID[m_nApplicationID][nCommandType & ~SHIFT][nKey]].fCommand == CCommands::OneShotModifierShift) {
		nOneShotModifier[nKey] = VK_SHIFT;
		DepressKey(nOneShotModifier[nKey]);
		bCherryOneShotModifier = TRUE;
		goto HOOK;
	} else if (Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::OneShotModifierShiftRepeat) {
		nOneShotModifier[nKey] = VK_SHIFT;
		DepressKey(nOneShotModifier[nKey]);
		bCherryOneShotModifier = TRUE;
		goto HOOK;
	} else if (Commands[m_nCommandID[m_nApplicationID][nCommandType & ~SHIFT][nKey]].fCommand == CCommands::OneShotModifierShiftRepeat) {
		ReleaseKey(nOneShotModifier[nKey]);
		bCherryOneShotModifier = FALSE;
		Kdu(nKey);
		goto HOOK;
	} else {
		for (int i = 0; i < MAX_KEY; ++i) {
			if (nOneShotModifier[i] == nKey) {
				break;
			}
		}
		if (i == MAX_KEY) {
			bCherryOneShotModifier = FALSE;
		}
	}

	if (0 <= m_nFunctionID[m_nApplicationID][nCommandType][nKey]
	 && m_nFunctionID[m_nApplicationID][nCommandType][nKey] < MAX_FUNCTION
	 && _tcslen(m_szFunctionDefinition[m_nFunctionID[m_nApplicationID][nCommandType][nKey]])) {
		CallFunction(m_nFunctionID[m_nApplicationID][nCommandType][nKey]);
		CCommands::Reset(GOTO_HOOK);
		goto HOOK;
	}

	if (!Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand) {
		if (nKey == VK_CONTROL
		 || nKey == VK_LCONTROL
		 || nKey == VK_RCONTROL
		 || nKey == VK_MENU
		 || nKey == VK_LMENU
		 || nKey == VK_RMENU
		 || nKey == VK_SHIFT
		 || nKey == VK_LSHIFT
		 || nKey == VK_RSHIFT) {
			goto DO_NOTHING;
		}

		if (!(nCommandType & SHIFT)) {
			if (CCommands::IsSetMark()) {
				if (CCommands::MoveCaret(nKey, nCommandType & CONTROL) != CONTINUE) {
					CCommands::ClearNumericArgument();
					goto HOOK;
				}
				CCommands::SetMark(FALSE);
			}
		}

		if (1 < CCommands::GetNumericArgument()) {
			Kdu(nKey, CCommands::GetNumericArgument());
			CCommands::ClearNumericArgument();
			goto HOOK;
		}

		goto DO_NOTHING;
	}

	if (CCommands::IsTemporarilyDisableXKeymacs()
	 && Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand != CCommands::KeyboardQuit) {
		CCommands::SetTemporarilyDisableXKeymacs(FALSE);
		goto DO_NOTHING;
	}

	if (m_bAtIbeamCursorOnly[m_nApplicationID][nCommandType][nKey]) {
		CURSORINFO cursorinfo = { sizeof(cursorinfo) };
		if (GetCursorInfo(&cursorinfo) && cursorinfo.flags && cursorinfo.hCursor != LoadCursor(NULL, IDC_IBEAM)) {
			goto DO_NOTHING;
		}
	}

	m_bRightControl	= IsDown(VK_RCONTROL);
	m_bRightAlt		= IsDown(VK_RMENU);
	m_bRightShift	= IsDown(VK_RSHIFT);

	if (!bLocked) {
		bLocked = TRUE;
		fCommand = Commands[m_nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand;
RECURSIVE_COMMAND:
		switch (fCommand()) {
		case GOTO_DO_NOTHING:
			bLocked = FALSE;
			goto DO_NOTHING;
		case GOTO_HOOK:
			bLocked = FALSE;
			goto HOOK;
		case GOTO_RECURSIVE:
			goto RECURSIVE;
		case GOTO_HOOKX:
			bLocked = FALSE;
			goto HOOKX;
		case GOTO_HOOK0_9:
			bLocked = FALSE;
			goto HOOK0_9;
		default:
			ASSERT(0);
			bLocked = FALSE;
			goto DO_NOTHING;
		}
	} else {
		goto HOOK_RECURSIVE_KEY;
	}

DO_NOTHING:
	ModifyShell_NotifyIcon(SHIFT_ICON, IsDown(VK_SHIFT));
	ModifyShell_NotifyIcon(CTRL_ICON, IsControl());
	ModifyShell_NotifyIcon(ALT_ICON, IsDown(VK_MENU));

	{
		static BOOL bDefiningMacro = FALSE;
		if (m_bDefiningMacro) {
			static BOOL bDown[MAX_KEY] = {'\0'};

			if (!bDefiningMacro) {
				while (m_Macro.GetHeadPosition()) {
					void *p = m_Macro.GetAt(m_Macro.GetHeadPosition());
					m_Macro.RemoveHead();
					delete p;
					p = NULL;
				}
				memset(bDown, 0, sizeof(bDown));
			}

			if ((!(lParam & BEING_RELEASED)) || bDown[wParam]) {
				try {
					KbdMacro_t *pKbdMacro = new KbdMacro;
					if (pKbdMacro) {
						pKbdMacro->nCode = nCode;
						pKbdMacro->wParam = wParam;
						pKbdMacro->lParam = lParam;
						pKbdMacro->bOriginal = TRUE;
						m_Macro.AddTail((CObject *)pKbdMacro);
					}
				}
				catch (CMemoryException* e) {
					e->Delete();
//					CUtils::Log("KeyboardProc: 'new' threw an exception");
				}
				if (!(lParam & BEING_RELEASED)) {
					bDown[wParam] = TRUE;
				}
			}
		}
		bDefiningMacro = m_bDefiningMacro;
	}

	return ::CallNextHookEx(m_hHookKeyboard, nCode, wParam, lParam);

RECURSIVE:
	Kdu(RECURSIVE_KEY, 1, FALSE);
	ModifyShell_NotifyIcon(SHIFT_ICON, IsDown(VK_SHIFT));
	ModifyShell_NotifyIcon(CTRL_ICON, IsControl());
	ModifyShell_NotifyIcon(ALT_ICON, IsDown(VK_MENU));
	return TRUE;

HOOK:
	CCommands::SetLastCommand(fCommand);
HOOK0_9:
HOOKX:
	ModifyShell_NotifyIcon(SHIFT_ICON, IsDown(VK_SHIFT));
	ModifyShell_NotifyIcon(CTRL_ICON, IsControl());
	ModifyShell_NotifyIcon(ALT_ICON, IsDown(VK_MENU));
HOOK_RECURSIVE_KEY:
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CXkeymacsData Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXkeymacsData::CXkeymacsData()
{
	ClearAll();
}

CXkeymacsData::~CXkeymacsData()
{

}

// set application name
void CXkeymacsData::SetApplicationName(LPCTSTR lpszApplicationName)
{
	m_strApplicationName.Format(lpszApplicationName);
}

// return application name
CString CXkeymacsData::GetApplicationName()
{
	return m_strApplicationName;
}

// set hook or not
void CXkeymacsData::SetCommandID(int nCommandType, int nKey, int nCommandID)
{
	ASSERT(0 <= nCommandType	|| nCommandType	< MAX_COMMAND_TYPE);
	ASSERT(0 <= nKey			|| nKey			< MAX_KEY);

	m_nCommandID[nCommandType][nKey] = nCommandID;
}

// return hook or not
int CXkeymacsData::GetCommandID(int nCommandType, int nKey)
{
	ASSERT(0 <= nCommandType	|| nCommandType	< MAX_COMMAND_TYPE);
	ASSERT(0 <= nKey			|| nKey			< MAX_KEY);

	return m_nCommandID[nCommandType][nKey];
}

// set hook at ibeam cursor only or not
void CXkeymacsData::SetAtIbeamCursorOnly(int nCommandType, int nKey, BOOL bAtIbeamCursorOnly)
{
	ASSERT(0 <= nCommandType	|| nCommandType	< MAX_COMMAND_TYPE);
	ASSERT(0 <= nKey			|| nKey			< MAX_KEY);

	m_bAtIbeamCursorOnly[nCommandType][nKey] = bAtIbeamCursorOnly;
}

// get hook at ibeam cursor only or not
BOOL CXkeymacsData::GetAtIbeamCursorOnly(int nCommandType, int nKey)
{
	ASSERT(0 <= nCommandType	|| nCommandType	< MAX_COMMAND_TYPE);
	ASSERT(0 <= nKey			|| nKey			< MAX_KEY);

	return m_bAtIbeamCursorOnly[nCommandType][nKey];
}

// clear all data
void CXkeymacsData::ClearAll()
{
	ZeroMemory(m_nCommandID, sizeof(m_nCommandID));
	ZeroMemory(m_bAtIbeamCursorOnly, sizeof(m_bAtIbeamCursorOnly));
	m_strApplicationName.Empty();
}

void CXkeymacsDll::SetApplicationName(int nApplicationID, CString szApplicationName)
{
	ZeroMemory(m_szSpecialApp[nApplicationID], sizeof(m_szSpecialApp[nApplicationID]));
	_tcsncpy(m_szSpecialApp[nApplicationID], szApplicationName, sizeof(m_szSpecialApp[nApplicationID]));
}

void CXkeymacsDll::SetWindowText(int nApplicationID, CString szWindowText)
{
	ZeroMemory(m_szWindowText[nApplicationID], sizeof(m_szWindowText[nApplicationID]));
	_tcsncpy(m_szWindowText[nApplicationID], szWindowText, sizeof(m_szWindowText[nApplicationID]));
}

void CXkeymacsDll::SetCommandID(int nApplicationID, int nCommandType, int nKey, int nCommandID)
{
	m_nCommandID[nApplicationID][nCommandType][nKey] = nCommandID;
}

void CXkeymacsDll::SetAtIbeamCursorOnly(int nApplicationID, int nCommandType, int nKey, BOOL bAtIbeamCursorOnly)
{
	m_bAtIbeamCursorOnly[nApplicationID][nCommandType][nKey] = bAtIbeamCursorOnly;
}

void CXkeymacsDll::SetKillRingMax(int nApplicationID, int nKillRingMax)
{
	m_nKillRingMax[nApplicationID] = nKillRingMax;
}

void CXkeymacsDll::SetUseDialogSetting(int nApplicationID, BOOL bUseDialogSetting)
{
	m_bUseDialogSetting[nApplicationID] = bUseDialogSetting;
}

// Clear data of nApplicationID
void CXkeymacsDll::Clear(int nApplicationID)
{
	if (0 <= nApplicationID && nApplicationID < MAX_APP) {
		ZeroMemory(m_szSpecialApp[nApplicationID], sizeof(m_szSpecialApp[nApplicationID]));
		ZeroMemory(m_nCommandID[nApplicationID], sizeof(m_nCommandID[nApplicationID]));
		ZeroMemory(m_bAtIbeamCursorOnly[nApplicationID], sizeof(m_bAtIbeamCursorOnly[nApplicationID]));
		m_nKillRingMax[nApplicationID] = 0;
		m_bUseDialogSetting[nApplicationID] = FALSE;
		m_nSettingStyle[nApplicationID] = 0;
	} else {
		ASSERT(0);
	}
}

void CXkeymacsData::SetApplicationTitle(LPCTSTR lpszApplicationTitle)
{
	m_strApplicationTitle.Format(lpszApplicationTitle);

	// delete white space at the end of the application title.
	while (!m_strApplicationTitle.IsEmpty()
		&& _istspace(m_strApplicationTitle.GetAt(m_strApplicationTitle.GetLength() - 1))) {
		m_strApplicationTitle.Delete(m_strApplicationTitle.GetLength() - 1);
	}
}

CString CXkeymacsData::GetApplicationTitle()
{
	return m_strApplicationTitle;
}

void CXkeymacsData::SetKillRingMax(int nKillRingMax)
{
	m_nKillRingMax = nKillRingMax;
}

int CXkeymacsData::GetKillRingMax()
{
	return m_nKillRingMax;
}

BOOL CXkeymacsDll::IsValidKey(BYTE bVk)
{
	if (bVk == 0xf0) {	// 0xf0: Eisu key. GetAsyncKeyState returns the wrong state of Eisu key.
		return FALSE;
	}

	if (CUtils::IsNT()) {
		switch (bVk) {
		case VK_CONTROL:
		case VK_MENU:
		case VK_SHIFT:
			return FALSE;
		default:
			break;
		}
	}

	return TRUE;
}

BOOL CXkeymacsDll::IsDepressedModifier(int (__cdecl *Modifier)(void), BOOL bPhysicalKey)
{
	BYTE bVk = 0;
	do {
		if (IsValidKey(bVk)
		 && IsDown(bVk, bPhysicalKey)
		 && Commands[m_nCommandID[m_nApplicationID][NONE][bVk]].fCommand == Modifier) {
			return TRUE;
		}
	} while (++bVk);
	return FALSE;
}

BOOL CXkeymacsDll::IsDepressedShiftKeyOnly(BYTE nKey)
{
	if (nKey != VK_SHIFT
	 && nKey != VK_LSHIFT
	 && nKey != VK_RSHIFT) {
		return FALSE;
	}

	BYTE bVk = 0;
	do {
		if (bVk == VK_SHIFT
		 || bVk == VK_LSHIFT
		 || bVk == VK_RSHIFT) {
			continue;
		}

		if (IsDown(bVk)) {
			return FALSE;
		}
	} while (++bVk);
	return TRUE;
}

BOOL CXkeymacsDll::IsControl()
{
	return CCommands::bC_() || IsDepressedModifier(CCommands::C_, FALSE);
}

BOOL CXkeymacsDll::IsMeta()
{
	return CCommands::bM_() || IsDepressedModifier(CCommands::MetaAlt, FALSE);
}

void CXkeymacsDll::AddKillRing(BOOL bNewData)
{
	if (m_nKillRingMax[m_nApplicationID] == 0) {
		return;
	}

	CClipboardSnap *pSnap = new CClipboardSnap;
	if( !pSnap ) return;

	BOOL bCapture = pSnap->Capture();
	bCapture = pSnap->Capture();	// for "office drawing shape format". Can CClipboardSnap care this problem?

	if( bCapture ) {
		if (bNewData) {
			m_oKillRing.AddHead(pSnap);
		} else {
			if (m_oKillRing.IsEmpty()) {
				m_oKillRing.AddHead(pSnap);
			} else {
				for (CClipboardSnap *pParent = m_oKillRing.GetHead(); pParent->GetNext(); pParent = pParent->GetNext()) {
					;
				}
				pParent->SetNext(pSnap);
			}
		}
	} else {
		delete pSnap;
		pSnap = NULL;
	}

	m_nKillRing = 0;

	if (m_nKillRingMax[m_nApplicationID] < m_oKillRing.GetCount()) {
		CClipboardSnap *pSnap = m_oKillRing.GetTail();
		delete pSnap;
		pSnap = NULL;
		m_oKillRing.RemoveTail();
	}
}

// Return TRUE if there is another data
// Return FALSE if there is no more data
CClipboardSnap* CXkeymacsDll::GetKillRing(CClipboardSnap* pSnap, BOOL bForce)
{
	if (m_nKillRingMax[m_nApplicationID] == 0) {
		return NULL;
	}

	if (m_oKillRing.IsEmpty()) {
		return NULL;
	}

	m_nKillRing %= m_oKillRing.GetCount();

	if (!bForce) {
		CClipboardSnap oCurrentSnap;
		oCurrentSnap.Capture();

		CClipboardSnap *pKillRing = m_oKillRing.GetAt(m_oKillRing.FindIndex(m_nKillRing));
		if (!pKillRing) {
			return NULL;
		}
		for (; pKillRing->GetNext(); pKillRing = pKillRing->GetNext()) {
			;
		}
		if (*pKillRing != oCurrentSnap) {
			return NULL;
		}
	}

	if (!pSnap) {
		pSnap = m_oKillRing.GetAt(m_oKillRing.FindIndex(m_nKillRing));
	}
	pSnap->Restore();

	return pSnap->GetNext();
}

void CXkeymacsDll::Original(int nCommandType, BYTE bVk, int nOriginal)
{
	nCommandType &= ~SHIFT;

	if (CUtils::IsNT()) {
		switch (bVk) {
		case VK_CONTROL:
			bVk = VK_LCONTROL;
			break;
		case VK_MENU:
			bVk = VK_LMENU;
			break;
		case VK_SHIFT:
			bVk = VK_LSHIFT;
			break;
		default:
			break;
		}
	}

	m_nOriginal[nCommandType][bVk] += nOriginal;
}

int CXkeymacsDll::Original(int nCommandType, BYTE bVk)
{
	nCommandType &= ~SHIFT;

	if (CUtils::IsNT()) {
		switch (bVk) {
		case VK_CONTROL:
			bVk = VK_LCONTROL;
			break;
		case VK_MENU:
			bVk = VK_LMENU;
			break;
		case VK_SHIFT:
			bVk = VK_LSHIFT;
			break;
		default:
			break;
		}
	}

	return m_nOriginal[nCommandType][bVk];
}

void CXkeymacsDll::IncreaseKillRingIndex(int nKillRing)
{
	m_nKillRing += nKillRing;
}

// move CCommands
CString CXkeymacsData::GetCommandName(int nCommandID)
{
	CString szCommandName(Commands[nCommandID].szCommandName);
	return szCommandName;
}

// move CCommands
int CXkeymacsData::GetDefaultCommandType(int nCommandID, int nIndex)
{
	if (nCommandID < 0 || sizeof(Commands) / sizeof(Commands[0]) <= nCommandID
	 || nIndex < 0 || sizeof(Commands[nCommandID].keybind) / sizeof(Commands[nCommandID].keybind[0]) <= nIndex) {
		ASSERT(0);
		return NONE;
	}

	int nCommandType	= Commands[nCommandID].keybind[nIndex].nCommandType;
	int bVk				= Commands[nCommandID].keybind[nIndex].bVk;

	if (m_b106Keyboard) {
		if (nCommandType & SHIFT) {	// Shift
			switch (bVk) {
			case '2':
			case '6':
			case 0xBA:		// VK_OEM_1		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the ';:' key
				nCommandType &= ~SHIFT;
				break;
			default:
				break;
			}
		} else {					// Normal
			switch (bVk) {
			case 0xBB:		// VK_OEM_PLUS	Windows 2000/XP: For any country/region, the '+' key
			case 0xC0:		// VK_OEM_3		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the '`~' key
			case 0xDE:		// VK_OEM_7		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
				nCommandType |= SHIFT;
				break;
			default:
				break;
			}
		}
	}

	return nCommandType;
}

// move CCommands
int CXkeymacsData::GetDefaultCommandKey(int nCommandID, int nIndex)
{
	if (nCommandID < 0 || sizeof(Commands) / sizeof(Commands[0]) <= nCommandID
	 || nIndex < 0 || sizeof(Commands[nCommandID].keybind) / sizeof(Commands[nCommandID].keybind[0]) <= nIndex) {
		ASSERT(0);
		return 0;
	}

	int nCommandType	= Commands[nCommandID].keybind[nIndex].nCommandType;
	int bVk				= Commands[nCommandID].keybind[nIndex].bVk;

	if (m_b106Keyboard) {
		if (nCommandType & SHIFT) {	// Shift
			switch (bVk) {
			case '0':
				bVk = '9';
				break;
			case '2':
				bVk = 0xC0;	// VK_OEM_3		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the '`~' key
				break;
			case '6':
				bVk = 0xDE;	// VK_OEM_7		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
				break;
			case '7':
				bVk = '6';
				break;
			case '8':
				bVk = 0xBA;	// VK_OEM_1		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the ';:' key
				break;
			case '9':
				bVk = '8';
				break;
			case 0xBD:		// VK_OEM_MINUS	Windows 2000/XP: For any country/region, the '-' key
				bVk = 0xE2;	// VK_OEM_102	Windows 2000/XP: Either the angle bracket key or the backslash key on the RT 102-key keyboard
				break;
			case 0xC0:
				bVk = 0xDE;	// VK_OEM_7		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
				break;
			case 0xDE:		// VK_OEM_7		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
				bVk = '2';
				break;
			default:
				break;
			}
		} else {					// Normal
			switch (bVk) {
			case 0xBA:		// VK_OEM_1		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the ';:' key
				bVk = 0xBB;	// VK_OEM_PLUS	Windows 2000/XP: For any country/region, the '+' key
				break;
			case 0xBB:		// VK_OEM_PLUS	Windows 2000/XP: For any country/region, the '+' key
				bVk = 0xBD;	// VK_OEM_MINUS	Windows 2000/XP: For any country/region, the '-' key
				break;
			case 0xDE:		// VK_OEM_7		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
				bVk = '7';
				break;
			default:
				break;
			}
		}
	}

	return bVk;
}

// move CCommands
int CXkeymacsData::GetDefaultControlID(int nCommandID, int nIndex)
{
	if (nCommandID < 0 || sizeof(Commands) / sizeof(Commands[0]) <= nCommandID
	 || nIndex < 0 || sizeof(Commands[nCommandID].keybind) / sizeof(Commands[nCommandID].keybind[0]) <= nIndex) {
		ASSERT(0);
		return 0;
	}

	return Commands[nCommandID].keybind[nIndex].nControlID;
}

// nobody use
int CXkeymacsDll::GetMickey(int nDifferential, int nThreshold1, int nThreshold2, int nAcceleration, int nSpeed)
{
	nDifferential = nDifferential * 10 / nSpeed;

	switch (nAcceleration) {
	case 2:
		if (nThreshold2 < fabs((double)(nDifferential / 4))) {
			nDifferential /= 4;
			break;
		}
		// Do NOT write break; here.
	case 1:
		if (nThreshold1 < fabs((double)(nDifferential / 2))) {
			nDifferential /= 2;
		}
		break;
	case 0:
		break;
	default:
		ASSERT(0);
		break;
	}

	return nDifferential;
}


int CXkeymacsData::GetSettingStyle()
{
	return m_nSettingStyle;
}

void CXkeymacsData::SetSettingStyle(int nSettingStyle)
{
	m_nSettingStyle = nSettingStyle;
}

void CXkeymacsDll::SetSettingStyle(int nApplicationID, int nSettingStyle)
{
	m_nSettingStyle[nApplicationID] = nSettingStyle;
}

int CXkeymacsData::GetCategoryID(int nCommandID)
{
	return Commands[nCommandID].nCategoryID;
}

void CXkeymacsData::SetIgnoreUndefinedMetaCtrl(BOOL bIgnoreUndefinedMetaCtrl)
{
	m_bIgnoreUndefinedMetaCtrl = bIgnoreUndefinedMetaCtrl;
}

BOOL CXkeymacsData::GetIgnoreUndefinedMetaCtrl()
{
	return m_bIgnoreUndefinedMetaCtrl;
}

void CXkeymacsDll::SetIgnoreUndefinedMetaCtrl(int nApplicationID, BOOL bIgnoreUndefinedMetaCtrl)
{
	m_bIgnoreUndefinedMetaCtrl[nApplicationID] = bIgnoreUndefinedMetaCtrl;
}

void CXkeymacsData::SetIgnoreUndefinedC_x(BOOL bIgnoreUndefinedC_x)
{
	m_bIgnoreUndefinedC_x = bIgnoreUndefinedC_x;
}

BOOL CXkeymacsData::GetIgnoreUndefinedC_x()
{
	return m_bIgnoreUndefinedC_x;
}

void CXkeymacsDll::SetIgnoreUndefinedC_x(int nApplicationID, BOOL bIgnoreUndefinedC_x)
{
	m_bIgnoreUndefinedC_x[nApplicationID] = bIgnoreUndefinedC_x;
}

void CXkeymacsData::SetEnableCUA(BOOL bEnableCUA)
{
	m_bEnableCUA = bEnableCUA;
}

BOOL CXkeymacsData::GetEnableCUA()
{
	return m_bEnableCUA;
}

void CXkeymacsDll::SetEnableCUA(int nApplicationID, BOOL bEnableCUA)
{
	m_bEnableCUA[nApplicationID] = bEnableCUA;
}

BOOL CXkeymacsDll::GetEnableCUA()
{
	return m_bEnableCUA[m_nApplicationID];
}

void CXkeymacsData::SetUseDialogSetting(BOOL bUseDialogSetting)
{
	m_bUseDialogSetting = bUseDialogSetting;
}

BOOL CXkeymacsData::GetUseDialogSetting()
{
	return m_bUseDialogSetting;
}

int CXkeymacsData::GetDescriptionID(int nCommandID)
{
	return Commands[nCommandID].nDescriptionID;
}

int CXkeymacsData::GetToolTipID(int nCommandID)
{
	return Commands[nCommandID].nToolTipID;
}

void CXkeymacsDll::DefiningMacro(BOOL bDefiningMacro)
{
	m_bDefiningMacro = bDefiningMacro;

	if (bDefiningMacro) {	// start-kbd-macro
		if (CCommands::bC_u()) {
			ReleaseKey(VK_SHIFT);
			CallMacro();
		}
	} else {				// end-kbd-macro
		while (!m_Macro.IsEmpty()) {
			KbdMacro_t *pKbdMacro = (KbdMacro_t *)m_Macro.GetTail();
			if (pKbdMacro->lParam & BEING_RELEASED) {
				break;
			} else {
				m_Macro.RemoveTail();
				delete pKbdMacro;
				pKbdMacro = NULL;
			}
		}

//		CUtils::Log(_T("Macro MemMap: start"));
		if (!m_Macro.IsEmpty()) {
			static HANDLE hMacro = NULL;
			if (!hMacro) {
				hMacro = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 0x3000, _T("macro"));
			}
			if (hMacro) {
//				CUtils::Log(_T("Macro MemMap: 1"));
				PVOID pView = MapViewOfFile(hMacro, FILE_MAP_ALL_ACCESS, 0, 0, 0);
//				CUtils::Log(_T("Macro MemMap: 2"));
				if (pView) {
//					CUtils::Log(_T("Macro MemMap: 2.5"));
					for (int i = 0; i < m_Macro.GetCount(); ++i) {
//						CUtils::Log(_T("Macro MemMap: 3-1 %d"), i);
						KbdMacro_t *pKbdMacro = (KbdMacro_t *)m_Macro.GetAt(m_Macro.FindIndex(i));
//						CUtils::Log(_T("Macro MemMap: 3-2 %d"), i);
						memcpy((LPTSTR) pView + i * sizeof(KbdMacro_t), pKbdMacro, sizeof(KbdMacro_t));
//						CUtils::Log(_T("Macro MemMap: 3-3 %d"), i);
					}
//					CUtils::Log(_T("Macro MemMap: 4"));
					UnmapViewOfFile(pView);
//					CUtils::Log(_T("Macro MemMap: 5"));
				} else {
//					CUtils::Log(_T("Macro MemMpa: error: %d"), GetLastError());
				}
			} else {
//				CUtils::Log(_T("Macro MemMap: 6"));
				ASSERT(0);
			}
		}
	}
}

BOOL CXkeymacsDll::DefiningMacro()
{
	return m_bDefiningMacro;
}

/**/ 
void CXkeymacsDll::CallMacro()
{
	BOOL bIsCtrlDown = IsDown(VK_CONTROL);
	if (bIsCtrlDown) {
		ReleaseKey(VK_CONTROL);
	}
	BOOL bIsAltDown = IsDown(VK_MENU);
	if (bIsAltDown) {
		ReleaseKey(VK_MENU);
	}
	BOOL bIsShiftDown = IsDown(VK_SHIFT);
	if (bIsShiftDown) {
		ReleaseKey(VK_SHIFT);
	}

	for (POSITION pos = m_Macro.GetHeadPosition(); pos; ) {
		KbdMacro_t *pKbdMacro = (KbdMacro_t *)m_Macro.GetNext(pos);
		if (pKbdMacro->lParam & BEING_RELEASED) {
			ReleaseKey((BYTE)pKbdMacro->wParam);
		} else {
			DepressKey((BYTE)pKbdMacro->wParam, pKbdMacro->bOriginal);
		}
	}

	if (bIsCtrlDown) {
		DepressKey(VK_CONTROL);
	}
	if (bIsAltDown) {
		DepressKey(VK_MENU);
	}
	if (bIsShiftDown) {
		DepressKey(VK_SHIFT);
	}
}

/*/
void CXkeymacsDll::CallMacro()	// for debug
{
	CString sz;
	for (POSITION pos = m_Macro.GetHeadPosition(); pos; ) {
		KbdMacro_t *pKbdMacro = (KbdMacro_t *)m_Macro.GetNext(pos);
		if (pKbdMacro->lParam & BEING_RELEASED) {
			CString t;
			t.Format(_T("0x%xu "), pKbdMacro->wParam);
			sz += t;
		} else {
			CString t;
			t.Format(_T("0x%xd "), pKbdMacro->wParam);
			sz += t;
		}
	}
//	CUtils::Log(sz);
}
/**/

void CXkeymacsData::Set106Keyboard(BOOL b106Keyboard)
{
	m_b106Keyboard = b106Keyboard;
}

BOOL CXkeymacsData::Is106Keyboard()
{
	return m_b106Keyboard;
}

int CXkeymacsDll::IsPassThrough(BYTE nKey)
{
	BYTE bVk = 0;
	do {
		if (IsDown(bVk)
		 && (Commands[m_nCommandID[m_nApplicationID][NONE][bVk]].fCommand == CCommands::PassThrough)) {
			if (bVk == nKey) {
				return GOTO_HOOK;
			}

			return GOTO_DO_NOTHING;
		}
	} while (++bVk);
	return CONTINUE;
}

void CXkeymacsDll::SetKeyboardHookFlag()
{
	SetKeyboardHookFlag(m_bHook);
}

void CXkeymacsDll::SetFunctionKey(int nFunctionID, int nApplicationID, int nCommandType, int nKey)
{
	if (nApplicationID	< 0 || MAX_APP			<= nApplicationID
	 || nCommandType	< 0 || MAX_COMMAND_TYPE	<= nCommandType
	 || nKey			< 0 || MAX_KEY			<= nKey) {
		return;
	}

	m_nFunctionID[nApplicationID][nCommandType][nKey] = nFunctionID;
}

void CXkeymacsDll::ClearFunctionDefinition()
{
	memset(m_nFunctionID, -1, sizeof(m_nFunctionID));
	memset(m_szFunctionDefinition, 0, sizeof(m_szFunctionDefinition));
}

void CXkeymacsDll::SetFunctionDefinition(int nFunctionID, CString szDefinition)
{
	if (nFunctionID < 0 || MAX_FUNCTION <= nFunctionID) {
		return;
	}

	memset(m_szFunctionDefinition[nFunctionID], 0, sizeof(m_szFunctionDefinition[nFunctionID]));
	_stprintf(m_szFunctionDefinition[nFunctionID], _T("%s"), szDefinition);

	return;

}

// call an original command which is defined in dot.xkeymacs
void CXkeymacsDll::CallFunction(int nFunctionID)
{
	CArray<KeyBind_t, KeyBind_t> keybinds;

	if (nFunctionID < 0 || MAX_FUNCTION <= nFunctionID || !_tcslen(m_szFunctionDefinition[nFunctionID])) {
		return;
	}

	BOOL bIsCtrlDown = CXkeymacsDll::IsDown(VK_CONTROL);
	BOOL bIsAltDown = CXkeymacsDll::IsDown(VK_MENU);
	BOOL bIsShiftDown = CXkeymacsDll::IsDown(VK_SHIFT);

	if (m_szFunctionDefinition[nFunctionID][0] == _T('"') && m_szFunctionDefinition[nFunctionID][_tcslen(m_szFunctionDefinition[nFunctionID]) - 1] == _T('"')) {
		for (unsigned int i = 1; i < _tcslen(m_szFunctionDefinition[nFunctionID]) - 1; ++i) {	// skip '"'
			keybinds.Add(ParseKey(nFunctionID, i));
		}
	} else if (m_szFunctionDefinition[nFunctionID][0] == _T('[') && m_szFunctionDefinition[nFunctionID][_tcslen(m_szFunctionDefinition[nFunctionID]) - 1] == _T(']')) {
		for (unsigned int i = 1; i < _tcslen(m_szFunctionDefinition[nFunctionID]) - 1; ++i) {	// skip '[' and ']'
			if (m_szFunctionDefinition[nFunctionID][i] == _T('?')) {	// [?f ?o ?o]
				++i;
				keybinds.Add(ParseKey(nFunctionID, i));
			} else {												// [ControlCharacter]
				for (int nKeyID = 0; nKeyID < sizeof(ControlCharacters) / sizeof(ControlCharacters[0]); ++nKeyID) {
					if (!_tcsncmp(m_szFunctionDefinition[nFunctionID] + i, ControlCharacters[nKeyID].name, _tcslen(ControlCharacters[nKeyID].name))) {
						KeyBind_t keybind = {NONE, ControlCharacters[nKeyID].bVk};
						keybinds.Add(keybind);
						i += _tcslen(ControlCharacters[nKeyID].name);
						break;
					}
				}
			}
		}
	} else {
		return;
	}

	BOOL bM_x = FALSE;
	TCHAR szPath[MAX_PATH] = {'\0'};
	unsigned int index = 0;
	BOOL bInitialized = FALSE;

	for (int i = 0; i < keybinds.GetSize(); ++i) {
		const int nCommandType = keybinds.GetAt(i).nCommandType;
		const BYTE bVk = keybinds.GetAt(i).bVk;

		if (nCommandType < MAX_COMMAND_TYPE && Commands[m_nCommandID[m_nApplicationID][nCommandType][bVk]].fCommand) {
			if (Commands[m_nCommandID[m_nApplicationID][nCommandType][bVk]].fCommand == CCommands::ExecuteExtendedCommand) {
				bM_x = TRUE;
			} else if (!bInitialized) {
				if (bIsCtrlDown) {
					CUtils::UpdateKeyboardState(VK_CONTROL, 0);
					ReleaseKey(VK_CONTROL);
				}

				if (bIsAltDown) {
					ReleaseKey(VK_MENU);
				}

				if (bIsShiftDown) {
					ReleaseKey(VK_SHIFT);
				}

				bInitialized = TRUE;
			}
//			CUtils::Log("CallFunction: Command Name: %s", Commands[m_nCommandID[m_nApplicationID][nCommandType][bVk]].szCommandName);
			while (Commands[m_nCommandID[m_nApplicationID][nCommandType][bVk]].fCommand() == GOTO_RECURSIVE) {
				;
			}
		} else if (bM_x) {
			if (bVk == VK_RETURN) {
				InvokeM_x(szPath);
			} else {
				for (TCHAR nAscii = 1; nAscii != 0; ++nAscii) { // repeat until overflow
					if (bVk != 0 && a2v(nAscii) == bVk && ((nCommandType & SHIFT) != 0) == IsShift(nAscii)) {
//						CUtils::Log("M-x: %#X (%c), %#X (%c)", bVk, bVk, nAscii, nAscii);
						szPath[index++] = nAscii;
						break;
					}
				}
			}
		} else {
			if (!bInitialized) {
				if (bIsCtrlDown) {
					CUtils::UpdateKeyboardState(VK_CONTROL, 0);
					ReleaseKey(VK_CONTROL);
				}

				if (bIsAltDown) {
					ReleaseKey(VK_MENU);
				}

				if (bIsShiftDown) {
					ReleaseKey(VK_SHIFT);
				}

				bInitialized = TRUE;
			}
			if (nCommandType & WIN_WIN) {
				DepressKey(VK_LWIN);
			}
			if (nCommandType & WIN_CTRL) {
				DepressKey(VK_CONTROL);
			}
			if (nCommandType & WIN_ALT) {
				DepressKey(VK_MENU);
			}
			if (nCommandType & SHIFT) {
				DepressKey(VK_SHIFT);
			}

			Kdu(bVk);

			if (nCommandType & SHIFT && (keybinds.GetSize() <= i + 1 || !(keybinds.GetAt(i + 1).nCommandType & SHIFT))) {
				ReleaseKey(VK_SHIFT);
			}
			if (nCommandType & WIN_ALT && (keybinds.GetSize() <= i + 1 || !(keybinds.GetAt(i + 1).nCommandType & WIN_ALT))) {
				ReleaseKey(VK_MENU);
			}
			if (nCommandType & WIN_CTRL && (keybinds.GetSize() <= i + 1 || !(keybinds.GetAt(i + 1).nCommandType & WIN_CTRL))) {
				ReleaseKey(VK_CONTROL);
			}
			if (nCommandType & WIN_WIN && (keybinds.GetSize() <= i + 1 || !(keybinds.GetAt(i + 1).nCommandType & WIN_WIN))) {
				ReleaseKey(VK_LWIN);
			}
		}
	}

	keybinds.RemoveAll();

	if (bInitialized) {
		// If these lines are invoked at M-x, a window transition does not work well.

		if (bIsShiftDown) {
			DepressKey(VK_SHIFT);
		}

		if (bIsAltDown) {
			DepressKey(VK_MENU);
		}

		if (bIsCtrlDown) {
			DepressKey(VK_CONTROL);
			CUtils::UpdateKeyboardState(VK_CONTROL, 1);
		}
	}
	return;
}

KeyBind_t CXkeymacsDll::ParseKey(const int nFunctionID, unsigned int &i)
{
	KeyBind keybind = {NONE};

	if (m_szFunctionDefinition[nFunctionID][i] == _T('\\')) {
		++i;
		BOOL bFound = FALSE;
		do {
			bFound = FALSE;
			for (int ModifierID = 0; ModifierID < sizeof(Modifiers) / sizeof(Modifiers[0]); ++ModifierID) {
				if (!_tcsncmp(m_szFunctionDefinition[nFunctionID] + i, Modifiers[ModifierID].name, _tcslen(Modifiers[ModifierID].name))
				 && _tcslen(Modifiers[ModifierID].name) < _tcslen(m_szFunctionDefinition[nFunctionID] + i)) {
					keybind.nCommandType |= Modifiers[ModifierID].id;
					i+= _tcslen(Modifiers[ModifierID].name);
					bFound = TRUE;
				}
			}
		} while (bFound);
	}
	if (IsShift(m_szFunctionDefinition[nFunctionID][i]) && !(keybind.nCommandType & (WIN_CTRL | WIN_ALT | WIN_WIN))) {
		keybind.nCommandType |= SHIFT;
	}

	for (int nKeyID = 0; nKeyID < sizeof(ControlCharacters) / sizeof(ControlCharacters[0]); ++nKeyID) {
		if (!_tcsncmp(m_szFunctionDefinition[nFunctionID] + i, ControlCharacters[nKeyID].name, _tcslen(ControlCharacters[nKeyID].name))) {
			i += _tcslen(ControlCharacters[nKeyID].name);
			break;
		}
	}
	if (nKeyID < sizeof(ControlCharacters) / sizeof(ControlCharacters[0])) {
		keybind.bVk = ControlCharacters[nKeyID].bVk;
	} else {
		keybind.bVk = a2v(m_szFunctionDefinition[nFunctionID][i]);
	}

	return keybind;
}

BOOL CXkeymacsDll::IsShift(TCHAR nAscii)
{
	switch (nAscii) {
	case _T(' '):
		return FALSE;
	case _T('!'):
	case _T('"'):
	case _T('#'):
	case _T('$'):
	case _T('%'):
	case _T('&'):
		return TRUE;
	case _T('\''):
		return CXkeymacsData::Is106Keyboard();
	case _T('('):
	case _T(')'):
	case _T('*'):
	case _T('+'):
		return TRUE;
	case _T(','):
	case _T('-'):
	case _T('.'):
	case _T('/'):
	case _T('0'): case _T('1'): case _T('2'): case _T('3'): case _T('4'): case _T('5'): case _T('6'): case _T('7'): case _T('8'): case _T('9'):
		return FALSE;
	case _T(':'):
		return !CXkeymacsData::Is106Keyboard();
	case _T(';'):
		return FALSE;
	case _T('<'):
		return TRUE;
	case _T('='):
		return CXkeymacsData::Is106Keyboard();
	case _T('>'):
	case _T('?'):
		return TRUE;
	case _T('@'):
		return !CXkeymacsData::Is106Keyboard();
	case _T('A'): case _T('B'): case _T('C'): case _T('D'): case _T('E'): case _T('F'): case _T('G'): case _T('H'): case _T('I'): case _T('J'): 
	case _T('K'): case _T('L'): case _T('M'): case _T('N'): case _T('O'): case _T('P'): case _T('Q'): case _T('R'): case _T('S'): case _T('T'): 
	case _T('U'): case _T('V'): case _T('W'): case _T('X'): case _T('Y'): case _T('Z'): 
		return TRUE;
	case _T('['):
	case _T('\\'):
	case _T(']'):
		return FALSE;
	case _T('^'):
		return !CXkeymacsData::Is106Keyboard();
	case _T('_'):
		return TRUE;
	case _T('`'):
		return CXkeymacsData::Is106Keyboard();
	case _T('a'): case _T('b'): case _T('c'): case _T('d'): case _T('e'): case _T('f'): case _T('g'): case _T('h'): case _T('i'): case _T('j'): 
	case _T('k'): case _T('l'): case _T('m'): case _T('n'): case _T('o'): case _T('p'): case _T('q'): case _T('r'): case _T('s'): case _T('t'): 
	case _T('u'): case _T('v'): case _T('w'): case _T('x'): case _T('y'): case _T('z'): 
		return FALSE;
	case _T('{'):
	case _T('|'):
	case _T('}'):
	case _T('~'):
		return TRUE;
	default:
		return FALSE;
	}
}

BYTE CXkeymacsDll::a2v(TCHAR nAscii)
{
	switch (nAscii) {
	case _T(' '):
		return VK_SPACE;
	case _T('!'):
		return '1';
	case _T('"'):
		return CXkeymacsData::Is106Keyboard() ? '2' : (BYTE) 0xde;	// VK_OEM_7
	case _T('#'):
		return '3';
	case _T('$'):
		return '4';
	case _T('%'):
		return '5';
	case _T('&'):
		return CXkeymacsData::Is106Keyboard() ? '6' : '7';
	case _T('\''):
		return CXkeymacsData::Is106Keyboard() ? '7' : (BYTE) 0xde;	// VK_OEM_7
	case _T('('):
		return CXkeymacsData::Is106Keyboard() ? '8' : '9';
	case _T(')'):
		return CXkeymacsData::Is106Keyboard() ? '9' : '0';
	case _T('*'):
		return CXkeymacsData::Is106Keyboard() ? (BYTE) 0xba : '8';	// VK_OEM_1
	case _T('+'):
		return 0xbb;	// VK_OEM_PLUS
	case _T(','):
		return 0xbc;	// VK_OEM_COMMA
	case _T('-'):
		return 0xbd;	// VK_OEM_MINUS
	case _T('.'):
		return 0xbe;	// VK_OEM_PERIOD
	case _T('/'):
		return 0xbf;	// VK_OEM_2
	case _T('0'): case _T('1'): case _T('2'): case _T('3'): case _T('4'): case _T('5'): case _T('6'): case _T('7'): case _T('8'): case _T('9'):
		return nAscii;
	case _T(':'):
		return 0xba;	// VK_OEM_1
	case _T(';'):
		return CXkeymacsData::Is106Keyboard() ? (BYTE) 0xbb : (BYTE) 0xba;	// VK_OEM_PLUS	VK_OEM_1
	case _T('<'):
		return 0xbc;	// VK_OEM_COMMA
	case _T('='):
		return CXkeymacsData::Is106Keyboard() ? (BYTE) 0xbd : (BYTE) 0xbb;	// VK_OEM_MINUS	VK_OEM_PLUS
	case _T('>'):
		return 0xbe;	// VK_OEM_PERIOD
	case _T('?'):
		return 0xbf;	// VK_OEM_2
	case _T('@'):
		return CXkeymacsData::Is106Keyboard() ? (BYTE) 0xc0 : '2';
	case _T('A'): case _T('B'): case _T('C'): case _T('D'): case _T('E'): case _T('F'): case _T('G'): case _T('H'): case _T('I'): case _T('J'): 
	case _T('K'): case _T('L'): case _T('M'): case _T('N'): case _T('O'): case _T('P'): case _T('Q'): case _T('R'): case _T('S'): case _T('T'): 
	case _T('U'): case _T('V'): case _T('W'): case _T('X'): case _T('Y'): case _T('Z'): 
		return nAscii;
	case _T('['):
		return 0xdb;	// VK_OEM_4
	case _T('\\'):
		return 0xdc;	// VK_OEM_5
	case _T(']'):
		return 0xdd;	// VK_OEM_6
	case _T('^'):
		return CXkeymacsData::Is106Keyboard() ? (BYTE) 0xde : '6';	// VK_OEM_7
	case _T('_'):
		return CXkeymacsData::Is106Keyboard() ? (BYTE) 0xe2 : (BYTE) 0xbd;	// VK_OEM_102	VK_OEM_MINUS
	case _T('`'):
		return 0xc0;	// VK_OEM_3
	case _T('a'): case _T('b'): case _T('c'): case _T('d'): case _T('e'): case _T('f'): case _T('g'): case _T('h'): case _T('i'): case _T('j'): 
	case _T('k'): case _T('l'): case _T('m'): case _T('n'): case _T('o'): case _T('p'): case _T('q'): case _T('r'): case _T('s'): case _T('t'): 
	case _T('u'): case _T('v'): case _T('w'): case _T('x'): case _T('y'): case _T('z'): 
		return (BYTE) (nAscii - (_T('a') - _T('A')));
	case _T('{'):
		return 0xdb;	// VK_OEM_4
	case _T('|'):
		return 0xdc;	// VK_OEM_5
	case _T('}'):
		return 0xdd;	// VK_OEM_6
	case _T('~'):
		return CXkeymacsData::Is106Keyboard() ? (BYTE) 0xde : (BYTE) 0xc0;	// VK_OEM_7	VK_OEM_3
	default:
		return 0;
	}
}

void CXkeymacsDll::DeleteAllShell_NotifyIcon()
{
	for (int icon = 0; icon < MAX_ICON_TYPE; ++icon) {
		DeleteShell_NotifyIcon((ICON_TYPE)icon);
	}
}

void CXkeymacsDll::AddAllShell_NotifyIcon()
{
	for (int icon = 0; icon < MAX_ICON_TYPE; ++icon) {
		AddShell_NotifyIcon((ICON_TYPE)icon);
	}
}

void CXkeymacsData::SetWindowText(LPCTSTR lpszWindowText)
{
	m_nWindowTextType = CUtils::GetWindowTextType(lpszWindowText);
	if (m_nWindowTextType == IDS_WINDOW_TEXT_IGNORE) {
		m_strWindowText = _T('*');
	} else {
		m_strWindowText.Format(lpszWindowText);
	}
}

CString CXkeymacsData::GetWindowText()
{
	return m_strWindowText;
}

void CXkeymacsData::SetWindowTextType(int nWindowTextType)
{
	m_nWindowTextType = nWindowTextType;
}

int CXkeymacsData::GetWindowTextType()
{
	return m_nWindowTextType;
}

BOOL CXkeymacsDll::IsMatchWindowText(CString szWindowText)
{
	BOOL bIsMatchWindowText = TRUE;

	TCHAR szCurrentWindowText[0x100] = {'\0'};
	GetWindowText(GetForegroundWindow(), szCurrentWindowText, sizeof(szCurrentWindowText));

	switch (CUtils::GetWindowTextType(szWindowText)) {
	case IDS_WINDOW_TEXT_MATCH:								// *foo*
		szWindowText.Delete(0);								// Delete first '*'
		szWindowText.Delete(szWindowText.GetLength() - 1);	// Delete last '*'
		bIsMatchWindowText = 0 <= CString(szCurrentWindowText).Find(szWindowText);
		break;
	case IDS_WINDOW_TEXT_MATCH_FORWARD:						// foo*
		szWindowText.Delete(szWindowText.GetLength() - 1);	// Delete last '*'
		bIsMatchWindowText = 0 == CString(szCurrentWindowText).Find(szWindowText);
		break;
	case IDS_WINDOW_TEXT_MATCH_BACKWARD:					// *foo
		szWindowText.Delete(0);								// Delete first '*'
		bIsMatchWindowText = 0 <= CString(szCurrentWindowText).Find(szWindowText, CString(szCurrentWindowText).GetLength() - szWindowText.GetLength());
		break;
	case IDS_WINDOW_TEXT_MATCH_FULL:						// foo
		bIsMatchWindowText = szWindowText == CString(szCurrentWindowText);
		break;
	case IDS_WINDOW_TEXT_IGNORE:							// *
		bIsMatchWindowText = TRUE;
		break;
	default:
		ASSERT(0);
		break;
	}

//	CUtils::Log(_T("IsMatchWindowText: %d, _%s_, _%s_"), bIsMatchWindowText, szCurrentWindowText, szWindowText);
	return bIsMatchWindowText;
}

void CXkeymacsDll::SetAccelerate(int nAccelerate)
{
	m_nAccelerate = nAccelerate;
}

int CXkeymacsDll::GetAccelerate()
{
	return m_nAccelerate;
}

void CXkeymacsDll::SetKeyboardSpeed(int nKeyboardSpeed)
{
	m_nKeyboardSpeed = nKeyboardSpeed;
}

unsigned int CXkeymacsDll::GetMaxKeyInterval()
{
	// m_nKeyboardSpeed == 0:	slowest repeat rate; approximately  2 characters per second
	// m_nKeyboardSpeed == 31:	fastest repeat rate; approximately 30 characters per second
	// 47 ms is max on my machine w/ KeyboardSpeed 31.
	// 1000 /  2 + 50 = 550
	// 1000 / 30 + 50 = 83
	return (unsigned int) (1000.0 / (2.0 + m_nKeyboardSpeed % 32 * 28.0 / 31.0) + 50.0);
}

void CXkeymacsDll::SetCursorData(HCURSOR hEnable, HCURSOR hDisableTMP, HCURSOR hDisableWOCQ, HICON hDisable, BOOL bEnable)
{
	m_hCursor[STATUS_ENABLE] = hEnable;
	m_hCursor[STATUS_DISABLE_TMP] = hDisableTMP;
	m_hCursor[STATUS_DISABLE_WOCQ] = hDisableWOCQ;
	m_hCursor[STATUS_DISABLE] = hDisable;
	m_bCursor = bEnable;
}

void CXkeymacsDll::DoSetCursor()
{
	if (m_bCursor && m_hCurrentCursor) {
		::SetCursor(m_hCurrentCursor);
	}
}

BOOL CXkeymacsData::Get326Compatible()
{
	return m_b326Compatible;
}

void CXkeymacsData::Set326Compatible(BOOL b326Compatible)
{
	m_b326Compatible = b326Compatible;
}

void CXkeymacsDll::Set326Compatible(int nApplicationID, BOOL b326Compatible)
{
	m_b326Compatible[nApplicationID] = b326Compatible;
}

BOOL CXkeymacsDll::Get326Compatible()
{
	return m_b326Compatible[m_nApplicationID];
}

void CXkeymacsDll::InvokeM_x(const TCHAR *const szPath)
{
//	CUtils::Log("M-x: szPath=_%s_", szPath);
	int (*fCommand)() = NULL;

	for (int i = 0; i < sizeof(Commands) / sizeof(Commands[0]); ++i) {
		if (_tcsicmp(szPath, Commands[i].szCommandName) == 0) {
			fCommand = Commands[i].fCommand;
			break;
		}
	}

	if (fCommand) {
//		CUtils::Log("M-x: Command: _%s_", Commands[i].szCommandName);
		fCommand();
	} else {
//		CUtils::Log("M-x: Path: _%s_", szPath);
		ShellExecute(NULL, NULL, szPath, NULL, NULL, SW_SHOWNORMAL);
	}
}

void CXkeymacsDll::ModifyM_xTip(const TCHAR *const szPath)
{
	if (szPath) {
		if (_tcslen(szPath) < sizeof(m_stNtfyIcon[MX_ICON].szTip) / sizeof(m_stNtfyIcon[MX_ICON].szTip[0]) - 5) {
			memset(m_stNtfyIcon[MX_ICON].szTip, 0, sizeof(m_stNtfyIcon[MX_ICON].szTip));
			_stprintf(m_stNtfyIcon[MX_ICON].szTip, "M-x %s", szPath);
		}
	} else {
		memset(m_stNtfyIcon[MX_ICON].szTip, 0, sizeof(m_stNtfyIcon[MX_ICON].szTip));
		_stprintf(m_stNtfyIcon[MX_ICON].szTip, "M-x LED");
	}

	ModifyShell_NotifyIcon(MX_ICON, CCommands::bM_x(), TRUE);
}
