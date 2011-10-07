// PropertiesList.cpp : implementation file
//

#include "propertieslist.h"
#include "profile.h"
#include "../xkeymacsdll/Commands.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertiesList property page

BOOL CPropertiesList::m_bSortAscending = TRUE;

IMPLEMENT_DYNCREATE(CPropertiesList, CPropertyPage)

CPropertiesList::CPropertiesList() : CPropertyPage(CPropertiesList::IDD)
{
	//{{AFX_DATA_INIT(CPropertiesList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPropertiesList::~CPropertiesList()
{
}

void CPropertiesList::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertiesList)
	DDX_Control(pDX, IDC_PROPERTIES_LIST, m_cPropertiesList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertiesList, CPropertyPage)
	//{{AFX_MSG_MAP(CPropertiesList)
	ON_WM_CREATE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_PROPERTIES_LIST, OnColumnclickPropertiesList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesList message handlers

BOOL CPropertiesList::OnSetActive() 
{
	m_pProperties->EnableControl(LIST_TAB);
	SetDialogData();

	return CPropertyPage::OnSetActive();
}

void CPropertiesList::SetDialogData()
{
	m_cPropertiesList.DeleteAllItems();
	m_nCategoryWidth = 0;
	m_nCommandWidth = 0;
	m_nKeyWidth = 0;

	const int nAppID = m_pProperties->GetApplicationID();
	for (int nComID = 1; nComID < MAX_COMMAND; ++nComID) {
		const LPCSTR szComName = CCommands::GetCommandName(nComID);
		if (!szComName[0])
			break;
		const CString category(MAKEINTRESOURCE(CCommands::GetCategoryID(nComID)));
		if (category.IsEmpty())
			continue;
		bool bInserted = false;
		for (int nType = 0; nType < MAX_COMMAND_TYPE; ++nType) {
			for (int nKey = 0; nKey < MAX_KEY; ++nKey)
				if (nComID == CProfile::GetCommandID(nAppID, nType, nKey)) {
					CString key;
					key.Format(_T("%s%s"), CProfile::CommandType2String(nType), CProfile::Key2String(nKey));
					InsertItem(category, szComName, key);
					bInserted = true;
				}
		}
		if (!bInserted)
			InsertItem(category, szComName, _T(""));
	}
	SortItem(m_nSelectedColumn);
	m_cPropertiesList.SetColumnWidth(0, m_nCategoryWidth + 0x20);
	m_cPropertiesList.SetColumnWidth(1, m_nCommandWidth + 0x20);
	m_cPropertiesList.SetColumnWidth(2, m_nKeyWidth + 0x20);

	UpdateData(FALSE);
}

void CPropertiesList::GetDialogData()
{
	UpdateData(TRUE);
}

int CPropertiesList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pProperties = (CProperties *)GetParent()->GetParent();

	return 0;
}

BOOL CPropertiesList::OnKillActive() 
{
	GetDialogData();
	return CPropertyPage::OnKillActive();
}

void CPropertiesList::EnableControl()
{
	BOOL bEnable = m_pProperties->IsEnableControl();

	m_cPropertiesList.EnableWindow(bEnable);
}

BOOL CPropertiesList::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	DWORD dwStyle = m_cPropertiesList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_cPropertiesList.SetExtendedStyle(dwStyle);

	m_nSelectedColumn = 0;
	m_bSortAscending = TRUE;

	m_cPropertiesList.InsertColumn(0, CString(MAKEINTRESOURCE(IDS_CATEGORY)), LVCFMT_LEFT);
	m_cPropertiesList.InsertColumn(1, CString(MAKEINTRESOURCE(IDS_COMMAND)), LVCFMT_LEFT);
	m_cPropertiesList.InsertColumn(2, CString(MAKEINTRESOURCE(IDS_KEY)), LVCFMT_LEFT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CALLBACK CPropertiesList::SortCategory(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	return Sort(lParam1, lParam2, lParamSort, SORT_CATEGORY);
}

int CALLBACK CPropertiesList::SortCommand(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	return Sort(lParam1, lParam2, lParamSort, SORT_COMMAND);
}

int CALLBACK CPropertiesList::SortKey(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	return Sort(lParam1, lParam2, lParamSort, SORT_KEY);
}

void CPropertiesList::OnColumnclickPropertiesList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (m_nSelectedColumn == pNMListView->iSubItem) {
		m_bSortAscending = !m_bSortAscending;
	} else {
		m_nSelectedColumn = pNMListView->iSubItem;
		m_bSortAscending = TRUE;
	}

	for (int nItem = 0; nItem < m_cPropertiesList.GetItemCount(); ++nItem) {
		m_cPropertiesList.SetItemData(nItem, nItem);
	}

	SortItem(pNMListView->iSubItem);

	*pResult = 0;
}

