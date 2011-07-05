#include "stdafx.h"
#include "data.h"

CData::CData()
{
	ClearAll();
}

CData::~CData()
{
}

void CData::SetWindowText(LPCTSTR lpszWindowText)
{
	m_nWindowTextType = CUtils::GetWindowTextType(lpszWindowText);
	if (m_nWindowTextType == IDS_WINDOW_TEXT_IGNORE) {
		m_strWindowText = _T('*');
	} else {
		m_strWindowText.Format(lpszWindowText);
	}
}

CString CData::GetWindowText()
{
	return m_strWindowText;
}

void CData::SetWindowTextType(int nWindowTextType)
{
	m_nWindowTextType = nWindowTextType;
}

int CData::GetWindowTextType()
{
	return m_nWindowTextType;
}

void CData::SetApplicationName(LPCTSTR lpszApplicationName)
{
	m_strApplicationName.Format(lpszApplicationName);
}

CString CData::GetApplicationName()
{
	return m_strApplicationName;
}

void CData::SetCommandID(int nType, int nKey, int nComID)
{
	ASSERT(nType >= 0 || nType < MAX_COMMAND_TYPE);
	ASSERT(nKey <= 0 || nKey < MAX_KEY);
	m_nCommandID[nType][nKey] = nComID;
}

int CData::GetCommandID(int nType, int nKey)
{
	ASSERT(nType >= 0 || nType < MAX_COMMAND_TYPE);
	ASSERT(nKey <= 0 || nKey < MAX_KEY);
	return m_nCommandID[nType][nKey];
}

void CData::ClearAll()
{
	ZeroMemory(m_nCommandID, sizeof(m_nCommandID));
	m_strApplicationName.Empty();
}

void CData::SetApplicationTitle(LPCTSTR lpszApplicationTitle)
{
	m_strApplicationTitle.Format(lpszApplicationTitle);
	// delete white space at the end of the application title.
	while (!m_strApplicationTitle.IsEmpty() &&
			_istspace(m_strApplicationTitle.GetAt(m_strApplicationTitle.GetLength() - 1)))
		m_strApplicationTitle.Delete(m_strApplicationTitle.GetLength() - 1);
}

CString CData::GetApplicationTitle()
{
	return m_strApplicationTitle;
}

void CData::SetKillRingMax(int nKillRingMax)
{
	m_nKillRingMax = nKillRingMax;
}

int CData::GetKillRingMax()
{
	return m_nKillRingMax;
}

int CData::GetSettingStyle()
{
	return m_nSettingStyle;
}

void CData::SetSettingStyle(int nSettingStyle)
{
	m_nSettingStyle = nSettingStyle;
}

void CData::SetIgnoreUndefinedMetaCtrl(BOOL bIgnoreUndefinedMetaCtrl)
{
	m_bIgnoreUndefinedMetaCtrl = bIgnoreUndefinedMetaCtrl;
}

BOOL CData::GetIgnoreUndefinedMetaCtrl()
{
	return m_bIgnoreUndefinedMetaCtrl;
}

void CData::SetIgnoreUndefinedC_x(BOOL bIgnoreUndefinedC_x)
{
	m_bIgnoreUndefinedC_x = bIgnoreUndefinedC_x;
}

BOOL CData::GetIgnoreUndefinedC_x()
{
	return m_bIgnoreUndefinedC_x;
}

void CData::SetEnableCUA(BOOL bEnableCUA)
{
	m_bEnableCUA = bEnableCUA;
}

BOOL CData::GetEnableCUA()
{
	return m_bEnableCUA;
}

void CData::SetUseDialogSetting(BOOL bUseDialogSetting)
{
	m_bUseDialogSetting = bUseDialogSetting;
}

BOOL CData::GetUseDialogSetting()
{
	return m_bUseDialogSetting;
}

BOOL CData::Get326Compatible()
{
	return m_b326Compatible;
}

void CData::Set326Compatible(BOOL b326Compatible)
{
	m_b326Compatible = b326Compatible;
}
