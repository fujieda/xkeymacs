#pragma once

#include <msctf.h>

class TSFHandler: public ITfThreadMgrEventSink, public ITfTextEditSink
{
public:
	TSFHandler();
	~TSFHandler();
	static void InitSink();
	static void SetFocus();

	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, void **obj);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	// ITfThreadMgrEventSink
	STDMETHODIMP OnInitDocumentMgr(ITfDocumentMgr *);
	STDMETHODIMP OnPopContext(ITfContext *);
	STDMETHODIMP OnPushContext(ITfContext *);
	STDMETHODIMP OnSetFocus(ITfDocumentMgr *, ITfDocumentMgr *);
	STDMETHODIMP OnUninitDocumentMgr(ITfDocumentMgr *);

	// ITfTextEditSink
	STDMETHODIMP OnEndEdit(ITfContext *, TfEditCookie , ITfEditRecord *);

private:
	ULONG m_RefCount;
	ITfThreadMgr *m_ThreadMgr;
	DWORD m_Cookie;
	ITfContext *m_Context;
	bool m_CompositionState;
};

