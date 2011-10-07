#include "mainfrm64.h"

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

CMainFrame::CMainFrame()
{
	// register window class
	WNDCLASS	stWndClass;
	CString szClassName(MAKEINTRESOURCE(AFX_IDS_APP_TITLE));

	stWndClass.style			= CS_BYTEALIGNWINDOW;
	stWndClass.lpfnWndProc		= &AfxWndProc;
	stWndClass.cbClsExtra		= 0;
	stWndClass.cbWndExtra		= 0;
	stWndClass.hInstance		= AfxGetInstanceHandle();
	stWndClass.hIcon			= NULL;
	stWndClass.hCursor			= NULL;
	stWndClass.hbrBackground	= (HBRUSH)COLOR_BACKGROUND;
	stWndClass.lpszMenuName		= NULL;
	stWndClass.lpszClassName	= szClassName;

	AfxRegisterClass(&stWndClass);

	// make window
	CRect	rect(0, 0, 100, 100);

	Create(szClassName, CString(MAKEINTRESOURCE(AFX_IDS_APP_TITLE)), WS_OVERLAPPEDWINDOW | WS_MINIMIZE, rect);
}

