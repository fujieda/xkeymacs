#include "StdAfx.h"
#include "TSFHandler.h"
#include "Utils.h"
#include "TLS.h"
#include "xkeymacsdll.h"
#include <ObjBase.h>

#ifdef DEBUG_IME
#define DebugLog(fmt, ...) CUtils::Log(fmt, __VA_ARGS__)
#else
#define DebugLog(fmt, ...)
#endif

TSFHandler::TSFHandler()
{
	m_RefCount = 1;
	m_ThreadMgr = nullptr;
	m_Cookie = TF_INVALID_COOKIE;
	m_Context = nullptr;
	m_CompositionState = false;
}

TSFHandler::~TSFHandler()
{
	if (m_ThreadMgr)
		m_ThreadMgr->Release();
	if (m_Context)
		m_Context->Release();
}

void TSFHandler::InitSink()
{
	if (TLS::GetTSFHandler())
		return;
	TSFHandler *tsfh = new TSFHandler();
	TLS::PutTSFHandler(tsfh);

	HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) {
		DebugLog(_T("CoInitializeEx failed."));
		return;
	}
	if (hr == S_FALSE)
		CoUninitialize();
	ITfThreadMgr *thread;
	if (FAILED(CoCreateInstance(CLSID_TF_ThreadMgr, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&thread)))) {
		DebugLog(_T("CoCreateInstance for ThreadMgr failed."));
		return;
	}
	tsfh->m_ThreadMgr = thread;
	ITfSource *src;
	if (FAILED(thread->QueryInterface(&src))) {
		DebugLog(_T("ThreadMgr->QueryInterface failed."));
		goto fail;
	}
	DWORD cookie;
	if (FAILED(src->AdviseSink(IID_ITfThreadMgrEventSink, static_cast<ITfThreadMgrEventSink *>(tsfh), &cookie))) {
		DebugLog(_T("Souece->AdviseSink failed."));
		src->Release();
		goto fail;
	}
	src->Release();
	return;
fail:
	delete tsfh;
	return;
}

STDMETHODIMP TSFHandler::QueryInterface(REFIID iid, void **obj)
{
	if (obj == nullptr)
		return E_INVALIDARG;
	*obj = nullptr;
	if (IsEqualIID(iid, IID_IUnknown) || iid == IID_ITfThreadMgrEventSink)
		*obj = static_cast<ITfThreadMgrEventSink *>(this);
	else if (iid == IID_ITfTextEditSink)
		*obj = static_cast<ITfTextEditSink *>(this);
	else
		return E_NOINTERFACE;
	AddRef();
	return S_OK;
}

STDMETHODIMP_(ULONG) TSFHandler::AddRef()
{
	return ++m_RefCount;
}

STDMETHODIMP_(ULONG) TSFHandler::Release()
{
	if (--m_RefCount == 0)
		delete this;
	return m_RefCount;
}

// ITfThreadMgrEventSink
STDMETHODIMP TSFHandler::OnInitDocumentMgr(ITfDocumentMgr *)
{
	DebugLog(_T("OnInitDocumentMgr"));
	return S_OK;
}

STDMETHODIMP TSFHandler::OnPopContext(ITfContext *)
{
	DebugLog(_T("OnPopContext"));
	return S_OK;
}

STDMETHODIMP TSFHandler::OnPushContext(ITfContext *)
{
	DebugLog(_T("OnPushContext"));
	return S_OK;
}

STDMETHODIMP TSFHandler::OnSetFocus(ITfDocumentMgr *docMgr, ITfDocumentMgr *)
{
	DebugLog(_T("OnSetFocus"));
	if (docMgr == nullptr)
		return S_OK;
	if (m_Cookie != TF_INVALID_COOKIE) {
		ITfSource *src;
		if (FAILED(m_Context->QueryInterface(&src))) {
			DebugLog(_T("Context->QueryInterface:0 failed."));
			return S_OK;
		}
		HRESULT hr = src->UnadviseSink(m_Cookie);
		src->Release();
		if (FAILED(hr)) {
			DebugLog(_T("Source->UnadviceThink failed."));
			return S_OK;
		}
		m_Context->Release();
		m_Context = nullptr;
		m_Cookie = TF_INVALID_COOKIE;
	}
	ITfContext *cxt;
	if (FAILED(docMgr->GetTop(&cxt))) {
		DebugLog(_T("DocumentMgr->GetTop failed."));
		return S_OK;
	}
	if (cxt == nullptr) {
		DebugLog(_T("ITfContext is null."));
		return S_OK;
	}
	ITfSource *src;
	if (FAILED(cxt->QueryInterface(&src))) {
		DebugLog(_T("Context->QueryInterface:1 failed."));
		goto fail;
	}
	if (FAILED(src->AdviseSink(IID_ITfTextEditSink, static_cast<ITfTextEditSink *>(this), &m_Cookie))) {
		DebugLog(_T("Source->AdviseSink(ITfTextEditSink) failed."));
		src->Release();
		goto fail;
	}
	src->Release();
	m_Context = cxt;
	return S_OK;
fail:
	cxt->Release();
	return S_OK;
}

STDMETHODIMP TSFHandler::OnUninitDocumentMgr(ITfDocumentMgr *)
{
	DebugLog(_T("OnUninitDocumentMgr"));
	return S_OK;
}

STDMETHODIMP TSFHandler::OnEndEdit(ITfContext *cxt, TfEditCookie, ITfEditRecord *)
{
	DebugLog(_T("OnEndEdit"));
	ITfContextComposition *comp;
	if (FAILED(cxt->QueryInterface(&comp))) {
		DebugLog(_T("Context->QueryInterface(ITfContextComposition) failed."));
		return S_OK;
	}
	IEnumITfCompositionView *enumCompView;
	if (FAILED(comp->EnumCompositions(&enumCompView))) {
		DebugLog(_T("ContextComposition->EnumCompositions failed."));
		goto fail;
	}
	ITfCompositionView *view;
	ULONG fetched;
	HRESULT hr = enumCompView->Next(1, &view, &fetched);
	if (FAILED(hr)) {
		DebugLog(_T("EnumCompositions->Next failed."));
		enumCompView->Release();
		goto fail;
	}
	DebugLog(_T("EnumComposition->Next succeeded. fetched=%d"), fetched);
	if (fetched) {
		if (!m_CompositionState)
			CXkeymacsDll::SetIMEState(true);
		m_CompositionState = true;
		view->Release();
	}
	else {
		if (m_CompositionState)
			CXkeymacsDll::SetIMEState(false);
		m_CompositionState = false;
	}
	enumCompView->Release();
fail:
	comp->Release();
	return S_OK;
}