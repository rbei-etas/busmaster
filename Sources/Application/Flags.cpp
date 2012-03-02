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
 * \authors   Amitesh Bharti, Amarnath Shastry, Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains implementaion of CFlags class 
 */
// For all standard header file include
#include "stdafx.h"
// Definition of App class
#include "BUSMASTER.h"
// Definition of CFlags class
#include "Flags.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCANMonitorApp theApp;

CFlags CFlags::sm_ouSingletonObj(NULL);     // Create singleton object here

/******************************************************************************/
/*  Function Name    :  CFlags                                                */
/*  Input(s)         :  PSTOOLBARINFO psToolBarInfo                           */
/*  Output           :                                                        */
/*  Functionality    :  Parameterised Constructor                             */
/*  Member of        :  CFlags                                                */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Modifications on :  15.11.2002, Parameterised Constructor                 */
/*  Modification By  :  Krishnaswamy B.N                                      */
/*  Modifications on :  24.05.2003, Initialised the Parameter m_bLogHexON     */
/*  Modification By  :  Krishnaswamy B.N                                      */
/*  Modifications on :  03.06.2003, Initialised the Parameter                 */
/*                      m_wDisplayTimeMode to absolute mode                   */
/*                      10.06.2003, Initialised the Parameter                 */
/*                      m_wLogTimeMode to absolute mode in                    */
/*  Modifications on :  03.06.2003, Initialised the Parameter                 */
/*                      m_wDisplayTimeMode to absolute mode                   */
/*                      10.06.2003, Initialised the Parameter                 */
/*                      m_wLogTimeMode to absolute mode in                    */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  17.06.2003, Initialised the Parameters for Display    */
/*                      log format and time mode                              */
/*  Modification By  :  Krishnaswamy B.N                                      */
/*  Modifications on :  19.08.2003, Initialised the Parameter m_nReplayMsgType*/
/*                      and m_bLogOverWriteON
/*  Modification By  :  Raja N                                                */
/*  Modifications on :  03.08.2004, Initialised the Parameter EnsureVisible   */
/******************************************************************************/
CFlags::CFlags(PSTOOLBARINFO psToolBarInfo)
{
    // Initialise all the flags
    // irrespective of the input parameter

    //*****************************************//
    // The states a toolbar button that can be in are
    // Pressed and Unpressed state.
    // Dependencies of the toolbar buttons
    // on other toolbar button will automatically
    // decide whether the toolbar button should be
    // in Enabled or in Disabled state
    // Hence    TRUE : Pressed
    //          FALSE : Unpressed
    // This is applicable only to the first 6 
    // toolbar buttons initialised below

    /*m_bDisplayFilterOn      = FALSE;    
    m_bLogFilterOn          = FALSE;
    m_bLogEnable            = FALSE;
    m_bTranslationMode      = FALSE;
    m_bDisplayHexON         = FALSE;
    m_bOverwriteEnable      = TRUE;
    m_wDisplayTimeMode      = eSYSTEM_MODE;
    m_bLogToWindow          = TRUE;
    m_bMsgFilterButton      = FALSE;
    m_bTranslationMode      = FALSE;
    m_bFilterUndefinedMsgs  = FALSE;
    m_bRxMsg                = FALSE;
    m_bSelectDatabaseFile   = FALSE;
    m_bDllLoaded            = FALSE;
    m_bTimerButtonOn        = FALSE;
    m_bFunctionEditorOn     = FALSE;
    m_bFilterTxMsg          = FALSE;
    m_bDbOpen               = FALSE;
    m_wReplayMode           = FALSE;
    m_bReplayFileSelected   = FALSE;
    m_bMsgHandlerOn         = FALSE;
    m_bScrolling            = TRUE;
    m_bMessageSelected      = FALSE;
    m_bTimeDelayReplayFile  = TRUE;
    m_bStopFilter           = TRUE;
    m_bConnected            = FALSE;
    m_bKeyHandlerOn         = FALSE;
    m_wControllerMode       = 1;  // Active mode
    m_bErrorHandlerOn       = FALSE;
    m_bDLLHandlerOn         = FALSE;
    m_bALLHandler           = FALSE;
    m_bParallelPortEPP      = TRUE;
    m_bLogHexON             = TRUE;
    m_wLogTimeMode          = eSYSTEM_MODE; // System mode
    m_nReplayMsgType        = eALL_MESSAGE; // default is all messages
    m_bLogOverWriteON       = TRUE;
	m_bSendSignalMSg        = FALSE;*/
    vInitializeFlags();
    // If Configuration file has this info
    // use the same
    if( psToolBarInfo != NULL )
    {
        m_bDisplayFilterOn      = psToolBarInfo->m_byMsgFilter;
        m_bLogFilterOn          = psToolBarInfo->m_byLogFilter;
        m_bLogEnable            = psToolBarInfo->m_byLogging;
        m_bTranslationMode      = psToolBarInfo->m_byMsgInterpret;
        m_bOverwriteEnable      = psToolBarInfo->m_byOverwrite;
        m_wLogTimeMode          = psToolBarInfo->m_byLogTimeMode;
        m_bLogHexON             = psToolBarInfo->m_byLogHexON;
        m_wDisplayTimeMode      = psToolBarInfo->m_byDisplayTimeMode;
        m_bDisplayHexON         = psToolBarInfo->m_byDisplayHexON;
        m_bLogOverWriteON       = psToolBarInfo->m_byLogOverWriteON;
        m_nReplayMsgType        = psToolBarInfo->m_byReplayMsgType;
        m_bScrolling            = psToolBarInfo->m_byEnsureVisible;
    }
}

