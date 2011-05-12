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

#define PROCESS_SIZE        MAX_PATH

//
// Function pointer types for accessing Toolhelp32 functions dynamically.
// By dynamically accessing these functions, we can do so only on Windows
// 95 and still run on Windows NT, which does not have these functions.
//
#include <tlhelp32.h>  // for Windows 95
typedef BOOL (WINAPI *PROCESSWALK)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
typedef HANDLE (WINAPI *CREATESNAPSHOT)(DWORD dwFlags, DWORD th32ProcessID);

struct TASK_LIST {
    DWORD dwProcessId;
    CHAR ProcessName[PROCESS_SIZE];
};
typedef TASK_LIST *PTASK_LIST;

enum { MAX_TASKS = 256 };

enum KEY_TYPE { NORMAL_KEY, PUSHED_KEY, DROPPABLE_KEY, ORIGINAL_KEY, REMAPPED_KEY, REMAPPED_PUSHED_KEY };
enum HKEY_TYPE { CURRENT_USER, LOCAL_MACHINE, MAX_HKEY_TYPE }; // USERS, CLASSES_ROOT, CURRENT_CONFIG, 

struct ScanCode
{
	BYTE nScanCode;
	BYTE nPrefixedScanCode;	// 0x00, 0xE0 or 0xE1	cf. Keyboard Scan Code Specification -- 16
};

struct ScanCodeMapping
{
	ScanCode current;
	ScanCode original;
};

class CKey;

typedef struct KeyboardLayout
{
	ScanCode scancode;
	int nBaseControlID;
	int nCurrentControlID;
	int nToolTipID;
	CKey *pBaseKey;
	CKey *pCurrentKey;
} KeyboardLayout;

