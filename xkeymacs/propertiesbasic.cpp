// PropertiesBasic.cpp : implementation file
//

#include "propertiesbasic.h"
#include "resource.h"
#include "profile.h"
#include "dotxkeymacs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertiesBasic property page

IMPLEMENT_DYNCREATE(CPropertiesBasic, CPropertyPage)

CPropertiesBasic::CPropertiesBasic() : CPropertyPage(CPropertiesBasic::IDD)
{
	//{{AFX_DATA_INIT(CPropertiesBasic)
	//}}AFX_DATA_INIT
}

CPropertiesBasic::~CPropertiesBasic()
{
}

BEGIN_MESSAGE_MAP(CPropertiesBasic, CPropertyPage)
	//{{AFX_MSG_MAP(CPropertiesBasic)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_ALT, OnAlt)
	ON_BN_CLICKED(IDC_C_, OnC_)
	ON_BN_CLICKED(IDC_CA, OnCa)
	ON_BN_CLICKED(IDC_CB, OnCb)
	ON_BN_CLICKED(IDC_CBACKSLASH, OnCbackslash)
	ON_BN_CLICKED(IDC_CBACKSLASH_106, OnCbackslash106)
	ON_BN_CLICKED(IDC_CD, OnCd)
	ON_BN_CLICKED(IDC_CDIGIT, OnCdigit)
	ON_BN_CLICKED(IDC_CE, OnCe)
	ON_BN_CLICKED(IDC_CF, OnCf)
	ON_BN_CLICKED(IDC_CG, OnCg)
	ON_BN_CLICKED(IDC_CH, OnCh)
	ON_BN_CLICKED(IDC_CI, OnCi)
	ON_BN_CLICKED(IDC_CK, OnCk)
	ON_BN_CLICKED(IDC_CL, OnCl)
	ON_BN_CLICKED(IDC_CM, OnCm)
	ON_BN_CLICKED(IDC_CMS5, OnCms5)
	ON_BN_CLICKED(IDC_CN, OnCn)
	ON_BN_CLICKED(IDC_CO, OnCo)
	ON_BN_CLICKED(IDC_CO2, OnCo2)
	ON_BN_CLICKED(IDC_CP, OnCp)
	ON_BN_CLICKED(IDC_CQ, OnCq)
	ON_BN_CLICKED(IDC_CR, OnCr)
	ON_BN_CLICKED(IDC_CS, OnCs)
	ON_BN_CLICKED(IDC_CSLASH, OnCslash)
	ON_BN_CLICKED(IDC_CSPACE, OnCspace)
	ON_BN_CLICKED(IDC_CT, OnCt)
	ON_BN_CLICKED(IDC_CU, OnCu)
	ON_BN_CLICKED(IDC_CV, OnCv)
	ON_BN_CLICKED(IDC_CW, OnCw)
	ON_BN_CLICKED(IDC_CXB, OnCxb)
	ON_BN_CLICKED(IDC_CXCC, OnCxcc)
	ON_BN_CLICKED(IDC_CXCF, OnCxcf)
	ON_BN_CLICKED(IDC_CXCJ, OnCxcj)
	ON_BN_CLICKED(IDC_CXCP, OnCxcp)
	ON_BN_CLICKED(IDC_CXCS, OnCxcs)
	ON_BN_CLICKED(IDC_CXCT, OnCxct)
	ON_BN_CLICKED(IDC_CXCW, OnCxcw)
	ON_BN_CLICKED(IDC_CXE, OnCxe)
	ON_BN_CLICKED(IDC_CXH, OnCxh)
	ON_BN_CLICKED(IDC_CXK, OnCxk)
	ON_BN_CLICKED(IDC_CXS0, OnCxs0)
	ON_BN_CLICKED(IDC_CXS9, OnCxs9)
	ON_BN_CLICKED(IDC_CXU, OnCxu)
	ON_BN_CLICKED(IDC_CY, OnCy)
	ON_BN_CLICKED(IDC_CZ, OnCz)
	ON_BN_CLICKED(IDC_LEFT_ALT, OnLeftAlt)
	ON_BN_CLICKED(IDC_MB, OnMb)
	ON_BN_CLICKED(IDC_MD, OnMd)
	ON_BN_CLICKED(IDC_MF, OnMf)
	ON_BN_CLICKED(IDC_MI, OnMi)
	ON_BN_CLICKED(IDC_MS1, OnMs1)
	ON_BN_CLICKED(IDC_MS5, OnMs5)
	ON_BN_CLICKED(IDC_MSCOMMA, OnMscomma)
	ON_BN_CLICKED(IDC_MSPERIOD, OnMsperiod)
	ON_BN_CLICKED(IDC_MV, OnMv)
	ON_BN_CLICKED(IDC_MW, OnMw)
	ON_BN_CLICKED(IDC_MY, OnMy)
	ON_BN_CLICKED(IDC_RIGHT_ALT, OnRightAlt)
	ON_BN_CLICKED(IDC_ESCAPE, OnEscape)
	ON_BN_CLICKED(IDC_SQUARE_BRA, OnSquareBra)
	ON_BN_CLICKED(IDC_MDEL, OnMdel)
	ON_BN_CLICKED(IDC_MF12, OnMf12)
	ON_BN_CLICKED(IDC_MT, OnMt)
	ON_BN_CLICKED(IDC_MC, OnMc)
	ON_BN_CLICKED(IDC_ML, OnMl)
	ON_BN_CLICKED(IDC_MU, OnMu)
	ON_BN_CLICKED(IDC_MX, OnMx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesBasic message handlers

BOOL CPropertiesBasic::OnSetActive() 
{
	m_pProperties->EnableControl(BASIC_TAB);
	SetDialogData();
	return CPropertyPage::OnSetActive();
}

BOOL CPropertiesBasic::OnKillActive() 
{
	GetDialogData();

	return CPropertyPage::OnKillActive();
}

BOOL CPropertiesBasic::PreTranslateMessage(MSG* pMsg) 
{
	m_ToolTip.RelayEvent(pMsg);

	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CPropertiesBasic::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertiesBasic)
	//}}AFX_DATA_MAP
}

