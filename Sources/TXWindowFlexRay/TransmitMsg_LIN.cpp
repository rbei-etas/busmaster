///***********************************************************************************
//Project           :   Frame
//FileName      :   TransmitMsg.cpp
//Description       :   Implementation of CTransmitMsgLIN class
//Directory     :
//Version           :   V1.0
//Authors           :   Ravi D
//Date Created  :   24/10/2006
//Date Modified :
//Modifications :
//**********************************************************************************/
#pragma once
#include "TxWindowFlexRay_stdafx.h"
//#include "DataTypes/Base_LIN_Buffer.h"
#include "Utility/Utility.h"
#include "..\DIL_Interface\BaseDIL_LIN.h"
#include "..\DIL_Interface\DIL_Interface_extern.h"
#include "TxMsgChildFrame.h"
#include "TransmitMsg_LIN.h"
#include "TxFlexrayMsgHandlerDlg.h"
#include "ConfigDetails.h"
#include "..\DataTypes\MsgSignal_Datatypes.h"
#include "Utility\Utility.h"


const BYTE COLUMN_MSGID     = 0x02;
const BYTE COLUMN_DESCR     = 0x04;
const BYTE COLUMN_CYCLE     = 0x08;
const BYTE COLUMN_CHANNEL   = 0x10;
const BYTE COLUMN_DATALEN   = 0x20;
const BYTE COLUMN_DATABYTE  = 0x40;


#define def_COLUMN_MSGNAME_INDEX                0
#define def_COLUMN_MSGID_INDEX                  1
#define def_COLUMN_CHANNEL_INDEX                    2
#define def_COLUMN_DLC_INDEX                3
#define def_COLUMN_MSGTYPE_INDEX                4
#define def_COLUMN_REPETITION_INDEX             5
#define def_COLUMN_KEY_INDEX                    6
#define def_COLUMN_DATABYTES_INDEX              7
#define defMAX_LMT_LIN_MSG_ID                   63

#define BITLENGTH 16

#define TARGET_RESOLUTION 1         // 1-millisecond target resolution
#define defSTR_FORMAT_DISPLAY_DEC    _("%-60s   [%16I64d]")
#define defSTR_FORMAT_DISPLAY_HEX    _("%-60s   [%16I64X]")
#define def_MAXIMUM_TIMER_VAL       60000
#define def_MINIMUM_TIMER_VAL       1
HWND CTransmitMsgLIN::m_hTransmitWnd = nullptr;

#define TX_MSG_WND_CONFIG_SECTION_NAME                  "TXWndProperty"
#define def_SEND_Message                                "&Send"
#define def_ADD_MESSAGE_SRING                           "Add &Message"

extern CBaseDIL_LIN* g_pouDIL_LIN_Interface;
extern BOOL bCalcBitMaskForSig(BYTE* pbyMaskByte, UINT unArrayLen,
                               UINT nByteNum, UINT nBitNum, UINT nLength,
                               EFORMAT_DATA bDataFormat);

IMPLEMENT_DYNCREATE(CTransmitMsgLIN, CFormView)

BEGIN_MESSAGE_MAP(CTransmitMsgLIN, CFormView)
    ON_BN_CLICKED(IDC_CBTN_ADDNEW, OnBtnClickedAddnew)

    ON_NOTIFY(NM_RCLICK, IDC_LSTC_TRANSMIT_MSG, OnNMTransmitMessagesRightClick)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_TRANSMIT_MSG, OnNMTransmitMessagesClick)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_TRANSMIT_MSG, OnItemchangedLstcMsgDetails)
    ON_BN_CLICKED(IDC_BT_UPDATE, OnBnClickedBtUpdate)
    ON_BN_CLICKED(IDC_DEL_FRAME, OnBnClickedBtDelFrame)
    ON_BN_CLICKED(IDC_BT_DEL_ALL, OnBnClickedBtDelAll)
    ON_BN_CLICKED(IDC_CLOSE_LIN, OnBtnClose)
    ON_BN_CLICKED(IDC_BT_DEL_ALL, OnBnClickedBtDelAll)
    ON_EN_KILLFOCUS(IDC_EDIT_DB1, OnKillfocusDataBytesEdit)
    ON_EN_KILLFOCUS(IDC_EDIT_DB2, OnKillfocusDataBytesEdit)
    ON_EN_KILLFOCUS(IDC_EDIT_DB3, OnKillfocusDataBytesEdit)
    ON_EN_KILLFOCUS(IDC_EDIT_DB4, OnKillfocusDataBytesEdit)
    ON_EN_KILLFOCUS(IDC_EDIT_DB5, OnKillfocusDataBytesEdit)
    ON_EN_KILLFOCUS(IDC_EDIT_DB6, OnKillfocusDataBytesEdit)
    ON_EN_KILLFOCUS(IDC_EDIT_DB7, OnKillfocusDataBytesEdit)
    ON_EN_KILLFOCUS(IDC_EDIT_DB8, OnKillfocusDataBytesEdit)
    ON_EN_UPDATE(IDC_EDIT_DLC, OnUpdateEditDLC)

    ON_WM_CLOSE()
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    ON_MESSAGE(WM_USER+44, vUserCommand)

    ON_CBN_SELCHANGE(IDC_COMB_MSG_ID_NAME, OnSelchangeCombMsgIdName)
    ON_CBN_EDITCHANGE(IDC_COMB_MSG_ID_NAME, OnEditchangeCombMsgIdName)
    ON_BN_CLICKED(IDC_CBTN_ADD_MSG_LIN, OnAddMessageClick)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_SIG_DETAILS, OnItemchangedLstcSigDetails)
    ON_BN_CLICKED(IDC_LIN_MSGTYPE_SLAVE, onLinSlaveTypeSelected)
    ON_BN_CLICKED(IDC_LIN_MSGTYPE_HEADER, onLinHeaderTypeSelected)
    ON_CBN_SELCHANGE(IDC_COMB_CHANNEL_ID, onComboChnlSel)

    ON_NOTIFY(NM_CLICK, IDC_LSTC_TRANSMIT_MSG, OnClick)

END_MESSAGE_MAP()

//**********************************************************************************
//Function Name :   CTransmitMsgLIN
//Input(s)      :   -
//Output        :   -
//Functionality :   Default Constructor
//Member of     :   CTransmitMsgLIN
//Friend of     :   -
//Authors           :   Ravi D
//Date Created  :   24/10/2006
//Modifications :   -
//************************************************************************************/

CTransmitMsgLIN::CTransmitMsgLIN()
    : CFormView(CTransmitMsgLIN::IDD)
{
    m_lCount = 0;
    m_lCellPos = 0;
    m_bInitDone = FALSE;
    m_bMsgLstInit = FALSE;
    m_bStartEnabled = TRUE;
    m_bDisplayColumns = COLUMN_MSGID | COLUMN_DESCR | COLUMN_CYCLE |
                        COLUMN_CHANNEL | COLUMN_DATALEN | COLUMN_DATABYTE;
    //  m_pouFlxTxMsgBuffer = nullptr;
    m_bModified = FALSE;
    m_bDataModified  = false;
    m_CurrentMsgType = LIN_SLAVE_RESPONSE;
    m_nSelectedMsgIndex = -1;
    m_bFromEvent = false;
    vInitialiseDataBytes();
}

void CTransmitMsgLIN::OnClick(NMHDR* /* pNMHDR */, LRESULT* /* pResult */)
{
}


///**********************************************************************************
//Function Name :   ~CTransmitMsgLIN
//Input(s)      :   -
//Output            :   -
//Functionality :   Destructor
//Member of     :   CTransmitMsgLIN
//Friend of     :   -
//Authors           :   Ravi D
//Date Created  :   24/10/2006
//Modifications :   -
//************************************************************************************/

CTransmitMsgLIN::~CTransmitMsgLIN()
{
}


//**********************************************************************************
//Function Name :   DoDataExchange
//Input(s)      :   pDX - Pointer to the exchange object
//Output            :   -
//Functionality :   To exchange and validate the dialog data
//Member of     :   CTransmitMsgLIN
//Friend of     :   -
//Authors           :   Ravi D
//Date Created  :   24/10/2006
//Modifications :   -
//************************************************************************************



void CTransmitMsgLIN::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CHex_Block_EditorDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here

    //DDX_Control(pDX, IDC_LSTC_SIG_DETAILS, m_oFlexListCtrl);
    DDX_Control(pDX, IDC_COMB_CHANNEL_ID, m_wndComboChannel);
    DDX_CBIndex(pDX, IDC_COMB_CHANNEL_ID, m_nComboChannel);
    DDX_Control(pDX, IDC_STAT_MATRIX_RECT, m_odSignalMatrix);

    DDX_Control(pDX, IDC_STAT_MATRIX_RECT, m_odSignalMatrix);
    DDX_Control(pDX, IDC_LSTC_SIG_DETAILS, m_omLctrSigList);
    DDX_Control(pDX, IDC_COMB_MSG_ID_NAME, m_omComboMsgIDorName);
    DDX_Control(pDX, IDC_CBTN_ADD_MSG_LIN, m_omButtonAddMsg);
    DDX_Control(pDX, IDC_EDIT_DLC, m_odDLC);
    //DDV_MinMaxInt(pDX, 8, 8, 8);
    DDX_Control(pDX, IDC_EDIT_DB8, m_odDB[7]);
    DDX_Control(pDX, IDC_EDIT_DB7, m_odDB[6]);
    DDX_Control(pDX, IDC_EDIT_DB6, m_odDB[5]);
    DDX_Control(pDX, IDC_EDIT_DB5, m_odDB[4]);
    DDX_Control(pDX, IDC_EDIT_DB4, m_odDB[3]);
    DDX_Control(pDX, IDC_EDIT_DB3, m_odDB[2]);
    DDX_Control(pDX, IDC_EDIT_DB2, m_odDB[1]);
    DDX_Control(pDX, IDC_EDIT_DB1, m_odDB[0]);
    DDX_Check(pDX, IDC_CHKB_MSGTYPE_RTR, m_bIsRTR);
    DDX_CBString(pDX, IDC_COMB_MSG_ID_NAME, m_omStrMsgIDorName);



    DDX_CBIndex(pDX, IDC_COMB_CHANNEL_ID, m_nChannelID);


    //DDV_MinMaxInt(pDX, m_nComboChannel, 1, 16);
}




LRESULT CTransmitMsgLIN::vUserCommand(WPARAM wParam, LPARAM lParam)
{
    int nUserSel = 0;
    nUserSel = static_cast <int>(wParam);
    switch(nUserSel)
    {
        case 0:
            UpdateTxView(CTxLINDataStore::ouGetTxLINDataStoreObj().m_nChannelsConfigured);
            // Init Signal List control
            m_omLctrSigList.DeleteAllItems();
            // Clear the matrix
            m_odSignalMatrix.vResetValues();
            m_odSignalMatrix.vSetMessageLength(0);
            vSetControlProperties();
            break;
        case 2:
            OnBusConnect((ESTATUS_BUS)lParam);
            break;
    }
    return S_OK;

}

void CTransmitMsgLIN::vChangeDelButtonStatus(bool /* bStatus */)
{
    CWnd* omBtWnd = (CWnd*)GetDlgItem(IDC_DEL_FRAME);
    if ( omBtWnd != nullptr )
    {
        if( CTxLINDataStore::ouGetTxLINDataStoreObj().m_eBusStatus == BUS_DISCONNECTED && m_lstMsg.GetItemCount() > 0 )
        {
            omBtWnd->EnableWindow(TRUE);
        }
        else
        {
            omBtWnd->EnableWindow(FALSE);

        }
    }

    omBtWnd = (CWnd*)GetDlgItem(IDC_BT_DEL_ALL);
    if ( omBtWnd != nullptr )
    {
        if( CTxLINDataStore::ouGetTxLINDataStoreObj().m_eBusStatus == BUS_DISCONNECTED && m_lstMsg.GetItemCount() > 0 )
        {
            omBtWnd->EnableWindow(TRUE);
        }
        else
        {
            omBtWnd->EnableWindow(FALSE);

        }
    }

    //Master or Slave Radio Button
    omBtWnd = (CWnd*)GetDlgItem(IDC_LIN_MSGTYPE_HEADER);
    if( CTxLINDataStore::ouGetTxLINDataStoreObj().m_eBusStatus == BUS_DISCONNECTED )
    {
        omBtWnd->EnableWindow(TRUE);
    }
    else
    {
        omBtWnd->EnableWindow(FALSE);
    }

    omBtWnd = (CWnd*)GetDlgItem(IDC_LIN_MSGTYPE_SLAVE);
    if( CTxLINDataStore::ouGetTxLINDataStoreObj().m_eBusStatus == BUS_DISCONNECTED )
    {
        omBtWnd->EnableWindow(TRUE);
    }
    else
    {
        omBtWnd->EnableWindow(FALSE);
    }
    omBtWnd = (CWnd*)GetDlgItem(IDC_CBTN_ADDNEW);
    if ( omBtWnd != nullptr )
    {
        int nChannel = m_wndComboChannel.GetCurSel();
        if ( (nChannel < 0 || nChannel > CHANNEL_ALLOWED) || CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouFrameList[nChannel].size() <= 0  )
        {
            omBtWnd->EnableWindow(FALSE);
        }
        else
        {
            omBtWnd->EnableWindow(TRUE);

        }
    }

    INT nSelItem = -1;
    nSelItem = m_lstMsg.GetNextItem(-1, LVIS_SELECTED);

    if(CTxLINDataStore::ouGetTxLINDataStoreObj().m_eBusStatus == BUS_CONNECTED && (nSelItem > -1) && (m_lstMsg.GetItemCount() > 0))
    {
        GetDlgItem(IDC_CBTN_ADD_MSG_LIN)->EnableWindow(FALSE);
    }
    else
    {
        GetDlgItem(IDC_CBTN_ADD_MSG_LIN)->EnableWindow(TRUE);
    }

    if(CTxLINDataStore::ouGetTxLINDataStoreObj().m_eBusStatus == BUS_CONNECTED)
    {
        GetDlgItem(IDC_COMB_MSG_ID_NAME)->EnableWindow(FALSE);
    }
    else
    {
        GetDlgItem(IDC_COMB_MSG_ID_NAME)->EnableWindow(TRUE);
    }

    if(CTxLINDataStore::ouGetTxLINDataStoreObj().m_eBusStatus == BUS_CONNECTED)
    {
        if((nSelItem > -1) && (m_lstMsg.GetItemCount() > 0))
        {
            m_omButtonAddMsg.EnableWindow(TRUE);
        }
        else
        {
            m_omButtonAddMsg.EnableWindow(FALSE);
        }
    }
    else
    {
        if((nSelItem > -1) && (m_lstMsg.GetItemCount() > 0))
        {
            m_omButtonAddMsg.EnableWindow(FALSE);
        }
        else
        {
            m_omButtonAddMsg.EnableWindow(TRUE);
        }
    }
}

void CTransmitMsgLIN::OnBusConnect(ESTATUS_BUS eBusStatus)
{
    CTxLINDataStore::ouGetTxLINDataStoreObj().m_eBusStatus = eBusStatus;

    if ( eBusStatus == BUS_CONNECTED)
    {
        vChangeDelButtonStatus(true);
    }
    else
    {
        vChangeDelButtonStatus(false);
    }

    if(eBusStatus == BUS_CONNECTED)
    {
        INT nSelItem = m_lstMsg.GetNextItem(-1, LVIS_SELECTED);

        if(nSelItem >= 0)
        {
            if ( m_lstMsg.GetCheck(nSelItem) ==  TRUE )
            {
                m_omComboMsgIDorName.EnableWindow(FALSE);
            }
            else
            {
                m_omComboMsgIDorName.EnableWindow(TRUE);
            }
        }
    }
    else
    {
        m_omComboMsgIDorName.EnableWindow(TRUE);
    }
}

void CTransmitMsgLIN::vGetStringFromValue(int nValue, std::string& strValue, bool bSymbol)
{
    char chValue[1024];
    if ( CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode == false)
    {
        if ( bSymbol == true )
        {
            sprintf_s(chValue, 1024, "%d", nValue);
        }
        else
        {
            sprintf_s(chValue, 1024, "%03d", nValue);
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
            sprintf_s(chValue, 1024, "%02X", nValue);
        }
    }
    strValue = chValue;
}


