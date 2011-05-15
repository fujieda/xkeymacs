// Profile.cpp: implementation of the CProfile class
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xkeymacs.h"
#include "Profile.h"
#include "MainFrm.h"
#include "DotXkeymacs.h"
#include <Imm.h>
#include <Shlwapi.h>	// Windows NT/2000: Requires Windows 2000 (or Windows NT 4.0 with Internet Explorer 4.0 or later). 
						// Windows 95/98/Me: Requires Windows 98 (or Windows 95 with Internet Explorer 4.0 or later). 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CXkeymacsData CProfile::m_XkeymacsData[MAX_APP];
TASK_LIST CProfile::m_TaskList[MAX_TASKS];
DWORD CProfile::m_dwTasks;
ScanCode_t CProfile::m_CurrentScanCodeMap[MAX_HKEY_TYPE][4][256];
ScanCode_t CProfile::m_ScanCodeMap[MAX_HKEY_TYPE][4][256];

enum { INITIAL_SIZE	= 51200 };
enum { EXTEND_SIZE	= 25600 };

void CProfile::Item2AppName(CString *const sz)
{
	if (IsTheString(*sz, IDS_DEFAULT_TITLE)) {
		sz->LoadString(IDS_DEFAULT);
	}

	if (IsTheString(*sz, IDS_DIALOG_TITLE)) {
		sz->LoadString(IDS_DIALOG);
	}

	int nStart, nEnd, nCount;

	nStart	= sz->ReverseFind(_T('(')) + 1;
	nEnd	= sz->Find(_T(')'), nStart) - 1;
	nCount	= (nEnd + 1) - nStart;
	*sz		= sz->Mid(nStart, nCount);
}

int CProfile::IsNotSameString(CComboBox *const pApplication, const CString szListItem)
{
	CString szItem, szList;
	szList = szListItem;
	Item2AppName(&szList);

	for (int i = 0; i < pApplication->GetCount(); ++i) {
		pApplication->GetLBText(i, szItem);
		Item2AppName(&szItem);
		if (!_tcsicmp(szItem, szList)) {
			return 0;
		}
	}

	return 1;
}

int CProfile::CountSeparator(const CString szMainString, const CString szSeparator)
{
	int index	= 0;
	int counter	= 0;

	while ((index = szMainString.Find(szSeparator, index)) != -1) {
		++index;
		++counter;
	}

	return counter;
}

void CProfile::GetNthString(CString *const szAppName, const CString szWindowName, const CString szSeparator, int n)
{
	int index = -1;

	while (--n) {
		index = szWindowName.Find(szSeparator, index + 1);
	}

	int nStart;
	if (index != -1) {
		nStart = index + szSeparator.GetLength();
	} else {
		nStart = 0;
	}

	int nEnd = szWindowName.Find(szSeparator, nStart);
	if (nEnd == -1) {
		nEnd = szWindowName.GetLength();
	}

	*szAppName = szWindowName.Mid(nStart, nEnd - nStart);
}

void CProfile::GetAppName(CString *const szAppName, LPCTSTR pWindowName)
{
	CString szWindowName(pWindowName);
	CString szSeparator(MAKEINTRESOURCE(IDS_SEPARATE_WINDOWTITLE));
	int nWord = CountSeparator(szWindowName, szSeparator) + 1;

	while (nWord) {
		GetNthString(szAppName, szWindowName, szSeparator, nWord);
		if (szAppName->GetAt(0) == _T('[')
		 || szAppName->Find(_T('.'), 0) != -1		// for Microsoft Project
		 || szAppName->Find(_T(']'), 0) != -1) {	// for the file name like [foo - bar]
			--nWord;
		} else {
			return;
		}
	}

	*szAppName = szWindowName;
}

BOOL CALLBACK CProfile::EnumWindowsProc(const HWND hWnd, const LPARAM lParam)
{
	CComboBox		*pApplication	= (CComboBox*)lParam;
	PTASK_LIST		pTask			= CProfile::m_TaskList;
	
	TCHAR szWindowName[WINDOW_NAME_LENGTH];
	TCHAR szClassName[CLASS_NAME_LENGTH];
	WINDOWPLACEMENT wpl;
	
	wpl.length = sizeof(WINDOWPLACEMENT);
	::GetWindowText(hWnd, szWindowName, sizeof(szWindowName));
	GetClassName(hWnd, szClassName, sizeof(szClassName));

	CString szAppName;
	// Get Process Name
	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	for (DWORD i = 0; i < CProfile::m_dwTasks; ++i) {
		if (pTask[i].dwProcessId == dwProcessId) {

			// Get Application Name
			if (szWindowName[0] == '\0') {
				continue;
			}
			if (!_tcsnicmp(pTask[i].ProcessName, CString(MAKEINTRESOURCE(IDS_B2)), sizeof(pTask[i].ProcessName))) {
				szAppName.LoadString(IDS_BECKY);
			} else if (!_tcsnicmp(pTask[i].ProcessName, CString(MAKEINTRESOURCE(IDS_EXPLORER)), sizeof(pTask[i].ProcessName))) {
				szAppName.LoadString(IDS_PROGRAM_MANAGER);
			} else if (!_tcsnicmp(pTask[i].ProcessName, CString(MAKEINTRESOURCE(IDS_MSIMN)), sizeof(pTask[i].ProcessName))) {
				szAppName.LoadString(IDS_OUTLOOK_EXPRESS);
			} else if (!_tcsnicmp(pTask[i].ProcessName, CString(MAKEINTRESOURCE(IDS_PROJECT)), sizeof(pTask[i].ProcessName))
					|| !_tcsnicmp(pTask[i].ProcessName, CString(MAKEINTRESOURCE(IDS_EXCEL)), sizeof(pTask[i].ProcessName))
					|| !_tcsnicmp(pTask[i].ProcessName, _T("psp.exe"), sizeof(pTask[i].ProcessName))) {
				GetNthString(&szAppName, szWindowName, CString(MAKEINTRESOURCE(IDS_SEPARATE_WINDOWTITLE)), 1);
			} else if (!_tcsnicmp(pTask[i].ProcessName, _T("sakura.exe"), sizeof(pTask[i].ProcessName))) {
				GetNthString(&szAppName, szWindowName, CString(MAKEINTRESOURCE(IDS_SEPARATE_WINDOWTITLE)), 2);	// '.' is included, so...
			} else if (!_tcsnicmp(pTask[i].ProcessName, CString(MAKEINTRESOURCE(IDS_MSDN)), sizeof(pTask[i].ProcessName))) {
				szAppName = szWindowName;
			} else if (!_tcsnicmp(pTask[i].ProcessName, _T("devenv.exe"), sizeof(pTask[i].ProcessName))) {
				szAppName.Format(_T("Microsoft Visual Studio .NET"));
			} else if (!_tcsnicmp(pTask[i].ProcessName, _T("vb6.exe"), sizeof(pTask[i].ProcessName))) {
				szAppName.Format(_T("Microsoft Visual Basic"));
			} else if (!_tcsnicmp(pTask[i].ProcessName, _T("ssexp.exe"), sizeof(pTask[i].ProcessName))) {
				szAppName.LoadString(IDS_VISUAL_SOURCESAFE_EXPLORER);
			} else if (!_tcsnicmp(pTask[i].ProcessName, _T("sh.exe"), sizeof(pTask[i].ProcessName))) {
				szAppName.Format(_T("MKS Korn Shell"));
			} else if (!_tcsnicmp(pTask[i].ProcessName, _T("csh.exe"), sizeof(pTask[i].ProcessName))) {
				szAppName.Format(_T("C Shell"));
			} else if (!_tcsnicmp(pTask[i].ProcessName, _T("vim.exe"), sizeof(pTask[i].ProcessName))) {
				szAppName.Format(_T("VIM"));
			} else {
				CUtils::SetCorrectApplicationName(pTask[i].ProcessName, sizeof(pTask[i].ProcessName), szWindowName, sizeof(szWindowName));
				GetAppName(&szAppName, szWindowName);
			}
			break;
		}
	}
	
	
	if ((IsWindowVisible(hWnd))									// Is visible?
	 && (GetWindow(hWnd, GW_OWNER) == NULL)						// Is top level window?
	 && (lstrlen(szWindowName) > 0)								// Have caption?
	 && (pApplication->FindString(-1, szClassName) == CB_ERR)) {// Is not same string?
		CString szListItem;
		szListItem.Format(IDS_APPLICATION_LIST_ITEM, szAppName, pTask[i].ProcessName);
		if (IsNotSameString(pApplication, szListItem)) {
			pApplication->AddString(szListItem);
		}
	}
	return TRUE;
}
	
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProfile::CProfile()
{

}

CProfile::~CProfile()
{

}

