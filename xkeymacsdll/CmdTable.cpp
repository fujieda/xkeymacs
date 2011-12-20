#include "CmdTable.h"
#include "xkeymacsdll.h"

LPCTSTR CmdTable::Name(int cmdID)
{
	return m_Table[cmdID].Name;
}

int CmdTable::CategoryID(int cmdID)
{
	return m_Table[cmdID].CategoryID;
}

int CmdTable::DescriptionID(int cmdID)
{
	return m_Table[cmdID].CategoryID;
}

int CmdTable::ToolTipID(int cmdID)
{
	return m_Table[cmdID].ToolTipID;
}

int (*CmdTable::Command(int cmdID))()
{
	return m_Table[cmdID].Command;
}

const KeyBind CmdTable::Bind(int cmdID, int index)
{
	if (!CXkeymacsDll::Is106Keyboard())
		return m_Table[cmdID].KeyBind[index];
	KeyBind bind;
	bind = m_Table[cmdID].KeyBind[index];
	if (bind.nType & SHIFT) {
		switch (bind.bVk) {
		case '0':
			bind.bVk = '9';
			return bind;
		case '2':
			bind.bVk = VK_OEM_3; // @
			bind.nType &= ~SHIFT;
			return bind;
		case '6':
			bind.bVk = VK_OEM_7; // ^
			bind.nType &= ~SHIFT;
			return bind;
		case '7':
			bind.bVk = '6'; // &
			return bind;
		case '8':
			bind.bVk = VK_OEM_1; // *
			return bind;
		case VK_OEM_1:
			bind.nType &= ~SHIFT; // :
			return bind;
		case VK_OEM_MINUS:
			bind.bVk = VK_OEM_102; // _
			return bind;
		case VK_OEM_3:
			bind.bVk = VK_OEM_7; // {
			return bind;
		case VK_OEM_7:
			bind.bVk = '2'; // "
			return bind;
		}
		return bind;
	}
	switch (bind.bVk) {
	case VK_OEM_1:
		bind.bVk = VK_OEM_PLUS; // :
		return bind;
	case VK_OEM_PLUS:
		bind.bVk = VK_OEM_MINUS; // =
		bind.nType |= SHIFT;
		return bind;
	case VK_OEM_3: // `
		bind.nType |= SHIFT;
		return bind;
	case VK_OEM_7:
		bind.bVk = '7'; // '
		bind.nType |= SHIFT;
		return bind;
	}
	return bind;
}

const int* CmdTable::Categories()
{
	return CmdTable::m_Categories;
}

int CmdTable::CmdByName(LPCTSTR name)
{
	for (int cmdID = 0; cmdID < MAX_COMMAND; ++cmdID)
		if (!_tcscmp(m_Table[cmdID].Name, name))
			return cmdID;
	return -1;
}