static KeyboardLayout KeyboardLayouts[] = {
	{{0x01, 0x00}, IDC_ESC,				IDC_ESC_,			IDS_ESC},
	{{0x02, 0x00}, IDC_1,				IDC_1_,				IDS_1},
	{{0x03, 0x00}, IDC_2,				IDC_2_,				IDS_2},
	{{0x04, 0x00}, IDC_3,				IDC_3_,				IDS_3},
	{{0x05, 0x00}, IDC_4,				IDC_4_,				IDS_4},
	{{0x06, 0x00}, IDC_5,				IDC_5_,				IDS_5},
	{{0x07, 0x00}, IDC_6,				IDC_6_,				IDS_6},
	{{0x08, 0x00}, IDC_7,				IDC_7_,				IDS_7},
	{{0x09, 0x00}, IDC_8,				IDC_8_,				IDS_8},
	{{0x0a, 0x00}, IDC_9,				IDC_9_,				IDS_9},
	{{0x0b, 0x00}, IDC_0,				IDC_0_,				IDS_0_},
	{{0x0c, 0x00}, IDC_MINUS,			IDC_MINUS_,			IDS_MINUS},
	{{0x0d, 0x00}, IDC_EQUAL,			IDC_EQUAL_,			IDS_EQUAL},		// =		/// ^
	{{0x0e, 0x00}, IDC_BACKSPACE,		IDC_BACKSPACE_,		IDS_BACKSPACE},
	{{0x0f, 0x00}, IDC_TAB,				IDC_TAB_,			IDS_TAB},
	{{0x10, 0x00}, IDC_Q,				IDC_Q_,				IDS_Q},
	{{0x11, 0x00}, IDC_W,				IDC_W_,				IDS_W},
	{{0x12, 0x00}, IDC_E,				IDC_E_,				IDS_E},
	{{0x13, 0x00}, IDC_R,				IDC_R_,				IDS_R},
	{{0x14, 0x00}, IDC_T,				IDC_T_,				IDS_T},
	{{0x15, 0x00}, IDC_Y,				IDC_Y_,				IDS_Y},
	{{0x16, 0x00}, IDC_U,				IDC_U_,				IDS_U},
	{{0x17, 0x00}, IDC_I,				IDC_I_,				IDS_I},
	{{0x18, 0x00}, IDC_O,				IDC_O_,				IDS_O},
	{{0x19, 0x00}, IDC_P,				IDC_P_,				IDS_P},
	{{0x1a, 0x00}, IDC_SQUARE_BRA,		IDC_SQUARE_BRA_,	IDS_SQUARE_BRA},	// 101/104: [, 106/109: @
	{{0x1b, 0x00}, IDC_SQUARE_CKET,		IDC_SQUARE_CKET_,	IDS_SQUARE_CKET},	// 101/104: ], 106/109: [
	{{0x1c, 0x00}, IDC_ENTER,			IDC_ENTER_,			IDS_ENTER},
	{{0x1c, 0xe0}, IDC_NUMPADENTER,		IDC_NUMPADENTER_,	IDS_NUMPADENTER},
	{{0x1d, 0x00}, IDC_LEFT_CTRL,		IDC_LEFT_CTRL_,		IDS_LEFT_CTRL},
	{{0x1d, 0xe0}, IDC_RIGHT_CTRL,		IDC_RIGHT_CTRL_,	IDS_RIGHT_CTRL},
	{{0x1d, 0xe1}, IDC_PAUSE,			IDC_PAUSE_,			IDS_PAUSE},
	{{0x1e, 0x00}, IDC_A,				IDC_A_,				IDS_A},
	{{0x1f, 0x00}, IDC_S,				IDC_S_,				IDS_S},
	{{0x20, 0x00}, IDC_D,				IDC_D_,				IDS_D},
	{{0x21, 0x00}, IDC_F,				IDC_F_,				IDS_F},
	{{0x22, 0x00}, IDC_G,				IDC_G_,				IDS_G},
	{{0x23, 0x00}, IDC_H,				IDC_H_,				IDS_H},
	{{0x24, 0x00}, IDC_J,				IDC_J_,				IDS_J},
	{{0x25, 0x00}, IDC_K,				IDC_K_,				IDS_K},
	{{0x26, 0x00}, IDC_L,				IDC_L_,				IDS_L},
	{{0x27, 0x00}, IDC_SEMICOLON,		IDC_SEMICOLON_,		IDS_SEMICOLON},
	{{0x28, 0x00}, IDC_QUOTE,			IDC_QUOTE_,			IDS_QUOTE},		// 101/104: ', 106/109: :
	{{0x29, 0x00}, IDC_BACK_QUOTE,		IDC_BACK_QUOTE_,	IDS_BACK_QUOTE},	// 101/104: `, 106/109: hankaku/zenkaku
	{{0x2a, 0x00}, IDC_LEFT_SHIFT,		IDC_LEFT_SHIFT_,	IDS_LEFT_SHIFT},
	{{0x2b, 0x00}, IDC_BACKSLASH,		IDC_BACKSLASH_,		IDS_BACKSLASH},	// 101/104: \, 106/109: ]
	{{0x2c, 0x00}, IDC_Z,				IDC_Z_,				IDS_Z},
	{{0x2d, 0x00}, IDC_X,				IDC_X_,				IDS_X},
	{{0x2e, 0x00}, IDC_C,				IDC_C_,				IDS_C},
	{{0x2f, 0x00}, IDC_V,				IDC_V_,				IDS_V},
	{{0x30, 0x00}, IDC_B,				IDC_B_,				IDS_B},
	{{0x31, 0x00}, IDC_N,				IDC_N_,				IDS_N},
	{{0x32, 0x00}, IDC_M,				IDC_M_,				IDS_M},
	{{0x33, 0x00}, IDC_COMMA,			IDC_COMMA_,			IDS_COMMA},
	{{0x34, 0x00}, IDC_PERIOD,			IDC_PERIOD_,		IDS_PERIOD},
	{{0x35, 0x00}, IDC_SLASH,			IDC_SLASH_,			IDS_SLASH},
	{{0x35, 0xe0}, IDC_DIVIDE,			IDC_DIVIDE_,		IDS_DIVIDE},
	{{0x36, 0x00}, IDC_RIGHT_SHIFT,		IDC_RIGHT_SHIFT_,	IDS_RIGHT_SHIFT},
	{{0x37, 0x00}, IDC_MULTIPLY,		IDC_MULTIPLY_,		IDS_MULTIPLY},
	{{0x37, 0xe0}, IDC_PRINT_SCREEN,	IDC_PRINT_SCREEN_,	IDS_PRINT_SCREEN},
	{{0x38, 0x00}, IDC_LEFT_ALT,		IDC_LEFT_ALT_,		IDS_LEFT_ALT},
	{{0x38, 0xe0}, IDC_RIGHT_ALT,		IDC_RIGHT_ALT_,		IDS_RIGHT_ALT},
	{{0x39, 0x00}, IDC_SPACE,			IDC_SPACE_,			IDS_SPACE},
	{{0x3a, 0x00}, IDC_CAPS_LOCK,		IDC_CAPS_LOCK_,		IDS_CAPS_LOCK},
	{{0x3b, 0x00}, IDC_F1,				IDC_F1_,			IDS_F1},
	{{0x3c, 0x00}, IDC_F2,				IDC_F2_,			IDS_F2},
	{{0x3d, 0x00}, IDC_F3,				IDC_F3_,			IDS_F3},
	{{0x3e, 0x00}, IDC_F4,				IDC_F4_,			IDS_F4},
	{{0x3f, 0x00}, IDC_F5,				IDC_F5_,			IDS_F5},
	{{0x40, 0x00}, IDC_F6,				IDC_F6_,			IDS_F6},
	{{0x41, 0x00}, IDC_F7,				IDC_F7_,			IDS_F7},
	{{0x42, 0x00}, IDC_F8,				IDC_F8_,			IDS_F8},
	{{0x43, 0x00}, IDC_F9,				IDC_F9_,			IDS_F9},
	{{0x44, 0x00}, IDC_F10,				IDC_F10_,			IDS_F10},
	{{0x45, 0xe0}, IDC_NUM_LOCK,		IDC_NUM_LOCK_,		IDS_NUM_LOCK},
	{{0x46, 0x00}, IDC_SCROLL_LOCK,		IDC_SCROLL_LOCK_,	IDS_SCROLL_LOCK},
	{{0x47, 0x00}, IDC_NUMPAD7,			IDC_NUMPAD7_,		IDS_NUMPAD7},
	{{0x47, 0xe0}, IDC_HOME,			IDC_HOME_,			IDS_HOME},
	{{0x48, 0x00}, IDC_NUMPAD8,			IDC_NUMPAD8_,		IDS_NUMPAD8},
	{{0x48, 0xe0}, IDC_UP,				IDC_UP_,			IDS_UP},
	{{0x49, 0x00}, IDC_NUMPAD9,			IDC_NUMPAD9_,		IDS_NUMPAD9},
	{{0x49, 0xe0}, IDC_PAGE_UP,			IDC_PAGE_UP_,		IDS_PAGE_UP},
	{{0x4a, 0x00}, IDC_SUBTRACT,		IDC_SUBTRACT_,		IDS_SUBTRACT},
	{{0x4b, 0x00}, IDC_NUMPAD4,			IDC_NUMPAD4_,		IDS_NUMPAD4},
	{{0x4b, 0xe0}, IDC_LEFT,			IDC_LEFT_,			IDS_LEFT},
	{{0x4c, 0x00}, IDC_NUMPAD5,			IDC_NUMPAD5_,		IDS_NUMPAD5},
	{{0x4d, 0x00}, IDC_NUMPAD6,			IDC_NUMPAD6_,		IDS_NUMPAD6},
	{{0x4d, 0xe0}, IDC_RIGHT,			IDC_RIGHT_,			IDS_RIGHT},
	{{0x4e, 0x00}, IDC_ADD,				IDC_ADD_,			IDS_ADD},
	{{0x4f, 0x00}, IDC_NUMPAD1,			IDC_NUMPAD1_,		IDS_NUMPAD1},
	{{0x4f, 0xe0}, IDC_END,				IDC_END_,			IDS_END},
	{{0x50, 0x00}, IDC_NUMPAD2,			IDC_NUMPAD2_,		IDS_NUMPAD2},
	{{0x50, 0xe0}, IDC_DOWN,			IDC_DOWN_,			IDS_DOWN},
	{{0x51, 0x00}, IDC_NUMPAD3,			IDC_NUMPAD3_,		IDS_NUMPAD3},
	{{0x51, 0xe0}, IDC_PAGE_DOWN,		IDC_PAGE_DOWN_,		IDS_PAGE_DOWN},
	{{0x52, 0x00}, IDC_NUMPAD0,			IDC_NUMPAD0_,		IDS_NUMPAD0},
	{{0x52, 0xe0}, IDC_INSERT,			IDC_INSERT_,		IDS_INSERT},
	{{0x53, 0x00}, IDC_NUMPADDELETE,	IDC_NUMPADDELETE_,	IDS_NUMPADDELETE},
	{{0x53, 0xe0}, IDC_DELETE,			IDC_DELETE_,		IDS_DELETE},
	{{0x57, 0x00}, IDC_F11,				IDC_F11_,			IDS_F11},
	{{0x58, 0x00}, IDC_F12,				IDC_F12_,			IDS_F12},
	{{0x5b, 0xe0}, IDC_LEFT_WINDOWS,	IDC_LEFT_WINDOWS_,	IDS_LEFT_WINDOWS},
	{{0x5c, 0xe0}, IDC_RIGHT_WINDOWS,	IDC_RIGHT_WINDOWS_,	IDS_RIGHT_WINDOWS},
	{{0x5d, 0xe0}, IDC_APPLICATION,		IDC_APPLICATION_,	IDS_APPLICATION},
	{{0x5e, 0xe0}, IDC_ACPI_POWER,		IDC_ACPI_POWER_,	IDS_ACPI_POWER},
	{{0x5f, 0xe0}, IDC_ACPI_SLEEP,		IDC_ACPI_SLEEP_,	IDS_ACPI_SLEEP},
	{{0x63, 0xe0}, IDC_ACPI_WAKE,		IDC_ACPI_WAKE_,		IDS_ACPI_WAKE},
	{{0x70, 0x00}, IDC_KANA,			IDC_KANA_,			IDS_KANA},
	{{0x73, 0x00}, IDC_BACKSLASH2,		IDC_BACKSLASH2_,	IDS_BACKSLASH2},	// \  ,./\ 
	{{0x79, 0x00}, IDC_CONVERT,			IDC_CONVERT_,		IDS_CONVERT},
	{{0x7b, 0x00}, IDC_NONCONVERT,		IDC_NONCONVERT_,	IDS_NONCONVERT},
	{{0x7d, 0x00}, IDC_BACKSLASH1,		IDC_BACKSLASH1_,	IDS_BACKSLASH1},	// \  -^\ 
};

