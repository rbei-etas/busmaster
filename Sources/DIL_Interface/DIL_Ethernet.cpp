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
* \file      DIL_Interface/DIL_ETHERNET.cpp
* \brief     Interface file for ETHERNET BUS
* \author    Ashwin R Uchil
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*
* Interface file for ETHERNET BUS
*/
#pragma once
#include "DIL_Interface_stdafx.h"
#include "include/basedefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "DIL_Ethernet.h"

typedef HRESULT (__stdcall* GETIDIL_ETHERNET_CONTROLLER)(void** ppvInterface);
static GETIDIL_ETHERNET_CONTROLLER pfGetIDILETHERNET_Controller;

#define VALIDATE_ETHERNET_POINTER(Ptr) if (Ptr == NULL) {return S_FALSE;}

class ENTRY_DIL
{
public:
    DWORD           m_dwDIL;
    string          m_acDIL;

};

static ENTRY_DIL sg_ListDIL[] =
{
    /* PTV[1.6.4] */
    // Added Short cut keys
    /* simulation should be the first entry... */
    {DAL_ETHERNET_NONE, "&Deactivate"},
    {DRIVER_ETHERNET, "Standard Ethernet"},
};


CDIL_ETHERNET::CDIL_ETHERNET()
{
	m_hDll = NULL;
    m_hOldDll = NULL;
    m_dwDriverID = DAL_NONE;
    pfGetIDILETHERNET_Controller = NULL;
    m_pBaseDILEthernet_Controller = NULL;
    m_pOldBaseDILEthernet_Controller = NULL;
}

CDIL_ETHERNET::~CDIL_ETHERNET()
{

}

/**
* Based on the parameter this function renders number of the driver interface
* layers supported or available. If 'bAvailable' is true, this returns number of
* the DILs implemented; else the list of the DILs supported by the existing
* license will be returned. If List is NULL, only number is returned.
*/
DWORD CDIL_ETHERNET::DILE_GetDILList(bool bAvailable, DILLIST* List)
{
	DWORD Result = 0;

    if (List != NULL)
    {
        for (int i = 0; i < sizeof(sg_ListDIL)/sizeof(ENTRY_DIL); i++)
        {
            (*List)[i].m_acName = sg_ListDIL[i].m_acDIL;
            //_tcscpy((*List)[i].m_acName, sg_ListDIL[i].m_acDIL);
            (*List)[i].m_dwDriverID = sg_ListDIL[i].m_dwDIL;
            Result++;
        }
    }

    return Result;
}

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
HRESULT CDIL_ETHERNET::DILE_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)  
{
	VALIDATE_ETHERNET_POINTER(m_pBaseDILEthernet_Controller);
	return m_pBaseDILEthernet_Controller->Ethernet_RegisterClient(bRegister,ClientID,pacClientName);
}

/**
* This function manages the target message buffer list. The two combinations
* are the following:
* bAction = MSGBUF_ADD, add pBufObj to the target message buffer list.
* bAction = MSGBUF_CLEAR, clear the list. pBufObj is ignored.
* At present maximum number of entries in the list is kept as 8.
*/
HRESULT CDIL_ETHERNET::DILE_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseEthernetBufFSE* pBufObj) 
{
	VALIDATE_ETHERNET_POINTER(m_pBaseDILEthernet_Controller);
	return m_pBaseDILEthernet_Controller->Ethernet_ManageMsgBuf(bAction, ClientID, pBufObj);
}

/* Call for all initialisation operations. */
HRESULT CDIL_ETHERNET::DILE_PerformInitOperations(void)
{
	VALIDATE_ETHERNET_POINTER(m_pBaseDILEthernet_Controller);
	return m_pBaseDILEthernet_Controller->Ethernet_PerformInitOperations();
}

/* Call for all uninitialisation operations */
HRESULT CDIL_ETHERNET::DILE_PerformClosureOperations(void)
{
	VALIDATE_ETHERNET_POINTER(m_pBaseDILEthernet_Controller);
	return m_pBaseDILEthernet_Controller->Ethernet_PerformClosureOperations();
}

