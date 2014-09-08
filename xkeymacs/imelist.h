#ifndef IMEINFO_H_INCLUDED
#define IMEINFO_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../xkeymacsdll/defs.h"
#include <list>

struct IMEInfo
{
	TCHAR description[WINDOW_TEXT_LENGTH];
	TCHAR filename[MAX_PATH];
};

typedef std::list<IMEInfo>::const_iterator IMEListIterator;

class IMEList
{
private:
	std::list<IMEInfo> list;
	void GetIMEList();
	void GetIMM();
	void GetTSF();
public:
	IMEListIterator begin();
	IMEListIterator end();
};

#endif // IME_INFO_H_INCLUDED
