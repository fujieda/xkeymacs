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
	static void LoadData();
	static void SaveData();
	static void InitDllData();
	static void StringToKey(LPCTSTR str, int& type, int& key);
	static CString KeyToString(int type, int key);
	static void InitAppList(CProperties& cProperties);
	static void CopyData(CString szDestinationApplication, CString szSourceApplication);
	static void ClearData(CString szCurrentApplication);
	static int AssignAppID(LPCSTR appName);
	static int DefaultAppID();
	static int GetAppID(LPCSTR appName);
	static int GetSettingStyle(int nAppID);
	static void SetSettingStyle(int nAppID, int nSettingStyle);
	static void SetAppTitle(int nAppID, const CString& appTitle);
	static int GetCommandID(int nAppID, int nType, int nKey);
	static void SetCommandID(int nAppID, int nType, int nKey, int nComID);
	static BOOL GetUseDialogSetting(int nAppID);
	static void SetUseDialogSetting(int nAppID, BOOL bUseDialogSetting);
	static void SetEnableCUA(int nAppID, BOOL bEnableCUA);
	static BOOL GetEnableCUA(int nAppID);
	static int GetKillRingMax(int nAppID);
	static void SetKillRingMax(int nAppID, int nKillRingMax);
	static CString GetWindowText(int nAppID);
	static void SetWindowText(int nAppID, CString szWindowText);
	static BOOL Is106Keyboard();
	static BOOL IsVistaOrLater();
	static void RestartComputer();
	static void ImportProperties();
	static void ExportProperties();
	static int GetKeyboardSpeed(void);

private:
	static Config m_Config;
	static KeyString m_KeyString;
	static TCHAR m_AppTitle[MAX_APP][WINDOW_TEXT_LENGTH];
	static void DeleteAllRegistryData();
	static void LevelUp();
	static void AddKeyBind2C_(LPCTSTR szApplicationName, BYTE bVk);
	static void LoadRegistry();
	static void SaveRegistry();
	static void SetDllData();
	static void SaveKeyBind(LPCTSTR appName, int comID, int type, int key);
	static void SaveKeyBind(LPCTSTR appName, LPCTSTR comName, int type, int key);
	static TASK_LIST m_TaskList[MAX_TASKS];
	static DWORD m_dwTasks;
	static void GetTaskList();
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	static void AddIMEInfo(CProperties& cProperties);
	static BOOL DiableTokenPrivileges();
	static BOOL AdjustTokenPrivileges(LPCTSTR lpName);
	static bool GetAppTitle(CString& appTitle, const CString& windowName, int nth = 1000);
};

#endif // !defined(AFX_PROFILE_H__9415254D_4656_484B_A730_E02580D8A221__INCLUDED_)
