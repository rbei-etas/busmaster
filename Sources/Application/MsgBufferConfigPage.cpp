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
 * \file      MsgBufferConfigPage.cpp
 * \brief     This file contain definition of all function of
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */

/* Project includes */
#include "stdafx.h"                 /* For standard include */
#include "BUSMASTER.h"              /* For App definition   */
#include "MsgBufferConfigPage.h"    /* For class definition */

extern CCANMonitorApp theApp;

IMPLEMENT_DYNCREATE(CMsgBufferConfigPage, CPropertyPage)

/**
 * \brief Standard default constructor
 *
 * Initialises data members
 */
CMsgBufferConfigPage::CMsgBufferConfigPage() :
    CPropertyPage(CMsgBufferConfigPage::IDD,
                  IDS_PPAGE_TITLE_BUFFER)
{
    m_unAppendSize = 0;
    m_unOverWriteSize = 0;
    m_unDisplayUpdateRate = 0;
    m_pnBufferSize = NULL;
}

void CMsgBufferConfigPage::vSetBufferSize(INT* pnBufferSize)
{
    m_pnBufferSize = pnBufferSize;
}

/**
 * \brief Standard destructor
 *
 * Standard destructor
 */
CMsgBufferConfigPage::~CMsgBufferConfigPage()
{
}

void CMsgBufferConfigPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_APPEND_SIZE, m_unAppendSize);
    DDV_MinMaxUInt(pDX, m_unAppendSize, defMIN_BUFFER_SIZE, defMAX_BUFFER_SIZE);
    DDX_Text(pDX, IDC_EDIT_OVERWRITE_SIZE, m_unOverWriteSize);
    DDV_MinMaxUInt(pDX, m_unOverWriteSize, defMIN_BUFFER_SIZE, defMAX_BUFFER_SIZE);
    DDX_Text(pDX, IDC_EDIT_DISPLAY_UPDATE, m_unDisplayUpdateRate);
    DDV_MinMaxUInt(pDX, m_unDisplayUpdateRate, defMIN_DISPLAY_UPDATE_TIME, defMAX_DISPLAY_UPDATE_TIME);
}

BEGIN_MESSAGE_MAP(CMsgBufferConfigPage, CPropertyPage)
    ON_BN_CLICKED(IDC_CBTN_SET_DEFAULT, OnCbtnSetDefault)
END_MESSAGE_MAP()

/**
 * \brief On OK
 *
 * Virtual Function. This handler will be called when the user
 * selects Ok from the property sheet. This will update the user
 * data in to the configuration class.
 */
void CMsgBufferConfigPage::OnOK()
{
    // Update Global Data Here
    UpdateData( TRUE );

    // Check for data boundary condition
    if( m_unAppendSize >= defMIN_BUFFER_SIZE &&
            m_unAppendSize <= defMAX_BUFFER_SIZE &&
            m_unOverWriteSize >= defMIN_BUFFER_SIZE &&
            m_unOverWriteSize <= defMAX_BUFFER_SIZE &&
            m_unDisplayUpdateRate >= defMIN_DISPLAY_UPDATE_TIME &&
            m_unDisplayUpdateRate <= defMAX_DISPLAY_UPDATE_TIME )
    {
        // Append Bufer Size
        m_pnBufferSize[defAPPEND_DATA_INDEX] = m_unAppendSize;
        // Overwrite Bufer Size
        m_pnBufferSize[defOVERWRITE_DATE_INDEX] = m_unOverWriteSize;
        // Display Update Rate
        m_pnBufferSize[ defDISPLAY_UPDATE_DATA_INDEX ] = m_unDisplayUpdateRate;
    }

    CPropertyPage::OnOK();
}

/**
 * \brief On Init Dialog
 *
 * This function will be called during initialization of dialog
 * box. This function will load the initial data from
 * configuration module
 */
BOOL CMsgBufferConfigPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    // Update the UI controls
    m_unAppendSize = m_pnBufferSize[defAPPEND_DATA_INDEX];
    m_unOverWriteSize = m_pnBufferSize[defOVERWRITE_DATE_INDEX];
    m_unDisplayUpdateRate = m_pnBufferSize[defDISPLAY_UPDATE_DATA_INDEX];
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

/**
 * \brief On Button Set Default
 *
 * This function will be called if the user selects the
 * SetDefault button. This will set the default value of the
 * various parameters
 */
void CMsgBufferConfigPage::OnCbtnSetDefault()
{
    m_unAppendSize = defDEF_APPEND_BUFFER_SIZE;
    m_unOverWriteSize = defDEF_OVERWRITE_BUFFER_SIZE;
    m_unDisplayUpdateRate = defDEF_DISPLAY_UPDATE_TIME;
    UpdateData( FALSE );
}
