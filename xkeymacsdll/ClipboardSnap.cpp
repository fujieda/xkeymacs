// ClipboardSnap.cpp: CClipboardSnap クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ClipboardSnap.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CClipboardSnap::CClipboardSnap() :  m_oFormatSnaps()
{
	m_pNext = NULL;
}

CClipboardSnap::~CClipboardSnap()
{
	ClearFormatSnaps();
}

//////////////////////////////////////////////////////////////////////
// コピー
//////////////////////////////////////////////////////////////////////

CClipboardSnap::CClipboardSnap( const CClipboardSnap& rhs ) : m_oFormatSnaps()
{
	m_pNext = NULL;
	int nLen = rhs.m_oFormatSnaps.GetSize();
	ClearFormatSnaps();
	m_oFormatSnaps.SetSize( nLen );

	for( int i = 0; i < nLen; ++i )
	{
		try {
			m_oFormatSnaps[ i ]  = new CClipboardFormatSnap;
			if( m_oFormatSnaps[ i ] ) *m_oFormatSnaps[ i ] = *rhs.m_oFormatSnaps[ i ];
		}
		catch (CMemoryException* e) {
			e->Delete();
//			CUtils::Log("CClipboardSnap::CClipboardSnap: 'new' threw an exception");
		}
	}

	if( m_pNext ) m_pNext->ClearFormatSnaps();
	m_pNext = rhs.m_pNext;
}

const BOOL CClipboardSnap::operator==( const CClipboardSnap& rhs )
{
	if( m_oFormatSnaps.GetSize() != rhs.m_oFormatSnaps.GetSize() ) return FALSE;

	for( int i = 0; i < m_oFormatSnaps.GetSize(); ++i )
	{
		if ( *m_oFormatSnaps[ i ] != *rhs.m_oFormatSnaps[ i ] ) return FALSE;
	}

	return TRUE;
}

const BOOL CClipboardSnap::operator!=( const CClipboardSnap& rhs )
{
	return !( *this == rhs );
}

const CClipboardSnap& CClipboardSnap::operator=( const CClipboardSnap& rhs )
{
	int nLen = rhs.m_oFormatSnaps.GetSize();
	ClearFormatSnaps();
	m_oFormatSnaps.SetSize( nLen );

	for( int i = 0; i < nLen; ++i )
	{
		try {
			m_oFormatSnaps[ i ]  = new CClipboardFormatSnap;
			if( m_oFormatSnaps[ i ] ) *m_oFormatSnaps[ i ] = *rhs.m_oFormatSnaps[ i ];
		}
		catch (CMemoryException* e) {
			e->Delete();
//			CUtils::Log("CClipboardSnap::operator=: 'new' threw an exception");
		}
	}

	if( m_pNext ) m_pNext->ClearFormatSnaps();
	m_pNext = rhs.m_pNext;

	return *this;
}

