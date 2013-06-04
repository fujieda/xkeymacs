#include "PipeName.h"

void PipeName::Init(LPCTSTR name)
{
	m_PipeName = tstring(_T("\\\\.\\pipe\\")) + name;
	TCHAR uname[UNLEN + 1] = {_T('\0')};
	ULONG size = UNLEN + 1;
	if (GetUserName(uname, &size)) {
		tstring str(uname);
		for (auto iter = str.begin(); iter != str.end(); ++iter)
			if (*iter == _T('\\'))
				*iter = _T('-');
		m_PipeName = (m_PipeName + _T("-") + str).substr(0, 256);

	}
	DWORD error = GetLastError();
}

LPCTSTR PipeName::GetName()
{
	return m_PipeName.c_str();
}