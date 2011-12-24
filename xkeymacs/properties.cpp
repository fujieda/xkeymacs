// Properties.cpp : implementation file
//

#include "properties.h"
#include "resource.h"
#include "profile.h"
#include "AppList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProperties dialog


CProperties::CProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CProperties::IDD, pParent)
{
	m_nAppID = 0;
	//{{AFX_DATA_INIT(CProperties)
	m_nSettingStyle = -1;
	m_nKillRingMax = 0;
	//}}AFX_DATA_INIT
}


void CProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProperties)
	DDX_Control(pDX, IDC_WINDOW_TEXT, m_cWindowText);
	DDX_Control(pDX, IDC_KILL_RING_MAX, m_cKillRingMax);
	DDX_Control(pDX, IDC_USE_DIALOG_SETTING, m_cUseDialogSetting);
	DDX_Control(pDX, IDC_SELECT_ALL, m_cSelectAll);
	DDX_Control(pDX, IDC_CLEAR_ALL, m_cClearAll);
	DDX_Control(pDX, IDC_LOAD_DEFAULT, m_cLoadDefault);
	DDX_Control(pDX, IDC_APPLICATION, m_cAppList);
	DDX_Radio(pDX, IDC_SETTING_DEFAULT, m_nSettingStyle);
	DDX_Text(pDX, IDC_KILL_RING_MAX, m_nKillRingMax);
	DDV_MinMaxInt(pDX, m_nKillRingMax, 0, 2147483647);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProperties, CDialog)
	//{{AFX_MSG_MAP(CProperties)
	ON_CBN_DROPDOWN(IDC_APPLICATION, OnDropdownApplication)
	ON_CBN_SELCHANGE(IDC_APPLICATION, OnSelchangeApplication)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_LOAD_DEFAULT, OnLoadDefault)
	ON_BN_CLICKED(IDC_SELECT_ALL, OnSelectAll)
	ON_BN_CLICKED(IDC_CLEAR_ALL, OnClearAll)
	ON_BN_CLICKED(IDC_SETTING_DEFAULT, OnSettingDefault)
	ON_BN_CLICKED(IDC_SETTING_DISABLE, OnSettingDisable)
	ON_BN_CLICKED(IDC_SETTING_SPECIFIC, OnSettingSpecific)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProperties message handlers

