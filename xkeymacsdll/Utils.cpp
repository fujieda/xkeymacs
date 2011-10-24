// Utils.cpp: implementation of the CUtils class
//
//////////////////////////////////////////////////////////////////////

#include "Utils.h"
#include "defs.h"
#include "../xkeymacs/resource.h"
#include <Imm.h>

AppName CUtils::m_appName;

LPCTSTR CUtils::GetAppName()
{
	return m_appName.GetAppName();
}

void CUtils::SetAppName()
{
	m_appName.SetAppName();
}

void CUtils::SetIMEState(bool on)
{
	m_appName.SetIMEState(on);
}

void CUtils::CorrectAppName(TCHAR (&appName)[MAX_PATH], TCHAR (&title)[WINDOW_TEXT_LENGTH])
{
	m_appName.CorrectAppName(appName, title);
}

bool CUtils::IsConsole()
{
	return m_appName.IsConsole();
}

BOOL CUtils::GetFindDialogTitle(CString *szDialogTitle)
{
	{
		TCHAR buf[WINDOW_TEXT_LENGTH] = {'\0'};
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

BOOL CUtils::IsXkeymacs()
{
	LPCTSTR names[] = {
		_T("xkeymacs.exe"),
		_T("xkeymacs64.exe"),
	};
	return m_appName.Match(names);
}

BOOL CUtils::IsChrome()
{
	return m_appName.Match(_T("chrome.exe"));
}

BOOL CUtils::IsAstecX()
{
	return m_appName.Match(_T("astecx.exe"));
}

BOOL CUtils::IsBecky()
{
	return m_appName.Match(_T("b2.exe"));
}

BOOL CUtils::IsEmacs()
{
	return m_appName.Match(_T("Emacs.exe"));
}

BOOL CUtils::IsExplorer()
{
	return m_appName.Match(_T("explorer.exe"));
}

BOOL CUtils::IsHidemaru()
{
	return m_appName.Match(_T("hidemaru.exe"));
}

BOOL CUtils::IsInternetExplorer()
{
	return m_appName.Match(_T("iexplore.exe"));
}

BOOL CUtils::IsLotus123()
{
	return m_appName.Match(_T("123w.exe"));
}

BOOL CUtils::IsLotusNotes()
{
	return m_appName.Match(_T("nlnotes.exe"));
}

BOOL CUtils::IsLotusWordPro()
{
	return m_appName.Match(_T("wordpro.exe"));
}

BOOL CUtils::IsMeadow()
{
	LPCTSTR names[] = {
		_T("Meadow.exe"),
		_T("Meadow95.exe"),
		_T("MeadowNT.exe"),
	};
	return m_appName.Match(names);
}

BOOL CUtils::IsMicrosoftFrontPage()
{
	return m_appName.Match(_T("frontpg.exe"));
}

BOOL CUtils::IsMicrosoftWord()
{
	return m_appName.Match(_T("winword.exe"));
}

BOOL CUtils::IsMozilla()
{
	LPCTSTR names[] = {
		_T("mozilla.exe"),
		_T("firefox.exe"),
	};
	return m_appName.Match(names);
}

BOOL CUtils::IsMuleForWin32()
{
	LPCTSTR names[] = {
		_T("mule.exe"),
		_T("mulent.exe"),
		_T("mulecd.exe"),
	};
	return m_appName.Match(names);
}

BOOL CUtils::IsNetscape()
{
	LPCTSTR names[] = {
		_T("netscp6.exe"),
		_T("netscp.exe"),
	};
	return m_appName.Match(names);
}

BOOL CUtils::IsNotepad()
{
	return m_appName.Match(_T("notepad.exe"));
}

BOOL CUtils::IsNotepadPP()
{
	return m_appName.Match(_T("notepad++.exe"));
}

BOOL CUtils::IsOpera()
{
	return m_appName.Match(_T("opera.exe"));
}

BOOL CUtils::IsOutlook()
{
	return m_appName.Match(_T("outlook.exe"));
}

BOOL CUtils::IsOutlookExpress()
{
	return m_appName.Match(_T("msimn.exe"));
}

BOOL CUtils::IsSakuraEditor()
{
	return m_appName.Match(_T("sakura.exe"));
}

BOOL CUtils::IsTeraTermPro()
{
	return m_appName.Match(_T("ttermpro.exe"));
}

BOOL CUtils::IsVisualCpp()
{
	return m_appName.Match(_T("msdev.exe"));
}

BOOL CUtils::IsWordpad()
{
	return m_appName.Match(_T("wordpad.exe"));
}

BOOL CUtils::IsXWin()
{
	return m_appName.Match(_T("XWin.exe"));
}

BOOL CUtils::IsXyzzy()
{
	return m_appName.Match(_T("xyzzy.exe"));
}

BOOL CUtils::OpenClipboard()
{
	// OpenClipboard will fail on OpenOffice.org/LibreOffice at the first time
	// and we must retry multiple times.
	for (int i = 0; i < 10; ++i, Sleep(1))
		if (::OpenClipboard(NULL))
			return TRUE;
//		else
//			Log(_T("failed to open clipboard: %d (%d)"), GetLastError(), i);
	return FALSE;
}

BOOL CUtils::GetClipboardText(CString& text)
{
	text.Empty();
	if (!OpenClipboard())
		return FALSE;
/*
	Log(_T("GetClipboardText"));
	UINT uFormat = 0;
	while (uFormat = EnumClipboardFormats(uFormat))
		Log(_T("uFormat = %d"), uFormat);
*/
	const HANDLE hClipboard = GetClipboardData(CF_TEXT);
	if (!hClipboard) {
		CloseClipboard();
		return FALSE;
	}
	const LPVOID pMem = GlobalLock(hClipboard);
	if (!pMem)
		return FALSE;
	text = reinterpret_cast<LPCSTR>(pMem);
	GlobalUnlock(hClipboard);
	EmptyClipboard();
	CloseClipboard();
	return TRUE;
}

BOOL CUtils::SetClipboardText(const CString& text)
{
	if (!OpenClipboard())
		return FALSE;
	const int nLength = text.GetLength() + 1;
	const HGLOBAL hClipboard = GlobalAlloc(GHND, nLength);
	if (!hClipboard)
		return FALSE;
	const LPVOID pMem = GlobalLock(hClipboard);
	memcpy(pMem, LPCTSTR(text), nLength);
	GlobalUnlock(hClipboard);
	EmptyClipboard();
	if (SetClipboardData(CF_TEXT, hClipboard) == NULL) {
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
	return m_appName.Match(_T("dwfm.exe"));
}

BOOL CUtils::IsK2Editor()
{
	return m_appName.Match(_T("K2Editor.exe"));
}

BOOL CUtils::IsEggExplorer()
{
	return m_appName.Match(_T("EggExp.exe"));
}

BOOL CUtils::IsDirector()
{
	return m_appName.Match(_T("Director.exe"));
}

BOOL CUtils::IsExcel()
{
	return m_appName.Match(_T("Excel.exe"));
}

BOOL CUtils::IsFireworks()
{
	LPCTSTR names[] = {
		_T("Fireworks 4.exe"),
		_T("Fireworks.exe"),
	};
	return m_appName.Match(names);
}

BOOL CUtils::IsDreamweaver()
{
	return m_appName.Match(_T("Dreamweaver.exe"));
}

BOOL CUtils::IsFlash()
{
	return m_appName.Match(_T("Flash.exe"));
}

BOOL CUtils::IsPhotoshop()
{
	return m_appName.Match(_T("Photoshp.exe"));
}

BOOL CUtils::IsIllustrator()
{
	return m_appName.Match(_T("Illustrator.exe"));
}

BOOL CUtils::IsMicrosoftPowerPoint()
{
	return m_appName.Match(_T("PowerPnt.exe"));
}

BOOL CUtils::IsReget()
{
	LPCTSTR names[] = {
		_T("Regetdx.exe"),
		_T("Regetjr.exe"),
	};
	return m_appName.Match(names);
}

BOOL CUtils::IsPaint()
{
	return m_appName.Match(_T("mspaint.exe"));
}

BOOL CUtils::IsSleipnir()
{
	return m_appName.Match(_T("Sleipnir.exe"));
}

BOOL CUtils::IsBash()
{
	return m_appName.Match(_T("bash.exe"));
}

static void invalid_parameter_handler(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t)
{
	return;
}

// for debug
void CUtils::Log(LPCTSTR fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	TCHAR log[1024];
	_set_invalid_parameter_handler(invalid_parameter_handler);
	if (_vstprintf_s(log, fmt, ap) < 0)
		_tcscpy_s(log, _T("invalid format"));

	static int n = 0;
	TCHAR path[MAX_PATH];
	if (GetTempPath(MAX_PATH, path)) {
#ifndef _WIN64
		_tmakepath_s(path, NULL, path, _T("xkeylog"), _T("txt"));
#else
		_tmakepath_s(path, NULL, path, _T("xkeylog64"), _T("txt"));
#endif
	} else
		_tcscpy_s(path, _T("c:\\xkeylog.txt"));
	FILE *fp;
	_tfopen_s(&fp, path, _T("a"));
	_ftprintf(fp, _T("%8d: %s\t%s\n"), n++, m_appName.GetAppName(), log);
	fclose(fp);
}

BOOL CUtils::IsSh()
{
	return m_appName.Match(_T("sh.exe"));
}

BOOL CUtils::IsCsh()
{
	return m_appName.Match(_T("csh.exe"));
}

BOOL CUtils::IsVisualStudio()
{
	if (IsVisualCpp() || CUtils::IsVisualBasic())
		return FALSE;
	TCHAR szWindowText[WINDOW_TEXT_LENGTH] = _T("");
	GetWindowText(GetForegroundWindow(), szWindowText, sizeof(szWindowText));
	return _tcsstr(szWindowText, _T(" - Microsoft Visual ")) != NULL;
}

BOOL CUtils::IsAccess()
{
	return m_appName.Match(_T("MSACCESS.EXE"));
}

BOOL CUtils::IsProject()
{
	return m_appName.Match(_T("WINPROJ.EXE"));
}

BOOL CUtils::IsVisualBasic()
{
	return m_appName.Match(_T("VB6.EXE"));
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
		TCHAR szWindowText[WINDOW_TEXT_LENGTH] = {'\0'};
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
	return m_appName.Match(_T("eclipse.exe"));
}

BOOL CUtils::IsDialog()
{
	HWND hwnd = GetForegroundWindow();
	TCHAR szWindowText[WINDOW_TEXT_LENGTH] = {'\0'};
	if (!GetWindowText(hwnd, szWindowText, sizeof(szWindowText)))
		return FALSE; // inside sound box
	return GetParent(GetForegroundWindow()) != NULL;
}

BOOL CUtils::IsEudora()
{
	return m_appName.Match(_T("Eudora.exe"));
}

BOOL CUtils::IsCodeWarrior()
{
	return m_appName.Match(_T("IDE.exe"));
}

BOOL CUtils::IseMemoPad()
{
	return m_appName.Match(_T("eMemoPad.exe"));
}

BOOL CUtils::IsStoryEditor()
{
	return m_appName.Match(_T("STRYEDIT.EXE"));
}

BOOL CUtils::IsNami2000()
{
	return m_appName.Match(_T("Nami2000.exe"));
}

BOOL CUtils::IsCorelDRAW()
{
	return m_appName.Match(_T("CorelDrw.exe"));
}

// If Clipboard data is empty, return true.
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
	CString text;
	GetClipboardText(text);
	return text.IsEmpty() || // for normal application
			text.GetLength() >= 3; // for VC++
}

BOOL CUtils::IsHusen()
{
	return m_appName.Match(_T("husen.exe"));
}

BOOL CUtils::IsAdobeReader()
{
	return m_appName.Match(_T("AcroRd32.exe"));
}

BOOL CUtils::IsOpenOffice()
{
	LPCTSTR names[] = {
		_T("soffice.exe"),
		_T("soffice.bin"),
	};
	return m_appName.Match(names);
}

BOOL CUtils::IsTuruKameMail()
{
	return m_appName.Match(_T("TuruKame.exe"));
}

BOOL CUtils::IsOedit()
{
	return m_appName.Match(_T("oedit.exe"));
}

BOOL CUtils::IsAutla()
{
	return m_appName.Match(_T("Autla.exe"));
}

BOOL CUtils::IsShuriken()
{
	return m_appName.Match(_T("JsvMail.exe"));
}

BOOL CUtils::IsEdLeaf()
{
	return m_appName.Match(_T("edleaf.exe"));
}

BOOL CUtils::IsJmEditor()
{
	LPCTSTR names[] = {
		_T("JmEdit.exe"),
		_T("JmEdit2.exe"),
	};
	return m_appName.Match(names);
}

BOOL CUtils::IsDana()
{
	return m_appName.Match(_T("Dana.exe"));
}

BOOL CUtils::IsIPMessenger()
{
	return m_appName.Match(_T("ipmsg.exe"));
}

BOOL CUtils::IsezHTML()
{
	return m_appName.Match(_T("ezhtml.exe"));
}

BOOL CUtils::IsTcsh()
{
	return m_appName.Match(_T("tcsh.exe"));
}

BOOL CUtils::IsZsh()
{
	return m_appName.Match(_T("zsh.exe"));
}

BOOL CUtils::IsPdksh()
{
	return m_appName.Match(_T("pdksh.exe"));
}

BOOL CUtils::IsFirefox()
{
	return m_appName.Match(_T("firefox.exe"));
}

BOOL CUtils::IsPHPEditor()
{
	return m_appName.Match(_T("php_editor.exe"));
}

BOOL CUtils::IsTeraPad()
{
	return m_appName.Match(_T("TeraPad.exe"));
}

BOOL CUtils::IsLispWorksPersonalEdition()
{
	TCHAR prefix[] = _T("lispworks-personal-");
	return !_tcsnicmp(m_appName.GetAppName(), prefix, _countof(prefix));
}

BOOL CUtils::IsBorlandCppBuilder()
{
	return m_appName.Match(_T("bcb.exe"));
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
	return m_appName.Match(_T("javaw.exe"));
}

BOOL CUtils::IsMSDN()
{
	return m_appName.Match(_T("hh.exe"));
}

BOOL CUtils::IsVisualSlickEdit()
{
	return m_appName.Match(_T("vs.exe"));
}

BOOL CUtils::IsOpenJane()
{
	return m_appName.Match(_T("Jane2ch.exe"));
}

BOOL CUtils::IsThunderbird()
{
	return m_appName.Match(_T("thunderbird.exe"));
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
	return m_appName.Match(_T("Luna.exe"));
}

BOOL CUtils::IsAtok()
{
	LPCTSTR name = m_appName.GetAppName();
	return !_tcsnicmp(name, _T("ATOK"), 4) && !_tcsicmp(name + _tcslen(name) - 4, _T(".IME"));
}
