// ClipboardFormatSnap.cpp: CClipboardFormatSnap クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ClipboardFormatSnap.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CClipboardFormatSnap::CClipboardFormatSnap() :
m_nFormat( 0 ), m_oData()
{
}

CClipboardFormatSnap::~CClipboardFormatSnap()
{
}

//////////////////////////////////////////////////////////////////////
// コピー
//////////////////////////////////////////////////////////////////////

CClipboardFormatSnap::CClipboardFormatSnap( const CClipboardFormatSnap& rhs ) :
m_nFormat( 0 ), m_oData()
{
	m_nFormat = rhs.m_nFormat;

	int nLen = rhs.m_oData.GetSize();
	m_oData.RemoveAll();
	m_oData.SetSize( nLen );

	for( int i = 0; i < nLen; ++i )
	{
		m_oData[ i ] = rhs.m_oData[ i ];
	}
}

const BOOL CClipboardFormatSnap::operator==( const CClipboardFormatSnap& rhs )
{
	if( m_nFormat != rhs.m_nFormat ) return FALSE;

	if( m_oData.GetSize() != rhs.m_oData.GetSize() ) return FALSE;

	for( int i = 0; i < m_oData.GetSize(); ++i )
	{
		if( m_oData[ i ] != rhs.m_oData[ i ] ) return FALSE;
	}

	return TRUE;
}

const BOOL CClipboardFormatSnap::operator!=( const CClipboardFormatSnap& rhs )
{
	return !( *this == rhs );
}

const CClipboardFormatSnap& CClipboardFormatSnap::operator=( const CClipboardFormatSnap& rhs )
{
	m_nFormat = rhs.m_nFormat;

	int nLen = rhs.m_oData.GetSize();
	m_oData.RemoveAll();
	m_oData.SetSize( nLen );

	for( int i = 0; i < nLen; ++i )
	{
		m_oData[ i ] = rhs.m_oData[ i ];
	}

	return *this;
}

void CClipboardFormatSnap::MergeText( const CClipboardFormatSnap &rhs, const int nSizeOfNullTerminator )
{
	const int nOriginalTextLength = GetTextLength( nSizeOfNullTerminator );
	if( 0 < nOriginalTextLength )
	{
		m_oData.SetSize( nOriginalTextLength + rhs.m_oData.GetSize() );
		for( int i = 0; i < rhs.m_oData.GetSize(); ++i )
		{
			m_oData[ nOriginalTextLength + i ] = rhs.m_oData[ i ];
		}
	}
	else
	{
		*this = rhs;
	}
}

void CClipboardFormatSnap::MergeRTF( const CClipboardFormatSnap& rhs )
{
	const int nOriginalHead = GetRTFBodyHead();
	const int nOriginalLength = GetRTFBodyLength();
	const int nRHSHead = rhs.GetRTFBodyHead();
	const int nRHSLength = rhs.GetRTFBodyLength();

	if( nRHSLength )
	{
		m_oData.SetSize( m_oData.GetSize() + nRHSLength );
		for( int i = m_oData.GetSize() - 1; nOriginalHead + nOriginalLength <= i ; --i )
		{
			m_oData[ i ] = m_oData[ i - nRHSLength ];
		}
		for( int j = 0; j < nRHSLength; ++j )
		{
			m_oData[ nOriginalHead + nOriginalLength + j ] = rhs.m_oData[ nRHSHead + j ];
		}
	}
}

const CClipboardFormatSnap& CClipboardFormatSnap::operator+( const CClipboardFormatSnap& rhs )
{
	if( !rhs.m_oData.GetSize() ) return *this;

	ASSERT( m_nFormat == rhs.m_nFormat );
	m_nFormat = rhs.m_nFormat;

	switch( m_nFormat )
	{
	case CF_TEXT:
	case CF_OEMTEXT:
		MergeText( rhs, 1 );
		break;
	case CF_UNICODETEXT:
		MergeText( rhs, 2 );
		break;
	case CF_LOCALE:
		*this = rhs;
		break;
	case 49310:	// Rich Text Format
		MergeRTF( rhs );
		break;
	default:
		ASSERT( 0 );
		*this = rhs;
		break;
	}

	return *this;
}

