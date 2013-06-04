#pragma once
#include <string>

#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif
