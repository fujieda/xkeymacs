#ifndef KEYBOARDLAYOUT_H_INCLUDED
#define KEYBOARDLAYOUT_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "profile.h"
#include "key.h"

struct ScanCode
{
	BYTE nScanCode;
	BYTE nPrefix;	// 0x00, 0xE0 or 0xE1	cf. Keyboard Scan Code Specification -- 16
};

struct ScanCodeMapping
{
	ScanCode current;
	ScanCode original;
};

struct KeyboardLayout
{
	ScanCode scancode;
	int nBaseControlID;
	int nCurrentControlID;
	int nToolTipID;
	CKey *pBaseKey;
	CKey *pCurrentKey;
};

class CKeyboardLayout: public CDialog
{
private:
	static KeyboardLayout m_KeyboardLayouts[112];
	static ScanCode m_CurrentScanCodeMap[MAX_HKEY_TYPE][4][256];
	static ScanCode m_ScanCodeMap[MAX_HKEY_TYPE][4][256];
	CToolTipCtrl m_ToolTip;
	int GetControlID(const ScanCode scancode, const BOOL bBase);
	BYTE PrefixID2Code(int nPrefixID);
	int Prefix2ID(BYTE nPrefix);
	DWORD GetScanCodeLength(HKEY_TYPE hkeyType);
	BOOL ChangedKeyboardLayout(HKEY_TYPE hkeyType);
	virtual BOOL PreTranslateMessage(MSG *pMsg);
protected:
	HKEY_TYPE m_HkeyType;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	void InitKeyboardLayout();
	void CKeyboardLayout::DestroyKeyboardLayout();
	BOOL GetScanCodeMap(HKEY_TYPE hkeyType, ScanCode original, ScanCode *current);
	int GetBaseControlID(const ScanCode scancode);
	int GetCurrentControlID(const ScanCode scancode);
	int LostKeyWarning(HKEY_TYPE hkeyType);
	void SaveScanCodeMap(HKEY_TYPE hkeyType);
	void LoadScanCodeMap(HKEY_TYPE hkeyType);
	DECLARE_MESSAGE_MAP()
public:
	CToolTipCtrl *ToolTip();
	KeyboardLayout *GetKeyboardLayout(int nKey, BOOL bBase = FALSE);
	void SetScanCodeMap(HKEY_TYPE hkeyType, ScanCodeMapping mapping);
	int GetPointedKey(const CPoint point);
	virtual int GetToolTipID(int nToolTipID);
	CKeyboardLayout(const HKEY_TYPE hkey_type, UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CKeyboardLayout();
};

#endif // KEYBOARDLAYOUT_H_INCLUDED