/*
const CClipboardFormatSnap& CClipboardFormatSnap::Merge( CClipboardFormatSnap& lhs, CClipboardFormatSnap& rhs )
{
	ASSERT( m_nFormat == rhs.m_nFormat );
	m_nFormat = lhs.m_nFormat;
	m_oData.RemoveAll();

	switch( m_nFormat )
	{
	case CF_TEXT:
	case CF_OEMTEXT:
		{
			const int nSizeOfNullTerminator = 1;
			if( lhs.m_oData.GetSize() && rhs.m_oData.GetSize() )
			{
				m_oData.SetSize( lhs.m_oData.GetSize() - nSizeOfNullTerminator + rhs.m_oData.GetSize() );

				int nLHS = 0;
				for( nLHS = 0; nLHS < lhs.m_oData.GetSize() - nSizeOfNullTerminator; ++nLHS )
				{
					m_oData[ nLHS ] = lhs.m_oData[ nLHS ];
				}
				for( int nRHS = 0; nRHS < rhs.m_oData.GetSize(); ++nRHS )
				{
					m_oData[ nLHS + nRHS ] = rhs.m_oData[ nRHS ];
				}
			}
			else if( lhs.m_oData.GetSize() )
			{
				*this = lhs;
			}
			else if( rhs.m_oData.GetSize() )
			{
				*this = rhs;
			}
			else
			{
				ASSERT( 0 );
			}
		}
		break;
	case CF_UNICODETEXT:
		{
			const int nSizeOfNullTerminator = 2;
			if( lhs.m_oData.GetSize() && rhs.m_oData.GetSize() )
			{
				m_oData.SetSize( lhs.m_oData.GetSize() - nSizeOfNullTerminator + rhs.m_oData.GetSize() );

				int nLHS = 0;
				for( nLHS = 0; nLHS < lhs.m_oData.GetSize() - nSizeOfNullTerminator; ++nLHS )
				{
					m_oData[ nLHS ] = lhs.m_oData[ nLHS ];
				}
				for( int nRHS = 0; nRHS < rhs.m_oData.GetSize(); ++nRHS )
				{
					m_oData[ nLHS + nRHS ] = rhs.m_oData[ nRHS ];
				}
			}
			else if( lhs.m_oData.GetSize() )
			{
				*this = lhs;
			}
			else if( rhs.m_oData.GetSize() )
			{
				*this = rhs;
			}
			else
			{
				ASSERT( 0 );
			}
		}
		break;
	case CF_LOCALE:
		*this = lhs;
		break;
	default:
		ASSERT( 0 );
		*this = lhs;
		break;
	}

	return *this;
}
*/

//////////////////////////////////////////////////////////////////////
// 操作
//////////////////////////////////////////////////////////////////////

BOOL CClipboardFormatSnap::Capture( int nFormat )
{
//	CUtils::Log("CClipboardFormatSnap::Capture: 0");
	m_nFormat = nFormat;

	//
	// (0) DataObjectは取り除く(4byteのポインタしか得られないため)
	//
	if( GetFormatName( nFormat ) == _T("DataObject") ) return FALSE;

	//
	// (0.5) Excel 上で CF_METAFILEPICT/CF_ENHMETAFILE に GetClipboardData すると "図が大きすぎます。" というエラーが表示される。
	//
//	CUtils::Log("CClipboardFormatSnap::Capture: 0.5");
	if( CUtils::IsExcel() && ( nFormat == CF_METAFILEPICT || nFormat == CF_ENHMETAFILE ) ) return FALSE;

	//
	// (1) Clipboard内のメモリブロック取得
	//
//	CUtils::Log("CClipboardFormatSnap::Capture: 1");
	HANDLE hData    = GetClipboardData( nFormat ); if( hData == NULL ) return FALSE;
//	CUtils::Log("CClipboardFormatSnap::Capture: 1.1");
	void *pData     = GlobalLock( hData );         if( pData == NULL ) return FALSE;
//	CUtils::Log("CClipboardFormatSnap::Capture: 1.2");
	size_t nLenData = GlobalSize( hData );         if( nLenData == 0 ) return FALSE;

	//
	// (2) ローカルオブジェクトへデータコピー
	//
//	CUtils::Log("CClipboardFormatSnap::Capture: 2");
	m_oData.RemoveAll();
//	CUtils::Log("CClipboardFormatSnap::Capture: 2.1");
	m_oData.SetSize( nLenData );
//	CUtils::Log("CClipboardFormatSnap::Capture: 2.2");
	CopyMemory( &m_oData[ 0 ], pData, nLenData );
//	CUtils::Log("CClipboardFormatSnap::Capture: 2.3");
	GlobalUnlock( hData );

	// for debug
//	CUtils::Log( "nFormat: %d (%s)", nFormat, GetFormatName( nFormat ) );
///	if( nFormat == CF_OEMTEXT )
//	if( nFormat == 49310 )	// Rich Text Format
///	if( nFormat == 49360 )	// HTML Format
///	if( nFormat == 49669 )	// Notes Editor Internal
///	if( nFormat == 49742 )	// HPB HTML Format
//	{
///		for( int i = 0; i < m_oData.GetSize(); ++i )
///		{
///			CUtils::Log( "%02x (%c)", m_oData[i], m_oData[ i ] );
///		}
///
///		CUtils::Log( "size = %d", m_oData.GetSize() );
//		char *sz = new char[ m_oData.GetSize() + 1 ];
//		memset( sz, 0, m_oData.GetSize() + 1 );
//		for( int i = 0; i < m_oData.GetSize(); ++i )
//		{
//			*(sz + i) = m_oData[ i ];
//		}
//		CUtils::Log( "%s", sz );
//		CUtils::Log( "length = %d", GetRTFBodyLength());
//		delete[] sz;
//	}

//	CUtils::Log("CClipboardFormatSnap::Capture: 3");
	return TRUE;
}

