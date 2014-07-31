///***********************************************************************************
//Project           :   Frame
//FileName      :   TransmitMsg.cpp
//Description       :   Implementation of CTransmitMsgEthernet class
//Directory     :
//Version           :   V1.0
//Authors           :   Ravi D
//Date Created  :   24/10/2006
//Date Modified :
//Modifications :
//**********************************************************************************/
#pragma once
#include "TxWindowEthernet_stdafx.h"
//#include "DataTypes/Base_LIN_Buffer.h"
#include "EthernetTxDefaultConfigDlg.h"
#include "Utility/Utility.h"
#include "HashDefines.h"
#include "..\DIL_Interface\BaseDIL_Ethernet.h"
#include "..\DIL_Interface\DIL_Interface_extern.h"
#include "TxMsgChildFrame.h"
#include "TransmitMsgEthernet.h"
#include "..\DataTypes\MsgSignal_Datatypes.h"




const BYTE COLUMN_MSGID     = 0x02;
const BYTE COLUMN_DESCR     = 0x04;
const BYTE COLUMN_CYCLE     = 0x08;
const BYTE COLUMN_CHANNEL   = 0x10;
const BYTE COLUMN_DATALEN   = 0x20;
const BYTE COLUMN_DATABYTE  = 0x40;


#define def_COLUMN_MSGNAME_INDEX                0
#define def_COLUMN_SRC_MAC						1
#define def_COLUMN_DEST_MAC						2
#define def_COLUMN_PROTOCOL						3
#define def_COLUMN_REPETITION_INDEX             4
#define def_COLUMN_KEY_INDEX                    5
#define def_COLUMN_DLC							6
#define def_COLUMN_DATABYTES_INDEX              7
#define defMAX_LMT_LIN_MSG_ID                   63

#define BITLENGTH 16
#define ETH_MAX_DATA_LENGTH						1500
#define TARGET_RESOLUTION 1         // 1-millisecond target resolution
#define defSTR_FORMAT_DISPLAY_DEC    _T("%-60s   [%16I64d]")
#define defSTR_FORMAT_DISPLAY_HEX    _T("%-60s   [%16I64X]")
#define def_MAXIMUM_TIMER_VAL       60000
#define def_MINIMUM_TIMER_VAL       1
HWND CTransmitMsgEthernet::m_hTransmitWnd = NULL;

#define TX_MSG_WND_CONFIG_SECTION_NAME                  "TXWndProperty"
#define def_SEND_Message                                "&Send"
#define def_ADD_MESSAGE_SRING                           "Add &Message"

extern CBaseDIL_ETHERNET* g_pouDIL_Ethernet_Interface;

typedef HRESULT (__stdcall* GET_CUSTOM_PROTOCOL_TX_INTERFACE)(void** ppvInterface);
static GET_CUSTOM_PROTOCOL_TX_INTERFACE pfGetCustomProtocolTxInterface;

IMPLEMENT_DYNCREATE(CTransmitMsgEthernet, CFormView)

BEGIN_MESSAGE_MAP(CTransmitMsgEthernet, CFormView)
    ON_WM_CLOSE()
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    ON_MESSAGE(WM_USER+44, vUserCommand)
	ON_BN_CLICKED(IDC_BTN_ETH_ADD_PACKET, OnBtnClickedAddnew)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBnClickedBtDelFrame)
	ON_NOTIFY(NM_RCLICK, IDC_LSTC_HDR_CONFIG, OnNMTransmitMessagesRightClick)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_HDR_CONFIG, OnNMTransmitMessagesClick)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_HDR_CONFIG, OnItemchangedLstcMsgDetails)
	ON_BN_CLICKED(IDC_BTN_ETH_CONFIGURATION, OnBnClickedBtnEthConfiguration)
	ON_EN_CHANGE(IDC_EDIT_DATA, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_RAW_FRAME, OnEnChangeEditRaw)
	ON_BN_CLICKED(IDC_BTN_PROTOCOL_SETTING, OnBnClickedBtnProtocolSetting)
END_MESSAGE_MAP()

//**********************************************************************************
//Function Name :   CTransmitMsgEthernet
//Input(s)      :   -
//Output        :   -
//Functionality :   Default Constructor
//Member of     :   CTransmitMsgEthernet
//Friend of     :   -
//Authors           :   Ravi D
//Date Created  :   24/10/2006
//Modifications :   -
//************************************************************************************/

CTransmitMsgEthernet::CTransmitMsgEthernet()
    : CFormView(CTransmitMsgEthernet::IDD)
{
    m_lCount					= 0;
    m_lCellPos					= 0;
    m_bInitDone					= FALSE;
    m_bMsgLstInit				= FALSE;
    m_bStartEnabled				= TRUE;
    m_bDisplayColumns			= COLUMN_MSGID | COLUMN_DESCR | COLUMN_CYCLE |
									COLUMN_CHANNEL | COLUMN_DATALEN | COLUMN_DATABYTE;

    m_bModified					= FALSE;
    m_bDataModified				= false;
    m_nSelectedMsgIndex			= -1;
    m_bFromEvent				= false;
	m_bIsDataSetByCode			= false;
	m_bIsRawSetByCode			= false;
	m_pBaseTxAppProtocol		= NULL;

    vInitialiseDataBytes();
}

void CTransmitMsgEthernet::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    int a = 0;
    return ;
}

///**********************************************************************************
//Function Name :   ~CTransmitMsgEthernet
//Input(s)      :   -
//Output            :   -
//Functionality :   Destructor
//Member of     :   CTransmitMsgEthernet
//Friend of     :   -
//Authors           :   Ravi D
//Date Created  :   24/10/2006
//Modifications :   -
//************************************************************************************/

CTransmitMsgEthernet::~CTransmitMsgEthernet()
{
}

//**********************************************************************************
//Function Name :   DoDataExchange
//Input(s)      :   pDX - Pointer to the exchange object
//Output            :   -
//Functionality :   To exchange and validate the dialog data
//Member of     :   CTransmitMsgEthernet
//Friend of     :   -
//Authors           :   Ravi D
//Date Created  :   24/10/2006
//Modifications :   -
//************************************************************************************
void CTransmitMsgEthernet::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CHex_Block_EditorDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LSTC_HDR_CONFIG, m_omHeaderList);
	DDX_Control(pDX, IDC_EDIT_DATA, m_omEditFrameData);
	DDX_Control(pDX, IDC_EDIT_RAW_FRAME, m_omEditRawFrame);
}

