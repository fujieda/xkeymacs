#if !defined(AFX_OPTIONSDLG_H__526E5E0B_92DC_4F9C_BCE5_5ADB5DFE3444__INCLUDED_)
#define AFX_OPTIONSDLG_H__526E5E0B_92DC_4F9C_BCE5_5ADB5DFE3444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

class COptionsDlg : public CDialog
{
// Construction
public:
	virtual  ~COptionsDlg();
	COptionsDlg(CWnd *pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsDlg)
	enum { IDD = IDD_OPTIONS };
	CButton	m_cMx;
	CButton	m_cM_;
	CButton	m_cAlt;
	CButton	m_cShift;
	CButton	m_cCtrl;
	CButton	m_cCx;
	CButton	m_cFileCompletion;
	CButton	m_cAutoRun;
	CButton	m_cOK;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__526E5E0B_92DC_4F9C_BCE5_5ADB5DFE3444__INCLUDED_)
