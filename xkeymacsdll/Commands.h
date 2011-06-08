// Commands.h: interface of the CCommands class
//
//////////////////////////////////////////////////////////////////////

#include "xkeymacsdll.h"

#if !defined(AFX_COMMANDS_H__3AD9D537_AB40_40BB_9697_35F1BB0CA431__INCLUDED_)
#define AFX_COMMANDS_H__3AD9D537_AB40_40BB_9697_35F1BB0CA431__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum SEARCH_DIRECTION { NA, FORWARD, BACKWARD, AGAIN };
enum MAXIMIZE_DIRECTION { VERTICAL, HORIZONTAL, ROLL_UP_UNROLL, MAX_MAXIMIZE_DIRECTION };
enum INPUT_METHOD_OPEN_STATUS {CLOSE_INPUT_METHOD, OPEN_INPUT_METHOD, TOGGLE_INPUT_METHOD};
enum CONSOLE_MENU_EDIT { CMD_MARK = 1, CMD_COPY, CMD_PASTE, CMD_SELECT_ALL, CMD_SCROLL, CMD_FIND };
enum CONSOLE_MENU { CMD_RESTORE = 1, CMD_MOVE, CMD_SIZE, CMD_MINIMIZE, CMD_MAXIMIZE, CMD_CLOSE, CMD_EDIT, CMD_DEFAULTS, CMD_PROPERTIES };
enum CASE_WORD { DOWNCASE, UPCASE, CAPITALIZE };
const int MAX_WINDOW = 64;

struct OriginalWindowPosition
{
	HWND hWnd;
	BOOL bMax[MAX_MAXIMIZE_DIRECTION];
	int nOriginalX;			// for HORIZONTAL
	int nOriginalY;			// for VERTICAL
	int nOriginalWidth;		// for HORIZONTAL
	int nOriginalHeight;	// for VERTICAL
};

