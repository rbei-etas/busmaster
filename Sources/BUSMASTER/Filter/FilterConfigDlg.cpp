/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      FilterConfigDlg.cpp
 * \brief     Implementation file for CFilterConfigDlg class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 * \req       The Requirement Specification defines no tags.
 *
 * Implementation file for CFilterConfigDlg class
 */


#include "Filter_stdafx.h"

/* C++ includes */
#include <sstream>
#include <string>
//#include "../Application/GettextBusmaster.h"
/* Project includes */
#include "Filter_resource.h"
#include "CANDefines.h"
#include "FilterConfigDlg.h"        // For Filter Configuration Dialog
#include "Utility\MultiLanguageSupport.h"

#define defNAMED_FILTER_LIST_COLUMNS            2
#define defFILTER_DETAILS_LIST_COLUMNS          7


UINT unGetMsgIDFromName(CString omMsgName)
{
    CString omStrMsgID;
    UINT unMsgID = (UINT)-1;
    CHAR* pcStopStr = nullptr;
    int nIndex = omMsgName.Find(defMSGID_EXTENDED);
    if(nIndex != -1)
    {
        int nLength = omMsgName.GetLength();
        omStrMsgID = omMsgName.Mid(nIndex+1,nLength-1);
        unMsgID = strtol((LPCTSTR )omStrMsgID,&pcStopStr,16);
    }
    return unMsgID;
}

UINT unGetSlotIDFromName(CString omMsgName)
{
    CString omStrMsgID;
    UINT unMsgID = (UINT)-1;
    CHAR* pcStopStr = nullptr;
    int nIndexDash = omMsgName.ReverseFind('-');
    int nIndexBracket = omMsgName.Find(defMSGID_NAME_DELIMITER);
    if(nIndexDash != -1 || nIndexBracket != -1)
    {
        omStrMsgID = omMsgName.Mid(nIndexDash+2,nIndexBracket-nIndexDash -2);// a '-' and a space hence nIndexDash+2
        unMsgID = _ttoi(omStrMsgID);
        //unMsgID = strtol((LPCTSTR )omStrMsgID,&pcStopStr,16);
    }
    return unMsgID;
}
/**
 * Standard default constructor
 */
CFilterConfigDlg::CFilterConfigDlg( SFILTERAPPLIED_CAN* psSrcList,
                                    IBMNetWorkGetService* pMsgDBDetails,
                                    UINT nHardware,
                                    CWnd* pParent /*=nullptr*/):
    CDialog(CFilterConfigDlg::IDD, pParent),
    m_pClusterConfig( pMsgDBDetails ),
    m_eCurrBUS(CAN)

{

    //{{AFX_DATA_INIT(CFilterConfigDlg)
    //}}AFX_DATA_INIT

    m_bDisableFilterCompUpdate = FALSE;
    m_bUpdating = FALSE;

    m_nSelecetedNamedFilterIndex = -1;
    m_omStrSelectedFilterNameBeforeEdit = "";

    m_nSelecetedFilterIndex = -1;

    m_psFilterApplied = psSrcList;
    m_nHardware = nHardware;
}

/**
 * Standard default constructor
 */
CFilterConfigDlg::CFilterConfigDlg( SFILTERAPPLIED_LIN* psSrcList,
                                    IBMNetWorkGetService* pClusterConfig,
                                    UINT nHardware,
                                    CWnd* pParent /*=nullptr*/):
    CDialog(CFilterConfigDlg::IDD, pParent),
    m_pClusterConfig(pClusterConfig),
    m_eCurrBUS(LIN)

{

    //{{AFX_DATA_INIT(CFilterConfigDlg)
    //}}AFX_DATA_INIT

    m_bDisableFilterCompUpdate = FALSE;
    m_bUpdating = FALSE;

    m_nSelecetedNamedFilterIndex = -1;
    m_omStrSelectedFilterNameBeforeEdit = "";

    m_nSelecetedFilterIndex = -1;

    m_psFilterAppliedLin = psSrcList;
    m_nHardware = nHardware;
}


/**
 * \param[in] pDX Pointer to data exchange object
 *
 * This will map UI controls will mapped data members. This will
 * be called during UpdateData funtion call for data update
 */
void CFilterConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFilterConfigDlg)
    DDX_Control(pDX, IDC_BTN_ADD_FILTER_TO_LIST, m_omAddFilter);
    DDX_Control(pDX, IDC_STAT_STATUS, m_omStatusText);
    DDX_Control(pDX, IDC_EDIT_RANGE_TO, m_omMsgIDRangeTo);
    DDX_Control(pDX, IDC_EDIT_RANGE_FROM, m_omMsgIDRangeFrom);
    DDX_Control(pDX, IDC_RADIO_ID, m_omRadioID);
    DDX_Control(pDX, IDC_COMB_MSG_TYPE, m_omMsgType);
    DDX_Control(pDX, IDC_COMB_MSG_ID_TYPE, m_omMsgIDType);
    DDX_Control(pDX, IDC_COMB_MSG_DIRECTION, m_omMsgDirection);
    DDX_Control(pDX, IDC_COMB_MSG_CHANNEL, m_omMsgChannel);
    DDX_Control(pDX, IDC_COMB_MSG_ID_FROM, m_omMsgIDFrom);
    DDX_Control(pDX, IDC_LSTC_FILTER_DETAILS, m_omLstcFilterDetails);
    DDX_Control(pDX, IDC_LSTC_FILTER_NAMES, m_omLstcFilterList);
    DDX_Control(pDX, IDC_RADIO_EVENT, m_omRadioEvent);
    DDX_Control(pDX, IDC_COMB_EVENT, m_omEventType);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilterConfigDlg, CDialog)
    //{{AFX_MSG_MAP(CFilterConfigDlg)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_FILTER_NAMES, OnItemchangedLstcFilterNames)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_FILTER_DETAILS, OnItemchangedLstcFilterDetails)
    ON_CBN_SELCHANGE(IDC_COMB_MSG_ID_FROM, OnSelchangeCombMsgIdFrom)
    ON_CBN_EDITCHANGE(IDC_COMB_MSG_ID_FROM, OnEditChangeMsgIDCombo)
    ON_BN_CLICKED(IDC_RADIO_ID, OnRadioMessageId)
    ON_BN_CLICKED(IDC_RADIO_EVENT, OnRadioEvent)
    ON_BN_CLICKED(IDC_RADIO_RANGE, OnRadioRange)
    ON_EN_UPDATE(IDC_EDIT_RANGE_FROM, OnUpdateEditRange)
    ON_CBN_SELCHANGE(IDC_COMB_MSG_ID_TYPE, OnSelchangeFilterComponentCombo)
    ON_BN_CLICKED(IDC_BTN_ADD_FILTER_TO_LIST, OnBtnAddFilterToList)
    ON_BN_CLICKED(IDC_BTN_DELETE_FILTER, OnBtnDeleteFilter)
    ON_BN_CLICKED(IDC_BTN_DELETE_ALL_FILTER, OnBtnDeleteAllFilter)
    ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LSTC_FILTER_NAMES, OnBeginlabeleditLstcFilterNames)
    ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
    ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
    ON_EN_UPDATE(IDC_EDIT_RANGE_TO, OnUpdateEditRange)
    ON_CBN_SELCHANGE(IDC_COMB_MSG_TYPE, OnSelchangeFilterComponentCombo)
    ON_CBN_SELCHANGE(IDC_COMB_MSG_DIRECTION, OnSelchangeFilterComponentCombo)
    ON_CBN_SELCHANGE(IDC_COMB_MSG_CHANNEL, OnSelchangeFilterComponentCombo)
    ON_CBN_SELCHANGE(IDC_COMB_EVENT, OnSelchangeFilterComponentCombo)
    ON_BN_CLICKED(ID_OK, OnOkPress)
    //}}AFX_MSG_MAP

    ON_BN_CLICKED(IDC_RADIO_NON_DB_MSG, OnBnClickedRadioNonDBMsg)


    // ON_CBN_SELCHANGE(IDC_COMBO_CYCLEREP, OnUpdateEditRange)

END_MESSAGE_MAP()

/**
 * \return FALSE - If focus is set to Any UI control explicitly
 *
 * Initialises dialog's UI components
 */
BOOL CFilterConfigDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    if(m_eCurrBUS == CAN)
    {
        SetWindowText("Configure Filter List - CAN");
    }
    else if(m_eCurrBUS == LIN)
    {
        SetWindowText("Configure Filter List - LIN");
    }


    // Create Image List first
    bCreateImageList();
    // Create Named Filter List
    m_omLstcFilterList.vSetSingleClickEnabled(FALSE);
    bCreateNamedFilterList();
    // Create Filter Details List
    bCreateFilterDetailsList();
    // Create Filter components
    bCreateFilterComps();
    // Initialise Filter Components
    vInitFilterComps();
    // Update List Controls with data
    bPopulateNamedFilterList();
    // Update Button Status
    vEnableDisableButtons();
    // Disable ADD button if there is no element in Filter List
    if( m_omLstcFilterList.GetItemCount() == 0 )
    {
        m_omAddFilter.EnableWindow( FALSE );
        CWnd* pWnd = GetDlgItem( IDC_STATIC_TO_STRING );
        if( pWnd != nullptr )
        {
            pWnd->EnableWindow( FALSE );
        }
    }

    if(m_eCurrBUS == CAN)
    {
        m_omMsgType.ShowWindow(TRUE);
        m_omMsgIDType.ShowWindow(TRUE);

        CButton* pWnd = (CButton*)GetDlgItem(IDC_STATIC_ID_TYPE);
        if( pWnd != nullptr )
        {
            pWnd->ShowWindow(TRUE);
        }
        pWnd = (CButton*)GetDlgItem(IDC_STATIC_FRAME_TYPE);
        if( pWnd != nullptr )
        {
            pWnd->ShowWindow(TRUE);
        }

        //
        /*m_omStMsgType.ShowWindow(TRUE);
        m_omStFrameType.ShowWindow(TRUE);*/
    }
    else if(m_eCurrBUS == LIN)
    {
        m_omMsgType.ShowWindow(FALSE);
        m_omMsgIDType.ShowWindow(FALSE);
        CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_ID_TYPE);
        if( pWnd != nullptr )
        {
            pWnd->ShowWindow(FALSE);
        }
        pWnd = (CStatic*)GetDlgItem(IDC_STATIC_FRAME_TYPE);
        if( pWnd != nullptr )
        {
            pWnd->ShowWindow(FALSE);
        }
    }


    return FALSE;
}

/**
 * \return Result of the image list creation
 *
 * Creates image list used in list controls
 */
BOOL CFilterConfigDlg::bCreateImageList()
{
    // Flag to indicate result
    BOOL bReturn = FALSE;
    // Create Image List
    if( m_omImagelist.Create( IDB_BMP_FILTER, 16,
                              1, defCOLOR_WHITE ) == TRUE )
    {
        // If successful set the result
        bReturn = TRUE;
    }
    // Return the result
    return bReturn;
}

/**
 * Creates Named filter list control
 */
BOOL CFilterConfigDlg::bCreateNamedFilterList()
{
    BOOL bReturn = TRUE;
    CHAR caColumnName[][defSIGNAL_LIST_STRING_MAX_LENGTH]
        = { defSTR_FILTER_NAME,
            defSTR_FILTER_TYPE
          };
    INT nColumnFormat[]     = { LVCFMT_LEFT,
                                LVCFMT_LEFT
                              };
    RECT rListCtrlRect;
    INT nTotalColunmSize = 0;
    INT nTotalStrLengthPixel = 0;

    // Set Image List if list is valid
    if( m_omImagelist.m_hImageList != nullptr )
    {
        m_omLstcFilterList.SetImageList(&m_omImagelist, LVSIL_SMALL);
    }

    //Calculate the total size of all column header
    m_omLstcFilterList.GetWindowRect( &rListCtrlRect);
    nTotalColunmSize     = rListCtrlRect.right - rListCtrlRect.left;
    nTotalStrLengthPixel = 0;

    int nColSize[ defNAMED_FILTER_LIST_COLUMNS ];
    nColSize[0] = static_cast<int>(nTotalColunmSize / 2.0);
    nColSize[1] = static_cast<int>(nTotalColunmSize / 2.0);
    //Insert each column name after calculating the size for the same.
    for(int i = 0; i < defNAMED_FILTER_LIST_COLUMNS; i++ )
    {
        m_omLstcFilterList.InsertColumn(i, _(caColumnName[i]),
                                        nColumnFormat[i], nColSize[i]);
    }

    // Set the extended style
    m_omLstcFilterList.SetExtendedStyle( LVS_EX_GRIDLINES |
                                         LVS_EX_FULLROWSELECT );

    // Return the result
    return bReturn;
}

/**
 * Updates filter details list
 */
BOOL CFilterConfigDlg::bCreateFilterDetailsList()
{
    BOOL bReturn = TRUE;
    CHAR caColumnName[][defSIGNAL_LIST_STRING_MAX_LENGTH]
        = { defSTR_FILTER_DETAILS_ID_FROM,
            defSTR_FILTER_DETAILS_ID_TO,
            defSTR_FILTER_DETAILS_ID_TYPE,
            defSTR_FILTER_DETAILS_MSG_TYPE,
            defSTR_FILTER_DETAILS_MSG_DIRECTION,
            defSTR_FILTER_DETAILS_MSG_CHANNEL,
            defSTR_FILTER_DETAILS_MSG_CHANNEL_MASK
          };
    INT nColumnFormat[]     = { LVCFMT_LEFT,
                                LVCFMT_LEFT,
                                LVCFMT_CENTER,
                                LVCFMT_CENTER,
                                LVCFMT_CENTER,
                                LVCFMT_CENTER,
                                LVCFMT_CENTER
                              };
    RECT rListCtrlRect;
    INT nTotalColunmSize = 0;
    INT nTotalStrLengthPixel = 0;

    // Set Image List if list is valid
    if( m_omImagelist.m_hImageList != nullptr )
    {
        m_omLstcFilterDetails.SetImageList(&m_omImagelist, LVSIL_SMALL);
    }

    //Calculate the total size of all column header
    m_omLstcFilterDetails.GetWindowRect( &rListCtrlRect);
    nTotalColunmSize     = rListCtrlRect.right - rListCtrlRect.left;
    nTotalStrLengthPixel = 0;

    int nColSize[ defFILTER_DETAILS_LIST_COLUMNS ]= {0};
    if(m_eCurrBUS == LIN )
    {
        nColSize[0] = static_cast<int>(nTotalColunmSize * 0.50 );
        nColSize[1] = static_cast<int>(nTotalColunmSize * 0.10 );

        nColSize[2] = static_cast<int>(nTotalColunmSize * 0 );
        nColSize[3] = static_cast<int>(nTotalColunmSize * 0 );

        nColSize[4] = static_cast<int>(nTotalColunmSize * 0.20 );
        nColSize[5] = static_cast<int>(nTotalColunmSize * 0.20 );

        nColSize[6] = static_cast<int>(nTotalColunmSize * 0 );
    }
    else if(m_eCurrBUS == CAN)
    {
        nColSize[0] = static_cast<int>(nTotalColunmSize * 0.25 );
        nColSize[1] = static_cast<int>(nTotalColunmSize * 0.10 );

        nColSize[2] = static_cast<int>(nTotalColunmSize * 0.15 );
        nColSize[3] = static_cast<int>(nTotalColunmSize * 0.15 );

        nColSize[4] = static_cast<int>(nTotalColunmSize * 0.15 );
        nColSize[5] = static_cast<int>(nTotalColunmSize * 0.15 );

        nColSize[6] = static_cast<int>(nTotalColunmSize * 0 );

    }


    //Insert each column name after calculating the size for the same.
    for( int nIndex = 0; nIndex < defFILTER_DETAILS_LIST_COLUMNS; nIndex++ )
    {
        m_omLstcFilterDetails.InsertColumn( nIndex,
                                            _(caColumnName[nIndex]),
                                            nColumnFormat[nIndex],
                                            nColSize[nIndex] );
        std::string strType = _(caColumnName[nIndex]);
        if(m_eCurrBUS == LIN)
        {
            if(strType == defSTR_FILTER_DETAILS_ID_TYPE)
            {
                m_omLstcFilterDetails.SetColumnWidth(nIndex, 0);
            }
            else if(strType == defSTR_FILTER_DETAILS_MSG_TYPE)
            {
                m_omLstcFilterDetails.SetColumnWidth(nIndex, 0);
            }
            else if(strType ==defSTR_FILTER_DETAILS_MSG_CHANNEL_MASK)
            {
                m_omLstcFilterDetails.SetColumnWidth(nIndex, 0);
            }
        }
        else if( m_eCurrBUS == CAN )
        {
            if(strType ==defSTR_FILTER_DETAILS_MSG_CHANNEL_MASK)
            {
                m_omLstcFilterDetails.SetColumnWidth(nIndex, 0);
            }
        }

    }

    // Set the extended style
    m_omLstcFilterDetails.SetExtendedStyle( LVS_EX_FULLROWSELECT);

    return bReturn;
}

/**
 * Creates Filter components
 */
