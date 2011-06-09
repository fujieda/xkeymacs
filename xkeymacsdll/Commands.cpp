// Commands.cpp: implementation of the CCommands class
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Commands.h"
#include "xkeymacsdll.h"
#include "Utils.h"
#include <math.h>
#include <Imm.h>

AFX_EXT_DATA const Command Commands[] = {
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

#pragma data_seg(".xkmcs")
DWORD CCommands::m_nNumericArgument = 1;
BOOL CCommands::m_bSetMark = FALSE;
BOOL CCommands::m_bC_ = FALSE;
BOOL CCommands::m_bC_x = FALSE;
BOOL CCommands::m_bM_ = FALSE;
BOOL CCommands::m_bM_x = FALSE;
BOOL CCommands::m_bDefaultNumericArgument = TRUE;
BOOL CCommands::m_bNegativeNumericArgument = FALSE;
BOOL CCommands::m_bC_u = FALSE;
SEARCH_DIRECTION CCommands::m_SearchDirection = NA;
int (*CCommands::m_LastKillCommand)() = NULL;
int (*CCommands::m_LastCommand)() = NULL;
BOOL CCommands::m_bFirstFindDialog = FALSE;
BOOL CCommands::m_bTemporarilyDisableXKeymacs = FALSE;
CArray<CClipboardSnap *, CClipboardSnap *> CCommands::m_oClipboardData;
OriginalWindowPosition CCommands::m_OriginalWindowPosition[MAX_WINDOW] = {'\0'};
BOOL CCommands::m_bIsSu = FALSE;
#pragma data_seg()


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommands::CCommands()
{

}

CCommands::~CCommands()
{

}

void CCommands::DepressKey(BYTE bVk)
{
	CXkeymacsDll::DepressKey(bVk);
}

void CCommands::ReleaseKey(BYTE bVk)
{
	CXkeymacsDll::ReleaseKey(bVk);
}

void CCommands::Kdu(BYTE bVk1, BYTE bVk2, BYTE bVk3, BYTE bVk4)
{
	BOOL bIsCtrlDown = CXkeymacsDll::IsDown(VK_CONTROL, FALSE);
	BOOL bIsAltDown = CXkeymacsDll::IsDown(VK_MENU, FALSE);
	BOOL bIsShiftDown = CXkeymacsDll::IsDown(VK_SHIFT, FALSE);

	if (CXkeymacsDll::Get326Compatible() || CUtils::IsAtok()) {
		bIsShiftDown = FALSE;
	}

	if (bIsCtrlDown) {
		CUtils::UpdateKeyboardState(VK_CONTROL, 0);
		ReleaseKey(VK_CONTROL);
	}
	if (bIsAltDown) {
		if (CUtils::IsVisualStudio2010())
			CXkeymacsDll::SetHookAltRelease(); // Ignore Alt release
		ReleaseKey(VK_MENU);
	}
	if (bIsShiftDown) {
		ReleaseKey(VK_SHIFT);
	}

	if (bVk1) {
		CXkeymacsDll::Kdu(bVk1, m_nNumericArgument);
	}
	if (bVk2) {
		CXkeymacsDll::Kdu(bVk2, m_nNumericArgument);
	}
	if (bVk3) {
		CXkeymacsDll::Kdu(bVk3, m_nNumericArgument);
	}
	if (bVk4) {
		CXkeymacsDll::Kdu(bVk4, m_nNumericArgument);
	}

	if (bIsShiftDown) {
		DepressKey(VK_SHIFT);
	}
	if (bIsAltDown) {
		DepressKey(VK_MENU);
	}
	if (bIsCtrlDown) {
		DepressKey(VK_CONTROL);
		CUtils::UpdateKeyboardState(VK_CONTROL, 1);
	}
}

void CCommands::SdKduSu(BYTE bVk1, BYTE bVk2, BYTE bVk3, BYTE bVk4)
{
	BOOL bIsCtrlDown = CXkeymacsDll::IsDown(VK_CONTROL, FALSE);
	BOOL bIsAltDown = CXkeymacsDll::IsDown(VK_MENU, FALSE);
	BOOL bIsShiftDown = CXkeymacsDll::IsDown(VK_SHIFT, FALSE);

	if (bIsCtrlDown) {
		ReleaseKey(VK_CONTROL);
	}
	if (bIsAltDown) {
		ReleaseKey(VK_MENU);
	}
	if (!bIsShiftDown) {
		DepressKey(VK_SHIFT);
	}

	CXkeymacsDll::Kdu(bVk1, m_nNumericArgument);
	if (bVk2) {
		CXkeymacsDll::Kdu(bVk2, m_nNumericArgument);
	}
	if (bVk3) {
		CXkeymacsDll::Kdu(bVk3, m_nNumericArgument);
	}
	if (bVk4) {
		CXkeymacsDll::Kdu(bVk4, m_nNumericArgument);
	}

	if (!bIsShiftDown) {
		if (CUtils::IsShuriken()) {
			m_bIsSu = TRUE;
		} else {
			ReleaseKey(VK_SHIFT);
		}
	}
	if (bIsAltDown) {
		DepressKey(VK_MENU);
	}
	if (bIsCtrlDown) {
		DepressKey(VK_CONTROL);
	}
}

void CCommands::Su()
{
	if (m_bIsSu) {
		ReleaseKey(VK_SHIFT);
		m_bIsSu = FALSE;
	}
}

void CCommands::CdKduCu(BYTE bVk1, BYTE bVk2)
{
	BOOL bIsCtrlDown = CXkeymacsDll::IsDown(VK_CONTROL, FALSE);
	BOOL bIsAltDown = CXkeymacsDll::IsDown(VK_MENU, FALSE);
	BOOL bIsShiftDown = CXkeymacsDll::IsDown(VK_SHIFT, FALSE);
//	if (!bVk1) {
//		CUtils::Log("%d, %d, %d", CXkeymacsDll::IsDown(VK_MENU, FALSE), CXkeymacsDll::IsDown(VK_LMENU, FALSE), CXkeymacsDll::IsDown(VK_RMENU, FALSE));
//	}
	if (!bIsCtrlDown) {
		DepressKey(VK_CONTROL);
		CUtils::UpdateKeyboardState(VK_CONTROL, 1);
	}
	if (bIsAltDown) {
		ReleaseKey(VK_MENU);
	}
	if (bIsShiftDown && !m_bSetMark) {
		ReleaseKey(VK_SHIFT);
	}

	if (bVk1) {
		CXkeymacsDll::Kdu(bVk1, m_nNumericArgument);
	}
	if (bVk2) {
		CXkeymacsDll::Kdu(bVk2, m_nNumericArgument);
	}

	if (bIsShiftDown && !m_bSetMark) {
		if (!CUtils::IsXPorLater()) {	// FIXME	Windows XP said VK_SHIFT was pressed physically after a user press VK_RSHIFT, so...
			DepressKey(VK_SHIFT);
		}
	}
	if (bIsAltDown) {
		DepressKey(VK_MENU);
	}
	if (!bIsCtrlDown) {
		CUtils::UpdateKeyboardState(VK_CONTROL, 0);
		ReleaseKey(VK_CONTROL);
	}
}

void CCommands::CdSdKduSuCu(BYTE bVk)
{
	BOOL bIsCtrlDown = CXkeymacsDll::IsDown(VK_CONTROL, FALSE);
	BOOL bIsAltDown = CXkeymacsDll::IsDown(VK_MENU, FALSE);
	BOOL bIsShiftDown = CXkeymacsDll::IsDown(VK_SHIFT, FALSE);

	if (!bIsCtrlDown) {
		DepressKey(VK_CONTROL);
		CUtils::UpdateKeyboardState(VK_CONTROL, 1);
	}
	if (bIsAltDown) {
		ReleaseKey(VK_MENU);
	}
	if (!bIsShiftDown) {
		DepressKey(VK_SHIFT);
	}

	CXkeymacsDll::Kdu(bVk, m_nNumericArgument);

	if (!bIsShiftDown) {
		ReleaseKey(VK_SHIFT);
	}
	if (bIsAltDown) {
		DepressKey(VK_MENU);
	}
	if (!bIsCtrlDown) {
		CUtils::UpdateKeyboardState(VK_CONTROL, 0);
		ReleaseKey(VK_CONTROL);
	}
}

void CCommands::AdKduAu(BYTE bVk1, BYTE bVk2, BYTE bVk3)
{
	BOOL bIsCtrlDown = CXkeymacsDll::IsDown(VK_CONTROL, FALSE);
	BOOL bIsAltDown = CXkeymacsDll::IsDown(VK_MENU, FALSE);
	BOOL bIsShiftDown = CXkeymacsDll::IsDown(VK_SHIFT, FALSE);

	if (bIsCtrlDown) {
		ReleaseKey(VK_CONTROL);
	}

	if (!bIsAltDown) {
		DepressKey(VK_MENU);
	}

	if (bIsShiftDown) {
		ReleaseKey(VK_SHIFT);
	}

	CXkeymacsDll::Kdu(bVk1, m_nNumericArgument);
	if (bVk2) {
		CXkeymacsDll::Kdu(bVk2, m_nNumericArgument);
	}
	if (bVk3) {
		CXkeymacsDll::Kdu(bVk3, m_nNumericArgument);
	}

	if (bIsShiftDown) {
		if (!CUtils::IsXPorLater()) {	// FIXME	Windows XP said VK_SHIFT was pressed physically after a user press VK_RSHIFT, so...
			DepressKey(VK_SHIFT);
		}
	}

	if (!bIsAltDown) {
		ReleaseKey(VK_MENU);
	}

	if (bIsCtrlDown) {
		DepressKey(VK_CONTROL);
	}
}

void CCommands::AdSdKduSuAu(BYTE bVk1)
{
	BOOL bIsCtrlDown = CXkeymacsDll::IsDown(VK_CONTROL, FALSE);
	BOOL bIsAltDown = CXkeymacsDll::IsDown(VK_MENU, FALSE);
	BOOL bIsShiftDown = CXkeymacsDll::IsDown(VK_SHIFT, FALSE);

	if (bIsCtrlDown) {
		ReleaseKey(VK_CONTROL);
	}

	if (!bIsAltDown) {
		DepressKey(VK_MENU);
	}

	if (!bIsShiftDown) {
		DepressKey(VK_SHIFT);
	}

	CXkeymacsDll::Kdu(bVk1, m_nNumericArgument);

	if (!bIsShiftDown) {
		ReleaseKey(VK_SHIFT);
	}

	if (!bIsAltDown) {
		ReleaseKey(VK_MENU);
	}

	if (bIsCtrlDown) {
		DepressKey(VK_CONTROL);
	}
}

// C-a: Home
int CCommands::BeginningOfLine()
{
	int nNumericArgument = 1;
	if (!m_bDefaultNumericArgument || m_bNegativeNumericArgument) {
		nNumericArgument = m_nNumericArgument;
		if (m_bNegativeNumericArgument) {
			if (nNumericArgument) {
				nNumericArgument *= -1;
			} else {
				nNumericArgument = -1;
			}
		}
	}
	ClearNumericArgument();

	if (nNumericArgument <= 0) {
		nNumericArgument = -1 * nNumericArgument + 1;
		while (nNumericArgument--) {
			ReleaseKey(VK_CONTROL);	// why?
			while (MoveCaret(VK_UP) != GOTO_HOOK);
		}
		ReleaseKey(VK_CONTROL);	// why?
		while (MoveCaret(VK_HOME) != GOTO_HOOK);
		DepressKey(VK_CONTROL);	// why?
	} else if (nNumericArgument == 1) {
		return Reset(MoveCaret(VK_HOME));
	} else if (2 <= nNumericArgument) {
		nNumericArgument -= 2;
		while (nNumericArgument--) {
			ReleaseKey(VK_CONTROL);	// why?
			while (MoveCaret(VK_DOWN) != GOTO_HOOK);
		}
		ReleaseKey(VK_CONTROL);	// why?
		while (MoveCaret(VK_END) != GOTO_HOOK);
		while (MoveCaret(VK_RIGHT) != GOTO_HOOK);
		DepressKey(VK_CONTROL);	// why?
	} else {
		ASSERT(0);
	}
	return Reset(GOTO_HOOK);
}

// C-b: Left
int CCommands::BackwardChar()
{
	if (m_bNegativeNumericArgument) {
		m_bNegativeNumericArgument = FALSE;
		return ForwardChar();
	}

	return Reset(MoveCaret(VK_LEFT));
}

// M-b: backward-word
int CCommands::BackwardWord()
{
	if (m_bNegativeNumericArgument) {
		m_bNegativeNumericArgument = FALSE;
		return ForwardWord();
	}

	return Reset(MoveCaret(VK_LEFT, TRUE));
}

// C-e: End
int CCommands::EndOfLine()
{
	int nNumericArgument = 1;
	if (!m_bDefaultNumericArgument || m_bNegativeNumericArgument) {
		nNumericArgument = m_nNumericArgument;
		if (m_bNegativeNumericArgument) {
			if (nNumericArgument) {
				nNumericArgument *= -1;
			} else {
				nNumericArgument = -1;
			}
		}
	}
	ClearNumericArgument();
	if (nNumericArgument <= 0) {
		nNumericArgument *= -1;
		while (nNumericArgument--) {
			ReleaseKey(VK_CONTROL);	// why?
			while (MoveCaret(VK_UP) != GOTO_HOOK);
		}
		ReleaseKey(VK_CONTROL);	// why?
		while (MoveCaret(VK_HOME) != GOTO_HOOK);
		while (MoveCaret(VK_LEFT) != GOTO_HOOK);
		DepressKey(VK_CONTROL);	// why?
	} else if (nNumericArgument == 1) {
		return (MoveCaret(VK_END));
	} else if (2 <= nNumericArgument) {
		--nNumericArgument;
		while (nNumericArgument--) {
			ReleaseKey(VK_CONTROL);	// why?
			while (MoveCaret(VK_DOWN) != GOTO_HOOK);
		}
		ReleaseKey(VK_CONTROL);	// why?
		while (MoveCaret(VK_END) != GOTO_HOOK);
		DepressKey(VK_CONTROL);	// why?
	} else {
		ASSERT(0);
	}
	return Reset(GOTO_HOOK);
}

// C-f: Right
int CCommands::ForwardChar()
{
//	TCHAR szWindowText[WINDOW_TEXT_LENGTH] = {'\0'};
//	GetWindowText(GetForegroundWindow(), szWindowText, sizeof(szWindowText));
//	CUtils::Log("C-f: %s", szWindowText);

	if (m_bNegativeNumericArgument) {
		m_bNegativeNumericArgument = FALSE;
		return BackwardChar();
	}

	return Reset(MoveCaret(VK_RIGHT));
}

// M-f: forward-word
int CCommands::ForwardWord()
{
	if (m_bNegativeNumericArgument) {
		m_bNegativeNumericArgument = FALSE;
		return BackwardWord();
	}

	return Reset(MoveCaret(VK_RIGHT, TRUE));
}

// C-n: Down
int CCommands::NextLine()
{
//	CUtils::Log(_T("C-n"));

	if (m_bNegativeNumericArgument) {
		m_bNegativeNumericArgument = FALSE;
		return PreviousLine();
	}

	return Reset(MoveCaret(VK_DOWN));
}

// C-o: open-line
int CCommands::OpenLine()
{
	static const int DEFAULT_NUMERIC_ARGUMENT = -1;
	static int nNumericArgument = DEFAULT_NUMERIC_ARGUMENT;

	if (nNumericArgument == DEFAULT_NUMERIC_ARGUMENT) {
		nNumericArgument = m_nNumericArgument;
		ClearNumericArgument();
	}

	if (CUtils::IsHidemaru()
	 || CUtils::IsTuruKameMail()) {
		static int nStep = 0;

		switch (nStep) {
		case 0:
			CdKduCu('M');
			nStep = 1;
			return Reset(GOTO_RECURSIVE);
		case 1:
			nStep = 0;
			Kdu(VK_UP, VK_END);
			if (--nNumericArgument) {
				return Reset(GOTO_RECURSIVE);
			}
			break;
		}
	} else {
		while (nNumericArgument--) {
			Kdu(VK_RETURN, VK_UP, VK_END);
		}
	}

	nNumericArgument = DEFAULT_NUMERIC_ARGUMENT;
	return Reset(GOTO_HOOK);
}

// C-p: Up
int CCommands::PreviousLine()
{
	if (m_bNegativeNumericArgument) {
		m_bNegativeNumericArgument = FALSE;
		return NextLine();
	}

	return Reset(MoveCaret(VK_UP));
}

// M-%
int CCommands::QueryReplace()
{
	ClearNumericArgument();
	if (CUtils::IsBecky()) {
		CdKduCu('Q', 'A');
	} else if (CUtils::IsEclipse()
			|| CUtils::IsInternetExplorer()
			|| CUtils::IsLotusNotes()
			|| CUtils::IsSleipnir()) {
		CdKduCu('F');
	} else if (CUtils::IsAutla()
			|| CUtils::IsBorlandCppBuilder()
			|| CUtils::IseMemoPad()
			|| CUtils::IsNami2000()
			|| CUtils::IsStoryEditor()) {
		CdKduCu('R');
	} else if (CUtils::IsEdLeaf()) {
		AdKduAu('E', 'E');
	} else if (CUtils::IsPHPEditor()) {
		AdKduAu('S', 'F');
	} else {
		CdKduCu('H');
	}
	return Reset(GOTO_HOOK);
}

// C-M-%
int CCommands::QueryReplaceRegexp()
{
	return QueryReplace();
}

// M-<:
int CCommands::BeginningOfBuffer()
{
	ClearNumericArgument();
	return Reset(MoveCaret(VK_HOME, TRUE));
}

// M->:
int CCommands::EndOfBuffer()
{
	ClearNumericArgument();
	return Reset(MoveCaret(VK_END, TRUE));
}

// C-h: Back space
int CCommands::DeleteBackwardChar()
{
	if (m_bNegativeNumericArgument) {
		m_bNegativeNumericArgument = FALSE;
		return DeleteChar();
	}

	Kdu(VK_BACK);
	m_bSetMark = FALSE;
	return Reset(GOTO_HOOK);
}

// C-d: Delete
int CCommands::DeleteChar()
{
	if (m_bNegativeNumericArgument) {
		m_bNegativeNumericArgument = FALSE;
		return DeleteBackwardChar();
	}

	m_bSetMark = FALSE;
	Kdu(VK_DELETE);
	return Reset(GOTO_HOOK);
}

int CCommands::KillLineForAllFormat()
{
	return KillLine(TRUE, KillLineForAllFormat);
}

// C-k: Shift-End, Shift-Delete, if the cursor is not at EOL (is End Of Line)
//		Shift-Right, Cut, if the cursor is at EOL
int CCommands::KillLine()
{
	return KillLine(FALSE);
}

int CCommands::KillLine(BOOL bAllFormat, int (*pCommand)())
{
	static BOOL bKeepAllFormat = FALSE;

	// Do not try to do these command at once.
	// Clipboard has old data till you go out this function.
	static int nStep = 0;
	static int nPrevStep = 0;
	static int nRepeat = 0;
	static CString szClipboardText;
	static BOOL bContinuous = FALSE;
	static BOOL bTopOfFile = FALSE;

	if (nStep == nPrevStep) {
		++nRepeat;
	} else {
		nRepeat = 0;
		nPrevStep = nStep;
	}

	switch (nStep) {
	case 0:		// Clear Selection and get next character to confirm the cursor is at the EOF or not.
//		CUtils::Log(_T("C-k: 0"));
		bTopOfFile = FALSE;

		if (!m_bDefaultNumericArgument || m_bNegativeNumericArgument) {
			if (m_bNegativeNumericArgument || m_nNumericArgument == 0) {
				// Shift+Up x -n Shift+Home Ctrl+X
				SdKduSu(VK_UP);
				ClearNumericArgument();
				SdKduSu(VK_HOME);
			} else {
				// Shift+Down x (n - 1) Shift+End Shift+Right Ctrl+X
				--m_nNumericArgument;
				SdKduSu(VK_DOWN);
				ClearNumericArgument();
				SdKduSu(VK_END, VK_RIGHT);
			}
			CdKduCu('X');
			nStep = 6;
			return Reset(GOTO_RECURSIVE);
		}

		if (m_LastCommand == pCommand) {
			bContinuous = TRUE;
		} else {
			bContinuous = FALSE;
			if (m_bSetMark) {
				m_bSetMark = FALSE;
				DeactivateMark();
			}
		}

		if (CUtils::IsDirector()
		 || CUtils::IsEudora()) {
			nStep = 1;
			return Reset(GOTO_RECURSIVE);
		}

		if (CUtils::IsLotus123()) {
			// C-k: Shift-End, Control-x
			SdKduSu(VK_END);
			CdKduCu('X');
			return Reset(GOTO_HOOK);
		}

		CaptureClipboardData(0, CUtils::IsLotusNotes());

		if (!CopyNextCharacter()) {
//			CUtils::Log(_T("Error: CopyNextCharacter"));
			return Reset(GOTO_DO_NOTHING);
		}

		nStep = 4;
		return Reset(GOTO_RECURSIVE);
	case 1:		// Get back character to confirm the cursor is at the TOF or not only for Macromedia Director.
//		CUtils::Log(_T("C-k: 1"));
		nStep = 0;
		CaptureClipboardData();

		if (!CopyBackCharacter()) {
			return Reset(GOTO_DO_NOTHING);
		}

		nStep = 2;
		return Reset(GOTO_RECURSIVE);
	case 2:		// Confirm the cursor is at the TOF or not only for Macromedia Director.
		{
//			CUtils::Log(_T("C-k: 2"));
			nStep = 0;
			Su();

			if (CUtils::IsTOF()) {
				bTopOfFile = TRUE;
			}

			// CopyBackCharacter move cursor right
			Kdu(VK_LEFT);
			RestoreClipboardData();

			nStep = 3;
			return Reset(GOTO_RECURSIVE);
		}
	case 3:		// Get next character to confirm the cursor is at the EOF or not.
//		CUtils::Log(_T("C-k: 3"));
		nStep = 0;

		if (CUtils::IsLotus123()) {
			// C-k: Shift-End, Control-x
			SdKduSu(VK_END);
			CdKduCu('X');
			return Reset(GOTO_HOOK);
		}

		CaptureClipboardData();

		if (!CopyNextCharacter()) {
//			CUtils::Log(_T("Error: CopyNextCharacter"));
			return Reset(GOTO_DO_NOTHING);
		}

		nStep = 4;
		return Reset(GOTO_RECURSIVE);
	case 4:		// Cut line
//		CUtils::Log(_T("C-k: 4"));
		nStep = 0;
		Su();

		if ((CUtils::IsDirector()
		  || CUtils::IsEudora())
		 && !bTopOfFile) {
			// CopyNextCharacter move cursor left
			Kdu(VK_RIGHT);
		}

		CUtils::GetClipboardText(&szClipboardText);
//		CUtils::Log(_T("%x, %s"), szClipboardText.GetAt(0), szClipboardText);

		if (szClipboardText.IsEmpty()) {	// EOF
//			CUtils::Log(_T("C-k: EOF"));
			if (CUtils::IsLotusWordPro()
			 || CUtils::IsMicrosoftFrontPage()
			 || CUtils::IsMicrosoftWord()) {
				SdKduSu(VK_RIGHT);
				Cut_();
				nStep = 5;
				return Reset(GOTO_RECURSIVE);
			} else if (CUtils::IsLotusNotes()) {
				Kdu(VK_END);
				ReleaseKey(VK_CONTROL);	// Why is it needed?
				SdKduSu(VK_DOWN, VK_HOME);
				Copy_();
				DepressKey(VK_CONTROL);	// Why is it needed?
				nStep = 5;
				return Reset(GOTO_RECURSIVE);
			} else {
				// default
				Kdu(VK_RIGHT);
				RestoreClipboardData();
//				CUtils::Log(_T("C-k: 4-1"));
				return Reset(GOTO_HOOK);
			}
		}

		if (szClipboardText.GetAt(0) != VK_RETURN) {
//			CUtils::Log(_T("C-k: szClipboardText.GetAt(0) != VK_RETURN"));
			if (CUtils::IsDirector()) {
				ReleaseKey(VK_CONTROL);
				SdKduSu(VK_END);
				Copy_();
				DepressKey(VK_CONTROL);
			} else if (CUtils::IseMemoPad()
					|| CUtils::IsHidemaru()	// Hidemaru clears mark after copy.
					|| CUtils::IsOpenOffice()
					|| CUtils::IsTuruKameMail()) {
				SdKduSu(VK_END);
				Cut_();
			} else if (CUtils::IsLotusWordPro()
					|| CUtils::IsMicrosoftWord()) {
				SdKduSu(VK_END, VK_LEFT);
				Cut_();
			} else {
				SdKduSu(VK_END);
				Copy_();
			}
		} else {	// EOL
//			CUtils::Log(_T("C-k: EOL"));
			if (CUtils::IsDirector()) {
				ReleaseKey(VK_CONTROL);
				SdKduSu(VK_RIGHT);
				Copy_();
				DepressKey(VK_CONTROL);
			} else if (CUtils::IsEclipse()
					|| CUtils::IsStoryEditor()) {
				SdKduSu(VK_RIGHT);
				Copy_();
			} else if (CUtils::IseMemoPad()
					|| CUtils::IsHidemaru()	// Hidemaru clears mark after copy.
					|| CUtils::IsOpenOffice()
					|| CUtils::IsTuruKameMail()) {
				SdKduSu(VK_RIGHT);
				Cut_();
			} else {
				SdKduSu(VK_RIGHT);
				Copy_();
			}
		}

		nStep = 5;
		return Reset(GOTO_RECURSIVE);
	case 5:		// input return if XKeymacs not only line but also line feed code and set the kill-line data on the clipboard.
//		CUtils::Log(_T("C-k: 5"));
		nStep = 0;

		if (1000 < nRepeat) {
//			CUtils::Log(_T("C-k: 5-1"));
			nStep = 6;
			return Reset(GOTO_RECURSIVE);
		}

//		CUtils::Log(_T("C-k: 5-1-1"));
		Su();
//		CUtils::Log(_T("C-k: 5-1-2"));
		CaptureClipboardData(1, CUtils::IsLotusNotes());
//		CUtils::Log(_T("C-k: 5-1-3"));

//		if (m_oClipboardData.GetSize() < 1 || m_oClipboardData[0] == NULL) {
////			CUtils::Log(_T("C-k: 5-2"));
//			nStep = 6;
//			return Reset(GOTO_RECURSIVE);
//		} else if (m_oClipboardData.GetSize() < 2 || m_oClipboardData[1] == NULL) {
////			CUtils::Log(_T("C-k: 5-3"));
//			nStep = 5;
//			return Reset(GOTO_RECURSIVE);
//		}

		if (CUtils::IseMemoPad()
		 || CUtils::IsHidemaru()	// Hidemaru clears mark after copy.
		 || CUtils::IsLotusWordPro()
		 || CUtils::IsMicrosoftWord ()
		 || CUtils::IsOpenOffice()
		 || CUtils::IsTuruKameMail()) {
			// do nothing
//			CUtils::Log(_T("C-k: 5-4"));
		} else {
			// This changed will be required to avoid Thunderbird bug.
			// if (0 < FindReturnFromClipboardData(1) && !CUtils::IsExcel() && !CUtils::IsThunderbird()) {
			if (0 < FindReturnFromClipboardData(1) && !CUtils::IsExcel()) {
//				CUtils::Log(_T("C-k: 5-5"));
				SdKduSu(VK_LEFT);
				Copy_();
				nStep = 5;
				return Reset(GOTO_RECURSIVE);
			} else {
				Kdu(VK_DELETE);
//				CUtils::Log(_T("C-k: 5-6"));
			}
		}

		bKeepAllFormat = bAllFormat;
		if (!bKeepAllFormat
		 && !m_oClipboardData[1]->IsAllMergeableFormat()) {
//			CUtils::Log(_T("C-k: 5-7"));
			bKeepAllFormat = TRUE;
		}

		if (bContinuous && !bKeepAllFormat) {
//			CUtils::Log(_T("C-k: 5-8"));
			*m_oClipboardData[0] += *m_oClipboardData[1];
		} else {
//			CUtils::Log(_T("C-k: 5-9"));
			*m_oClipboardData[0] = *m_oClipboardData[1];
		}
		RestoreClipboardData(0);

//		CUtils::Log(_T("C-k: 5-10"));
		nStep = 6;
		return Reset(GOTO_RECURSIVE);
	case 6:		// add data into kill-ring
//		CUtils::Log(_T("C-k: 6"));
		nStep = 0;
		Su();

		if (bContinuous && bKeepAllFormat) {
			CXkeymacsDll::AddKillRing(FALSE);
		} else {
			CXkeymacsDll::AddKillRing();
		}
		return Reset(GOTO_HOOK);
		break;
	}

	return Reset(GOTO_HOOK);
}

// C-x u: C-/: C-_: Ctrl-Z
int CCommands::Undo()
{
	if (CUtils::IsMicrosoftWord()) {
		static BOOL bIsCtrlDown;
		static BOOL bIsAltDown;
		static BOOL bIsShiftDown;

		bIsCtrlDown = CXkeymacsDll::IsDown(VK_CONTROL, FALSE);
		bIsAltDown = CXkeymacsDll::IsDown(VK_MENU, FALSE);
		bIsShiftDown = CXkeymacsDll::IsDown(VK_SHIFT, FALSE);

		if (!bIsCtrlDown) {
			DepressKey(VK_CONTROL);
		}
		if (bIsAltDown) {
			ReleaseKey(VK_MENU);
		}
		if (bIsShiftDown) {
			ReleaseKey(VK_SHIFT);
		}

		PostMessage(GetFocus(), WM_KEYDOWN, 'Z', 0);

		if (bIsShiftDown) {
			DepressKey(VK_SHIFT);
		}
		if (bIsAltDown) {
			DepressKey(VK_MENU);
		}
		if (!bIsCtrlDown) {
			ReleaseKey(VK_CONTROL);
		}

	} else {
		CdKduCu('Z');
	}
	return Reset(GOTO_HOOK);
}

// C-x C-c: Alt-Space, Alt-C
int CCommands::SaveBuffersKillEmacs()
{
	ClearNumericArgument();

	HWND hWnd = GetForegroundWindow();
	if (hWnd) {
		OriginalWindowPosition *pPos = GetOriginalWindowPosition(hWnd);
		if (pPos && pPos->bMax[ROLL_UP_UNROLL]) {
			RollUpUnroll();
		}
	}

	if (CUtils::IsConsole()) {
		SystemMenu(CMD_CLOSE);
	} else if (CUtils::IsExplorer()) {
		if (!CUtils::IsXPorLater()) {
			ReleaseKey(VK_CONTROL);
		}
		AdKduAu(VK_F4);
	} else if (CUtils::IsInternetExplorer()
			|| CUtils::IsJavaW()) {
		AdKduAu(VK_F4);
	} else {
		AdKduAu(VK_SPACE, 'C');
	}
	return Reset(GOTO_HOOK);
}

// C-z: Alt-Space, Alt-N
int CCommands::IconifyOrDeiconifyFrame()
{
	ClearNumericArgument();
	if (CUtils::IsConsole()) {
		SystemMenu(CMD_MINIMIZE);
	} else {
		AdKduAu(VK_SPACE, 'N');
	}
	return Reset(GOTO_HOOK);
}

int CCommands::Restore()
{
	ClearNumericArgument();
	if (CUtils::IsConsole()) {
		SystemMenu(CMD_RESTORE);
	} else {
		AdKduAu(VK_SPACE, 'R');
	}
	return Reset(GOTO_HOOK);
}

int CCommands::Move()
{
	ClearNumericArgument();
	if (CUtils::IsConsole()) {
		SystemMenu(CMD_MOVE);
	} else {
		AdKduAu(VK_SPACE, 'M');
	}
	return Reset(GOTO_HOOK);
}

int CCommands::Size()
{
	ClearNumericArgument();
	if (CUtils::IsConsole()) {
		SystemMenu(CMD_SIZE);
	} else {
		AdKduAu(VK_SPACE, 'S');
	}
	return Reset(GOTO_HOOK);
}

int CCommands::Minimize()
{
	return IconifyOrDeiconifyFrame();
}

int CCommands::Maximize()
{
	ClearNumericArgument();

	HWND hWnd = GetForegroundWindow();
	if (!hWnd) {
		return Reset(GOTO_HOOK);
	}

	WINDOWPLACEMENT wndpl = {sizeof(WINDOWPLACEMENT)};
	if (!GetWindowPlacement(hWnd, &wndpl)) {
		return Reset(GOTO_HOOK);
	}

	if (wndpl.showCmd == SW_SHOWMAXIMIZED) {
		wndpl.showCmd = SW_RESTORE;
	} else {
		wndpl.showCmd = SW_SHOWMAXIMIZED;
	}

	SetWindowPlacement(hWnd, &wndpl);
	return Reset(GOTO_HOOK);
}

OriginalWindowPosition* CCommands::GetOriginalWindowPosition(HWND hWnd)
{
	for (int i = 0; i < MAX_WINDOW; i++) {
		if (m_OriginalWindowPosition[i].hWnd == hWnd) {
			return &m_OriginalWindowPosition[i];
		}
	}

	for (int j = 0; j < MAX_WINDOW; j++) {
		if (m_OriginalWindowPosition[j].hWnd == 0) {
			m_OriginalWindowPosition[j].hWnd = hWnd;
			memset(&m_OriginalWindowPosition[(j + 1) % MAX_WINDOW], 0, sizeof(OriginalWindowPosition));
			return &m_OriginalWindowPosition[j];
		}
	}

	ASSERT(0);
	return NULL;
}

int CCommands::Maximize(MAXIMIZE_DIRECTION direction)
{
	ClearNumericArgument();

	HWND hWnd = GetForegroundWindow();
	if (!hWnd) {
		return Reset(GOTO_HOOK);
	}

	RECT workarea = {'\0'};
	if (!SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0)) {
		return Reset(GOTO_HOOK);
	}

	RECT window = {'\0'};
	if (!GetWindowRect(hWnd, &window)) {
		return Reset(GOTO_HOOK);
	}

	OriginalWindowPosition *pPos = GetOriginalWindowPosition(hWnd);
	if (!pPos) {
		return Reset(GOTO_HOOK);
	}

	int X = window.left;
	int Y = window.top;
	int nWidth = window.right - window.left;
	int nHeight = window.bottom - window.top;

	switch (direction) {
	case VERTICAL:
		if (pPos->bMax[direction]) {
			Y = pPos->nOriginalY;
			nHeight = pPos->nOriginalHeight;
		} else {
			pPos->nOriginalY = Y;
			pPos->nOriginalHeight = nHeight;

			Y = workarea.top;
			nHeight = workarea.bottom - workarea.top;
		}
		break;
	case HORIZONTAL:
		if (pPos->bMax[direction]) {
			X = pPos->nOriginalX;
			nWidth = pPos->nOriginalWidth;
		} else {
			pPos->nOriginalX = X;
			pPos->nOriginalWidth = nWidth;

			X = workarea.left;
			nWidth = workarea.right - workarea.left;
		}
		break;
	case ROLL_UP_UNROLL:
		if (pPos->bMax[direction]) {
			nHeight = pPos->nOriginalHeight;
		} else {
			pPos->nOriginalHeight = nHeight;

			nHeight = 0x15;
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	MoveWindow(hWnd, X, Y, nWidth, nHeight, TRUE);
	pPos->bMax[direction] = !pPos->bMax[direction];
	return Reset(GOTO_HOOK);
}

int CCommands::MaximizeVertically()
{
	return Maximize(VERTICAL);
}

int CCommands::MaximizeHorizontally()
{
	return Maximize(HORIZONTAL);
}

// C-Space: 
int CCommands::SetMarkCommand()
{
	ClearNumericArgument();
	if (m_bSetMark) {
		DeactivateMark();
	}
	m_bSetMark = TRUE;

	if (CUtils::IsConsole()) {
		SystemMenuEdit(CMD_MARK);
	}

	return Reset(GOTO_HOOK);
}

// C-g: Keyboard quit
int CCommands::KeyboardQuit()
{
	ClearNumericArgument();
	if (CUtils::IsFindDialog()) {
		Kdu(VK_ESCAPE);
	}
	if (m_bSetMark) {
		m_bSetMark = FALSE;
		DeactivateMark();
	}
	SetTemporarilyDisableXKeymacs(FALSE);
	return Reset(GOTO_HOOK);
}

DWORD CCommands::ClickCaret()
{
	HWND hWnd = GetFocus();

	POINT CaretPos = {'\0'};
	if (!GetCaretPos(&CaretPos)) {
		return GetLastError();
	}
//	CUtils::Log(_T("client: x = %d, y = %d"), CaretPos.x, CaretPos.y);

	if (!ClientToScreen(hWnd, &CaretPos)) {
		return GetLastError();
	}
//	CUtils::Log(_T("screen: x = %d, y = %d"), CaretPos.x, CaretPos.y);

	POINT Delta = {0, 1};
	if (CUtils::IsSakuraEditor()) {
		Delta.x = 1;
		Delta.y = 29;	// OK: 29-42 NG: 28, 43
	}

	const POINT clickPoint = {CaretPos.x + Delta.x, CaretPos.y + Delta.y};
	return Click(&clickPoint);
}

DWORD CCommands::DeactivateMark()
{
	if (CUtils::IsOpenOffice()
	 || CUtils::IsVisualBasicEditor()) {
		// GetCaretPos always returen (x,y) = (0,0) on OpenOffice, so ...
		// GetCaretPos always returen (x,y) = (-2199,*) on M$ Excel VBE, so ...
		Kdu(VK_ESCAPE);
		return ERROR_SUCCESS;	// i.e. return 0;
	}
	if (CUtils::IsFlash()
	 || CUtils::IsInternetExplorer()
	 || CUtils::IsMicrosoftPowerPoint()
	 || CUtils::IsMicrosoftWord()
	 || CUtils::IsSleipnir()
	 || CUtils::IsThunderbird()) {
		// GetCaretPos always returen (x,y) = (0,0) on M$ Word and Thunderbird, so ...
		// GetCaretPos always returen start point on IE (and Sleipnir that uses IE), so ...
		Kdu(VK_RIGHT);
		return ERROR_SUCCESS;	// i.e. return 0;
	}
	if (CUtils::IsDirector()) {
		// GetCaretPos always returen (x,y) = (0,0) on Macromedia Director, so ...
		Kdu(VK_RIGHT, VK_LEFT);
		return ERROR_SUCCESS;	// i.e. return 0;
	}
	if (CUtils::IsExplorer()) {
		AdKduAu('E', 'A');
		AdKduAu('E', 'I');
		return ERROR_SUCCESS;	// i.e. return 0;
	}
	if (CUtils::IsVisualStudio()) {	// Ctrl + Click select a word on Visual Studio .NET
		BOOL bIsCtrlDown = CXkeymacsDll::IsDown(VK_CONTROL, FALSE);
		if (bIsCtrlDown)
			ReleaseKey(VK_CONTROL);
		DWORD res = ClickCaret();
		if (bIsCtrlDown)
			DepressKey(VK_CONTROL);
		return res;
	}

	return ClickCaret();
}

// C-x C-f: Alt-F, Alt-O
int CCommands::FindFile()
{
	ClearNumericArgument();
	if (CUtils::IsFlash()
	 || CUtils::IsSleipnir()) {
		CdKduCu('O');
	} else if (CUtils::IsVisualStudio()) {
		AdKduAu('F', 'O', 'F');
	} else {
		AdKduAu('F', 'O');
	}
	return Reset(GOTO_HOOK);
}

// C-i: Tab
int CCommands::TabToTabStop()
{
	ClearNumericArgument();
	if (m_bSetMark) {
		m_bSetMark = FALSE;
	}
	Kdu(VK_TAB);
	return Reset(GOTO_HOOK);
}

// C-m: Enter
int CCommands::Newline()
{
	Kdu(VK_RETURN);
	return Reset(GOTO_HOOK);
}

// C-x h: Ctrl-End, Ctrl-Shift-Home
int CCommands::MarkWholeBuffer()
{
	ClearNumericArgument();
	if (CUtils::IsConsole()) {
		SystemMenuEdit(CMD_SELECT_ALL);
	} else if (CUtils::IsDreamweaver()
			|| CUtils::IsFireworks()
			|| CUtils::IsFlash()
			|| CUtils::IsIllustrator()
			|| CUtils::IsPaint()
			|| CUtils::IsPhotoshop()) {
		CdKduCu('A');
	} else if (CUtils::IsExplorer()
			|| CUtils::IsReget()) {
		Kdu(VK_END);
		m_bSetMark = TRUE;
		return Reset(MoveCaret(VK_HOME));
	} else {
		CdKduCu(VK_END);
		m_bSetMark = TRUE;
		return Reset(MoveCaret(VK_HOME, TRUE));
	}
	return Reset(GOTO_HOOK);
}

// C-x C-p:
int CCommands::MarkPage()
{
	return MarkWholeBuffer();
}

// C-x C-s: Alt-F, Alt-S
int CCommands::SaveBuffer()
{
	ClearNumericArgument();
	if (CUtils::IsFlash()
	 || CUtils::IsSleipnir()
	 || CUtils::IsThunderbird()) {
		CdKduCu('S');
		return Reset(GOTO_HOOK);
	} else {
		// default
		AdKduAu('F', 'S');
		return Reset(GOTO_HOOK);
	}
}

// C-v: Page Down
int CCommands::ScrollUp()
{
	if (m_bNegativeNumericArgument) {
		m_bNegativeNumericArgument = FALSE;
		return ScrollDown();
	}

	if (m_bDefaultNumericArgument) {
		return Reset(MoveCaret(VK_NEXT));
	} else {
		while (m_nNumericArgument--) {
			SendMessage(GetFocus(), WM_VSCROLL, SB_LINEDOWN, NULL);
		}
	}
	return Reset(GOTO_HOOK);
}

// M-v: Page Up
int CCommands::ScrollDown()
{
	if (m_bNegativeNumericArgument) {
		m_bNegativeNumericArgument = FALSE;
		return ScrollUp();
	}

	if (m_bDefaultNumericArgument) {
		if (CUtils::IsFirefox()
		 || CUtils::IsVisualCpp()) {
			CdKduCu(0);	// dummy to clear Alt effect.
		}
		return Reset(MoveCaret(VK_PRIOR));
	} else {
		while (m_nNumericArgument--) {
			SendMessage(GetFocus(), WM_VSCROLL, SB_LINEUP, NULL);
		}
	}
	return Reset(GOTO_HOOK);
}

BOOL CCommands::bC_()
{
	return m_bC_;
}

void CCommands::bC_(BOOL b)
{
	m_bC_ = b;
}

BOOL CCommands::bC_x()
{
	return m_bC_x;
}

void CCommands::bC_x(BOOL b)
{
	m_bC_x = b;
}

BOOL CCommands::bM_x()
{
	return m_bM_x;
}

void CCommands::bM_x(const BOOL b)
{
	m_bM_x = b;

	if (b) {
		CXkeymacsDll::SetM_xTip("");
	} else {
		CXkeymacsDll::SetM_xTip(NULL);
	}
}

// C-x: only set flag
int CCommands::C_x()
{
	if (CXkeymacsDll::GetEnableCUA() && m_bSetMark) {
		Cut();
	} else {
		bC_x(TRUE);
	}
	return Reset(GOTO_HOOKX);
}

// C-x C-w: Alt-F, Alt-A
int CCommands::WriteFile()
{
	ClearNumericArgument();
	if (CUtils::IsSleipnir()) {
		CdKduCu('S');
	} else if (CUtils::IsFlash()) {
		CdSdKduSuCu('S');
	} else {
		AdKduAu('F', 'A');
	}
	return Reset(GOTO_HOOK);
}

int CCommands::Paste()
{
	ClearNumericArgument();
	m_bSetMark = FALSE;
	if (CUtils::IsConsole()) {
		SystemMenuEdit(CMD_PASTE);
	} else {
		CdKduCu('V');
	}
	return Reset(GOTO_HOOK);
}

// C-w: Ctrl-X
int CCommands::KillRegion()
{
	static int nStep = 0;
	switch (nStep) {
	case 0:
		m_bSetMark = FALSE;
		ClearNumericArgument();
		CdKduCu('X');
		if (CUtils::IsExplorer()) {
			return Reset(GOTO_HOOK);
		}
		nStep = 1;
		return Reset(GOTO_RECURSIVE);
	case 1:
		nStep = 0;
		CXkeymacsDll::AddKillRing();
		return Reset(GOTO_HOOK);
	}
	// dummy
	return Reset(GOTO_HOOK);
}

// M-w: Ctrl-C
int CCommands::KillRingSave()
{
	static int nStep = 0;
	switch (nStep) {
	case 0:
		ClearNumericArgument();
		m_bSetMark = FALSE;
		if (CUtils::IsConsole()) {
			SystemMenuEdit(CMD_COPY);
			return Reset(GOTO_HOOK);
		} else if (CUtils::IsMicrosoftPowerPoint()) {	// PowerPoint did not want to deactivate-mark. Temporary code?
			CdKduCu('X');
		} else {
			CdKduCu('C');
		}
		if (CUtils::IsExplorer()) {
			return Reset(GOTO_HOOK);
		}
		nStep = 1;
		return Reset(GOTO_RECURSIVE);
	case 1:
		nStep = 0;
		CXkeymacsDll::AddKillRing();
		if (CUtils::IsMicrosoftPowerPoint()) {	// Because DeactivateMark move object like graph.
			CdKduCu('V');
		} else {
			DeactivateMark();
		}
		return Reset(GOTO_HOOK);
	}
	// dummy
	return Reset(GOTO_HOOK);
}

// C-y: Ctrl-V
int CCommands::Yank()
{
	m_bSetMark = FALSE;
	if (CUtils::IsConsole()) {
		SystemMenuEdit(CMD_PASTE);
	} else {
		static CClipboardSnap* pSnap = NULL;
		if (!m_bDefaultNumericArgument) {
			DecreaseNumericArgument();
			CXkeymacsDll::IncreaseKillRingIndex(m_nNumericArgument);
			pSnap = CXkeymacsDll::GetKillRing(pSnap);
		} else if (pSnap) {
			pSnap = CXkeymacsDll::GetKillRing(pSnap);
		} else {
			if (CUtils::IsExcel() && m_LastKillCommand == KillRegion) {
				// do nothing to work C-w not Copy but Cut
			} else if (CUtils::IsLotusNotes() && (m_LastKillCommand == KillRegion || m_LastKillCommand == KillRingSave)) {
				// do nothing
			} else {
				pSnap = CXkeymacsDll::GetKillRing(pSnap, FALSE);
			}
		}
		CdKduCu('V');
		if (pSnap) {
			return Reset(GOTO_RECURSIVE);
		}
	}
	return Reset(GOTO_HOOK);
}

// M-y: yank-pop
int CCommands::YankPop()
{
	ClearNumericArgument();

	if (m_LastCommand == YankPop
	 || m_LastCommand == Yank) {
		static CClipboardSnap* pSnap = NULL;
		if (!pSnap) {
			CClipboardSnap* pOldSnap = NULL;
			pOldSnap = CXkeymacsDll::GetKillRing(pOldSnap);
			if (pOldSnap) {
				for (int i = 0; i < pOldSnap->GetCount(); ++i) {
					CdKduCu('Z');
				}
			} else {
				CdKduCu('Z');
			}
			CXkeymacsDll::IncreaseKillRingIndex();
		}
		pSnap = CXkeymacsDll::GetKillRing(pSnap);
		CdKduCu('V');
		if (pSnap) {
			return Reset(GOTO_RECURSIVE);
		}
	}

	return Reset(GOTO_HOOK);
}

// M-!: cmd.exe/command.com
int CCommands::ShellCommand()
{
	TCHAR szShell[MAX_PATH];
	size_t len;
	!_tgetenv_s(&len, szShell, _T("XKEYMACS_SHELL")) && len != 0 && CUtils::Run(szShell) ||
		!_tgetenv_s(&len, szShell, _T("COMSPEC")) && len != 0 && CUtils::Run(szShell);
	return Reset(GOTO_HOOK);
}

int CCommands::Ignore()
{
	Reset();	// If you delete this line, Ignore() works as Enable/Disable XKeymacs only on Release build. Why?
	return Reset(GOTO_HOOK);
}

void CCommands::bM_(BOOL b)
{
	m_bM_ = b;
}

BOOL CCommands::bM_()
{
	return m_bM_;
}

// C-[
int CCommands::Meta()
{
	bM_(TRUE);
	return Reset(GOTO_HOOKX);
}

// Esc
int CCommands::MetaEscape()
{
	Meta();
	return Reset(GOTO_DO_NOTHING);
}

// Alt
int CCommands::MetaAlt()
{
	return Reset(GOTO_DO_NOTHING);
}

// C-0
int CCommands::NumericArgument0()
{
	return NumericArgument(0);
}

// C-1
int CCommands::NumericArgument1()
{
	return NumericArgument(1);
}

// C-2
int CCommands::NumericArgument2()
{
	return NumericArgument(2);
}

// C-3
int CCommands::NumericArgument3()
{
	return NumericArgument(3);
}

// C-4
int CCommands::NumericArgument4()
{
	return NumericArgument(4);
}

// C-5
int CCommands::NumericArgument5()
{
	return NumericArgument(5);
}

// C-6
int CCommands::NumericArgument6()
{
	return NumericArgument(6);
}

// C-7
int CCommands::NumericArgument7()
{
	return NumericArgument(7);
}

// C-8
int CCommands::NumericArgument8()
{
	return NumericArgument(8);
}

// C-9
int CCommands::NumericArgument9()
{
	return NumericArgument(9);
}

// C-#
int CCommands::NumericArgument(int n)
{
	if (m_bDefaultNumericArgument) {
		m_nNumericArgument = n;
		m_bDefaultNumericArgument = FALSE;
	} else {
		m_nNumericArgument = m_nNumericArgument * 10 + n;
	}
	return Reset(GOTO_HOOK0_9);
}

// C--, M--
int CCommands::NumericArgumentMinus()
{
	m_bNegativeNumericArgument = !m_bNegativeNumericArgument;
	return Reset(GOTO_HOOK0_9);
}

// C-u
int CCommands::UniversalArgument()
{
	if (m_bC_u) {
		m_nNumericArgument *= 4;
	} else {
		m_nNumericArgument = 4;
		m_bC_u = TRUE;
	}
	return Reset(GOTO_HOOKX);
}

// change function name and member variable name like bUniversalArgument()
BOOL CCommands::bC_u()
{
	return m_bC_u;
}

void CCommands::bC_u(BOOL b)
{
	m_bC_u = b;
}

int CCommands::Repeat(BYTE bVk)
{
	if (m_bDefaultNumericArgument) {
		return Reset(GOTO_DO_NOTHING);
	} else {
		CXkeymacsDll::Kdu(bVk, m_nNumericArgument);
		return Reset(GOTO_HOOK);
	}
}

// C-l: Recenter
int CCommands::Recenter()
{
	if (CUtils::IsBecky()) {
		static int nStep = 0;
		RECT ClientRect = {'\0'};
		GetClientRect(GetFocus(), &ClientRect);
		static int gap = INT_MAX;

		static POINT CaretPos = {'\0'};
		GetCaretPos(&CaretPos);

		switch (nStep) {
		case 0:
			nStep = 0;	// omaginai
			if (CaretPos.y < ClientRect.bottom / 2) {
				SendMessage(GetFocus(), WM_VSCROLL, SB_LINEUP, NULL);
			} else if (ClientRect.bottom / 2 < CaretPos.y) {
				SendMessage(GetFocus(), WM_VSCROLL, SB_LINEDOWN, NULL);
			} else {
				gap = INT_MAX;
				return Reset(GOTO_HOOK);
				break;
			}

			Kdu(VK_DOWN);
			nStep = 1;
			return Reset(GOTO_RECURSIVE);

		case 1:
			nStep = 0;
			// When a client screen can display 2n lines, the center line is (n + 1)th line.
			int newgap = CaretPos.y - ClientRect.bottom / 2;
			if (fabs((double)gap) <= fabs((double)newgap)) {
				if ((fabs((double)gap) == fabs((double)newgap)) && (gap != newgap) && newgap < 0) {
					SendMessage(GetFocus(), WM_VSCROLL, SB_LINEUP, NULL);
				}
				gap = INT_MAX;
				return Reset(GOTO_HOOK);
				break;
			}

			gap = newgap;
			return Reset(GOTO_RECURSIVE);
		}
	} else if (CUtils::IsMicrosoftWord()
			|| CUtils::IsThunderbird()) {
		RECT ClientRect = {'\0'};
		GetClientRect(GetFocus(), &ClientRect);
//		CUtils::Log(_T("top = %d, bottom = %d, left = %d, right = %d"), ClientRect.top, ClientRect.bottom, ClientRect.left, ClientRect.right);

		POINT CaretPos = {'\0'};
		GetCaretPos(&CaretPos);

		static const int LENGTH_PER_SCROLL = 32;

		if (CaretPos.y < ClientRect.bottom / 2) {
			VScroll(SB_LINEUP, (ClientRect.bottom / 2 - CaretPos.y) / (LENGTH_PER_SCROLL / 2) + 1);
		} else if (ClientRect.bottom / 2 < CaretPos.y) {
			VScroll(SB_LINEDOWN, (CaretPos.y - ClientRect.bottom / 2) / (LENGTH_PER_SCROLL / 2));
		}

		return Reset(GOTO_HOOK);
	} else {
		RECT ClientRect = {'\0'};
		GetClientRect(GetFocus(), &ClientRect);
		int gap = INT_MAX;

//		CUtils::Log(_T("top = %d, bottom = %d, left = %d, right = %d"), ClientRect.top, ClientRect.bottom, ClientRect.left, ClientRect.right);

		for (;;) {
			POINT CaretPos = {'\0'};
			GetCaretPos(&CaretPos);

			if (CaretPos.y < ClientRect.bottom / 2) {
				VScroll(SB_LINEUP);
			} else if (ClientRect.bottom / 2 < CaretPos.y) {
				VScroll(SB_LINEDOWN);
			} else {
				break;
			}

			// When a client screen can display 2n lines, the center line is (n + 1)th line.
			int newgap = CaretPos.y - ClientRect.bottom / 2;
			if (fabs((double)gap) <= fabs((double)newgap)) {
				if ((fabs((double)gap) == fabs((double)newgap)) && (gap != newgap) && newgap < 0) {
					VScroll(SB_LINEUP);
				}
				break;
			}
			gap = newgap;
		}
		return Reset(GOTO_HOOK);
	}
	return Reset(GOTO_HOOK);
}

// C-t: Shift-Left, Shift-Delete, Right Shift-Insert, if the cursor is not at EOL (is End Of Line)
//		Left, Shift-Right, Shift-Delete, Left, Shift-Insert, Right, if the cursor is at EOL
int CCommands::TransposeChars()
{
	ClearNumericArgument();	// tmp

	// Do not try to do these command at once.
	// Clipboard has old data till you go out this function.
	static int nStep = 0;
	switch (nStep) {
	case 0:		// Clear Selection
		if (m_bSetMark) {
			m_bSetMark = FALSE;
			Kdu(VK_RIGHT, VK_LEFT);
			nStep = 1;
			return Reset(GOTO_RECURSIVE);
		} else {
			// Do nothing. Go to case 1:
		}
	case 1:		// Get back character to confirm the cursor is at TOF or not.
		nStep = 0;
		CaptureClipboardData();

		// simple version
		if (CUtils::IsLotus123()) {
			// C-t: Left, Shift-Right, Shift-Delete, Left, Shift-Insert, Right
			Kdu(VK_LEFT);
			SdKduSu(VK_RIGHT, VK_DELETE);
			Kdu(VK_LEFT);
			SdKduSu(VK_INSERT);
			Kdu(VK_RIGHT);
			nStep = 7;
			return Reset(GOTO_RECURSIVE);
		}

		if (!CopyBackCharacter()) {
			return Reset(GOTO_DO_NOTHING);
		}

		nStep = 2;
		return Reset(GOTO_RECURSIVE);
	case 2:		// Get next character to confirm the cursor is at the EOF or not.
		{
			nStep = 0;
			Su();

			if (CUtils::IsTOF()) {
				Kdu(VK_LEFT);
				nStep = 7;
				return Reset(GOTO_RECURSIVE);
			}

			if (CUtils::IsDirector()) {
				// CopyBackCharacter move cursor right
				Kdu(VK_LEFT);
				ReleaseKey(VK_CONTROL);
			}

			if (CUtils::IsEudora()) {
				// CopyBackCharacter move cursor right
				Kdu(VK_LEFT);
			}

			if (!CopyNextCharacter()) {
				nStep = 7;
				return Reset(GOTO_RECURSIVE);
			}

			if (CUtils::IsDirector()) {
				DepressKey(VK_CONTROL);
			}

			nStep = 3;
			return Reset(GOTO_RECURSIVE);
		}
	case 3:		// Cut Left Character
		{
			nStep = 0;
			Su();

			CString szClipboardText;
			CUtils::GetClipboardText(&szClipboardText);
//			CUtils::Log(_T("%x, %d, %s"), szClipboardText.GetAt(0), szClipboardText.GetLength(), szClipboardText);

			if (CUtils::IsDirector()
			 ||	CUtils::IsEudora()) {
				// CopyNextCharacter move cursor left
				Kdu(VK_RIGHT);
			}

			if (!szClipboardText.IsEmpty()
			 && szClipboardText.GetAt(0) != VK_RETURN) {
				if (CUtils::IseMemoPad()
				 || CUtils::IsFlash()
				 || CUtils::IsK2Editor()
				 || CUtils::IsShuriken()) {
					SdKduSu(VK_LEFT);
					CdKduCu('X');
				} else if (CUtils::IsDirector()) {
					ReleaseKey(VK_CONTROL);
					SdKduSu(VK_LEFT);
					CdKduCu('X');
					Kdu(VK_RIGHT);
					DepressKey(VK_CONTROL);
				} else {
					SdKduSu(VK_LEFT, VK_DELETE);
				}
				nStep = 4;
				return Reset(GOTO_RECURSIVE);
			} else {
				if (szClipboardText.IsEmpty()) {	// EOF
					if (CUtils::IsLotusNotes()
					 || CUtils::IsLotusWordPro()
					 || CUtils::IsMicrosoftFrontPage()
					 || CUtils::IsMicrosoftWord()) {
						// These applications say "EOL is EOF".
						Kdu(VK_END);
					} else {
						// default
						Kdu(VK_RIGHT);
					}
				}

				nStep = 5;
				return Reset(GOTO_RECURSIVE);
			}

			nStep = 7;
			return Reset(GOTO_RECURSIVE);
		}
	case 4:		// Paste the cut character
		nStep = 0;
		Su();
		Kdu(VK_RIGHT);
		if (CUtils::IsDirector()
		 || CUtils::IseMemoPad()
		 || CUtils::IsFlash()
		 || CUtils::IsK2Editor()
		 || CUtils::IsShuriken()) {
			CdKduCu('V');
		} else {
			SdKduSu(VK_INSERT);
		}
		nStep = 7;
		return Reset(GOTO_RECURSIVE);
	case 5:		// Cut character at the EOF.
		nStep = 0;
		if (CUtils::IsDirector()
		 || CUtils::IseMemoPad()
		 || CUtils::IsFlash()
		 || CUtils::IsK2Editor()
		 || CUtils::IsShuriken()) {
			SdKduSu(VK_LEFT);
			CdKduCu('X');
		} else {
			SdKduSu(VK_LEFT, VK_DELETE);
		}
		nStep = 6;
		return Reset(GOTO_RECURSIVE);
	case 6:		// Paste the cut character near the EOF.
		nStep = 0;
		Su();
		Kdu(VK_LEFT);
		if (CUtils::IsDirector()
		 || CUtils::IseMemoPad()
		 || CUtils::IsFlash()
		 || CUtils::IsK2Editor()
		 || CUtils::IsShuriken()) {
			DepressKey(VK_CONTROL);		// Why is this code needed?
			CdKduCu('V');
		} else {
			SdKduSu(VK_INSERT);
		}
		Kdu(VK_RIGHT);
		nStep = 7;
		return Reset(GOTO_RECURSIVE);
	case 7:		// Set the original data on the clipboard.
		nStep = 0;
		Su();
		RestoreClipboardData();
		return Reset(GOTO_HOOK);
	}
	return Reset(GOTO_HOOK);
}

void CCommands::Cut_()
{
	CdKduCu('X');
}

int CCommands::Cut()
{
	ClearNumericArgument();
	m_bSetMark = FALSE;
	Cut_();
	return Reset(GOTO_HOOK);
}

void CCommands::Copy_()
{
	if (CUtils::IsConsole()) {
		SystemMenuEdit(CMD_COPY);
	} else if (CUtils::IsEclipse()
			|| CUtils::IsStoryEditor()) {
		CdKduCu(VK_INSERT);
	} else {
		CdKduCu('C');
	}
}

int CCommands::Copy()
{
	ClearNumericArgument();
	m_bSetMark = FALSE;
	Copy_();
	return Reset(GOTO_HOOK);
}

// Copy next character of the cursor if the caret is NOT at end of the file
// Move the caret to left if caret is at end of the file
BOOL CCommands::CopyNextCharacter()
{
	if (!CUtils::OpenClipboard()) {
		ASSERT(0);
		return FALSE;
	}
	if (!EmptyClipboard()) {
		ASSERT(0);
		return FALSE;
	}
	if (!CloseClipboard()) {
		ASSERT(0);
		return FALSE;
	}

	SdKduSu(VK_RIGHT);
	Copy_();
	Kdu(VK_LEFT);

	return TRUE;
}

BOOL CCommands::CopyBackCharacter()
{
	if (!CUtils::OpenClipboard()) {
		ASSERT(0);
		return FALSE;
	}
	if (!EmptyClipboard()) {
		ASSERT(0);
		return FALSE;
	}
	if (!CloseClipboard()) {
		ASSERT(0);
		return FALSE;
	}

	SdKduSu(VK_LEFT);
	Copy_();
	Kdu(VK_RIGHT);

	return TRUE;
}

BOOL CCommands::CopyPreviousLine()
{
	if (!CUtils::OpenClipboard()) {
		ASSERT(0);
		return FALSE;
	}
	if (!EmptyClipboard()) {
		ASSERT(0);
		return FALSE;
	}
	if (!CloseClipboard()) {
		ASSERT(0);
		return FALSE;
	}

	SdKduSu(VK_UP);
	Copy_();

	return TRUE;
}

BOOL CCommands::CopyCurrentLine()
{
	if (!CUtils::OpenClipboard()) {
		ASSERT(0);
		return FALSE;
	}
	if (!EmptyClipboard()) {
		ASSERT(0);
		return FALSE;
	}
	if (!CloseClipboard()) {
		ASSERT(0);
		return FALSE;
	}

	SdKduSu(VK_HOME);
	Copy_();

	return TRUE;
}

void CCommands::DecreaseNumericArgument()
{
	if (m_bNegativeNumericArgument) {
		++m_nNumericArgument;
	} else {
		if (m_nNumericArgument == 0) {
			++m_nNumericArgument;
			m_bNegativeNumericArgument = TRUE;
		} else {
			--m_nNumericArgument;
		}
	}
}

void CCommands::ClearNumericArgument()
{
	m_bDefaultNumericArgument = TRUE;
	m_nNumericArgument = 1;
	m_bNegativeNumericArgument = FALSE;
}

void CCommands::Reset()
{
	Reset(GOTO_HOOK);
}

int CCommands::Reset(int rc)
{
	switch(rc) {
	case GOTO_HOOK:
		ClearNumericArgument();
		m_bC_u = FALSE;
	case GOTO_HOOK0_9:
		bC_(FALSE);
		bC_x(FALSE);
		bM_(FALSE);
		bM_x(FALSE);
	case GOTO_HOOKX:
		break;

	case GOTO_RECURSIVE:
		break;

	case GOTO_DO_NOTHING:
		break;

	default:
		break;
	}
	return rc;
}

// C-q
int CCommands::EnableOrDisableXKeymacs()
{
	// dummy
	ASSERT(0);
	static int i = 0;	// If there is not this 2 line, EnableOrDisableXKeymacs, EnableXKeymacs and DisableXKeymacs has same function pointer.
	++i;
	return Reset(GOTO_HOOK);
}

int CCommands::EnableXKeymacs()
{
	// dummy
	ASSERT(0);
	static int i = 0;
	++i;
	return Reset(GOTO_HOOK);
}

int CCommands::DisableXKeymacs()
{
	// dummy
	ASSERT(0);
	static int i = 0;
	++i;
	return Reset(GOTO_HOOK);
}

int CCommands::GetNumericArgument()
{
	return m_nNumericArgument;
}

// C-r
int CCommands::IsearchBackward()
{
	return Search(BACKWARD);
}

// C-s
int CCommands::IsearchForward()
{
	return Search(FORWARD);
}

void CCommands::OpenFindDialog()
{
	if (CUtils::IsAutla()
	 || CUtils::IsBorlandCppBuilder()
	 || CUtils::IsCodeWarrior()
	 || CUtils::IsDana()
	 || CUtils::IseMemoPad()
	 || CUtils::IsJmEditor()
	 || CUtils::IsK2Editor()
	 || CUtils::IsOedit()
	 || CUtils::IsOpenJane()
	 || CUtils::IsPHPEditor()
	 || CUtils::IsStoryEditor()
	 || CUtils::IsTeraPad()) {
		AdKduAu('S', 'F');
	} else if (CUtils::IsConsole()) {
		SystemMenuEdit(CMD_FIND);
	} else if (CUtils::IsSakuraEditor()) {
		AdKduAu('S', VK_RETURN);
	} else if (CUtils::IsNami2000()) {
		AdKduAu('D', 'F');
	} else if (CUtils::IsVisualStudio()) {
		AdKduAu('E', 'F', 'F');
	} else if (CUtils::IsDirector()) {
		AdKduAu('E', 'F', 'T');
	} else if (CUtils::IsAdobeReader()) {
		AdKduAu('E', 'S');
	} else if (CUtils::IsDWFM()
			|| CUtils::IsEggExplorer()
			|| CUtils::IsExplorer()
			|| CUtils::IsOutlookExpress()) {
		Kdu(VK_F3);
	} else if (CUtils::IsHidemaru()
			|| CUtils::IsLunascape()
			|| CUtils::IsMicrosoftPowerPoint()
			|| CUtils::IsMozilla()
			|| CUtils::IsNetscape()
			|| CUtils::IsOpera()
			|| CUtils::IsSleipnir()) {
		CdKduCu('F');
	} else if (CUtils::IsDreamweaver()
			|| CUtils::IsezHTML()
			|| CUtils::IsFlash()) {
		DepressKey(VK_CONTROL);		// Why is this coad needed?
		CdKduCu('F');
	} else if (CUtils::IsIPMessenger()) {
		DepressKey(VK_CONTROL);
		PostMessage(GetFocus(), WM_KEYDOWN, 'F', 0);
		PostMessage(GetFocus(), WM_KEYUP, 'F', 0);
		ReleaseKey(VK_CONTROL);
	} else {
		// default
		AdKduAu('E', 'F');
	}

	// Set Direction if Find dialog already exist
	if (CUtils::IsDWFM()
	 || CUtils::IsEggExplorer()
	 || CUtils::IsExplorer()
	 || CUtils::IsFirefox()
	 || CUtils::IsHidemaru()
	 || CUtils::IsK2Editor()
	 || CUtils::IsLotusWordPro()
	 || CUtils::IsMicrosoftPowerPoint()
	 || CUtils::IsMozilla()
	 || CUtils::IsMSDN()
	 || CUtils::IsNetscape()
	 || CUtils::IsOpera()
	 || CUtils::IsSakuraEditor()) {
	} else if (CUtils::IsInternetExplorer()
			|| CUtils::IsSleipnir()) {
		// I want to set direction but IE does not allow me.
	} else {
		SetSearchDirection();
	}
}

int CCommands::Search(SEARCH_DIRECTION direction)
{
//	CUtils::Log(_T("%s"), CUtils::GetApplicationName());

	if (CUtils::IsIllustrator()
	 || CUtils::IsPaint()
	 || CUtils::IsPhotoshop()) {
		return Reset(GOTO_DO_NOTHING);
	}

	ClearNumericArgument();

	if ((direction != FORWARD) && (direction != BACKWARD)) {
		ASSERT(0);
	}

	if (CUtils::IsVisualCpp()
	 || CUtils::IsVisualStudio()) {
		switch (direction) {
		case FORWARD:
			CdKduCu('I');
			break;
		case BACKWARD:
			CdSdKduSuCu('I');
			break;
		default:
			ASSERT(0);
			break;
		}
		return Reset(GOTO_HOOK);
	}

	if (!CUtils::IsFindDialog()) {
//		CUtils::Log(_T("Find Dialog is not."));

		// F3 or Shift+F3
		if (m_LastCommand == IsearchForward
		 || m_LastCommand == IsearchBackward) {
			if (CUtils::IsBecky()
			 || CUtils::IsFirefox()
			 || CUtils::IsHidemaru()
			 || CUtils::IsPHPEditor()) {
				if (m_LastCommand == IsearchForward && direction != FORWARD
				 || m_LastCommand == IsearchBackward && direction != BACKWARD) {
					// do nothing
				} else if (direction == FORWARD) {
					Kdu(VK_F3);
				} else if (direction == BACKWARD) {
					SdKduSu(VK_F3);
				}
				return Reset(GOTO_HOOK);
			} else if (CUtils::IsBorlandCppBuilder()) {
				if (m_LastCommand == IsearchForward && direction != FORWARD
				 || m_LastCommand == IsearchBackward && direction != BACKWARD) {
					AdKduAu('S', 'F');
					SetSearchDirection(direction);
					Kdu(VK_RETURN);
				} else {
					Kdu(VK_F3);
				}
				return Reset(GOTO_HOOK);
			}
		}

		m_SearchDirection = direction;
		OpenFindDialog();
	} else {
//		CUtils::Log(_T("Find Dialog is opened."));

		if (CUtils::IsHidemaru()
		 || CUtils::IsK2Editor()
		 || CUtils::IsOedit()
		 || CUtils::IsSakuraEditor()) {
			BYTE bDirection = 0;

			if (direction == FORWARD) {
				bDirection = GetDirectionForwardKey();
			} else if (direction == BACKWARD) {
				bDirection = GetDirectionBackwardKey();
			} else {
				ASSERT(0);
			}
			AdKduAu(bDirection);
		} else if (CUtils::IsBorlandCppBuilder()) {
			if (m_SearchDirection != direction) {
				SetSearchDirection(direction);
			} else {
				Kdu(VK_RETURN);
			}
		} else if (CUtils::IsJmEditor()
				|| CUtils::IsPHPEditor()) {
			Kdu(VK_RETURN);
		} else if (CUtils::IsAutla()) {
			if (direction == FORWARD) {
				Kdu(VK_F3);
			} else if (direction == BACKWARD) {
				SdKduSu(VK_F3);
			} else {
				ASSERT(0);
			}
		} else {
			if (m_SearchDirection != direction) {
				// Set Direction if Find dialog already exist
				if (CUtils::IsConsole()) {
					SetSearchDirection(BACKWARD);	// bad style
					m_SearchDirection = direction;
				} else {
					SetSearchDirection(direction);
				}
				return Reset(GOTO_HOOK);
			}

			// "Find Next(F)" if Find dialog already exist
			if (CUtils::IsStoryEditor()) {
				AdKduAu('F');
			} else {
				Kdu(VK_RETURN);
			}

			// open Find dialog again
			CString szDialogTitle;
			CUtils::GetFindDialogTitle(&szDialogTitle);
			if (CUtils::IsNami2000()) {
				AdKduAu('D', 'F');
				SetSearchDirection();
			} else if (CUtils::IsOutlook()
					&& szDialogTitle.Compare(_T("ŒŸõ"))) {
				// Outlook (w/o Japanese version)
				AdKduAu('E', 'F');
			} else if (CUtils::IsVisualCpp()) {
				CdKduCu('F');
				DepressKey(VK_CONTROL);		// Why is this need???
			} else if (CUtils::IsCodeWarrior()
					|| CUtils::IsDana()
					|| CUtils::IsStoryEditor()
					|| CUtils::IsTeraPad()) {
				AdKduAu('S', 'F');
			} else if (CUtils::IsezHTML()) {
				CdKduCu('F');
			}

			// set cursor at end of FindWhat
			if (CUtils::IsMozilla()
			 || CUtils::IsNetscape()) {
				// do nothing
			} else if (CUtils::IsMicrosoftWord()) {
				CUtils::UpdateKeyboardState(VK_CONTROL, 0);
				ReleaseKey(VK_CONTROL);

				AdKduAu('N');
				Kdu(VK_ESCAPE);
				Kdu(VK_END);

				DepressKey(VK_CONTROL);
				CUtils::UpdateKeyboardState(VK_CONTROL, 1);
			} else {
				AdKduAu(GetFindWhatKey());
			}
			Kdu(VK_END);
		}
	}
	return Reset(GOTO_HOOK);
}

void CCommands::SetSearchDirection(SEARCH_DIRECTION direction)
{
//	CUtils::Log(_T("SetSearchDirection: %d"), direction);
	if ((direction != FORWARD) && (direction != BACKWARD)) {
		return;
	}

	m_SearchDirection = direction;

	if (GetDirectionForwardKey() == 0 && GetDirectionBackwardKey() == 0) {
		Kdu(VK_END);
		return;
	} else if (CUtils::IsConsole()) {
		if (direction == BACKWARD) {	// bad style
			Kdu(VK_TAB, VK_TAB);
			Kdu(VK_RIGHT);
			SdKduSu(VK_TAB, VK_TAB);
		}
	} else if (CUtils::IsMicrosoftWord()) {
		CUtils::UpdateKeyboardState(VK_CONTROL, 0);
		ReleaseKey(VK_CONTROL);

		AdKduAu('M');
		AdKduAu(0xBA);	// VK_OEM_1	Used for miscellaneous characters; it can vary by keyboard. 
						//			Windows 2000/XP: For the US standard keyboard, the ';:' key
		Kdu(VK_UP, VK_UP);
		if (direction == BACKWARD) {
			Kdu(VK_DOWN);
		}
		AdKduAu('N');
		Kdu(VK_ESCAPE);
		Kdu(VK_END);

		DepressKey(VK_CONTROL);
		CUtils::UpdateKeyboardState(VK_CONTROL, 1);
	} else if (CUtils::IsLotusNotes()) {
		BYTE bDirection	= 0;

		if (direction == FORWARD) {
			bDirection = GetDirectionForwardKey();
		} else if (direction == BACKWARD) {
			bDirection = GetDirectionBackwardKey();
		}

		AdKduAu(GetSearchOptionKey(), bDirection, GetSearchOptionKey());
		{
			CString szDialogTitle;
			CUtils::GetFindDialogTitle(&szDialogTitle);

			if (!szDialogTitle.Compare(_T("ŒŸõ"))
			 || !szDialogTitle.Compare(_T("Find"))) {
				SdKduSu(VK_TAB);
			} else {
				SdKduSu(VK_TAB, VK_TAB);
			}
		}
		Kdu(VK_END);
	} else if (CUtils::IsBecky()) {
		AdKduAu(GetDirectionForwardKey());
		if (direction == BACKWARD) {
			Kdu(VK_UP);
		}
		Kdu(VK_TAB, VK_TAB, VK_TAB, VK_TAB);
		Kdu(VK_END);
	} else if (CUtils::IsMozilla()
			|| CUtils::IsNetscape()) {
		Kdu(VK_TAB, VK_TAB, VK_TAB, VK_SPACE);
		SdKduSu(VK_TAB, VK_TAB, VK_TAB);
	} else if (CUtils::IsezHTML()) {
		BYTE bDirection	= 0;

		if (direction == FORWARD) {
			bDirection = GetDirectionForwardKey();
		} else if (direction == BACKWARD) {
			bDirection = GetDirectionBackwardKey();
		}

		AdKduAu(bDirection);
		SdKduSu(VK_TAB, VK_TAB, VK_TAB, VK_TAB);
	} else {
		// default
		BYTE bDirection	= 0;

		if (direction == FORWARD) {
			bDirection = GetDirectionForwardKey();
		} else if (direction == BACKWARD) {
			bDirection = GetDirectionBackwardKey();
		}

//		CUtils::Log(_T("aSetSearchDirection: Direction = %c, FindWhatKey = %c"), bDirection, GetFindWhatKey());
		AdKduAu(bDirection, GetFindWhatKey());
	}
}

// C-x C-j, C-o, C-\, C-BACKSLASH
int CCommands::ToggleInputMethod()
{
	SetInputMethodOpenStatus(TOGGLE_INPUT_METHOD);
	return Reset(GOTO_HOOK);
}

int CCommands::CompleteToggleInputMethod()
{
	SetInputMethodOpenStatus(TOGGLE_INPUT_METHOD, TRUE);
	return Reset(GOTO_HOOK);
}

int CCommands::OpenInputMethod()
{
	SetInputMethodOpenStatus(OPEN_INPUT_METHOD);
	return Reset(GOTO_HOOK);
}

int CCommands::CloseInputMethod()
{
	SetInputMethodOpenStatus(CLOSE_INPUT_METHOD);
	return Reset(GOTO_HOOK);
}

int CCommands::CompleteCloseInputMethod()
{
	SetInputMethodOpenStatus(CLOSE_INPUT_METHOD, TRUE);
	return Reset(GOTO_HOOK);
}

inline HWND ObtainFocus()
{
	const HWND hWnd = GetFocus();
	if (hWnd)
		return hWnd;
	GUITHREADINFO gui;
	gui.cbSize = sizeof(gui);
	return GetGUIThreadInfo(GetWindowThreadProcessId(GetForegroundWindow(), 0), &gui) ? gui.hwndFocus : GetForegroundWindow();
}

void CCommands::SetInputMethodOpenStatus(INPUT_METHOD_OPEN_STATUS status, BOOL isComplete) {
	extern UINT g_ImeManipulationMessage;
	HWND hWnd = ObtainFocus();
	//CUtils::Log(_T(" post ime manip, %d, %d, %p"), status, isComplete, hWnd);
	PostMessage(hWnd, g_ImeManipulationMessage, status, isComplete);
}

void CCommands::DoSetInputMethodOpenStatus(INPUT_METHOD_OPEN_STATUS status, BOOL isComplete)
{
	ClearNumericArgument();
	HKL hKL = GetKeyboardLayout(0);
	if (CUtils::IsConsole()) {
		Kdu(VK_KANJI);
	} else if (ImmIsIME(hKL)) {
		// default
		HWND hWnd = ObtainFocus();
		HIMC hIMC = ImmGetContext(hWnd);
		//CUtils::Log(_T(" do ime manip, %d, %d, %d, %p, %p"), status, isComplete, ImmGetOpenStatus(hIMC), hWnd, hIMC);

		if (isComplete && ImmGetOpenStatus(hIMC)) {
			ImmNotifyIME(hIMC, NI_COMPOSITIONSTR, CPS_COMPLETE, 0);
		}

		switch (status) {
		case CLOSE_INPUT_METHOD:
			ImmSetOpenStatus(hIMC, FALSE);
			break;
		case OPEN_INPUT_METHOD:
			ImmSetOpenStatus(hIMC, TRUE);
			break;
		case TOGGLE_INPUT_METHOD:
			ImmSetOpenStatus(hIMC, !ImmGetOpenStatus(hIMC));
			break;
		default:
			ASSERT(0);
			break;
		}

		ImmReleaseContext(hWnd, hIMC);
	} else {
		Kdu(VK_KANJI);
	}
}

// C-x k: Ctrl+F4
int CCommands::KillBuffer()
{
	CdKduCu(VK_F4);
	return Reset(GOTO_HOOK);
}

BOOL CCommands::IsKillCommand(int (*nFunctionPointer)())
{
	for (int nCommandID = 1; nCommandID < MAX_COMMAND; ++nCommandID) {
		if (Commands[nCommandID].fCommand == nFunctionPointer) {
			return !_tcsncmp(Commands[nCommandID].szCommandName, _T("kill-"), 5);
		}
	}
	return FALSE;
}

void CCommands::SetLastCommand(int (__cdecl *LastCommand)(void))
{
	m_LastCommand = LastCommand;

	if (IsKillCommand(LastCommand)) {
		m_LastKillCommand = LastCommand;
	}
}

// M-d: 
int CCommands::KillWord()
{
	if (CUtils::IsMicrosoftPowerPoint()
	 || CUtils::IsThunderbird()) {
		CdKduCu(VK_DELETE);
		return Reset(GOTO_HOOK);
	}

	static int nStep = 0;
	static CString szClipboardText;
	static int nWhiteSpace = 0;
	switch (nStep) {
	case 0:
//		CUtils::Log(_T("M-d: 0"));
		ClearNumericArgument();
		szClipboardText.Empty();
		nWhiteSpace = 0;
		CdSdKduSuCu(VK_RIGHT);
		CdKduCu('X');
		nStep = 1;
		return Reset(GOTO_RECURSIVE);
	case 1:
		{
//			CUtils::Log(_T("M-d: 1"));
			nStep = 0;
			CUtils::GetClipboardText(&szClipboardText);
			int nWordEnd = szClipboardText.GetLength();
			int nFirstSpace = szClipboardText.Find(_T(' '));
//			CUtils::Log(_T("M-d: 1-1 _%s_%c"), szClipboardText, szClipboardText.GetAt(szClipboardText.GetLength() - 1));
			if (nFirstSpace == 0) {
//				CUtils::Log(_T("M-d: 1-1-1"));
				nStep = 0;
				return Reset(GOTO_RECURSIVE);
			} else if (0 < nFirstSpace) {
//				CUtils::Log(_T("M-d: 1-1-2"));
				nWordEnd = nFirstSpace;	// (nFirstSpace + 1) - 1
			}
			int nFirstTab = szClipboardText.Find(_T('\t'));
			if (nFirstTab == 0) {
//				CUtils::Log(_T("M-d: 1-2-1"));
				nStep = 0;
				return Reset(GOTO_RECURSIVE);
			} else if (0 < nFirstTab && nFirstTab < nWordEnd) {
//				CUtils::Log(_T("M-d: 1-2-2"));
				nWordEnd = nFirstTab;	// (nFirstTab + 1) - 1
			}
			if (nWordEnd == szClipboardText.GetLength()) {
//				CUtils::Log(_T("M-d: 1-3-1"));
				nStep = 4;
			} else {
//				CUtils::Log(_T("M-d: 1-3-2"));
				CString szEndWhiteSpace;
				nWhiteSpace = szClipboardText.GetLength() - nWordEnd;
				szEndWhiteSpace = szClipboardText.Right(nWhiteSpace);
				CUtils::SetClipboardText(&szEndWhiteSpace);
				szClipboardText = szClipboardText.Left(nWordEnd);
				nStep = 2;
			}
			return Reset(GOTO_RECURSIVE);
		}
	case 2:
//		CUtils::Log(_T("M-d: 2"));
		nStep = 0;
		CdKduCu('V');
		nStep = 3;
		return Reset(GOTO_RECURSIVE);
	case 3:
//		CUtils::Log(_T("M-d: 3"));
		nStep = 0;
		m_bDefaultNumericArgument = FALSE;
		m_nNumericArgument = nWhiteSpace;
		Kdu(VK_LEFT);
		nStep = 4;
		return Reset(GOTO_RECURSIVE);
	case 4:
//		CUtils::Log(_T("M-d: 4"));
		nStep = 0;
		CUtils::SetClipboardText(&szClipboardText);
		nStep = 5;
		return Reset(GOTO_RECURSIVE);
	case 5:
//		CUtils::Log(_T("M-d: 5"));
		nStep = 0;
		CXkeymacsDll::AddKillRing();
		m_bSetMark = FALSE;
		return Reset(GOTO_HOOK);
	}
	// dummy
	return Reset(GOTO_HOOK);
}

// M-DEL
int CCommands::BackwardKillWord()
{
	CdSdKduSuCu(VK_LEFT);
	Cut_();
	return Reset(GOTO_HOOK);
}

// Select All
int CCommands::SelectAll()
{
	if (CUtils::IsConsole()) {
		SystemMenuEdit(CMD_SELECT_ALL);
	} else {
		CdKduCu('A');
	}
	return Reset(GOTO_HOOK);
}

// C-x (
int CCommands::StartKbdMacro()
{
	if (CUtils::IsMicrosoftWord()) {
		AdKduAu('T', 'M', 'R');
	} else if (CUtils::IsHidemaru()) {
		SdKduSu(VK_F1);
	} else {
		CXkeymacsDll::DefiningMacro(TRUE);
	}
	return Reset(GOTO_HOOK);
}

// C-x )
int CCommands::EndKbdMacro()
{
	if (CUtils::IsMicrosoftWord()) {
		AdKduAu('T', 'M', 'R');
	} else if (CUtils::IsHidemaru()) {
		SdKduSu(VK_F1);
	} else {
		CXkeymacsDll::DefiningMacro(FALSE);
	}
	return Reset(GOTO_HOOK);
}

// C-x e
int CCommands::CallLastKbdMacro()
{
	if (CUtils::IsMicrosoftWord()) {
		AdKduAu('T', 'M', 'M');
	} else if (CUtils::IsHidemaru()) {
		int n = 1;
		if (!m_bDefaultNumericArgument) {
			n = m_nNumericArgument;
		}
		ClearNumericArgument();
		while (n--) {
			AdKduAu('M', 'P');
		}
	} else {
		while (m_nNumericArgument--) {
			CXkeymacsDll::CallMacro();
		}
	}
	return Reset(GOTO_HOOK);
}

int CCommands::SwitchBetweenInputLocales()
{
	BOOL bIsCtrlDown = CXkeymacsDll::IsDown(VK_CONTROL, FALSE);
	BOOL bIsAltDown = CXkeymacsDll::IsDown(VK_MENU, FALSE);

	// Alt+Shift
	if (bIsCtrlDown) {
		ReleaseKey(VK_CONTROL);
	}
	DepressKey(VK_MENU);	// why?
	ReleaseKey(VK_MENU);
	DepressKey(VK_MENU);
	DepressKey(VK_SHIFT);
	ReleaseKey(VK_SHIFT);
	if (!bIsAltDown) {
		ReleaseKey(VK_MENU);
	}
	if (bIsCtrlDown) {
		DepressKey(VK_CONTROL);
	}

	return Reset(GOTO_HOOK);
}

// C-x b
int CCommands::SwitchToBuffer()
{
	ClearNumericArgument();
	// GetWindowInfo does not get information to find out if the window is MDI or SDI.
	CdKduCu(VK_TAB);
	return Reset(GOTO_HOOK);
}

int CCommands::C_()
{
	return Reset(GOTO_HOOKX);
}

int CCommands::C_Eisu()
{
	bC_(TRUE);
	return Reset(GOTO_HOOKX);
}

int CCommands::Escape()
{
	ClearNumericArgument();
	Kdu(VK_ESCAPE);
	return Reset(GOTO_HOOK);
}

int CCommands::Tilde()
{
	ClearNumericArgument();
	if (CXkeymacsDll::Is106Keyboard()) {
		SdKduSu(0xDE);	// VK_OEM_7		Used for miscellaneous characters; it can vary by keyboard. 
		Su();			//				Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
	} else {
		SdKduSu(0xC0);	// VK_OEM_3		Used for miscellaneous characters; it can vary by keyboard. 
		Su();			//				Windows 2000/XP: For the US standard keyboard, the '`~' key
	}
	return Reset(GOTO_HOOK);
}

int CCommands::BackQuote()
{
	ClearNumericArgument();
	if (CXkeymacsDll::Is106Keyboard()) {
		SdKduSu(0xC0);
		Su();
	} else {
		ReleaseKey(VK_SHIFT);
		Kdu(0xC0);	// VK_OEM_3		Used for miscellaneous characters; it can vary by keyboard. 
					//				Windows 2000/XP: For the US standard keyboard, the '`~' key
	}
	return Reset(GOTO_HOOK);
}

BOOL CCommands::IsSetMark()
{
	return m_bSetMark;
}

BYTE CCommands::GetFindWhatKey()
{
	BYTE bFindWhat = 'N';

	if (CUtils::IsBecky()
	 || CUtils::IsDana()
	 || CUtils::IsEclipse()
	 || CUtils::IsezHTML()
	 || CUtils::IsLotusNotes()
	 || CUtils::IsVisualBasicEditor()) {
		bFindWhat = 'F';
	} else if (CUtils::IseMemoPad()) {
		bFindWhat = 'K';
	} else if (CUtils::IsHidemaru()
			|| CUtils::IsJmEditor()) {
		bFindWhat = 'S';
	} else if (CUtils::IsBorlandCppBuilder()
			|| CUtils::IsNami2000()
			|| CUtils::IsStoryEditor()
			|| CUtils::IsTeraPad()) {
		bFindWhat = 'T';
	} else if (CUtils::IsAdobeReader()
			|| CUtils::IsAutla()
			|| CUtils::IsCodeWarrior()
			|| CUtils::IsFirefox()
			|| CUtils::IsFlash()
			|| CUtils::IsIPMessenger()
			|| CUtils::IsK2Editor()
			|| CUtils::IsOedit()
			|| CUtils::IsOpera()
			|| CUtils::IsOpenJane()
			|| CUtils::IsPHPEditor()
			|| CUtils::IsSakuraEditor()) {
		bFindWhat = 0;
	}

	return bFindWhat;
}

BYTE CCommands::GetSearchOptionKey()
{
	BYTE bSearchOption = 0;

	if (CUtils::IsLotusNotes()) {
		bSearchOption = 'O';
	}

	return bSearchOption;
}

BYTE CCommands::GetDirectionForwardKey()
{
	BYTE bDirectionForward = 'D';
	CString szDialogTitle;
	CUtils::GetFindDialogTitle(&szDialogTitle);

	if (CUtils::IsAdobeReader()
	 || CUtils::IsAutla()
	 || CUtils::IsCodeWarrior()
	 || CUtils::IsDirector()
	 || CUtils::IsDreamweaver()
	 || CUtils::IsExcel()
	 || CUtils::IsFirefox()
	 || CUtils::IsFireworks()
	 || CUtils::IsFlash()
	 || CUtils::IsIPMessenger()
	 || CUtils::IsJmEditor()
	 || CUtils::IsLotus123()
	 || CUtils::IsMicrosoftPowerPoint()
	 || CUtils::IsOpera()
	 || CUtils::IsOpenJane()
	 || CUtils::IsStoryEditor()
	 || CUtils::IsVisualBasicEditor()
	 || CUtils::IsVisualStudio()
	 || CUtils::IsWordpad()) {
		bDirectionForward = 0;
	} else if (CUtils::IsLotusNotes()
			|| CUtils::IsNami2000()
			|| CUtils::IsSakuraEditor()) {
		bDirectionForward = 'D';
	} else if (CUtils::IsOedit()) {
		bDirectionForward = 'F';
	} else if (CUtils::IsBecky()) {
		if (!szDialogTitle.Compare(_T("–{•¶“àŒŸõ"))) {
			bDirectionForward = 'O';
		} else {
			bDirectionForward = 0;
		}
	} else if (CUtils::IsK2Editor()) {
		bDirectionForward = 'L';
	} else if (CUtils::IseMemoPad()
			|| CUtils::IsHidemaru()) {
		bDirectionForward = 'N';
	} else if (CUtils::IsEclipse()) {
		bDirectionForward = 'O';
	}

	if (CUtils::IsOutlook()
	 && !szDialogTitle.Compare(_T("Find"))) {
		// Outlook (English version)
		bDirectionForward = 0;
	}

	return bDirectionForward;
}

BYTE CCommands::GetDirectionBackwardKey()
{
	BYTE bDirectionBackward	= 'U';
	CString szDialogTitle;
	CUtils::GetFindDialogTitle(&szDialogTitle);

	if (CUtils::IsAdobeReader()
	 || CUtils::IsAutla()
	 || CUtils::IsCodeWarrior()
	 || CUtils::IsDirector()
	 || CUtils::IsDreamweaver()
	 || CUtils::IsExcel()
	 || CUtils::IsFirefox()
	 || CUtils::IsFireworks()
	 || CUtils::IsFlash()
	 || CUtils::IsIPMessenger()
	 || CUtils::IsJmEditor()
	 || CUtils::IsLotus123()
	 || CUtils::IsMicrosoftPowerPoint()
	 || CUtils::IsOpera()
	 || CUtils::IsOpenJane()
	 || CUtils::IsStoryEditor()
	 || CUtils::IsVisualBasicEditor()
	 || CUtils::IsWordpad()) {
		bDirectionBackward = 0;
	} else if (CUtils::IsBorlandCppBuilder()
			|| CUtils::IsEclipse()
			|| CUtils::IseMemoPad()
			|| CUtils::IsLotusNotes()
			|| CUtils::IsNami2000()) {
		bDirectionBackward = 'B';
	} else if (CUtils::IsBecky()) {
		if (!szDialogTitle.Compare(_T("–{•¶“àŒŸõ"))) {
			bDirectionBackward = 'B';
		} else {
			bDirectionBackward = 0;
		}
	} else if (CUtils::IsHidemaru()
			|| CUtils::IsOedit()) {
		bDirectionBackward = 'P';
	} else if (CUtils::IsK2Editor()
			|| CUtils::IsSakuraEditor()) {
		bDirectionBackward = 'U';
	}

	if (CUtils::IsOutlook()
	 && !szDialogTitle.Compare(_T("Find"))) {
		// Outlook (English version)
		bDirectionBackward = 0;
	}

	return bDirectionBackward;
}

void CCommands::SetMark(BOOL bSetMark)
{
	m_bSetMark = bSetMark;
}

GOTO CCommands::MoveCaret(BYTE bVk, BOOL bCtrl)
{
	switch (bVk) {
	case VK_PRIOR:
	case VK_NEXT:
	case VK_END:
	case VK_HOME:
	case VK_LEFT:
	case VK_UP:
	case VK_RIGHT:
	case VK_DOWN:
		break;
	default:
		return CONTINUE;
	}

	struct {
		BYTE bVk;
		BOOL bCtrl;
		DWORD time;
	} static last;

	const DWORD time = GetTickCount();
//	CUtils::Log(_T("%d - %d = %d, %d, %d"), time, last.time, time - last.time, CXkeymacsDll::GetMaxKeyInterval(), CXkeymacsDll::GetAccelerate());
	if (CXkeymacsDll::GetAccelerate() && bVk == last.bVk && bCtrl == last.bCtrl && time - last.time < CXkeymacsDll::GetMaxKeyInterval()) {
		NumericArgument(CXkeymacsDll::GetAccelerate());
	}
	last.bVk = bVk;
	last.bCtrl = bCtrl;
	last.time = time;

	if (bCtrl) {
		if (m_bSetMark) {
			CdSdKduSuCu(bVk);
		} else {
			CdKduCu(bVk);
		}
	} else {
		if (m_bSetMark) {
			if (CUtils::IsShuriken()) {
				static BOOL bIsShiftDown = FALSE;
				static int nStep = 0;
				switch (nStep) {
				case 0:
					{
						BOOL bIsCtrlDown = CXkeymacsDll::IsDown(VK_CONTROL, FALSE);
						BOOL bIsAltDown = CXkeymacsDll::IsDown(VK_MENU, FALSE);
						bIsShiftDown = CXkeymacsDll::IsDown(VK_SHIFT, FALSE);

						if (bIsCtrlDown) {
							ReleaseKey(VK_CONTROL);
						}
						if (bIsAltDown) {
							ReleaseKey(VK_MENU);
						}
						if (!bIsShiftDown) {
							DepressKey(VK_SHIFT);
						}

						CXkeymacsDll::Kdu(bVk, m_nNumericArgument);

						if (bIsAltDown) {
							DepressKey(VK_MENU);
						}
						if (bIsCtrlDown) {
							DepressKey(VK_CONTROL);
						}
						nStep = 1;
						return GOTO_RECURSIVE;
					}
				case 1:
					nStep = 0;
					if (!bIsShiftDown) {
						ReleaseKey(VK_SHIFT);
					}
					return GOTO_HOOK;
				}
			} else {
				SdKduSu(bVk);
			}
		} else {
			Kdu(bVk);
		}
	}
	return GOTO_HOOK;
}

int CCommands::PassThrough()
{
	Reset();	// Why?
	return Reset(GOTO_HOOK);
}

int CCommands::Redo()
{
	if (CUtils::IsCorelDRAW()) {
		CdSdKduSuCu('Z');
	} else {
		CdKduCu('Y');
	}
	return Reset(GOTO_HOOK);
}

int CCommands::TemporarilyDisableXKeymacs()
{
//	CUtils::Log(_T("SetTemporarilyDisableXKeymacs"));
	SetTemporarilyDisableXKeymacs(TRUE);
	Reset();	// Why?
	return Reset(GOTO_HOOK);
}

BOOL CCommands::IsTemporarilyDisableXKeymacs()
{
	return m_bTemporarilyDisableXKeymacs;
}

void CCommands::SetTemporarilyDisableXKeymacs(BOOL bTemporarilyDisableXKeymacs)
{
	m_bTemporarilyDisableXKeymacs = bTemporarilyDisableXKeymacs;
	CXkeymacsDll::SetKeyboardHookFlag();
}

int CCommands::New()
{
	ClearNumericArgument();
	m_bSetMark = FALSE;
	CdKduCu('N');
	return Reset(GOTO_HOOK);
}

int CCommands::Print()
{
	ClearNumericArgument();
	m_bSetMark = FALSE;
	CdKduCu('P');
	return Reset(GOTO_HOOK);
}

int CCommands::Find()
{
	ClearNumericArgument();
	m_bSetMark = FALSE;
	CdKduCu('F');
	return Reset(GOTO_HOOK);
}

int CCommands::FindNext()
{
	ClearNumericArgument();
	m_bSetMark = FALSE;
	Kdu(VK_F3);
	return Reset(GOTO_HOOK);
}

int CCommands::SystemMenu(const CONSOLE_MENU nDown)
{
//	CUtils::Log(_T("SystemMenu"));
	if (!CUtils::IsConsole()) {
		return ERROR_SUCCESS;
	}

	RECT consoleWindowRect = {'\0'};
	if (!GetWindowRect(GetForegroundWindow(), &consoleWindowRect)) {
		return GetLastError();
	}
//	CUtils::Log(_T("t = %d, l = %d"), consoleWindowRect.top, consoleWindowRect.left);

	const POINT clickPoint = {consoleWindowRect.left + 5, consoleWindowRect.top + 5};
	int rc = Click(&clickPoint);
	if (rc != ERROR_SUCCESS) {
		return rc;
	}

	if (nDown != CMD_EDIT) {
		for (int i = 0; i <= nDown; ++i) {
			Kdu(VK_DOWN);
		}
		Kdu(VK_UP);
		Kdu(VK_RETURN);
	}
	return ERROR_SUCCESS;	// i.e. return 0;
}

int CCommands::SystemMenuEdit(const CONSOLE_MENU_EDIT nDown)
{
	if (!CUtils::IsConsole()) {
		return ERROR_SUCCESS;
	}

	SystemMenu(CMD_EDIT);
	RECT consoleWindowRect = {'\0'};
	if (!GetWindowRect(GetForegroundWindow(), &consoleWindowRect)) {
		return GetLastError();
	}

	const int y = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU) * 6 + 20;	// y = A + B * 6 + 20
	const POINT clickPoint = {consoleWindowRect.left + 50, consoleWindowRect.top + y};	// 146 <= y <= 161 on W2K	(146 + 161 + 1) / 2 = 154 = 19 + 19 x 6 + 21
	int rc = Click(&clickPoint);														// 157 <= y <= 172 on WXP	(157 + 172 + 1) / 2 = 165 = 26 + 20 x 6 + 19
	if (rc != ERROR_SUCCESS) {
		return rc;
	}

	for (int i = 0; i <= nDown; ++i) {
		Kdu(VK_DOWN);
	}
	Kdu(VK_UP);	// Why? Because Mark did not work well only one VK_DOWN (or seven VK_DOWNs).

	Kdu(VK_RETURN);
	return ERROR_SUCCESS;	// i.e. return 0;
}

int CCommands::Click(const POINT *const pClickPoint)
{
	BOOL bIsAltDown = CXkeymacsDll::IsDown(VK_MENU, FALSE);

	POINT CursorPos = {'\0'};
	if (!GetCursorPos(&CursorPos)) {
		return GetLastError();
	}

	if (!SetCursorPos(pClickPoint->x, pClickPoint->y)) {
		return GetLastError();
	}
	if (CUtils::IsConsole()
	 && bIsAltDown) {
		ReleaseKey(VK_MENU);
	}
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo());
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, GetMessageExtraInfo());
	if (!SetCursorPos(CursorPos.x, CursorPos.y)) {
		return GetLastError();
	}
	return ERROR_SUCCESS;
}