// This method initializes data in the registry, or retrieves and validates registry data.
// bSaveAndValidate specifies a flag that indicates 
// whether registry data is being initialized (FALSE) or data is being retrieved (TRUE). 
void CProfile::UpdateRegistryData(const BOOL bSaveAndValidate)
{
	CString szEntry;
	CString szApplicationName;
	CString szApplicationTitle;
	CString szWindowText;
	CString szWindowTextType;

	BOOL bUseDialogSetting = FALSE;

	for (int nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {
		// application name
		CString szSection(MAKEINTRESOURCE(IDS_REG_SECTION_APPLICATION));
		szEntry.Format(IDS_REG_ENTRY_APPLICATION, nApplicationID);
		if (bSaveAndValidate) {	// retrieve
			m_XkeymacsData[nApplicationID].ClearAll();
			szApplicationName = AfxGetApp()->GetProfileString(szSection, szEntry);
			if (szApplicationName.IsEmpty()) {
				if (nApplicationID) {
					if (!bUseDialogSetting) {
						szApplicationName.LoadString(IDS_DIALOG);
						bUseDialogSetting = TRUE;
					} else {
						continue;
					}
				} else {
					szApplicationName.LoadString(IDS_DEFAULT);
				}
			} else {
				if (szApplicationName == CString(MAKEINTRESOURCE(IDS_DIALOG))) {
					bUseDialogSetting = TRUE;
				}
			}
			m_XkeymacsData[nApplicationID].SetApplicationName(szApplicationName);
		} else {				// initialize
			szApplicationName = m_XkeymacsData[nApplicationID].GetApplicationName();
			if (szApplicationName.IsEmpty()) {
				continue;
			}
			AfxGetApp()->WriteProfileString(szSection, szEntry, szApplicationName);
		}

		// application title
		szEntry.LoadString(IDS_REG_ENTRY_APPLICATOIN_TITLE);
		if (bSaveAndValidate) {	// retrieve
			szApplicationTitle = AfxGetApp()->GetProfileString(szApplicationName, szEntry);
			m_XkeymacsData[nApplicationID].SetApplicationTitle(szApplicationTitle);
		} else {				// initialize
			szApplicationTitle = m_XkeymacsData[nApplicationID].GetApplicationTitle();
			while (!szApplicationTitle.IsEmpty() && szApplicationTitle.GetAt(0) == _T(' ')) {
				szApplicationTitle.Delete(0);
			}
			AfxGetApp()->WriteProfileString(szApplicationName, szEntry, szApplicationTitle);
		}

		// window text
		szEntry.LoadString(IDS_REG_ENTRY_WINDOW_TEXT);
		if (bSaveAndValidate) {	// retrieve
			szWindowText = AfxGetApp()->GetProfileString(szApplicationName, szEntry, _T("*"));
			if (szWindowText.IsEmpty()) {
				szWindowText = _T('*');
			}
			m_XkeymacsData[nApplicationID].SetWindowText(szWindowText);
		} else {				// initialize
			szWindowText = m_XkeymacsData[nApplicationID].GetWindowText();
			AfxGetApp()->WriteProfileString(szApplicationName, szEntry, szWindowText);
		}

		// window text type
		szEntry.LoadString(IDS_REG_ENTRY_WINDOW_TEXT_TYPE);
		if (bSaveAndValidate) {	// retrieve
			szWindowTextType = AfxGetApp()->GetProfileString(szApplicationName, szEntry);

			int nWindowTextType = IDS_WINDOW_TEXT_IGNORE;
			if (szWindowTextType == CString(MAKEINTRESOURCE(IDS_WINDOW_TEXT_MATCH))) {
				nWindowTextType = IDS_WINDOW_TEXT_MATCH;
			} else if (szWindowTextType == CString(MAKEINTRESOURCE(IDS_WINDOW_TEXT_MATCH_FORWARD))) {
				nWindowTextType = IDS_WINDOW_TEXT_MATCH_FORWARD;
			} else if (szWindowTextType == CString(MAKEINTRESOURCE(IDS_WINDOW_TEXT_MATCH_BACKWARD))) {
				nWindowTextType = IDS_WINDOW_TEXT_MATCH_BACKWARD;
			} else if (szWindowTextType == CString(MAKEINTRESOURCE(IDS_WINDOW_TEXT_MATCH_FULL))) {
				nWindowTextType = IDS_WINDOW_TEXT_MATCH_FULL;
			}

			m_XkeymacsData[nApplicationID].SetWindowTextType(nWindowTextType);
		} else {				// initialize
			szWindowTextType.LoadString(m_XkeymacsData[nApplicationID].GetWindowTextType());
			AfxGetApp()->WriteProfileString(szApplicationName, szEntry, szWindowTextType);
		}

		// on/off
		if (bSaveAndValidate) {	// retrieve
			for (int nCommandID = 1; nCommandID < sizeof(Commands) / sizeof(Commands[0]); ++nCommandID) {
				szEntry = CXkeymacsData::GetCommandName(nCommandID);
				if (szEntry.IsEmpty()) {
					break;
				}

				HKEY hKey = NULL;
				CString szSubKey(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA));
				szSubKey += _T("\\") + szApplicationName + _T("\\") + szEntry;
				if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
					// Use registry data
					TCHAR szKeyBind[128] = {'\0'};
					DWORD dwKeyBind = sizeof(szKeyBind);
					FILETIME ft = {'\0'};	// not use
					for (DWORD dwIndex = 0;
						 RegEnumKeyEx(hKey, dwIndex, szKeyBind, &dwKeyBind, NULL, NULL, NULL, &ft) == ERROR_SUCCESS;
						 ++dwIndex) {
						int nCommandType = 0;
						int nKey = 0;
						ReadKeyBind(&nCommandType, &nKey, szKeyBind);
						m_XkeymacsData[nApplicationID].SetCommandID(nCommandType, nKey, nCommandID);

//						if (nCommandType == CONTROL && nKey == 'D') {
//							CUtils::Log("GetProfileInt(at ibeam cursor only): %s, %s", szSubKey, szKeyBind);
//						}
						const CString szSection = szSubKey.Right(szSubKey.GetLength() - CString(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA)).GetLength() - _tcslen(_T("\\"))) + _T("\\") + szKeyBind;
						const BOOL bAtIbeamCursorOnly = AfxGetApp()->GetProfileInt(szSection, CString(MAKEINTRESOURCE(IDS_REG_ENTRY_AT_IBEAM_CURSOR_ONLY)), FALSE);
						m_XkeymacsData[nApplicationID].SetAtIbeamCursorOnly(nCommandType, nKey, bAtIbeamCursorOnly);

						memset(szKeyBind, 0, sizeof(szKeyBind));
						dwKeyBind = sizeof(szKeyBind);
					}
					RegCloseKey(hKey);
				} else {
					// Use default setting
					for (int i = 0; ; ++i) {
						if (CXkeymacsData::GetDefaultControlID(nCommandID, i) == IDC_CO2) {
							continue;
						}

						int nCommandType = CXkeymacsData::GetDefaultCommandType(nCommandID, i);
						int nKey = CXkeymacsData::GetDefaultCommandKey(nCommandID, i);
						if (nKey == 0) {
							break;
						}
						m_XkeymacsData[nApplicationID].SetCommandID(nCommandType, nKey, nCommandID);
						m_XkeymacsData[nApplicationID].SetAtIbeamCursorOnly(nCommandType, nKey, FALSE);
					}
				}
			}
			for (int nFunctionID = 0; nFunctionID < CDotXkeymacs::GetFunctionNumber(); ++nFunctionID) {
				HKEY hKey = NULL;
				CString szSubKey(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA));
				szSubKey += _T("\\") + szApplicationName + _T("\\") + CDotXkeymacs::GetFunctionSymbol(nFunctionID);
				if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
					// Use registry data
					CDotXkeymacs::ClearKey(nFunctionID, nApplicationID);
					TCHAR szKeyBind[128] = {'\0'};
					DWORD dwKeyBind = sizeof(szKeyBind);
					FILETIME ft = {'\0'};	// not use
					for (DWORD dwIndex = 0; RegEnumKeyEx(hKey, dwIndex, szKeyBind, &dwKeyBind, NULL, NULL, NULL, &ft) == ERROR_SUCCESS; ++dwIndex) {
						int nCommandType = 0;
						int nKey = 0;
						ReadKeyBind(&nCommandType, &nKey, szKeyBind);
						CDotXkeymacs::SetKey(nFunctionID, nApplicationID, nCommandType, nKey);

						memset(szKeyBind, 0, sizeof(szKeyBind));
						dwKeyBind = sizeof(szKeyBind);
					}
					RegCloseKey(hKey);
				}
			}
		} else {				// initialize
			// create all commands
			for (int nCommandID = 1; nCommandID < sizeof(Commands) / sizeof(Commands[0]); ++nCommandID) {
				szEntry = CXkeymacsData::GetCommandName(nCommandID);
				if (szEntry.IsEmpty()) {
					break;
				}

				SaveCommand(szApplicationName, nCommandID);
			}
			for (int nCommandType = 0; nCommandType < MAX_COMMAND_TYPE; ++nCommandType) {
				for (int nKey = 0; nKey < MAX_KEY; ++nKey) {
					int nCommandID = m_XkeymacsData[nApplicationID].GetCommandID(nCommandType, nKey);
					SaveKeyBind(szApplicationName, nCommandID, nCommandType, nKey);
				}
			}
			for (int nFunctionID = 0; nFunctionID < CDotXkeymacs::GetFunctionNumber(); ++nFunctionID) {
				for (int nKeyID = 0; nKeyID < CDotXkeymacs::GetKeyNumber(nFunctionID, nApplicationID); ++nKeyID) {
					int nCommandType = 0;
					int nKey = 0;
					CDotXkeymacs::GetKey(nFunctionID, nApplicationID, nKeyID, &nCommandType, &nKey);
					SaveKeyBind(szApplicationName, CDotXkeymacs::GetFunctionSymbol(nFunctionID), nCommandType, nKey);
				}
			}
		}

		// kill-ring-max
		szEntry.LoadString(IDS_REG_ENTRY_KILL_RING_MAX);
		if (bSaveAndValidate) {	// retrieve
			int nKillRingMax = AfxGetApp()->GetProfileInt(szApplicationName, szEntry, 1);
			m_XkeymacsData[nApplicationID].SetKillRingMax(nKillRingMax);
		} else {				// initialize
			int nKillRingMax = m_XkeymacsData[nApplicationID].GetKillRingMax();
			AfxGetApp()->WriteProfileInt(szApplicationName, szEntry, nKillRingMax);
		}

		// Use Dialog Setting
		szEntry.LoadString(IDS_REG_ENTRY_USE_DIALOG_SETTING);
		if (bSaveAndValidate) {	// retrieve
			BOOL bUseDialogSetting = AfxGetApp()->GetProfileInt(szApplicationName,szEntry, 1);
			m_XkeymacsData[nApplicationID].SetUseDialogSetting(bUseDialogSetting);
		} else {				// initialize
			BOOL bUseDialogSetting = m_XkeymacsData[nApplicationID].GetUseDialogSetting();
			AfxGetApp()->WriteProfileInt(szApplicationName, szEntry, bUseDialogSetting);
		}

		// Setting Style
		szEntry.LoadString(IDS_REG_ENTRY_DISABLE_XKEYMACS);
		if (bSaveAndValidate) {	// retrieve
			int nSettingStyle = SETTING_SPECIFIC;
			if (AfxGetApp()->GetProfileInt(szApplicationName, szEntry, 0) != 0) {
				nSettingStyle = SETTING_DISABLE;
			}
			m_XkeymacsData[nApplicationID].SetSettingStyle(nSettingStyle);
		} else {				// initialize
			BOOL bDisableXkeymacs = FALSE;
			if (m_XkeymacsData[nApplicationID].GetSettingStyle() == SETTING_DISABLE) {
				bDisableXkeymacs = TRUE;
			}
			AfxGetApp()->WriteProfileInt(szApplicationName, szEntry, bDisableXkeymacs);
		}

		// Ignore Meta Ctrl+? when it is undefined.
		szEntry.LoadString(IDC_REG_ENTRY_IGNORE_META_CTRL);
		if (bSaveAndValidate) {	// retrieve
			m_XkeymacsData[nApplicationID].SetIgnoreUndefinedMetaCtrl(AfxGetApp()->GetProfileInt(szApplicationName, szEntry, 0));
		} else {				// initialize
			AfxGetApp()->WriteProfileInt(szApplicationName, szEntry, m_XkeymacsData[nApplicationID].GetIgnoreUndefinedMetaCtrl());
		}

		// Ignore C-x ? when it is undefined.
		szEntry.LoadString(IDC_REG_ENTRY_IGNORE_C_X);
		if (bSaveAndValidate) {	// retrieve
			m_XkeymacsData[nApplicationID].SetIgnoreUndefinedC_x(AfxGetApp()->GetProfileInt(szApplicationName, szEntry, 0));
		} else {				// initialize
			AfxGetApp()->WriteProfileInt(szApplicationName, szEntry, m_XkeymacsData[nApplicationID].GetIgnoreUndefinedC_x());
		}

		// Enable CUA-mode
		szEntry.LoadString(IDC_REG_ENTRY_ENABLE_CUA);
		if (bSaveAndValidate) {	// retrieve
			m_XkeymacsData[nApplicationID].SetEnableCUA(AfxGetApp()->GetProfileInt(szApplicationName, szEntry, 0));
		} else {				// initialize
			AfxGetApp()->WriteProfileInt(szApplicationName, szEntry, m_XkeymacsData[nApplicationID].GetEnableCUA());
		}

		// Version 3.26 compatible mode
		szEntry.LoadString(IDS_REG_ENTRY_326_COMPATIBLE);
		if (bSaveAndValidate) {	// retrieve
			m_XkeymacsData[nApplicationID].Set326Compatible(AfxGetApp()->GetProfileInt(szApplicationName, szEntry, 0));
		} else {				// initialize
			AfxGetApp()->WriteProfileInt(szApplicationName, szEntry, m_XkeymacsData[nApplicationID].Get326Compatible());
		}
	}
}