class AFX_EXT_CLASS CCommands  
{
public:
	static int ExecuteExtendedCommand();
	static int CapitalizeWord();
	static int UpcaseWord();
	static int DowncaseWord();
	static int FillParagraph();
	static int TransposeWords();
	static int CycleItemsInversely();
	static int CycleItems();
	static int OneShotModifierAltRepeat();
	static int OneShotModifierAlt();
	static int OneShotModifierCtrlRepeat();
	static int OneShotModifierCtrl();
	static int OneShotModifierShiftRepeat();
	static int OneShotModifierShift();
	static int Retop();
	static int RollUpUnroll();
	static int Underscore();
	static int CapsLock();
	static int DisableXKeymacs();
	static int EnableXKeymacs();
	static int BackwardKillWord();
	static int KillLineForAllFormat();
	static int TransposeLines();
	static int FindNext();
	static int Find();
	static int Print();
	static int New();
	static int Cut();
	static int Paste();
	static int Copy();
	static void SetTemporarilyDisableXKeymacs(BOOL bTemporarilyDisableXKeymacs);
	static BOOL IsTemporarilyDisableXKeymacs();
	static int TemporarilyDisableXKeymacs();
	static int Redo();
	static int PassThrough();
	static GOTO MoveCaret(BYTE bVk, BOOL bCtrl = FALSE);
	static int MaximizeHorizontally();
	static int MaximizeVertically();
	static int Maximize();
	static int Minimize();
	static int Size();
	static int Move();
	static int Restore();
	static void SetMark(BOOL bSetMark);
	static BOOL IsSetMark();
	static int BackQuote();
	static int Tilde();
	static int Escape();
	static int C_Eisu();
	static int C_();
	static int SwitchToBuffer();
	static int SwitchBetweenInputLocales();
	static int CallLastKbdMacro();
	static int EndKbdMacro();
	static int StartKbdMacro();
	static int SelectAll();
	static int MarkWholeBuffer();
	static int KillWord();
	static void SetLastCommand(int (*LastCommand)());
	static int KillBuffer();
	static int Reset(int rc);
	static int CloseInputMethod();
	static int CompleteCloseInputMethod();
	static int OpenInputMethod();
	static int NumericArgument(int n);
	static void ClearNumericArgument();
	static int GetNumericArgument();
	static int EnableOrDisableXKeymacs();
	static int IsearchForward();
	static int IsearchBackward();
	static int TransposeChars();
	static int Recenter();
	static int Repeat(BYTE bVk);
	static void bC_u(BOOL b);
	static BOOL bC_u();
	static int UniversalArgument();
	static int NumericArgumentMinus();
	static int NumericArgument9();
	static int NumericArgument8();
	static int NumericArgument7();
	static int NumericArgument6();
	static int NumericArgument5();
	static int NumericArgument4();
	static int NumericArgument3();
	static int NumericArgument2();
	static int NumericArgument1();
	static int NumericArgument0();
	static int MetaAlt();
	static int MetaEscape();
	static int Meta();
	static void bM_(BOOL b);
	static BOOL bM_();
	static int Ignore();
	static int ShellCommand();
	static int YankPop();
	static int Yank();
	static int KillRingSave();
	static int KillRegion();
	static int WriteFile();
	static int C_x();
	static void Reset();
	static void bC_(BOOL b);
	static BOOL bC_();
	static void bC_x(BOOL b);
	static BOOL bC_x();
	static void bM_x(BOOL b);
	static BOOL bM_x();
	static int ScrollDown();
	static int ScrollUp();
	static int SaveBuffer();
	static int MarkPage();
	static int Newline();
	static int TabToTabStop();
	static int FindFile();
	static int KeyboardQuit();
	static int SetMarkCommand();
	static int IconifyOrDeiconifyFrame();
	static int SaveBuffersKillEmacs();
	static int Undo();
	static int KillLine();
	static int DeleteChar();
	static int DeleteBackwardChar();
	static int EndOfBuffer();
	static int BeginningOfBuffer();
	static int QueryReplaceRegexp();
	static int QueryReplace();
	static int ToggleInputMethod();
	static int CompleteToggleInputMethod();
	static int OpenLine();
	static int PreviousLine();
	static int NextLine();
	static int BackwardWord();
	static int ForwardWord();
	static int EndOfLine();
	static int BackwardChar();
	static int BeginningOfLine();
	static int ForwardChar();
	static CString GetCommandName(int nCommandID);
	static int GetCategoryID(int nCommandID);
	static int GetDescriptionID(int nCommandID);
	static int GetToolTipID(int nCommandID);
	static int GetDefaultCommandType(int nCommandID, int nIndex);
	static int GetDefaultCommandKey(int nCommandID, int nIndex);
	static int GetDefaultControlID(int nCommandID, int nIndex);
	static void DoSetInputMethodOpenStatus(INPUT_METHOD_OPEN_STATUS status, BOOL isComplete = FALSE);
	CCommands();
	virtual ~CCommands();

private:
	static BOOL CutFollowingWord();
	static int CaseWord(CASE_WORD nCase);
	static BOOL CopyCurrentLine();
	static BOOL CopyPreviousLine();
	static LRESULT VScroll(UINT nSBCode, int nTimes = 1);
	static BOOL IsKillCommand(int (*nFunctionPointer)());
	static void PrintFunctionName(int (*nFunctionPointer)());
	static OriginalWindowPosition* GetOriginalWindowPosition(HWND hWnd);
	static OriginalWindowPosition m_OriginalWindowPosition[MAX_WINDOW];
	static int KillLine(BOOL bAllFormat, int (*pCommand)() = KillLine);
	static BOOL IsEmptyClipboardData(const int nID = 0);
	static int FindReturnFromClipboardData(const int nID = 0);
	static void Cut_();
	static void RestoreClipboardData(const int nID = 0);
	static void CaptureClipboardData(const int nID = 0, const BOOL bTextOnly = FALSE);
	static CArray<CClipboardSnap *, CClipboardSnap *> m_oClipboardData;
	static int SystemMenuEdit(const CONSOLE_MENU_EDIT nDown);
	static int Click(const POINT* const pClickPoint);
	static int SystemMenu(const CONSOLE_MENU nDown);
	static BOOL m_bTemporarilyDisableXKeymacs;
	static int Maximize(MAXIMIZE_DIRECTION direction);
	static DWORD DeactivateMark();
	static BOOL m_bFirstFindDialog;
	static void Copy_();
	static BYTE GetDirectionBackwardKey();
	static BYTE GetDirectionForwardKey();
	static BYTE GetSearchOptionKey();
	static BYTE GetFindWhatKey();
	static void OpenFindDialog();
	static BOOL m_bC_;
	static void CdSdKduSuCu(BYTE bVk);
	static int (*m_LastKillCommand)();
	static int (*m_LastCommand)();
	static void SetInputMethodOpenStatus(INPUT_METHOD_OPEN_STATUS status, BOOL isComplete = FALSE);
	static void SetSearchDirection(SEARCH_DIRECTION direction = m_SearchDirection);
	static SEARCH_DIRECTION m_SearchDirection;
	static int Search(SEARCH_DIRECTION direction);
	static void DecreaseNumericArgument();
	static BOOL CopyBackCharacter();
	static BOOL CopyNextCharacter();
	static BOOL m_bC_u;
	static BOOL m_bNegativeNumericArgument;
	static BOOL m_bDefaultNumericArgument;
	static BOOL m_bM_x;
	static BOOL m_bM_;
	static BOOL m_bC_x;
	static DWORD ClickCaret();
	static BOOL m_bSetMark;
	static void ReleaseKey(BYTE bVk);
	static void DepressKey(BYTE bVk);
	static void SdKduSu(BYTE bVk1, BYTE bVk2 = 0, BYTE bVk3 = 0, BYTE bVk4 = 0);
	static void AdSdKduSuAu(BYTE bVk1);
	static void AdKduAu(BYTE bVk1, BYTE bVk2 = 0, BYTE bVk3 = 0);
	static void CdKduCu(BYTE bVk1, BYTE bVk2 = 0);
	static void Kdu(BYTE bVk1 = 0, BYTE bVk2 = 0, BYTE bVk3 = 0, BYTE bVk4 = 0);
	static DWORD m_nNumericArgument;
	static void Su();
	static BOOL m_bIsSu;
};

const int MAX_KEY_BIND = 8;

struct Command
{
	LPCTSTR szCommandName;
	int nCategoryID;
	int nDescriptionID;
	int nToolTipID;
	int (*fCommand)();
	KeyBind keybind[MAX_KEY_BIND];
};

#include "../xkeymacs/resource.h"

const int MAX_COMMAND = 115;
extern AFX_EXT_DATA const Command Commands[MAX_COMMAND];

#endif // !defined(AFX_COMMANDS_H__3AD9D537_AB40_40BB_9697_35F1BB0CA431__INCLUDED_)
