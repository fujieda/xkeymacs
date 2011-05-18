// DotXkeymacs.cpp: implementation of the CDotXkeymacs class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <direct.h>
#include <Shlwapi.h>
#include "DotXkeymacs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CObList		CDotXkeymacs::m_oFunctionDefinition;
int			CDotXkeymacs::m_nIndex[MAX_APP][MAX_COMMAND_TYPE][MAX_KEY] = {'\0'};
const TCHAR	CDotXkeymacs::m_szExt[] = _T("xkeymacs");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDotXkeymacs::CDotXkeymacs()
{

}

CDotXkeymacs::~CDotXkeymacs()
{

}

void CDotXkeymacs::Load(LPCTSTR lpszFileName)
{
	CStdioFile oDotXkeymacs;
	if (oDotXkeymacs.Open(lpszFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyWrite | CFile::typeText)) {
		CString szRead;
		while (oDotXkeymacs.ReadString(szRead)) {
			if (IsFunctionDefinition(szRead)) {
				CFunctionDefinition *pFunctionDefinition = new CFunctionDefinition(GetSymbol(szRead), GetDefinition(szRead));

				// Delete a listed definition which has the same symbol as a new one.
				for (POSITION currentPos, pos = m_oFunctionDefinition.GetHeadPosition(); (currentPos = pos) != NULL; ) {
					CFunctionDefinition *pCurrentDefinition = (CFunctionDefinition *)m_oFunctionDefinition.GetNext(pos);

					if (pCurrentDefinition->GetSymbol() == pFunctionDefinition->GetSymbol()) {
						CFunctionDefinition *pOverwritten = (CFunctionDefinition *)m_oFunctionDefinition.GetAt(currentPos);
						m_oFunctionDefinition.RemoveAt(currentPos);
						delete pOverwritten;
					}
				}

				m_oFunctionDefinition.AddTail((CObject *)pFunctionDefinition);
			}
		}
	}
}

void CDotXkeymacs::LoadMainData(LPCTSTR lpszFileName)
{
	TCHAR szModuleFileName[MAX_PATH] = {'\0'};
	TCHAR szDrive[_MAX_DRIVE] = {'\0'};
	TCHAR szDir[_MAX_DIR] = {'\0'};

	if (GetModuleFileName(NULL, szModuleFileName, sizeof(szModuleFileName))) {
		_tsplitpath_s(szModuleFileName, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, NULL, 0, NULL, 0);
	}

	TCHAR szOldPath[MAX_PATH] = {'\0'};	// This path is used by XKeymacs 3.22 and earlier
	_tmakepath_s(szOldPath, szDrive, szDir, lpszFileName, m_szExt);

	PathAppend(szDir, _T("etc"));
	TCHAR szEtc[MAX_PATH] = {'\0'};
	_tmakepath_s(szEtc, szDrive, szDir, NULL, NULL);
	(void)_tmkdir(szEtc);	// make etc directory if needed

	TCHAR szPath[MAX_PATH] = {'\0'};
	_tmakepath_s(szPath, szDrive, szDir, lpszFileName, m_szExt);

	if (_trename(szOldPath, szPath)) {			// try to move old file as backup when rename returns an error because files exist in both directorys
		TCHAR szBackupPath[MAX_PATH] = {'\0'};
		TCHAR szBackupFlag[_MAX_FNAME] = _T("~");
		_tcscat_s(szBackupFlag, lpszFileName);
		_tmakepath_s(szBackupPath, szDrive, szDir, szBackupFlag, m_szExt);
		(void)_trename(szOldPath, szBackupPath);	// do nothing when a backup file has existed already
	}

	Load(szPath);
}

void CDotXkeymacs::LoadUserData(LPCTSTR lpszFileName)
{
	TCHAR szPath[MAX_PATH] = {'\0'};
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, TRUE)) {
		_tmakepath_s(szPath, NULL, szPath, lpszFileName, m_szExt);
		Load(szPath);
	}
}

void CDotXkeymacs::Load()
{
	static LPCTSTR szFileName = _T("dot");

	ClearFunctionDefinition();
	LoadMainData(GetLanguage());	// just for localization
	LoadMainData(szFileName);
	LoadUserData(szFileName);	// overwrite main data
}

