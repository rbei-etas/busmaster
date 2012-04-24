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
 * \file      BaseDIL_CAN.h
 * \brief     Base class for CDIL_CAN class.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Base class for CDIL_CAN class.
 */

#pragma once

/* C++ includes */
#include <string>

/* Project includes */
#include "DataTypes/DIL_DataTypes.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "Datatypes/Base_WrapperErrorLogger.h"

using namespace std;

class CBaseDIL_CAN
{
public:
	/**
	 * Constructor
	 */
    CBaseDIL_CAN(){};

	/**
	 * Destructor
	 */
    ~CBaseDIL_CAN(){};

    /**
	 * Based on the parameter this function renders number of the driver interface
     * layers supported or available. If 'bAvailable' is true, this returns number of
     * the DILs implemented; else the list of the DILs supported by the existing 
     * license will be returned. If List is NULL, only number is returned.
	 */
    virtual DWORD DILC_GetDILList(bool bAvailable, DILLIST* List) = 0;

    /**
	 * DILC_SelectDriver selects a driver abstraction layer (DAL). If support for the 
     * intended one isn't allowed with the present license, this returns NO_LICENSE.
     * A list for DALs (or bDriverID) is shown below:
     * DRIVER_CAN_STUB, //Simulation engine without the hardware
     * DRIVER_CAN_PEAK_USB, // PEAK-CAN USB hardware
     * DRIVER_CAN_PEAK_PP,  // PEAK-CAN Parallel Port Interface
     * DRIVER_CAN_ICS_NEOVI, // IntrepidCS neoVI hardware
     * DRIVER_CAN_ETAS_BOA.     // ETAS BOA Framework 
     */
    virtual HRESULT DILC_SelectDriver(DWORD dwDriverID, HWND hWndParent,
                                        Base_WrapperErrorLogger* pILog) = 0;

    /**
	 * DILC_RegisterClient registers/unregisters the client. 
     * bRegister = TRUE for registeration 
     * bRegister = FALSE for unregisteration
     * Only registered client's buffer will be updated on receive of a msg in the bus.

     * Following are the return values
     * S_OK -->registeration successful
     * ERR_CLIENT_EXISTS -->Client already registered. Client Id will be stored in ClientId.
     * ERR_NO_CLIENT_EXIST --> No client exists. Usually this happens if we try to remove non existing client.
     * ERR_NO_MORE_CLIENT_ALLOWED -->No more clients are allowed to register.
	 */
    virtual HRESULT DILC_RegisterClient(BOOL bRegister, DWORD& ClientID, TCHAR* pacClientName) = 0;

	/**
	 * This function manages the target message buffer list. The two combinations
	 * are the following:
     * bAction = MSGBUF_ADD, add pBufObj to the target message buffer list.
     * bAction = MSGBUF_CLEAR, clear the list. pBufObj is ignored.
     * At present maximum number of entries in the list is kept as 8.
	 */
    virtual HRESULT DILC_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj) = 0;

    /**
	 * This returns ID of the driver interface layer presently selected.
	 */
    virtual DWORD DILC_GetSelectedDriver(void) = 0;

    /**
	 * Call for all initialisation operations.
	 */
    virtual HRESULT DILC_PerformInitOperations(void) = 0;

    /**
	 * Call for all uninitialisation operations
	 */
    virtual HRESULT DILC_PerformClosureOperations(void) = 0;

    /**
	 * Call this function to get a system time and the time stamp associated with it
	 */
    virtual HRESULT DILC_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL) = 0;

    /**
	 * Call this function to list the hardware interfaces available and receive
	 * user's choice.
	 */
    virtual HRESULT DILC_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount) = 0;

    /**
	 * Call this function to select a particular hardware interface.
	 */
    virtual HRESULT DILC_SelectHwInterfaces(const INTERFACE_HW_LIST& asSelHwInterface, INT nCount) = 0;

    /**
	 * Call this function to deselect the selected hardware interface.
	 */
    virtual HRESULT DILC_DeselectHwInterfaces(void) = 0;

    /**
	 * Function to display the configuration dialog box for the selected DIL. If
     * the dialog box needs to be displayed been initialised, pass the relevant data
     * as InitData. If it is null, the dialog box is uninitialised. This also contains
     * the user's choice as OUT parameter
	 */
    virtual HRESULT DILC_DisplayConfigDlg(PCHAR& InitData, int& Length) = 0;

    /**
	 * To set the configuration data for the currently selected DIL. Caller must 
     * release the memory.
	 */
    virtual HRESULT DILC_SetConfigData(PCHAR pInitData, int Length) = 0;

    /**
	 * Start the controller
	 */
    virtual HRESULT DILC_StartHardware(void) = 0;

    /**
	 * Stop the controller
	 */
    virtual HRESULT DILC_StopHardware(void) = 0;

    /**
	 * Reset Hardware
	 */
    virtual HRESULT DILC_ResetHardware(void) = 0;

    /**
	 * Send messages
	 */
    virtual HRESULT DILC_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg) = 0;

    /**
	 * Call to get descriptive string of the last error occurred
	 */
    virtual HRESULT DILC_GetLastErrorString(string &acErrorStr) = 0;

	/**
	 * Call to get controller status. Caller has to give the handle of a
     * event which will set whenever the controller changes the state.
     * #define defCONTROLLER_ACTIVE                   1
     * #define defCONTROLLER_PASSIVE                  2
     * #define defCONTROLLER_BUSOFF                   3
	 */
    virtual HRESULT DILC_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus) = 0;

    /**
	 * Call to get Controller parameters. Value will be returned stored in lParam
     * Possible values for ECONTR_PARAM are ...
     */
    virtual HRESULT DILC_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam) = 0;

	/**
	 * Call to Get Error Counts
	 */
    virtual HRESULT  DILC_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam) = 0;
};
