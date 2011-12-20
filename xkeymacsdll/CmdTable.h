#pragma once
#include "defs.h"

const int MAX_KEY_BIND = 8;
const int MAX_COMMAND = 115;

class AFX_EXT_CLASS CmdTable
{
public:
	struct CmdDef
	{
		LPCTSTR Name;
		int CategoryID;
		int DescriptionID;
		int ToolTipID;
		int (*Command)();
		KeyBind KeyBind[MAX_KEY_BIND];
	};
	static LPCTSTR Name(int cmdID);
	static int CategoryID(int cmdID);
	static int DescriptionID(int cmdID);
	static int ToolTipID(int cmdID);
	static int (*Command(int cmdID))();
	static const KeyBind Bind(int cmdID, int index);
	static const int* Categories();
	static int CmdByName(LPCTSTR name);
	template <typename T> static void EnumCommands(T* obj, void (T::*func)(int cmdID, const KeyBind& bind, int arg), int arg)
	{
		for (int cmdID = 0; cmdID < MAX_COMMAND; ++cmdID)
			for (int i = 0; i < MAX_KEY_BIND; ++i) {
				KeyBind bind = Bind(cmdID, i);
				if (bind.bVk == 0)
					break;
				(obj->*func)(cmdID, bind, arg);
			}
	};
private:
	static const CmdDef m_Table[MAX_COMMAND];
	static const int m_Categories[];
};
