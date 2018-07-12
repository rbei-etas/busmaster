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
 * \file      MsgFilterConfigPage.cpp
 * \brief     Implementation file for CMsgFilterConfigPage class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CMsgFilterConfigPage class
 */

#include "stdafx.h"                 // For standard includes
#include "common.h"
#include "BUSMASTER.h"            // For application class decl
#include "MsgFilterConfigPage.h"    // For Filter conf property page class decl
#include "Filter/Filter_extern.h"   // For filter

#define defFILTER_IMAGE_INDEX           2

#define defSTR_WINDOW_TITLE             "Filter"
#define defSTR_FILTER_WINDOW_TITLE      "Configure Message Display filter"
// For application object
extern CCANMonitorApp theApp;
// Message Thread Handle
//extern DWORD GUI_dwThread_MsgDisp;

IMPLEMENT_DYNCREATE(CMsgFilterConfigPage, CPropertyPage)

/*******************************************************************************
  Function Name  : CMsgFilterConfigPage
  Description    : Constructor overload
  Member of      : CMsgFilterConfigPage
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 8.7.2005
  Modifications  :
*******************************************************************************/

CMsgFilterConfigPage::CMsgFilterConfigPage(std::string caption, filterDetails* filterDetails, HWND /*hMsgWnd*/)
    :CPropertyPage(CMsgFilterConfigPage::IDD, IDS_PPAGE_TITLE_MSG_FILTER), mCaption(caption)
{
    mFilterDetails = filterDetails;
}
/*******************************************************************************
  Function Name  : CMsgFilterConfigPage
  Description    : Standard default constructor
  Member of      : CMsgFilterConfigPage
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 8.7.2005
  Modifications  :
*******************************************************************************/
CMsgFilterConfigPage::CMsgFilterConfigPage() :
    CPropertyPage(CMsgFilterConfigPage::IDD, IDS_PPAGE_TITLE_MSG_FILTER )
{
    //{{AFX_DATA_INIT(CMsgFilterConfigPage)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_hMsgWnd = nullptr;
    m_psFilterConfigured = nullptr;
}
/*******************************************************************************
  Function Name  : ~CMsgFilterConfigPage
  Description    : Standard Destructor
  Member of      : CMsgFilterConfigPage
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 8.7.2005
  Modifications  :
*******************************************************************************/
CMsgFilterConfigPage::~CMsgFilterConfigPage()
{
}

/*******************************************************************************
  Function Name  : DoDataExchange
  Input(s)       : pDX - Pointer to data exchange object
  Output         : -
  Functionality  : This will map UI controls will mapped data members. This will
                   be called during UpdateData funtion call for data update
  Member of      : CMsgFilterConfigPage
  Author(s)      : Raja N
  Date Created   : 8.7.2005
  Modifications  :
*******************************************************************************/
void CMsgFilterConfigPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMsgFilterConfigPage)
    DDX_Control(pDX, IDC_LST_DISPLAY_FILTER_LIST, m_omLstcFilterList);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgFilterConfigPage, CPropertyPage)
    //{{AFX_MSG_MAP(CMsgFilterConfigPage)
    ON_BN_CLICKED(IDC_BTN_CONFIGURE, OnBtnConfigure)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*******************************************************************************
  Function Name  : OnInitDialog
  Input(s)       : -
  Output         : FALSE - If focus is set to Any UI control explicitly
  Functionality  : Initialises dialog's UI components
  Member of      : CMsgFilterConfigPage
  Author(s)      : Raja N
  Date Created   : 8.7.2005
  Modifications  :
*******************************************************************************/
BOOL CMsgFilterConfigPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    // Create Image List
    bCreateImageList();
    // Create Filter List UI
    vCreateFilterUIList();

    // Init data list
    //vInitFilterDataList();
    // Populate Filter List
    vInitFilterUIList();
    // Update Title
    SetWindowText( _(defSTR_WINDOW_TITLE) );

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

/*******************************************************************************
  Function Name  : bCreateImageList
  Input(s)       : -
  Output         : -
  Functionality  : To create image list used in the dialog
  Member of      : CMsgFilterConfigPage
  Author(s)      : Raja N
  Date Created   : 8.7.2005
  Modifications  :
*******************************************************************************/
BOOL CMsgFilterConfigPage::bCreateImageList()
{
    // Flag to indicate result
    BOOL bReturn = FALSE;
    // Create Image List
    if( m_omImageList.Create( IDB_BMP_FILTER, 16,
                              1, defCOLOR_WHITE ) == TRUE )
    {
        // If successful set the result
        bReturn = TRUE;
    }
    // Return the result
    return bReturn;
}

