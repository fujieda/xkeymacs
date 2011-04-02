#if !defined(AFX_109KEYBOARD_H__269D3317_3B74_469E_8C53_49EE2D785965__INCLUDED_)
#define AFX_109KEYBOARD_H__269D3317_3B74_469E_8C53_49EE2D785965__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Key.h"

// 109Keyboard.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C109Keyboard dialog

class C109Keyboard : public CDialog
{
// Construction
public:
	CToolTipCtrl m_ToolTip;
	C109Keyboard(HKEY_TYPE hkey_type, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(C109Keyboard)
	enum { IDD = IDD_109KEYBOARD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C109Keyboard)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(C109Keyboard)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HKEY_TYPE m_HkeyType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_109KEYBOARD_H__269D3317_3B74_469E_8C53_49EE2D785965__INCLUDED_)