void CPropertiesBasic::EnableControl()
{
	BOOL enable = m_pProperties->IsEnableControl();
	CmdTable::EnumCommands(this, &CPropertiesBasic::EnableControlHandler, enable);
}

void CPropertiesBasic::EnableControlHandler(int, const KeyBind& bind, int enable)
{
	if (GetDlgItem(bind.nControlID))
		GetDlgItem(bind.nControlID)->EnableWindow(enable);
}

void CPropertiesBasic::GetDialogData()
{
	UpdateData();
	int appID = m_pProperties->GetApplicationID();
	if (appID == MAX_APP)
		return;
	CmdTable::EnumCommands(this, &CPropertiesBasic::GetDialogDataHandler, appID);
}

void CPropertiesBasic::GetDialogDataHandler(int cmdID, const KeyBind& bind, int appID)
{
	if (!m_Changed[cmdID])
		return;
	if (IsDlgButtonChecked(bind.nControlID)) {
		CProfile::SetCmdID(appID, bind.nType, bind.bVk, cmdID);
		CDotXkeymacs::RemoveKey(appID, bind.nType, bind.bVk);
	} else if (bind.nControlID != IDC_CO2)
		CProfile::SetCmdID(appID, bind.nType, bind.bVk, 0);
}

void CPropertiesBasic::SetDialogData()
{
	SetDefaultBind(BST_CHECKED);
	memset(m_Changed, 0, sizeof(m_Changed));
	CheckDlgButton(IDC_CO2, BST_CHECKED);
	int appID = m_pProperties->GetApplicationID();
	if (appID == MAX_APP)
		return;
	CmdTable::EnumCommands(this, &CPropertiesBasic::SetDialogDataHandler, appID);
	UpdateData(FALSE);
}

void CPropertiesBasic::SetDialogDataHandler(int cmdID, const KeyBind& bind, int appID)
{
	if (cmdID != CProfile::GetCmdID(appID, bind.nType, bind.bVk))
		CheckDlgButton(bind.nControlID, BST_UNCHECKED);
}

