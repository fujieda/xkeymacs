#pragma once

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

#define PIPENAME_MAX 256

struct Config {
	AppConfig AppConfig[MAX_APP];
	KeyBind FuncDefs[MAX_FUNCTION][MAX_DEFINITION];
	bool Is106Keyboard;
	TCHAR PipeNameForIPC32[PIPENAME_MAX + 1];
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

#define PIPENAME_IPC32 _T("xkeymacs32")
#define PIPENAME_IPC64 _T("xkeymacs64")
