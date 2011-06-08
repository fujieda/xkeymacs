// Utils.h: interface for the CUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTILS_H__D64BCD3F_F2E2_41F7_AD58_0F32E2D46942__INCLUDED_)
#define AFX_UTILS_H__D64BCD3F_F2E2_41F7_AD58_0F32E2D46942__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xkeymacsdll.h"

class AFX_EXT_CLASS CUtils  
{
public:
	static BOOL IsAtok();
	static BOOL IsLunascape();
	static int GetWindowTextType(CString strWindowText);
	static BOOL IsThunderbird();
	static BOOL UpdateKeyboardState(BYTE bVk, BYTE bState);
	static BOOL IsOpenJane();
	static void PrintWindowInfo(WINDOWINFO *pwi);
	static BOOL IsVisualSlickEdit();
	static BOOL IsMSDN();
	static BOOL IsJavaW();
	static BOOL Run(CString szCommandLine, BOOL isWait = FALSE);
	static BOOL IsBorlandCppBuilder();
	static BOOL IsLispWorksPersonalEdition();
	static BOOL IsTeraPad();
	static BOOL IsPHPEditor();
	static BOOL IsFirefox();
	static BOOL IsPdksh();
	static BOOL IsZsh();
	static BOOL IsTcsh();
	static BOOL IsezHTML();
	static BOOL IsIPMessenger();
	static BOOL IsDana();
	static BOOL IsJmEditor();
	static BOOL IsXPorLater();
	static BOOL IsEdLeaf();
	static BOOL IsShuriken();
	static BOOL IsAutla();
	static BOOL OpenClipboard();
	static BOOL IsOedit();
	static BOOL IsTuruKameMail();
	static BOOL IsOpenOffice();
	static BOOL IsAdobeReader();
	static BOOL IsHusen();
	static BOOL IsTOF();
	static BOOL IsEOF();
	static BOOL IsCorelDRAW();
	static BOOL IsNami2000();
	static BOOL IsStoryEditor();
	static BOOL IseMemoPad();
	static BOOL IsCodeWarrior();
	static BOOL IsVisualBasicEditor();
	static BOOL IsEudora();
	static BOOL IsDialog();
	static int GetClipboardTextLength();
	static BOOL IsEclipse();
	static BOOL IsFindDialog();
	static BOOL IsVisualBasic();
	static BOOL IsProject();
	static BOOL IsAccess();
	static BOOL IsVisualStudio();
	static BOOL IsVisualStudio2010();
	static BOOL IsCsh();
	static BOOL IsSh();
	static BOOL IsBash();
	static void SetCorrectApplicationName(LPTSTR szApplicationName, LPTSTR szWindowText);
	static void Log(LPTSTR fmt, ...);
	static BOOL IsSleipnir();
	static BOOL IsConsole();
	static BOOL IsPaint();
	static BOOL IsReget();
	static BOOL IsMicrosoftPowerPoint();
	static BOOL IsIllustrator();
	static BOOL IsPhotoshop();
	static BOOL IsFlash();
	static BOOL IsDreamweaver();
	static BOOL IsFireworks();
	static BOOL IsExcel();
	static BOOL IsDirector();
	static BOOL IsEggExplorer();
	static BOOL IsK2Editor();
	static BOOL IsDWFM();
	static BOOL IsDefaultIgnoreApplication();
	static BOOL IsXWin();
	static BOOL IsEmacs();
	static BOOL SetClipboardText(CString *szClipboardText);
	static BOOL GetClipboardText(CString *szClipboardText);
	static void InitCUtils();
	static void SetApplicationName(BOOL bImeComposition);
	static LPCTSTR const GetApplicationName();
	static BOOL IsXyzzy();
	static BOOL IsWordpad();
	static BOOL IsVisualCpp();
	static BOOL IsTeraTermPro();
	static BOOL IsSakuraEditor();
	static BOOL IsOutlookExpress();
	static BOOL IsOutlook();
	static BOOL IsOpera();
	static BOOL IsNotepad();
	static BOOL IsNetscape();
	static BOOL IsMuleForWin32();
	static BOOL IsMozilla();
	static BOOL IsMicrosoftWord();
	static BOOL IsMicrosoftFrontPage();
	static BOOL IsMeadow();
	static BOOL IsLotusWordPro();
	static BOOL IsLotusNotes();
	static BOOL IsLotus123();
	static BOOL IsInternetExplorer();
	static BOOL IsHidemaru();
	static BOOL IsExplorer();
	static BOOL IsBecky();
	static BOOL IsAstecX();
	static BOOL IsXkeymacs();
	static BOOL CUtils::GetFindDialogTitle(CString *szDialogTitle);
	CUtils();
	virtual ~CUtils();

private:
	static BOOL IsTOForEOF();
	static int GetFormatTag(LPTSTR szFormatTag);
	static void FairConsoleApplicationName(LPTSTR szApplicationName, LPTSTR szWindowText);
	static BOOL IsConsole(LPCTSTR szApplicationName);
	static BOOL IsJavaW(LPCTSTR szApplicationName);
	static void SetIMEName();
	static OSVERSIONINFO m_OsVersionInfo;
	static TCHAR m_szIMEName[MAX_PATH];
	static TCHAR m_szApplicationName[MAX_PATH];
};

#endif // !defined(AFX_UTILS_H__D64BCD3F_F2E2_41F7_AD58_0F32E2D46942__INCLUDED_)
