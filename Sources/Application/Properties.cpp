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
 * \file      Properties.cpp
 * \brief     This file contain the definition all member function of
 * \author    Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition all member function of
 */
// For all standard header file include
#include "stdafx.h"
// Definition of App class
#include "BUSMASTER.h"
// Definition of CProperties class.
#include "Properties.h"

// For Application object
extern CCANMonitorApp theApp;

/******************************************************************************/
/*  Function Name    :  CProperties                                           */    
/*  Input(s)         :                                                        */    
/*  Output           :                                                        */    
/*  Functionality    :  Constructor is called when user create an object of   */
/*                      this class. m_omStrArrayProperties data member will be*/
/*                      assing the value passed as parameter omStrStringArray */
/*  Member of        :  CProperties                                           */    
/*  Friend of        :      -                                                 */    
/*  Author(s)        :  Amitesh Bharti                                        */    
/*  Date Created     :  27.11.2002                                            */    
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  14.03.2005, Chaneged data type from CStringArray to   */
/*                      structure of CStringArray to support multiple values  */
/*                      of each type                                          */
/******************************************************************************/
CProperties::CProperties(const SPROPERTIESLIST& sProperties)
    : CDialog(CProperties::IDD, NULL)
{
    //{{AFX_DATA_INIT(CProperties)
    //}}AFX_DATA_INIT
    // Copy One after another as CString Array might be using dynamic allocation
    // So structure copy will simply copy the location
    // Copy Baud Rate
    m_sProperties.m_omBaudRate .RemoveAll();
    m_sProperties.m_omBaudRate.Copy( sProperties.m_omBaudRate );
    // Copy Database Files
    m_sProperties.m_omDatabaseFile.RemoveAll();
    m_sProperties.m_omDatabaseFile.Copy( sProperties.m_omDatabaseFile );
    // Copy DLL Files
    m_sProperties.m_omDLLFile.RemoveAll();
    m_sProperties.m_omDLLFile.Copy( sProperties.m_omDLLFile );
    // Copy Log Files
    m_sProperties.m_omLogFile.RemoveAll();
    m_sProperties.m_omLogFile.Copy( sProperties.m_omLogFile );
    // Copy Replay Files
    m_sProperties.m_omReplayFile.RemoveAll();
    m_sProperties.m_omReplayFile.Copy( sProperties.m_omReplayFile );
}
/******************************************************************************/
/*  Function Name    :  DoDataExchange                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework to exchange and validate      */
/*                         dialog data                                        */
/*                                                                            */
/*  Member of        :  CProperties                                           */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  27.11.2002                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  14.03.2005, Added Tree Control for Property display   */
/******************************************************************************/
void CProperties::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CProperties)
    DDX_Control(pDX, IDC_TREE_PROPERTIES, m_omPropertyTree);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProperties, CDialog)
    //{{AFX_MSG_MAP(CProperties)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */    
