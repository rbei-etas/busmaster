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
 * @brief Implementation file for CMDIChildBase class
 * @author Raja N
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CMDIChildBase class
 */

#include "TxWindow_stdafx.h"             // For Standard Includes
#include "Tx_MDIChildBase.h"       // For MDI Base class definition
#include "TxWndDataStore.h"     // For Tx Window Configuration

IMPLEMENT_DYNCREATE(CMDIChildBase, CMDIChildWnd)

CMDIChildBase::CMDIChildBase()
{
    // Default constructor.
    // Creating this class with out window identity parameter will switch off
    // saving/loading window position functionality. In this case the user
    // class should have been deriverd from **** CMDIChildWnd **** to avoid
    // one more level in class hierarchy
    ASSERT( FALSE );
}

CMDIChildBase::CMDIChildBase(eCONFIGDETAILS eWindowIdentity)
{
    m_eWindowIdentity =  eWindowIdentity;
}

CMDIChildBase::~CMDIChildBase()
{
}

BEGIN_MESSAGE_MAP(CMDIChildBase, CMDIChildWnd)
    ON_WM_SHOWWINDOW()
    ON_WM_CREATE()
END_MESSAGE_MAP()

void CMDIChildBase::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CMDIChildWnd::OnShowWindow(bShow, nStatus);
    // If it is show window set appropriate window postion
    if (bShow == TRUE )
    {
        // Set window placement got from configuration module
        SetWindowPlacement(&m_sWindowPlacement);
    }
    else
    {
        // Save window postion
        vUpdateWndCo_Ords();
    }
}

void CMDIChildBase::vGetWinStatus()
{
}

void CMDIChildBase::vSaveWinStatus()
{
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
        CTxWndDataStore::ouGetTxWndDataStoreObj().bSetTxData(TX_WINDOW_PLACEMENT, &m_sWindowPlacement);
    }
}

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

void CMDIChildBase::vUpdateWndCo_Ords()
{
    vSaveWinStatus();
}

void CMDIChildBase::vUpdateWinStatus()
{
    if(::IsWindow(GetSafeHwnd()))
    {
        vGetWinStatus();
        SetWindowPlacement(&m_sWindowPlacement);
    }
}