#pragma once

#include "StdAfx.h"
#include "defs.h"

class AppName
{
public:
	AppName(void);
	void SetAppName();
	LPCTSTR GetAppName() const;
	void SetIMEState(bool on);
	void CorrectAppName(TCHAR (&appName)[MAX_PATH], TCHAR (&title)[WINDOW_TEXT_LENGTH]);
	bool IsConsole() const;
	bool Match(LPCTSTR name) const;
	template<size_t size> bool Match(LPCTSTR (&names)[size]) const
	{
		for (int i = 0; i < size; ++i)
			if (!_tcsicmp(m_appName, names[i]))
				return true;
		return false;
	}
private:
	TCHAR m_appName[MAX_PATH];
	TCHAR m_IMEName[MAX_PATH];
	static TCHAR m_fallbackIMEName[MAX_PATH];
	bool m_imeState;
	void ConsoleAppName(TCHAR (&appName)[MAX_PATH], CString& title);
	void RemovePrefixes(CString& title) const;
	bool IsCmdExe(CString& title) const;
	bool IsConsole(LPCTSTR appName) const;
};
