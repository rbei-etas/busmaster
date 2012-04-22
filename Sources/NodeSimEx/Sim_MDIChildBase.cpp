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
 * \file      Sim_MDIChildBase.cpp
 * \brief     Implementation file for CMDIChildBase class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CMDIChildBase class
 */


#include "NodeSimEx_stdafx.h"   // For Standard Includes
#include "Hashdefines.h"
#include "Sim_MDIChildBase.h"       // For MDI Base class definition

IMPLEMENT_DYNCREATE(CMDIChildBase, CMDIChildWnd)

/*******************************************************************************
  Function Name  : CMDIChildBase
  Description    : Standard default constructor
  Member of      : CMDIChildBase
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 15.4.2005
  Modifications  :
*******************************************************************************/
CMDIChildBase::CMDIChildBase()
{
    // Default constructor.
    // Creating this class with out window identity parameter will switch off
    // saving/loading window position functionality. In this case the user
    // class should have been deriverd from **** CMDIChildWnd **** to avoid
    // one more level in class hierarchy
    ASSERT( FALSE );
}


/*******************************************************************************
  Function Name  : CMDIChildBase
  Description    : Standard default constructor
  Member of      : CMDIChildBase
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 15.4.2005
  Modifications  :
*******************************************************************************/
CMDIChildBase::CMDIChildBase(eACONFIGDETAILS eWindowIdentity)
{
    m_eWindowIdentity =  eWindowIdentity;
}

/*******************************************************************************
  Function Name  : ~CMDIChildBase
  Description    : Standard Destructor
  Member of      : CMDIChildBase
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 15.4.2005
  Modifications  :
*******************************************************************************/
CMDIChildBase::~CMDIChildBase()
{
}


BEGIN_MESSAGE_MAP(CMDIChildBase, CMDIChildWnd)
    //{{AFX_MSG_MAP(CMDIChildBase)
    ON_WM_SHOWWINDOW()
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*******************************************************************************
  Function Name  : OnShowWindow
  Input(s)       : bShow - Show or Hide command
  Output         : nStatus - Status of the window
  Functionality  : This event handler will set window postion in case of "show
                   window" and will save window position in case of "hide
                   window"
  Member of      : CMDIChildBase
  Author(s)      : Raja N
  Date Created   : 18.4.2005
  Modifications  : 
*******************************************************************************/
void CMDIChildBase::OnShowWindow(BOOL bShow, UINT nStatus) 
{
    CMDIChildWnd::OnShowWindow(bShow, nStatus);
    // If it is show window set appropriate window postion
    if (bShow == TRUE )
    {
        // Set window placement got from configuration module
        //SetWindowPlacement(&m_sWindowPlacement);
    }
    else
    {
        // Save window postion
        //vUpdateWndCo_Ords();
    }
}

/*******************************************************************************
  Function Name  : vGetWinStatus
  Input(s)       : -
  Output         : -
  Functionality  : This function will update window postion from configuration
                   module. This function will use m_eWindowIdentity member to
                   denote window type.
  Member of      : CMDIChildBase
  Author(s)      : Raja N
  Date Created   : 18.4.2005
  Modifications  : 
*******************************************************************************/
void CMDIChildBase::vGetWinStatus()
{
}

/*******************************************************************************
  Function Name  : vSaveWinStatus
  Input(s)       : -
  Output         : -
  Functionality  : This function will get window postion and will save it in to
                   the configuration module if it is different from the
                   previously saved value
  Member of      : CMDIChildBase
  Author(s)      : Raja N
  Date Created   : 18.4.2005
  Modifications  : 
*******************************************************************************/
void CMDIChildBase::vSaveWinStatus()
{
#if 0
    // Use temp structure for comparission
    WINDOWPLACEMENT sCurrentPlacement;
    // Get current window postion
    GetWindowPlacement(&sCurrentPlacement);
    // Set the flag
    sCurrentPlacement.flags |= WPF_SETMINPOSITION;
    // Check for difference
    if( memcmp( &sCurrentPlacement,
                &m_sWindowPlacement,
                sizeof(WINDOWPLACEMENT)) != 0 )
    {
        // Update memeber value
        m_sWindowPlacement = sCurrentPlacement;
        // Set flag to restore minimised postion also
        m_sWindowPlacement.flags |= WPF_SETMINPOSITION;
        // Update configuration module
        theApp.bSetData( m_eWindowIdentity, (void*)&m_sWindowPlacement);
    }
#endif
}

/*******************************************************************************
  Function Name  : OnCreate
  Input(s)       : -
  Output         : -
  Functionality  : 
  Member of      : CMDIChildBase
  Author(s)      : Raja N
  Date Created   : 18.4.2005
  Modifications  : 
*******************************************************************************/
int CMDIChildBase::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    int nCreate = -1;
    // Call base member for window creation
    if (CMDIChildWnd::OnCreate(lpCreateStruct) != -1)
    {
        // Get window status from configuration module
        vGetWinStatus();
        // Set the resule to success
        nCreate = 0;
    }
    // Return the result
    return nCreate;
}

/*******************************************************************************
  Function Name  : vUpdateWndCo_Ords
  Input(s)       : -
  Output         : -
  Functionality  : This function will save window postion in to configuration
                   module. This function will use private function
                   vSaveWinStatus to save window position
  Member of      : CMDIChildBase
  Author(s)      : Raja N
  Date Created   : 18.4.2005
  Modifications  : 
*******************************************************************************/
void CMDIChildBase::vUpdateWndCo_Ords()
{
    vSaveWinStatus();
}

/*******************************************************************************
  Function Name  : vUpdateWinStatus
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called from main frame to update
                   window status information in the configuration module while
                   loading a configuration file and while closing the window
  Member of      : CMDIChildBase
  Author(s)      : Raja N
  Date Created   : 18.4.2005
  Modifications  : 
*******************************************************************************/
void CMDIChildBase::vUpdateWinStatus()
{
    if(::IsWindow(GetSafeHwnd()))
    {
        vGetWinStatus();
        SetWindowPlacement(&m_sWindowPlacement);
    }
}