LRESULT CTransmitMsgEthernet::vUserCommand(WPARAM wParam, LPARAM lParam)
{
    int nUserSel = 0;
    nUserSel = static_cast <int>(wParam);
    switch(nUserSel)
    {
        case 0:
            UpdateTxView(CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_nChannelsConfigured);
            break;
        case 2:
            OnBusConnect((ESTATUS_BUS)lParam);
            break;
    }
    return S_OK;

}

void CTransmitMsgEthernet::vChangeDelButtonStatus(bool bStatus)
{

}

void CTransmitMsgEthernet::OnBusConnect(ESTATUS_BUS eBusStatus)
{
    CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_eBusStatus = eBusStatus;

    if ( eBusStatus == BUS_CONNECTED)
    {
        vChangeDelButtonStatus(true);
    }
    else
    {
        vChangeDelButtonStatus(false);
    }

}

void CTransmitMsgEthernet::vGetStringFromValue(int nValue, string& strValue, bool bSymbol)
{
    char chValue[1024];
   /* if ( CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_bHexMode == false)
    {
        if ( bSymbol == true )
        {
            sprintf_s(chValue, 1024, "%d", nValue);
        }
        else
        {
            sprintf_s(chValue, 1024, "%d", nValue);
        }
    }
    else
    {
        if ( bSymbol == true )
        {
            sprintf_s(chValue, 1024, "0x%X", nValue);
        }
        else
        {
            sprintf_s(chValue, 1024, "%X", nValue);
        }
    }*/
	sprintf_s(chValue, 1024, "%x", nValue);
    strValue = chValue;
}

void CTransmitMsgEthernet::OnInitialUpdate()
{
    CHAR caColumnName[defMESSAGE_BLOCK_COLUMN][defSTRING_SIZE] =
    {
        defMESSAGE_BLOCK_NAME,
        defMESSAGE_BLOCK_TRIGGER,
        defMESSAGE_BLOCK_TRIG_VAL
    };

    CFormView::OnInitialUpdate();

    m_bUpdating = FALSE;
    CWnd* pParentWnd = NULL;
    // Get Parent window pointer
    pParentWnd = pomGetParentWindow();
    // Update view pointer
    if( pParentWnd != NULL )
    {
        // Register this view pointer
        ((CTxMsgChildFrame*)pParentWnd)->m_pEthernetTransmitMsgView = this;
    }

    ResizeParentToFit(FALSE);
    ResizeParentToFit(TRUE);

    m_bInitDone = TRUE;

    // MDI child need this

    //************added by anish**********************

    RECT rListCtrlRect;
    INT nTotalColunmSize = 0;
    INT nTotalStrLengthPixel = 0;
    INT nColumnSize = 0;

	//vInitSignalListCtrl();
     //Setup the list control to add the messages.
    m_omHeaderList.SubclassDlgItem(IDC_LSTC_HDR_CONFIG, this);
    m_omHeaderList.SetExtendedStyle( LVS_EX_FULLROWSELECT |  LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES);

    CWnd* pWnd = m_omHeaderList.GetWindow(GW_CHILD);
    ASSERT(pWnd != NULL);

    CRect rctList;
    m_omHeaderList.GetWindowRect(&rctList);
    int nColWidth = rctList.Width() - 2;
    int colwidths[] = { 10, 10, 10, 7, 7, 4, 20 };
    int total_cx = 0;

    TCHAR* acColData[]=
    {
        _T("Packet Name"), _T("Source MAC"), _T("Destination Mac"), _T("Protocol"),
        _T("Repetition"), _T("Key"), _T("DLC")
    };

    LVCOLUMN lvcolumn;
    memset(&lvcolumn, 0, sizeof(lvcolumn));
    lvcolumn.mask =  LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;

    // Associate Header control Image List
    if( m_omHeaderImageList.m_hImageList == NULL )
    {
        m_omHeaderImageList.Create( IDR_BMP_CHECKBOX, 16, 1, RGB(0,0,255) );
    }
    // Set the Image List
    // Only if it is sucessfully created
    if( m_omHeaderImageList.m_hImageList != NULL )
    {
        m_omHeaderList.SetImageList(&m_omHeaderImageList, LVSIL_SMALL);
        CHeaderCtrl* pHeader = m_omHeaderList.GetHeaderCtrl();
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
                //pHeader->SetItem(6, &hditem );
            }
        }
    }
    m_hTransmitWnd = m_hWnd;

     //add columns
    //ColumnProperty ouProperty;
    //ouProperty.bMovable = true;
    for (int i = 0; i < (sizeof (acColData) / sizeof acColData[0]); i++)
    {
        // Column formating
        if (( i == FIRST_COL )) //4 col - Cycle No 6-DataLen , 7 - DataBytes
        {
            lvcolumn.fmt = LVCFMT_LEFT ;
        }

        else
        {
            lvcolumn.fmt = LVCFMT_CENTER;
        }


        lvcolumn.pszText = acColData[i];
        lvcolumn.cchTextMax = (int)strlen(acColData[i]);
        lvcolumn.iSubItem = i;
        lvcolumn.cx = (acColData[i + 1] == NULL) ? nColWidth - total_cx - 2 : (nColWidth * colwidths[i]) / 64;
        total_cx += lvcolumn.cx;
        //ouProperty.bMovable = true;
        if ( i== FIFTH_COL|| i == SEVENTH_COL )
        {
            //ouProperty.bMovable = false;
        }

        m_omHeaderList.InsertColumn(i, &lvcolumn);

    }

    m_hTransmitWnd = m_hWnd;

    int nConfigChannel = CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_nChannelsConfigured;
    CString strChannel;

    UpdateTxView(CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_nChannelsConfigured);
    m_bInitDone = FALSE;
    if ( CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_eBusStatus == BUS_CONNECTED)
    {
        vChangeDelButtonStatus(false);
    }
    else
    {
        vChangeDelButtonStatus(true);
    }

    UpdateTxView(CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_nChannelsConfigured);
}