BOOL CClipboardFormatSnap::Restore()
{
	int nLenData = m_oData.GetSize() * sizeof(BYTE);
	HANDLE hClip = GlobalAlloc( GMEM_MOVEABLE, nLenData );
	void *pClip = GlobalLock( hClip );
	CopyMemory( pClip, &m_oData[ 0 ], nLenData );

	GlobalUnlock( hClip );
	SetClipboardData( m_nFormat, hClip );
	return TRUE;
}

BOOL CClipboardFormatSnap::IsSameFormat( const CClipboardFormatSnap *const pFormatSnap )
{
	return m_nFormat == pFormatSnap->m_nFormat;
}

BOOL CClipboardFormatSnap::IsMergeableFormat()
{
	return IsMergeableFormat( m_nFormat );
}

BOOL CClipboardFormatSnap::IsMergeableFormat( const int nFormat )
{
	switch( nFormat )
	{
	case CF_TEXT:
	case CF_OEMTEXT:
	case CF_UNICODETEXT:
	case CF_LOCALE:
//	case 49310:	// Rich Text Format
		return TRUE;
		break;
	default:
		break;
	}

	return FALSE;
}

int CClipboardFormatSnap::FindReturn()
{
//	CUtils::Log( "%s", GetFormatName( m_nFormat ) );
	switch( m_nFormat )
	{
	case CF_TEXT:
	case CF_OEMTEXT:
		{
			for( int i = 0; i < m_oData.GetSize(); ++i )
			{
//				CUtils::Log( "%#02x (%c)", m_oData[ i ], m_oData[ i ] );
				if( m_oData[ i ] == VK_RETURN ) return i;	// CR
				if( !m_oData[ i ] ) break;					// NULL Terminator
			}
		}
		break;
	case CF_UNICODETEXT:
		{
//			for( int i = 0; i < m_oData.GetSize(); ++i )
//			{
//				CUtils::Log( "%#02x (%c)", m_oData[ i ], m_oData[ i ] );
//			}
		}
		break;
	default:
		break;
	}

	return -1;
}

const int CClipboardFormatSnap::GetTextLength( const int nSizeOfNullTerminator )
{
	try {
		char *pNullTerminator = new char[ nSizeOfNullTerminator ];

		if( !pNullTerminator ) return -1;
		memset( pNullTerminator, 0, nSizeOfNullTerminator );

		char *pCurrentCharactor = new char[ nSizeOfNullTerminator ];
		if( !pCurrentCharactor )
		{
			delete [] pNullTerminator;
			pNullTerminator = NULL;
			return -1;
		}

		for( int i = 0; i < m_oData.GetSize() - nSizeOfNullTerminator + 1; i += nSizeOfNullTerminator )
		{
			for( int j = 0; j < nSizeOfNullTerminator; ++j ) pCurrentCharactor[ j ] = m_oData[ i + j ];

			if( !memcmp( pCurrentCharactor, pNullTerminator, nSizeOfNullTerminator ) )
			{
				delete [] pNullTerminator;
				pNullTerminator = NULL;
				delete [] pCurrentCharactor;
				pCurrentCharactor = NULL;
				return i;
			}
		}

		delete [] pNullTerminator;
		pNullTerminator = NULL;
		delete [] pCurrentCharactor;
		pCurrentCharactor = NULL;
	}
	catch (CMemoryException* e) {
		e->Delete();
//		CUtils::Log("CClipboardFormatSnap::GetTextLength: 'new' threw an exception");
	}

	return -1;
}

