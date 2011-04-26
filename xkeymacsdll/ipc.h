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
	int nMessage;
	BOOL b326Compatible[MAX_APP];
	TCHAR szWindowText[MAX_APP][WINDOW_TEXT_LENGTH];
	TCHAR szFunctionDefinition[MAX_FUNCTION][MAX_DEFINITION];
	int nFunctionID[MAX_APP][MAX_COMMAND_TYPE][MAX_KEY];
	BOOL bEnableCUA[MAX_APP];
	BOOL bUseDialogSetting[MAX_APP];
	BOOL bIgnoreUndefinedC_x[MAX_APP];
	BOOL bIgnoreUndefinedMetaCtrl[MAX_APP];
	int nSettingStyle[MAX_APP];
	int nKillRingMax[MAX_APP];
	TCHAR szSpecialApp[MAX_APP][CLASS_NAME_LENGTH];
	int nCommandID[MAX_APP][MAX_COMMAND_TYPE][MAX_KEY];
	BOOL bAtIbeamCursorOnly[MAX_APP][MAX_COMMAND_TYPE][MAX_KEY];
	BOOL b106Keyboard;
};

enum XKEYMACS_IPC { XKEYMACS_EXIT, XKEYMACS_RELOAD, XKEYMACS_RESET };

#define ICON_PIPE _T("\\\\.\\pipe\\XKEYMACS_ICON")
#define IPC_PIPE _T("\\\\.\\pipe\\XKEYMACS_IPC")
#endif