void CTransmitMsgLIN::OnInitialUpdate()
{
    char caColumnName[defMESSAGE_BLOCK_COLUMN][defSTRING_SIZE];
    strncpy(caColumnName[0], defMESSAGE_BLOCK_NAME, defSTRING_SIZE);
    strncpy(caColumnName[1], defMESSAGE_BLOCK_TRIGGER, defSTRING_SIZE);
    strncpy(caColumnName[2], defMESSAGE_BLOCK_TRIG_VAL, defSTRING_SIZE);

    CFormView::OnInitialUpdate();

    m_bUpdating = FALSE;
    CWnd* pParentWnd = nullptr;
    // Get Parent window pointer
    pParentWnd = pomGetParentWindow();
    // Update view pointer
    if( pParentWnd != nullptr )
    {
        // Register this view pointer
        ((CTxMsgChildFrame*)pParentWnd)->m_pLinTransmitMsgView = this;
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
    m_omLctrMsgBlockName.GetWindowRect( &rListCtrlRect);
    nTotalColunmSize     = rListCtrlRect.right - rListCtrlRect.left;
    for(UINT i=0; i<defMESSAGE_BLOCK_COLUMN; i++)
    {
        nTotalStrLengthPixel +=
            m_omLctrMsgBlockName.GetStringWidth(caColumnName[i]) ;
    }
    for(UINT i=0; i<defMESSAGE_BLOCK_COLUMN; i++)
    {
        nColumnSize  = m_omLctrMsgBlockName.GetStringWidth(caColumnName[i]) ;
        nColumnSize +=
            (nTotalColunmSize-nTotalStrLengthPixel)/defMESSAGE_BLOCK_COLUMN;
        m_omLctrMsgBlockName.InsertColumn(i,caColumnName[i],
                                          LVCFMT_CENTER, nColumnSize);
    }
    m_omLctrMsgBlockName.SetExtendedStyle( LVS_EX_FULLROWSELECT);

    // Setup the list control to add the messages.
    m_lstMsg.SubclassDlgItem(IDC_LSTC_TRANSMIT_MSG, this);
    m_lstMsg.SetExtendedStyle( LVS_EX_FULLROWSELECT |  LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES);

    CWnd* pWnd = m_lstMsg.GetWindow(GW_CHILD);
    ASSERT(pWnd != nullptr);


    //m_lstMsg.SetEditable();


    CRect rctList;
    m_lstMsg.GetWindowRect(&rctList);
    int nColWidth = rctList.Width() - 2;
    int colwidths[] = { 10, 5, 5, 7, 7, 7, 4, 15 };
    int total_cx = 0;

    TCHAR* acColData[]=
    {
        _("Message Name"), _("Frame Id"), _("Channel"), _("Data Length"), _("Message Type"),
        _("Repetition"), _("Key"), _("Data Bytes")
    };

    LVCOLUMN lvcolumn;
    memset(&lvcolumn, 0, sizeof(lvcolumn));
    lvcolumn.mask =  LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;



    // Associate Header control Image List
    if( m_omHeaderImageList.m_hImageList == nullptr )
    {
        m_omHeaderImageList.Create( IDR_BMP_CHECKBOX, 16, 1, RGB(0,0,255) );
    }
    // Set the Image List
    // Only if it is sucessfully created
    if( m_omHeaderImageList.m_hImageList != nullptr )
    {
        m_lstMsg.SetImageList(&m_omHeaderImageList, LVSIL_SMALL);
        CHeaderCtrl* pHeader = m_lstMsg.GetHeaderCtrl();
        if( pHeader != nullptr )
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

    // add columns
    ColumnProperty ouProperty;
    ouProperty.bMovable = true;
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
        lvcolumn.cx = (acColData[i + 1] == nullptr) ? nColWidth - total_cx - 2 : (nColWidth * colwidths[i]) / 64;
        total_cx += lvcolumn.cx;
        ouProperty.bMovable = true;
        if ( i== FIFTH_COL|| i == SEVENTH_COL )
        {
            ouProperty.bMovable = false;
        }

        m_lstMsg.InsertColumn(i, &lvcolumn);

    }


    // Set the Image List
    // Only if it is sucessfully created
    /*if( m_omHeaderImageList.m_hImageList != nullptr )
    {
        //m_lstMsg.SetImageList(&m_omHeaderImageList, LVS_SMALLICON);
        CHeaderCtrl* pHeader = m_lstMsg.GetHeaderCtrl();
        if( pHeader != nullptr )
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
    }*/


    m_hTransmitWnd = m_hWnd;



    int nConfigChannel = CTxLINDataStore::ouGetTxLINDataStoreObj().m_nChannelsConfigured;
    CString strChannel;

    for ( int i = 0 ; i < nConfigChannel ; i++ )
    {
        strChannel.Format("%d", i+1);
        m_wndComboChannel.AddString(strChannel);
    }

    m_wndComboChannel.SetCurSel(0);

    CButton* pBtWnd = (CButton*)GetDlgItem(IDC_RADIO_BYTE_VIEW);
    if ( pBtWnd != nullptr)
    {
        pBtWnd->SetCheck(TRUE);
    }


    UpdateTxView(CTxLINDataStore::ouGetTxLINDataStoreObj().m_nChannelsConfigured);
    m_bInitDone = FALSE;
    InitFlexList();
    if ( CTxLINDataStore::ouGetTxLINDataStoreObj().m_eBusStatus == BUS_CONNECTED)
    {
        vChangeDelButtonStatus(false);
    }
    else
    {
        vChangeDelButtonStatus(true);
    }
    //vChangeDelButtonStatus(!CTxLINDataStore::ouGetTxLINDataStoreObj().m_eBusStatus);
    //GetDlgItem(IDC_BT_UPDATE)->EnableWindow(FALSE);



    // Set Default values for Data bytes and othe CAN attributes
    vSetDefaultValues();

    vSetControlProperties();



    // Init Signal List control
    vInitSignalListCtrl();
    UpdateTxView(CTxLINDataStore::ouGetTxLINDataStoreObj().m_nChannelsConfigured);
    // Clear the matrix
    m_odSignalMatrix.vResetValues();
    m_odSignalMatrix.vSetMessageLength(0);

    /*SLISTINFO       sListInfo;
    sListInfo.m_eType = eBuddy;
    m_omLctrSigList.vSetColumnInfo(0, 2, sListInfo);*/


}


/*******************************************************************************
 Function Name    : vInitSignalListCtrl
 Input(s)         : -
 Output           : -
 Functionality    : This function will be called from OnInitDialog to init
                    signal list.
 Member of        : CTxMsgDetailsView
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 19.07.2004
 Modification By  : Raja N
 Modification on  : 02.08.2004, Removed Hardcoded values, Moved all create
                    functions in to OnCreate handler, fixes for 64 bit signal
                    value problem
*******************************************************************************/
void CTransmitMsgLIN::vInitSignalListCtrl()
{
    char caColumnName[4][defSIGNAL_LIST_STRING_MAX_LENGTH];
    strncpy(caColumnName[0], defSTR_COL_SIGNAL_NAME, defSIGNAL_LIST_STRING_MAX_LENGTH);
    strncpy(caColumnName[1], defSTR_RAW_COLUMN, defSIGNAL_LIST_STRING_MAX_LENGTH);
    strncpy(caColumnName[2], defSTR_PHYSICAL_COLUMN, defSIGNAL_LIST_STRING_MAX_LENGTH);
    strncpy(caColumnName[3], defSTR_SIGNAL_UNIT, defSIGNAL_LIST_STRING_MAX_LENGTH);

    INT nColumnFormat[]     = { LVCFMT_LEFT,
                                LVCFMT_CENTER,
                                LVCFMT_CENTER,
                                LVCFMT_LEFT
                              };
    RECT rListCtrlRect;
    INT nTotalColunmSize = 0;
    INT nTotalStrLengthPixel = 0;
    INT nColumnSize = 0;
    // Set the Image List
    /*if( m_omSigImageList.m_hImageList == nullptr )
    {
        m_omSigImageList.Create( IDR_BMP_MSG_SIG_WATCH, 16,
                                 1, defCOLOR_WHITE );
    }
    m_omLctrSigList.SetImageList(&m_omSigImageList, LVSIL_SMALL);*/

    //Calculate the total size of all column header
    m_omLctrSigList.GetWindowRect( &rListCtrlRect);
    nTotalColunmSize     = rListCtrlRect.right - rListCtrlRect.left;
    nTotalStrLengthPixel = 0;

    UINT i; //i declared outside loop
    for(i=0; i<defSIGNAL_FRAME_COLUMN; i++)
    {
        nTotalStrLengthPixel +=
            m_omLctrSigList.GetStringWidth(_(caColumnName[i])) ;
    }
    //Insert each column name after calculating the size for the same.
    for(i=0; i<4; i++)
    {
        nColumnSize  = m_omLctrSigList.GetStringWidth(_(caColumnName[i])) ;
        nColumnSize +=
            (nTotalColunmSize-nTotalStrLengthPixel)/defSIGNAL_FRAME_COLUMN;
        m_omLctrSigList.InsertColumn(i, _(caColumnName[i]),
                                     nColumnFormat[i], nColumnSize - 1);
    }

    // Set the extended style
    m_omLctrSigList.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    SLISTINFO       sListInfo;

    // Init Signal Name column
    // Column 0 : User Function call
    sListInfo.m_eType = eUser;
    m_omLctrSigList.vSetColumnInfo(0, 0, sListInfo);
    //sProgInfo.m_pfHandler = vSignalNameHandler;
    /* sProgInfo.m_pUserParam = this;
     m_omLctrSigList.vSetUserProgInfo(0, 0, sProgInfo);*/

    SNUMERICINFO sNumInfo;

    sNumInfo.m_uMinVal.m_un64Value = 0;
    sNumInfo.m_uMaxVal.m_un64Value = 100;
    sNumInfo.m_uDelta.m_un64Value = 1;

    sNumInfo.m_byFlag = BUDDY_CTRL;
    // Physical value should be in dec always
    sNumInfo.m_byBase = defBASE_DEC;

    // Init Raw Value column
    // Column 1 : Numeric Edit with Spin Control
    sListInfo.m_eType = eBuddy;
    m_omLctrSigList.vSetColumnInfo(0, 1, sListInfo);
    m_omLctrSigList.vSetNumericInfo(0, 1, sNumInfo);

    // Init Phy Value column
    // Column 2 : Numeric Edit with Spin Control
    sListInfo.m_eType = eBuddy;
    m_omLctrSigList.vSetColumnInfo(0, 2, sListInfo);
    m_omLctrSigList.vSetNumericInfo(0, 2, sNumInfo);

    // Init Unit column
    // Column 3 : Numeric Edit with Spin Control
    // Init is not required as it non-editable column


    // Disable Signal List as there is no selected message at this time
    //m_omLctrSigList.EnableWindow( FALSE );
    // Signal Matrix is not created at this point of time
    // Disable that after create
}

void CTransmitMsgLIN::vSetControlProperties()
{
    // Temp base holder
    INT nTempBase = -1;
    // Limit number of character input
    UINT unNumberOfChars = 0;


    // Set the control properties
    // to accept data in HEX if the base mode is HEXADECIMAL
    if (true == CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode)
    {
        nTempBase = BASE_HEXADECIMAL;
        unNumberOfChars = 2;
    }
    else
    {
        nTempBase = BASE_DECIMAL;
        unNumberOfChars = 3;
    }

    for(INT nIndex = 0; nIndex < defMAX_BYTE; nIndex++)
    {
        m_odDB[nIndex].vSetBase(nTempBase);
    }

    // Base for DLC
    m_odDLC.vSetBase(BASE_DECIMAL);

    // Set number of characters for input
    //  DLC
    m_odDLC.SetLimitText(1);

    // Databytes
    for(INT nIndex = 0; nIndex < defMAX_BYTE; nIndex++)
    {
        m_odDB[nIndex].SetLimitText(unNumberOfChars);
    }

    // Set all the edit controls
    // to accept unsigned numbers
    m_odDLC.vSetSigned( FALSE );
    // Databytes
    for(INT nIndex = 0; nIndex < defMAX_BYTE; nIndex++)
    {
        m_odDB[nIndex].vSetSigned( FALSE);
    }

    if(((CButton*)(GetDlgItem(IDC_LIN_MSGTYPE_HEADER)))->GetCheck() == FALSE)
    {
        ((CButton*)(GetDlgItem(IDC_LIN_MSGTYPE_SLAVE)))->SetCheck(TRUE);
    }
}

void CTransmitMsgLIN::onLinSlaveTypeSelected()
{
    m_CurrentMsgType = LIN_SLAVE_RESPONSE;
    char chText[MAX_PATH] = "";
    //1. Enable DLC and Databytes
    m_odDLC.SetReadOnly(false);
    /*for ( int nId = 0; nId <= 8; nId++ )
    {
        m_odDB[nId].SetReadOnly(false);
    }*/
    vUpdateDataBytes();
    //2. Update Text In List Ctrl if any item is selected;
    int nSelItem = m_lstMsg.GetNextItem(-1, LVNI_SELECTED);
    if ( -1 != nSelItem )
    {
        std::list<LIN_FRAME_DATA>::iterator itrList = CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouLIN_Frame_Data.begin();
        advance(itrList, nSelItem);
        if ( itrList !=  CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouLIN_Frame_Data.end() )
        {
            std::string strVal, strTemp;
            sprintf(chText, "%d", itrList->m_ouLinMessage.m_ucDataLen);

            m_lstMsg.SetItemText(nSelItem, def_COLUMN_DLC_INDEX, chText);

            strVal = "";
            for ( int i =0 ; i < itrList->m_ouLinMessage.m_ucDataLen; i++ )
            {
                vGetStringFromValue(itrList->m_ouLinMessage.m_ucData[i], strTemp, false);
                strVal += strTemp;
                strVal += " ";
            }
            m_lstMsg.SetItemText(nSelItem, def_COLUMN_DATABYTES_INDEX, strVal.c_str());

            m_lstMsg.SetItemText( nSelItem, def_COLUMN_MSGTYPE_INDEX, "Slave" );

            itrList->m_ouLinMessage.m_ucMsgTyp = m_CurrentMsgType;
        }

    }
    m_omLctrSigList.EnableWindow(true);

}

void CTransmitMsgLIN::onLinHeaderTypeSelected()
{
    m_CurrentMsgType = LIN_MASTER_RESPONSE;
    m_omLctrSigList.EnableWindow(false);

    //1. Disable DLC and Databytes
    m_odDLC.SetReadOnly(true);
    for ( int nId = 0; nId < 8; nId++ )
    {
        m_odDB[nId].SetReadOnly(true);
    }

    //2. Update Text In List Ctrl if any item is selected;
    int nSelItem = m_lstMsg.GetNextItem(-1, LVNI_SELECTED);
    if ( -1 != nSelItem )
    {
        m_lstMsg.SetItemText( nSelItem, def_COLUMN_DLC_INDEX, "" );
        m_lstMsg.SetItemText( nSelItem, def_COLUMN_DATABYTES_INDEX, "" );
        m_lstMsg.SetItemText( nSelItem, def_COLUMN_MSGTYPE_INDEX, "Master" );
        std::list<LIN_FRAME_DATA>::iterator itrList = CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouLIN_Frame_Data.begin();
        advance(itrList, nSelItem);
        if ( itrList !=  CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouLIN_Frame_Data.end() )
        {
            itrList->m_ouLinMessage.m_ucMsgTyp = m_CurrentMsgType;
        }

    }
}


/*******************************************************************************
Function Name  : pomGetParentWindow
Input(s)       : -
Output         : CWnd * - Pointer to CTxMsgChildFrame
Functionality  : This Function will return parent window pointer. That is
pointer to CTxMsgChildFrame. This will return nullptr incase of
failure
Member of      : CTxFunctionsView
Author(s)      : Raja N
Date Created   : 25.4.2005
Modifications  :
*******************************************************************************/
CWnd* CTransmitMsgLIN::pomGetParentWindow() const
{
    CWnd* pWnd = nullptr;
    // Get Splitter window pointer
    pWnd = GetParent();
    // Get Tx Msg Child Window pointer from Splitter window pointer
    if( pWnd != nullptr )
    {
        pWnd = pWnd->GetParent();
    }
    // Return Tx Msg Child window pointer or nullptr incase of failure
    return pWnd;
}



void CTransmitMsgLIN::OnBtnClickedAddnew()
{

}



void CTransmitMsgLIN::GetTotalClientRect(LPRECT lpRect)
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

int CTransmitMsgLIN:: nGetCheckedItemCount()
{
    int nSelectedItemCount = 0;
    for(int nIndex = 0; nIndex < m_lstMsg.GetItemCount() + 1; nIndex++)
    {
        if ( m_lstMsg.GetCheck(nIndex) ==  TRUE )
        {
            nSelectedItemCount++;
        }
    }
    return  nSelectedItemCount;
}




//********************************************************************************
//  Function Name       : InitFlexList
//  Input(s)            : none
//  Output              : void
//  Description         : Init The FlexListCtrl
//  Member of           : CTransmitMsgLIN
//  Friend of           : None
//  Author              : Ashwin R Uchil
//  Creation Date       : 21.3.2013
//  Modifications       :
//********************************************************************************

//void CTransmitMsgLIN::InitFlexList(BYTE yByteCount, unsigned short* pwBuffer)
void CTransmitMsgLIN::DeleteAllMsgListColumns()
{
    int nColumns=m_omLctrSigList.GetHeaderCtrl()->GetItemCount();

    for ( int i = nColumns-1; i >= 0; i--)
    {
        m_omLctrSigList.DeleteColumn (i);
    }
}
void CTransmitMsgLIN::InitFlexList()
{
    m_omLctrSigList.DeleteAllItems();
    DeleteAllMsgListColumns();

    //CHAR caColumnName[][defSIGNAL_LIST_STRING_MAX_LENGTH]
    //= { "Signal Name",
    //  "Raw Value",
    //  "Physical Value",
    //  "Unit"
    //};
    //INT nColumnFormat[]     = { LVCFMT_RIGHT,
    //  LVCFMT_CENTER,
    //  LVCFMT_CENTER,
    //  LVCFMT_CENTER
    //};
    //RECT rListCtrlRect;
    //INT nTotalColunmSize = 0;
    //INT nTotalStrLengthPixel = 0;
    //INT nColumnSize = 0;

    //m_omLctrSigList.GetWindowRect( &rListCtrlRect);
    //nTotalColunmSize     = rListCtrlRect.right - rListCtrlRect.left;
    //nTotalStrLengthPixel = 0;

    //UINT i; //i declared outside loop
    //for(i=0; i<4; i++)
    //{
    //  nTotalStrLengthPixel +=
    //      m_omLctrSigList.GetStringWidth(caColumnName[i]) ;
    //}
    ////Insert each column name after calculating the size for the same.
    //for(i=0; i<4; i++)
    //{
    //  nColumnSize  = m_omLctrSigList.GetStringWidth(caColumnName[i]) ;
    //  nColumnSize +=
    //      (nTotalColunmSize-nTotalStrLengthPixel)/4;
    //  m_omLctrSigList.InsertColumn(i, caColumnName[i],
    //      nColumnFormat[i], nColumnSize - 1);
    //}

    //// Set the extended style
    //m_omLctrSigList.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    //SNUMERICINFO    sNumInfo;
    //SLISTINFO       sListInfo;
    //SUSERPROGINFO   sProgInfo;




    //sListInfo.m_eType = eNoControl;
    //m_omLctrSigList.vSetColumnInfo(0, 1, sListInfo);
    //sNumInfo.m_byBase = BASE_HEXADECIMAL;
    //sNumInfo.m_nTextLimit = 4;
    //m_omLctrSigList.vSetNumericInfo(0, 1, sNumInfo);

    //// Column 2 : Numeric Edit with Spin Control
    //sListInfo.m_eType = eNoControl;
    //m_omLctrSigList.vSetColumnInfo(0, 2, sListInfo);
    ////m_oFlexListCtrl.vSetNumericInfo(0, 2, sNumInfo);

    //sListInfo.m_eType = eNoControl;
    //m_omLctrSigList.vSetColumnInfo(0, 3, sListInfo);
    ////m_oFlexListCtrl.vSetNumericInfo(0, 3, sNumInfo);

    //sListInfo.m_eType = eNoControl;
    //m_omLctrSigList.vSetColumnInfo(0, 4, sListInfo);
    //m_oFlexListCtrl.vSetNumericInfo(0, 4, sNumInfo);



}

//********************************************************************************
//  Function Name       : OnBnClickedBtUpdate
//  Input(s)            : None
//  Output              : void
//  Description         : None
//  Member of           : CTransmitMsgLIN
//  Friend of           : None
//  Author              : Anupam Kumar
//  Creation Date       : 16/03/2007
//  Modifications       :
//********************************************************************************

void CTransmitMsgLIN::OnBnClickedBtUpdate()
{


}


//********************************************************************************
//  Function Name       : OnBnClickedBtDelFrame
//  Input(s)            : None
//  Output              : void
//  Description         : None
//  Member of           : CTransmitMsgLIN
//  Friend of           : None
//  Author              : Ashwin R Uchil
//  Creation Date       : 24.4.2013
//  Modifications       :
//********************************************************************************
void CTransmitMsgLIN::OnBnClickedBtDelFrame()
{
    int nSelItem = m_lstMsg.GetNextItem(-1, LVNI_SELECTED);
    if ( -1 != nSelItem )
    {
        vDeleteFrameFrmList(nSelItem);
        if(m_lstMsg.GetItemCount() > 0)
        {
            nSelItem = nSelItem % m_lstMsg.GetItemCount();
            m_lstMsg.SetItemState(nSelItem, LVIS_SELECTED, LVIS_SELECTED);
            m_lstMsg.SetSelectionMark(nSelItem);
        }
        else
        {
            vClearAllViews();
        }
    }
    vChangeDelButtonStatus();

}

void CTransmitMsgLIN::vClearAllViews()
{
    // Init Signal List control
    m_omLctrSigList.DeleteAllItems();
    // Clear the matrix
    m_odSignalMatrix.vResetValues();
    m_odSignalMatrix.vSetMessageLength(0);
    vSetDefaultValues();
    m_omButtonAddMsg.SetWindowText(def_ADD_MESSAGE_SRING);
}

void CTransmitMsgLIN::vDeleteFrameFrmList(INT nIndex)
{
    m_lstMsg.DeleteItem(nIndex);
    CTxLINDataStore::ouGetTxLINDataStoreObj().nDeleteMessageAt(nIndex);
}

void CTransmitMsgLIN::vDeleteAllFrames()
{
    m_lstMsg.DeleteAllItems();
    CTxLINDataStore::ouGetTxLINDataStoreObj().nDeleteAllMessages();
    CTxLINDataStore::ouGetTxLINDataStoreObj().nDeleteAllKeyEntrs();
}

//********************************************************************************
//  Function Name       : OnBnClickedBtDelAll
//  Input(s)            :
//  Output              : void
//  Description         : Event handler fordeleting msg block/group
//  Member of           : CTransmitMsgLIN
//  Friend of           : None
//  Author              : Anish kr
//  Creation Date       : 14/03/07
//  Modifications       :
//********************************************************************************

void CTransmitMsgLIN::OnBnClickedBtDelAll()
{
    int nLstCount = m_lstMsg.GetItemCount();

    if(nLstCount > 0)
    {
        if(AfxMessageBox("Are you sure you want to clear all frames in the block?",MB_YESNO) == IDNO )
        {
            return;
        }
    }

    vDeleteAllFrames();
    vClearAllViews();
    vChangeDelButtonStatus();
}

void CTransmitMsgLIN::OnAddMessageClick()
{
    LVITEM lvitem = {0};
    lvitem.mask =  LVIF_DI_SETITEM;

    m_lstMsg.GetItem(&lvitem);
    CString strBtnText = "";
    m_omButtonAddMsg.GetWindowText(strBtnText);

    if(strBtnText.CompareNoCase(def_SEND_Message) == 0)
    {
        int nSel = m_lstMsg.GetNextItem(-1, LVIS_SELECTED);
        if ( nSel >= 0)
        {
            CTxLINDataStore::ouGetTxLINDataStoreObj().nTransmitMsgAt(nSel);
        }

        return;
    }

    BOOL bDataValid = FALSE;
    // Validate the data
    LIN_FRAME_DATA ouLinFrame;
    bDataValid = bValidateData(ouLinFrame);
    if(bDataValid == TRUE )
    {
        ouLinFrame.m_ouLinMessage.m_ucChksum = 0;
        ouLinFrame.m_ouLinMessage.m_ucChksumTyp = 0;
        ouLinFrame.m_ouLinMessage.m_ucMsgTyp = eGetMessageType();
        ouLinFrame.m_ouLinMessage.m_ucChannel = nGetChannel();

        nGetMessageName(ouLinFrame.m_strFrameName);
        nAddMessageToList(ouLinFrame);


        //Follwoing lines has to be removed
        /*ouLinFrame.nActualTimer = 10;
        ouLinFrame.nCurrentTimerVal = 10;
        ouLinFrame.m_bTimerEnabled = true;*/

        CTxLINDataStore::ouGetTxLINDataStoreObj().nInsertMessage(ouLinFrame);

    }
    vUpdateStateDataBytes();
    vChangeDelButtonStatus();
}

int CTransmitMsgLIN::nGetChannel()
{
    return ((CComboBox*)GetDlgItem(IDC_COMB_CHANNEL_ID))->GetCurSel() + 1;
}

void CTransmitMsgLIN::vUpdateMsgInList(LIN_FRAME_DATA ouLinData)
{
    INT nRows = m_lstMsg.GetNextItem(-1, LVIS_SELECTED);
    //INT nRows = m_nSelectedMsgIndex;
    if ( nRows < 0 )
    {
        nRows = m_lstMsg.GetItemCount();
    }

    m_lstMsg.SetItemText(nRows, def_COLUMN_MSGNAME_INDEX, ouLinData.m_strFrameName.c_str());

    std::string strText;

    vGetStringFromValue(ouLinData.m_ouLinMessage.m_ucMsgID, strText);
    m_lstMsg.SetItemText(nRows, def_COLUMN_MSGID_INDEX, strText.c_str());

    strText = "";
    if ( ouLinData.m_ouLinMessage.m_ucMsgTyp == LIN_SLAVE_RESPONSE )
    {
        vGetStringFromValue(ouLinData.m_ouLinMessage.m_ucDataLen, strText);
    }
    m_lstMsg.SetItemText(nRows, def_COLUMN_DLC_INDEX, strText.c_str());

    char chValue[MAX_PATH];
    sprintf_s(chValue, MAX_PATH, "%d", ouLinData.m_ouLinMessage.m_ucChannel);
    m_lstMsg.SetItemText(nRows, def_COLUMN_CHANNEL_INDEX, chValue);


    if ( ouLinData.m_ouLinMessage.m_ucMsgTyp == LIN_SLAVE_RESPONSE )
    {
        strText = "Slave";
    }
    else
    {
        strText = "Master";
    }

    m_lstMsg.SetItemText(nRows, def_COLUMN_MSGTYPE_INDEX, strText.c_str());

    /*vGetStringFromValue(ouLinData.nCurrentTimerVal, strText);

    SLISTINFO ouListInfo;
    ouListInfo.m_eType = eNumber;
    sUserProgInfo ouUserProgInfo;
    ouUserProgInfo.m_bIcon = true;
    ouUserProgInfo.m_pfHandler = vCheckBoxClickHandler;
    ouUserProgInfo.m_pUserParam = this;

    m_lstMsg.vSetColumnInfo(nRows, def_COLUMN_REPETITION_INDEX, ouListInfo);
    m_lstMsg.vSetUserProgInfo(nRows, def_COLUMN_REPETITION_INDEX, ouUserProgInfo);
    m_lstMsg.SetItemText(nRows, def_COLUMN_REPETITION_INDEX, strText.c_str());*/

    /*strText = ouLinData.m_chKeyVal;

    ouListInfo.m_eType = eAlphaNumericType;
    ouUserProgInfo.m_bIcon = true;
    ouUserProgInfo.m_pfHandler= vCheckBoxClickHandler;
    ouUserProgInfo.m_pUserParam = this;

    m_lstMsg.vSetColumnInfo(nRows, def_COLUMN_KEY_INDEX, ouListInfo);
    m_lstMsg.vSetUserProgInfo(nRows, def_COLUMN_KEY_INDEX, ouUserProgInfo);
    m_lstMsg.SetItemText(nRows, def_COLUMN_KEY_INDEX, strText.c_str());*/

    std::string strTemp;
    strText = "";
    if ( ouLinData.m_ouLinMessage.m_ucMsgTyp == LIN_SLAVE_RESPONSE )
    {
        for ( int i =0 ; i < ouLinData.m_ouLinMessage.m_ucDataLen; i++ )
        {
            vGetStringFromValue(ouLinData.m_ouLinMessage.m_ucData[i], strTemp, false);
            strText += strTemp;
            strText += " ";
        }
    }

    m_lstMsg.SetItemText(nRows, def_COLUMN_DATABYTES_INDEX, strText.c_str());

    m_lstMsg.SetCheck(nRows, ouLinData.bSelected);
}

int CTransmitMsgLIN::nAddMessageToList(LIN_FRAME_DATA& ouLinData, int nRows)
{
    if ( nRows <= 0 )
    {
        nRows = m_lstMsg.GetItemCount();
    }
    m_lstMsg.InsertItem(nRows, ouLinData.m_strFrameName.c_str(), -1);


    std::string strText;

    vGetStringFromValue(ouLinData.m_ouLinMessage.m_ucMsgID, strText);
    m_lstMsg.SetItemText(nRows, def_COLUMN_MSGID_INDEX, strText.c_str());

    char chText[MAX_PATH] = {0};
    sprintf_s(chText, MAX_PATH, "%d", ouLinData.m_ouLinMessage.m_ucDataLen);
    //vGetStringFromValue(ouLinData.m_ouLinMessage.m_ucDataLen, strText);
    if ( ouLinData.m_ouLinMessage.m_ucMsgTyp == LIN_SLAVE_RESPONSE )
    {
        m_lstMsg.SetItemText(nRows, def_COLUMN_DLC_INDEX, chText);
    }
    else
    {
        m_lstMsg.SetItemText(nRows, def_COLUMN_DLC_INDEX, "");
    }

    char chValue[MAX_PATH];
    sprintf_s(chValue, MAX_PATH, "%d", ouLinData.m_ouLinMessage.m_ucChannel);
    m_lstMsg.SetItemText(nRows, def_COLUMN_CHANNEL_INDEX, chValue);




    if ( ouLinData.m_ouLinMessage.m_ucMsgTyp == LIN_SLAVE_RESPONSE )
    {
        strText = "Slave";
    }
    else
    {
        strText = "Master";
    }

    m_lstMsg.SetItemText(nRows, def_COLUMN_MSGTYPE_INDEX, strText.c_str());


    sprintf_s(chText, MAX_PATH, "%d", ouLinData.nActualTimer);
    //vGetStringFromValue(ouLinData.nCurrentTimerVal, strText);

    SLISTINFO ouListInfo;
    ouListInfo.m_eType = eNumber;
    sUserProgInfo ouUserProgInfo;
    ouUserProgInfo.m_bIcon = true;
    ouUserProgInfo.m_pfHandler = vCheckBoxClickHandler;
    ouUserProgInfo.m_pUserParam = this;

    m_lstMsg.vSetColumnInfo(nRows, def_COLUMN_REPETITION_INDEX, ouListInfo);
    m_lstMsg.vSetUserProgInfo(nRows, def_COLUMN_REPETITION_INDEX, ouUserProgInfo);
    m_lstMsg.SetItemText(nRows, def_COLUMN_REPETITION_INDEX, chText);

    strText = ouLinData.m_chKeyVal;

    ouListInfo.m_eType = eAlphaNumericType;
    ouUserProgInfo.m_bIcon = true;
    ouUserProgInfo.m_pfHandler= vCheckBoxClickHandler;
    ouUserProgInfo.m_pUserParam = this;

    m_lstMsg.vSetColumnInfo(nRows, def_COLUMN_KEY_INDEX, ouListInfo);
    m_lstMsg.vSetUserProgInfo(nRows, def_COLUMN_KEY_INDEX, ouUserProgInfo);
    m_lstMsg.SetItemText(nRows, def_COLUMN_KEY_INDEX, strText.c_str());

    if(ouLinData.bKeyEnabled == TRUE)
    {
        CTxLINDataStore::ouGetTxLINDataStoreObj().nInsertKey(nRows, strText[0]);
    }

    std::string strTemp = "";
    strText = "";
    if ( ouLinData.m_ouLinMessage.m_ucMsgTyp == LIN_SLAVE_RESPONSE )
    {
        std::string strTemp = "";

        for ( int i =0 ; i < ouLinData.m_ouLinMessage.m_ucDataLen; i++ )
        {
            vGetStringFromValue(ouLinData.m_ouLinMessage.m_ucData[i], strTemp, false);
            strText += strTemp;
            strText += " ";
        }
    }

    m_lstMsg.SetItemText(nRows, def_COLUMN_DATABYTES_INDEX, strText.c_str());

    LVITEM lvitem = {0};
    lvitem.mask =  LVIF_IMAGE;
    lvitem.pszText = "";
    lvitem.iImage = ouLinData.m_bTimerEnabled; // Index of first subitem image
    lvitem.iItem = nRows; // Index of item
    lvitem.iSubItem = def_COLUMN_REPETITION_INDEX; // Index of subitem
    m_lstMsg.SetItem(&lvitem);

    lvitem.mask =  LVIF_IMAGE;
    lvitem.pszText = "a";
    lvitem.iImage = ouLinData.bKeyEnabled; // Index of first subitem image
    lvitem.iItem = nRows; // Index of item
    lvitem.iSubItem = def_COLUMN_KEY_INDEX; // Index of subitem
    m_lstMsg.SetItem(&lvitem);
    //m_lstMsg.SetItem(nRows, 6, LVIF_IMAGE, "H", 1, 0, 0, 0);
    //m_lstMsg.SetSelectionMark(nRows);

    m_lstMsg.SetCheck(nRows, ouLinData.bSelected);
    return S_OK;
}


eLIN_MSG_TYPE CTransmitMsgLIN::eGetMessageType()
{
    if ( ((CButton*)GetDlgItem(IDC_LIN_MSGTYPE_SLAVE))->GetCheck() == TRUE )
    {
        return LIN_SLAVE_RESPONSE;
    }
    return LIN_MASTER_RESPONSE;
}

void CTransmitMsgLIN::OnClose()
{
    CFormView::OnClose();
}


void CTransmitMsgLIN::vLoadTxMsgConfig()
{
    BYTE* SectionBuffer = nullptr;
    int nBufferLength = 0;
    //Remove all the TX msg block before copying from config file
    // CTxLINDataStore::ouGetTxLINDataStoreObj().vRemoveAllBlock(m_omMsgBlockMan);
    //CTxLINDataStore::ouGetTxFlexRayDataStoreObj().vRemoveAllBlock(CTxLINDataStore::ouGetTxFlexRayDataStoreObj().m_omTxMsgBlockMan);
    // First Get Tx Msg List

    CConfigDetails::ouGetConfigDetailsObject().bGetData((void*&) SectionBuffer,
            nBufferLength,TX_MSG_WND_CONFIG_SECTION_NAME);
    if (SectionBuffer != nullptr)
    {
        delete [] SectionBuffer;
    }
    else //new file : Load default value TBD
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

void CTransmitMsgLIN::vRefreshTxMsgWindow()
{
    m_bInitDone = TRUE;
    // Clear the message list control
    m_lstMsg.DeleteAllItems();
    //Remove all items from the list control
    m_omLctrMsgBlockName.DeleteAllItems();
}



void CTransmitMsgLIN::OnDestroy()
{

}
/*******************************************************************************
Function Name  : OnBtnClose
Input(s)       : -
Output         : -
Functionality  : This button handler will be called during Close button press.
This function will send WM_CLOSE message to close Tx Child
window.
Member of      : CTransmitMsgLIN
Author(s)      : Ashwin R Uchil
Date Created   : 16.4.2013
Modifications  :
*******************************************************************************/
void CTransmitMsgLIN::OnBtnClose()
{
    // Get Child Frame Pointer
    CWnd* pWnd = nullptr;
    pWnd = pomGetParentWindow();
    // Post close message
    if( pWnd != nullptr )
    {
        // This will invoked the InvokeClose function
        ((CTxMsgChildFrame*)pWnd)->PostMessage( WM_CLOSE );
    }
}



HRESULT CTransmitMsgLIN::UpdateTxView(int& m_nChannelsConfigured)
{
    int nChannel = m_wndComboChannel.GetCurSel();


    //Not yet No Channel was added
    if ( nChannel < 0 )
    {
        CString strChannel;
        m_wndComboChannel.Clear();
        for ( int i = 0 ; i < m_nChannelsConfigured ; i++ )
        {
            strChannel.Format("%d", i+1);
            m_wndComboChannel.AddString(strChannel);
        }

        m_wndComboChannel.SetCurSel(0);
    }

    nChannel = m_wndComboChannel.GetCurSel();

    //Atleast now any channels are added
    if ( nChannel < 0 || nChannel > CHANNEL_ALLOWED )
    {
        return S_FALSE;
    }
    vSetDefaultValues();
    m_lstMsg.DeleteAllItems();

    vUpdateMessageList();


    m_omComboMsgIDorName.ResetContent();
    char chString[MAX_PATH] = "";
    std::map<int, std::string>::iterator itrMsgId = CTxLINDataStore::ouGetTxLINDataStoreObj().m_strMessageNames[nChannel].begin();
    while (itrMsgId != CTxLINDataStore::ouGetTxLINDataStoreObj().m_strMessageNames[nChannel].end())
    {
        if ( CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode == false )
        {
            //sprintf_s(chString, MAX_PATH, "%-25s [%8d]", itrMsgId->second.c_str(), itrMsgId->first);
            sprintf_s(chString, MAX_PATH, "%s [%0d]", itrMsgId->second.c_str(), itrMsgId->first);
        }
        else
        {
            sprintf_s(chString, MAX_PATH, "%s [0x%02X]", itrMsgId->second.c_str(), itrMsgId->first);
        }

        m_omComboMsgIDorName.AddString(chString);
        itrMsgId++;
    }
    vAdjustWidthMessageComboBox();

    //return S_OK;
    //TODO:: Adding Config Frames Is pending

    vChangeDelButtonStatus();
    return S_OK;
}

void CTransmitMsgLIN::vUpdateMessageList()
{
    CTxLINDataStore& ouLinData = CTxLINDataStore::ouGetTxLINDataStoreObj();
    std::list<LIN_FRAME_DATA>::iterator itrLinData = ouLinData.m_ouLIN_Frame_Data.begin();
    char chText[MAX_PATH] = {0};
    FRAME_STRUCT ouFrame;

    m_lstMsg.DeleteAllItems();
    ouLinData.nDeleteAllKeyEntrs();
    m_bFromEvent = true;

    while ( itrLinData != ouLinData.m_ouLIN_Frame_Data.end())
    {
        if ( CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouClusterConfig != nullptr )
        {
            if ( S_OK == ouLinData.m_ouClusterConfig->m_ouFlexChannelConfig[itrLinData->m_ouLinMessage.m_ucChannel-1].GetFrame(itrLinData->m_ouLinMessage.m_ucMsgID, ouFrame) )
            {
                itrLinData->m_strFrameName = ouFrame.m_strFrameName;
            }
            else
            {
                vGetStringFromValue(itrLinData->m_ouLinMessage.m_ucMsgID, itrLinData->m_strFrameName);
            }
        }
        nAddMessageToList(*itrLinData);
        itrLinData++;
    }

    m_bFromEvent = false;
}

int CTransmitMsgLIN::nShowListOfCurrentSignls()
{
    BOOL bDataValid = FALSE;
    // Validate the data
    LIN_FRAME_DATA ouLinFrame;
    bDataValid = bValidateData(ouLinFrame);
    if(bDataValid == TRUE )
    {
        m_odSignalMatrix.vSetMessageLength(ouLinFrame.m_ouLinMessage.m_ucDataLen);
        m_odSignalMatrix.vSetByteValue(ouLinFrame.m_ouLinMessage.m_ucData);
        ouLinFrame.m_ouLinMessage.m_ucChksum = 0;
        ouLinFrame.m_ouLinMessage.m_ucChksumTyp = 0;
        ouLinFrame.m_ouLinMessage.m_ucMsgTyp = m_CurrentMsgType;
        nGetMessageName(ouLinFrame.m_strFrameName);
        bUpdateSignalList(ouLinFrame);
    }
    return S_OK;
}
void CTransmitMsgLIN::OnEditchangeCombMsgIdName()
{
    BOOL bIDValid = FALSE;
    INT nMsgID = -1;
    // Get the message ID.
    nMsgID = nGetMessageID();
    // If it is valid
    if(nMsgID != -1 )
    {
        UpdateData(TRUE);
        // Check for valid message ID
        if( nMsgID >= 0 && nMsgID <= 63 )
        {
            bIDValid = TRUE;
            m_omButtonAddMsg.EnableWindow(TRUE);
        }
    }
    if( bIDValid == TRUE )
    {
        // Update Message List Information
        LIN_FRAME_DATA ouLinData;
        // Reset the status text
        bSetStatusText( "" );
        nShowListOfCurrentSignls();
        vUpdateViews(TRUE);

        int nselIndex = m_lstMsg.GetNextItem(-1, LVIS_SELECTED);

        if(nselIndex >= 0)
        {
            CString strMsgId;
            m_omComboMsgIDorName.GetWindowText(strMsgId);
            BOOL bHex = TRUE;
            CHAR* pcStr = nullptr;
            bHex =   CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode;
            UINT nMsgId;
            if(bHex == true )
            {
                nMsgId = strtol(strMsgId,&pcStr,16);
            }
            else
            {
                nMsgId = strtol(strMsgId,&pcStr,10);
            }

            std::string omMsgName, omMsgId;
            vGetStringFromValue(nMsgId, omMsgId);
            FRAME_STRUCT ouFrame;

            m_lstMsg.SetItemText(nselIndex, def_COLUMN_MSGID_INDEX, omMsgId.c_str());
            std::list<LIN_FRAME_DATA>::iterator itrLinData = CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouLIN_Frame_Data.begin();
            advance(itrLinData, nselIndex);

            if ( S_OK==nGetDbFrame(ouFrame) )
            {
                omMsgName = ouFrame.m_strFrameName;
                m_lstMsg.SetItemText(nselIndex, def_COLUMN_MSGNAME_INDEX, omMsgName.c_str());
                if(itrLinData != CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouLIN_Frame_Data.end())
                {
                    itrLinData->m_ouLinMessage.m_ucMsgID = nMsgID;
                    itrLinData->m_strFrameName = omMsgName;
                }
            }
            else
            {
                m_lstMsg.SetItemText(nselIndex, def_COLUMN_MSGNAME_INDEX, omMsgId.c_str());
                if(itrLinData != CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouLIN_Frame_Data.end())
                {
                    itrLinData->m_ouLinMessage.m_ucMsgID = nMsgID;
                    itrLinData->m_strFrameName = nMsgID;
                }
            }
        }
    }
    else
    {
        bSetStatusText(defSTR_INVALID_MESSAGE_ID);
        m_omButtonAddMsg.EnableWindow(FALSE);
        m_odSignalMatrix.vSetMessageLength(0);
        m_omLctrSigList.DeleteAllItems();
    }

}


/*******************************************************************************
 Function Name    : bSetStatusText
 Input(s)         : pStrText - Pointer to the Display Text
 Output           : bool - Success or Failure
 Functionality    : This function is will update the status bar in this dialog.
                    This is used to give an indication to the user about invalid
                    inputs entered if any.
 Member of        : CTxMsgDetailsView
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 19.07.2004
 Modification By  :
 Modification on  :
*******************************************************************************/
BOOL CTransmitMsgLIN::bSetStatusText(const char* pStrText)
{
    BOOL bSuccess = FALSE;
    if( pStrText != nullptr )
    {
        CWnd* pWnd = GetDlgItem(IDC_STAT_STATUS_LIN);
        if( pWnd != nullptr )
        {
            pWnd->SetWindowText(pStrText);
            bSuccess = TRUE;
        }
    }
    return bSuccess;
}

void CTransmitMsgLIN::onComboChnlSel()
{
    int nChannel = m_wndComboChannel.GetCurSel();
    if ( nChannel < 0 || nChannel > CHANNEL_ALLOWED )
    {
        return;
    }

    int nSelIndex = m_lstMsg.GetNextItem(-1, LVIS_SELECTED);
    if(nSelIndex >= 0)
    {
        vUpdateSelMsgDetails(eChannel, nSelIndex, -1, nChannel);
    }
}

void CTransmitMsgLIN::OnSelchangeCombMsgIdName()
{
    vUpdateViews(TRUE);
}

void CTransmitMsgLIN::vUpdateViews(BOOL bIsUpdate)
{
    CString omStrMsgName = "";

    int nChannel = m_wndComboChannel.GetCurSel();
    if ( nChannel < 0 || nChannel > CHANNEL_ALLOWED )
    {
        return;
    }

    int nSelectedIndex = m_omComboMsgIDorName.GetCurSel();
    if( nSelectedIndex != -1 )
    {
        m_omComboMsgIDorName.GetLBText( nSelectedIndex ,omStrMsgName );
        m_omComboMsgIDorName.SetWindowText( omStrMsgName );
    }

    INT nMsgID = nGetMessageID();
    bool bValidMsgID = true;
    if(nMsgID != -1)
    {
        //INT nMsgID = unGetMsgIDFromName(omStrMsgName);
        ClusterConfig* pTemConfig = CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouClusterConfig;
        if ( pTemConfig != nullptr )
        {
            FRAME_STRUCT ouFrameStruct;
            CString omStrMsgName = "";
            HRESULT hr = pTemConfig->m_ouFlexChannelConfig[nChannel].GetFrame(nMsgID, 0, UNSPECIFIED, ouFrameStruct);
            if ( hr != S_OK )
            {
                bValidMsgID = false;
            }
            else
            {
                //m_ouCurrentMsg.m_ouFlexFrame = ouFrameStruct;
                omStrMsgName.Format("%d", ouFrameStruct.m_nLength);
                m_odDLC.SetWindowText( omStrMsgName);
                bSetStatusText( "" );
                vUpdateStateDataBytes();
                nShowListOfCurrentSignls();

                /*list<SIGNAL_STRUCT> lstSig;
                ouFrameStruct.GetSignalList(lstSig);*/
                memset(m_unData, 0, 8);
                vInitialiseDataBytes();
                UINT unSigCnt = m_omLctrSigList.GetItemCount();
                for(INT nIndex = 0; nIndex < unSigCnt; nIndex++)
                {
                    vUpdateFromRawValue(nIndex, def_RAW_VALUE_COLUMN);
                }

                // Update existing message
                if(bIsUpdate == TRUE)
                {
                    nUpdateSelectedMessage();
                }
                m_omButtonAddMsg.EnableWindow(TRUE);
            }
        }
    }

}

int CTransmitMsgLIN::nUpdateSelectedMessage()
{
    int nSel = m_lstMsg.GetNextItem(-1, LVIS_SELECTED );
    if ( nSel != -1 )
    {
        LIN_FRAME_DATA ouLinData;
        if ( TRUE == bValidateData(ouLinData) )
        {
            EnterCriticalSection(&CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouCSMsgList);
            std::list<LIN_FRAME_DATA>::iterator itrLinData = CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouLIN_Frame_Data.begin();
            advance(itrLinData, nSel);
            if ( itrLinData !=  CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouLIN_Frame_Data.end() )
            {
                itrLinData->m_ouLinMessage.m_ucChannel = ouLinData.m_ouLinMessage.m_ucChannel;
                memcpy(itrLinData->m_ouLinMessage.m_ucData, ouLinData.m_ouLinMessage.m_ucData, 8);
                itrLinData->m_ouLinMessage.m_ucDataLen = ouLinData.m_ouLinMessage.m_ucDataLen;
                itrLinData->m_ouLinMessage.m_ucMsgID = ouLinData.m_ouLinMessage.m_ucMsgID;
                // Confirm
                itrLinData->m_ouLinMessage.m_ucMsgTyp = ouLinData.m_ouLinMessage.m_ucMsgTyp;
            }
            LeaveCriticalSection(&CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouCSMsgList);
            nUpdateSelectedMessageInList(ouLinData);
        }
    }
    return 0;
}

int CTransmitMsgLIN::nUpdateSelectedMessageInList(LIN_FRAME_DATA& ouLinData)
{
    int nMsgCode = nGetMessageID();

    // Check if it is valid.
    if( nMsgCode < 0 || nMsgCode > defMAX_LMT_LIN_MSG_ID )
    {
        nMsgCode = -1;
        return 0;
    }

    FRAME_STRUCT ouFrame;
    if ( S_OK==nGetDbFrame(ouFrame) )
    {
        ouLinData.m_strFrameName = ouFrame.m_strFrameName;
    }
    else
    {
        vGetStringFromValue(nMsgCode, ouLinData.m_strFrameName);
    }
    int nSel = m_lstMsg.GetNextItem(-1, LVIS_SELECTED );
    if ( nSel >= 0 )
    {
        vUpdateMsgInList(ouLinData);
    }

    return 0;
}

UINT CTransmitMsgLIN::unGetMsgIDFromName(CString omMsgName)
{
    CString omStrMsgID;
    UINT unMsgID = (UINT)-1;
    CHAR* pcStopStr = nullptr;
    std::string strStartKey;
    if ( CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode == false)
    {
        strStartKey = "[";
    }
    else
    {
        strStartKey = "[0x";
    }

    int nIndex1 = omMsgName.Find(strStartKey.c_str());
    if(nIndex1 != -1)
    {
        int nIndex2 = omMsgName.ReverseFind(']');
        if(nIndex1 != -1 && nIndex2 > nIndex1 )
        {
            int nLength = omMsgName.GetLength();
            omStrMsgID = omMsgName.Mid(nIndex1+1,nIndex2-nIndex1);
            if ( CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode == false)
            {
                unMsgID = strtol((LPCTSTR )omStrMsgID,&pcStopStr,defBASE_DEC);
            }
            else
            {
                unMsgID = strtol((LPCTSTR )omStrMsgID,&pcStopStr,defBASE_HEX);
            }
        }
    }
    return unMsgID;
}


BOOL CTransmitMsgLIN::bCheckIfValueIsMoreThan255(USHORT usNoOfEditCtrlsToCheck, UCHAR ucData[8])
{
    // Assume failure
    BOOL bResult = FALSE;
    // We have to check if the data byte value
    // in "Decimal" mode exceeds more than 255
    UINT unIDValue = IDC_EDIT_DB1;
    USHORT usTempCount = 0;

    CString omStrValue("");
    UINT unValue;
    CRadixEdit* pRadixEdit = nullptr;

    while ( ( usTempCount < usNoOfEditCtrlsToCheck ) &&
            ( bResult == FALSE ))
    {
        // Get value
        pRadixEdit = (CRadixEdit*)GetDlgItem(unIDValue);
        if ( pRadixEdit != nullptr)
        {
            unValue = static_cast<UINT> (pRadixEdit->lGetValue());
            if ( unValue > 255 )
            {
                CString omErrorStr;
                omErrorStr.Format( _(defSTR_INVALID_DATA_BYTES),
                                   usTempCount + 1 );
                bSetStatusText( omErrorStr );
                bResult = TRUE;// Exit loop
            }
            else
            {
                if (usTempCount < 8 )
                {
                    ucData[usTempCount] = (UCHAR)unValue;
                }
                // Next control
                unIDValue++;// ID
                usTempCount++;// Count
            }
        }
    }
    return bResult;
}

int CTransmitMsgLIN::nGetDbFrame(FRAME_STRUCT& ouFrame)
{
    int nChannel = m_wndComboChannel.GetCurSel();
    if ( nChannel >= 0&& nChannel < CHANNEL_ALLOWED )
    {
        // Get the message ID.
        int nMsgCode = nGetMessageID();
        ClusterConfig* pTempConfig = CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouClusterConfig;
        if ( pTempConfig != nullptr )
        {
            return pTempConfig->m_ouFlexChannelConfig[nChannel].GetFrame(nMsgCode, 0, UNSPECIFIED, ouFrame);
        }
    }

    return 0;
}

int CTransmitMsgLIN::nGetMessageName(std::string& strMsgName)
{
    int nChannel = m_wndComboChannel.GetCurSel();
    if ( nChannel >= 0&& nChannel < CHANNEL_ALLOWED )
    {
        // Get the message ID.
        int nMsgCode = nGetMessageID();
        ClusterConfig* pTempConfig = CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouClusterConfig;
        if ( pTempConfig != nullptr )
        {
            FRAME_STRUCT ouFrame;
            if ( S_OK==nGetDbFrame(ouFrame) )
            {
                strMsgName = ouFrame.m_strFrameName;
            }
            else
            {
                vGetStringFromValue(nMsgCode, strMsgName);
            }
        }
    }
    return S_OK;
}

BOOL CTransmitMsgLIN::bUpdateSignalList(LIN_FRAME_DATA& sMsg)
{
    std::list<SIGNAL_STRUCT> lstSigStrct;
    vGetSelSignalFromFrame(sMsg.m_ouLinMessage.m_ucMsgID, lstSigStrct);

    std::list<SIGNAL_STRUCT>::iterator itrSig = lstSigStrct.begin();
    int i = 0;
    m_omLctrSigList.DeleteAllItems();

    SNUMERICINFO sNumInfo;

    sNumInfo.m_byFlag = BUDDY_CTRL;
    // Physical value should be in dec always
    sNumInfo.m_byBase = defBASE_DEC;
    //sNumInfo.m_uDelta =

    for (; itrSig != lstSigStrct.end(); itrSig++ )
    {
        m_omLctrSigList.InsertItem(i, itrSig->m_strSignalName.c_str());


        DWORD dwLowerLimit = 0;
        DWORD dwUpperLimit = 0;

        if(itrSig->m_ouCompuMethod.m_eCompuType == LINEAR_ENUM)
        {
            dwUpperLimit = itrSig->m_ouCompuMethod.m_uMethod.m_LinearCode.m_sRange.m_dwUpperLimit;
            dwLowerLimit = itrSig->m_ouCompuMethod.m_uMethod.m_LinearCode.m_sRange.m_dwLowerLimit;
            sNumInfo.m_uDelta.m_un64Value = itrSig->m_ouCompuMethod.m_uMethod.m_LinearCode.m_dN1;
        }
        else
        {
            dwUpperLimit = itrSig->m_ouCompuMethod.m_uMethod.m_IdenticalCode.m_sRange.m_dwUpperLimit;
            dwLowerLimit = itrSig->m_ouCompuMethod.m_uMethod.m_IdenticalCode.m_sRange.m_dwLowerLimit;
            sNumInfo.m_uDelta.m_un64Value = 1;
        }

        DWORD dwDefVal = itrSig->m_unDefaultVal;
        CString omLLEnggValue = "", omULEnggValue = "", omDefEnggValue = "";

        // Raw to physical
        itrSig->m_ouCompuMethod.omGetEnggValue(dwLowerLimit, omLLEnggValue);
        itrSig->m_ouCompuMethod.omGetEnggValue(dwUpperLimit, omULEnggValue);
        itrSig->m_ouCompuMethod.omGetEnggValue(dwDefVal, omDefEnggValue);

        char chValue[1024];
        sprintf(chValue, "%d", dwDefVal);
        m_omLctrSigList.SetItemText(i, 1, chValue);
        m_omLctrSigList.SetItemText(i, 2, omDefEnggValue);
        m_omLctrSigList.SetItemText(i, 3, itrSig->m_omUnit.c_str());

        SLISTINFO sListInfo;
        sListInfo.m_eType = eNoControl;
        m_omLctrSigList.vSetColumnInfo(i, 0, sListInfo);

        // Raw value
        sNumInfo.m_uMinVal.m_un64Value = dwLowerLimit;
        sNumInfo.m_uMaxVal.m_un64Value = dwUpperLimit;

        if( false ==  CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode)
        {
            sNumInfo.m_byBase = defBASE_DEC;
        }
        else
        {
            sNumInfo.m_byBase = defBASE_HEX;
        }

        sListInfo.m_eType = eNumber;
        sNumInfo.m_byFlag = BUDDY_CTRL;
        sNumInfo.m_nSigLength = itrSig->m_nLength;
        sNumInfo.m_uDelta.m_un64Value = 1;
        m_omLctrSigList.vSetColumnInfo(i, 1, sListInfo);
        m_omLctrSigList.vSetNumericInfo(i, 1, sNumInfo);

        // Physical value

        if(itrSig->m_ouCompuMethod.m_eCompuType == LINEAR_ENUM)
        {
            // Signal Factor
            sNumInfo.m_byFlag |= FLOAT_VAL;
            sNumInfo.m_uDelta.m_dValue = (itrSig->m_ouCompuMethod.m_uMethod.m_LinearCode.m_dN1);
            sNumInfo.m_uMinVal.m_dValue = atof(omLLEnggValue);
            sNumInfo.m_uMaxVal.m_dValue = atof(omULEnggValue);
            //sNumInfo.m_uDelta.m_dValue = itrSig->m_ouCompuMethod.m_uMethod.m_LinearCode.m_dN1;
        }
        else
        {
            sNumInfo.m_byFlag |= FLOAT_VAL;
            sNumInfo.m_uMinVal.m_dValue = atof(omLLEnggValue);
            sNumInfo.m_uMaxVal.m_dValue = atof(omULEnggValue);
            sNumInfo.m_uDelta.m_dValue = 1;
        }

        sNumInfo.m_nSigLength = itrSig->m_nLength;

        sNumInfo.m_byBase = defBASE_DEC;

        sListInfo.m_eType = eNumber;
        //sNumInfo.m_byFlag = BUDDY_CTRL;
        m_omLctrSigList.vSetColumnInfo(i, 2, sListInfo);
        m_omLctrSigList.vSetNumericInfo(i, 2, sNumInfo);

        sListInfo.m_eType = eNoControl;
        m_omLctrSigList.vSetColumnInfo(i, 3, sListInfo);

        i++;
    }
    return S_OK;
}
BOOL CTransmitMsgLIN::bValidateData(LIN_FRAME_DATA& ouFrameData)
{
    // Assume Failure
    BOOL bIsValid = FALSE;
    // Selected Message Code
    int nMsgCode = -1;
    // User has to input atleast msg code and dlc
    CString omStr = "";
    UpdateData(TRUE);

    int nChannel = m_wndComboChannel.GetCurSel();
    if ( nChannel >= 0&& nChannel < CHANNEL_ALLOWED )
    {
        ouFrameData.m_ouLinMessage.m_ucChannel = nChannel+1;
    }

    // Update Message type
    ouFrameData.m_ouLinMessage.m_ucMsgTyp = eGetMessageType();

    // Get the message ID.
    nMsgCode = nGetMessageID();

    // Check if it is valid.
    if( nMsgCode < 0 || nMsgCode > defMAX_LMT_LIN_MSG_ID )
    {
        nMsgCode = -1;
    }

    if ( nMsgCode != -1 )
    {
        // Selected DLC Value
        ouFrameData.m_ouLinMessage.m_ucMsgID = nMsgCode;
        USHORT usDLC = 0;

        m_odDLC.GetWindowText(omStr);
        if ( omStr.IsEmpty() != TRUE )
        {
            usDLC = (USHORT)m_odDLC.lGetValue();
            if(usDLC >= 0  && usDLC <=8)
            {
                ouFrameData.m_ouLinMessage.m_ucDataLen = usDLC;
                bIsValid = TRUE;
            }
        }
        if(bIsValid == TRUE )
        {
            memset(ouFrameData.m_ouLinMessage.m_ucData, 0, 8);
            bIsValid = bCheckIfValueIsMoreThan255(usDLC, ouFrameData.m_ouLinMessage.m_ucData);
            // The bCheckIfValueIsMoreThan255 function returns TRUE if
            // value is invalid
            bIsValid = !bIsValid;
        }
    }
    return bIsValid;
}

INT CTransmitMsgLIN::nGetMessageID()
{
    CString omStrMsgName( "" );
    INT nMsgID = -1;
    m_omComboMsgIDorName.GetWindowText(omStrMsgName);
    // Get message Id from database in case user has selected a message name.
    //if it is name [] is present
    int nIndex = omStrMsgName.Find(defMSGID_NAME_START_CHAR);
    if(nIndex != -1)
    {
        nMsgID = unGetMsgIDFromName(omStrMsgName);
    }
    // If the message name is not selected then get it from edit box and convert
    // it to the integer value.
    else
    {
        UINT unMsgID = 0;
        CHAR* pcStr = nullptr;
        BOOL bHex = TRUE;

        bHex =   CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode;
        if(bHex == true )
        {
            unMsgID = strtol(omStrMsgName,&pcStr,16);
        }
        else
        {
            unMsgID = strtol(omStrMsgName,&pcStr,10);
        }
        // check if the field is empty or user have entered "0".
        // so in case user has entered "0" or if non zero value is
        // entered and strtol function has writtened correct value.
        if(omStrMsgName.CompareNoCase("0") != 0 && unMsgID != 0 || omStrMsgName.CompareNoCase("0") == 0 )
        {
            nMsgID = static_cast<INT> (unMsgID);
        }
    }
    vUpdateStateDataBytes();

    return nMsgID;
}

void CTransmitMsgLIN::vUpdateStateDataBytes()
{
    if ( ((CButton*)GetDlgItem(IDC_LIN_MSGTYPE_SLAVE))->GetCheck() == TRUE )
    {
        unsigned int dlc = (unsigned int) m_odDLC.lGetValue();

        for(INT nIndex = 0; nIndex < defMAX_BYTE; nIndex++)
        {
            m_odDB[nIndex].SetReadOnly( dlc < (nIndex+1));
        }
    }
    else
    {
        for(INT nIndex = 0; nIndex < defMAX_BYTE; nIndex++)
        {
            m_odDB[nIndex].SetReadOnly( true );
        }
    }


    /*m_odDB1.SetReadOnly(dlc < 1);
    m_odDB2.SetReadOnly(dlc < 2);
    m_odDB3.SetReadOnly(dlc < 3);
    m_odDB4.SetReadOnly(dlc < 4);
    m_odDB5.SetReadOnly(dlc < 5);
    m_odDB6.SetReadOnly(dlc < 6);
    m_odDB7.SetReadOnly(dlc < 7);
    m_odDB8.SetReadOnly(dlc < 8);*/
}

void CTransmitMsgLIN::vSetDefaultValues()
{
    int nConfigChannel = CTxLINDataStore::ouGetTxLINDataStoreObj().m_nChannelsConfigured;
    if ( nConfigChannel > 0 )
    {
        // Set Message ID/Name
        m_omComboMsgIDorName.SetWindowText("");
        // Set Databytes
        CString omStrDataBytesValue;
        // Two digits in case of Hex mode
        // Three digits in case of Dec mode
        BOOL bIsHex = CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode;
        if( bIsHex )
        {
            omStrDataBytesValue.Format(defFORMAT_DATA_HEX,0);
        }
        else
        {
            omStrDataBytesValue.Format(defFORMAT_DATA_DECIMAL,0);
        }

        for(INT nIndex = 0; nIndex < defMAX_BYTE; nIndex++)
        {
            m_odDB[nIndex].SetWindowText( omStrDataBytesValue );
        }

        // Set DLC
        m_odDLC.SetReadOnly(false);
        m_odDLC.vSetValue(8);
        // Set the selection to Channel 1
        m_omComboChannelID.SetCurSel(0);

    }
    else
    {
        m_odDLC.SetReadOnly(true);
        m_odDLC.vSetValue(0);
    }
    vUpdateStateDataBytes();
}

//void CTransmitMsgLIN::vInitSignalListCtrl()
//{
//  CHAR caColumnName[][defSIGNAL_LIST_STRING_MAX_LENGTH]
//  = { defSTR_COL_SIGNAL_NAME,
//      defSTR_RAW_COLUMN,
//      defSTR_PHYSICAL_COLUMN,
//      defSTR_SIGNAL_UNIT
//  };
//  INT nColumnFormat[]     = { LVCFMT_LEFT,
//      LVCFMT_CENTER,
//      LVCFMT_CENTER,
//      LVCFMT_LEFT
//  };
//  RECT rListCtrlRect;
//  INT nTotalColunmSize = 0;
//  INT nTotalStrLengthPixel = 0;
//  INT nColumnSize = 0;
//  // Set the Image List
//
//  //Calculate the total size of all column header
//  m_omLctrSigList.GetWindowRect( &rListCtrlRect);
//  nTotalColunmSize     = rListCtrlRect.right - rListCtrlRect.left;
//  nTotalStrLengthPixel = 0;
//
//  UINT i; //i declared outside loop
//  for(i=0; i<4; i++)
//  {
//      nTotalStrLengthPixel +=
//          m_omLctrSigList.GetStringWidth(_(caColumnName[i])) ;
//  }
//  //Insert each column name after calculating the size for the same.
//  for(i=0; i<4; i++)
//  {
//      nColumnSize  = m_omLctrSigList.GetStringWidth(_(caColumnName[i])) ;
//      nColumnSize +=
//          (nTotalColunmSize-nTotalStrLengthPixel)/defSIGNAL_FRAME_COLUMN;
//      m_omLctrSigList.InsertColumn(i, _(caColumnName[i]),
//          nColumnFormat[i], nColumnSize - 1);
//  }
//
//  // Set the extended style
//  m_omLctrSigList.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
//  SNUMERICINFO    sNumInfo;
//  SLISTINFO       sListInfo;
//  SUSERPROGINFO   sProgInfo;
//
//  // Init Signal Name column
//  // Column 0 : User Function call
//  sListInfo.m_eType = eUser;
//  m_omLctrSigList.vSetColumnInfo(0, 0, sListInfo);
//  sProgInfo.m_pfHandler = vCheckBoxClickHandler;
//  sProgInfo.m_pUserParam = this;
//  m_omLctrSigList.vSetUserProgInfo(0, 0, sProgInfo);
//
//  // Init Raw Value column
//  // Column 1 : Numeric Edit with Spin Control
//  sListInfo.m_eType = eBuddy;
//  m_omLctrSigList.vSetColumnInfo(0, 1, sListInfo);
//
//  // Init Phy Value column
//  // Column 2 : Numeric Edit with Spin Control
//  sListInfo.m_eType = eBuddy;
//  m_omLctrSigList.vSetColumnInfo(0, 2, sListInfo);
//
//  // Init Unit column
//  // Column 3 : Numeric Edit with Spin Control
//  // Init is not required as it non-editable column
//
//
//  // Disable Signal List as there is no selected message at this time
//  m_omLctrSigList.EnableWindow( TRUE );
//  // Signal Matrix is not created at this point of time
//  // Disable that after create
//}

void CTransmitMsgLIN::vCheckBoxClickHandler( CListCtrl* pThis, int nItem, int nSubItem, void* uParam)
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

    ((CTransmitMsgLIN*)uParam)->vUpdateSelMsgDetails(eUpdate, nItem, -1, -1, bEnable == true);

    ((CListCtrl*)pThis)->SetItem(&lvitem);
}

INT CTransmitMsgLIN::GetSelectedSignalStruct( SIGNAL_STRUCT& ouSignal, int nIndex )
{
    /*list<SIGNAL_STRUCT> ouSignalList;
    m_ouCurrentMsg.GetSignalList(ouSignalList);*/

    INT nMsgId = nGetMessageID();

    std::list<SIGNAL_STRUCT> ouSignalList;
    vGetSelSignalFromFrame(nMsgId, ouSignalList);
    CString omStrSigName =
        m_omLctrSigList.GetItemText(nIndex, 0);
    for ( std::list<SIGNAL_STRUCT>::iterator itr = ouSignalList.begin(); itr != ouSignalList.end(); itr++ )
    {
        if ( omStrSigName == itr->m_strSignalName.c_str() )
        {
            ouSignal = *itr;
            return S_OK;
        }
    }
    return S_FALSE;
}

void CTransmitMsgLIN::vUpdateSignalMatrix(int nSelectedIndex, FRAME_STRUCT ouFrmData)
{
    SIGNAL_STRUCT psSignal;
    // Get the signal pointer
    HRESULT hr = GetSelectedSignalStruct(psSignal, nSelectedIndex);

    // If it is a valid signal
    if( hr == S_OK )
    {
        // Calculate the Signal mask
        BYTE abySigMask[8] = {0};
        int nByte = psSignal.m_unStartbit / 8 +1;
        int nSignal = psSignal.m_unStartbit % 8;
        bCalcBitMaskForSig( abySigMask,
                            8,
                            nByte,
                            nSignal,
                            psSignal.m_nLength,
                            (EFORMAT_DATA)psSignal.m_ouEndianness);
        // Update the matrix
        UINT nMsgLen = 0;
        CString strMsgId;
        BOOL bHex = TRUE;
        CHAR* pcStr = nullptr;
        bHex =   CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode;
        UINT nMsgId;
        if(bHex == true )
        {
            nMsgId = strtol(strMsgId,&pcStr,16);
        }
        else
        {
            nMsgId = strtol(strMsgId,&pcStr,10);
        }

        if (nMsgId != -1)
        {
            nMsgLen = m_odDLC.lGetValue();
        }
        m_odSignalMatrix.vSetMessageLength(nMsgLen);
        m_odSignalMatrix.vSetHighlight(abySigMask, nMsgLen);
    }
}
void CTransmitMsgLIN::OnItemchangedLstcSigDetails( NMHDR* pNMHDR, LRESULT* pResult )
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // Update Matrix while selection got changed or updating
    // through program
    if(pNMListView->uChanged != LVIF_TEXT )
    {
        // Selected & Focused
        if( pNMListView->uNewState == (LVIS_FOCUSED | LVIS_SELECTED) )
        {
            FRAME_STRUCT ouFrame;
            if ( S_OK == CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouClusterConfig->m_ouFlexChannelConfig[nGetChannel()-1].GetFrame(nGetMessageID(), ouFrame) )
            {
                vUpdateSignalMatrix(pNMListView->iItem, ouFrame);
            }
        }
    }

    // Notification for text change
    if( pNMListView->uChanged == LVIF_TEXT && m_bUpdating == FALSE)
    {
        LV_DISPINFO ls;
        // Get the change information from the list control
        if(m_omLctrSigList.sGetModificationStructure(ls) == TRUE)
        {
            INT nSel = m_lstMsg.GetNextItem(-1, LVIS_SELECTED);
            switch( ls.item.iSubItem)
            {
                    // Raw Value Change
                case 1:
                    vUpdateFromRawValue( ls.item.iItem,
                                         ls.item.iSubItem );
                    if( nSel >= 0 )
                    {
                        vUpdateSelMsgDetails(eDataBytes, nSel);
                    }
                    break;
                    // Physical Value Change
                case 2:
                    vUpdateFromPhysicalValue( ls.item.iItem,
                                              ls.item.iSubItem );
                    if( nSel >= 0 )
                    {
                        vUpdateSelMsgDetails(eDataBytes, nSel);
                    }
                    break;
            }
        }
        //vCallApplyChanges();
    }


    *pResult = 0;
}


