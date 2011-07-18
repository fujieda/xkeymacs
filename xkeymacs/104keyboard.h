#if !defined(AFX_104KEYBOARD_H__0206A99F_158D_419A_A816_CC493DDDA16C__INCLUDED_)
#define AFX_104KEYBOARD_H__0206A99F_158D_419A_A816_CC493DDDA16C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "keyboardlayout.h"

// 104Keyboard.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C104Keyboard dialog

class C104Keyboard : public CKeyboardLayout
{
// Construction
public:
	C104Keyboard(HKEY_TYPE hkey_type, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(C104Keyboard)
	enum { IDD = IDD_104KEYBOARD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_104KEYBOARD_H__0206A99F_158D_419A_A816_CC493DDDA16C__INCLUDED_)