// C-x C-t
int CCommands::TransposeLines()
{
	static int nStep = 0;
	switch (nStep) {
	case 0:
		nStep = 0;
//		CUtils::Log(_T("C-x C-t: 0"));
		Kdu(VK_END);
		CaptureClipboardData();
		nStep = 1;
		return Reset(GOTO_RECURSIVE);
	case 1:
		nStep = 0;
//		CUtils::Log(_T("C-x C-t: 1"));
		if (!CopyNextCharacter()) {
			return Reset(GOTO_DO_NOTHING);
		}
		nStep = 2;
		return Reset(GOTO_RECURSIVE);
	case 2:
		nStep = 0;
//		CUtils::Log(_T("C-x C-t: 2"));
		Su();
		if (CUtils::IsEOF()) {
			Kdu(VK_END, VK_RETURN, VK_UP, VK_END);
		}
		Kdu(VK_HOME);
		nStep = 3;
		return Reset(GOTO_RECURSIVE);
	case 3:
		nStep = 0;
//		CUtils::Log(_T("C-x C-t: 3"));
		if (!CopyBackCharacter()) {
			return Reset(GOTO_DO_NOTHING);
		}
		nStep = 4;
		return Reset(GOTO_RECURSIVE);
	case 4:
		nStep = 0;
//		CUtils::Log(_T("C-x C-t: 4"));
		Su();
		if (CUtils::IsTOF()) {	// TOF
			Kdu(VK_DOWN, VK_END);
//			CUtils::Log(_T("C-x C-t: TOF"));
			nStep = 5;
		} else {
			nStep = 7;
		}
		return Reset(GOTO_RECURSIVE);
	case 5:
		nStep = 0;
//		CUtils::Log(_T("C-x C-t: 5"));
		if (!CopyNextCharacter()) {
			return Reset(GOTO_DO_NOTHING);
		}
		nStep = 6;
		return Reset(GOTO_RECURSIVE);
	case 6:
		nStep = 0;
//		CUtils::Log(_T("C-x C-t: 6"));
		Su();
		if (CUtils::IsEOF()) {
			Kdu(VK_END, VK_RETURN, VK_UP, VK_END);
//			CUtils::Log(_T("C-x C-t: EOF2"));
		}
		nStep = 7;
		return Reset(GOTO_RECURSIVE);
	case 7:
		nStep = 0;
//		CUtils::Log(_T("C-x C-t: 7"));
		Kdu(VK_UP, VK_HOME);
		SdKduSu(VK_DOWN);
		CdKduCu('X');
		nStep = 8;
		return Reset(GOTO_RECURSIVE);
	case 8:
		nStep = 0;
//		CUtils::Log(_T("C-x C-t: 8"));
		Su();
		Kdu(VK_DOWN);
		CdKduCu('V');
		nStep = 9;
		return Reset(GOTO_RECURSIVE);
	case 9:
		nStep = 0;
//		CUtils::Log(_T("C-x C-t: 9"));
		RestoreClipboardData();
		break;
	}
	return Reset(GOTO_HOOK);
}