/*******************************************************************************
Function Name  : pomGetParentWindow
Input(s)       : -
Output         : CWnd * - Pointer to CTxMsgChildFrame
Functionality  : This Function will return parent window pointer. That is
pointer to CTxMsgChildFrame. This will return NULL incase of
failure
Member of      : CTxFunctionsView
Author(s)      : Raja N
Date Created   : 25.4.2005
Modifications  :
*******************************************************************************/
CWnd* CTransmitMsgEthernet::pomGetParentWindow() const
{
    CWnd* pWnd = NULL;
    // Get Splitter window pointer
    pWnd = GetParent();
    // Get Tx Msg Child Window pointer from Splitter window pointer
    if( pWnd != NULL )
    {
        pWnd = pWnd->GetParent();
    }
    // Return Tx Msg Child window pointer or NULL incase of failure
    return pWnd;
}

void CTransmitMsgEthernet::OnBtnClickedAddnew()
{
	ETHERNET_FRAME_DATA* pEFrameData = new ETHERNET_FRAME_DATA;
	pEFrameData->m_strFrameName = "Packet1";
	pEFrameData->bKeyEnabled  = false;
	pEFrameData->m_chKeyVal = 'a';
	strcpy((char*)pEFrameData->m_ouEthernetMessage.m_ucName, "Packet1");
	strcpy((char*)pEFrameData->m_ouEthernetMessage.m_ucSourceMAC, "000000000000\0");
	strcpy((char*)pEFrameData->m_ouEthernetMessage.m_ucDestMac, "FFFFFFFFFFFF\0");
	strcpy((char*)pEFrameData->m_ouEthernetMessage.m_ucProtocolName, "RAW");
	pEFrameData->m_ouEthernetMessage.m_unDataLen = 0;
	strcpy((char*)pEFrameData->m_ouEthernetMessage.m_ucData, "");
	
	string strRaw = "FF FF FF FF FF FF 00 00 00 00 00 00 ";
	m_omEditRawFrame.SetWindowTextA(strRaw.c_str());
	CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().nInsertMessage(*pEFrameData);
	nAddMessageToList(*pEFrameData);
}

void CTransmitMsgEthernet::OnBnClickedBtnEthConfiguration()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CEthernetTxDefaultConfigDlg oDefaultConfig;
	if(oDefaultConfig.DoModal() == IDOK)
	{

	}
}

void CTransmitMsgEthernet::GetTotalClientRect(LPRECT lpRect)
{
    //GetResizableWnd()->GetClientRect(lpRect);
    GetClientRect(lpRect);

    // get scrollable size
    CSize size = GetTotalSize();

    // before initialization, "size" is dialog template size
    if (!m_bInitDone)
    {
        lpRect->right = lpRect->left + size.cx;
        lpRect->bottom = lpRect->top + size.cy;
        return;
    }

    // otherwise, give correct size if scrollbars active

    if (m_nMapMode < 0) // scrollbars disabled
    {
        return;
    }

    // enlarge reported client area when needed
    CRect rect(lpRect);
    if (rect.Width() < size.cx)
    {
        lpRect->right = lpRect->left + size.cx;
    }
    if (rect.Height() < size.cy)
    {
        lpRect->bottom = lpRect->top + size.cy;
    }
}

int CTransmitMsgEthernet:: nGetCheckedItemCount()
{
    int nSelectedItemCount = 0;
    for(int nIndex = 0; nIndex < m_omHeaderList.GetItemCount() + 1; nIndex++)
    {
        if ( m_omHeaderList.GetCheck(nIndex) ==  TRUE )
        {
            nSelectedItemCount++;
        }
    }
    return  nSelectedItemCount;
}

void CTransmitMsgEthernet::DeleteAllMsgListColumns()
{

}

//********************************************************************************
//  Function Name       : OnBnClickedBtUpdate
//  Input(s)            : None
//  Output              : void
//  Description         : None
//  Member of           : CTransmitMsgEthernet
//  Friend of           : None
//  Author              : Anupam Kumar
//  Creation Date       : 16/03/2007
//  Modifications       :
//********************************************************************************
void CTransmitMsgEthernet::OnBnClickedBtUpdate()
{


}

//********************************************************************************
//  Function Name       : OnBnClickedBtDelFrame
//  Input(s)            : None
//  Output              : void
//  Description         : None
//  Member of           : CTransmitMsgEthernet
//  Friend of           : None
//  Author              : Ashwin R Uchil
//  Creation Date       : 24.4.2013
//  Modifications       :
//********************************************************************************
void CTransmitMsgEthernet::OnBnClickedBtDelFrame()
{
    int nSelItem = m_omHeaderList.GetNextItem(-1, LVNI_SELECTED);
    if ( -1 != nSelItem )
    {
        vDeleteFrameFrmList(nSelItem);
        if(m_omHeaderList.GetItemCount() > 0)
        {
            nSelItem = nSelItem % m_omHeaderList.GetItemCount();
            m_omHeaderList.SetItemState(nSelItem, LVIS_SELECTED, LVIS_SELECTED);
            m_omHeaderList.SetSelectionMark(nSelItem);
        }
    }
    vChangeDelButtonStatus();

}

void CTransmitMsgEthernet::vDeleteFrameFrmList(INT nIndex)
{
    m_omHeaderList.DeleteItem(nIndex);
    CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().nDeleteMessageAt(nIndex);
}

void CTransmitMsgEthernet::vDeleteAllFrames()
{
    m_omHeaderList.DeleteAllItems();
    CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().nDeleteAllMessages();
    CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().nDeleteAllKeyEntrs();
}

