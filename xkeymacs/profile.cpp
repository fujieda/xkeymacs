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

struct CommandTypeName
{
	int nType;
	LPCTSTR szName;
};

static const CommandTypeName CommandTypes[] = {
	{NONE,							_T("")},
	{SHIFT,							_T("Shift+")},
	{CONTROL,						_T("Ctrl+")},
	{CONTROL+SHIFT,					_T("Ctrl+Shift+")},
	{META,							_T("Meta+")},
	{META+SHIFT,					_T("Meta+Shift+")},
	{CONTROL+META,					_T("Ctrl+Meta+")},
	{CONTROL+META+SHIFT,			_T("Ctrl+Meta+Shift+")},
	{CONTROLX+NONE,					_T("Ctrl+X ")},
	{CONTROLX+SHIFT,				_T("Ctrl+X Shift+")},
	{CONTROLX+CONTROL,				_T("Ctrl+X Ctrl+")},
	{CONTROLX+CONTROL+SHIFT,		_T("Ctrl+X Ctrl+Shift+")},
	{CONTROLX+META,					_T("Ctrl+X Meta+")},
	{CONTROLX+META+SHIFT,			_T("Ctrl+X Meta+Shift+")},
	{CONTROLX+CONTROL+META,			_T("Ctrl+X Ctrl+Meta+")},
	{CONTROLX+CONTROL+META+SHIFT,	_T("Ctrl+X Ctrl+Meta+Shift+")},
};

