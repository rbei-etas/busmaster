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
 * \file      Flags.cpp
 * \brief     Contains implementaion of CFlags class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains implementaion of CFlags class
 */

// For all standard header file include
#include "NodeSimEx_stdafx.h"
// Definition of CFlags class
#include "Flags.h"

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
    m_bDllLoaded        = FALSE;
    m_bTimerButtonOn    = FALSE;
    m_bFunctionEditorOn = FALSE;
    m_bMsgHandlerOn     = FALSE;
    m_bKeyHandlerOn     = FALSE;
    m_bErrorHandlerOn   = FALSE;
    m_bDLLHandlerOn     = FALSE;
    m_bALLHandler       = FALSE;
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
VOID CFlags::vSetFlagStatus(eSIMSYSFLAG eWhichFlag, INT nValue)
{
    m_omCriticalSec.Lock();

    switch( eWhichFlag )
    {
        case H_DLLLOADED :
            m_bDllLoaded = nValue;
            break;
        case H_TIMERBUTTON :
            m_bTimerButtonOn = nValue;
            break;
        case H_MSGHANDLERBUTTON :
            m_bMsgHandlerOn = nValue;
            break;
        case H_KEY_HANDLER_ON:
            m_bKeyHandlerOn = nValue;
            break;
        case H_ERROR_HANDLER:
            m_bErrorHandlerOn = nValue;
            break;
        case H_DLL_HANDLER:
            m_bDLLHandlerOn = nValue;
            break;
        case H_ALL_HANDLER:
            m_bALLHandler = nValue;
            break;
        case H_CONNECTED:
            break;
        default:
             //Invalid flag enum value
            ASSERT ( FALSE );    
    }
    m_omCriticalSec.Unlock();
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
int CFlags::nGetFlagStatus(eSIMSYSFLAG eWhichFlag)
{
    INT nRetValue = -1;

    m_omCriticalSec.Lock();

    switch( eWhichFlag )
    {
        case H_DLLLOADED :
            nRetValue = m_bDllLoaded;
            break;
        case H_TIMERBUTTON :
            nRetValue = m_bTimerButtonOn;
            break;
        case H_MSGHANDLERBUTTON :
            nRetValue = m_bMsgHandlerOn;
            break;
        case H_KEY_HANDLER_ON:
            nRetValue  = m_bKeyHandlerOn;
            break;
        case H_ERROR_HANDLER:
            nRetValue  = m_bErrorHandlerOn;
            break;
        case H_DLL_HANDLER:
            nRetValue  = m_bDLLHandlerOn;
            break;
        case H_ALL_HANDLER:
            nRetValue  = m_bALLHandler;
            break;
        case H_CONNECTED:
            nRetValue = TRUE;
            break;
        default:
            // Invalid flag enum value
            ASSERT ( FALSE );
    }
    m_omCriticalSec.Unlock();

    return nRetValue;
}
