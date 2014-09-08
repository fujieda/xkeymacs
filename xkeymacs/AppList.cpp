#include "AppList.h"
#include "profile.h"
#include "imelist.h"
#include "../xkeymacsdll/AppName.h"
#include <vector>

struct ProcArg {
	CProperties* Properties;
	std::vector<PROCESSENTRY32> Processes;
};

void AppList::SetAppList(CProperties& cProperties)
{
	ProcArg arg;
	GetProcesses(arg.Processes);
	arg.Properties = &cProperties;
	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&arg));

	TCHAR appTitle[MAX_APP][WINDOW_TEXT_LENGTH];
	TCHAR appName[MAX_APP][CLASS_NAME_LENGTH];
	CProfile::GetAppList(appTitle, appName);
	for (int i = 0; i < MAX_APP; ++i) {
		if (!appName[i][0] || !_tcscmp(appName[i], _T("IME")))
			continue;
		if (CString(MAKEINTRESOURCE(IDS_DEFAULT)) == appName[i] ||
				CString(MAKEINTRESOURCE(IDS_DIALOG)) == appName[i])
			continue;
		cProperties.AddItem(appTitle[i], appName[i]);
	}
	AddIMEInfo(cProperties);
}

void AppList::GetProcesses(std::vector<PROCESSENTRY32>& list)
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return;
	PROCESSENTRY32 p;
	p.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnap, &p)) {
		do {
			list.push_back(p);
		} while (Process32Next(hProcessSnap, &p));
	}
	CloseHandle(hProcessSnap);
}

BOOL CALLBACK AppList::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	ProcArg *arg = reinterpret_cast<ProcArg*>(lParam);
	
	TCHAR windowText[WINDOW_TEXT_LENGTH];
	::GetWindowText(hWnd, windowText, WINDOW_TEXT_LENGTH);
	if (!(IsWindowVisible(hWnd) &&
			GetWindow(hWnd, GW_OWNER) == NULL &&
			windowText[0] != _T('\0')))
		return TRUE;

	CString appTitle;
	DWORD procID;
	GetWindowThreadProcessId(hWnd, &procID);
	TCHAR appName[CLASS_NAME_LENGTH];
	for (std::vector<PROCESSENTRY32>::const_iterator i = arg->Processes.begin(); i < arg->Processes.end(); ++i) {
		if (i->th32ProcessID != procID)
			continue;
		_tcscpy_s(appName, i->szExeFile);
		appTitle = GetAppTitle(windowText, appName);
		break;
	}
	arg->Properties->AddItem(appTitle, appName);
	return TRUE;
}

CString AppList::GetAppTitle(TCHAR (&windowText)[WINDOW_TEXT_LENGTH], TCHAR (&appName)[CLASS_NAME_LENGTH])
{
	CString appTitle;
	if (CString(MAKEINTRESOURCE(IDS_B2)).CompareNoCase(appName) == 0)
		appTitle.LoadString(IDS_BECKY);
	else if (CString(MAKEINTRESOURCE(IDS_EXPLORER)).CompareNoCase(appName) == 0)
		appTitle.LoadString(IDS_PROGRAM_MANAGER);
	else if (CString(MAKEINTRESOURCE(IDS_MSIMN)).CompareNoCase(appName) == 0)
		appTitle.LoadString(IDS_OUTLOOK_EXPRESS);
	else if (CString(MAKEINTRESOURCE(IDS_PROJECT)).CompareNoCase(appName) == 0 ||
			CString(MAKEINTRESOURCE(IDS_EXCEL)).CompareNoCase(appName) == 0 ||
			_tcsicmp(_T("psp.exe"), appName) == 0 ||
			_tcsicmp(_T("devenv.exe"), appName) == 0)
		GetNthSegment(windowText, appTitle, 1);
	else if (_tcsicmp(_T("sakura.exe"), appName) == 0)
		GetNthSegment(windowText, appTitle, 2); // '.' is included, so...
	else if (CString(MAKEINTRESOURCE(IDS_MSDN)).CompareNoCase(appName) == 0)
		appTitle = windowText;
	else if (_tcsicmp(_T("vb6.exe"), appName) == 0)
		appTitle = _T("Microsoft Visual Basic");
	else if (_tcsicmp(_T("ssexp.exe"), appName) == 0)
		appTitle.LoadString(IDS_VISUAL_SOURCESAFE_EXPLORER);
	else if (_tcsicmp(_T("mintty.exe"), appName) == 0)
		appTitle = _T("mintty");
	else {
		AppName::CorrectAppName(windowText, appName);
		GetNthSegment(windowText, appTitle, -1);
	}
	return appTitle;
}

// This function set the nth segment in a window name separated by " - "
// to the 2nd argument return whether it found an appropriate segment.
// If there aren't sufficient segments, it returns the last segment
// appropriate for the application title.
bool AppList::GetNthSegment(const CString& windowText, CString& part, int nth)
{
	int sep = windowText.Find(_T(" - "));
	if (sep < 0) {
		part = windowText;
		return false;
	}
	if (nth == 0) {
		part = windowText.Left(sep);
		return true;
	}
	if (GetNthSegment(windowText.Right(windowText.GetLength() - sep - 3), part, nth - 1) ||
			part.GetAt(0) != _T('[') && part.FindOneOf(_T(".]")) == -1)
		return true;
	part = windowText.Left(sep);
	return false;
}

void AppList::AddIMEInfo(CProperties& cProperties)
{
	IMEList imeList;
	for (IMEListIterator p = imeList.begin(); p != imeList.end(); ++p)
		cProperties.AddItem(p->description, p->filename);
}
