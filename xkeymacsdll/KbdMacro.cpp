#include "KbdMacro.h"
#include "xkeymacsdll.h"

void KbdMacro::Start()
{
	ZeroMemory(m_down, sizeof(m_down));
	m_keys.clear();
}

void KbdMacro::Record(BYTE vk, bool up)
{
	if (up && !m_down[vk])
		return;
	if (!up)
		m_down[vk] = true;
	Key k = {vk, up};
	m_keys.push_back(k);
}

void KbdMacro::End()
{
	while (!m_keys.empty()) { // remove not pushed up
		if (!m_keys.front().up)
			break;
		m_keys.erase(m_keys.begin());
	}
	while (!m_keys.empty()) {  // remove not released push
		if (m_keys.back().up)
			break;
		m_keys.pop_back();
	}
}

void KbdMacro::Call() const
{
	UINT before = CXkeymacsDll::GetModifierState(FALSE);
	CXkeymacsDll::SetModifierState(0, before);
	for (auto k = m_keys.cbegin(); k != m_keys.cend(); ++k)
		if (k->up)
			CXkeymacsDll::ReleaseKey(k->vk);
		else
			CXkeymacsDll::DepressKey(k->vk);
	CXkeymacsDll::SetModifierState(before, 0);
}