void CCommands::CaptureClipboardData(const int nID, const BOOL bTextOnly)
{
//	CUtils::Log("CaptureClipboardData: 1");
	{
		int nSize = nID + 1;
		if (m_oClipboardData.GetSize() < nSize) {
//			CUtils::Log("CaptureClipboardData: 2");
			try {
				m_oClipboardData.SetSize(nSize);
			}
			catch (CMemoryException* e) {
				e->Delete();
//				CUtils::Log("CaptureClipboardData: SetSize() threw an exception");
			}
		}
	}

//	CUtils::Log("CaptureClipboardData: 3");
	if (!m_oClipboardData[nID]) {
//		CUtils::Log("CaptureClipboardData: 4");
		try {
			CClipboardSnap *pSnap = new CClipboardSnap;
			if (!pSnap) {
//				CUtils::Log("CaptureClipboardData: 5");
				return;
			}
			m_oClipboardData.SetAt(nID, pSnap);
		}
		catch (CMemoryException* e) {
			e->Delete();
//			CUtils::Log("CaptureClipboardData: 'new' threw an exception");
		}
	}

//	CUtils::Log("CaptureClipboardData: 6: nID=%d, size=%d", nID, m_oClipboardData.GetSize());
	BOOL bCapture = m_oClipboardData[nID]->Capture(bTextOnly);
//	CUtils::Log("CaptureClipboardData: 7");
	Sleep(0);	// for OpenOffice
	bCapture = m_oClipboardData[nID]->Capture(bTextOnly);	// for "office drawing shape format". Can CClipboardSnap care this problem?

//	CUtils::Log("CaptureClipboardData: 8");
	if (!bCapture) {
//		CUtils::Log(_T("Can not Capture in CaptureClipboardData"));
		delete m_oClipboardData[nID];
//		CUtils::Log("CaptureClipboardData: 9");
		m_oClipboardData.SetAt(nID, NULL);
//		CUtils::Log("CaptureClipboardData: 10");
	}
}