BOOL CDotXkeymacs::IsFunctionDefinition(CString szFunctionDefinition)
{
	return !_tcsncmp(szFunctionDefinition, CString(MAKEINTRESOURCE(IDS_FSET)), _tcslen(CString(MAKEINTRESOURCE(IDS_FSET))));
}

CString CDotXkeymacs::GetSymbol(CString szFunctionDefinition)
{
	const int nFirst = _tcslen(CString(MAKEINTRESOURCE(IDS_FSET))) + _tcslen(_T("'"));
	return szFunctionDefinition.Mid(nFirst, szFunctionDefinition.Find(_T(' '), nFirst) - nFirst);
}

CString CDotXkeymacs::GetDefinition(CString szFunctionDefinition)
{
	const int nFirst = szFunctionDefinition.Find(_T(' '), _tcslen(CString(MAKEINTRESOURCE(IDS_FSET)))) + _tcslen(_T("'"));
	return szFunctionDefinition.Mid(nFirst, szFunctionDefinition.GetLength() - nFirst - _tcslen(_T(")")));
}

void CDotXkeymacs::ClearFunctionDefinition()
{
	while (!m_oFunctionDefinition.IsEmpty()) {
		CFunctionDefinition *pFunctionDefinition = (CFunctionDefinition *)m_oFunctionDefinition.GetHead();
		delete pFunctionDefinition;
		pFunctionDefinition = NULL;
		m_oFunctionDefinition.RemoveHead();
	}
	memset(m_nIndex, -1, sizeof(m_nIndex));
}

int CDotXkeymacs::GetFunctionNumber()
{
	return m_oFunctionDefinition.GetCount();
}

CString CDotXkeymacs::GetFunctionSymbol(int nIndex)
{
	if (nIndex < 0 || m_oFunctionDefinition.GetCount() <= nIndex) {
		return CString();
	}

	if (CFunctionDefinition *pFunctionDefinition = (CFunctionDefinition *)m_oFunctionDefinition.GetAt(m_oFunctionDefinition.FindIndex(nIndex))) {
		return pFunctionDefinition->GetSymbol();
	}
	return CString();
}

CString CDotXkeymacs::GetFunctionDefinition(int nIndex)
{
	if (nIndex < 0 || m_oFunctionDefinition.GetCount() <= nIndex) {
		return CString();
	}

	if (CFunctionDefinition *pFunctionDefinition = (CFunctionDefinition *)m_oFunctionDefinition.GetAt(m_oFunctionDefinition.FindIndex(nIndex))) {
		return pFunctionDefinition->GetDefinition();
	}
	return CString();
}

CString CDotXkeymacs::GetFunctionDefinition(CString szSymbol)
{
	for (POSITION pos = m_oFunctionDefinition.GetHeadPosition(); pos;) {
		CFunctionDefinition *pFunctionDefinition = (CFunctionDefinition *)m_oFunctionDefinition.GetNext(pos);
		if (!pFunctionDefinition->GetSymbol().Compare(szSymbol)) {
			return pFunctionDefinition->GetDefinition();
		}
	}

	return CString(_T("Undefined Command"));
}

void CDotXkeymacs::ClearKey(int nIndex, int nApplicationID)
{
	if (nIndex < 0 || m_oFunctionDefinition.GetCount() <= nIndex) {
		return;
	}

	if (CFunctionDefinition *pFunctionDefinition = (CFunctionDefinition *)m_oFunctionDefinition.GetAt(m_oFunctionDefinition.FindIndex(nIndex))) {
		pFunctionDefinition->ClearKey(nApplicationID);
	}

	for (int nCommandType = 0; nCommandType < MAX_COMMAND_TYPE; ++nCommandType) {
		for (int nKey = 0; nKey < MAX_KEY; ++nKey) {
			if (m_nIndex[nApplicationID][nCommandType][nKey] == nIndex) {
				m_nIndex[nApplicationID][nCommandType][nKey] = -1;
			}
		}
	}
}