void CProfile::LoadRegistryData()
{
	CDotXkeymacs::Load();
	LevelUp();
	UpdateRegistryData(TRUE);
}

void CProfile::SaveRegistryData()
{
	DeleteAllRegistryData();
	UpdateRegistryData(FALSE);
	SetDllData();
}

void CProfile::SetDllData()
{
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();

	pMainFrame->m_pXkeymacsDll->ClearFunctionDefinition();
	for (int nFunctionID = 0; nFunctionID < CDotXkeymacs::GetFunctionNumber(); ++nFunctionID) {
		pMainFrame->m_pXkeymacsDll->SetFunctionDefinition(nFunctionID, CDotXkeymacs::GetFunctionDefinition(nFunctionID));
	}

	for (int nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {

		CString szApplicationName = m_XkeymacsData[nApplicationID].GetApplicationName();

		if (szApplicationName.IsEmpty()) {
			pMainFrame->m_pXkeymacsDll->Clear(nApplicationID);
			continue;
		}

		pMainFrame->m_pXkeymacsDll->SetApplicationName(nApplicationID, szApplicationName);
		pMainFrame->m_pXkeymacsDll->SetWindowText(nApplicationID, m_XkeymacsData[nApplicationID].GetWindowText());
		pMainFrame->m_pXkeymacsDll->SetCommandID(nApplicationID, CONTROL, 'X', 0);
		pMainFrame->m_pXkeymacsDll->SetAtIbeamCursorOnly(nApplicationID, CONTROL, 'X', FALSE);

		for (int nCommandType = 0; nCommandType < MAX_COMMAND_TYPE; ++nCommandType) {
			for (int nKey = 0; nKey < MAX_KEY; ++nKey) {
				const int nCommandID = m_XkeymacsData[nApplicationID].GetCommandID(nCommandType, nKey);
				pMainFrame->m_pXkeymacsDll->SetCommandID(nApplicationID, nCommandType, nKey, nCommandID);
				const BOOL bAtIbeamCursorOnly = m_XkeymacsData[nApplicationID].GetAtIbeamCursorOnly(nCommandType, nKey);
				pMainFrame->m_pXkeymacsDll->SetAtIbeamCursorOnly(nApplicationID, nCommandType, nKey, bAtIbeamCursorOnly);
				if ((nCommandType & CONTROLX) && nCommandID) {
					pMainFrame->m_pXkeymacsDll->SetCommandID(nApplicationID, CONTROL, 'X', 1);			// Commands[1] is C-x
					pMainFrame->m_pXkeymacsDll->SetAtIbeamCursorOnly(nApplicationID, CONTROL, 'X', bAtIbeamCursorOnly);
				}
			}
		}

		for (int nFunctionID = 0; nFunctionID < CDotXkeymacs::GetFunctionNumber(); ++nFunctionID) {
			for (int nKeyID = 0; nKeyID < CDotXkeymacs::GetKeyNumber(nFunctionID, nApplicationID); ++nKeyID) {
				int nCommandType = 0;
				int nKey = 0;
				CDotXkeymacs::GetKey(nFunctionID, nApplicationID, nKeyID, &nCommandType, &nKey);
				pMainFrame->m_pXkeymacsDll->SetFunctionKey(nFunctionID, nApplicationID, nCommandType, nKey);
				if (nCommandType & CONTROLX) {
					pMainFrame->m_pXkeymacsDll->SetCommandID(nApplicationID, CONTROL, 'X', 1);			// Commands[1] is C-x
					const BOOL bAtIbeamCursorOnly = m_XkeymacsData[nApplicationID].GetAtIbeamCursorOnly(nCommandType, nKey);
					pMainFrame->m_pXkeymacsDll->SetAtIbeamCursorOnly(nApplicationID, CONTROL, 'X', bAtIbeamCursorOnly);
				}
			}
		}

		pMainFrame->m_pXkeymacsDll->SetKillRingMax(nApplicationID, m_XkeymacsData[nApplicationID].GetKillRingMax());
		pMainFrame->m_pXkeymacsDll->SetUseDialogSetting(nApplicationID, m_XkeymacsData[nApplicationID].GetUseDialogSetting());
		pMainFrame->m_pXkeymacsDll->SetSettingStyle(nApplicationID, m_XkeymacsData[nApplicationID].GetSettingStyle());
		pMainFrame->m_pXkeymacsDll->SetIgnoreUndefinedMetaCtrl(nApplicationID, m_XkeymacsData[nApplicationID].GetIgnoreUndefinedMetaCtrl());
		pMainFrame->m_pXkeymacsDll->SetIgnoreUndefinedC_x(nApplicationID, m_XkeymacsData[nApplicationID].GetIgnoreUndefinedC_x());
		pMainFrame->m_pXkeymacsDll->SetEnableCUA(nApplicationID, m_XkeymacsData[nApplicationID].GetEnableCUA());
		pMainFrame->m_pXkeymacsDll->Set326Compatible(nApplicationID, m_XkeymacsData[nApplicationID].Get326Compatible());
	}
}

void CProfile::ReadKeyBind(int *const pnCommandType, int *const pnKey, LPCTSTR szKeyBind)
{
	*pnCommandType = KeyBind2CommandType(szKeyBind);
	*pnKey = KeyBind2Key(szKeyBind + _tcslen(CommandType2String(*pnCommandType)));
}

CString CProfile::WriteKeyBind(const int nCommandType, const int nKey)
{
	CString szKeyBind;
	szKeyBind.Format(_T("%s%s"), CommandType2String(nCommandType), Key2String(nKey));
	return szKeyBind;
}

int CProfile::KeyBind2CommandType(LPCTSTR szKeyBind)
{
	for (int nCommandType = MAX_COMMAND_TYPE - 1; nCommandType; --nCommandType) {
		if (IsCommandType(nCommandType, szKeyBind)) {
			return nCommandType;
		}
	}
	return NONE;
}

int CProfile::KeyBind2Key(LPCTSTR szKey)
{
	for (int nKey = 1; nKey < 0xff; ++nKey) {
		if (!_tcscmp(szKey, Key2String(nKey))) {
			return nKey;
		}
	}
	return 0;
}

LPCTSTR CProfile::CommandType2String(int nCommandType)
{
	if (nCommandType < 0 || sizeof(CommandTypes) / sizeof(CommandTypes[0]) <= nCommandType) {
		ASSERT(0);
		nCommandType = NONE;
	}
	return CommandTypes[nCommandType].szCommandTypeName;
}

LPCTSTR CProfile::Key2String(int nKey)
{
	if (CProfile::Is106Keyboard()) {
		switch (nKey) {
		case 0xBA:
			return _T(":");
		case 0xBB:
			return _T(";");
		case 0xC0:
			return _T("@");
		case 0xDE:
			return _T("^");
		default:
			break;
		}
	}

	if (nKey < 0 || sizeof(KeyNames) / sizeof(KeyNames[0]) <= nKey) {
		ASSERT(0);
		nKey = 0;
	}
	return KeyNames[nKey].name;
}

BOOL CProfile::IsCommandType(const int nCommandType, LPCTSTR szKeyBind)
{
	LPCTSTR szCommandType = CommandType2String(nCommandType);

	if (!_tcsnicmp(szKeyBind, szCommandType, _tcslen(szCommandType))) {
		return TRUE;
	}

	return FALSE;
}

void CProfile::SaveKeyBind(const CString szApplicationName, const int nCommandID, const int nCommandType, const int nKey)
{
	if (!nCommandID) {
		return;
	}

	CString szCommandName = CXkeymacsData::GetCommandName(nCommandID);
	if (szCommandName.IsEmpty()) {
		return;
	}

	SaveKeyBind(szApplicationName, szCommandName, nCommandType, nKey);
}

void CProfile::SaveKeyBind(const CString szApplicationName, const CString szCommandName, const int nCommandType, const int nKey)
{
	CString szKeyBind = WriteKeyBind(nCommandType, nKey);
	CString szSubKey(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA));
	szSubKey += _T("\\") + szApplicationName + _T("\\") + szCommandName;
	if (!szKeyBind.IsEmpty()) {
		szSubKey += _T("\\") + szKeyBind;
	}

	HKEY hKey = NULL;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS) {
		RegCloseKey(hKey);
	}
}

