#if !defined(AFX_KEY_H__509440FD_B54D_4C17_AAA1_C394820F4FB6__INCLUDED_)
#define AFX_KEY_H__509440FD_B54D_4C17_AAA1_C394820F4FB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Key.h : header file
//

#include "stdafx.h"
#include "profile.h"

/////////////////////////////////////////////////////////////////////////////
// CKey window

enum KEY_TYPE { NORMAL_KEY, PUSHED_KEY, DROPPABLE_KEY, ORIGINAL_KEY, REMAPPED_KEY, REMAPPED_PUSHED_KEY };

class CKey : public CButton
{
// Construction
public:
	CKey(int nKey = 0, int nKeyType = NORMAL_KEY, HKEY_TYPE hkeyType = CURRENT_USER);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKey)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetKeyType();
	void SetKeyType(int nKeyType);
	virtual ~CKey();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKey)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	BOOL IsDroppableKey(int nKey);
	int m_nDroppableKeyType;
	int m_nDroppableKey;
	int m_nKey;
	int m_nKeyType;
	HKEY_TYPE m_HkeyType;
	void SetDraggingCursor();
	void SetNormalCursor();
	void SetNoCursor();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEY_H__509440FD_B54D_4C17_AAA1_C394820F4FB6__INCLUDED_)
