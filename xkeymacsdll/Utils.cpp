// Utils.cpp: implementation of the CUtils class
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Utils.h"
#include "../xkeymacs/resource.h"
#include <Imm.h>
#pragma data_seg(".xkmcs")
TCHAR CUtils::m_szApplicationName[MAX_PATH] = {'\0'};
TCHAR CUtils::m_szIMEName[MAX_PATH] = _T("IME");	// IDS_IME_FILE_NAME
OSVERSIONINFO CUtils::m_OsVersionInfo = {sizeof(OSVERSIONINFO)};
#pragma data_seg()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUtils::CUtils()
{

}

CUtils::~CUtils()
{

}

BOOL CUtils::GetFindDialogTitle(CString *szDialogTitle)
{
	{
		TCHAR buf[0x100] = {'\0'};
		GetWindowText(GetForegroundWindow(), buf, sizeof(buf));
//		CUtils::Log(_T("Window Text --%s--"), buf);
		szDialogTitle->Format(_T("%s"), buf);
	}

	if (!szDialogTitle->Compare(_T("検索"))					// notepad, wordpad, IE, regedit, 
														// Excel, Front Page, PowerPoint, Acrobat Reader, IBM HPB
	 || !szDialogTitle->Compare(_T("Find"))					// notepad, Outlook, Outlook Express, Opera
	 || !szDialogTitle->Compare(_T("Find in this Page"))	// Mozilla
	 || !szDialogTitle->Compare(_T("Find/Replace"))			// Eclipse
	 || !szDialogTitle->Compare(_T("本文内検索"))			// Becky!
	 || !szDialogTitle->Compare(_T("メール検索"))			// Becky!
	 || !szDialogTitle->Compare(_T("文字列の検索"))			// M$ VC++
	 || !szDialogTitle->Compare(_T("検索と置換"))			// M$ Word, 123
	 || !szDialogTitle->Compare(_T("文書のテキストの検索"))	// Lotus Notes
	 || !szDialogTitle->Compare(_T("このページを検索"))		// Netscape 6
	 || !szDialogTitle->Compare(_T("文字列検索"))			// Dana
	 || !szDialogTitle->Compare(_T("検索/置換"))			// PHP Editor
	 || !szDialogTitle->Compare(_T("置換"))
	 || !szDialogTitle->Compare(_T("Replace"))) {
		return TRUE;
	}

	szDialogTitle->Empty();
	return FALSE;
}

BOOL CUtils::IsFindDialog()
{
	CString szDialogTitle;
	return GetFindDialogTitle(&szDialogTitle);
}

BOOL CUtils::IsAstecX()
{
	return !_tcsicmp(m_szApplicationName, _T("astecx.exe"));
}

BOOL CUtils::IsBecky()
{
	return !_tcsicmp(m_szApplicationName, _T("b2.exe"));
}

BOOL CUtils::IsEmacs()
{
	return !_tcsicmp(m_szApplicationName, _T("Emacs.exe"));
}

BOOL CUtils::IsExplorer()
{
	return !_tcsicmp(m_szApplicationName, _T("explorer.exe"));
}

BOOL CUtils::IsHidemaru()
{
	return !_tcsicmp(m_szApplicationName, _T("hidemaru.exe"));
}

BOOL CUtils::IsInternetExplorer()
{
	return !_tcsicmp(m_szApplicationName, _T("iexplore.exe"));
}

BOOL CUtils::IsLotus123()
{
	return !_tcsicmp(m_szApplicationName, _T("123w.exe"));
}

BOOL CUtils::IsLotusNotes()
{
	return !_tcsicmp(m_szApplicationName, _T("nlnotes.exe"));
}

BOOL CUtils::IsLotusWordPro()
{
	return !_tcsicmp(m_szApplicationName, _T("wordpro.exe"));
}

