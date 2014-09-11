#pragma once
#include "TSFHandler.h"

class TLS
{
public:
	static bool Alloc();
	static void Free();
	static void FreeLocal();
	static HHOOK GetKeyboardHook();
	static void PutKeyboardHook(HHOOK hook);
	static TSFHandler *GetTSFHandler();
	static void PutTSFHandler(TSFHandler *tsf);

private:
	static DWORD m_TlsIndex;
	static TLS *AllocLocal();
	HHOOK m_KeyboardHook;
	TSFHandler *m_TSFHandler;
};