const int CClipboardFormatSnap::GetRTFBodyHead() const
{
	int nBra = 0;
	int i;
	for( i = 0; i < m_oData.GetSize(); ++i )
	{
		BOOL bBody = FALSE;

		switch( m_oData[ i ] )
		{
		case '{':
			if( nBra++ )
			{
				for( ++i ; i < m_oData.GetSize(); ++i )
				{
					switch( m_oData[ i ] )
					{
					case '{':
						++nBra;
						break;
					case '}':
						--nBra;
						break;
					case '\\':
//						// for Microsoft Word
//						if( i < m_oData.GetSize() + 7 &&
//							m_oData[ i + 1 ] == 'i' &&
//							m_oData[ i + 2 ] == 'n' &&
//							m_oData[ i + 3 ] == 's' &&
//							m_oData[ i + 4 ] == 'r' &&
//							m_oData[ i + 5 ] == 's' &&
//							m_oData[ i + 6 ] == 'i' &&
//							m_oData[ i + 7 ] == 'd' )
//						{
//							--i;
//							nBra = 1;
//						}
						break;
					default:
						break;
					}

					if( nBra == 1 ) break;
				}
			}
			break;

		case '}':
			if( --nBra == 0 ) {
				i = m_oData.GetSize();
				bBody = TRUE;
			}
			break;

		case '\\':	// tag
			for( ; i < m_oData.GetSize(); ++i )
			{
				BOOL bTagEnd = FALSE;
				switch( m_oData[ i ] )
				{
				case '{':
				case '}':
					--i;
					bTagEnd = TRUE;
					break;
				case ' ':
					bTagEnd = TRUE;
					break;
				case '\\':
					if( i < m_oData.GetSize() + 5 &&
						m_oData[ i + 1 ] == 'p' &&
						m_oData[ i + 2 ] == 'a' &&
						m_oData[ i + 3 ] == 'r' &&
						m_oData[ i + 4 ] == 0x0d &&
						m_oData[ i + 5 ] == 0x0a )
					{
						bTagEnd = TRUE;
						bBody = TRUE;
					}
					if( i < m_oData.GetSize() + 1 &&
						( m_oData[ i + 1 ] == '\\' ||
						  m_oData[ i + 1 ] == '{' ||
						  m_oData[ i + 1 ] == '}' ) )
					{
						bTagEnd = TRUE;
						bBody = TRUE;
					}
					break;
				default:
					break;
				}

				if( bTagEnd ) break;
			}
			break;

		case 0x0d:
			if( i < m_oData.GetSize() + 5 &&
					m_oData[ i + 1 ] == 0x0a &&
					m_oData[ i + 2 ] == '\\' &&
					m_oData[ i + 3 ] == 'p' &&
					m_oData[ i + 4 ] == 'a' &&
					m_oData[ i + 5 ] == 'r' )
			{
				bBody = TRUE;
			}
			break;
		case 0x0a:
			break;

		default:	// body
			bBody = TRUE;
			break;
		}

		if( bBody ) break;
	}

	return i;
}

const int CClipboardFormatSnap::GetRTFBodyLength() const
{
//	CUtils::Log("const int CClipboardFormatSnap::GetRTFBodyLength() const: GetRTFBodyHead() = %d, m_oData.GetSize() = %d", GetRTFBodyHead(), m_oData.GetSize());
	int nLength  = 0;

	for( int i = GetRTFBodyHead(); i < m_oData.GetSize(); ++i )
	{
		if( m_oData[ i ] == '}' ) break;
		else if( m_oData[ i ] == '\\' )
		{
//			CUtils::Log( "Body: %#04x (%c)", m_oData[ i ], m_oData[ i ] );
//			CUtils::Log( "Body: %#04x (%c)", m_oData[ i + 1 ], m_oData[ i + 1 ] );
			nLength += 2;
			++i;
		}
		else
		{
//			CUtils::Log( "Body: %#04x (%c)", m_oData[ i ], m_oData[ i ] );
			++nLength;
		}
	}

	return nLength;
}

BOOL CClipboardFormatSnap::IsEmpty()
{
	BOOL rc = TRUE;
	switch( m_nFormat )
	{
	case CF_TEXT:
	case CF_OEMTEXT:
		if( ( 1 < m_oData.GetSize() ) && m_oData[ 0 ] ) rc = FALSE;
		break;
	case CF_UNICODETEXT:
		if( ( 2 < m_oData.GetSize() ) && m_oData[ 0 ] && m_oData[ 1 ] ) rc = FALSE;
		break;
	default:
		break;
	}
	return rc;
}

