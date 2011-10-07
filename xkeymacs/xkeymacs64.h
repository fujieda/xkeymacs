#ifndef AFX_XKEYMACS64_H_INCLUDED
#define AFX_XKEYMACS64_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

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
private:
	HANDLE m_hMutex;
};

#endif // AFX_XKEYMACS64_H_INCLUDED
