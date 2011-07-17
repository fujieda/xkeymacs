#include "imelist.h"
#include <vector>
#include <Imm.h>
#include <msctf.h>

IMEListIterator IMEList::begin()
{
	if (list.empty())
		GetIMEList();
	return list.begin();
}

IMEListIterator IMEList::end()
{
	return list.end();
}

void IMEList::GetIMEList()
{
	GetIMM();
	GetTSF();
}

void IMEList::GetIMM()
{
	const int n = GetKeyboardLayoutList(0, NULL);
	if (!n)
		return;
	std::vector<HKL> hkls(n);
	GetKeyboardLayoutList(n, &hkls[0]);
	for (std::vector<HKL>::const_iterator p = hkls.begin(); p != hkls.end(); ++p) {
		IMEInfo info;
		if (ImmGetDescription(*p, info.szDescription, WINDOW_TEXT_LENGTH) &&
				ImmGetIMEFileName(*p, info.szFileName, MAX_PATH))
			list.push_back(info);
	}
}

void IMEList::GetTSF()
{
	CoInitialize(NULL);
	HRESULT hr;
	ITfInputProcessorProfiles *pProfiles;
	hr = CoCreateInstance(CLSID_TF_InputProcessorProfiles, NULL, CLSCTX_INPROC_SERVER, IID_ITfInputProcessorProfiles, reinterpret_cast<LPVOID*>(&pProfiles));
	if (FAILED(hr)) {
		CoUninitialize();
		return;
	}
	const LANGID langid = GetUserDefaultLangID();
	IEnumTfLanguageProfiles *pEnum;
	hr = pProfiles->EnumLanguageProfiles(langid, &pEnum);
	if (FAILED(hr))
		goto fail;
	TF_LANGUAGEPROFILE prof;
	ULONG fetch;
	while (pEnum->Next(1, &prof, &fetch) == S_OK) {
		BSTR bstr;
		hr = pProfiles->GetLanguageProfileDescription(prof.clsid, langid, prof.guidProfile, &bstr);
		if (FAILED(hr))
			continue;
		IMEInfo info;
#ifdef _MBCS
		WideCharToMultiByte(CP_ACP, 0, bstr, -1, info.szDescription, MAX_PATH, NULL, NULL);
#else
		wcscpy_s(info.szDescription, WINDOW_TEXT_LENGTH, bstr);
#endif
		bool exist = false;
		for (IMEListIterator p = list.begin(); p != list.end(); ++p)
			if (!_tcscmp(info.szDescription, p->szDescription)) { // already get via IMM
				exist = true;
				break;
			}
		if (!exist) {
			_tcscpy_s(info.szFileName, _T("IME"));
			list.push_back(info);
			break;
		}
	}
	pEnum->Release();
fail:
	pProfiles->Release();
	CoUninitialize();
}
