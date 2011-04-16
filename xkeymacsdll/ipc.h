#ifndef IPC_H_INCLUDED
#define IPC_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct ICONMSG {
	ICON_TYPE nType;
	int nState;
	TCHAR szTip[128];
};

#define ICON_PIPE _T("\\\\.\\pipe\\XKEYMACS_ICON")
#endif