/*  Input(s)         :                                                        */    
/*  Output           :                                                        */    
/*  Functionality    :  This function will be called by frame work after      */
/*                       DoModal() function call. The edit control attached to*/
/*                      this dialog will be initialied with text in item of   */
/*                      data member m_omStrArrayProperties after inserting the*/
/*                      new line charactors.                                  */
/*  Member of        :  CProperties                                           */    
/*  Friend of        :      -                                                 */    
/*  Author(s)        :  Amitesh Bharti                                        */    
/*  Date Created     :  27.11.2002                                            */    
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  14.03.2005, Added code to create and populate tree    */
/*                      control to show application configuration             */
/******************************************************************************/
BOOL CProperties::OnInitDialog() 
{
    // Store image list creation result
    BOOL bImageListCreated = FALSE;
    // Call Parent class function to do initial needful
    CDialog::OnInitDialog();
    // Create Image List
    bImageListCreated = m_omTreeImageList.Create( IDB_BMP_PROPERTIES,
                                                  defICON_SIZE,
                                                  defICON_GROW,
                                                  WHITE_COLOR );
    // Check for Success
    if( bImageListCreated == TRUE )
    {
        // Set image list to the Tree Control
        m_omPropertyTree.SetImageList(&m_omTreeImageList, TVSIL_NORMAL);

        // Create Root Nodes
        CString omStrVersion;
        omStrVersion.LoadString(IDS_VERSION);
        omStrVersion = omStrVersion;
        HTREEITEM hRootItem =
            m_omPropertyTree.InsertItem( omStrVersion,
                                defVERSION_ICON_INDEX,
                                defVERSION_ICON_INDEX );

        m_omPropertyTree.SetItemState( hRootItem,
            TVIS_EXPANDED | TVIS_BOLD | TVIS_SELECTED,
            TVIS_EXPANDED | TVIS_BOLD | TVIS_SELECTED );

        // Create Channel Information
        HTREEITEM hChannels =
            m_omPropertyTree.InsertItem( defSTR_CHANNEL_HEADING,
                                defCHANNEL_ICON_INDEX,
                                defCHANNEL_ICON_INDEX,
                                hRootItem );
        // Insert each channel infomration
        int nChannels = m_sProperties.m_omBaudRate.GetSize();
        int nIndex; //nIndex declared outside loop
        for( nIndex = 0; nIndex < nChannels; nIndex++ )
        {
            m_omPropertyTree.InsertItem( 
                                m_sProperties.m_omBaudRate[ nIndex ],
                                defCHANNEL_ICON_INDEX,
                                defCHANNEL_ICON_INDEX,
                                hChannels );
        }

        // Create Log File Entries
        HTREEITEM hLogs =
            m_omPropertyTree.InsertItem( defSTR_LOGS_HEADING,
                                defLOGFILE_ICON_INDEX,
                                defLOGFILE_ICON_INDEX,
                                hRootItem );
        // Get Log file name count
        int nLogs = m_sProperties.m_omLogFile.GetSize();
        // Insert Log file names
        for( nIndex = 0; nIndex < nLogs; nIndex++ )
        {
            m_omPropertyTree.InsertItem(
                            m_sProperties.m_omLogFile[ nIndex ],
                            defLOGFILE_ICON_INDEX,
                            defLOGFILE_ICON_INDEX,
                            hLogs );
        }

        // Create Replay File Entries
        HTREEITEM hReplays =
            m_omPropertyTree.InsertItem( defSTR_REPLAY_HEADING,
                            defREPLAYFILE_ICON_INDEX,
                            defREPLAYFILE_ICON_INDEX,
                            hRootItem );
        // Get Replay file count
        int nReplay = m_sProperties.m_omReplayFile.GetSize();
        // Insert each items
        for( nIndex = 0; nIndex < nReplay; nIndex++ )
        {
            m_omPropertyTree.InsertItem( 
                            m_sProperties.m_omReplayFile[ nIndex ],
                            defREPLAYFILE_ICON_INDEX,
                            defREPLAYFILE_ICON_INDEX,
                            hReplays );
        }

        // Create Database File Entries
        HTREEITEM hDatabases =
            m_omPropertyTree.InsertItem( defSTR_DATABASE_HEADING,
                            defDATABASE_ICON_INDEX,
                            defDATABASE_ICON_INDEX,
                            hRootItem );
        // Insert each database infomration
        int nDatabases = m_sProperties.m_omDatabaseFile.GetSize();
        // Insert each items
        for( nIndex = 0; nIndex < nDatabases; nIndex++ )
        {
            m_omPropertyTree.InsertItem( 
                            m_sProperties.m_omDatabaseFile[ nIndex ],
                            defDATABASE_ICON_INDEX,
                            defDATABASE_ICON_INDEX,
                            hDatabases );
        }

        // Create DLL File Entries
        HTREEITEM hDLLs =
            m_omPropertyTree.InsertItem( defSTR_USER_PROG_HEADING,
                            defUSERDLL_ICON_INDEX,
                            defUSERDLL_ICON_INDEX,
                            hRootItem );
        // Get the size
        int nDllFiles = m_sProperties.m_omDLLFile.GetSize();
        // Insert each items
        for( nIndex = 0; nIndex < nDllFiles; nIndex++ )
        {
            m_omPropertyTree.InsertItem( 
                            m_sProperties.m_omDLLFile[ nIndex ],
                            defUSERDLL_ICON_INDEX,
                            defUSERDLL_ICON_INDEX,
                            hDLLs);
        }
    }

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