/*******************************************************************************
  Function Name  : vCreateFilterUIList
  Input(s)       : -
  Output         : -
  Functionality  : To create UI list for filter
  Member of      : CMsgFilterConfigPage
  Author(s)      : Raja N
  Date Created   : 8.7.2005
  Modifications  :
*******************************************************************************/
void CMsgFilterConfigPage::vCreateFilterUIList()
{
    // Create One Colunm "Filter Name"
    m_omLstcFilterList.InsertColumn(0, _(defSTR_FILTER_NAME_COLUMN) );
    // Set Image List if list is valid
    if(m_omImageList.m_hImageList != nullptr )
    {
        m_omLstcFilterList.SetImageList( &m_omImageList, LVSIL_SMALL );
    }
    // Set the width to occupy whole size
    CRect omRect;
    m_omLstcFilterList.GetWindowRect( omRect );
    int nWidth = static_cast<int>( omRect.Width() * 0.95 );
    m_omLstcFilterList.SetColumnWidth( 0, nWidth );
    // Enable Checkbox
    m_omLstcFilterList.SetExtendedStyle(
        m_omLstcFilterList.GetExtendedStyle() | LVS_EX_CHECKBOXES );
}

/*******************************************************************************
  Function Name  : vInitFilterDataList
  Input(s)       : -
  Output         : -
  Functionality  : To initialise message filter list data
  Member of      : CMsgFilterConfigPage
  Author(s)      : Raja N
  Date Created   : 8.7.2005
  Modifications  :
*******************************************************************************/
void CMsgFilterConfigPage::vInitFilterDataList()
{
#if 0
    // Get the data from config module and update local list
    CModuleFilterArray* psData = &m_omFilterList;
    // Update Filter List
    theApp.bGetData( MSG_DISPLAY_FILTER_DETAILS, (void** )&psData );
#endif
}

/*******************************************************************************
  Function Name  : vInitFilterUIList
  Input(s)       : -
  Output         : -
  Functionality  : To populate UI Filter list
  Member of      : CMsgFilterConfigPage
  Author(s)      : Raja N
  Date Created   : 8.7.2005
  Modifications  :
*******************************************************************************/
void CMsgFilterConfigPage::vInitFilterUIList()
{
    // Remove existing items if any
    m_omLstcFilterList.DeleteAllItems();

    int i = 0;
for (auto filter : mFilterDetails->mFitersApplied)
    {
        m_omLstcFilterList.InsertItem(i,
                                      filter.first.c_str(),
                                      defFILTER_IMAGE_INDEX);

        // Update Check box status
        m_omLstcFilterList.SetCheck(i, filter.second);
        i++;
    }
    return;
}

/*******************************************************************************
  Function Name  : vUpdateDataFromUI
  Input(s)       : -
  Output         : -
  Functionality  : To update filter list with UI data
  Member of      : CMsgFilterConfigPage
  Author(s)      : Raja N
  Date Created   : 8.7.2005
  Modifications  :
*******************************************************************************/
void CMsgFilterConfigPage::vUpdateDataFromUI()
{
    // Update List from UI
    int nSize = m_omLstcFilterList.GetItemCount();
    for( int nIndex = 0; nIndex < nSize; nIndex++ )
    {
        // Filter Name from UI

        CString omFilterName = m_omLstcFilterList.GetItemText(nIndex, 0);
        auto filter = mFilterDetails->mFitersApplied.find(omFilterName.GetBuffer(0));
        if (mFilterDetails->mFitersApplied.end() != filter)
        {
            filter->second = (m_omLstcFilterList.GetCheck(nIndex)!=0);
        }
    }
}

void CMsgFilterConfigPage::updateFilterConfiguration(CMainEntryList& SrcList)
{
    mUpdatedFilters.clear();

    const SMAINENTRY& sMainEntry = SrcList.GetHead();
    POSITION pos = sMainEntry.m_odSelEntryList.GetHeadPosition();
    while (pos)
    {
        SSUBENTRY sSubEntry = sMainEntry.m_odSelEntryList.GetNext(pos);
        auto filter = mFilterDetails->mFitersApplied.find(sSubEntry.m_omSubEntryName.GetBuffer(0));
        if (filter != mFilterDetails->mFitersApplied.end())
        {
            mUpdatedFilters[sSubEntry.m_omSubEntryName.GetBuffer(0)] = filter->second;
        }
        else
        {
            mUpdatedFilters[sSubEntry.m_omSubEntryName.GetBuffer(0)] = true;
        }
    }
    mFilterDetails->mFitersApplied = mUpdatedFilters;
}
void CMsgFilterConfigPage::populateFilterConfiguration(CMainEntryList& DestList)
{

    DestList.RemoveAll();

    SMAINENTRY sMainEntry;
    sMainEntry.m_omMainEntryName = mCaption.c_str();

for (auto filter : mFilterDetails->mFiltersConfigured)
    {
        SSUBENTRY sSubEntry;
        sSubEntry.m_omSubEntryName.Format("%s", filter.c_str());

        if (mFilterDetails->mFitersApplied.find(filter) != mFilterDetails->mFitersApplied.end())
        {
            sMainEntry.m_odSelEntryList.AddTail(sSubEntry);
        }
        else
        {
            sMainEntry.m_odUnSelEntryList.AddTail(sSubEntry);
        }
    }
    DestList.AddTail(sMainEntry);
}

