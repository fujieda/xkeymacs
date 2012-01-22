#include "FuncDefs.h"
#include "../xkeymacsdll/CmdTable.h"
#include "../xkeymacsdll/xkeymacsdll.h"

int FuncDefs::m_NumOfDefs;
TCHAR FuncDefs::m_Names[MAX_FUNCTION][SUB_KEY_NAME_LENGTH];
TCHAR FuncDefs::m_StrDefs[MAX_FUNCTION][MAX_DEFINITION];
KeyBind FuncDefs::m_Defs[MAX_FUNCTION][MAX_DEFINITION];

void FuncDefs::Load()
{
	Clear();
	LoadFiles();
	ParseDefs();
}

int FuncDefs::GetID(LPCTSTR name)
{
	for (int i = 0; i < m_NumOfDefs; ++i)
		if (!_tcscmp(m_Names[i], name))
			return i;
	return -1;
}

LPCTSTR FuncDefs::GetName(int id)
{
	return m_Names[id];
}

int FuncDefs::GetNumOfDefs()
{
	return m_NumOfDefs;
}

LPCTSTR FuncDefs::GetStrDef(int id)
{
	return m_StrDefs[id];
}

KeyBind (*FuncDefs::GetDefs())[MAX_DEFINITION]
{
	return m_Defs;
}

void FuncDefs::Clear()
{
	m_NumOfDefs = 0;
	ZeroMemory(m_Names, sizeof(m_Names));
	ZeroMemory(m_StrDefs, sizeof(m_StrDefs));
	ZeroMemory(m_Defs, sizeof(m_Defs));
}

void FuncDefs::LoadFiles()
{
	const TCHAR ext[] = _T("xkeymacs");
	TCHAR mfname[MAX_PATH], drive[_MAX_DRIVE], dir[_MAX_DIR], etc[_MAX_DIR];
	if (GetModuleFileName(NULL, mfname, MAX_PATH) == 0) // no such case
		return;
	_tsplitpath_s(mfname, drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, 0, NULL, 0);
	_tmakepath_s(etc, drive, dir, _T("etc"), NULL);
	for (int i = 0; i < 3; i++) {
		TCHAR fname[_MAX_FNAME];
		TCHAR file[MAX_PATH];
		switch (i) {
		case 0:
			if (!GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGLANGUAGE, fname, _MAX_FNAME))
				continue;
			MoveOldFile(drive, dir, fname, ext);
			_tmakepath_s(file, NULL, etc, fname, ext);
			LoadFile(file);
			break;
		case 1:
			_tcscpy_s(fname, _T("dot"));
			MoveOldFile(drive, dir, fname, ext);
			_tmakepath_s(file, NULL, etc, fname, ext);
			LoadFile(file);
			break;
		case 2:
			TCHAR appdata[MAX_PATH];
			if (!SHGetSpecialFolderPath(NULL, appdata, CSIDL_APPDATA, TRUE))
				continue;
			_tmakepath_s(file, NULL, appdata, fname, ext);
			LoadFile(file);
			break;
		}
	}
}

// This function moves files in the installed directory used by 3.22 or older under 'etc'.
void FuncDefs::MoveOldFile(LPCTSTR drive, LPCTSTR dir, LPCTSTR fname, LPCTSTR ext)
{
	TCHAR old[MAX_PATH];
	_tmakepath_s(old, drive, dir, fname, ext);
	if (!PathFileExists(old))
		return;
	TCHAR etc[MAX_PATH];
	_tmakepath_s(etc, drive, dir, _T("etc"), NULL);
	if (!PathFileExists(etc) && !CreateDirectory(etc, NULL) ||
			!PathIsDirectory(etc)) // a normal file 'etc' exists
		return;
	TCHAR newpath[MAX_PATH];
	_tmakepath_s(newpath, NULL, etc, fname, ext);
	if (MoveFile(old, newpath))
		return;
	// move an old file under 'etc' as a backup file.
	TCHAR backup[_MAX_FNAME] = _T("~");
	_tcscat_s(backup, fname);
	_tmakepath_s(newpath, NULL, etc, backup, ext);
	MoveFile(old, newpath);
	return;
}

void FuncDefs::LoadFile(LPCTSTR file)
{
	FILE *fp;
	TCHAR buf[7 + SUB_KEY_NAME_LENGTH + 1 + MAX_DEFINITION + 3]; // (fset 'sym def)\n\0
	if (_tfopen_s(&fp, file, "r") != 0)
		return;
	while (_fgetts(buf, _countof(buf), fp)) {
		if (_tcslen(buf) < _tcslen(_T("(fset 'a [])")) || _tcsncmp(buf, _T("(fset "), 6))
			continue;
		LPTSTR space = _tcschr(buf + 7, _T(' '));
		if (!space)
			continue;
		*space = 0;
		LPCTSTR name = buf + 7;
		LPTSTR def = space + 1;
		if (_tcslen(name) > SUB_KEY_NAME_LENGTH - 1 || _tcslen(def) > MAX_DEFINITION - 1)
			continue;
		int id = GetID(name);
		if (id < 0) {
			if (m_NumOfDefs == MAX_FUNCTION || CheckOverride(name))
				continue;
			id = m_NumOfDefs++;
		}
		LPTSTR last = def + _tcslen(def) - 1;
		if (*last == _T('\n'))
			*last-- = 0;
		if (*last == _T(')'))
			*last = 0;
		_tcscpy_s(m_Names[id], name);
		_tcscpy_s(m_StrDefs[id], def);
	}
	fclose(fp);
}