void CPropertiesList::DisplayIndicator()
{
	CHeaderCtrl *pHeaderCtrl = m_cPropertiesList.GetHeaderCtrl();

	for (int nItemCount = 0; nItemCount < pHeaderCtrl->GetItemCount(); ++nItemCount) {
		HDITEM hditem;
		if (nItemCount == m_nSelectedColumn) {
			hditem.mask = HDI_FORMAT | HDI_BITMAP;
			hditem.fmt = HDF_STRING | HDF_BITMAP | HDF_BITMAP_ON_RIGHT;
			const int IDB_SORT = m_bSortAscending ? IDB_SORT_ASCENDING : IDB_SORT_DESCENDING;
			hditem.hbm = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SORT), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);
			pHeaderCtrl->SetItem(nItemCount, &hditem);
		} else {
			hditem.mask = HDI_FORMAT;
			hditem.fmt = HDF_STRING;
			hditem.hbm = NULL;
			pHeaderCtrl->SetItem(nItemCount, &hditem);
		}
	}
}

void CPropertiesList::SortItem(int nColumn)
{
	switch (nColumn) {
	case 0:
		m_cPropertiesList.SortItems(CPropertiesList::SortCategory, (LPARAM) &m_cPropertiesList);
		break;
	case 1:
		m_cPropertiesList.SortItems(CPropertiesList::SortCommand, (LPARAM) &m_cPropertiesList);
		break;
	case 2:
		m_cPropertiesList.SortItems(CPropertiesList::SortKey, (LPARAM) &m_cPropertiesList);
		break;
	default:
		m_cPropertiesList.SortItems(CPropertiesList::SortCategory, (LPARAM) &m_cPropertiesList);
		break;
	}

	DisplayIndicator();
}

void CPropertiesList::InsertItem(LPCTSTR szCategory, LPCTSTR szCommandName, LPCTSTR szKey)
{
	int nItem = m_cPropertiesList.InsertItem(m_cPropertiesList.GetItemCount(), szCategory);
	m_cPropertiesList.SetItemData(nItem, nItem);
	m_cPropertiesList.SetItemText(nItem, 1, szCommandName);
	m_cPropertiesList.SetItemText(nItem, 2, szKey);

	m_nCategoryWidth = __max(m_nCategoryWidth, m_cPropertiesList.GetStringWidth(szCategory));
	m_nCommandWidth = __max(m_nCommandWidth, m_cPropertiesList.GetStringWidth(szCommandName));
	m_nKeyWidth = __max(m_nKeyWidth, m_cPropertiesList.GetStringWidth(szKey));
}

int CPropertiesList::Sort(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort, SORT_TYPE sortType)
{
	CListCtrl *pListCtrl = (CListCtrl *) lParamSort;

	CString szCategory1 = pListCtrl->GetItemText(lParam1, 0);
	CString szCommand1 = pListCtrl->GetItemText(lParam1, 1);
	CString szKey1 = pListCtrl->GetItemText(lParam1, 2);

	CString szCategory2 = pListCtrl->GetItemText(lParam2, 0);
	CString szCommand2 = pListCtrl->GetItemText(lParam2, 1);
	CString szKey2 = pListCtrl->GetItemText(lParam2, 2);

	int rc = 0;
	switch (sortType) {
	case SORT_CATEGORY:
		rc = rc ? rc : _tcscmp(szCategory1, szCategory2);
		// Do NOT write break; here.
	case SORT_COMMAND:
		rc = rc ? rc : _tcscmp(szCommand1, szCommand2);
		// Do NOT write break; here.
	case SORT_KEY:
		if (!rc) {
			rc = rc ? rc : _tcscmp(szKey1, szKey2);
			if (szKey1.IsEmpty() && !szKey2.IsEmpty()) {
				rc = 1;
			} else if (!szKey1.IsEmpty() && szKey2.IsEmpty()) {
				rc = -1;
			}
		}
		break;
	default:
		break;
	}
	return m_bSortAscending ? rc : rc * -1;
}
