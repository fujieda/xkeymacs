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
		if (ImmGetDescription(*p, info.description, WINDOW_TEXT_LENGTH) &&
				ImmGetIMEFileName(*p, info.filename, MAX_PATH))
			list.push_back(info);
	}
}

void IMEList::GetTSF()
{
	CoInitialize(NULL);
	ITfInputProcessorProfiles *ipp;
	if (FAILED(CoCreateInstance(CLSID_TF_InputProcessorProfiles, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ipp)))) {
		CoUninitialize();
		return;
	}
	ITfInputProcessorProfileMgr *mgr;
	if (FAILED(ipp->QueryInterface(&mgr)))
		goto fail;
	TF_INPUTPROCESSORPROFILE prof;
	if (FAILED(mgr->GetActiveProfile(GUID_TFCAT_TIP_KEYBOARD, &prof))) {
		mgr->Release();
		goto fail;
	}
	mgr->Release();
	if (prof.dwProfileType != TF_PROFILETYPE_INPUTPROCESSOR) // current IME is not TIP
		goto fail;
	BSTR bstr;
	if (FAILED(ipp->GetLanguageProfileDescription(prof.clsid, prof.langid, prof.guidProfile, &bstr)))
		goto fail;
	IMEInfo info;
#ifdef _MBCS
	WideCharToMultiByte(CP_ACP, 0, bstr, -1, info.description, WINDOW_TEXT_LENGTH, NULL, NULL);
#else
	wcscpy_s(info.description, WINDOW_TEXT_LENGTH, bstr);
#endif
	_tcscpy_s(info.filename, _T("IME"));
	list.push_back(info);
fail:
	ipp->Release();
	CoUninitialize();
}