int CTransmitMsgEthernet::nAddMessageToList(ETHERNET_FRAME_DATA& ouEthernetData, int nRows)
{
	int  nCount = m_omHeaderList.GetItemCount();
	m_omHeaderList.InsertItem( nCount, STR_EMPTY);
	if( nCount != -1 )
	{
		SLISTINFO ouListInfo;

		m_omHeaderList.SetItemState( nCount, 0, LVIS_SELECTED | LVIS_FOCUSED );
		sUserProgInfo ouUserProgInfo;
		ouListInfo.m_eType = eText;
		LVITEM lvitem = {0};
		lvitem.mask =  LVIF_IMAGE;
		lvitem.pszText = (LPSTR)ouEthernetData.m_ouEthernetMessage.m_ucName;
		lvitem.iImage = 2 ; // Index of first subitem image
		lvitem.iItem = nCount; // Index of item
		lvitem.iSubItem = def_COLUMN_MSGNAME_INDEX; // Index of subitem
		
		m_omHeaderList.vSetColumnInfo(nCount, def_COLUMN_MSGNAME_INDEX, ouListInfo);
		m_omHeaderList.SetItem(&lvitem);
		m_omHeaderList.SetItemText( nCount, def_COLUMN_MSGNAME_INDEX, (LPSTR)ouEthernetData.m_strFrameName.c_str());

		// Init Raw Value column
		// Column 1 :  Edit  Control
		ouListInfo.m_eType = eText;
		m_omHeaderList.vSetColumnInfo(nCount, def_COLUMN_SRC_MAC, ouListInfo);
		static char chTem[12], cMAC[20];
		strcpy(chTem, (char*)ouEthernetData.m_ouEthernetMessage.m_ucSourceMAC); 
		sprintf(cMAC,"%c%c:%c%c:%c%c:%c%c:%c%c:%c%c",chTem[0],chTem[1],chTem[2],chTem[3],chTem[4],chTem[5],chTem[6],chTem[7],chTem[8],chTem[9],chTem[10],chTem[11]);
		m_omHeaderList.SetItemText( nCount, def_COLUMN_SRC_MAC, cMAC );

		// Init Phy Value column
		// Column 2 :  Edit Control
		ouListInfo.m_eType = eText;
		m_omHeaderList.vSetColumnInfo(nCount, def_COLUMN_DEST_MAC, ouListInfo);
		strcpy(chTem, (char*)ouEthernetData.m_ouEthernetMessage.m_ucDestMac); 
		sprintf(cMAC,"%c%c:%c%c:%c%c:%c%c:%c%c:%c%c",chTem[0],chTem[1],chTem[2],chTem[3],chTem[4],chTem[5],chTem[6],chTem[7],chTem[8],chTem[9],chTem[10],chTem[11]);
		m_omHeaderList.SetItemText( nCount, def_COLUMN_DEST_MAC, cMAC);

		// Init Unit column
		// Column 3 :  Edit Control
		ouListInfo.m_eType = eText;
		m_omHeaderList.vSetColumnInfo(nCount, def_COLUMN_PROTOCOL, ouListInfo);
		m_omHeaderList.SetItemText( nCount, def_COLUMN_PROTOCOL, (LPSTR)ouEthernetData.m_ouEthernetMessage.m_ucProtocolName );

		ouListInfo.m_eType = eNumber;
		ouUserProgInfo.m_bIcon = true;
		ouUserProgInfo.m_pfHandler = vCheckBoxClickHandler;
		ouUserProgInfo.m_pUserParam = this;

		m_omHeaderList.vSetColumnInfo(nCount, def_COLUMN_REPETITION_INDEX, ouListInfo);
		m_omHeaderList.vSetUserProgInfo(nCount, def_COLUMN_REPETITION_INDEX, ouUserProgInfo);
		m_omHeaderList.SetItemText(nCount, def_COLUMN_REPETITION_INDEX, "10");

		string strTemp;
		ouListInfo.m_eType = eAlphaNumericType;
		ouUserProgInfo.m_bIcon = true;
		ouUserProgInfo.m_pfHandler= vCheckBoxClickHandler;
		ouUserProgInfo.m_pUserParam = this;
		strTemp = ouEthernetData.m_chKeyVal;
		m_omHeaderList.vSetColumnInfo(nCount, def_COLUMN_KEY_INDEX, ouListInfo);
		m_omHeaderList.vSetUserProgInfo(nCount, def_COLUMN_KEY_INDEX, ouUserProgInfo);
		m_omHeaderList.SetItemText(nCount, def_COLUMN_KEY_INDEX,strTemp.c_str());

		if(ouEthernetData.bKeyEnabled == TRUE)
		{
			CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().nInsertKey(nCount, strTemp[0]);
		}

		string strText = "";
		
		//LVITEM lvitem = {0};
		lvitem.mask =  LVIF_IMAGE;
		lvitem.pszText = "";
		lvitem.iImage = 0 ; // Index of first subitem image
		lvitem.iItem = nCount; // Index of item
		lvitem.iSubItem = def_COLUMN_REPETITION_INDEX; // Index of subitem
		m_omHeaderList.SetItem(&lvitem);

		lvitem.mask =  LVIF_IMAGE;
		lvitem.pszText = "a";
		lvitem.iImage = 0 ; // Index of first subitem image
		lvitem.iItem = nCount; // Index of item
		lvitem.iSubItem = def_COLUMN_KEY_INDEX; // Index of subitem
		m_omHeaderList.SetItem(&lvitem);
		//m_lstMsg.SetItem(nRows, 6, LVIF_IMAGE, "H", 1, 0, 0, 0);
		//m_lstMsg.SetSelectionMark(nRows);

		m_omHeaderList.SetCheck(nCount, 0);
		ouListInfo.m_eType = eText;
		m_omHeaderList.vSetColumnInfo(nCount, def_COLUMN_DLC, ouListInfo);
		string  strDLC = to_string(ouEthernetData.m_ouEthernetMessage.m_unDataLen);
		m_omHeaderList.SetItemText( nCount, def_COLUMN_DLC, strDLC.c_str());
	}

	m_omHeaderList.EnableWindow( TRUE );
	return TRUE;
}

void CTransmitMsgEthernet::OnClose()
{
    CFormView::OnClose();
}

void CTransmitMsgEthernet::vLoadTxMsgConfig()
{
    //BYTE* SectionBuffer = NULL;
    //int nBufferLength = 0;
    ////Remove all the TX msg block before copying from config file
    //// CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().vRemoveAllBlock(m_omMsgBlockMan);
    ////CTxLINDataStore::ouGetTxFlexRayDataStoreObj().vRemoveAllBlock(CTxLINDataStore::ouGetTxFlexRayDataStoreObj().m_omTxMsgBlockMan);
    //// First Get Tx Msg List

    //CConfigDetails::ouGetConfigDetailsObject().bGetData((void*&) SectionBuffer,
    //        nBufferLength,TX_MSG_WND_CONFIG_SECTION_NAME);
    //if (SectionBuffer != NULL)
    //{
    //    delete [] SectionBuffer;
    //}
    //else //new file : Load default value TBD
    {
        //Load default window placement
        WINDOWPLACEMENT TxWndPlacement;
        ::GetWindowPlacement(GetParent()->m_hWnd, &TxWndPlacement);
        TxWndPlacement.showCmd &= ~SW_MINIMIZE;
        TxWndPlacement.showCmd |= SW_RESTORE;
        ::SetWindowPlacement(GetParent()->m_hWnd, &TxWndPlacement);
    }
    //Update UI
    vRefreshTxMsgWindow();
}

