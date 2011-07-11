// Profile.h: interface of the CProfile class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILE_H__9415254D_4656_484B_A730_E02580D8A221__INCLUDED_)
#define AFX_PROFILE_H__9415254D_4656_484B_A730_E02580D8A221__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winperf.h>   // for Windows NT
#include "resource.h"

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
	static BOOL IsDialog(CString sz);
	static BOOL GetUseDialogSetting(int nAppID);
	static void SetUseDialogSetting(int nAppID, BOOL bUseDialogSetting);
	static int GetAppID(const LPCSTR szAppName);
	static void CopyData(CString szDestinationApplication, CString szSourceApplication);
	static void ReadKeyBind(int& pnCommandType, int& pnKey, LPCTSTR szKeyBind);
	static LPCTSTR Key2String(int nKey);
	static LPCTSTR CommandType2String(int nType);
	static int GetCurrentApplicationID(CComboBox *cApplicationList, CString szCurrentApplication);
	static int GetKillRingMax(int nAppID);
	static void SetKillRingMax(int nAppID, int nKillRingMax);
	static int GetCommandID(int nAppID, int nType, int nKey);
	static void SetCommandID(int nAppID, int nType, int nKey, int nComID);
	static void UpdateApplicationTitle(CComboBox *cApplicationList, CString szCurrentApplication, int nAppID, BOOL bSaveAndValidate);
	static void GetApplicationTitle(CComboBox *cApplicationList, CString &rList, int nIndex = -1);
	static BOOL IsDefault(CString sz);
	static int GetApplicationIndex(CString szApplicationName, BOOL bSaveAndValidate, int *nSettingStyle);
	static void InitApplicationList(CComboBox *cApplicationList);
	static int GetSavedSettingCount();
	static void ClearData(CString szCurrentApplication);
	static void InitDllData();
	static void LoadData();
	static void SaveData();

private:
	static void SaveKeyBind(const LPCSTR szAppName, const LPCSTR szComName, int nType, int nKey);
	static BOOL DiableTokenPrivileges();
	static BOOL AdjustTokenPrivileges(LPCTSTR lpName);
	static void SaveCommand(const LPCSTR szAppName, int nComID);
	static void SaveKeyBind(const LPCSTR szAppName, int nComID, int nType, int nKey);
	static void AddKeyBind2C_(const LPCSTR szApplicationName, BYTE bVk);
	static void LevelUp();
	static void Item2AppName(CString *sz);
	static int IsNotSameString(CComboBox *pApplication, CString szListItem);
	static int CountSeparator(CString szMainString, CString szSeparator);
	static void GetNthString(CString *szAppName, CString szWindowName, CString szSeparator, int n);
	static void GetAppName(CString *szAppName, LPCTSTR pWindowName);
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	static BOOL IsTheString(CString sz, UINT nID);
	static void SetDllData();
	static CONFIG m_Config;
	static TCHAR m_szAppTitle[MAX_APP][WINDOW_TEXT_LENGTH];
	static void DeleteAllRegistryData();
	static TASK_LIST m_TaskList[MAX_TASKS];
	static DWORD m_dwTasks;
	static void GetTaskList();
	static BOOL IsCommandType(int nType, LPCTSTR szKeyBind);
	static int KeyBind2Key(const LPCTSTR szKey);
	static int KeyBind2CommandType(const LPCTSTR szKeyBind);
	static CString WriteKeyBind(int nType, int nKey);
	static void LoadRegistry();
	static void SaveRegistry();
	static void AddIMEInfo(CComboBox *cApplicationList);
};

#endif // !defined(AFX_PROFILE_H__9415254D_4656_484B_A730_E02580D8A221__INCLUDED_)