void CProfile::SaveCommand(const CString szApplicationName, const int nCommandID)
{
	SaveKeyBind(szApplicationName, nCommandID, 0, 0);
}

void CProfile::AddKeyBind2C_(const CString szApplicationName, const BYTE bVk)
{
	for (int nCommandID = 0; nCommandID < sizeof(Commands) / sizeof(Commands[0]); ++nCommandID) {
		if (Commands[nCommandID].fCommand == CCommands::C_) {
			break;
		}
	}

	SaveKeyBind(szApplicationName, nCommandID, NONE, bVk);
}

void CProfile::LevelUp()
{
	const int nDefalutLevel = 0;
	const int nLatestLevel = 4;

	CString szSection;
	CString szEntry;
	szEntry.Format(_T("Level"));

	switch (AfxGetApp()->GetProfileInt(szSection, szEntry, nDefalutLevel)) {
	case nDefalutLevel:
		{
			for (int nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {
				CString szEntry;
				szEntry.Format(IDS_REG_ENTRY_APPLICATION, nApplicationID);

				CString szApplicationName;
				szApplicationName = AfxGetApp()->GetProfileString(CString(MAKEINTRESOURCE(IDS_REG_SECTION_APPLICATION)), szEntry);
				if (szApplicationName.IsEmpty()) {
					continue;
				}

				AddKeyBind2C_(szApplicationName, VK_LCONTROL);
				AddKeyBind2C_(szApplicationName, VK_RCONTROL);
			}
		}
		// Do NOT write break; here.
	case 1:
		{
			for (int nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {
				CString szEntry;
				szEntry.Format(IDS_REG_ENTRY_APPLICATION, nApplicationID);

				CString szApplicationName;
				szApplicationName = AfxGetApp()->GetProfileString(CString(MAKEINTRESOURCE(IDS_REG_SECTION_APPLICATION)), szEntry);
				if (szApplicationName.IsEmpty()) {
					continue;
				}

				// Set kill-ring-max 1 if it is 0.
				if (!AfxGetApp()->GetProfileInt(szApplicationName, CString(MAKEINTRESOURCE(IDS_REG_ENTRY_KILL_RING_MAX)), 0)) {
					AfxGetApp()->WriteProfileInt(szApplicationName, CString(MAKEINTRESOURCE(IDS_REG_ENTRY_KILL_RING_MAX)), 1);
				}
			}
		}
		// Do NOT write break; here.
	case 2:
		{
			for (int nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {
				CString szEntry;
				szEntry.Format(IDS_REG_ENTRY_APPLICATION, nApplicationID);

				CString szApplicationName;
				szApplicationName = AfxGetApp()->GetProfileString(CString(MAKEINTRESOURCE(IDS_REG_SECTION_APPLICATION)), szEntry);
				if (szApplicationName.IsEmpty()) {
					continue;
				}

				// Chaged a label from Enter to newline.
				CString szSrcSubKey(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA));
				szSrcSubKey += _T("\\") + szApplicationName + _T("\\") + _T("Enter");
				CString szDestSubKey(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA));
				szDestSubKey += _T("\\") + szApplicationName + _T("\\") + _T("newline");
				HKEY hKeyDest = NULL;
				if (RegCreateKeyEx(HKEY_CURRENT_USER, szDestSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyDest, NULL) == ERROR_SUCCESS) {
					SHCopyKey(HKEY_CURRENT_USER, szSrcSubKey, hKeyDest, NULL);
					SHDeleteKey(HKEY_CURRENT_USER, szSrcSubKey);
					RegCloseKey(hKeyDest);
				}
			}
		}
		// Do NOT write break; here.
	case 3:
		{
			for (int nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {
				CString szEntry;
				szEntry.Format(IDS_REG_ENTRY_APPLICATION, nApplicationID);

				CString szApplicationName;
				szApplicationName = AfxGetApp()->GetProfileString(CString(MAKEINTRESOURCE(IDS_REG_SECTION_APPLICATION)), szEntry);
				if (szApplicationName.IsEmpty()) {
					continue;
				}

				// rename original function to remove IDS_REG_ORIGINAL_PREFIX
				for (int nFunctionID = 0; nFunctionID < CDotXkeymacs::GetFunctionNumber(); ++nFunctionID) {
					HKEY hKey = NULL;
					CString szSubKey(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA));
					szSubKey += _T("\\") + szApplicationName + _T("\\") + CString(MAKEINTRESOURCE(IDS_REG_ORIGINAL_PREFIX)) + CDotXkeymacs::GetFunctionSymbol(nFunctionID);
					if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
						// Use registry data
						TCHAR szKeyBind[128] = {'\0'};
						DWORD dwKeyBind = sizeof(szKeyBind);
						FILETIME ft = {'\0'};	// not use
						for (DWORD dwIndex = 0; RegEnumKeyEx(hKey, dwIndex, szKeyBind, &dwKeyBind, NULL, NULL, NULL, &ft) == ERROR_SUCCESS; ++dwIndex) {
							int nCommandType = 0;
							int nKey = 0;
							ReadKeyBind(&nCommandType, &nKey, szKeyBind);
							SaveKeyBind(szApplicationName, CDotXkeymacs::GetFunctionSymbol(nFunctionID), nCommandType, nKey);

							memset(szKeyBind, 0, sizeof(szKeyBind));
							dwKeyBind = sizeof(szKeyBind);
						}
						RegCloseKey(hKey);
					}
				}
			}
		}
//	case 4:
//		foo();
//	...
//	case nLatestLevel-1:
//		bar();
		AfxGetApp()->WriteProfileInt(szSection, szEntry, nLatestLevel);
		break;
	default:
		break;
	}
}