BOOL CFilterConfigDlg::bCreateFilterComps()
{
    // Set Message ID Type
    m_omMsgIDType.ResetContent();
    m_omMsgIDType.AddString( _(defSTR_MSG_ID_TYPE_STD) );
    m_omMsgIDType.AddString( _(defSTR_MSG_ID_TYPE_EXTD) );
    m_omMsgIDType.AddString( _(defSTR_SELECTION_ALL) );

    // Message Type
    m_omMsgType.ResetContent();
    m_omMsgType.AddString( _(defSTR_MSG_TYPE_NON_RTR) );
    m_omMsgType.AddString( _(defSTR_MSG_TYPE_RTR) );
    m_omMsgType.AddString( _(defSTR_SELECTION_ALL) );

    // Direction
    m_omMsgDirection.ResetContent();
    m_omMsgDirection.AddString( _(defSTR_MSG_DIR_RX) );
    m_omMsgDirection.AddString( _(defSTR_MSG_DIR_TX) );
    m_omMsgDirection.AddString( _(defSTR_SELECTION_ALL) );

    if(m_eCurrBUS == CAN)
    {
        m_omEventType.ResetContent();

        m_omEventType.AddString(_(defFILTER_ERR_FRAME));
        m_omEventType.SetCurSel(0);
    }
    else if(m_eCurrBUS == LIN)
    {
        m_omEventType.ResetContent();

        m_omEventType.AddString(_(defFILTER_CHECKSUM_EVNT_LIN));
        m_omEventType.AddString(_(defFILTER_RX_FRAME_EVNT_LIN));
        m_omEventType.AddString(_(defFILTER_SLAVE_NO_RESP_EVNT_LIN));
        m_omEventType.AddString(_(defFILTER_SYNC_EVNT_LIN));
        m_omEventType.SetCurSel(0);
    }

    // Channels
    m_omMsgChannel.ResetContent();

    if(m_eCurrBUS == CAN)
    {
        // Add All Channels as first item
        m_omMsgChannel.AddString(_(defSTR_SELECTION_ALL));
        for (UINT Index = 1; Index <= m_nHardware; Index++)
        {
            CString omStr;
            // Format Channel ID String Say 1,2,...
            omStr.Format(defFORMAT_MSGID_DECIMAL, Index);
            m_omMsgChannel.AddString(omStr);
        }
    }
    else if(m_eCurrBUS == LIN )
    {
        // Add All Channels as first item
        m_omMsgChannel.AddString(_(defSTR_SELECTION_ALL));
        int unChannels = 0;
        m_pClusterConfig->GetChannelCount(LIN, unChannels);
        for (UINT nIndex = 0; nIndex < unChannels; nIndex++)
        {
            CString omStr;
            // Format Channel ID String Say 1,2,...
            omStr.Format(defFORMAT_MSGID_DECIMAL, nIndex+1);
            m_omMsgChannel.AddString(omStr);
        }
        m_omMsgIDType.ResetContent();
        m_omMsgType.ResetContent();
        m_omMsgIDType.EnableWindow(FALSE);
        m_omMsgType.EnableWindow(FALSE);
    }

    return TRUE;
}

/**
 * Populates database message combobox with DB messaged from
 * database module
 * \req      RS_FLX_06
 * \design   DS_FLX_06
 * \codetag  CT_FLX_06
 */
void CFilterConfigDlg::vPopulateDBMessages()
{
    m_omMsgIDFrom.ResetContent();
    m_omMsgIDFrom.SetWindowText("0");
    if ( m_eCurrBUS == LIN || m_eCurrBUS == CAN )
    {
        // Get number of mesages in database
        if ( m_pClusterConfig != nullptr )
        {
            char omStrMsgId[MAX_PATH] = "";
            //theApp.m_pouMsgSignal->unListGetMessageIDs( pIDArray );
            unsigned int unId;
            sMESSAGE* pMessage = nullptr;

            int nChannels = 0;
            m_pClusterConfig->GetChannelCount( m_eCurrBUS, nChannels );
            ICluster* pCluster;
            for(INT nIndex = 0; nIndex < nChannels; nIndex++)
            {
                m_pClusterConfig->GetDBService( m_eCurrBUS, nIndex, 0, &pCluster ); //   m_ouFlexChannelConfig[nIndex].m_ouClusterInfo.GetFrames(lstFrameStrct);
                if ( nullptr == pCluster)
                {
                    continue;
                }

                std::list<IFrame*> itrFrameList;
                m_pClusterConfig->GetFrameList( m_eCurrBUS, nIndex, itrFrameList );
                std::string strName;

for ( auto itrFrame : itrFrameList)
                {
                    itrFrame->GetName(strName);
                    itrFrame->GetFrameId(unId);

                    //sprintf_s(omStrMsgId, MAX_PATH, defSTR_MSG_ID_IN_HEX, itrFrame->m_nSlotId);

                    sprintf_s(omStrMsgId, MAX_PATH, "[0x%x]%s", unId, strName.c_str());
                    //sprintf(omStrMsgKey, "%s", omStrMsgId+omStrMsgName);
                    //omStrMsgId = omStrMsgId + omStrMsgName;
                    m_omMsgIDFrom.AddString(omStrMsgId);
                }
            }

        }
    }

    vAdjustWidthMessageComboBox();
}

/**
 * Populates named filter list with the list of filters from
 * the input filter list
 */
BOOL CFilterConfigDlg::bPopulateNamedFilterList()
{
    if(m_eCurrBUS == CAN)
    {
        // Get the details from Filter List
        int nSize = m_psFilterApplied->m_ushTotal;
        // Clear existing List
        m_omLstcFilterList.DeleteAllItems();
        // Disable UI Update
        m_bUpdating = TRUE;
        // iterate through list
        for( int nIndex = 0; nIndex < nSize; nIndex++ )
        {
            // Pointers to retrieve data from map
            PSFILTERSET psFilterSet = m_psFilterApplied->m_psFilters + nIndex;
            // Filter Name
            CString omStrFilterName = "";
            if( psFilterSet != nullptr )
            {
                omStrFilterName.Format("%s", psFilterSet->m_sFilterName.m_acFilterName.c_str());
                // Insert the item
                // Filter Name
                m_omLstcFilterList.InsertItem( nIndex,
                                               omStrFilterName,
                                               defFILTER_IMAGE_INDEX_FILTER );
                // Update Filter Type
                m_omLstcFilterList.SetItemText( nIndex , 1,
                                                psFilterSet->m_sFilterName.m_bFilterType ?
                                                _(defSTR_FILTER_TYPE_PASS) :
                                                _(defSTR_FILTER_TYPE_STOP) );
                // Set edit property
                SLISTINFO sListInfo;
                // Init Signal Name column
                // Column 0 : User Function call
                sListInfo.m_eType = eText;
                m_omLstcFilterList.vSetColumnInfo( nIndex, 0, sListInfo);
                // Init Raw Value column
                // Column 1 : Numeric Edit with Spin Control
                sListInfo.m_eType = eComboItem;
                sListInfo.m_omEntries.Add( _(defSTR_FILTER_TYPE_STOP) );
                sListInfo.m_omEntries.Add( _(defSTR_FILTER_TYPE_PASS) );
                m_omLstcFilterList.vSetColumnInfo( nIndex, 1, sListInfo);
            }
        }
        m_bUpdating = FALSE;
        // Set the focus to the first item
        m_omLstcFilterList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED,
                                        LVIS_SELECTED | LVIS_FOCUSED );
    }
    else if(m_eCurrBUS == LIN)
    {
        // Get the details from Filter List
        int nSize = m_psFilterAppliedLin->m_ushTotal;
        // Clear existing List
        m_omLstcFilterList.DeleteAllItems();
        // Disable UI Update
        m_bUpdating = TRUE;
        // iterate through list
        for( int nIndex = 0; nIndex < nSize; nIndex++ )
        {
            // Pointers to retrieve data from map
            PSFILTERSET psFilterSet = m_psFilterAppliedLin->m_psFilters + nIndex;
            // Filter Name
            CString omStrFilterName = "";
            if( psFilterSet != nullptr )
            {
                omStrFilterName.Format("%s", psFilterSet->m_sFilterName.m_acFilterName.c_str());
                // Insert the item
                // Filter Name
                m_omLstcFilterList.InsertItem( nIndex,
                                               omStrFilterName,
                                               defFILTER_IMAGE_INDEX_FILTER );
                // Update Filter Type
                m_omLstcFilterList.SetItemText( nIndex , 1,
                                                psFilterSet->m_sFilterName.m_bFilterType ?
                                                _(defSTR_FILTER_TYPE_PASS) :
                                                _(defSTR_FILTER_TYPE_STOP) );
                // Set edit property
                SLISTINFO sListInfo;
                // Init Signal Name column
                // Column 0 : User Function call
                sListInfo.m_eType = eText;
                m_omLstcFilterList.vSetColumnInfo( nIndex, 0, sListInfo);
                // Init Raw Value column
                // Column 1 : Numeric Edit with Spin Control
                sListInfo.m_eType = eComboItem;
                sListInfo.m_omEntries.Add( _(defSTR_FILTER_TYPE_STOP) );
                sListInfo.m_omEntries.Add( _(defSTR_FILTER_TYPE_PASS) );
                m_omLstcFilterList.vSetColumnInfo( nIndex, 1, sListInfo);
            }
        }
        m_bUpdating = FALSE;
        // Set the focus to the first item
        m_omLstcFilterList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED,
                                        LVIS_SELECTED | LVIS_FOCUSED );
    }

    return TRUE;
}

/**
 * Sets the focus to the next control in the dialog
 */
void CFilterConfigDlg::OnOK()
{
    NextDlgCtrl();
}

/**
 * Updateds the filter details with the newly selected filter.
 * Updates filter name or type in case it is called because of
 * editing by the user.
 */
void CFilterConfigDlg::OnItemchangedLstcFilterNames( NMHDR* pNMHDR,
        LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // Change in filter name. Update Filter Details
    if(pNMListView->uChanged != LVIF_TEXT && m_bUpdating == FALSE )
    {
        // Selected & Focused
        if( pNMListView->uNewState == (LVIS_FOCUSED | LVIS_SELECTED) )
        {
            // Update selection
            m_nSelecetedNamedFilterIndex = pNMListView->iItem;
            // Update selected filter details
            vUpdateFilterDetails( pNMListView->iItem);
        }
        // Update Button Status
        vEnableDisableButtons();
    }
    // Notification for text change
    // User changed Filter Name or Filter Type.
    // Validate and Update corresponding data
    if( pNMListView->uChanged == LVIF_TEXT && m_bUpdating == FALSE)
    {
        LV_DISPINFO ls;
        // Get the change information from the list control
        if(m_omLstcFilterList.sGetModificationStructure(ls) == TRUE)
        {
            switch( ls.item.iSubItem)
            {
                    // Filter Name
                case 0:
                    vUpdateFromFilterName( ls.item.iItem,
                                           ls.item.iSubItem );
                    break;
                    // Filter Type
                case 1:
                    vUpdateFromFilterType( ls.item.iItem,
                                           ls.item.iSubItem );
                    break;
            }
        }
    }
    *pResult = 0;
}

/**
 * Updates filter components with the selected filter details
 */
void CFilterConfigDlg::OnItemchangedLstcFilterDetails( NMHDR* pNMHDR,
        LRESULT* pResult )
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // Update Filter Components if user selection is changed
    if(m_bUpdating == FALSE && pNMListView->uChanged != LVIF_TEXT )
    {
        // Selected & Focused
        if( pNMListView->uNewState == (LVIS_FOCUSED | LVIS_SELECTED) )
        {
            m_nSelecetedFilterIndex = pNMListView->iItem;
            // Update Filter Components
            vUpdateFilterComponents( pNMListView->iItem);
        }
        else
        {
            m_nSelecetedFilterIndex = -1;
        }

        // Update Button Details
        vEnableDisableButtons();
    }
    *pResult = 0;
}

/**
 * \param[in] nSelectedItemIndex List control index
 *
 * Updated filter components with the filter details poined by
 * the input.
 */
void CFilterConfigDlg::vUpdateFilterDetails( int nSelectedItemIndex )
{
    if(m_eCurrBUS == CAN)
    {
        // Update Filter Details
        CString omStrFilterName =
            m_omLstcFilterList.GetItemText( nSelectedItemIndex, 0 );
        const PSFILTERSET psSet = SFILTERSET::psGetFilterSetPointer
                                  (m_psFilterApplied->m_psFilters, m_psFilterApplied->m_ushTotal, omStrFilterName.GetBuffer(MAX_PATH));
        if( psSet != nullptr )
        {
            vUpdateFilterDetails( psSet );
        }
    }
    else if(m_eCurrBUS == LIN)
    {
        // Update Filter Details
        CString omStrFilterName =
            m_omLstcFilterList.GetItemText( nSelectedItemIndex, 0 );
        const PSFILTERSET psSet = SFILTERSET::psGetFilterSetPointer
                                  (m_psFilterAppliedLin->m_psFilters, m_psFilterAppliedLin->m_ushTotal, omStrFilterName.GetBuffer(MAX_PATH));
        if( psSet != nullptr )
        {
            vUpdateFilterDetails( psSet );
        }
    }
}

/**
 * \param[in] sFilter Filter details
 * \param[in] sFilterDisplyInfo formatted Filter details structure
 *
 * This function formats the filter details and fills the
 * sFilterDisplyInfo structure with the formatted strings.
 */
void CFilterConfigDlg::vFormatDisplayString(
    const SFILTER_CAN& sFilter,
    SFILTERDISPLAYINFO& sFilterDisplyInfo )
{
    // Get the database pointer from the application
    // Set the default image index
    sFilterDisplyInfo.m_nImageIndex = defFILTER_IMAGE_INDEX_NDB;
    // Clear ID From and To String
    sFilterDisplyInfo.m_omStrMsgIDFrom = "";
    sFilterDisplyInfo.m_omStrMsgIDTo = "";
    // If database is valid
    if ( sFilter.m_ucFilterType == defFILTER_TYPE_SINGLE_ID )
    {
        m_omMsgDirection.EnableWindow( TRUE );
        // Get the database name for ID From
        CString omStr = "";

        IFrame* ouFrameStrct;
        if ( S_OK == hGetFrameFromId( sFilter.m_dwMsgIDFrom, CAN, 0, nullptr, &ouFrameStrct ) )
        {
            std::string strName;
            ouFrameStrct->GetName( strName );
            omStr = strName.c_str();
        }

        if ( omStr != "" )
        {
            std::ostringstream oss;
            oss << std::hex << sFilter.m_dwMsgIDFrom;
            oss << " [" << omStr << "]";
            sFilterDisplyInfo.m_omStrMsgIDFrom = oss.str();
            // Update Image Index
            sFilterDisplyInfo.m_nImageIndex = defFILTER_IMAGE_INDEX_DB;
        }
        else
        {
            std::ostringstream oss;
            oss << std::hex << sFilter.m_dwMsgIDFrom;
            sFilterDisplyInfo.m_omStrMsgIDFrom = oss.str();
        }
    }
    else if ( sFilter.m_ucFilterType == defFILTER_TYPE_ID_RANGE )
    {
        m_omMsgDirection.EnableWindow( TRUE );
        // Range of messages
        // Format ID FRom
        std::ostringstream oss1;
        oss1 << std::hex << sFilter.m_dwMsgIDFrom;
        sFilterDisplyInfo.m_omStrMsgIDFrom = oss1.str();
        std::ostringstream oss2;
        oss2 << std::hex << sFilter.m_dwMsgIDTo;
        sFilterDisplyInfo.m_omStrMsgIDTo = oss2.str();
        // Update Image Index
        sFilterDisplyInfo.m_nImageIndex = defFILTER_IMAGE_INDEX_ID_RANGE;
    }
    else if ( sFilter.m_ucFilterType == defFILTER_TYPE_EVENT )
    {
        m_omMsgDirection.EnableWindow( FALSE );
        sFilterDisplyInfo.m_omStrMsgIDFrom = sFilter.m_omEventName;
        // Update Image Index
        sFilterDisplyInfo.m_nImageIndex = defFILTER_IMAGE_INDEX_DB;
    }
    else if ( sFilter.m_ucFilterType == defFILTER_TYPE_ECU )
    {
        sFilterDisplyInfo.m_nImageIndex = defFILTER_IMAGE_INDEX_ECU;
    }

    if ( sFilter.m_byIDType == TYPE_ID_CAN_EXTENDED )
    {
        sFilterDisplyInfo.m_omStrMsgIDType = _( defSTR_MSG_ID_TYPE_EXTD );
    }
    else if ( sFilter.m_byIDType == TYPE_ID_CAN_STANDARD )
    {
        sFilterDisplyInfo.m_omStrMsgIDType = _( defSTR_MSG_ID_STANDARD );
    }
    else if ( sFilter.m_byIDType == TYPE_ID_CAN_ALL )
    {
        sFilterDisplyInfo.m_omStrMsgIDType = _( defSTR_SELECTION_ALL );
    }
    else
    {
        sFilterDisplyInfo.m_omStrMsgIDType = "";
    }


    // Message Type
    if ( sFilter.m_byMsgType == TYPE_MSG_CAN_RTR )
    {
        sFilterDisplyInfo.m_omStrMsgType = _( defSTR_MSG_TYPE_RTR );
    }
    else if ( sFilter.m_byMsgType == TYPE_MSG_CAN_NON_RTR )
    {
        sFilterDisplyInfo.m_omStrMsgType = _( defSTR_MSG_TYPE_NON_RTR );
    }
    else if ( sFilter.m_byMsgType == TYPE_MSG_CAN_ALL )
    {
        sFilterDisplyInfo.m_omStrMsgType = _( defSTR_SELECTION_ALL );
    }
    else
    {
        sFilterDisplyInfo.m_omStrMsgType = "";
    }

    if ( sFilter.m_ucFilterType == defFILTER_TYPE_EVENT )
    {
        sFilterDisplyInfo.m_omStrMsgIDType = "";
        sFilterDisplyInfo.m_omStrMsgType = "";
    }


    // Message Direction
    switch ( sFilter.m_eDrctn )
    {
        case DIR_ALL: // All Message
            sFilterDisplyInfo.m_omStrMsgDirection = _( defSTR_SELECTION_ALL );
            break;
        case DIR_TX: // Tx Message
            sFilterDisplyInfo.m_omStrMsgDirection = _( defSTR_MSG_DIR_TX );
            break;
        case DIR_RX: // Rx
            sFilterDisplyInfo.m_omStrMsgDirection = _( defSTR_MSG_DIR_RX );
            break;
        default:
            ASSERT( FALSE );
            sFilterDisplyInfo.m_omStrMsgDirection = "";
            break;
    }

    // Channel Number
    if ( sFilter.m_eChannel == CAN_CHANNEL_ALL )
    {
        sFilterDisplyInfo.m_omStrMsgChannel = _( defSTR_SELECTION_ALL );
    }
    else
    {
        // Format Channel ID String Say 1,2,...
        std::ostringstream oss;
        oss << std::dec << sFilter.m_eChannel;
        sFilterDisplyInfo.m_omStrMsgChannel = oss.str();
    }
}