void CTransmitMsgLIN::vUpdateSignalMatrix(void)
{
    // Set the bit matrix
    for ( int index = 0; index < defMAX_BYTE; index++)
    {
        int nBit = index * defBITS_IN_BYTE;
        m_bData[ nBit ] = m_unData[ index ].sByte.Bit0;
        m_bData[ nBit + 1] = m_unData[ index ].sByte.Bit1;
        m_bData[ nBit + 2] = m_unData[ index ].sByte.Bit2;
        m_bData[ nBit + 3] = m_unData[ index ].sByte.Bit3;

        m_bData[ nBit + 4] = m_unData[ index ].sByte.Bit4;
        m_bData[ nBit + 5] = m_unData[ index ].sByte.Bit5;
        m_bData[ nBit + 6] = m_unData[ index ].sByte.Bit6;
        m_bData[ nBit + 7] = m_unData[ index ].sByte.Bit7;

    }

    //m_odSignalMatrix.vSetMessageLength(m_odDLC.lGetValue());

    // Set the data value
    m_odSignalMatrix.vSetValue( m_bData );

    // Update Signal Matrix List
    m_odSignalMatrix.Invalidate();
}


/*******************************************************************************
 Function Name    : vUpdateFromRawValue
 Input(s)         : nItem       - Item Index
                    nSubItem    - SubItem Index
 Output           :  -
 Functionality    : This function will validate and update the raw value
                    after edit. This will update Signal Matrix, data bytes. This
                    will finally update Global Tx block data.
 Member of        : CTxMsgDetailsView
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 19.07.2004
 Modification By  : Raja N
 Modification on  : 02.08.2004, Modified nConvertStringToInt with
                   bConvertStringToInt64 global utility
                    function call
*******************************************************************************/
void CTransmitMsgLIN::vUpdateFromRawValue(int nItem, int nSubItem)
{
    CString omStrSignalName;
    BOOL bDataInvalid = FALSE;

    //ouSigStrct = psGetSelectedSignalStruct(nItem);

    UINT unMsgId = nGetMessageID();

    std::list<SIGNAL_STRUCT> lstSigStrct;
    vGetSelSignalFromFrame(unMsgId, lstSigStrct);

    CString omStrSigName = m_omLctrSigList.GetItemText(nItem, 0);

    SIGNAL_STRUCT ouSigStrct;

    if(nItem >= 0)
    {
        psGetSelectedSignalStruct(lstSigStrct, omStrSigName, ouSigStrct);
    }

    // If it is a valid signal
    if( ouSigStrct.m_strSignalName != "" )
    {
        // Get the Signal Raw Value
        CString omStr = m_omLctrSigList.GetItemText( nItem, nSubItem );
        CString omstrDefault;
        __int64 nI64SignVal;
        __int64 n64Min, n64Max;
        // Set the base value
        int nBase = CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode ? defBASE_HEX : defBASE_DEC;

        // Get the int 64 val from the string
        bConvertStringToInt64( omStr, nI64SignVal, nBase );

        if(ouSigStrct.m_ouCompuMethod.m_eCompuType == LINEAR_ENUM)
        {
            n64Min = ouSigStrct.m_ouCompuMethod.m_uMethod.m_LinearCode.m_sRange.m_dwLowerLimit;
            n64Max = ouSigStrct.m_ouCompuMethod.m_uMethod.m_LinearCode.m_sRange.m_dwUpperLimit;
        }
        else
        {
            n64Min = ouSigStrct.m_ouCompuMethod.m_uMethod.m_IdenticalCode.m_sRange.m_dwLowerLimit;
            n64Max = ouSigStrct.m_ouCompuMethod.m_uMethod.m_IdenticalCode.m_sRange.m_dwUpperLimit;
        }


        if ( (UINT64)nI64SignVal > n64Max )
        {
            nI64SignVal = n64Max;
            bDataInvalid = TRUE;
        }
        else if ( (UINT64)nI64SignVal < n64Min )
        {
            nI64SignVal = n64Min;
            bDataInvalid = TRUE;
        }

        CString strEnggVal = "";
        ouSigStrct.m_ouCompuMethod.omGetEnggValue(nI64SignVal, strEnggVal);

        // Update the data
        UCHAR ucData[defMAX_BYTE] = {0};
        //copy the existing data
        memcpy(ucData, m_unData, sizeof(BYTE) * defMAX_BYTE);
        //update the data
        vSetSignalValue(ouSigStrct, ucData, nI64SignVal);
        //copy the data
        vInitialiseDataBytes();
        memcpy(m_unData, ucData, sizeof(UCHAR) * defMAX_BYTE);
        //commented by kadoor vUpdateSignalData( psSignal, nI64SignVal );
        vUpdateDataBytes();
        BOOL bFound = FALSE;

        CString omstrRawVal;

        if(CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode == true)
        {
            omstrRawVal.Format("%X", nI64SignVal);
        }
        else
        {
            omstrRawVal.Format("%d", nI64SignVal);
        }

        m_omLctrSigList.SetItemText( nItem, def_RAW_VALUE_COLUMN,
                                     omstrRawVal );
        m_omLctrSigList.SetItemText( nItem, def_PHY_VALUE_COLUMN,
                                     strEnggVal );

        // Set the data value
        vUpdateSignalMatrix();
    }
}

