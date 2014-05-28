///***********************************************************************************
//Project           :   Frame
//FileName      :   TransmitMsg.cpp
//Description       :   Implementation of CTransmitMsg class
//Directory     :
//Version           :   V1.0
//Authors           :   Ravi D
//Date Created  :   24/10/2006
//Date Modified :
//Modifications :
//**********************************************************************************/
#pragma once
#include "TxWindowFlexRay_stdafx.h"
#include "DataTypes/Base_FlexRay_Buffer.h"
#include "Utility/Utility.h"
#include "..\DIL_Interface\BaseDIL_FLEXRAY.h"
#include "..\DIL_Interface\DIL_Interface_extern.h"
#include "TxMsgChildFrame.h"
#include "TransmitMsg.h"
#include "TxFlexrayMsgHandlerDlg.h"
#include "ConfigDetails.h"
#include "..\DataTypes\MsgSignal_Datatypes.h"


const BYTE COLUMN_MSGID     = 0x02;
const BYTE COLUMN_DESCR     = 0x04;
const BYTE COLUMN_CYCLE     = 0x08;
const BYTE COLUMN_CHANNEL   = 0x10;
const BYTE COLUMN_DATALEN   = 0x20;
const BYTE COLUMN_DATABYTE  = 0x40;


#define def_MSGNAME_COLUMN_INDEX    0
#define def_SLOTID_COLUMN_INDEX     1
#define def_BASECYCLE_COLUMN_INDEX  2
#define def_REPETITION_COLUMN_INDEX 3
#define def_CHANNEL_COLUMN_INDEX    4
#define def_DLC_COLUMN_INDEX        5
#define def_SLOTTYPE_COLUMN_INDEX   6

#define BITLENGTH 16

#define TARGET_RESOLUTION 1         // 1-millisecond target resolution
#define defSTR_FORMAT_DISPLAY_DEC    _T("%-60s   [%16I64d]")
#define defSTR_FORMAT_DISPLAY_HEX    _T("%-60s   [%16I64X]")

HWND CTransmitMsg::m_hTransmitWnd = NULL;

#define TX_MSG_WND_CONFIG_SECTION_NAME                  "TXWndProperty"

extern CBaseDIL_FLEXRAY* g_pouDIL_FLEXRAY_Interface;


IMPLEMENT_DYNCREATE(CTransmitMsg, CFormView)

BEGIN_MESSAGE_MAP(CTransmitMsg, CFormView)
    ON_BN_CLICKED(IDC_CBTN_ADDNEW, OnBtnClickedAddnew)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_TRANSMIT_MSG, OnNMTransmitMessagesClick)
    ON_NOTIFY(NM_RCLICK, IDC_LSTC_TRANSMIT_MSG, OnNMTransmitMessagesRightClick)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDC_BT_UPDATE, OnBnClickedBtUpdate)
    ON_BN_CLICKED(IDC_DEL_FRAME, OnBnClickedBtDelFrame)
    ON_BN_CLICKED(IDC_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_BT_DEL_ALL, OnBnClickedBtDelAll)

    ON_WM_CLOSE()

    ON_MESSAGE(ON_LIST_ITEM_CLICK, OnClickFlexListCtrl)

    ON_WM_CLOSE()
    ON_WM_DESTROY()
    ON_MESSAGE(WM_USER+44,vUserCommand)
    ON_MESSAGE(WM_DATA_UPDATED,vOnDataBytesUpdated)
    ON_BN_CLICKED(IDC_RADIO_SIGNAL_VIEW, OnRadioSignalView)
    ON_BN_CLICKED(IDC_RADIO_BYTE_VIEW, OnRadioByteView)

END_MESSAGE_MAP()

//**********************************************************************************
//Function Name :   CTransmitMsg
//Input(s)      :   -
//Output        :   -
//Functionality :   Default Constructor
//Member of     :   CTransmitMsg
//Friend of     :   -
//Authors           :   Ravi D
//Date Created  :   24/10/2006
//Modifications :   -
//************************************************************************************/

CTransmitMsg::CTransmitMsg()
    : CFormView(CTransmitMsg::IDD)
{
    m_lCount = 0;
    m_lCellPos = 0;
    m_bInitDone = FALSE;
    m_bMsgLstInit = FALSE;
    m_bStartEnabled = TRUE;
    m_bDisplayColumns = COLUMN_MSGID | COLUMN_DESCR | COLUMN_CYCLE |
                        COLUMN_CHANNEL | COLUMN_DATALEN | COLUMN_DATABYTE;
    //m_omMsgBlockMan.RemoveAll();
    m_pouFlxTxMsgBuffer = NULL;
    m_bModified = FALSE;
    m_ouCurrentView = BYTE_VIEW;
    m_bDataModified  = false;
}

///**********************************************************************************
//Function Name :   ~CTransmitMsg
//Input(s)      :   -
//Output            :   -
//Functionality :   Destructor
//Member of     :   CTransmitMsg
//Friend of     :   -
//Authors           :   Ravi D
//Date Created  :   24/10/2006
//Modifications :   -
//************************************************************************************/

CTransmitMsg::~CTransmitMsg()
{
}


//**********************************************************************************
//Function Name :   DoDataExchange
//Input(s)      :   pDX - Pointer to the exchange object
//Output            :   -
//Functionality :   To exchange and validate the dialog data
//Member of     :   CTransmitMsg
//Friend of     :   -
//Authors           :   Ravi D
//Date Created  :   24/10/2006
//Modifications :   -
//************************************************************************************

void CTransmitMsg::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CHex_Block_EditorDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    DDX_Control(pDX, IDC_SIGNAL_FLEXLIST, m_oFlexListCtrl);
    DDX_Control(pDX, IDC_COMBO_CHANNEL_SEL, m_wndComboChannel);
    DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_SEL, m_nComboChannel);
    DDV_MinMaxInt(pDX, m_nComboChannel, 1, 16);
}


