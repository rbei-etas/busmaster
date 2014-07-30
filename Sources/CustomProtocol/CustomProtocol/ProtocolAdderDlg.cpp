// ProtocolAdderDlg.cpp : implementation file
//
#pragma once

#include "CustomProtocol_stdafx.h"
#include "CustomProtocol.h"
#include "ProtocolAdderDlg.h"

#define def_HEADERNAME_COL		0
#define def_CHECKSUM_COL		1
#define def_NO_OF_BYTES_COL		2

// CProtocolAdderDlg dialog

IMPLEMENT_DYNAMIC(CProtocolAdderDlg, CDialog)

CProtocolAdderDlg::CProtocolAdderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProtocolAdderDlg::IDD, pParent)
{

}

CProtocolAdderDlg::~CProtocolAdderDlg()
{
}

void CProtocolAdderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LSTC_PROTOCOL_HEADERS, m_omAddHeaderList);
}


BEGIN_MESSAGE_MAP(CProtocolAdderDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_ADD_HEADER, OnBnClickedBtnAddHeader)
END_MESSAGE_MAP()


// CProtocolAdderDlg message handlers
BOOL CProtocolAdderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_omAddHeaderList.SubclassDlgItem(IDC_LSTC_PROTOCOL_HEADERS, this);
    m_omAddHeaderList.SetExtendedStyle( LVS_EX_FULLROWSELECT |  LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES);

	CRect rctList;
    m_omAddHeaderList.GetWindowRect(&rctList);
    int nColWidth = rctList.Width() - 2;
    int colwidths[] = { 20, 10, 10};
    int total_cx = 0;

    TCHAR* acColData[]=
    {
        _T("Header Name"), _T("Checksum"), _T("Number of Bytes")
    };

    LVCOLUMN lvcolumn;
    memset(&lvcolumn, 0, sizeof(lvcolumn));
    lvcolumn.mask =  LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;

	if( m_omHeaderImageList.m_hImageList != NULL )
    {
        m_omAddHeaderList.SetImageList(&m_omHeaderImageList, LVSIL_SMALL);
        CHeaderCtrl* pHeader = m_omAddHeaderList.GetHeaderCtrl();
        if( pHeader != NULL )
        {
            pHeader->SetImageList( &m_omHeaderImageList );
            HDITEM hditem;
            hditem.mask = HDI_IMAGE | HDI_FORMAT;
            if( pHeader->GetItem(0, &hditem ) == TRUE )
            {
                hditem.fmt |=  HDF_IMAGE;
                hditem.iImage = 0;
                pHeader->SetItem(0, &hditem );
            }
        }
    }

	for (int i = 0; i < (sizeof (acColData) / sizeof acColData[0]); i++)
    {
        lvcolumn.fmt = LVCFMT_CENTER;
        
        lvcolumn.pszText = acColData[i];
        lvcolumn.cchTextMax = (int)strlen(acColData[i]);
        lvcolumn.iSubItem = i;
        lvcolumn.cx = (acColData[i + 1] == NULL) ? nColWidth - total_cx - 2 : (nColWidth * colwidths[i]) / 64;
        total_cx += lvcolumn.cx;

        m_omAddHeaderList.InsertColumn(i, &lvcolumn);
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CProtocolAdderDlg::OnBnClickedOk()
{
	SBASEPROTOCOL   sBaseProtocol;
	string strTemp;
	char chName[25];
	((CEdit*)GetDlgItem(IDC_EDIT_APP_PROTOCOL_NAME))->GetWindowTextA(chName, 100);
	sBaseProtocol.strName = chName;
	((CEdit*)GetDlgItem(IDC_EDIT_HEADER_COUNT))->GetWindowTextA(chName, 100);
	sBaseProtocol.nHeaderCount = atoi(chName);

	for(int i =0; i < sBaseProtocol.nHeaderCount; i++)
	{
		strTemp = m_omAddHeaderList.GetItemText(i, def_HEADERNAME_COL);
		sBaseProtocol.sHeaders[i].strName = strTemp;

		strTemp = m_omAddHeaderList.GetItemText(i, def_NO_OF_BYTES_COL);
		sBaseProtocol.sHeaders[i].nHeaderLength = atoi(strTemp.c_str());
	}

	CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_BaseProtocolList.push_back(sBaseProtocol);

	CDialog::OnOK();
}

int CProtocolAdderDlg::nAddMessageToList(int nRow)
{
	//int  nRow = m_omAddHeaderList.GetItemCount();
	m_omAddHeaderList.InsertItem( nRow, STR_EMPTY);
	if( nRow != -1 )
	{
		SLISTINFO ouListInfo;

		m_omAddHeaderList.SetItemState( nRow, def_HEADERNAME_COL, LVIS_SELECTED | LVIS_FOCUSED );
		ouListInfo.m_eType = eText;
		LVITEM lvitem = {0};
		lvitem.mask =  LVIF_IMAGE;
		lvitem.pszText = "";
		lvitem.iImage = 2 ; // Index of first subitem image
		lvitem.iItem = nRow; // Index of item
		lvitem.iSubItem = 0; // Index of subitem
		
		m_omAddHeaderList.vSetColumnInfo(nRow, def_HEADERNAME_COL, ouListInfo);
		m_omAddHeaderList.SetItem(&lvitem);
		m_omAddHeaderList.SetItemText( nRow, def_HEADERNAME_COL, "Header");

		// Init Raw Value column
		// Column 1 :  Edit  Control
		ouListInfo.m_eType = eText;
		m_omAddHeaderList.vSetColumnInfo(nRow, def_CHECKSUM_COL, ouListInfo);
		m_omAddHeaderList.SetItemText( nRow, def_CHECKSUM_COL, "" );

		sUserProgInfo ouUserProgInfo;
		ouUserProgInfo.m_bIcon = true;
		ouUserProgInfo.m_pfHandler = vCheckBoxClickHandler;
		ouUserProgInfo.m_pUserParam = this;

		m_omAddHeaderList.vSetUserProgInfo(nRow, 1, ouUserProgInfo);

		// Init Phy Value column
		// Column 2 :  Edit Control
		ouListInfo.m_eType = eText;
		m_omAddHeaderList.vSetColumnInfo(nRow, def_NO_OF_BYTES_COL, ouListInfo);
		m_omAddHeaderList.SetItemText( nRow, def_NO_OF_BYTES_COL, "1");

		//LVITEM lvitem = {0};
		lvitem.mask =  LVIF_IMAGE;
		lvitem.pszText = "";
		lvitem.iImage = 0 ; // Index of first subitem image
		lvitem.iItem = nRow; // Index of item
		lvitem.iSubItem = 1; // Index of subitem
		m_omAddHeaderList.SetItem(&lvitem);
		m_omAddHeaderList.SetCheck(nRow, 0);
	}

	m_omAddHeaderList.EnableWindow( TRUE );
	return TRUE;
}

void CProtocolAdderDlg::OnBnClickedBtnAddHeader()
{
	nAddMessageToList(m_omAddHeaderList.GetItemCount());
}

void CProtocolAdderDlg::vCheckBoxClickHandler( CListCtrl* pThis, int nItem, int nSubItem, void* uParam)
{
    CString str;
    str.Format("Row %d Column %d", nItem, nSubItem );
    LVITEM lvitem = {0};
    lvitem.mask =  LVIF_IMAGE;
    lvitem.iItem = nItem;
    lvitem.iSubItem = nSubItem;

    ((CListCtrl*)pThis)->GetItem(&lvitem);


    lvitem.mask =  LVIF_IMAGE;
    lvitem.pszText = "";

    //BOOL bEnable = FALSE;
    //eMsgUpdate eUpdate;
    //if ( nSubItem == 1 )
    //{
    //    eUpdate = eKeyCheck;
    //}

    if ( lvitem.iImage == 0 )
    {
        lvitem.iImage = 1;
        //bEnable = TRUE;
    }
    else
    {
        lvitem.iImage = 0;
        //bEnable = FALSE;
    }


    ((CListCtrl*)pThis)->SetItem(&lvitem);
}