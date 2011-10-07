// Key.cpp : implementation file
//

#include "key.h"
#include "keyboardlayout.h"
#include "profile.h"

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
	switch (m_nKeyType) {
	case NORMAL_KEY:
		SetKeyType(PUSHED_KEY);
		SetDraggingCursor();
		break;
	case REMAPPED_KEY:
		SetKeyType(REMAPPED_PUSHED_KEY);
		break;
	}

	CButton::OnLButtonDown(nFlags, point);
}

void CKey::OnLButtonUp(const UINT nFlags, CPoint point) 
{
	if (m_nKeyType == PUSHED_KEY) {
		SetKeyType(NORMAL_KEY);
	}

	CKeyboardLayout *parent = reinterpret_cast<CKeyboardLayout *>(GetParent());
	ClientToScreen(&point);
	if (m_nKeyType == REMAPPED_PUSHED_KEY) {
		if (m_nKey == parent->GetPointedKey(point)) {
			KeyboardLayout *pKeyboardLayout = parent->GetKeyboardLayout(m_nKey);
			if (pKeyboardLayout) {
				CString szWindowText;
				parent->GetDlgItem(pKeyboardLayout->nBaseControlID)->GetWindowText(szWindowText);
				SetWindowText(szWindowText);

				parent->ToolTip()->UpdateTipText(parent->GetToolTipID(pKeyboardLayout->nToolTipID), this);

				ScanCodeMapping mapping = {{0, 0}, {pKeyboardLayout->scancode.nScanCode, pKeyboardLayout->scancode.nPrefix}};
				parent->SetScanCodeMap(m_HkeyType, mapping);
			}
			SetKeyType(ORIGINAL_KEY);
		} else {
			SetKeyType(REMAPPED_KEY);
		}
	}

	int nPointedKey = parent->GetPointedKey(point);
	if (m_nDroppableKey && nPointedKey) {
		if (m_nDroppableKey != nPointedKey) {
			reinterpret_cast<CKey*>(parent->GetDlgItem(m_nDroppableKey))->SetKeyType(m_nDroppableKeyType);
			if (IsDroppableKey(nPointedKey)) {
				m_nDroppableKey = nPointedKey;
			} else {
				m_nDroppableKey = 0;
			}
		}

		if (m_nDroppableKey) {
			KeyboardLayout *pKeyboardLayout = parent->GetKeyboardLayout(m_nDroppableKey);
			KeyboardLayout *pBaseKeyboardLayout = parent->GetKeyboardLayout(m_nKey);
			if (pKeyboardLayout && pBaseKeyboardLayout) {
				CString szWindowText;
				GetWindowText(szWindowText);
				parent->GetDlgItem(pKeyboardLayout->nCurrentControlID)->SetWindowText(szWindowText);
				parent->ToolTip()->UpdateTipText(parent->GetToolTipID(pBaseKeyboardLayout->nToolTipID), parent->GetDlgItem(pKeyboardLayout->nCurrentControlID));
				reinterpret_cast<CKey *>(parent->GetDlgItem(pKeyboardLayout->nBaseControlID))->SetKeyType(NORMAL_KEY);
				reinterpret_cast<CKey*>(parent->GetDlgItem(pKeyboardLayout->nCurrentControlID))->SetKeyType(REMAPPED_KEY);

				ScanCodeMapping mapping = {{pBaseKeyboardLayout->scancode.nScanCode, pBaseKeyboardLayout->scancode.nPrefix}, 
											{pKeyboardLayout->scancode.nScanCode, pKeyboardLayout->scancode.nPrefix}};
				parent->SetScanCodeMap(m_HkeyType, mapping);
			}
		}
	}

	m_nDroppableKey = 0;
	m_nDroppableKeyType = NORMAL_KEY;

	SetNormalCursor();

	CButton::OnLButtonUp(nFlags, point);
}

void CKey::OnMouseMove(const UINT nFlags, CPoint point) 
{
	if (nFlags & MK_LBUTTON) {
		ClientToScreen(&point);
		CKeyboardLayout *parent = (CKeyboardLayout *)GetParent();
		if (m_nKeyType == PUSHED_KEY) {
			int nPointedKey = parent->GetPointedKey(point);
			if (nPointedKey) {
				if (nPointedKey != m_nDroppableKey) {
					if (m_nDroppableKey) {
						reinterpret_cast<CKey*>(parent->GetDlgItem(m_nDroppableKey))->SetKeyType(m_nDroppableKeyType);
						SetNoCursor();
					}
					if (IsDroppableKey(nPointedKey)) {
						m_nDroppableKey = nPointedKey;
						m_nDroppableKeyType = ((CKey*)parent->GetDlgItem(m_nDroppableKey))->GetKeyType();
						reinterpret_cast<CKey*>(parent->GetDlgItem(m_nDroppableKey))->SetKeyType(DROPPABLE_KEY);
						SetDraggingCursor();
					}
				}
			} else {
				if (m_nDroppableKey) {
					reinterpret_cast<CKey*>(parent->GetDlgItem(m_nDroppableKey))->SetKeyType(m_nDroppableKeyType);
					m_nDroppableKey = 0;
					m_nDroppableKeyType = NORMAL_KEY;
					SetNoCursor();
				}
			}
		}

		if (m_nKeyType == REMAPPED_PUSHED_KEY) {
			if (m_nKey != parent->GetPointedKey(point)) {
				SetKeyType(REMAPPED_KEY);
			}
		}
		if (m_nKeyType == REMAPPED_KEY) {
			if (m_nKey == parent->GetPointedKey(point)) {
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

	KeyboardLayout *pKeyboardLayout = reinterpret_cast<CKeyboardLayout *>(GetParent())->GetKeyboardLayout(m_nKey, TRUE);
	if (!pKeyboardLayout
	 || pKeyboardLayout->nBaseControlID == nKey
	 || pKeyboardLayout->nCurrentControlID == nKey) {
		return FALSE;
	}
	return TRUE;
}

void CKey::SetDraggingCursor()
{
	HCURSOR hCursor = (HCURSOR)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_DRAG_CURSOR),
										 IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	::SetCursor(hCursor);
}

void CKey::SetNormalCursor()
{
	HCURSOR hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW),
										 IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	::SetCursor(hCursor);
}

void CKey::SetNoCursor()
{
	HCURSOR hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_NO),
										 IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	::SetCursor(hCursor);
}
