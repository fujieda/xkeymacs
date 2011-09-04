// xkeymacs.h: interface for the CXkeymacsDll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XKEYMACS_H__88552DEC_1233_4A0A_BE62_9EF7BC618EC6__INCLUDED_)
#define AFX_XKEYMACS_H__88552DEC_1233_4A0A_BE62_9EF7BC618EC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "ClipboardSnap.h"

const int MAX_APP = 64;
const int NONE = 0x0, SHIFT = 0x1, CONTROL = 0x2, META = 0x4, CONTROLX = 0x8,
	   MAX_COMMAND_TYPE = NONE + SHIFT + CONTROL + META + CONTROLX + 1;
const int /* WIN_SHIFT = 0x100, */ WIN_CTRL = 0x200, WIN_ALT = 0x400, WIN_WIN = 0x800;
const int MAX_KEY = 256;
const int MAX_FUNCTION = 64;
const int MAX_DEFINITION = 256;

const int WINDOW_NAME_LENGTH = 128;
const int CLASS_NAME_LENGTH = 128;
const int SUB_KEY_NAME_LENGTH = 128;
const int WINDOW_TEXT_LENGTH = WINDOW_NAME_LENGTH * 2 + 3;

enum ICON_TYPE { MAIN_ICON, CX_ICON, MX_ICON, META_ICON, SHIFT_ICON, CTRL_ICON, ALT_ICON, MAX_ICON_TYPE };
enum XKEYMACS_STATUS { STATUS_ENABLE, STATUS_DISABLE_TMP, STATUS_DISABLE_WOCQ, STATUS_DISABLE, MAX_STATUS };
enum { OFF_ICON, ON_ICON };

enum GOTO { GOTO_DO_NOTHING, GOTO_HOOK, GOTO_RECURSIVE, GOTO_HOOKX, GOTO_HOOK0_9, CONTINUE };
enum SETTING_STYLE { SETTING_DEFAULT, SETTING_SPECIFIC, SETTING_DISABLE, SETTING_UNDEFINED };

#include "ipc.h"

struct KeyBind
{
	int nType;
	BYTE bVk;
	int nControlID;
};

struct KeyName
{
	BYTE bVk;
	LPCTSTR name;
};

struct KbdMacro
{
	int nCode;
	WPARAM wParam;
	LPARAM lParam;
	BOOL bOriginal;
};

const DWORD HOOK_ALT_LATER = 0x10000;

class AFX_EXT_CLASS CXkeymacsDll  
{
public:
	static BOOL SaveConfig();
	static BOOL LoadConfig();
	static void SetConfig(const CONFIG& config);
	static void SetM_xTip(LPCTSTR szPath);
	static BOOL Get326Compatible();
	static void SetCursorData(HCURSOR hEnable, HCURSOR hDisableTMP, HCURSOR hDisableWOCQ, HICON hDisable, BOOL bEnable);
	static unsigned int GetMaxKeyInterval(void);
	static void SetKeyboardSpeed(int nKeyboardSpeed);
	static int GetAccelerate(void);
	static void SetAccelerate(int nAccelerate);
	static void Clear(int nAppID);
	static BOOL IsKeyboardHook();
	static void ReleaseHooks();
	static void ReleaseKeyboardHook();
	static void SetHooks();
	static void SetKeyboardHook();
	static void ResetHooks();
	static void AddKillRing(BOOL bNewData = TRUE);
	static void CallMacro();
	static void StartRecordMacro();
	static void EndRecordMacro();
	static void DepressKey(BYTE bVk, BOOL bOriginal = TRUE);
	static BOOL GetEnableCUA();
	static CClipboardSnap* GetKillRing(CClipboardSnap *pSnap, BOOL bForce = TRUE);
	static void IncreaseKillRingIndex(int nKillRing = 1);
	static UINT GetModifierState(BOOL bPhysicalKey = TRUE);
	static void SetModifierState(UINT before, UINT after);
	static BOOL IsDown(BYTE bVk, BOOL bPhysicalKey = TRUE);
	static void Kdu(BYTE bVk, DWORD n = 1, BOOL bOriginal = TRUE);
	static void ReleaseKey(BYTE bVk);
	static BOOL Is106Keyboard();
	static void ToggleKeyboardHookState();
	static void ShowKeyboardHookState();
	static BOOL SendIconMessage(ICONMSG *pMsg, DWORD num);

private:
	static bool m_bEnableKeyboardHook;
	static DWORD m_nHookAltRelease;
	static TCHAR m_M_xTip[128];
	static void InvokeM_x(LPCTSTR szPath);
	static BOOL UpdateKeyboardState(BYTE bVK, BYTE bState);
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
	static HHOOK m_hHookCallWnd;
	static HHOOK m_hHookGetMessage;
	static HHOOK m_hHookShell;
	static BOOL IsControl();
	static BOOL IsMeta();
	static void SetModifierIcons();
	static void DoKeybd_event(BYTE bVk, DWORD dwFlags);
	static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK ShellProc(int nCode, WPARAM wParam, LPARAM lParam);
	static int GetAppID(LPCSTR szName, int fallback);
	static KeyBind ParseKey(LPCTSTR& def);
	static BYTE a2v(TCHAR nAscii);
	static BOOL IsShift(TCHAR nAscii);
	static void CallFunction(int nFuncID);
	static int IsPassThrough(BYTE nKey);
	static void CancelMarkWithShift(BYTE nKey, bool bRelease);
	static BOOL IsDepressedModifier(int Modifier(), BOOL bPhysicalKey = TRUE);
	static BOOL IsValidKey(BYTE bVk);
	static std::list<KbdMacro> m_Macro;
	static BOOL m_bRecordingMacro;
	static BOOL m_bDown[MAX_KEY];
	static void SetOriginal(UINT nType, BYTE bVk);
	static int CheckOriginal(UINT nType, BYTE bVk);
	static void InitKeyboardProc(BOOL bImeComposition);
	static int m_nAppID;
	static BYTE m_nOriginal[MAX_COMMAND_TYPE][MAX_KEY];
	static int m_nKillRing;
	static CList<CClipboardSnap *, CClipboardSnap *> m_oKillRing;
	static BOOL m_bHook;
	static BOOL m_bRightShift;
	static BOOL m_bRightAlt;
	static BOOL m_bRightControl;
	static CONFIG m_Config;
};

#endif // !defined(AFX_XKEYMACS_H__88552DEC_1233_4A0A_BE62_9EF7BC618EC6__INCLUDED_)
