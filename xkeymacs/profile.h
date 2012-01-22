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
	static void GetAppList(TCHAR (&appTitle)[MAX_APP][WINDOW_TEXT_LENGTH], TCHAR (&appName)[MAX_APP][CLASS_NAME_LENGTH]);
	static void ClearData(LPCTSTR appName);
	static void CopyDefault(LPCTSTR appName);
	static void CopyDefault(int dst);
	static int AssignAppID(LPCTSTR appName);
	static int DefaultAppID();
	static int GetAppID(LPCTSTR appName);
	static int GetSettingStyle(int nAppID);
	static void SetSettingStyle(int nAppID, int nSettingStyle);
	static void SetAppTitle(int nAppID, const CString& appTitle);
	static int GetCmdID(int nAppID, int nType, int nKey);
	static void SetCmdID(int nAppID, int nType, int nKey, int nComID);
	static int GetFuncID(int nAppID, int nType, int nKey);
	static void SetFuncID(int nAppID, int nType, int nKey, int nFuncID);
	static bool GetUseDialogSetting(int nAppID);
	static void SetUseDialogSetting(int nAppID, bool setting);
	static void SetEnableCUA(int nAppID, bool enable);
	static bool GetEnableCUA(int nAppID);
	static int GetKillRingMax(int nAppID);
	static void SetKillRingMax(int nAppID, int nKillRingMax);
	static LPCTSTR GetWindowText(int nAppID);
	static void SetWindowText(int nAppID, const CString& text);
	static bool Is106Keyboard();
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
	static BOOL DiableTokenPrivileges();
	static BOOL AdjustTokenPrivileges(LPCTSTR lpName);
};

#endif // !defined(AFX_PROFILE_H__9415254D_4656_484B_A730_E02580D8A221__INCLUDED_)
