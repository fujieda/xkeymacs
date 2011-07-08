// PropertiesAdvanced.cpp : implementation file
//

#include "stdafx.h"
#include "xkeymacs.h"
#include "DotXkeymacs.h"
#include "Profile.h"
#include "Properties.h"
#include "PropertiesAdvanced.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertiesAdvanced property page
int	CPropertiesAdvanced::m_nApplicationID = 0;
int CPropertiesAdvanced::m_nCommandID = 0;
CEdit * CPropertiesAdvanced::m_pNewKey = NULL;
CButton * CPropertiesAdvanced::m_pAssign = NULL;
CStatic * CPropertiesAdvanced::m_pCurrentlyAssigned = NULL;
CListBox * CPropertiesAdvanced::m_pCurrentKeys = NULL;
HHOOK CPropertiesAdvanced::m_hKeyboardHook = NULL;
BOOL CPropertiesAdvanced::m_bC_x = FALSE;
BOOL CPropertiesAdvanced::m_bC = TRUE;
BOOL CPropertiesAdvanced::m_bM = TRUE;
BOOL CPropertiesAdvanced::m_bShift = TRUE;
int CPropertiesAdvanced::m_nAssignCommandType = 0;
int CPropertiesAdvanced::m_nAssignKey = 0;
int CPropertiesAdvanced::m_nCommandIDs[MAX_COMMAND_TYPE][MAX_KEY] = {'\0'};

IMPLEMENT_DYNCREATE(CPropertiesAdvanced, CPropertyPage)

CPropertiesAdvanced::CPropertiesAdvanced() : CPropertyPage(CPropertiesAdvanced::IDD)
{
	//{{AFX_DATA_INIT(CPropertiesAdvanced)
	m_bEnableCUA = FALSE;
	//}}AFX_DATA_INIT
}

CPropertiesAdvanced::~CPropertiesAdvanced()
{
}

void CPropertiesAdvanced::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertiesAdvanced)
	DDX_Control(pDX, IDC_CURRENTLY_ASSIGNED, m_cCurrentlyAssigned);
	DDX_Control(pDX, IDC_RESET_ALL, m_cResetAll);
	DDX_Control(pDX, IDC_REMOVE, m_cRemove);
	DDX_Control(pDX, IDC_NEW_KEY, m_cNewKey);
	DDX_Control(pDX, IDC_DESCRIPTION, m_cDescription);
	DDX_Control(pDX, IDC_CURRENT_KEYS, m_cCurrentKeys);
	DDX_Control(pDX, IDC_COMMANDS, m_cCommands);
	DDX_Control(pDX, IDC_CATEGORY, m_cCategory);
	DDX_Control(pDX, IDC_ASSIGN, m_cAssign);
	DDX_Check(pDX, IDC_ENABLE_CUA, m_bEnableCUA);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertiesAdvanced, CPropertyPage)
	//{{AFX_MSG_MAP(CPropertiesAdvanced)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_CATEGORY, OnSelchangeCategory)
	ON_LBN_SELCHANGE(IDC_COMMANDS, OnSelchangeCommands)
	ON_LBN_SELCHANGE(IDC_CURRENT_KEYS, OnSelchangeCurrentKeys)
	ON_EN_SETFOCUS(IDC_NEW_KEY, OnSetfocusNewKey)
	ON_BN_CLICKED(IDC_ASSIGN, OnAssign)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_RESET_ALL, OnResetAll)
	ON_BN_CLICKED(IDC_C_X, OnCX)
	ON_EN_KILLFOCUS(IDC_NEW_KEY, OnKillfocusNewKey)
	ON_BN_CLICKED(IDC_ENABLE_CUA, OnEnableCua)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesAdvanced message handlers

BOOL CPropertiesAdvanced::OnSetActive() 
{
	m_pProperties->EnableControl(ADVANCED_TAB);
	SetDialogData(m_pProperties->GetCurrentApplication());

	return CPropertyPage::OnSetActive();
}

void CPropertiesAdvanced::SetDialogData(CString szApplicationName)
{
	UpdateDialogData(szApplicationName, FALSE);
}

void CPropertiesAdvanced::UpdateDialogData(CString szApplicationName, BOOL bSaveAndValidate)
{
	m_nApplicationID = m_pProperties->GetApplicationID();

	if (bSaveAndValidate) {	// GetDialogData
		UpdateData();
	}

	if (m_nApplicationID == MAX_APP) {	// FIXME
		return;
	}

	InitCommandIDs();
	m_bEnableCUA = CProfile::GetEnableCUA(m_nApplicationID);

	if (!bSaveAndValidate) {	// SetDialogData
		SetCurrentKeys();
	}

	if (!bSaveAndValidate) {	// SetDialogData
		UpdateData(FALSE);
	}
}

