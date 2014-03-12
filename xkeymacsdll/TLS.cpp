#include "StdAfx.h"
#include "TLS.h"

DWORD TLS::m_TlsIndex = TLS_OUT_OF_INDEXES;

bool TLS::Alloc()
{
	return (m_TlsIndex = TlsAlloc()) != TLS_OUT_OF_INDEXES;
}

void TLS::Free()
{
	FreeLocal();
	if (m_TlsIndex != TLS_OUT_OF_INDEXES)
		TlsFree(m_TlsIndex);
	m_TlsIndex = TLS_OUT_OF_INDEXES;
}

TLS *TLS::AllocLocal()
{
	if (m_TlsIndex == TLS_OUT_OF_INDEXES)
		return nullptr;
	LPVOID data = TlsGetValue(m_TlsIndex);
	if (!data) {
		data = LocalAlloc(LPTR, sizeof(TLS));
		if (data)
			TlsSetValue(m_TlsIndex, data);
	}
	return reinterpret_cast<TLS *>(data);
}

void TLS::FreeLocal()
{
	if (m_TlsIndex == TLS_OUT_OF_INDEXES)
		return;
	LPVOID data = TlsGetValue(m_TlsIndex);
	if (!data)
		return;
	LocalFree(data);
	TlsSetValue(m_TlsIndex, nullptr);
}

HHOOK TLS::GetKeyboardHook()
{
	TLS *tls = AllocLocal();
	return tls ? tls->m_KeyboardHook : nullptr;
}

void TLS::PutKeyboardHook(HHOOK hook)
{
	TLS *tls = AllocLocal();
	if (tls)
		tls->m_KeyboardHook = hook;
}