void CTransmitMsgLIN::vInitialiseDataBytes()
{
    for(INT nIndex = 0; nIndex < 8; nIndex++)
    {
        m_unData[nIndex].byByte = 0;
    }
}

/*******************************************************************************
 Function Name    : vUpdateFromRawValue
 Input(s)         : nItem       - Item Index
                    nSubItem    - SubItem Index
 Output           :  -
 Functionality    : This function will validate and update the raw value
                    after edit. This will update Signal Matrix, data bytes. This
                    will finally update Global Tx block data.
 Member of        : CTxMsgDetailsView
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 19.07.2004
 Modification By  : Raja N
 Modification on  : 02.08.2004, Modified nConvertStringToInt with
                   bConvertStringToInt64 global utility
                    function call
*******************************************************************************/
void CTransmitMsgLIN::vUpdateFromPhysicalValue(int nItem, int nSubItem)
{
    CString omStrSignalName;
    BOOL bDataInvalid = FALSE;

    //ouSigStrct = psGetSelectedSignalStruct(nItem);

    UINT unMsgId = nGetMessageID();

    std::list<SIGNAL_STRUCT> lstSigStrct;
    vGetSelSignalFromFrame(unMsgId, lstSigStrct);

    CString omStrSigName = m_omLctrSigList.GetItemText(nItem, 0);

    SIGNAL_STRUCT ouSigStrct;

    if(nItem >= 0)
    {
        psGetSelectedSignalStruct(lstSigStrct, omStrSigName, ouSigStrct);
    }

    // If it is a valid signal
    if( ouSigStrct.m_strSignalName != "" )
    {
        // Get the Signal Raw Value
        CString omStr = m_omLctrSigList.GetItemText( nItem, nSubItem );

        __int64 n64Min, n64Max;
        // Set the base value
        double fPhyValue = atof(omStr);

        CString strRawVal = "";
        ouSigStrct.m_ouCompuMethod.omGetRawValue(strRawVal, fPhyValue);

        INT nRawVal = atoi(strRawVal);

        if(ouSigStrct.m_ouCompuMethod.m_eCompuType == LINEAR_ENUM)
        {
            n64Min = ouSigStrct.m_ouCompuMethod.m_uMethod.m_LinearCode.m_sRange.m_dwLowerLimit;
            n64Max = ouSigStrct.m_ouCompuMethod.m_uMethod.m_LinearCode.m_sRange.m_dwUpperLimit;
        }
        else
        {
            n64Min = ouSigStrct.m_ouCompuMethod.m_uMethod.m_IdenticalCode.m_sRange.m_dwLowerLimit;
            n64Max = ouSigStrct.m_ouCompuMethod.m_uMethod.m_IdenticalCode.m_sRange.m_dwUpperLimit;
        }

        if ( nRawVal > n64Max )
        {
            nRawVal = n64Max;
            bDataInvalid = TRUE;
        }
        else if ( nRawVal < n64Min )
        {
            nRawVal = n64Min;
            bDataInvalid = TRUE;
        }

        ouSigStrct.m_ouCompuMethod.omGetEnggValue(nRawVal, omStr);


        if( true == CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode)
        {
            strRawVal.Format("%X", nRawVal);
        }
        else
        {
            strRawVal.Format("%d", nRawVal);
        }

        m_omLctrSigList.SetItemText( nItem, def_RAW_VALUE_COLUMN,
                                     strRawVal );
        m_omLctrSigList.SetItemText( nItem, def_PHY_VALUE_COLUMN,
                                     omStr );

        // Update the data
        UCHAR ucData[defMAX_BYTE] = {0};
        //copy the existing data
        memcpy(ucData, m_unData, sizeof(BYTE) * defMAX_BYTE);
        //update the data
        vSetSignalValue(ouSigStrct, ucData, nRawVal);
        //copy the data
        vInitialiseDataBytes();
        memcpy(m_unData, ucData, sizeof(UCHAR) * defMAX_BYTE);
        //commented by kadoor vUpdateSignalData( psSignal, nI64SignVal );
        vUpdateDataBytes();
        // Set the data value
        vUpdateSignalMatrix();
        m_odSignalMatrix.vSetValue( m_bData );

        //SetDirty(FALSE);
        //vUpdateSelectedMessageDetails();
    }
}