/**
* Call this function to get a system time and the time stamp associated with it
*/
HRESULT CDIL_ETHERNET::DILE_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL)
{
	VALIDATE_ETHERNET_POINTER(m_pBaseDILEthernet_Controller);
	return m_pBaseDILEthernet_Controller->Ethernet_GetTimeModeMapping(CurrSysTime, TimeStamp, QueryTickCount);
}

/**
* Function to display the configuration dialog box for the selected DIL. If
* the dialog box needs to be displayed been initialised, pass the relevant data
* as InitData. If it is null, the dialog box is uninitialised. This also contains
* the user's choice as OUT parameter
*/
HRESULT CDIL_ETHERNET::DILE_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length)
{
	VALIDATE_ETHERNET_POINTER(m_pBaseDILEthernet_Controller);
	return m_pBaseDILEthernet_Controller->Ethernet_DisplayConfigDlg(InitData, Length);
}

/**
* To set the configuration data for the currently selected DIL. Caller must
* release the memory.
*/
HRESULT CDIL_ETHERNET::DILE_SetConfigData(PSCONTROLLER_DETAILS pInitData, int Length)
{
	VALIDATE_ETHERNET_POINTER(m_pBaseDILEthernet_Controller);
	return m_pBaseDILEthernet_Controller->Ethernet_SetConfigData(pInitData, Length);
}


//Get the logger from main frame
HRESULT CDIL_ETHERNET::DILE_SetAppParams(HWND hWnd, Base_WrapperErrorLogger* pILog)
{
	VALIDATE_ETHERNET_POINTER(m_pBaseDILEthernet_Controller);
	if (pILog == NULL)
    {
       return ERRLGR_INVALID;
    }
	else
	{
		m_pBaseDILEthernet_Controller->Ethernet_SetAppParams(hWnd, pILog);
	}
	return S_OK;
}

/**
* Send messages
*/
HRESULT CDIL_ETHERNET::DILE_SendMsg(DWORD dwClientID, const STETHERNET_MSG& sEthernetTxMsg)
{
	VALIDATE_ETHERNET_POINTER(m_pBaseDILEthernet_Controller);
	return m_pBaseDILEthernet_Controller->Ethernet_SendMsg(dwClientID, sEthernetTxMsg);
}

/**
* Call to get descriptive string of the last error occurred
*/
HRESULT CDIL_ETHERNET::DILE_GetLastErrorString(string& acErrorStr)
{
	VALIDATE_ETHERNET_POINTER(m_pBaseDILEthernet_Controller);
	return m_pBaseDILEthernet_Controller->Ethernet_GetLastErrorString(acErrorStr);
}

/**
* Call to Get Error Counts
*/
HRESULT  CDIL_ETHERNET::DILE_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
	VALIDATE_ETHERNET_POINTER(m_pBaseDILEthernet_Controller);
	return m_pBaseDILEthernet_Controller->Ethernet_GetErrorCount(sErrorCnt, nChannel, eContrParam);
}

/**
 * Start the controller
 */
HRESULT CDIL_ETHERNET::DILE_StartHardware(void)
{
	VALIDATE_ETHERNET_POINTER(m_pBaseDILEthernet_Controller);
    return m_pBaseDILEthernet_Controller->Ethernet_StartHardware();
}

/**
 * Stop the controller
 */
HRESULT CDIL_ETHERNET::DILE_StopHardware(void)
{
	VALIDATE_ETHERNET_POINTER(m_pBaseDILEthernet_Controller);
    return m_pBaseDILEthernet_Controller->Ethernet_StopHardware();
}

/* This function selects a driver abstraction layer (DAL). If support for the
 * intended one isn't allowed with the present license, this returns NO_LICENSE.
 */
