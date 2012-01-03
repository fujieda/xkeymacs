// DotXkeymacs.h: interface for the CDotXkeymacs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOTXKEYMACS_H__0538C427_5733_47DA_B2C6_2E957EEFC59D__INCLUDED_)
#define AFX_DOTXKEYMACS_H__0538C427_5733_47DA_B2C6_2E957EEFC59D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../xkeymacsdll/defs.h"

class CDotXkeymacs  
{
public:
	static void RemoveKey(const int nAppID, const int nType, const int nKey);
	static void RemoveKey(const int nIndex, const int nAppID, const int nType, const int nKey);
	static CString GetFunctionDefinition(int nIndex);
	static int GetIndex(int nAppID, int nType, int nKey);
	static void GetKey(int nIndex, int nAppID, int nKeyID, int *pCommandType, int *pKey);
	static int GetKeyNumber(int nIndex, int nAppID);
	static int GetIndex(CString szSymbol);
	static void SetKey(int nIndex, int nAppID, int nType, int nKey);
	static void ClearKey(int nIndex, int nAppID);
	static CString GetFunctionDefinition(CString szSymbol);
	static CString GetFunctionSymbol(int nIndex);
	static int GetFunctionNumber();
	static void Load();

private:
	static LPCTSTR GetLanguage();
	static const TCHAR m_szExt[];
	static int m_nIndex[MAX_APP][MAX_COMMAND_TYPE][MAX_KEY];
	static void ClearFunctionDefinition();
	static CString GetDefinition(CString szFunctionDefinition);
	static CString GetSymbol(CString szFunctionDefinition);
	static BOOL IsFunctionDefinition(CString szFunctionDefinition);
	static CObList m_oFunctionDefinition;
	static void LoadUserData(LPCTSTR lpszFileName);
	static void LoadMainData(LPCTSTR lpszFileName);
	static void Load(LPCTSTR lpszFileName);
};

#endif // !defined(AFX_DOTXKEYMACS_H__0538C427_5733_47DA_B2C6_2E957EEFC59D__INCLUDED_)