void CProfile::InitDllData()
{
	LoadRegistryData();
	SetDllData();
}

void CProfile::ClearData(const CString szCurrentApplication)
{
	for (int nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {
		if (m_XkeymacsData[nApplicationID].GetApplicationName() == szCurrentApplication) {
			break;
		}
	}
	if (nApplicationID < MAX_APP) {
		m_XkeymacsData[nApplicationID].ClearAll();
	}
}

// return count of saved settings
int CProfile::GetSavedSettingCount()
{
	int nSavedSetting = 0;

	for (int nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {
		CString szApplicationName;
		szApplicationName = m_XkeymacsData[nApplicationID].GetApplicationName();
		if (!szApplicationName.IsEmpty()) {
			++nSavedSetting;
		}
	}

	return nSavedSetting;
}

void CProfile::InitApplicationList(CComboBox *const cApplicationList)
{
	cApplicationList->ResetContent();

	m_dwTasks = GetTaskList(m_TaskList, MAX_TASKS);

	EnumWindows(EnumWindowsProc, (LPARAM)cApplicationList);

	for (int i = 0; i < MAX_APP; ++i) {
		CString szApplicationName	= m_XkeymacsData[i].GetApplicationName();
		CString szApplicationTitle	= m_XkeymacsData[i].GetApplicationTitle();

		CString szListItem;
		szListItem.Format(IDS_APPLICATION_LIST_ITEM, szApplicationTitle, szApplicationName);
		if (IsNotSameString(cApplicationList, szListItem)
		 && !IsDefault(szApplicationName)
		 && !IsDialog(szApplicationName)
		 && !szApplicationName.IsEmpty()) {
			cApplicationList->AddString(szListItem);
		}
	}

	// Add IME
	HKL hKL = GetKeyboardLayout(0);
	if (ImmIsIME(hKL)) {
		LPTSTR szIMEDescription = NULL;
		UINT nIMEDescription = ImmGetDescription(hKL, NULL, 0);
		if (nIMEDescription) {
			nIMEDescription += sizeof(TCHAR);	// for NULL
			if ((szIMEDescription = new TCHAR[nIMEDescription]) != NULL) {
				ImmGetDescription(hKL, szIMEDescription, nIMEDescription);
			}
//			CUtils::Log(_T("nIMEDescription = %d, szIMEDescription = _%s_"), nIMEDescription, szIMEDescription);
		}

		LPTSTR szIMEFileName = NULL;
		UINT nIMEFileName = ImmGetIMEFileName(hKL, NULL, 0);
		if (nIMEFileName) {
			nIMEFileName += sizeof(TCHAR);
			if ((szIMEFileName = new TCHAR[nIMEFileName]) != NULL) {
				ImmGetIMEFileName(hKL, szIMEFileName, nIMEFileName);
			}
//			CUtils::Log(_T("nIMEFileName = %d, szIMEFileName = _%s_"), nIMEFileName, szIMEFileName);
		}

		CString szIMETitle;
		CString szIME(MAKEINTRESOURCE(IDS_IME_FILE_NAME));
		szIMETitle.Format(IDS_APPLICATION_LIST_ITEM, szIMEDescription ? szIMEDescription : szIME, szIMEFileName ? szIMEFileName : szIME);
//		CUtils::Log(_T("szIMETitle = _%s_, szIMEDescription = _%s_, szIMEFileName = _%s_"), szIMETitle, szIMEDescription, szIMEFileName);
		if (IsNotSameString(cApplicationList, szIMETitle)) {
			cApplicationList->AddString(szIMETitle);
		}

		delete[] szIMEDescription;
		szIMEDescription = NULL;
		delete[] szIMEFileName;
		szIMEFileName = NULL;
	}

	// Add Dialog
	cApplicationList->InsertString(0, CString(MAKEINTRESOURCE(IDS_DIALOG_TITLE)));

	// Add Default
	cApplicationList->InsertString( 0, CString(MAKEINTRESOURCE(IDS_DEFAULT_TITLE)));
	cApplicationList->SelectString(-1, CString(MAKEINTRESOURCE(IDS_DEFAULT_TITLE)));
}

DWORD CProfile::GetTaskList(PTASK_LIST pTask, const DWORD dwNumTasks)
{
	for (int i = 0; i < MAX_TASKS; ++i) {
		ZeroMemory(&pTask[i], sizeof(PTASK_LIST));
	}

	OSVERSIONINFO verInfo = {0};
	verInfo.dwOSVersionInfoSize = sizeof (verInfo);
	GetVersionEx(&verInfo);
	if (verInfo.dwPlatformId == VER_PLATFORM_WIN32_NT
	 && verInfo.dwMajorVersion < 5) {
		return GetTaskListNT(pTask, dwNumTasks);
	}

	HMODULE hKernel = GetModuleHandle(_T("KERNEL32.DLL"));
	if (!hKernel) {
		return 0;
	}

	CREATESNAPSHOT pCreateToolhelp32Snapshot = (CREATESNAPSHOT)GetProcAddress(hKernel, "CreateToolhelp32Snapshot");
	if (!pCreateToolhelp32Snapshot) {
		return 0;
	}

	PROCESSWALK pProcess32First = (PROCESSWALK)GetProcAddress(hKernel, "Process32First");
	if (!pProcess32First) {
		return 0;
	}

	PROCESSWALK pProcess32Next = (PROCESSWALK)GetProcAddress(hKernel, "Process32Next");
	if (!pProcess32Next) {
		return 0;
	}

	HANDLE hProcessSnap = pCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == (HANDLE)-1) {
		return 0;
	}

	DWORD dwTaskCount = 0;
	PROCESSENTRY32 processEntry32 = {sizeof(PROCESSENTRY32)};
	if (pProcess32First(hProcessSnap, &processEntry32)) {
		do {
			LPTSTR pCurChar = NULL;
			for (pCurChar = processEntry32.szExeFile + lstrlen(processEntry32.szExeFile); *pCurChar != _T('\\') && pCurChar != processEntry32.szExeFile; --pCurChar) {
				;
			}
			if (*pCurChar == _T('\\')) {
				++pCurChar;
			}

			lstrcpy(pTask->ProcessName, pCurChar);
			pTask->dwProcessId = processEntry32.th32ProcessID;

			++dwTaskCount;
			++pTask;
		} while (dwTaskCount < dwNumTasks && pProcess32Next(hProcessSnap, &processEntry32));
	}

	CloseHandle(hProcessSnap);
	return dwTaskCount;
}

LPBYTE CProfile::GetCounters()
{
	LANGID lid = MAKELANGID(LANG_ENGLISH, SUBLANG_NEUTRAL);
	CString szSubKey;
	szSubKey.Format(CString(MAKEINTRESOURCE(IDS_REGSUBKEY_PERF)), lid);
	HKEY hKey = NULL;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szSubKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
		return NULL;
	}

	DWORD dwSize = 0;
	if (RegQueryValueEx(hKey, CString(MAKEINTRESOURCE(IDS_REGSUBKEY_COUNTERS)), NULL, NULL, NULL, &dwSize) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return NULL;
	}
	LPBYTE pCounters = (LPBYTE) calloc(dwSize, sizeof(BYTE));
	if (pCounters == NULL) {
		RegCloseKey(hKey);
		return NULL;
	}
	if (RegQueryValueEx(hKey, CString(MAKEINTRESOURCE(IDS_REGSUBKEY_COUNTERS)), NULL, NULL, pCounters, &dwSize) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		free(pCounters);
		return NULL;
	}
	RegCloseKey(hKey);
	return pCounters;
}

BOOL CProfile::GetProcessInfo(CString *const szProcessName, DWORD *const dwProcessId)
{
	LPBYTE pCounters = GetCounters();
	if (!pCounters) {
		return FALSE;
	}
	LPTSTR pTopOfString = (LPTSTR)pCounters;
	while (*pTopOfString) {
		if (_tcsicmp(pTopOfString, CString(MAKEINTRESOURCE(IDS_PROCESS))) == 0) {
			for (LPTSTR p2 = pTopOfString - 2; _istdigit(*p2); --p2) {
				;
			}
			szProcessName->Format(_T("%s"), p2 + 1);	// 230
		} else if (_tcsicmp(pTopOfString, CString(MAKEINTRESOURCE(IDS_PROCESSID))) == 0) {
			for (LPTSTR p2 = pTopOfString - 2; _istdigit(*p2); --p2) {
				;
			}
			*dwProcessId = _ttol(p2 + 1);			// 784
		}
		pTopOfString += (_tcslen(pTopOfString) + 1);
	}
	free(pCounters);
	return TRUE;
}

