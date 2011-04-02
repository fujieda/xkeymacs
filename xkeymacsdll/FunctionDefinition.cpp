// FunctionDefinition.cpp: implementation of the CFunctionDefinition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FunctionDefinition.h"
#include "Commands.h"
#include "Utils.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFunctionDefinition::CFunctionDefinition()
{

}

CFunctionDefinition::CFunctionDefinition(CString szSymbol, CString szDefinition)
{
	SetSymbol(szSymbol);
	SetDefinition(szDefinition);
}

CFunctionDefinition::~CFunctionDefinition()
{

}

void CFunctionDefinition::SetSymbol(CString szSymbol)
{
	m_szSymbol = szSymbol;
}

void CFunctionDefinition::SetDefinition(CString szDefinition)
{
	m_szDefinition = szDefinition;
}

CString CFunctionDefinition::GetSymbol()
{
	return m_szSymbol;
}

CString CFunctionDefinition::GetDefinition()
{
	return m_szDefinition;
}

void CFunctionDefinition::ClearKey(int nApplicationID)
{
	if (nApplicationID < 0 || MAX_APP <= nApplicationID) {
		return;
	}

	while (!m_oKey[nApplicationID].IsEmpty()) {
		KeyBind *pKeyBind = (KeyBind *)m_oKey[nApplicationID].GetHead();
		delete pKeyBind;
		pKeyBind = NULL;
		m_oKey[nApplicationID].RemoveHead();
	}
}

void CFunctionDefinition::SetKey(int nApplicationID, int nCommandType, int nKey)
{
	if (nApplicationID < 0 || MAX_APP <= nApplicationID) {
		return;
	}

	try {
		KeyBind *pKeyBind = new KeyBind;

		if (pKeyBind) {
			pKeyBind->nCommandType = nCommandType;
			pKeyBind->bVk = (BYTE)nKey;
			pKeyBind->nControlID = 0;

			m_oKey[nApplicationID].AddTail((CObject *)pKeyBind);
		}
	}
	catch (CMemoryException* e) {
		e->Delete();
//		CUtils::Log("CFunctionDefinition::SetKey: 'new' threw an exception");
	}
}

int CFunctionDefinition::GetKeyNumber(int nApplicationID)
{
	if (nApplicationID < 0 || MAX_APP <= nApplicationID) {
		return 0;
	}

	return m_oKey[nApplicationID].GetCount();
}

void CFunctionDefinition::GetKey(int nApplicationID, int nKeyID, int *pCommandType, int *pKey)
{
	*pCommandType = 0;
	*pKey = 0;

	if (nApplicationID < 0 || MAX_APP <= nApplicationID) {
		return;
	}
	if (nKeyID < 0 || m_oKey[nApplicationID].GetCount() <= nKeyID) {
		return;
	}

	KeyBind *pKeyBind = (KeyBind *)m_oKey[nApplicationID].GetAt(m_oKey[nApplicationID].FindIndex(nKeyID));
	*pCommandType = pKeyBind->nCommandType;
	*pKey = pKeyBind->bVk;
}

void CFunctionDefinition::RemoveKey(int nApplicationID, int nCommandType, int nKey)
{
	if (nApplicationID < 0 || MAX_APP <= nApplicationID) {
		return;
	}

	for (POSITION pos = m_oKey[nApplicationID].GetHeadPosition(); pos; ) {
		POSITION currentPos = pos;
		KeyBind *pKeyBind = (KeyBind *)m_oKey[nApplicationID].GetNext(pos);
		if (pKeyBind->nCommandType == nCommandType && pKeyBind->bVk == nKey) {
			delete pKeyBind;
			pKeyBind = NULL;
			m_oKey[nApplicationID].RemoveAt(currentPos);
			break;
		}
	}
}
