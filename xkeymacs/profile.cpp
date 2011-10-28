// Profile.cpp: implementation of the CProfile class
//
//////////////////////////////////////////////////////////////////////

#include "profile.h"
#include "imelist.h"
#include "xkeymacs.h"
#include "dotxkeymacs.h"
#include "mainfrm.h"
#include "../xkeymacsdll/xkeymacsdll.h"
#include "../xkeymacsdll/Utils.h"
#include <TlHelp32.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Config CProfile::m_Config;
TCHAR CProfile::m_szAppTitle[MAX_APP][WINDOW_TEXT_LENGTH];
TASK_LIST CProfile::m_TaskList[MAX_TASKS];
DWORD CProfile::m_dwTasks;
KeyString CProfile::m_KeyString(CProfile::Is106Keyboard() != FALSE);

// This function returns the nth string in a window name separated by " - ".
// If there aren't a sufficient number of strings, it returns the last string
// appropriate for the title.
bool CProfile::GetAppTitle(CString& appTitle, const CString& windowName, int nth)
{
	const CString sep(MAKEINTRESOURCE(IDS_SEPARATE_WINDOWTITLE));
	const int nSep = windowName.Find(sep);
	if (nSep < 0) {
		appTitle = windowName;
		return false;
	}
	if (GetAppTitle(appTitle, windowName.Right(windowName.GetLength() - nSep - sep.GetLength()), --nth) ||
			!nth || nth > 0 && appTitle.GetAt(0) != _T('[') && appTitle.FindOneOf(_T(".]")) == -1)
		return true;
	appTitle = windowName.Left(nSep);
	return false;
}

BOOL CALLBACK CProfile::EnumWindowsProc(const HWND hWnd, const LPARAM lParam)
{
	CProperties *pProperties = reinterpret_cast<CProperties*>(lParam);
	PTASK_LIST pTask = CProfile::m_TaskList;
	
	TCHAR szWindowName[WINDOW_TEXT_LENGTH];
	TCHAR szClassName[CLASS_NAME_LENGTH];
	WINDOWPLACEMENT wpl;
	
	wpl.length = sizeof(WINDOWPLACEMENT);
	::GetWindowText(hWnd, szWindowName, sizeof(szWindowName));
	GetClassName(hWnd, szClassName, sizeof(szClassName));

	CString appTitle;
	// Get Process Name
	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	DWORD i;
	for (i = 0; i < CProfile::m_dwTasks; ++i) {
		if (pTask[i].dwProcessId == dwProcessId) {

			// Get Application Name
			if (szWindowName[0] == '\0') {
				continue;
			}
			if (!_tcsnicmp(pTask[i].ProcessName, CString(MAKEINTRESOURCE(IDS_B2)), sizeof(pTask[i].ProcessName))) {
				appTitle.LoadString(IDS_BECKY);
			} else if (!_tcsnicmp(pTask[i].ProcessName, CString(MAKEINTRESOURCE(IDS_EXPLORER)), sizeof(pTask[i].ProcessName))) {
				appTitle.LoadString(IDS_PROGRAM_MANAGER);
			} else if (!_tcsnicmp(pTask[i].ProcessName, CString(MAKEINTRESOURCE(IDS_MSIMN)), sizeof(pTask[i].ProcessName))) {
				appTitle.LoadString(IDS_OUTLOOK_EXPRESS);
			} else if (!_tcsnicmp(pTask[i].ProcessName, CString(MAKEINTRESOURCE(IDS_PROJECT)), sizeof(pTask[i].ProcessName))
					|| !_tcsnicmp(pTask[i].ProcessName, CString(MAKEINTRESOURCE(IDS_EXCEL)), sizeof(pTask[i].ProcessName))
					|| !_tcsnicmp(pTask[i].ProcessName, _T("psp.exe"), sizeof(pTask[i].ProcessName))) {
				GetAppTitle(appTitle, szWindowName, 1);
			} else if (!_tcsnicmp(pTask[i].ProcessName, _T("sakura.exe"), sizeof(pTask[i].ProcessName))) {
				GetAppTitle(appTitle, szWindowName, 2); // '.' is included, so...
			} else if (!_tcsnicmp(pTask[i].ProcessName, CString(MAKEINTRESOURCE(IDS_MSDN)), sizeof(pTask[i].ProcessName))) {
				appTitle = szWindowName;
			} else if (!_tcsnicmp(pTask[i].ProcessName, _T("devenv.exe"), sizeof(pTask[i].ProcessName))) {
				appTitle.Format(_T("Microsoft Visual Studio .NET"));
			} else if (!_tcsnicmp(pTask[i].ProcessName, _T("vb6.exe"), sizeof(pTask[i].ProcessName))) {
				appTitle.Format(_T("Microsoft Visual Basic"));
			} else if (!_tcsnicmp(pTask[i].ProcessName, _T("ssexp.exe"), sizeof(pTask[i].ProcessName))) {
				appTitle.LoadString(IDS_VISUAL_SOURCESAFE_EXPLORER);
			} else if (!_tcsnicmp(pTask[i].ProcessName, _T("sh.exe"), sizeof(pTask[i].ProcessName))) {
				appTitle.Format(_T("MKS Korn Shell"));
			} else if (!_tcsnicmp(pTask[i].ProcessName, _T("csh.exe"), sizeof(pTask[i].ProcessName))) {
				appTitle.Format(_T("C Shell"));
			} else if (!_tcsnicmp(pTask[i].ProcessName, _T("vim.exe"), sizeof(pTask[i].ProcessName))) {
				appTitle.Format(_T("VIM"));
			} else {
				CUtils::CorrectAppName(pTask[i].ProcessName, szWindowName);
				GetAppTitle(appTitle, szWindowName);
			}
			break;
		}
	}
	
	if (IsWindowVisible(hWnd) && // Is visible?
			GetWindow(hWnd, GW_OWNER) == NULL && // Is top level window?
			lstrlen(szWindowName) > 0) { // Have caption?
		pProperties->AddItem(appTitle, pTask[i].ProcessName);
	}
	return TRUE;
}

