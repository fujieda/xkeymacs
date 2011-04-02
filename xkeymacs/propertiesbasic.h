#if !defined(AFX_PROPERTIESBASIC_H__04D80B48_2913_4D7C_86A4_3049877B6996__INCLUDED_)
#define AFX_PROPERTIESBASIC_H__04D80B48_2913_4D7C_86A4_3049877B6996__INCLUDED_

class CProperties;
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertiesBasic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertiesBasic dialog

class CPropertiesBasic : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropertiesBasic)

// Construction
public:
	void EnableControl();
	void GetDialogData();
	void InitChanged(BOOL bChanged);
	void SetDialogData(CString szApplicationName);
	void SetAllDialogData(UINT nCheck, BOOL bChanged);
	CPropertiesBasic();
	~CPropertiesBasic();

// Dialog Data
	//{{AFX_DATA(CPropertiesBasic)
	enum { IDD = IDD_PROPERTIES_BASIC };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropertiesBasic)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropertiesBasic)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAlt();
	afx_msg void OnC_();
	afx_msg void OnCa();
	afx_msg void OnCb();
	afx_msg void OnCbackslash();
	afx_msg void OnCbackslash106();
	afx_msg void OnCd();
	afx_msg void OnCdigit();
	afx_msg void OnCe();
	afx_msg void OnCf();
	afx_msg void OnCg();
	afx_msg void OnCh();
	afx_msg void OnCi();
	afx_msg void OnCk();
	afx_msg void OnCl();
	afx_msg void OnCm();
	afx_msg void OnCms5();
	afx_msg void OnCn();
	afx_msg void OnCo();
	afx_msg void OnCo2();
	afx_msg void OnCp();
	afx_msg void OnCq();
	afx_msg void OnCr();
	afx_msg void OnCs();
	afx_msg void OnCslash();
	afx_msg void OnCspace();
	afx_msg void OnCt();
	afx_msg void OnCu();
	afx_msg void OnCv();
	afx_msg void OnCw();
	afx_msg void OnCxb();
	afx_msg void OnCxcc();
	afx_msg void OnCxcf();
	afx_msg void OnCxcj();
	afx_msg void OnCxcp();
	afx_msg void OnCxcs();
	afx_msg void OnCxct();
	afx_msg void OnCxcw();
	afx_msg void OnCxe();
	afx_msg void OnCxh();
	afx_msg void OnCxk();
	afx_msg void OnCxs0();
	afx_msg void OnCxs9();
	afx_msg void OnCxu();
	afx_msg void OnCy();
	afx_msg void OnCz();
	afx_msg void OnLeftAlt();
	afx_msg void OnMb();
	afx_msg void OnMd();
	afx_msg void OnMf();
	afx_msg void OnMi();
	afx_msg void OnMs1();
	afx_msg void OnMs5();
	afx_msg void OnMscomma();
	afx_msg void OnMsperiod();
	afx_msg void OnMv();
	afx_msg void OnMw();
	afx_msg void OnMy();
	afx_msg void OnRightAlt();
	afx_msg void OnEscape();
	afx_msg void OnSquareBra();
	virtual BOOL OnInitDialog();
	afx_msg void OnMdel();
	afx_msg void OnMf12();
	afx_msg void OnMt();
	afx_msg void OnMc();
	afx_msg void OnMl();
	afx_msg void OnMu();
	afx_msg void OnMx();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CToolTipCtrl m_ToolTip;
	void Changed(int nObjectID);
	CProperties* m_pProperties;
	BOOL m_bChanged[sizeof(Commands)/sizeof(Commands[0])];
	void UpdateDialogData(CString szApplicationName, BOOL bSaveAndValidate = TRUE);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIESBASIC_H__04D80B48_2913_4D7C_86A4_3049877B6996__INCLUDED_)
