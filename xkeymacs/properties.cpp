// Properties.cpp : implementation file
//

#include "stdafx.h"
#include "xkeymacs.h"
#include "Profile.h"
#include "Properties.h"

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
	m_nApplicationID = 0;
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
	DDX_Control(pDX, IDC_APPLICATION, m_cApplication);
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

	InitApplicationList();

	CProfile::LoadRegistryData();
	m_szCurrentApplication.LoadString(IDS_DEFAULT);
	SetDialogData(m_szCurrentApplication);
	EnableUseDefaultButton(FALSE);
	EnableLoadDefaultButton(FALSE);

	EnableControl(BASIC_TAB);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProperties::OnDropdownApplication() 
{
	InitApplicationList();

	int nID = GetCurrentApplicationID();
	if (nID != CB_ERR) {
		m_cApplication.SetCurSel(nID);
	} else {
		GetDialogData();
		m_szCurrentApplication.LoadString(IDS_DEFAULT_TITLE);
		m_cApplication.SelectString(-1, m_szCurrentApplication);
		SetDialogData(m_szCurrentApplication);
	}
}

void CProperties::OnSelchangeApplication() 
{
	GetDialogData();

	CProfile::GetApplicationTitle(&m_cApplication, m_szCurrentApplication, m_cApplication.GetCurSel());
	int nStart;
	if ((nStart = m_szCurrentApplication.ReverseFind(_T('('))) != -1) {
		++nStart;
		m_szCurrentApplication =
			m_szCurrentApplication.Mid(nStart, m_szCurrentApplication.GetLength() - nStart - 1);
	}

	SetDialogData(m_szCurrentApplication);
	EnableControl(ACTIVE_TAB);
}

void CProperties::InitApplicationList()
{
	CProfile::InitApplicationList(&m_cApplication);
}

int CProperties::GetCurrentApplicationID()
{
	return CProfile::GetCurrentApplicationID(&m_cApplication, m_szCurrentApplication);
}

void CProperties::GetDialogData()
{
	if (m_nSettingStyle == SETTING_DEFAULT) {
		CProfile::ClearData(m_szCurrentApplication);
	} else {
		UpdateDialogData(m_szCurrentApplication, TRUE);

		if (m_sheet.GetActivePage() == &m_basic) {			// FIXME
			m_basic.GetDialogData();
		} else if (m_sheet.GetActivePage() == &m_advanced) {
			m_advanced.GetDialogData();
		} else if (m_sheet.GetActivePage() ==  &m_list) {
			m_list.GetDialogData();
		} else {
			ASSERT(0);
		}
	}
}

void CProperties::SetDialogData(CString szApplicationName)
{
	UpdateDialogData(szApplicationName, FALSE);

	if (m_sheet.GetActivePage() == &m_basic) {			// FIXME
		m_basic.SetDialogData(szApplicationName);
	} else if (m_sheet.GetActivePage() == &m_advanced) {
		m_advanced.SetDialogData(szApplicationName);
	} else if (m_sheet.GetActivePage() ==  &m_list) {
		m_list.SetDialogData(szApplicationName);
	} else {
		ASSERT(0);
	}
}

void CProperties::UpdateDialogData(CString szApplicationName, BOOL bSaveAndValidate)
{
	if (bSaveAndValidate) {	// GetDialogData
		UpdateData();
	}

	m_nApplicationID = CProfile::GetApplicationIndex(szApplicationName, bSaveAndValidate, &m_nSettingStyle);

	if (CProfile::IsDefault(szApplicationName)
	 || CProfile::IsDialog(szApplicationName)) {
		EnableUseDefaultButton(FALSE);
	} else {
		EnableUseDefaultButton(TRUE);
	}

	// application title
	CProfile::UpdateApplicationTitle(&m_cApplication, m_szCurrentApplication, m_nApplicationID, bSaveAndValidate);

	// kill-ring-max
	if (bSaveAndValidate) {	// GetDialogData
		CProfile::SetKillRingMax(m_nApplicationID, m_nKillRingMax);
	} else {				// SetDialogData
		m_nKillRingMax = CProfile::GetKillRingMax(m_nApplicationID);
	}

	// Use Dialog Setting
	if (bSaveAndValidate) {	// GetDialogData
		CProfile::SetUseDialogSetting(m_nApplicationID, m_cUseDialogSetting.GetCheck() == BST_CHECKED);
	} else {				// SetDialogData
		m_cUseDialogSetting.SetCheck(CProfile::GetUseDialogSetting(m_nApplicationID) ? BST_CHECKED : BST_UNCHECKED);
	}

	// Check Window Title
	if (bSaveAndValidate) {	// GetDialogData
		CString szWindowText;
		m_cWindowText.GetWindowText(szWindowText);
		CProfile::SetWindowText(m_nApplicationID, szWindowText);
	} else {				// SetDialogData
		m_cWindowText.SetWindowText(CProfile::GetWindowText(m_nApplicationID));
	}

	if (!bSaveAndValidate) {	// SetDialogData
		UpdateData(FALSE);
	}
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
	if (CProfile::IsDialog(m_szCurrentApplication)) {
		m_cUseDialogSetting.EnableWindow(FALSE);
	}
}

void CProperties::EnableLoadDefaultButton(BOOL bEnable)
{
	m_cLoadDefault.EnableWindow(bEnable);
	if (CProfile::IsDefault(m_szCurrentApplication)) {
		m_cLoadDefault.EnableWindow(FALSE);
	}
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
	CProfile::LoadRegistryData();

	CDialog::OnCancel();
}

void CProperties::OnApply() 
{
	GetDialogData();
	CProfile::SaveRegistryData();
}

void CProperties::OnLoadDefault() 
{
	CProfile::CopyData(m_szCurrentApplication, CString(MAKEINTRESOURCE(IDS_DEFAULT)));
	SetDialogData(m_szCurrentApplication);
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
	return m_nApplicationID;
}

CString CProperties::GetCurrentApplication()
{
	return m_szCurrentApplication;
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

	if (CProfile::GetApplicationIndex(m_szCurrentApplication) == MAX_APP) {
		CProfile::CopyData(m_szCurrentApplication, CString(MAKEINTRESOURCE(IDS_DEFAULT)));
		SetDialogData(m_szCurrentApplication);
	}
}

BOOL CProperties::IsEnableControl()
{
	return m_nSettingStyle == SETTING_SPECIFIC;
}
