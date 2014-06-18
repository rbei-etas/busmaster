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
 * @brief     Interface file for CMDIChildBase class
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CMDIChildBase class
 */

#pragma once

#include "Application/hashdefines.h"

class CMDIChildBase : public CMDIChildWnd
{
public:
    CMDIChildBase();
    virtual ~CMDIChildBase();

    /** Constructor with Window Identity */
    CMDIChildBase(eCONFIGDETAILS eWindowIdentity);

    /**
     * To Update window with co-ordinates from Configuration module
     *
     * This function will save window postion in to configuration
     * module. This function will use private function
     * vSaveWinStatus to save window position
     */
    void vUpdateWndCo_Ords();

    /**
     * To save window co-ordinates in to Configuration module
     *
     * This function will be called from main frame to update
     * window status information in the configuration module while
     * loading a configuration file and while closing the window
     */
    void vUpdateWinStatus();

protected:
    /**
     * helping function to Save window postion from
     * configuration module
     *
     * This function will get window postion and will save it in to
     * the configuration module if it is different from the
     * previously saved value
     */
    void vSaveWinStatus();

    /**
     * helping function to loading window postion from
     * configuration module
     *
     * This function will update window postion from configuration
     * module. This function will use m_eWindowIdentity member to
     * denote window type.
     */
    void vGetWinStatus();

    /**
     * This event handler will set window postion in case of "show
     * window" and will save window position in case of "hide
     * window"
     *
     * @param[in] bShow Show or Hide command
     * @param[in] nStatus Status of the window
     */
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    DECLARE_MESSAGE_MAP()

    /* Attributes */

    /** Window placement Attribute */
    WINDOWPLACEMENT m_sWindowPlacement;

    /** Set the window identity parameter */
    eCONFIGDETAILS  m_eWindowIdentity;

private:
    DECLARE_DYNCREATE(CMDIChildBase)
};