void CPropertiesBasic::SetDefaultBind(UINT checked)
{
	CmdTable::EnumCommands(this, &CPropertiesBasic::SetDefaultBindHandler, checked);
	CheckDlgButton(IDC_CO2, BST_UNCHECKED);
}

void CPropertiesBasic::SetDefaultBindHandler(int cmdID, const KeyBind& bind, int checked)
{
	m_Changed[cmdID] = true;
	CheckDlgButton(bind.nControlID, checked);
}

void CPropertiesBasic::Changed(int ctrlID)
{
	CmdTable::EnumCommands(this, &CPropertiesBasic::ChangedHandler, ctrlID);
}

void CPropertiesBasic::ChangedHandler(int cmdID, const KeyBind& bind, int ctrlID)
{
	if (bind.nControlID == ctrlID ||
			ctrlID == IDC_CO && bind.nControlID == IDC_CO2 ||
			bind.nControlID == IDC_CO && ctrlID == IDC_CO2)
		m_Changed[cmdID] = true;
}

BOOL CPropertiesBasic::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	if (m_ToolTip.Create(this))
		CmdTable::EnumCommands(this, &CPropertiesBasic::OnInitDialogHandler, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertiesBasic::OnInitDialogHandler(int cmdID, const KeyBind& bind, int)
{
	if (GetDlgItem(bind.nControlID))
		m_ToolTip.AddTool(GetDlgItem(bind.nControlID), CString(MAKEINTRESOURCE(CmdTable::ToolTipID(cmdID))));
}

int CPropertiesBasic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_pProperties = static_cast<CProperties *>(GetParent()->GetParent());
	return 0;
}

void CPropertiesBasic::OnAlt() 
{
	Changed(IDC_ALT);
}

void CPropertiesBasic::OnC_() 
{
	Changed(IDC_C_);
}

void CPropertiesBasic::OnCa() 
{
	Changed(IDC_CA);
}

void CPropertiesBasic::OnCb() 
{
	Changed(IDC_CB);
}

void CPropertiesBasic::OnCbackslash() 
{
	Changed(IDC_CBACKSLASH);
}

void CPropertiesBasic::OnCbackslash106() 
{
	Changed(IDC_CBACKSLASH_106);
}

void CPropertiesBasic::OnCd() 
{
	Changed(IDC_CD);
}

void CPropertiesBasic::OnCdigit() 
{
	Changed(IDC_CDIGIT);
}

void CPropertiesBasic::OnCe() 
{
	Changed(IDC_CE);
}

void CPropertiesBasic::OnCf() 
{
	Changed(IDC_CF);
}

void CPropertiesBasic::OnCg() 
{
	Changed(IDC_CG);
}

void CPropertiesBasic::OnCh() 
{
	Changed(IDC_CH);
}

void CPropertiesBasic::OnCi() 
{
	Changed(IDC_CI);
}

void CPropertiesBasic::OnCk() 
{
	Changed(IDC_CK);
}

void CPropertiesBasic::OnCl() 
{
	Changed(IDC_CL);
}

void CPropertiesBasic::OnCm() 
{
	Changed(IDC_CM);
}

void CPropertiesBasic::OnCms5() 
{
	Changed(IDC_CMS5);
}

void CPropertiesBasic::OnCn() 
{
	Changed(IDC_CN);
}

void CPropertiesBasic::OnCo() 
{
	Changed(IDC_CO);
	if (IsDlgButtonChecked(IDC_CO) == BST_CHECKED) {
		CheckDlgButton(IDC_CO2, BST_UNCHECKED);
		Changed(IDC_CO2);
	}
}

void CPropertiesBasic::OnCo2() 
{
	Changed(IDC_CO2);
	if (IsDlgButtonChecked(IDC_CO2) == BST_CHECKED) {
		CheckDlgButton(IDC_CO, BST_UNCHECKED);
		Changed(IDC_CO);
	}
}

void CPropertiesBasic::OnCp() 
{
	Changed(IDC_CP);
}

void CPropertiesBasic::OnCq() 
{
	Changed(IDC_CQ);
}

void CPropertiesBasic::OnCr() 
{
	Changed(IDC_CR);
}

void CPropertiesBasic::OnCs() 
{
	Changed(IDC_CS);
}

