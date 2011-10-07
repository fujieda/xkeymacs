// 104Keyboard.cpp : implementation file
//

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
