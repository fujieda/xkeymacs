// PropertiesBasic.cpp : implementation file
//

#include "stdafx.h"
#include "xkeymacs.h"
#include "DotXkeymacs.h"
#include "Profile.h"
#include "Properties.h"
#include "PropertiesBasic.h"

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

void CPropertiesBasic::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertiesBasic)
	//}}AFX_DATA_MAP
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
	// TODO: Add your specialized code here and/or call the base class
	m_pProperties->EnableControl(BASIC_TAB);
	SetDialogData(m_pProperties->GetCurrentApplication());

	return CPropertyPage::OnSetActive();
}

void CPropertiesBasic::SetAllDialogData(UINT nCheck, BOOL bChanged)
{
	for (int nCommandID = 0; nCommandID < sizeof(Commands) / sizeof(Commands[0]); ++nCommandID) {
		CString szCommandName = CXkeymacsData::GetCommandName(nCommandID);
		if (szCommandName.IsEmpty()) {
			break;
		}

		for (int i = 0; ; ++i) {
			int nKey = CXkeymacsData::GetDefaultCommandKey(nCommandID, i);
			int nControlID = CXkeymacsData::GetDefaultControlID(nCommandID, i);
			if (nKey == 0) {
				break;
			}
			CheckDlgButton(nControlID, nCheck);
		}
	}
	CheckDlgButton(IDC_CO2, BST_UNCHECKED);

	InitChanged(bChanged);
}

void CPropertiesBasic::SetDialogData(CString szApplicationName)
{
	UpdateDialogData(szApplicationName, FALSE);
}

void CPropertiesBasic::UpdateDialogData(CString szApplicationName, BOOL bSaveAndValidate)
{
	if (bSaveAndValidate) {	// GetDialogData
		UpdateData();
	}

	if (m_pProperties->GetApplicationID() == MAX_APP) {
		if (!bSaveAndValidate) {
			SetAllDialogData(1, FALSE);	// FIXME
		}
		return;
	}

	// on/off
	if (!bSaveAndValidate) {	// SetDialogData
		SetAllDialogData(1, FALSE);
		CheckDlgButton(IDC_CO2, BST_CHECKED);
	}
	for (int nCommandID = 0; nCommandID < sizeof(Commands) / sizeof(Commands[0]); ++nCommandID) {
		CString szCommandName = CXkeymacsData::GetCommandName(nCommandID);
		if (szCommandName.IsEmpty()) {
			break;
		}

		for (int i = 0; ; ++i) {
			int nCommandType = CXkeymacsData::GetDefaultCommandType(nCommandID, i);
			int nKey = CXkeymacsData::GetDefaultCommandKey(nCommandID, i);
			int nControlID = CXkeymacsData::GetDefaultControlID(nCommandID, i);
			if (nKey == 0) {
				break;
			}
			if (bSaveAndValidate) {	// GetDialogData
				if (nControlID == IDC_CO2) {
					continue;
				}
				if (!m_bChanged[nCommandID]) {
					continue;
				}
				if (IsDlgButtonChecked(nControlID)) {
					CProfile::SetCommandID(m_pProperties->GetApplicationID(), nCommandType, nKey, nCommandID);
					CDotXkeymacs::RemoveKey(m_pProperties->GetApplicationID(), nCommandType, nKey);
				} else {
					CProfile::SetCommandID(m_pProperties->GetApplicationID(), nCommandType, nKey, 0);
				}
			} else {				// SetDialogData
				if (nCommandID != CProfile::GetCommandID(m_pProperties->GetApplicationID(), nCommandType, nKey)) {
					CheckDlgButton(nControlID, BST_UNCHECKED);
				}
			}
		}
	}
	// only for Toggle Imput Method Editor C-o: Canna mode
	if (bSaveAndValidate) {	// GetDialogData
		int nCommandType = CONTROL;
		int nKey = 'O';
		if (CProfile::GetCommandID(m_pProperties->GetApplicationID(), nCommandType, nKey) == 0) {
			if (IsDlgButtonChecked(IDC_CO2)) {
				for (int nCommandID = 0; nCommandID < sizeof(Commands) / sizeof(Commands[0]); ++nCommandID) {
					CString szCommandName = CXkeymacsData::GetCommandName(nCommandID);
					if (szCommandName.IsEmpty()) {
						break;
					}
					if (!szCommandName.CompareNoCase(_T("toggle-input-method"))) {
						CProfile::SetCommandID(m_pProperties->GetApplicationID(), nCommandType, nKey, nCommandID);
						CProfile::SetCommandID(m_pProperties->GetApplicationID(), CONTROL+SHIFT, nKey, nCommandID);
						break;
					}
				}
			}
		}
	}

	if (!bSaveAndValidate) {	// SetDialogData
		UpdateData(FALSE);
	}
}

