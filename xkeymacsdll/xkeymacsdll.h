// xkeymacs.h: interface for the CXkeymacsDll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XKEYMACS_H__88552DEC_1233_4A0A_BE62_9EF7BC618EC6__INCLUDED_)
#define AFX_XKEYMACS_H__88552DEC_1233_4A0A_BE62_9EF7BC618EC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KbdMacro.h"
#include "ClipboardSnap.h"
#include "defs.h"
#include "ipc.h"
#include <list>

const DWORD HOOK_ALT_LATER = 0x10000;

class AFX_EXT_CLASS CXkeymacsDll  
{
public:
	static BOOL SaveConfig();
	static BOOL LoadConfig();
	static void SetConfig(const Config& config);
	static void SetHooks();
	static void ResetHooks();
	static void ReleaseHooks();
	static void ReleaseKeyboardHook();
	static void SetHookStateDirect(bool enable);
	static void ToggleHookState();
	static bool GetHookState();
	static void ShowHookState();
	static void SetIMEState(bool on);
	static void SetM_xTip(LPCTSTR szPath);
	static void SendIconMessage(IconState *state, int num);
	static BOOL IsDown(BYTE bVk, BOOL bPhysicalKey = TRUE);
	static void Kdu(BYTE bVk, DWORD n = 1, BOOL bOriginal = TRUE);
	static void DepressKey(BYTE bVk, BOOL bOriginal = TRUE);
	static void ReleaseKey(BYTE bVk);
	static UINT GetModifierState(BOOL bPhysicalKey = TRUE);
	static void SetModifierState(UINT before, UINT after);
	static void AddKillRing(BOOL bNewData = TRUE);
	static CClipboardSnap* GetKillRing(CClipboardSnap *pSnap, BOOL bForce = TRUE);
	static void IncreaseKillRingIndex(int nKillRing = 1);
	static bool GetEnableCUA();
	static bool Get326Compatible();
	static bool Is106Keyboard();
	static void SetKbMacro(KbdMacro* kbdMacro);
	static SHORT ConvVkey(SHORT in, int mode);
	static int GetAccelerate(void);
	static void SetAccelerate(int nAccelerate);
	static void SetKeyboardSpeed(int nKeyboardSpeed);
	static unsigned int GetMaxKeyInterval(void);
	static void SetCursorData(HCURSOR hEnable, HCURSOR hDisableTMP, HCURSOR hDisableWOCQ, HICON hDisable, BOOL bEnable);

private:
	static Config m_Config;
	static AppConfig* m_CurrentConfig;
	static BYTE (*m_CmdID)[MAX_KEY];
	static char (*m_FuncID)[MAX_KEY];
	static HHOOK m_hHookCallWnd;
	static HHOOK m_hHookCallWndRet;
	static HHOOK m_hHookGetMessage;
	static HHOOK m_hHookShell;
	static bool m_bEnableKeyboardHook;
	static void SetKeyboardHook(DWORD threadId = 0);
	static bool m_bHook;
	static void SetHookState(bool enable);
	static DWORD m_nHookAltRelease;
	static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK ShellProc(int nCode, WPARAM wParam, LPARAM lParam);
	static void InitKeyboardProc();
	static AppConfig* GetAppConfig(LPCTSTR szName, AppConfig* fallback);
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
	static KbdMacro* m_kbdMacro;
	static void CallFunction(int id);
	static int m_nAccelerate;
	static int m_nKeyboardSpeed;
	static HCURSOR m_hCurrentCursor;
	static BOOL m_bCursor;
	static HCURSOR m_hCursor[MAX_STATUS];
	static void DoSetCursor();
};

#endif // !defined(AFX_XKEYMACS_H__88552DEC_1233_4A0A_BE62_9EF7BC618EC6__INCLUDED_)