const CClipboardSnap& CClipboardSnap::operator+=( const CClipboardSnap& rhs )
{
	const CClipboardSnap lhs = *this;

	// count the number of mergeable clipboard format
	int nMergeableClipboardFormatNumber = 0;
	struct MergeableClipboardFormatPosition
	{
		int m_nLHS;
		int m_nRHS;
	};

	const int nMergeableClipboardFormatPosition = lhs.m_oFormatSnaps.GetSize() + rhs.m_oFormatSnaps.GetSize();
	try {
		MergeableClipboardFormatPosition *const pMergeableClipboardFormatPosition = new MergeableClipboardFormatPosition[ nMergeableClipboardFormatPosition ];
		if( !pMergeableClipboardFormatPosition ) return *this;

		memset( pMergeableClipboardFormatPosition, -1, sizeof(MergeableClipboardFormatPosition) * nMergeableClipboardFormatPosition );

		for( int nLHS = 0; nLHS < lhs.m_oFormatSnaps.GetSize(); ++nLHS )	// LHS: Left Hand Side
		{
			if( lhs.m_oFormatSnaps[ nLHS ]->IsMergeableFormat() )
			{
				( pMergeableClipboardFormatPosition + nMergeableClipboardFormatNumber )->m_nLHS = nLHS;
				++nMergeableClipboardFormatNumber;
			}
		}
		for( int nRHS = 0; nRHS < rhs.m_oFormatSnaps.GetSize(); ++nRHS )	// RHS: Right Hand Side
		{
			if( rhs.m_oFormatSnaps[ nRHS ]->IsMergeableFormat() )
			{
				int i;
				for( i = 0; i < nMergeableClipboardFormatNumber; ++i )
				{
					if( ( pMergeableClipboardFormatPosition + i )->m_nLHS < 0 ) continue;
					if( rhs.m_oFormatSnaps[ nRHS ]->IsSameFormat( lhs.m_oFormatSnaps[ ( pMergeableClipboardFormatPosition + i )->m_nLHS ] ) ) break;
				}

				( pMergeableClipboardFormatPosition + i )->m_nRHS = nRHS;
				if( i ==  nMergeableClipboardFormatNumber )
				{
					++nMergeableClipboardFormatNumber;
				}
			}
		}

		// clear current data
		m_oFormatSnaps.RemoveAll();
		m_oFormatSnaps.SetSize( nMergeableClipboardFormatNumber );

		// set data
		for( int i = 0; i < nMergeableClipboardFormatNumber; ++i )
		{
			CClipboardFormatSnap *pFormatSnap = new CClipboardFormatSnap;
			if( pFormatSnap )
			{
				m_oFormatSnaps[ i ] = pFormatSnap;
				if( ( ( pMergeableClipboardFormatPosition + i )->m_nLHS < 0 ) &&
					( ( pMergeableClipboardFormatPosition + i )->m_nRHS < 0 ) )
				{
					ASSERT( 0 );
				}
				else if( ( pMergeableClipboardFormatPosition + i )->m_nLHS < 0 )
				{
					*m_oFormatSnaps[ i ] = *rhs.m_oFormatSnaps[ ( pMergeableClipboardFormatPosition + i )->m_nRHS ];
				}
				else if( ( pMergeableClipboardFormatPosition + i )->m_nRHS < 0 )
				{
					*m_oFormatSnaps[ i ] = *lhs.m_oFormatSnaps[ ( pMergeableClipboardFormatPosition + i )->m_nLHS ];
				}
				else
				{
//					*lhs.m_oFormatSnaps[ ( pMergeableClipboardFormatPosition + i )->m_nLHS ] += *rhs.m_oFormatSnaps[ ( pMergeableClipboardFormatPosition + i )->m_nRHS ];
//					*m_oFormatSnaps[ i ] = *lhs.m_oFormatSnaps[ ( pMergeableClipboardFormatPosition + i )->m_nLHS ];
					*m_oFormatSnaps[ i ] = *lhs.m_oFormatSnaps[ ( pMergeableClipboardFormatPosition + i )->m_nLHS ] +
										   *rhs.m_oFormatSnaps[ ( pMergeableClipboardFormatPosition + i )->m_nRHS ];
//					m_oFormatSnaps[ i ]->Merge( *lhs.m_oFormatSnaps[ ( pMergeableClipboardFormatPosition + i )->m_nLHS ],
//												*rhs.m_oFormatSnaps[ ( pMergeableClipboardFormatPosition + i )->m_nRHS ] );
				}
			}
		}
	}
	catch (CMemoryException* e) {
		e->Delete();
//		CUtils::Log("CClipboardSnap::operator+=: 'new' threw an exception");
	}

	return *this;
}

//////////////////////////////////////////////////////////////////////
// 操作
//////////////////////////////////////////////////////////////////////

