#include "mainfrm64.h"

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

CMainFrame::CMainFrame()
{
	LPCTSTR title = _T("XKeymacs64");

	// register window class
	WNDCLASS	stWndClass;
	stWndClass.style			= CS_BYTEALIGNWINDOW;
	stWndClass.lpfnWndProc		= &AfxWndProc;
	stWndClass.cbClsExtra		= 0;
	stWndClass.cbWndExtra		= 0;
	stWndClass.hInstance		= AfxGetInstanceHandle();
	stWndClass.hIcon			= NULL;
	stWndClass.hCursor			= NULL;
	stWndClass.hbrBackground	= (HBRUSH)COLOR_BACKGROUND;
	stWndClass.lpszMenuName		= NULL;
	stWndClass.lpszClassName	= title;
	AfxRegisterClass(&stWndClass);

	// make window
	CRect rect(0, 0, 100, 100);
	Create(title , title, WS_OVERLAPPEDWINDOW | WS_MINIMIZE, rect);
}

