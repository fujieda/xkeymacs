#ifndef IPC_H_INCLUDED
#define IPC_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "defs.h"

struct IconMsg {
	ICON_TYPE nType;
	int nState;
	TCHAR szTip[128];
};

struct AppConfig {
	TCHAR AppName[CLASS_NAME_LENGTH];
	TCHAR WindowText[WINDOW_TEXT_LENGTH];
	BYTE SettingStyle;
	BYTE CmdID[MAX_COMMAND_TYPE][MAX_KEY];
	BYTE FuncID[MAX_COMMAND_TYPE][MAX_KEY];
	BYTE KillRingMax;
	bool Is326Compatible;
	bool EnableCUA;
	bool UseDialogSetting;
	bool IgnoreUndefC_x;
	bool IgnoreUndefMetaCtrl;
};

struct Config {
	AppConfig AppConfig[MAX_APP];
	TCHAR FuncDef[MAX_FUNCTION][MAX_DEFINITION];
	bool Is106Keyboard;
};

enum XKEYMACS_IPC { XKEYMACS_EXIT, XKEYMACS_RELOAD, XKEYMACS_RESET };

#define ICON_PIPE _T("\\\\.\\pipe\\XKEYMACS_ICON")
#define IPC_PIPE _T("\\\\.\\pipe\\XKEYMACS_IPC")
#endif