bool FuncDefs::CheckOverride(LPCTSTR name)
{
	for (int i = 0; i < MAX_COMMAND; i++)
		if (!_tcscmp(name, CmdTable::Name(i)))
			return true;
	return false;
}

static const struct {
	LPCTSTR name;
	int id;
} Modifiers[] = {
//	{ _T("A-"), ALT },
	{ _T("C-"), CONTROL},
//	{ _T("H-"), HYPER },
	{ _T("M-"), META },
	{ _T("S-"), SHIFT },
//	{ _T("s-"), SUPER },
	{ _T("Ctrl+"), WIN_CTRL },
	{ _T("Alt+"), WIN_ALT },
	{ _T("Win+"), WIN_WIN },
};
static const int MAX_MODIFIER = _countof(Modifiers);

static const struct {
	BYTE bVk;
	LPCTSTR name;
} KeyNames[] = {
//	{ VK_LBUTTON,		_T("mouse-1") },				// does not work well
//	{ VK_RBUTTON,		_T("mouse-3") },				// does not work well
	{ VK_CANCEL,		_T("break") },
//	{ VK_MBUTTON,		_T("mouse-2") },				// does not work well
	{ VK_BACK,			_T("backspace") },
	{ VK_TAB,			_T("tab") },
	{ VK_RETURN,		_T("return") },
	{ VK_CAPITAL,		_T("capslock") },
	{ VK_KANA,			_T("kana") },
	{ VK_KANJI,			_T("kanji") },
	{ VK_ESCAPE,		_T("escape") },
	{ VK_CONVERT,		_T("convert") },
	{ VK_NONCONVERT,	_T("nonconvert") },
//	{ VK_SPACE,			_T("SPC") },					// [? ]
	{ VK_PRIOR,			_T("prior") },
	{ VK_NEXT,			_T("next") },
	{ VK_END,			_T("end") },
	{ VK_HOME,			_T("home") },
	{ VK_LEFT,			_T("left") },
	{ VK_UP,			_T("up") },
	{ VK_RIGHT,			_T("right") },
	{ VK_DOWN,			_T("down") },
	{ VK_SELECT,		_T("select") },
	{ VK_PRINT,			_T("print") },
	{ VK_EXECUTE,		_T("execute") },
	{ VK_SNAPSHOT,		_T("printscreen") },			// work as print
	{ VK_INSERT,		_T("insert") },
	{ VK_DELETE,		_T("delete") },
	{ VK_LWIN,			_T("lwindow") },
	{ VK_RWIN,			_T("rwindow") },
	{ VK_APPS,			_T("apps") },
	{ VK_SLEEP,			_T("sleep") },
	{ VK_NUMPAD0,		_T("kp-0") },
	{ VK_NUMPAD1,		_T("kp-1") },
	{ VK_NUMPAD2,		_T("kp-2") },
	{ VK_NUMPAD3,		_T("kp-3") },
	{ VK_NUMPAD4,		_T("kp-4") },
	{ VK_NUMPAD5,		_T("kp-5") },
	{ VK_NUMPAD6,		_T("kp-6") },
	{ VK_NUMPAD7,		_T("kp-7") },
	{ VK_NUMPAD8,		_T("kp-8") },
	{ VK_NUMPAD9,		_T("kp-9") },
	{ VK_MULTIPLY,		_T("kp-multiply") },
	{ VK_ADD,			_T("kp-add") },
	{ VK_SUBTRACT,		_T("kp-subtract") },
	{ VK_DECIMAL,		_T("kp-decimal") },
	{ VK_DIVIDE,		_T("kp-divide") },
//	{ VK_F1,			_T("f1") },						// FIXME
//	{ VK_F2,			_T("f2") },						// Move at the end of definition of function keys to keep away confliction f1/f2 and f1?/f2? by _tcsncmp() i.e. strncmp()
	{ VK_F3,			_T("f3") },
	{ VK_F4,			_T("f4") },
	{ VK_F5,			_T("f5") },
	{ VK_F6,			_T("f6") },
	{ VK_F7,			_T("f7") },
	{ VK_F8,			_T("f8") },
	{ VK_F9,			_T("f9") },
	{ VK_F10,			_T("f10") },
	{ VK_F11,			_T("f11") },
	{ VK_F12,			_T("f12") },
	{ VK_F13,			_T("f13") },
	{ VK_F14,			_T("f14") },
	{ VK_F15,			_T("f15") },
	{ VK_F16,			_T("f16") },
	{ VK_F17,			_T("f17") },
	{ VK_F18,			_T("f18") },
	{ VK_F19,			_T("f19") },
	{ VK_F20,			_T("f20") },
	{ VK_F21,			_T("f21") },
	{ VK_F22,			_T("f22") },
	{ VK_F23,			_T("f23") },
	{ VK_F24,			_T("f24") },
	{ VK_F1,			_T("f1") },
	{ VK_F2,			_T("f2") },
	{ VK_NUMLOCK,		_T("kp-numlock") },
	{ VK_SCROLL,		_T("scroll") },
	{ 0xa6,				_T("browser-back") },			// VK_BROWSER_BACK
	{ 0xa7,				_T("browser-forward") },		// VK_BROWSER_FORWARD
	{ 0xa8,				_T("browser-refresh") },		// VK_BROWSER_REFRESH
	{ 0xa9,				_T("browser-stop") },			// VK_BROWSER_STOP
	{ 0xaa,				_T("browser-search") },			// VK_BROWSER_SEARCH
	{ 0xab,				_T("browser-favorites") },		// VK_BROWSER_FAVORITES
	{ 0xac,				_T("browser-home") },			// VK_BROWSER_HOME
	{ 0xad,				_T("volume-mute") },			// VK_VOLUME_MUTE
	{ 0xae,				_T("volume-down") },			// VK_VOLUME_DOWN
	{ 0xaf,				_T("volume-up") },				// VK_VOLUME_UP
	{ 0xb0,				_T("media-next-track") },		// VK_MEDIA_NEXT_TRACK
	{ 0xb1,				_T("media-prev-track") },		// VK_MEDIA_PREV_TRACK
	{ 0xb2,				_T("media-stop") },				// VK_MEDIA_STOP
	{ 0xb3,				_T("media-play-pause") },		// VK_MEDIA_PLAY_PAUSE
	{ 0xb4,				_T("launch-mail") },			// VK_LAUNCH_MAIL
	{ 0xb5,				_T("launch-media-select") },	// VK_LAUNCH_MEDIA_SELECT
	{ 0xb6,				_T("launch-1") },				// VK_LAUNCH_APP1
	{ 0xb7,				_T("launch-2") },				// VK_LAUNCH_APP2
};
static const int MAX_KEYNAME = _countof(KeyNames);

