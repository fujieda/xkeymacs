// xkeymacsdll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "resource.h"
#include "Utils.h"
#include "Commands.h"
#include <afxdllx.h>
#include <math.h>
#include <Imm.h>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct Modifier {
	LPCTSTR name;
	int id;
};

static const Modifier Modifiers[] = {
//	{ _T("A-"), ALT },
	{ _T("C-"), CONTROL},
//	{ _T("H-"), HYPER },
	{ _T("M-"), META },
	{ _T("S-"), SHIFT },
//	{ _T("s-"), SUPER },
	{ _T("Ctrl+"), WIN_CTRL },
	{ _T("Alt+"), WIN_ALT },
	{ _T("Win+"), WIN_WIN },
};
static const int MAX_MODIFIER = _countof(Modifiers);

static const KeyName KeyNames[] = {
//	{ VK_LBUTTON,		_T("mouse-1") },				// does not work well
//	{ VK_RBUTTON,		_T("mouse-3") },				// does not work well
	{ VK_CANCEL,		_T("break") },
//	{ VK_MBUTTON,		_T("mouse-2") },				// does not work well
	{ VK_BACK,			_T("backspace") },
	{ VK_TAB,			_T("tab") },
	{ VK_RETURN,		_T("return") },
	{ VK_CAPITAL,		_T("capslock") },
	{ VK_KANA,			_T("kana") },
	{ VK_KANJI,			_T("kanji") },
	{ VK_ESCAPE,		_T("escape") },
	{ VK_CONVERT,		_T("convert") },
	{ VK_NONCONVERT,	_T("nonconvert") },
//	{ VK_SPACE,			_T("SPC") },					// [? ]
	{ VK_PRIOR,			_T("prior") },
	{ VK_NEXT,			_T("next") },
	{ VK_END,			_T("end") },
	{ VK_HOME,			_T("home") },
	{ VK_LEFT,			_T("left") },
	{ VK_UP,			_T("up") },
	{ VK_RIGHT,			_T("right") },
	{ VK_DOWN,			_T("down") },
	{ VK_SELECT,		_T("select") },
	{ VK_PRINT,			_T("print") },
	{ VK_EXECUTE,		_T("execute") },
	{ VK_SNAPSHOT,		_T("printscreen") },			// work as print
	{ VK_INSERT,		_T("insert") },
	{ VK_DELETE,		_T("delete") },
	{ VK_LWIN,			_T("lwindow") },
	{ VK_RWIN,			_T("rwindow") },
	{ VK_APPS,			_T("apps") },
	{ VK_SLEEP,			_T("sleep") },
	{ VK_NUMPAD0,		_T("kp-0") },
	{ VK_NUMPAD1,		_T("kp-1") },
	{ VK_NUMPAD2,		_T("kp-2") },
	{ VK_NUMPAD3,		_T("kp-3") },
	{ VK_NUMPAD4,		_T("kp-4") },
	{ VK_NUMPAD5,		_T("kp-5") },
	{ VK_NUMPAD6,		_T("kp-6") },
	{ VK_NUMPAD7,		_T("kp-7") },
	{ VK_NUMPAD8,		_T("kp-8") },
	{ VK_NUMPAD9,		_T("kp-9") },
	{ VK_MULTIPLY,		_T("kp-multiply") },
	{ VK_ADD,			_T("kp-add") },
	{ VK_SUBTRACT,		_T("kp-subtract") },
	{ VK_DECIMAL,		_T("kp-decimal") },
	{ VK_DIVIDE,		_T("kp-divide") },
//	{ VK_F1,			_T("f1") },						// FIXME
//	{ VK_F2,			_T("f2") },						// Move at the end of definition of function keys to keep away confliction f1/f2 and f1?/f2? by _tcsncmp() i.e. strncmp()
	{ VK_F3,			_T("f3") },
	{ VK_F4,			_T("f4") },
	{ VK_F5,			_T("f5") },
	{ VK_F6,			_T("f6") },
	{ VK_F7,			_T("f7") },
	{ VK_F8,			_T("f8") },
	{ VK_F9,			_T("f9") },
	{ VK_F10,			_T("f10") },
	{ VK_F11,			_T("f11") },
	{ VK_F12,			_T("f12") },
	{ VK_F13,			_T("f13") },
	{ VK_F14,			_T("f14") },
	{ VK_F15,			_T("f15") },
	{ VK_F16,			_T("f16") },
	{ VK_F17,			_T("f17") },
	{ VK_F18,			_T("f18") },
	{ VK_F19,			_T("f19") },
	{ VK_F20,			_T("f20") },
	{ VK_F21,			_T("f21") },
	{ VK_F22,			_T("f22") },
	{ VK_F23,			_T("f23") },
	{ VK_F24,			_T("f24") },
	{ VK_F1,			_T("f1") },
	{ VK_F2,			_T("f2") },
	{ VK_NUMLOCK,		_T("kp-numlock") },
	{ VK_SCROLL,		_T("scroll") },
	{ 0xa6,				_T("browser-back") },			// VK_BROWSER_BACK
	{ 0xa7,				_T("browser-forward") },		// VK_BROWSER_FORWARD
	{ 0xa8,				_T("browser-refresh") },		// VK_BROWSER_REFRESH
	{ 0xa9,				_T("browser-stop") },			// VK_BROWSER_STOP
	{ 0xaa,				_T("browser-search") },			// VK_BROWSER_SEARCH
	{ 0xab,				_T("browser-favorites") },		// VK_BROWSER_FAVORITES
	{ 0xac,				_T("browser-home") },			// VK_BROWSER_HOME
	{ 0xad,				_T("volume-mute") },			// VK_VOLUME_MUTE
	{ 0xae,				_T("volume-down") },			// VK_VOLUME_DOWN
	{ 0xaf,				_T("volume-up") },				// VK_VOLUME_UP
	{ 0xb0,				_T("media-next-track") },		// VK_MEDIA_NEXT_TRACK
	{ 0xb1,				_T("media-prev-track") },		// VK_MEDIA_PREV_TRACK
	{ 0xb2,				_T("media-stop") },				// VK_MEDIA_STOP
	{ 0xb3,				_T("media-play-pause") },		// VK_MEDIA_PLAY_PAUSE
	{ 0xb4,				_T("launch-mail") },			// VK_LAUNCH_MAIL
	{ 0xb5,				_T("launch-media-select") },	// VK_LAUNCH_MEDIA_SELECT
	{ 0xb6,				_T("launch-1") },				// VK_LAUNCH_APP1
	{ 0xb7,				_T("launch-2") },				// VK_LAUNCH_APP2
};
static const int MAX_KEYNAME = _countof(KeyNames);