void CClipboardFormatSnap::Add( const char *const pAddedString, const BOOL bTop )
{
	switch( m_nFormat )
	{
	case CF_TEXT:
	case CF_OEMTEXT:
		Add( pAddedString, bTop, 1 );
		break;
	case CF_UNICODETEXT:
		Add( pAddedString, bTop, 2 );
		break;
	default:
		break;
	}
}

void CClipboardFormatSnap::Add( const char *const pAddedString, const BOOL bTop, const int nSizeOfNullTerminator )
{
	int nOffset = GetTextLength( nSizeOfNullTerminator );
	if( nOffset < 0 ) nOffset = 0;

	if( m_oData.GetSize() < ( nOffset + nSizeOfNullTerminator * ( (int)strlen( pAddedString ) + 1 ) ) )
	{
		m_oData.SetSize( nOffset + nSizeOfNullTerminator * ( strlen( pAddedString ) + 1 ) );
	}

	if( bTop )
	{
		int i;
		for( i = nOffset + nSizeOfNullTerminator - 1; 0 <= i ; --i )
		{
			m_oData[ nSizeOfNullTerminator * strlen(pAddedString) + i ] = m_oData[ i ];
		}
		for( i = 0; i < ( int )strlen( pAddedString ); ++i )
		{
			for ( int j = 0; j < nSizeOfNullTerminator; ++j ) {
				if( j )	m_oData[ i * nSizeOfNullTerminator + j ] = 0;
				else	m_oData[ i * nSizeOfNullTerminator + j ] = *( pAddedString + i );
			}
		}
	}
	else
	{
		unsigned int i;
		for( i = 0; i < strlen( pAddedString ); ++i )
		{
			for( int j = 0; j < nSizeOfNullTerminator; ++j )
			{
				if( j )	m_oData[ nOffset + i * nSizeOfNullTerminator + j ] = 0;
				else	m_oData[ nOffset + i * nSizeOfNullTerminator + j ] = *( pAddedString + i );
			}
		}

		for( int j = 0; j < nSizeOfNullTerminator; ++j )
		{
			m_oData[ nOffset + i * nSizeOfNullTerminator + j ] = 0;
		}
	}
}

void CClipboardFormatSnap::DeleteLastReturn()
{
	switch( m_nFormat )
	{
	case CF_TEXT:
	case CF_OEMTEXT:
		DeleteLastReturn( 1 );
		break;
	case CF_UNICODETEXT:
		DeleteLastReturn( 2 );
		break;
	default:
		break;
	}
}

void CClipboardFormatSnap::DeleteLastReturn(const int nSizeOfNullTerminator)
{
	if( GetTextLength( nSizeOfNullTerminator ) - nSizeOfNullTerminator * 2 < 0 ) return;

	try {
		char *pCRLF = new char[ nSizeOfNullTerminator * 2 ];
		if( !pCRLF ) return;
		memset( pCRLF, 0, nSizeOfNullTerminator * 2 );
		*pCRLF = '\r';
		*(pCRLF + nSizeOfNullTerminator) = '\n';

		BOOL bDelete = TRUE;
		for( int i = 0; i < nSizeOfNullTerminator * 2; ++i )
		{
//			CUtils::Log( "%#02x (%c) AA _%#02x_", m_oData[ GetTextLength( nSizeOfNullTerminator ) - nSizeOfNullTerminator * 2 + i ], m_oData[ GetTextLength( nSizeOfNullTerminator ) - nSizeOfNullTerminator * 2 + i ], *( pCRLF + i ) );
			if( m_oData[ GetTextLength( nSizeOfNullTerminator ) - nSizeOfNullTerminator * 2 + i ] != *( pCRLF + i ) )
			{
				bDelete = FALSE;
				break;
			}
		}

		if( bDelete )
		{
			const int nOffset = GetTextLength( nSizeOfNullTerminator );
			for( int i = 0; i < nSizeOfNullTerminator * 2; ++i )
			{
//				CUtils::Log( "%#02x (%c) BB", m_oData[ GetTextLength( nSizeOfNullTerminator ) - nSizeOfNullTerminator * 2 + i ], m_oData[ GetTextLength( nSizeOfNullTerminator ) - nSizeOfNullTerminator * 2 + i ] );
				m_oData[ nOffset - nSizeOfNullTerminator * 2 + i ] = 0;
			}
		}
	}
	catch (CMemoryException* e) {
		e->Delete();
//		CUtils::Log("CClipboardFormatSnap::DeleteLastReturn: 'new' threw an exception");
	}
}