void CTransmitMsgEthernet::vRefreshTxMsgWindow()
{
    m_bInitDone = TRUE;
    // Clear the message list control
    m_omHeaderList.DeleteAllItems();
    //Remove all items from the list control
	m_omEditFrameData.SetWindowTextA("");

	m_omEditRawFrame.SetWindowTextA("");
}

void CTransmitMsgEthernet::OnDestroy()
{

}

/*******************************************************************************
Function Name  : OnBtnClose
Input(s)       : -
Output         : -
Functionality  : This button handler will be called during Close button press.
This function will send WM_CLOSE message to close Tx Child
window.
Member of      : CTransmitMsgEthernet
Author(s)      : Ashwin R Uchil
Date Created   : 16.4.2013
Modifications  :
*******************************************************************************/
void CTransmitMsgEthernet::OnBtnClose()
{
    // Get Child Frame Pointer
    CWnd* pWnd = NULL;
    pWnd = pomGetParentWindow();
    // Post close message
    if( pWnd != NULL )
    {
        // This will invoked the InvokeClose function
        ((CTxMsgChildFrame*)pWnd)->PostMessage( WM_CLOSE );
    }
}

HRESULT CTransmitMsgEthernet::UpdateTxView(int& m_nChannelsConfigured)
{
    m_omHeaderList.DeleteAllItems();

    vUpdateMessageList();

    vChangeDelButtonStatus();
    return S_OK;
}

void CTransmitMsgEthernet::vUpdateMessageList()
{
    CTxEthernetDataStore& ouEthernetData = CTxEthernetDataStore::ouGetTxEthernetDataStoreObj();
    list<ETHERNET_FRAME_DATA>::iterator itrLinData = ouEthernetData.m_ouEthernet_Frame_Data.begin();
    char chText[MAX_PATH] = {0};
    FRAME_STRUCT ouFrame;

    m_omHeaderList.DeleteAllItems();
    ouEthernetData.nDeleteAllKeyEntrs();
    m_bFromEvent = true;

    while ( itrLinData != ouEthernetData.m_ouEthernet_Frame_Data.end())
    {
        nAddMessageToList(*itrLinData);
        itrLinData++;
    }

    m_bFromEvent = false;
}

int CTransmitMsgEthernet::nUpdateSelectedMessage()
{
    int nSel = m_omHeaderList.GetNextItem(-1, LVIS_SELECTED );
    if ( nSel != -1 )
    {
        ETHERNET_FRAME_DATA ouLinData;
       // if ( TRUE == bValidateData(ouLinData) )
        {
            EnterCriticalSection(&CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_ouCSMsgList);
            list<ETHERNET_FRAME_DATA>::iterator itrLinData = CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_ouEthernet_Frame_Data.begin();
            advance(itrLinData, nSel);
            if ( itrLinData !=  CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_ouEthernet_Frame_Data.end() )
            {
                itrLinData->m_ouEthernetMessage.m_ucChannel = ouLinData.m_ouEthernetMessage.m_ucChannel;
                memcpy(itrLinData->m_ouEthernetMessage.m_ucData, ouLinData.m_ouEthernetMessage.m_ucData, 8);
                itrLinData->m_ouEthernetMessage.m_unDataLen = ouLinData.m_ouEthernetMessage.m_unDataLen;
                itrLinData->m_ouEthernetMessage.m_unMsgID = ouLinData.m_ouEthernetMessage.m_unMsgID;
            }
            LeaveCriticalSection(&CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_ouCSMsgList);
        }
    }
    return 0;
}

void CTransmitMsgEthernet::vCheckBoxClickHandler( CListCtrl* pThis, int nItem, int nSubItem, void* uParam)
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

    BOOL bEnable = FALSE;
    eMsgUpdate eUpdate;
    if ( nSubItem == def_COLUMN_KEY_INDEX )
    {
        eUpdate = eKeyCheck;
    }
    else if ( nSubItem == def_COLUMN_REPETITION_INDEX )
    {
        eUpdate = eRepetitionCheck;
    }
    if ( lvitem.iImage == 0 )
    {
        lvitem.iImage = 1;
        bEnable = TRUE;
    }
    else
    {
        lvitem.iImage = 0;
        bEnable = FALSE;
    }

	 ((CTransmitMsgEthernet*)uParam)->vUpdateSelMsgDetails(eUpdate, nItem, -1, -1, bEnable);

    ((CListCtrl*)pThis)->SetItem(&lvitem);
}

INT CTransmitMsgEthernet::GetSelectedSignalStruct( SIGNAL_STRUCT& ouSignal, int nIndex )
{
   
    return S_FALSE;
}

void CTransmitMsgEthernet::vInitialiseDataBytes()
{
    for(INT nIndex = 0; nIndex < 8; nIndex++)
    {
        m_unData[nIndex].byByte = 0;
    }
}

void CTransmitMsgEthernet::vUpdateDataBytes(ETHERNET_FRAME_DATA& ouData)
{
    ////UINT unIDValue = IDC_EDIT_DB1;// ID of first databyte edit control
    //CEdit* pEdit = NULL;
    //CString omStr, omStrFormatData;

    //if( true == CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_bHexMode)
    //{
    //    omStrFormatData = defFORMAT_DATA_HEX;
    //}
    //else
    //{
    //    omStrFormatData = defFORMAT_DATA_DECIMAL;
    //}

}

extern unsigned char hex_digit_value(char c);
//unsigned char hex_digit_value(char c)
//{
//    if ('0' <= c && c <= '9') { return c - '0'; }
//    if ('a' <= c && c <= 'f') { return c + 10 - 'a'; }
//    if ('A' <= c && c <= 'F') { return c + 10 - 'A'; }
//	c= 0xff;
//    return -1;
//}