static const KeyName KeyNames[] = {
	{0,				_T("")},
	{VK_LBUTTON,	_T("Left mouse button")},
	{VK_RBUTTON,	_T("Right mouse button")},
	{VK_CANCEL,		_T("Break")},
	{VK_MBUTTON,	_T("Middle mouse button")},
	{0x05,			_T("X1 mouse button")},	// VK_XBUTTON1
	{0x06,			_T("X2 mouse button")},	// VK_XBUTTON2
	{0x07,			_T("Undefined")},
	{VK_BACK,		_T("Backspace")},
	{VK_TAB,		_T("Tab")},
	{0x0a,			_T("Reserved")},
	{0x0b,			_T("Reserved")},
	{VK_CLEAR,		_T("Clear")},
	{VK_RETURN,		_T("Enter")},
	{0x0e,			_T("Undefined")},
	{0x0f,			_T("Undefined")},
	{VK_SHIFT,		_T("Shift")},
	{VK_CONTROL,	_T("Ctrl")},
	{VK_MENU,		_T("Alt")},
	{VK_PAUSE,		_T("Pause")},
	{VK_CAPITAL,	_T("Caps Lock")},
	{VK_KANA,		_T("Kana")},	//	{VK_HANGUEL,	"Hanguel"},	{VK_HANGUL,		"Hangul"},
	{0x16,			_T("Undefined")},
	{VK_JUNJA,		_T("Junja")},
	{VK_FINAL,		_T("Final")},
	{VK_KANJI,		_T("Kanji")},	//	{VK_HANJA,		"Hanja"},
	{0x1a,			_T("Undefined")},
	{VK_ESCAPE,		_T("Esc")},
	{VK_CONVERT,	_T("Convert")},		// ïœä∑
	{VK_NONCONVERT,	_T("Nonconvert")},	// ñ≥ïœä∑
	{VK_ACCEPT,		_T("Accept")},
	{VK_MODECHANGE,	_T("Mode change")},
	{VK_SPACE,		_T("Space")},
	{VK_PRIOR,		_T("Page Up")},
	{VK_NEXT,		_T("Page Down")},
	{VK_END,		_T("End")},
	{VK_HOME,		_T("Home")},
	{VK_LEFT,		_T("Left")},
	{VK_UP,			_T("Up")},
	{VK_RIGHT,		_T("Right")},
	{VK_DOWN,		_T("Down")},
	{VK_SELECT,		_T("Select")},
	{VK_PRINT,		_T("Print")},
	{VK_EXECUTE,	_T("Execute")},
	{VK_SNAPSHOT,	_T("Print Screen")},
	{VK_INSERT,		_T("Ins")},
	{VK_DELETE,		_T("Del")},
	{VK_HELP,		_T("Help")},
	{'0',			_T("0")},
	{'1',			_T("1")},
	{'2',			_T("2")},
	{'3',			_T("3")},
	{'4',			_T("4")},
	{'5',			_T("5")},
	{'6',			_T("6")},
	{'7',			_T("7")},
	{'8',			_T("8")},
	{'9',			_T("9")},
	{0x3a,			_T("Undefined")},
	{0x3b,			_T("Undefined")},
	{0x3c,			_T("Undefined")},
	{0x3d,			_T("Undefined")},
	{0x3e,			_T("Undefined")},
	{0x3f,			_T("Undefined")},
	{0x40,			_T("Undefined")},
	{'A',			_T("A")},
	{'B',			_T("B")},
	{'C',			_T("C")},
	{'D',			_T("D")},
	{'E',			_T("E")},
	{'F',			_T("F")},
	{'G',			_T("G")},
	{'H',			_T("H")},
	{'I',			_T("I")},
	{'J',			_T("J")},
	{'K',			_T("K")},
	{'L',			_T("L")},
	{'M',			_T("M")},
	{'N',			_T("N")},
	{'O',			_T("O")},
	{'P',			_T("P")},
	{'Q',			_T("Q")},
	{'R',			_T("R")},
	{'S',			_T("S")},
	{'T',			_T("T")},
	{'U',			_T("U")},
	{'V',			_T("V")},
	{'W',			_T("W")},
	{'X',			_T("X")},
	{'Y',			_T("Y")},
	{'Z',			_T("Z")},
	{VK_LWIN,		_T("Left Windows")},
	{VK_RWIN,		_T("Right Windows")},
	{VK_APPS,		_T("Application")},
	{0x5e,			_T("Reserved")},
	{0x5f,			_T("Sleep")},	// VK_SLEEP
	{VK_NUMPAD0,	_T("Num 0")},
	{VK_NUMPAD1,	_T("Num 1")},
	{VK_NUMPAD2,	_T("Num 2")},
	{VK_NUMPAD3,	_T("Num 3")},
	{VK_NUMPAD4,	_T("Num 4")},
	{VK_NUMPAD5,	_T("Num 5")},
	{VK_NUMPAD6,	_T("Num 6")},
	{VK_NUMPAD7,	_T("Num 7")},
	{VK_NUMPAD8,	_T("Num 8")},
	{VK_NUMPAD9,	_T("Num 9")},
	{VK_MULTIPLY,	_T("Num *")},
	{VK_ADD,		_T("Num +")},
	{VK_SEPARATOR,	_T("Separator")},
	{VK_SUBTRACT,	_T("Num -")},
	{VK_DECIMAL,	_T("Num .")},
	{VK_DIVIDE,		_T("Num /")},
	{VK_F1,			_T("F1")},
	{VK_F2,			_T("F2")},
	{VK_F3,			_T("F3")},
	{VK_F4,			_T("F4")},
	{VK_F5,			_T("F5")},
	{VK_F6,			_T("F6")},
	{VK_F7,			_T("F7")},
	{VK_F8,			_T("F8")},
	{VK_F9,			_T("F9")},
	{VK_F10,		_T("F10")},
	{VK_F11,		_T("F11")},
	{VK_F12,		_T("F12")},
	{VK_F13,		_T("F13")},
	{VK_F14,		_T("F14")},
	{VK_F15,		_T("F15")},
	{VK_F16,		_T("F16")},
	{VK_F17,		_T("F17")},
	{VK_F18,		_T("F18")},
	{VK_F19,		_T("F19")},
	{VK_F20,		_T("F20")},
	{VK_F21,		_T("F21")},
	{VK_F22,		_T("F22")},
	{VK_F23,		_T("F23")},
	{VK_F24,		_T("F24")},
	{0x88,			_T("Unassigned")},
	{0x89,			_T("Unassigned")},
	{0x8a,			_T("Unassigned")},
	{0x8b,			_T("Unassigned")},
	{0x8c,			_T("Unassigned")},
	{0x8d,			_T("Unassigned")},
	{0x8e,			_T("Unassigned")},
	{0x8f,			_T("Unassigned")},
	{VK_NUMLOCK,	_T("Num Lock")},
	{VK_SCROLL,		_T("Scroll Lock")},
	{0x92,			_T("OEM specific")},
	{0x93,			_T("OEM specific")},
	{0x94,			_T("OEM specific")},
	{0x95,			_T("OEM specific")},
	{0x96,			_T("OEM specific")},
	{0x97,			_T("Unassigned")},
	{0x98,			_T("Unassigned")},
	{0x99,			_T("Unassigned")},
	{0x9a,			_T("Unassigned")},
	{0x9b,			_T("Unassigned")},
	{0x9c,			_T("Unassigned")},
	{0x9d,			_T("Unassigned")},
	{0x9e,			_T("Unassigned")},
	{0x9f,			_T("Unassigned")},
	{VK_LSHIFT,		_T("Left Shift")},
	{VK_RSHIFT,		_T("Right Shift")},
	{VK_LCONTROL,	_T("Left Ctrl")},
	{VK_RCONTROL,	_T("Right Ctrl")},
	{VK_LMENU,		_T("Left Alt")},
	{VK_RMENU,		_T("Right Alt")},
	{0xa6,			_T("Browser Back")},		// VK_BROWSER_BACK
	{0xa7,			_T("Browser Forward")},		// VK_BROWSER_FORWARD
	{0xa8,			_T("Browser Refresh")},		// VK_BROWSER_REFRESH
	{0xa9,			_T("Browser Stop")},		// VK_BROWSER_STOP
	{0xaa,			_T("Browser Search")},		// VK_BROWSER_SEARCH
	{0xab,			_T("Browser Favorites")},	// VK_BROWSER_FAVORITES
	{0xac,			_T("Browser Start")},		// VK_BROWSER_HOME
	{0xad,			_T("Volume Mute")},			// VK_VOLUME_MUTE
	{0xae,			_T("Volume Down")},			// VK_VOLUME_DOWN
	{0xaf,			_T("Volume Up")},			// VK_VOLUME_UP
	{0xb0,			_T("Next Track")},			// VK_MEDIA_NEXT_TRACK
	{0xb1,			_T("Previous Track")},		// VK_MEDIA_PREV_TRACK
	{0xb2,			_T("Stop Media")},			// VK_MEDIA_STOP
	{0xb3,			_T("Play/Pause Media")},	// VK_MEDIA_PLAY_PAUSE
	{0xb4,			_T("Start Mail")},			// VK_LAUNCH_MAIL
	{0xb5,			_T("Select Media")},		// VK_LAUNCH_MEDIA_SELECT
	{0xb6,			_T("Start Application 1")},	// VK_LAUNCH_APP1
	{0xb7,			_T("Start Application 2")},	// VK_LAUNCH_APP2
	{0xb8,			_T("Reserved")},
	{0xb9,			_T("Reserved")},
	{0xba,			_T(";")},					// VK_OEM_1
//	{0xba,			_T(":")},					// VK_OEM_1		// for Japanese keyboard
	{0xbb,			_T("+")},					// VK_OEM_PLUS
//	{0xbb,			_T(";")},					// VK_OEM_PLUS	// for Japanese keyboard
	{0xbc,			_T(",")},					// VK_OEM_COMMA
	{0xbd,			_T("-")},					// VK_OEM_MINUS
	{0xbe,			_T(".")},					// VK_OEM_PERIOD
	{0xbf,			_T("/")},					// VK_OEM_2
	{0xc0,			_T("`")},					// VK_OEM_3
//	{0xc0,			_T("@")},					// VK_OEM_3		// for Japanese keyboard
	{0xc1,			_T("Reserved")},
	{0xc2,			_T("Reserved")},
	{0xc3,			_T("Reserved")},
	{0xc4,			_T("Reserved")},
	{0xc5,			_T("Reserved")},
	{0xc6,			_T("Reserved")},
	{0xc7,			_T("Reserved")},
	{0xc8,			_T("Reserved")},
	{0xc9,			_T("Reserved")},
	{0xca,			_T("Reserved")},
	{0xcb,			_T("Reserved")},
	{0xcc,			_T("Reserved")},
	{0xcd,			_T("Reserved")},
	{0xce,			_T("Reserved")},
	{0xcf,			_T("Reserved")},
	{0xd0,			_T("Reserved")},
	{0xd1,			_T("Reserved")},
	{0xd2,			_T("Reserved")},
	{0xd3,			_T("Reserved")},
	{0xd4,			_T("Reserved")},
	{0xd5,			_T("Reserved")},
	{0xd6,			_T("Reserved")},
	{0xd7,			_T("Reserved")},
	{0xd8,			_T("Unassigned")},
	{0xd9,			_T("Unassigned")},
	{0xda,			_T("Unassigned")},
	{0xdb,			_T("[")},					// VK_OEM_4
	{0xdc,			_T("Backslash")},			// VK_OEM_5
	{0xdd,			_T("]")},					// VK_OEM_6
	{0xde,			_T("'")},					// VK_OEM_7
	{0xdf,			_T("OEM specific")},		// VK_OEM_8
	{0xe0,			_T("Reserved")},
	{0xe1,			_T("OEM specific")},
	{0xe2,			_T("Backslash for 106 keyboard")},	// VK_OEM_102
	{0xe3,			_T("OEM specific")},
	{0xe4,			_T("OEM specific")},
	{0xe5,			_T("Process")},				// VK_PROCESSKEY
	{0xe6,			_T("OEM specific")},
	{0xe7,			_T("Packet")},	// VK_PACKET
	{0xe8,			_T("Unassigned")},
	{0xe9,			_T("OEM specific")},
	{0xea,			_T("OEM specific")},
	{0xeb,			_T("OEM specific")},
	{0xec,			_T("OEM specific")},
	{0xed,			_T("OEM specific")},
	{0xee,			_T("OEM specific")},
	{0xef,			_T("OEM specific")},
	{0xf0,			_T("Eisu")},					// âpêî
	{0xf1,			_T("OEM specific")},
	{0xf2,			_T("Hiragana")},				// Ç–ÇÁÇ™Ç»
	{0xf3,			_T("Hankaku/Zenkaku 0xf3")},	// "îºäp/ëSäp"
	{0xf4,			_T("Hankaku/Zenkaku 0xf4")},	// "îºäp/ëSäp"
	{0xf5,			_T("OEM specific")},
	{VK_ATTN,		_T("Attn")},
	{VK_CRSEL,		_T("CrSel")},
	{VK_EXSEL,		_T("ExSel")},
	{VK_EREOF,		_T("Erace EOF")},
	{VK_PLAY,		_T("Play")},
	{VK_ZOOM,		_T("Zoom")},
	{VK_NONAME,		_T("Noname")},
	{VK_PA1,		_T("PA1")},
	{VK_OEM_CLEAR,	_T("OEM Clear")},
	{0xff,			_T("Fn")},
};

