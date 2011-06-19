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
#include "data.h"

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
	static CString GetWindowText(int nApplicationID);
	static void SetWindowText(int nApplicationID, CString szWindowText);
	static void SetEnableCUA(int nApplicationID, BOOL bEnableCUA);
	static BOOL GetEnableCUA(int nApplicationID);
	static void ImportProperties();
	static void ExportProperties();
	static void RestartComputer();
	static BOOL IsVistaOrLater();
	static void SaveScanCodeMap(HKEY_TYPE hkeyType);
	static void LoadScanCodeMap(HKEY_TYPE hkeyType);
	static BOOL Is106Keyboard();
	static BOOL IsDialog(CString sz);
	static BOOL GetUseDialogSetting(int nApplicationID);
	static void SetUseDialogSetting(int nApplicationID, BOOL bUseDialogSetting);
	static int GetApplicationIndex(CString szApplicationName);
	static void CopyData(CString szDestinationApplication, CString szSourceApplication);
	static void ReadKeyBind(int *pnCommandType, int *pnKey, LPCTSTR szKeyBind);
	static LPCTSTR Key2String(int nKey);
	static LPCTSTR CommandType2String(int nCommandType);
	static int GetCurrentApplicationID(CComboBox *cApplicationList, CString szCurrentApplication);
	static int GetKillRingMax(int nApplicationID);
	static void SetKillRingMax(int nApplicationID, int nKillRingMax);
	static int GetCommandID(int nApplicationID, int nCommandType, int nKey);
	static void SetCommandID(int nApplicationID, int nCommandType, int nKey, int nCommandID);
	static void UpdateApplicationTitle(CComboBox *cApplicationList, CString szCurrentApplication, int nApplicationID, BOOL bSaveAndValidate);
	static void GetApplicationTitle(CComboBox *cApplicationList, CString &rList, int nIndex = -1);
	static BOOL IsDefault(CString sz);
	static int GetApplicationIndex(CString szApplicationName, BOOL bSaveAndValidate, int *nSettingStyle);
	static void InitApplicationList(CComboBox *cApplicationList);
	static int GetSavedSettingCount();
	static void ClearData(CString szCurrentApplication);
	static void InitDllData();
	static void SaveData();
	static void LoadData();
	CProfile();
	virtual ~CProfile();

private:
	static void SaveKeyBind(CString szApplicationName, CString szCommandName, int nCommandType, int nKey);
	static BOOL DiableTokenPrivileges();
	static BOOL AdjustTokenPrivileges(LPCTSTR lpName);
	static void SaveCommand(CString szApplicationName, int nCommandID);
	static void SaveKeyBind(CString szApplicationName, int nCommandID, int nCommandType, int nKey);
	static void AddKeyBind2C_(CString szApplicationName, BYTE bVk);
	static void LevelUp();
	static void Item2AppName(CString *sz);
	static int IsNotSameString(CComboBox *pApplication, CString szListItem);
	static int CountSeparator(CString szMainString, CString szSeparator);
	static void GetNthString(CString *szAppName, CString szWindowName, CString szSeparator, int n);
	static void GetAppName(CString *szAppName, LPCTSTR pWindowName);
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	static BOOL IsTheString(CString sz, UINT nID);
	static void SetDllData();
	static CData m_Data[MAX_APP];
	static void DeleteAllRegistryData();
	static TASK_LIST m_TaskList[MAX_TASKS];
	static DWORD m_dwTasks;
	static void GetTaskList();
	static BOOL IsCommandType(int nCommandType, LPCTSTR szKeyBind);
	static int KeyBind2Key(LPCTSTR szKey);
	static int KeyBind2CommandType(LPCTSTR szKeyBind);
	static CString WriteKeyBind(int nCommandType, int nKey);
	static void UpdateRegistryData(BOOL bSaveAndValidate);
};

#endif // !defined(AFX_PROFILE_H__9415254D_4656_484B_A730_E02580D8A221__INCLUDED_)