class CProfile
{
public:
	static int GetKeyboardSpeed(void);
	static int LostKeyWarning(HKEY_TYPE hkeyType);
	static CString GetWindowText(int nApplicationID);
	static void SetWindowText(int nApplicationID, CString szWindowText);
	static void SetEnableCUA(int nApplicationID, BOOL bEnableCUA);
	static BOOL GetEnableCUA(int nApplicationID);
	static void ImportProperties();
	static void ExportProperties();
	static KeyboardLayout* GetKeyboardLayouts(int nKey);
	static int GetToolTipID(int nToolTipID);
	static void SetNoCursor();
	static void SetNormalCursor();
	static void SetDraggingCursor();
	static void SetScanCodeMap(HKEY_TYPE hkeyType, ScanCodeMapping mapping);
	static int GetCurrentControlID(ScanCode scancode);
	static int GetBaseControlID(ScanCode scancode);
	static BOOL GetScanCodeMap(HKEY_TYPE hkeyType, ScanCode original, ScanCode *current);
	static void RestartComputer();
	static BOOL IsVistaOrLater();
	static BOOL IsNT();
	static BOOL Is9x();
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
	static TASK_LIST m_TaskList[MAX_TASKS];
	static DWORD m_dwTasks;
	static void InitApplicationList(CComboBox *cApplicationList);
	static int GetSavedSettingCount();
	static void ClearData(CString szCurrentApplication);
	static void InitDllData();
	static void SaveRegistryData();
	static void LoadRegistryData();
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
	static BOOL ChangedKeyboardLayout(HKEY_TYPE hkeyType);
	static BYTE PrefixedScanCodeID2Code(int nPrefixedScanCodeID);
	static DWORD GetScanCodeLength(HKEY_TYPE hkeyType);
	static int PrefixedScanCode2ID(BYTE nPrefixedScanCode);
	static ScanCode m_CurrentScanCodeMap[MAX_HKEY_TYPE][4][256];
	static ScanCode m_ScanCodeMap[MAX_HKEY_TYPE][4][256];
	static int GetControlID(ScanCode scancode, BOOL bBase);
	static void Item2AppName(CString *sz);
	static int IsNotSameString(CComboBox *pApplication, CString szListItem);
	static int CountSeparator(CString szMainString, CString szSeparator);
	static void GetNthString(CString *szAppName, CString szWindowName, CString szSeparator, int n);
	static void GetAppName(CString *szAppName, LPCTSTR pWindowName);
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	static BOOL IsTheString(CString sz, UINT nID);
	static PPERF_DATA_BLOCK GetPerformanceData(CString szProcessName);
	static BOOL GetProcessInfo(CString *szProcessName, DWORD *dwProcessId);
	static LPBYTE GetCounters();
	static void SetDllData();
	static CXkeymacsData m_XkeymacsData[MAX_APP];
	static void DeleteAllRegistryData();
	static DWORD GetTaskListNT(PTASK_LIST pTask, DWORD dwNumTasks);
	static DWORD GetTaskList(PTASK_LIST pTask, DWORD dwNumTasks);
	static BOOL IsCommandType(int nCommandType, LPCTSTR szKeyBind);
	static int KeyBind2Key(LPCTSTR szKey);
	static int KeyBind2CommandType(LPCTSTR szKeyBind);
	static CString WriteKeyBind(int nCommandType, int nKey);
	static void UpdateRegistryData(BOOL bSaveAndValidate);
};

struct CommandTypeName
{
	int nCommandType;
	LPCTSTR szCommandTypeName;
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


#endif // !defined(AFX_PROFILE_H__9415254D_4656_484B_A730_E02580D8A221__INCLUDED_)
