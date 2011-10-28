// Profile.h: interface of the CProfile class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILE_H__9415254D_4656_484B_A730_E02580D8A221__INCLUDED_)
#define AFX_PROFILE_H__9415254D_4656_484B_A730_E02580D8A221__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "properties.h"
#include "KeyString.h"
#include "../xkeymacsdll/ipc.h"

struct TASK_LIST {
    DWORD dwProcessId;
    CHAR ProcessName[MAX_PATH];
};
typedef TASK_LIST *PTASK_LIST;

enum { MAX_TASKS = 256 };

enum HKEY_TYPE { CURRENT_USER, LOCAL_MACHINE, MAX_HKEY_TYPE };

class CKey;

class CProfile
{
public:
	static int GetKeyboardSpeed(void);
	static CString GetWindowText(int nAppID);
	static void SetWindowText(int nAppID, CString szWindowText);
	static void SetEnableCUA(int nAppID, BOOL bEnableCUA);
	static BOOL GetEnableCUA(int nAppID);
	static void ImportProperties();
	static void ExportProperties();
	static void RestartComputer();
	static BOOL IsVistaOrLater();
	static void SaveScanCodeMap(HKEY_TYPE hkeyType);
	static void LoadScanCodeMap(HKEY_TYPE hkeyType);
	static BOOL Is106Keyboard();
	static BOOL GetUseDialogSetting(int nAppID);
	static void SetUseDialogSetting(int nAppID, BOOL bUseDialogSetting);
	static int GetAppID(const LPCSTR szAppName);
	static void CopyData(CString szDestinationApplication, CString szSourceApplication);
	static void StringToKey(LPCTSTR str, int& type, int& key);
	static CString KeyToString(int type, int key);
	static int GetKillRingMax(int nAppID);
	static void SetKillRingMax(int nAppID, int nKillRingMax);
	static int GetCommandID(int nAppID, int nType, int nKey);
	static void SetCommandID(int nAppID, int nType, int nKey, int nComID);
	static void SetAppTitle(const int nAppID, const CString& appTitle);
	static int DefaultAppID();
	static int AssignAppID(const LPCSTR szAppName);
	static int GetSettingStyle(const int nAppID);
	static void SetSettingStyle(const int nAppID, const int nSettingStyle);
	static void InitAppList(CProperties& cProperties);
	static int GetSavedSettingCount();
	static void ClearData(CString szCurrentApplication);
	static void InitDllData();
	static void LoadData();
	static void SaveData();

private:
	static KeyString m_KeyString;
	static BOOL DiableTokenPrivileges();
	static BOOL AdjustTokenPrivileges(LPCTSTR lpName);
	static void SaveKeyBind(const LPCTSTR appName, int comID, int type, int key);
	static void SaveKeyBind(const LPCTSTR appName, const LPCTSTR comName, int type, int key);
	static void AddKeyBind2C_(const LPCTSTR szApplicationName, BYTE bVk);
	static void LevelUp();
	static bool GetAppTitle(CString& appTitle, const CString& windowName, int nth = 1000);
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	static void SetDllData();
	static Config m_Config;
	static TCHAR m_szAppTitle[MAX_APP][WINDOW_TEXT_LENGTH];
	static void DeleteAllRegistryData();
	static TASK_LIST m_TaskList[MAX_TASKS];
	static DWORD m_dwTasks;
	static void GetTaskList();
	static void LoadRegistry();
	static void SaveRegistry();
	static void AddIMEInfo(CProperties& cProperties);
};

#endif // !defined(AFX_PROFILE_H__9415254D_4656_484B_A730_E02580D8A221__INCLUDED_)
