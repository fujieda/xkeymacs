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

static const KeyName_t KeyNames[] = {
	{0,				_T("")},
	{VK_LBUTTON,	_T("Left mouse button")},
	{VK_RBUTTON,	_T("Right mouse button")},
	{VK_CANCEL,		_T("Break")},
	{VK_MBUTTON,	_T("Middle mouse button")},
	{0x05,			_T("X1 mouse button")},	// VK_XBUTTON1
	{0x06,			_T("X2 mouse button")},	// VK_XBUTTON2
	{0x07,			_T("Undefined")},
	{VK_BACK,		_T("Backspace")},
	{VK_TAB,		_T("Tab")},
	{0x0a,			_T("Reserved")},
	{0x0b,			_T("Reserved")},
	{VK_CLEAR,		_T("Clear")},
	{VK_RETURN,		_T("Enter")},
	{0x0e,			_T("Undefined")},
	{0x0f,			_T("Undefined")},
	{VK_SHIFT,		_T("Shift")},
	{VK_CONTROL,	_T("Ctrl")},
	{VK_MENU,		_T("Alt")},
	{VK_PAUSE,		_T("Pause")},
	{VK_CAPITAL,	_T("Caps Lock")},
	{VK_KANA,		_T("Kana")},	//	{VK_HANGUEL,	"Hanguel"},	{VK_HANGUL,		"Hangul"},
	{0x16,			_T("Undefined")},
	{VK_JUNJA,		_T("Junja")},
	{VK_FINAL,		_T("Final")},
	{VK_KANJI,		_T("Kanji")},	//	{VK_HANJA,		"Hanja"},
	{0x1a,			_T("Undefined")},
	{VK_ESCAPE,		_T("Esc")},
	{VK_CONVERT,	_T("Convert")},		// ïœä∑
	{VK_NONCONVERT,	_T("Nonconvert")},	// ñ≥ïœä∑
	{VK_ACCEPT,		_T("Accept")},
	{VK_MODECHANGE,	_T("Mode change")},
	{VK_SPACE,		_T("Space")},
	{VK_PRIOR,		_T("Page Up")},
	{VK_NEXT,		_T("Page Down")},
	{VK_END,		_T("End")},
	{VK_HOME,		_T("Home")},
	{VK_LEFT,		_T("Left")},
	{VK_UP,			_T("Up")},
	{VK_RIGHT,		_T("Right")},
	{VK_DOWN,		_T("Down")},
	{VK_SELECT,		_T("Select")},
	{VK_PRINT,		_T("Print")},
	{VK_EXECUTE,	_T("Execute")},
	{VK_SNAPSHOT,	_T("Print Screen")},
	{VK_INSERT,		_T("Ins")},
	{VK_DELETE,		_T("Del")},
	{VK_HELP,		_T("Help")},
	{'0',			_T("0")},
	{'1',			_T("1")},
	{'2',			_T("2")},
	{'3',			_T("3")},
	{'4',			_T("4")},
	{'5',			_T("5")},
	{'6',			_T("6")},
	{'7',			_T("7")},
	{'8',			_T("8")},
	{'9',			_T("9")},
	{0x3a,			_T("Undefined")},
	{0x3b,			_T("Undefined")},
	{0x3c,			_T("Undefined")},
	{0x3d,			_T("Undefined")},
	{0x3e,			_T("Undefined")},
	{0x3f,			_T("Undefined")},
	{0x40,			_T("Undefined")},
	{'A',			_T("A")},
	{'B',			_T("B")},
	{'C',			_T("C")},
	{'D',			_T("D")},
	{'E',			_T("E")},
	{'F',			_T("F")},
	{'G',			_T("G")},
	{'H',			_T("H")},
	{'I',			_T("I")},
	{'J',			_T("J")},
	{'K',			_T("K")},
	{'L',			_T("L")},
	{'M',			_T("M")},
	{'N',			_T("N")},
	{'O',			_T("O")},
	{'P',			_T("P")},
	{'Q',			_T("Q")},
	{'R',			_T("R")},
	{'S',			_T("S")},
	{'T',			_T("T")},
	{'U',			_T("U")},
	{'V',			_T("V")},
	{'W',			_T("W")},
	{'X',			_T("X")},
	{'Y',			_T("Y")},
	{'Z',			_T("Z")},
	{VK_LWIN,		_T("Left Windows")},
	{VK_RWIN,		_T("Right Windows")},
	{VK_APPS,		_T("Application")},
	{0x5e,			_T("Reserved")},
	{0x5f,			_T("Sleep")},	// VK_SLEEP
	{VK_NUMPAD0,	_T("Num 0")},
	{VK_NUMPAD1,	_T("Num 1")},
	{VK_NUMPAD2,	_T("Num 2")},
	{VK_NUMPAD3,	_T("Num 3")},
	{VK_NUMPAD4,	_T("Num 4")},
	{VK_NUMPAD5,	_T("Num 5")},
	{VK_NUMPAD6,	_T("Num 6")},
	{VK_NUMPAD7,	_T("Num 7")},
	{VK_NUMPAD8,	_T("Num 8")},
	{VK_NUMPAD9,	_T("Num 9")},
	{VK_MULTIPLY,	_T("Num *")},
	{VK_ADD,		_T("Num +")},
	{VK_SEPARATOR,	_T("Separator")},
	{VK_SUBTRACT,	_T("Num -")},
	{VK_DECIMAL,	_T("Num .")},
	{VK_DIVIDE,		_T("Num /")},
	{VK_F1,			_T("F1")},
	{VK_F2,			_T("F2")},
	{VK_F3,			_T("F3")},
	{VK_F4,			_T("F4")},
	{VK_F5,			_T("F5")},
	{VK_F6,			_T("F6")},
	{VK_F7,			_T("F7")},
	{VK_F8,			_T("F8")},
	{VK_F9,			_T("F9")},
	{VK_F10,		_T("F10")},
	{VK_F11,		_T("F11")},
	{VK_F12,		_T("F12")},
	{VK_F13,		_T("F13")},
	{VK_F14,		_T("F14")},
	{VK_F15,		_T("F15")},
	{VK_F16,		_T("F16")},
	{VK_F17,		_T("F17")},
	{VK_F18,		_T("F18")},
	{VK_F19,		_T("F19")},
	{VK_F20,		_T("F20")},
	{VK_F21,		_T("F21")},
	{VK_F22,		_T("F22")},
	{VK_F23,		_T("F23")},
	{VK_F24,		_T("F24")},
	{0x88,			_T("Unassigned")},
	{0x89,			_T("Unassigned")},
	{0x8a,			_T("Unassigned")},
	{0x8b,			_T("Unassigned")},
	{0x8c,			_T("Unassigned")},
	{0x8d,			_T("Unassigned")},
	{0x8e,			_T("Unassigned")},
	{0x8f,			_T("Unassigned")},
	{VK_NUMLOCK,	_T("Num Lock")},
	{VK_SCROLL,		_T("Scroll Lock")},
	{0x92,			_T("OEM specific")},
	{0x93,			_T("OEM specific")},
	{0x94,			_T("OEM specific")},
	{0x95,			_T("OEM specific")},
	{0x96,			_T("OEM specific")},
	{0x97,			_T("Unassigned")},
	{0x98,			_T("Unassigned")},
	{0x99,			_T("Unassigned")},
	{0x9a,			_T("Unassigned")},
	{0x9b,			_T("Unassigned")},
	{0x9c,			_T("Unassigned")},
	{0x9d,			_T("Unassigned")},
	{0x9e,			_T("Unassigned")},
	{0x9f,			_T("Unassigned")},
	{VK_LSHIFT,		_T("Left Shift")},
	{VK_RSHIFT,		_T("Right Shift")},
	{VK_LCONTROL,	_T("Left Ctrl")},
	{VK_RCONTROL,	_T("Right Ctrl")},
	{VK_LMENU,		_T("Left Alt")},
	{VK_RMENU,		_T("Right Alt")},
	{0xa6,			_T("Browser Back")},		// VK_BROWSER_BACK
	{0xa7,			_T("Browser Forward")},		// VK_BROWSER_FORWARD
	{0xa8,			_T("Browser Refresh")},		// VK_BROWSER_REFRESH
	{0xa9,			_T("Browser Stop")},		// VK_BROWSER_STOP
	{0xaa,			_T("Browser Search")},		// VK_BROWSER_SEARCH
	{0xab,			_T("Browser Favorites")},	// VK_BROWSER_FAVORITES
	{0xac,			_T("Browser Start")},		// VK_BROWSER_HOME
	{0xad,			_T("Volume Mute")},			// VK_VOLUME_MUTE
	{0xae,			_T("Volume Down")},			// VK_VOLUME_DOWN
	{0xaf,			_T("Volume Up")},			// VK_VOLUME_UP
	{0xb0,			_T("Next Track")},			// VK_MEDIA_NEXT_TRACK
	{0xb1,			_T("Previous Track")},		// VK_MEDIA_PREV_TRACK
	{0xb2,			_T("Stop Media")},			// VK_MEDIA_STOP
	{0xb3,			_T("Play/Pause Media")},	// VK_MEDIA_PLAY_PAUSE
	{0xb4,			_T("Start Mail")},			// VK_LAUNCH_MAIL
	{0xb5,			_T("Select Media")},		// VK_LAUNCH_MEDIA_SELECT
	{0xb6,			_T("Start Application 1")},	// VK_LAUNCH_APP1
	{0xb7,			_T("Start Application 2")},	// VK_LAUNCH_APP2
	{0xb8,			_T("Reserved")},
	{0xb9,			_T("Reserved")},
	{0xba,			_T(";")},					// VK_OEM_1
//	{0xba,			_T(":")},					// VK_OEM_1		// for Japanese keyboard
	{0xbb,			_T("+")},					// VK_OEM_PLUS
//	{0xbb,			_T(";")},					// VK_OEM_PLUS	// for Japanese keyboard
	{0xbc,			_T(",")},					// VK_OEM_COMMA
	{0xbd,			_T("-")},					// VK_OEM_MINUS
	{0xbe,			_T(".")},					// VK_OEM_PERIOD
	{0xbf,			_T("/")},					// VK_OEM_2
	{0xc0,			_T("`")},					// VK_OEM_3
//	{0xc0,			_T("@")},					// VK_OEM_3		// for Japanese keyboard
	{0xc1,			_T("Reserved")},
	{0xc2,			_T("Reserved")},
	{0xc3,			_T("Reserved")},
	{0xc4,			_T("Reserved")},
	{0xc5,			_T("Reserved")},
	{0xc6,			_T("Reserved")},
	{0xc7,			_T("Reserved")},
	{0xc8,			_T("Reserved")},
	{0xc9,			_T("Reserved")},
	{0xca,			_T("Reserved")},
	{0xcb,			_T("Reserved")},
	{0xcc,			_T("Reserved")},
	{0xcd,			_T("Reserved")},
	{0xce,			_T("Reserved")},
	{0xcf,			_T("Reserved")},
	{0xd0,			_T("Reserved")},
	{0xd1,			_T("Reserved")},
	{0xd2,			_T("Reserved")},
	{0xd3,			_T("Reserved")},
	{0xd4,			_T("Reserved")},
	{0xd5,			_T("Reserved")},
	{0xd6,			_T("Reserved")},
	{0xd7,			_T("Reserved")},
	{0xd8,			_T("Unassigned")},
	{0xd9,			_T("Unassigned")},
	{0xda,			_T("Unassigned")},
	{0xdb,			_T("[")},					// VK_OEM_4
	{0xdc,			_T("Backslash")},			// VK_OEM_5
	{0xdd,			_T("]")},					// VK_OEM_6
	{0xde,			_T("'")},					// VK_OEM_7
	{0xdf,			_T("OEM specific")},		// VK_OEM_8
	{0xe0,			_T("Reserved")},
	{0xe1,			_T("OEM specific")},
	{0xe2,			_T("Backslash for 106 keyboard")},	// VK_OEM_102
	{0xe3,			_T("OEM specific")},
	{0xe4,			_T("OEM specific")},
	{0xe5,			_T("Process")},				// VK_PROCESSKEY
	{0xe6,			_T("OEM specific")},
	{0xe7,			_T("Packet")},	// VK_PACKET
	{0xe8,			_T("Unassigned")},
	{0xe9,			_T("OEM specific")},
	{0xea,			_T("OEM specific")},
	{0xeb,			_T("OEM specific")},
	{0xec,			_T("OEM specific")},
	{0xed,			_T("OEM specific")},
	{0xee,			_T("OEM specific")},
	{0xef,			_T("OEM specific")},
	{0xf0,			_T("Eisu")},					// âpêî
	{0xf1,			_T("OEM specific")},
	{0xf2,			_T("Hiragana")},				// Ç–ÇÁÇ™Ç»
	{0xf3,			_T("Hankaku/Zenkaku 0xf3")},	// "îºäp/ëSäp"
	{0xf4,			_T("Hankaku/Zenkaku 0xf4")},	// "îºäp/ëSäp"
	{0xf5,			_T("OEM specific")},
	{VK_ATTN,		_T("Attn")},
	{VK_CRSEL,		_T("CrSel")},
	{VK_EXSEL,		_T("ExSel")},
	{VK_EREOF,		_T("Erace EOF")},
	{VK_PLAY,		_T("Play")},
	{VK_ZOOM,		_T("Zoom")},
	{VK_NONAME,		_T("Noname")},
	{VK_PA1,		_T("PA1")},
	{VK_OEM_CLEAR,	_T("OEM Clear")},
	{0xff,			_T("")},
};

typedef struct Translate {
	WORD wLanguage;
	WORD wCodePage;
} Translate_t;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XKEYMACS_H__2E65B9CB_5ADB_4C3B_891F_8EFE395FBB2D__INCLUDED_)