void CCommands::RestoreClipboardData(const int nID)
{
	if (nID + 1 <= m_oClipboardData.GetSize() && m_oClipboardData[nID]) {
		m_oClipboardData[nID]->Restore();
	}
}

int CCommands::FindReturnFromClipboardData(const int nID)
{
	if (nID + 1 <= m_oClipboardData.GetSize() && m_oClipboardData[nID]) {
		return m_oClipboardData[nID]->FindReturn();
	}
	return -1;
}

BOOL CCommands::IsEmptyClipboardData(const int nID)
{
	if (nID + 1 <= m_oClipboardData.GetSize() && m_oClipboardData[nID]) {
		return m_oClipboardData[nID]->IsEmpty();
	}
	return FALSE;
}

void CCommands::PrintFunctionName(int (*nFunctionPointer)())
{
	for (int nCommandID = 1; nCommandID < MAX_COMMAND; ++nCommandID) {
		if (Commands[nCommandID].fCommand == nFunctionPointer) {
			CUtils::Log(_T("m_LastCommand: %s"), Commands[nCommandID].szCommandName);
		}
	}
}

int CCommands::CapsLock()
{
	ClearNumericArgument();
	Kdu(VK_CAPITAL);
	return Reset(GOTO_HOOK);
}

int CCommands::Underscore()
{
	ClearNumericArgument();
	if (CXkeymacsDll::Is106Keyboard()) {
		SdKduSu(0xE2);	// VK_OEM_102	Windows 2000/XP: Either the angle bracket key or the backslash key on the RT 102-key keyboard
		Su();
	} else {
		SdKduSu(0xBD);	// VK_OEM_MINUS	Windows 2000/XP: For any country/region, the '-' key
		Su();
	}
	return Reset(GOTO_HOOK);
}