void UpdateDataToDataStore(STETHERNET_MSG& ouEthernetMessage, string strData)
{
	//Convert 2 characters  to 1 byte  "FF" to 0xFF
	for (int i = 0, j= 0; i < ouEthernetMessage.m_unDataLen ; i++, j+=2)
    {
		ouEthernetMessage.m_ucData[i] = (hex_digit_value(strData[j]) << 4) | ( hex_digit_value(strData[j+1]));            
    }
}
/*******************************************************************************
 Function Name    : psGetSelectedSignalStruct
 Input(s)         : nIndex      -  Selected Signal index
 Output           : sSIGNALS *  - Selected Signal Pointer
 Functionality    : This function will return the signal pointer of the signal
                    pointed by the index. If it is not found it will return NULL
 Member of        : CTxMsgDetailsView
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 19.07.2004
 Modification By  :
 Modification on  :
*******************************************************************************/
void CTransmitMsgEthernet::psGetSelectedSignalStruct(list<SIGNAL_STRUCT> lstSigStrct, CString omStrSigName, SIGNAL_STRUCT& ouSigStrct)
{

    list<SIGNAL_STRUCT>::iterator itrSigList = lstSigStrct.begin();

    // Get the Signal List head pointer
    itrSigList = lstSigStrct.begin();

    // Iterate through the list
    while( itrSigList != lstSigStrct.end())
    {
        // Compare the signal name
        if( omStrSigName.Compare(itrSigList->m_strSignalName.c_str() ) == 0 )
        {
            // If found then stop traversing
            ouSigStrct = *itrSigList;
            break;
        }
        itrSigList++;
    }
}

void CTransmitMsgEthernet::vUpdateSelMsgDetails(eMsgUpdate eUpdateType, INT nItem, INT nDlc, INT nChannel, BOOL bEnable)
{
    EnterCriticalSection(&CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_ouCSMsgList);
    if(nItem != -1)
    {
        list<ETHERNET_FRAME_DATA>::iterator itrList = CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_ouEthernet_Frame_Data.begin();

        std::advance(itrList, nItem);
        string strTemp;
        string strText = "";
        switch(eUpdateType)
        {
			case ePacketName:
				{
					string strName  = m_omHeaderList.GetItemText(nItem, def_COLUMN_MSGNAME_INDEX);
					itrList->m_strFrameName =  strName.c_str();
				}
				break;
			case eSrcMAC:
				{
					string strSrcMac  = m_omHeaderList.GetItemText(nItem, def_COLUMN_SRC_MAC);
					strSrcMac.erase(remove(strSrcMac.begin(), strSrcMac.end(), ':'), strSrcMac.end());
					strcpy((char*)itrList->m_ouEthernetMessage.m_ucSourceMAC, strSrcMac.c_str());
				}
				break;
		    case eDestMAC:
				{
					string strDestMac  = m_omHeaderList.GetItemText(nItem, def_COLUMN_DEST_MAC);
					strDestMac.erase(remove(strDestMac.begin(), strDestMac.end(), ':'), strDestMac.end());
					strcpy((char*)itrList->m_ouEthernetMessage.m_ucDestMac, strDestMac.c_str());
				}
				break;
            case eEnable:
            {
                itrList->bSelected = bEnable;
                CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().nHandleSelectionState( nItem, bEnable);
            }
            break;
            case eDataBytes:
				{

					//While calculting the total char length displayed in editbox, consider spaces + 2 * length since each bytes is shown as 2 char
					if(itrList->m_ouEthernetMessage.m_unDataLen <= 0)
					{
						break;
					}
					int nTotalDataCharLength = itrList->m_ouEthernetMessage.m_unDataLen * 2 + (itrList->m_ouEthernetMessage.m_unDataLen -1);
					char chData[4000];
					string strData = "", strTemp = "";

					m_omEditFrameData.GetWindowTextA(chData, 3000);
					strData = chData;
					int i = 0, nLength = strData.length();
					
					while(strData.find(' ') != -1)
					{
						if(i >= strData.length())
						{
							break;
						}
						strData.replace(strData.find(' '), 1, "");
						i++;
					}
					UpdateDataToDataStore(itrList->m_ouEthernetMessage, strData);
					break;
				}
			case eDlc:
				{
					string strDLC  = m_omHeaderList.GetItemText(nItem, def_COLUMN_DLC);
					int nDLC = atoi(strDLC.c_str());
					if(nDLC != 0)
					{
						itrList->m_ouEthernetMessage.m_unDataLen = (unsigned int)nDLC;
						//vGetStringFromValue(nDlc, strDlc, true);
						//m_omHeaderList.SetItemText(nItem, def_COLUMN_DLC, strDLC.c_str()  );
						vInitialiseDataList(nDLC, nItem);
					}
					else
					{
						m_omEditFrameData.SetWindowTextA("");
					}
				}
				break;
            case eChannel:

                if(nChannel != -1)
                {
                    //itrList->m_ouEthernetMessage.m_unChannel = nChannel+1;
                }
                break;
            case eKeyCheck:
            {
                itrList->bKeyEnabled = bEnable;

                if(itrList->bKeyEnabled == TRUE)
                {
                    CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().nInsertKey(nItem, chGetKeyVal(nItem));
                }
                else
                {
                    CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().nDeleteKey(nItem);
                }
            }
            break;
            case eKeyval:
            {
                CString omStrKeyVal = m_omHeaderList.GetItemText(nItem, def_COLUMN_KEY_INDEX);
                itrList->m_chKeyVal = ' ';
                if ( omStrKeyVal.GetLength() > 0 )
                {
                    itrList->m_chKeyVal = omStrKeyVal.GetAt(0);
                }

                if(itrList->bKeyEnabled == TRUE)
                {
                    CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().nInsertKey(nItem, itrList->m_chKeyVal);
                }
            }
            break;
            case eRepetitionCheck:
            {
                itrList->m_bTimerEnabled = bEnable;
            }
            break;
            case eRepetitionVal:
            {
                CString omVal = m_omHeaderList.GetItemText(nItem, def_COLUMN_REPETITION_INDEX);

                //BOOL bHex =   CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_bHexMode;
                UINT nTimer;
                /*CHAR* pcStr = NULL;
                if(bHex == TRUE )
                {
                    nTimer = strtol(omVal,&pcStr,16);
                }
                else
                {
                    nTimer = strtol(omVal,&pcStr,10);
                }*/

                nTimer = atol(omVal);

                nTimer = atol(omVal);
                if ( nTimer < def_MINIMUM_TIMER_VAL )
                {
                    nTimer = def_MINIMUM_TIMER_VAL;
                    CString strVal;
                    strVal.Format("%d", nTimer);
                    //vGetStringFromValue(nTimer, strVal);
                    m_omHeaderList.SetItemText(nItem, def_COLUMN_REPETITION_INDEX, strVal);
                }
                else if ( nTimer > def_MAXIMUM_TIMER_VAL )
                {
                    nTimer = def_MAXIMUM_TIMER_VAL;
                    CString strVal;
                    strVal.Format("%d", nTimer);
                    //vGetStringFromValue(nTimer, strVal, true);
                    m_omHeaderList.SetItemText(nItem, def_COLUMN_REPETITION_INDEX, strVal);
                }

                itrList->nActualTimer = nTimer;
                itrList->nCurrentTimerVal = itrList->nActualTimer;
            }
            break;
        }
    }
    LeaveCriticalSection(&CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_ouCSMsgList);
}