void CPropertiesBasic::InitChanged(BOOL bChanged)
{
	for (int i = 0; i < sizeof(m_bChanged)/sizeof(m_bChanged[0]); ++i) {
		if (CXkeymacsData::GetDefaultControlID(i, 0) || !bChanged) {
			m_bChanged[i] = bChanged;
		}
	}
}

int CPropertiesBasic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_pProperties = (CProperties *)GetParent()->GetParent();

	return 0;
}

void CPropertiesBasic::GetDialogData()
{
	UpdateDialogData(m_pProperties->GetCurrentApplication(), TRUE);
}

void CPropertiesBasic::OnAlt() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_ALT);
}

void CPropertiesBasic::OnC_() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_C_);
}

void CPropertiesBasic::OnCa() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CA);
}

void CPropertiesBasic::OnCb() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CB);
}

void CPropertiesBasic::OnCbackslash() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CBACKSLASH);
}

void CPropertiesBasic::OnCbackslash106() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CBACKSLASH_106);
}

void CPropertiesBasic::OnCd() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CD);
}

void CPropertiesBasic::OnCdigit() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CDIGIT);
}

void CPropertiesBasic::OnCe() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CE);
}

void CPropertiesBasic::OnCf() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CF);
}

void CPropertiesBasic::OnCg() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CG);
}

void CPropertiesBasic::OnCh() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CH);
}

void CPropertiesBasic::OnCi() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CI);
}

void CPropertiesBasic::OnCk() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CK);
}

void CPropertiesBasic::OnCl() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CL);
}

void CPropertiesBasic::OnCm() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CM);
}

void CPropertiesBasic::OnCms5() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CMS5);
}

void CPropertiesBasic::OnCn() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CN);
}

void CPropertiesBasic::OnCo() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CO);
	if (IsDlgButtonChecked(IDC_CO) == BST_CHECKED) {
		CheckDlgButton(IDC_CO2, BST_UNCHECKED);
		Changed(IDC_CO2);
	}
}

void CPropertiesBasic::OnCo2() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CO2);
	if (IsDlgButtonChecked(IDC_CO2) == BST_CHECKED) {
		CheckDlgButton(IDC_CO, BST_UNCHECKED);
		Changed(IDC_CO);
	}
}

void CPropertiesBasic::OnCp() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CP);
}

void CPropertiesBasic::OnCq() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CQ);
}

void CPropertiesBasic::OnCr() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CR);
}

void CPropertiesBasic::OnCs() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CS);
}

void CPropertiesBasic::OnCslash() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CSLASH);
}

void CPropertiesBasic::OnCspace() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CSPACE);
}

void CPropertiesBasic::OnCt() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CT);
}

void CPropertiesBasic::OnCu() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CU);
}

void CPropertiesBasic::OnCv() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CV);
}

void CPropertiesBasic::OnCw() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CW);
}

void CPropertiesBasic::OnCxb() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CXB);
}

void CPropertiesBasic::OnCxcc() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CXCC);
}

void CPropertiesBasic::OnCxcf() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CXCF);
}

void CPropertiesBasic::OnCxcj() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CXCJ);
}

void CPropertiesBasic::OnCxcp() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CXCP);
}

void CPropertiesBasic::OnCxcs() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CXCS);
}

void CPropertiesBasic::OnCxct() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CXCT);
}

void CPropertiesBasic::OnCxcw() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CXCW);
}

void CPropertiesBasic::OnCxe() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CXE);
}

void CPropertiesBasic::OnCxh() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CXH);
}

void CPropertiesBasic::OnCxk() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CXK);
}

void CPropertiesBasic::OnCxs0() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CXS0);
}

void CPropertiesBasic::OnCxs9() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CXS9);
}

void CPropertiesBasic::OnCxu() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CXU);
}

void CPropertiesBasic::OnCy() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CY);
}

void CPropertiesBasic::OnCz() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_CZ);
}

