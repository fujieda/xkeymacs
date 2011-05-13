// xkeymacs.h : main header file for the XKEYMACS application
//

#if !defined(AFX_XKEYMACS_H__2E65B9CB_5ADB_4C3B_891F_8EFE395FBB2D__INCLUDED_)
#define AFX_XKEYMACS_H__2E65B9CB_5ADB_4C3B_891F_8EFE395FBB2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

enum Instance
{
	FirstInstance,
	SecondInstance,
};


/////////////////////////////////////////////////////////////////////////////
// CXkeymacsApp:
// See xkeymacs.cpp for the implementation of this class
//

class CXkeymacsApp : public CWinApp
{
public:
	CXkeymacsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXkeymacsApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CXkeymacsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	Instance m_Instance;
	HANDLE m_hMutex;
};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStatic	m_cURL;
	CString	m_szVersionInformation;
	CString	m_szLegalCopyright;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnUrl();
	afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

typedef struct Translate {
	WORD wLanguage;
	WORD wCodePage;
} Translate_t;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XKEYMACS_H__2E65B9CB_5ADB_4C3B_891F_8EFE395FBB2D__INCLUDED_)
