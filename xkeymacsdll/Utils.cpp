// Utils.cpp: implementation of the CUtils class
//
//////////////////////////////////////////////////////////////////////

#include "Utils.h"
#include "defs.h"
#include "../xkeymacs/resource.h"
#include <Imm.h>

bool CUtils::IsConsole()
{
	return AppName::IsConsole();
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
	return AppName::Match(names);
}

BOOL CUtils::IsChrome()
{
	return AppName::Match(_T("chrome.exe"));
}

BOOL CUtils::IsAstecX()
{
	return AppName::Match(_T("astecx.exe"));
}

BOOL CUtils::IsBecky()
{
	return AppName::Match(_T("b2.exe"));
}

BOOL CUtils::IsEmacs()
{
	return AppName::Match(_T("Emacs.exe"));
}

BOOL CUtils::IsExplorer()
{
	return AppName::Match(_T("explorer.exe"));
}

BOOL CUtils::IsHidemaru()
{
	return AppName::Match(_T("hidemaru.exe"));
}

BOOL CUtils::IsInternetExplorer()
{
	return AppName::Match(_T("iexplore.exe"));
}

BOOL CUtils::IsLotus123()
{
	return AppName::Match(_T("123w.exe"));
}

BOOL CUtils::IsLotusNotes()
{
	return AppName::Match(_T("nlnotes.exe"));
}

BOOL CUtils::IsLotusWordPro()
{
	return AppName::Match(_T("wordpro.exe"));
}

BOOL CUtils::IsMeadow()
{
	LPCTSTR names[] = {
		_T("Meadow.exe"),
		_T("Meadow95.exe"),
		_T("MeadowNT.exe"),
	};
	return AppName::Match(names);
}

BOOL CUtils::IsMicrosoftFrontPage()
{
	return AppName::Match(_T("frontpg.exe"));
}

BOOL CUtils::IsMicrosoftWord()
{
	return AppName::Match(_T("winword.exe"));
}

BOOL CUtils::IsMozilla()
{
	LPCTSTR names[] = {
		_T("mozilla.exe"),
		_T("firefox.exe"),
	};
	return AppName::Match(names);
}

BOOL CUtils::IsMuleForWin32()
{
	LPCTSTR names[] = {
		_T("mule.exe"),
		_T("mulent.exe"),
		_T("mulecd.exe"),
	};
	return AppName::Match(names);
}

BOOL CUtils::IsNetscape()
{
	LPCTSTR names[] = {
		_T("netscp6.exe"),
		_T("netscp.exe"),
	};
	return AppName::Match(names);
}

BOOL CUtils::IsNotepad()
{
	return AppName::Match(_T("notepad.exe"));
}

BOOL CUtils::IsNotepadPP()
{
	return AppName::Match(_T("notepad++.exe"));
}

BOOL CUtils::IsOpera()
{
	return AppName::Match(_T("opera.exe"));
}

BOOL CUtils::IsOutlook()
{
	return AppName::Match(_T("outlook.exe"));
}

BOOL CUtils::IsOutlookExpress()
{
	return AppName::Match(_T("msimn.exe"));
}

BOOL CUtils::IsSakuraEditor()
{
	return AppName::Match(_T("sakura.exe"));
}

BOOL CUtils::IsTeraTermPro()
{
	return AppName::Match(_T("ttermpro.exe"));
}

BOOL CUtils::IsVisualCpp()
{
	return AppName::Match(_T("msdev.exe"));
}

BOOL CUtils::IsWordpad()
{
	return AppName::Match(_T("wordpad.exe"));
}

BOOL CUtils::IsXWin()
{
	return AppName::Match(_T("XWin.exe"));
}

BOOL CUtils::IsXyzzy()
{
	return AppName::Match(_T("xyzzy.exe"));
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
	text = reinterpret_cast<LPCTSTR>(pMem);
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
	return AppName::Match(_T("dwfm.exe"));
}

BOOL CUtils::IsK2Editor()
{
	return AppName::Match(_T("K2Editor.exe"));
}

BOOL CUtils::IsEggExplorer()
{
	return AppName::Match(_T("EggExp.exe"));
}

BOOL CUtils::IsDirector()
{
	return AppName::Match(_T("Director.exe"));
}

BOOL CUtils::IsExcel()
{
	return AppName::Match(_T("Excel.exe"));
}

BOOL CUtils::IsFireworks()
{
	LPCTSTR names[] = {
		_T("Fireworks 4.exe"),
		_T("Fireworks.exe"),
	};
	return AppName::Match(names);
}

BOOL CUtils::IsDreamweaver()
{
	return AppName::Match(_T("Dreamweaver.exe"));
}

BOOL CUtils::IsFlash()
{
	return AppName::Match(_T("Flash.exe"));
}

BOOL CUtils::IsPhotoshop()
{
	return AppName::Match(_T("Photoshp.exe"));
}

BOOL CUtils::IsIllustrator()
{
	return AppName::Match(_T("Illustrator.exe"));
}

BOOL CUtils::IsMicrosoftPowerPoint()
{
	return AppName::Match(_T("PowerPnt.exe"));
}