LRESULT CTransmitMsg::vOnDataBytesUpdated(WPARAM wParam, LPARAM lParam)
{
    GetDlgItem(IDC_BT_UPDATE)->EnableWindow(TRUE);
    GetDlgItem(IDC_BT_UPDATE)->EnableWindow(TRUE);
    m_bDataModified = true;
    return S_OK;
}

LRESULT CTransmitMsg::vUserCommand(WPARAM wParam, LPARAM lParam)
{
    int nUserSel = 0;
    nUserSel = static_cast <int>(wParam);
    switch(nUserSel)
    {
        case 0:
            UpdateTxView(CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_nChannelsConfigured);
            SwitchView(m_ouCurrentView);
            break;
        case 1:
            OnBusConnect((bool)lParam);
    }
    return S_OK;

}

void CTransmitMsg::vChangeDelButtonStatus(bool bStatus)
{
    CWnd* omBtWnd = (CWnd*)GetDlgItem(IDC_DEL_FRAME);
    if ( omBtWnd != NULL )
    {
        if( CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_eBusStatus == BUS_DISCONNECTED && m_lstMsg.GetItemCount() > 0 )
        {
            omBtWnd->EnableWindow(TRUE);
        }
        else
        {
            omBtWnd->EnableWindow(FALSE);

        }
    }

    omBtWnd = (CWnd*)GetDlgItem(IDC_BT_DEL_ALL);
    if ( omBtWnd != NULL )
    {
        if( CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_eBusStatus == BUS_DISCONNECTED && m_lstMsg.GetItemCount() > 0 )
        {
            omBtWnd->EnableWindow(TRUE);
        }
        else
        {
            omBtWnd->EnableWindow(FALSE);

        }
    }

    omBtWnd = (CWnd*)GetDlgItem(IDC_CBTN_ADDNEW);
    if ( omBtWnd != NULL )
    {
        int nChannel = m_wndComboChannel.GetCurSel();
        if ( (nChannel < 0 || nChannel > CHANNEL_ALLOWED) || CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFrameList[nChannel].size() <= 0  )
        {
            omBtWnd->EnableWindow(FALSE);
        }
        else
        {
            omBtWnd->EnableWindow(TRUE);

        }
    }
}

void CTransmitMsg::OnBusConnect(bool bConnect)
{
    vChangeDelButtonStatus(!bConnect);
}

void CTransmitMsg::vGetStringFromValue(int nValue, std::string& strValue)
{
    char chValue[1024];
    if ( CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_bHexMode == false)
    {
        sprintf_s(chValue, 1024, "%d", nValue);
    }
    else
    {
        sprintf_s(chValue, 1024, "0x%X", nValue);
    }
    strValue = chValue;
}


// CTransmitMsg message handlers
//**********************************************************************************
//Function Name :   OnInitialUpdate
//Input(s)      :   -
//Output        :   -
//Functionality :   Initialize the form controls
//Member of     :   CTransmitMsg
//Friend of     :   -
//Authors           :   Ravi D
//Date Created  :   24/10/2006
//Modifications :   -
//************************************************************************************/

