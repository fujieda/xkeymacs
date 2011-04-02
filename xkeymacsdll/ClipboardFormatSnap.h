// ClipboardFormatSnap.h: CClipboardFormatSnap クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIPBOARDFORMATSNAP_H__75207F92_15D2_4D59_B413_730D8875976D__INCLUDED_)
#define AFX_CLIPBOARDFORMATSNAP_H__75207F92_15D2_4D59_B413_730D8875976D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>


// ユーティリティ関数

inline CString GetFormatName( int nFormat )
{
	// フォーマット名を返す関数
	// デバッグ用であり、win2k以降を前提としている
	// 古いOSで当関数を使うとうその名前を返す場合がある
	// OWNEDDISPLAYなどのCFもフォローしてないのでwin2kでもうそつく

	// 01-17
	CString sDefaultName[] = {
		_T("CF_TEXT"),
		_T("CF_BITMAP"),
		_T("CF_METAFILEPICT"),
		_T("CF_SYLK"),
		_T("CF_DIF"),
		_T("CF_TIFF"),
		_T("CF_OEMTEXT"),
		_T("CF_DIB"),
		_T("CF_PALETTE"),
		_T("CF_PENDATA"),
		_T("CF_RIFF"),
		_T("CF_WAVE"),
		_T("CF_UNICODETEXT"),
		_T("CF_ENHMETAFILE"),
		_T("CF_HDROP"),
		_T("CF_LOCALE"),
		_T("CF_DIBV5")
	};

	if( nFormat <= 0  ) return _T("");
	if( nFormat <= 17 ) return sDefaultName[ nFormat - 1 ];

	TCHAR sFormatName[ 10000 ];
	if( !GetClipboardFormatName( nFormat, sFormatName, 10000 ) ) return _T("");
	return sFormatName;
}

class CClipboardFormatSnap  
{
	//
	// * Constructors
	//
public:
	CClipboardFormatSnap();
	virtual ~CClipboardFormatSnap();

	//
	// * Copy
	//
public:
	CClipboardFormatSnap( const CClipboardFormatSnap& rhs );
	const CClipboardFormatSnap& operator=( const CClipboardFormatSnap& rhs );
	const BOOL operator==( const CClipboardFormatSnap& rhs );
	const BOOL operator!=( const CClipboardFormatSnap& rhs );

	//
	// * Operations
	//
public:
	BOOL Capture( int nFormat );
	BOOL Restore();
	BOOL IsSameFormat( const CClipboardFormatSnap* const pFormatSnap );
	BOOL IsMergeableFormat();
	static BOOL IsMergeableFormat( const int nFormat );
	int FindReturn();
	void DeleteLastReturn();
	void Add( const char* const pAddedString, const BOOL bTop );
	BOOL IsEmpty();
	const CClipboardFormatSnap& operator+( const CClipboardFormatSnap& rhs );
///	const CClipboardFormatSnap& Merge( CClipboardFormatSnap& lhs, CClipboardFormatSnap& rhs );
protected:
	void DeleteLastReturn( const int nSizeOfNullTerminator );
	void Add( const char* const pAddedString, const BOOL bTop, const int nSizeOfNullTerminator );
	const int GetTextLength( const int nSizeOfNullTerminator );
	const int GetRTFBodyLength() const;
	const int GetRTFBodyHead() const;
	void MergeText( const CClipboardFormatSnap& rhs, const int nSizeOfNullTerminator );
	void MergeRTF( const CClipboardFormatSnap& rhs );

	//
	// * Attributes
	//
private:
	int m_nFormat;
	CArray<BYTE, BYTE> m_oData;
};

#endif // !defined(AFX_CLIPBOARDFORMATSNAP_H__75207F92_15D2_4D59_B413_730D8875976D__INCLUDED_)
