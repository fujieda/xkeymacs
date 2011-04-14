// 104Keyboard.cpp : implementation file
//

#include "stdafx.h"
#include <Lmcons.h>
#include "xkeymacs.h"
#include "104Keyboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C104Keyboard dialog


C104Keyboard::C104Keyboard(const HKEY_TYPE hkey_type, CWnd *const pParent /*=NULL*/)
	: CKeyboardLayout(hkey_type, C104Keyboard::IDD, pParent)
{
	//{{AFX_DATA_INIT(C104Keyboard)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void C104Keyboard::DoDataExchange(CDataExchange *const pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C104Keyboard)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C104Keyboard, CDialog)
	//{{AFX_MSG_MAP(C104Keyboard)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C104Keyboard message handlers

BOOL C104Keyboard::OnInitDialog() 
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

	InitKeyboardLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C104Keyboard::OnOK() 
{
	if (LostKeyWarning(m_HkeyType) != IDCANCEL) {
		SaveScanCodeMap(m_HkeyType);

		CDialog::OnOK();
	}
}

void C104Keyboard::OnDestroy() 
{
	CDialog::OnDestroy();

	DestroyKeyboardLayout();
}

BOOL C104Keyboard::PreTranslateMessage(MSG *const pMsg) 
{
	switch (pMsg->message) {
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		m_ToolTip.RelayEvent(pMsg);
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
