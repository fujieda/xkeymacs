#pragma once

#include <vector>

class KbdMacro
{
	struct Key
	{
		BYTE vk;
		bool up;
	};
	std::vector<Key> m_keys;
	bool m_down[256];
public:
	void Start();
	void Record(BYTE vk, bool up);
	void End();
	void Call() const;
};

