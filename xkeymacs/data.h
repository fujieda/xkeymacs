#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CData  
{
public:
	void ClearAll();
	void Set326Compatible(BOOL b326Compatible);
	BOOL Get326Compatible();
	int GetWindowTextType();
	void SetWindowTextType(int nWindowTextType);
	CString GetWindowText();
	void SetWindowText(LPCTSTR lpszWindowText);
	void SetUseDialogSetting(BOOL bUseDialogSetting);
	BOOL GetUseDialogSetting();
	BOOL GetEnableCUA();
	void SetEnableCUA(BOOL bEnableCUA);
	BOOL GetIgnoreUndefinedC_x();
	void SetIgnoreUndefinedC_x(BOOL bIgnoreUndefinedC_x);
	BOOL GetIgnoreUndefinedMetaCtrl();
	void SetIgnoreUndefinedMetaCtrl(BOOL bIgnoreUndefinedMetaCtrl);
	void SetSettingStyle(int nSettingStyle);
	int GetSettingStyle();
	void SetKillRingMax(int nKillRingMax);
	int GetKillRingMax();
	CString GetApplicationTitle();
	void SetApplicationTitle(LPCTSTR lpszApplicationTitle);

	int GetCommandID(int nCommandType, int nKey);
	void SetCommandID(int nCommandType, int nKey, int nCommandID);
	BOOL GetAtIbeamCursorOnly(int nCommandType, int nKey);
	void SetAtIbeamCursorOnly(int nCommandType, int nKey, BOOL bAtIbeamCursorOnly);
	CString GetApplicationName();
	void SetApplicationName(LPCTSTR lpszApplicationName);
	CData();
	virtual ~CData();
private:
	BOOL m_b326Compatible;
	int m_nWindowTextType;
	CString m_strWindowText;
	BOOL m_bEnableCUA;
	BOOL m_bUseDialogSetting;
	BOOL m_bIgnoreUndefinedC_x;
	BOOL m_bIgnoreUndefinedMetaCtrl;
	int m_nSettingStyle;
	int m_nKillRingMax;
	CString m_strApplicationTitle;
	CString m_strApplicationName;
	int m_nCommandID[MAX_COMMAND_TYPE][MAX_KEY];
	BOOL m_bAtIbeamCursorOnly[MAX_COMMAND_TYPE][MAX_KEY];
};

#endif // DATA_H_INCLUDED