void CPropertiesAdvanced::GetDialogData()
{
	UpdateDialogData(m_pProperties->GetCurrentApplication(), TRUE);
}

int CPropertiesAdvanced::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pProperties = (CProperties *)GetParent()->GetParent();

	return 0;
}

BOOL CPropertiesAdvanced::OnKillActive() 
{
	GetDialogData();

	return CPropertyPage::OnKillActive();
}

void CPropertiesAdvanced::EnableControl()
{
	BOOL bEnable = m_pProperties->IsEnableControl();

	m_cCategory.EnableWindow(bEnable);
	m_cCommands.EnableWindow(bEnable);
	m_cCurrentKeys.EnableWindow(bEnable);
	m_cNewKey.EnableWindow(bEnable);

	m_cAssign.EnableWindow(FALSE);
	m_cRemove.EnableWindow(FALSE);
	m_cResetAll.EnableWindow(bEnable);
}

BOOL CPropertiesAdvanced::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_nApplicationID = m_pProperties->GetApplicationID();
	InitCategoryList();
	SetCommands();

	SetDialogData(m_pProperties->GetCurrentApplication());

	m_cAssign.EnableWindow(FALSE);
	m_cRemove.EnableWindow(FALSE);

	m_cCurrentlyAssigned.SetWindowText(_T(""));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertiesAdvanced::InitCategoryList()
{
	int nCategoryIDs[128] = {'\0'};

	// get all category type
	m_cCategory.ResetContent();
	for (int nComID = 1; nComID < MAX_COMMAND; ++nComID) {
		CString szCommandName = CCommands::GetCommandName(nComID);
		if (szCommandName.IsEmpty()) {
			break;
		}
		int nCategoryID = CCommands::GetCategoryID(nComID);
		if (!nCategoryID) {
			continue;
		}

		BOOL bExist = FALSE;
		int i;
		for (i = 0; nCategoryIDs[i]; ++i) {
			if (nCategoryIDs[i] == nCategoryID) {
				bExist = TRUE;
			}
		}
		if (!bExist) {
			nCategoryIDs[i] = nCategoryID;
		}
	}

	// sort category type in combo box
	int nMinimumID = 0;
	for (int i = 0; nCategoryIDs[i]; ++i) {
		int nID = 0;
		for (int j = 0; nCategoryIDs[j]; ++j) {
			if (nMinimumID < nCategoryIDs[j]
			 && (nID == 0 || nCategoryIDs[j] < nID)) {
				nID = nCategoryIDs[j];
			}
		}
		m_cCategory.InsertString(-1, CString(MAKEINTRESOURCE(nID)));
		nMinimumID = nID;
	}

	if (CDotXkeymacs::GetFunctionNumber()) {
		m_cCategory.InsertString(-1, CString(MAKEINTRESOURCE(IDS_ORIGINAL)));
	}

	m_cCategory.SetCurSel(0);
}

void CPropertiesAdvanced::SetCommands()
{
	m_cCommands.ResetContent();

	CString szCategory;
	m_cCategory.GetLBText(m_cCategory.GetCurSel(), szCategory);
	if (szCategory.Compare(CString(MAKEINTRESOURCE(IDS_ORIGINAL)))) {
		for (int nComID = 1; nComID < MAX_COMMAND; ++nComID) {
			CString szCommandName = CCommands::GetCommandName(nComID);
			if (szCommandName.IsEmpty()) {
				break;
			}

			if (szCategory == CString(MAKEINTRESOURCE(CCommands::GetCategoryID(nComID)))) {
				m_cCommands.AddString(szCommandName);
			}
		}
	} else {
		for (int nFuncID = 0; nFuncID < CDotXkeymacs::GetFunctionNumber(); ++nFuncID) {
			BOOL bOriginal = TRUE;
			for (int nComID = 1; nComID < MAX_COMMAND; ++nComID) {
				CString szCommandName = CCommands::GetCommandName(nComID);
				if (szCommandName.IsEmpty()) {
					break;
				}

				if (szCommandName == CDotXkeymacs::GetFunctionSymbol(nFuncID)) {
					bOriginal = FALSE;	// overwriting build-in keybindings
					break;
				}
			}
			if (bOriginal) {
				m_cCommands.AddString(CDotXkeymacs::GetFunctionSymbol(nFuncID));
			}
		}
	}

	m_cCommands.SetCurSel(0);
	SetCurrentKeys();
}

void CPropertiesAdvanced::SetCurrentKeys()
{
	m_cCurrentKeys.ResetContent();
	m_cRemove.EnableWindow(FALSE);

	CString szCategory;
	m_cCategory.GetLBText(m_cCategory.GetCurSel(), szCategory);

	CString szCurrentCommandName;
	m_cCommands.GetText(m_cCommands.GetCurSel(), szCurrentCommandName);

	if (szCategory.Compare(CString(MAKEINTRESOURCE(IDS_ORIGINAL)))) {
		for (int nType = 0; nType < MAX_COMMAND_TYPE; ++nType) {
			for (int nKey = 0; nKey < MAX_KEY; ++nKey) {
				if (CCommands::GetCommandName(CProfile::GetCommandID(m_nApplicationID, nType, nKey)) == szCurrentCommandName) {
					CString sz;
					sz.Format(_T("%s%s"), CProfile::CommandType2String(nType), CProfile::Key2String(nKey));
					m_cCurrentKeys.AddString(sz);
				}
			}
		}

		CString szCommandName;
		for (int nComID = 0; nComID < MAX_COMMAND; ++nComID) {
			szCommandName = CCommands::GetCommandName(nComID);
			if (szCommandName.IsEmpty()) {
				break;
			}
			if (szCommandName == szCurrentCommandName) {
				m_nCommandID = nComID;
				break;
			}
		}

		m_cDescription.SetWindowText(CString(MAKEINTRESOURCE(CCommands::GetDescriptionID(m_nCommandID))));

		// overwrite by original command's description if needed
		for (int nFuncID = 0; nFuncID < CDotXkeymacs::GetFunctionNumber(); ++nFuncID) {
			if (szCommandName == CDotXkeymacs::GetFunctionSymbol(nFuncID)) {
				m_cDescription.SetWindowText(CDotXkeymacs::GetFunctionDefinition(szCurrentCommandName));
				break;
			}
		}
	} else {
		const int nIndex = CDotXkeymacs::GetIndex(szCurrentCommandName);
		for (int nKeyID = 0; nKeyID < CDotXkeymacs::GetKeyNumber(nIndex, m_nApplicationID); ++nKeyID) {
			int nType = 0;
			int nKey = 0;
			CDotXkeymacs::GetKey(nIndex, m_nApplicationID, nKeyID, &nType, &nKey);

			CString sz;
			sz.Format(_T("%s%s"), CProfile::CommandType2String(nType), CProfile::Key2String(nKey));
			m_cCurrentKeys.AddString(sz);
		}

		m_cDescription.SetWindowText(CDotXkeymacs::GetFunctionDefinition(szCurrentCommandName));
	}
}

void CPropertiesAdvanced::OnSelchangeCategory() 
{
	SetCommands();
	ClearNewKey();
}

void CPropertiesAdvanced::OnSelchangeCommands() 
{
	BOOL bEnableWindow = FALSE;
	CString szCurrentCommandName;
	m_cCommands.GetText(m_cCommands.GetCurSel(), szCurrentCommandName);
	if (szCurrentCommandName.CompareNoCase(_T("C-"))
	 && szCurrentCommandName.CompareNoCase(_T("Meta for Alt"))
	 && szCurrentCommandName.CompareNoCase(_T("Pass Through"))) {
		bEnableWindow = TRUE;
	}
	GetDlgItem(IDC_C_X)->EnableWindow(bEnableWindow);
	m_bC = bEnableWindow;
	m_bM = bEnableWindow;
	m_bShift = bEnableWindow;

	SetCurrentKeys();
	ClearNewKey();
}

void CPropertiesAdvanced::OnSelchangeCurrentKeys() 
{
	TCHAR szKeyBind[128] = {'\0'};
	m_cCurrentKeys.GetText(m_cCurrentKeys.GetCurSel(), szKeyBind);
	CProfile::ReadKeyBind(m_nRemoveCommandType, m_nRemoveKey, szKeyBind);
	m_cRemove.EnableWindow();
}

void CPropertiesAdvanced::OnSetfocusNewKey() 
{
	m_pNewKey = &m_cNewKey;
	m_pAssign = &m_cAssign;
	m_pCurrentlyAssigned = &m_cCurrentlyAssigned;
	m_pCurrentKeys = &m_cCurrentKeys;
	m_hKeyboardHook = ::SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)CPropertiesAdvanced::KeyboardProc, NULL, AfxGetApp()->m_nThreadID);

	m_bC_x = IsDlgButtonChecked(IDC_C_X) && GetDlgItem(IDC_C_X)->IsWindowEnabled();
}

void CPropertiesAdvanced::OnAssign() 
{
	// Remove Current Setting
	CProfile::SetCommandID(m_nApplicationID, m_nAssignCommandType, m_nAssignKey, 0);
	SetCommandID(m_nAssignCommandType, m_nAssignKey, 0);
	CDotXkeymacs::RemoveKey(m_nApplicationID, m_nAssignCommandType, m_nAssignKey);

	// Assign New Setting
	CString szItem;
	szItem.Format(_T("%s%s"), CProfile::CommandType2String(m_nAssignCommandType), CProfile::Key2String(m_nAssignKey));
	if (m_cCurrentKeys.FindString(-1, szItem) == LB_ERR) {	// This key bind has not assignd to the same command yet.
		CString szCategory;
		m_cCategory.GetLBText(m_cCategory.GetCurSel(), szCategory);

		if (szCategory.Compare(CString(MAKEINTRESOURCE(IDS_ORIGINAL)))) {
			CProfile::SetCommandID(m_nApplicationID, m_nAssignCommandType, m_nAssignKey, m_nCommandID);
			SetCommandID(m_nAssignCommandType, m_nAssignKey, m_nCommandID);
		} else {
			CString szCurrentCommandName;
			m_cCommands.GetText(m_cCommands.GetCurSel(), szCurrentCommandName);
			CDotXkeymacs::SetKey(CDotXkeymacs::GetIndex(szCurrentCommandName), m_nApplicationID, m_nAssignCommandType, m_nAssignKey);
		}
		m_cCurrentKeys.AddString(szItem);
	}

	ClearNewKey();
}

void CPropertiesAdvanced::OnRemove() 
{
	CString szCategory;
	m_cCategory.GetLBText(m_cCategory.GetCurSel(), szCategory);

	if (szCategory.Compare(CString(MAKEINTRESOURCE(IDS_ORIGINAL)))) {
		CProfile::SetCommandID(m_nApplicationID, m_nRemoveCommandType, m_nRemoveKey, 0);
		SetCommandID(m_nRemoveCommandType, m_nRemoveKey, 0);
	} else {
		CString szCurrentCommandName;
		m_cCommands.GetText(m_cCommands.GetCurSel(), szCurrentCommandName);
		CDotXkeymacs::RemoveKey(CDotXkeymacs::GetIndex(szCurrentCommandName), m_nApplicationID, m_nRemoveCommandType, m_nRemoveKey);
	}

	m_cCurrentKeys.DeleteString(m_cCurrentKeys.GetCurSel());
	m_cRemove.EnableWindow(FALSE);
	m_nRemoveCommandType = 0;
	m_nRemoveKey = 0;
}

void CPropertiesAdvanced::OnResetAll() 
{
	CProfile::LoadData();
	InitCommandIDs();
	SetCurrentKeys();
}

void CPropertiesAdvanced::ClearNewKey()
{
	m_cNewKey.SetWindowText(_T(""));
	m_cAssign.EnableWindow(FALSE);
	m_nAssignCommandType = 0;
	m_nAssignKey = 0;
	m_cCurrentlyAssigned.SetWindowText(_T(""));
}

LRESULT CALLBACK CPropertiesAdvanced::KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0 || code == HC_NOREMOVE) {
		return ::CallNextHookEx(m_hKeyboardHook, code, wParam, lParam);
	}

	if (lParam & BEING_RELEASED) {	// Key Up
	} else {						// Key Down
		switch (wParam) {
		case VK_CONTROL:
			if (lParam & EXTENDED_KEY) {
				wParam = VK_RCONTROL;
			} else {
				wParam = VK_LCONTROL;
			}
			break;
		case VK_MENU:
			if (lParam & EXTENDED_KEY) {
				wParam = VK_RMENU;
			} else {
				wParam = VK_LMENU;
			}
			break;
		case VK_SHIFT:
			if (lParam & EXTENDED_KEY) {
				wParam = VK_RSHIFT;
			} else {
				wParam = VK_LSHIFT;
			}
			break;
		default:
			break;
		}
		m_nAssignKey = wParam;
		SetNewKey();
	}

	return TRUE;
}