/******************************************************************************/
/*  Function Name    :  CFlags                                                */    
/*  Input(s)         :  PSTOOLBARINFO psToolBarInfo                           */    
/*  Output           :                                                        */    
/*  Functionality    :  Constructor is called when user create an object of   */
/*                      this class. Initialisation of all data members        */
/*  Member of        :  CFlags                                                */    
/*  Friend of        :      -                                                 */    
/*  Author(s)        :  Amarnath Shastry,Amitesh Bharti                       */    
/*  Date Created     :  27.02.2002                                            */    
/*  Modification By  :  Amitesh Bharti                                        */  
/*  Modifications on :  06.03.2002                                            */
/*  Modification By  :  Ratnadip Choudhury                                    */  
/*  Modifications on :  25.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  16.08.2002, Added Initialisation for flag for Replay  */
/*                                    time delay option                       */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  11.10.2002,Added flags: m_bStopFilter,  m_bConnected, */
/*                                  m_bKeyHandlerOn                           */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  22.02.2003,Added flags: m_bErrorHandlerOn             */
/*                      m_bDLLHandlerOn,m_bALLHandler and m_wControllerMode   */
/*  Author(s)        :  Krishnaswamy B.N                                      */
/*  Modifications on :  24.05.2003, Initialised the Parameter m_bLogHexON     */
/*  Author(s)        :  Krishnaswamy B.N                                      */
/*  Modifications on :  03.06.2003, Initialised the Parameter
                        m_bDisplayTimeMode and m_wLogTimeMode                 */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  17.06.2003, Initialised the Parameters for Display    */
/*                      log format and time mode                              */
/*  Author(s)        :  Krishnaswamy B.N                                      */
/*  Modifications on :  19.08.2003, Initialised the Parameter m_nReplayMsgType*/
/*                       and m_bLogOverWriteON................................*/
/******************************************************************************/
CFlags::CFlags()
{
	vInitializeFlags();
}

