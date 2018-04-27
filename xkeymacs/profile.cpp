// Profile.cpp: implementation of the CProfile class
//
//////////////////////////////////////////////////////////////////////

#include "profile.h"
#include "xkeymacs.h"
#include "FuncDefs.h"
#include "mainfrm.h"
#include "../xkeymacsdll/xkeymacsdll.h"
#include "../xkeymacsdll/Utils.h"
#include "../xkeymacsdll/PipeName.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Config CProfile::m_Config;
KeyString CProfile::m_KeyString(CProfile::Is106Keyboard() != FALSE);
TCHAR CProfile::m_AppTitle[MAX_APP][WINDOW_TEXT_LENGTH];

void CProfile::LoadData()
{
	FuncDefs::Load();
	LevelUp();
	LoadRegistry();
}

void CProfile::SaveData()
{
	DeleteAllRegistryData();
	SaveRegistry();
	SetDllData();
}

void CProfile::DeleteAllRegistryData()
{
	HKEY hkey = NULL;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, CString(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA)), 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS) {
		// I am sure that I have to do only one time, but...
		for (int nAppID = 0; nAppID < MAX_APP; ++nAppID) {
			DWORD dwIndex = 0;
			TCHAR szName[SUB_KEY_NAME_LENGTH] = {'\0'};
			DWORD dwName = sizeof(szName);
			FILETIME filetime;

			while (RegEnumKeyEx(hkey, dwIndex++, szName, &dwName, NULL, NULL, NULL, &filetime) == ERROR_SUCCESS) {
//				RegDeleteKey(hkey, szName);
				SHDeleteKey(hkey, szName);
				ZeroMemory(szName, sizeof(szName));
				dwName = sizeof(szName);
			}
		}
		RegCloseKey(hkey);
	}
}

void CProfile::LevelUp()
{
	int nCurrentLevel = AfxGetApp()->GetProfileInt(_T(""), _T("Level"), 0);
	for (int nAppID = 0; nAppID < MAX_APP; ++nAppID) {
		CString entry;
		entry.Format(IDS_REG_ENTRY_APPLICATION, nAppID);
		const CString appName = AfxGetApp()->GetProfileString(CString(MAKEINTRESOURCE(IDS_REG_SECTION_APPLICATION)), entry);
		if (appName.IsEmpty())
			continue;
		switch (nCurrentLevel) {
		case 0:
			AddKeyBind2C_(appName, VK_LCONTROL);
			AddKeyBind2C_(appName, VK_RCONTROL);
		// fall through
		case 1:
			// Set kill-ring-max 1 if it is 0.
			if (!AfxGetApp()->GetProfileInt(appName, CString(MAKEINTRESOURCE(IDS_REG_ENTRY_KILL_RING_MAX)), 0))
				AfxGetApp()->WriteProfileInt(appName, CString(MAKEINTRESOURCE(IDS_REG_ENTRY_KILL_RING_MAX)), 1);
		// fall through
		case 2:
			{
				// Chaged a label from Enter to newline.
				const CString subKey = CString(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA)) + _T("\\") + appName;
				const CString srcKey = subKey + _T("\\") + _T("Enter");
				const CString dstKey = subKey + _T("\\") + _T("newline");
				HKEY hDstKey = NULL;
				if (RegCreateKeyEx(HKEY_CURRENT_USER, dstKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hDstKey, NULL) == ERROR_SUCCESS) {
					SHCopyKey(HKEY_CURRENT_USER, srcKey, hDstKey, NULL);
					SHDeleteKey(HKEY_CURRENT_USER, srcKey);
					RegCloseKey(hDstKey);
				}
			}
		// fall through
		case 3:
			// rename original function to remove IDS_REG_ORIGINAL_PREFIX
			for (int nFuncID = 0; nFuncID < FuncDefs::GetNumOfDefs(); ++nFuncID) {
				HKEY hKey = NULL;
				const CString subKey = CString(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA)) + _T("\\") + appName + _T("\\") + CString(MAKEINTRESOURCE(IDS_REG_ORIGINAL_PREFIX)) + FuncDefs::GetName(nFuncID);
				if (RegOpenKeyEx(HKEY_CURRENT_USER, subKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
					TCHAR szKeyBind[128];
					DWORD dwKeyBind = sizeof(szKeyBind);
					for (DWORD dwIndex = 0; RegEnumKeyEx(hKey, dwIndex, szKeyBind, &dwKeyBind, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; ++dwIndex) {
						int nType, nKey;
						StringToKey(szKeyBind, nType, nKey);
						SaveKeyBind(appName, FuncDefs::GetName(nFuncID), nType, nKey);
						dwKeyBind = sizeof(szKeyBind);
					}
					RegCloseKey(hKey);
				}
			}
		}
	}
	AfxGetApp()->WriteProfileInt(_T(""), _T("Level"), 4);
}