static AFX_EXTENSION_MODULE XkeymacsdllDLL = { NULL, NULL };

static HINSTANCE g_hDllInst = NULL;
static DWORD g_TlsIndex = NULL;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	g_hDllInst = hInstance;
	LPVOID lpData;
	
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
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

		if ((g_TlsIndex = TlsAlloc()) == TLS_OUT_OF_INDEXES)
			return FALSE;
		// fall through
	case DLL_THREAD_ATTACH:
		if ((lpData = LocalAlloc(LPTR, sizeof(HHOOK))) != NULL)
			TlsSetValue(g_TlsIndex, lpData);
		break;
	case DLL_PROCESS_DETACH:
		TRACE0("XKEYMACSDLL.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(XkeymacsdllDLL);
		CXkeymacsDll::ReleaseKeyboardHook();
		if ((lpData = TlsGetValue(g_TlsIndex)) != NULL)
			LocalFree(lpData);
		TlsFree(g_TlsIndex);
		break;
	case DLL_THREAD_DETACH:
		CXkeymacsDll::ReleaseKeyboardHook();
		if ((lpData = TlsGetValue(g_TlsIndex)) != NULL)
			LocalFree(lpData);
		break;
	}
	return 1;   // ok
}

//////////////////////////////////////////////////////////////////////
// CXkeymacsDll Class
//////////////////////////////////////////////////////////////////////

#include "xkeymacsDll.h"
#pragma data_seg(".xkmcs")
	bool	CXkeymacsDll::m_bEnableKeyboardHook = false;
	DWORD	CXkeymacsDll::m_nHookAltRelease	= 0;
	BOOL	CXkeymacsDll::m_bRightControl	= FALSE;
	BOOL	CXkeymacsDll::m_bRightAlt		= FALSE;
	BOOL	CXkeymacsDll::m_bRightShift		= FALSE;
	BOOL	CXkeymacsDll::m_bHook			= TRUE;
	CList<CClipboardSnap *, CClipboardSnap *> CXkeymacsDll::m_oKillRing;
	int		CXkeymacsDll::m_nKillRing = 0;
	BYTE	CXkeymacsDll::m_nOriginal[MAX_COMMAND_TYPE][MAX_KEY] = {'\0'};
	int		CXkeymacsDll::m_nAppID = 0;
	int		CXkeymacsDll::m_nAccelerate = 0;
	int		CXkeymacsDll::m_nKeyboardSpeed = 31;
	HCURSOR	CXkeymacsDll::m_hCursor[MAX_STATUS] = {'\0'};
	HCURSOR CXkeymacsDll::m_hCurrentCursor = NULL;
	BOOL	CXkeymacsDll::m_bCursor = FALSE;
	TCHAR	CXkeymacsDll::m_M_xTip[128] = "";
	CONFIG	CXkeymacsDll::m_Config = {0};
#pragma data_seg()
HHOOK CXkeymacsDll::m_hHookCallWnd = NULL;
HHOOK CXkeymacsDll::m_hHookCallWndRet = NULL;
HHOOK CXkeymacsDll::m_hHookGetMessage = NULL;
HHOOK CXkeymacsDll::m_hHookShell = NULL;
BOOL CXkeymacsDll::m_bRecordingMacro = FALSE;
BOOL CXkeymacsDll::m_bDown[MAX_KEY] = {0};
std::list<KbdMacro> CXkeymacsDll::m_Macro;

BOOL CXkeymacsDll::SaveConfig()
{
	TCHAR szTmp[MAX_PATH];
	if (!GetTempPath(MAX_PATH, szTmp))
		return FALSE;
	if (_tmakepath_s(szTmp, NULL, szTmp, _T("xkeymacs"), _T("tmp")))
		return FALSE;
	HANDLE hFile = CreateFile(szTmp, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD written;
	BOOL res = WriteFile(hFile, &m_Config, sizeof(m_Config), &written, NULL) || written != sizeof(m_Config);
	CloseHandle(hFile);
	return res;
}

BOOL CXkeymacsDll::LoadConfig()
{
	TCHAR szTmp[MAX_PATH];
	if (!GetTempPath(MAX_PATH, szTmp))
		return FALSE;
	if (_tmakepath_s(szTmp, NULL, szTmp, _T("xkeymacs"), _T("tmp")))
		return FALSE;
	HANDLE hFile = CreateFile(szTmp, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD read;
	BOOL res = ReadFile(hFile, &m_Config, sizeof(m_Config), &read, NULL) && read == sizeof(m_Config);
	CloseHandle(hFile);
	return res;
}

void CXkeymacsDll::SetConfig(const CONFIG& config)
{
	m_Config = config;
}

void CXkeymacsDll::SetHooks()
{
	m_hHookCallWnd = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, g_hDllInst, 0);
	m_hHookCallWndRet = SetWindowsHookEx(WH_CALLWNDPROCRET, CallWndRetProc, g_hDllInst, 0);
	m_hHookGetMessage = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, g_hDllInst, 0);
	m_hHookShell = SetWindowsHookEx(WH_SHELL, ShellProc, g_hDllInst, 0);
	m_bEnableKeyboardHook = true;
}

void CXkeymacsDll::SetKeyboardHook()
{
	HHOOK *phHook = reinterpret_cast<HHOOK *>(TlsGetValue(g_TlsIndex));
	if (!phHook)
		return;
	if (!*phHook)
		*phHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, g_hDllInst, GetCurrentThreadId());
}

inline void unhook(HHOOK &hh)
{
	if (hh)
		UnhookWindowsHookEx(hh);
	hh = NULL;
}

void CXkeymacsDll::ResetHooks() 
{
	ReleaseHooks();
	SetHooks();
}

