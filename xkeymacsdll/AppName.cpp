#include "AppName.h"
#include "Utils.h"
#include <Imm.h>

#pragma data_seg(".xkmcs")
TCHAR AppName::m_FallbackIMEName[MAX_PATH] = _T("");
#pragma data_seg()
TCHAR AppName::m_AppName[CLASS_NAME_LENGTH] = _T("");
TCHAR AppName::m_IMEName[MAX_PATH] = _T("");
bool AppName::m_Inited = false;
bool AppName::m_IMEState = false;

void AppName::Init()
{
	if (m_Inited)
		return;
	if (m_FallbackIMEName[0] == _T('\0')) {
		// preserve the IME file name of the current TIF if it works as an Imm32 IME.
		HKL hKL = GetKeyboardLayout(0);
		if (!ImmIsIME(hKL) || !ImmGetIMEFileName(hKL, m_FallbackIMEName, MAX_PATH))
			_tcscpy_s(m_FallbackIMEName, _T("IME"));
	}
	TCHAR path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	CString s(path);
	s.Delete(0, s.ReverseFind(_T('\\')) + 1);
	_tcscpy_s(m_AppName, s);
	TCHAR text[WINDOW_TEXT_LENGTH];
	GetWindowText(GetForegroundWindow(), text, sizeof(text));
	CorrectAppName(text, m_AppName);
}

LPCTSTR AppName::GetAppName()
{
	return m_IMEState ? m_IMEName : m_AppName;
}

void AppName::SetIMEState(bool on)
{
	AppName::Init();
	m_IMEState = on;
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
	_tcscpy_s(m_IMEName, m_FallbackIMEName);
}

bool AppName::GetIMEState()
{
	return m_IMEState;
}

// The code starting here is derived from work by co <cogood＠gmail.com>.
void AppName::CorrectAppName(TCHAR (&text)[WINDOW_TEXT_LENGTH], TCHAR (&appName)[CLASS_NAME_LENGTH])
{
	CString s(text);
	if (IsConsole(appName)) {
		RemovePrefixes(s);
		int prev, sep;
		for (prev = -1, sep = 0; (sep = s.Find(_T(" - "), sep)) >= 0; prev = sep, sep += 3)
			;
		if (prev >= 0) {
			s.Delete(0, prev + 3);
			ConsoleAppName(s, appName);
			_tcscpy_s(text, s);
			return;
		}
		if (IsCmdExe(s)) {
			_tcscpy_s(appName, "cmd.exe");
			_tcscpy_s(text, s);
			return;
		}
		if (s.Find(_T("Cygwin Bash Shell")) == 0 ||
				s[0] == _T('~') || s[0] == _T('/')) {
			_tcscpy_s(appName, _T("bash.exe"));
			_tcscpy_s(text, _T("Cygwin Bash Shell"));
		} else if (s.Find(_T("MKS Korn Shell")) == 0 || s.Find(_T("cat")) == 0) {
			_tcscpy_s(appName, _T("sh.exe"));
		} else if (s.Find(_T(":/"), 1) == 1 || s.Find(_T(":\\"), 1) == 1) {
			_tcscpy_s(appName, _T("csh.exe"));
			_tcscpy_s(text, "C Shell");
		} else {
			ConsoleAppName(s, appName);
			_tcscpy_s(text, s);
		}
	} else if (!_tcsicmp(appName, _T("javaw.exe"))) {
		if (s.Find(_T(" - Eclipse Platform")) || s == _T("Find/Replace"))
			_tcscpy_s(appName, "eclipse.exe");
		else if (s.Find(_T("BlueJ")))
			_tcscpy_s(appName, "bluej.exe");
		else if (s.Find(_T("JUDE")))
			_tcscpy_s(appName, "jude.exe");
	}
}

bool AppName::IsConsole()
{
	return IsConsole(m_AppName);
}

bool AppName::IsConsole(LPCTSTR appName)
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

bool AppName::IsCmdExe(const CString& text)
{
	LPCTSTR prompts[] = {
		_T("command prompt"),
		_T("コマンド プロンプト"),
		_T("system32\\cmd.exe"),
		_T("syswow64\\cmd.exe")
	};
	for (int i = 0; i < _countof(prompts); ++i)
		if (text.Right(static_cast<int>(_tcslen(prompts[i]))).CompareNoCase(prompts[i]) == 0)
			return true;
	return false;
}

void AppName::ConsoleAppName(CString& text, TCHAR (&appName)[CLASS_NAME_LENGTH])
{
	int endQuote = text.Find(_T('"'), 1);
	int space = text.Find(_T(' '), 1);
	if (text[0] == _T('"') && endQuote > 0)
		text = text.Mid(1, endQuote - 1); // A string surrounded by " is a command name.
	else if (space >= 0)
		text = text.Left(space + 1); // The first token is a command name.
	int backSlash = text.ReverseFind(_T('\\')); // remove its directory.
	if (backSlash >= 0)
		text.Delete(0, backSlash + 1);
	_tcsncpy_s(appName,
		text.Right(4).CompareNoCase(_T(".exe")) == 0
		? text : text + _T(".exe"), _TRUNCATE);
}

void AppName::RemovePrefixes(CString& text)
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
		if (text.Find(prefixes[i]) == 0)
			text.Delete(0, static_cast<int>(_tcslen(prefixes[i])));
}
// The derived code ends here.

bool AppName::Match(LPCTSTR name)
{
	LPCTSTR names[1] = { name };
	return Match(names);
}
