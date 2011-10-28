#pragma once
#include "stdafx.h"

class KeyString
{
public:
	KeyString(bool is106Keyboard);
	CString ToString(int type, int key);
	void ToKey(LPCTSTR str, int& type, int& key);
};