void CPropertiesAdvanced::SetNewKey()
{
	CString szNewKey;
	int nType = NONE;

	if (m_bC_x) {
		szNewKey += _T("Ctrl+X ");
		nType += CONTROLX;
	}
	if (IsCtrlDown()) {
		szNewKey += _T("Ctrl+");
		nType += CONTROL;
	}
	if (IsMetaDown()) {
		szNewKey += _T("Meta+");
		nType += META;
	}
	if (IsShiftDown()) {
		szNewKey += _T("Shift+");
		nType += SHIFT;
	}

	m_nAssignCommandType = nType;

	szNewKey += CProfile::Key2String(m_nAssignKey);
	if (m_pNewKey) {
		m_pNewKey->SetWindowText(szNewKey);
	}

	if (m_pAssign) {
		BOOL bEnable = TRUE;
		if (m_pCurrentKeys
		 && m_pCurrentKeys->FindString(-1, szNewKey) != LB_ERR) {	// This key bind is already assigned.
			bEnable = FALSE;
		}
		m_pAssign->EnableWindow(bEnable);
	}

	if (m_pCurrentlyAssigned) {
		CString szCurrentlyAssigned(_T("Currently assigned to:\n"));

		if (m_nCommandIDs[m_nAssignCommandType][m_nAssignKey] || CDotXkeymacs::GetIndex(m_nApplicationID, m_nAssignCommandType, m_nAssignKey) == -1) {
			szCurrentlyAssigned += CCommands::GetCommandName(m_nCommandIDs[m_nAssignCommandType][m_nAssignKey]);
		} else {
			szCurrentlyAssigned += CDotXkeymacs::GetFunctionSymbol(CDotXkeymacs::GetIndex(m_nApplicationID, m_nAssignCommandType, m_nAssignKey));
		}
		m_pCurrentlyAssigned->SetWindowText(szCurrentlyAssigned);
	}
}

void CPropertiesAdvanced::SetCommandID(int nType, int nKey, int nComID)
{
	m_nCommandIDs[nType][nKey] = nComID;

	// Set C-x if it is needed.
	if ((nType & CONTROLX)) {
		// Get CommandID of C-x.
		int nCommandIDofC_x;
		for (nCommandIDofC_x = 0; nCommandIDofC_x < MAX_COMMAND; ++nCommandIDofC_x) {
			if (!_tcsicmp(CCommands::GetCommandName(nCommandIDofC_x), _T("C-x"))) {
				break;
			}
		}

		if (nComID) {
			m_nCommandIDs[CONTROL]['X'] = nCommandIDofC_x;
		} else {
			for (int i = 0; i < MAX_COMMAND_TYPE; ++i) {	// i is command-type.
				if (!(i & CONTROLX)) {
					continue;
				}
				for (int j = 0; j < MAX_KEY; ++j) {		// j is key.
					if (m_nCommandIDs[i][j]) {
						m_nCommandIDs[CONTROL]['X'] = nCommandIDofC_x;	// needless
						return;
					}
				}
			}
			m_nCommandIDs[CONTROL]['X'] = 0;
		}
	}
}

void CPropertiesAdvanced::InitCommandIDs()
{
	for (int nType = 0; nType < MAX_COMMAND_TYPE; ++nType) {
		for (int nKey = 0; nKey < MAX_KEY; ++nKey) {
			SetCommandID(nType, nKey, CProfile::GetCommandID(m_nApplicationID, nType, nKey));
		}
	}
}

void CPropertiesAdvanced::OnCX() 
{
	if (m_nAssignKey) {
		OnSetfocusNewKey();
		SetNewKey();
	}
}

void CPropertiesAdvanced::OnKillfocusNewKey() 
{
	if (m_hKeyboardHook) {
		::UnhookWindowsHookEx(m_hKeyboardHook);
	}
	m_hKeyboardHook = NULL;
}

void CPropertiesAdvanced::OnEnableCua() 
{
	UpdateData();
	CProfile::SetEnableCUA(m_nApplicationID, m_bEnableCUA);
}

BOOL CPropertiesAdvanced::IsCtrlDown()
{
	return m_bC && IsFooDown(CString(_T("C-")));
}

BOOL CPropertiesAdvanced::IsMetaDown()
{
	return m_bM && IsFooDown(CString(_T("Meta for Alt")));
}

BOOL CPropertiesAdvanced::IsShiftDown()
{
	// temporary code
	return m_bShift
		&& (GetKeyState(VK_SHIFT ) < 0
		 || GetKeyState(VK_LSHIFT) < 0
		 || GetKeyState(VK_RSHIFT) < 0);

//	return m_bShift && IsFooDown(CString("Foo"));
}

BOOL CPropertiesAdvanced::IsFooDown(CString szCommandName)
{
	for (int nKey = 0; nKey < MAX_KEY; ++nKey) {
		if (CCommands::GetCommandName(CProfile::GetCommandID(0, NONE, nKey)) == szCommandName) {	// FIXME
			if (GetKeyState(nKey) < 0) {
				return TRUE;
			}
		}
	}
	return FALSE;
}
