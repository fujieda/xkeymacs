#pragma once
#include "../xkeymacsdll/defs.h"

class FuncDefs
{
public:
	static void Load();
	static int GetID(LPCTSTR name);
	static LPCTSTR GetName(int id);
	static int GetNumOfDefs();
	static LPCTSTR GetStrDef(int id);
	static KeyBind (*GetDefs())[MAX_DEFINITION];
private:
	static int m_NumOfDefs;
	static TCHAR m_Names[MAX_FUNCTION][SUB_KEY_NAME_LENGTH];
	static TCHAR m_StrDefs[MAX_FUNCTION][MAX_DEFINITION];
	static KeyBind m_Defs[MAX_FUNCTION][MAX_DEFINITION];
	static void Clear();
	static void LoadFiles();
	static void MoveOldFile(LPCTSTR drive, LPCTSTR dir, LPCTSTR fname, LPCTSTR ext);
	static void LoadFile(LPCTSTR file);
	static bool CheckOverride(LPCTSTR name);
	static void ParseDefs();
	static KeyBind ParseKey(LPCTSTR& def);
};

