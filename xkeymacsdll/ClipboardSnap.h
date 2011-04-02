// ClipboardSnap.h: CClipboardSnap クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIPBOARDSNAP_H__1FAE18B5_B0B4_4431_B3C4_BE6B26447AF2__INCLUDED_)
#define AFX_CLIPBOARDSNAP_H__1FAE18B5_B0B4_4431_B3C4_BE6B26447AF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "clipboardformatsnap.h"

class CClipboardSnap  
{
	//
	// * Constructors
	//
public:
	CClipboardSnap();
	virtual ~CClipboardSnap();

	//
	// * Copy
	//
public:
	CClipboardSnap( const CClipboardSnap& rhs );
	const BOOL operator==( const CClipboardSnap& rhs );
	const BOOL operator!=( const CClipboardSnap& rhs );
	const CClipboardSnap& operator=( const CClipboardSnap& rhs );
	const CClipboardSnap& operator+=( const CClipboardSnap& rhs );

	//
	// * Operations
	//
public:
	BOOL Restore();
	BOOL Capture( const BOOL bTextOnly = FALSE );
	int FindReturn();
	void DeleteLastReturn();
	void Add( const char* const pAddedString, const BOOL bTop );
	BOOL IsEmpty();
	void SetNext( CClipboardSnap* pNext );
	CClipboardSnap* GetNext();
	BOOL IsAllMergeableFormat();
	int GetCount();
private:
	void ClearFormatSnaps();

	//
	// * Attributes
	//
private:
	CArray<CClipboardFormatSnap *, CClipboardFormatSnap *> m_oFormatSnaps;
	CClipboardSnap* m_pNext;
};

#endif // !defined(AFX_CLIPBOARDSNAP_H__1FAE18B5_B0B4_4431_B3C4_BE6B26447AF2__INCLUDED_)