void CProfile::AddKeyBind2C_(LPCTSTR appName, BYTE bVk)
{
	int nComID;
	for (nComID = 0; nComID < MAX_COMMAND; ++nComID)
		if (CmdTable::Command(nComID) == CCommands::C_)
			break;
	SaveKeyBind(appName, nComID, NONE, bVk);
}

void CProfile::LoadRegistry()
{
	bool bDialog = false;
	const CString section(MAKEINTRESOURCE(IDS_REG_SECTION_APPLICATION));
	ZeroMemory(&m_Config, sizeof(m_Config));
	for (int nAppID = 0; nAppID < MAX_APP; ++nAppID) {
		AppConfig& appConfig = m_Config.AppConfig[nAppID];
		CString entry;
		entry.Format(IDS_REG_ENTRY_APPLICATION, nAppID);
		CString appName = AfxGetApp()->GetProfileString(section, entry);
		if (appName.IsEmpty())  {
			if (nAppID) {
				if (bDialog)
					continue;
				appName.LoadString(IDS_DIALOG);
				bDialog = true;
			} else
				appName.LoadString(IDS_DEFAULT);
		} else if (appName == CString(MAKEINTRESOURCE(IDS_DIALOG)))
			bDialog = true;
		_tcsncpy_s(appConfig.AppName, appName, _TRUNCATE);
		entry.LoadString(IDS_REG_ENTRY_APPLICATOIN_TITLE);
		_tcsncpy_s(m_AppTitle[nAppID], AfxGetApp()->GetProfileString(appName, entry), _TRUNCATE);
		entry.LoadString(IDS_REG_ENTRY_WINDOW_TEXT);
		_tcsncpy_s(appConfig.WindowText, AfxGetApp()->GetProfileString(appName, entry, _T("*")), _TRUNCATE);

		const CString regApp = CString(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA)) + _T("\\") + appName;
		for (BYTE nComID = 1; nComID < MAX_COMMAND; ++nComID) {
			entry = CmdTable::Name(nComID);
			HKEY hKey;
			const CString regKey = regApp + _T("\\") + entry;
			if (RegOpenKeyEx(HKEY_CURRENT_USER, regKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
				TCHAR szKeyBind[128];
				DWORD dwKeyBind = _countof(szKeyBind);
				for (DWORD dwIndex = 0; RegEnumKeyEx(hKey, dwIndex, szKeyBind, &dwKeyBind, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; ++dwIndex) {
					int nType, nKey;
					StringToKey(szKeyBind, nType, nKey);
					appConfig.CmdID[nType][nKey] = nComID;
					dwKeyBind = _countof(szKeyBind);
				}
				RegCloseKey(hKey);
			} else {
				// Set the default assignment
				for (int i = 0; i < MAX_KEY_BIND; ++i) {
					KeyBind bind = CmdTable::Bind(nComID, i);
					if (bind.bVk == 0)
						break;
					if (bind.nControlID == IDC_CO2)
						continue;
					appConfig.CmdID[bind.nType][bind.bVk] = nComID;
				}
			}
		}
		memset(appConfig.FuncID, -1, sizeof(appConfig.FuncID));
		for (int nFuncID = 0; nFuncID < FuncDefs::GetNumOfDefs(); ++nFuncID) {
			HKEY hKey;
			CString regKey = regApp + _T("\\") + FuncDefs::GetName(nFuncID);
			if (RegOpenKeyEx(HKEY_CURRENT_USER, regKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
				TCHAR szKeyBind[128];
				DWORD dwKeyBind = _countof(szKeyBind);
				for (DWORD dwIndex = 0; RegEnumKeyEx(hKey, dwIndex, szKeyBind, &dwKeyBind, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; ++dwIndex) {
					int nType, nKey;
					StringToKey(szKeyBind, nType, nKey);
					appConfig.FuncID[nType][nKey] = static_cast<char>(nFuncID);
					dwKeyBind = _countof(szKeyBind);
				}
				RegCloseKey(hKey);
			}
		}

		entry.LoadString(IDS_REG_ENTRY_KILL_RING_MAX);
		int n = AfxGetApp()->GetProfileInt(appName, entry, 1);
		appConfig.KillRingMax = static_cast<BYTE>(n > 255 ? 255 : n);
		entry.LoadString(IDS_REG_ENTRY_DISABLE_XKEYMACS);
		appConfig.SettingStyle = static_cast<BYTE>(AfxGetApp()->GetProfileInt(appName, entry, 0) ? SETTING_DISABLE : SETTING_SPECIFIC);
		entry.LoadString(IDS_REG_ENTRY_USE_DIALOG_SETTING);
		appConfig.UseDialogSetting = AfxGetApp()->GetProfileInt(appName, entry, 1) != 0;
		entry.LoadString(IDC_REG_ENTRY_IGNORE_META_CTRL);
		appConfig.IgnoreUndefMetaCtrl = AfxGetApp()->GetProfileInt(appName, entry, 0) != 0;
		entry.LoadString(IDC_REG_ENTRY_IGNORE_C_X);
		appConfig.IgnoreUndefC_x = AfxGetApp()->GetProfileInt(appName, entry, 0) != 0;
		entry.LoadString(IDC_REG_ENTRY_ENABLE_CUA);
		appConfig.EnableCUA = AfxGetApp()->GetProfileInt(appName, entry, 0) != 0;
		entry.LoadString(IDS_REG_ENTRY_326_COMPATIBLE);
		appConfig.Is326Compatible = AfxGetApp()->GetProfileInt(appName, entry, 0) != 0;
	}
}

void CProfile::SaveRegistry()
{
	const CString section(MAKEINTRESOURCE(IDS_REG_SECTION_APPLICATION));	
	for (int nAppID = 0; nAppID < MAX_APP; ++nAppID) {
		const AppConfig& appConfig = m_Config.AppConfig[nAppID];
		LPCTSTR appName = appConfig.AppName;
		CString entry;
		entry.Format(IDS_REG_ENTRY_APPLICATION, nAppID);
		if (!appName[0]) {
			if (!AfxGetApp()->GetProfileString(section, entry).IsEmpty())
				AfxGetApp()->WriteProfileString(section, entry, _T(""));
			continue;
		}
		AfxGetApp()->WriteProfileString(section, entry, appName);

		entry.LoadString(IDS_REG_ENTRY_APPLICATOIN_TITLE);
		CString appTitle = m_AppTitle[nAppID];
		appTitle.TrimLeft(_T(' '));
		AfxGetApp()->WriteProfileString(appName, entry, appTitle);
		entry.LoadString(IDS_REG_ENTRY_WINDOW_TEXT);
		AfxGetApp()->WriteProfileString(appName, entry, appConfig.WindowText);

		// Create all commands
		for (int nComID = 1; nComID < MAX_COMMAND; ++nComID)
			SaveKeyBind(appName, nComID, 0, 0);
		for (int nType = 0; nType < MAX_COMMAND_TYPE; ++nType)
			for (int nKey = 0; nKey < MAX_KEY; ++nKey) {
				SaveKeyBind(appName, appConfig.CmdID[nType][nKey], nType, nKey);
				int id = appConfig.FuncID[nType][nKey];
				if (id >= 0)
					SaveKeyBind(appName, FuncDefs::GetName(id), nType, nKey);
			}

		entry.LoadString(IDS_REG_ENTRY_KILL_RING_MAX);
		AfxGetApp()->WriteProfileInt(appName, entry, appConfig.KillRingMax);
		entry.LoadString(IDS_REG_ENTRY_USE_DIALOG_SETTING);
		AfxGetApp()->WriteProfileInt(appName, entry, appConfig.UseDialogSetting);
		entry.LoadString(IDS_REG_ENTRY_DISABLE_XKEYMACS);
		AfxGetApp()->WriteProfileInt(appName, entry, appConfig.SettingStyle == SETTING_DISABLE);
		entry.LoadString(IDC_REG_ENTRY_IGNORE_META_CTRL);
		AfxGetApp()->WriteProfileInt(appName, entry, appConfig.IgnoreUndefMetaCtrl);
		entry.LoadString(IDC_REG_ENTRY_IGNORE_C_X);
		AfxGetApp()->WriteProfileInt(appName, entry, appConfig.IgnoreUndefC_x);
		entry.LoadString(IDC_REG_ENTRY_ENABLE_CUA);
		AfxGetApp()->WriteProfileInt(appName, entry, appConfig.EnableCUA);
		entry.LoadString(IDS_REG_ENTRY_326_COMPATIBLE);
		AfxGetApp()->WriteProfileInt(appName, entry, appConfig.Is326Compatible);
	}
}

void CProfile::SetDllData()
{
	memcpy(m_Config.FuncDefs, FuncDefs::GetDefs(), sizeof(m_Config.FuncDefs));
	for (int nAppID = 0; nAppID < MAX_APP; ++nAppID) {
		AppConfig& appConfig = m_Config.AppConfig[nAppID];
		appConfig.CmdID[CONTROL]['X'] = 0; // C-x is unassigned.
		for (int nType = 0; nType < MAX_COMMAND_TYPE; ++nType)
			for (int nKey = 0; nKey < MAX_KEY; ++nKey)
				if ((nType & CONTROLX) && (appConfig.CmdID[nType][nKey] || appConfig.FuncID[nType][nKey] >= 0))
					appConfig.CmdID[CONTROL]['X'] = 1; // C-x is available.
	}
	m_Config.Is106Keyboard = Is106Keyboard();
	_tcscpy_s(m_Config.PipeNameForIPC32, PipeName(PIPENAME_IPC32).GetName());
	CXkeymacsDll::SetConfig(m_Config);
	if (!CXkeymacsApp::IsWow64())
		return;
	if (!CXkeymacsDll::SaveConfig())
		return;
	CXkeymacsApp::SendIPC64Message(IPC64_RELOAD);
}

void CProfile::SaveKeyBind(LPCTSTR appName, int comID, int type, int key)
{
	if (!comID)
		return;
	LPCTSTR comName = CmdTable::Name(comID);
	if (!comName[0])
		return;
	SaveKeyBind(appName, comName, type, key);
}

void CProfile::SaveKeyBind(LPCTSTR appName, LPCTSTR comName, int type, int key)
{
	CString subKey = CString(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA)) + _T("\\") + appName + _T("\\") + comName;
	CString s = KeyToString(type, key);
	if (!s.IsEmpty())
		subKey = subKey + _T("\\") + s;
	HKEY hKey = NULL;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, subKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
		RegCloseKey(hKey);
}

void CProfile::StringToKey(LPCTSTR str, int& type, int& key)
{
	m_KeyString.ToKey(str, type, key);
}

CString CProfile::KeyToString(int type, int key)
{
	return m_KeyString.ToString(type, key);
}

void CProfile::GetAppList(TCHAR (&appTitle)[MAX_APP][WINDOW_TEXT_LENGTH], TCHAR (&appName)[MAX_APP][CLASS_NAME_LENGTH])
{
	memcpy(appTitle, m_AppTitle, sizeof(m_AppTitle));
	for (int i = 0; i < MAX_APP; ++i)
		memcpy(appName[i], m_Config.AppConfig[i].AppName, CLASS_NAME_LENGTH);
}

void CProfile::ClearData(LPCTSTR appName)
{
	int n = GetAppID(appName);
	if (n == MAX_APP)
		return;
	AppConfig& appConfig = m_Config.AppConfig[n];
	ZeroMemory(appConfig.CmdID, sizeof(appConfig.CmdID));
	ZeroMemory(appConfig.AppName, sizeof(appConfig.AppName));
}

void CProfile::CopyDefault(LPCTSTR appName)
{
	CopyDefault(GetAppID(appName));
}

void CProfile::CopyDefault(int dst)
{
	int src = DefaultAppID();
	if (src == MAX_APP || dst == MAX_APP)
		return;
	m_Config.AppConfig[dst].SettingStyle = SETTING_SPECIFIC;
	size_t size = sizeof(AppConfig) - offsetof(AppConfig, CmdID);
	memcpy_s(&m_Config.AppConfig[dst].CmdID, size, &m_Config.AppConfig[src].CmdID, size);
}

int CProfile::AssignAppID(LPCTSTR appName)
{
	int id = GetAppID(appName);
	if (id != MAX_APP)
		return id;
	for (id = 0; id < MAX_APP; ++id) {
		AppConfig& appConfig = m_Config.AppConfig[id];
		if (!appConfig.AppName[0]) {
			_tcsncpy_s(appConfig.AppName, appName, _TRUNCATE);
			CopyDefault(id);
			return id;
		}
	}
	return id;
}

int CProfile::DefaultAppID()
{
	const CString name(MAKEINTRESOURCE(IDS_DEFAULT));
	for(int nAppID = 0; nAppID < MAX_APP; ++nAppID)
		if (name == m_Config.AppConfig[nAppID].AppName)
			return nAppID;
	return MAX_APP;
}

int CProfile::GetAppID(LPCTSTR appName)
{
	int nAppID = 0;
	for (nAppID = 0; nAppID < MAX_APP; ++nAppID)
		if (!_tcsicmp(appName, m_Config.AppConfig[nAppID].AppName))
			break;
	return nAppID;
}

int CProfile::GetSettingStyle(int nAppID)
{
	if (nAppID == MAX_APP)
		return SETTING_DEFAULT;
	return m_Config.AppConfig[nAppID].SettingStyle;
}

void CProfile::SetSettingStyle(int nAppID, int nSettingStyle)
{
	if (nAppID == MAX_APP)
		return;
	m_Config.AppConfig[nAppID].SettingStyle = static_cast<BYTE>(nSettingStyle);
}

void CProfile::SetAppTitle(int nAppID, const CString& appTitle)
{
	_tcsncpy_s(m_AppTitle[nAppID], appTitle, _TRUNCATE);
}

int CProfile::GetCmdID(int nAppID, int nType, int nKey)
{
	int nComID = m_Config.AppConfig[nAppID].CmdID[nType][nKey];
	if (nKey == 0xf0 && CmdTable::Command(nComID) == CCommands::C_Eisu)
		// Change CommandID C_
		for (nComID = 1; nComID < MAX_COMMAND; nComID++)
			if (CmdTable::Command(nComID) == CCommands::C_)
				break;
	return nComID;
}

void CProfile::SetCmdID(int nAppID, int nType, int nKey, int nComID)
{
	if (nKey == 0xf0 && CmdTable::Command(nComID) == CCommands::C_)
		// Change CommandID C_Eisu
		for (nComID = 1; nComID < MAX_COMMAND; ++nComID)
			if (CmdTable::Command(nComID) == CCommands::C_Eisu)
				break;
	m_Config.AppConfig[nAppID].CmdID[nType][nKey] = static_cast<BYTE>(nComID);
}

int CProfile::GetFuncID(int nAppID, int nType, int nKey)
{
	return m_Config.AppConfig[nAppID].FuncID[nType][nKey];
}

void CProfile::SetFuncID(int nAppID, int nType, int nKey, int nFuncID)
{
	m_Config.AppConfig[nAppID].FuncID[nType][nKey] = static_cast<BYTE>(nFuncID);
}

bool CProfile::GetUseDialogSetting(int nAppID)
{
	return m_Config.AppConfig[nAppID].UseDialogSetting;
}

void CProfile::SetUseDialogSetting(int nAppID, bool setting)
{
	m_Config.AppConfig[nAppID].UseDialogSetting = setting;
}

bool CProfile::GetEnableCUA(int nAppID)
{
	return m_Config.AppConfig[nAppID].EnableCUA;
}

void CProfile::SetEnableCUA(int nAppID, bool bEnableCUA)
{
	m_Config.AppConfig[nAppID].EnableCUA = static_cast<BYTE>(bEnableCUA);
}

int CProfile::GetKillRingMax(int nAppID)
{
	return m_Config.AppConfig[nAppID].KillRingMax;
}

void CProfile::SetKillRingMax(int nAppID, int nKillRingMax)
{
	m_Config.AppConfig[nAppID].KillRingMax = static_cast<BYTE>(nKillRingMax > 255 ? 255 : nKillRingMax);
}

LPCTSTR CProfile::GetWindowText(int nAppID)
{
	return m_Config.AppConfig[nAppID].WindowText;
}

void CProfile::SetWindowText(int nAppID, const CString& text)
{
	_tcsncpy_s(m_Config.AppConfig[nAppID].WindowText,
		CUtils::GetWindowTextType(text) == IDS_WINDOW_TEXT_IGNORE ? _T("*") : text, _TRUNCATE);
}

bool CProfile::Is106Keyboard()
{
	static KEYBOARD_TYPE keyboard = UNKNOWN_KEYBOARD;

	if (keyboard == UNKNOWN_KEYBOARD) {
		OSVERSIONINFO verInfo = {0};
		verInfo.dwOSVersionInfoSize = sizeof (verInfo);
		GetVersionEx(&verInfo);

		DWORD subtype = 0;
		DWORD cbData = sizeof(subtype);

		if (verInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) {
			HKEY hKey = NULL;
			const CString szSubKey(_T("SYSTEM\\CurrentControlSet\\Services\\i8042prt\\Parameters"));
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szSubKey, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
				static const CString szValueName(_T("OverrideKeyboardSubtype"));
				if (RegQueryValueEx(hKey, szValueName, NULL, NULL, (LPBYTE)&subtype, &cbData) != ERROR_SUCCESS) {
					subtype = 0;
				}
				RegCloseKey(hKey);
			}
		} else if (verInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
			subtype = GetPrivateProfileInt(_T("keyboard"), _T("subtype"), 0, _T("system.ini"));
		}

		keyboard = (subtype & 0x02) ? JAPANESE_KEYBOARD : ENGLISH_KEYBOARD;
	}

	return keyboard == JAPANESE_KEYBOARD;
}

BOOL CProfile::IsVistaOrLater()
{
	OSVERSIONINFO info = {sizeof(OSVERSIONINFO)};
	GetVersionEx(&info);

	if (6 <= info.dwMajorVersion) {
		return TRUE;
	}
	return FALSE;
}

void CProfile::RestartComputer()
{
	if (!AdjustTokenPrivileges(SE_SHUTDOWN_NAME)) {
		return;
	}

	ExitWindowsEx(EWX_REBOOT, 0);
}

void CProfile::ImportProperties()
{
	if (!AdjustTokenPrivileges(SE_RESTORE_NAME)) {
		return;
	}

	CFileDialog oFileOpenDialog(TRUE, _T("reg"), _T("xkeymacs"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, CString(MAKEINTRESOURCE(IDS_REGISTRATION_FILTER)));
	if (oFileOpenDialog.DoModal() == IDOK) {
		CString szCommandLine;
		szCommandLine.Format(_T("reg import \"%s\""), oFileOpenDialog.GetPathName());
		CUtils::Run(szCommandLine, TRUE, TRUE);	// reg import "x:\xkeymacs.reg"
	}

	DiableTokenPrivileges();
	return;
}

void CProfile::ExportProperties()
{
	if (!AdjustTokenPrivileges(SE_BACKUP_NAME)) {
		return;
	}

	CFileDialog oFileOpenDialog(FALSE, _T("reg"), _T("xkeymacs"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, CString(MAKEINTRESOURCE(IDS_REGISTRATION_FILTER)));
	if (oFileOpenDialog.DoModal() == IDOK) {
		CString szCommandLine;
		szCommandLine.Format(_T("reg export HKEY_CURRENT_USER\\%s \"%s\" /y"), CString(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA)), oFileOpenDialog.GetPathName());
		CUtils::Run(szCommandLine, TRUE, TRUE);	// reg export HKEY_CURRENT_USER\Software\Oishi\XKeymacs2 "x:\xkeymacs.reg" /y
	}

	DiableTokenPrivileges();
	return;
}

BOOL CProfile::AdjustTokenPrivileges(LPCTSTR lpName)
{
	BOOL rc = TRUE;

	HANDLE hToken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
		LUID luid;
		if (LookupPrivilegeValue(NULL, lpName, &luid)) {
			TOKEN_PRIVILEGES tp;
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = luid;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			if (!::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
				rc = FALSE;
			}
		} else {
			rc = FALSE;
		}
		CloseHandle(hToken);
	} else {
		rc = FALSE;
	}

	return rc;
}

BOOL CProfile::DiableTokenPrivileges()
{
	BOOL rc = TRUE;

	HANDLE hToken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
		if (!::AdjustTokenPrivileges(hToken, TRUE, NULL, NULL, NULL, NULL)) {
			rc = FALSE;
		}
		CloseHandle(hToken);
	} else {
		rc = FALSE;
	}

	return rc;
}

int CProfile::GetKeyboardSpeed()
{
	int nKeyboardSpeed = 31; // default value of Windows
	CString szSubKey(MAKEINTRESOURCE(IDS_REGSUBKEY_KEYBOARD));
	CString szValueName(MAKEINTRESOURCE(IDS_KEYBOARD_SPEED));

	HKEY hKey = NULL;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
		// get data size
		DWORD dwType = REG_SZ;
		BYTE data[4] = {0};
		DWORD dwcbData = sizeof(data)/sizeof(data[0]);
		RegQueryValueEx(hKey, szValueName, NULL, &dwType, (LPBYTE)&data, &dwcbData);
		RegCloseKey(hKey);

		for (DWORD i = 0; i < dwcbData; ++i) {
			if (data[i]) {
				if (i) {
					nKeyboardSpeed = nKeyboardSpeed * 10 + data[i] - _T('0');
				} else {
					nKeyboardSpeed = data[i] - _T('0');
				}
			} else {
				break;
			}
		}
	}

	return nKeyboardSpeed;
}