void CDotXkeymacs::SetKey(int nIndex, int nApplicationID, int nCommandType, int nKey)
{
	if (nIndex < 0 || m_oFunctionDefinition.GetCount() <= nIndex) {
		return;
	}

	if (CFunctionDefinition *pFunctionDefinition = (CFunctionDefinition *)m_oFunctionDefinition.GetAt(m_oFunctionDefinition.FindIndex(nIndex))) {
		pFunctionDefinition->SetKey(nApplicationID, nCommandType, nKey);
		m_nIndex[nApplicationID][nCommandType][nKey] = nIndex;
	}
}

int CDotXkeymacs::GetIndex(CString szSymbol)
{
	int nIndex = 0;
	for (nIndex = 0; nIndex < m_oFunctionDefinition.GetCount(); ++nIndex) {
		CFunctionDefinition *pFunctionDefinition = (CFunctionDefinition *)m_oFunctionDefinition.GetAt(m_oFunctionDefinition.FindIndex(nIndex));
		if (!pFunctionDefinition->GetSymbol().Compare(szSymbol)) {
			return nIndex;
		}
	}
	return -1;
}

int CDotXkeymacs::GetKeyNumber(int nIndex, int nApplicationID)
{
	if (nIndex < 0 || m_oFunctionDefinition.GetCount() <= nIndex) {
		return 0;
	}

	CFunctionDefinition *pFunctionDefinition = (CFunctionDefinition *)m_oFunctionDefinition.GetAt(m_oFunctionDefinition.FindIndex(nIndex));
	return pFunctionDefinition->GetKeyNumber(nApplicationID);
}

void CDotXkeymacs::GetKey(int nIndex, int nApplicationID, int nKeyID, int *pCommandType, int *pKey)
{
	if (nIndex < 0 || m_oFunctionDefinition.GetCount() <= nIndex) {
		return;
	}

	CFunctionDefinition *pFunctionDefinition = (CFunctionDefinition *)m_oFunctionDefinition.GetAt(m_oFunctionDefinition.FindIndex(nIndex));
	pFunctionDefinition->GetKey(nApplicationID, nKeyID, pCommandType, pKey);
}

int CDotXkeymacs::GetIndex(int nApplicationID, int nCommandType, int nKey)
{
	return m_nIndex[nApplicationID][nCommandType][nKey];
}

void CDotXkeymacs::RemoveKey(const int nIndex, const int nApplicationID, const int nCommandType, const int nKey)
{
	if (nIndex < 0 || m_oFunctionDefinition.GetCount() <= nIndex) {
		return;
	}

	CFunctionDefinition *pFunctionDefinition = (CFunctionDefinition *)m_oFunctionDefinition.GetAt(m_oFunctionDefinition.FindIndex(nIndex));
	if (pFunctionDefinition) {
		pFunctionDefinition->RemoveKey(nApplicationID, nCommandType, nKey);
	}
}

void CDotXkeymacs::RemoveKey(const int nApplicationID, const int nCommandType, const int nKey)
{
	for (int nIndex = 0; nIndex < m_oFunctionDefinition.GetCount(); ++nIndex) {
		RemoveKey(nIndex, nApplicationID, nCommandType, nKey);
	}
}

LPCTSTR CDotXkeymacs::GetLanguage()
{
	LPCTSTR szLanguage = _T("unknown");

	TCHAR lptstrFilename[MAX_PATH] = {'\0'};
	TCHAR windir[MAX_PATH] = "";
	size_t len;
	_tgetenv_s(&len, windir, _T("windir"));
	_tmakepath_s(lptstrFilename, NULL, windir, _T("explorer"), _T("exe"));
	DWORD dwLen = GetFileVersionInfoSize(lptstrFilename, NULL);

	if (dwLen) {
		LPVOID lpData = malloc(dwLen);

		if (lpData && GetFileVersionInfo(lptstrFilename, NULL, dwLen, lpData)) {
			struct Translate {
				WORD wLanguage;
				WORD wCodePage;
			} *lpTranslate;
			UINT cbTranslate = 0;

			if (VerQueryValue(lpData, _T("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &cbTranslate) && sizeof(Translate) <= cbTranslate) {
				for (int i = 0; i < sizeof(Languages)/sizeof(Languages[0]); ++i) {
					if (Languages[i].wLanguage == lpTranslate->wLanguage) {
						szLanguage = Languages[i].szLanguage;
						break;
					}
				}
			}
		}

		free(lpData);
	}

	return szLanguage;
}
