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
	static void SetHooks();
	static void SetKeyboardHook();
	static void ResetHooks();
	static void ReleaseHooks();
	static void ReleaseKeyboardHook();
	static void ToggleKeyboardHookState();
	static BOOL IsKeyboardHook();
	static void ShowKeyboardHookState();
	static void SetM_xTip(LPCTSTR szPath);
	static BOOL SendIconMessage(ICONMSG *pMsg, DWORD num);
	static BOOL IsDown(BYTE bVk, BOOL bPhysicalKey = TRUE);
	static void Kdu(BYTE bVk, DWORD n = 1, BOOL bOriginal = TRUE);
	static void DepressKey(BYTE bVk, BOOL bOriginal = TRUE);
	static void ReleaseKey(BYTE bVk);
	static UINT GetModifierState(BOOL bPhysicalKey = TRUE);
	static void SetModifierState(UINT before, UINT after);
	static void AddKillRing(BOOL bNewData = TRUE);
	static CClipboardSnap* GetKillRing(CClipboardSnap *pSnap, BOOL bForce = TRUE);
	static void IncreaseKillRingIndex(int nKillRing = 1);
	static BOOL GetEnableCUA();
	static BOOL Get326Compatible();
	static BOOL Is106Keyboard();
	static void StartRecordMacro();
	static void EndRecordMacro();
	static void CallMacro();
	static int GetAccelerate(void);
	static void SetAccelerate(int nAccelerate);
	static void SetKeyboardSpeed(int nKeyboardSpeed);
	static unsigned int GetMaxKeyInterval(void);
	static void SetCursorData(HCURSOR hEnable, HCURSOR hDisableTMP, HCURSOR hDisableWOCQ, HICON hDisable, BOOL bEnable);

private:
	static CONFIG m_Config;
	static HHOOK m_hHookCallWnd;
	static HHOOK m_hHookCallWndRet;
	static HHOOK m_hHookGetMessage;
	static HHOOK m_hHookShell;
	static bool m_bEnableKeyboardHook;
	static BOOL m_bHook;
	static DWORD m_nHookAltRelease;
	static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK ShellProc(int nCode, WPARAM wParam, LPARAM lParam);
	static int m_nAppID;
	static void InitKeyboardProc(BOOL bImeComposition);
	static int GetAppID(LPCSTR szName, int fallback);
	static BOOL m_bRightShift;
	static BOOL m_bRightControl;
	static BOOL m_bRightAlt;
	static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
	static void CancelMarkWithShift(BYTE nKey, bool bRelease);
	static int IsPassThrough(BYTE nKey);
	static void InvokeM_x(LPCTSTR szPath);
	static TCHAR m_M_xTip[128];
	static void SetModifierIcons();
	static void DoKeybd_event(BYTE bVk, DWORD dwFlags);
	static void SetOriginal(UINT nType, BYTE bVk);
	static BYTE m_nOriginal[MAX_COMMAND_TYPE][MAX_KEY];
	static int CheckOriginal(UINT nType, BYTE bVk);
	static BOOL UpdateKeyboardState(BYTE bVK, BYTE bState);
	static BOOL IsControl();
	static BOOL IsMeta();
	static BOOL IsDepressedModifier(int Modifier(), BOOL bPhysicalKey = TRUE);
	static CList<CClipboardSnap *, CClipboardSnap *> m_oKillRing;
	static int m_nKillRing;
	static BOOL m_bRecordingMacro;
	static BOOL m_bDown[MAX_KEY];
	static std::list<KbdMacro> m_Macro;
	static void CallFunction(int nFuncID);
	static KeyBind ParseKey(LPCTSTR& def);
	static BOOL IsShift(TCHAR nAscii);
	static BYTE a2v(TCHAR nAscii);
	static BOOL IsMatchWindowText(CString szWindowText);
	static int m_nAccelerate;
	static int m_nKeyboardSpeed;
	static HCURSOR m_hCurrentCursor;
	static BOOL m_bCursor;
	static HCURSOR m_hCursor[MAX_STATUS];
	static void DoSetCursor();
};

#endif // !defined(AFX_XKEYMACS_H__88552DEC_1233_4A0A_BE62_9EF7BC618EC6__INCLUDED_)