void CXkeymacsDll::ReleaseHooks()
{
	unhook(m_hHookCallWnd);
	unhook(m_hHookCallWndRet);
	unhook(m_hHookGetMessage);
	unhook(m_hHookShell);
	m_bEnableKeyboardHook = false;
}

void CXkeymacsDll::ReleaseKeyboardHook()
{
	HHOOK *phHook = reinterpret_cast<HHOOK *>(TlsGetValue(g_TlsIndex));
	if (phHook)
		unhook(*phHook);
}

void CXkeymacsDll::ToggleKeyboardHookState()
{
	m_bHook = !m_bHook;
	ShowKeyboardHookState();
}

void CXkeymacsDll::ShowKeyboardHookState()
{
	ICONMSG msg = {MAIN_ICON,};
	if (m_bHook) {
		if (CCommands::IsTemporarilyDisableXKeymacs()) {
			msg.nState = STATUS_DISABLE_TMP;
			m_hCurrentCursor = m_hCursor[STATUS_DISABLE_TMP];
		} else {
			msg.nState = STATUS_ENABLE;
			m_hCurrentCursor = m_hCursor[STATUS_ENABLE];
		}
	} else {
		msg.nState = STATUS_DISABLE_WOCQ;
	}
	if (m_Config.nSettingStyle[m_nAppID] == SETTING_DISABLE
	 || (!_tcsicmp(m_Config.szSpecialApp[m_nAppID], _T("Default"))
	  && CUtils::IsDefaultIgnoreApplication())) {
		msg.nState = STATUS_DISABLE;
		m_hCurrentCursor = m_hCursor[STATUS_DISABLE];
	}
	SendIconMessage(&msg, 1);
	DoSetCursor();
}

BOOL CXkeymacsDll::IsKeyboardHook()
{
	return m_bHook;
}