HRESULT CDIL_ETHERNET::DILE_SelectDriver(DWORD dwDriverID, HWND hWndOwner,
                                    Base_WrapperErrorLogger* pILog)
 {
	 USES_CONVERSION;

    HRESULT hResult = S_FALSE;

    if (DILE_GetSelectedDriver() == dwDriverID)
    {
        hResult = DAL_ALREADY_SELECTED;
    }
	else if (pILog == NULL)
	{
		hResult = ERRLGR_INVALID;
	}
	if (hWndOwner == NULL)
	{
		/* Log a warning message informing about the invalidity of the
		owner window handle. */
	}
	//Save the currently selected DIL library handle
	if ( m_hDll )
	{
		//FreeLibrary(m_hDll);
		//m_pBaseDILLIN_Controller = NULL;
		m_hOldDll = m_hDll;
		m_pOldBaseDILEthernet_Controller = m_pBaseDILEthernet_Controller;
		m_dwOldDriverID = m_dwDriverID;
		m_hDll = NULL;
	}
	switch(dwDriverID)
	{
		case DRIVER_ETHERNET:
			{
				m_hDll = LoadLibrary("Ethernet_Interface.dll");			//NO library is required for standard ethernet
				break;
			}
		default:
			DILE_PerformClosureOperations();
			//vSelectInterface_Dummy();
			m_dwDriverID = DAL_NONE;
			return hResult;
	}
	 if (m_hDll == NULL)
	 {
		  hResult = ERR_LOAD_DRIVER;
            pILog->vLogAMessage(A2T(__FILE__), __LINE__, _("Load library failed..."));
	 }
	 else
	 {
		 pILog->vLogAMessage(A2T(__FILE__), __LINE__, _("Load library successful..."));
		 pfGetIDILETHERNET_Controller = (GETIDIL_ETHERNET_CONTROLLER)GetProcAddress(m_hDll, "GetIDIL_Ethernet_Controller");
		 pfGetIDILETHERNET_Controller((void**)&m_pBaseDILEthernet_Controller);
		 if ( m_pBaseDILEthernet_Controller )
		 {
			 // set the application parameters.
			 m_pBaseDILEthernet_Controller->Ethernet_SetAppParams(hWndOwner, pILog);
			 hResult = m_pBaseDILEthernet_Controller->Ethernet_LoadDriverLibrary();
			  switch (hResult)
                {
                    case S_OK:
                    case DLL_ALREADY_LOADED:
                        m_dwDriverID = dwDriverID;
                        hResult = S_OK;
                        break;

                    default:
                        hResult = ERR_LOAD_DRIVER;
                        pILog->vLogAMessage(A2T(__FILE__), __LINE__, _("Load library failed..."));
                        /* New Code */
                        /* Get rid of current DIL library */
                        if ( m_pBaseDILEthernet_Controller )
                        {
                            m_pBaseDILEthernet_Controller->Ethernet_PerformClosureOperations();
                            m_pBaseDILEthernet_Controller->Ethernet_UnloadDriverLibrary();
                        }
                        FreeLibrary(m_hDll);
                        //m_pBaseDILLIN_Controller = NULL;
                        if ( m_hOldDll )
                        {
                            m_hDll = m_hOldDll;
                            m_pBaseDILEthernet_Controller = m_pOldBaseDILEthernet_Controller;
                            m_dwDriverID =  m_dwOldDriverID;
                        }
                        /* New Code */
                        break;
			  }
		 }
	 }
	 return hResult;
}

/**
 * \brief     Get selected driver
 * \req       RSI_14_005 - DILL_GetSelectedDriver
 * \req       RS_23_03 - Getter for the presently selected driver
 *
 * This returns ID of the driver interface layer presently selected.
 */
DWORD CDIL_ETHERNET::DILE_GetSelectedDriver(void)
{
    return m_dwDriverID;
}


/**
* Call this function to list the hardware interfaces available and receive
* user's choice.
*/
HRESULT CDIL_ETHERNET::DILE_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount)
{
	VALIDATE_ETHERNET_POINTER(m_pBaseDILEthernet_Controller);
	return m_pBaseDILEthernet_Controller->Ethernet_ListHwInterfaces(sSelHwInterface, nCount);
}

/**
* Call this function to select hardware interfaces.
*/
HRESULT CDIL_ETHERNET::DILE_SelectHwInterfaces(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount)
{
	return S_OK;
}

/**
* Call this function to deselect the selected hardware interface.
*/
HRESULT CDIL_ETHERNET::DILE_DeselectHwInterfaces(void)
{
	return S_OK;
}

/**
* Gets custom protocol dlls HMODULE and can be used to call custom functions
*/
HMODULE CDIL_ETHERNET::DILE_GetCustomProtocolHandle()
{
	return m_pBaseDILEthernet_Controller->Ethernet_GetCustomProtocolHandle();
}