/**
 * \param[in] nIndex List control index
 * \param[in] sDisplayInfo Display information struct
 *
 * Updates the list control row pointed by nIndex with the
 * display format info with the input structure sDisplayInfo
 */
void CFilterConfigDlg::vUpdateFilterListDetails( int nIndex,
        const SFILTERDISPLAYINFO& sDisplayInfo )
{
    BOOL bOldUpdateValue = m_bUpdating;

    m_bUpdating = TRUE;
    // ID From
    m_omLstcFilterDetails.SetItemText( nIndex,
                                       0,
                                       sDisplayInfo.m_omStrMsgIDFrom.c_str());
    // ID To
    m_omLstcFilterDetails.SetItemText( nIndex,
                                       1,
                                       sDisplayInfo.m_omStrMsgIDTo.c_str());
    // ID Type
    m_omLstcFilterDetails.SetItemText( nIndex,
                                       2,
                                       sDisplayInfo.m_omStrMsgIDType.c_str());
    // Message Type
    m_omLstcFilterDetails.SetItemText( nIndex,
                                       3,
                                       sDisplayInfo.m_omStrMsgType.c_str());
    // Message Direction
    m_omLstcFilterDetails.SetItemText( nIndex,
                                       4,
                                       sDisplayInfo.m_omStrMsgDirection.c_str());
    // Channel Number
    m_omLstcFilterDetails.SetItemText( nIndex,
                                       5,
                                       sDisplayInfo.m_omStrMsgChannel.c_str());
    // Channel Mask
    m_omLstcFilterDetails.SetItemText( nIndex,
                                       6,
                                       sDisplayInfo.m_omStrMsgChannelMask.c_str());
    // Update image Index
    LVITEM sItem;
    sItem.mask      = LVIF_IMAGE;
    sItem.iItem     = m_nSelecetedFilterIndex;
    sItem.iSubItem  = 0;
    sItem.iImage    = sDisplayInfo.m_nImageIndex;
    m_omLstcFilterDetails.SetItem(&sItem);

    m_bUpdating = bOldUpdateValue;
}

/**
 * \param[in] psFilterData Filter info structure
 *
 * Updates UI with the filter information passed.
 * \req       RS_FLX_06
 * \design   DS_FLX_06
 * \codetag  CT_FLX_06
 */
void CFilterConfigDlg::vUpdateFilterDetails( const PSFILTERSET psFilterSet )
{
    if( psFilterSet != nullptr )
    {
        m_bUpdating = TRUE;
        // Clear existing list items
        m_omLstcFilterDetails.DeleteAllItems();
        if(m_eCurrBUS == CAN)
        {
            // Get Filter List count
            int nCount = psFilterSet->m_ushFilters;

            // Add items in to the list
            for( int nIndex = 0; nIndex < nCount; nIndex++ )
            {
                SFILTER_CAN* psFilter = (SFILTER_CAN*)(psFilterSet->m_psFilterInfo) + nIndex;
                CString omStrMsgIDFrom, omStrMsgIDTo, omStrChannel;
                // Get Display Text
                SFILTERDISPLAYINFO sDisplayInfo;
                vFormatDisplayString( *psFilter, sDisplayInfo );
                // Insert in to the list
                m_omLstcFilterDetails.InsertItem( nIndex,
                                                  "",
                                                  sDisplayInfo.m_nImageIndex );
                // Update Fileds
                vUpdateFilterListDetails( nIndex, sDisplayInfo);
            }
            m_bUpdating = FALSE;
            // Set the selection to Invalid
            m_nSelecetedFilterIndex = -1;
        }
        else if (m_eCurrBUS == LIN)
        {
            // Get Filter List count
            int nCount = psFilterSet->m_ushFilters;

            if (psFilterSet->m_eCurrBus == LIN)
            {
                // Add items in to the list
                for (int nIndex = 0; nIndex < nCount; nIndex++)
                {
                    SFILTER_LIN* psFilter = (SFILTER_LIN*)(psFilterSet->m_psFilterInfo) + nIndex;
                    CString omStrMsgIDFrom, omStrMsgIDTo, omStrChannel;
                    // Get Display Text
                    SFILTERDISPLAYINFO sDisplayInfo;
                    vFormatDisplayString(*psFilter, sDisplayInfo);
                    // Insert in to the list
                    m_omLstcFilterDetails.InsertItem(nIndex,
                                                     "",
                                                     sDisplayInfo.m_nImageIndex);
                    // Update Fileds
                    vUpdateFilterListDetails(nIndex, sDisplayInfo);
                }
                m_bUpdating = FALSE;
                // Set the selection to Invalid
                m_nSelecetedFilterIndex = -1;
            }
        }
    }
}

/**
 * \param[in] nItem Selected Row, nSubItem - Sub item of the selected row
 *
 * This will be called whenever user edits an item in the named
 * filter list. This will validate the info and save if it is
 * valid
 */
void CFilterConfigDlg::vUpdateFromFilterName(int nItem, int nSubItem)
{
    // Find the new name in the map
    CString omStrNewName = m_omLstcFilterList.GetItemText( nItem, nSubItem );
    if( omStrNewName.IsEmpty() )
    {
        MessageBox(_("Filter name cannot be empty"), "BUSMASTER", MB_OK|MB_ICONINFORMATION);
        // Restore the old name
        m_bUpdating = TRUE;
        m_omLstcFilterList.SetItemText( nItem, nSubItem,
                                        m_omStrSelectedFilterNameBeforeEdit );
        m_bUpdating = FALSE;
    }
    else
    {
        if(m_eCurrBUS == CAN)
        {
            // Update only if there is a change
            if( omStrNewName.Compare( m_omStrSelectedFilterNameBeforeEdit ) != 0 )
            {
                PSFILTERSET psTemp =  SFILTERSET::psGetFilterSetPointer(m_psFilterApplied->m_psFilters,
                                      m_psFilterApplied->m_ushTotal,omStrNewName.GetBuffer(MAX_PATH));
                if(psTemp != nullptr)
                {
                    MessageBox(_("Name already exist"), "BUSMASTER", MB_OK | MB_ICONINFORMATION);
                    // Restore the name
                    m_bUpdating = TRUE;
                    m_omLstcFilterList.SetItemText( nItem, nSubItem,
                                                    m_omStrSelectedFilterNameBeforeEdit );
                    m_bUpdating = FALSE;
                }
                else
                {
                    psTemp = SFILTERSET::psGetFilterSetPointer(m_psFilterApplied->m_psFilters,
                             m_psFilterApplied->m_ushTotal,m_omStrSelectedFilterNameBeforeEdit.GetBuffer(MAX_PATH));
                    ASSERT(psTemp != nullptr);
                    psTemp->m_sFilterName.m_acFilterName = omStrNewName.GetBuffer(MAX_PATH);
                    m_omStrSelectedFilterNameBeforeEdit = omStrNewName;
                }
            }
        }
        else if(m_eCurrBUS == LIN)
        {
            // Update only if there is a change
            if( omStrNewName.Compare( m_omStrSelectedFilterNameBeforeEdit ) != 0 )
            {
                PSFILTERSET psTemp =  SFILTERSET::psGetFilterSetPointer(m_psFilterAppliedLin->m_psFilters,
                                      m_psFilterAppliedLin->m_ushTotal,omStrNewName.GetBuffer(MAX_PATH));
                if(psTemp != nullptr)
                {
                    MessageBox(_("Name already exist"), "BUSMASTER", MB_OK | MB_ICONINFORMATION);
                    // Restore the name
                    m_bUpdating = TRUE;
                    m_omLstcFilterList.SetItemText( nItem, nSubItem,
                                                    m_omStrSelectedFilterNameBeforeEdit );
                    m_bUpdating = FALSE;
                }
                else
                {
                    psTemp = SFILTERSET::psGetFilterSetPointer(m_psFilterAppliedLin->m_psFilters,
                             m_psFilterAppliedLin->m_ushTotal,m_omStrSelectedFilterNameBeforeEdit.GetBuffer(MAX_PATH));
                    ASSERT(psTemp != nullptr);
                    psTemp->m_sFilterName.m_acFilterName = omStrNewName.GetBuffer(MAX_PATH);
                    m_omStrSelectedFilterNameBeforeEdit = omStrNewName;
                }
            }
        }
    }
}

/**
 * \param[in] nItem Row, nSubItem - Sub item in the row
 *
 * This function handles filter type change event
 * \req       RS_FLX_06
 * \design   DS_FLX_06
 * \codetag  CT_FLX_06
 */
void CFilterConfigDlg::vUpdateFromFilterType(int nItem, int nSubItem)
{
    // Get the Filter Type from the list
    CString omStrNewName =
        m_omLstcFilterList.GetItemText( nItem, nSubItem );

    UCHAR ucFilterType = 0;
    // Compare with Pass
    if( omStrNewName.Compare(_(defSTR_FILTER_TYPE_PASS)) == 0 )
    {
        ucFilterType = 1;
    }

    if(m_eCurrBUS == CAN)
    {
        CString omStrFilterName = m_omLstcFilterList.GetItemText(
                                      m_nSelecetedNamedFilterIndex, 0 );
        const PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                          m_psFilterApplied->m_psFilters,
                                          m_psFilterApplied->m_ushTotal,
                                          omStrFilterName.GetBuffer(MAX_PATH));
        if( psTempSet != nullptr )
        {
            // update Filter Type
            psTempSet->m_sFilterName.m_bFilterType = ucFilterType;
        }
    }
    else if(m_eCurrBUS == LIN)
    {
        CString omStrFilterName = m_omLstcFilterList.GetItemText(
                                      m_nSelecetedNamedFilterIndex, 0 );
        const PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                          m_psFilterAppliedLin->m_psFilters,
                                          m_psFilterAppliedLin->m_ushTotal,
                                          omStrFilterName.GetBuffer(MAX_PATH));
        if( psTempSet != nullptr )
        {
            // update Filter Type
            psTempSet->m_sFilterName.m_bFilterType = ucFilterType;
        }
    }
}

/**
 * \param[in] nSelectedItem Selected filter item
 *
 * This function updates the selected filter details
 * \req       RS_FLX_06
 * \design   DS_FLX_06
 * \codetag  CT_FLX_06
 */
void CFilterConfigDlg::vUpdateFilterComponents(int nSelectedItem)
{
    if(m_eCurrBUS == CAN)
    {
        // Get the Selected filter information
        CString omStrFilterName =
            m_omLstcFilterList.GetItemText( m_nSelecetedNamedFilterIndex, 0 );
        PSFILTERSET psFilterSet = SFILTERSET::psGetFilterSetPointer(m_psFilterApplied->m_psFilters,
                                  m_psFilterApplied->m_ushTotal, omStrFilterName.GetBuffer(MAX_PATH));
        if( psFilterSet != nullptr )
        {
            SFILTER_CAN* psFilter = (SFILTER_CAN*)(psFilterSet->m_psFilterInfo) + nSelectedItem;

            // Update Components
            vUpdateFilterComponents( *psFilter );
        }
    }
    else if(m_eCurrBUS == LIN)
    {
        // Get the Selected filter information
        CString omStrFilterName =
            m_omLstcFilterList.GetItemText( m_nSelecetedNamedFilterIndex, 0 );
        PSFILTERSET psFilterSet = SFILTERSET::psGetFilterSetPointer(m_psFilterAppliedLin->m_psFilters,
                                  m_psFilterAppliedLin->m_ushTotal, omStrFilterName.GetBuffer(MAX_PATH));
        if( psFilterSet != nullptr )
        {
            SFILTER_LIN* psFilter = (SFILTER_LIN*)(psFilterSet->m_psFilterInfo) + nSelectedItem;

            // Update Components
            vUpdateFilterComponents( *psFilter );
        }
    }
}

/**
 * \param[in] sFilter Filter details struct
 *
 * This function updates filter components with given filter
 * details
 */
void CFilterConfigDlg::vUpdateFilterComponents( const SFILTER_CAN& sFilter )
{
    // Disable validation of data while update it
    m_bDisableFilterCompUpdate = TRUE;
    // Get Database Pointer

    // ID From Text
    // Update Message Id or Range
    if ( sFilter.m_ucFilterType == defFILTER_TYPE_SINGLE_ID )
    {
        // Enable Message ID Section
        // Disable Range Controls
        m_omRadioEvent.SetCheck( FALSE );
        m_omEventType.EnableWindow( FALSE );
        m_omMsgIDRangeFrom.EnableWindow( FALSE );
        m_omMsgIDRangeTo.EnableWindow( FALSE );
        // Enable ID Control
        m_omMsgIDFrom.EnableWindow();
        CButton* pWnd = (CButton*)GetDlgItem( IDC_RADIO_RANGE );
        if ( pWnd != nullptr )
        {
            pWnd->SetCheck( FALSE );
            m_omRadioID.SetCheck( TRUE );
        }

        // Update Text
        if ( m_pClusterConfig != nullptr )
        {
            IFrame* ouFrameStrct;
            CString strTemp = "";
            std::string omStrIDFrom = "";
            if ( S_OK == hGetFrameFromId( sFilter.m_dwMsgIDFrom, CAN, 0/*Channel Index */, nullptr, &ouFrameStrct ) )
            {
                ouFrameStrct->GetName( omStrIDFrom );
                strTemp = omStrIDFrom.c_str();
            }
            if ( strTemp.IsEmpty() == TRUE )
            {
                strTemp.Format( defSTR_MSG_ID_FORMAT_NDB,
                                sFilter.m_dwMsgIDFrom );
            }
            else
            {
                CString omIdWithMsg;
                omIdWithMsg.Format( defSTR_MSG_ID_IN_HEX, sFilter.m_dwMsgIDFrom );
                strTemp = omIdWithMsg + strTemp;
            }
            // Apply the text
            m_omMsgIDFrom.SetWindowText( strTemp );
        }
        else
        {
            CString omStrIDFrom;
            omStrIDFrom.Format( defSTR_MSG_ID_FORMAT_NDB,
                                sFilter.m_dwMsgIDFrom );
            m_omMsgIDFrom.SetWindowText( omStrIDFrom );
        }
    }
    else if ( sFilter.m_ucFilterType == defFILTER_TYPE_ID_RANGE )
    {
        m_omRadioEvent.SetCheck( FALSE );
        m_omEventType.EnableWindow( FALSE );
        // Enable Message Range Section
        m_omMsgIDRangeFrom.EnableWindow();
        m_omMsgIDRangeTo.EnableWindow();
        // Get Window Pointer to set the value
        CButton* pWnd = (CButton*)GetDlgItem( IDC_RADIO_RANGE );
        if ( pWnd != nullptr )
        {
            pWnd->SetCheck( TRUE );
            m_omRadioID.SetCheck( FALSE );
        }
        // Disable ID Control
        m_omMsgIDFrom.EnableWindow( FALSE );

        // Update Message Range Values
        m_omMsgIDRangeFrom.vSetValue( sFilter.m_dwMsgIDFrom );
        m_omMsgIDRangeTo.vSetValue( sFilter.m_dwMsgIDTo );
    }
    else if ( sFilter.m_ucFilterType == defFILTER_TYPE_EVENT )
    {
        CButton* pWnd = (CButton*)GetDlgItem( IDC_RADIO_RANGE );
        if ( pWnd != nullptr )
        {
            pWnd->SetCheck( FALSE );
        }

        m_omMsgIDRangeFrom.EnableWindow( FALSE );
        m_omMsgIDRangeTo.EnableWindow( FALSE );
        m_omMsgDirection.EnableWindow( FALSE );
        m_omMsgIDFrom.EnableWindow( FALSE );
        m_omRadioID.SetCheck( FALSE );
        m_omEventType.EnableWindow( TRUE );
        m_omRadioEvent.SetCheck( TRUE );

        m_omEventType.SelectString( -1, sFilter.m_omEventName.c_str() );
    }
    // Update
    // ID Type
    INT Index = 0; // Id type standard

    switch ( sFilter.m_byIDType )
    {
        case TYPE_ID_CAN_EXTENDED:
            Index = 1; // Id type extended
            break;

        case TYPE_ID_CAN_ALL:
            Index = 2; // Id Type all
            break;
    }

    m_omMsgIDType.SetCurSel( Index );
    // Message Type
    Index = 0;// Non RTR

    switch ( sFilter.m_byMsgType )
    {
        case TYPE_MSG_CAN_RTR:
            Index = 1;// RTR
            break;

        case TYPE_MSG_CAN_ALL:
            Index = 2; // All
            break;
    }
    m_omMsgType.SetCurSel( Index );
    // Message Direction
    // Set the Direction
    switch ( sFilter.m_eDrctn )
    {
        case DIR_ALL: // All
            m_omMsgDirection.SetCurSel( 2 );
            break;

        case DIR_TX: // Tx Msg from UI
            m_omMsgDirection.SetCurSel( 1 );
            break;

        case DIR_RX: // All Msg from UI
            m_omMsgDirection.SetCurSel( 0 );
            break;

        default:    // Invalid Option
            ASSERT( FALSE );
            break;
    }
    UINT Channel = sFilter.m_eChannel;

    m_omMsgChannel.SetCurSel( Channel );
    // Enable On-line validation
    m_bDisableFilterCompUpdate = FALSE;
}