int CCommands::RollUpUnroll()
{
	return Maximize(ROLL_UP_UNROLL);
}

int CCommands::Retop()
{
	RECT ClientRect = {'\0'};
	GetClientRect(GetFocus(), &ClientRect);

	for (int i = 0; i < 0x10000; ++i) {
		POINT CaretPos = {'\0'};
		GetCaretPos(&CaretPos);
		if (ClientRect.top + 10 < CaretPos.y) {
			SendMessage(GetFocus(), WM_VSCROLL, SB_LINEDOWN, NULL);
		} else {
			return Reset(GOTO_HOOK);
		}
	}

	Kdu(VK_UP, VK_DOWN);
	return Reset(GOTO_HOOK);
}

int CCommands::OneShotModifierShift()
{
	// dummy
	ASSERT(0);
	static int i = 0;
	++i;
	return Reset(GOTO_HOOK);
}

int CCommands::OneShotModifierShiftRepeat()
{
	// dummy
	ASSERT(0);
	static int i = 0;
	++i;
	return Reset(GOTO_HOOK);
}

int CCommands::OneShotModifierCtrl()
{
	// dummy
	ASSERT(0);
	static int i = 0;
	++i;
	return Reset(GOTO_HOOK);
}

int CCommands::OneShotModifierCtrlRepeat()
{
	// dummy
	ASSERT(0);
	static int i = 0;
	++i;
	return Reset(GOTO_HOOK);
}

