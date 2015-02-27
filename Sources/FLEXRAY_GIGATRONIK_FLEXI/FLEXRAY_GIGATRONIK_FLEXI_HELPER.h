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

/**********************************************************************************************************************
 *
 *  FILENAME:           FLEXRAY_GIGATRONIK_FLEXI_Log.h
 *
 *  COPYRIGHT:          Gigatronik Ingolstadt GmbH
 *  AUTHOR, DPT:        M. Morgott, KE
 *
 *  DATE OF CREATION:   28.11.2014
 *
 *  DESCRIPTION:        Define helper class.
 *
 *********************************************************************************************************************/

/** --- CHANGE LOG ---
 *  V1.0.0.0        First reviewed version
 *
 */

#pragma once

/* include afxwin.h to avoid windows.h inclusion error */
#include <afxwin.h>

/* project includes */
#include "DIL_Interface\CommanDIL_Flexray.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_dialog_keyslot.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_extern.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_configuration.h"


//#include "temp_debug.h"


/***************************************** class definition *****************************************/
class Flexi_Helper
{
public:
    /* called on FLEXRAY_LoadDriverLibrary, load API functions from flexi API handle  */
    static HRESULT Get_FLEXI_API_Pointers(HMODULE& hLibFLEXI, S_FLEXI_POINTER_TABLE& flexi_API);

    /* called on FLEXRAY_SetConfigData, returns ProtCfg for flexi configuration  */
    static HRESULT Get_ProtCfg(GtFr_ProtocolCfgType& ProtCfg, CHANNEL_CONFIG& FibexContainer);

    /* called on FLEXRAY_SetConfigData, returns CtrlCfg for flexi configuration  */
    static HRESULT Get_CtrlCfg(GtFr_CtrlCfgType CtrlCfg[]);

    /* called on FLEXRAY_SetConfigData, returns MsgBufCfg for flexi configuration */
    static HRESULT Get_MsgBufCfg(GtFr_MsgBufCfgType MsgBufCfg[],
                                 const GtFr_ProtocolCfgType& ProtCfg,
                                 CHANNEL_CONFIG FibexContainer,
                                 std::map<SLOT_BASECYCLE, MSG_BUF_IDX_TYPE>& map_MsgBufIdx,
                                 std::map<SLOT_BASECYCLE, MSG_REPETITION_TYPE>& map_MsgRep,
                                 const GtFr_MsgBufTxModeType& transmission_mode_static,
                                 const GtFr_MsgBufTxModeType& transmission_mode_dynamic,
                                 const flexi_Keyslot_Settings& Keyslot_Settings,
                                 const std::string& strLogFile);

    /* callback function to process the events from the flex-i. */
    static void callback_EventQueue(GtFr_EventQueueStatusType EventQueueStatus, GtFr_EventType* events);

    /* This function will be raised if an queue overflow occurs.*/
    static void callback_EventQueueOverflow(GtFr_QueueOverflowArgsType overFlowArgs);

private:

};
