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

struct CONFIG {
	BYTE b326Compatible[MAX_APP];
	TCHAR szWindowText[MAX_APP][WINDOW_TEXT_LENGTH];
	TCHAR szFunctionDefinition[MAX_FUNCTION][MAX_DEFINITION];
	BYTE nFunctionID[MAX_APP][MAX_COMMAND_TYPE][MAX_KEY];
	BYTE bEnableCUA[MAX_APP];
	BYTE bUseDialogSetting[MAX_APP];
	BYTE bIgnoreUndefinedC_x[MAX_APP];
	BYTE bIgnoreUndefinedMetaCtrl[MAX_APP];
	BYTE nSettingStyle[MAX_APP];
	BYTE nKillRingMax[MAX_APP];
	TCHAR szSpecialApp[MAX_APP][CLASS_NAME_LENGTH];
	BYTE nCommandID[MAX_APP][MAX_COMMAND_TYPE][MAX_KEY];
	BYTE b106Keyboard;
};

enum XKEYMACS_IPC { XKEYMACS_EXIT, XKEYMACS_RELOAD, XKEYMACS_RESET };

#define ICON_PIPE _T("\\\\.\\pipe\\XKEYMACS_ICON")
#define IPC_PIPE _T("\\\\.\\pipe\\XKEYMACS_IPC")
#endif