/**
 * \param[in] sFilter Filter details struct
 *
 * This function updates filter components with given filter
 * details
 */
void CFilterConfigDlg::vUpdateFilterComponents(const SFILTER_LIN& sFilter)
{
    // Disable validation of data while update it
    m_bDisableFilterCompUpdate = TRUE;
    // Get Database Pointer

    // ID From Text
    // Update Message Id or Range
    if( sFilter.m_ucFilterType == defFILTER_TYPE_SINGLE_ID )
    {
        m_omRadioEvent.SetCheck(FALSE);
        m_omEventType.EnableWindow(FALSE);
        m_omMsgDirection.EnableWindow( TRUE );
        // Enable Message ID Section
        // Disable Range Controls
        m_omMsgIDRangeFrom.EnableWindow( FALSE );
        m_omMsgIDRangeTo.EnableWindow( FALSE );
        // Enable ID Control
        m_omMsgIDFrom.EnableWindow( );
        CButton* pWnd = (CButton*)GetDlgItem(IDC_RADIO_RANGE);
        if( pWnd != nullptr )
        {
            pWnd->SetCheck( FALSE );
            m_omRadioID.SetCheck( TRUE );
        }

        if(m_pClusterConfig != nullptr)
        {
            IFrame* ouFrameStrct;
            CString strTemp = "";
            std::string omStrIDFrom = "";
            if(S_OK == hGetFrameFromId(sFilter.m_dwMsgIDFrom, LIN, 0/*Channel Index */, nullptr, &ouFrameStrct))
            {
                ouFrameStrct->GetName(omStrIDFrom);
                strTemp = omStrIDFrom.c_str();
            }

            if( strTemp.IsEmpty() == TRUE )
            {
                strTemp.Format( defSTR_MSG_ID_FORMAT_NDB,
                                sFilter.m_dwMsgIDFrom );
            }
            else
            {
                CString omIdWithMsg;
                omIdWithMsg.Format(defSTR_MSG_ID_IN_HEX, sFilter.m_dwMsgIDFrom);
                strTemp = omIdWithMsg + strTemp;
            }
            // Apply the text
            m_omMsgIDFrom.SetWindowText( strTemp );
        }
        else
        {
            CString omStrIDFrom;
            omStrIDFrom.Format( defSTR_MSG_ID_FORMAT_NDB,
                                sFilter.m_dwMsgIDFrom );
            m_omMsgIDFrom.SetWindowText( omStrIDFrom );
        }
    }
    else if( sFilter.m_ucFilterType == defFILTER_TYPE_ID_RANGE )
    {
        m_omRadioEvent.SetCheck(FALSE);
        m_omEventType.EnableWindow(FALSE);
        m_omMsgDirection.EnableWindow( TRUE );
        // Enable Message Range Section
        m_omMsgIDRangeFrom.EnableWindow( );
        m_omMsgIDRangeTo.EnableWindow( );
        // Get Window Pointer to set the value
        CButton* pWnd = (CButton*)GetDlgItem(IDC_RADIO_RANGE);
        if( pWnd != nullptr )
        {
            pWnd->SetCheck( TRUE );
            m_omRadioID.SetCheck( FALSE );
        }
        // Disable ID Control
        m_omMsgIDFrom.EnableWindow( FALSE );

        // Update Message Range Values
        m_omMsgIDRangeFrom.vSetValue( sFilter.m_dwMsgIDFrom );
        m_omMsgIDRangeTo.vSetValue( sFilter.m_dwMsgIDTo );
    }
    else if( sFilter.m_ucFilterType == defFILTER_TYPE_EVENT )
    {
        CButton* pWnd = (CButton*)GetDlgItem(IDC_RADIO_RANGE);
        if( pWnd != nullptr )
        {
            pWnd->SetCheck( FALSE );
        }

        m_omMsgIDRangeFrom.EnableWindow( FALSE );
        m_omMsgIDRangeTo.EnableWindow( FALSE );
        m_omMsgDirection.EnableWindow( FALSE );
        m_omMsgIDFrom.EnableWindow(FALSE);
        m_omRadioID.SetCheck(FALSE);
        m_omEventType.EnableWindow(TRUE);
        m_omRadioEvent.SetCheck( TRUE );

        m_omEventType.SelectString(-1, sFilter.m_omEventName.c_str());
    }

    // Message Direction
    // Set the Direction
    switch( sFilter.m_eDrctn )
    {
        case DIR_ALL: // All
            m_omMsgDirection.SetCurSel( 2 );
            break;

        case DIR_TX: // Tx Msg from UI
            m_omMsgDirection.SetCurSel( 1 );
            break;

        case DIR_RX: // All Msg from UI
            m_omMsgDirection.SetCurSel( 0 );
            break;

        default:    // Invalid Option
            ASSERT( FALSE );
            break;
    }
    UINT Channel = sFilter.m_eChannel;

    m_omMsgChannel.SetCurSel(Channel);
    // Enable On-line validation
    m_bDisableFilterCompUpdate = FALSE;
}

/**
 * This function returns the pointer to the selected named
 * filter.
 */
//PSNAMEDFILTER CFilterConfigDlg::psGetSelectedNamedFilter()
//{
//    CString omStrFilterName =
//        m_omLstcFilterList.GetItemText( m_nSelecetedNamedFilterIndex, 0 );
//    PSNAMEDFILTER psSelectedData =
//                (PSNAMEDFILTER)m_rsFilterList.m_romFilterMap[ omStrFilterName ];
//    return psSelectedData;
//}

/**
 * \param[in] omCombo Constant reference to CCombobox
 *
 * This function parses the combobox text to get message ID from it.
 */
int CFilterConfigDlg::nGetMsgIDFromCombo( const CComboBox& omCombo )
{
    CString omStrMsgName( "" );
    INT nMsgID = -1;
    omCombo.GetWindowText(omStrMsgName);
    // Get message Id from database in case user has selected a message name.
    // if message name is there then [] will be present
    int nIndex = omStrMsgName.Find( defMSGID_NAME_START_CHAR );
    if(nIndex != -1)
    {
        nMsgID = unGetMsgIDFromName( omStrMsgName );
    }

    else
    {
        // Compute value from the value directly
        UINT unMsgID = 0;
        CHAR* pcStr =nullptr;
        unMsgID = strtol(omStrMsgName,&pcStr,16);
        // check if the field is empty or user have entered "0".
        // so in case user has entered "0" or if non zero value is
        // entered and strtol function has writtened correct value.
        if( omStrMsgName.CompareNoCase("0") != 0 && unMsgID != 0 ||
                omStrMsgName.CompareNoCase("0") == 0 )
        {
            nMsgID = static_cast<INT> (unMsgID);
        }
    }
    return nMsgID;
}

/**
 * This function will be called during the selection change of
 * message ID/Name combobox
 */
void CFilterConfigDlg::OnSelchangeCombMsgIdFrom()
{
    if( m_bDisableFilterCompUpdate == FALSE )
    {
        CString omStrMsgName = "";
        // Update the Combox->Edit control
        int nSelectedIndex = m_omMsgIDFrom.GetCurSel();
        if( nSelectedIndex != -1 )
        {
            m_omMsgIDFrom.GetLBText( nSelectedIndex ,omStrMsgName );
            m_omMsgIDFrom.SetWindowText( omStrMsgName );
        }
        OnEditChangeMsgIDCombo();
    }
}

void CFilterConfigDlg::OnRadioEvent()
{
    m_omEventType.EnableWindow(TRUE);

    // Disable Range Controls
    m_omMsgIDRangeFrom.EnableWindow( FALSE );
    m_omMsgIDRangeTo.EnableWindow( FALSE );
    // Disable ID Control
    m_omMsgIDFrom.EnableWindow( FALSE);
    m_omMsgDirection.EnableWindow( FALSE );

    m_omMsgType.EnableWindow(FALSE);
    m_omMsgIDType.EnableWindow(FALSE);


    m_omMsgDirection.EnableWindow(false);
    m_omMsgIDFrom.EnableWindow(false);

    m_omMsgChannel.EnableWindow(true);

    // Update If there is any item got selected
    if( m_nSelecetedFilterIndex != -1 )
    {
        // Update Text
        CString omStrFilterName = m_omLstcFilterList.GetItemText(
                                      m_nSelecetedNamedFilterIndex, 0 );
        if(m_eCurrBUS == CAN)
        {
            m_omMsgType.ShowWindow(TRUE);
            m_omMsgIDType.ShowWindow(TRUE);
            m_omMsgType.EnableWindow(FALSE);
            m_omMsgIDType.EnableWindow(FALSE);

            PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                        m_psFilterApplied->m_psFilters,
                                        m_psFilterApplied->m_ushTotal,
                                        omStrFilterName.GetBuffer(MAX_PATH));
            if( psTempSet != nullptr )
            {
                // Get the data from the CArray
                SFILTER_CAN* psFilter = (SFILTER_CAN*)
                                        (psTempSet->m_psFilterInfo) + m_nSelecetedFilterIndex;
                psFilter->m_ucFilterType = defFILTER_TYPE_EVENT;// Update the UI

                CString omStrIDFrom = "";
                omStrIDFrom = psFilter->m_omEventName.c_str();
                // Apply the text
                m_omEventType.SetWindowText( omStrIDFrom );
            }
            // Validate and Update UI
            SFILTER_CAN sFilter;
            BOOL bDataValid = bGetFilterData( sFilter );
            // Update Add Button
            m_omAddFilter.EnableWindow( bDataValid );

            if( bDataValid == TRUE && m_nSelecetedFilterIndex != -1)
            {
                // Get Display Text
                SFILTERDISPLAYINFO sDisplayInfo;
                vFormatDisplayString( sFilter, sDisplayInfo );
                vUpdateFilterListDetails( m_nSelecetedFilterIndex, sDisplayInfo );
            }
        }
        else if(m_eCurrBUS == LIN)
        {
            PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                        m_psFilterAppliedLin->m_psFilters,
                                        m_psFilterAppliedLin->m_ushTotal,
                                        omStrFilterName.GetBuffer(MAX_PATH));
            if( psTempSet != nullptr )
            {
                m_omMsgType.ShowWindow(FALSE);
                m_omMsgIDType.ShowWindow(FALSE);

                // Get the data from the CArray
                SFILTER_LIN* psFilter = (SFILTER_LIN*)
                                        (psTempSet->m_psFilterInfo) + m_nSelecetedFilterIndex;
                psFilter->m_ucFilterType = defFILTER_TYPE_EVENT;// Update the UI

                CString omStrIDFrom = "";

                omStrIDFrom = psFilter->m_omEventName.c_str();
                // Apply the text
                m_omEventType.SetWindowText( omStrIDFrom );
            }
            // Validate and Update UI
            SFILTER_LIN sFilter;
            BOOL bDataValid = bGetFilterData( sFilter );
            // Update Add Button
            m_omAddFilter.EnableWindow( bDataValid );

            if( bDataValid == TRUE && m_nSelecetedFilterIndex != -1)
            {
                // Get Display Text
                SFILTERDISPLAYINFO sDisplayInfo;
                vFormatDisplayString( sFilter, sDisplayInfo );
                vUpdateFilterListDetails( m_nSelecetedFilterIndex, sDisplayInfo );
            }
        }
    }
}
/**
 * This function will be called during the ID/Range radio button
 * change. This will update the filter details after validation
 */
void CFilterConfigDlg::OnRadioMessageId()
{
    // Disable Range Controls
    m_omMsgIDRangeFrom.EnableWindow( FALSE );
    m_omMsgIDRangeTo.EnableWindow( FALSE );
    // Enable ID Control
    m_omMsgIDFrom.EnableWindow( );
    m_omMsgDirection.EnableWindow(TRUE);
    m_omEventType.EnableWindow(FALSE);

    m_omMsgType.EnableWindow(TRUE);
    m_omMsgIDType.EnableWindow(TRUE);






    // Update If there is any item got selected
    if( m_nSelecetedFilterIndex != -1 )
    {
        // Update Text
        CString omStrFilterName = m_omLstcFilterList.GetItemText(
                                      m_nSelecetedNamedFilterIndex, 0 );
        if(m_eCurrBUS == CAN)
        {
            m_omMsgType.ShowWindow(TRUE);
            m_omMsgIDType.ShowWindow(TRUE);
            m_omMsgType.EnableWindow(TRUE);
            m_omMsgIDType.EnableWindow(TRUE);
            PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                        m_psFilterApplied->m_psFilters,
                                        m_psFilterApplied->m_ushTotal,
                                        omStrFilterName.GetBuffer(MAX_PATH));
            if( psTempSet != nullptr )
            {
                // Get the data from the CArray
                SFILTER_CAN* psFilter = (SFILTER_CAN*)
                                        (psTempSet->m_psFilterInfo) + m_nSelecetedFilterIndex;
                psFilter->m_ucFilterType = defFILTER_TYPE_SINGLE_ID;// Update the UI

                if ( m_pClusterConfig != nullptr )
                {
                    CString omStrIDFrom = "";
                    IFrame* ouFrameStrct = nullptr;
                    if ( S_OK == hGetFrameFromId( psFilter->m_dwMsgIDFrom, CAN, 0, nullptr, &ouFrameStrct ) )
                    {
                        std::string strName;
                        ouFrameStrct->GetName( strName );
                        omStrIDFrom = strName.c_str();
                    }

                    if( omStrIDFrom.IsEmpty() == TRUE )
                    {
                        omStrIDFrom.Format( defSTR_MSG_ID_FORMAT_NDB,
                                            psFilter->m_dwMsgIDFrom );
                    }
                    else
                    {
                        if( omStrIDFrom != "" )
                        {
                            CString omIdWithMsg = "";
                            omIdWithMsg.Format(_(defSTR_MSG_ID_IN_HEX), psFilter->m_dwMsgIDFrom);
                            std::ostringstream oss;
                            oss << std::hex << omIdWithMsg << omStrIDFrom;

                            omStrIDFrom = (CString)oss.str().c_str();
                        }
                    }
                    // Apply the text
                    m_omMsgIDFrom.SetWindowText( omStrIDFrom );
                }
                else
                {
                    CString omStrIDFrom;
                    omStrIDFrom.Format( defSTR_MSG_ID_FORMAT_NDB,
                                        psFilter->m_dwMsgIDFrom );
                    m_omMsgIDFrom.SetWindowText( omStrIDFrom );
                }
            }
            SFILTER_CAN sFilter;
            BOOL bDataValid = bGetFilterData( sFilter );
            m_omAddFilter.EnableWindow( bDataValid );
            if( bDataValid == TRUE && m_nSelecetedFilterIndex != -1)
            {
                SFILTERDISPLAYINFO sDisplayInfo;
                vFormatDisplayString( sFilter, sDisplayInfo );
                vUpdateFilterListDetails( m_nSelecetedFilterIndex, sDisplayInfo );
            }
        }
        else if(m_eCurrBUS == LIN)
        {
            m_omMsgType.ShowWindow(FALSE);
            m_omMsgIDType.ShowWindow(FALSE);

            PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                        m_psFilterAppliedLin->m_psFilters,
                                        m_psFilterAppliedLin->m_ushTotal,
                                        omStrFilterName.GetBuffer(MAX_PATH));
            if( psTempSet != nullptr )
            {
                SFILTER_LIN* psFilter = (SFILTER_LIN*)
                                        (psTempSet->m_psFilterInfo) + m_nSelecetedFilterIndex;
                psFilter->m_ucFilterType = defFILTER_TYPE_SINGLE_ID;// Update the UI

                CString omStrIDFrom = "";
                IFrame* ouFrameStrct;
                if(m_pClusterConfig != nullptr)
                {
                    if(S_OK == hGetFrameFromId(psFilter->m_dwMsgIDFrom, LIN, 0, nullptr, &ouFrameStrct))
                    {
                        std::string strName;
                        ouFrameStrct->GetName(strName);
                        omStrIDFrom = strName.c_str();
                    }
                    if( omStrIDFrom.IsEmpty() == TRUE )
                    {
                        omStrIDFrom.Format( defSTR_MSG_ID_FORMAT_NDB,
                                            psFilter->m_dwMsgIDFrom );
                    }
                    else
                    {
                        if( omStrIDFrom != "" )
                        {
                            CString omIdWithMsg = "";
                            omIdWithMsg.Format(_(defSTR_MSG_ID_IN_HEX), psFilter->m_dwMsgIDFrom);
                            std::ostringstream oss;
                            oss << std::hex << omIdWithMsg << omStrIDFrom;
                            omStrIDFrom = (CString)oss.str().c_str();
                        }
                    }
                    m_omMsgIDFrom.SetWindowText( omStrIDFrom );
                }
                else
                {
                    CString omStrIDFrom;
                    omStrIDFrom.Format( defSTR_MSG_ID_FORMAT_NDB,
                                        psFilter->m_dwMsgIDFrom );
                    m_omMsgIDFrom.SetWindowText( omStrIDFrom );
                }
            }
            // Validate and Update UI
            SFILTER_LIN sFilter;
            BOOL bDataValid = bGetFilterData( sFilter );
            // Update Add Button
            m_omAddFilter.EnableWindow( bDataValid );

            if( bDataValid == TRUE && m_nSelecetedFilterIndex != -1)
            {
                // Get Display Text
                SFILTERDISPLAYINFO sDisplayInfo;
                vFormatDisplayString( sFilter, sDisplayInfo );
                vUpdateFilterListDetails( m_nSelecetedFilterIndex, sDisplayInfo );
            }
        }
    }
}

