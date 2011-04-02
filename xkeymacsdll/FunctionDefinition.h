// FunctionDefinition.h: interface for the CFunctionDefinition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNCTIONDEFINITION_H__22E7286C_E86F_4964_896F_28A66E94A7A2__INCLUDED_)
#define AFX_FUNCTIONDEFINITION_H__22E7286C_E86F_4964_896F_28A66E94A7A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xkeymacsdll.h"

class AFX_EXT_CLASS CFunctionDefinition
{
public:
	void RemoveKey(int nApplicationID, int nCommandType, int nKey);
	void GetKey(int nApplicationID, int nKeyID, int *pCommandType, int *pKey);
	int GetKeyNumber(int nApplicationID);
	void SetKey(int nApplicationID, int nCommandType, int nKey);
	void ClearKey(int nApplicationID);
	CString GetDefinition();
	CString GetSymbol();
	void SetDefinition(CString szDefinition);
	void SetSymbol(CString szSymbol);
	CFunctionDefinition();
	CFunctionDefinition(CString szSymbol, CString szDefinition);
	virtual ~CFunctionDefinition();

private:
	CObList m_oKey[MAX_APP];
	CString m_szDefinition;
	CString m_szSymbol;
};

#endif // !defined(AFX_FUNCTIONDEFINITION_H__22E7286C_E86F_4964_896F_28A66E94A7A2__INCLUDED_)