void CTransmitMsg::OnInitialUpdate()
{
    CHAR caColumnName[defMESSAGE_BLOCK_COLUMN][defSTRING_SIZE] =
    {
        defMESSAGE_BLOCK_NAME,
        defMESSAGE_BLOCK_TRIGGER,
        defMESSAGE_BLOCK_TRIG_VAL
    };

    CFormView::OnInitialUpdate();

    CWnd* pParentWnd = NULL;
    // Get Parent window pointer
    pParentWnd = pomGetParentWindow();
    // Update view pointer
    if( pParentWnd != NULL )
    {
        // Register this view pointer
        ((CTxMsgChildFrame*)pParentWnd)->m_pTransmitMsgView = this;
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
    m_omLctrMsgBlockName.SetExtendedStyle(
        LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

    // Setup the list control to add the messages.
    m_lstMsg.SubclassDlgItem(IDC_LSTC_TRANSMIT_MSG, this);
    m_lstMsg.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

    CWnd* pWnd = m_lstMsg.GetWindow(GW_CHILD);
    ASSERT(pWnd != NULL);


    //m_lstMsg.SetEditable();


    CRect rctList;
    m_lstMsg.GetWindowRect(&rctList);
    int nColWidth = rctList.Width() - 2;
    int colwidths[7] = { 12, 10, 10, 7, 7,8, 25 };
    int total_cx = 0;

    TCHAR* acColData[]=
    {
        _T("Message Name"), _T("Frame Id"), _T("Base Cycle"), _T("Repetition"),
        _T("Channel(A\\B)"), _T("Data Length"), _T("Slot Type")
    };

    LVCOLUMN lvcolumn;
    memset(&lvcolumn, 0, sizeof(lvcolumn));
    lvcolumn.mask =  LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
    ColumnProperty  ouProp;
    ouProp.bMovable = true;
    // add columns
    for (int i = 0; i < (sizeof (acColData) / sizeof acColData[0]); i++)
    {
        // Column formating
        if (( i == FIRST_COL )||( i == SEVENTH_COL)/*||( i == SEVENTH_COL)*/) //4 col - Cycle No 6-DataLen , 7 - DataBytes
        {
            lvcolumn.fmt = LVCFMT_LEFT;
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
        m_lstMsg.InsertColumn(i, &lvcolumn, ouProp);
    }



    m_hTransmitWnd = m_hWnd;



    int nConfigChannel = CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_nChannelsConfigured;
    CString strChannel;

    for ( int i = 0 ; i < nConfigChannel ; i++ )
    {
        strChannel.Format("%d", i+1);
        m_wndComboChannel.AddString(strChannel);
    }

    m_wndComboChannel.SetCurSel(0);

    CButton* pBtWnd = (CButton*)GetDlgItem(IDC_RADIO_BYTE_VIEW);
    if ( pBtWnd != NULL)
    {
        pBtWnd->SetCheck(TRUE);
    }


    UpdateTxView(CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_nChannelsConfigured);
    m_bInitDone = FALSE;
    InitFlexList(m_ouCurrentView);
    if ( CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_eBusStatus == BUS_CONNECTED )
    {
        vChangeDelButtonStatus(false);
    }
    else
    {
        vChangeDelButtonStatus(true);
    }

    GetDlgItem(IDC_BT_UPDATE)->EnableWindow(FALSE);
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
CWnd* CTransmitMsg::pomGetParentWindow() const
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


void CTransmitMsg::vInsertMessage(FRAME_STRUCT& ouFrame)
{
    int nRows = m_lstMsg.GetItemCount();

    m_lstMsg.InsertItem(nRows, ouFrame.m_strFrameName.c_str());

    std::string strText;

    vGetStringFromValue(ouFrame.m_nSlotId, strText);
    m_lstMsg.SetItemText(nRows, def_SLOTID_COLUMN_INDEX, strText.c_str());


    vGetStringFromValue(ouFrame.m_nBaseCycle, strText);
    m_lstMsg.SetItemText(nRows, def_BASECYCLE_COLUMN_INDEX, strText.c_str());


    vGetStringFromValue(ouFrame.m_nReptition, strText);
    m_lstMsg.SetItemText(nRows, def_REPETITION_COLUMN_INDEX, strText.c_str());

    CString strString;
    if ( ouFrame.m_ouChannel == CHANNEL_A )
    {
        strString.Format("%s", "A" );
    }
    else if ( ouFrame.m_ouChannel == CHANNEL_B )
    {
        strString.Format("%s", "B" );
    }
    else if ( ouFrame.m_ouChannel == CHANNEL_AB )
    {
        strString.Format("%s", "AB" );
    }
    m_lstMsg.SetItemText(nRows, def_CHANNEL_COLUMN_INDEX, strString);

    vGetStringFromValue(ouFrame.m_nLength, strText);
    m_lstMsg.SetItemText(nRows, def_DLC_COLUMN_INDEX, strText.c_str());

    //Has to be moved to function
    if (ouFrame.m_eSlotType == STATIC )
    {
        strText = "static";
    }
    if (ouFrame.m_eSlotType == DYNAMIC )
    {
        strText = "dynamic";
    }
    m_lstMsg.SetItemText(nRows, def_SLOTTYPE_COLUMN_INDEX, strText.c_str());

}

void CTransmitMsg::OnBtnClickedAddnew()
{
    // Retrieve supported/permitted transmit message list from DIL and
    // initialise the dialog box with the same.

    m_bModified = TRUE;


    int nChannel = m_wndComboChannel.GetCurSel();
    if ( nChannel < 0 || nChannel > CHANNEL_ALLOWED )
    {
        return ;
    }
    //TODO::Check required.

    CFlexrayMsgHandlerDlg odFlexrayMsgHandlerDlg(nChannel);

    if (odFlexrayMsgHandlerDlg.DoModal() == IDOK)
    {
        std::map<long, int>::iterator itrSelectedItems = odFlexrayMsgHandlerDlg.m_pSelectedItem.begin();

        std::list<FRAME_STRUCT>& ouFrameList = CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFrameList[nChannel];
        std::list<FRAME_STRUCT>::iterator itrFrameList = ouFrameList.begin();
        for ( ; itrSelectedItems != odFlexrayMsgHandlerDlg.m_pSelectedItem.end(); itrSelectedItems++ )
        {
            itrFrameList = ouFrameList.begin();
            advance(itrFrameList, itrSelectedItems->second);
            if ( ouFrameList.end() != itrFrameList )
            {
                FLEXRAY_FRAME_DATA ouFrameData;
                ouFrameData.m_ouFlexFrame = *itrFrameList;
                CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFlexray_Frame_Data[nChannel].push_back(ouFrameData);
                vInsertMessage(*itrFrameList);
            }
        }
    }
    vChangeDelButtonStatus();
}



int CTransmitMsg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFormView::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    return 0;
}



void CTransmitMsg::GetTotalClientRect(LPRECT lpRect)
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



//**********************************************************************************
//Function Name :   nGetCheckedItemCount
//Input(s)      :   -
//Output        :   -
//Functionality :   find the List Item Checked Count
//Member of     :   CTransmitMsg
//Friend of     :   -
//Authors       :   Sandip R. Bhirud
//Date Created  :   07/03/2007
//Modifications :   -
//************************************************************************************

int CTransmitMsg:: nGetCheckedItemCount()
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
//  Member of           : CTransmitMsg
//  Friend of           : None
//  Author              : Ashwin R Uchil
//  Creation Date       : 21.3.2013
//  Modifications       :
//********************************************************************************

//void CTransmitMsg::InitFlexList(BYTE yByteCount, unsigned short* pwBuffer)
void CTransmitMsg::DeleteAllMsgListColumns()
{
    int nColumns=m_oFlexListCtrl.GetHeaderCtrl()->GetItemCount();

    for ( int i = nColumns-1; i >= 0; i--)
    {
        m_oFlexListCtrl.DeleteColumn (i);
    }
}
void CTransmitMsg::InitFlexList(DATA_VIEW ouDataView)
{
    m_oFlexListCtrl.DeleteAllItems();
    DeleteAllMsgListColumns();
    if ( ouDataView == SIGNAL_VIEW )
    {
        CHAR caColumnName[][defSIGNAL_LIST_STRING_MAX_LENGTH]
            = { "Signal Name",
                "Raw Value",
                "Physical Value",
                "Unit"
              };
        INT nColumnFormat[]     = { LVCFMT_RIGHT,
                                    LVCFMT_CENTER,
                                    LVCFMT_CENTER,
                                    LVCFMT_CENTER
                                  };
        RECT rListCtrlRect;
        INT nTotalColunmSize = 0;
        INT nTotalStrLengthPixel = 0;
        INT nColumnSize = 0;

        m_oFlexListCtrl.GetWindowRect( &rListCtrlRect);
        nTotalColunmSize     = rListCtrlRect.right - rListCtrlRect.left;
        nTotalStrLengthPixel = 0;

        UINT i; //i declared outside loop
        for(i=0; i<4; i++)
        {
            nTotalStrLengthPixel +=
                m_oFlexListCtrl.GetStringWidth(caColumnName[i]) ;
        }
        //Insert each column name after calculating the size for the same.
        for(i=0; i<4; i++)
        {
            nColumnSize  = m_oFlexListCtrl.GetStringWidth(caColumnName[i]) ;
            nColumnSize +=
                (nTotalColunmSize-nTotalStrLengthPixel)/4;
            m_oFlexListCtrl.InsertColumn(i, caColumnName[i],
                                         nColumnFormat[i], nColumnSize - 1);
        }

        // Set the extended style
        m_oFlexListCtrl.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
        SNUMERICINFO    sNumInfo;
        SLISTINFO       sListInfo;
        SUSERPROGINFO   sProgInfo;




        sListInfo.m_eType = eNoControl;
        m_oFlexListCtrl.vSetColumnInfo(0, 1, sListInfo);
        sNumInfo.m_byBase = BASE_HEXADECIMAL;
        sNumInfo.m_nTextLimit = 4;
        m_oFlexListCtrl.vSetNumericInfo(0, 1, sNumInfo);

        // Column 2 : Numeric Edit with Spin Control
        sListInfo.m_eType = eNoControl;
        m_oFlexListCtrl.vSetColumnInfo(0, 2, sListInfo);
        //m_oFlexListCtrl.vSetNumericInfo(0, 2, sNumInfo);

        sListInfo.m_eType = eNoControl;
        m_oFlexListCtrl.vSetColumnInfo(0, 3, sListInfo);
        //m_oFlexListCtrl.vSetNumericInfo(0, 3, sNumInfo);

        sListInfo.m_eType = eNoControl;
        m_oFlexListCtrl.vSetColumnInfo(0, 4, sListInfo);
        //m_oFlexListCtrl.vSetNumericInfo(0, 4, sNumInfo);
    }

    else if ( ouDataView == BYTE_VIEW )
    {
        CHAR caColumnName[][defSIGNAL_LIST_STRING_MAX_LENGTH]
            = { "Index",
                "00",
                "01",
                "02",
                "03",
                "04",
                "05",
                "06",
                "07"
              };
        INT nColumnFormat[]     = { LVCFMT_RIGHT,
                                    LVCFMT_CENTER,
                                    LVCFMT_CENTER,
                                    LVCFMT_CENTER,
                                    LVCFMT_CENTER,
                                    LVCFMT_CENTER,
                                    LVCFMT_CENTER,
                                    LVCFMT_CENTER,
                                    LVCFMT_CENTER
                                  };
        RECT rListCtrlRect;
        INT nTotalColunmSize = 0;
        INT nTotalStrLengthPixel = 0;
        INT nColumnSize = 0;

        m_oFlexListCtrl.GetWindowRect( &rListCtrlRect);
        nTotalColunmSize     = rListCtrlRect.right - rListCtrlRect.left;
        nTotalStrLengthPixel = 0;

        UINT i; //i declared outside loop
        for(i=0; i<defSIGNAL_FRAME_COLUMN; i++)
        {
            nTotalStrLengthPixel +=
                m_oFlexListCtrl.GetStringWidth(caColumnName[i]) ;
        }
        //Insert each column name after calculating the size for the same.
        for(i=0; i<defSIGNAL_FRAME_COLUMN; i++)
        {
            nColumnSize  = m_oFlexListCtrl.GetStringWidth(caColumnName[i]) ;
            nColumnSize +=
                (nTotalColunmSize-nTotalStrLengthPixel)/defSIGNAL_FRAME_COLUMN;
            m_oFlexListCtrl.InsertColumn(i, caColumnName[i],
                                         nColumnFormat[i], nColumnSize - 1);
        }

        // Set the extended style
        m_oFlexListCtrl.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
        SNUMERICINFO    sNumInfo;
        SLISTINFO       sListInfo;
        SUSERPROGINFO   sProgInfo;


        sListInfo.m_eType = eBuddy;

        sNumInfo.m_uMaxVal.m_n64Value = 256;
        sNumInfo.m_uMinVal.m_n64Value = 0;
        sNumInfo.m_byFlag = SIGNED_VAL;

        CButton* omBtnWindow = (CButton*)GetDlgItem(IDC_RADIO_BYTE_VIEW);
        if ( NULL != omBtnWindow )
        {
            if ( CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_bHexMode == false)
            {
                omBtnWindow->SetWindowText("Byte View (DEC)");
                sNumInfo.m_byBase = BASE_DECIMAL;
                sNumInfo.m_nTextLimit = 3;
            }
            else
            {
                omBtnWindow->SetWindowText("Byte View (HEX)");
                sNumInfo.m_byBase = BASE_HEXADECIMAL;
                sNumInfo.m_nTextLimit = 2;
            }
        }



        m_oFlexListCtrl.vSetColumnInfo(0, 1, sListInfo);
        m_oFlexListCtrl.vSetNumericInfo(0, 1, sNumInfo);

        // Column 2 : Numeric Edit with Spin Control
        sListInfo.m_eType = eNoControl;
        m_oFlexListCtrl.vSetColumnInfo(0, 2, sListInfo);
        m_oFlexListCtrl.vSetNumericInfo(0, 2, sNumInfo);

        sListInfo.m_eType = eNoControl;
        m_oFlexListCtrl.vSetColumnInfo(0, 3, sListInfo);
        m_oFlexListCtrl.vSetNumericInfo(0, 3, sNumInfo);

        sListInfo.m_eType = eNoControl;
        m_oFlexListCtrl.vSetColumnInfo(0, 4, sListInfo);
        m_oFlexListCtrl.vSetNumericInfo(0, 4, sNumInfo);

        sListInfo.m_eType = eNoControl;
        m_oFlexListCtrl.vSetColumnInfo(0, 5, sListInfo);
        m_oFlexListCtrl.vSetNumericInfo(0, 5, sNumInfo);

        sListInfo.m_eType = eNoControl;
        m_oFlexListCtrl.vSetColumnInfo(0, 6, sListInfo);
        m_oFlexListCtrl.vSetNumericInfo(0, 6, sNumInfo);

        sListInfo.m_eType = eNoControl;
        m_oFlexListCtrl.vSetColumnInfo(0, 7, sListInfo);
        m_oFlexListCtrl.vSetNumericInfo(0, 7, sNumInfo);

        sListInfo.m_eType = eNoControl;
        m_oFlexListCtrl.vSetColumnInfo(0, 8, sListInfo);
        m_oFlexListCtrl.vSetNumericInfo(0, 8, sNumInfo);
    }

}

BEGIN_EVENTSINK_MAP(CTransmitMsg, CFormView)

END_EVENTSINK_MAP()

//********************************************************************************
//  Function Name       : OnBnClickedBtUpdate
//  Input(s)            : None
//  Output              : void
//  Description         : None
//  Member of           : CTransmitMsg
//  Friend of           : None
//  Author              : Anupam Kumar
//  Creation Date       : 16/03/2007
//  Modifications       :
//********************************************************************************

void CTransmitMsg::OnBnClickedBtUpdate()
{
    vStoreDataBytes();

    int nChannel = m_wndComboChannel.GetCurSel();
    int nSlectedFrame = m_lstMsg.GetNextItem(-1, LVIS_SELECTED);
    if ( nChannel < 0 || nChannel >= CHANNEL_ALLOWED  || nSlectedFrame == -1)
    {
        return;
    }

    std::list<FLEXRAY_FRAME_DATA>::iterator itrFrameData = CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFlexray_Frame_Data[nChannel].begin();
    if ( itrFrameData ==  CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFlexray_Frame_Data[nChannel].end() )
    {
        return;
    }

    advance(itrFrameData, nSlectedFrame);
    if ( itrFrameData ==  CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFlexray_Frame_Data[nChannel].end() )
    {
        return;
    }

    //Validate Again with Same Messsage Name
    if ( itrFrameData->m_ouFlexFrame.m_strFrameName == m_ouCurrentMsg.m_ouFlexFrame.m_strFrameName )
    {
        *itrFrameData = m_ouCurrentMsg;
    }

    //TODO::Send Updated Frame
    if (  m_lstMsg.GetCheck(nSlectedFrame) == true )
    {
        CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().UpdateMessagetoDIL(*itrFrameData, false);
    }
    GetDlgItem(IDC_BT_UPDATE)->EnableWindow(FALSE);
}


//********************************************************************************
//  Function Name       : OnBnClickedBtDelFrame
//  Input(s)            : None
//  Output              : void
//  Description         : None
//  Member of           : CTransmitMsg
//  Friend of           : None
//  Author              : Ashwin R Uchil
//  Creation Date       : 24.4.2013
//  Modifications       :
//********************************************************************************
void CTransmitMsg::OnBnClickedBtDelFrame()
{

    int nSelItem = m_lstMsg.GetNextItem(-1, LVNI_SELECTED);
    if ( -1 != nSelItem )
    {

        int nChannel = m_wndComboChannel.GetCurSel();

        if ( nChannel < 0 || nChannel > CHANNEL_ALLOWED )
        {
            return ;
        }
        /*if(AfxMessageBox("Are you sure you want to clear selected frame?",MB_YESNO) == IDNO )
        {
            return;
        }*/
        m_lstMsg.DeleteItem(nSelItem);
        std::list<FLEXRAY_FRAME_DATA>::iterator itrFrameData = CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFlexray_Frame_Data[nChannel].begin();
        advance(itrFrameData, nSelItem);
        CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFlexray_Frame_Data[nChannel].erase(itrFrameData);

        if(nSelItem >= 0 && m_lstMsg.GetItemCount() > 0)
        {
            nSelItem = nSelItem % m_lstMsg.GetItemCount();
            m_lstMsg.SetItemState(nSelItem, LVIS_SELECTED, LVIS_SELECTED);
            m_lstMsg.SetSelectionMark(nSelItem);
            SwitchView(m_ouCurrentView);
        }
        else
        {
            InitFlexList(m_ouCurrentView);
        }

    }
    vChangeDelButtonStatus();
    return;

}



//********************************************************************************
//  Function Name       : OnBnClickedBtDelAll
//  Input(s)            :
//  Output              : void
//  Description         : Event handler fordeleting msg block/group
//  Member of           : CTransmitMsg
//  Friend of           : None
//  Author              : Anish kr
//  Creation Date       : 14/03/07
//  Modifications       :
//********************************************************************************

void CTransmitMsg::OnBnClickedBtDelAll()
{
    int nLstCount = m_lstMsg.GetItemCount();

    if(nLstCount > 0)
    {
        if(AfxMessageBox("Are you sure you want to clear all frames in the block?",MB_YESNO) == IDNO )
        {
            return;
        }
    }

    while(nLstCount > 0)
    {
        int nChannel = m_wndComboChannel.GetCurSel();
        if ( nChannel < 0 || nChannel > CHANNEL_ALLOWED )
        {
            return ;
        }
        m_lstMsg.DeleteItem(nLstCount - 1);
        std::list<FLEXRAY_FRAME_DATA>::iterator itrFrameData = CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFlexray_Frame_Data[nChannel].begin();
        advance(itrFrameData, nLstCount - 1);
        CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFlexray_Frame_Data[nChannel].erase(itrFrameData);
        SwitchView(m_ouCurrentView);
        nLstCount = m_lstMsg.GetItemCount();
    }
    vChangeDelButtonStatus();
}


void CTransmitMsg::OnClose()
{
    CFormView::OnClose();
}


void CTransmitMsg::vLoadTxMsgConfig()
{
    BYTE* SectionBuffer = NULL;
    int nBufferLength = 0;
    //Remove all the TX msg block before copying from config file
    //  CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().vRemoveAllBlock(m_omMsgBlockMan);
    //CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().vRemoveAllBlock(CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_omTxMsgBlockMan);
    // First Get Tx Msg List

    CConfigDetails::ouGetConfigDetailsObject().bGetData((void*&) SectionBuffer,
            nBufferLength,TX_MSG_WND_CONFIG_SECTION_NAME);
    if (SectionBuffer != NULL)
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

void CTransmitMsg::vRefreshTxMsgWindow()
{
    m_bInitDone = TRUE;
    // Clear the message list control
    m_lstMsg.DeleteAllItems();
    //Remove all items from the list control
    m_omLctrMsgBlockName.DeleteAllItems();
    //int nBlockCount = m_omMsgBlockMan.GetSize();
    //if (nBlockCount > 0)
    //{
    //    for (int i = 0; i < nBlockCount; i++)
    //    {
    //        SMSGBLOCK* pouCurrBlock = m_omMsgBlockMan.GetAt(i);
    //        m_omLctrMsgBlockName.InsertItem(i, pouCurrBlock->m_omBlockName);
    //        if (pouCurrBlock->m_bCyclic)
    //        {
    //            m_omLctrMsgBlockName.SetItemText(i, 1, defMSG_CYCLIC);
    //        }
    //        else
    //        {
    //            m_omLctrMsgBlockName.SetItemText(i, 1, defMSG_MONOSHOT);
    //        }
    //        //The timing value is valid for bot type of blocks
    //        CString omTmpStr = _T("");
    //        omTmpStr.Format("%d ms", pouCurrBlock->m_unTimeInterval);
    //        m_omLctrMsgBlockName.SetItemText(i, 2, omTmpStr);

    //        m_omLctrMsgBlockName.SetCheck(i, pouCurrBlock->m_bActive);
    //    }
    //    m_bInitDone = FALSE;
    //}
}



LRESULT CTransmitMsg::OnClickFlexListCtrl(WPARAM WParam, LPARAM LParam)
{
    int nRow = (int)WParam;
    int nColumn = (int)LParam;
    m_nSelDataColIndex = nColumn;
    return TRUE;
}


void CTransmitMsg::OnDestroy()
{
    // CFormView::OnDestroy();

    // TODO: Add your message handler code here
}
/*******************************************************************************
  Function Name  : OnBtnClose
  Input(s)       : -
  Output         : -
  Functionality  : This button handler will be called during Close button press.
                   This function will send WM_CLOSE message to close Tx Child
                   window.
  Member of      : CTransmitMsg
  Author(s)      : Ashwin R Uchil
  Date Created   : 16.4.2013
  Modifications  :
*******************************************************************************/
void CTransmitMsg::OnBtnClose()
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



HRESULT CTransmitMsg::UpdateTxView(int& m_nChannelsConfigured)
{
    int nChannel = m_wndComboChannel.GetCurSel();

    CButton* omBtnWindow = (CButton*)GetDlgItem(IDC_RADIO_BYTE_VIEW);
    if ( NULL != omBtnWindow )
    {
        if ( CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_bHexMode == false)
        {
            omBtnWindow->SetWindowText("Byte View (DEC)");
        }
        else
        {
            omBtnWindow->SetWindowText("Byte View (HEX)");
        }
    }




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
    m_lstMsg.DeleteAllItems();




    m_bDataModified = false;
    std::list<FLEXRAY_FRAME_DATA>& ouFrameDataList = CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFlexray_Frame_Data[nChannel];
    std::list<FLEXRAY_FRAME_DATA>::iterator itFrameData = ouFrameDataList.begin();
    int nRows =0;
    for(; itFrameData != ouFrameDataList.end(); itFrameData++)
    {
        m_lstMsg.InsertItem(nRows, itFrameData->m_ouFlexFrame.m_strFrameName.c_str());

        std::string strText;


        vGetStringFromValue(itFrameData->m_ouFlexFrame.m_nSlotId, strText);
        m_lstMsg.SetItemText(nRows, def_SLOTID_COLUMN_INDEX, strText.c_str());


        vGetStringFromValue(itFrameData->m_ouFlexFrame.m_nBaseCycle, strText);
        m_lstMsg.SetItemText(nRows, def_BASECYCLE_COLUMN_INDEX, strText.c_str());


        vGetStringFromValue(itFrameData->m_ouFlexFrame.m_nReptition, strText);
        m_lstMsg.SetItemText(nRows, def_REPETITION_COLUMN_INDEX, strText.c_str());

        CString strString;
        if ( itFrameData->m_ouFlexFrame.m_ouChannel == CHANNEL_A )
        {
            strString.Format("%s", "A" );
        }
        else if ( itFrameData->m_ouFlexFrame.m_ouChannel == CHANNEL_B )
        {
            strString.Format("%s", "B" );
        }
        else if ( itFrameData->m_ouFlexFrame.m_ouChannel == CHANNEL_AB )
        {
            strString.Format("%s", "AB" );
        }
        m_lstMsg.SetItemText(nRows, def_CHANNEL_COLUMN_INDEX, strString);


        vGetStringFromValue(itFrameData->m_ouFlexFrame.m_nLength, strText);
        m_lstMsg.SetItemText(nRows, def_DLC_COLUMN_INDEX, strText.c_str());

        //Has to be moved to function
        if (itFrameData->m_ouFlexFrame.m_eSlotType == STATIC )
        {
            strText = "static";
        }
        if (itFrameData->m_ouFlexFrame.m_eSlotType == DYNAMIC )
        {
            strText = "dynamic";
        }

        m_lstMsg.SetItemText(nRows, def_SLOTTYPE_COLUMN_INDEX, strText.c_str());

        m_lstMsg.SetCheck( nRows, itFrameData->bSelected);
        nRows++;
    }

    vChangeDelButtonStatus();
    return S_OK;
}
void CTransmitMsg::OnNMTransmitMessagesRightClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    OnNMTransmitMessagesClick(pNMHDR, pResult);
    return;
}

void CTransmitMsg::OnNMTransmitMessagesClick(NMHDR* pNMHDR, LRESULT* pResult)
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
        GetDlgItem(IDC_BT_UPDATE)->EnableWindow(FALSE);
        m_ouCurrentView = BYTE_VIEW;

        ((CButton*)GetDlgItem(IDC_RADIO_BYTE_VIEW))->SetCheck(TRUE);
        ((CButton*)GetDlgItem(IDC_RADIO_SIGNAL_VIEW))->SetCheck(FALSE);
        *pResult = 1;
        m_bDataModified = false;
        SwitchView(m_ouCurrentView);
        return;
    }
    //Selection Change
    else if(hitinfo.flags == LVHT_ONITEMSTATEICON )
    {
        this->EnableWindow(FALSE);
        m_bDataModified = false;
        // They clicked on one that is not selected... just change it
        // ... do something here
        bChecked = m_lstMsg.GetCheck(nItem);
        bChecked = !bChecked;
        m_lstMsg.SetCheck(nItem,bChecked);

        //To change selection state;

        std::list<FLEXRAY_FRAME_DATA>& ouFrameDataList = CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFlexray_Frame_Data[nChannel];
        std::list<FLEXRAY_FRAME_DATA>::iterator itFrameData = ouFrameDataList.begin();
        advance(itFrameData, nItem);
        if ( itFrameData != ouFrameDataList.end())
        {
            itFrameData->bSelected = bChecked;
        }
        char chText[256];
        char chTextBase[256];
        m_lstMsg.GetItemText(nItem, 1, chText, 256);
        m_lstMsg.GetItemText(nItem, 2, chTextBase, 256);

        int nSelectedId = -1;
        int nSelBaseCycle = -1;
        if ( CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_bHexMode == true )
        {
            nSelectedId = strtoul(chText, NULL, 16);
            nSelBaseCycle = strtoul(chTextBase, NULL, 16);
        }
        else
        {
            nSelectedId = strtoul(chText, NULL, 10);
            nSelBaseCycle = strtoul(chTextBase, NULL, 10);
        }



        int nRowCount = m_lstMsg.GetItemCount();
        for ( int i = 0 ; i < nRowCount; i++ )
        {
            m_lstMsg.GetItemText(i, 1, chText, 256);
            m_lstMsg.GetItemText(i, 2, chTextBase, 256);
            int nCurrentId = -1;
            int nCurrentBaseCycle = -1;
            if ( CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_bHexMode == true )
            {
                nCurrentId = strtoul(chText, NULL, 16);
                nCurrentBaseCycle = strtoul(chTextBase, NULL, 16);
            }
            else
            {
                nCurrentId = strtoul(chText, NULL, 10);
                nCurrentBaseCycle = strtoul(chTextBase, NULL, 10);
            }

            if ( nCurrentId == nSelectedId && nCurrentBaseCycle == nSelBaseCycle && nItem != i )
            {
                m_lstMsg.SetCheck(i,false);
                std::list<FLEXRAY_FRAME_DATA>::iterator itFrameDataTemp = ouFrameDataList.begin();
                advance(itFrameDataTemp, i);
                if ( itFrameDataTemp != ouFrameDataList.end())
                {
                    itFrameDataTemp->bSelected = false;
                }

            }
        }
        m_lstMsg.Invalidate();
        m_lstMsg.SetRedraw();




        CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().UpdateMessagetoDIL(*itFrameData, !bChecked);

        *pResult = 1;

        this->EnableWindow(TRUE);

        return;
    }
}

void CTransmitMsg::OnRadioByteView()
{
    //No need todo anything
    if ( m_ouCurrentView == BYTE_VIEW )
    {
        return;
    }

    m_ouCurrentView = BYTE_VIEW;
    SwitchView(m_ouCurrentView);
}
void CTransmitMsg::OnRadioSignalView()
{
    //No need todo anything
    if ( m_ouCurrentView == SIGNAL_VIEW )
    {
        return;
    }

    m_ouCurrentView = SIGNAL_VIEW;
    SwitchView(m_ouCurrentView);
}
void CTransmitMsg::SwitchView(DATA_VIEW ouViewType)
{
    int nItem = m_lstMsg.GetSelectionMark();
    if ( m_lstMsg.GetItemCount() == 0)
    {
        InitFlexList(ouViewType);
    }
    else if ( nItem == -1 )
    {
        InitFlexList(ouViewType);
        return;
    }

    else
    {
        std::list<FLEXRAY_FRAME_DATA>::iterator itrFrame;
        int nChannel = m_wndComboChannel.GetCurSel();
        if ( nChannel < 0 || nChannel > CHANNEL_ALLOWED )
        {
            return ;
        }
        if ( CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFlexray_Frame_Data[nChannel].size() > 0 )
        {
            itrFrame = CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFlexray_Frame_Data[nChannel].begin();
            advance(itrFrame, nItem);
            if ( itrFrame == CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_ouFlexray_Frame_Data[nChannel].end() )
            {
                return;
            }
        }
        else
        {
            return;
        }
        if ( m_bDataModified == false )
        {
            m_ouCurrentMsg = *itrFrame;
        }
        if ( ouViewType == BYTE_VIEW )
        {
            vStoreSignalValues();
            InitFlexList(ouViewType);
            //char acDataBytes[2048] = {'\0'};
            for (DWORD j = 0, k = 0; j < m_ouCurrentMsg.m_ouFlexFrame.m_nLength; j++)
            {
                unsigned char ucCurrData = m_ouCurrentMsg.m_ouData[j];
                //sprintf(&(acDataBytes[k]), "%04X ", ucCurrData);
                k += 5;

                //setting data to the signal list
                int nRow = (j / 8);         //row number is current (WORD number / 8) , since there r 8 colums
                int nCol = (j % 8) + 1;     //since there r 8 columns

                CString     omstrData;
                SLISTINFO       sListInfo;
                SNUMERICINFO    sNumInfo;
                sListInfo.m_eType = eKeyBuddy;
                sNumInfo.m_uMaxVal.m_n64Value = 255;
                sNumInfo.m_uMinVal.m_n64Value = 0;
                sNumInfo.m_byFlag = SIGNED_VAL;
                if (  CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_bHexMode == true )
                {
                    sNumInfo.m_nTextLimit = 2;


                    sNumInfo.m_byBase = BASE_HEXADECIMAL;
                }
                else
                {
                    sNumInfo.m_nTextLimit = 3;
                    sNumInfo.m_byBase = BASE_DECIMAL;
                }


                m_oFlexListCtrl.vSetColumnInfo(nRow, nCol, sListInfo);
                m_oFlexListCtrl.vSetNumericInfo(nRow, nCol,sNumInfo);

                CString strString;
                strString.Format("%03d", j);
                if(nCol == 1)       //Insert the row only once after writing 8 columns, else a row will b inserted 8 times
                {
                    m_oFlexListCtrl.InsertItem(j, strString);
                }


                if ( CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_bHexMode == false)
                {
                    strString.Format("%03d", ucCurrData);
                }
                else
                {
                    strString.Format("%02X", ucCurrData);
                }
                m_oFlexListCtrl.SetItemText(nRow,nCol,strString);
            }
        }
        else
        {
            vStoreDataBytes();
            InitFlexList(ouViewType);
            std::list<Flexray_SSIGNALINFO> ouSignalInfoList;
            bGetSignalInfo(m_ouCurrentMsg.m_ouFlexFrame, m_ouCurrentMsg.m_ouData, m_ouCurrentMsg.m_ouFlexFrame.m_nLength, ouSignalInfoList);

            std::list<Flexray_SSIGNALINFO>::iterator itrSig = ouSignalInfoList.begin();
            int i = 0;
            for (; itrSig != ouSignalInfoList.end(); itrSig++ )
            {
                m_oFlexListCtrl.InsertItem(i, itrSig->m_omSigName.c_str());


                m_oFlexListCtrl.SetItemText(i, 1, itrSig->m_omRawValue.c_str());
                m_oFlexListCtrl.SetItemText(i, 2, itrSig->m_omEnggValue.c_str());
                m_oFlexListCtrl.SetItemText(i, 3, itrSig->m_omUnit.c_str());

                SLISTINFO sListInfo;
                sListInfo.m_eType = eNoControl;
                m_oFlexListCtrl.vSetColumnInfo(i, 0, sListInfo);

                sListInfo.m_eType = eNoControl;
                m_oFlexListCtrl.vSetColumnInfo(i, 1, sListInfo);

                sListInfo.m_eType = eNoControl;
                m_oFlexListCtrl.vSetColumnInfo(i, 2, sListInfo);

                sListInfo.m_eType = eNoControl;
                m_oFlexListCtrl.vSetColumnInfo(i, 3, sListInfo);


                i++;
            }
        }
    }
}

void CTransmitMsg::vStoreDataBytes()
{
    int nRows = m_oFlexListCtrl.GetItemCount();
    int nCols = m_oFlexListCtrl.GetHeaderCtrl()->GetItemCount();
    char chText[MAX_PATH];
    int n= 0;
    for ( int i = 0; i < nRows; i++ )
    {
        for ( int j = 1; j < nCols; j++ )
        {
            m_oFlexListCtrl.GetItemText(i, j, chText, MAX_PATH );
            if (  CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_bHexMode == true )
            {
                m_ouCurrentMsg.m_ouData[n] = strtoul(chText, NULL, 16);
            }
            else
            {
                m_ouCurrentMsg.m_ouData[n] = strtoul(chText, NULL, 10);
            }

            n++;
        }
    }
}

void CTransmitMsg::vStoreSignalValues()
{
    //TODO::
    //vUpdateSignalValues(m_ouCurrentMsg.m_ouFlexFrame, m_ouCurrentMsg.m_ouData);
}