BOOL CProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	while (m_sheet.GetPageCount()) {
		m_sheet.RemovePage(0);
	}

	m_sheet.AddPage(&m_basic);
	m_sheet.AddPage(&m_advanced);
	m_sheet.AddPage(&m_list);

	m_sheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
	m_sheet.ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_sheet.ModifyStyle( 0, WS_TABSTOP );

	CRect rcSheet;
	GetDlgItem(IDC_TAB)->GetWindowRect(&rcSheet);
	ScreenToClient(&rcSheet);
	m_sheet.SetWindowPos(NULL, rcSheet.left - 11, rcSheet.top - 8, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

	SetForegroundWindow();

	InitAppList();

	CProfile::LoadData();
	m_appName.LoadString(IDS_DEFAULT);
	SetDialogData();
	EnableUseDefaultButton(FALSE);
	EnableLoadDefaultButton(FALSE);

	EnableControl(BASIC_TAB);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProperties::OnDropdownApplication() 
{
	InitAppList();
	int i;
	for (i = 0; i < m_cAppList.GetCount(); ++i) {
		CString appTitle, appName;
		GetAppTitleAndName(i, appTitle, appName);
		if (m_appName == appName)
			m_cAppList.SetCurSel(i);
	}
	if (i == m_cAppList.GetCount()) {
		m_appTitle.LoadString(IDS_DEFAULT_TITLE);
		m_cAppList.SelectString(-1, m_appTitle);
		SetDialogData();
	}
}

void CProperties::OnSelchangeApplication() 
{
	GetAppTitleAndName(m_cAppList.GetCurSel(), m_appTitle, m_appName);
	SetDialogData();
	EnableControl(ACTIVE_TAB);
}

void CProperties::InitAppList()
{
	m_cAppList.ResetContent();
	AppList::SetAppList(*this);
	// Add Dialog
	m_cAppList.InsertString(0, CString(MAKEINTRESOURCE(IDS_DIALOG_TITLE)));
	// Add Default
	m_cAppList.InsertString( 0, CString(MAKEINTRESOURCE(IDS_DEFAULT_TITLE)));
	m_cAppList.SelectString(-1, CString(MAKEINTRESOURCE(IDS_DEFAULT_TITLE)));
}

void CProperties::GetDialogData()
{
	UpdateData();
	if (m_nSettingStyle == SETTING_DEFAULT) {
		CProfile::ClearData(m_appName);
		return;
	}
	m_nAppID = CProfile::AssignAppID(m_appName);
	if (m_nAppID == MAX_APP)
		return;
	CProfile::SetSettingStyle(m_nAppID, m_nSettingStyle);
	CProfile::SetAppTitle(m_nAppID, m_appTitle);
	CProfile::SetKillRingMax(m_nAppID, m_nKillRingMax);
	CProfile::SetUseDialogSetting(m_nAppID, m_cUseDialogSetting.GetCheck() == BST_CHECKED);
	CString szWindowText;
	m_cWindowText.GetWindowText(szWindowText);
	CProfile::SetWindowText(m_nAppID, szWindowText);

	if (m_sheet.GetActivePage() == &m_basic) // FIXME
		m_basic.GetDialogData();
	else if (m_sheet.GetActivePage() == &m_advanced)
		m_advanced.GetDialogData();
	else if (m_sheet.GetActivePage() ==  &m_list)
		m_list.GetDialogData();
}

void CProperties::SetDialogData()
{
	m_nAppID = CProfile::GetAppID(m_appName);
	m_nSettingStyle = CProfile::GetSettingStyle(m_nAppID);
	if (m_nAppID == MAX_APP)
		m_nAppID = CProfile::DefaultAppID(); // fall back to the default
	EnableUseDefaultButton(m_appName != CString(MAKEINTRESOURCE(IDS_DEFAULT)) &&
							m_appName != CString(MAKEINTRESOURCE(IDS_DIALOG)));
	m_nKillRingMax = CProfile::GetKillRingMax(m_nAppID);
	m_cUseDialogSetting.SetCheck(CProfile::GetUseDialogSetting(m_nAppID) ? BST_CHECKED : BST_UNCHECKED);
	m_cWindowText.SetWindowText(CProfile::GetWindowText(m_nAppID));
	UpdateData(FALSE);

	if (m_sheet.GetActivePage() == &m_basic) // FIXME
		m_basic.SetDialogData();
	else if (m_sheet.GetActivePage() == &m_advanced)
		m_advanced.SetDialogData();
	else if (m_sheet.GetActivePage() ==  &m_list)
		m_list.SetDialogData();
}

void CProperties::AddItem(const CString& appTitle, const CString& appName)
{
	for (int i = 0; i < m_cAppList.GetCount(); ++i) {
		CString tmpTitle, tmpName;
		GetAppTitleAndName(i, tmpTitle, tmpName);
		if (appName == tmpName)
			return;
	}
	CString item;
	item.Format(IDS_APPLICATION_LIST_ITEM, appTitle, appName);
	m_cAppList.AddString(item);
}

void CProperties::GetAppTitleAndName(const int nSel, CString& appTitle, CString& appName)
{
	appTitle.Empty();
	appName.Empty();
	CString item;
	if (nSel < 0)
		m_cAppList.GetWindowText(item);
	else
		m_cAppList.GetLBText(nSel, item);

	if (item == CString(MAKEINTRESOURCE(IDS_DEFAULT_TITLE))) {
		appName.LoadString(IDS_DEFAULT);
		return;
	}
	if (item == CString(MAKEINTRESOURCE(IDS_DIALOG_TITLE))) {
		appName.LoadString(IDS_DIALOG);
		return;
	}
	int nParen = item.ReverseFind(_T('('));
	if (nParen < 0)
		return;
	appTitle = item.Left(nParen);
	appName = item.Mid(nParen + 1, item.GetLength() - nParen -2);
}

void CProperties::EnableControl(TAB_NAME tab)
{
	if (tab & ACTIVE_TAB) {
		if (m_sheet.GetActivePage() == &m_basic) {	// FIXME
			tab = BASIC_TAB;
		} else if (m_sheet.GetActivePage() == &m_advanced) {
			tab = ADVANCED_TAB;
		} else if (m_sheet.GetActivePage() == &m_list) {
			tab = LIST_TAB;
		}
	}

	BOOL bEnable = (m_nSettingStyle == SETTING_SPECIFIC);

	m_cKillRingMax.EnableWindow(bEnable);
	EnableLoadDefaultButton(bEnable);

	m_cSelectAll.EnableWindow(bEnable && tab & BASIC_TAB);
	m_cClearAll.EnableWindow(bEnable && tab & BASIC_TAB);

	EnableUseDialogSettingButton(bEnable);

	m_cWindowText.EnableWindow(bEnable);

	if (tab & BASIC_TAB) {
		m_basic.EnableControl();
	}
	if (tab & ADVANCED_TAB) {
		m_advanced.EnableControl();
	}
	if (tab & LIST_TAB) {
		m_list.EnableControl();
	}
}

void CProperties::EnableUseDialogSettingButton(BOOL bEnable)
{
	m_cUseDialogSetting.EnableWindow(bEnable);
	if (m_appName == CString(MAKEINTRESOURCE(IDS_DIALOG)))
		m_cUseDialogSetting.EnableWindow(FALSE);
}

void CProperties::EnableLoadDefaultButton(BOOL bEnable)
{
	m_cLoadDefault.EnableWindow(bEnable);
	if (m_appName == CString(MAKEINTRESOURCE(IDS_DEFAULT)))
		m_cLoadDefault.EnableWindow(FALSE);
}

void CProperties::EnableUseDefaultButton(BOOL bEnable)
{
	if (GetDlgItem(IDC_SETTING_DEFAULT)) {
		GetDlgItem(IDC_SETTING_DEFAULT)->EnableWindow(bEnable);
	}
}

void CProperties::OnOK() 
{
	OnApply();

	CDialog::OnOK();
}

void CProperties::OnCancel() 
{
	CProfile::LoadData();

	CDialog::OnCancel();
}

void CProperties::OnApply() 
{
	GetDialogData();
	CProfile::SaveData();
}

void CProperties::OnLoadDefault() 
{
	CProfile::CopyDefault(m_appName);
	SetDialogData();
}

void CProperties::OnSelectAll() 
{
	m_basic.SetAllDialogData(1, TRUE);
}

void CProperties::OnClearAll() 
{
	m_basic.SetAllDialogData(0, TRUE);
}

int CProperties::GetApplicationID()
{
	return m_nAppID;
}

CString CProperties::GetCurrentApplication()
{
	return m_appName;
}

void CProperties::OnSettingDefault() 
{
	UpdateData(); // get data
	EnableControl(ACTIVE_TAB);
}

void CProperties::OnSettingDisable() 
{
	UpdateData();
	EnableControl(ACTIVE_TAB);
}

void CProperties::OnSettingSpecific() 
{
	UpdateData();
	EnableControl(ACTIVE_TAB);
	m_nAppID = CProfile::AssignAppID(m_appName);
}

BOOL CProperties::IsEnableControl()
{
	return m_nSettingStyle == SETTING_SPECIFIC;
}
