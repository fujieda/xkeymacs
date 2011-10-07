// FunctionDefinition.cpp: implementation of the CFunctionDefinition class.
//
//////////////////////////////////////////////////////////////////////

#include "FunctionDefinition.h"
#include "Utils.h"

CFunctionDefinition::CFunctionDefinition(CString szSymbol, CString szDefinition)
{
	SetSymbol(szSymbol);
	SetDefinition(szDefinition);
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

void CFunctionDefinition::ClearKey(int nAppID)
{
	if (nAppID < 0 || MAX_APP <= nAppID) {
		return;
	}

	while (!m_oKey[nAppID].IsEmpty()) {
		KeyBind *pKeyBind = (KeyBind *)m_oKey[nAppID].GetHead();
		delete pKeyBind;
		pKeyBind = NULL;
		m_oKey[nAppID].RemoveHead();
	}
}

void CFunctionDefinition::SetKey(int nAppID, int nType, int nKey)
{
	if (nAppID < 0 || MAX_APP <= nAppID) {
		return;
	}

	try {
		KeyBind *pKeyBind = new KeyBind;

		if (pKeyBind) {
			pKeyBind->nType = nType;
			pKeyBind->bVk = (BYTE)nKey;
			pKeyBind->nControlID = 0;

			m_oKey[nAppID].AddTail((CObject *)pKeyBind);
		}
	}
	catch (CMemoryException* e) {
		e->Delete();
//		CUtils::Log("CFunctionDefinition::SetKey: 'new' threw an exception");
	}
}

int CFunctionDefinition::GetKeyNumber(int nAppID)
{
	if (nAppID < 0 || MAX_APP <= nAppID) {
		return 0;
	}

	return m_oKey[nAppID].GetCount();
}

void CFunctionDefinition::GetKey(int nAppID, int nKeyID, int *pCommandType, int *pKey)
{
	*pCommandType = 0;
	*pKey = 0;

	if (nAppID < 0 || MAX_APP <= nAppID) {
		return;
	}
	if (nKeyID < 0 || m_oKey[nAppID].GetCount() <= nKeyID) {
		return;
	}

	KeyBind *pKeyBind = (KeyBind *)m_oKey[nAppID].GetAt(m_oKey[nAppID].FindIndex(nKeyID));
	*pCommandType = pKeyBind->nType;
	*pKey = pKeyBind->bVk;
}

void CFunctionDefinition::RemoveKey(int nAppID, int nType, int nKey)
{
	if (nAppID < 0 || MAX_APP <= nAppID) {
		return;
	}

	for (POSITION pos = m_oKey[nAppID].GetHeadPosition(); pos; ) {
		POSITION currentPos = pos;
		KeyBind *pKeyBind = (KeyBind *)m_oKey[nAppID].GetNext(pos);
		if (pKeyBind->nType == nType && pKeyBind->bVk == nKey) {
			delete pKeyBind;
			pKeyBind = NULL;
			m_oKey[nAppID].RemoveAt(currentPos);
			break;
		}
	}
}
