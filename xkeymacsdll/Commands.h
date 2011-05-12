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
enum { MAX_WINDOW = 64 };
enum CASE_WORD { DOWNCASE, UPCASE, CAPITALIZE };

typedef struct OriginalWindowPosition
{
	HWND hWnd;
	BOOL bMax[MAX_MAXIMIZE_DIRECTION];
	int nOriginalX;			// for HORIZONTAL
	int nOriginalY;			// for VERTICAL
	int nOriginalWidth;		// for HORIZONTAL
	int nOriginalHeight;	// for VERTICAL
} OriginalWindowPosition_t;

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
	CCommands();
	virtual ~CCommands();

private:
	static BOOL CutFollowingWord();
	static int CaseWord(CASE_WORD nCase);
	static BOOL CopyCurrentLine();
	static BOOL CopyPreviousLine();
	static LRESULT VScroll(UINT nSBCode, int nTimes = 1);
	static BOOL GetCaretPos(LPPOINT lpPoint);
	static BOOL IsKillCommand(int (*nFunctionPointer)());
	static void PrintFunctionName(int (*nFunctionPointer)());
	static OriginalWindowPosition_t* GetOriginalWindowPosition(HWND hWnd);
	static OriginalWindowPosition_t m_OriginalWindowPosition[MAX_WINDOW];
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
	static BOOL SetCaretPosition();
	static void GetCaretPosition();
	static CPtrList m_FindText;
	static CPtrList m_ScrollInfo;
	static CPtrList m_CaretPos;
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

enum { MAX_KEY_BIND = 8 };

typedef struct Command
{
	LPCTSTR szCommandName;
	int nCategoryID;
	int nDescriptionID;
	int nToolTipID;
	int (*fCommand)();
	KeyBind keybind[MAX_KEY_BIND];
} Command_t;

#include "../xkeymacs/resource.h"

