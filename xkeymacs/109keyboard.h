#if !defined(AFX_109KEYBOARD_H__269D3317_3B74_469E_8C53_49EE2D785965__INCLUDED_)
#define AFX_109KEYBOARD_H__269D3317_3B74_469E_8C53_49EE2D785965__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "keyboardlayout.h"

// 109Keyboard.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C109Keyboard dialog

class C109Keyboard : public CKeyboardLayout
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
protected:
	int GetToolTipID(int nToolTipID);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_109KEYBOARD_H__269D3317_3B74_469E_8C53_49EE2D785965__INCLUDED_)