void CTransmitMsgEthernet::vInitialiseDataList(int nDlc, int nItem)
{
	string strData = "";
	for(int nCnt =0; nCnt < nDlc; nCnt++)
	{
		strData.append("00 ");
	}
	m_omEditFrameData.SetWindowTextA(strData.c_str());

	string strSrcMac  = m_omHeaderList.GetItemText(nItem, def_COLUMN_SRC_MAC);
	for (int i = 0; i < strSrcMac.length(); ++i) 
	{
		if (strSrcMac[i] == ':')
			strSrcMac[i] = ' ';
	}
	strSrcMac.append(" ");
	string strDestMac  = m_omHeaderList.GetItemText(nItem, def_COLUMN_DEST_MAC);
	for (int i = 0; i < strDestMac.length(); ++i) 
	{
		if (strDestMac[i] == ':')
			strDestMac[i] = ' ';
	}
	strDestMac.append(" ");
	string strRaw;
	strRaw.append(strDestMac);
	strRaw.append(strSrcMac);
	strRaw.append(strData);
	m_bIsRawSetByCode = true;  //avoid resetting of data frames
	m_omEditRawFrame.SetWindowTextA(strRaw.c_str());
}


void CTransmitMsgEthernet::OnEnChangeEditData()
{
	if(m_bIsDataSetByCode)
	{
		m_bIsDataSetByCode = false;
		return;
	}
	string strData = "", strRaw = "" ;
	char chData[1500];
	int nDataLenth = m_omEditFrameData.GetWindowTextLengthA();
	m_omEditFrameData.GetWindowTextA(chData, 1500);
	strData = chData;

	if(strData.length() == 0)
	{
		return;
	}
	//char chRaw[1540];
	m_omEditRawFrame.GetWindowTextA(chData, m_omEditRawFrame.GetWindowTextLengthA());
	strRaw = chData;

	if(strRaw.length() < 37)
	{
		return;
	}
	strRaw.replace(strRaw.begin() + 36, strRaw.end(), strData);
	m_bIsRawSetByCode = true;
	m_omEditRawFrame.SetWindowTextA(strRaw.c_str());

	vUpdateSelMsgDetails(eDataBytes, m_omHeaderList.GetSelectionMark());
}

void CTransmitMsgEthernet::OnEnChangeEditRaw()
{
	if(m_bIsRawSetByCode)
	{
		m_bIsRawSetByCode = false;
		return;
	}
	string strData = "", strRaw = "", strSrcMAC = "", strDestMac = "";
	char chData[1500];
	m_omEditRawFrame.GetWindowTextA(chData,1500);
	strRaw = chData;

	if(strRaw.length() < 36)
	{
		return;
	}
	strDestMac = strRaw.substr(0, 17);
	strSrcMAC = strRaw.substr(18, 17);

	for (int i = 0; i < strDestMac.length(); ++i) 
	{
		if (strDestMac[i] == ' ')
			strDestMac[i] = ':';
		if (strSrcMAC[i] == ' ')
			strSrcMAC[i] = ':';
	}
	m_omHeaderList.SetItemText(m_omHeaderList.GetSelectionMark(), def_COLUMN_DEST_MAC,strDestMac.c_str());
	m_omHeaderList.SetItemText(m_omHeaderList.GetSelectionMark(), def_COLUMN_SRC_MAC,strSrcMAC.c_str());

	vUpdateSelMsgDetails(eSrcMAC, m_omHeaderList.GetSelectionMark());

	vUpdateSelMsgDetails(eDestMAC, m_omHeaderList.GetSelectionMark());

	if(strRaw.length() < 37)
	{
		return;
	}

	m_omEditFrameData.GetWindowTextA(chData, 1500);
	strData = chData;

	if(strData.length() == 0)
	{
		return;
	}

	strData = strRaw.substr(36, strRaw.length()-36);
	
	m_bIsDataSetByCode = true;
	m_omEditFrameData.SetWindowTextA(strData.c_str());
	vUpdateSelMsgDetails(eDataBytes, m_omHeaderList.GetSelectionMark());
	//char chRaw[1540];
	
}

char CTransmitMsgEthernet::chGetKeyVal(int nItem)
{
    CString omStrKeyVal = m_omHeaderList.GetItemText(nItem, def_COLUMN_KEY_INDEX);
    char chKeyVal = ' ';
    if ( omStrKeyVal.GetLength() > 0 )
    {
        chKeyVal = omStrKeyVal.GetAt(0);
    }

    return chKeyVal;
}

void CTransmitMsgEthernet::OnNMTransmitMessagesRightClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    OnNMTransmitMessagesClick(pNMHDR, pResult);
    return;
}

void CTransmitMsgEthernet::OnNMTransmitMessagesClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    LVHITTESTINFO hitinfo;
    *pResult = 0;

    bool bChecked = false;

    //Copy click point
    hitinfo.pt = pNMListView->ptAction;

    //Make the hit test...
    int nItem = m_omHeaderList.HitTest(&hitinfo);

    if(hitinfo.flags == LVHT_ONITEM || hitinfo.flags == LVHT_ONITEMLABEL)
    {

        return;
    }
    //Selection Change
    else if(hitinfo.flags == LVHT_ONITEMSTATEICON )
    {

        return;
    }
}

