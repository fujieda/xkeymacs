#if !defined(AFX_PROPERTIES_H__13ABBD97_6862_4CDA_86ED_4863B4845A1B__INCLUDED_)
#define AFX_PROPERTIES_H__13ABBD97_6862_4CDA_86ED_4863B4845A1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <VersionHelpers.h>

#include "propertiesbasic.h"
#include "propertiesadvanced.h"
#include "propertieslist.h"

enum TAB_NAME { BASIC_TAB = 1, ADVANCED_TAB = 2, LIST_TAB = 4, ACTIVE_TAB = 8 };

// Properties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProperties dialog

class CProperties : public CDialog
{
// Construction
public:
	CProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProperties)
	enum { IDD = IDD_PROPERTIES };
	CEdit	m_cWindowText;
	CEdit	m_cKillRingMax;
	CButton	m_cUseDialogSetting;
	CButton	m_cSelectAll;
	CButton	m_cClearAll;
	CButton	m_cLoadDefault;
	CComboBox	m_cAppList;
	int		m_nSettingStyle;
	int		m_nKillRingMax;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL IsEnableControl();
	CString GetCurrentApplication();
	int GetApplicationID();
	void EnableControl(TAB_NAME tab = BASIC_TAB);
	void AddItem(const CString& appTitle, const CString& appName);
protected:
	// Generated message map functions
	//{{AFX_MSG(CProperties)
	virtual BOOL OnInitDialog();
	afx_msg void OnDropdownApplication();
	afx_msg void OnSelchangeApplication();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply();
	afx_msg void OnLoadDefault();
	afx_msg void OnSelectAll();
	afx_msg void OnClearAll();
	afx_msg void OnSettingDefault();
	afx_msg void OnSettingDisable();
	afx_msg void OnSettingSpecific();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nAppID;
	CPropertiesList m_list;
	CPropertiesAdvanced m_advanced;
	CPropertiesBasic m_basic;
	CPropertySheet m_sheet;
	CString m_appTitle;
	CString m_appName;
	void EnableLoadDefaultButton(BOOL bEnable);
	void EnableUseDefaultButton(BOOL bEnable);
	void EnableUseDialogSettingButton(BOOL bEnable);
	void GetDialogData();
	void InitAppList();
	void SetDialogData();
	void GetAppTitleAndName(const int nSel, CString& appTitle, CString& appName);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIES_H__13ABBD97_6862_4CDA_86ED_4863B4845A1B__INCLUDED_)
