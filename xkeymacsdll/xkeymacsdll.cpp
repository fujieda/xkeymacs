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

static const KeyName ControlCharacters[] = {
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
//	{ VK_F2,			_T("f2") },						// Move at the end of definition of funcgtion keys to keep away confliction f1/f2 and f1?/f2? by _tcsncmp() i.e. strncmp()
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

static AFX_EXTENSION_MODULE XkeymacsdllDLL = { NULL, NULL };

HINSTANCE g_hDllInst = NULL;
UINT g_ImeManipulationMessage = 0;
#pragma data_seg(".xkmcs")
HHOOK g_hHookKeyboard = NULL;
HHOOK g_hHookDummy = NULL;
#pragma data_seg()

inline bool IsWow64(HANDLE mod) {
	typedef BOOL (WINAPI *pfnIsWow64Process_t)(HANDLE, PBOOL);
	if (const pfnIsWow64Process_t IsWow64Process = (pfnIsWow64Process_t)GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process")) {
		BOOL b;
		return IsWow64Process(mod, &b) && b;
	}
	return false;
}

inline bool Is64System() {
	SYSTEM_INFO info;
	GetNativeSystemInfo(&info);
	return info.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_INTEL;
}

inline bool Is64Process(HANDLE mod) {
	return Is64System() && !IsWow64(mod);
}

const bool IsDll64 = sizeof(void *) == 8;

inline bool Is64ProcessHwnd(HWND hwnd) {
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE hmod = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	bool b = Is64Process(hmod);
	CloseHandle(hmod);
	return b;
}

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	g_hDllInst = hInstance;
	
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		TRACE0("XKEYMACSDLL.DLL Initializing!\n");
		g_ImeManipulationMessage = RegisterWindowMessage(_T("XkManipulateIME"));

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
		break;
	case DLL_PROCESS_DETACH:
		TRACE0("XKEYMACSDLL.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(XkeymacsdllDLL);
		break;
	}
	return 1;   // ok
}

//////////////////////////////////////////////////////////////////////
// CXkeymacsDll Class
//////////////////////////////////////////////////////////////////////

#include "xkeymacsDll.h"
#pragma data_seg(".xkmcs")
	DWORD	CXkeymacsDll::m_nHookAltRelease	= 0;
	HHOOK	CXkeymacsDll::m_hHookCallWnd = NULL;
	HHOOK	CXkeymacsDll::m_hHookCallWndRet = NULL;
	HHOOK	CXkeymacsDll::m_hHookGetMessage = NULL;
	HHOOK	CXkeymacsDll::m_hHookShell = NULL;
	BOOL	CXkeymacsDll::m_bRightControl	= FALSE;
	BOOL	CXkeymacsDll::m_bRightAlt		= FALSE;
	BOOL	CXkeymacsDll::m_bRightShift		= FALSE;
	BOOL	CXkeymacsDll::m_bHook			= TRUE;
	CList<CClipboardSnap *, CClipboardSnap *> CXkeymacsDll::m_oKillRing;
	int		CXkeymacsDll::m_nKillRing = 0;
	int		CXkeymacsDll::m_nOriginal[MAX_COMMAND_TYPE][MAX_KEY] = {'\0'};
	int		CXkeymacsDll::m_nApplicationID = 0;
	int		CXkeymacsDll::m_nAccelerate = 0;
	int		CXkeymacsDll::m_nKeyboardSpeed = 31;
	HCURSOR	CXkeymacsDll::m_hCursor[MAX_STATUS] = {'\0'};
	HCURSOR CXkeymacsDll::m_hCurrentCursor = NULL;
	BOOL	CXkeymacsDll::m_bCursor = FALSE;
	TCHAR	CXkeymacsDll::m_M_xTip[128] = "";
	CONFIG	CXkeymacsDll::m_Config = {0};
#pragma data_seg()
BOOL CXkeymacsDll::m_bRecordingMacro = FALSE;
BOOL CXkeymacsDll::m_bDown[MAX_KEY] = {0};
CList<KbdMacro, KbdMacro&> CXkeymacsDll::m_Macro;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXkeymacsDll::CXkeymacsDll()
{

}

CXkeymacsDll::~CXkeymacsDll()
{

}

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

// set hooks
LRESULT WINAPI DummyProc(int code, WPARAM wp, LPARAM lp) {
	return CallNextHookEx(0, code, wp, lp);
}