BOOL CUtils::IsReget()
{
	LPCTSTR names[] = {
		_T("Regetdx.exe"),
		_T("Regetjr.exe"),
	};
	return AppName::Match(names);
}

BOOL CUtils::IsPaint()
{
	return AppName::Match(_T("mspaint.exe"));
}

BOOL CUtils::IsSleipnir()
{
	return AppName::Match(_T("Sleipnir.exe"));
}

BOOL CUtils::IsBash()
{
	return AppName::Match(_T("bash.exe"));
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
	_ftprintf(fp, _T("%8d: %s\t%s\n"), n++, AppName::GetAppName(), log);
	fclose(fp);
}

BOOL CUtils::IsSh()
{
	return AppName::Match(_T("sh.exe"));
}

BOOL CUtils::IsCsh()
{
	return AppName::Match(_T("csh.exe"));
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
	return AppName::Match(_T("MSACCESS.EXE"));
}

BOOL CUtils::IsProject()
{
	return AppName::Match(_T("WINPROJ.EXE"));
}

BOOL CUtils::IsVisualBasic()
{
	return AppName::Match(_T("VB6.EXE"));
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
	return AppName::Match(_T("eclipse.exe"));
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
	return AppName::Match(_T("Eudora.exe"));
}

BOOL CUtils::IsCodeWarrior()
{
	return AppName::Match(_T("IDE.exe"));
}

BOOL CUtils::IseMemoPad()
{
	return AppName::Match(_T("eMemoPad.exe"));
}

BOOL CUtils::IsStoryEditor()
{
	return AppName::Match(_T("STRYEDIT.EXE"));
}

BOOL CUtils::IsNami2000()
{
	return AppName::Match(_T("Nami2000.exe"));
}

BOOL CUtils::IsCorelDRAW()
{
	return AppName::Match(_T("CorelDrw.exe"));
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
	return AppName::Match(_T("husen.exe"));
}

BOOL CUtils::IsAdobeReader()
{
	return AppName::Match(_T("AcroRd32.exe"));
}

BOOL CUtils::IsOpenOffice()
{
	LPCTSTR names[] = {
		_T("soffice.exe"),
		_T("soffice.bin"),
	};
	return AppName::Match(names);
}

BOOL CUtils::IsTuruKameMail()
{
	return AppName::Match(_T("TuruKame.exe"));
}

BOOL CUtils::IsOedit()
{
	return AppName::Match(_T("oedit.exe"));
}

BOOL CUtils::IsAutla()
{
	return AppName::Match(_T("Autla.exe"));
}

BOOL CUtils::IsShuriken()
{
	return AppName::Match(_T("JsvMail.exe"));
}

BOOL CUtils::IsEdLeaf()
{
	return AppName::Match(_T("edleaf.exe"));
}

BOOL CUtils::IsJmEditor()
{
	LPCTSTR names[] = {
		_T("JmEdit.exe"),
		_T("JmEdit2.exe"),
	};
	return AppName::Match(names);
}

BOOL CUtils::IsDana()
{
	return AppName::Match(_T("Dana.exe"));
}

BOOL CUtils::IsIPMessenger()
{
	return AppName::Match(_T("ipmsg.exe"));
}

BOOL CUtils::IsezHTML()
{
	return AppName::Match(_T("ezhtml.exe"));
}

BOOL CUtils::IsTcsh()
{
	return AppName::Match(_T("tcsh.exe"));
}

BOOL CUtils::IsZsh()
{
	return AppName::Match(_T("zsh.exe"));
}

BOOL CUtils::IsPdksh()
{
	return AppName::Match(_T("pdksh.exe"));
}

BOOL CUtils::IsFirefox()
{
	return AppName::Match(_T("firefox.exe"));
}

BOOL CUtils::IsPHPEditor()
{
	return AppName::Match(_T("php_editor.exe"));
}

BOOL CUtils::IsTeraPad()
{
	return AppName::Match(_T("TeraPad.exe"));
}

BOOL CUtils::IsLispWorksPersonalEdition()
{
	TCHAR prefix[] = _T("lispworks-personal-");
	return !_tcsnicmp(AppName::GetAppName(), prefix, _countof(prefix));
}

BOOL CUtils::IsBorlandCppBuilder()
{
	return AppName::Match(_T("bcb.exe"));
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
	return AppName::Match(_T("javaw.exe"));
}

BOOL CUtils::IsMSDN()
{
	return AppName::Match(_T("hh.exe"));
}

BOOL CUtils::IsVisualSlickEdit()
{
	return AppName::Match(_T("vs.exe"));
}

BOOL CUtils::IsOpenJane()
{
	return AppName::Match(_T("Jane2ch.exe"));
}

BOOL CUtils::IsThunderbird()
{
	return AppName::Match(_T("thunderbird.exe"));
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
	return AppName::Match(_T("Luna.exe"));
}

BOOL CUtils::IsAtok()
{
	LPCTSTR name = AppName::GetAppName();
	return !_tcsnicmp(name, _T("ATOK"), 4) && !_tcsicmp(name + _tcslen(name) - 4, _T(".IME"));
}
