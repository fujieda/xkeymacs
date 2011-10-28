#include "KeyString.h"
#include "../xkeymacsdll/defs.h"

static const struct {
	UINT Type;
	LPCTSTR Name;
} Types[] = {
	{CONTROLX, _T("Ctrl+X ")},
	{CONTROL, _T("Ctrl+")},
	{META, _T("Meta+")},
	{SHIFT, _T("Shift+")},
};

static LPCSTR Keys[] = {
	_T(""), // 0
	_T("Left mouse button"), // VK_LBUTTON
	_T("Right mouse button"), // VK_RBUTTON
	_T("Break"), // VK_CANCEL
	_T("Middle mouse button"), // VK_MBUTTON
	_T("X1 mouse button"), // VK_XBUTTON1
	_T("X2 mouse button"), // VK_XBUTTON2
	_T("Undefined"), // 0x07
	_T("Backspace"), // VK_BACK
	_T("Tab"), // VK_TAB
	_T("Reserved"), // 0x0a
	_T("Reserved"), // 0x0b
	_T("Clear"), // VK_CLEAR
	_T("Enter"), // VK_RETURN
	_T("Undefined"), // 0x0e
	_T("Undefined"), // 0x0f
	_T("Shift"), // VK_SHIFT
	_T("Ctrl"), // VK_CONTROL
	_T("Alt"), // VK_MENU
	_T("Pause"), // VK_PAUSE
	_T("Caps Lock"), // VK_CAPITAL
	_T("Kana"),	// VK_KANA {VK_HANGUEL, "Hanguel"},	{VK_HANGUL, "Hangul"} 
	_T("Undefined"), // 0x16
	_T("Junja"), // VK_JUNJA
	_T("Final"), // VK_FINAL
	_T("Kanji"), //VK_KANJI {VK_HANJA, "Hanja"}
	_T("Undefined"), // 0x1a
	_T("Esc"), // VK_ESCAPE
	_T("Convert"), // VK_CONVERT ïœä∑
	_T("Nonconvert"), // VK_NONCONVERT ñ≥ïœä∑
	_T("Accept"), // VK_ACCEPT
	_T("Mode change"), // VK_MODECHANGE
	_T("Space"), // VK_SPACE
	_T("Page Up"), // VK_PRIOR
	_T("Page Down"), // VK_NEXT
	_T("End"), // VK_END
	_T("Home"), // VK_HOME
	_T("Left"), // VK_LEFT
	_T("Up"), // VK_UP
	_T("Right"), // VK_RIGHT
	_T("Down"), // VK_DOWN
	_T("Select"), // VK_SELECT
	_T("Print"), // VK_PRINT
	_T("Execute"), // VK_EXECUTE
	_T("Print Screen"), // VK_SNAPSHOT
	_T("Ins"), // VK_INSERT
	_T("Del"), // VK_DELETE
	_T("Help"), // VK_HELP
	_T("0"), // '0'
	_T("1"), // '1'
	_T("2"), // '2'
	_T("3"), // '3'
	_T("4"), // '4'
	_T("5"), // '5'
	_T("6"), // '6'
	_T("7"), // '7'
	_T("8"), // '8'
	_T("9"), // '9'
	_T("Undefined"), // 0x3a
	_T("Undefined"), // 0x3b
	_T("Undefined"), // 0x3c
	_T("Undefined"), // 0x3d
	_T("Undefined"), // 0x3e
	_T("Undefined"), // 0x3f
	_T("Undefined"), // 0x40
	_T("A"), // 'A'
	_T("B"), // 'B'
	_T("C"), // 'C'
	_T("D"), // 'D'
	_T("E"), // 'E'
	_T("F"), // 'F'
	_T("G"), // 'G'
	_T("H"), // 'H'
	_T("I"), // 'I'
	_T("J"), // 'J'
	_T("K"), // 'K'
	_T("L"), // 'L'
	_T("M"), // 'M'
	_T("N"), // 'N'
	_T("O"), // 'O'
	_T("P"), // 'P'
	_T("Q"), // 'Q'
	_T("R"), // 'R'
	_T("S"), // 'S'
	_T("T"), // 'T'
	_T("U"), // 'U'
	_T("V"), // 'V'
	_T("W"), // 'W'
	_T("X"), // 'X'
	_T("Y"), // 'Y'
	_T("Z"), // 'Z'
	_T("Left Windows"), // VK_LWIN
	_T("Right Windows"), // VK_RWIN
	_T("Application"), // VK_APPS
	_T("Reserved"), // 0x5e
	_T("Sleep"), // VK_SLEEP
	_T("Num 0"), // VK_NUMPAD0
	_T("Num 1"), // VK_NUMPAD1
	_T("Num 2"), // VK_NUMPAD2
	_T("Num 3"), // VK_NUMPAD3
	_T("Num 4"), // VK_NUMPAD4
	_T("Num 5"), // VK_NUMPAD5
	_T("Num 6"), // VK_NUMPAD6
	_T("Num 7"), // VK_NUMPAD7
	_T("Num 8"), // VK_NUMPAD8
	_T("Num 9"), // VK_NUMPAD9
	_T("Num *"), // VK_MULTIPLY
	_T("Num +"), // VK_ADD
	_T("Separator"), // VK_SEPARATOR
	_T("Num -"), // VK_SUBTRACT
	_T("Num ."), // VK_DECIMAL
	_T("Num /"), // VK_DIVIDE
	_T("F1"), // VK_F1
	_T("F2"), // VK_F2
	_T("F3"), // VK_F3
	_T("F4"), // VK_F4
	_T("F5"), // VK_F5
	_T("F6"), // VK_F6
	_T("F7"), // VK_F7
	_T("F8"), // VK_F8
	_T("F9"), // VK_F9
	_T("F10"), // VK_F10
	_T("F11"), // VK_F11
	_T("F12"), // VK_F12
	_T("F13"), // VK_F13
	_T("F14"), // VK_F14
	_T("F15"), // VK_F15
	_T("F16"), // VK_F16
	_T("F17"), // VK_F17
	_T("F18"), // VK_F18
	_T("F19"), // VK_F19
	_T("F20"), // VK_F20
	_T("F21"), // VK_F21
	_T("F22"), // VK_F22
	_T("F23"), // VK_F23
	_T("F24"), // VK_F24
	_T("Unassigned"), // 0x88
	_T("Unassigned"), // 0x89
	_T("Unassigned"), // 0x8a
	_T("Unassigned"), // 0x8b
	_T("Unassigned"), // 0x8c
	_T("Unassigned"), // 0x8d
	_T("Unassigned"), // 0x8e
	_T("Unassigned"), // 0x8f
	_T("Num Lock"), // VK_NUMLOCK
	_T("Scroll Lock"), // VK_SCROLL
	_T("OEM specific"), // 0x92
	_T("OEM specific"), // 0x93
	_T("OEM specific"), // 0x94
	_T("OEM specific"), // 0x95
	_T("OEM specific"), // 0x96
	_T("Unassigned"), // 0x97
	_T("Unassigned"), // 0x98
	_T("Unassigned"), // 0x99
	_T("Unassigned"), // 0x9a
	_T("Unassigned"), // 0x9b
	_T("Unassigned"), // 0x9c
	_T("Unassigned"), // 0x9d
	_T("Unassigned"), // 0x9e
	_T("Unassigned"), // 0x9f
	_T("Left Shift"), // VK_LSHIFT
	_T("Right Shift"), // VK_RSHIFT
	_T("Left Ctrl"), // VK_LCONTROL
	_T("Right Ctrl"), // VK_RCONTROL
	_T("Left Alt"), // VK_LMENU
	_T("Right Alt"), // VK_RMENU
	_T("Browser Back"), // VK_BROWSER_BACK
	_T("Browser Forward"), // VK_BROWSER_FORWARD
	_T("Browser Refresh"), // VK_BROWSER_REFRESH
	_T("Browser Stop"), // VK_BROWSER_STOP
	_T("Browser Search"), // VK_BROWSER_SEARCH
	_T("Browser Favorites"), // VK_BROWSER_FAVORITES
	_T("Browser Start"), // VK_BROWSER_HOME
	_T("Volume Mute"), // VK_VOLUME_MUTE
	_T("Volume Down"), // VK_VOLUME_DOWN
	_T("Volume Up"), // VK_VOLUME_UP
	_T("Next Track"), // VK_MEDIA_NEXT_TRACK
	_T("Previous Track"), // VK_MEDIA_PREV_TRACK
	_T("Stop Media"), // VK_MEDIA_STOP
	_T("Play/Pause Media"), // VK_MEDIA_PLAY_PAUSE
	_T("Start Mail"), // VK_LAUNCH_MAIL
	_T("Select Media"), // VK_LAUNCH_MEDIA_SELECT
	_T("Start Application 1"), // VK_LAUNCH_APP1
	_T("Start Application 2"), // VK_LAUNCH_APP2
	_T("Reserved"), // 0xb8
	_T("Reserved"), // 0xb9
	_T(";"), // VK_OEM_1 _T(":") for Japanese keyboard
	_T("+"), // VK_OEM_PLUS _T(";") for Japanese keyboard
	_T(","), // VK_OEM_COMMA
	_T("-"), // VK_OEM_MINUS
	_T("."), // VK_OEM_PERIOD
	_T("/"), // VK_OEM_2
	_T("`"), // VK_OEM_3 _T("@") for Japanese keyboard
	_T("Reserved"), // 0xc1
	_T("Reserved"), // 0xc2
	_T("Reserved"), // 0xc3
	_T("Reserved"), // 0xc4
	_T("Reserved"), // 0xc5
	_T("Reserved"), // 0xc6
	_T("Reserved"), // 0xc7
	_T("Reserved"), // 0xc8
	_T("Reserved"), // 0xc9
	_T("Reserved"), // 0xca
	_T("Reserved"), // 0xcb
	_T("Reserved"), // 0xcc
	_T("Reserved"), // 0xcd
	_T("Reserved"), // 0xce
	_T("Reserved"), // 0xcf
	_T("Reserved"), // 0xd0
	_T("Reserved"), // 0xd1
	_T("Reserved"), // 0xd2
	_T("Reserved"), // 0xd3
	_T("Reserved"), // 0xd4
	_T("Reserved"), // 0xd5
	_T("Reserved"), // 0xd6
	_T("Reserved"), // 0xd7
	_T("Unassigned"), // 0xd8
	_T("Unassigned"), // 0xd9
	_T("Unassigned"), // 0xda
	_T("["), // VK_OEM_4
	_T("Backslash"), // VK_OEM_5
	_T("]"), // VK_OEM_6
	_T("'"), // VK_OEM_7
	_T("OEM specific"), // VK_OEM_8
	_T("Reserved"), // 0xe0
	_T("OEM specific"), // 0xe1
	_T("Backslash for 106 keyboard"), // VK_OEM_102
	_T("OEM specific"), // 0xe3
	_T("OEM specific"), // 0xe4
	_T("Process"), // VK_PROCESSKEY
	_T("OEM specific"), // 0xe6
	_T("Packet"), // VK_PACKET
	_T("Unassigned"), // 0xe8
	_T("OEM specific"), // 0xe9
	_T("OEM specific"), // 0xea
	_T("OEM specific"), // 0xeb
	_T("OEM specific"), // 0xec
	_T("OEM specific"), // 0xed
	_T("OEM specific"), // 0xee
	_T("OEM specific"), // 0xef
	_T("Eisu"), // 0xf0 // âpêî
	_T("OEM specific"), // 0xf1
	_T("Hiragana"), // 0xf2 Ç–ÇÁÇ™Ç»
	_T("Hankaku/Zenkaku 0xf3"), // 0xf3 "îºäp/ëSäp"
	_T("Hankaku/Zenkaku 0xf4"), // 0xf4 "îºäp/ëSäp"
	_T("OEM specific"), // 0xf5
	_T("Attn"), // VK_ATTN
	_T("CrSel"), // VK_CRSEL
	_T("ExSel"), // VK_EXSEL
	_T("Erace EOF"), // VK_EREOF
	_T("Play"), // VK_PLAY
	_T("Zoom"), // VK_ZOOM
	_T("Noname"), // VK_NONAME
	_T("PA1"), // VK_PA1
	_T("OEM Clear"), // VK_OEM_CLEAR
	_T("Fn"), // 0xff
};

KeyString::KeyString(bool is106Keyboard)
{
	if (!is106Keyboard)
		return;
	Keys[0xba] = _T(":");
	Keys[0xbb] = _T(";");
	Keys[0xc0] = _T("@");
	Keys[0xde] = _T("^");
}

CString KeyString::ToString(int type, int key)
{
	CString str;
	for (int i = 0; i < _countof(Types); ++i)
		if ((type & Types[i].Type) != 0)
			str += Types[i].Name;
	str += Keys[key];
	return str;
}

void KeyString::ToKey(LPCTSTR str, int& type, int& key)
{
	CString s(str);
	type = NONE;
	for (int i = 0; i < _countof(Types); ++i)
		if (s.Find(Types[i].Name) == 0) {
			type |= Types[i].Type;
			s.Delete(0, static_cast<int>(_tcslen(Types[i].Name)));
		}
	key = 0;
	for (int i = 1; i < 256; ++i)
		if (s == Keys[i]) {
			key = i;
			break;
		}
}