void CFlags::vInitializeFlags()
{
	m_bDisplayFilterOn      = FALSE;    
    m_bLogFilterOn          = FALSE;
    m_bLogEnable            = FALSE;
    m_bTranslationMode      = FALSE;
    m_bDisplayHexON         = FALSE;
    m_bOverwriteEnable      = TRUE;
    m_wDisplayTimeMode      = eSYSTEM_MODE;
    m_bLogToWindow          = TRUE;
    m_bLogEnable            = FALSE;
    m_bMsgFilterButton      = FALSE;
    m_bTranslationMode      = FALSE;
    m_bFilterUndefinedMsgs  = FALSE;
    m_bRxMsg                = FALSE;
    m_bSelectDatabaseFile   = FALSE;
    m_bDllLoaded            = FALSE;
    m_bTimerButtonOn        = FALSE;
    m_bFunctionEditorOn     = FALSE;
    m_bFilterTxMsg          = FALSE;
    m_bDbOpen               = FALSE;
    m_wReplayMode           = FALSE;
    m_bReplayFileSelected   = FALSE;
    m_bMsgHandlerOn         = FALSE;
    m_bScrolling            = TRUE;
    m_bMessageSelected      = FALSE;
    m_bTimeDelayReplayFile  = TRUE;
    m_bStopFilter           = TRUE;
    m_bConnected            = FALSE;
    m_bKeyHandlerOn         = FALSE;
    m_wControllerMode       = TRUE;  // Active mode
    m_bErrorHandlerOn       = FALSE;
    m_bDLLHandlerOn         = FALSE;
    m_bALLHandler           = FALSE;
    m_bLogHexON             = TRUE;
    m_wLogTimeMode          = eSYSTEM_MODE; // absolute mode
    m_nReplayMsgType        = eALL_MESSAGE; // All messages
    m_bLogOverWriteON       = TRUE;         // over write by default
	m_bSendSignalMSg	    = FALSE;
    m_bActivatedJ1939       = FALSE; // CAN is default active bus for configuration.
}
/******************************************************************************/
/*  Function Name    :  ~CFlags                                               */    
/*  Input(s)         :                                                        */    
/*  Output           :                                                        */    
/*  Functionality    :  This destractor is called when object is getting      */
/*                      destroyed                                             */
/*  Member of        :  CFlags                                                */    
/*  Friend of        :      -                                                 */    
/*  Author(s)        :  Amarnath Shastry,Amitesh Bharti                       */    
/*  Date Created     :  27.02.2002                                            */    
/*  Modifications    :                                                        */    
/*                                                                            */    
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
/*  Author(s)        :  Amarnath Shastry,Amitesh Bharti                       */    
/*  Date Created     :  27.02.2002                                            */    
/*  Modification By  :  Amitesh Bharti, Ratnadip Choudhury                    */  
/*  Modifications on :  06.03.2002                                            */
/*  Modification By  :  Amarnath Shastry                                      */
/*  Modifications on :  23.03.2002                                            */
/*  Modification By  :  Ratnadip Choudhury                                    */
/*  Modifications on :  25.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  16.08.2002, Added Flag for Replay Time Delay Option   */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  11.10.2002,Added flags: m_bStopFilter,  m_bConnected, */
/*                                  m_bKeyHandlerOn                           */
/*  Modifications on :  02.12.2002, Amarnath S                                */
/*                      Added theApp.vSetConfigurationModified() to reflect   */
/*                      toolbar buttom status in Configuration                */ 
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  22.02.2003,Added flags: m_bErrorHandlerOn             */
/*                      m_bDLLHandlerOn,m_bALLHandler and m_wControllerMode   */
/*  Modification By  :  Krishnaswamy B.N.                                     */
/*  Modifications on :  24.05.2003,Added flags: m_bLogHexON                   */
/*  Modification By  :  Krishnaswamy B.N.                                     */
/*  Modifications on :  03.06.2003,Modified case statement for ABSOLUTE MODE  */
/*                      and added case statement for LOGTIMEMODE              */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  17.06.2003, Modfication to integrate the saving of    */
/*                      status of Display and log format and time mode        */
/*  Modification By  :  Krishnaswamy B.N.                                     */
/*  Modifications on :  19.08.2003,Added case statements for                  */
/*                      eLOGOVERWRITEMODEON and eLOGREPLAYMSGTYPE             */
/*  Modification By  :  Raja N                                                */
/*  Modifications on :  03.08.2004,Added default case statement               */
/******************************************************************************/
VOID CFlags::vSetFlagStatus(eCANMONITORFLAG eWhichFlag, INT nValue)
{
    m_omCriticalSec.Lock();

    switch( eWhichFlag )
    {
        case LOGTODISPLAY: m_bLogToWindow = nValue; break;
        case LOGTOFILE:
        {
            if (m_bLogEnable != nValue)
            {
                theApp.vSetConfigurationModified();
            }
            m_bLogEnable    = nValue ;
        }
        break;
        case REPLAYMODE: m_wReplayMode = (WORD)nValue ; break;
        case REPLAYFILESELECTED: m_bReplayFileSelected = nValue; break;
        case LOGFILTER:
            if ( m_bLogFilterOn  != nValue )
            {
                theApp.vSetConfigurationModified();
            }
            m_bLogFilterOn  = nValue ;
            break;
        case DISPLAYFILTERON :
            if ( m_bDisplayFilterOn != nValue)
            {
                theApp.vSetConfigurationModified();
            }
            m_bDisplayFilterOn = nValue;
            break;
        case TRANSLATIONMODE :
            if ( m_bTranslationMode != nValue)
            {
                theApp.vSetConfigurationModified();
            }
            m_bTranslationMode = nValue;
            break;
        case FILTERUNDEFINEDMSGS :
            m_bFilterUndefinedMsgs = nValue;
            break;
        case RXMSG :
            m_bRxMsg = nValue;
            break;
        case SELECTDATABASEFILE :
            m_bSelectDatabaseFile = nValue ;
            break;
        case DLLLOADED :
            m_bDllLoaded = nValue;
            break;
        case TIMERBUTTON :
            m_bTimerButtonOn = nValue;
            break;
        case MSGHANDLERBUTTON :
            m_bMsgHandlerOn = nValue;
            break;
        case FUNCEDITOR :
            m_bFunctionEditorOn = nValue;
            break;
        case FILTERTXMSGS :
            m_bFilterTxMsg = nValue;
            break;
        case DBOPEN :
            m_bDbOpen = nValue;
            break;
        case HEX :
            if ( m_bDisplayHexON != nValue)
            {
                theApp.vSetConfigurationModified();
            }
            m_bDisplayHexON = nValue;
            break;
        case DISPLAYTIMEMODE:
            if ( static_cast<INT>(m_wDisplayTimeMode) != nValue )
            {
                theApp.vSetConfigurationModified();
            }
            m_wDisplayTimeMode = static_cast<WORD>(nValue);
            break;
        case SCROLLING :
            // Update if there is a change
            if( m_bScrolling != nValue )
            {
                m_bScrolling = nValue;
                // Set the modification flag to true
                theApp.vSetConfigurationModified();
            }
            break;
        case SENDMESG: m_bMessageSelected = nValue; break;
        case OVERWRITE:
            if ( m_bOverwriteEnable != nValue )
            {
                theApp.vSetConfigurationModified();
            }
            m_bOverwriteEnable = nValue;
            break;
        case TIME_DELAY_REPLAY_FILE: 
                               m_bTimeDelayReplayFile = nValue; break;
        case PASS_STOP_FILTER: m_bStopFilter = nValue; break;
        case CONNECTED:        m_bConnected = nValue; break;
        case KEY_HANDLER_ON:   m_bKeyHandlerOn = nValue; break;
        case ERROR_HANDLER:    m_bErrorHandlerOn = nValue; break;
        case DLL_HANDLER:      m_bDLLHandlerOn = nValue; break;
        case ALL_HANDLER:      m_bALLHandler = nValue; break;
        case CONTROLLER_MODE:  m_wControllerMode = static_cast<WORD>(nValue); break;
        case PARALLEL_PORT_EPP:m_bParallelPortEPP = nValue; break;
        case LOGHEXON:
            if (m_bLogHexON != nValue )
            {
                theApp.vSetConfigurationModified();
            }
            m_bLogHexON = nValue;
            break;
        case LOGTIMEMODE: 
            if ( static_cast<WORD>(m_wLogTimeMode) != nValue )
            {
                theApp.vSetConfigurationModified();
            }
            m_wLogTimeMode = static_cast<WORD>(nValue);
            break;
        case eLOGOVERWRITEMODEON: m_bLogOverWriteON = nValue; break;
        case eLOGREPLAYMSGTYPE: m_nReplayMsgType = nValue; break;
		case SEND_SIGNAL_MSG: m_bSendSignalMSg = nValue; break;
        case ACTIVATED_J1939: m_bActivatedJ1939 = (BOOL) nValue; break;
        default: ASSERT(FALSE); // Invalid flag enum value
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
/*  Author(s)        :  Amarnath Shastry,Amitesh Bharti                       */    
/*  Date Created     :  27.02.2002                                            */    
/*  Modification By  :  Amitesh Bharti, Ratnadip Choudhury                    */  
/*  Modifications on :  06.03.2002                                            */
/*  Modification By  :  Amarnath Shastry                                      */
/*  Modifications on :  23.03.2002                                            */
/*  Modification By  :  Ratnadip Choudhury                                    */
/*  Modifications on :  25.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  16.08.2002, Added Flag for Replay Time Delay Option   */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  11.10.2002,Added flags: m_bStopFilter,  m_bConnected, */
/*                                  m_bKeyHandlerOn                           */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  22.02.2003,Added flags: m_bErrorHandlerOn             */
/*                      m_bDLLHandlerOn,m_bALLHandler and m_wControllerMode   */
/*  Modification By  :  Krishnaswamy B.N.                                     */
/*  Modifications on :  24.05.2003,Added flags: m_bLogHexON                   */
/*  Modification By  :  Krishnaswamy B.N.                                     */
/*  Modifications on :  06.06.2003,Added case statement for flag              */ 
/*  Modification By  :  Krishnaswamy B.N.                                     */
/*  Modifications on :  03.06.2003,Modified case statement for ABSOLUTE MODE  */
/*                      and added case statement for LOGTIMEMODE              */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  17.06.2003, Modfication to integrate the saving of    */
/*                      status of Display and log format and time mode        */
/*  Modification By  :  Krishnaswamy B.N.                                     */
/*  Modifications on :  19.08.2003,Modified case statement ...................*/
/*                      for eLOGOVERWRITEMODEON and eLOGREPLAYMSGTYPE         */
/*  Modification By  :  Raja N                                                */
/*  Modifications on :  03.08.2004,Added default case statement               */
/******************************************************************************/
int CFlags::nGetFlagStatus(eCANMONITORFLAG eWhichFlag)
{
    INT nRetValue = -1;

    m_omCriticalSec.Lock();

    switch( eWhichFlag )
    {
        case LOGTODISPLAY:
            nRetValue = m_bLogToWindow;
            break;
        case LOGTOFILE :
            nRetValue = m_bLogEnable;
            break;
        case REPLAYMODE :
            nRetValue = m_wReplayMode;
            break;
        case REPLAYFILESELECTED :
            nRetValue = m_bReplayFileSelected;
            break;
        case LOGFILTER :
            nRetValue = m_bLogFilterOn;
            break;
        case DISPLAYFILTERON :
            nRetValue = m_bDisplayFilterOn;
            break;
        case TRANSLATIONMODE :
            nRetValue = m_bTranslationMode;
            break;
        case FILTERUNDEFINEDMSGS :
            nRetValue = m_bFilterUndefinedMsgs;
            break;
        case FILTERTXMSGS :
            nRetValue = m_bFilterTxMsg;
            break;
        case RXMSG :
            nRetValue = m_bRxMsg;
            break;
        case SELECTDATABASEFILE :
            nRetValue = m_bSelectDatabaseFile;
            break;
        case DLLLOADED :
            nRetValue = m_bDllLoaded;
            break;
        case TIMERBUTTON :
            nRetValue = m_bTimerButtonOn;
            break;
        case MSGHANDLERBUTTON :
            nRetValue = m_bMsgHandlerOn;
            break;
        case FUNCEDITOR :
            nRetValue = m_bFunctionEditorOn;
            break;
        case DBOPEN :
            nRetValue = m_bDbOpen;
            break;
        case HEX :
            nRetValue = m_bDisplayHexON;
            break;
        case DISPLAYTIMEMODE :
            nRetValue = static_cast<INT>(m_wDisplayTimeMode);
            break;
        case SCROLLING :
            nRetValue = m_bScrolling;
            break;
        case SENDMESG :
            nRetValue = m_bMessageSelected;
            break;
        case OVERWRITE:
            nRetValue = m_bOverwriteEnable;
            break;
        case TIME_DELAY_REPLAY_FILE:
            nRetValue = m_bTimeDelayReplayFile ;
            break;
        case PASS_STOP_FILTER:
            nRetValue  = m_bStopFilter;
            break;
        case CONNECTED:
            nRetValue  = m_bConnected;
            break;
        case KEY_HANDLER_ON:
            nRetValue  = m_bKeyHandlerOn;
            break;
        case ERROR_HANDLER:
            nRetValue  = m_bErrorHandlerOn;
            break;
        case DLL_HANDLER:
            nRetValue  = m_bDLLHandlerOn;
            break;
        case ALL_HANDLER:
            nRetValue  = m_bALLHandler;
            break;
        case CONTROLLER_MODE:
            nRetValue  = m_wControllerMode;
            break;
        case PARALLEL_PORT_EPP:
           nRetValue  = m_bParallelPortEPP;
             break;
        case LOGHEXON:
            nRetValue = m_bLogHexON;
            break;
        case LOGTIMEMODE:
            nRetValue =  static_cast<INT>(m_wLogTimeMode);
            break;
        case eLOGOVERWRITEMODEON : 
            nRetValue = m_bLogOverWriteON;
            break;
        case eLOGREPLAYMSGTYPE : 
            nRetValue = m_nReplayMsgType;
            break;
		case SEND_SIGNAL_MSG:
			nRetValue  = m_bSendSignalMSg;
			break;
        case ACTIVATED_J1939: 
            nRetValue = (int) m_bActivatedJ1939;
            break;
        default:
            // Invalid flag enum value
            ASSERT ( FALSE );
    }
    m_omCriticalSec.Unlock();

    return nRetValue;
}
/******************************************************************************/
/*  Function Name    :  vGetToolbarButtonStatus                               */    
/*  Input(s)         :  PSTOOLBARINFO psToolBarInfo                           */    
/*  Output           :                                                        */    
/*  Functionality    :  Fills the structure                                   */
/*  Member of        :  CFlags                                                */    
/*  Friend of        :      -                                                 */    
/*  Author(s)        :  Amarnath Shastry                                      */    
/*  Date Created     :  15.11.2002                                            */    
/*  Modification By  :  Krishnaswamy B.N                                      */  
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  17.06.2003, Modification to integrate the saving of   */
/*                      status of Display and log format and time mode        */
/*  Modification By  :  Krishnaswamy B.N                                      */
/*                      19.08.2003,.Modification to integrate the saving the  */
/*                      status of m_bLogOverWriteON and m_nReplayMsgType      */
/*  Modification By  :  Raja N                                                */
/*                      19.08.2003,.Modification to integrate the saving the  */
/*                      status of Ensure Visible flag                         */
/******************************************************************************/
void CFlags::vGetToolbarButtonStatus(PSTOOLBARINFO psToolBarInfo)
{
    if ( psToolBarInfo != NULL )
    {
    psToolBarInfo->m_byMsgFilter        = static_cast<BYTE>(m_bDisplayFilterOn);
    psToolBarInfo->m_byLogFilter        = static_cast<BYTE>(m_bLogFilterOn);
    psToolBarInfo->m_byLogging          = static_cast<BYTE>(m_bLogEnable);
    psToolBarInfo->m_byMsgInterpret     = static_cast<BYTE>(m_bTranslationMode);
    psToolBarInfo->m_byOverwrite        = static_cast<BYTE>(m_bOverwriteEnable);
    psToolBarInfo->m_byLogTimeMode      = static_cast<BYTE>(m_wLogTimeMode);
    psToolBarInfo->m_byLogHexON         = static_cast<BYTE>(m_bLogHexON);
    psToolBarInfo->m_byDisplayTimeMode  = static_cast<BYTE>(m_wDisplayTimeMode);
    psToolBarInfo->m_byDisplayHexON     = static_cast<BYTE>(m_bDisplayHexON);
    psToolBarInfo->m_byLogOverWriteON   = static_cast<BYTE>(m_bLogOverWriteON);
    psToolBarInfo->m_byReplayMsgType    = static_cast<BYTE>(m_nReplayMsgType);
    psToolBarInfo->m_byEnsureVisible    = static_cast<BYTE>(m_bScrolling);
    }
}
/******************************************************************************/
/*  Function Name    :  vGetToolbarButtonStatus                               */
/*  Input(s)         :  PSTOOLBARINFO psToolBarInfo                           */
/*  Output           :                                                        */
/*  Functionality    :  Sets values of toolbar members                        */
/*  Member of        :  CFlags                                                */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  28.11.2002                                            */
/*  Modification By  :  Krishnaswamy B.N                                      */
/*  Date Modified    :  03.06.2003                                            */
/*                      Removed storing of time mode in toolbar info          */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  17.06.2003, Modification to integrate the saving of   */
/*                      status of Display and log format and time mode        */
/*  Modification By  :  Krishnaswamy B.N                                      */
/*                      19.08.2003,.Modification to integrate the saving the  */
/*                      status of m_bLogOverWriteON and m_nReplayMsgType      */
/*  Modification By  :  Raja N                                                */
/*                      02.08.2004, Uncommented the log button status         */
/*                      and added save of Ensure Visible status               */
/******************************************************************************/
void CFlags::vSetToolbarButtonStatus(PSTOOLBARINFO psToolBarInfo)
{
    if ( psToolBarInfo != NULL )
    {
        m_bDisplayFilterOn      = psToolBarInfo->m_byMsgFilter;
        m_bLogFilterOn          = psToolBarInfo->m_byLogFilter;
        m_bLogEnable            = psToolBarInfo->m_byLogging;
        m_bTranslationMode      = psToolBarInfo->m_byMsgInterpret;
        m_bOverwriteEnable      = psToolBarInfo->m_byOverwrite;
        m_wLogTimeMode          = psToolBarInfo->m_byLogTimeMode;
        m_bLogHexON             = psToolBarInfo->m_byLogHexON;
        m_wDisplayTimeMode      = psToolBarInfo->m_byDisplayTimeMode;
        m_bDisplayHexON         = psToolBarInfo->m_byDisplayHexON;
        m_bLogOverWriteON       = psToolBarInfo->m_byLogOverWriteON;
        m_nReplayMsgType        = psToolBarInfo->m_byReplayMsgType;
        m_bScrolling            = psToolBarInfo->m_byEnsureVisible;
    }
}

/******************************************************************************
Function Name   : ouGetFlagObj
Input(s)        : -
Output          : CFlags&
Functionality   : static function return singleton object
Member of       : CFlags
Friend of       : -
Author(s)       : Ratnadip Choudhury, 19.03.2011
Modifications   : 
******************************************************************************/
CFlags& CFlags::ouGetFlagObj(void)
{
    return sm_ouSingletonObj;
}