PPERF_DATA_BLOCK CProfile::GetPerformanceData(const CString szProcessName)
{
	DWORD dwSize = INITIAL_SIZE;
	PPERF_DATA_BLOCK pPerformanceData = (PPERF_DATA_BLOCK) calloc(dwSize, sizeof(BYTE));
	if (pPerformanceData == NULL) {
		return NULL;
	}

	for (;;) {
		switch (RegQueryValueEx(HKEY_PERFORMANCE_DATA, szProcessName, NULL, NULL, (LPBYTE)pPerformanceData, &dwSize)) {
		case ERROR_SUCCESS:
			if (0 < dwSize
			 && pPerformanceData->Signature[0] == (WCHAR)'P'
			 && pPerformanceData->Signature[1] == (WCHAR)'E'
			 && pPerformanceData->Signature[2] == (WCHAR)'R'
			 && pPerformanceData->Signature[3] == (WCHAR)'F') {
				return pPerformanceData;
			}
		case ERROR_MORE_DATA:
			dwSize += EXTEND_SIZE;
			pPerformanceData = (PPERF_DATA_BLOCK) realloc(pPerformanceData, dwSize);
			if (!pPerformanceData) {
				return NULL;
			}
			memset(pPerformanceData, 0, dwSize);
			break;
		default:
			free(pPerformanceData);
			return NULL;
		}
	}
}

// Add running application's names to the list
// only for _Windows NT_
DWORD CProfile::GetTaskListNT(PTASK_LIST pTask, DWORD dwNumTasks)
{
	CString szProcessName;
	DWORD dwProcessIdTitle = 0;
	if (!GetProcessInfo(&szProcessName, &dwProcessIdTitle)) {
		return dwNumTasks;
	}

	PPERF_DATA_BLOCK pPerformanceData = GetPerformanceData(szProcessName);
	if (!pPerformanceData) {
		return dwNumTasks;
	}

	PPERF_OBJECT_TYPE pObj = (PPERF_OBJECT_TYPE) ((DWORD)pPerformanceData + pPerformanceData->HeaderLength);
	PPERF_COUNTER_DEFINITION pCounterDef = (PPERF_COUNTER_DEFINITION) ((DWORD)pObj + pObj->HeaderLength);
	DWORD dwProcessIdCounter = 0;
	for (DWORD i = 0; i < pObj->NumCounters; ++i) {
		if (pCounterDef->CounterNameTitleIndex == dwProcessIdTitle) {
			dwProcessIdCounter = pCounterDef->CounterOffset;
			break;
		}
		++pCounterDef;
	}

	dwNumTasks = min(dwNumTasks - 1, (DWORD)pObj->NumInstances);
	PPERF_INSTANCE_DEFINITION pInst = (PPERF_INSTANCE_DEFINITION) ((DWORD)pObj + pObj->DefinitionLength);
	for (i = 0; i < dwNumTasks; ++i) {
		LPCWSTR pProcessName = (LPCWSTR) ((DWORD)pInst + pInst->NameOffset);

		CHAR szProcessName[MAX_PATH] = {'\0'};
		if (!WideCharToMultiByte(CP_ACP, 0, pProcessName, -1, szProcessName, sizeof(szProcessName), NULL, NULL)) {
			_tcscpy(pTask->ProcessName, CString(MAKEINTRESOURCE(IDS_UNKNOWN_TASK)));
		}

		if (_tcslen(szProcessName)+4 <= sizeof(pTask->ProcessName)) {
			_tcscpy(pTask->ProcessName, szProcessName);
			_tcscat(pTask->ProcessName, CString(MAKEINTRESOURCE(IDS_EXTENSION_EXECUTABLE)));
		}

		PPERF_COUNTER_BLOCK pCounter = (PPERF_COUNTER_BLOCK) ((DWORD)pInst + pInst->ByteLength);
		pTask->dwProcessId = *((LPDWORD) ((DWORD)pCounter + dwProcessIdCounter));
		if (pTask->dwProcessId == 0) {
			pTask->dwProcessId = (DWORD) -2;
		}

		++pTask;
		pInst = (PPERF_INSTANCE_DEFINITION) ((DWORD)pCounter + pCounter->ByteLength);
	}

	free(pPerformanceData);
	return dwNumTasks;
}

