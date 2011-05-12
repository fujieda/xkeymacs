// 109Keyboard.cpp : implementation file
//

#include "stdafx.h"
#include <Lmcons.h>
#include "xkeymacs.h"
#include "109Keyboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C109Keyboard dialog


C109Keyboard::C109Keyboard(const HKEY_TYPE hkey_type, CWnd *const pParent /*=NULL*/)
	: CDialog(C109Keyboard::IDD, pParent)
{
	//{{AFX_DATA_INIT(C109Keyboard)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_HkeyType = hkey_type;
}


void C109Keyboard::DoDataExchange(CDataExchange *const pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C109Keyboard)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C109Keyboard, CDialog)
	//{{AFX_MSG_MAP(C109Keyboard)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C109Keyboard message handlers

BOOL C109Keyboard::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString szWindowText;
	CString szFor;
	switch (m_HkeyType) {
	case CURRENT_USER:
		{
			TCHAR szUserName[UNLEN + 1] = {'0'};
			DWORD nSize = UNLEN + 1;
			if (GetUserName(szUserName, &nSize)) {
				szFor.Format(IDS_FOR, szUserName);
			} else {
				szFor.LoadString(IDS_FOR_LOGGED_ON_USER);
			}
		}
		break;
	case LOCAL_MACHINE:
		szFor.LoadString(IDS_FOR_ANY_USER);
		break;
	default:
		break;
	}
	this->GetWindowText(szWindowText);
	this->SetWindowText(szWindowText + szFor);

	m_ToolTip.Create(this, TTS_ALWAYSTIP | TTS_NOPREFIX);
	m_ToolTip.SetMaxTipWidth(0x100);	// Enable multiline

	CProfile::LoadScanCodeMap(m_HkeyType);
	for (int i = 0; i < sizeof(KeyboardLayouts) / sizeof(KeyboardLayouts[0]); ++i) {
		if (!GetDlgItem(KeyboardLayouts[i].nBaseControlID)
		 || !GetDlgItem(KeyboardLayouts[i].nCurrentControlID)) {
			continue;
		}

		KeyboardLayouts[i].pBaseKey = new CKey(KeyboardLayouts[i].nBaseControlID, NORMAL_KEY, m_HkeyType);
		KeyboardLayouts[i].pBaseKey->SubclassDlgItem(KeyboardLayouts[i].nBaseControlID, this);
		m_ToolTip.AddTool(GetDlgItem(KeyboardLayouts[i].nBaseControlID), CProfile::GetToolTipID(KeyboardLayouts[i].nToolTipID));

		KeyboardLayouts[i].pCurrentKey = new CKey(KeyboardLayouts[i].nCurrentControlID, ORIGINAL_KEY, m_HkeyType);
		KeyboardLayouts[i].pCurrentKey->SubclassDlgItem(KeyboardLayouts[i].nCurrentControlID, this);
		m_ToolTip.AddTool(GetDlgItem(KeyboardLayouts[i].nCurrentControlID), CProfile::GetToolTipID(KeyboardLayouts[i].nToolTipID));

		ScanCode current = {'\0'};
		if (CProfile::GetScanCodeMap(m_HkeyType, KeyboardLayouts[i].scancode, &current)) {
			KeyboardLayouts[i].pCurrentKey->SetKeyType(REMAPPED_KEY);

			CString szWindowText;
			GetDlgItem(CProfile::GetBaseControlID(current))->GetWindowText(szWindowText);
			GetDlgItem(KeyboardLayouts[i].nCurrentControlID)->SetWindowText(szWindowText);

			KeyboardLayout *pKeyboardLayout = CProfile::GetKeyboardLayouts(CProfile::GetBaseControlID(current));
			if (pKeyboardLayout) {
				m_ToolTip.UpdateTipText(CProfile::GetToolTipID(pKeyboardLayout->nToolTipID), GetDlgItem(KeyboardLayouts[i].nCurrentControlID));
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C109Keyboard::OnOK() 
{
	if (CProfile::LostKeyWarning(m_HkeyType) != IDCANCEL) {
		CProfile::SaveScanCodeMap(m_HkeyType);

		CDialog::OnOK();
	}
}

void C109Keyboard::OnDestroy() 
{
	CDialog::OnDestroy();

	for (int i = 0; i < sizeof(KeyboardLayouts) / sizeof(KeyboardLayouts[0]); ++i) {
		if (!GetDlgItem(KeyboardLayouts[i].nBaseControlID)
		 || !GetDlgItem(KeyboardLayouts[i].nCurrentControlID)) {
			continue;
		}

		if (KeyboardLayouts[i].pBaseKey) {
			delete KeyboardLayouts[i].pBaseKey;
			KeyboardLayouts[i].pBaseKey = NULL;
		}
		if (KeyboardLayouts[i].pCurrentKey) {
			delete KeyboardLayouts[i].pCurrentKey;
			KeyboardLayouts[i].pCurrentKey = NULL;
		}
	}
}

BOOL C109Keyboard::PreTranslateMessage(MSG *const pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (pMsg->message) {
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		m_ToolTip.RelayEvent(pMsg);
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