int CCommands::OneShotModifierAlt()
{
	// dummy
	ASSERT(0);
	static int i = 0;
	++i;
	return Reset(GOTO_HOOK);
}

int CCommands::OneShotModifierAltRepeat()
{
	// dummy
	ASSERT(0);
	static int i = 0;
	++i;
	return Reset(GOTO_HOOK);
}

int CCommands::CycleItems()
{
	ClearNumericArgument();
	AdKduAu(VK_ESCAPE);
	return Reset(GOTO_HOOK);
}

int CCommands::CycleItemsInversely()
{
	ClearNumericArgument();
	AdSdKduSuAu(VK_ESCAPE);
	return Reset(GOTO_HOOK);
}

// M-t
int CCommands::TransposeWords()
{
	static int nStep = 0;
	switch (nStep) {
	case 0:
		nStep = 0;
//		CUtils::Log(_T("M-t: 0"));
		CdKduCu(VK_LEFT);
		CaptureClipboardData();
		nStep = 1;
		return Reset(GOTO_RECURSIVE);
	case 1:
		nStep = 0;
//		CUtils::Log(_T("M-t: 1"));
		CdSdKduSuCu(VK_RIGHT);
		CdKduCu('X');
		nStep = 2;
		return Reset(GOTO_RECURSIVE);
	case 2:
		nStep = 0;
//		CUtils::Log(_T("M-t: 2"));
		Su();
		CdKduCu(VK_RIGHT);
		CdKduCu('V');
		Kdu(VK_LEFT);
		nStep = 3;
		return Reset(GOTO_RECURSIVE);
	case 3:
		nStep = 0;
//		CUtils::Log(_T("M-t: 3"));
		RestoreClipboardData();
		break;
	}
	return Reset(GOTO_HOOK);
}