// return application index
// and update setting style
// if there is NOT the application in the data, this function takes care of it.
int CProfile::GetApplicationIndex(const CString szApplicationName, const BOOL bSaveAndValidate, int *const nSettingStyle)
{
	if (!bSaveAndValidate) {	// SetDialogData
		*nSettingStyle = SETTING_UNDEFINED;
	}

	int nApplicationID = GetApplicationIndex(szApplicationName);

	if (nApplicationID == MAX_APP) {
		if (bSaveAndValidate) {	// GetDialogData
			for (nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {
				CString sz = m_XkeymacsData[nApplicationID].GetApplicationName();
				if (sz.IsEmpty()) {
					m_XkeymacsData[nApplicationID].SetApplicationName(szApplicationName);
					break;
				}
			}
			if (nApplicationID == MAX_APP) {
				return nApplicationID;
			}
		} else {				// SetDialogData
			for (nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {
				if (IsDefault(m_XkeymacsData[nApplicationID].GetApplicationName())) {
					*nSettingStyle = SETTING_DEFAULT;
					break;
				}
			}
			if (nApplicationID == MAX_APP) {
				return nApplicationID;
			}
		}
	}

	if (bSaveAndValidate) {	// GetDialogData
		m_XkeymacsData[nApplicationID].SetSettingStyle(*nSettingStyle);
	} else {				// SetDialogData
		if (*nSettingStyle == SETTING_UNDEFINED) {	// It means that *nSettingStyle != SETTING_DEFAULT.
			*nSettingStyle = m_XkeymacsData[nApplicationID].GetSettingStyle();
		}
	}

	return nApplicationID;
}

BOOL CProfile::IsTheString(const CString sz, const UINT nID)
{
	return sz == CString(MAKEINTRESOURCE(nID));
}

// if sz is "Default", return TRUE
BOOL CProfile::IsDefault(const CString sz)
{
	return IsTheString(sz, IDS_DEFAULT);
}

// if sz is "Dialog", return TRUE
BOOL CProfile::IsDialog(const CString sz)
{
	return IsTheString(sz, IDS_DIALOG);
}

void CProfile::GetApplicationTitle(CComboBox *const cApplicationList, CString &rList, const int nIndex)
{
	if (0 <= nIndex) {
		cApplicationList->GetLBText(nIndex, rList);
	} else {
		cApplicationList->GetWindowText(rList);
	}

	if (IsTheString(rList, IDS_DEFAULT_TITLE)) {
		rList.LoadString(IDS_DEFAULT);
	}

	if (IsTheString(rList, IDS_DIALOG_TITLE)) {
		rList.LoadString(IDS_DIALOG);
	}

	return;
}

void CProfile::UpdateApplicationTitle(CComboBox *const cApplicationList, const CString szCurrentApplication, const int nApplicationID, const BOOL bSaveAndValidate)
{
	static CString szApplicationTitle;
	if (bSaveAndValidate) {	// GetDialogData
		if (!CProfile::IsDefault(szCurrentApplication)) {
			m_XkeymacsData[nApplicationID].SetApplicationTitle(szApplicationTitle);
		}
		szApplicationTitle.Empty();
	} else {				// SetDialogData
		CString szListItem;
		CProfile::GetApplicationTitle(cApplicationList, szListItem);
		int nEndTitle = szListItem.ReverseFind(_T('('));
		if (nEndTitle > 0) {
			szApplicationTitle = szListItem.Left(nEndTitle);
		}
	}
}

void CProfile::SetCommandID(const int nApplicationID, const int nCommandType, const int nKey, int nCommandID)
{
	if (nKey == 0xf0 && Commands[nCommandID].fCommand == CCommands::C_) {
		// Change CommandID C_Eisu
		for (nCommandID = 1; nCommandID < sizeof(Commands) / sizeof(Commands[0]); ++nCommandID) {
			if (Commands[nCommandID].fCommand == CCommands::C_Eisu) {
				break;
			}
		}
	}
	m_XkeymacsData[nApplicationID].SetCommandID(nCommandType, nKey, nCommandID);
}

int CProfile::GetCommandID(const int nApplicationID, const int nCommandType, const int nKey)
{
	int nCommandID = m_XkeymacsData[nApplicationID].GetCommandID(nCommandType, nKey);
	if (nKey == 0xf0 && Commands[nCommandID].fCommand == CCommands::C_Eisu) {
		// Change CommandID C_
		for (nCommandID = 1; nCommandID < sizeof(Commands) / sizeof(Commands[0]); ++nCommandID) {
			if (Commands[nCommandID].fCommand == CCommands::C_) {
				break;
			}
		}
	}
	return nCommandID;
}

void CProfile::SetKillRingMax(const int nApplicationID, const int nKillRingMax)
{
	m_XkeymacsData[nApplicationID].SetKillRingMax(nKillRingMax);
}

int CProfile::GetKillRingMax(const int nApplicationID)
{
	return m_XkeymacsData[nApplicationID].GetKillRingMax();
}

void CProfile::SetUseDialogSetting(const int nApplicationID, const BOOL bUseDialogSetting)
{
	m_XkeymacsData[nApplicationID].SetUseDialogSetting(bUseDialogSetting);
}

BOOL CProfile::GetUseDialogSetting(const int nApplicationID)
{
	return m_XkeymacsData[nApplicationID].GetUseDialogSetting();
}

void CProfile::SetWindowText(const int nApplicationID, const CString szWindowText)
{
	m_XkeymacsData[nApplicationID].SetWindowText(szWindowText);
}

CString CProfile::GetWindowText(const int nApplicationID)
{
	return m_XkeymacsData[nApplicationID].GetWindowText();
}

void CProfile::DeleteAllRegistryData()
{
	HKEY hkey = NULL;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, CString(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA)), 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS) {
		// I am sure that I have to do only one time, but...
		for (int nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {
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

int CProfile::GetCurrentApplicationID(CComboBox *const cApplicationList, const CString szCurrentApplication)
{
	int nCounter = cApplicationList->GetCount();
	CString szListItem;
	int nCurSel = cApplicationList->GetCurSel();

	for (int i = 0; i < nCounter; ++i) {
		cApplicationList->SetCurSel(i);
		CProfile::GetApplicationTitle(cApplicationList, szListItem);
		if (szListItem.Find(szCurrentApplication) != -1) {
			cApplicationList->SetCurSel(nCurSel);
			return i;
		}
	}
	return -1;
}

void CProfile::CopyData(const CString szDestinationApplication, const CString szSourceApplication)
{
	int nSettingStyle = SETTING_SPECIFIC;
	int nDestinationApplication = GetApplicationIndex(szDestinationApplication, TRUE, &nSettingStyle);
	int nSourceApplication = GetApplicationIndex(szSourceApplication);

	CString szApplicationName = m_XkeymacsData[nDestinationApplication].GetApplicationName();
	CString szApplicationTitle = m_XkeymacsData[nDestinationApplication].GetApplicationTitle();
	CString szWindowText = m_XkeymacsData[nDestinationApplication].GetWindowText();
	int nWindowTextType = m_XkeymacsData[nDestinationApplication].GetWindowTextType();

	m_XkeymacsData[nDestinationApplication] = m_XkeymacsData[nSourceApplication];

	m_XkeymacsData[nDestinationApplication].SetApplicationName(szApplicationName);
	m_XkeymacsData[nDestinationApplication].SetApplicationTitle(szApplicationTitle);
	m_XkeymacsData[nDestinationApplication].SetWindowText(szWindowText);
	m_XkeymacsData[nDestinationApplication].SetWindowTextType(nWindowTextType);
}

// return application index
// if there is NOT the application in the data, return MAX_APP
int CProfile::GetApplicationIndex(const CString szApplicationName)
{
	int nApplicationID = 0;
	for (nApplicationID = 0; nApplicationID < MAX_APP; ++nApplicationID) {
		if (m_XkeymacsData[nApplicationID].GetApplicationName() == szApplicationName) {
			break;
		}
	}
	return nApplicationID;
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

void CProfile::LoadScanCodeMap(const HKEY_TYPE hkeyType)
{
	memset(m_ScanCodeMap[hkeyType], 0, sizeof(m_ScanCodeMap[hkeyType]));
	memset(m_CurrentScanCodeMap[hkeyType], 0, sizeof(m_CurrentScanCodeMap[hkeyType]));

	CString szSubKey;
	CString szValueName;
	HKEY hKey = HKEY_LOCAL_MACHINE;
	if (IsNT()) {
		switch (hkeyType) {
		case CURRENT_USER:
			hKey = HKEY_CURRENT_USER;
			szSubKey.LoadString(IDS_REGSUBKEY_KEYBOARD_LAYOUT);
			break;
		case LOCAL_MACHINE:
			szSubKey.LoadString(IDS_REGSUBKEY_KEYBOARD_LAYOUT_ANY_USER);
			break;
		default:
			return;
		}
		szValueName.LoadString(IDS_SCANCODE_MAP);
	} else if (Is9x()) {
		szSubKey.LoadString(IDS_REGSUBKEY_KEY_REMAP);
		szValueName.LoadString(IDS_0);
	} else {
		return;
	}

	HKEY hkResult = NULL;
	if (RegOpenKeyEx(hKey, szSubKey, 0, KEY_QUERY_VALUE, &hkResult) == ERROR_SUCCESS) {
		// get data size
		DWORD dwType = REG_BINARY;
		DWORD dwData = 0;
		RegQueryValueEx(hkResult, szValueName, NULL, &dwType, NULL, &dwData);

		// get data
		LPBYTE lpData = new BYTE[dwData];
		if (lpData) {
			RegQueryValueEx(hkResult, szValueName, NULL, &dwType, lpData, &dwData);
		}
		RegCloseKey(hkResult);

		if (IsNT()) {
			if (lpData && dwData) {
				DWORD offset = 0;
				offset += 8;	// skip Version Information and Flags
				DWORD *pdwMappings = (DWORD *)(lpData + offset);
				offset += 4;	// skip Number of Mappings
				DWORD *pdwNullTerminator = (DWORD *)(lpData + dwData - 4);

				if (4 * *pdwMappings + 12 != dwData) {
					// illegal data
				} else if (*pdwNullTerminator != 0) {
					// illegal data
				} else {
					while (offset < dwData - 4) {
						ScanCodeMapping_t *pScanCodeMapping = (ScanCodeMapping_t *)(lpData + offset);
						offset += 4;	// go to next data

						m_CurrentScanCodeMap[hkeyType][PrefixedScanCode2ID(pScanCodeMapping->original.nPrefixedScanCode)][pScanCodeMapping->original.nScanCode].nPrefixedScanCode = pScanCodeMapping->current.nPrefixedScanCode;
						m_CurrentScanCodeMap[hkeyType][PrefixedScanCode2ID(pScanCodeMapping->original.nPrefixedScanCode)][pScanCodeMapping->original.nScanCode].nScanCode = pScanCodeMapping->current.nScanCode;
						m_ScanCodeMap[hkeyType][PrefixedScanCode2ID(pScanCodeMapping->original.nPrefixedScanCode)][pScanCodeMapping->original.nScanCode].nPrefixedScanCode = pScanCodeMapping->current.nPrefixedScanCode;
						m_ScanCodeMap[hkeyType][PrefixedScanCode2ID(pScanCodeMapping->original.nPrefixedScanCode)][pScanCodeMapping->original.nScanCode].nScanCode = pScanCodeMapping->current.nScanCode;
					}
				}
			}
		}
		delete[] lpData;
		lpData = NULL;
	}
}

int CProfile::LostKeyWarning(const HKEY_TYPE hkeyType)
{
	if (!ChangedKeyboardLayout(hkeyType)) {
		return IDOK;
	}

	CString szLostKeysList;

	for (int nPrefixedScanCodeID = 0; nPrefixedScanCodeID < 3; ++nPrefixedScanCodeID) {
		for (int nScanCode = 0; nScanCode < 256; ++nScanCode) {
			if (m_ScanCodeMap[hkeyType][nPrefixedScanCodeID][nScanCode].nScanCode) {
				BOOL lostKey = TRUE;

				for (int i = 0; i < 3 && lostKey; ++i) {
					for (int j = 0; j < 256 && lostKey; ++j) {
						if (m_ScanCodeMap[hkeyType][i][j].nPrefixedScanCode == nPrefixedScanCodeID
						 && m_ScanCodeMap[hkeyType][i][j].nScanCode == nScanCode) {
							lostKey = FALSE;
						}
					}
				}

				if (lostKey) {
					for (int i = 0; i < sizeof(KeyboardLayouts)/sizeof(KeyboardLayouts[0]); ++i) {
						if (KeyboardLayouts[i].scancode.nScanCode == nScanCode
							&& KeyboardLayouts[i].scancode.nPrefixedScanCode == nPrefixedScanCodeID) {
							CString szLostKey;
							szLostKey.Format(IDS_ERR_LOST_KEY, CString(MAKEINTRESOURCE(GetToolTipID(KeyboardLayouts[i].nToolTipID))));
							szLostKeysList += szLostKey;
							break;
						}
					}
				}
			}
		}
	}

	if (szLostKeysList.IsEmpty()) {
		return IDOK;
	}

	return AfxMessageBox(szLostKeysList, MB_OKCANCEL | MB_ICONEXCLAMATION);
}

void CProfile::SaveScanCodeMap(const HKEY_TYPE hkeyType)
{
	CString szSubKey;
	CString szValueName;
	HKEY hKey = HKEY_LOCAL_MACHINE;
	if (IsNT()) {
		switch (hkeyType) {
		case CURRENT_USER:
			hKey = HKEY_CURRENT_USER;
			szSubKey.LoadString(IDS_REGSUBKEY_KEYBOARD_LAYOUT);
			break;
		case LOCAL_MACHINE:
			szSubKey.LoadString(IDS_REGSUBKEY_KEYBOARD_LAYOUT_ANY_USER);
			break;
		default:
			return;
		}
		szValueName.LoadString(IDS_SCANCODE_MAP);
	} else if (Is9x()) {
		szSubKey.LoadString(IDS_REGSUBKEY_KEY_REMAP);
		szValueName.LoadString(IDS_0);
	} else {
		return;
	}

	HKEY hkResult = NULL;
	if (RegCreateKeyEx(hKey, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hkResult, NULL) == ERROR_SUCCESS) {
		if (IsNT()) {
			DWORD cbData = GetScanCodeLength(hkeyType);
			if (cbData <= 16) {
				RegDeleteValue(hkResult, szValueName);
			} else {
				LPBYTE lpData = new BYTE[cbData];
				memset(lpData, 0, sizeof(BYTE) * cbData);

				{
					DWORD dwMappings = (cbData - 12) / 4;
					memmove(lpData + 8, &dwMappings, 4);
				}

				int offset = 12;
				for (int nPrefixedScanCodeID = 0; nPrefixedScanCodeID < 3; ++nPrefixedScanCodeID) {
					for (int nScanCode = 0; nScanCode < 256; ++nScanCode) {
						if (m_ScanCodeMap[hkeyType][nPrefixedScanCodeID][nScanCode].nScanCode) {
							ScanCodeMapping_t sScanCodeMapping = {'\0'};
							sScanCodeMapping.original.nPrefixedScanCode = PrefixedScanCodeID2Code(nPrefixedScanCodeID);
							sScanCodeMapping.original.nScanCode = (BYTE)nScanCode;
							sScanCodeMapping.current.nPrefixedScanCode = m_ScanCodeMap[hkeyType][nPrefixedScanCodeID][nScanCode].nPrefixedScanCode;
							sScanCodeMapping.current.nScanCode = m_ScanCodeMap[hkeyType][nPrefixedScanCodeID][nScanCode].nScanCode;
							memcpy(lpData + offset, &sScanCodeMapping, sizeof(sScanCodeMapping));
							offset += sizeof(sScanCodeMapping);
						}
					}
				}
				RegSetValueEx(hkResult, szValueName, 0, REG_BINARY, lpData, cbData);

				delete[] lpData;
				lpData = NULL;
			}
		}
		RegCloseKey(hkResult);
	}

	// Do you want to restart computer?
	if (ChangedKeyboardLayout(hkeyType)) {
		if (AfxMessageBox(CString(MAKEINTRESOURCE(IDS_RESTART_OR_NOT)), MB_YESNO | MB_ICONQUESTION) == IDYES) {
			RestartComputer();
		}
	}
}

// Return True if Windows 95, Windows 98, or Windows Me. 
BOOL CProfile::Is9x()
{
	OSVERSIONINFO info = {sizeof(OSVERSIONINFO)};
	GetVersionEx(&info);

	if (info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
		return TRUE;
	}
	return FALSE;
}

// Return True if Windows NT 4.0, Windows 2000, Windows XP, Windows 2003 or Windows Vista.
BOOL CProfile::IsNT()
{
	OSVERSIONINFO info = {sizeof(OSVERSIONINFO)};
	GetVersionEx(&info);

	if (info.dwPlatformId == VER_PLATFORM_WIN32_NT) {
		return TRUE;
	}
	return FALSE;
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

int CProfile::GetControlID(const ScanCode_t scancode, const BOOL bBase)
{
	for (int i = 0; ; ++i) {
		if (KeyboardLayouts[i].scancode.nPrefixedScanCode == scancode.nPrefixedScanCode
		 && KeyboardLayouts[i].scancode.nScanCode == scancode.nScanCode) {
			if (bBase) {
				return KeyboardLayouts[i].nBaseControlID;
			} else {
				return KeyboardLayouts[i].nCurrentControlID;
			}
		}
	}
	return 0;
}

int CProfile::GetBaseControlID(const ScanCode_t scancode)
{
	return GetControlID(scancode, TRUE);
}

int CProfile::GetCurrentControlID(const ScanCode_t scancode)
{
	return GetControlID(scancode, FALSE);
}

BOOL CProfile::GetScanCodeMap(const HKEY_TYPE hkeyType, const ScanCode_t original, ScanCode_t *const current)
{
	if (!current) {
		return FALSE;
	}
	*current = m_ScanCodeMap[hkeyType][PrefixedScanCode2ID(original.nPrefixedScanCode)][original.nScanCode];
	return 0 < current->nScanCode;
}

void CProfile::SetScanCodeMap(const HKEY_TYPE hkeyType, const ScanCodeMapping_t ScanCodeMappeing)
{
	m_ScanCodeMap[hkeyType][PrefixedScanCode2ID(ScanCodeMappeing.original.nPrefixedScanCode)][ScanCodeMappeing.original.nScanCode] = ScanCodeMappeing.current;
}

int CProfile::PrefixedScanCode2ID(const BYTE nPrefixedScanCode)
{
	int nID = 0;

	switch (nPrefixedScanCode) {
	case 0x00:
		nID = 0;
		break;
	case 0xe0:
		nID = 1;
		break;
	case 0xe1:
		nID = 2;
		break;
	default:
		// invalid scan code
		nID = 3;
		break;
	}

	return nID;
}

BYTE CProfile::PrefixedScanCodeID2Code(const int nPrefixedScanCodeID)
{
	BYTE nCode = 0;

	switch (nPrefixedScanCodeID) {
	case 0:
		nCode = 0x00;
		break;
	case 1:
		nCode = 0xe0;
		break;
	case 2:
		nCode = 0xe1;
		break;
	default:
		ASSERT(0);
		break;
	}

	return nCode;
}

DWORD CProfile::GetScanCodeLength(const HKEY_TYPE hkeyType)
{
	DWORD dwScanCodeLength = 0;
	dwScanCodeLength += 4;	// Header: Version Information
	dwScanCodeLength += 4;	// Header: Flags
	dwScanCodeLength += 4;	// Header: Number of Mappings
	for (int nPrefixedScanCodeID = 0; nPrefixedScanCodeID < 3; ++nPrefixedScanCodeID) {
		for (int nScanCode = 0; nScanCode < 256; ++nScanCode) {
			if (m_ScanCodeMap[hkeyType][nPrefixedScanCodeID][nScanCode].nScanCode) {
				dwScanCodeLength += 4;	// Individual Mappings
			}
		}
	}
	dwScanCodeLength += 4;	// Null Terminator (0x00000000)
	return dwScanCodeLength;
}

BOOL CProfile::ChangedKeyboardLayout(const HKEY_TYPE hkeyType)
{
	for (int nPrefixedScanCodeID = 0; nPrefixedScanCodeID < 3; ++nPrefixedScanCodeID) {
		for (int nScanCode = 0; nScanCode < 256; ++nScanCode) {
			if (m_ScanCodeMap[hkeyType][nPrefixedScanCodeID][nScanCode].nPrefixedScanCode	!= m_CurrentScanCodeMap[hkeyType][nPrefixedScanCodeID][nScanCode].nPrefixedScanCode
			 || m_ScanCodeMap[hkeyType][nPrefixedScanCodeID][nScanCode].nScanCode			!= m_CurrentScanCodeMap[hkeyType][nPrefixedScanCodeID][nScanCode].nScanCode) {
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CProfile::SetDraggingCursor()
{
	HCURSOR hCursor = (HCURSOR)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_DRAG_CURSOR),
										 IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	SetCursor(hCursor);
}

void CProfile::SetNormalCursor()
{
	HCURSOR hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW),
										 IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	SetCursor(hCursor);
}

void CProfile::SetNoCursor()
{
	HCURSOR hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_NO),
										 IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	SetCursor(hCursor);
}

int CProfile::GetToolTipID(int nToolTipID)
{
	if (Is106Keyboard()) {
		switch (nToolTipID) {
		case IDS_EQUAL:			// ^
			nToolTipID = IDS_CARET;
			break;
		case IDS_SQUARE_BRA:	// @
			nToolTipID = IDS_AT_MARK;
			break;
		case IDS_SQUARE_CKET:	// [
			nToolTipID = IDS_SQUARE_BRA;
			break;
		case IDS_QUOTE:			// :
			nToolTipID = IDS_COLON;
			break;
		case IDS_BACK_QUOTE:	// Hankaku/Zenkaku
			nToolTipID = IDS_HANKAKU;
			break;
		case IDS_BACKSLASH:		// ]
			nToolTipID = IDS_SQUARE_CKET;
			break;
		}
	}

	return nToolTipID;
}

KeyboardLayout_t* CProfile::GetKeyboardLayouts(const int nKey)
{
	for (int i = 0; i < sizeof(KeyboardLayouts) / sizeof(KeyboardLayouts[0]); ++i) {
		if (KeyboardLayouts[i].nBaseControlID == nKey
		 || KeyboardLayouts[i].nCurrentControlID == nKey) {
			return &KeyboardLayouts[i];
		}
	}
	return NULL;
}

BOOL CProfile::AdjustTokenPrivileges(LPCTSTR lpName)
{
	BOOL rc = TRUE;

	if (IsNT()) {
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
	}

	return rc;
}

BOOL CProfile::DiableTokenPrivileges()
{
	BOOL rc = TRUE;

	if (IsNT()) {
		HANDLE hToken;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
			if (!::AdjustTokenPrivileges(hToken, TRUE, NULL, NULL, NULL, NULL)) {
				rc = FALSE;
			}
			CloseHandle(hToken);
		} else {
			rc = FALSE;
		}
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

BOOL CProfile::GetEnableCUA(const int nApplicationID)
{
	return m_XkeymacsData[nApplicationID].GetEnableCUA();
}

void CProfile::SetEnableCUA(const int nApplicationID, const BOOL bEnableCUA)
{
	m_XkeymacsData[nApplicationID].SetEnableCUA(bEnableCUA);
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