BOOL CUtils::IsMeadow()
{
	return !_tcsicmp(m_szApplicationName, _T("Meadow.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("Meadow95.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("MeadowNT.exe"));
}

BOOL CUtils::IsMicrosoftFrontPage()
{
	return !_tcsicmp(m_szApplicationName, _T("frontpg.exe"));
}

BOOL CUtils::IsMicrosoftWord()
{
	return !_tcsicmp(m_szApplicationName, _T("winword.exe"));
}

BOOL CUtils::IsMozilla()
{
	return !_tcsicmp(m_szApplicationName, _T("mozilla.exe"));
}

BOOL CUtils::IsMuleForWin32()
{
	return !_tcsicmp(m_szApplicationName, _T("mule.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("mulent.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("mulecd.exe"));
}

BOOL CUtils::IsNetscape()
{
	return !_tcsicmp(m_szApplicationName, _T("netscp6.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("netscp.exe"));
}

BOOL CUtils::IsNotepad()
{
	return !_tcsicmp(m_szApplicationName, _T("notepad.exe"));
}

BOOL CUtils::IsOpera()
{
	return !_tcsicmp(m_szApplicationName, _T("opera.exe"));
}

BOOL CUtils::IsOutlook()
{
	return !_tcsicmp(m_szApplicationName, _T("outlook.exe"));
}

BOOL CUtils::IsOutlookExpress()
{
	return !_tcsicmp(m_szApplicationName, _T("msimn.exe"));
}

BOOL CUtils::IsSakuraEditor()
{
	return !_tcsicmp(m_szApplicationName, _T("sakura.exe"));
}

BOOL CUtils::IsTeraTermPro()
{
	return !_tcsicmp(m_szApplicationName, _T("ttermpro.exe"));
}

BOOL CUtils::IsVisualCpp()
{
	return !_tcsicmp(m_szApplicationName, _T("msdev.exe"));
}

BOOL CUtils::IsWordpad()
{
	return !_tcsicmp(m_szApplicationName, _T("wordpad.exe"));
}

BOOL CUtils::IsXWin()
{
	return !_tcsicmp(m_szApplicationName, _T("XWin.exe"));
}

BOOL CUtils::IsXyzzy()
{
	return !_tcsicmp(m_szApplicationName, _T("xyzzy.exe"));
}

LPCTSTR const CUtils::GetApplicationName()
{
	return m_szApplicationName;
}

void CUtils::FairConsoleApplicationName(LPTSTR szApplicationName, int nApplicationNameLength, LPTSTR szWindowText, int nWindowTextLength)
{
	if (IsFindDialog()) {
		return;
	}

	if (*szWindowText == '"' && _tcschr(szWindowText+1, _T('"'))) {		// "foo bar" -> foo bar
		int nApplicationName = _tcschr(szWindowText+1, _T('"')) - szWindowText - 1;	// length of "foo bar"
		_tcsncpy(szWindowText, szWindowText + 1, nApplicationName);
		memset(szWindowText + nApplicationName, 0, nWindowTextLength - nApplicationName);
	} else if (_tcschr(szWindowText, _T(' '))) {	// foo bar -> foo
		LPTSTR pFirstSpace = _tcschr(szWindowText, _T(' '));
		memset(pFirstSpace, 0, nWindowTextLength - (pFirstSpace - szWindowText));
	}

	memset(szApplicationName, 0, nApplicationNameLength);
	_stprintf(szApplicationName, _T("%s"), szWindowText);

	static LPCTSTR const szExe = _T(".exe");
	if (_tcsnicmp(szApplicationName + _tcslen(szApplicationName) - _tcslen(szExe), szExe, _tcslen(szExe))) {
		_tcscat(szApplicationName, szExe);
	}
}

// Set real application name in the szApplicationName.
void CUtils::SetCorrectApplicationName(LPTSTR szApplicationName, const int nApplicationNameLength, LPTSTR szWindowText, const int nWindowTextLength)
{
	if (IsConsole(szApplicationName, nApplicationNameLength)) {
		int i = 0;
		static LPCTSTR const szPromptName[] = {_T("Command Prompt"), _T("Mark Command Prompt"), _T("Select Command Prompt"), _T("MS-DOS Prompt"),
											   _T("Visual Studio .NET Command Prompt"), _T("Visual Studio .NET 2003 Command Prompt"),
											   _T("コマンド プロンプト"), _T("範囲指定 コマンド プロンプト"), _T("選択 コマンド プロンプト"), _T("MS-DOS プロンプト"),
											   _T("Visual Studio .NET コマンド プロンプト"), _T("Visual Studio .NET 2003 コマンド プロンプト")};
		static LPCTSTR const szPromptPath[] = {_T("system32\\cmd.exe")};	// WindowText of Command Prompt is sometimes this style. But MS-DOS Prompt's is always MS-DOS Prompt.
		static LPCTSTR const szSeparator = _T(" - ");

		for (i = 0; i < sizeof(szPromptName) / sizeof(szPromptName[0]); ++i) {
			if (!_tcsicmp(szWindowText, szPromptName[i])) {	// "Command Prompt"
				return;
			}

			TCHAR sz[0x100] = {'\0'};
			_stprintf(sz, _T("%s%s"), szPromptName[i], szSeparator);

			if (!_tcsnicmp(szWindowText, sz, _tcslen(sz))) {	// "Command Promp - foo"
				_tcscpy(szWindowText, szWindowText + _tcslen(sz));
				FairConsoleApplicationName(szApplicationName, nApplicationNameLength, szWindowText, nWindowTextLength);
				return;
			}
		}

		for (i = 0; i < sizeof(szPromptPath) / sizeof(szPromptPath[0]); ++i) {
			TCHAR szWindowTextLower[0x100] = {'\0'};
			_tcscpy(szWindowTextLower, szWindowText);
			_tcslwr(szWindowTextLower);

			if (_tcsstr(szWindowTextLower, szPromptPath[i])) {
				TCHAR sz[0x100] = {'\0'};
				_stprintf(sz, _T("%s%s"), szPromptPath[i], szSeparator);

				if (_tcsstr(szWindowTextLower, sz)) {				// "X:\WINNT\system32\cmd.exe - foo"
					_tcscpy(szWindowText, _tcsstr(szWindowTextLower, sz) + _tcslen(sz));
					FairConsoleApplicationName(szApplicationName, nApplicationNameLength, szWindowText, nWindowTextLength);
					return;
				} else {									// "X:\WINNT\system32\cmd.exe"
					return;
				}
			}
		}

		if (!_tcsicmp(szWindowText, _T("Cygwin Bash Shell"))
		 || (*szWindowText == _T('~'))
		 || (*szWindowText == _T('/'))) {						// Bash
			memset(szApplicationName, 0, nApplicationNameLength);
			_stprintf(szApplicationName, _T("bash.exe"));
			memset(szWindowText, 0, nWindowTextLength);
			_stprintf(szWindowText, _T("bash"));
		} else if (!_tcsicmp(szWindowText + _tcslen(szWindowText) - 8, _T(" - pdksh"))) {
			memset(szApplicationName, 0, nApplicationNameLength);
			_stprintf(szApplicationName, _T("pdksh.exe"));
			memset(szWindowText, 0, nWindowTextLength);
			_stprintf(szWindowText, _T("pdksh"));
		} else if (!_tcsicmp(szWindowText + _tcslen(szWindowText) - 7, _T(" - tcsh"))) {
			memset(szApplicationName, 0, nApplicationNameLength);
			_stprintf(szApplicationName, _T("tcsh.exe"));
			memset(szWindowText, 0, nWindowTextLength);
			_stprintf(szWindowText, _T("tcsh"));
		} else if (!_tcsicmp(szWindowText + _tcslen(szWindowText) - 6, _T(" - zsh"))) {
			memset(szApplicationName, 0, nApplicationNameLength);
			_stprintf(szApplicationName, _T("zsh.exe"));
			memset(szWindowText, 0, nWindowTextLength);
			_stprintf(szWindowText, _T("zsh"));
		} else if (!_tcsnicmp(szWindowText, _T("MKS Korn Shell"), 14)
				|| !_tcsnicmp(szWindowText, _T("cat"), 3)) {
			memset(szApplicationName, 0, nApplicationNameLength);
			_stprintf(szApplicationName, _T("sh.exe"));
			memset(szWindowText, 0, nWindowTextLength);
			_stprintf(szWindowText, _T("MKS Korn Shell"));
		} else if (!_tcsnicmp(szWindowText + 1, _T(":/"), 2)
				|| !_tcsnicmp(szWindowText + 1, _T(":\\"), 2)) {
			memset(szApplicationName, 0, nApplicationNameLength);
			_stprintf(szApplicationName, _T("csh.exe"));
			memset(szWindowText, 0, nWindowTextLength);
			_stprintf(szWindowText, _T("C Shell"));
		} else if (_tcsstr(szWindowText, _T(" - VIM"))) {
			memset(szApplicationName, 0, nApplicationNameLength);
			_stprintf(szApplicationName, _T("vim.exe"));
			memset(szWindowText, 0, nWindowTextLength);
			_stprintf(szWindowText, _T("VIM"));
		} else if (_tcsstr(szWindowText, _T(" - Poderosa"))) {
			memset(szApplicationName, 0, nApplicationNameLength);
			_stprintf(szApplicationName, _T("Poderosa.exe"));
			memset(szWindowText, 0, nWindowTextLength);
			_stprintf(szWindowText, _T("Poderosa"));
		} else {											// unknown console application
			FairConsoleApplicationName(szApplicationName, nApplicationNameLength, szWindowText, nWindowTextLength);
		}
	} else if (IsJavaW(szApplicationName, nApplicationNameLength)) {
		if (!_tcsicmp(szWindowText + _tcslen(szWindowText) - 19, _T(" - Eclipse Platform"))) {
			memset(szApplicationName, 0, nApplicationNameLength);
			_stprintf(szApplicationName, _T("eclipse.exe"));
		} else if (!_tcsicmp(szWindowText, _T("BlueJ"))
			    || !_tcsnicmp(szWindowText, _T("BlueJ: "), 7)) {
			memset(szApplicationName, 0, nApplicationNameLength);
			_stprintf(szApplicationName, _T("bluej.exe"));
		} else if (!_tcsicmp(szWindowText, _T("JUDE"))
			    || !_tcsnicmp(szWindowText, _T("JUDE - "), 7)) {
			memset(szApplicationName, 0, nApplicationNameLength);
			_stprintf(szApplicationName, _T("jude.exe"));
		}
	}
	return;
}

void CUtils::SetApplicationName(BOOL bImeComposition)
{
//	CUtils::Log(_T("SetApplicationName: start"));

	memset(m_szApplicationName, 0, sizeof(m_szApplicationName));

	if (_tcscmp(m_szIMEName, _T("IME")) == 0) {	// IDS_IME_FILE_NAME
		HKL hKL = GetKeyboardLayout(0);
		if (ImmIsIME(hKL)) {
			if (!ImmGetIMEFileName(hKL, m_szIMEName, sizeof(m_szIMEName))) {
				_tcsncpy(m_szIMEName, _T("IME"), sizeof(m_szIMEName));	// IDS_IME_FILE_NAME
			}
//			CUtils::Log(_T("SetApplicationName: m_szIMEName == %s"), m_szIMEName);
		}
	}

	if (bImeComposition) {
//		CUtils::Log(_T("SetApplicationName: bImeComposition"));

		HKL hKL = GetKeyboardLayout(0);
		if (ImmIsIME(hKL)) {
			if (!ImmGetIMEFileName(hKL, m_szApplicationName, sizeof(m_szApplicationName))) {
				_tcsncpy(m_szApplicationName, m_szIMEName, sizeof(m_szApplicationName));
			}
			_tcsncpy(m_szIMEName, m_szApplicationName, sizeof(m_szIMEName));
		} else {
			// ImmIsIME return 0 on Word2002, Excel2002, etc. with IME2002, so...
			// _tcsncpy(m_szApplicationName, _T("imjp81.ime"), sizeof(m_szApplicationName));
			_tcsncpy(m_szApplicationName, m_szIMEName, sizeof(m_szApplicationName));
		}
	} else {
//		CUtils::Log(_T("SetApplicationName: appication (%s)"), m_szApplicationName);

		GetModuleFileName(NULL, m_szApplicationName, sizeof(m_szApplicationName));
		CString szFn(m_szApplicationName);
		szFn.Delete(0, szFn.ReverseFind(_T('\\')) + 1);
		ZeroMemory(m_szApplicationName, sizeof(m_szApplicationName));
		_tcsncpy(m_szApplicationName, szFn, szFn.GetLength());

//		CUtils::Log(_T("SetApplicationName: appication [%s]"), m_szApplicationName);

		if (IsConsole()) {
//			CUtils::Log(_T("SetApplicationName: console"));

			memset(m_szApplicationName, 0, sizeof(m_szApplicationName));
			if (IsNT()) {
				_tcscpy(m_szApplicationName, _T("CMD.exe"));
			} else {
				_tcscpy(m_szApplicationName, _T("WINOA386.MOD"));
			}
			TCHAR szWindowText[0x100] = {'\0'};
			GetWindowText(GetForegroundWindow(), szWindowText, sizeof(szWindowText));
			SetCorrectApplicationName(m_szApplicationName, sizeof(m_szApplicationName), szWindowText, sizeof(szWindowText));
		} else if (IsJavaW()) {
			TCHAR szWindowText[0x100] = {'\0'};
			GetWindowText(GetForegroundWindow(), szWindowText, sizeof(szWindowText));
			SetCorrectApplicationName(m_szApplicationName, sizeof(m_szApplicationName), szWindowText, sizeof(szWindowText));
		}
		if (!_tcsicmp(m_szApplicationName, _T("Cygwin.exe"))) {
//			CUtils::Log(_T("SetApplicationName: cygwin"));

			memset(m_szApplicationName, 0, sizeof(m_szApplicationName));
			_tcscpy(m_szApplicationName, _T("bash.exe"));
		}
//		CUtils::Log(_T("name: %s"), m_szApplicationName);
	}
}

void CUtils::SetIMEName()
{
	HKL hKL = GetKeyboardLayout(0);
	if (ImmIsIME(hKL)) {
		ImmGetIMEFileName(hKL, m_szIMEName, sizeof(m_szIMEName));
	}
}

void CUtils::InitCUtils()
{
	GetVersionEx(&m_OsVersionInfo);
	SetIMEName();
}

BOOL CUtils::IsNTor9x()
{
	return ((m_OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && (m_OsVersionInfo.dwMajorVersion <= 4))
		 || (m_OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);
}

BOOL CUtils::IsXPorLater()
{
	return 5 < m_OsVersionInfo.dwMajorVersion
		|| 5 == m_OsVersionInfo.dwMajorVersion && 1 <= m_OsVersionInfo.dwMinorVersion;
}

BOOL CUtils::IsNT()
{
	return m_OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

BOOL CUtils::OpenClipboard()
{
	const int RETRY_COUNT = 0x10;

	BOOL bOpened = FALSE;
	for (int i = 0; i <= RETRY_COUNT; ++i) {
		if (::OpenClipboard(NULL)) {
//			Log(_T("ok (%d)"), i);
			bOpened = TRUE;
			break;
		} else {
			Sleep(1);	// for OpenOffice
//			Log(_T("CUtils::OpenClipboard: %d (%d)"), GetLastError(), i);
		}
	}
	ASSERT(bOpened);
	return bOpened;
}

BOOL CUtils::GetClipboardText(CString *szClipboardText)
{
	if (!szClipboardText) {
		return FALSE;
	}

	szClipboardText->Empty();

	if (!OpenClipboard()) {
//		CUtils::Log(_T("Cannot open the Clipboard"));
		return FALSE;
	}

/*
	{
		Log(_T("GetClipboardText"));
		UINT uFormat = 0;
		for (;;) {
			uFormat = EnumClipboardFormats(uFormat);
			if (uFormat) {
				Log(_T("uFormat = %d"), uFormat);
			} else {
				break;
			}
		}
// winuser.h
//#define CF_TEXT             1
//#define CF_METAFILEPICT     3
//#define CF_OEMTEXT          7
//#define CF_UNICODETEXT      13
//#define CF_ENHMETAFILE      14
//#define CF_LOCALE           16
//#define CF_DSPTEXT          0x0081
// OwnerLink						49155
// Native							49156
// DataObject						49161
// Embed Source						49163
// Object Descriptor				49166
// Ole Private Data					49171
// Rich Text Format					49311
// Rich Text Format					49312
// HTML Format						49360
// HTML Format						49361
// Link								49408
// RTF As Text						49595
// Rich Text Format Without Objects	49618
// Star Object Descriptor (XML)		49681
// Star Embed Source (XML)			49708
// HPB HTML Format					49742
	}
*/

	UINT uFormat = CF_OEMTEXT;
	if (IsSakuraEditor()) {
		uFormat = CF_TEXT;
	}

	HANDLE hClipboardText;
	if ((hClipboardText = ::GetClipboardData(uFormat)) == NULL) {
//		DWORD dw = GetLastError();
//		CUtils::Log(_T("Unable to get Clipboard data: %d"), dw);
		CloseClipboard();
		return FALSE;
	}

	szClipboardText->Format(_T("%s"), hClipboardText);
	EmptyClipboard();
	CloseClipboard();
	return TRUE;
}

BOOL CUtils::SetClipboardText(CString *szClipboardText)
{
	if (!OpenClipboard()) {
//		CUtils::Log(_T("Cannot open the Clipboard in SetClipboardText"));
		return FALSE;
	}

	int nLength = szClipboardText->GetLength() + 1;
	HGLOBAL hClipboardText = GlobalAlloc(GHND, nLength);
	if (hClipboardText == NULL) {
//		CUtils::Log(_T("Failed: GlobalAlloc in SetClipboardText"));
		return FALSE;
	}

	LPTSTR lpStr = (LPTSTR)GlobalLock(hClipboardText);
	lstrcpyn(lpStr, *szClipboardText, nLength);
	GlobalUnlock(hClipboardText);
	EmptyClipboard();

	UINT uFormat = CF_OEMTEXT;
	if (IsSakuraEditor()) {
		uFormat = CF_TEXT;
	}

	if ((hClipboardText = ::SetClipboardData(uFormat, hClipboardText)) == NULL) {
//		DWORD dw = GetLastError();
//		CUtils::Log(_T("Unable to set Clipboard data: %d"), dw);
		CloseClipboard();
		return FALSE;
	}

	CloseClipboard();
	return TRUE;
}

BOOL CUtils::IsDefaultIgnoreApplication()
{
	if (IsAstecX()
	 || IsBash()
	 || IsCsh()
	 || IsEmacs()
	 || IsLispWorksPersonalEdition()
	 || IsMeadow()
	 || IsMuleForWin32()
	 || IsPdksh()
	 || IsSh()
	 || IsTcsh()
	 || IsTeraTermPro() && !IsDialog()
	 || IsVisualSlickEdit() && !IsDialog()
	 || IsXWin()
	 || IsXyzzy() && !IsDialog()
	 || IsZsh()) {
		return TRUE;
	}
	return FALSE;
}

BOOL CUtils::IsDWFM()
{
	return !_tcsicmp(m_szApplicationName, _T("dwfm.exe"));
}

BOOL CUtils::IsK2Editor()
{
	return !_tcsicmp(m_szApplicationName, _T("K2Editor.exe"));
}

BOOL CUtils::IsEggExplorer()
{
	return !_tcsicmp(m_szApplicationName, _T("EggExp.exe"));
}

BOOL CUtils::IsDirector()
{
	return !_tcsicmp(m_szApplicationName, _T("Director.exe"));
}

BOOL CUtils::IsExcel()
{
	return !_tcsicmp(m_szApplicationName, _T("Excel.exe"));
}

BOOL CUtils::IsFireworks()
{
	return !_tcsicmp(m_szApplicationName, _T("Fireworks 4.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("Fireworks.exe"));
}

BOOL CUtils::IsDreamweaver()
{
	return !_tcsicmp(m_szApplicationName, _T("Dreamweaver.exe"));
}

BOOL CUtils::IsFlash()
{
	return !_tcsicmp(m_szApplicationName, _T("Flash.exe"));
}

BOOL CUtils::IsPhotoshop()
{
	return !_tcsicmp(m_szApplicationName, _T("Photoshp.exe"));
}

BOOL CUtils::IsIllustrator()
{
	return !_tcsicmp(m_szApplicationName, _T("Illustrator.exe"));
}

BOOL CUtils::IsMicrosoftPowerPoint()
{
	return !_tcsicmp(m_szApplicationName, _T("PowerPnt.exe"));
}

BOOL CUtils::IsReget()
{
	return !_tcsicmp(m_szApplicationName, _T("Regetdx.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("Regetjr.exe"));
}

BOOL CUtils::IsPaint()
{
	return !_tcsicmp(m_szApplicationName, _T("mspaint.exe"));
}

BOOL CUtils::IsConsole()
{
//	Log(_T("_%s_"), m_szApplicationName);
	return !m_szApplicationName[0]
		|| !_tcsicmp(m_szApplicationName, _T("xkeymacs.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("conhost.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("conime.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("csh.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("WINOA386.MOD"))
		|| !_tcsicmp(m_szApplicationName, _T("CMD.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("bash.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("ftp.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("sh.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("telnet.exe"));
}

BOOL CUtils::IsConsole(LPCTSTR szApplicationName, int nApplicationNameLength)
{
	return !_tcsnicmp(szApplicationName, _T("WINOA386.MOD"), nApplicationNameLength)
		|| !_tcsnicmp(szApplicationName, _T("CMD.exe"), nApplicationNameLength);
}

BOOL CUtils::IsJavaW(LPCTSTR szApplicationName, int nApplicationNameLength)
{
	return !_tcsnicmp(szApplicationName, _T("javaw.exe"), nApplicationNameLength);
}

BOOL CUtils::IsSleipnir()
{
	return !_tcsicmp(m_szApplicationName, _T("Sleipnir.exe"));
}

BOOL CUtils::IsBash()
{
	return !_tcsicmp(m_szApplicationName, _T("bash.exe"));
}

// for debug
void CUtils::Log(LPTSTR fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	static const int LOG_MAX = 0x10000;
	TCHAR szLog[LOG_MAX] = {'\0'};

	for (unsigned int nIndex = 0; nIndex < _tcslen(fmt); ) {
		LPTSTR pNextString = fmt + nIndex;
		LPTSTR pLogEnd = szLog + _tcslen(szLog);

		if (*pNextString == _T('%')) {
			TCHAR szFormatTag[LOG_MAX] = {'0'};
			_tcscpy(szFormatTag, pNextString);

			switch (GetFormatTag(szFormatTag)) {
			case _T('d'):
			case _T('i'):
			case _T('o'):
			case _T('x'):
			case _T('X'):
			case _T('u'):
			case _T('c'):
				_stprintf(pLogEnd, szFormatTag, va_arg(ap, int));
				break;
			case _T('s'):
				_stprintf(pLogEnd, szFormatTag, va_arg(ap, LPTSTR));
				break;
			case _T('f'):
			case _T('e'):
			case _T('E'):
			case _T('g'):
			case _T('G'):
				_stprintf(pLogEnd, szFormatTag, va_arg(ap, double));
				break;
			case _T('p'):
				_stprintf(pLogEnd, szFormatTag, va_arg(ap, void *));
				break;
			case _T('n'):
				_stprintf(pLogEnd, szFormatTag, va_arg(ap, int *));
				break;
			case _T('%'):
			default:
				_stprintf(pLogEnd, _T("%s"), szFormatTag);
				break;
			}

			nIndex += _tcslen(szFormatTag);
		} else {
			TCHAR szString[LOG_MAX] = {'0'};
			_tcscpy(szString, pNextString);
			LPTSTR pString = _tcstok(szString, _T("%"));
			_stprintf(pLogEnd, _T("%s"), pString);

			nIndex += _tcslen(pString);
		}
	}

	va_end(ap);

	static int n = 0;
	TCHAR szPath[MAX_PATH] = {'\0'};
	if (GetTempPath(MAX_PATH, szPath)) {
#ifndef _WIN64
		_tmakepath(szPath, NULL, szPath, _T("xkeylog"), _T("txt"));
#else
		_tmakepath(szPath, NULL, szPath, _T("xkeylog64"), _T("txt"));
#endif
	} else {
		_tcscpy(szPath, _T("c:\\xkeylog.txt"));
	}
	FILE *fp = _tfopen(szPath, _T("a"));
	_ftprintf(fp, _T("%8d: %s	%s\n"), n++, m_szApplicationName, szLog);
	fflush(fp);
	fclose(fp);
}

BOOL CUtils::IsSh()
{
	return !_tcsicmp(m_szApplicationName, _T("sh.exe"));
}

BOOL CUtils::IsCsh()
{
	return !_tcsicmp(m_szApplicationName, _T("csh.exe"));
}

BOOL CUtils::IsVisualStudioDotNet()
{
	return !_tcsicmp(m_szApplicationName, _T("devenv.exe"));
}

BOOL CUtils::IsAccess()
{
	return !_tcsicmp(m_szApplicationName, _T("MSACCESS.EXE"));
}

BOOL CUtils::IsProject()
{
	return !_tcsicmp(m_szApplicationName, _T("WINPROJ.EXE"));
}

BOOL CUtils::IsVisualBasic()
{
	return !_tcsicmp(m_szApplicationName, _T("VB6.EXE"));
}

BOOL CUtils::IsVisualBasicEditor()
{
	if (IsVisualBasic()) {
		return TRUE;
	}

	if (IsAccess()
	 || IsExcel()
	 || IsMicrosoftFrontPage()
	 || IsMicrosoftPowerPoint()
	 || IsMicrosoftWord()
	 || IsOutlook()
	 || IsProject()) {
		TCHAR szWindowText[0x100] = {'\0'};
		GetWindowText(GetForegroundWindow(), szWindowText, sizeof(szWindowText));
		static LPCTSTR const szVBE = _T("Microsoft Visual Basic - ");
		if (!_tcsnicmp(szWindowText, szVBE, _tcslen(szVBE))) {
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CUtils::IsEclipse()
{
	TCHAR szWindowText[0x100] = {'\0'};
	GetWindowText(GetForegroundWindow(), szWindowText, sizeof(szWindowText));

	LPCTSTR szEclipse = _T(" - Eclipse Platform");
	LPCTSTR szFind = _T("Find/Replace");

	return IsJavaW()
		&& (_tcslen(szEclipse) < _tcslen(szWindowText) && !_tcsicmp(szWindowText + _tcslen(szWindowText) - _tcslen(szEclipse), szEclipse)
		 || _tcslen(szFind) == _tcslen(szWindowText) && !_tcsicmp(szWindowText, szFind));
}

int CUtils::GetClipboardTextLength()
{
	CString szClipboardText;
	CUtils::GetClipboardText(&szClipboardText);
	CUtils::SetClipboardText(&szClipboardText);

//	return sz.GetLength();

	int nLength = 0;
	for (int i = 0; i < szClipboardText.GetLength(); ++i) {
		if (szClipboardText.GetAt(i) & 0x80) {
			++i;
		}
		++nLength;
	}
	return nLength;
}

BOOL CUtils::IsDialog()
{
	HWND hwnd = GetForegroundWindow();
	TCHAR szWindowText[0x100] = {'\0'};
	if (!GetWindowText(hwnd, szWindowText, sizeof(szWindowText)))
		return FALSE; // inside sound box
	return GetParent(GetForegroundWindow()) != NULL;
}

int CUtils::GetFormatTag(LPTSTR szFormatTag)
{
	if (*(szFormatTag) != _T('%')) {
		return NULL;
	}

	unsigned int nIndex = 1;

	// flags
	while (nIndex < _tcslen(szFormatTag)) {
		switch (*(szFormatTag + nIndex)) {
		case _T('-'):
		case _T('+'):
		case _T(' '):
		case _T('0'):
		case _T('#'):
			++nIndex;
			continue;
		default:
			break;
		}

		break;
	}

	// width
	while (_istdigit(*(szFormatTag + nIndex))) {
		++nIndex;
	}

	// precision
	if (*(szFormatTag + nIndex) == _T('.')) {
		++nIndex;
		while (_istdigit(*(szFormatTag + nIndex))) {
			++nIndex;
		}
	}

	// prefix
	switch (*(szFormatTag + nIndex)) {
	case _T('h'):
	case _T('l'):
	case _T('L'):
		++nIndex;
		break;
	}

	// type
	switch (*(szFormatTag + nIndex)) {
	case _T('d'):
	case _T('i'):
	case _T('o'):
	case _T('x'):
	case _T('X'):
	case _T('u'):
	case _T('c'):
	case _T('s'):
	case _T('f'):
	case _T('e'):
	case _T('E'):
	case _T('g'):
	case _T('G'):
	case _T('p'):
	case _T('n'):
	case _T('%'):
		*(szFormatTag + nIndex + 1) = NULL;
		return *(szFormatTag + nIndex);
	default:
		*(szFormatTag + nIndex) = NULL;
		return NULL;
	}
}

BOOL CUtils::IsEudora()
{
	return !_tcsicmp(m_szApplicationName, _T("Eudora.exe"));
}

BOOL CUtils::IsCodeWarrior()
{
	return !_tcsicmp(m_szApplicationName, _T("IDE.exe"));
}

BOOL CUtils::IseMemoPad()
{
	return !_tcsicmp(m_szApplicationName, _T("eMemoPad.exe"));
}

BOOL CUtils::IsStoryEditor()
{
	return !_tcsicmp(m_szApplicationName, _T("STRYEDIT.EXE"));
}

BOOL CUtils::IsNami2000()
{
	return !_tcsicmp(m_szApplicationName, _T("Nami2000.exe"));
}

BOOL CUtils::IsCorelDRAW()
{
	return !_tcsicmp(m_szApplicationName, _T("CorelDrw.exe"));
}

// If Clipboard data is empty, return ture.
// You must CopyNextCharacter in previous step.
// Clipboard data is destroyed.
BOOL CUtils::IsEOF()
{
	return IsTOForEOF();
}

// If Clipboard data is empty, return ture.
// You must CopyBackCharacter in previous step.
// Clipboard data is destroyed.
BOOL CUtils::IsTOF()
{
	return IsTOForEOF();
}

BOOL CUtils::IsTOForEOF()
{
	CString szClipboardText;
	GetClipboardText(&szClipboardText);
	return szClipboardText.IsEmpty()			// for normal application
		|| szClipboardText.GetLength() >= 3;	// for VC++
}

BOOL CUtils::IsHusen()
{
	return !_tcsicmp(m_szApplicationName, _T("husen.exe"));
}

BOOL CUtils::IsAdobeReader()
{
	return !_tcsicmp(m_szApplicationName, _T("AcroRd32.exe"));
}

BOOL CUtils::IsOpenOffice()
{
	return !_tcsicmp(m_szApplicationName, _T("soffice.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("soffice.bin"));
}

BOOL CUtils::IsTuruKameMail()
{
	return !_tcsicmp(m_szApplicationName, _T("TuruKame.exe"));
}

BOOL CUtils::IsOedit()
{
	return !_tcsicmp(m_szApplicationName, _T("oedit.exe"));
}

BOOL CUtils::IsAutla()
{
	return !_tcsicmp(m_szApplicationName, _T("Autla.exe"));
}

BOOL CUtils::IsShuriken()
{
	return !_tcsicmp(m_szApplicationName, _T("JsvMail.exe"));
}

BOOL CUtils::IsEdLeaf()
{
	return !_tcsicmp(m_szApplicationName, _T("edleaf.exe"));
}

BOOL CUtils::IsJmEditor()
{
	return !_tcsicmp(m_szApplicationName, _T("JmEdit.exe"))
		|| !_tcsicmp(m_szApplicationName, _T("JmEdit2.exe"));
}

BOOL CUtils::IsDana()
{
	return !_tcsicmp(m_szApplicationName, _T("Dana.exe"));
}

BOOL CUtils::IsIPMessenger()
{
	return !_tcsicmp(m_szApplicationName, _T("ipmsg.exe"));
}

BOOL CUtils::IsezHTML()
{
	return !_tcsicmp(m_szApplicationName, _T("ezhtml.exe"));
}

BOOL CUtils::IsTcsh()
{
	return !_tcsicmp(m_szApplicationName, _T("tcsh.exe"));
}

BOOL CUtils::IsZsh()
{
	return !_tcsicmp(m_szApplicationName, _T("zsh.exe"));
}

BOOL CUtils::IsPdksh()
{
	return !_tcsicmp(m_szApplicationName, _T("pdksh.exe"));
}

BOOL CUtils::IsFirefox()
{
	return !_tcsicmp(m_szApplicationName, _T("firefox.exe"));
}

BOOL CUtils::IsPHPEditor()
{
	return !_tcsicmp(m_szApplicationName, _T("php_editor.exe"));
}

BOOL CUtils::IsTeraPad()
{
	return !_tcsicmp(m_szApplicationName, _T("TeraPad.exe"));
}

BOOL CUtils::IsLispWorksPersonalEdition()
{
	CString szLispWorks(_T("lispworks-personal-"));
	return !_tcsnicmp(m_szApplicationName, szLispWorks, szLispWorks.GetLength());

//	return !_tcsicmp(m_szApplicationName, _T("lispworks-personal-4300.exe"));
}

BOOL CUtils::IsBorlandCppBuilder()
{
	return !_tcsicmp(m_szApplicationName, _T("bcb.exe"));
}

BOOL CUtils::Run(CString szCommandLine, BOOL isWait)
{
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	BOOL rc = CreateProcess(NULL, szCommandLine.GetBuffer(szCommandLine.GetLength() + 1), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (rc) {
		if (isWait) {
			WaitForSingleObject( pi.hProcess, INFINITE );
		}

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

	return rc;
}

BOOL CUtils::IsJavaW()
{
	return !_tcsicmp(m_szApplicationName, _T("javaw.exe"));
}

BOOL CUtils::IsMSDN()
{
	return !_tcsicmp(m_szApplicationName, _T("hh.exe"));
}

BOOL CUtils::IsVisualSlickEdit()
{
	return !_tcsicmp(m_szApplicationName, _T("vs.exe"));
}

void CUtils::PrintWindowInfo(WINDOWINFO *pwi)
{
	Log(_T("windowinfo.cbSize: %d"), pwi->cbSize);
	Log(_T("windowinfo.rcWindow.top: %d"), pwi->rcWindow.top);
	Log(_T("windowinfo.rcWindow.bottom: %d"), pwi->rcWindow.bottom);
	Log(_T("windowinfo.rcWindow.left: %d"), pwi->rcWindow.left);
	Log(_T("windowinfo.rcWindow.right: %d"), pwi->rcWindow.right);
	Log(_T("windowinfo.rcClient.top: %d"), pwi->rcClient.top);
	Log(_T("windowinfo.rcClient.bottom: %d"), pwi->rcClient.bottom);
	Log(_T("windowinfo.rcClient.left: %d"), pwi->rcClient.left);
	Log(_T("windowinfo.rcClient.right: %d"), pwi->rcClient.right);
	Log(_T("windowinfo.dwStyle: %d"), pwi->dwStyle);
	Log(_T("windowinfo.dwExStyle: %d"), pwi->dwExStyle);
	Log(_T("windowinfo.dwWindowStatus: %d"), pwi->dwWindowStatus);
	Log(_T("windowinfo.cxWindowBorders: %d"), pwi->cxWindowBorders);
	Log(_T("windowinfo.cyWindowBorders: %d"), pwi->cyWindowBorders);
	Log(_T("windowinfo.atomWindowType: %d"), pwi->atomWindowType);
	Log(_T("windowinfo.wCreatorVersion: %d"), pwi->wCreatorVersion);
}

BOOL CUtils::IsOpenJane()
{
	return !_tcsicmp(m_szApplicationName, _T("Jane2ch.exe"));
}

BOOL CUtils::UpdateKeyboardState(BYTE bVk, BYTE bState)
{
	BYTE ks[256] = {'\0'};
	BOOL rc = FALSE;

	if ((rc = GetKeyboardState(ks)) != FALSE) {
		ks[bVk] = bState;
		rc = SetKeyboardState(ks);
	}
	return rc;
}

BOOL CUtils::IsThunderbird()
{
	return !_tcsicmp(m_szApplicationName, _T("thunderbird.exe"));
}

int CUtils::GetWindowTextType(CString strWindowText)
{
	int nWindowTextType = IDS_WINDOW_TEXT_IGNORE;

	if (3 <= strWindowText.GetLength()
	 && strWindowText.Find(_T('*')) == 0
	 && strWindowText.ReverseFind(_T('*')) == strWindowText.GetLength() - 1) {
		nWindowTextType = IDS_WINDOW_TEXT_MATCH;
	} else if (2 <= strWindowText.GetLength()
			&& strWindowText.Find(_T('*')) != 0
			&& strWindowText.ReverseFind(_T('*')) == strWindowText.GetLength() - 1) {
		nWindowTextType = IDS_WINDOW_TEXT_MATCH_FORWARD;
	} else if (2 <= strWindowText.GetLength()
			&& strWindowText.Find(_T('*')) == 0
			&& strWindowText.ReverseFind(_T('*')) != strWindowText.GetLength() - 1) {
		nWindowTextType = IDS_WINDOW_TEXT_MATCH_BACKWARD;
	} else if (!strWindowText.IsEmpty()
			&& strWindowText.Find(_T('*')) != 0
			&& strWindowText.ReverseFind(_T('*')) != strWindowText.GetLength() - 1) {
		nWindowTextType = IDS_WINDOW_TEXT_MATCH_FULL;
	}

	return nWindowTextType;
}

BOOL CUtils::IsLunascape()
{
	return !_tcsicmp(m_szApplicationName, _T("Luna.exe"));
}

BOOL CUtils::IsAtok()
{
	return !_tcsnicmp(m_szApplicationName, _T("ATOK"), 4) && !_tcsnicmp(m_szApplicationName + _tcslen(m_szApplicationName) - 4, _T(".IME"), 4);
}