LRESULT CCommands::VScroll(UINT nSBCode, const int nTimes)
{
	if (CUtils::IsMicrosoftWord()
	 || CUtils::IsThunderbird()) {
		RECT ClientRect = {'\0'};
		GetClientRect(GetFocus(), &ClientRect);

		POINT righttop = {ClientRect.right, ClientRect.top};
		ClientToScreen(GetFocus(), &righttop);
//		CUtils::Log(_T("righttop: x = %d, y = %d"), righttop.x, righttop.y);

		POINT rightbottom = {ClientRect.right, ClientRect.bottom};
		ClientToScreen(GetFocus(), &rightbottom);
//		CUtils::Log(_T("rightbottom: x = %d, y = %d"), rightbottom.x, rightbottom.y);

//		POINT CursorPos = {'\0'};
//		GetCursorPos(&CursorPos);
//		CUtils::Log(_T("VScroll: x = %d, y = %d"), CursorPos.x, CursorPos.y);

		POINT scrollpos = {'\0'};

		switch (nSBCode) {
		case SB_LINEUP:
			if (CUtils::IsMicrosoftWord()) {
				scrollpos.x = righttop.x + 8;	// 0 to 16
				scrollpos.y = righttop.y - 9;	// 1 to 17
			} else if (CUtils::IsThunderbird()) {
				scrollpos.x = righttop.x - 10;	// 2 to 18
				scrollpos.y = righttop.y + 8;	// 0 to 16
			}
			break;
		case SB_LINEDOWN:
			if (CUtils::IsMicrosoftWord()) {
				scrollpos.x = rightbottom.x + 8;	//  0 to 16
				scrollpos.y = rightbottom.y - 60;	// 52 to 68
			} else if (CUtils::IsThunderbird()) {
				scrollpos.x = rightbottom.x - 10;	// 2 to 18
				scrollpos.y = rightbottom.y - 9;	// 1 to 17
			}
			break;
		default:
			return 1;
		}

		int i = 0;
		for (i = 0; i < nTimes; ++i) {
			Click(&scrollpos);
		}

		return 0;
	}

	return SendMessage(GetFocus(), WM_VSCROLL, nSBCode, NULL);
}

