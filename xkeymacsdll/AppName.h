#pragma once

#include "defs.h"

class AFX_EXT_CLASS AppName
{
public:
	static void Init();
	static LPCTSTR GetAppName();
	static void SetIMEState(bool on);
	static void CorrectAppName(TCHAR (&text)[WINDOW_TEXT_LENGTH], TCHAR (&appName)[MAX_PATH]);
	static bool IsConsole();
	static bool Match(LPCTSTR name);
	template<size_t size> static bool Match(LPCTSTR (&names)[size])
	{
		for (int i = 0; i < size; ++i)
			if (!_tcsicmp(m_AppName, names[i]))
				return true;
		return false;
	}
private:
	static TCHAR m_AppName[MAX_PATH];
	static TCHAR m_IMEName[MAX_PATH];
	static TCHAR m_FallbackIMEName[MAX_PATH];
	static bool m_Inited;
	static bool m_IMEState;
	static void ConsoleAppName(CString& text, TCHAR (&appName)[MAX_PATH]);
	static void RemovePrefixes(CString& text);
	static bool IsCmdExe(const CString& text);
	static bool IsConsole(LPCTSTR appName);
};