void CProfile::LoadRegistry()
{
	bool bDialog = false;
	const CString section(MAKEINTRESOURCE(IDS_REG_SECTION_APPLICATION));	
	for (int nAppID = 0; nAppID < MAX_APP; ++nAppID) {
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
		_tcsncpy_s(m_Config.szSpecialApp[nAppID], appName, _TRUNCATE);
		entry.LoadString(IDS_REG_ENTRY_APPLICATOIN_TITLE);
		_tcsncpy_s(m_szAppTitle[nAppID], AfxGetApp()->GetProfileString(appName, entry), _TRUNCATE);
		entry.LoadString(IDS_REG_ENTRY_WINDOW_TEXT);
		_tcsncpy_s(m_Config.szWindowText[nAppID], AfxGetApp()->GetProfileString(appName, entry, _T("*")), _TRUNCATE);

		const CString regApp = CString(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA)) + _T("\\") + appName;
		for (BYTE nComID = 1; nComID < MAX_COMMAND; ++nComID) {
			entry = CCommands::GetCommandName(nComID);
			HKEY hKey;
			const CString regKey = regApp + _T("\\") + entry;
			if (RegOpenKeyEx(HKEY_CURRENT_USER, regKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
				TCHAR szKeyBind[128];
				DWORD dwKeyBind = _countof(szKeyBind);
				for (DWORD dwIndex = 0; RegEnumKeyEx(hKey, dwIndex, szKeyBind, &dwKeyBind, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; ++dwIndex) {
					int nType, nKey;
					StringToKey(szKeyBind, nType, nKey);
					m_Config.nCommandID[nAppID][nType][nKey] = nComID;
					dwKeyBind = _countof(szKeyBind);
				}
				RegCloseKey(hKey);
			} else {
				// Set the default assignment
				for (int i = 0; const int nKey = CCommands::GetDefaultCommandKey(nComID, i); ++i) {
					if (CCommands::GetDefaultControlID(nComID, i) == IDC_CO2)
						continue;
					const int nType = CCommands::GetDefaultCommandType(nComID, i);
					m_Config.nCommandID[nAppID][nType][nKey] = nComID;
				}
			}
		}
		for (int nFuncID = 0; nFuncID < CDotXkeymacs::GetFunctionNumber(); ++nFuncID) {
			HKEY hKey;
			const CString regKey = regApp + _T("\\") + CDotXkeymacs::GetFunctionSymbol(nFuncID);
			if (RegOpenKeyEx(HKEY_CURRENT_USER, regKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
				CDotXkeymacs::ClearKey(nFuncID, nAppID);
				TCHAR szKeyBind[128];
				DWORD dwKeyBind = _countof(szKeyBind);
				for (DWORD dwIndex = 0; RegEnumKeyEx(hKey, dwIndex, szKeyBind, &dwKeyBind, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; ++dwIndex) {
					int nType, nKey;
					StringToKey(szKeyBind, nType, nKey);
					CDotXkeymacs::SetKey(nFuncID, nAppID, nType, nKey);
					dwKeyBind = _countof(szKeyBind);
				}
				RegCloseKey(hKey);
			}
		}

		entry.LoadString(IDS_REG_ENTRY_KILL_RING_MAX);
		int n = AfxGetApp()->GetProfileInt(appName, entry, 1);
		m_Config.nKillRingMax[nAppID] = static_cast<BYTE>(n > 255 ? 255 : n);
		entry.LoadString(IDS_REG_ENTRY_USE_DIALOG_SETTING);
		m_Config.bUseDialogSetting[nAppID] = static_cast<BYTE>(AfxGetApp()->GetProfileInt(appName, entry, 1));
		entry.LoadString(IDS_REG_ENTRY_DISABLE_XKEYMACS);
		m_Config.nSettingStyle[nAppID] = static_cast<BYTE>(AfxGetApp()->GetProfileInt(appName, entry, 0) ? SETTING_DISABLE : SETTING_SPECIFIC);
		entry.LoadString(IDC_REG_ENTRY_IGNORE_META_CTRL);
		m_Config.bIgnoreUndefinedMetaCtrl[nAppID] = static_cast<BYTE>(AfxGetApp()->GetProfileInt(appName, entry, 0));
		entry.LoadString(IDC_REG_ENTRY_IGNORE_C_X);
		m_Config.bIgnoreUndefinedC_x[nAppID] = static_cast<BYTE>(AfxGetApp()->GetProfileInt(appName, entry, 0));
		entry.LoadString(IDC_REG_ENTRY_ENABLE_CUA);
		m_Config.bEnableCUA[nAppID] = static_cast<BYTE>(AfxGetApp()->GetProfileInt(appName, entry, 0));
		entry.LoadString(IDS_REG_ENTRY_326_COMPATIBLE);
		m_Config.b326Compatible[nAppID] = static_cast<BYTE>(AfxGetApp()->GetProfileInt(appName, entry, 0));
	}
}

void CProfile::SaveRegistry()
{
	const CString section(MAKEINTRESOURCE(IDS_REG_SECTION_APPLICATION));	
	for (int nAppID = 0; nAppID < MAX_APP; ++nAppID) {
		const LPCTSTR szAppName = m_Config.szSpecialApp[nAppID];
		CString entry;
		entry.Format(IDS_REG_ENTRY_APPLICATION, nAppID);
		if (!szAppName[0]) {
			if (!AfxGetApp()->GetProfileString(section, entry).IsEmpty())
				AfxGetApp()->WriteProfileString(section, entry, _T(""));
			continue;
		}
		AfxGetApp()->WriteProfileString(section, entry, szAppName);

		entry.LoadString(IDS_REG_ENTRY_APPLICATOIN_TITLE);
		CString appTitle = m_szAppTitle[nAppID];
		appTitle.TrimLeft(_T(' '));
		AfxGetApp()->WriteProfileString(szAppName, entry, appTitle);
		entry.LoadString(IDS_REG_ENTRY_WINDOW_TEXT);
		AfxGetApp()->WriteProfileString(szAppName, entry, m_Config.szWindowText[nAppID]);

		// Create all commands
		for (int nComID = 1; nComID < MAX_COMMAND; ++nComID)
			SaveKeyBind(szAppName, nComID, 0, 0);
		for (int nType = 0; nType < MAX_COMMAND_TYPE; ++nType)
			for (int nKey = 0; nKey < MAX_KEY; ++nKey)
				SaveKeyBind(szAppName, m_Config.nCommandID[nAppID][nType][nKey], nType, nKey);
		for (int nFuncID = 0; nFuncID < CDotXkeymacs::GetFunctionNumber(); ++nFuncID)
			for (int nKeyID = 0; nKeyID < CDotXkeymacs::GetKeyNumber(nFuncID, nAppID); ++nKeyID) {
				int nType, nKey;
				CDotXkeymacs::GetKey(nFuncID, nAppID, nKeyID, &nType, &nKey);
				SaveKeyBind(szAppName, CDotXkeymacs::GetFunctionSymbol(nFuncID), nType, nKey);
			}

		entry.LoadString(IDS_REG_ENTRY_KILL_RING_MAX);
		AfxGetApp()->WriteProfileInt(szAppName, entry, m_Config.nKillRingMax[nAppID]);
		entry.LoadString(IDS_REG_ENTRY_USE_DIALOG_SETTING);
		AfxGetApp()->WriteProfileInt(szAppName, entry, m_Config.bUseDialogSetting[nAppID]);
		entry.LoadString(IDS_REG_ENTRY_DISABLE_XKEYMACS);
		AfxGetApp()->WriteProfileInt(szAppName, entry, m_Config.nSettingStyle[nAppID] == SETTING_DISABLE);
		entry.LoadString(IDC_REG_ENTRY_IGNORE_META_CTRL);
		AfxGetApp()->WriteProfileInt(szAppName, entry, m_Config.bIgnoreUndefinedMetaCtrl[nAppID]);
		entry.LoadString(IDC_REG_ENTRY_IGNORE_C_X);
		AfxGetApp()->WriteProfileInt(szAppName, entry, m_Config.bIgnoreUndefinedC_x[nAppID]);
		entry.LoadString(IDC_REG_ENTRY_ENABLE_CUA);
		AfxGetApp()->WriteProfileInt(szAppName, entry, m_Config.bEnableCUA[nAppID]);
		entry.LoadString(IDS_REG_ENTRY_326_COMPATIBLE);
		AfxGetApp()->WriteProfileInt(szAppName, entry, m_Config.b326Compatible[nAppID]);
	}
}

void CProfile::LoadData()
{
	CDotXkeymacs::Load();
	LevelUp();
	LoadRegistry();
}

void CProfile::SaveData()
{
	DeleteAllRegistryData();
	SaveRegistry();
	SetDllData();
}

void CProfile::SetDllData()
{
	memset(m_Config.nFunctionID, -1, sizeof(m_Config.nFunctionID));
	for (int nFuncID = 0; nFuncID < CDotXkeymacs::GetFunctionNumber(); ++nFuncID)
		_tcscpy_s(m_Config.szFunctionDefinition[nFuncID], CDotXkeymacs::GetFunctionDefinition(nFuncID));

	for (int nAppID = 0; nAppID < MAX_APP; ++nAppID) {
		m_Config.nCommandID[nAppID][CONTROL]['X'] = 0; // C-x is unassigned.
		for (int nType = 0; nType < MAX_COMMAND_TYPE; ++nType)
			for (int nKey = 0; nKey < MAX_KEY; ++nKey)
				if ((nType & CONTROLX) && m_Config.nCommandID[nAppID][nType][nKey])
					m_Config.nCommandID[nAppID][CONTROL]['X'] = 1; // C-x is available.
		for (BYTE nFuncID = 0; nFuncID < CDotXkeymacs::GetFunctionNumber(); ++nFuncID)
			for (int nKeyID = 0; nKeyID < CDotXkeymacs::GetKeyNumber(nFuncID, nAppID); ++nKeyID) {
				int nType, nKey;
				CDotXkeymacs::GetKey(nFuncID, nAppID, nKeyID, &nType, &nKey);
				m_Config.nFunctionID[nAppID][nType][nKey] = nFuncID;
				if (nType & CONTROLX)
					m_Config.nCommandID[nAppID][CONTROL]['X'] = 1; // C-x is available.
			}
	}
	m_Config.b106Keyboard = static_cast<BYTE>(Is106Keyboard());
	CXkeymacsDll::SetConfig(m_Config);
	CXkeymacsApp *pApp = static_cast<CXkeymacsApp *>(AfxGetApp());
	if (!pApp->IsWow64())
		return;
	if (!CXkeymacsDll::SaveConfig())
		return;
	pApp->SendIPCMessage(XKEYMACS_RELOAD);
}

void CProfile::SaveKeyBind(const LPCTSTR appName, int comID, int type, int key)
{
	if (!comID)
		return;
	LPCTSTR comName = CCommands::GetCommandName(comID);
	if (!comName[0])
		return;
	SaveKeyBind(appName, comName, type, key);
}

void CProfile::SaveKeyBind(const LPCTSTR appName, const LPCTSTR comName, int type, int key)
{
	CString subKey = CString(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA)) + _T("\\") + appName + _T("\\") + comName;
	CString s = KeyToString(type, key);
	if (s.IsEmpty())
		subKey = subKey + _T("\\") + s;
	HKEY hKey = NULL;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, subKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
		RegCloseKey(hKey);
}

void CProfile::StringToKey(const LPCTSTR str, int& type, int& key)
{
	m_KeyString.ToKey(str, type, key);
}

CString CProfile::KeyToString(int type, int key)
{
	return m_KeyString.ToString(type, key);
}

void CProfile::AddKeyBind2C_(const LPCSTR szAppName, const BYTE bVk)
{
	int nComID;
	for (nComID = 0; nComID < MAX_COMMAND; ++nComID)
		if (Commands[nComID].fCommand == CCommands::C_)
			break;
	SaveKeyBind(szAppName, nComID, NONE, bVk);
}

void CProfile::LevelUp()
{
	const int nCurrentLevel = AfxGetApp()->GetProfileInt(_T(""), _T("Level"), 0);
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
			for (int nFuncID = 0; nFuncID < CDotXkeymacs::GetFunctionNumber(); ++nFuncID) {
				HKEY hKey = NULL;
				const CString subKey = CString(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA)) + _T("\\") + appName + _T("\\") + CString(MAKEINTRESOURCE(IDS_REG_ORIGINAL_PREFIX)) + CDotXkeymacs::GetFunctionSymbol(nFuncID);
				if (RegOpenKeyEx(HKEY_CURRENT_USER, subKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
					// Use registry data
					TCHAR szKeyBind[128];
					DWORD dwKeyBind = sizeof(szKeyBind);
					for (DWORD dwIndex = 0; RegEnumKeyEx(hKey, dwIndex, szKeyBind, &dwKeyBind, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; ++dwIndex) {
						int nType, nKey;
						StringToKey(szKeyBind, nType, nKey);
						SaveKeyBind(appName, CDotXkeymacs::GetFunctionSymbol(nFuncID), nType, nKey);
						dwKeyBind = sizeof(szKeyBind);
					}
					RegCloseKey(hKey);
				}
			}
		}
	}
	AfxGetApp()->WriteProfileInt(_T(""), _T("Level"), 4);
}

void CProfile::InitDllData()
{
	LoadData();
	SetDllData();
}

void CProfile::ClearData(const CString szCurrentApplication)
{
	for (int nAppID = 0; nAppID < MAX_APP; ++nAppID)
		if (szCurrentApplication == m_Config.szSpecialApp[nAppID]) {
			ZeroMemory(m_Config.nCommandID[nAppID], sizeof(m_Config.nCommandID[nAppID]));
			ZeroMemory(m_Config.szSpecialApp[nAppID], CLASS_NAME_LENGTH);
			return;
		}
}

// return count of saved settings
int CProfile::GetSavedSettingCount()
{
	int nSavedSetting = 0;
	for (int nAppID = 0; nAppID < MAX_APP; ++nAppID)
		if (m_Config.szSpecialApp[nAppID][0])
			++nSavedSetting;
	return nSavedSetting;
}

void CProfile::InitAppList(CProperties& cProperties)
{
	GetTaskList();

	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&cProperties));

	for (int i = 0; i < MAX_APP; ++i) {
		const LPCTSTR szAppName = m_Config.szSpecialApp[i];
		const LPCTSTR szAppTitle = m_szAppTitle[i];
		if (!szAppName[0] || !_tcscmp(szAppName, _T("IME")))
			continue;
		if (CString(MAKEINTRESOURCE(IDS_DEFAULT)) == szAppName ||
				CString(MAKEINTRESOURCE(IDS_DIALOG)) == szAppName)
			continue;
		cProperties.AddItem(szAppTitle, szAppName);
	}
	AddIMEInfo(cProperties);
}