void CXkeymacsDll::SetHooks()
{
	m_hHookCallWnd = SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)CallWndProc, g_hDllInst, 0);
	m_hHookCallWndRet = SetWindowsHookEx(WH_CALLWNDPROCRET, (HOOKPROC)CallWndRetProc, g_hDllInst, 0);
	m_hHookGetMessage = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)GetMsgProc, g_hDllInst, 0);
	m_hHookShell = SetWindowsHookEx(WH_SHELL, (HOOKPROC)ShellProc, g_hDllInst, 0);
	g_hHookDummy = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)DummyProc, g_hDllInst, 0);
	g_hHookKeyboard = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, g_hDllInst, 0);
}

inline void unhook(HHOOK &hh) {
	if (hh)
		UnhookWindowsHookEx(hh);
	hh = NULL;
}

void CXkeymacsDll::ResetHooks() 
{
	ReleaseHooks();
	SetHooks();
}

// release hooks
void CXkeymacsDll::ReleaseHooks()
{
	unhook(m_hHookCallWnd);
	unhook(m_hHookCallWndRet);
	unhook(m_hHookGetMessage);
	unhook(m_hHookShell);
	unhook(g_hHookKeyboard);
	unhook(g_hHookDummy);
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
	if (m_Config.nSettingStyle[m_nApplicationID] == SETTING_DISABLE
	 || (!_tcsicmp(m_Config.szSpecialApp[m_nApplicationID], _T("Default"))
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
	if (nCode < 0)
		CallNextHookEx(m_hHookCallWnd, nCode, wParam, lParam);
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
	return CallNextHookEx(m_hHookCallWnd, nCode, wParam, lParam);
}

LRESULT CALLBACK CXkeymacsDll::CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam)
{
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
	return CallNextHookEx(m_hHookCallWndRet, nCode, wParam, lParam);
}

LRESULT CALLBACK CXkeymacsDll::GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	const MSG *msg = reinterpret_cast<MSG *>(lParam);
	if (msg->message == g_ImeManipulationMessage) {
		if (wParam)
			CCommands::DoSetInputMethodOpenStatus((INPUT_METHOD_OPEN_STATUS)msg->wParam, msg->lParam);
		return 1;
	}
	switch (msg->message) {
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
	if (nCode == HSHELL_WINDOWACTIVATED) {
		TCHAR className[256];
		GetClassName((HWND)wParam, className, 255);
		if (!_tcsicmp(className, _T("ConsoleWindowClass"))) {
			InitKeyboardProc(FALSE);
			ShowKeyboardHookState();
		}
	}
	return CallNextHookEx( m_hHookShell, nCode, wParam, lParam );
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

void CXkeymacsDll::SetModifierState(UINT after, UINT before)
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

	BOOL bHookApp = CUtils::IsVisualCpp() || CUtils::IsFirefox() || CUtils::IsVisualStudio() || CUtils::IsInternetExplorer();
	if (after & META && !(before & META)) {
		if (bHookApp)
			m_nHookAltRelease |= HOOK_ALT_LATER;
		DepressKey(VK_MENU);
	} else if (!(after & META) && before & META) {
		if (bHookApp)
			m_nHookAltRelease++;
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

	if (_tcsnicmp(m_Config.szSpecialApp[m_nApplicationID], CUtils::GetApplicationName(), 0xF) || !IsMatchWindowText(m_Config.szWindowText[m_nApplicationID])) {	// PROCESSENTRY32 has only 0xF bytes of Name
		m_nApplicationID = -1;

		for (int nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {
			if (!_tcsnicmp(m_Config.szSpecialApp[nApplicationID], CUtils::GetApplicationName(), 0xF) && IsMatchWindowText(m_Config.szWindowText[nApplicationID])) {

				if (m_nApplicationID < 0
				 || CUtils::GetWindowTextType(m_Config.szWindowText[m_nApplicationID]) < CUtils::GetWindowTextType(m_Config.szWindowText[nApplicationID])
				 || CUtils::GetWindowTextType(m_Config.szWindowText[m_nApplicationID]) == CUtils::GetWindowTextType(m_Config.szWindowText[nApplicationID])
				 && _tcscmp(m_Config.szWindowText[m_nApplicationID], m_Config.szWindowText[nApplicationID]) <= 0) {
					m_nApplicationID = nApplicationID;
				}
			}
		}

		if (m_nApplicationID < 0) {
			for (int nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {
				if (!_tcsicmp(m_Config.szSpecialApp[nApplicationID], _T("Default"))) {
					m_nApplicationID = nApplicationID;
					break;
				}
			}

			if (m_nApplicationID < 0) {
				m_nApplicationID = 0;
			}
		}
	}

	if (m_Config.nSettingStyle[m_nApplicationID] != SETTING_DISABLE
	 && (_tcsicmp(m_Config.szSpecialApp[m_nApplicationID], _T("Default")) || !CUtils::IsDefaultIgnoreApplication())
	 && !bImeComposition
	 && CUtils::IsDialog()) {
		// Use Dialog Setting
		if (m_Config.bUseDialogSetting[m_nApplicationID]) {
			int nOriginalApplicationID = m_nApplicationID;
			for (m_nApplicationID = 0; m_nApplicationID < MAX_APP; ++m_nApplicationID) {
				if (!_tcsicmp(m_Config.szSpecialApp[m_nApplicationID], _T("Dialog"))) {
					break;
				}
			}
			if (m_nApplicationID == MAX_APP) {
				m_nApplicationID = nOriginalApplicationID;
			}
		}
	}

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

// emulate emacs	// cf virtual-key code
LRESULT CALLBACK CXkeymacsDll::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	ASSERT(0 <= wParam && wParam <= UCHAR_MAX);

	UINT nCommandType = NONE;
	BYTE nOrigKey = (BYTE)wParam;

	static BOOL bLocked = FALSE;
	static const BYTE RECURSIVE_KEY = 0x07;
	static int (*fCommand)() = NULL;
	static BYTE nOneShotModifier[MAX_KEY] = {'\0'};
	static BOOL bCherryOneShotModifier = FALSE;

//	CUtils::Log(_T("nCode = %#x, nKey = %#x, lParam = %p, %d, %d"), nOrigCode, nKey, lParam, IsDll64, Is64ProcessHwnd(GetForegroundWindow()));

	if (Is64ProcessHwnd(GetForegroundWindow()) != IsDll64 || CUtils::IsXkeymacs())
		return CallNextHookEx(g_hHookKeyboard, nCode, wParam, lParam);

	if (nCode < 0 || nCode == HC_NOREMOVE) {
		goto DO_NOTHING;
	}

//	CUtils::Log(_T("nKey = %#x, ext = %d, rel = %d, pre = %d, %#hx, %#hx"), nOrigKey,
//		(lParam & EXTENDED_KEY) ? 1 : 0, (lParam & BEING_RELEASED) ? 1 : 0, (lParam & REPEATED_KEY) ? 1 : 0,
//		GetKeyState(nOrigKey), GetAsyncKeyState(nOrigKey));

	if (nOrigKey == RECURSIVE_KEY) {
		if (lParam & BEING_RELEASED) {
			goto HOOK_RECURSIVE_KEY;
		} else {
			goto RECURSIVE_COMMAND;
		}
	}

	{
		static BOOL bShift = FALSE;
		if (IsDepressedShiftKeyOnly(nOrigKey)) {
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

	BYTE nKey = nOrigKey;
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

	if (lParam & BEING_RELEASED) {
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
			for (int i = 0; i < MAX_COMMAND_TYPE; i++) {
				int (*func)() = Commands[m_Config.nCommandID[m_nApplicationID][i][nKey]].fCommand;
				if (func && !(nOrigKey == VK_MENU && func == CCommands::MetaAlt))
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

	if (m_Config.nSettingStyle[m_nApplicationID] == SETTING_DISABLE) {
		goto DO_NOTHING;
	}

	// Do Nothing for Meadow, Mule for Win32, ... if those use default setting.
	if (!_tcsicmp(m_Config.szSpecialApp[m_nApplicationID], _T("Default"))
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
	nCommandType = IsDown(VK_SHIFT) * SHIFT | IsControl() * CONTROL | IsMeta() * META | CCommands::bC_x() * CONTROLX;
	// Ignore undefined C-x ?
	if (nCommandType & CONTROLX) {
		if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == NULL
		 && m_Config.nFunctionID[m_nApplicationID][nCommandType][nKey] < 0) {
			if (m_Config.bIgnoreUndefinedC_x[m_nApplicationID]) {
				CCommands::Reset(GOTO_HOOK);
				goto HOOK;
			}
			nCommandType &= ~CONTROLX;
		}
	}
	// Ignore undefined Meta Ctrl+?
	if (CCommands::bM_() && (nCommandType & CONTROL)) {
		if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == NULL
		 && m_Config.nFunctionID[m_nApplicationID][nCommandType][nKey] < 0) {
			if (m_Config.bIgnoreUndefinedMetaCtrl[m_nApplicationID]) {
				if (CheckOriginal(CONTROL, nKey)) {
					goto DO_NOTHING;
				}
				CCommands::Reset(GOTO_HOOK);
				goto HOOK;
			}
			nCommandType &= ~META;
		}
	}

	int nVirtualCommandType = GetModifierState(FALSE);
	if (nOrigKey == VK_CONTROL)
		nVirtualCommandType &= ~CONTROL;
	if (nOrigKey == VK_MENU)
		nVirtualCommandType &= ~META;
	if (CheckOriginal(nVirtualCommandType, nOrigKey)) {
		goto DO_NOTHING;
	}

	if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::EnableOrDisableXKeymacs) {
		ToggleKeyboardHookState();
		goto HOOK;
	}
	if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::EnableXKeymacs) {
		if (!m_bHook) {
			ToggleKeyboardHookState();
		}
		goto HOOK;
	}
	if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::DisableXKeymacs) {
		if (m_bHook) {
			ToggleKeyboardHookState();
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
		} else if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::BackwardChar) {
			if (index) {
				--index;
			}
			goto HOOKX;
		} else if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::BeginningOfLine) {
			index = 0;
			goto HOOKX;
		} else if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::DeleteBackwardChar) {
			if (index) {
				--index;
				memmove(&szPath[index], &szPath[index + 1], _tcslen(szPath) - index);
				SetM_xTip(szPath);
			}
			goto HOOKX;
		} else if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::DeleteChar) {
			if (index < _tcslen(szPath)) {
				memmove(&szPath[index], &szPath[index + 1], _tcslen(szPath) - index);
				SetM_xTip(szPath);
			}
			goto HOOKX;
		} else if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::EndOfLine) {
			index = _tcslen(szPath);
			goto HOOKX;
		} else if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::ForwardChar) {
			if (index < _tcslen(szPath)) {
				++index;
			}
			goto HOOKX;
		} else if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::KeyboardQuit) {
			CCommands::bM_x(FALSE);
			index = 0;
			memset(szPath, 0, sizeof(szPath));
			goto HOOK;
		} else if (nKey == VK_RETURN
				|| Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::Newline) {
			InvokeM_x(szPath);

			CCommands::bM_x(FALSE);
			index = 0;
			memset(szPath, 0, sizeof(szPath));
			goto HOOK;
		} else if (index < MAX_PATH - 1) {
			const BOOL bIsShiftDown = IsDown(VK_SHIFT, FALSE);
			for (TCHAR nAscii = 1; nAscii != 0; ++nAscii) { // repeat until overflow
				if (nKey != 0 && a2v(nAscii) == nKey && bIsShiftDown == IsShift(nAscii)) {
//					CUtils::Log("M-x: %#X (%c), %#X (%c)", nKey, nKey, nAscii, nAscii);
					if (index < _tcslen(szPath)) {
						memmove(&szPath[index + 1], &szPath[index], __min(_tcslen(szPath) - index, MAX_PATH - (index + 1) - 1));
					}
					szPath[index++] = nAscii;
//					CUtils::Log("M-x: %c(%#04x)", nAscii, nAscii);
					SetM_xTip(szPath);
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

	if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType & ~CONTROL][nKey]].fCommand == CCommands::OneShotModifierCtrl) {
		nOneShotModifier[nKey] = VK_CONTROL;
		DepressKey(VK_CONTROL);
		bCherryOneShotModifier = TRUE;
		goto HOOK;
	} else if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::OneShotModifierCtrlRepeat) {
		nOneShotModifier[nKey] = VK_CONTROL;
		DepressKey(VK_CONTROL);
		bCherryOneShotModifier = TRUE;
		goto HOOK;
	} else if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType & ~CONTROL][nKey]].fCommand == CCommands::OneShotModifierCtrlRepeat) {
		ReleaseKey(VK_CONTROL);
		bCherryOneShotModifier = FALSE;
		Kdu(nKey);
		goto HOOK;
	} else if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType & ~META][nKey]].fCommand == CCommands::OneShotModifierAlt) {
		nOneShotModifier[nKey] = VK_MENU;
		DepressKey(VK_MENU);
		bCherryOneShotModifier = TRUE;
		goto HOOK;
	} else if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::OneShotModifierAltRepeat) {
		nOneShotModifier[nKey] = VK_MENU;
		DepressKey(VK_MENU);
		bCherryOneShotModifier = TRUE;
		goto HOOK;
	} else if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType & ~META][nKey]].fCommand == CCommands::OneShotModifierAltRepeat) {
		ReleaseKey(VK_MENU);
		bCherryOneShotModifier = FALSE;
		Kdu(nKey);
		goto HOOK;
	} else if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType & ~SHIFT][nKey]].fCommand == CCommands::OneShotModifierShift) {
		nOneShotModifier[nKey] = VK_SHIFT;
		DepressKey(VK_SHIFT);
		bCherryOneShotModifier = TRUE;
		goto HOOK;
	} else if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand == CCommands::OneShotModifierShiftRepeat) {
		nOneShotModifier[nKey] = VK_SHIFT;
		DepressKey(VK_SHIFT);
		bCherryOneShotModifier = TRUE;
		goto HOOK;
	} else if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType & ~SHIFT][nKey]].fCommand == CCommands::OneShotModifierShiftRepeat) {
		ReleaseKey(VK_SHIFT);
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

	if (0 <= m_Config.nFunctionID[m_nApplicationID][nCommandType][nKey]
	 && m_Config.nFunctionID[m_nApplicationID][nCommandType][nKey] < MAX_FUNCTION
	 && _tcslen(m_Config.szFunctionDefinition[m_Config.nFunctionID[m_nApplicationID][nCommandType][nKey]])) {
		CallFunction(m_Config.nFunctionID[m_nApplicationID][nCommandType][nKey]);
		CCommands::Reset(GOTO_HOOK);
		goto HOOK;
	}

	if (!Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand) {
		if (nOrigKey == VK_CONTROL || nOrigKey == VK_MENU || nOrigKey == VK_SHIFT) {
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
	 && Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand != CCommands::KeyboardQuit) {
		CCommands::SetTemporarilyDisableXKeymacs(FALSE);
		goto DO_NOTHING;
	}

	m_bRightControl = IsDown(VK_RCONTROL, FALSE);
	m_bRightAlt = IsDown(VK_RMENU, FALSE);
	m_bRightShift = IsDown(VK_RSHIFT, FALSE);

	if (!bLocked) {
		bLocked = TRUE;
		fCommand = Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][nKey]].fCommand;
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
	SetModifierIcons();
	if (m_bRecordingMacro) {
		if (!(lParam & BEING_RELEASED) || m_bDown[wParam]) {
			try {
				KbdMacro m;
				m.nCode = nCode;
				m.wParam = wParam;
				m.lParam = lParam;
				m.bOriginal = TRUE;
				m_Macro.AddTail(m);
			} catch (CMemoryException* e) {
				e->Delete();
//				CUtils::Log("KeyboardProc: 'new' threw an exception");
			}
			if (!(lParam & BEING_RELEASED))
				m_bDown[wParam] = TRUE;
		}
	}

	return CallNextHookEx(g_hHookKeyboard, nCode, wParam, lParam);

RECURSIVE:
	Kdu(RECURSIVE_KEY, 1, FALSE);
	goto HOOKX;
HOOK:
	CCommands::SetLastCommand(fCommand);
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

void CXkeymacsDll::SetApplicationName(int nApplicationID, CString szApplicationName)
{
	ZeroMemory(m_Config.szSpecialApp[nApplicationID], CLASS_NAME_LENGTH);
	_tcsncpy_s(m_Config.szSpecialApp[nApplicationID], szApplicationName, _TRUNCATE);
}

void CXkeymacsDll::SetWindowText(int nApplicationID, CString szWindowText)
{
	ZeroMemory(m_Config.szWindowText[nApplicationID], WINDOW_TEXT_LENGTH);
	_tcsncpy_s(m_Config.szWindowText[nApplicationID], szWindowText, _TRUNCATE);
}

void CXkeymacsDll::SetCommandID(int nApplicationID, int nCommandType, int nKey, int nCommandID)
{
	m_Config.nCommandID[nApplicationID][nCommandType][nKey] = nCommandID;
}

void CXkeymacsDll::SetKillRingMax(int nApplicationID, int nKillRingMax)
{
	m_Config.nKillRingMax[nApplicationID] = nKillRingMax;
}

void CXkeymacsDll::SetUseDialogSetting(int nApplicationID, BOOL bUseDialogSetting)
{
	m_Config.bUseDialogSetting[nApplicationID] = bUseDialogSetting;
}

// Clear data of nApplicationID
void CXkeymacsDll::Clear(int nApplicationID)
{
	if (0 <= nApplicationID && nApplicationID < MAX_APP) {
		ZeroMemory(m_Config.szSpecialApp[nApplicationID], sizeof(m_Config.szSpecialApp[nApplicationID]));
		ZeroMemory(m_Config.nCommandID[nApplicationID], sizeof(m_Config.nCommandID[nApplicationID]));
		m_Config.nKillRingMax[nApplicationID] = 0;
		m_Config.bUseDialogSetting[nApplicationID] = FALSE;
		m_Config.nSettingStyle[nApplicationID] = 0;
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
				Commands[m_Config.nCommandID[m_nApplicationID][NONE][bVk]].fCommand == Modifier)
			return TRUE;
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

		if (IsDown(bVk, FALSE)) {
			return FALSE;
		}
	} while (++bVk);
	return TRUE;
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
	if (m_Config.nKillRingMax[m_nApplicationID] == 0) {
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

	if (m_Config.nKillRingMax[m_nApplicationID] < m_oKillRing.GetCount()) {
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
	if (m_Config.nKillRingMax[m_nApplicationID] == 0) {
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

void CXkeymacsDll::SetOriginal(UINT nCommandType, BYTE bVk)
{
	m_nOriginal[nCommandType & ~SHIFT][bVk]++;
}

int CXkeymacsDll::CheckOriginal(UINT nCommandType, BYTE bVk)
{
	nCommandType &= ~SHIFT;
	if (m_nOriginal[nCommandType][bVk])
		return m_nOriginal[nCommandType][bVk]--;
	return 0;
}

void CXkeymacsDll::IncreaseKillRingIndex(int nKillRing)
{
	m_nKillRing += nKillRing;
}

void CXkeymacsDll::SetSettingStyle(int nApplicationID, int nSettingStyle)
{
	m_Config.nSettingStyle[nApplicationID] = nSettingStyle;
}

void CXkeymacsDll::SetIgnoreUndefinedMetaCtrl(int nApplicationID, BOOL bIgnoreUndefinedMetaCtrl)
{
	m_Config.bIgnoreUndefinedMetaCtrl[nApplicationID] = bIgnoreUndefinedMetaCtrl;
}

void CXkeymacsDll::SetIgnoreUndefinedC_x(int nApplicationID, BOOL bIgnoreUndefinedC_x)
{
	m_Config.bIgnoreUndefinedC_x[nApplicationID] = bIgnoreUndefinedC_x;
}

void CXkeymacsDll::SetEnableCUA(int nApplicationID, BOOL bEnableCUA)
{
	m_Config.bEnableCUA[nApplicationID] = bEnableCUA;
}

BOOL CXkeymacsDll::GetEnableCUA()
{
	return m_Config.bEnableCUA[m_nApplicationID];
}

void CXkeymacsDll::StartRecordMacro()
{
	m_bRecordingMacro = TRUE;
	if (CCommands::bC_u())
		CallMacro();
	m_Macro.RemoveAll();
	ZeroMemory(m_bDown, MAX_KEY);
}

void CXkeymacsDll::EndRecordMacro()
{
	m_bRecordingMacro = FALSE;
	while (!m_Macro.IsEmpty()) { // remove not released push
		KbdMacro& m = m_Macro.GetTail();
		if (m.lParam & BEING_RELEASED)
			break;
		m_Macro.RemoveTail();
	}
}

void CXkeymacsDll::CallMacro()
{
	if (m_bRecordingMacro)
		m_bRecordingMacro = FALSE;
	UINT before = GetModifierState(FALSE);
	SetModifierState(0, before);
	for (POSITION pos = m_Macro.GetHeadPosition(); pos; ) {
		KbdMacro& m = m_Macro.GetNext(pos);
		if (m.lParam & BEING_RELEASED)
			ReleaseKey((BYTE)m.wParam);
		else
			DepressKey((BYTE)m.wParam, m.bOriginal);
	}
	SetModifierState(before, 0);
}

/*
void CXkeymacsDll::CallMacro()	// for debug
{
	CString sz;
	for (POSITION pos = m_Macro.GetHeadPosition(); pos; ) {
		KbdMacro m = m_Macro.GetNext(pos);
		if (m.lParam & BEING_RELEASED) {
			CString t;
			t.Format(_T("0x%xu "), m.wParam);
			sz += t;
		} else {
			CString t;
			t.Format(_T("0x%xd "), m.wParam);
			sz += t;
		}
	}
//	CUtils::Log(sz);
}
*/

void CXkeymacsDll::Set106Keyboard(BOOL b106Keyboard)
{
	m_Config.b106Keyboard = b106Keyboard;
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
		 && (Commands[m_Config.nCommandID[m_nApplicationID][NONE][bVk]].fCommand == CCommands::PassThrough)) {
			if (bVk == nKey) {
				return GOTO_HOOK;
			}

			return GOTO_DO_NOTHING;
		}
	} while (++bVk);
	return CONTINUE;
}

void CXkeymacsDll::SetFunctionKey(int nFunctionID, int nApplicationID, int nCommandType, int nKey)
{
	if (nApplicationID	< 0 || MAX_APP			<= nApplicationID
	 || nCommandType	< 0 || MAX_COMMAND_TYPE	<= nCommandType
	 || nKey			< 0 || MAX_KEY			<= nKey) {
		return;
	}

	m_Config.nFunctionID[nApplicationID][nCommandType][nKey] = nFunctionID;
}

void CXkeymacsDll::ClearFunctionDefinition()
{
	memset(m_Config.nFunctionID, -1, sizeof(m_Config.nFunctionID));
	memset(m_Config.szFunctionDefinition, 0, sizeof(m_Config.szFunctionDefinition));
}

void CXkeymacsDll::SetFunctionDefinition(int nFunctionID, CString szDefinition)
{
	if (nFunctionID < 0 || MAX_FUNCTION <= nFunctionID) {
		return;
	}

	memset(m_Config.szFunctionDefinition[nFunctionID], 0, sizeof(m_Config.szFunctionDefinition[nFunctionID]));
	_stprintf_s(m_Config.szFunctionDefinition[nFunctionID], _T("%s"), szDefinition);

	return;

}

// call an original command which is defined in dot.xkeymacs
void CXkeymacsDll::CallFunction(int nFunctionID)
{
	CArray<KeyBind, KeyBind> keybinds;

	if (nFunctionID < 0 || MAX_FUNCTION <= nFunctionID || !_tcslen(m_Config.szFunctionDefinition[nFunctionID])) {
		return;
	}

	UINT before = GetModifierState(FALSE);

	if (m_Config.szFunctionDefinition[nFunctionID][0] == _T('"') && m_Config.szFunctionDefinition[nFunctionID][_tcslen(m_Config.szFunctionDefinition[nFunctionID]) - 1] == _T('"')) {
		for (unsigned int i = 1; i < _tcslen(m_Config.szFunctionDefinition[nFunctionID]) - 1; ++i) {	// skip '"'
			keybinds.Add(ParseKey(nFunctionID, i));
		}
	} else if (m_Config.szFunctionDefinition[nFunctionID][0] == _T('[') && m_Config.szFunctionDefinition[nFunctionID][_tcslen(m_Config.szFunctionDefinition[nFunctionID]) - 1] == _T(']')) {
		for (unsigned int i = 1; i < _tcslen(m_Config.szFunctionDefinition[nFunctionID]) - 1; ++i) {	// skip '[' and ']'
			if (m_Config.szFunctionDefinition[nFunctionID][i] == _T('?')) {	// [?f ?o ?o]
				++i;
				keybinds.Add(ParseKey(nFunctionID, i));
			} else {												// [ControlCharacter]
				for (int nKeyID = 0; nKeyID < sizeof(ControlCharacters) / sizeof(ControlCharacters[0]); ++nKeyID) {
					if (!_tcsncmp(m_Config.szFunctionDefinition[nFunctionID] + i, ControlCharacters[nKeyID].name, _tcslen(ControlCharacters[nKeyID].name))) {
						KeyBind keybind = {NONE, ControlCharacters[nKeyID].bVk};
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

		if (nCommandType < MAX_COMMAND_TYPE && Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][bVk]].fCommand) {
			if (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][bVk]].fCommand == CCommands::ExecuteExtendedCommand) {
				bM_x = TRUE;
			} else if (!bInitialized) {
				SetModifierState(0, before);
				bInitialized = TRUE;
			}
//			CUtils::Log("CallFunction: Command Name: %s", Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][bVk]].szCommandName);
			while (Commands[m_Config.nCommandID[m_nApplicationID][nCommandType][bVk]].fCommand() == GOTO_RECURSIVE) {
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
				SetModifierState(0, before);
				bInitialized = TRUE;
			}
			if (nCommandType & WIN_WIN)
				DepressKey(VK_LWIN);
			if (nCommandType & WIN_CTRL)
				DepressKey(VK_CONTROL);
			if (nCommandType & WIN_ALT)
				DepressKey(VK_MENU);
			if (nCommandType & SHIFT)
				DepressKey(VK_SHIFT);

			Kdu(bVk);

			if (nCommandType & SHIFT && (keybinds.GetSize() <= i + 1 || !(keybinds.GetAt(i + 1).nCommandType & SHIFT)))
				ReleaseKey(VK_SHIFT);
			if (nCommandType & WIN_ALT && (keybinds.GetSize() <= i + 1 || !(keybinds.GetAt(i + 1).nCommandType & WIN_ALT)))
				ReleaseKey(VK_MENU);
			if (nCommandType & WIN_CTRL && (keybinds.GetSize() <= i + 1 || !(keybinds.GetAt(i + 1).nCommandType & WIN_CTRL)))
				ReleaseKey(VK_CONTROL);
			if (nCommandType & WIN_WIN && (keybinds.GetSize() <= i + 1 || !(keybinds.GetAt(i + 1).nCommandType & WIN_WIN)))
				ReleaseKey(VK_LWIN);
		}
	}

	keybinds.RemoveAll();

	if (bInitialized)
		// If these lines are invoked at M-x, a window transition does not work well.
		SetModifierState(before, 0);
	return;
}

KeyBind CXkeymacsDll::ParseKey(const int nFunctionID, unsigned int &i)
{
	KeyBind keybind = {NONE};

	if (m_Config.szFunctionDefinition[nFunctionID][i] == _T('\\')) {
		++i;
		BOOL bFound = FALSE;
		do {
			bFound = FALSE;
			for (int ModifierID = 0; ModifierID < sizeof(Modifiers) / sizeof(Modifiers[0]); ++ModifierID) {
				if (!_tcsncmp(m_Config.szFunctionDefinition[nFunctionID] + i, Modifiers[ModifierID].name, _tcslen(Modifiers[ModifierID].name))
				 && _tcslen(Modifiers[ModifierID].name) < _tcslen(m_Config.szFunctionDefinition[nFunctionID] + i)) {
					keybind.nCommandType |= Modifiers[ModifierID].id;
					i+= _tcslen(Modifiers[ModifierID].name);
					bFound = TRUE;
				}
			}
		} while (bFound);
	}
	if (IsShift(m_Config.szFunctionDefinition[nFunctionID][i]) && !(keybind.nCommandType & (WIN_CTRL | WIN_ALT | WIN_WIN))) {
		keybind.nCommandType |= SHIFT;
	}

	for (int nKeyID = 0; nKeyID < sizeof(ControlCharacters) / sizeof(ControlCharacters[0]); ++nKeyID) {
		if (!_tcsncmp(m_Config.szFunctionDefinition[nFunctionID] + i, ControlCharacters[nKeyID].name, _tcslen(ControlCharacters[nKeyID].name))) {
			i += _tcslen(ControlCharacters[nKeyID].name);
			break;
		}
	}
	if (nKeyID < sizeof(ControlCharacters) / sizeof(ControlCharacters[0])) {
		keybind.bVk = ControlCharacters[nKeyID].bVk;
	} else {
		keybind.bVk = a2v(m_Config.szFunctionDefinition[nFunctionID][i]);
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

void CXkeymacsDll::Set326Compatible(int nApplicationID, BOOL b326Compatible)
{
	m_Config.b326Compatible[nApplicationID] = b326Compatible;
}

BOOL CXkeymacsDll::Get326Compatible()
{
	return m_Config.b326Compatible[m_nApplicationID];
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