void CPropertiesBasic::OnEscape() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_ESCAPE);
}

void CPropertiesBasic::OnLeftAlt() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_LEFT_ALT);
}

void CPropertiesBasic::OnMb() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MB);
}

void CPropertiesBasic::OnMd() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MD);
}

void CPropertiesBasic::OnMf() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MF);
}

void CPropertiesBasic::OnMi() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MI);
}

void CPropertiesBasic::OnMs1() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MS1);
}

void CPropertiesBasic::OnMs5() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MS5);
}

void CPropertiesBasic::OnMscomma() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MSCOMMA);
}

void CPropertiesBasic::OnMsperiod() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MSPERIOD);
}

void CPropertiesBasic::OnMv() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MV);
}

void CPropertiesBasic::OnMw() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MW);
}

void CPropertiesBasic::OnMy() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MY);
}

void CPropertiesBasic::OnRightAlt() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_RIGHT_ALT);
}

void CPropertiesBasic::OnSquareBra() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_SQUARE_BRA);
}

void CPropertiesBasic::Changed(int nObjectID)
{
	for (int nCommandID = 0; nCommandID < sizeof(Commands) / sizeof(Commands[0]); ++nCommandID) {
		CString szCommandName = CXkeymacsData::GetCommandName(nCommandID);
		if (szCommandName.IsEmpty()) {
			break;
		}

		for (int i = 0; ; ++i) {
			if (CXkeymacsData::GetDefaultCommandKey(nCommandID, i) == 0) {
				break;
			}

			int nControlID = CXkeymacsData::GetDefaultControlID(nCommandID, i);
			if ((nControlID == nObjectID)
			 || ((nObjectID == IDC_CO) && (nControlID == IDC_CO2))
			 || ((nObjectID == IDC_CO2) && (nControlID == IDC_CO))) {
				m_bChanged[nCommandID] = TRUE;
			}
		}
	}
}

BOOL CPropertiesBasic::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetDialogData();

	return CPropertyPage::OnKillActive();
}

void CPropertiesBasic::EnableControl()
{
	BOOL bEnable = m_pProperties->IsEnableControl();

	for (int nCommandID = 0; nCommandID < sizeof(Commands) / sizeof(Commands[0]); ++nCommandID) {
		CString szCommandName = CXkeymacsData::GetCommandName(nCommandID);
		if (szCommandName.IsEmpty()) {
			break;
		}

		for (int i = 0; ; ++i) {
			int nKey = CXkeymacsData::GetDefaultCommandKey(nCommandID, i);
			int nControlID = CXkeymacsData::GetDefaultControlID(nCommandID, i);
			if (nKey == 0) {
				break;
			}
			if (GetDlgItem(nControlID)) {
				GetDlgItem(nControlID)->EnableWindow(bEnable);
			}
		}
	}
}

BOOL CPropertiesBasic::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTip.RelayEvent(pMsg);

	return CPropertyPage::PreTranslateMessage(pMsg);
}

BOOL CPropertiesBasic::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here
	if (m_ToolTip.Create(this)) {
		for (int nCommandID = 0; nCommandID < sizeof(Commands) / sizeof(Commands[0]); ++nCommandID) {
			CString szCommandName = CXkeymacsData::GetCommandName(nCommandID);
			if (szCommandName.IsEmpty()) {
				break;
			}

			for (int i = 0; ; ++i) {
				int nKey = CXkeymacsData::GetDefaultCommandKey(nCommandID, i);
				int nControlID = CXkeymacsData::GetDefaultControlID(nCommandID, i);
				if (nKey == 0) {
					break;
				}
				if (GetDlgItem(nControlID)) {
					m_ToolTip.AddTool(GetDlgItem(nControlID), CString(MAKEINTRESOURCE(CXkeymacsData::GetToolTipID(nCommandID))));
				}
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertiesBasic::OnMdel() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MDEL);
}

void CPropertiesBasic::OnMf12() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MF12);
}

void CPropertiesBasic::OnMt() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MT);
}

void CPropertiesBasic::OnMc() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MC);
}

void CPropertiesBasic::OnMl() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_ML);
}

void CPropertiesBasic::OnMu() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MU);
}

void CPropertiesBasic::OnMx() 
{
	// TODO: Add your control notification handler code here
	Changed(IDC_MX);
}