void CProfile::AddIMEInfo(CProperties& cProperties)
{
	IMEList imeList;
	for (IMEListIterator p = imeList.begin(); p != imeList.end(); ++p)
		cProperties.AddItem(p->szDescription, p->szFileName);
}

void CProfile::GetTaskList()
{
	ZeroMemory(m_TaskList, sizeof(m_TaskList));

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == (HANDLE)-1)
		return;

	m_dwTasks = 0;
	PROCESSENTRY32 processEntry32 = {sizeof(PROCESSENTRY32)};
	if (Process32First(hProcessSnap, &processEntry32)) {
		do {
			lstrcpy(m_TaskList[m_dwTasks].ProcessName, processEntry32.szExeFile);
			m_TaskList[m_dwTasks++].dwProcessId = processEntry32.th32ProcessID;
		} while (m_dwTasks < MAX_TASKS && Process32Next(hProcessSnap, &processEntry32));
	}

	CloseHandle(hProcessSnap);
}

int CProfile::DefaultAppID()
{
	const CString name(MAKEINTRESOURCE(IDS_DEFAULT));
	for(int nAppID = 0; nAppID < MAX_APP; ++nAppID)
		if (name == m_Config.szSpecialApp[nAppID])
			return nAppID;
	return MAX_APP;
}

