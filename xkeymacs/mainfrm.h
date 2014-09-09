// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__CF58719A_1B31_4BC0_8E55_06E0AE5BED2E__INCLUDED_)
#define AFX_MAINFRM_H__CF58719A_1B31_4BC0_8E55_06E0AE5BED2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "about.h"
#include "optionsdlg.h"
#include "104Keyboard.h"
#include "109Keyboard.h"
#include "properties.h"

enum KEYBOARD_TYPE { UNKNOWN_KEYBOARD, ENGLISH_KEYBOARD, JAPANESE_KEYBOARD, MAX_KEYBOARD_TYPE };

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
public:
	CMainFrame();
protected:
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAbout();
	afx_msg void OnQuit();
	afx_msg void OnDisable();
	afx_msg void OnProperties();
	afx_msg void OnOptions();
	afx_msg void OnKeyboard();
	afx_msg void OnImport();
	afx_msg void OnExport();
	afx_msg void OnReset();
	afx_msg void OnHelpFinder();
	afx_msg void OnKeyboardAnyUser();
	//}}AFX_MSG
	afx_msg LRESULT OnPowerBroadCast(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bKeyboardDlgExist[MAX_KEYBOARD_TYPE][MAX_HKEY_TYPE];
	int m_nResultKeyboardDlg[MAX_KEYBOARD_TYPE][MAX_HKEY_TYPE];
	C109Keyboard *m_p109KeyboardDlg[MAX_HKEY_TYPE];
	C104Keyboard *m_p104KeyboardDlg[MAX_HKEY_TYPE];
	void PopUpKeyboardDialog(HKEY_TYPE hkeyType);
	void SetForegroundTheDialog(CDialog *pDialog);
	void CloseDialog(CDialog *pDialog, int *pnResult);
	void SetForegroundSomeDialog();
	void PopUpOptionsDialog();
	void PopUpDialog(CDialog *pDialog, int *pnDialogExist, BOOL *pbDialogExist);
	void PopUpAboutDialog();
	void PopUpPropertiesDialog();
	void ShowRightMenu(ICON_TYPE icon);
	BOOL m_bPropertiesDlgExist;
	BOOL m_bOptionsDlgExist;
	BOOL m_bAboutDlgExist;
	COptionsDlg *m_pOptionsDlg;
	int m_nResultOptionsDlg;
	CAboutDlg *m_pAboutDlg;
	int	m_nResultAboutDlg;
	int m_nResultPropertiesDlg;
	CProperties *m_pPropertiesDlg;
	// notify icons
	static BOOL m_bIcon[MAX_ICON_TYPE];
	static HICON m_hIcon[MAX_ICON_TYPE][MAX_STATUS];
	static DWORD m_dwOldMessage[MAX_ICON_TYPE];
	static NOTIFYICONDATA m_stNtfyIcon[MAX_ICON_TYPE];
	static NOTIFYICONDATA m_stOldNtfyIcon[MAX_ICON_TYPE];
	void SetIconData(ICON_TYPE icon, int tip, int on, int off, int reg);
	static void AddShell_NotifyIcon(ICON_TYPE icon);
	static void DeleteShell_NotifyIcon(ICON_TYPE icon);
	static void DoShell_NotifyIcon(ICON_TYPE icon, DWORD dwMessage);
	static void DeleteAllShell_NotifyIcon();
	static void AddAllShell_NotifyIcon();
	HANDLE m_hThread;
	void StartPollThread();
	void TerminatePollThread();
	static DWORD WINAPI PollMessage(LPVOID lpParam);
	static HANDLE CreateNamedPipeWithLowIntegrityAccess();
public:
	void EnableShell_NotifyIcon(ICON_TYPE icon, BOOL bEnable);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__CF58719A_1B31_4BC0_8E55_06E0AE5BED2E__INCLUDED_)