/**
 * This function will be called during the ID/Range radio button
 * change. This will update the filter details after validation
 */
void CFilterConfigDlg::OnRadioRange()
{
    // Disable Range Controls
    m_omMsgIDFrom.EnableWindow( FALSE );
    // Enable ID Control
    m_omMsgIDRangeFrom.EnableWindow( );
    m_omMsgIDRangeTo.EnableWindow( );
    m_omMsgDirection.EnableWindow(TRUE);
    m_omEventType.EnableWindow(FALSE);

    m_omMsgType.EnableWindow(TRUE);
    m_omMsgIDType.EnableWindow(TRUE);

    // Update selected item
    if( m_nSelecetedFilterIndex != -1 )
    {
        // Update Text
        CString omStrFilterName = m_omLstcFilterList.GetItemText(
                                      m_nSelecetedNamedFilterIndex, 0 );
        if(m_eCurrBUS == CAN)
        {
            m_omMsgType.ShowWindow(TRUE);
            m_omMsgIDType.ShowWindow(TRUE);
            m_omMsgType.EnableWindow(TRUE);
            m_omMsgIDType.EnableWindow(TRUE);

            PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                        m_psFilterApplied->m_psFilters,
                                        m_psFilterApplied->m_ushTotal,
                                        omStrFilterName.GetBuffer(MAX_PATH));
            if( psTempSet != nullptr )
            {
                SFILTER_CAN* psFilter =
                    (SFILTER_CAN*)(psTempSet->m_psFilterInfo) + m_nSelecetedFilterIndex;
                psFilter->m_ucFilterType = defFILTER_TYPE_ID_RANGE;
                if( psFilter->m_dwMsgIDTo <= psFilter->m_dwMsgIDFrom )
                {
                    psFilter->m_dwMsgIDTo =psFilter->m_dwMsgIDFrom + 1;
                }

                // Update UI
                // Get Display Text
                SFILTERDISPLAYINFO sDisplayInfo;
                vFormatDisplayString( *psFilter, sDisplayInfo );
                vUpdateFilterListDetails( m_nSelecetedFilterIndex, sDisplayInfo );

                m_omMsgIDRangeFrom.vSetValue( psFilter->m_dwMsgIDFrom );
                m_omMsgIDRangeTo.vSetValue( psFilter->m_dwMsgIDTo );
            }
        }
        else if(m_eCurrBUS == LIN)
        {
            m_omMsgType.ShowWindow(FALSE);
            m_omMsgIDType.ShowWindow(FALSE);

            PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                        m_psFilterAppliedLin->m_psFilters,
                                        m_psFilterAppliedLin->m_ushTotal,
                                        omStrFilterName.GetBuffer(MAX_PATH));
            if( psTempSet != nullptr )
            {
                SFILTER_LIN* psFilter =
                    (SFILTER_LIN*)(psTempSet->m_psFilterInfo) + m_nSelecetedFilterIndex;
                psFilter->m_ucFilterType = defFILTER_TYPE_ID_RANGE;
                if( psFilter->m_dwMsgIDTo <= psFilter->m_dwMsgIDFrom )
                {
                    psFilter->m_dwMsgIDTo =psFilter->m_dwMsgIDFrom + 1;
                }
                SFILTERDISPLAYINFO sDisplayInfo;
                vFormatDisplayString( *psFilter, sDisplayInfo );
                vUpdateFilterListDetails( m_nSelecetedFilterIndex, sDisplayInfo );
                m_omMsgIDRangeFrom.vSetValue( psFilter->m_dwMsgIDFrom );
                m_omMsgIDRangeTo.vSetValue( psFilter->m_dwMsgIDTo );
            }
        }
    }
    // To validate the range
    m_omMsgIDRangeFrom.vSetValue( m_omMsgIDRangeFrom.lGetValue() );

}

/**
 * This function will ve called during range editbox change.
 * This validates the range and updates if it is valid
 */
void CFilterConfigDlg::OnUpdateEditRange()
{
    if( m_bDisableFilterCompUpdate == FALSE )
    {
        if(m_eCurrBUS == CAN)
        {
            SFILTER_CAN sFilter;
            // Get Message From UI and Validate
            BOOL bDataValid = bGetFilterData( sFilter );
            // Update Add Button
            m_omAddFilter.EnableWindow( bDataValid );
            // Update Add Button
            m_omAddFilter.EnableWindow( bDataValid );
            if( bDataValid == TRUE &&
                    // Check for selection in the filter list
                    m_nSelecetedFilterIndex != -1 )
            {
                bUpdateSelectedItem( sFilter );
            }
        }
        else if(m_eCurrBUS == LIN)
        {
            SFILTER_LIN sFilter;
            BOOL bDataValid = bGetFilterData( sFilter );
            m_omAddFilter.EnableWindow( bDataValid );
            m_omAddFilter.EnableWindow( bDataValid );
            if( bDataValid == TRUE &&
                    m_nSelecetedFilterIndex != -1 )
            {
                bUpdateSelectedItem( sFilter );
            }
        }
    }
}

/**
 * This function will be called during text change in the
 * message ID/Name combobox. This will validate the text and
 * will update filter details if it is valid
 */
void CFilterConfigDlg::OnEditChangeMsgIDCombo()
{
    if(m_eCurrBUS == CAN)
    {
        SFILTER_CAN sFilter;
        // Get Message From UI and Validate
        BOOL bDataValid = bGetFilterData( sFilter );
        // Update Add Button
        m_omAddFilter.EnableWindow( bDataValid );

        if( bDataValid == TRUE )
        {
            if( m_nSelecetedFilterIndex != -1 )
            {
                // Update Data
                bUpdateSelectedItem( sFilter );
            }
        }
    }
    else if(m_eCurrBUS == LIN)
    {
        SFILTER_LIN sFilter;
        BOOL bDataValid = bGetFilterData( sFilter );
        m_omAddFilter.EnableWindow( bDataValid );
        if(bDataValid == TRUE )
        {
            if( m_nSelecetedFilterIndex != -1 )
            {
                bUpdateSelectedItem( sFilter );
            }
        }
    }
}

HRESULT CFilterConfigDlg::hGetFrameFromId(UINT unSlotId, ETYPE_BUS eBus, int nChannel, void* pouProtocolId, IFrame** ouFrameStrct)
{
    int nChannelCoinfigured = 0;
    m_pClusterConfig->GetChannelCount(eBus, nChannelCoinfigured);
    if ( nChannelCoinfigured < nChannel )
    {
        return S_FALSE;
    }

    if( EC_SUCCESS == m_pClusterConfig->GetFrame(eBus, nChannel, unSlotId, pouProtocolId, ouFrameStrct) )
    {
        return S_OK;
    }

    return S_FALSE;
}

/******************************************************************************
  Function Name    :  hGetFrameFromId
  Input(s)         :  unSlotId - Slot id of message to be retreived.
                   :  nBasecycle - Base cycle of message to be retreived.
                   :  unChannel - channel
  Output           :  void
  Functionality    :  Retreive a frame matching Slot id, Base cycle and channel
  Member of        :  CFilterConfigDlg
  Friend of        :  -
  Author(s)        :  Shailesh H
  Date Created     :  22.12.2014
  Modification date:
  Modification By  :
  Requirement ID   : RS_FLX_06
******************************************************************************/
//HRESULT CFilterConfigDlg::hGetFrameFromId(UINT unSlotId,int nBaseCycle, ECHANNEL unChannel, FRAME_STRUCT& ouFrameStrct)
//{
//    std::list<FRAME_STRUCT> lstFrameStrct;
//
//    for(INT nIndex = 0; nIndex < m_pClusterConfig->m_nChannelsConfigured; nIndex++)
//    {
//        if(S_OK == m_pClusterConfig->m_ouFlexChannelConfig[nIndex].GetFrame(unSlotId ,nBaseCycle ,unChannel, ouFrameStrct))
//        {
//            return S_OK;
//        }
//    }
//
//    return S_OK;
//}
/**
 * This function will be called during the change in message ID
 * type, frame type, direction and channel combo boxes. This
 * will validate filter components values and will update the
 * filter details.
 */
void CFilterConfigDlg::OnSelchangeFilterComponentCombo()
{
    if( m_bDisableFilterCompUpdate == FALSE )
    {
        if(m_eCurrBUS == CAN)
        {
            SFILTER_CAN sFilter;
            // Get Message From UI and Validate
            BOOL bDataValid = bGetFilterData( sFilter );
            // Update Add Button
            m_omAddFilter.EnableWindow( bDataValid );

            if( bDataValid == TRUE &&
                    // Check for selection in the filter list
                    m_nSelecetedFilterIndex != -1 )
            {
                // Update Data
                bUpdateSelectedItem( sFilter );
            }
        }
        else if(m_eCurrBUS == LIN)
        {
            vPopulateChannelDBMessages();

            SFILTER_LIN sFilter;
            // Get Message From UI and Validate
            BOOL bDataValid = bGetFilterData( sFilter );
            // Update Add Button
            m_omAddFilter.EnableWindow( bDataValid );

            if( bDataValid == TRUE &&
                    // Check for selection in the filter list
                    m_nSelecetedFilterIndex != -1 )
            {
                // Update Data
                bUpdateSelectedItem( sFilter );
            }
        }
    }
}

void CFilterConfigDlg::vPopulateChannelDBMessages()
{
    CString strChnlNum;
    m_omMsgChannel.GetWindowText(strChnlNum);

    UINT unChnlIndex = atoi(strChnlNum);

    if(unChnlIndex > 0)
    {
        std::map<UID_ELEMENT, IElement*> lstFrameStrct;
        ICluster* pCluster;
        m_pClusterConfig->GetDBService(LIN, unChnlIndex - 1, 0, &pCluster);
        if ( nullptr == pCluster )
        {
            return;
        }

        pCluster->GetElementList(eFrameElement, lstFrameStrct);

        CString omMsgId = "";
        /*m_omMsgIDFrom.GetWindowText(omMsgId);*/
        UINT unMsgId = nGetMsgIDFromCombo( m_omMsgIDFrom );

        m_omMsgIDFrom.ResetContent();
        BOOL bIsMsgExists = FALSE;

        if(lstFrameStrct.size() > 0)
        {
            //std::list<IFrame*>::iterator itrFrameStrct = lstFrameStrct.begin();
            std::string strName;
            unsigned int unId;
for ( auto itrFrameStrct : lstFrameStrct )
            {
                itrFrameStrct.second->GetName(strName);
                ((IFrame*)itrFrameStrct.second)->GetFrameId(unId);
                char omStrMsgId[MAX_PATH] = "";
                //sprintf_s(omStrMsgId, MAX_PATH, defSTR_MSG_ID_IN_HEX, itrFrame->m_nSlotId);

                sprintf_s(omStrMsgId, MAX_PATH, "[0x%x]%s", unId, strName.c_str());

                m_omMsgIDFrom.AddString(omStrMsgId);

                if(unMsgId == unId)
                {
                    omMsgId = omStrMsgId;
                    bIsMsgExists = TRUE;
                }
            }
        }

        if(bIsMsgExists == TRUE)
        {
            m_omMsgIDFrom.SetWindowText(omMsgId);
        }
        else
        {
            CString omStr = "";
            std::ostringstream oss;
            oss << std::hex << unMsgId;
            std::string strMsgId = oss.str();

            m_omMsgIDFrom.SetWindowText(strMsgId.c_str());
        }
    }
}

/**
 * \param[in] omStrText new status bar text
 *
 * Sets the status bar text
 */
void CFilterConfigDlg::vSetStatusText(CString omStrText)
{
    // Set the window text
    m_omStatusText.SetWindowText( omStrText );
}

/**
 * \param[in] sFilter Reference to filter structure
 * \return Filter detais valid or not
 *
 * Updates the filter structure with filter details from UI.
 */
BOOL CFilterConfigDlg::bGetFilterData(SFILTER_CAN& sFilter)
{
    // Get Filter Type
    BOOL bIDFilter = m_omRadioID.GetCheck();
    BOOL bDataValid = FALSE;
    BOOL bRange = FALSE, bEventType = FALSE;

    CButton* pWnd = (CButton*)GetDlgItem(IDC_RADIO_RANGE);
    if( pWnd != nullptr )
    {
        bRange = pWnd->GetCheck();
    }

    bEventType = m_omRadioEvent.GetCheck();

    // For Single Message ID
    if( bIDFilter != 0 )
    {
        // Get Message ID
        INT nMsgID = nGetMsgIDFromCombo( m_omMsgIDFrom );
        // Proceed if ID is valid
        if( nMsgID != -1 && nMsgID >= 0 )
        {
            // Check for ID Range
            INT nExtendedID = m_omMsgIDType.GetCurSel();

            if(nExtendedID != 0 )
            {
                if( nMsgID < defMAX_LMT_EXTD_MSG_ID )
                {
                    bDataValid = TRUE;
                }
                else
                {
                    vSetStatusText(_("Message ID exceeds extended ID range"));
                }
            }
            else
            {
                if( nMsgID < MAX_LMT_FOR_STD_MSG_ID )
                {
                    bDataValid = TRUE;
                }
                else
                {
                    vSetStatusText(_("Message ID exceeds standard ID range"));
                }
            }

            // Copy data if data is valid
            if( bDataValid == TRUE )
            {
                // Filter Type
                sFilter.m_ucFilterType = defFILTER_TYPE_SINGLE_ID;
                // Message ID
                sFilter.m_dwMsgIDFrom = nMsgID;
                // Set Message ID To to the same value
                sFilter.m_dwMsgIDTo = nMsgID;
                // Set ID Type
                switch (nExtendedID)
                {
                    case 0: // Standard
                        sFilter.m_byIDType = TYPE_ID_CAN_STANDARD;
                        break;

                    case 1: // Extended
                        sFilter.m_byIDType = TYPE_ID_CAN_EXTENDED;
                        break;

                    case 2: //ALL
                        sFilter.m_byIDType = TYPE_ID_CAN_ALL;
                        break;
                }
                INT MsgType = m_omMsgType.GetCurSel();
                switch (MsgType)
                {
                    case 0: // NON RTR
                        sFilter.m_byMsgType = TYPE_MSG_CAN_NON_RTR;
                        break;

                    case 1: // RTR
                        sFilter.m_byMsgType = TYPE_MSG_CAN_RTR;
                        break;

                    case 2: // ALL
                        sFilter.m_byMsgType = TYPE_MSG_CAN_ALL;
                        break;
                }

                // Set the Direction
                switch( m_omMsgDirection.GetCurSel())
                {
                    case 0: // Rx Msg from UI
                        sFilter.m_eDrctn = DIR_RX;
                        break;

                    case 1: // Tx Msg from UI
                        sFilter.m_eDrctn = DIR_TX;
                        break;

                    case 2: // All Msg from UI
                        sFilter.m_eDrctn = DIR_ALL;
                        break;

                    default:    // Invalid Option
                        ASSERT( FALSE );
                        break;
                }
                // Set Channel
                UINT Index = static_cast<UCHAR>( m_omMsgChannel.GetCurSel());
                if (Index == 0)
                {
                    sFilter.m_eChannel = CAN_CHANNEL_ALL;
                }
                else if ((Index >= CHANNEL_CAN_MIN) && (Index <= m_nHardware))
                {
                    sFilter.m_eChannel = Index;
                }
                else
                {
                    sFilter.m_eChannel = CHANNEL_All_UNSPECIFIED;
                }
            }
        }
        else
        {
            vSetStatusText(_("Invalid Message ID"));
        }
    }
    else if(bRange == TRUE)
    {
        // Get Message Range
        UINT unMsgIDFrom = static_cast<UINT>(m_omMsgIDRangeFrom.lGetValue());
        UINT unMsgIDTo = static_cast<UINT>(m_omMsgIDRangeTo.lGetValue());
        // Validate IDs
        INT nExtendedID = m_omMsgIDType.GetCurSel();

        if(nExtendedID != 0 )
        {
            if( unMsgIDFrom < defMAX_LMT_EXTD_MSG_ID )
            {
                bDataValid = TRUE;
            }
            else
            {
                vSetStatusText(_("Start range message ID exceeds extended ID range"));
            }
        }
        else
        {
            if( unMsgIDFrom < MAX_LMT_FOR_STD_MSG_ID )
            {
                bDataValid = TRUE;
            }
            else
            {
                vSetStatusText(_("Start range message ID exceeds standard ID range"));
            }
        }

        if( bDataValid == TRUE )
        {
            // Set ID Valid to false
            bDataValid = FALSE;
            if( nExtendedID != 0 )
            {
                if( unMsgIDTo < defMAX_LMT_EXTD_MSG_ID )
                {
                    bDataValid = TRUE;
                }
                else
                {
                    vSetStatusText(_("End range message ID exceeds extended ID range"));
                }
            }
            else
            {
                if( unMsgIDTo < MAX_LMT_FOR_STD_MSG_ID )
                {
                    bDataValid = TRUE;
                }
                else
                {
                    vSetStatusText(_("End range message ID exceeds standard ID range"));
                }
            }
        }
        // Check for range
        if( bDataValid == TRUE && unMsgIDFrom >= unMsgIDTo )
        {
            bDataValid = FALSE;
            vSetStatusText(_("Start Range ID is greater than or equal to End Range ID"));
        }
        // Copy data if data is valid
        if( bDataValid == TRUE )
        {
            // Filter Type
            sFilter.m_ucFilterType = defFILTER_TYPE_ID_RANGE;
            // Message ID
            sFilter.m_dwMsgIDFrom = unMsgIDFrom;
            // Set Message ID To to the same value
            sFilter.m_dwMsgIDTo = unMsgIDTo;
            // Set ID Type
            // Set ID Type
            switch (nExtendedID)
            {
                case 0: // Standard
                    sFilter.m_byIDType = TYPE_ID_CAN_STANDARD;
                    break;

                case 1: // Extended
                    sFilter.m_byIDType = TYPE_ID_CAN_EXTENDED;
                    break;

                case 2: //ALL
                    sFilter.m_byIDType = TYPE_ID_CAN_ALL;
                    break;
            }

            INT MsgType = m_omMsgType.GetCurSel();

            switch (MsgType)
            {
                case 0: // Standard
                    sFilter.m_byMsgType = TYPE_MSG_CAN_NON_RTR;
                    break;

                case 1: // Extended
                    sFilter.m_byMsgType = TYPE_MSG_CAN_RTR;
                    break;

                case 2:
                    sFilter.m_byMsgType = TYPE_MSG_CAN_ALL;
                    break;

                default:    // Invalid Option
                    ASSERT( FALSE );
                    break;
            }

            // Set the Direction
            switch( m_omMsgDirection.GetCurSel())
            {
                case 0: // Rx Msg from UI
                    sFilter.m_eDrctn = DIR_RX;
                    break;

                case 1: // Tx Msg from UI
                    sFilter.m_eDrctn = DIR_TX;
                    break;

                case 2: // All Msg from UI
                    sFilter.m_eDrctn = DIR_ALL;
                    break;

                default:    // Invalid Option
                    ASSERT( FALSE );
                    break;
            }

            // Set Channel
            UINT Index = static_cast<UCHAR>( m_omMsgChannel.GetCurSel());
            if (Index == 0)
            {
                sFilter.m_eChannel = CAN_CHANNEL_ALL;
            }
            else if ((Index >= CHANNEL_CAN_MIN) && (Index <= m_nHardware))
            {
                sFilter.m_eChannel = Index;
            }
            else
            {
                sFilter.m_eChannel = CHANNEL_All_UNSPECIFIED;
            }
        }
    }
    else if(bEventType == TRUE)
    {
        CString omStrEventName( "" );

        m_omEventType.GetWindowText(omStrEventName);

        if(omStrEventName.IsEmpty() == FALSE)
        {
            bDataValid = TRUE;

            // Filter Type
            sFilter.m_ucFilterType = defFILTER_TYPE_EVENT;
            sFilter.m_omEventName = omStrEventName;
            // Message ID
            sFilter.m_dwMsgIDFrom = 0;
            // Set Message ID To to the same value
            sFilter.m_dwMsgIDTo = 0;
            sFilter.m_eDrctn  = DIR_ALL;

            // Set Channel
            UINT Index = static_cast<UCHAR>( m_omMsgChannel.GetCurSel());
            if (Index == 0)
            {
                sFilter.m_eChannel = CAN_CHANNEL_ALL;
            }
            else if ((Index >= CHANNEL_CAN_MIN) && (Index <= m_nHardware))
            {
                sFilter.m_eChannel = Index;
            }
            else
            {
                sFilter.m_eChannel = CHANNEL_All_UNSPECIFIED;
            }
        }
    }

    // IF valid data then clear status message
    if( bDataValid == TRUE )
    {
        vSetStatusText( "" );
    }
    return bDataValid;
}