int CProfile::AssignAppID(const LPCSTR szAppName)
{
	int nAppID = GetAppID(szAppName);
	if (nAppID != MAX_APP)
		return nAppID;
	for (nAppID = 0; nAppID < MAX_APP; ++nAppID)
		if (!m_Config.szSpecialApp[nAppID][0]) {
			_tcsncpy_s(m_Config.szSpecialApp[nAppID], szAppName, _TRUNCATE);
			return nAppID;
		}
	return nAppID;
}

int CProfile::GetSettingStyle(const int nAppID)
{
	if (nAppID == MAX_APP)
		return SETTING_DEFAULT;
	return m_Config.nSettingStyle[nAppID];
}

void CProfile::SetSettingStyle(int nAppID, int nSettingStyle)
{
	if (nAppID == MAX_APP)
		return;
	m_Config.nSettingStyle[nAppID] = static_cast<BYTE>(nSettingStyle);
}

BOOL CProfile::Is106Keyboard()
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
			CString szSubKey(_T("SYSTEM\\CurrentControlSet\\Services\\i8042prt\\Parameters"));
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

void CProfile::SetAppTitle(const int nAppID, const CString& appTitle)
{
	_tcsncpy_s(m_szAppTitle[nAppID], appTitle, _TRUNCATE);
}

