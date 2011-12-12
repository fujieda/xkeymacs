#pragma once
#include "properties.h"
#include <TlHelp32.h>

class AppList
{
public:
	static void SetAppList(CProperties& properties);
private:
	static void GetProcesses(std::vector<PROCESSENTRY32>& list);
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	static void AddIMEInfo(CProperties& cProperties);
	static CString GetAppTitle(TCHAR (&windowText)[WINDOW_TEXT_LENGTH], TCHAR (&appName)[CLASS_NAME_LENGTH]);
	static bool GetNthSegment(const CString& windowName, CString& part, int nth);
};