BOOL CClipboardSnap::Capture( const BOOL bTextOnly )
{
//	CUtils::Log( "CClipboardSnap::Capture(): start" );

	ClearFormatSnaps();
	if( !CUtils::OpenClipboard() ) return FALSE;

	int nFormat = 0;
	int nFormatCount = CountClipboardFormats();

	for( int i = 0; i < nFormatCount; ++i )
	{
		nFormat = EnumClipboardFormats( nFormat );
		if( nFormat == 0 ) break;
//		CUtils::Log("CClipboardSnap::Capture(): i = %d, nFormat = %d (%s)", i, nFormat, GetFormatName(nFormat));

		if( bTextOnly && !CClipboardFormatSnap::IsMergeableFormat( nFormat ) ) continue;

		// for Microsoft Word
		if( i && nFormat == 49163 ) continue;
		if( i && nFormat == 49156 ) continue;

		try {
			CClipboardFormatSnap *pFormatSnap = new CClipboardFormatSnap;

			if( pFormatSnap && !pFormatSnap->Capture( nFormat ) ) {
//				AfxMessageBox( _T("Capture Ignored or Failed:\n") + GetFormatName(nFormat) );
//				CUtils::Log( "CClipboardSnap::Capture(): Capture Ignored or Failed" );
			} else {
//				CUtils::Log( "CClipboardSnap::Capture(): add start" );
				m_oFormatSnaps.Add( pFormatSnap );
//				CUtils::Log( "CClipboardSnap::Capture(): add end" );
			}
		}
		catch (CMemoryException* e) {
			e->Delete();
//			CUtils::Log("CClipboardSnap::Capture: 'new' threw an exception");
		}
	}

	CloseClipboard();
//	CUtils::Log( "CClipboardSnap::Capture(): end" );
	return TRUE;
}

BOOL CClipboardSnap::Restore()
{
	if( !CUtils::OpenClipboard() ) return FALSE;
	EmptyClipboard();

	for( int i = 0; i < m_oFormatSnaps.GetSize(); ++i )
	{
		m_oFormatSnaps[ i ]->Restore();
	}

	CloseClipboard();
	return TRUE;
}

void CClipboardSnap::ClearFormatSnaps()
{
	for( int i = 0; i < m_oFormatSnaps.GetSize(); ++i )
	{
		delete m_oFormatSnaps[ i ];
	}
	m_oFormatSnaps.RemoveAll();

	if( m_pNext ) m_pNext->ClearFormatSnaps();
	m_pNext = NULL;
}

int CClipboardSnap::FindReturn()
{
	for( int i = 0; i < m_oFormatSnaps.GetSize(); ++i )
	{
		int rc = m_oFormatSnaps[ i ]->FindReturn();
		if( rc != -1 ) return rc;
	}
	return -1;
}

BOOL CClipboardSnap::IsEmpty()
{
	for( int i = 0; i < m_oFormatSnaps.GetSize(); ++i )
	{
		if( !m_oFormatSnaps[ i ]->IsEmpty() ) return FALSE;
	}
	return TRUE;
}

void CClipboardSnap::Add( const char *const pAddedString, const BOOL bTop )
{
	for( int i = 0; i < m_oFormatSnaps.GetSize(); ++i )
	{
		m_oFormatSnaps[ i ]->Add( pAddedString, bTop );
	}
}

void CClipboardSnap::DeleteLastReturn()
{
	for( int i = 0; i < m_oFormatSnaps.GetSize(); ++i )
	{
		m_oFormatSnaps[ i ]->DeleteLastReturn();
	}
}

void CClipboardSnap::SetNext( CClipboardSnap* pNext )
{
	if( m_pNext ) m_pNext->ClearFormatSnaps();
	m_pNext = pNext;
}

CClipboardSnap* CClipboardSnap::GetNext()
{
	return m_pNext;
}

BOOL CClipboardSnap::IsAllMergeableFormat()
{
	for( int i = 0; i < m_oFormatSnaps.GetSize(); ++i )
	{
		if( !m_oFormatSnaps[ i ]->IsMergeableFormat() ) return FALSE;
	}
	return TRUE;
}

int CClipboardSnap::GetCount()
{
	int nCount = 1;
	for( CClipboardSnap* pSnap = this; pSnap->GetNext(); pSnap = pSnap->GetNext() ) ++nCount;
	return ++nCount;
}