/*******************************************************************************
 Function Name    : vUpdateDataBytes
 Input(s)         :  -
 Output           :  -
 Functionality    : This function will be called to update the data bytes with
                    the signal values from the signal list. This will take care
                    of frame format Big/Little endian.
 Member of        : CTxMsgDetailsView
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 19.07.2004
 Modification By  :
 Modification on  :
*******************************************************************************/
void CTransmitMsgLIN::vUpdateDataBytes()
{
    // Display databytes
    UINT unIDValue = IDC_EDIT_DB1;// ID of first databyte edit control
    CEdit* pEdit = nullptr;
    CString omStr, omStrFormatData;

    /*int nSel = m_lstMsg.GetNextItem(-1, LVIS_SELECTED);

    if(nSel >= 0)*/
    {
        //LIN_FRAME_DATA objLinFrame = vGetMsgDetails(nSel);

        if( true == CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode)
        {
            omStrFormatData = defFORMAT_DATA_HEX;
        }
        else
        {
            omStrFormatData = defFORMAT_DATA_DECIMAL;
        }

        for(UINT i =0 ; i< defMAX_BYTE; i++ )
        {
            pEdit = (CEdit*)GetDlgItem(unIDValue + i);
            int index = i;
            if ( pEdit != nullptr  )
            {
                if(i >= m_odDLC.lGetValue())
                {
                    omStr = "";
                }
                else
                {
                    omStr.Format(omStrFormatData ,m_unData[index].byByte);
                }
                pEdit->SetWindowText(omStr);
            }

            vUpdateStateDataBytes();
        }
    }
}