Config CProfile::m_Config;
TCHAR CProfile::m_szAppTitle[MAX_APP][WINDOW_TEXT_LENGTH];
TASK_LIST CProfile::m_TaskList[MAX_TASKS];
DWORD CProfile::m_dwTasks;

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
				CUtils::SetCorrectApplicationName(pTask[i].ProcessName, szWindowName);
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
					ReadKeyBind(nType, nKey, szKeyBind);
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
					ReadKeyBind(nType, nKey, szKeyBind);
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

		const CString regApp = CString(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA)) + _T("\\") + szAppName;
		// Create all commands
		for (int nComID = 1; nComID < MAX_COMMAND; ++nComID)
			SaveCommand(szAppName, nComID);
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

void CProfile::ReadKeyBind(int& nCommandType, int& nKey, const LPCTSTR szKeyBind)
{
	nCommandType = KeyBind2CommandType(szKeyBind);
	nKey = KeyBind2Key(szKeyBind + _tcslen(CommandType2String(nCommandType)));
}

CString CProfile::WriteKeyBind(const int nType, const int nKey)
{
	CString szKeyBind;
	szKeyBind.Format(_T("%s%s"), CommandType2String(nType), Key2String(nKey));
	return szKeyBind;
}

int CProfile::KeyBind2CommandType(LPCTSTR szKeyBind)
{
	for (int nType = MAX_COMMAND_TYPE - 1; nType; --nType) {
		if (IsCommandType(nType, szKeyBind)) {
			return nType;
		}
	}
	return NONE;
}