void CProfile::SetCommandID(const int nAppID, const int nType, const int nKey, int nComID)
{
	if (nKey == 0xf0 && Commands[nComID].fCommand == CCommands::C_)
		// Change CommandID C_Eisu
		for (nComID = 1; nComID < MAX_COMMAND; ++nComID)
			if (Commands[nComID].fCommand == CCommands::C_Eisu)
				break;
	m_Config.nCommandID[nAppID][nType][nKey] = static_cast<BYTE>(nComID);
}

int CProfile::GetCommandID(const int nAppID, const int nType, const int nKey)
{
	int nComID = m_Config.nCommandID[nAppID][nType][nKey];
	if (nKey == 0xf0 && Commands[nComID].fCommand == CCommands::C_Eisu)
		// Change CommandID C_
		for (nComID = 1; nComID < MAX_COMMAND; nComID++)
			if (Commands[nComID].fCommand == CCommands::C_)
				break;
	return nComID;
}

void CProfile::SetKillRingMax(const int nAppID, const int nKillRingMax)
{
	m_Config.nKillRingMax[nAppID] = static_cast<BYTE>(nKillRingMax > 255 ? 255 : nKillRingMax);
}

int CProfile::GetKillRingMax(const int nAppID)
{
	return m_Config.nKillRingMax[nAppID];
}

