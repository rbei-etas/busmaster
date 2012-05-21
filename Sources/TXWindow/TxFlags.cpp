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
 * \file      TxFlags.cpp
 * \brief     Contains implementaion of CFlags class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains implementaion of CFlags class
 */
// For all standard header file include
#include "TxWindow_stdafx.h"
#include "TxFlags.h"



/******************************************************************************/
/*  Function Name    :  CFlags                                                */
/*  Input(s)         :  PSTOOLBARINFO psToolBarInfo                           */
/*  Output           :                                                        */
/*  Functionality    :  Constructor is called when user create an object of   */
/*                      this class. Initialisation of all data members        */
/*  Member of        :  CFlags                                                */
/*  Friend of        :      -                                                 */
/*  Author(s)        :
/*  Date Created     :
/******************************************************************************/
CFlags::CFlags()
{
    m_bHex          = FALSE;
    m_bSendMsgOn    = FALSE;
    m_bConnected    = FALSE;
}
/******************************************************************************/
/*  Function Name    :  ~CFlags                                               */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This destractor is called when object is getting      */
/*                      destroyed                                             */
/*  Member of        :  CFlags                                                */
/*  Friend of        :      -                                                 */
/*  Author(s)        :
/*  Date Created     :
/*  Modifications    :
/*
/******************************************************************************/
CFlags::~CFlags()
{
}
/******************************************************************************/
/*  Function Name    :  vSetFlagStatus                                        */
/*  Input(s)         :  Flag identifer and value to set that flag             */
/*  Output           :                                                        */
/*  Functionality    :  This function set the correspoding flag whose         */
/*                      with value passed as nValue parameter.The eWhichFlag  */
/*                      identified the flag to be set.                        */
/*  Member of        :  CFlags                                                */
/*  Friend of        :      -                                                 */
/*  Author(s)        :
/*  Date Created     :
/******************************************************************************/
VOID CFlags::vSetFlagStatus(eTXWNDFLAG eWhichFlag, INT nValue)
{
    m_omFlagCritSec.Lock();

    switch( eWhichFlag )
    {
        case TX_HEX :
            m_bHex = nValue;
            break;

        case TX_SENDMESG :
            m_bSendMsgOn = nValue;
            break;

        case TX_CONNECTED :
            m_bConnected = nValue;
            break;

        default:
            //Invalid flag enum value
            ASSERT ( FALSE );
    }

    m_omFlagCritSec.Unlock();
}
/******************************************************************************/
/*  Function Name    :  nGetFlagStatus                                        */
/*  Input(s)         :  Flag identifer                                        */
/*  Output           :  State of flag                                         */
/*  Functionality    :  This function returns the state of flag The eWhichFlag*/
/*                      identified the flag.                                  */
/*  Member of        :  CFlags                                                */
/*  Friend of        :      -                                                 */
/*  Author(s)        :
/*  Date Created     :
/******************************************************************************/
int CFlags::nGetFlagStatus(eTXWNDFLAG eWhichFlag)
{
    INT nRetValue = -1;
    m_omFlagCritSec.Lock();

    switch( eWhichFlag )
    {
        case TX_HEX :
            nRetValue  = m_bHex;
            break;

        case TX_SENDMESG :
            nRetValue  = m_bSendMsgOn;
            break;

        case TX_CONNECTED :
            nRetValue  = m_bConnected;
            break;

        default:
            // Invalid flag enum value
            ASSERT ( FALSE );
    }

    m_omFlagCritSec.Unlock();
    return nRetValue;
}
