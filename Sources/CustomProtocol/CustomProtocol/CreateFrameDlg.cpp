// CreateFrameDlg.cpp : implementation file
//

#include "CustomProtocol_stdafx.h"
#include "CustomProtocol.h"
#include "CreateFrameDlg.h"
#include "CustomTxDataStore.h"


// CCreateFrameDlg dialog
CTxCustomDataStore CTxCustomDataStore::m_sTxEthernetDataStoreObj;

IMPLEMENT_DYNAMIC(CCreateFrameDlg, CDialog)

CCreateFrameDlg::CCreateFrameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateFrameDlg::IDD, pParent)
{
	m_SelectedDerivedProtocolIndex = -1;
}

CCreateFrameDlg::~CCreateFrameDlg()
{
}

void CCreateFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LSTC_CREATE_FRAME, m_CreateFrameListc);
}


BEGIN_MESSAGE_MAP(CCreateFrameDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_DELETE_PROTOCOL, OnBnClickedBtnDeleteProtocol)
	ON_CBN_SELCHANGE(IDC_COMBO_ADD_BASE_PROTOCOL, OnCbnSelchangeComboAddBaseProtocol)
END_MESSAGE_MAP()


// CCreateFrameDlg message handlers
BOOL CCreateFrameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CreateFrameListc.SubclassDlgItem(IDC_LSTC_CREATE_FRAME, this);
    m_CreateFrameListc.SetExtendedStyle( LVS_EX_FULLROWSELECT |  LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES);

	CRect rctList;
    m_CreateFrameListc.GetWindowRect(&rctList);
    int nColWidth = rctList.Width() - 2;
    int colwidths[] = { 20, 20, 20, 40};
    int total_cx = 0;

    TCHAR* acColData[]=
    {
        _T("Protocol Name"), _T("Header Name"), _T("Number of Bytes"), _T("Headers")
    };

    LVCOLUMN lvcolumn;
    memset(&lvcolumn, 0, sizeof(lvcolumn));
    lvcolumn.mask =  LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;

	for (int i = 0; i < (sizeof (acColData) / sizeof acColData[0]); i++)
    {
        lvcolumn.fmt = LVCFMT_CENTER;
        
        lvcolumn.pszText = acColData[i];
        lvcolumn.cchTextMax = (int)strlen(acColData[i]);
        lvcolumn.iSubItem = i;
        lvcolumn.cx = (acColData[i + 1] == NULL) ? nColWidth - total_cx - 2 : (nColWidth * colwidths[i]) / 64;
        total_cx += lvcolumn.cx;

        m_CreateFrameListc.InsertColumn(i, &lvcolumn);
    }

	//Adding base protocols to base protocol combo
	list<SBASEPROTOCOL>::iterator itrList = CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_BaseProtocolList.begin();
	while( itrList !=  CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_BaseProtocolList.end() )
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_ADD_BASE_PROTOCOL))->AddString(itrList->strName.c_str());
		itrList++;
	}

	//Adding derived protocols to derived protocol combo
	list<SDERIVEDPROTOCOL>::iterator itrListDerived = CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_DerivedProtocolList.begin();
	while( itrListDerived !=  CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_DerivedProtocolList.end() )
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_ADD_BASE_PROTOCOL))->AddString(itrListDerived->strName.c_str());
		itrListDerived++;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCreateFrameDlg::OnBnClickedOk()
{
	//Onclick save button this function will be called
	SDERIVEDPROTOCOL sDerivedProtocol;
	char chName[100];
	((CComboBox*)GetDlgItem(IDC_COMBO_ADD_BASE_PROTOCOL))->GetWindowTextA(chName, 100);
	sDerivedProtocol.strName = chName;

	//Get base protocol details from list ctrl
	list<SBASEPROTOCOL>::iterator itrList = CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_BaseProtocolList.begin();
	for(int i =0; i< m_CreateFrameListc.GetItemCount(); i++)
	{
		string strProtocolName  = m_CreateFrameListc.GetItemText(i, 0);
		if(strProtocolName.length() != 0)
		{
			while( itrList !=  CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_BaseProtocolList.end() )
			{
				if(itrList->strName.compare(strProtocolName) == 0)
				{
					AddHeaderValuesToBaseProtocol(*itrList, i);
					sDerivedProtocol.BaseProtocol[sDerivedProtocol.nBaseProtocolCnt] = (*itrList);
					sDerivedProtocol.nBaseProtocolCnt++;
				}
				itrList++;
			}
		}
	}

	//Add the new derived protocol to derived protocol list
	m_SelectedDerivedProtocolIndex = CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_DerivedProtocolList.size();
	CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_DerivedProtocolList.push_back(sDerivedProtocol);
	CDialog::OnOK();
}

