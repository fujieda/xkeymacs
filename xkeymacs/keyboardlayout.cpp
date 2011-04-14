#include "stdafx.h"
#include "keyboardlayout.h"

const int MAX_KEYBOARD_LAYOUT = 112;
KeyboardLayout CKeyboardLayout::m_KeyboardLayouts[] = {
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

ScanCode CKeyboardLayout::m_CurrentScanCodeMap[MAX_HKEY_TYPE][4][256];
ScanCode CKeyboardLayout::m_ScanCodeMap[MAX_HKEY_TYPE][4][256];

CKeyboardLayout::CKeyboardLayout(const HKEY_TYPE hkey_type, UINT nIDTemplate, CWnd* pParentWnd)
	: CDialog(nIDTemplate, pParentWnd)
{
	m_HkeyType = hkey_type;
}

CKeyboardLayout::~CKeyboardLayout()
{
}

void CKeyboardLayout::InitKeyboardLayout()
{
	m_ToolTip.Create(this, TTS_ALWAYSTIP | TTS_NOPREFIX);
	m_ToolTip.SetMaxTipWidth(0x100);	// Enable multiline

	LoadScanCodeMap(m_HkeyType);
		for (int i = 0; i < MAX_KEYBOARD_LAYOUT; ++i) {
		if (!GetDlgItem(m_KeyboardLayouts[i].nBaseControlID)
		 || !GetDlgItem(m_KeyboardLayouts[i].nCurrentControlID)) {
			continue;
		}

		m_KeyboardLayouts[i].pBaseKey = new CKey(m_KeyboardLayouts[i].nBaseControlID, NORMAL_KEY, m_HkeyType);
		m_KeyboardLayouts[i].pBaseKey->SubclassDlgItem(m_KeyboardLayouts[i].nBaseControlID, this);
		m_ToolTip.AddTool(GetDlgItem(m_KeyboardLayouts[i].nBaseControlID), GetToolTipID(m_KeyboardLayouts[i].nToolTipID));

		m_KeyboardLayouts[i].pCurrentKey = new CKey(m_KeyboardLayouts[i].nCurrentControlID, ORIGINAL_KEY, m_HkeyType);
		m_KeyboardLayouts[i].pCurrentKey->SubclassDlgItem(m_KeyboardLayouts[i].nCurrentControlID, this);
		m_ToolTip.AddTool(GetDlgItem(m_KeyboardLayouts[i].nCurrentControlID), GetToolTipID(m_KeyboardLayouts[i].nToolTipID));

		ScanCode current = {'\0'};
		if (GetScanCodeMap(m_HkeyType, m_KeyboardLayouts[i].scancode, &current)) {
			m_KeyboardLayouts[i].pCurrentKey->SetKeyType(REMAPPED_KEY);

			CString szWindowText;
			GetDlgItem(GetBaseControlID(current))->GetWindowText(szWindowText);
			GetDlgItem(m_KeyboardLayouts[i].nCurrentControlID)->SetWindowText(szWindowText);

			KeyboardLayout *pKeyboardLayout = GetKeyboardLayout(GetBaseControlID(current));
			if (pKeyboardLayout) {
				m_ToolTip.UpdateTipText(GetToolTipID(pKeyboardLayout->nToolTipID), GetDlgItem(m_KeyboardLayouts[i].nCurrentControlID));
			}
		}
	}
}

KeyboardLayout *CKeyboardLayout::GetKeyboardLayout(const int nKey, BOOL bBaseOnly)
{
	for (int i = 0; i < MAX_KEYBOARD_LAYOUT; ++i) {
		if (m_KeyboardLayouts[i].nBaseControlID == nKey
		 || !bBaseOnly && m_KeyboardLayouts[i].nCurrentControlID == nKey) {
			return &m_KeyboardLayouts[i];
		}
	}
	return NULL;
}

void CKeyboardLayout::DestroyKeyboardLayout()
{
	for (int i = 0; i < MAX_KEYBOARD_LAYOUT; ++i) {
		if (!GetDlgItem(m_KeyboardLayouts[i].nBaseControlID)
		 || !GetDlgItem(m_KeyboardLayouts[i].nCurrentControlID)) {
			continue;
		}
		if (m_KeyboardLayouts[i].pBaseKey) {
			delete m_KeyboardLayouts[i].pBaseKey;
			m_KeyboardLayouts[i].pBaseKey = NULL;
		}
		if (m_KeyboardLayouts[i].pCurrentKey) {
			delete m_KeyboardLayouts[i].pCurrentKey;
			m_KeyboardLayouts[i].pCurrentKey = NULL;
		}
	}
}

CToolTipCtrl *CKeyboardLayout::ToolTip()
{
	return &m_ToolTip;
}

void CKeyboardLayout::LoadScanCodeMap(const HKEY_TYPE hkeyType)
{
	memset(m_ScanCodeMap[hkeyType], 0, sizeof(m_ScanCodeMap[hkeyType]));
	memset(m_CurrentScanCodeMap[hkeyType], 0, sizeof(m_CurrentScanCodeMap[hkeyType]));

	CString szSubKey;
	CString szValueName;
	HKEY hKey = HKEY_LOCAL_MACHINE;
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
					ScanCodeMapping *pMapping = (ScanCodeMapping *)(lpData + offset);
					offset += 4;	// go to next data
					m_CurrentScanCodeMap[hkeyType][Prefix2ID(pMapping->original.nPrefix)][pMapping->original.nScanCode].nPrefix = pMapping->current.nPrefix;
					m_CurrentScanCodeMap[hkeyType][Prefix2ID(pMapping->original.nPrefix)][pMapping->original.nScanCode].nScanCode = pMapping->current.nScanCode;
					m_ScanCodeMap[hkeyType][Prefix2ID(pMapping->original.nPrefix)][pMapping->original.nScanCode].nPrefix = pMapping->current.nPrefix;
					m_ScanCodeMap[hkeyType][Prefix2ID(pMapping->original.nPrefix)][pMapping->original.nScanCode].nScanCode = pMapping->current.nScanCode;
				}
			}
		}
		delete[] lpData;
		lpData = NULL;
	}
}