void CTransmitMsgEthernet::OnItemchangedLstcMsgDetails( NMHDR* pNMHDR, LRESULT* pResult )
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;


    // Selected & Focused
    if( pNMListView->uNewState & (LVIS_FOCUSED | LVIS_SELECTED) )
    {
        // For Row selection
        //vUpdateSignalMatrix( pNMListView->iItem);
        if(pNMListView->iItem != -1)
        {
            ETHERNET_FRAME_DATA objEthernetFrame = vGetMsgDetails(pNMListView->iItem);
            nUpdateMessageDetails(objEthernetFrame);
        }
    }

    // Notification for text change
    if( pNMListView->uChanged == LVIF_TEXT)
    {
        LV_DISPINFO ls;
        if(m_omHeaderList.sGetModificationStructure(ls) == TRUE)
        {
            switch( ls.item.iSubItem)
            {
				case def_COLUMN_MSGNAME_INDEX:
				     vUpdateSelMsgDetails(ePacketName, pNMListView->iItem);
					 break;
				case def_COLUMN_SRC_MAC:
				     vUpdateSelMsgDetails(eSrcMAC, pNMListView->iItem);
					 break;
				case def_COLUMN_DEST_MAC:
				     vUpdateSelMsgDetails(eDestMAC, pNMListView->iItem);
					 break;
                    // Raw Value Change
                case def_COLUMN_REPETITION_INDEX:
                    vUpdateSelMsgDetails(eRepetitionVal, pNMListView->iItem);
                    break;
                    // Physical Value Change
                case def_COLUMN_KEY_INDEX:
                    vUpdateSelMsgDetails(eKeyval, pNMListView->iItem);
                    break;
				case def_COLUMN_DLC:
					vUpdateSelMsgDetails(eDlc, pNMListView->iItem);
                    break;
            }
        }

    }
    if( pNMListView->uNewState & defMASK_CHECK_UNCHECK && m_bFromEvent == false )
    {

        vUpdateSelMsgDetails(eEnable, pNMListView->iItem, 0, 1, m_omHeaderList.GetCheck(pNMListView->iItem));
    }

    if((pNMListView->iItem >= 0) && ( pNMListView->uNewState & (LVIS_FOCUSED|LVIS_SELECTED)))
    {
        m_nSelectedMsgIndex = pNMListView->iItem ;
        if(CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_eBusStatus == BUS_CONNECTED)
        {
            //m_omButtonAddMsg.EnableWindow(TRUE);
        }
        else
        {
            //m_omButtonAddMsg.EnableWindow(FALSE);
        }
        *pResult = 0;
    }
    int nSlectedFrame = m_omHeaderList.GetNextItem(-1, LVIS_SELECTED);
    if ( nSlectedFrame < 0 )
    {
        m_nSelectedMsgIndex = -1;
        //m_omButtonAddMsg.SetWindowText(def_ADD_MESSAGE_SRING);
        *pResult = 0;
        return;
    }

    *pResult = 0;
}

INT CTransmitMsgEthernet::nUpdateMessageDetails(ETHERNET_FRAME_DATA& ouEthernetData)
{
   
	string strTemp = "", strText = "";
	for ( int i =0 ; i < ouEthernetData.m_ouEthernetMessage.m_unDataLen; i++ )
		{
		vGetStringFromValue(ouEthernetData.m_ouEthernetMessage.m_ucData[i], strTemp, false);
		strText += strTemp;
		strText += " ";
		}

	m_omEditFrameData.SetWindowTextA(strText.c_str());
	m_bIsDataSetByCode = true;

	string strDestMAC = "", strSrcMAC = "", strRAWFrame = "";
	char chSrcMAC[12], chDestMAC[12], chTempSrc[19], chTempDest[19];
	memcpy(chSrcMAC, (char*)ouEthernetData.m_ouEthernetMessage.m_ucSourceMAC, 12);
	sprintf(chTempSrc, "%c%c %c%c %c%c %c%c %c%c %c%c \0",
		chSrcMAC[0],chSrcMAC[1],chSrcMAC[2],chSrcMAC[3],chSrcMAC[4],chSrcMAC[5],chSrcMAC[6],chSrcMAC[7],chSrcMAC[8],chSrcMAC[9],chSrcMAC[10],chSrcMAC[11]);

	memcpy(chDestMAC, (char*)ouEthernetData.m_ouEthernetMessage.m_ucDestMac, 12);
	sprintf(chTempDest, "%c%c %c%c %c%c %c%c %c%c %c%c \0",
		chDestMAC[0],chDestMAC[1],chDestMAC[2],chDestMAC[3],chDestMAC[4],chDestMAC[5],chDestMAC[6],chDestMAC[7],chDestMAC[8],chDestMAC[9],chDestMAC[10],chDestMAC[11]);

	strRAWFrame = chTempDest;
	strRAWFrame.append( chTempSrc);
	strRAWFrame += strText;
	m_omEditRawFrame.SetWindowTextA(strRAWFrame.c_str());

    return 0;
}

ETHERNET_FRAME_DATA CTransmitMsgEthernet::vGetMsgDetails(INT nSelIndex)
{
    UINT unIndex = 0;
    list<ETHERNET_FRAME_DATA>::iterator itrList = CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_ouEthernet_Frame_Data.begin();

    std::advance(itrList, nSelIndex);

    return *itrList;
}

void CTransmitMsgEthernet::OnBnClickedBtnProtocolSetting()
{
	int nItem = m_omHeaderList.GetSelectionMark();
	if(nItem == -1)
	{
		AfxMessageBox("Please select a frame in the list and then click Protocol Settings");
		return;
	}

	HMODULE hCustomDll = CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().hGetCustomProtocolHandle();
	if(hCustomDll == NULL)
	{
		AfxMessageBox("Unable to find custom protocol dll. Please load the dll and try again.");
		return;
	}
	pfGetCustomProtocolTxInterface  = (GET_CUSTOM_PROTOCOL_TX_INTERFACE)GetProcAddress(hCustomDll, "GetCustomTxAppLayerProtocol");
	
	if(pfGetCustomProtocolTxInterface == NULL)
	{
		AfxMessageBox("Unable to find base interface in custom dll.");
		return;
	}

	pfGetCustomProtocolTxInterface((void**)&m_pBaseTxAppProtocol);

	list<ETHERNET_FRAME_DATA>::iterator itrList = CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_ouEthernet_Frame_Data.begin();
	std::advance(itrList, nItem);

	STDATA   sData;
	sData.unDataLength = itrList->m_ouEthernetMessage.m_unDataLen;
	sData.chData = new u_char[itrList->m_ouEthernetMessage.m_unDataLen];
	memcpy(sData.chData, itrList->m_ouEthernetMessage.m_ucData, itrList->m_ouEthernetMessage.m_unDataLen) ;
	HRESULT hResult = m_pBaseTxAppProtocol->AddHeaderToData(sData);
	if(hResult == S_OK)
	{
		memcpy(itrList->m_ouEthernetMessage.m_ucData, sData.chData, sData.unDataLength);
		nUpdateMessageDetails(*itrList);
	}
}

