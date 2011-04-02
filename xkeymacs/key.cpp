// Key.cpp : implementation file
//

#include "stdafx.h"
#include "xkeymacs.h"
#include "Key.h"
#include "Profile.h"
#include "104Keyboard.h"
#include "109Keyboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKey

CKey::CKey(const int nKey, const int nKeyType, const HKEY_TYPE hkeyType)
{
	m_nKey = nKey;
	m_nKeyType = nKeyType;
	m_nDroppableKey = 0;
	m_nDroppableKeyType = NORMAL_KEY;
	m_HkeyType = hkeyType;
}

CKey::~CKey()
{
}


BEGIN_MESSAGE_MAP(CKey, CButton)
	//{{AFX_MSG_MAP(CKey)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKey message handlers



void CKey::DrawItem(const LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	static const COLORREF BLACK = RGB(0, 0, 0);
	static const COLORREF RED = RGB(255, 0, 0);
	static const COLORREF BLUE = RGB(0, 0, 255);

	COLORREF textcolor = BLACK;
	UINT nEdge = EDGE_RAISED;
	CRect textrect;
	textrect.CopyRect(&lpDrawItemStruct->rcItem);

	switch (m_nKeyType) {
	case NORMAL_KEY:
		nEdge = EDGE_RAISED;
		textcolor = BLACK;
		break;
	case PUSHED_KEY:
		nEdge = EDGE_SUNKEN;
		textcolor = BLACK;
		textrect.OffsetRect(1, 1);
		break;
	case DROPPABLE_KEY:
		nEdge = EDGE_SUNKEN;
		textcolor = BLUE;
		textrect.OffsetRect(1, 1);
		break;
	case ORIGINAL_KEY:
		nEdge = EDGE_ETCHED;
		textcolor = BLACK;
		break;
	case REMAPPED_KEY:
		nEdge = EDGE_RAISED;
		textcolor = RED;
		break;
	case REMAPPED_PUSHED_KEY:
		nEdge = EDGE_SUNKEN;
		textcolor = RED;
		textrect.OffsetRect(1, 1);
		break;
	default:
		ASSERT(0);
		break;
	}

	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect buttonrect;
	buttonrect.CopyRect(&lpDrawItemStruct->rcItem);

	pDC->DrawEdge(buttonrect, nEdge, BF_RECT);
	pDC->SetTextColor(textcolor);

	CString szWindowText;
	GetWindowText(szWindowText);

	if (textcolor == RED) { // FW_BOLD
		LOGFONT logfont;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(logfont), &logfont);
		logfont.lfWeight = FW_BOLD;

		CFont font;
		font.CreateFontIndirect(&logfont);

		pDC->SelectObject(&font);
		pDC->DrawText(szWindowText, textrect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pDC->SelectStockObject(DEFAULT_GUI_FONT);
	} else { // FW_NORMAL
		pDC->DrawText(szWindowText, textrect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}

void CKey::SetKeyType(const int nKeyType)
{
	if (m_nKeyType != nKeyType) {
		m_nKeyType = nKeyType;
		RedrawWindow();
	}
}

int CKey::GetKeyType()
{
	return m_nKeyType;
}

void CKey::OnLButtonDown(const UINT nFlags, const CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	switch (m_nKeyType) {
	case NORMAL_KEY:
		SetKeyType(PUSHED_KEY);
		CProfile::SetDraggingCursor();
		break;
	case REMAPPED_KEY:
		SetKeyType(REMAPPED_PUSHED_KEY);
		break;
	}

	CButton::OnLButtonDown(nFlags, point);
}

void CKey::OnLButtonUp(const UINT nFlags, const CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_nKeyType == PUSHED_KEY) {
		SetKeyType(NORMAL_KEY);
	}

	if (m_nKeyType == REMAPPED_PUSHED_KEY) {
		if (m_nKey == GetPointedKey(point)) {
			KeyboardLayout_t *pKeyboardLayout = CProfile::GetKeyboardLayouts(m_nKey);
			if (pKeyboardLayout) {
				CString szWindowText;
				GetParent()->GetDlgItem(pKeyboardLayout->nBaseControlID)->GetWindowText(szWindowText);
				SetWindowText(szWindowText);

				if (CProfile::Is106Keyboard()) {
					((C109Keyboard*)GetParent())->m_ToolTip.UpdateTipText(CProfile::GetToolTipID(pKeyboardLayout->nToolTipID), this);
				} else {
					((C104Keyboard*)GetParent())->m_ToolTip.UpdateTipText(CProfile::GetToolTipID(pKeyboardLayout->nToolTipID), this);
				}

				ScanCodeMapping_t ScanCodeMapping = {{0, 0}, {pKeyboardLayout->scancode.nScanCode, pKeyboardLayout->scancode.nPrefixedScanCode}};
				CProfile::SetScanCodeMap(m_HkeyType, ScanCodeMapping);
			}
			SetKeyType(ORIGINAL_KEY);
		} else {
			SetKeyType(REMAPPED_KEY);
		}
	}

	int nPointedKey = GetPointedKey(point);
	if (m_nDroppableKey && nPointedKey) {
		if (m_nDroppableKey != nPointedKey) {
			((CKey*)GetParent()->GetDlgItem(m_nDroppableKey))->SetKeyType(m_nDroppableKeyType);
			if (IsDroppableKey(nPointedKey)) {
				m_nDroppableKey = nPointedKey;
			} else {
				m_nDroppableKey = 0;
			}
		}

		if (m_nDroppableKey) {
			KeyboardLayout_t *pKeyboardLayout = CProfile::GetKeyboardLayouts(m_nDroppableKey);
			KeyboardLayout_t *pBaseKeyboardLayout = CProfile::GetKeyboardLayouts(m_nKey);
			if (pKeyboardLayout && pBaseKeyboardLayout) {
				CString szWindowText;
				GetWindowText(szWindowText);
				GetParent()->GetDlgItem(pKeyboardLayout->nCurrentControlID)->SetWindowText(szWindowText);

				if (CProfile::Is106Keyboard()) {
					((C109Keyboard*)GetParent())->m_ToolTip.UpdateTipText(CProfile::GetToolTipID(pBaseKeyboardLayout->nToolTipID), GetParent()->GetDlgItem(pKeyboardLayout->nCurrentControlID));
				} else {
					((C104Keyboard*)GetParent())->m_ToolTip.UpdateTipText(CProfile::GetToolTipID(pBaseKeyboardLayout->nToolTipID), GetParent()->GetDlgItem(pKeyboardLayout->nCurrentControlID));
				}

				((CKey*)GetParent()->GetDlgItem(pKeyboardLayout->nBaseControlID))->SetKeyType(NORMAL_KEY);
				((CKey*)GetParent()->GetDlgItem(pKeyboardLayout->nCurrentControlID))->SetKeyType(REMAPPED_KEY);

				ScanCodeMapping_t ScanCodeMapping = {{pBaseKeyboardLayout->scancode.nScanCode, pBaseKeyboardLayout->scancode.nPrefixedScanCode}, 
													 {pKeyboardLayout->scancode.nScanCode, pKeyboardLayout->scancode.nPrefixedScanCode}};
				CProfile::SetScanCodeMap(m_HkeyType, ScanCodeMapping);
			}
		}
	}

	m_nDroppableKey = 0;
	m_nDroppableKeyType = NORMAL_KEY;

	CProfile::SetNormalCursor();

	CButton::OnLButtonUp(nFlags, point);
}

int CKey::GetPointedKey(const CPoint point)
{
	for (int i = 0; i < sizeof(KeyboardLayouts) / sizeof(KeyboardLayouts[0]); ++i) {
		if (!GetParent()->GetDlgItem(KeyboardLayouts[i].nBaseControlID)
		 || !GetParent()->GetDlgItem(KeyboardLayouts[i].nCurrentControlID)) {
			continue;
		}

		CRect rButton;
		GetParent()->GetDlgItem(KeyboardLayouts[i].nBaseControlID)->GetWindowRect(&rButton);
		ScreenToClient(&rButton);
		if (rButton.PtInRect(point)) {
			return KeyboardLayouts[i].nBaseControlID;
		}
		GetParent()->GetDlgItem(KeyboardLayouts[i].nCurrentControlID)->GetWindowRect(&rButton);
		ScreenToClient(&rButton);
		if (rButton.PtInRect(point)) {
			return KeyboardLayouts[i].nCurrentControlID;
		}
	}
	return 0;
}

void CKey::OnMouseMove(const UINT nFlags, const CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (nFlags & MK_LBUTTON) {
		if (m_nKeyType == PUSHED_KEY) {
			int nPointedKey = GetPointedKey(point);
			if (nPointedKey) {
				if (nPointedKey != m_nDroppableKey) {
					if (m_nDroppableKey) {
						((CKey*)GetParent()->GetDlgItem(m_nDroppableKey))->SetKeyType(m_nDroppableKeyType);
						CProfile::SetNoCursor();
					}
					if (IsDroppableKey(nPointedKey)) {
						m_nDroppableKey = nPointedKey;
						m_nDroppableKeyType = ((CKey*)GetParent()->GetDlgItem(m_nDroppableKey))->GetKeyType();
						((CKey*)GetParent()->GetDlgItem(m_nDroppableKey))->SetKeyType(DROPPABLE_KEY);
						CProfile::SetDraggingCursor();
					}
				}
			} else {
				if (m_nDroppableKey) {
					((CKey*)GetParent()->GetDlgItem(m_nDroppableKey))->SetKeyType(m_nDroppableKeyType);
					m_nDroppableKey = 0;
					m_nDroppableKeyType = NORMAL_KEY;
					CProfile::SetNoCursor();
				}
			}
		}

		if (m_nKeyType == REMAPPED_PUSHED_KEY) {
			if (m_nKey != GetPointedKey(point)) {
				SetKeyType(REMAPPED_KEY);
			}
		}
		if (m_nKeyType == REMAPPED_KEY) {
			if (m_nKey == GetPointedKey(point)) {
				SetKeyType(REMAPPED_PUSHED_KEY);
			}
		}
	}
	
	CButton::OnMouseMove(nFlags, point);
}

BOOL CKey::IsDroppableKey(const int nKey)
{
	if (m_nKeyType != PUSHED_KEY
	 || !nKey) {
		return FALSE;
	}

	for (int i = 0; i < sizeof(KeyboardLayouts) / sizeof(KeyboardLayouts[0]); ++i) {
		if (KeyboardLayouts[i].nBaseControlID != m_nKey) {
			continue;
		}

		if (KeyboardLayouts[i].nBaseControlID == nKey
		 || KeyboardLayouts[i].nCurrentControlID == nKey) {
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}