// M-q
int CCommands::FillParagraph()
{
	static const int nFillColumn = 70; // tmp
	static CString szClipboardText;
	static int nOldLineLength = 0;

	ClearNumericArgument();	// tmp

	static int nStep = 0;
	switch (nStep) {
	case 0:		// Clear Selection
//		CUtils::Log(_T("M-q: 0"));
		nStep = 0;

		CaptureClipboardData();
		if (m_bSetMark) {
			m_bSetMark = FALSE;
		}
		Kdu(VK_HOME);
		nOldLineLength = 0;

		nStep = 1;
		return Reset(GOTO_RECURSIVE);
	case 1:		// Copy previous line
//		CUtils::Log(_T("M-q: 1"));
		nStep = 0;

		if (!CopyPreviousLine()) {
			return Reset(GOTO_DO_NOTHING);
		}

		nStep = 2;
		return Reset(GOTO_RECURSIVE);
	case 2:		// Check previous line to find TOP (top of paragraph)
//		CUtils::Log(_T("M-q: 2"));
		nStep = 0;

		CUtils::GetClipboardText(&szClipboardText);
//		CUtils::Log(_T("M-q: 2-1: _%s_"), szClipboardText);
		if (szClipboardText.IsEmpty()) {
			// TOF
//			CUtils::Log(_T("M-q: 2: TOF"));
			Kdu(VK_HOME);
			nStep = 3;
		} else if (szClipboardText == _T("\r\n")
				|| szClipboardText == _T("\r")
				|| szClipboardText == _T("\n")) {
			// blank line i.e. top of paragraph
			// UNIX: LF "\n"
			// WINDOWS: CR LF "\r\n"
			// MAC: CR "\r"
//			CUtils::Log(_T("M-q: 2: TOP"));
			Kdu(VK_DOWN);
			nStep = 3;
		} else {
			// middle of paragraph
//			CUtils::Log(_T("M-q: 2: MOP_%d_"), szClipboardText.GetLength());
//			CUtils::Log(_T("M-q: 2: MOP_%c(%d)%c(%d)"), szClipboardText.GetAt(0), szClipboardText.GetAt(0), szClipboardText.GetAt(1), szClipboardText.GetAt(1));
			Kdu(VK_HOME);
			nStep = 1;
		}

		return Reset(GOTO_RECURSIVE);
	case 3:		// Go to EOL to copy a current line at the next step
//		CUtils::Log(_T("M-q: 3"));
		nStep = 0;

		Kdu(VK_END);

		nStep = 4;
		return Reset(GOTO_RECURSIVE);
	case 4:		// Copy a current line
//		CUtils::Log(_T("M-q: 4"));
		nStep = 0;

		if (!CopyCurrentLine()) {
			return Reset(GOTO_DO_NOTHING);
		}

		nStep = 5;
		return Reset(GOTO_RECURSIVE);
	case 5:		// Just release shift
//		CUtils::Log(_T("M-q: 5"));
		nStep = 0;

		ReleaseKey(VK_SHIFT);

		nStep = 6;
		return Reset(GOTO_RECURSIVE);
	case 6:		// Check current line and fill paragraph
//		CUtils::Log(_T("M-q: 6"));
		nStep = 0;

		CUtils::GetClipboardText(&szClipboardText);
		if (szClipboardText.GetLength() == nOldLineLength + 1) {
//			CUtils::Log(_T("M-q: 6-1 Finished"));
			Kdu(VK_END, VK_BACK, VK_RETURN);
			nStep = 8;
		} else if (szClipboardText.GetLength() < nFillColumn) {
//			CUtils::Log(_T("M-q: 6-2 This line is too small"));
			Kdu(VK_END, VK_SPACE, VK_DELETE, VK_HOME);
			nStep = 3;
		} else {
//			CUtils::Log(_T("M-q: 6-3 %d_%s_%d"), szClipboardText.Left(nFillColumn).GetLength(), szClipboardText.Left(nFillColumn), szClipboardText.Left(nFillColumn).ReverseFind(_T(' ')));
			const int offset = szClipboardText.Left(nFillColumn).ReverseFind(_T(' '));
			if (offset == 0) {
//				CUtils::Log(_T("M-q: 6-3-1 TOL is space"));
				Kdu(VK_DELETE);
			} else if (0 < offset) {
//				CUtils::Log(_T("M-q: 6-3-2"));
				ReleaseKey(VK_MENU); // FIXME
				for (int i = 0; i < offset; ++i) {
					Kdu(VK_RIGHT);
				}
				nStep = 7;
				return Reset(GOTO_RECURSIVE);
			} else {
//				CUtils::Log(_T("M-q: 6-3-3 No space in first fill-column"));
				const int offset = szClipboardText.Find(_T(' '));
				if (offset < 0) {
					Kdu(VK_HOME, VK_DOWN);
				} else {
					ReleaseKey(VK_MENU); // FIXME
					for (int i = 0; i < offset; ++i) {
						Kdu(VK_RIGHT);
					}
					nStep = 7;
					return Reset(GOTO_RECURSIVE);
				}
			}
			nStep = 3;
		}
		nOldLineLength = szClipboardText.GetLength();

		return Reset(GOTO_RECURSIVE);
	case 7:		// Linefeed
//		CUtils::Log(_T("M-q: 7"));
		nStep = 0;

		Kdu(VK_RETURN, VK_DELETE);

		nStep = 3;
		return Reset(GOTO_RECURSIVE);
	case 8:		// Restore clipboard data
//		CUtils::Log(_T("M-q: 8"));
		nStep = 0;

		RestoreClipboardData();

		return Reset(GOTO_HOOK);
	}
	return Reset(GOTO_HOOK);
}

// M-l
int CCommands::DowncaseWord()
{
	return CaseWord(DOWNCASE);
}

// M-u
int CCommands::UpcaseWord()
{
	return CaseWord(UPCASE);
}

// M-c
int CCommands::CapitalizeWord()
{
	return CaseWord(CAPITALIZE);
}

int CCommands::CaseWord(CASE_WORD nCase)
{
	ClearNumericArgument();	// tmp

	// Do not try to do these command at once.
	// Clipboard has old data till you go out this function.
	static GOTO nNext = GOTO_HOOK;
	static int nStep = 0;
	static int nLastWhiteSpace = 0;
	static BOOL bFirstCharacter = TRUE;
	switch (nStep) {
	case 0:		// Clear Selection
		nLastWhiteSpace = 0;

		if (m_bSetMark) {
			m_bSetMark = FALSE;
			Kdu(VK_RIGHT, VK_LEFT);
			nStep = 1;
			return Reset(GOTO_RECURSIVE);
		} else {
			// Do nothing. Go to case 1:
		}
	case 1:		// Get back character to confirm the cursor is at TOF or not.
		nStep = 0;

		CaptureClipboardData();

		if (!CutFollowingWord()) {
			return Reset(GOTO_DO_NOTHING);
		}

		nStep = 2;
		return Reset(GOTO_RECURSIVE);
	case 2:
		{
			nStep = 0;

			CString szClipboardText;
			CUtils::GetClipboardText(&szClipboardText);

			CString szTrimmed = szClipboardText;
			szTrimmed.TrimLeft();
			if (szClipboardText.IsEmpty()) {
				nNext = GOTO_HOOK;
				RestoreClipboardData();
				return Reset(GOTO_HOOK);
			} else if (szTrimmed.IsEmpty()) {
				if (nNext == GOTO_HOOK) {
					nNext = GOTO_RECURSIVE;
				} else {
					if (CUtils::IsHidemaru()) {
						nLastWhiteSpace = 1;
					}
					nNext = GOTO_HOOK;
				}
				CUtils::SetClipboardText(&szClipboardText);
				nStep = 3;
				return Reset(GOTO_RECURSIVE);
			} else if (CUtils::IsHidemaru()) {
				nNext = GOTO_RECURSIVE;
			} else {
				nNext = GOTO_HOOK;
			}

			switch (nCase) {
			case DOWNCASE:
				szClipboardText.MakeLower();
				break;
			case UPCASE:
				szClipboardText.MakeUpper();
				break;
			case CAPITALIZE:
				{
					for (int i = 0; i < szClipboardText.GetLength(); ++i) {
						if (_istspace(szClipboardText.GetAt(i))) {
							continue;
						} else if (bFirstCharacter) {
							bFirstCharacter = FALSE;
							if (_istlower(szClipboardText.GetAt(i))) {
								szClipboardText.SetAt(i, (TCHAR) toupper(szClipboardText.GetAt(i)));
							}
						} else {
							if (_istupper(szClipboardText.GetAt(i))) {
								szClipboardText.SetAt(i, (TCHAR) tolower(szClipboardText.GetAt(i)));
							}
						}
					}
					break;
				}
			default:
				ASSERT(0);
			}

			CUtils::SetClipboardText(&szClipboardText);

			nLastWhiteSpace = 0;
			for (int j = szClipboardText.GetLength() - 1; !szClipboardText.IsEmpty() && 0 <= j; --j) {
				if (_istspace(szClipboardText.GetAt(j))) {
					++nLastWhiteSpace;
				} else {
					break;
				}
			}

			nStep = 3;
			return Reset(GOTO_RECURSIVE);
		}
	case 3:
		{
			nStep = 0;

			Paste();

			for (int i = 0; i < nLastWhiteSpace; ++i) {
				Kdu(VK_LEFT);
			}

			nStep = 4;
			return Reset(GOTO_RECURSIVE);
		}
	case 4:
		nStep = 0;

		RestoreClipboardData();

		if (nNext == GOTO_HOOK) {
			bFirstCharacter = TRUE;
		}
		return Reset(nNext);
	}

	return Reset(GOTO_HOOK);
}

BOOL CCommands::CutFollowingWord()
{
	if (!CUtils::OpenClipboard()) {
		ASSERT(0);
		return FALSE;
	}
	if (!EmptyClipboard()) {
		ASSERT(0);
		return FALSE;
	}
	if (!CloseClipboard()) {
		ASSERT(0);
		return FALSE;
	}

	if (CUtils::IsHidemaru()) {
		// On Hidemaru, Cut_() does not work well after CdSdKduSuCu(). I do not know why.
		SdKduSu(VK_RIGHT);
	} else {
		CdSdKduSuCu(VK_RIGHT);
	}
	Cut_();

	return TRUE;
}

// M-x: execute-extended-command
int CCommands::ExecuteExtendedCommand()
{
//	CUtils::Log(_T("ExecuteExtendedCommand"));
	bM_x(TRUE);
	return Reset(GOTO_HOOKX);
}

CString CCommands::GetCommandName(int nCommandID)
{
	CString szCommandName(Commands[nCommandID].szCommandName);
	return szCommandName;
}

int CCommands::GetCategoryID(int nCommandID)
{
	return Commands[nCommandID].nCategoryID;
}

int CCommands::GetDescriptionID(int nCommandID)
{
	return Commands[nCommandID].nDescriptionID;
}

int CCommands::GetToolTipID(int nCommandID)
{
	return Commands[nCommandID].nToolTipID;
}

int CCommands::GetDefaultCommandType(int nCommandID, int nIndex)
{
	if (nCommandID < 0 || sizeof(Commands) / sizeof(Commands[0]) <= nCommandID
	 || nIndex < 0 || sizeof(Commands[nCommandID].keybind) / sizeof(Commands[nCommandID].keybind[0]) <= nIndex) {
		ASSERT(0);
		return NONE;
	}

	int nCommandType	= Commands[nCommandID].keybind[nIndex].nCommandType;
	int bVk				= Commands[nCommandID].keybind[nIndex].bVk;

	if (CXkeymacsDll::Is106Keyboard()) {
		if (nCommandType & SHIFT) {	// Shift
			switch (bVk) {
			case '2':
			case '6':
			case 0xBA:		// VK_OEM_1		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the ';:' key
				nCommandType &= ~SHIFT;
				break;
			default:
				break;
			}
		} else {					// Normal
			switch (bVk) {
			case 0xBB:		// VK_OEM_PLUS	Windows 2000/XP: For any country/region, the '+' key
			case 0xC0:		// VK_OEM_3		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the '`~' key
			case 0xDE:		// VK_OEM_7		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
				nCommandType |= SHIFT;
				break;
			default:
				break;
			}
		}
	}

	return nCommandType;
}

int CCommands::GetDefaultCommandKey(int nCommandID, int nIndex)
{
	if (nCommandID < 0 || sizeof(Commands) / sizeof(Commands[0]) <= nCommandID
	 || nIndex < 0 || sizeof(Commands[nCommandID].keybind) / sizeof(Commands[nCommandID].keybind[0]) <= nIndex) {
		ASSERT(0);
		return 0;
	}

	int nCommandType	= Commands[nCommandID].keybind[nIndex].nCommandType;
	int bVk				= Commands[nCommandID].keybind[nIndex].bVk;

	if (CXkeymacsDll::Is106Keyboard()) {
		if (nCommandType & SHIFT) {	// Shift
			switch (bVk) {
			case '0':
				bVk = '9';
				break;
			case '2':
				bVk = 0xC0;	// VK_OEM_3		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the '`~' key
				break;
			case '6':
				bVk = 0xDE;	// VK_OEM_7		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
				break;
			case '7':
				bVk = '6';
				break;
			case '8':
				bVk = 0xBA;	// VK_OEM_1		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the ';:' key
				break;
			case '9':
				bVk = '8';
				break;
			case 0xBD:		// VK_OEM_MINUS	Windows 2000/XP: For any country/region, the '-' key
				bVk = 0xE2;	// VK_OEM_102	Windows 2000/XP: Either the angle bracket key or the backslash key on the RT 102-key keyboard
				break;
			case 0xC0:
				bVk = 0xDE;	// VK_OEM_7		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
				break;
			case 0xDE:		// VK_OEM_7		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
				bVk = '2';
				break;
			default:
				break;
			}
		} else {					// Normal
			switch (bVk) {
			case 0xBA:		// VK_OEM_1		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the ';:' key
				bVk = 0xBB;	// VK_OEM_PLUS	Windows 2000/XP: For any country/region, the '+' key
				break;
			case 0xBB:		// VK_OEM_PLUS	Windows 2000/XP: For any country/region, the '+' key
				bVk = 0xBD;	// VK_OEM_MINUS	Windows 2000/XP: For any country/region, the '-' key
				break;
			case 0xDE:		// VK_OEM_7		Used for miscellaneous characters; it can vary by keyboard. 
							//				Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
				bVk = '7';
				break;
			default:
				break;
			}
		}
	}

	return bVk;
}

int CCommands::GetDefaultControlID(int nCommandID, int nIndex)
{
	if (nCommandID < 0 || sizeof(Commands) / sizeof(Commands[0]) <= nCommandID
	 || nIndex < 0 || sizeof(Commands[nCommandID].keybind) / sizeof(Commands[nCommandID].keybind[0]) <= nIndex) {
		ASSERT(0);
		return 0;
	}

	return Commands[nCommandID].keybind[nIndex].nControlID;
}
