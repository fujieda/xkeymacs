#if !defined(AFX_PROPERTIESLIST_H__0BC352B3_8869_46D9_873B_D65D705F1228__INCLUDED_)
#define AFX_PROPERTIESLIST_H__0BC352B3_8869_46D9_873B_D65D705F1228__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertiesList.h : header file
//

#include "stdafx.h"
#include "resource.h"
enum SORT_TYPE { SORT_CATEGORY, SORT_COMMAND, SORT_KEY };
class CProperties;

/////////////////////////////////////////////////////////////////////////////
// CPropertiesList dialog

class CPropertiesList : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropertiesList)

// Construction
public:
	void EnableControl();
	void GetDialogData();
	void SetDialogData();
	CPropertiesList();
	~CPropertiesList();

// Dialog Data
	//{{AFX_DATA(CPropertiesList)
	enum { IDD = IDD_PROPERTIES_LIST };
	CListCtrl	m_cPropertiesList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropertiesList)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropertiesList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnColumnclickPropertiesList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nKeyWidth;
	int m_nCommandWidth;
	int m_nCategoryWidth;
	static int Sort(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort, SORT_TYPE sortType);
	void InsertItem(LPCTSTR szCategory, LPCTSTR szCommandName, LPCTSTR szKey);
	void SortItem(int nColumn);
	void DisplayIndicator();
	static BOOL m_bSortAscending;
	int m_nSelectedColumn;
	static int CALLBACK SortKey(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK SortCommand(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK SortCategory(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	CProperties* m_pProperties;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#include "properties.h"

#endif // !defined(AFX_PROPERTIESLIST_H__0BC352B3_8869_46D9_873B_D65D705F1228__INCLUDED_)
