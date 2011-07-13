#include "stdafx.h"
#include "about.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	int major, minor, build, revision;
	major = minor = build = revision = 0;

	TCHAR szFileName[MAX_PATH] = {'\0'};
	GetModuleFileName(NULL, szFileName, sizeof(szFileName));
	DWORD dwHandle = 0;

	DWORD dwLen = GetFileVersionInfoSize(szFileName, &dwHandle);
	if (dwLen) {
		std::vector<BYTE> data(dwLen);
		if (GetFileVersionInfo(szFileName, dwHandle, dwLen, &data[0])) {
			VS_FIXEDFILEINFO *pInfo;
			UINT uLen;
			if (VerQueryValue(&data[0], _T("\\"), reinterpret_cast<LPVOID *>(&pInfo), &uLen)) {
				major = pInfo->dwProductVersionMS >> 16;
				minor = pInfo->dwProductVersionMS & 0xffff;
				build = pInfo->dwProductVersionLS >> 16;
				revision = pInfo->dwProductVersionLS & 0xffff;
			}
		}
	}
	m_versionInfo.Format(_T("%s Version %d.%d"), CString(MAKEINTRESOURCE(AFX_IDS_APP_TITLE)), major, minor);
	if (build) {
		CString s;
		s.Format(_T(".%d"), build);
		m_versionInfo += s;
		if (revision) {
			s.Format(_T(".%d"), revision);
			m_versionInfo += s;
		}
	}
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_URL, m_cURL);
	DDX_Text(pDX, IDC_VERSION_INFORMATION, m_versionInfo);
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