/**
 * \param[in] sFilter Reference to filter structure
 * \return Filter detais valid or not
 *
 * Updates the filter structure with filter details from UI.
 */
BOOL CFilterConfigDlg::bGetFilterData(SFILTER_LIN& sFilter)
{
    // Get Filter Type
    BOOL bIDFilter = m_omRadioID.GetCheck();
    BOOL bRange = FALSE, bEventType = FALSE;

    CButton* pWnd = (CButton*)GetDlgItem(IDC_RADIO_RANGE);
    if( pWnd != nullptr )
    {
        bRange = pWnd->GetCheck();
    }

    bEventType = m_omRadioEvent.GetCheck();

    BOOL bDataValid = FALSE;

    // For Single Message ID
    if( bIDFilter != 0 )
    {
        // Get Message ID
        INT nMsgID = nGetMsgIDFromCombo( m_omMsgIDFrom );
        // Proceed if ID is valid
        if( nMsgID != -1 && nMsgID >= 0 )
        {
            if(nMsgID >= 0 && nMsgID <= defMAX_LMT_LIN_MSG_ID)
            {
                bDataValid = TRUE;
            }
            else
            {
                vSetStatusText(_("Valid range for LIN Message ID is 0 - 0x3F"));
            }

            // Copy data if data is valid
            if( bDataValid == TRUE )
            {
                // Filter Type
                sFilter.m_ucFilterType = defFILTER_TYPE_SINGLE_ID;
                // Message ID
                sFilter.m_dwMsgIDFrom = nMsgID;
                // Set Message ID To to the same value
                sFilter.m_dwMsgIDTo = nMsgID;

                // Set the Direction
                switch( m_omMsgDirection.GetCurSel())
                {
                    case 0: // Rx Msg from UI
                        sFilter.m_eDrctn = DIR_RX;
                        break;

                    case 1: // Tx Msg from UI
                        sFilter.m_eDrctn = DIR_TX;
                        break;

                    case 2: // All Msg from UI
                        sFilter.m_eDrctn = DIR_ALL;
                        break;

                    default:    // Invalid Option
                        ASSERT( FALSE );
                        break;
                }
                // Set Channel
                UINT Index = static_cast<UCHAR>( m_omMsgChannel.GetCurSel());
                int unChannelsConfigured = 0;
                m_pClusterConfig->GetChannelCount(LIN, unChannelsConfigured);
                if (Index == 0)
                {
                    sFilter.m_eChannel = CAN_CHANNEL_ALL;
                }
                else if ((Index >= CHANNEL_CAN_MIN) && (Index <= unChannelsConfigured))
                {
                    sFilter.m_eChannel = Index;
                }
                else
                {
                    sFilter.m_eChannel = CHANNEL_All_UNSPECIFIED;
                }
            }
        }
        else
        {
            vSetStatusText(_("Invalid Message ID"));
        }
    }
    else if(bRange == TRUE)
    {
        // Get Message Range
        UINT unMsgIDFrom = static_cast<UINT>(m_omMsgIDRangeFrom.lGetValue());
        UINT unMsgIDTo = static_cast<UINT>(m_omMsgIDRangeTo.lGetValue());

        if( unMsgIDFrom >= 0 && unMsgIDFrom <= defMAX_LMT_LIN_MSG_ID)
        {
            bDataValid = TRUE;
        }
        else
        {
            vSetStatusText(_("Valid range for LIN Message ID is 0 - 0x3F"));
        }

        if( bDataValid == TRUE )
        {
            // Set ID Valid to false
            bDataValid = FALSE;

            if( unMsgIDTo <= defMAX_LMT_LIN_MSG_ID )
            {
                bDataValid = TRUE;
            }
            else
            {
                vSetStatusText(_("Valid range for LIN Message ID is 0 - 0x3F"));
            }
        }

        // Check for range
        if( bDataValid == TRUE && unMsgIDFrom >= unMsgIDTo )
        {
            bDataValid = FALSE;
            vSetStatusText(_("Start Range ID is greater than or equal to End Range ID"));
        }
        // Copy data if data is valid
        if( bDataValid == TRUE )
        {
            // Filter Type
            sFilter.m_ucFilterType = defFILTER_TYPE_ID_RANGE;
            // Message ID
            sFilter.m_dwMsgIDFrom = unMsgIDFrom;
            // Set Message ID To to the same value
            sFilter.m_dwMsgIDTo = unMsgIDTo;
            // Set the Direction
            switch( m_omMsgDirection.GetCurSel())
            {
                case 0: // Rx Msg from UI
                    sFilter.m_eDrctn = DIR_RX;
                    break;

                case 1: // Tx Msg from UI
                    sFilter.m_eDrctn = DIR_TX;
                    break;

                case 2: // All Msg from UI
                    sFilter.m_eDrctn = DIR_ALL;
                    break;

                default:    // Invalid Option
                    ASSERT( FALSE );
                    break;
            }

            // Set Channel
            UINT Index = static_cast<UCHAR>( m_omMsgChannel.GetCurSel());
            int unChannelsConfigured = 0;
            m_pClusterConfig->GetChannelCount(LIN, unChannelsConfigured);
            if (Index == 0)
            {
                sFilter.m_eChannel = CAN_CHANNEL_ALL;
            }
            else if ((Index >= CHANNEL_CAN_MIN) && (Index <= unChannelsConfigured))
            {
                sFilter.m_eChannel = Index;
            }
            else
            {
                sFilter.m_eChannel = CHANNEL_All_UNSPECIFIED;
            }
        }
    }
    else if(bEventType == TRUE)
    {
        CString omStrEventName( "" );

        m_omEventType.GetWindowText(omStrEventName);

        if(omStrEventName.IsEmpty() == FALSE)
        {
            bDataValid = TRUE;

            // Filter Type
            sFilter.m_ucFilterType = defFILTER_TYPE_EVENT;
            sFilter.m_omEventName = omStrEventName;
            // Message ID
            sFilter.m_dwMsgIDFrom = 0;
            // Set Message ID To to the same value
            sFilter.m_dwMsgIDTo = 0;
            sFilter.m_eDrctn  = DIR_ALL;

            // Set Channel
            UINT Index = static_cast<UCHAR>( m_omMsgChannel.GetCurSel());
            int unChannelsConfigured = 0;
            m_pClusterConfig->GetChannelCount(LIN, unChannelsConfigured);
            if (Index == 0)
            {
                sFilter.m_eChannel = CAN_CHANNEL_ALL;
            }
            else if ((Index >= CHANNEL_CAN_MIN) && (Index <= unChannelsConfigured))
            {
                sFilter.m_eChannel = Index;
            }
            else
            {
                sFilter.m_eChannel = CHANNEL_All_UNSPECIFIED;
            }
        }
    }
    // IF valid data then clear status message
    if( bDataValid == TRUE )
    {
        vSetStatusText( "" );
    }
    return bDataValid;
}

/**
 * \param[in] sFilter Filter Details
 * Updates the selected item in the list with the given filter
 * details.
 */
BOOL CFilterConfigDlg::bUpdateSelectedItem(SFILTER_LIN& sFilter)
{
    BOOL bIsAnytingUpdated = FALSE;
    // If Selected Item is valid
    if( m_nSelecetedFilterIndex != -1 )
    {
        // Update Text
        CString omStrFilterName = m_omLstcFilterList.GetItemText(
                                      m_nSelecetedNamedFilterIndex, 0 );
        PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                    m_psFilterAppliedLin->m_psFilters,
                                    m_psFilterAppliedLin->m_ushTotal,
                                    omStrFilterName.GetBuffer(MAX_PATH));
        if( psTempSet != nullptr )
        {
            SFILTER_LIN* psDest = (SFILTER_LIN*)(psTempSet->m_psFilterInfo) + m_nSelecetedFilterIndex;
            *psDest = sFilter;
            // Update UI
            // Get Display Text
            SFILTERDISPLAYINFO sDisplayInfo;
            // Get Display Text
            vFormatDisplayString( sFilter, sDisplayInfo );
            // Update Display Text
            vUpdateFilterListDetails( m_nSelecetedFilterIndex,
                                      sDisplayInfo );
            bIsAnytingUpdated = TRUE;
        }
    }
    return bIsAnytingUpdated;
}

/**
 * \param[in] sFilter Filter details
 * \param[in] sFilterDisplyInfo formatted Filter details structure
 *
 * This function formats the filter details and fills the
 * sFilterDisplyInfo structure with the formatted strings.
 */
void CFilterConfigDlg::vFormatDisplayString(
    const SFILTER_LIN& sFilter,
    SFILTERDISPLAYINFO& sFilterDisplyInfo)
{
    // Get the database pointer from the application

    // Set the default image index
    sFilterDisplyInfo.m_nImageIndex = defFILTER_IMAGE_INDEX_NDB;
    // Clear ID From and To String
    sFilterDisplyInfo.m_omStrMsgIDFrom = "";
    sFilterDisplyInfo.m_omStrMsgIDTo = "";
    // If database is valid
    if( sFilter.m_ucFilterType == defFILTER_TYPE_SINGLE_ID )
    {
        m_omMsgDirection.EnableWindow( TRUE );
        // Get the database name for ID From
        CString omStr = "";

        IFrame* ouFrameStrct;
        if(S_OK == hGetFrameFromId(sFilter.m_dwMsgIDFrom, LIN, 0, nullptr, &ouFrameStrct))
        {
            std::string strName;
            ouFrameStrct->GetName(strName);
            omStr = strName.c_str();
        }

        if( omStr != "" )
        {
            std::ostringstream oss;
            oss << std::hex << sFilter.m_dwMsgIDFrom;
            oss << " [" << omStr << "]";
            sFilterDisplyInfo.m_omStrMsgIDFrom = oss.str();
            // Update Image Index
            sFilterDisplyInfo.m_nImageIndex = defFILTER_IMAGE_INDEX_DB;
        }
        else
        {
            std::ostringstream oss;
            oss << std::hex << sFilter.m_dwMsgIDFrom;
            sFilterDisplyInfo.m_omStrMsgIDFrom = oss.str();
        }
    }
    else if( sFilter.m_ucFilterType == defFILTER_TYPE_ID_RANGE )
    {
        m_omMsgDirection.EnableWindow( TRUE );
        // Range of messages
        // Format ID FRom
        std::ostringstream oss1;
        oss1 << std::hex << sFilter.m_dwMsgIDFrom;
        sFilterDisplyInfo.m_omStrMsgIDFrom = oss1.str();
        std::ostringstream oss2;
        oss2 << std::hex << sFilter.m_dwMsgIDTo;
        sFilterDisplyInfo.m_omStrMsgIDTo = oss2.str();
        // Update Image Index
        sFilterDisplyInfo.m_nImageIndex = defFILTER_IMAGE_INDEX_ID_RANGE;
    }
    else if( sFilter.m_ucFilterType == defFILTER_TYPE_EVENT )
    {
        m_omMsgDirection.EnableWindow( FALSE );
        sFilterDisplyInfo.m_omStrMsgIDFrom = sFilter.m_omEventName;
        // Update Image Index
        sFilterDisplyInfo.m_nImageIndex = defFILTER_IMAGE_INDEX_DB;
    }
    else if(sFilter.m_ucFilterType == defFILTER_TYPE_ECU)
    {
        sFilterDisplyInfo.m_nImageIndex = defFILTER_IMAGE_INDEX_ECU;
    }
    // Message Direction
    switch( sFilter.m_eDrctn )
    {
        case DIR_ALL : // All Message
            sFilterDisplyInfo.m_omStrMsgDirection = _(defSTR_SELECTION_ALL);
            break;
        case DIR_TX: // Tx Message
            sFilterDisplyInfo.m_omStrMsgDirection = _(defSTR_MSG_DIR_TX);
            break;
        case DIR_RX: // Rx
            sFilterDisplyInfo.m_omStrMsgDirection = _(defSTR_MSG_DIR_RX);
            break;
        default:
            ASSERT( FALSE );
            sFilterDisplyInfo.m_omStrMsgDirection = "";
            break;
    }

    // Channel Number
    if( sFilter.m_eChannel == CAN_CHANNEL_ALL )
    {
        sFilterDisplyInfo.m_omStrMsgChannel = _(defSTR_SELECTION_ALL);
    }
    else
    {
        // Format Channel ID String Say 1,2,...
        std::ostringstream oss;
        oss << std::dec << sFilter.m_eChannel;
        sFilterDisplyInfo.m_omStrMsgChannel = oss.str();
    }
}

/**
 * \param[in] sFilter Filter Details
 * Updates the selected item in the list with the given filter
 * details.
 */