void FuncDefs::ParseDefs()
{
	for (int i = 0; i < m_NumOfDefs; i++) {
		LPCTSTR def = m_StrDefs[i];
		KeyBind *p = m_Defs[i];
		LPCTSTR last = def + _tcslen(def) - 1;
		if (*def == _T('"') && *last == _T('"')) {
			++def; // skip '"'
			while (def < last)
				*p++ = ParseKey(def);
		} else if (*def == _T('[') && *last == _T(']')) {
			while (++def < last) { // skip '[', ']', and ' '
				if (*def == _T('?')) { // [?f ?o ?o]
					*p++ = ParseKey(++def);
					continue;
				}
				// [VK]
				for (int i = 0; i < MAX_KEYNAME; ++i) {
					size_t keylen = _tcslen(KeyNames[i].name);
					if (!_tcsncmp(def, KeyNames[i].name, keylen)) {
						KeyBind keybind = {NONE, KeyNames[i].bVk};
						*p++ = keybind;
						def += keylen;
						break;
					}
				}
			}
		}
	}
}

KeyBind FuncDefs::ParseKey(LPCTSTR& def)
{
	KeyBind keybind = {NONE};
	if (*def == _T('\\')) { // set modifiers
		++def;
	LOOP:
		for (int i = 0; i < MAX_MODIFIER; ++i) {
			size_t len = _tcslen(Modifiers[i].name);
			if (!_tcsncmp(def, Modifiers[i].name, len)) {
				keybind.nType |= Modifiers[i].id;
				def += len;
				goto LOOP;
			}
		}
	}
	int i = 0;
	for (; i < MAX_KEYNAME; ++i) {
		size_t len = _tcslen(KeyNames[i].name);
		if (!_tcsncmp(def, KeyNames[i].name, len)) {
			def += len;
			break;
		}
	}
	if (i < MAX_KEYNAME) {
		keybind.bVk = KeyNames[i].bVk;
		return keybind;
	}
	SHORT r = CXkeymacsDll::ConvVkey(*def++, 0);
	if (r & (1 << 8) && !(keybind.nType & (WIN_CTRL | WIN_ALT | WIN_WIN)))
		keybind.nType |= SHIFT;
	keybind.bVk = static_cast<BYTE>(r);
	return keybind;
}
