#ifndef IPC_H_INCLUDED
#define IPC_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "defs.h"

struct AppConfig {
	TCHAR AppName[CLASS_NAME_LENGTH];
	TCHAR WindowText[WINDOW_TEXT_LENGTH];
	BYTE SettingStyle;
	BYTE CmdID[MAX_COMMAND_TYPE][MAX_KEY];
	char FuncID[MAX_COMMAND_TYPE][MAX_KEY];
	BYTE KillRingMax;
	bool Is326Compatible;
	bool EnableCUA;
	bool UseDialogSetting;
	bool IgnoreUndefC_x;
	bool IgnoreUndefMetaCtrl;
};

struct Config {
	AppConfig AppConfig[MAX_APP];
	KeyBind FuncDefs[MAX_FUNCTION][MAX_DEFINITION];
	bool Is106Keyboard;
};

enum XKEYMACS_IPC32 { IPC32_TERMINATE, IPC32_ICON, IPC32_HOOKSTATE };
enum XKEYMACS_IPC64 { IPC64_EXIT, IPC64_RELOAD, IPC64_DISABLE, IPC64_ENABLE };

struct IconState {
	ICON_TYPE Type;
	int State;
	TCHAR Tip[128];
};

struct IPC32Message {
	XKEYMACS_IPC32 Type;
	union {
		bool Enable;
		IconState IconState[MAX_ICON_TYPE];
	};
};

#define XKEYMACS32_PIPE _T("\\\\.\\pipe\\XKEYMACS_IPC32")
#define XKEYMACS64_PIPE _T("\\\\.\\pipe\\XKEYMACS_IPC64")
#endif
