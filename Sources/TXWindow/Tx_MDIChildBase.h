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
 * \file      Tx_MDIChildBase.h
 * \brief     Interface file for CMDIChildBase class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CMDIChildBase class
 */

#pragma once

#include "Application/hashdefines.h"

class CMDIChildBase : public CMDIChildWnd
{
    DECLARE_DYNCREATE(CMDIChildBase)
public:
    // Constructor with Window Identity
    CMDIChildBase(eCONFIGDETAILS eWindowIdentity);
    // Default constructor
    CMDIChildBase();
    virtual ~CMDIChildBase();
    // To Update window with co-ordinates from Configuration module
    void vUpdateWndCo_Ords();
    // To save window co-ordinates in to Configuration module
    void vUpdateWinStatus();
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMDIChildBase)
    //}}AFX_VIRTUAL

// Implementation
protected:
    // helping function to Save and loading window postion from
    // configuration module
    void vSaveWinStatus();
    void vGetWinStatus( );
    // Generated message map functions
    //{{AFX_MSG(CMDIChildBase)
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

// Attributes
    // Make members as protected so that derivation of this class shall use
    // if it wants
    // Window placement Attribute
    WINDOWPLACEMENT m_sWindowPlacement;
    // Set the window identity parameter
    eCONFIGDETAILS  m_eWindowIdentity;
};