static void vPopulateFilterApplied(const SFILTERAPPLIED_CAN* psFilterConfigured, SFILTERAPPLIED_CAN& sFilterApplied, CMainEntryList& SrcList)
{
    if ( psFilterConfigured == nullptr )
    {
        return;
    }

    const SMAINENTRY& sMainEntry = SrcList.GetHead();
    int nCount  = sMainEntry.m_odSelEntryList.GetCount();

    SFILTERAPPLIED_CAN    sTempAppliedFilter;
    sTempAppliedFilter.bClone(sFilterApplied);
    sFilterApplied.vClear();
    sFilterApplied.m_psFilters = new SFILTERSET[nCount];

    POSITION pos = sMainEntry.m_odSelEntryList.GetHeadPosition();
    while (pos)
    {
        SSUBENTRY sSubEntry = sMainEntry.m_odSelEntryList.GetNext(pos);
        const PSFILTERSET psTemp = SFILTERSET::psGetFilterSetPointer(psFilterConfigured->m_psFilters,
                                   psFilterConfigured->m_ushTotal, sSubEntry.m_omSubEntryName.GetBuffer(MAX_PATH));
        if (psTemp != nullptr)
        {
            sFilterApplied.m_psFilters[sFilterApplied.m_ushTotal].bClone(*psTemp);
            sFilterApplied.m_ushTotal++;
        }
    }

    //restore the enable flag for all filters
    for(int nAppFilterCnt = 0; nAppFilterCnt < sFilterApplied.m_ushTotal; nAppFilterCnt++)
    {
        for(int nTempFilCnt = 0; nTempFilCnt < sTempAppliedFilter.m_ushTotal; nTempFilCnt++ )
        {
            if((&sTempAppliedFilter.m_psFilters[nTempFilCnt]) != nullptr)
            {
                if(sFilterApplied.m_psFilters[nAppFilterCnt].m_sFilterName.m_acFilterName ==
                        sTempAppliedFilter.m_psFilters[nTempFilCnt].m_sFilterName.m_acFilterName)
                {
                    ((sFilterApplied.m_psFilters)+ nAppFilterCnt) ->m_bEnabled
                        =  ((sTempAppliedFilter.m_psFilters)+ nTempFilCnt) ->m_bEnabled;
                }
            }
        }
    }
}
/*******************************************************************************
  Function Name  : OnBtnConfigure
  Input(s)       : -
  Output         : -
  Functionality  : To show popup filter configuration dialog
  Member of      : CMsgFilterConfigPage
  Author(s)      : Raja N
  Date Created   : 8.7.2005
  Modifications  :
*******************************************************************************/
void CMsgFilterConfigPage::OnBtnConfigure()
{
    // Update User Modifications
    vUpdateDataFromUI();
    CMainEntryList DestList;

    populateFilterConfiguration(DestList);
    //Show dialog
    if (Filter_ShowSelDlg(this, &DestList) == IDOK)
    {
        updateFilterConfiguration(DestList);
        //vPopulateFilterApplied((SFILTERAPPLIED_CAN*)m_psFilterConfigured, m_sFilterAppliedCan, DestList);
        vInitFilterUIList();
    }
}

/*******************************************************************************
  Function Name  : OnOK
  Input(s)       : -
  Output         : -
  Functionality  : This will be called when ENTER key is pressed or Ok button is
                   selected. This will update the data from UI and will update
                   configuration module with the changes.
  Member of      : CMsgFilterConfigPage
  Author(s)      : Raja N
  Date Created   : 8.7.2005
  Modifications  :
*******************************************************************************/
void CMsgFilterConfigPage::OnOK()
{
    // Update List from UI
    vUpdateDataFromUI();
    CPropertyPage::OnOK();
}

