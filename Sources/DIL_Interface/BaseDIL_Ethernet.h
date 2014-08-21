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
 * \file      BaseDIL_ETHERNET.h
 * \brief     Base class for CDIL_ETHERNET class.
 * \author    Ashwin R Uchil
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Base class for CDIL_ETHERNET class.
 */
#if !defined BASEDIL_ETHERNET_H__INCLUDED_
#define BASEDIL_ETHERNET_H__INCLUDED_

#include "DataTypes/DIL_DataTypes.h"
#include "Datatypes/Base_WrapperErrorLogger.h"
#include "Datatypes/MsgBufAll_DataTypes.h"

class CBaseDIL_ETHERNET
{
public:
    /**
     * Constructor
     */
    CBaseDIL_ETHERNET() {};

    /**
     * Destructor
     */
    ~CBaseDIL_ETHERNET() {};

	 /**
     * Based on the parameter this function renders number of the driver interface
     * layers supported or available. If 'bAvailable' is true, this returns number of
     * the DILs implemented; else the list of the DILs supported by the existing
     * license will be returned. If List is NULL, only number is returned.
     */
    virtual DWORD DILE_GetDILList(bool bAvailable, DILLIST* List) = 0;

	//Set the owner(main window) and logger from main window
	virtual HRESULT DILE_SetAppParams(HWND hWnd, Base_WrapperErrorLogger* pILog) = 0;

	   /**
     * DILE_RegisterClient registers/unregisters the client.
     * bRegister = TRUE for registeration
     * bRegister = FALSE for unregisteration
     * Only registered client's buffer will be updated on receive of a msg in the bus.

     * Following are the return values
     * S_OK -->registeration successful
     * ERR_CLIENT_EXISTS -->Client already registered. Client Id will be stored in ClientId.
     * ERR_NO_CLIENT_EXIST --> No client exists. Usually this happens if we try to remove non existing client.
     * ERR_NO_MORE_CLIENT_ALLOWED -->No more clients are allowed to register.
     */
    virtual HRESULT DILE_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName) = 0;

	    /**
     * This function manages the target message buffer list. The two combinations
     * are the following:
     * bAction = MSGBUF_ADD, add pBufObj to the target message buffer list.
     * bAction = MSGBUF_CLEAR, clear the list. pBufObj is ignored.
     * At present maximum number of entries in the list is kept as 8.
     */
    virtual HRESULT DILE_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseEthernetBufFSE* pBufObj) = 0;

	/* Call for all initialisation operations. */
    virtual HRESULT DILE_PerformInitOperations(void) = 0;

	 /* Call for all uninitialisation operations */
    virtual HRESULT DILE_PerformClosureOperations(void) = 0;

	/**
     * Call this function to get a system time and the time stamp associated with it
     */
    virtual HRESULT DILE_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL) = 0;

	    /**
     * Function to display the configuration dialog box for the selected DIL. If
     * the dialog box needs to be displayed been initialised, pass the relevant data
     * as InitData. If it is null, the dialog box is uninitialised. This also contains
     * the user's choice as OUT parameter
     */
    virtual HRESULT DILE_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length) = 0;

	    /**
     * To set the configuration data for the currently selected DIL. Caller must
     * release the memory.
     */
    virtual HRESULT DILE_SetConfigData(PSCONTROLLER_DETAILS pInitData, int Length) = 0;

	 /**
     * Send messages
     */
    virtual HRESULT DILE_SendMsg(DWORD dwClientID, const STETHERNET_MSG& sCanTxMsg) = 0;

	 /**
    * Call to get descriptive string of the last error occurred
     */
    virtual HRESULT DILE_GetLastErrorString(string& acErrorStr) = 0;

	/**
     * Call to Get Error Counts
     */
    virtual HRESULT  DILE_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam) = 0;

	/**
	* Start the controller
	*/
	virtual HRESULT DILE_StartHardware(void) = 0;

	/**
	* Stop the controller
	*/
	virtual HRESULT DILE_StopHardware(void) = 0;


	 /**
     * This function selects a driver abstraction layer (DAL). If support for the
     * intended one isn't allowed with the present license, this returns NO_LICENSE.
     * A list for DALs (or bDriverID) is shown below:
     * DRIVER_ETHERNET_INTERFACE   : DAL for Ethernet
     * DAL_NONE                    : Dummy DAL */
    virtual HRESULT DILE_SelectDriver(DWORD dwDriverID, HWND hWndParent,
										Base_WrapperErrorLogger* pILog) = 0;

	/**
	* This returns ID of the driver interface layer presently selected.
	*/
	virtual DWORD DILE_GetSelectedDriver(void) = 0;

	/**
	* Call this function to list the hardware interfaces available and receive
	* user's choice.
	*/
	virtual HRESULT DILE_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount) = 0;

	/**
	* Call this function to select hardware interfaces.
	*/
	virtual HRESULT DILE_SelectHwInterfaces(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount) = 0;

	/**
	* Call this function to deselect the selected hardware interface.
	*/
	virtual HRESULT DILE_DeselectHwInterfaces(void) = 0;

	/**
	* Gets custom protocol dlls HMODULE and can be used to call custom functions
	*/
	virtual HMODULE DILE_GetCustomProtocolHandle() = 0;

};


#endif // BASEDIL_ETHERNET_H__INCLUDED_