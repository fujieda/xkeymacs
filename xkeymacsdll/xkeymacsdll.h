// xkeymacs.h: interface for the CXkeymacsDll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XKEYMACS_H__88552DEC_1233_4A0A_BE62_9EF7BC618EC6__INCLUDED_)
#define AFX_XKEYMACS_H__88552DEC_1233_4A0A_BE62_9EF7BC618EC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ClipboardSnap.h"

enum { MAX_APP = 64 };
enum { NONE = 0x0, SHIFT = 0x1, CONTROL = 0x2, META = 0x4, CONTROLX = 0x8,
	   MAX_COMMAND_TYPE = NONE + SHIFT + CONTROL + META + CONTROLX + 1 };
enum { /* WIN_SHIFT = 0x100, */ WIN_CTRL = 0x200, WIN_ALT = 0x400, WIN_WIN = 0x800 };
enum { MAX_KEY = 256 };
enum { MAX_FUNCTION = 64 };
enum { MAX_DEFINITION = 256 };

enum { WINDOW_NAME_LENGTH	= 128 };
enum { CLASS_NAME_LENGTH	= 128 };
enum { SUB_KEY_NAME_LENGTH	= 128 };
enum { WINDOW_TEXT_LENGTH   = 128 + 128 + 3};

enum ICON_TYPE { MAIN_ICON, CX_ICON, MX_ICON, META_ICON, SHIFT_ICON, CTRL_ICON, ALT_ICON, MAX_ICON_TYPE };
enum XKEYMACS_STATUS { STATUS_ENABLE, STATUS_DISABLE_TMP, STATUS_DISABLE_WOCQ, STATUS_DISABLE, MAX_STATUS };
enum { ON_ICON, OFF_ICON };	// alias of status

enum GOTO { GOTO_DO_NOTHING, GOTO_HOOK, GOTO_RECURSIVE, GOTO_HOOKX, GOTO_HOOK0_9, CONTINUE };
enum SETTING_STYLE { SETTING_DEFAULT, SETTING_SPECIFIC, SETTING_DISABLE, SETTING_UNDEFINED };

enum { EXTENDED_KEY = 0x01000000 };
enum { REPEATED_KEY = 0x40000000 };
enum { BEING_RELEASED = 0x80000000 };

typedef struct KeyBind
{
	int nCommandType;
	BYTE bVk;
	int nControlID;
} KeyBind_t;

typedef struct KbdMacro
{
	int nCode;
	WPARAM wParam;
	LPARAM lParam;
	BOOL bOriginal;
} KbdMacro_t;

typedef struct Modifier {
	LPCTSTR name;
	int id;
} Modifier_t;

