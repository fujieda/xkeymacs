// PropertiesAdvanced.cpp : implementation file
//

#include "propertiesadvanced.h"
#include "resource.h"
#include "profile.h"
#include "FuncDefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertiesAdvanced property page
int	CPropertiesAdvanced::m_nAppID = 0;
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
	SetDialogData();

	return CPropertyPage::OnSetActive();
}

void CPropertiesAdvanced::SetDialogData()
{
	m_nAppID = m_pProperties->GetApplicationID();
	if (m_nAppID == MAX_APP)
		return;
	InitCommandIDs();
	m_bEnableCUA = CProfile::GetEnableCUA(m_nAppID);
	SetCurrentKeys();
	UpdateData(FALSE);
}

void CPropertiesAdvanced::GetDialogData()
{
	UpdateData();
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

	m_nAppID = m_pProperties->GetApplicationID();
	InitCategoryList();
	SetCommands();

	SetDialogData();

	m_cAssign.EnableWindow(FALSE);
	m_cRemove.EnableWindow(FALSE);

	m_cCurrentlyAssigned.SetWindowText(_T(""));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertiesAdvanced::InitCategoryList()
{
	for (const int *c = CmdTable::Categories(); *c; ++c)
		m_cCategory.InsertString(-1, CString(MAKEINTRESOURCE(*c)));
	if (FuncDefs::GetNumOfDefs())
		m_cCategory.InsertString(-1, CString(MAKEINTRESOURCE(IDS_ORIGINAL)));
	m_cCategory.SetCurSel(0);
}

void CPropertiesAdvanced::SetCommands()
{
	m_cCommands.ResetContent();

	CString category;
	m_cCategory.GetLBText(m_cCategory.GetCurSel(), category);
	if (category.Compare(CString(MAKEINTRESOURCE(IDS_ORIGINAL)))) {
		for (int cmd = 1; cmd < MAX_COMMAND; ++cmd)
			if (category == CString(MAKEINTRESOURCE(CmdTable::CategoryID(cmd))))
				m_cCommands.AddString(CmdTable::Name(cmd));
	} else {
		for (int nFuncID = 0; nFuncID < FuncDefs::GetNumOfDefs(); ++nFuncID)
			m_cCommands.AddString(FuncDefs::GetName(nFuncID));
	}

	m_cCommands.SetCurSel(0);
	SetCurrentKeys();
}

void CPropertiesAdvanced::SetCurrentKeys()
{
	m_cCurrentKeys.ResetContent();
	m_cRemove.EnableWindow(FALSE);

	CString category;
	m_cCategory.GetLBText(m_cCategory.GetCurSel(), category);
	CString name;
	m_cCommands.GetText(m_cCommands.GetCurSel(), name);

	if (category.Compare(CString(MAKEINTRESOURCE(IDS_ORIGINAL)))) {
		for (int type = 0; type < MAX_COMMAND_TYPE; ++type) {
			for (int key = 0; key < MAX_KEY; ++key)
				if (!_tcscmp(CmdTable::Name(CProfile::GetCmdID(m_nAppID, type, key)), name))
					m_cCurrentKeys.AddString(CProfile::KeyToString(type, key));
		}
		for (int cmd = 0; cmd < MAX_COMMAND; ++cmd) {
			if (!_tcscmp(CmdTable::Name(cmd), name)) {
				m_nCommandID = cmd;
				break;
			}
		}
		m_cDescription.SetWindowText(CString(MAKEINTRESOURCE(CmdTable::DescriptionID(m_nCommandID))));
	} else {
		int id = FuncDefs::GetID(name);
		if (id < 0)
			return;
		for (int type = 0; type < MAX_COMMAND_TYPE; ++type) {
			for (int key = 0; key < MAX_KEY; ++key)
				if (CProfile::GetFuncID(m_nAppID, type, key) == id)
					m_cCurrentKeys.AddString(CProfile::KeyToString(type, key));
		}
		m_cDescription.SetWindowText(FuncDefs::GetStrDef(id));
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
	CProfile::StringToKey(szKeyBind, m_nRemoveCommandType, m_nRemoveKey);
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
	CProfile::SetCmdID(m_nAppID, m_nAssignCommandType, m_nAssignKey, 0);
	SetCmdID(m_nAssignCommandType, m_nAssignKey, 0);
	CProfile::SetFuncID(m_nAppID, m_nAssignCommandType, m_nAssignKey, -1);

	// Assign New Setting
	CString item = CProfile::KeyToString(m_nAssignCommandType, m_nAssignKey);
	if (m_cCurrentKeys.FindString(-1, item) == LB_ERR) {	// This key bind has not assignd to the same command yet.
		CString category;
		m_cCategory.GetLBText(m_cCategory.GetCurSel(), category);
		if (category.Compare(CString(MAKEINTRESOURCE(IDS_ORIGINAL)))) {
			CProfile::SetCmdID(m_nAppID, m_nAssignCommandType, m_nAssignKey, m_nCommandID);
			SetCmdID(m_nAssignCommandType, m_nAssignKey, m_nCommandID);
		} else {
			CString name;
			m_cCommands.GetText(m_cCommands.GetCurSel(), name);
			int id = FuncDefs::GetID(name);
			if (id >= 0)
				CProfile::SetFuncID(m_nAppID, m_nAssignCommandType, m_nAssignKey, id);
		}
		m_cCurrentKeys.AddString(item);
	}

	ClearNewKey();
}

void CPropertiesAdvanced::OnRemove() 
{
	CString category;
	m_cCategory.GetLBText(m_cCategory.GetCurSel(), category);

	if (category.Compare(CString(MAKEINTRESOURCE(IDS_ORIGINAL)))) {
		CProfile::SetCmdID(m_nAppID, m_nRemoveCommandType, m_nRemoveKey, 0);
		SetCmdID(m_nRemoveCommandType, m_nRemoveKey, 0);
	} else
		CProfile::SetFuncID(m_nAppID, m_nRemoveCommandType, m_nRemoveKey, -1);

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
	if (code < 0 || code == HC_NOREMOVE)
		return ::CallNextHookEx(m_hKeyboardHook, code, wParam, lParam);
	if (HIWORD(lParam) & KF_UP)
		return TRUE;
	const bool bExt = (HIWORD(lParam) & KF_EXTENDED) != 0;
	switch (wParam) {
	case VK_SHIFT:
		wParam = (bExt) ? VK_RSHIFT : VK_LSHIFT;
		break;
	case VK_CONTROL:
		wParam = (bExt) ? VK_RCONTROL : VK_LCONTROL;
		break;
	case VK_MENU:
		wParam = (bExt) ? VK_RMENU : VK_LMENU;
		break;
	}
	m_nAssignKey = wParam;
	SetNewKey();
	return TRUE;
}

void CPropertiesAdvanced::SetNewKey()
{
	UINT type = NONE;
	if (m_bC_x)
		type |= CONTROLX;
	if (IsCtrlDown())
		type |= CONTROL;
	if (IsMetaDown())
		type |= META;
	if (IsShiftDown())
		type |= SHIFT;
	m_nAssignCommandType = type;

	const CString newKey = CProfile::KeyToString(type, m_nAssignKey);
	if (m_pNewKey)
		m_pNewKey->SetWindowText(newKey);
	if (m_pAssign)
		m_pAssign->EnableWindow(!m_pCurrentKeys || m_pCurrentKeys->FindString(-1, newKey) == LB_ERR); // this key isn't bound;
	if (m_pCurrentlyAssigned) {
		CString assigned(_T("Currently assigned to:\n"));
		if (m_nCommandIDs[m_nAssignCommandType][m_nAssignKey] || CProfile::GetFuncID(m_nAppID, m_nAssignCommandType, m_nAssignKey) == -1)
			assigned += CmdTable::Name(m_nCommandIDs[m_nAssignCommandType][m_nAssignKey]);
		else
			assigned += FuncDefs::GetName(CProfile::GetFuncID(m_nAppID, m_nAssignCommandType, m_nAssignKey));
		m_pCurrentlyAssigned->SetWindowText(assigned);
	}
}

void CPropertiesAdvanced::SetCmdID(int type, int key, int cmd)
{
	m_nCommandIDs[type][key] = cmd;
	if (!(type & CONTROLX))
		return;
	// Make C-x enable or disable.
	if (cmd)
		m_nCommandIDs[CONTROL]['X'] = 1;
	else {
		m_nCommandIDs[CONTROL]['X'] = 0;
		for (int i = 0; i < MAX_COMMAND_TYPE; ++i)
			for (int j = 0; j < MAX_KEY; ++j)
				if ((i & CONTROLX) && m_nCommandIDs[i][j])
					m_nCommandIDs[CONTROL]['X'] = 1;
	}
}

void CPropertiesAdvanced::InitCommandIDs()
{
	for (int type = 0; type < MAX_COMMAND_TYPE; ++type)
		for (int key = 0; key < MAX_KEY; ++key)
			SetCmdID(type, key, CProfile::GetCmdID(m_nAppID, type, key));
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
	if (m_hKeyboardHook)
		::UnhookWindowsHookEx(m_hKeyboardHook);
	m_hKeyboardHook = NULL;
}

void CPropertiesAdvanced::OnEnableCua() 
{
	UpdateData();
	CProfile::SetEnableCUA(m_nAppID, m_bEnableCUA != FALSE);
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
	for (int key = 0; key < MAX_KEY; ++key) {
		if (CmdTable::Name(CProfile::GetCmdID(0, NONE, key)) == szCommandName) {	// FIXME
			if (GetKeyState(key) < 0) {
				return TRUE;
			}
		}
	}
	return FALSE;
}
