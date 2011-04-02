#if !defined(AFX_PROPERTIESADVANCED_H__41C61B2B_97BA_4015_8F1E_CA65AC628E42__INCLUDED_)
#define AFX_PROPERTIESADVANCED_H__41C61B2B_97BA_4015_8F1E_CA65AC628E42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertiesAdvanced.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertiesAdvanced dialog

class CPropertiesAdvanced : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropertiesAdvanced)

// Construction
public:
	void EnableControl();
	void GetDialogData();
	void SetDialogData(CString szApplicationName);
	CPropertiesAdvanced();
	~CPropertiesAdvanced();

// Dialog Data
	//{{AFX_DATA(CPropertiesAdvanced)
	enum { IDD = IDD_PROPERTIES_ADVANCED };
	CStatic	m_cCurrentlyAssigned;
	CButton	m_cResetAll;
	CButton	m_cRemove;
	CEdit	m_cNewKey;
	CStatic	m_cDescription;
	CListBox	m_cCurrentKeys;
	CListBox	m_cCommands;
	CComboBox	m_cCategory;
	CButton	m_cAssign;
	BOOL	m_bEnableCUA;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropertiesAdvanced)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropertiesAdvanced)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCategory();
	afx_msg void OnSelchangeCommands();
	afx_msg void OnSelchangeCurrentKeys();
	afx_msg void OnSetfocusNewKey();
	afx_msg void OnAssign();
	afx_msg void OnRemove();
	afx_msg void OnResetAll();
	afx_msg void OnCX();
	afx_msg void OnKillfocusNewKey();
	afx_msg void OnEnableCua();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	static int m_nApplicationID;
	static BOOL IsFooDown(CString szCommandName);
	static BOOL IsShiftDown();
	static BOOL IsMetaDown();
	static BOOL IsCtrlDown();
	void InitCommandIDs();
	static void SetCommandID(int nCommandType, int nKey, int nCommandID);
	static int m_nCommandIDs[MAX_COMMAND_TYPE][MAX_KEY];
	static void SetNewKey();
	static int m_nAssignKey;
	static int m_nAssignCommandType;
	static BOOL m_bShift;
	static BOOL m_bM;
	static BOOL m_bC;
	static BOOL m_bC_x;
	static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
	static HHOOK m_hKeyboardHook;
	static CListBox * m_pCurrentKeys;
	static CStatic * m_pCurrentlyAssigned;
	static CButton * m_pAssign;
	static CEdit * m_pNewKey;
	int m_nRemoveKey;
	int m_nRemoveCommandType;
	void ClearNewKey();
	static int m_nCommandID;
	void SetCurrentKeys();
	void SetCommands();
	void InitCategoryList();
	CProperties* m_pProperties;
	void UpdateDialogData(CString szApplicationName, BOOL bSaveAndValidate = TRUE);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIESADVANCED_H__41C61B2B_97BA_4015_8F1E_CA65AC628E42__INCLUDED_)