void CTransmitMsgLIN::vUpdateDataBytes(LIN_FRAME_DATA& ouData)
{
    UINT unIDValue = IDC_EDIT_DB1;// ID of first databyte edit control
    CEdit* pEdit = nullptr;
    CString omStr, omStrFormatData;

    if( true == CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode)
    {
        omStrFormatData = defFORMAT_DATA_HEX;
    }
    else
    {
        omStrFormatData = defFORMAT_DATA_DECIMAL;
    }

    for(UINT i =0 ; i< defMAX_BYTE; i++ )
    {
        pEdit = (CEdit*)GetDlgItem(unIDValue + i);
        int index = i;
        if ( pEdit != nullptr  )
        {
            if(i >= ouData.m_ouLinMessage.m_ucDataLen)
            {
                omStr = "";
            }
            else
            {
                omStr.Format(omStrFormatData ,ouData.m_ouLinMessage.m_ucData[index]);
            }
            pEdit->SetWindowText(omStr);
        }
    }
    vUpdateStateDataBytes();
}
UINT64 CTransmitMsgLIN::un64GetBitMask(SIGNAL_STRUCT CurrSig)
{
    UINT64 Result = 0x1;

    // First make the required number of bits (m_unSignalLength) up.
    Result <<= CurrSig.m_nLength;
    --Result; // These bits are now up.

    // Then shift them to the appropriate place.
    short Shift = (INTEL == CurrSig.m_ouEndianness) ?
                  ((short)CurrSig.m_unStartbit)
                  : 64 - CurrSig.m_unStartbit;
    Result <<= Shift;

    if (MOTOROLA == CurrSig.m_ouEndianness)
    {
        BYTE* pbStr = (BYTE*) &Result;

        BYTE bTmp = 0x0;
        bTmp = pbStr[0];
        pbStr[0] = pbStr[7] ;
        pbStr[7] = bTmp;
        bTmp = pbStr[1];
        pbStr[1] = pbStr[6] ;
        pbStr[6] = bTmp;
        bTmp = pbStr[2];
        pbStr[2] = pbStr[5] ;
        pbStr[5] = bTmp;
        bTmp = pbStr[3];
        pbStr[3] = pbStr[4] ;
        pbStr[4] = bTmp;
    }
    return Result;
}

