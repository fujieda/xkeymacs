#include "stdafx.h"
#include "about.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_szVersionInformation = _T("");
	m_szLegalCopyright = _T("");
	//}}AFX_DATA_INIT

	m_szLegalCopyright.Format(_T("Copyright (C) 2001-2006"));	// rough

	WORD wMajorVersion		= 0;
	WORD wMinorVersion		= 0;
	WORD wBuildVersion		= 0;
	WORD wRevisionVersion	= 0;

	TCHAR szFileName[MAX_PATH] = {'\0'};
	GetModuleFileName(NULL, szFileName, sizeof(szFileName));
	DWORD dwHandle = 0;

	DWORD dwLen = GetFileVersionInfoSize(szFileName, &dwHandle);
	if (dwLen) {
		LPVOID lpData = new BYTE[dwLen];
		if (GetFileVersionInfo(szFileName, dwHandle, dwLen, lpData)) {
			VS_FIXEDFILEINFO *pInfo;
			UINT uLen;
			if (VerQueryValue(lpData, _T("\\"), (LPVOID *)&pInfo, &uLen)) {
				wMajorVersion		= (WORD)((pInfo->dwProductVersionMS >> 16) & 0xffff);
				wMinorVersion		= (WORD)((pInfo->dwProductVersionMS      ) & 0xffff);
				wBuildVersion		= (WORD)((pInfo->dwProductVersionLS >> 16) & 0xffff);
				wRevisionVersion	= (WORD)((pInfo->dwProductVersionLS      ) & 0xffff);
			}

			struct Translate {
				WORD wLanguage;
				WORD wCodePage;
			} *lpTranslate = NULL;
			UINT cbTranslate = 0;
			if (VerQueryValue(lpData, _T("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &cbTranslate) && sizeof(*lpTranslate) <= cbTranslate) {
				LPVOID lpLegalCopyright = NULL;
				UINT uLen = 0;
				CString SubBlock;

				SubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\LegalCopyright"), lpTranslate->wLanguage, lpTranslate->wCodePage);
				if (VerQueryValue(lpData, SubBlock.GetBuffer(SubBlock.GetLength()), (LPVOID *)&lpLegalCopyright, &uLen) && uLen) {
					m_szLegalCopyright.Format(_T("%s"), lpLegalCopyright);
				}
			}
		}
		delete[] lpData;
		lpData = NULL;
	}

	m_szVersionInformation.Format(_T("%s Version %d.%d"), CString(MAKEINTRESOURCE(AFX_IDS_APP_TITLE)), wMajorVersion, wMinorVersion);

	if (wBuildVersion) {
		CString sz;
		sz.Format(_T(".%d"), wBuildVersion);
		m_szVersionInformation += sz;

		if (wRevisionVersion) {
			CString sz;
			sz.Format(_T(".%d"), wRevisionVersion);
			m_szVersionInformation += sz;
		}
	}
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_URL, m_cURL);
	DDX_Text(pDX, IDC_VERSION_INFORMATION, m_szVersionInformation);
	DDX_Text(pDX, IDC_LEGAL_COPYRIGHT, m_szLegalCopyright);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_URL, OnUrl)
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers


void CAboutDlg::OnOK() 
{
	CDialog::OnOK();
}

void CAboutDlg::OnClose() 
{
	CDialog::OnClose();
}

void CAboutDlg::OnUrl() 
{
	CString szURL;
	m_cURL.GetWindowText(szURL);

	ShellExecute(m_hWnd, NULL, szURL, NULL, NULL, SW_SHOWNORMAL);
}

HBRUSH CAboutDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	COLORREF crBlue = RGB(0, 0, 0xff);

	if (pWnd == &m_cURL) {
		pDC->SetTextColor(crBlue);
	}

	return hbr;
}

BOOL CAboutDlg::OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message) 
{
	CRect rect;
	m_cURL.GetWindowRect(rect);

	POINT p;
	::GetCursorPos(&p);

	if (rect.PtInRect(p)) {
		HCURSOR hCursor = (HCURSOR)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_HAND_CURSOR),
											 IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
		SetCursor(hCursor);
		return TRUE;	
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
