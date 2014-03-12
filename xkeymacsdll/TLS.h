#pragma once
class TLS
{
public:
	static bool Alloc();
	static void Free();
	static void FreeLocal();
	static HHOOK GetKeyboardHook();
	static void PutKeyboardHook(HHOOK hook);

private:
	static DWORD m_TlsIndex;
	static TLS *m_LocalData;
	HHOOK m_KeyboardHook;
	static TLS *AllocLocal();
};