void CTransmitMsgLIN::vSetSignalValue(SIGNAL_STRUCT ouSigStrct, UCHAR aucData[8],
                                      UINT64 u64SignVal)
{
    ASSERT(pouCurrSignal != nullptr);
    /* Signal valuedata type happens to be of the same size of the entire CAN
    data byte array. Hence there is an opportunity to take advantage of this
    idiosyncratic characteristics. We will shifts the bit array in u64SignVal
    by the required number of bit positions to exactly map it as a data byte
    array and then interchange positions of bytes as per the endianness and
    finally use it as the etching mask on the target. */
    UINT64* pu64Target = (UINT64*) aucData; // We should be able to work on
    BYTE* pbData = (BYTE*)&u64SignVal; // these variables as an arrayof
    // bytes andvice versa.
    // First findout offset between the last significant bits of the signal
    // and theframe. Finding out the lsb will directly answer to thisquery.
    UINT64 unMaxVal = pow((double)2, (double)ouSigStrct.m_nLength);
    unMaxVal -= 1;
    u64SignVal = u64SignVal&unMaxVal;
    if(ouSigStrct.m_ouEndianness == INTEL)// If Intel format
    {
        int Offset = ouSigStrct.m_unStartbit;
        u64SignVal<<= Offset;// Exactly map the data bits on the databytes.
    }

    UINT64 unTmp = un64GetBitMask(ouSigStrct);
    *pu64Target&= ~unTmp;// All bits related to the current signal willbe
    // be made0.
    *pu64Target |= u64SignVal;
}


/*******************************************************************************
 Function Name    : vUpdateFromPhysicalValue
 Input(s)         : nItem       - Item Index
                    nSubItem    - SubItem Index
 Output           :  -
 Functionality    : This function will validate and update the physical value
                    after edit. This will update Signal Matrix, data bytes. This
                    will finally update Global Tx block data.
 Member of        : CTxMsgDetailsView
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 19.07.2004
 Modification By  : Raja N
 Modification on  : 02.08.2004, Int 64 problem fix and changed type casting in
                    to C++ version operators
*******************************************************************************/
//void CTransmitMsgLIN::vUpdateFromPhysicalValue(int nItem, int nSubItem)
//{
//    CString omStrSignalName;
//    sSIGNALS* psSignal = nullptr;
//
//    psSignal = psGetSelectedSignalStruct(nItem);
//    // If it is a valid signal
//    if( psSignal != nullptr )
//    {
//        CString omstrDefault;
//        double dSignVal = 0.0;
//        __int64 n64SigVal = 0;
//        UINT64 un64SigVal = 0;
//        // Get the string from the list item
//        CString omStr = m_omLctrSigList.GetItemText( nItem, nSubItem );
//        BOOL bFound = FALSE;
//        BOOL bDataInvalid = FALSE;
//        // Check for Signal Descriptor
//        if( psSignal->m_oSignalIDVal != nullptr )
//        {
//            CSignalDescVal* psDesc = nullptr;
//            psDesc = psSignal->m_oSignalIDVal;
//
//            while( psDesc != nullptr && bFound == FALSE )
//            {
//                if( psDesc->m_omStrSignalDescriptor.Compare(omStr) == 0 )
//                {
//                    //                    n64SigVal = psDesc->m_n64SignalVal;
//                    n64SigVal = psDesc->m_DescValue.n64Value;
//                    un64SigVal = psDesc->m_DescValue.un64Value;
//                    bFound = TRUE;
//                }
//                else
//                {
//                    psDesc = psDesc->m_pouNextSignalSignalDescVal;
//                }
//            }
//        }
//        // Proceed for Non Signal Descriptor values and Signals with out any
//        // Signal Descriptor
//        if( bFound == FALSE)
//        {
//            // Calculate Raw Value
//            // Get the Signal Raw Value
//
//            // Get the value
//            dSignVal = 0;
//
//            dSignVal = atof(omStr);
//
//
//            if(psSignal->m_bySignalType == CHAR_INT)
//            {
//                /*n64SigVal = static_cast<UINT64>(
//                                ( dSignVal - psSignal->m_fSignalOffset) /
//                                psSignal->m_fSignalFactor );*/
//
//                // To handle the float data type precision values which is not zero
//                // (For Ex: 0.3000 is displayed as 0.300000001)
//                float f64SigVal = ( dSignVal - psSignal->m_fSignalOffset) /
//                                  psSignal->m_fSignalFactor;
//                n64SigVal = static_cast<INT64>(f64SigVal);
//                if (f64SigVal - (INT64)(f64SigVal) > 0.99990000)
//                {
//                    n64SigVal = static_cast<INT64>(ceil(f64SigVal));
//                }
//                else if (f64SigVal - (INT64)(f64SigVal) < 0.99990000)
//                {
//                    n64SigVal = static_cast<INT64>(floor(f64SigVal));
//                }
//            }
//            else
//            {
//                // Apply factor & offset
//                /*un64SigVal = static_cast<UINT64>(
//                                 ( dSignVal - psSignal->m_fSignalOffset) /
//                                 psSignal->m_fSignalFactor );*/
//
//                // To handle the float data type precision values which is not zero
//                // (For Ex: 0.3000 is displayed as 0.300000001)
//                float f64SigVal = ( dSignVal - psSignal->m_fSignalOffset) /
//                                  psSignal->m_fSignalFactor;
//                un64SigVal = static_cast<UINT64>(f64SigVal);
//                if (f64SigVal - (UINT64)(f64SigVal) > 0.99990000)
//                {
//                    un64SigVal = static_cast<UINT64>(ceil(f64SigVal));
//                }
//            }
//            // Check for Min-Max Limit
//            // If signed extend the sign bit
//            if(psSignal->m_bySignalType == CHAR_INT)
//            {
//                __int64 nVal = psSignal->m_SignalMaxValue.n64Value;
//                if( psSignal->m_bySignalType == CHAR_INT )
//                {
//                    s_vExtendSignBit( nVal,psSignal->m_unSignalLength );
//                }
//
//                if ( n64SigVal > nVal )
//                {
//                    n64SigVal = nVal;
//                    bDataInvalid = TRUE;
//                }
//                else
//                {
//                    nVal = psSignal->m_SignalMinValue.n64Value;
//                    if( psSignal->m_bySignalType == CHAR_INT )
//                    {
//                        s_vExtendSignBit( nVal,
//                                          psSignal->m_unSignalLength );
//                    }
//
//                    if ( n64SigVal < nVal )
//                    {
//                        n64SigVal = nVal;
//                        bDataInvalid = TRUE;
//                    }
//                }
//            }
//            else
//            {
//                UINT64 nVal = psSignal->m_SignalMaxValue.un64Value;
//                /*if( psSignal->m_bySignalType == CHAR_INT )
//                {
//                s_vExtendSignBit( nVal,psSignal->m_unSignalLength );
//                }*/
//
//                if ( un64SigVal > nVal )
//                {
//                    un64SigVal = nVal;
//                    bDataInvalid = TRUE;
//                }
//                else
//                {
//                    nVal = psSignal->m_SignalMinValue.un64Value;
//                    if ( un64SigVal < nVal )
//                    {
//                        un64SigVal = nVal;
//                        bDataInvalid = TRUE;
//                    }
//                }
//            }
//        }
//        // Update the data
//        UCHAR ucData[8] = {0};
//        //copy the existing data
//        memcpy(ucData, m_unData, sizeof(BYTE) * defMAX_BYTE);
//        //update the data
//        if(psSignal->m_bySignalType == CHAR_INT)
//        {
//            sSIGNALS::vSetSignalValue(psSignal, ucData, n64SigVal);
//        }
//        else
//        {
//            sSIGNALS::vSetSignalValue(psSignal, ucData, /*n64SigVal*/un64SigVal);
//        }
//
//        //copy to the memeber variable
//        memcpy(m_unData, ucData, sizeof(BYTE) * defMAX_BYTE);
//
//        //commented by kadoor vUpdateSignalData( psSignal, n64SigVal );
//        vUpdateDataBytes();
//        // Calculate Physical Value and update
//        // Set the Raw Value
//        if( TRUE == CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_HEX))
//        {
//            if( psSignal->m_bySignalType == CHAR_INT )
//            {
//                __int64 un64Val = n64SigVal;
//                s_vRemoveUnwantedBits( un64Val, psSignal->m_unSignalLength);
//                omstrDefault.Format( defFORMAT_INT64_HEX, un64Val);
//            }
//            else
//            {
//                UINT64 un64Val = un64SigVal;
//                s_vRemoveUnwantedBits( un64Val, psSignal->m_unSignalLength);
//                omstrDefault.Format( defFORMAT_INT64_HEX, un64Val);
//            }
//        }
//        else
//        {
//            if( psSignal->m_bySignalType == CHAR_INT )
//            {
//                omstrDefault.Format( defFORMAT_INT64_DECIMAL, n64SigVal);
//            }
//            else
//            {
//                omstrDefault.Format(defFORMAT_UINT64_DEC, un64SigVal);
//            }
//        }
//
//        m_omLctrSigList.SetItemText( nItem, def_RAW_VALUE_COLUMN,
//                                     omstrDefault );
//        // Set back the phy value. This will be the actual phy with
//        // decimal loss
//        // Check for Signal Descriptor value
//        if ( bFound == FALSE || bDataInvalid == TRUE)
//        {
//            if( psSignal->m_oSignalIDVal != nullptr )
//            {
//                CSignalDescVal* psDesc = nullptr;
//                psDesc = psSignal->m_oSignalIDVal;
//
//                while( psDesc != nullptr && bFound == FALSE )
//                {
//                    //                    if( psDesc->m_n64SignalVal == n64SigVal )
//                    if( psDesc->m_DescValue.n64Value == n64SigVal )
//                    {
//                        omstrDefault = psDesc->m_omStrSignalDescriptor;
//                        bFound = TRUE;
//                    }
//                    else
//                    {
//                        psDesc = psDesc->m_pouNextSignalSignalDescVal;
//                    }
//                }
//            }
//            if( bFound == FALSE )
//            {
//                if( psSignal->m_bySignalType == CHAR_INT )
//                {
//                    s_vExtendSignBit( n64SigVal,psSignal->m_unSignalLength );
//                }
//                BOOL bFloat = FALSE;
//                // Check for possible float value
//                // Check Factor. Float Value - Int value should be greater
//                // then 0 to have floating point value
//                if( ( psSignal->m_fSignalFactor -
//                        static_cast<int>(psSignal->m_fSignalFactor) ) > 0.0 )
//                {
//                    bFloat = TRUE;
//                }
//                // Check Offset only if the float flag is not set
//                else if( psSignal->m_fSignalOffset -
//                         static_cast<int>(psSignal->m_fSignalOffset) > 0.0 )
//                {
//                    bFloat = TRUE;
//                }
//                if( bFloat == TRUE )
//                {
//                    if( psSignal->m_bySignalType == CHAR_INT )
//                    {
//                        dSignVal = n64SigVal * psSignal->m_fSignalFactor +
//                                   psSignal->m_fSignalOffset;
//                        omstrDefault.Format(defSTR_FORMAT_PHY_VALUE, dSignVal);
//                    }
//                    else
//                    {
//                        dSignVal = un64SigVal * psSignal->m_fSignalFactor +
//                                   psSignal->m_fSignalOffset;
//                        omstrDefault.Format(defSTR_FORMAT_PHY_VALUE, dSignVal);
//                    }
//                }
//                else
//                {
//
//                    if( psSignal->m_bySignalType == CHAR_INT )
//                    {
//                        n64SigVal = __int64(n64SigVal * psSignal->m_fSignalFactor +
//                                            psSignal->m_fSignalOffset);
//                        omstrDefault.Format(defFORMAT_INT64_DECIMAL, n64SigVal);
//                    }
//                    else
//                    {
//                        un64SigVal = UINT64(un64SigVal * psSignal->m_fSignalFactor +
//                                            psSignal->m_fSignalOffset);
//                        omstrDefault.Format(defFORMAT_UINT64_DEC, un64SigVal);
//                    }
//                }
//
//            }
//            m_omLctrSigList.SetItemText( nItem, def_PHY_VALUE_COLUMN,
//                                         omstrDefault );
//        }
//        // Set the data value
//        vUpdateSignalMatrix();
//        //m_odSignalMatrix.vSetValue( m_bData );
//        //SetDirty(FALSE);
//        //vUpdateSelectedMessageDetails();
//    }
//}