int CKeyboardLayout::LostKeyWarning(const HKEY_TYPE hkeyType)
{
	if (!ChangedKeyboardLayout(hkeyType)) {
		return IDOK;
	}

	CString szLostKeysList;

	for (int nPrefixID = 0; nPrefixID < 3; ++nPrefixID) {
		for (int nScanCode = 0; nScanCode < 256; ++nScanCode) {
			if (m_ScanCodeMap[hkeyType][nPrefixID][nScanCode].nScanCode) {
				BOOL lostKey = TRUE;

				for (int i = 0; i < 3 && lostKey; ++i) {
					for (int j = 0; j < 256 && lostKey; ++j) {
						if (m_ScanCodeMap[hkeyType][i][j].nPrefix == nPrefixID
						 && m_ScanCodeMap[hkeyType][i][j].nScanCode == nScanCode) {
							lostKey = FALSE;
						}
					}
				}

				if (lostKey) {
					for (int i = 0; i < MAX_KEYBOARD_LAYOUT; ++i) {
						if (m_KeyboardLayouts[i].scancode.nScanCode == nScanCode
							&& m_KeyboardLayouts[i].scancode.nPrefix == nPrefixID) {
							CString szLostKey;
							szLostKey.Format(IDS_ERR_LOST_KEY, CString(MAKEINTRESOURCE(GetToolTipID(m_KeyboardLayouts[i].nToolTipID))));
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

void CKeyboardLayout::SaveScanCodeMap(const HKEY_TYPE hkeyType)
{
	CString szSubKey;
	CString szValueName;
	HKEY hKey = HKEY_LOCAL_MACHINE;
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

	HKEY hkResult = NULL;
	if (RegCreateKeyEx(hKey, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hkResult, NULL) == ERROR_SUCCESS) {
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
			for (int nPrefixID = 0; nPrefixID < 3; ++nPrefixID) {
				for (int nScanCode = 0; nScanCode < 256; ++nScanCode) {
					if (m_ScanCodeMap[hkeyType][nPrefixID][nScanCode].nScanCode) {
						ScanCodeMapping sScanCodeMapping = {'\0'};
						sScanCodeMapping.original.nPrefix = PrefixID2Code(nPrefixID);
						sScanCodeMapping.original.nScanCode = (BYTE)nScanCode;
						sScanCodeMapping.current.nPrefix = m_ScanCodeMap[hkeyType][nPrefixID][nScanCode].nPrefix;
						sScanCodeMapping.current.nScanCode = m_ScanCodeMap[hkeyType][nPrefixID][nScanCode].nScanCode;
						memcpy(lpData + offset, &sScanCodeMapping, sizeof(sScanCodeMapping));
						offset += sizeof(sScanCodeMapping);
					}
				}
			}
			RegSetValueEx(hkResult, szValueName, 0, REG_BINARY, lpData, cbData);

			delete[] lpData;
			lpData = NULL;
		}
		RegCloseKey(hkResult);
	}

	// Do you want to restart computer?
	if (ChangedKeyboardLayout(hkeyType)) {
		if (AfxMessageBox(CString(MAKEINTRESOURCE(IDS_RESTART_OR_NOT)), MB_YESNO | MB_ICONQUESTION) == IDYES) {
			CProfile::RestartComputer();
		}
	}
}

int CKeyboardLayout::GetControlID(const ScanCode scancode, const BOOL bBase)
{
	for (int i = 0; i < MAX_KEYBOARD_LAYOUT; i++)
		if (m_KeyboardLayouts[i].scancode.nPrefix == scancode.nPrefix &&
				m_KeyboardLayouts[i].scancode.nScanCode == scancode.nScanCode) {
			if (bBase)
				return m_KeyboardLayouts[i].nBaseControlID;
			else
				return m_KeyboardLayouts[i].nCurrentControlID;
		}
	return 0;
}

int CKeyboardLayout::GetBaseControlID(const ScanCode scancode)
{
	return GetControlID(scancode, TRUE);
}

int CKeyboardLayout::GetCurrentControlID(const ScanCode scancode)
{
	return GetControlID(scancode, FALSE);
}

BOOL CKeyboardLayout::GetScanCodeMap(const HKEY_TYPE hkeyType, const ScanCode original, ScanCode *const current)
{
	if (!current) {
		return FALSE;
	}
	*current = m_ScanCodeMap[hkeyType][Prefix2ID(original.nPrefix)][original.nScanCode];
	return 0 < current->nScanCode;
}

void CKeyboardLayout::SetScanCodeMap(const HKEY_TYPE hkeyType, const ScanCodeMapping ScanCodeMappeing)
{
	m_ScanCodeMap[hkeyType][Prefix2ID(ScanCodeMappeing.original.nPrefix)][ScanCodeMappeing.original.nScanCode] = ScanCodeMappeing.current;
}

int CKeyboardLayout::Prefix2ID(const BYTE nPrefix)
{
	int nID = 0;
	switch (nPrefix) {
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

BYTE CKeyboardLayout::PrefixID2Code(const int nID)
{
	BYTE nCode = 0;
	switch (nID) {
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

DWORD CKeyboardLayout::GetScanCodeLength(const HKEY_TYPE hkeyType)
{
	DWORD dwScanCodeLength = 0;
	dwScanCodeLength += 4;	// Header: Version Information
	dwScanCodeLength += 4;	// Header: Flags
	dwScanCodeLength += 4;	// Header: Number of Mappings
	for (int nID = 0; nID < 3; nID++)
		for (int nScanCode = 0; nScanCode < 256; nScanCode++)
			if (m_ScanCodeMap[hkeyType][nID][nScanCode].nScanCode)
				dwScanCodeLength += 4;	// Individual Mappings
	dwScanCodeLength += 4;	// Null Terminator (0x00000000)
	return dwScanCodeLength;
}

BOOL CKeyboardLayout::ChangedKeyboardLayout(const HKEY_TYPE hkeyType)
{
	for (int nID = 0; nID < 3; nID++)
		for (int nScanCode = 0; nScanCode < 256; nScanCode++)
			if (m_ScanCodeMap[hkeyType][nID][nScanCode].nPrefix !=
					m_CurrentScanCodeMap[hkeyType][nID][nScanCode].nPrefix ||
					m_ScanCodeMap[hkeyType][nID][nScanCode].nScanCode !=
					m_CurrentScanCodeMap[hkeyType][nID][nScanCode].nScanCode)
				return TRUE;
	return FALSE;
}

int CKeyboardLayout::GetPointedKey(const CPoint point)
{
	for (int i = 0; i < MAX_KEYBOARD_LAYOUT; ++i) {
		if (!GetDlgItem(m_KeyboardLayouts[i].nBaseControlID) ||
				!GetDlgItem(m_KeyboardLayouts[i].nCurrentControlID))
			continue;
		CRect rButton;
		GetDlgItem(m_KeyboardLayouts[i].nBaseControlID)->GetWindowRect(&rButton);
		if (rButton.PtInRect(point))
			return m_KeyboardLayouts[i].nBaseControlID;
		GetDlgItem(m_KeyboardLayouts[i].nCurrentControlID)->GetWindowRect(&rButton);
		if (rButton.PtInRect(point))
			return m_KeyboardLayouts[i].nCurrentControlID;
	}
	return 0;
}

int CKeyboardLayout::GetToolTipID(int nToolTipID)
{
	return nToolTipID;
}