BOOL CFilterConfigDlg::bUpdateSelectedItem(SFILTER_CAN& sFilter)
{

    // Check whether is DB Message.

    if ( m_pClusterConfig != nullptr )
    {
        IFrame* pMessage = nullptr;
        m_pClusterConfig->GetFrame( CAN, 0, sFilter.m_dwMsgIDFrom, nullptr, &pMessage );

        if ( nullptr != pMessage )
        {
            // Update Message Type Combobox and the struct
            CANFrameProps frameProps;
            pMessage->GetProperties( frameProps );

            // Updating the Id type (Standard or Extended)
            if ( frameProps.m_canMsgType == eCan_Standard )
            {
                sFilter.m_byIDType = TYPE_ID_CAN_STANDARD;
                m_omMsgIDType.SetCurSel( 0 );
            }
            else
            {
                sFilter.m_byIDType = TYPE_ID_CAN_EXTENDED;
                m_omMsgIDType.SetCurSel( 1 );
            }

            /*sFilter.m_byMsgType =
            static_cast<UCHAR>( pMessage->m_bMessageFrameFormat );*/
        }
    }
    // Check for selection in the filter list


    BOOL bIsAnytingUpdated = FALSE;
    // If Selected Item is valid
    if( m_nSelecetedFilterIndex != -1 )
    {
        // Update Text
        CString omStrFilterName = m_omLstcFilterList.GetItemText(
                                      m_nSelecetedNamedFilterIndex, 0 );
        PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                    m_psFilterApplied->m_psFilters,
                                    m_psFilterApplied->m_ushTotal,
                                    omStrFilterName.GetBuffer(MAX_PATH));
        if( psTempSet != nullptr )
        {
            SFILTER_CAN* psDest = (SFILTER_CAN*)(psTempSet->m_psFilterInfo) + m_nSelecetedFilterIndex;
            *psDest = sFilter;
            // Update UI
            // Get Display Text
            SFILTERDISPLAYINFO sDisplayInfo;
            // Get Display Text
            vFormatDisplayString( sFilter, sDisplayInfo );
            // Update Display Text
            vUpdateFilterListDetails( m_nSelecetedFilterIndex,
                                      sDisplayInfo );
            bIsAnytingUpdated = TRUE;
        }
    }
    return bIsAnytingUpdated;
}

/**
 * Adds the user configured filter in to the selected
 * named filter. If validation fails then return FALSE and
 * nothing will be added in to the list
 */
BOOL CFilterConfigDlg::bAddNewItem(SFILTER_CAN& sFilter)
{
    BOOL bIsAnytingUpdated = FALSE;
    // Get Selected Filter Pointer
    // Update Text
    CString omStrFilterName = m_omLstcFilterList.GetItemText(
                                  m_nSelecetedNamedFilterIndex, 0 );
    PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                m_psFilterApplied->m_psFilters,
                                m_psFilterApplied->m_ushTotal,
                                omStrFilterName.GetBuffer(MAX_PATH));
    if( psTempSet != nullptr )
    {
        //SFILTER_CAN* psFilter =
        SFILTER_CAN* psFilter = new SFILTER_CAN[psTempSet->m_ushFilters + 1];
        SFILTER_CAN* psTemp = nullptr;
        for (USHORT i = 0; i < psTempSet->m_ushFilters; i++)
        {
            psTemp = (SFILTER_CAN*)(psTempSet->m_psFilterInfo) + i;
            psFilter[i] = *psTemp;
            psTemp->vClear();
        }
        psTemp = (SFILTER_CAN*)(psTempSet->m_psFilterInfo);
        delete[] psTemp;
        psFilter[psTempSet->m_ushFilters++] = sFilter;
        psTempSet->m_psFilterInfo = psFilter;
        // Update UI
        // Get Display Text
        SFILTERDISPLAYINFO sDisplayInfo;
        // Get Display Text
        vFormatDisplayString( sFilter, sDisplayInfo );
        // Insert in to the list at the end
        int nIndex = m_omLstcFilterDetails.GetItemCount();
        m_omLstcFilterDetails.InsertItem( nIndex,
                                          "",
                                          sDisplayInfo.m_nImageIndex );
        // Update Display Text
        vUpdateFilterListDetails( nIndex,
                                  sDisplayInfo );
        bIsAnytingUpdated = TRUE;
    }
    // Return the result
    return bIsAnytingUpdated;
}

/**
 * Adds the user configured filter in to the selected
 * named filter. If validation fails then return FALSE and
 * nothing will be added in to the list
 */
BOOL CFilterConfigDlg::bAddNewItem(SFILTER_LIN& sFilter)
{
    BOOL bIsAnytingUpdated = FALSE;
    // Get Selected Filter Pointer
    // Update Text
    CString omStrFilterName = m_omLstcFilterList.GetItemText(
                                  m_nSelecetedNamedFilterIndex, 0 );
    PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                m_psFilterAppliedLin->m_psFilters,
                                m_psFilterAppliedLin->m_ushTotal,
                                omStrFilterName.GetBuffer(MAX_PATH));
    if( psTempSet != nullptr )
    {
        SFILTER_LIN* psFilter = new SFILTER_LIN[psTempSet->m_ushFilters + 1];
        SFILTER_LIN* psTemp = nullptr;
        for (USHORT i = 0; i < psTempSet->m_ushFilters; i++)
        {
            psTemp = (SFILTER_LIN*)(psTempSet->m_psFilterInfo) + i;
            psFilter[i] = *psTemp;
            psTemp->vClear();
        }
        psTemp = (SFILTER_LIN*)(psTempSet->m_psFilterInfo);
        delete[] psTemp;
        psFilter[psTempSet->m_ushFilters++] = sFilter;
        psTempSet->m_psFilterInfo = psFilter;
        // Update UI
        // Get Display Text
        SFILTERDISPLAYINFO sDisplayInfo;
        // Get Display Text
        vFormatDisplayString( sFilter, sDisplayInfo );
        // Insert in to the list at the end
        int nIndex = m_omLstcFilterDetails.GetItemCount();
        m_omLstcFilterDetails.InsertItem( nIndex,
                                          "",
                                          sDisplayInfo.m_nImageIndex );
        // Update Display Text
        vUpdateFilterListDetails( nIndex,
                                  sDisplayInfo );
        bIsAnytingUpdated = TRUE;
    }
    // Return the result
    return bIsAnytingUpdated;
}


/**
 * This function will be called when user selects Add button.
 */
void CFilterConfigDlg::OnBtnAddFilterToList()
{
    if(m_eCurrBUS == CAN)
    {
        // Get the Filter Details from the UI
        SFILTER_CAN sFilter;
        BOOL bDataValid = bGetFilterData(sFilter);
        if( bDataValid == TRUE )
        {
            // Add the Filter to the selected Named Filter
            bAddNewItem( sFilter );
            vEnableDisableButtons();
        }
        else
        {
            // Disable Add Button
            m_omAddFilter.EnableWindow( FALSE );
        }
    }
    else if(m_eCurrBUS == LIN)
    {
        // Get the Filter Details from the UI
        SFILTER_LIN sFilter;
        BOOL bDataValid = bGetFilterData(sFilter);
        if( bDataValid == TRUE )
        {
            // Add the Filter to the selected Named Filter
            bAddNewItem( sFilter );
            vEnableDisableButtons();
        }
        else
        {
            // Disable Add Button
            m_omAddFilter.EnableWindow( FALSE );
        }
    }
}

/**
 * This function will be called when user selects delete button.
 * This will remove selected filter list.
 */
void CFilterConfigDlg::OnBtnDeleteFilter()
{
    // If selected index is valid
    if( m_nSelecetedFilterIndex != -1 )
    {
        CString omStrFilterName = m_omLstcFilterList.GetItemText(
                                      m_nSelecetedNamedFilterIndex, 0 );
        if(m_eCurrBUS == CAN)
        {
            PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                        m_psFilterApplied->m_psFilters,
                                        m_psFilterApplied->m_ushTotal,
                                        omStrFilterName.GetBuffer(MAX_PATH));
            if( psTempSet != nullptr )
            {
                SFILTER_CAN* psSrc = nullptr;
                SFILTER_CAN* psDest = nullptr;
                psDest = (SFILTER_CAN*)(psTempSet->m_psFilterInfo) + m_nSelecetedFilterIndex;
                psSrc  = (SFILTER_CAN*)(psTempSet->m_psFilterInfo) + (psTempSet->m_ushFilters - 1);
                *psDest = *psSrc;
                psSrc->vClear();
                psSrc = nullptr;
                --(psTempSet->m_ushFilters);

                // Disable UI Update
                m_bUpdating = TRUE;
                // Delete the selected item from the list
                m_omLstcFilterDetails.DeleteItem( m_nSelecetedFilterIndex );
                // Enable UI Update
                m_bUpdating = FALSE;
                vEnableDisableButtons();
                if( m_nSelecetedFilterIndex > psTempSet->m_ushFilters - 1 )
                {
                    // Update Selected indes
                    m_nSelecetedFilterIndex = psTempSet->m_ushFilters - 1;
                }

                // Set the selection to this item
                m_omLstcFilterDetails.SetItemState( m_nSelecetedFilterIndex,
                                                    LVIS_SELECTED | LVIS_FOCUSED,
                                                    LVIS_SELECTED | LVIS_FOCUSED );
            }
        }
        else if(m_eCurrBUS == LIN)
        {
            PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                        m_psFilterAppliedLin->m_psFilters,
                                        m_psFilterAppliedLin->m_ushTotal,
                                        omStrFilterName.GetBuffer(MAX_PATH));
            if( psTempSet != nullptr )
            {
                SFILTER_LIN* psSrc = nullptr;
                SFILTER_LIN* psDest = nullptr;
                psDest = (SFILTER_LIN*)(psTempSet->m_psFilterInfo) + m_nSelecetedFilterIndex;
                psSrc  = (SFILTER_LIN*)(psTempSet->m_psFilterInfo) + (psTempSet->m_ushFilters - 1);
                *psDest = *psSrc;
                psSrc->vClear();
                psSrc = nullptr;
                --(psTempSet->m_ushFilters);

                // Disable UI Update
                m_bUpdating = TRUE;
                // Delete the selected item from the list
                m_omLstcFilterDetails.DeleteItem( m_nSelecetedFilterIndex );
                // Enable UI Update
                m_bUpdating = FALSE;
                vEnableDisableButtons();
                if( m_nSelecetedFilterIndex > psTempSet->m_ushFilters - 1 )
                {
                    // Update Selected indes
                    m_nSelecetedFilterIndex = psTempSet->m_ushFilters - 1;
                }

                // Set the selection to this item
                m_omLstcFilterDetails.SetItemState( m_nSelecetedFilterIndex,
                                                    LVIS_SELECTED | LVIS_FOCUSED,
                                                    LVIS_SELECTED | LVIS_FOCUSED );
            }
        }
    }
}

/**
 * This will initialise all filter details related UI components
 */
void CFilterConfigDlg::vInitFilterComps()
{
    m_bDisableFilterCompUpdate = TRUE;
    // Initialise Msg ID from Field
    // Get List of database messages
    vPopulateDBMessages();
    // Set Message ID Type
    m_omMsgIDType.SetCurSel(0);
    // Message Type
    m_omMsgType.SetCurSel(0);
    // Message Direction
    m_omMsgDirection.SetCurSel(0);
    // Message Channel
    m_omMsgChannel.SetCurSel(0);

    // Init Radix Edit controls
    m_omMsgIDRangeFrom.vSetBase(BASE_HEXADECIMAL);
    m_omMsgIDRangeFrom.vSetSigned( FALSE );
    m_omMsgIDRangeFrom.SetLimitText( 8 );

    m_omMsgIDRangeTo.vSetBase(BASE_HEXADECIMAL);
    m_omMsgIDRangeTo.vSetSigned( FALSE );
    m_omMsgIDRangeTo.SetLimitText( 8 );
    // Set the selection to Message Id
    // Enable Message ID Section
    // Disable Range Controls
    // Init with default value
    m_omMsgIDRangeFrom.vSetValue( 0 );
    m_omMsgIDRangeTo.vSetValue( 1 );


    m_omMsgIDRangeFrom.EnableWindow( FALSE );
    m_omMsgIDRangeTo.EnableWindow( FALSE );
    // Enable ID Control
    m_omMsgIDFrom.EnableWindow( );
    CButton* pWnd = (CButton*)GetDlgItem(IDC_RADIO_RANGE);
    if( pWnd != nullptr )
    {
        pWnd->SetCheck( FALSE );
        m_omRadioID.SetCheck( TRUE );

    }
    m_bDisableFilterCompUpdate = FALSE;
}

/**
 * This function will be called while user selects delete all
 * button. This will ask for conformation and will clear the
 * list if user conforms.
 */
void CFilterConfigDlg::OnBtnDeleteAllFilter()
{
    if (MessageBox(_("Do you want to delete all filters from the list?"), "BUSMASTER", MB_YESNO|MB_ICONQUESTION ) == IDYES )
    {
        CString omStrFilterName = m_omLstcFilterList.GetItemText(
                                      m_nSelecetedNamedFilterIndex, 0 );
        if(m_eCurrBUS == CAN)
        {
            PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                        m_psFilterApplied->m_psFilters,
                                        m_psFilterApplied->m_ushTotal,
                                        omStrFilterName.GetBuffer(MAX_PATH));
            // Remove All Elements from the list
            if( psTempSet != nullptr )
            {
                SFILTER_CAN* psFilter = (SFILTER_CAN*)(psTempSet->m_psFilterInfo);
                delete[] psFilter;
                psTempSet->m_psFilterInfo = nullptr;
                psTempSet->m_ushFilters = 0;
                // Delete All elements from the list
                m_bUpdating = TRUE;
                m_omLstcFilterDetails.DeleteAllItems();
                vEnableDisableButtons();
                m_nSelecetedFilterIndex = -1;
                m_bUpdating = FALSE;
            }
        }
        else if(m_eCurrBUS == LIN)
        {
            PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                        m_psFilterAppliedLin->m_psFilters,
                                        m_psFilterAppliedLin->m_ushTotal,
                                        omStrFilterName.GetBuffer(MAX_PATH));
            // Remove All Elements from the list
            if( psTempSet != nullptr )
            {
                SFILTER_LIN* psFilter = (SFILTER_LIN*)(psTempSet->m_psFilterInfo);
                delete[] psFilter;
                psTempSet->m_psFilterInfo = nullptr;
                psTempSet->m_ushFilters = 0;
                // Delete All elements from the list
                m_bUpdating = TRUE;
                m_omLstcFilterDetails.DeleteAllItems();
                vEnableDisableButtons();
                m_nSelecetedFilterIndex = -1;
                m_bUpdating = FALSE;
            }
        }
    }
}

/**
 * This function will be called during the label edit start.
 * This will save the filter name before edit to cancel the edit
 * if new name alteady exist or invalid.
 */
void CFilterConfigDlg::OnBeginlabeleditLstcFilterNames( NMHDR* pNMHDR,
        LRESULT* pResult)
{
    LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
    // Store Filter Name
    if( pDispInfo->item.iSubItem == 0 )
    {
        // Store the old Filter Name
        m_omStrSelectedFilterNameBeforeEdit =
            m_omLstcFilterList.GetItemText( pDispInfo->item.iItem, 0 );
    }
    *pResult = 0;
}

/**
 * This function will be called while user selects delete of
 * Named Filer from the list. This will delete the selected
 * named filter and will clear the list. The item change handler
 * will set the latest selected filter details.
 */
void CFilterConfigDlg::OnBtnDelete()
{
    // Delete selected filter from the map
    if( m_nSelecetedNamedFilterIndex != -1 )
    {

        // Get Selected filter name
        CString omStrFilterName =
            m_omLstcFilterList.GetItemText( m_nSelecetedNamedFilterIndex, 0 );
        if(m_eCurrBUS == CAN)
        {
            // Get the entry from the map
            const PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                              m_psFilterApplied->m_psFilters,
                                              m_psFilterApplied->m_ushTotal, omStrFilterName.GetBuffer(MAX_PATH));
            if( psTempSet != nullptr )
            {
                // Delete the item from the UI first
                m_bUpdating = TRUE;
                m_omLstcFilterList.DeleteItem( m_nSelecetedNamedFilterIndex );
                // copy the last filter set into current filter set
                psTempSet->bClone(
                    m_psFilterApplied->m_psFilters[m_psFilterApplied->m_ushTotal - 1]);
                //clear the last set
                m_psFilterApplied->m_psFilters[m_psFilterApplied->m_ushTotal - 1].vClear();
                --(m_psFilterApplied->m_ushTotal);
                int nSize = m_omLstcFilterList.GetItemCount();
                // Reset the selection to the last
                if( m_nSelecetedNamedFilterIndex >= nSize )
                {
                    m_nSelecetedNamedFilterIndex = nSize - 1;
                }

                m_bUpdating = FALSE;
                if( m_nSelecetedNamedFilterIndex != -1 )
                {
                    m_omLstcFilterList.SetItemState(
                        m_nSelecetedNamedFilterIndex,
                        LVIS_SELECTED | LVIS_FOCUSED,
                        LVIS_SELECTED | LVIS_FOCUSED );
                }
                else
                {
                    // Delete all items of Filter details List
                    m_omLstcFilterDetails.DeleteAllItems();
                    // Update Buttons
                    vEnableDisableButtons();
                    // Disable Add Button
                    m_omAddFilter.EnableWindow( FALSE );
                    CWnd* pWnd = GetDlgItem( IDC_STATIC_TO_STRING );
                    if( pWnd != nullptr )
                    {
                        pWnd->EnableWindow( FALSE );
                    }
                }
            }
        }
        else if(m_eCurrBUS == LIN)
        {
            // Get the entry from the map
            const PSFILTERSET psTempSet = SFILTERSET::psGetFilterSetPointer(
                                              m_psFilterAppliedLin->m_psFilters,
                                              m_psFilterAppliedLin->m_ushTotal, omStrFilterName.GetBuffer(MAX_PATH));
            if( psTempSet != nullptr )
            {
                // Delete the item from the UI first
                m_bUpdating = TRUE;
                m_omLstcFilterList.DeleteItem( m_nSelecetedNamedFilterIndex );
                // copy the last filter set into current filter set
                psTempSet->bClone(
                    m_psFilterAppliedLin->m_psFilters[m_psFilterAppliedLin->m_ushTotal - 1]);
                //clear the last set
                m_psFilterAppliedLin->m_psFilters[m_psFilterAppliedLin->m_ushTotal - 1].vClear();
                --(m_psFilterAppliedLin->m_ushTotal);
                int nSize = m_omLstcFilterList.GetItemCount();
                // Reset the selection to the last
                if( m_nSelecetedNamedFilterIndex >= nSize )
                {
                    m_nSelecetedNamedFilterIndex = nSize - 1;
                }

                m_bUpdating = FALSE;
                if( m_nSelecetedNamedFilterIndex != -1 )
                {
                    m_omLstcFilterList.SetItemState(
                        m_nSelecetedNamedFilterIndex,
                        LVIS_SELECTED | LVIS_FOCUSED,
                        LVIS_SELECTED | LVIS_FOCUSED );
                }
                else
                {
                    // Delete all items of Filter details List
                    m_omLstcFilterDetails.DeleteAllItems();
                    // Update Buttons
                    vEnableDisableButtons();
                    // Disable Add Button
                    m_omAddFilter.EnableWindow( FALSE );
                    CWnd* pWnd = GetDlgItem( IDC_STATIC_TO_STRING );
                    if( pWnd != nullptr )
                    {
                        pWnd->EnableWindow( FALSE );
                    }
                }
            }
        }
    }
}