int CProfile::KeyBind2Key(LPCTSTR szKey)
{
	for (int nKey = 1; nKey <= 0xff; ++nKey) {
		if (!_tcscmp(szKey, Key2String(nKey))) {
			return nKey;
		}
	}
	return 0;
}

LPCTSTR CProfile::CommandType2String(int nType)
{
	if (nType < 0 || sizeof(CommandTypes) / sizeof(CommandTypes[0]) <= nType) {
		ASSERT(0);
		nType = NONE;
	}
	return CommandTypes[nType].szName;
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

BOOL CProfile::IsCommandType(const int nType, LPCTSTR szKeyBind)
{
	LPCTSTR szCommandType = CommandType2String(nType);

	if (!_tcsnicmp(szKeyBind, szCommandType, _tcslen(szCommandType))) {
		return TRUE;
	}

	return FALSE;
}

void CProfile::SaveKeyBind(const LPCSTR szAppName, const int nComID, const int nType, const int nKey)
{
	if (!nComID)
		return;
	const LPCSTR szComName = CCommands::GetCommandName(nComID);
	if (!szComName[0])
		return;
	SaveKeyBind(szAppName, szComName, nType, nKey);
}

void CProfile::SaveKeyBind(const LPCSTR szAppName, const LPCSTR szComName, const int nType, const int nKey)
{
	const CString szKeyBind = WriteKeyBind(nType, nKey);
	CString szSubKey = CString(MAKEINTRESOURCE(IDS_REGSUBKEY_DATA)) + _T("\\") + szAppName + _T("\\") + szComName;
	if (!szKeyBind.IsEmpty())
		szSubKey += _T("\\") + szKeyBind;
	HKEY hKey = NULL;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
		RegCloseKey(hKey);
}

void CProfile::SaveCommand(const LPCSTR szAppName, const int nComID)
{
	SaveKeyBind(szAppName, nComID, 0, 0);
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
						ReadKeyBind(nType, nKey, szKeyBind);
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
