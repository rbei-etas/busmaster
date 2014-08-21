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
* \file      DIL_Interface/DIL_ETHERNET.h
* \brief     Definition file for CDIL_ETHERNET class.
* \author    Ashwin R Uchil
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*
* Definition file for CDIL_FLEXRAY class.
*/
#if !defined DIL_ETHERNET_H__INCLUDED_
#define DIL_ETHERNET_H__INCLUDED_

#include "Include/Basedefs.h"
#include "BaseDIL_ETHERNET.h"
#include "BaseDIL_Ethernet_Controller.h"
//#include "BaseDIL_ETHERNET_Controller.h"

class CDIL_ETHERNET : public CBaseDIL_ETHERNET
{
public:
	/** Constructor */
	CDIL_ETHERNET();

	/* Destructor */
	~CDIL_ETHERNET();


	/* member variable to hold the pointer of currently selected controller interface */
    CBaseDIL_Ethernet_Controller* m_pBaseDILEthernet_Controller;
    /* member variable to hold the previously selected controller interface */
    CBaseDIL_Ethernet_Controller* m_pOldBaseDILEthernet_Controller;


	/* Variable to maintain currently selected Driver ID */
	DWORD m_dwDriverID;
	/* Variable to store previously selected Driver ID */
	DWORD m_dwOldDriverID;

	/* Variable to hold handle to currently selected controller DIL */
	HMODULE m_hDll;
	/* Variable to hold handle to previously selected controller DIL */
	HMODULE m_hOldDll;

	/**
	* Based on the parameter this function renders number of the driver interface
	* layers supported or available. If 'bAvailable' is true, this returns number of
	* the DILs implemented; else the list of the DILs supported by the existing
	* license will be returned. If List is NULL, only number is returned.
	*/
	DWORD DILE_GetDILList(bool bAvailable, DILLIST* List)  ;

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
	HRESULT DILE_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)  ;

	/**
	* This function manages the target message buffer list. The two combinations
	* are the following:
	* bAction = MSGBUF_ADD, add pBufObj to the target message buffer list.
	* bAction = MSGBUF_CLEAR, clear the list. pBufObj is ignored.
	* At present maximum number of entries in the list is kept as 8.
	*/
	HRESULT DILE_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseEthernetBufFSE* pBufObj)  ;

	/* Call for all initialisation operations. */
	HRESULT DILE_PerformInitOperations(void)  ;

	/* Call for all uninitialisation operations */
	HRESULT DILE_PerformClosureOperations(void)  ;

	/**
	* Call this function to get a system time and the time stamp associated with it
	*/
	HRESULT DILE_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount)  ;

	/**
	* Function to display the configuration dialog box for the selected DIL. If
	* the dialog box needs to be displayed been initialised, pass the relevant data
	* as InitData. If it is null, the dialog box is uninitialised. This also contains
	* the user's choice as OUT parameter
	*/
	HRESULT DILE_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length)  ;

	/**
	* To set the configuration data for the currently selected DIL. Caller must
	* release the memory.
	*/
	HRESULT DILE_SetConfigData(PSCONTROLLER_DETAILS pInitData, int Length)  ;

	/**
	* Send messages
	*/
	HRESULT DILE_SendMsg(DWORD dwClientID, const STETHERNET_MSG& sCanTxMsg)  ;

	/**
	* Call to get descriptive string of the last error occurred
	*/
	HRESULT DILE_GetLastErrorString(string& acErrorStr)  ;

	/**
	* Call to Get Error Counts
	*/
	HRESULT  DILE_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)  ;

	//Set the owner(main window) and logger from main window
	HRESULT DILE_SetAppParams(HWND hWnd, Base_WrapperErrorLogger* pILog);

	/**
	* Start the controller
	*/
	HRESULT DILE_StartHardware(void);

	/**
	* Stop the controller
	*/
	HRESULT DILE_StopHardware(void);

	 /**
     * This function selects a driver abstraction layer (DAL). If support for the
     * intended one isn't allowed with the present license, this returns NO_LICENSE.
     * A list for DALs (or bDriverID) is shown below:
     * DRIVER_ETHERNET_INTERFACE   : DAL for Ethernet
     * DAL_NONE                    : Dummy DAL */
    HRESULT DILE_SelectDriver(DWORD dwDriverID, HWND hWndParent,
                              Base_WrapperErrorLogger* pILog);

	/**
	* This returns ID of the driver interface layer presently selected.
	*/
	DWORD DILE_GetSelectedDriver(void);

	/**
	* Call this function to list the hardware interfaces available and receive
	* user's choice.
	*/
	HRESULT DILE_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount);

	/**
	* Call this function to select hardware interfaces.
	*/
	HRESULT DILE_SelectHwInterfaces(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);

	/**
	* Call this function to deselect the selected hardware interface.
	*/
	HRESULT DILE_DeselectHwInterfaces(void);

	/**
	* Gets custom protocol dlls HMODULE and can be used to call custom functions
	*/
	HMODULE DILE_GetCustomProtocolHandle();

};

#endif //DIL_ETHERNET_H__INCLUDED_
