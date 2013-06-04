#pragma once

#include "tstring.h"

class AFX_EXT_CLASS PipeName
{
public:
	PipeName() {};
	PipeName(LPCTSTR name) { Init(name); };
	void Init(LPCTSTR name);
	LPCTSTR GetName();
private:
	tstring m_PipeName;
};

