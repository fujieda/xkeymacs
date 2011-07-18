// 109Keyboard.cpp : implementation file
//

#include "stdafx.h"
#include "109Keyboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C109Keyboard dialog


C109Keyboard::C109Keyboard(const HKEY_TYPE hkey_type, CWnd *const pParent /*=NULL*/)
	: CKeyboardLayout(hkey_type, C109Keyboard::IDD, pParent)
{
	//{{AFX_DATA_INIT(C109Keyboard)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

int C109Keyboard::GetToolTipID(int nToolTipID)
{
	switch (nToolTipID) {
	case IDS_EQUAL:			// ^
		nToolTipID = IDS_CARET;
		break;
	case IDS_SQUARE_BRA:	// @
		nToolTipID = IDS_AT_MARK;
		break;
	case IDS_SQUARE_CKET:	// [
		nToolTipID = IDS_SQUARE_BRA;
		break;
	case IDS_QUOTE:			// :
		nToolTipID = IDS_COLON;
		break;
	case IDS_BACK_QUOTE:	// Hankaku/Zenkaku
		nToolTipID = IDS_HANKAKU;
		break;
	case IDS_BACKSLASH:		// ]
		nToolTipID = IDS_SQUARE_CKET;
		break;
	}
	return nToolTipID;
}