static const Modifier_t Modifiers[] = {
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

typedef struct KeyName
{
	BYTE bVk;
	LPCTSTR name;
} KeyName_t;

static const KeyName_t ControlCharacters[] = {
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

class AFX_EXT_CLASS CXkeymacsDll  
{
public:
	static void ModifyM_xTip(const TCHAR *const szPath);
	static BOOL Get326Compatible();
	void Set326Compatible(int nApplicationID, BOOL b326Compatible);
	void SetCursorData(HCURSOR hEnable, HCURSOR hDisableTMP, HCURSOR hDisableWOCQ, HICON hDisable, BOOL bEnable);
	static unsigned int GetMaxKeyInterval(void);
	static void SetKeyboardSpeed(int nKeyboardSpeed);
	static int GetAccelerate(void);
	static void SetAccelerate(int nAccelerate);
	void SetWindowText(int nApplicationID, CString szWindowText);
//	static int m_nPassThrough;
	void AddAllShell_NotifyIcon();
	void DeleteAllShell_NotifyIcon();
	void SetKillRingMax(int nApplicationID, int nKillRingMax);
	void CXkeymacsDll::EnableShell_NotifyIcon(ICON_TYPE icon, BOOL bEnable);
	void SetNotifyIconData(ICON_TYPE icon, NOTIFYICONDATA stNtfyIcon, HICON hEnable, HICON hDisableTMP, HICON hDisableWOCQ, HICON hDisable, BOOL bEnable);
	void SetNotifyIconData(ICON_TYPE icon, NOTIFYICONDATA stNtfyIcon, HICON hOn, HICON hOff, BOOL bEnable);
	void Clear(int nApplicationID);
	BOOL IsKeyboardHook();
	void SetCommandID(int nApplicationID, int nCommandType, int nKey, int nCommandID);
	void SetAtIbeamCursorOnly(int nApplicationID, int nCommandType, int nKey, BOOL bAtIbeamCursorOnly);
	void SetApplicationName(int nApplicationID, CString szApplicationName);
	static void ReleaseHooks();
	static void ReleaseKeyboardHook();
	void SetEnableCUA(int nApplicationID, BOOL bEnableCUA);
	void SetIgnoreUndefinedC_x(int nApplicationID, BOOL bIgnoreUndefinedC_x);
	void SetIgnoreUndefinedMetaCtrl(int nApplicationID, BOOL bIgnoreUndefinedMetaCtrl);
	static void SetHooks();
	static void SetKeyboardHook();
	void SetSettingStyle(int nApplicationID, int nSettingStyle);
	void SetUseDialogSetting(int nApplicationID, BOOL bUseDialogSetting);
	static void AddKillRing(BOOL bNewData = TRUE);
	static void CallMacro();
	static void ClearFunctionDefinition();
	static void DefiningMacro(BOOL bDefiningMacro);
	static void DepressKey(BYTE bVk, BOOL bOriginal = TRUE);
	static BOOL GetEnableCUA();
	static CClipboardSnap* GetKillRing(CClipboardSnap *pSnap, BOOL bForce = TRUE);
	static void IncreaseKillRingIndex(int nKillRing = 1);
	static BOOL IsDown(BYTE bVk, BOOL bPhysicalKey = TRUE);
	static void Kdu(BYTE bVk, DWORD n = 1, BOOL bOriginal = TRUE);
	static int ModifyShell_NotifyIcon(ICON_TYPE icon, BOOL bNewStatus, BOOL bForce = TRUE);
	static void ReleaseKey(BYTE bVk);
	static void SetFunctionDefinition(int nFunctionID, CString szDefinition);
	static void SetFunctionKey(int nFunctionID, int nApplicationID, int nCommandType, int nKey);
	static void SetKeyboardHookFlag();
	static void SetKeyboardHookFlag(BOOL bFlag);
	CXkeymacsDll();
	virtual ~CXkeymacsDll();
private:
	static void InvokeM_x(const TCHAR* const szPath);
	static BOOL m_b326Compatible[MAX_APP];
	static void LogCallWndProcMessage(WPARAM wParam, LPARAM lParam);
	static void DoSetCursor();
	static HCURSOR m_hCurrentCursor;
	static BOOL m_bCursor;
	static HCURSOR m_hCursor[MAX_STATUS];
	static int m_nKeyboardSpeed;
	static int m_nAccelerate;
	static HHOOK m_hHookCallWndRet;
	static LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam);
	static BOOL IsMatchWindowText(CString szWindowText);
	static TCHAR m_szWindowText[MAX_APP][WINDOW_TEXT_LENGTH];
	static BOOL m_bEnableKeyboardHook;
	static HHOOK m_hHookCallWnd;
	static HHOOK m_hHookGetMessage;
	static HHOOK m_hHookShell;
	static void AddShell_NotifyIcon(ICON_TYPE icon);
	void DeleteShell_NotifyIcon(ICON_TYPE icon);
	static BOOL DefiningMacro();
	static BOOL IsControl();
	static BOOL IsMeta();
	static void DoKeybd_event(BYTE bVk, DWORD dwFlags);
	static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK ShellProc(int nCode, WPARAM wParam, LPARAM lParam);
	static KeyBind_t ParseKey(const int nFunctionID, unsigned int &i);
	static BYTE a2v(TCHAR nAscii);
	static BOOL IsShift(TCHAR nAscii);
	static TCHAR m_szFunctionDefinition[MAX_FUNCTION][MAX_DEFINITION];
	static void CallFunction(int nFunctionID);
	static int m_nFunctionID[MAX_APP][MAX_COMMAND_TYPE][MAX_KEY];
	static BOOL m_bEnableCUA[MAX_APP];
	static TCHAR m_szOldApplicationName[MAX_PATH];
	static TCHAR m_szApplicationName[MAX_PATH];
	static int IsPassThrough(BYTE nKey);
	static BOOL IsDepressedShiftKeyOnly(BYTE nKey);
	static BOOL IsDepressedModifier(int Modifier(), BOOL bPhysicalKey = TRUE);
	static BOOL IsValidKey(BYTE bVk);
	static CObList m_Macro;
	static BOOL m_bDefiningMacro;
	static void Original(int nCommandType, BYTE bVk, int nOriginal);
	static int Original(int nCommandType, BYTE bVk);
	static HICON m_hIcon[MAX_ICON_TYPE][MAX_STATUS];
	static BOOL m_bUseDialogSetting[MAX_APP];
	static void InitKeyboardProc(BOOL bImeComposition);
	static BOOL m_bIgnoreUndefinedC_x[MAX_APP];
	static BOOL m_bIgnoreUndefinedMetaCtrl[MAX_APP];
	static int m_nSettingStyle[MAX_APP];
	static int m_nApplicationID;
	static int m_nOriginal[MAX_COMMAND_TYPE][MAX_KEY];
	static int m_nKillRing;
	static CList<CClipboardSnap *, CClipboardSnap *> m_oKillRing;
	static int m_nKillRingMax[MAX_APP];
	static int GetMickey(int nDifferential, int nThreshold1, int nThreshold2, int nAcceleration, int nSpeed);
	static BOOL DoShell_NotifyIcon(ICON_TYPE icon, DWORD dwMessage);
	static BOOL m_bIcon[MAX_ICON_TYPE];
	static DWORD m_dwOldMessage[MAX_ICON_TYPE];
	static NOTIFYICONDATA m_stNtfyIcon[MAX_ICON_TYPE];
	static NOTIFYICONDATA m_stOldNtfyIcon[MAX_ICON_TYPE];
	static BOOL m_bHook;
	static BOOL m_bRightShift;
	static BOOL m_bRightAlt;
	static BOOL m_bRightControl;
	static TCHAR m_szSpecialApp[MAX_APP][CLASS_NAME_LENGTH];
	static int m_nCommandID[MAX_APP][MAX_COMMAND_TYPE][MAX_KEY];
	static BOOL m_bAtIbeamCursorOnly[MAX_APP][MAX_COMMAND_TYPE][MAX_KEY];
};


class AFX_EXT_CLASS CXkeymacsData  
{
public:
	void Set326Compatible(BOOL b326Compatible);
	BOOL Get326Compatible();
	int GetWindowTextType();
	void SetWindowTextType(int nWindowTextType);
	CString GetWindowText();
	void SetWindowText(LPCTSTR lpszWindowText);
	static BOOL Is106Keyboard();
	static void Set106Keyboard(BOOL b106Keyboard);
	static int GetToolTipID(int nCommandID);
	static int GetDescriptionID(int nCommandID);
	void SetUseDialogSetting(BOOL bUseDialogSetting);
	BOOL GetUseDialogSetting();
	BOOL GetEnableCUA();
	void SetEnableCUA(BOOL bEnableCUA);
	BOOL GetIgnoreUndefinedC_x();
	void SetIgnoreUndefinedC_x(BOOL bIgnoreUndefinedC_x);
	BOOL GetIgnoreUndefinedMetaCtrl();
	void SetIgnoreUndefinedMetaCtrl(BOOL bIgnoreUndefinedMetaCtrl);
	static int GetCategoryID(int nCommandID);
	void SetSettingStyle(int nSettingStyle);
	int GetSettingStyle();
	static int GetDefaultControlID(int nCommandID, int nIndex);
	static int GetDefaultCommandKey(int nCommandID, int nIndex);
	static int GetDefaultCommandType(int nCommandID, int nIndex);
	static CString GetCommandName(int nCommandID);
	void SetKillRingMax(int nKillRingMax);
	int GetKillRingMax();
	CString GetApplicationTitle();
	void SetApplicationTitle(LPCTSTR lpszApplicationTitle);
	void ClearAll();
	int GetCommandID(int nCommandType, int nKey);
	void SetCommandID(int nCommandType, int nKey, int nCommandID);
	BOOL GetAtIbeamCursorOnly(int nCommandType, int nKey);
	void SetAtIbeamCursorOnly(int nCommandType, int nKey, BOOL bAtIbeamCursorOnly);
	CString GetApplicationName();
	void SetApplicationName(LPCTSTR lpszApplicationName);
	CXkeymacsData();
	virtual ~CXkeymacsData();

private:
	BOOL m_b326Compatible;
	int m_nWindowTextType;
	CString m_strWindowText;
	BOOL m_bEnableCUA;
	static BOOL m_b106Keyboard;
	BOOL m_bUseDialogSetting;
	BOOL m_bIgnoreUndefinedC_x;
	BOOL m_bIgnoreUndefinedMetaCtrl;
	int m_nSettingStyle;
	int m_nKillRingMax;
	CString m_strApplicationTitle;
	CString m_strApplicationName;
	int m_nCommandID[MAX_COMMAND_TYPE][MAX_KEY];
	BOOL m_bAtIbeamCursorOnly[MAX_COMMAND_TYPE][MAX_KEY];
};

#endif // !defined(AFX_XKEYMACS_H__88552DEC_1233_4A0A_BE62_9EF7BC618EC6__INCLUDED_)