/*******************************************************************************
 Function Name    : psGetSelectedSignalStruct
 Input(s)         : nIndex      -  Selected Signal index
 Output           : sSIGNALS *  - Selected Signal Pointer
 Functionality    : This function will return the signal pointer of the signal
                    pointed by the index. If it is not found it will return nullptr
 Member of        : CTxMsgDetailsView
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 19.07.2004
 Modification By  :
 Modification on  :
*******************************************************************************/
void CTransmitMsgLIN::psGetSelectedSignalStruct(std::list<SIGNAL_STRUCT> lstSigStrct, CString omStrSigName, SIGNAL_STRUCT& ouSigStrct)
{

    std::list<SIGNAL_STRUCT>::iterator itrSigList = lstSigStrct.begin();

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

void CTransmitMsgLIN::vGetSelSignalFromFrame(UINT unMsgId, std::list<SIGNAL_STRUCT>& omSignalList)
{
    ClusterConfig* pTemConfig = CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouClusterConfig;

    int nChannel = m_wndComboChannel.GetCurSel();
    if ( nChannel < 0 || nChannel > CHANNEL_ALLOWED )
    {
        return;
    }

    if ( pTemConfig != nullptr )
    {
        FRAME_STRUCT ouFrameStruct;
        CString omStrMsgName = "";
        HRESULT hr = pTemConfig->m_ouFlexChannelConfig[nChannel].GetFrame(unMsgId, 0, UNSPECIFIED, ouFrameStruct);

        BOOL bStop = FALSE;
        // Get the selected signal information
        /*CString omStrSigName =
            m_omLctrSigList.GetItemText(nIndex, 0);*/

        ouFrameStruct.GetSignalList(omSignalList);

        // Get the Signal List head pointer
        //psSignal = m_psSelectedMsgDetails->m_psSignals;
        //list<SIGNAL_STRUCT>::iterator itrSigList = omSignalList.begin();

        // Iterate through the list
        //while( itrSigList != omSignalList.end())
        //{
        //  // Compare the signal name
        //  if( omStrSigName.Compare(itrSigList->m_strSignalName.c_str() ) == 0 )
        //  {
        //      // If found then stop traversing
        //      break;
        //  }
        //  itrSigList++;
        //}
    }
}


void CTransmitMsgLIN::OnKillfocusDataBytesEdit()
{
    UINT unIDValue = IDC_EDIT_DB1;// ID of first databyte edit control

    CEdit* pEdit = nullptr;
    CString omStr = "";

    UCHAR ucData[8];
    CString  omStrFormatData;

    if( true == CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode)
    {
        omStrFormatData = def_HEX_FORMAT;
    }
    else
    {
        omStrFormatData = def_DECIMAL_FORMAT;
    }

    __int64 nValue;
    for(INT nIndex = 0; nIndex < defMAX_BYTE; nIndex++)
    {
        nValue = m_odDB[nIndex].lGetValue();
        if(nValue > 255)
        {
            nValue = 255;
            omStr.Format(omStrFormatData , nValue);
            m_odDB[nIndex].SetWindowText(omStr);
        }
        ucData[nIndex] = nValue;
    }

    std::list<Flexray_SSIGNALINFO> ouSignalInfoList;
    int nSel = m_lstMsg.GetNextItem(-1, LVIS_SELECTED);

    FRAME_STRUCT ouFrame;

    if ( S_OK == CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouClusterConfig->m_ouFlexChannelConfig[nGetChannel()-1].GetFrame(nGetMessageID(), ouFrame) )
    {
        bGetSignalInfo(ouFrame, ucData, ouFrame.m_nLength, ouSignalInfoList, CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode);

        std::list<Flexray_SSIGNALINFO>::iterator itrSig = ouSignalInfoList.begin();

        UINT unIndex = 0;
        while(itrSig != ouSignalInfoList.end())
        {
            m_omLctrSigList.SetItemText(unIndex, def_RAW_VALUE_COLUMN, itrSig->m_omRawValue.c_str());
            m_omLctrSigList.SetItemText(unIndex, def_PHY_VALUE_COLUMN, itrSig->m_omEnggValue.c_str());

            itrSig++;
            unIndex++;
        }
    }

    //copy the data
    vInitialiseDataBytes();
    memcpy(m_unData, ucData, sizeof(UCHAR) * defMAX_BYTE);

    vUpdateSignalMatrix();

    CString strDLC;
    m_odDLC.GetWindowText(strDLC);

    int nDLC =atoi(strDLC);

    std::string strTemp;
    std::string strText = "";
    for ( int i =0 ; i < nDLC; i++ )
    {
        vGetStringFromValue(m_unData[i].byByte, strTemp, false);
        strText += strTemp;
        strText += " ";
    }

    if ( nSel >= 0 )
    {
        vUpdateSelMsgDetails(eDataBytes, nSel);
        m_lstMsg.SetItemText(nSel, def_COLUMN_DATABYTES_INDEX, strText.c_str());
    }

    //m_omLctrSigList.SetSelectionMark(-1);

    int nSelIndex = m_omLctrSigList.GetNextItem(-1, LVNI_SELECTED);
    if( nSelIndex != -1 )
    {
        m_omLctrSigList.SetItemState( nSelIndex,
                                      0,
                                      LVIS_SELECTED | LVIS_FOCUSED );
    }
}


void CTransmitMsgLIN::vUpdateSelMsgDetails(eMsgUpdate eUpdateType, INT nItem, INT nDlc, INT nChannel, bool bEnable)
{
    EnterCriticalSection(&CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouCSMsgList);
    if(nItem != -1)
    {
        std::list<LIN_FRAME_DATA>::iterator itrList = CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouLIN_Frame_Data.begin();

        std::advance(itrList, nItem);
        std::string strTemp;
        std::string strText = "";
        switch(eUpdateType)
        {
            case eEnable:
            {
                itrList->bSelected = bEnable;
                CTxLINDataStore::ouGetTxLINDataStoreObj().nHandleSelectionState(nItem, bEnable);
            }
            break;
            case eDataBytes:

                //copy the data
                memcpy(itrList->m_ouLinMessage.m_ucData, m_unData, sizeof(UCHAR) * defMAX_BYTE);
                for ( int i =0 ; i < itrList->m_ouLinMessage.m_ucDataLen; i++ )
                {
                    vGetStringFromValue(m_unData[i].byByte, strTemp, false);
                    strText += strTemp;
                    strText += " ";
                }

                m_lstMsg.SetItemText(nItem, def_COLUMN_DATABYTES_INDEX, strText.c_str());

                break;
            case eDlc:
                if(nDlc != -1)
                {
                    itrList->m_ouLinMessage.m_ucDataLen = nDlc;

                    CString strDlc;
                    strDlc.Format("%d", nDlc);
                    //vGetStringFromValue(nDlc, strDlc, true);
                    m_lstMsg.SetItemText(nItem, def_COLUMN_DLC_INDEX, strDlc);
                }
                break;
            case eChannel:

                if(nChannel != -1)
                {
                    itrList->m_ouLinMessage.m_ucChannel = nChannel+1;
                }
                break;
            case eKeyCheck:
            {
                itrList->bKeyEnabled = bEnable;

                if(itrList->bKeyEnabled == TRUE)
                {
                    CTxLINDataStore::ouGetTxLINDataStoreObj().nInsertKey(nItem, chGetKeyVal(nItem));
                }
                else
                {
                    CTxLINDataStore::ouGetTxLINDataStoreObj().nDeleteKey(nItem);
                }
            }
            break;
            case eKeyval:
            {
                CString omStrKeyVal = m_lstMsg.GetItemText(nItem, def_COLUMN_KEY_INDEX);
                itrList->m_chKeyVal = ' ';
                if ( omStrKeyVal.GetLength() > 0 )
                {
                    itrList->m_chKeyVal = omStrKeyVal.GetAt(0);
                }

                if(itrList->bKeyEnabled == TRUE)
                {
                    CTxLINDataStore::ouGetTxLINDataStoreObj().nInsertKey(nItem, itrList->m_chKeyVal);
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
                CString omVal = m_lstMsg.GetItemText(nItem, def_COLUMN_REPETITION_INDEX);

                //BOOL bHex =   CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode;
                UINT nTimer;
                /*CHAR* pcStr = nullptr;
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
                    m_lstMsg.SetItemText(nItem, def_COLUMN_REPETITION_INDEX, strVal);
                }
                else if ( nTimer > def_MAXIMUM_TIMER_VAL )
                {
                    nTimer = def_MAXIMUM_TIMER_VAL;
                    CString strVal;
                    strVal.Format("%d", nTimer);
                    //vGetStringFromValue(nTimer, strVal, true);
                    m_lstMsg.SetItemText(nItem, def_COLUMN_REPETITION_INDEX, strVal);
                }

                itrList->nActualTimer = nTimer;
                itrList->nCurrentTimerVal = itrList->nActualTimer;
            }
            break;
        }
    }
    LeaveCriticalSection(&CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouCSMsgList);
}

char CTransmitMsgLIN::chGetKeyVal(int nItem)
{
    CString omStrKeyVal = m_lstMsg.GetItemText(nItem, def_COLUMN_KEY_INDEX);
    char chKeyVal = ' ';
    if ( omStrKeyVal.GetLength() > 0 )
    {
        chKeyVal = omStrKeyVal.GetAt(0);
    }

    return chKeyVal;
}

/*******************************************************************************
 Function Name    : OnUpdateEditDLC
 Input(s)         :
 Output           :
 Functionality    : This function will be called by frame work when user
                    changes DLC edit box value

 Member of        : CTxMsgDetailsView
 Friend of        :     -
 Author(s)        : Raja N
 Date Created     : 31.07.2004
 Modification     : Raja N on 10.08.2004, Added update of list data OnUpdate
/******************************************************************************/
void CTransmitMsgLIN::OnUpdateEditDLC()
{
    CWnd* pomWnd = GetFocus();
    bSetStatusText("");
    // Avoid processing the data if the dialog is canceled
    if( pomWnd != nullptr)
    {
        INT unID = pomWnd->GetDlgCtrlID();
        // If the update is because of user change
        if( unID == IDC_EDIT_DLC )
        {
            BOOL bEntryValid = FALSE;
            // Get the message IS
            int nMsgID = nGetMessageID();
            // IF it is valid
            if( nMsgID != -1)
            {
                // Get DLC value
                int nDLC =  (int)(m_odDLC.lGetValue());
                // If DLC Text is empty the above function call will return
                // 0. So make it -1 to denote invalid value
                if( nDLC == 0 )
                {
                    CString omStrText;
                    m_odDLC.GetWindowText(omStrText);
                    if( omStrText.IsEmpty() == TRUE )
                    {
                        nDLC = -1;
                    }
                }
                // If it is valid
                if( nDLC <= 0 || nDLC > 8 )
                {
                    bSetStatusText(defSTR_INVALID_DLC);
                }
                else
                {
                    vUpdateStateDataBytes();

                    int nSel = m_lstMsg.GetNextItem(-1, LVIS_SELECTED );
                    if ( nSel >= 0 )
                    {
                        vUpdateSelMsgDetails(eDlc, nSel, nDLC);
                        vUpdateSelMsgDetails(eDataBytes, nSel);
                    }
                }
            }
        }
    }
}


void CTransmitMsgLIN::OnNMTransmitMessagesRightClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    OnNMTransmitMessagesClick(pNMHDR, pResult);
}

void CTransmitMsgLIN::OnNMTransmitMessagesClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    LVHITTESTINFO hitinfo;
    *pResult = 0;

    bool bChecked = false;

    //Copy click point
    hitinfo.pt = pNMListView->ptAction;

    //Make the hit test...
    int nItem = m_lstMsg.HitTest(&hitinfo);
    int nChannel = m_wndComboChannel.GetCurSel();
    if ( nChannel < 0 || nChannel > CHANNEL_ALLOWED )
    {
        *pResult = 1;
        return;
    }

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


void CTransmitMsgLIN::OnItemchangedLstcMsgDetails( NMHDR* pNMHDR, LRESULT* pResult )
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ( CTxLINDataStore::ouGetTxLINDataStoreObj().m_eBusStatus ==  BUS_CONNECTED )
    {
        m_omComboMsgIDorName.EnableWindow(FALSE);
    }
    else
    {
        m_omComboMsgIDorName.EnableWindow(TRUE);
    }

    // Selected & Focused
    if( pNMListView->uNewState & (LVIS_FOCUSED | LVIS_SELECTED) )
    {
        // For Row selection
        //vUpdateSignalMatrix( pNMListView->iItem);
        if(pNMListView->iItem != -1)
        {
            LIN_FRAME_DATA objLinFrame = vGetMsgDetails(pNMListView->iItem);
            nUpdateMessageDetails(objLinFrame);
        }
    }

    // Notification for text change
    if( pNMListView->uChanged == LVIF_TEXT)
    {
        LV_DISPINFO ls;
        if(m_lstMsg.sGetModificationStructure(ls) == TRUE)
        {
            switch( ls.item.iSubItem)
            {
                    // Raw Value Change
                case def_COLUMN_REPETITION_INDEX:
                    vUpdateSelMsgDetails(eRepetitionVal, pNMListView->iItem);
                    break;
                    // Physical Value Change
                case def_COLUMN_KEY_INDEX:
                    vUpdateSelMsgDetails(eKeyval, pNMListView->iItem);
                    break;
            }
        }

    }
    if( pNMListView->uNewState & defMASK_CHECK_UNCHECK && m_bFromEvent == false )
    {

        vUpdateSelMsgDetails(eEnable, pNMListView->iItem, 0, 1, m_lstMsg.GetCheck(pNMListView->iItem) == TRUE);
    }

    if((pNMListView->iItem >= 0) && ( pNMListView->uNewState & (LVIS_FOCUSED|LVIS_SELECTED)))
    {
        m_nSelectedMsgIndex = pNMListView->iItem ;

        if(CTxLINDataStore::ouGetTxLINDataStoreObj().m_eBusStatus == BUS_CONNECTED)
        {
            m_omButtonAddMsg.SetWindowText(def_SEND_Message);
            m_omButtonAddMsg.EnableWindow(TRUE);
        }
        else
        {
            m_omButtonAddMsg.EnableWindow(FALSE);
        }
        *pResult = 0;
    }
    int nSlectedFrame = m_lstMsg.GetNextItem(-1, LVIS_SELECTED);
    if ( nSlectedFrame < 0 )
    {
        m_nSelectedMsgIndex = -1;
        m_omButtonAddMsg.SetWindowText(def_ADD_MESSAGE_SRING);
        if(CTxLINDataStore::ouGetTxLINDataStoreObj().m_eBusStatus == BUS_CONNECTED)
        {
            GetDlgItem(IDC_CBTN_ADD_MSG_LIN)->EnableWindow(FALSE);
        }
        else
        {
            GetDlgItem(IDC_CBTN_ADD_MSG_LIN)->EnableWindow(TRUE);
        }
        *pResult = 0;
        return;
    }

    *pResult = 0;
}

INT CTransmitMsgLIN::nUpdateMessageDetails(LIN_FRAME_DATA& ouLinData)
{
    vInitialiseDataBytes();
    memcpy(m_unData, ouLinData.m_ouLinMessage.m_ucData, sizeof(UCHAR) * defMAX_BYTE);

    //6. DLC
    CString strText;
    strText.Format("%d", ouLinData.m_ouLinMessage.m_ucDataLen);
    m_odDLC.SetWindowText(strText);

    //1. DataBytes
    vUpdateDataBytes(ouLinData);

    //2. Signal List
    bUpdateSignalList(ouLinData);

    //3. Signal Values
    if ( nGetChannel() >= 1 )
    {
        FRAME_STRUCT ouFrame;
        if ( S_OK == CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouClusterConfig->m_ouFlexChannelConfig[nGetChannel()-1].GetFrame(ouLinData.m_ouLinMessage.m_ucMsgID, ouFrame) )
        {
            //5. Combo
            char chString[MAX_PATH];

            if ( CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode == false )
            {
                //sprintf_s(chString, MAX_PATH, "%-25s [%8d]", itrMsgId->second.c_str(), itrMsgId->first);
                sprintf_s(chString, MAX_PATH, "%s [%0d]", ouLinData.m_strFrameName.c_str(), ouLinData.m_ouLinMessage.m_ucMsgID);
            }
            else
            {
                sprintf_s(chString, MAX_PATH, "%s [0x%02X]", ouLinData.m_strFrameName.c_str(), ouLinData.m_ouLinMessage.m_ucMsgID);
            }
            m_omComboChannelID.SetCurSel(-1);
            m_omComboMsgIDorName.SetWindowText(chString);

            std::list<Flexray_SSIGNALINFO> ouSignalInfo;
            bGetSignalInfo(ouFrame, ouLinData.m_ouLinMessage.m_ucData, ouLinData.m_ouLinMessage.m_ucDataLen, ouSignalInfo
                           , CTxLINDataStore::ouGetTxLINDataStoreObj().m_bHexMode);
            std::list<Flexray_SSIGNALINFO>::iterator itrSignalList = ouSignalInfo.begin();
            int nRow = 0;
            while ( itrSignalList != ouSignalInfo.end() )
            {
                m_omLctrSigList.SetItemText(nRow, def_RAW_VALUE_COLUMN, itrSignalList->m_omRawValue.c_str());
                m_omLctrSigList.SetItemText(nRow, def_PHY_VALUE_COLUMN, itrSignalList->m_omEnggValue.c_str());
                itrSignalList++;
                nRow++;
            }
        }
        else
        {
            //5. Combo
            std::string strValue;
            vGetStringFromValue(ouLinData.m_ouLinMessage.m_ucMsgID, strValue);
            m_omComboChannelID.SetCurSel(-1);
            m_omComboMsgIDorName.SetWindowText(strValue.c_str());
        }

    }

    //4. Matrix
    vInitialiseDataBytes();
    memcpy(m_unData, ouLinData.m_ouLinMessage.m_ucData, sizeof(UCHAR) * defMAX_BYTE);
    m_odSignalMatrix.vSetMessageLength(m_odDLC.lGetValue());
    vUpdateSignalMatrix();


    //5. DLC
    m_odDLC.SetReadOnly(false);

    //6. Header or Slave
    ((CButton*)GetDlgItem(IDC_LIN_MSGTYPE_SLAVE))->SetCheck(TRUE);
    ((CButton*)GetDlgItem(IDC_LIN_MSGTYPE_HEADER))->SetCheck(FALSE);

    //7. Channel
    m_omComboChannelID.SetCurSel(ouLinData.m_ouLinMessage.m_ucChannel - 1);
    m_omLctrSigList.EnableWindow(true);
    if ( ouLinData.m_ouLinMessage.m_ucMsgTyp == LIN_MASTER_RESPONSE )
    {
        //1. Disable DLC and Databytes
        m_odDLC.SetReadOnly(true);
        for ( int nId = 0; nId < 8; nId++ )
        {
            m_odDB[nId].SetReadOnly(true);
        }

        ((CButton*)GetDlgItem(IDC_LIN_MSGTYPE_SLAVE))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_LIN_MSGTYPE_HEADER))->SetCheck(TRUE);
        m_omLctrSigList.EnableWindow(false);

    }
    return 0;
}


LIN_FRAME_DATA CTransmitMsgLIN::vGetMsgDetails(INT nSelIndex)
{
    UINT unIndex = 0;
    std::list<LIN_FRAME_DATA>::iterator itrList = CTxLINDataStore::ouGetTxLINDataStoreObj().m_ouLIN_Frame_Data.begin();

    std::advance(itrList, nSelIndex);

    return *itrList;
}

/**
* \brief Adjust Message Combo Box Width
*
* This function adjusts the width of the Message Combo Box
* according to its contents.
*/
void CTransmitMsgLIN::vAdjustWidthMessageComboBox()
{
    CString str;
    CSize sz;
    int dx = 0;
    TEXTMETRIC tm;
    CDC* pDC = m_omComboMsgIDorName.GetDC();
    CFont* pFont = m_omComboMsgIDorName.GetFont();

    // Select the listbox font, save the old font
    CFont* pOldFont = pDC->SelectObject(pFont);
    // Get the text metrics for avg char width
    pDC->GetTextMetrics(&tm);

    for (int i = 0; i < m_omComboMsgIDorName.GetCount(); i++)
    {
        m_omComboMsgIDorName.GetLBText(i, str);
        sz = pDC->GetTextExtent(str);
        sz = pDC->GetTextExtent(str);

        // Add the avg width to prevent clipping
        sz.cx += tm.tmAveCharWidth;
        if (sz.cx > dx)
        {
            dx = sz.cx;
        }
    }

    // Select the old font back into the DC
    pDC->SelectObject(pOldFont);
    m_omComboMsgIDorName.ReleaseDC(pDC);
    // Adjust the width for the vertical scroll bar and the left and right border.
    dx += ::GetSystemMetrics(SM_CXVSCROLL) + 2*::GetSystemMetrics(SM_CXEDGE);

    // Set the width of the list box so that every item is completely visible.
    m_omComboMsgIDorName.SetDroppedWidth(dx);
}
