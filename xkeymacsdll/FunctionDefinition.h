// FunctionDefinition.h: interface for the CFunctionDefinition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNCTIONDEFINITION_H__22E7286C_E86F_4964_896F_28A66E94A7A2__INCLUDED_)
#define AFX_FUNCTIONDEFINITION_H__22E7286C_E86F_4964_896F_28A66E94A7A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "defs.h"

class AFX_EXT_CLASS CFunctionDefinition
{
public:
	void RemoveKey(int nAppID, int nType, int nKey);
	void GetKey(int nAppID, int nKeyID, int *pCommandType, int *pKey);
	int GetKeyNumber(int nAppID);
	void SetKey(int nAppID, int nType, int nKey);
	void ClearKey(int nAppID);
	CString GetDefinition();
	CString GetSymbol();
	void SetDefinition(CString szDefinition);
	void SetSymbol(CString szSymbol);
	CFunctionDefinition(CString szSymbol, CString szDefinition);

private:
	CObList m_oKey[MAX_APP];
	CString m_szDefinition;
	CString m_szSymbol;
};

#endif // !defined(AFX_FUNCTIONDEFINITION_H__22E7286C_E86F_4964_896F_28A66E94A7A2__INCLUDED_)