void CCreateFrameDlg::AddHeaderValuesToBaseProtocol(SBASEPROTOCOL& sBaseProtocol, int nRow)
{
	//Add header values in BYTEs to all the headers of Base protocol
	for(int i= 0; i < sBaseProtocol.nHeaderCount; i++)
	{
		AddHeaderValues(sBaseProtocol.sHeaders[i], nRow);
	}
}

void CCreateFrameDlg::AddHeaderValues(SHEADER& sHeader, int nRow)
{
	string strHeaderVal = m_CreateFrameListc.GetItemText(nRow, 3);

	strcpy(sHeader.chValue, strHeaderVal.c_str());
}

void CCreateFrameDlg::OnBnClickedBtnDeleteProtocol()
{
	// TODO: Add your control notification handler code here
}

void CCreateFrameDlg::OnCbnSelchangeComboAddBaseProtocol()
{
	int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_ADD_BASE_PROTOCOL))->GetCurSel();
	if(nSel != -1)
	{
		list<SBASEPROTOCOL>::iterator itrList = CTxCustomDataStore::ouGetTxEthernetDataStoreObj().m_BaseProtocolList.begin();
		std::advance(itrList, nSel);
		for(int nCnt =0; nCnt <itrList->nHeaderCount; nCnt++ )
		{
			nAddMessageToList(*itrList,nCnt);
		}
	}
}

int CCreateFrameDlg::nAddMessageToList( SBASEPROTOCOL sBaseProtocol, int nHeaderIndex )
{
	int  nRow = m_CreateFrameListc.GetItemCount();
	m_CreateFrameListc.InsertItem( nRow, STR_EMPTY);
	if( nRow != -1 )
	{
		SLISTINFO ouListInfo;

		m_CreateFrameListc.SetItemState( nRow, 0, LVIS_SELECTED | LVIS_FOCUSED );
		ouListInfo.m_eType = eText;
		LVITEM lvitem = {0};
		
		m_CreateFrameListc.vSetColumnInfo(nRow, 0, ouListInfo);
		m_CreateFrameListc.SetItem(&lvitem);

		//Add the base protocol name only for the first row
		if(nHeaderIndex == 0)
		{
			m_CreateFrameListc.SetItemText( nRow, 0, sBaseProtocol.strName.c_str());
		}
		else
		{
			m_CreateFrameListc.SetItemText( nRow, 0, "");
		}

		// Init Raw Value column
		// Column 1 :  Edit  Control
		ouListInfo.m_eType = eText;
		m_CreateFrameListc.vSetColumnInfo(nRow, 1, ouListInfo);
		m_CreateFrameListc.SetItemText( nRow, 1, sBaseProtocol.sHeaders[nHeaderIndex].strName.c_str() );

		// Init Phy Value column
		// Column 2 :  Edit Control
		char  chBytes[5];
		ouListInfo.m_eType = eText;
		m_CreateFrameListc.vSetColumnInfo(nRow, 2, ouListInfo);
		m_CreateFrameListc.SetItemText( nRow, 2, _itoa(sBaseProtocol.sHeaders[nHeaderIndex].nHeaderLength, chBytes,10 ));

		string str = "";
		for(int i = 0; i< sBaseProtocol.sHeaders[nHeaderIndex].nHeaderLength; i++ )
		{
			str.append("00 ");
		}
		ouListInfo.m_eType = eText;
		m_CreateFrameListc.vSetColumnInfo(nRow, 3, ouListInfo);
		m_CreateFrameListc.SetItemText( nRow, 3, str.c_str());

	}

	m_CreateFrameListc.EnableWindow( TRUE );
	return TRUE;
}