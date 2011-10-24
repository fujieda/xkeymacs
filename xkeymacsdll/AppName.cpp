#include "AppName.h"
#include "Utils.h"
#include <Imm.h>

#pragma data_seg(".xkmcs")
TCHAR AppName::m_fallbackIMEName[MAX_PATH] = _T("");
#pragma data_seg()

AppName::AppName(void) : m_imeState(false)
{
	GetModuleFileName(NULL, m_appName, sizeof(m_appName));
	CString path(m_appName);
	path.Delete(0, path.ReverseFind(_T('\\')) + 1);
	_tcscpy_s(m_appName, path);
	m_IMEName[0] = _T('\0');
}

void AppName::SetAppName()
{
	if (m_fallbackIMEName[0] == _T('\0')) {
		// preserve the IME file name of the current TIF if it works as an Imm32 IME.
		HKL hKL = GetKeyboardLayout(0);
		if (!ImmIsIME(hKL) || !ImmGetIMEFileName(hKL, m_fallbackIMEName, sizeof(m_IMEName)))
			_tcscpy_s(m_fallbackIMEName, _T("IME"));
	}
	TCHAR title[WINDOW_TEXT_LENGTH];
	GetWindowText(GetForegroundWindow(), title, sizeof(title));
	CorrectAppName(m_appName, title);
}

LPCTSTR AppName::GetAppName() const
{
	return m_imeState ? m_IMEName : m_appName;
}

void AppName::SetIMEState(bool on)
{
	m_imeState = on;
	if (!on)
		return;
	HKL hKL = GetKeyboardLayout(0);
	if (ImmIsIME(hKL)) {
		if (!ImmGetIMEFileName(hKL, m_IMEName, sizeof(m_IMEName)))
			_tcscpy_s(m_IMEName, _T("IME")); // TIP on TSF
		return;
	}
	// ImmIsIME returns false if you use TSF aware applications with a TIF (aka IME).
	// The following take the preserved IME file name of it.
	_tcscpy_s(m_IMEName, m_fallbackIMEName);
}

// The code starting here is derived from work by co <cogood＠gmail.com>.
void AppName::CorrectAppName(TCHAR (&appName)[MAX_PATH], TCHAR (&title)[WINDOW_TEXT_LENGTH])
{
	CString strTitle(title);
	if (IsConsole(appName)) {
		int sep = strTitle.Find(_T(" - "));
		if (sep >= 0) {
			strTitle.Delete(0, sep + 3);
			_tcscpy_s(title, strTitle);
			ConsoleAppName(appName, strTitle);
			return;
		}
		RemovePrefixes(strTitle);
		_tcscpy_s(title, strTitle);
		if (IsCmdExe(strTitle))
			_tcscpy_s(appName, _T("cmd.exe"));
		else if (strTitle.Find(_T("Cygwin Bash Shell")) == 0 ||
				strTitle[0] == _T('~') || strTitle[0] == _T('/')) {
			_tcscpy_s(appName, _T("bash.exe"));
			_tcscpy_s(title, _T("Cygwin Bash Shell"));
		} else if (strTitle.Find(_T("MKS Korn Shell")) == 0) {
			_tcscpy_s(appName, _T("sh.exe"));
			_tcscpy_s(title, strTitle);
		} else if (strTitle.Find(_T(":/"), 1) == 1 || strTitle.Find(_T(":\\"), 1) == 1) {
			_tcscpy_s(appName, _T("csh.exe"));
			_tcscpy_s(title, "C Shell");
		} else
			ConsoleAppName(appName, strTitle);
	} else if (!_tcsicmp(appName, _T("javaw.exe"))) {
		if (strTitle.Find(_T(" - Eclipse Platform")) || strTitle == _T("Find/Replace"))
			_tcscpy_s(appName, "eclipse.exe");
		else if (strTitle.Find(_T("BlueJ")))
			_tcscpy_s(appName, "bluej.exe");
		else if (strTitle.Find(_T("JUDE")))
			_tcscpy_s(appName, "jude.exe");
	}
}

void AppName::ConsoleAppName(TCHAR (&appName)[MAX_PATH], CString& title)
{
	int endQuote = title.Find(_T('"'), 1);
	int space = title.Find(_T(' '), 1);
	if (title[0] == _T('"') && endQuote >= 0)
		title = title.Mid(1, endQuote - 1); // A string surrounded by " is a command name.
	else if (space >= 0)
		title = title.Left(space + 1); // The first token is a command name.
	int backSlash = title.ReverseFind(_T('\\')); // remove its directory.
	if (backSlash >= 0)
		title.Delete(0, backSlash + 1);
	if (title.Right(4) != _T(".exe"))
		title.Append(_T(".exe"));
	_tcscpy_s(appName, title);
}

void AppName::RemovePrefixes(CString& title) const
{
	LPCTSTR prefixes[] = {
		_T("範囲指定 "), // via the edit menu
		_T("スクロール "), // via the edit menu
		_T("選択 "), // via QuickEdit
		_T("管理者: "), // remove it last
		_T("Mark "),
		_T("Scroll "),
		_T("Select "),
		_T("Administrator: "),
	};
	for (int i = 0; i < _countof(prefixes); ++i)
		if (title.Find(prefixes[i]) == 0)
			title.Delete(0, static_cast<int>(_tcslen(prefixes[i])));
}

bool AppName::IsCmdExe(CString& title) const
{
	LPCTSTR prompts[] = {
		_T("command prompt"),
		_T("コマンド プロンプト"),
		_T("system32\\cmd.exe"),
		_T("syswow64\\cmd.exe")
	};
	const CString& lower = title.MakeLower();
	for (int i = 0; i < _countof(prompts); ++i)
		if (lower.Find(prompts[i]) >= 0)
			return true;
	return false;
}
// The derived code ends here.

bool AppName::IsConsole() const
{
	return IsConsole(m_appName);
}

bool AppName::IsConsole(LPCTSTR appName) const
{
	LPCTSTR names[] = {
		"conhost.exe",
		"conime.exe",
		"csh.exe",
		"cmd.exe",
		"bash.exe",
		"ftp.exe",
		"sh.exe",
		"telnet.exe",
	};
	if (!appName[0])
		return true;
	for (int i = 0; i < _countof(names); ++i)
		if (!_tcsicmp(appName, names[i]))
			return true;
	return false;
}

bool AppName::Match(LPCTSTR name) const
{
	LPCTSTR names[1] = { name };
	return Match(names);
}