static const Command_t Commands[] = {
	{_T("[unassigned]"),				0,				0,									0,									NULL,								{'\0'}},
	{_T("C-x"),							0,				IDS_CONTROL_X_PREFIX_D,				IDS_CONTROL_X_PREFIX_T,				CCommands::C_x,						{'\0'}},
	{_T("C-"),							IDS_OTHER,		IDS_CONTROL_D,						IDS_CONTROL_T,						CCommands::C_,						{{NONE, VK_CONTROL, 0},
																																									 {NONE, VK_LCONTROL, 0},
																																									 {NONE, VK_RCONTROL, 0},
																																									 {'\0'}}},
	{_T("C- for Eisu"),					0,				IDS_CONTROL_PREFIX_D,				IDS_CONTROL_PREFIX_T,				CCommands::C_Eisu,					{'\0'}},
	{_T("Caps Lock"),					IDS_OTHER,		IDS_CAPS_LOCK_D,					IDS_CAPS_LOCK_T,					CCommands::CapsLock,				{'\0'}},
	{_T("Close Input Method"),			IDS_IME,		IDS_CLOSE_INPUT_METHOD_D,			IDS_CLOSE_INPUT_METHOD_T,			CCommands::CloseInputMethod,		{'\0'}},
	{_T("Complete & Close Input Method"),	IDS_IME,	IDS_COMPLETE_CLOSE_INPUT_METHOD_D,	IDS_COMPLETE_CLOSE_INPUT_METHOD_T,	CCommands::CompleteCloseInputMethod,{'\0'}},
	{_T("Complete & Toggle Input Method"),	IDS_IME,	IDS_COMPLETE_TOGGLE_INPUT_METHOD_D,	IDS_COMPLETE_TOGGLE_INPUT_METHOD_T,	CCommands::CompleteToggleInputMethod,	{'\0'}},
	{_T("Copy"),						IDS_OTHER,		IDS_COPY_D,							IDS_COPY_T,							CCommands::Copy,					{'\0'}},
	{_T("Cut"),							IDS_OTHER,		IDS_CUT_D,							IDS_CUT_T,							CCommands::Cut,						{'\0'}},
	{_T("Cycle items"),					IDS_OTHER,		IDS_CYCLE_ITEMS_D,					IDS_CYCLE_ITEMS_T,					CCommands::CycleItems,				{'\0'}},
	{_T("Cycle items inversely"),		IDS_OTHER,		IDS_CYCLE_ITEMS_INVERSELY_D,		IDS_CYCLE_ITEMS_INVERSELY_T,		CCommands::CycleItemsInversely,		{'\0'}},
	{_T("Disable XKeymacs"),			IDS_OTHER,		IDS_DISABLE_XKEYMACS_D,				IDS_DISABLE_XKEYMACS_T,				CCommands::DisableXKeymacs,			{'\0'}},
	{_T("Enable or Disable XKeymacs"),	IDS_OTHER,		IDS_ENABLE_OR_DISABLE_XKEYMACS_D,	IDS_ENABLE_OR_DISABLE_XKEYMACS_T,	CCommands::EnableOrDisableXKeymacs,	{{CONTROL, 'Q', IDC_CQ},
																																									 {CONTROL+SHIFT, 'Q', IDC_CQ},
																																									 {'\0'}}},
	{_T("Enable XKeymacs"),				IDS_OTHER,		IDS_ENABLE_XKEYMACS_D,				IDS_ENABLE_XKEYMACS_T,				CCommands::EnableXKeymacs,			{'\0'}},
	{_T("newline"),						IDS_OTHER,		IDS_NEWLINE_D,						IDS_NEWLINE_T,						CCommands::Newline,					{{CONTROL, 'M', IDC_CM},
																																									{CONTROL+SHIFT, 'M', IDC_CM},
																																									 {'\0'}}},
	{_T("Esc"),							IDS_OTHER,		IDS_ESCAPE_D,						IDS_ESCAPE_T,						CCommands::Escape,					{'\0'}},
	{_T("Find"),						IDS_OTHER,		IDS_FIND_D,							IDS_FIND_T,							CCommands::Find,					{'\0'}},
	{_T("Find Next"),					IDS_OTHER,		IDS_FIND_NEXT_D,					IDS_FIND_NEXT_T,					CCommands::FindNext,				{'\0'}},
	{_T("Ignore"),						IDS_OTHER,		IDS_IGNORE_D,						IDS_IGNORE_T,						CCommands::Ignore,					{'\0'}},
	{_T("Kill Line for All Format"),	IDS_OTHER,		IDS_KILL_LINE_FOR_ALL_FORMAT_D,		IDS_KILL_LINE_FOR_ALL_FORMAT_T,		CCommands::KillLineForAllFormat,	{'\0'}},
	{_T("Maximize"),					IDS_OTHER,		IDS_MAXIMIZE_D,						IDS_MAXIMIZE_T,						CCommands::Maximize,				{'\0'}},
	{_T("Maximize Horizontally"),		IDS_OTHER,		IDS_MAXIMIZE_HORIZONTALLY_D,		IDS_MAXIMIZE_HORIZONTALLY_T,		CCommands::MaximizeHorizontally,	{'\0'}},
	{_T("Maximize Vertically"),			IDS_OTHER,		IDS_MAXIMIZE_VERTICALLY_D,			IDS_MAXIMIZE_VERTICALLY_T,			CCommands::MaximizeVertically,		{'\0'}},
	{_T("Meta"),						IDS_OTHER,		IDS_META_PREFIX_D,					IDS_META_PREFIX_T,					CCommands::Meta,					{{CONTROL, 0xDB, IDC_SQUARE_BRA},
																																									 {'\0'}}},	// VK_OEM_4
	{_T("Meta for Alt"),				IDS_OTHER,		IDS_META_D,							IDS_META_T,							CCommands::MetaAlt,					{{NONE, VK_MENU, IDC_ALT},
																																									 {NONE, VK_LMENU, IDC_LEFT_ALT},
																																									 {NONE, VK_RMENU, IDC_RIGHT_ALT},
																																									 {'\0'}}},
	{_T("Meta for Escape"),				IDS_OTHER,		IDS_META_PREFIX_AND_ORIGINAL_D,		IDS_META_PREFIX_AND_ORIGINAL_T,		CCommands::MetaEscape,				{{NONE, VK_ESCAPE, IDC_ESCAPE},
																																									 {NONE+SHIFT, VK_ESCAPE, IDC_ESCAPE},
																																									 {'\0'}}},	// add SHIFT
	{_T("Minimize"),					IDS_OTHER,		IDS_MINIMIZE_D,						IDS_MINIMIZE_T,						CCommands::Minimize,				{'\0'}},
	{_T("Move"),						IDS_OTHER,		IDS_MOVE_D,							IDS_MOVE_T,							CCommands::Move,					{'\0'}},
	{_T("New"),							IDS_OTHER,		IDS_NEW_D,							IDS_NEW_T,							CCommands::New,						{'\0'}},
	{_T("One Shot Modifier Alt"),		IDS_MODIFIER,	IDS_OSMODIFIER_ALT_D,				IDS_OSMODIFIER_ALT_T,				CCommands::OneShotModifierAlt,		{'\0'}},
	{_T("One Shot Modifier Alt with Repeat"),	IDS_MODIFIER,	IDS_OSMODIFIER_ALT_REPEAT_D,	IDS_OSMODIFIER_ALT_REPEAT_T,	CCommands::OneShotModifierAltRepeat,	{'\0'}},
	{_T("One Shot Modifier Ctrl"),		IDS_MODIFIER,	IDS_OSMODIFIER_CTRL_D,				IDS_OSMODIFIER_CTRL_T,				CCommands::OneShotModifierCtrl,		{'\0'}},
	{_T("One Shot Modifier Ctrl with Repeat"),	IDS_MODIFIER,	IDS_OSMODIFIER_CTRL_REPEAT_D,	IDS_OSMODIFIER_CTRL_REPEAT_T,	CCommands::OneShotModifierCtrlRepeat,	{'\0'}},
	{_T("One Shot Modifier Shift"),		IDS_MODIFIER,	IDS_OSMODIFIER_SHIFT_D,				IDS_OSMODIFIER_SHIFT_T,				CCommands::OneShotModifierShift,	{'\0'}},
	{_T("One Shot Modifier Shift with Repeat"),	IDS_MODIFIER,	IDS_OSMODIFIER_SHIFT_REPEAT_D,	IDS_OSMODIFIER_SHIFT_REPEAT_T,	CCommands::OneShotModifierShiftRepeat,	{'\0'}},
	{_T("Open Input Method"),			IDS_IME,		IDS_OPEN_INPUT_METHOD_D,			IDS_OPEN_INPUT_METHOD_T,			CCommands::OpenInputMethod,			{'\0'}},
	{_T("Pass Through"),				IDS_OTHER,		IDS_PASS_THROUGH_D,					IDS_PASS_THROUGH_T,					CCommands::PassThrough,				{'\0'}},
	{_T("Paste"),						IDS_OTHER,		IDS_PASTE_D,						IDS_PASTE_T,						CCommands::Paste,					{'\0'}},
	{_T("Print"),						IDS_OTHER,		IDS_PRINT_D,						IDS_PRINT_T,						CCommands::Print,					{'\0'}},
	{_T("Redo"),						IDS_OTHER,		IDS_REDO_D,							IDS_REDO_T,							CCommands::Redo,					{'\0'}},
	{_T("Restore"),						IDS_OTHER,		IDS_RESTORE_D,						IDS_RESTORE_T,						CCommands::Restore,					{'\0'}},
	{_T("Retop"),						IDS_OTHER,		IDS_RETOP_D,						IDS_RETOP_T,						CCommands::Retop,					{'\0'}},
	{_T("Roll-up/Unroll"),				IDS_WINDOW,		IDS_ROLL_UP_UNROLL_D,				IDS_ROLL_UP_UNROLL_T,				CCommands::RollUpUnroll,			{{META, VK_F12, IDC_MF12},
																																									 {'\0'}}},
	{_T("Select All"),					IDS_OTHER,		IDS_SELECT_ALL_D,					IDS_SELECT_ALL_T,					CCommands::SelectAll,				{'\0'}},
	{_T("Size"),						IDS_OTHER,		IDS_SIZE_D,							IDS_SIZE_T,							CCommands::Size,					{'\0'}},
	{_T("Switch Between Input Locales"),IDS_IME,		IDS_SWITCH_BETWEEN_INPUT_LOCALES_D,	IDS_SWITCH_BETWEEN_INPUT_LOCALES_T,	CCommands::SwitchBetweenInputLocales,	{'\0'}},
	{_T("Temporarily Disable XKeymacs"),IDS_OTHER,		IDS_TEMPORARILY_DISABLE_XKEYMACS_D,	IDS_TEMPORARILY_DISABLE_XKEYMACS_T,	CCommands::TemporarilyDisableXKeymacs,	{'\0'}},
	{_T("~"),							IDS_OTHER,		IDS_TILDE_D,						IDS_TILDE_T,						CCommands::Tilde,					{'\0'}},
	{_T("`"),							IDS_OTHER,		IDS_BACK_QUOTE_D,					IDS_BACK_QUOTE_T,					CCommands::BackQuote,				{'\0'}},
	{_T("_"),							IDS_OTHER,		IDS_UNDERSCORE_D,					IDS_UNDERSCORE_T,					CCommands::Underscore,				{'\0'}},
	{_T("backward-char"),				IDS_MOTION,		IDS_BACKWARD_CHAR_D,				IDS_BACKWARD_CHAR_T,				CCommands::BackwardChar,			{{CONTROL, 'B', IDC_CB},
																																									 {CONTROL+SHIFT, 'B', IDC_CB},
																																									 {'\0'}}},
	{_T("backward-kill-word"),			IDS_KILL,		IDS_BACKWARD_KILL_WORD_D,			IDS_BACKWARD_KILL_WORD_T,			CCommands::BackwardKillWord,		{{META, VK_BACK, IDC_MDEL},
																																									 {META+SHIFT, VK_BACK, IDC_MDEL},
																																									 {'\0'}}},
	{_T("backward-word"),				IDS_MOTION,		IDS_BACKWARD_WORD_D,				IDS_BACKWARD_WORD_T,				CCommands::BackwardWord,			{{META, 'B', IDC_MB},
																																									 {META+SHIFT, 'B', IDC_MB},
																																									 {'\0'}}},
	{_T("beginning-of-buffer"),			IDS_MOTION,		IDS_BEGINNING_OF_BUFFER_D,			IDS_BEGINNING_OF_BUFFER_T,			CCommands::BeginningOfBuffer,		{{META+SHIFT, 0xBC, IDC_MSCOMMA},
																																									 {'\0'}}},	// VK_OEM_COMMA
	{_T("beginning-of-line"),			IDS_MOTION,		IDS_BEGINNING_OF_LINE_D,			IDS_BEGINNING_OF_LINE_T,			CCommands::BeginningOfLine,			{{CONTROL, 'A', IDC_CA},
																																									 {CONTROL+SHIFT, 'A', IDC_CA},
																																									 {'\0'}}},
	{_T("call-last-kbd-macro"),			IDS_OTHER,		IDS_CALL_LAST_KBD_MACRO_D,			IDS_CALL_LAST_KBD_MACRO_T,			CCommands::CallLastKbdMacro,		{{CONTROLX, 'E', IDC_CXE},
																																									 {CONTROLX+SHIFT, 'E', IDC_CXE},
																																									 {'\0'}}},
	{_T("capitalize-word"),				IDS_OTHER,		IDS_CAPITALIZE_WORD_D,				IDS_CAPITALIZE_WORD_T,				CCommands::CapitalizeWord,			{{META, 'C', IDC_MC},
																																									 {META+SHIFT, 'C', IDC_MC},
																																									 {'\0'}}},
	{_T("delete-backward-char"),		IDS_KILL,		IDS_DELETE_BACKWARD_CHAR_D,			IDS_DELETE_BACKWARD_CHAR_T,			CCommands::DeleteBackwardChar,		{{CONTROL, 'H', IDC_CH},
																																									 {CONTROL+SHIFT, 'H', IDC_CH},
																																									 {'\0'}}},
	{_T("delete-char"),					IDS_KILL,		IDS_DELETE_CHAR_D,					IDS_DELETE_CHAR_T,					CCommands::DeleteChar,				{{CONTROL, 'D', IDC_CD},
																																									 {CONTROL+SHIFT, 'D', IDC_CD},
																																									 {'\0'}}},
	{_T("downcase-word"),				IDS_OTHER,		IDS_DOWNCASE_WORD_D,				IDS_DOWNCASE_WORD_T,				CCommands::DowncaseWord,			{{META, 'L', IDC_ML},
																																									 {META+SHIFT, 'L', IDC_ML},
																																									 {'\0'}}},
	{_T("end-kbd-macro"),				IDS_OTHER,		IDS_END_KBD_MACRO_D,				IDS_END_KBD_MACRO_T,				CCommands::EndKbdMacro,				{{CONTROLX+SHIFT, '0', IDC_CXS0},
																																									 {'\0'}}},
	{_T("end-of-buffer"),				IDS_MOTION,		IDS_END_OF_BUFFER_D,				IDS_END_OF_BUFFER_T,				CCommands::EndOfBuffer,				{{META+SHIFT, 0xBE, IDC_MSPERIOD},
																																									 {'\0'}}},	// VK_OEM_PERIOD
	{_T("end-of-line"),					IDS_MOTION,		IDS_END_OF_LINE_D,					IDS_END_OF_LINE_T,					CCommands::EndOfLine,				{{CONTROL, 'E', IDC_CE},
																																									 {CONTROL+SHIFT, 'E', IDC_CE},
																																									 {'\0'}}},
	{_T("execute-extended-command"),    IDS_OTHER,      IDS_EXECUTE_EXTENDED_COMMAND_D,     IDS_EXECUTE_EXTENDED_COMMAND_T,     CCommands::ExecuteExtendedCommand,  {{META, 'X', IDC_MX},
																																									 {META+SHIFT, 'X', IDC_MX},
																																									 {'\0'}}},
//	{_T("fill-paragraph"),				IDS_OTHER,		IDS_FILL_PARAGRAPH_D,				IDS_FILL_PARAGRAPH_T,				CCommands::FillParagraph,			{'\0'}},
	{_T("find-file"),					IDS_FILES,		IDS_FIND_FILE_D,					IDS_FIND_FILE_T,					CCommands::FindFile,				{{CONTROL+CONTROLX, 'F', IDC_CXCF},
																																									 {CONTROL+SHIFT+CONTROLX, 'F', IDC_CXCF},
																																									 {'\0'}}},
	{_T("forward-char"),				IDS_MOTION,		IDS_FORWARD_CHAR_D,					IDS_FORWARD_CHAR_T,					CCommands::ForwardChar,				{{CONTROL, 'F', IDC_CF},
																																									 {CONTROL+SHIFT, 'F', IDC_CF},
																																									 {'\0'}}},
	{_T("forward-word"),				IDS_MOTION,		IDS_FORWARD_WORD_D,					IDS_FORWARD_WORD_T,					CCommands::ForwardWord,				{{META, 'F', IDC_MF},
																																									 {META+SHIFT, 'F', IDC_MF},
																																									 {'\0'}}},
	{_T("iconify-or-deiconify-frame"),	IDS_WINDOW,		IDS_ICONIFY_OR_DEICONIFY_FRAME_D,	IDS_ICONIFY_OR_DEICONIFY_FRAME_T,	CCommands::IconifyOrDeiconifyFrame,	{{CONTROL, 'Z', IDC_CZ},
																																									 {CONTROL+SHIFT, 'Z', IDC_CZ},
																																									 {'\0'}}},
	{_T("isearch-backward"),			IDS_SEARCH,		IDS_ISEARCH_BACKWARD_D,				IDS_ISEARCH_BACKWARD_T,				CCommands::IsearchBackward,			{{CONTROL, 'R', IDC_CR},
																																									 {CONTROL+SHIFT, 'R', IDC_CR},
																																									 {'\0'}}},
	{_T("isearch-forward"),				IDS_SEARCH,		IDS_ISEARCH_FORWARD_D,				IDS_ISEARCH_FORWARD_T,				CCommands::IsearchForward,			{{CONTROL, 'S', IDC_CS},
																																									 {CONTROL+SHIFT, 'S', IDC_CS},
																																									 {'\0'}}},
	{_T("keyboard-quit"),				IDS_RECOVERY,	IDS_KEYBOARD_QUIT_D,				IDS_KEYBOARD_QUIT_T,				CCommands::KeyboardQuit,			{{CONTROL, 'G', IDC_CG},
																																									 {CONTROL+SHIFT, 'G', IDC_CG},
																																									 {'\0'}}},
	{_T("kill-buffer"),					IDS_FILES,		IDS_KILL_BUFFER_D,					IDS_KILL_BUFFER_T,					CCommands::KillBuffer,				{{CONTROLX, 'K', IDC_CXK},
																																									 {SHIFT+CONTROLX, 'K', IDC_CXK},
																																									 {'\0'}}},
	{_T("kill-line"),					IDS_KILL,		IDS_KILL_LINE_D,					IDS_KILL_LINE_T,					CCommands::KillLine,				{{CONTROL, 'K', IDC_CK},
																																									 {CONTROL+SHIFT, 'K', IDC_CK},
																																									 {'\0'}}},
	{_T("kill-region"),					IDS_KILL,		IDS_KILL_REGION_D,					IDS_KILL_REGION_T,					CCommands::KillRegion,				{{CONTROL, 'W', IDC_CW},
																																									 {CONTROL+SHIFT, 'W', IDC_CW},
																																									 {'\0'}}},
	{_T("kill-ring-save"),				IDS_KILL,		IDS_KILL_RING_SAVE_D,				IDS_KILL_RING_SAVE_T,				CCommands::KillRingSave,			{{META, 'W', IDC_MW},
																																									 {META+SHIFT, 'W', IDC_MW},
																																									 {'\0'}}},
	{_T("kill-word"),					IDS_KILL,		IDS_KILL_WORD_D,					IDS_KILL_WORD_T,					CCommands::KillWord,				{{META, 'D', IDC_MD},
																																									 {META+SHIFT, 'D', IDC_MD},
																																									 {'\0'}}},
	{_T("mark-page"),					IDS_OTHER,		IDS_MARK_PAGE_D,					IDS_MARK_PAGE_T,					CCommands::MarkPage,				{{CONTROL+CONTROLX, 'P', IDC_CXCP},
																																									 {CONTROL+SHIFT+CONTROLX, 'P', IDC_CXCP},
																																									 {'\0'}}},
	{_T("mark-whole-buffer"),			IDS_OTHER,		IDS_MARK_WHOLE_BUFFER_D,			IDS_MARK_WHOLE_BUFFER_T,			CCommands::MarkWholeBuffer,			{{CONTROLX, 'H', IDC_CXH},
																																									 {CONTROLX+SHIFT, 'H', IDC_CXH},
																																									 {'\0'}}},
	{_T("next-line"),					IDS_MOTION,		IDS_NEXT_LINE_D,					IDS_NEXT_LINE_T,					CCommands::NextLine,				{{CONTROL, 'N', IDC_CN},
																																									 {CONTROL+SHIFT, 'N', IDC_CN},
																																									 {'\0'}}},
	{_T("numeric argument 0"),			IDS_OTHER,		IDS_DIGIT_ARGUMENT0_D,				IDS_DIGIT_ARGUMENT0_T,				CCommands::NumericArgument0,		{{CONTROL, '0', IDC_CDIGIT},
																																									 {META, '0', IDC_CDIGIT},
																																									 {'\0'}}},
	{_T("numeric argument 1"),			IDS_OTHER,		IDS_DIGIT_ARGUMENT1_D,				IDS_DIGIT_ARGUMENT1_T,				CCommands::NumericArgument1,		{{CONTROL, '1', IDC_CDIGIT},
																																									 {META, '1', IDC_CDIGIT},
																																									 {'\0'}}},
	{_T("numeric argument 2"),			IDS_OTHER,		IDS_DIGIT_ARGUMENT2_D,				IDS_DIGIT_ARGUMENT2_T,				CCommands::NumericArgument2,		{{CONTROL, '2', IDC_CDIGIT},
																																									 {META, '2', IDC_CDIGIT},
																																									 {'\0'}}},
	{_T("numeric argument 3"),			IDS_OTHER,		IDS_DIGIT_ARGUMENT3_D,				IDS_DIGIT_ARGUMENT3_T,				CCommands::NumericArgument3,		{{CONTROL, '3', IDC_CDIGIT},
																																									 {META, '3', IDC_CDIGIT},
																																									 {'\0'}}},
	{_T("numeric argument 4"),			IDS_OTHER,		IDS_DIGIT_ARGUMENT4_D,				IDS_DIGIT_ARGUMENT4_T,				CCommands::NumericArgument4,		{{CONTROL, '4', IDC_CDIGIT},
																																									 {META, '4', IDC_CDIGIT},
																																									 {'\0'}}},
	{_T("numeric argument 5"),			IDS_OTHER,		IDS_DIGIT_ARGUMENT5_D,				IDS_DIGIT_ARGUMENT5_T,				CCommands::NumericArgument5,		{{CONTROL, '5', IDC_CDIGIT},
																																									 {META, '5', IDC_CDIGIT},
																																									 {'\0'}}},
	{_T("numeric argument 6"),			IDS_OTHER,		IDS_DIGIT_ARGUMENT6_D,				IDS_DIGIT_ARGUMENT6_T,				CCommands::NumericArgument6,		{{CONTROL, '6', IDC_CDIGIT},
																																									 {META, '6', IDC_CDIGIT},
																																									 {'\0'}}},
	{_T("numeric argument 7"),			IDS_OTHER,		IDS_DIGIT_ARGUMENT7_D,				IDS_DIGIT_ARGUMENT7_T,				CCommands::NumericArgument7,		{{CONTROL, '7', IDC_CDIGIT},
																																									 {META, '7', IDC_CDIGIT},
																																									 {'\0'}}},
	{_T("numeric argument 8"),			IDS_OTHER,		IDS_DIGIT_ARGUMENT8_D,				IDS_DIGIT_ARGUMENT8_T,				CCommands::NumericArgument8,		{{CONTROL, '8', IDC_CDIGIT},
																																									 {META, '8', IDC_CDIGIT},
																																									 {'\0'}}},
	{_T("numeric argument 9"),			IDS_OTHER,		IDS_DIGIT_ARGUMENT9_D,				IDS_DIGIT_ARGUMENT9_T,				CCommands::NumericArgument9,		{{CONTROL, '9', IDC_CDIGIT},
																																									 {META, '9', IDC_CDIGIT},
																																									 {'\0'}}},
	{_T("numeric argument -"),			IDS_OTHER,		IDS_NEGATIVE_ARGUMENT_D,			IDS_NEGATIVE_ARGUMENT_T,			CCommands::NumericArgumentMinus,	{{CONTROL, 0xBD, IDC_CDIGIT},
																																									 {META, 0xBD, IDC_CDIGIT},
																																									 {'\0'}}},
	{_T("open-line"),					IDS_OTHER,		IDS_OPEN_LINE_D,					IDS_OPEN_LINE_T,					CCommands::OpenLine,				{{CONTROL, 'O', IDC_CO},
																																									 {CONTROL+SHIFT, 'O', IDC_CO},
																																									 {'\0'}}},
	{_T("previous-line"),				IDS_MOTION,		IDS_PREVIOUS_LINE_D,				IDS_PREVIOUS_LINE_T,				CCommands::PreviousLine,			{{CONTROL, 'P', IDC_CP},
																																									 {CONTROL+SHIFT, 'P', IDC_CP},
																																									 {'\0'}}},
	{_T("query-replace"),				IDS_SEARCH,		IDS_QUERY_REPLACE_D,				IDS_QUERY_REPLACE_T,				CCommands::QueryReplace,			{{META+SHIFT, '5', IDC_MS5},
																																									 {'\0'}}},
	{_T("query-replace-regexp"),		IDS_SEARCH,		IDS_QUERY_REPLACE_REGEXP_D,			IDS_QUERY_REPLACE_REGEXP_T,			CCommands::QueryReplaceRegexp,		{{CONTROL+META+SHIFT, '5', IDC_CMS5},
																																									 {'\0'}}},
	{_T("recenter"),					IDS_MOTION,		IDS_RECENTER_D,						IDS_RECENTER_T,						CCommands::Recenter,				{{CONTROL, 'L', IDC_CL},
																																									 {CONTROL+SHIFT, 'L', IDC_CL},
																																									 {'\0'}}},
	{_T("save-buffer"),					IDS_FILES,		IDS_SAVE_BUFFER_D,					IDS_SAVE_BUFFER_T,					CCommands::SaveBuffer,				{{CONTROL+CONTROLX, 'S', IDC_CXCS},
																																									 {CONTROL+SHIFT+CONTROLX, 'S', IDC_CXCS},
																																									 {'\0'}}},
	{_T("save-buffers-kill-emacs"),		IDS_WINDOW,		IDS_SAVE_BUFFERS_KILL_EMACS_D,		IDS_SAVE_BUFFERS_KILL_EMACS_T,		CCommands::SaveBuffersKillEmacs,	{{CONTROL+CONTROLX, 'C', IDC_CXCC},
																																									 {CONTROL+SHIFT+CONTROLX, 'C', IDC_CXCC},
																																									 {'\0'}}},
	{_T("scroll-down"),					IDS_MOTION,		IDS_SCROLL_DOWN_D,					IDS_SCROLL_DOWN_T,					CCommands::ScrollDown,				{{META, 'V', IDC_MV},
																																									 {META+SHIFT, 'V', IDC_MV},
																																									 {'\0'}}},
	{_T("scroll-up"),					IDS_MOTION,		IDS_SCROLL_UP_D,					IDS_SCROLL_UP_T,					CCommands::ScrollUp,				{{CONTROL, 'V', IDC_CV},
																																									 {CONTROL+SHIFT, 'V', IDC_CV},
																																									 {'\0'}}},
	{_T("set-mark-command"),			IDS_OTHER,		IDS_SET_MARK_COMMAND_D,				IDS_SET_MARK_COMMAND_T,				CCommands::SetMarkCommand,			{{CONTROL, VK_SPACE, IDC_CSPACE},
																																									 {CONTROL+SHIFT, VK_SPACE, IDC_CSPACE},
																																									 {'\0'}}},
	{_T("shell-command"),				IDS_OTHER,		IDS_SHELL_COMMAND_D,				IDS_SHELL_COMMAND_T,				CCommands::ShellCommand,			{{META+SHIFT, '1', IDC_MS1},
																																									 {'\0'}}},
	{_T("start-kbd-macro"),				IDS_OTHER,		IDS_START_KBD_MACRO_D,				IDS_START_KBD_MACRO_T,				CCommands::StartKbdMacro,			{{CONTROLX+SHIFT, '9', IDC_CXS9},
																																									 {'\0'}}},
	{_T("switch-to-buffer"),			IDS_OTHER,		IDS_SWITCH_TO_BUFFER_D,				IDS_SWITCH_TO_BUFFER_T,				CCommands::SwitchToBuffer,			{{CONTROLX, 'B', IDC_CXB},
																																									 {CONTROLX+SHIFT, 'B', IDC_CXB},
																																									 {'\0'}}},
	{_T("tab-to-tab-stop"),				IDS_OTHER,		IDS_TAB_D,							IDS_TAB_T,							CCommands::TabToTabStop,			{{CONTROL, 'I', IDC_CI},
																																									 {CONTROL+SHIFT, 'I', IDC_CI},
																																									 {META, 'I', IDC_MI},
																																									 {META+SHIFT, 'I', IDC_MI},
																																									 {'\0'}}},
	{_T("toggle-input-method"),			IDS_IME,		IDS_TOGGLE_WIN32IME_MODE_D,			IDS_TOGGLE_WIN32IME_MODE_T,			CCommands::ToggleInputMethod,		{{CONTROL+CONTROLX, 'J', IDC_CXCJ},
																																									 {CONTROL+SHIFT+CONTROLX, 'J', IDC_CXCJ},
																																									 {CONTROL, 'O', IDC_CO2},
																																									 {CONTROL+SHIFT, 'O', IDC_CO2},
																																									 {CONTROL, 0xDC, IDC_CBACKSLASH},
																																									 {CONTROL, 0xE2, IDC_CBACKSLASH_106},
																																									 {'\0'}}},
	{_T("transpose-chars"),				IDS_KILL,		IDS_TRANSPOSE_CHARS_D,				IDS_TRANSPOSE_CHARS_T,				CCommands::TransposeChars,			{{CONTROL, 'T', IDC_CT},
																																									 {CONTROL+SHIFT, 'T', IDC_CT},
																																									 {'\0'}}},
	{_T("transpose-lines"),				IDS_KILL,		IDS_TRANSPOSE_LINES_D,				IDS_TRANSPOSE_LINES_T,				CCommands::TransposeLines,			{{CONTROL+CONTROLX, 'T', IDC_CXCT},
																																									 {CONTROL+SHIFT+CONTROLX, 'T', IDC_CXCT},
																																									 {'\0'}}},
	{_T("transpose-words"),				IDS_KILL,		IDS_TRANSPOSE_WORDS_D,				IDS_TRANSPOSE_WORDS_T,				CCommands::TransposeWords,			{{META, 'T', IDC_MT},
																																									 {META+SHIFT, 'T', IDC_MT},
																																									 {'\0'}}},
	{_T("undo"),						IDS_RECOVERY,	IDS_UNDO_D,							IDS_UNDO_T,							CCommands::Undo,					{{CONTROLX, 'U', IDC_CXU},
																																									 {CONTROL+SHIFT, 0xBD, IDC_C_},
																																									 {CONTROL, 0xBF, IDC_CSLASH},
																																									 {'\0'}}},
	{_T("universal-argument"),			IDS_OTHER,		IDS_UNIVERSAL_ARGUMENT_D,			IDS_UNIVERSAL_ARGUMENT_T,			CCommands::UniversalArgument,		{{CONTROL, 'U', IDC_CU},
																																									 {CONTROL+SHIFT, 'U', IDC_CU},
																																									 {'\0'}}},
	{_T("upcase-word"),					IDS_OTHER,		IDS_UPCASE_WORD_D,					IDS_UPCASE_WORD_T,					CCommands::UpcaseWord,				{{META, 'U', IDC_MU},
																																									 {META+SHIFT, 'U', IDC_MU},
																																									 {'\0'}}},
	{_T("write-file"),					IDS_FILES,		IDS_WRITE_FILE_D,					IDS_WRITE_FILE_T,					CCommands::WriteFile,				{{CONTROL+CONTROLX, 'W', IDC_CXCW},
																																									 {CONTROL+SHIFT+CONTROLX, 'W', IDC_CXCW},
																																									 {'\0'}}},
	{_T("yank"),						IDS_KILL,		IDS_YANK_D,							IDS_YANK_T,							CCommands::Yank,					{{CONTROL, 'Y', IDC_CY},
																																									 {CONTROL+SHIFT, 'Y', IDC_CY},
																																									 {'\0'}}},
	{_T("yank-pop"),					IDS_KILL,		IDS_YANK_POP_D,						IDS_YANK_POP_T,						CCommands::YankPop,					{{META, 'Y', IDC_MY},
																																									 {META+SHIFT, 'Y', IDC_MY},
																																									 {'\0'}}},
};

#endif // !defined(AFX_COMMANDS_H__3AD9D537_AB40_40BB_9697_35F1BB0CA431__INCLUDED_)