LRESULT CALLBACK CXkeymacsDll::CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	SetKeyboardHook();
	if (nCode >= 0) {
		const CWPSTRUCT *cwps = reinterpret_cast<CWPSTRUCT *>(lParam);
		switch (cwps->message) {
		case WM_IME_STARTCOMPOSITION:
			InitKeyboardProc(TRUE);
			break;
		case WM_IME_ENDCOMPOSITION:
			InitKeyboardProc(FALSE);
			break;
		case WM_SETFOCUS:
			if (cwps->hwnd == GetForegroundWindow()) {
				InitKeyboardProc(FALSE);
				ShowKeyboardHookState();
			}
			break;
		case WM_NCACTIVATE:
			if (cwps->wParam && cwps->hwnd == GetForegroundWindow()) {
				InitKeyboardProc(FALSE);
				ShowKeyboardHookState();
			}
			break;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK CXkeymacsDll::CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	SetKeyboardHook();
	if (nCode >= 0) {
		const CWPRETSTRUCT *cwprets = reinterpret_cast<CWPRETSTRUCT *>(lParam);
		switch (cwprets->message) {
		case WM_SETTEXT:
			if (cwprets->hwnd == GetForegroundWindow())
				InitKeyboardProc(FALSE);
			break;
		case WM_SETCURSOR:
			DoSetCursor();
			break;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK CXkeymacsDll::GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	SetKeyboardHook();
	if (nCode >= 0) {
		const MSG *msg = reinterpret_cast<MSG *>(lParam);
		switch (msg->message) {
		case WM_IME_STARTCOMPOSITION:
			InitKeyboardProc(TRUE);
			break;
		case WM_IME_ENDCOMPOSITION:
			InitKeyboardProc(FALSE);
			break;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK CXkeymacsDll::ShellProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HSHELL_WINDOWACTIVATED) {
		TCHAR className[256];
		GetClassName((HWND)wParam, className, 255);
		if (!_tcsicmp(className, _T("ConsoleWindowClass"))) {
			InitKeyboardProc(FALSE);
			ShowKeyboardHookState();
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

UINT CXkeymacsDll::GetModifierState(BOOL bPhysicalKey)
{
	UINT result = 0;
	if (IsDown(VK_SHIFT, bPhysicalKey))
		result |= SHIFT;
	if (IsDown(VK_CONTROL, bPhysicalKey))
		result |= CONTROL;
	if (IsDown(VK_MENU, bPhysicalKey))
		result |= META;
	return result;
}

void CXkeymacsDll::SetModifierState(const UINT after, const UINT before)
{
	if (after & SHIFT && !(before & SHIFT))
		DepressKey(VK_SHIFT);
	else if (!(after & SHIFT) && before & SHIFT)
		ReleaseKey(VK_SHIFT);

	if (after & CONTROL && !(before & CONTROL)) {
		UpdateKeyboardState(VK_CONTROL, 1);
		DepressKey(VK_CONTROL);
	} else if (!(after & CONTROL) && before & CONTROL) {
		ReleaseKey(VK_CONTROL);
		UpdateKeyboardState(VK_CONTROL, 0);
	}

	const BOOL bHookApp =
		CUtils::IsVisualCpp() ||  CUtils::IsVisualStudio() ||
		CUtils::IsInternetExplorer() || CUtils::IsFirefox() || CUtils::IsChrome();
	if (after & META && !(before & META)) {
		if (bHookApp)
			m_nHookAltRelease |= HOOK_ALT_LATER;
		DepressKey(VK_MENU);
	} else if (!(after & META) && before & META) {
		if (bHookApp)
			++m_nHookAltRelease;
		ReleaseKey(VK_MENU);
	}
}

BOOL CXkeymacsDll::UpdateKeyboardState(BYTE bVk, BYTE bState)
{
	BYTE ks[256] = {'\0'};
	if (!GetKeyboardState(ks))
		return FALSE;
	ks[bVk] = bState;
	return SetKeyboardState(ks);
}

BOOL CXkeymacsDll::IsDown(BYTE bVk, BOOL bPhysicalKey)
{
	return bPhysicalKey ? GetAsyncKeyState(bVk) < 0 : GetKeyState(bVk) < 0;
}

// Do keybd_event
void CXkeymacsDll::DoKeybd_event(BYTE bVk, DWORD dwFlags)
{
	// Set KEYEVENTF_EXTENDEDKEY if needed
	switch (bVk) {
	case VK_CONTROL:
		if (m_bRightControl)
			dwFlags |= KEYEVENTF_EXTENDEDKEY;
		break;

	case VK_MENU:
		if (m_bRightAlt)
			dwFlags |= KEYEVENTF_EXTENDEDKEY;
		break;

	case VK_SHIFT:
		if (m_bRightShift) {
			if (CUtils::IsXPorLater())
				bVk = VK_RSHIFT;
			else
				dwFlags |= KEYEVENTF_EXTENDEDKEY;
		}
		break;
	case VK_PAUSE:
		if (IsDown(VK_CONTROL, FALSE)) // Break
			dwFlags |= KEYEVENTF_EXTENDEDKEY;
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
	}
//	CUtils::Log(_T("b: %x, %x, %x, %#hx, %#hx"), bVk, dwFlags, GetMessageExtraInfo(), GetKeyState(bVk), GetAsyncKeyState(bVk));
	keybd_event(bVk, 0, dwFlags, GetMessageExtraInfo());
//	CUtils::Log(_T("a: %x, %x, %x, %#hx, %#hx"), bVk, dwFlags, GetMessageExtraInfo(), GetKeyState(bVk), GetAsyncKeyState(bVk));
}

// the key is being depressed
void CXkeymacsDll::DepressKey(BYTE bVk, BOOL bOriginal)	// bVk is virtual-key code, MSDN said
{
	if (bOriginal) {
//		CUtils::Log(_T("i: %x, %d, %d, %d, %d, %d, %d, %d, %d"), bVk,
//			IsDown(VK_CONTROL), IsDown(VK_CONTROL, FALSE), IsDepressedModifier(CCommands::C_), IsDepressedModifier(CCommands::C_, FALSE),
//			IsDown(VK_MENU), IsDown(VK_MENU, FALSE), IsDepressedModifier(CCommands::MetaAlt), IsDepressedModifier(CCommands::MetaAlt, FALSE));
		SetOriginal(GetModifierState(), bVk);
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
	CUtils::SetApplicationName(bImeComposition);
	if (_tcsnicmp(m_Config.szSpecialApp[m_nAppID], CUtils::GetApplicationName(), 0xF) || !IsMatchWindowText(m_Config.szWindowText[m_nAppID])) {	// PROCESSENTRY32 has only 0xF bytes of Name
		m_nAppID = -1;
		for (int nAppID = 0; nAppID < MAX_APP; ++nAppID) {
			if (_tcsnicmp(m_Config.szSpecialApp[nAppID], CUtils::GetApplicationName(), 0xF) || !IsMatchWindowText(m_Config.szWindowText[nAppID]))
				continue;
			if (m_nAppID < 0)
				m_nAppID = nAppID;
			else {
				const LPCSTR curText = m_Config.szWindowText[m_nAppID];
				const LPCSTR newText = m_Config.szWindowText[nAppID];
				const int curType = CUtils::GetWindowTextType(curText);
				const int newType = CUtils::GetWindowTextType(newText);
				if (curType < newType || curType == newType && _tcscmp(curText, newText) <= 0)
					m_nAppID = nAppID;
			}
		}
		if (m_nAppID < 0)
			m_nAppID = GetAppID(_T("Default"), 0);
	}
	if (m_Config.nSettingStyle[m_nAppID] != SETTING_DISABLE &&
			(_tcsicmp(m_Config.szSpecialApp[m_nAppID], _T("Default")) || !CUtils::IsDefaultIgnoreApplication()) &&
			!bImeComposition && CUtils::IsDialog() && m_Config.bUseDialogSetting[m_nAppID])
		// Use Dialog Setting
		m_nAppID = GetAppID(_T("Dialog"), m_nAppID);

	ICONMSG msg[3] = {
		{CX_ICON, OFF_ICON, ""},
		{MX_ICON, OFF_ICON, ""},
		{META_ICON, OFF_ICON, ""}
	};
	SendIconMessage(msg, 3);
	CCommands::SetMark(FALSE);
	CCommands::SetTemporarilyDisableXKeymacs(FALSE);
	CCommands::Reset();
}

int CXkeymacsDll::GetAppID(const LPCSTR szName, const int fallback)
{
	for (int i = 0; i < MAX_APP; ++i)
		if (!_tcsicmp(m_Config.szSpecialApp[i], szName))
			return i;
	return fallback;
}

LRESULT CALLBACK CXkeymacsDll::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	const BYTE nOrigKey = static_cast<BYTE>(wParam);
	const bool bRelease = (lParam & BEING_RELEASED) != 0;
	const bool bExtended = (lParam & EXTENDED_KEY) != 0;

	static BOOL bLocked = FALSE;
	static const BYTE RECURSIVE_KEY = 0x07;
	static int (*fLastCommand)() = NULL;
	static BYTE nOneShotModifier[MAX_KEY] = {'\0'};
	static BOOL bCherryOneShotModifier = FALSE;

//	CUtils::Log(_T("nCode = %#x, nKey = %#x, lParam = %#x"), nCode, nOrigKey, lParam);

	if (!m_bEnableKeyboardHook || CUtils::IsXkeymacs() ||
			nCode < 0 || nCode == HC_NOREMOVE)
		return CallNextHookEx(NULL, nCode, wParam, lParam);

//	CUtils::Log(_T("nKey = %#x, ext = %d, rel = %d, pre = %d, %#hx, %#hx"), nOrigKey,
//		(lParam & EXTENDED_KEY) ? 1 : 0, (lParam & BEING_RELEASED) ? 1 : 0, (lParam & REPEATED_KEY) ? 1 : 0,
//		GetKeyState(nOrigKey), GetAsyncKeyState(nOrigKey));

	if (nOrigKey == RECURSIVE_KEY) {
		if (bRelease)
			goto HOOK_RECURSIVE_KEY;
		else
			goto RECURSIVE_COMMAND;
	}

	CancelMarkWithShift(nOrigKey, bRelease);

	BYTE nKey = nOrigKey;
	switch (nKey) {
	case VK_CONTROL:
		nKey = bExtended ? VK_RCONTROL : VK_LCONTROL;
		break;
	case VK_MENU:
		nKey = bExtended ? VK_RMENU : VK_LMENU;
		break;
	case VK_SHIFT:
		nKey = bExtended ? VK_RSHIFT : VK_LSHIFT;
		break;
	}

#define fCommand(type) (Commands[m_Config.nCommandID[m_nAppID][(type)][nKey]].fCommand)
#define nFunctionID (m_Config.nFunctionID[m_nAppID][nType][nKey])

	if (bRelease) {
		switch (nOrigKey) {
		case VK_MENU:
			if (m_nHookAltRelease) {
				if (m_nHookAltRelease & ~HOOK_ALT_LATER)
					m_nHookAltRelease--;
				else if (m_nHookAltRelease & HOOK_ALT_LATER)
					m_nHookAltRelease = 0;
				goto HOOK;
			}
			// pass through
		case VK_LWIN:
		case VK_RWIN:
		case VK_APPS:
			for (int i = 0; i < MAX_COMMAND_TYPE; ++i) {
				int (*const fCommand)() = fCommand(i);
				if (fCommand && !(nOrigKey == VK_MENU && fCommand == CCommands::MetaAlt))
					goto HOOK;
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

	if (m_Config.nSettingStyle[m_nAppID] == SETTING_DISABLE)
		goto DO_NOTHING;

	// Do Nothing for Meadow, Mule for Win32, ... if those use default setting.
	if (!_tcsicmp(m_Config.szSpecialApp[m_nAppID], _T("Default")) && CUtils::IsDefaultIgnoreApplication())
		goto DO_NOTHING;

	switch (IsPassThrough(nKey)) {
	case GOTO_DO_NOTHING:
		goto DO_NOTHING;
	case GOTO_HOOK:
		goto HOOK;
	case CONTINUE:
		break;
	}

	// set command type
	int nType = IsDown(VK_SHIFT) * SHIFT | IsControl() * CONTROL | IsMeta() * META | CCommands::bC_x() * CONTROLX;
	// Ignore undefined C-x ?
	if (nType & CONTROLX && fCommand(nType) == NULL && nFunctionID < 0) {
		if (m_Config.bIgnoreUndefinedC_x[m_nAppID]) {
			CCommands::Reset(GOTO_HOOK);
			goto HOOK;
		}
		nType &= ~CONTROLX;
	}
	// Ignore undefined Meta Ctrl+?
	if (CCommands::bM_() && nType & CONTROL && fCommand(nType) == NULL && nFunctionID < 0) {
		if (m_Config.bIgnoreUndefinedMetaCtrl[m_nAppID]) {
			if (CheckOriginal(CONTROL, nKey))
				goto DO_NOTHING;
			CCommands::Reset(GOTO_HOOK);
			goto HOOK;
			nType &= ~META;
		}
	}

	int nVirtualType = GetModifierState(FALSE);
	if (nOrigKey == VK_CONTROL)
		nVirtualType &= ~CONTROL;
	if (nOrigKey == VK_MENU)
		nVirtualType &= ~META;
	if (CheckOriginal(nVirtualType, nOrigKey)) {
		goto DO_NOTHING;
	}

	int (*const fCommand)() = fCommand(nType);
	if (fCommand == CCommands::EnableOrDisableXKeymacs) {
		ToggleKeyboardHookState();
		goto HOOK;
	}
	if (fCommand == CCommands::EnableXKeymacs) {
		if (!m_bHook)
			ToggleKeyboardHookState();
		goto HOOK;
	}
	if (fCommand == CCommands::DisableXKeymacs) {
		if (m_bHook)
			ToggleKeyboardHookState();
		goto HOOK;
	}
	if (!m_bHook)
		goto DO_NOTHING;

	if (CCommands::bM_x() && !bRelease) {
		static size_t index = 0;
		static TCHAR szPath[MAX_PATH] = {'\0'};
		if (fCommand == CCommands::BackwardChar) {
			if (index)
				--index;
			goto HOOKX;
		} else if (fCommand == CCommands::BeginningOfLine) {
			index = 0;
			goto HOOKX;
		} else if (fCommand == CCommands::DeleteBackwardChar) {
			if (index) {
				--index;
				memmove(szPath + index, szPath + index + 1, MAX_PATH - index);
				SetM_xTip(szPath);
			}
			goto HOOKX;
		} else if (fCommand == CCommands::DeleteChar) {
			if (index < _tcslen(szPath)) {
				memmove(szPath + index, szPath + index + 1, MAX_PATH - index);
				SetM_xTip(szPath);
			}
			goto HOOKX;
		} else if (fCommand == CCommands::EndOfLine) {
			index = _tcslen(szPath);
			goto HOOKX;
		} else if (fCommand == CCommands::ForwardChar) {
			if (index < _tcslen(szPath))
				++index;
			goto HOOKX;
		} else if (fCommand == CCommands::KeyboardQuit) {
			CCommands::bM_x(FALSE);
			index = 0;
			memset(szPath, 0, sizeof(szPath));
			goto HOOK;
		} else if (nKey == VK_RETURN || fCommand == CCommands::Newline) {
			InvokeM_x(szPath);
			CCommands::bM_x(FALSE);
			index = 0;
			memset(szPath, 0, sizeof(szPath));
			goto HOOK;
		} else if (nKey && index < MAX_PATH - 1) {
			const BOOL bIsShiftDown = IsDown(VK_SHIFT, FALSE);
			TCHAR nAscii = 0;
			do { // 1-127
				if (a2v(++nAscii) == nKey && bIsShiftDown == IsShift(nAscii)) {
//					CUtils::Log("M-x: %#X (%c), %#X (%c)", nKey, nKey, nAscii, nAscii);
					if (index < _tcslen(szPath))
						memmove(szPath + index + 1, szPath + index, MAX_PATH - index - 1);
					szPath[index++] = nAscii;
//					CUtils::Log("M-x: %c(%#04x)", nAscii, nAscii);
					SetM_xTip(szPath);
					goto HOOKX;
				}
			} while (nAscii != 127);
		}
	}

	if (CCommands::bC_u() && nType == NONE) {
		if ('0' <= nKey && nKey <= '9') {
			CCommands::NumericArgument(nKey - '0');
			goto HOOK0_9;
		}
		if (nKey == VK_OEM_MINUS) {
			CCommands::NumericArgumentMinus();
			goto HOOK0_9;
		}
	}

#define OneShotModifier(type, vk, mod) \
	if (fCommand(nType & ~type) == CCommands::OneShotModifier ## mod || \
			fCommand(nType) == CCommands::OneShotModifier ## mod ## Repeat) { \
		nOneShotModifier[nKey] = vk; \
		DepressKey(vk); \
		bCherryOneShotModifier = TRUE; \
		goto HOOK; \
	} else if (fCommand(nType & ~CONTROL) == CCommands::OneShotModifier ## mod ## Repeat) { \
		ReleaseKey(vk); \
		bCherryOneShotModifier = FALSE; \
		Kdu(nKey); \
		goto HOOK; \
	}

	OneShotModifier(CONTROL, VK_CONTROL, Ctrl);
	OneShotModifier(META, VK_MENU, Alt);
	OneShotModifier(SHIFT, VK_SHIFT, Shift);
	int i;
	for (i = 0; i < MAX_KEY; ++i)
		if (nOneShotModifier[i] == nOrigKey)
			break;
	if (i == MAX_KEY)
		bCherryOneShotModifier = FALSE;

	if (0 <= nFunctionID && nFunctionID < MAX_FUNCTION && m_Config.szFunctionDefinition[nFunctionID][0]) {
		CallFunction(nFunctionID);
		CCommands::Reset(GOTO_HOOK);
		goto HOOK;
	}
#undef fCommand
#undef nFunctionID

	if (!fCommand) {
		if (nOrigKey == VK_CONTROL || nOrigKey == VK_MENU || nOrigKey == VK_SHIFT)
			goto DO_NOTHING;
		if (!(nType & SHIFT)) {
			if (CCommands::IsSetMark()) {
				if (CCommands::MoveCaret(nKey, nType & CONTROL) != CONTINUE) {
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

	if (CCommands::IsTemporarilyDisableXKeymacs()  && fCommand != CCommands::KeyboardQuit) {
		CCommands::SetTemporarilyDisableXKeymacs(FALSE);
		goto DO_NOTHING;
	}

	m_bRightControl = IsDown(VK_RCONTROL, FALSE);
	m_bRightAlt = IsDown(VK_RMENU, FALSE);
	m_bRightShift = IsDown(VK_RSHIFT, FALSE);

	if (bLocked)
		goto HOOK_RECURSIVE_KEY;
	bLocked = TRUE;
	fLastCommand = fCommand;
RECURSIVE_COMMAND:
	switch (fLastCommand()) {
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
	}

DO_NOTHING:
	SetModifierIcons();
	if (m_bRecordingMacro && (!bRelease || m_bDown[wParam])) {
		KbdMacro m = { nCode, wParam, lParam, TRUE };
		m_Macro.push_back(m);
		m_bDown[wParam] |= !bRelease;
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);

RECURSIVE:
	Kdu(RECURSIVE_KEY, 1, FALSE);
	goto HOOKX;
HOOK:
	CCommands::SetLastCommand(fLastCommand);
HOOK0_9:
HOOKX:
	SetModifierIcons();
HOOK_RECURSIVE_KEY:
	return TRUE;
}

void CXkeymacsDll::SetModifierIcons()
{
#define IconState(x) ((x) ? ON_ICON : OFF_ICON)
	ICONMSG msg[6] = {
		{MX_ICON, IconState(CCommands::bM_x()), ""},
		{CX_ICON, IconState(CCommands::bC_x()), ""},
		{META_ICON, IconState(CCommands::bM_()), ""},
		{SHIFT_ICON, IconState(IsDown(VK_SHIFT, FALSE)), ""},
		{CTRL_ICON, IconState(IsControl()), ""},
		{ALT_ICON, IconState(IsDown(VK_MENU, FALSE)), ""}
	};
	_tcscpy_s(msg[0].szTip, m_M_xTip);
	SendIconMessage(msg, 6);
}

// Clear data of nAppID
void CXkeymacsDll::Clear(int nAppID)
{
	if (0 <= nAppID && nAppID < MAX_APP) {
		ZeroMemory(m_Config.szSpecialApp[nAppID], sizeof(m_Config.szSpecialApp[nAppID]));
		ZeroMemory(m_Config.nCommandID[nAppID], sizeof(m_Config.nCommandID[nAppID]));
		m_Config.nKillRingMax[nAppID] = 0;
		m_Config.bUseDialogSetting[nAppID] = FALSE;
		m_Config.nSettingStyle[nAppID] = 0;
	} else {
		ASSERT(0);
	}
}

BOOL CXkeymacsDll::IsValidKey(BYTE bVk)
{
	if (bVk == 0xf0) {	// 0xf0: Eisu key. GetAsyncKeyState returns the wrong state of Eisu key.
		return FALSE;
	}

	switch (bVk) {
	case VK_CONTROL:
	case VK_MENU:
	case VK_SHIFT:
		return FALSE;
	default:
		break;
	}

	return TRUE;
}

BOOL CXkeymacsDll::IsDepressedModifier(int (__cdecl *Modifier)(void), BOOL bPhysicalKey)
{
	BYTE bVk = 0;
	do {
		if (IsValidKey(bVk) && IsDown(bVk, bPhysicalKey) &&
				Commands[m_Config.nCommandID[m_nAppID][NONE][bVk]].fCommand == Modifier)
			return TRUE;
	} while (++bVk);
	return FALSE;
}

void CXkeymacsDll::CancelMarkWithShift(BYTE nKey, bool bRelease)
{
	static bool bShift;
	if (nKey != VK_SHIFT)
		goto exit;
	BYTE bVk = 0;
	do {
		if (bVk == VK_SHIFT || VK_LSHIFT || VK_RSHIFT)
			continue;
		if (IsDown(bVk, FALSE))
			goto exit;
	} while (++bVk);
	if (!bRelease) {
		bShift = TRUE;
		return;
	}
	if (bShift)
		CCommands::SetMark(FALSE);
exit:
	bShift = FALSE;
	return;
}

BOOL CXkeymacsDll::IsControl()
{
	return CCommands::bC_() || IsDepressedModifier(CCommands::C_);
}

BOOL CXkeymacsDll::IsMeta()
{
	return CCommands::bM_() || IsDepressedModifier(CCommands::MetaAlt);
}

void CXkeymacsDll::AddKillRing(BOOL bNewData)
{
	if (m_Config.nKillRingMax[m_nAppID] == 0) {
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
				CClipboardSnap *pParent;
				for (pParent = m_oKillRing.GetHead(); pParent->GetNext(); pParent = pParent->GetNext()) {
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

	if (m_Config.nKillRingMax[m_nAppID] < m_oKillRing.GetCount()) {
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
	if (m_Config.nKillRingMax[m_nAppID] == 0) {
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

void CXkeymacsDll::SetOriginal(UINT nType, BYTE bVk)
{
	m_nOriginal[nType & ~SHIFT][bVk]++;
}

int CXkeymacsDll::CheckOriginal(UINT nType, BYTE bVk)
{
	nType &= ~SHIFT;
	if (m_nOriginal[nType][bVk])
		return m_nOriginal[nType][bVk]--;
	return 0;
}

void CXkeymacsDll::IncreaseKillRingIndex(int nKillRing)
{
	m_nKillRing += nKillRing;
}

BOOL CXkeymacsDll::GetEnableCUA()
{
	return m_Config.bEnableCUA[m_nAppID];
}

void CXkeymacsDll::StartRecordMacro()
{
	if (CCommands::bC_u())
		CallMacro();
	m_bRecordingMacro = TRUE;
	m_Macro.erase(m_Macro.begin(), m_Macro.end());
	ZeroMemory(m_bDown, MAX_KEY);
}

void CXkeymacsDll::EndRecordMacro()
{
	m_bRecordingMacro = FALSE;
	while (!m_Macro.empty()) { // remove not released push
		const KbdMacro& m = m_Macro.back();
		if (m.lParam & BEING_RELEASED)
			break;
		m_Macro.pop_back();
	}
}

void CXkeymacsDll::CallMacro()
{
	if (m_bRecordingMacro)
		m_bRecordingMacro = FALSE;
	UINT before = GetModifierState(FALSE);
	SetModifierState(0, before);
	for (std::list<KbdMacro>::const_iterator m = m_Macro.begin(); m != m_Macro.end(); ++m)
		if (m->lParam & BEING_RELEASED)
			ReleaseKey(static_cast<BYTE>(m->wParam));
		else
			DepressKey(static_cast<BYTE>(m->wParam), m->bOriginal);
	SetModifierState(before, 0);
}

BOOL CXkeymacsDll::Is106Keyboard()
{
	return m_Config.b106Keyboard;
}

int CXkeymacsDll::IsPassThrough(BYTE nKey)
{
	BYTE bVk = 0;
	do {
		if (IsDown(bVk)
		 && (Commands[m_Config.nCommandID[m_nAppID][NONE][bVk]].fCommand == CCommands::PassThrough)) {
			if (bVk == nKey) {
				return GOTO_HOOK;
			}

			return GOTO_DO_NOTHING;
		}
	} while (++bVk);
	return CONTINUE;
}

// call an original command which is defined in dot.xkeymacs
void CXkeymacsDll::CallFunction(int nFuncID)
{
	if (nFuncID < 0 || nFuncID >= MAX_FUNCTION)
		return;
	LPCTSTR def = m_Config.szFunctionDefinition[nFuncID];
	if (!def[0])
		return;
	std::vector<KeyBind> keybinds;
	const LPCTSTR last = def + _tcslen(def) - 1;
	if (*def == _T('"') && *last == _T('"')) {
		++def; // skip '"'
		while (def < last)
			keybinds.push_back(ParseKey(def));
	} else if (*def == _T('[') && *last == _T(']')) {
		while (++def < last) { // skip '[', ']', and ' '
			if (*def == _T('?')) { // [?f ?o ?o]
				keybinds.push_back(ParseKey(++def));
				continue;
			}
			// [VK]
			for (int i = 0; i < MAX_KEYNAME; ++i) {
				size_t keylen = _tcslen(KeyNames[i].name);
				if (!_tcsncmp(def, KeyNames[i].name, keylen)) {
					KeyBind keybind = {NONE, KeyNames[i].bVk};
					keybinds.push_back(keybind);
					def += keylen;
					break;
				}
			}
		}
	} else
		return;

	BOOL bM_x = FALSE;
	TCHAR szPath[MAX_PATH] = {'\0'};
	unsigned int index = 0;
	BOOL bInitialized = FALSE;
	UINT before = GetModifierState(FALSE);

	for (std::vector<KeyBind>::const_iterator p = keybinds.begin(); p != keybinds.end(); ++p) {
		const int nType = p->nType;
		const BYTE bVk = p->bVk;
		int (*fCommand)() = nType < MAX_COMMAND_TYPE ? Commands[m_Config.nCommandID[m_nAppID][nType][bVk]].fCommand : NULL;
		if (fCommand) {
			if (fCommand == CCommands::ExecuteExtendedCommand)
				bM_x = TRUE;
			else if (!bInitialized) {
				SetModifierState(0, before);
				bInitialized = TRUE;
			}
//			CUtils::Log("CallFunction: Command Name: %s", Commands[m_Config.nCommandID[m_nAppID][nType][bVk]].szCommandName);
			while (fCommand() == GOTO_RECURSIVE)
				;
			continue;
		}
		if (bM_x) {
			if (bVk == VK_RETURN)
				InvokeM_x(szPath);
			else if (bVk != 0) {
				TCHAR nAscii = 0;
				do { // 1-127
					if (a2v(++nAscii) == bVk && ((nType & SHIFT) != 0) == IsShift(nAscii)) {
//						CUtils::Log("M-x: %#X (%c), %#X (%c)", bVk, bVk, nAscii, nAscii);
						szPath[index++] = nAscii;
						break;
					}
				} while (nAscii != 127);
			}
			continue;
		}
		if (!bInitialized) {
			SetModifierState(0, before);
			bInitialized = TRUE;
		}
		if (nType & WIN_WIN)
			DepressKey(VK_LWIN);
		if (nType & WIN_CTRL)
			DepressKey(VK_CONTROL);
		if (nType & WIN_ALT)
			DepressKey(VK_MENU);
		if (nType & SHIFT)
			DepressKey(VK_SHIFT);
		Kdu(bVk);
		const int nNextType = (p + 1) != keybinds.end() ? (p + 1)->nType : 0;
		if (nType & SHIFT && !(nNextType & SHIFT))
			ReleaseKey(VK_SHIFT);
		if (nType & WIN_ALT && !(nNextType & WIN_ALT))
			ReleaseKey(VK_MENU);
		if (nType & WIN_CTRL && !(nNextType & WIN_CTRL))
			ReleaseKey(VK_CONTROL);
		if (nType & WIN_WIN && !(nNextType & WIN_WIN))
			ReleaseKey(VK_LWIN);
	}

	if (bInitialized)
		// If this lines is invoked on M-x, a window transition does not work well.
		SetModifierState(before, 0);
	return;
}

KeyBind CXkeymacsDll::ParseKey(LPCTSTR& def)
{
	KeyBind keybind = {NONE};
	if (*def == _T('\\')) { // set modifiers
		++def;
	LOOP:
		for (int i = 0; i < MAX_MODIFIER; ++i) {
			size_t len = _tcslen(Modifiers[i].name);
			if (!_tcsncmp(def, Modifiers[i].name, len)) {
				keybind.nType |= Modifiers[i].id;
				def += len;
				goto LOOP;
			}
		}
	}
	if (IsShift(*def) && !(keybind.nType & (WIN_CTRL | WIN_ALT | WIN_WIN)))
		keybind.nType |= SHIFT;
	int i = 0;
	for (; i < MAX_KEYNAME; ++i) {
		size_t len = _tcslen(KeyNames[i].name);
		if (!_tcsncmp(def, KeyNames[i].name, len)) {
			def += len;
			break;
		}
	}
	keybind.bVk = i < MAX_KEYNAME ? KeyNames[i].bVk : a2v(*def++);
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
		return m_Config.b106Keyboard;
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
		return !m_Config.b106Keyboard;
	case _T(';'):
		return FALSE;
	case _T('<'):
		return TRUE;
	case _T('='):
		return m_Config.b106Keyboard;
	case _T('>'):
	case _T('?'):
		return TRUE;
	case _T('@'):
		return !m_Config.b106Keyboard;
	case _T('A'): case _T('B'): case _T('C'): case _T('D'): case _T('E'): case _T('F'): case _T('G'): case _T('H'): case _T('I'): case _T('J'): 
	case _T('K'): case _T('L'): case _T('M'): case _T('N'): case _T('O'): case _T('P'): case _T('Q'): case _T('R'): case _T('S'): case _T('T'): 
	case _T('U'): case _T('V'): case _T('W'): case _T('X'): case _T('Y'): case _T('Z'): 
		return TRUE;
	case _T('['):
	case _T('\\'):
	case _T(']'):
		return FALSE;
	case _T('^'):
		return !m_Config.b106Keyboard;
	case _T('_'):
		return TRUE;
	case _T('`'):
		return m_Config.b106Keyboard;
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
		return m_Config.b106Keyboard ? '2' : (BYTE) 0xde;	// VK_OEM_7
	case _T('#'):
		return '3';
	case _T('$'):
		return '4';
	case _T('%'):
		return '5';
	case _T('&'):
		return m_Config.b106Keyboard ? '6' : '7';
	case _T('\''):
		return m_Config.b106Keyboard ? '7' : (BYTE) 0xde;	// VK_OEM_7
	case _T('('):
		return m_Config.b106Keyboard ? '8' : '9';
	case _T(')'):
		return m_Config.b106Keyboard ? '9' : '0';
	case _T('*'):
		return m_Config.b106Keyboard ? (BYTE) 0xba : '8';	// VK_OEM_1
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
		return m_Config.b106Keyboard ? (BYTE) 0xbb : (BYTE) 0xba;	// VK_OEM_PLUS	VK_OEM_1
	case _T('<'):
		return 0xbc;	// VK_OEM_COMMA
	case _T('='):
		return m_Config.b106Keyboard ? (BYTE) 0xbd : (BYTE) 0xbb;	// VK_OEM_MINUS	VK_OEM_PLUS
	case _T('>'):
		return 0xbe;	// VK_OEM_PERIOD
	case _T('?'):
		return 0xbf;	// VK_OEM_2
	case _T('@'):
		return m_Config.b106Keyboard ? (BYTE) 0xc0 : '2';
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
		return m_Config.b106Keyboard ? (BYTE) 0xde : '6';	// VK_OEM_7
	case _T('_'):
		return m_Config.b106Keyboard ? (BYTE) 0xe2 : (BYTE) 0xbd;	// VK_OEM_102	VK_OEM_MINUS
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
		return m_Config.b106Keyboard ? (BYTE) 0xde : (BYTE) 0xc0;	// VK_OEM_7	VK_OEM_3
	default:
		return 0;
	}
}

BOOL CXkeymacsDll::IsMatchWindowText(CString szWindowText)
{
	BOOL bIsMatchWindowText = TRUE;

	TCHAR szCurrentWindowText[WINDOW_TEXT_LENGTH] = {'\0'};
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

BOOL CXkeymacsDll::Get326Compatible()
{
	return m_Config.b326Compatible[m_nAppID];
}

void CXkeymacsDll::InvokeM_x(const TCHAR *const szPath)
{
//	CUtils::Log("M-x: szPath=_%s_", szPath);
	int (*fCommand)() = NULL;

	for (int i = 0; i < MAX_COMMAND; ++i) {
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

void CXkeymacsDll::SetM_xTip(const TCHAR *const szPath)
{
	_tcscpy_s(m_M_xTip, "M-x LED");
	if (szPath && _tcslen(szPath) < 128 - 5)
		_stprintf_s(m_M_xTip, "M-x %s", szPath);
}

BOOL CXkeymacsDll::SendIconMessage(ICONMSG *pMsg, DWORD num)
{
	DWORD ack, read;
	return CallNamedPipe(ICON_PIPE, pMsg, sizeof(ICONMSG) * num, &ack, sizeof(DWORD), &read, NMPWAIT_NOWAIT);
}