void CPropertiesBasic::OnCslash() 
{
	Changed(IDC_CSLASH);
}

void CPropertiesBasic::OnCspace() 
{
	Changed(IDC_CSPACE);
}

void CPropertiesBasic::OnCt() 
{
	Changed(IDC_CT);
}

void CPropertiesBasic::OnCu() 
{
	Changed(IDC_CU);
}

void CPropertiesBasic::OnCv() 
{
	Changed(IDC_CV);
}

void CPropertiesBasic::OnCw() 
{
	Changed(IDC_CW);
}

void CPropertiesBasic::OnCxb() 
{
	Changed(IDC_CXB);
}

void CPropertiesBasic::OnCxcc() 
{
	Changed(IDC_CXCC);
}

void CPropertiesBasic::OnCxcf() 
{
	Changed(IDC_CXCF);
}

void CPropertiesBasic::OnCxcj() 
{
	Changed(IDC_CXCJ);
}

void CPropertiesBasic::OnCxcp() 
{
	Changed(IDC_CXCP);
}

void CPropertiesBasic::OnCxcs() 
{
	Changed(IDC_CXCS);
}

void CPropertiesBasic::OnCxct() 
{
	Changed(IDC_CXCT);
}

void CPropertiesBasic::OnCxcw() 
{
	Changed(IDC_CXCW);
}

void CPropertiesBasic::OnCxe() 
{
	Changed(IDC_CXE);
}

void CPropertiesBasic::OnCxh() 
{
	Changed(IDC_CXH);
}

void CPropertiesBasic::OnCxk() 
{
	Changed(IDC_CXK);
}

void CPropertiesBasic::OnCxs0() 
{
	Changed(IDC_CXS0);
}

void CPropertiesBasic::OnCxs9() 
{
	Changed(IDC_CXS9);
}

void CPropertiesBasic::OnCxu() 
{
	Changed(IDC_CXU);
}

void CPropertiesBasic::OnCy() 
{
	Changed(IDC_CY);
}

void CPropertiesBasic::OnCz() 
{
	Changed(IDC_CZ);
}

void CPropertiesBasic::OnEscape() 
{
	Changed(IDC_ESCAPE);
}

void CPropertiesBasic::OnLeftAlt() 
{
	Changed(IDC_LEFT_ALT);
}

void CPropertiesBasic::OnMb() 
{
	Changed(IDC_MB);
}

void CPropertiesBasic::OnMd() 
{
	Changed(IDC_MD);
}

void CPropertiesBasic::OnMf() 
{
	Changed(IDC_MF);
}

void CPropertiesBasic::OnMi() 
{
	Changed(IDC_MI);
}

void CPropertiesBasic::OnMs1() 
{
	Changed(IDC_MS1);
}

void CPropertiesBasic::OnMs5() 
{
	Changed(IDC_MS5);
}

void CPropertiesBasic::OnMscomma() 
{
	Changed(IDC_MSCOMMA);
}

void CPropertiesBasic::OnMsperiod() 
{
	Changed(IDC_MSPERIOD);
}

void CPropertiesBasic::OnMv() 
{
	Changed(IDC_MV);
}

void CPropertiesBasic::OnMw() 
{
	Changed(IDC_MW);
}

void CPropertiesBasic::OnMy() 
{
	Changed(IDC_MY);
}

void CPropertiesBasic::OnRightAlt() 
{
	Changed(IDC_RIGHT_ALT);
}

void CPropertiesBasic::OnSquareBra() 
{
	Changed(IDC_SQUARE_BRA);
}

void CPropertiesBasic::OnMdel() 
{
	Changed(IDC_MDEL);
}

void CPropertiesBasic::OnMf12() 
{
	Changed(IDC_MF12);
}

void CPropertiesBasic::OnMt() 
{
	Changed(IDC_MT);
}

void CPropertiesBasic::OnMc() 
{
	Changed(IDC_MC);
}

void CPropertiesBasic::OnMl() 
{
	Changed(IDC_ML);
}

void CPropertiesBasic::OnMu() 
{
	Changed(IDC_MU);
}

void CPropertiesBasic::OnMx() 
{
	Changed(IDC_MX);
}