void CProfile::SetUseDialogSetting(const int nAppID, const BOOL bUseDialogSetting)
{
	m_Config.bUseDialogSetting[nAppID] = static_cast<BYTE>(bUseDialogSetting);
}

BOOL CProfile::GetUseDialogSetting(const int nAppID)
{
	return m_Config.bUseDialogSetting[nAppID];
}

void CProfile::SetWindowText(const int nAppID, const CString szWindowText)
{
	if (CUtils::GetWindowTextType(szWindowText) == IDS_WINDOW_TEXT_IGNORE)
		_tcscpy_s(m_Config.szWindowText[nAppID], _T("*"));
	else
		_tcsncpy_s(m_Config.szWindowText[nAppID], szWindowText, _TRUNCATE);
}

CString CProfile::GetWindowText(const int nAppID)
{
	return m_Config.szWindowText[nAppID];
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

void CProfile::CopyData(const CString szDstApp, const CString szSrcApp)
{
	const int nDstApp = AssignAppID(szDstApp);
	const int nSrcApp = GetAppID(szSrcApp);
	if (nDstApp == MAX_APP || nSrcApp == MAX_APP)
		return;
	SetSettingStyle(nDstApp, SETTING_SPECIFIC);

#define CopyMember(member) CopyMemory(&m_Config. ## member ## [nDstApp], &m_Config. ## member ## [nSrcApp], sizeof(m_Config. ## member ## [nSrcApp]))
	CopyMember(b326Compatible);
	CopyMember(nFunctionID);
	CopyMember(bEnableCUA);
	CopyMember(bUseDialogSetting);
	CopyMember(bIgnoreUndefinedC_x);
	CopyMember(bIgnoreUndefinedMetaCtrl);
	CopyMember(nKillRingMax);
	CopyMember(nCommandID);
#undef CopyMember
}

// return application index
// if there is NOT the application in the data, return MAX_APP
int CProfile::GetAppID(const LPCSTR szAppName)
{
	int nAppID = 0;
	for (nAppID = 0; nAppID < MAX_APP; ++nAppID)
		if (!_tcscmp(szAppName, m_Config.szSpecialApp[nAppID]))
			break;
	return nAppID;
}

// Return True if Windows Vista or later.
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

void CProfile::ExportProperties()
{
	if (!AdjustTokenPrivileges(SE_BACKUP_NAME)) {
		return;
	}

	CFileDialog oFileOpenDialog(FALSE, _T("reg"), _T("xkeymacs"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, CString(MAKEINTRESOURCE(IDS_REGISTRATION_FILTER)));
	if (oFileOpenDialog.DoModal() == IDOK) {
		CString szCommandLine;
		szCommandLine.Format(_T("regedit /e \"%s\" HKEY_CURRENT_USER\\%s"), oFileOpenDialog.GetPathName(), CString(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA)));
		CUtils::Run(szCommandLine, TRUE);	// regedit /e "x:\xkeymacs.reg" HKEY_CURRENT_USER\Software\Oishi\XKeymacs2
	}

	DiableTokenPrivileges();
	return;
}

void CProfile::ImportProperties()
{
	if (!AdjustTokenPrivileges(SE_RESTORE_NAME)) {
		return;
	}

	CFileDialog oFileOpenDialog(TRUE, _T("reg"), _T("xkeymacs"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, CString(MAKEINTRESOURCE(IDS_REGISTRATION_FILTER)));
	if (oFileOpenDialog.DoModal() == IDOK) {
		CString szCommandLine;
		szCommandLine.Format(_T("regedit \"%s\""), oFileOpenDialog.GetPathName());
		CUtils::Run(szCommandLine, TRUE);	// regedit "x:\xkeymacs.reg"
	}

	DiableTokenPrivileges();
	return;
}

BOOL CProfile::GetEnableCUA(const int nAppID)
{
	return m_Config.bEnableCUA[nAppID];
}

void CProfile::SetEnableCUA(const int nAppID, const BOOL bEnableCUA)
{
	m_Config.bEnableCUA[nAppID] = static_cast<BYTE>(bEnableCUA);
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