/**
 * This will add a new Named filter in to the list. This will
 * iterate through the list of filters to find unique filter
 * name and will add the found entry into the named filter list
 */
void CFilterConfigDlg::OnBtnAdd()
{
    if(m_eCurrBUS == CAN)
    {
        vAddCANFilter();
    }
    else if(m_eCurrBUS == LIN)
    {
        vAddLINFilter();
    }
}

void CFilterConfigDlg::vAddCANFilter()
{
    // To Add new Named Filter
    // Find the default name appended with number to form unique entry
    CString omStrFilterName;
    BOOL bNameFound = FALSE;
    for( int nCount = 0;
            bNameFound == FALSE && nCount < defNUMBER_OF_FILTERS_SUPPORTED;
            nCount++ )
    {
        omStrFilterName.Format( defSTR_DEFAULT_FILTER_FORMAT,
                                _(defSTR_DEFAULT_FILTER_NAME),
                                nCount + 1 );
        if( SFILTERSET::psGetFilterSetPointer(m_psFilterApplied->m_psFilters, m_psFilterApplied->m_ushTotal,
                                              omStrFilterName.GetBuffer(MAX_PATH)) == nullptr)
        {
            bNameFound = TRUE;
        }
    }
    if( bNameFound == TRUE )
    {
        PSFILTERSET psNewSet = new SFILTERSET[m_psFilterApplied->m_ushTotal + 1];
        if (psNewSet != nullptr)
        {
            psNewSet[m_psFilterApplied->m_ushTotal].m_eCurrBus = CAN;
            for (USHORT i = 0; i < m_psFilterApplied->m_ushTotal; i++)
            {
                psNewSet[i].bClone(m_psFilterApplied->m_psFilters[i]);
                m_psFilterApplied->m_psFilters[i].vClear();
            }
            //now clear m_psFilters
            delete[] m_psFilterApplied->m_psFilters;
            PSFILTERSET psSetIndex =  psNewSet + m_psFilterApplied->m_ushTotal;
            if( psSetIndex != nullptr )
            {
                psSetIndex->m_sFilterName.m_bFilterType = 0;  // Stop Filter
                // Add the filter in to the map
                psSetIndex->m_sFilterName.m_acFilterName = omStrFilterName.GetBuffer(MAX_PATH);
                ++(m_psFilterApplied->m_ushTotal);

                m_psFilterApplied->m_psFilters = psNewSet;
                // Update List control
                m_bUpdating = TRUE;
                int nCount = m_omLstcFilterList.GetItemCount();
                m_omLstcFilterList.InsertItem( nCount,
                                               omStrFilterName,
                                               defFILTER_IMAGE_INDEX_FILTER );
                m_omLstcFilterList.SetItemText( nCount, 1,
                                                _(defSTR_FILTER_TYPE_STOP) );
                // Set edit property
                SLISTINFO sListInfo;
                // Init Signal Name column
                // Column 0 : User Function call
                sListInfo.m_eType = eText;
                m_omLstcFilterList.vSetColumnInfo( nCount, 0, sListInfo);
                // Init Raw Value column
                // Column 1 : Numeric Edit with Spin Control
                sListInfo.m_eType = eComboItem;
                sListInfo.m_omEntries.Add( _(defSTR_FILTER_TYPE_STOP) );
                sListInfo.m_omEntries.Add( _(defSTR_FILTER_TYPE_PASS) );
                m_omLstcFilterList.vSetColumnInfo( nCount, 1, sListInfo);

                m_bUpdating = FALSE;
                m_omLstcFilterList.SetItemState ( nCount,
                                                  LVIS_SELECTED | LVIS_FOCUSED,
                                                  LVIS_SELECTED | LVIS_FOCUSED );
                // Update Add Button in case of first addition
                if( m_omLstcFilterList.GetItemCount() == 1 )
                {
                    SFILTER_CAN sFilter;
                    if( bGetFilterData( sFilter ) == TRUE )
                    {
                        m_omAddFilter.EnableWindow();
                        CWnd* pWnd = GetDlgItem( IDC_STATIC_TO_STRING );
                        if( pWnd != nullptr )
                        {
                            pWnd->EnableWindow();
                        }
                    }
                }
            }
            else
            {
                MessageBox(_("Unable to create memory for new Filter"), "BUSMASTER", MB_OK|MB_ICONINFORMATION);
            }
        }
        else
        {
            MessageBox(_("Unable to create memory for new Filter"), "BUSMASTER", MB_OK | MB_ICONINFORMATION);
        }
    }
    else
    {
        MessageBox(_("Unable to find unique name for the filter!!"), "BUSMASTER", MB_OK | MB_ICONINFORMATION);
    }
}

void CFilterConfigDlg::vAddLINFilter()
{
    // To Add new Named Filter
    // Find the default name appended with number to form unique entry
    CString omStrFilterName;
    BOOL bNameFound = FALSE;
    for( int nCount = 0;
            bNameFound == FALSE && nCount < defNUMBER_OF_FILTERS_SUPPORTED;
            nCount++ )
    {
        omStrFilterName.Format( defSTR_DEFAULT_FILTER_FORMAT,
                                _(defSTR_DEFAULT_FILTER_NAME),
                                nCount + 1 );
        if( SFILTERSET::psGetFilterSetPointer(m_psFilterAppliedLin->m_psFilters, m_psFilterAppliedLin->m_ushTotal,
                                              omStrFilterName.GetBuffer(MAX_PATH)) == nullptr)
        {
            bNameFound = TRUE;
        }
    }
    if( bNameFound == TRUE )
    {
        PSFILTERSET psNewSet = new SFILTERSET[m_psFilterAppliedLin->m_ushTotal + 1];
        if (psNewSet != nullptr)
        {
            psNewSet[m_psFilterAppliedLin->m_ushTotal].m_eCurrBus = LIN;
            for (USHORT i = 0; i < m_psFilterAppliedLin->m_ushTotal; i++)
            {
                psNewSet[i].bClone(m_psFilterAppliedLin->m_psFilters[i]);
                m_psFilterAppliedLin->m_psFilters[i].vClear();
            }
            //now clear m_psFilters
            delete[] m_psFilterAppliedLin->m_psFilters;
            PSFILTERSET psSetIndex =  psNewSet + m_psFilterAppliedLin->m_ushTotal;
            if( psSetIndex != nullptr )
            {
                psSetIndex->m_sFilterName.m_bFilterType = 0;  // Stop Filter
                // Add the filter in to the map
                psSetIndex->m_sFilterName.m_acFilterName = omStrFilterName.GetBuffer(MAX_PATH);
                ++(m_psFilterAppliedLin->m_ushTotal);

                m_psFilterAppliedLin->m_psFilters = psNewSet;
                // Update List control
                m_bUpdating = TRUE;
                int nCount = m_omLstcFilterList.GetItemCount();
                m_omLstcFilterList.InsertItem( nCount,
                                               omStrFilterName,
                                               defFILTER_IMAGE_INDEX_FILTER );
                m_omLstcFilterList.SetItemText( nCount, 1,
                                                _(defSTR_FILTER_TYPE_STOP) );
                // Set edit property
                SLISTINFO sListInfo;
                // Init Signal Name column
                // Column 0 : User Function call
                sListInfo.m_eType = eText;
                m_omLstcFilterList.vSetColumnInfo( nCount, 0, sListInfo);
                // Init Raw Value column
                // Column 1 : Numeric Edit with Spin Control
                sListInfo.m_eType = eComboItem;
                sListInfo.m_omEntries.Add( _(defSTR_FILTER_TYPE_STOP) );
                sListInfo.m_omEntries.Add( _(defSTR_FILTER_TYPE_PASS) );
                m_omLstcFilterList.vSetColumnInfo( nCount, 1, sListInfo);

                m_bUpdating = FALSE;
                m_omLstcFilterList.SetItemState ( nCount,
                                                  LVIS_SELECTED | LVIS_FOCUSED,
                                                  LVIS_SELECTED | LVIS_FOCUSED );
                // Update Add Button in case of first addition
                if( m_omLstcFilterList.GetItemCount() == 1 )
                {
                    SFILTER_LIN sFilter;
                    if( bGetFilterData( sFilter ) == TRUE )
                    {
                        m_omAddFilter.EnableWindow();
                        CWnd* pWnd = GetDlgItem( IDC_STATIC_TO_STRING );
                        if( pWnd != nullptr )
                        {
                            pWnd->EnableWindow();
                        }
                    }
                }
                m_omMsgIDType.EnableWindow(FALSE);
                m_omMsgType.EnableWindow(FALSE);
            }
            else
            {
                MessageBox(_("Unable to create memory for new Filter"), "BUSMASTER", MB_OK | MB_ICONINFORMATION);
            }
        }
        else
        {
            MessageBox(_("Unable to create memory for new Filter"), "BUSMASTER", MB_OK | MB_ICONINFORMATION);
        }
    }
    else
    {
        MessageBox(_("Unable to find unique name for the filter!!"), "BUSMASTER", MB_OK | MB_ICONINFORMATION);
    }
}
/**
 * This will update UI controls based on selections
 */
void CFilterConfigDlg::vEnableDisableButtons()
{
    BOOL bEnable = FALSE;
    CWnd* pWnd = nullptr;
    // Named Filter List
    // Add Button
    int nItems = m_omLstcFilterList.GetItemCount();
    if( nItems < defNUMBER_OF_FILTERS_SUPPORTED )
    {
        bEnable = TRUE;
    }
    pWnd = GetDlgItem( IDC_BTN_ADD );
    if( pWnd != nullptr )
    {
        pWnd->EnableWindow( bEnable );
    }
    pWnd = nullptr;
    bEnable = FALSE;

    // Delete Button
    if( m_omLstcFilterList.GetSelectedCount() != 0 )
    {
        bEnable = TRUE;
    }
    pWnd = GetDlgItem( IDC_BTN_DELETE );
    if( pWnd != nullptr )
    {
        pWnd->EnableWindow( bEnable );
    }
    pWnd = nullptr;
    bEnable = FALSE;

    // Filter List
    // Delete Button
    if( m_omLstcFilterDetails.GetSelectedCount() != 0 )
    {
        bEnable = TRUE;
    }
    pWnd = GetDlgItem( IDC_BTN_DELETE_FILTER );
    if( pWnd != nullptr )
    {
        pWnd->EnableWindow( bEnable );
    }
    pWnd = nullptr;
    bEnable = FALSE;

    // Delete All Button
    if( m_omLstcFilterDetails.GetItemCount() != 0 )
    {
        bEnable = TRUE;
    }
    pWnd = GetDlgItem( IDC_BTN_DELETE_ALL_FILTER );
    if( pWnd != nullptr )
    {
        pWnd->EnableWindow( bEnable );
    }
    pWnd = nullptr;
    bEnable = FALSE;

    vEnableDisableFilterComps( m_omLstcFilterList.GetItemCount() != 0 );
}

/**
 * This will update Filter components enable/disable option
 * based on selection.
 */
void CFilterConfigDlg::vEnableDisableFilterComps( BOOL bEnable )
{
    CButton* pWnd = nullptr;
    // Message ID and Range Controls
    m_omRadioID.EnableWindow( bEnable );

    BOOL bRadioNonDB = FALSE;
    if(m_eCurrBUS == CAN)
    {
        m_omRadioEvent.EnableWindow(bEnable);
    }


    BOOL bRadioRange = FALSE, bRadioEvent = FALSE;

    pWnd = (CButton*)GetDlgItem( IDC_RADIO_RANGE );
    if( pWnd != nullptr )
    {
        pWnd->EnableWindow( bEnable );
        bRadioRange = pWnd->GetCheck();
    }
    bRadioEvent = m_omRadioEvent.GetCheck();

    if( m_omRadioID.GetCheck() != 0 )
    {
        m_omMsgIDFrom.EnableWindow( bEnable );
        m_omMsgIDRangeFrom.EnableWindow( FALSE );
        m_omMsgIDRangeTo.EnableWindow( FALSE );
        m_omEventType.EnableWindow(FALSE);
        m_omMsgDirection.EnableWindow( bEnable );
        // Disable the string Range To also
        /*pWnd = GetDlgItem( IDC_STATIC_TO_STRING );
        if( pWnd != nullptr )
        {
            pWnd->EnableWindow( FALSE );
        }*/
    }
    else if(bRadioRange == TRUE)
    {
        m_omMsgIDFrom.EnableWindow( FALSE );
        m_omMsgIDRangeFrom.EnableWindow( bEnable );
        m_omMsgIDRangeTo.EnableWindow( bEnable );
        m_omEventType.EnableWindow(FALSE);
        m_omMsgDirection.EnableWindow( bEnable );
        // Enable the string Range To also
        /*pWnd = GetDlgItem( IDC_STATIC_TO_STRING );
        if( pWnd != nullptr )
        {
            pWnd->EnableWindow( TRUE );
        }*/
    }
    else if(bRadioEvent == TRUE)
    {
        m_omMsgIDFrom.EnableWindow( FALSE );
        m_omMsgIDRangeFrom.EnableWindow( FALSE );
        m_omMsgIDRangeTo.EnableWindow( FALSE );
        m_omEventType.EnableWindow(bEnable);
        m_omMsgDirection.EnableWindow( FALSE );
        m_omMsgIDType.EnableWindow( FALSE );
        m_omMsgType.EnableWindow( FALSE );
    }
    if(bRadioNonDB == TRUE)
    {
        m_omMsgChannel.EnableWindow(TRUE);
        m_omEventType.EnableWindow(FALSE);
        m_omMsgDirection.EnableWindow(TRUE);
        m_omMsgIDFrom.EnableWindow(FALSE);
    }


    if(m_eCurrBUS == CAN)
    {
        if(bRadioEvent == FALSE)
        {
            // ID Type
            m_omMsgIDType.EnableWindow( bEnable );
            // MSG Type
            m_omMsgType.EnableWindow( bEnable );
        }
    }
    else if(m_eCurrBUS == LIN)
    {
        // ID Type
        m_omMsgIDType.EnableWindow( FALSE );
        // MSG Type
        m_omMsgType.EnableWindow( FALSE );

        m_omRadioEvent.EnableWindow(bEnable);
    }

    // Direction
    //m_omMsgDirection.EnableWindow( bEnable );
    // Channel
    m_omMsgChannel.EnableWindow( bEnable );
    //m_omChanMask.EnableWindow(bEnable );
}

/**
 * This function will be called while user selects OK button.
 * This will call parent class function to end dialog session.
 */
void CFilterConfigDlg::OnOkPress()
{
    // Close the dialog with OK return
    CDialog::OnOK();
}

void CFilterConfigDlg::vAdjustWidthMessageComboBox()
{
    // Find the longest string in the combo box.
    CString str;
    CSize sz;
    int dx = 0;
    TEXTMETRIC tm;
    CDC* pDC = m_omMsgIDFrom.GetDC();
    CFont* pFont = m_omMsgIDFrom.GetFont();

    // Select the listbox font, save the old font
    CFont* pOldFont = pDC->SelectObject(pFont);
    // Get the text metrics for avg char width
    pDC->GetTextMetrics(&tm);

    for (int i = 0; i < m_omMsgIDFrom.GetCount(); i++)
    {
        m_omMsgIDFrom.GetLBText(i, str);
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
    m_omMsgIDFrom.ReleaseDC(pDC);

    // Adjust the width for the vertical scroll bar and the left and right border.
    dx += ::GetSystemMetrics(SM_CXVSCROLL) + 2*::GetSystemMetrics(SM_CXEDGE);

    // Set the width of the list box so that every item is completely visible.
    m_omMsgIDFrom.SetDroppedWidth(dx);
}


void CFilterConfigDlg::OnBnClickedRadioNonDBMsg()
{
    m_omMsgChannel.EnableWindow(TRUE);
    m_omEventType.EnableWindow(FALSE);
    m_omMsgDirection.EnableWindow(TRUE);
    m_omMsgIDFrom.EnableWindow(FALSE);
}



EDIRECTION CFilterConfigDlg::eGetDirection(int nIndex)
{
    switch(nIndex)
    {
        case 0: // Rx Msg from UI
            return DIR_RX;
            break;

        case 1: // Tx Msg from UI
            return DIR_TX;
            break;

        case 2: // All Msg from UI
            return DIR_ALL;
            break;

        default:    // Invalid Option
            ASSERT( FALSE );
			return DIR_ALL;
            break;
    }
}

