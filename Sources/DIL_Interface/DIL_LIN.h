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
 * \file      DIL_Interface/DIL_LIN.h
 * \brief     Definition file for CDIL_LIN class.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CDIL_LIN class.
 */
#if !defined DIL_LIN_H__INCLUDED_
#define DIL_LIN_H__INCLUDED_

#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "Include/Basedefs.h"
#include "BaseDIL_LIN.h"
#include "BaseDIL_LIN_Controller.h"

class CDIL_LIN : public CBaseDIL_LIN
{
public:
    /** Constructor */
    CDIL_LIN();

    /* Destructor */
    ~CDIL_LIN();

    BOOL InitInstance(void);
    int ExitInstance(void);

    /* Variable to maintain currently selected Driver ID */
    DWORD m_dwDriverID;
    /* Variable to store previously selected Driver ID */
    DWORD m_dwOldDriverID;

    /* member variable to hold the pointer of currently selected controller interface */
    CBaseDIL_LIN_Controller* m_pBaseDILLIN_Controller;
    /* member variable to hold the previously selected controller interface */
    CBaseDIL_LIN_Controller* m_pOldBaseDILLIN_Controller;

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
    DWORD DILL_GetDILList(bool bAvailable, DILLIST* List);

    /**
     * This function selects a driver abstraction layer (DAL). If support for the
     * intended one isn't allowed with the present license, this returns NO_LICENSE.
     * A list for DALs (or bDriverID) is shown below:
     * DRIVER_STUB                 : DAL for Stub
     * DRIVER_ES520                : DAL for ES520
     * DRIVER_TZMCYCLONE           : DAL for TZMCyclone
     * DRIVER_LIN_USB              : DAL for LIN usb
     * DRIVER_LIN_PARALLEL_PORT    : DAL for LIN parallel port
     * DAL_NONE                    : Dummy DAL */
    HRESULT DILL_SelectDriver(DWORD dwDriverID, HWND hWndParent,
                              Base_WrapperErrorLogger* pILog);

    /**
     * This function registers the client. Only registered client's buffer will be
     * updated on receive of a msg in the bus.
     * Following are the return values
     * -1 registeration failed
     *  1 registeration successful
     *  2 Client already registered
     * -2 No more clients are allowed to register*/
    HRESULT DILL_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);

    /**
     * This function manages the target message buffer list. The two combinations
     * are the following:
     * bAction = MSGBUF_ADD, add pBufObj to the target message buffer list.
     * bAction = MSGBUF_CLEAR, clear the list. pBufObj is ignored.
     * At present maximum number of entries in the list is kept as 8.
     */
    HRESULT DILL_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseLINBufFSE* pBufObj);

    /**
     * This returns ID of the driver interface layer presently selected.
     */
    DWORD DILL_GetSelectedDriver(void);

    /**
     * Call for all initialisation operations.
     */
    HRESULT DILL_PerformInitOperations(void);

    /**
     * Call for all uninitialisation operations
     */
    HRESULT DILL_PerformClosureOperations(void);

    /**
     * Call this function to get a system time and the time stamp associated with it
     */
    HRESULT DILL_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL);

    /**
     * Call this function to list the hardware interfaces available and receive
     * user's choice.
     */
    HRESULT DILL_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount);

    /**
     * Call this function to select hardware interfaces.
     */
    HRESULT DILL_SelectHwInterfaces(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);

    /**
     * Call this function to deselect the selected hardware interface.
     */
    HRESULT DILL_DeselectHwInterfaces(void);

    /**
     * Function to display the configuration dialog box for the selected DIL. If
     * the dialog box needs to be displayed been initialised, pass the relevant data
     * as InitData. If it is null, the dialog box is uninitialised. This also contains
     * the user's choice as OUT parameter
     */
    HRESULT DILL_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length);

    /**
     * To set the configuration data for the currently selected DIL. Caller must
     * release the memory.
     */
    HRESULT DILL_SetConfigData(PSCONTROLLER_DETAILS InitData, int Length);

    /**
     * Start the controller
     */
    HRESULT DILL_StartHardware(void);

    /**
     * Stop the controller
     */
    HRESULT DILL_StopHardware(void);

    /**
     * Reset Hardware
     */
    HRESULT DILL_ResetHardware(void);


    /**
     * Send messages
     */
    HRESULT DILL_SendMsg(DWORD dwClientID, STLIN_MSG& sLinTxMsg);

    HRESULT DILL_SetSlaveRespData(const STLIN_MSG stRespMsg);
    HRESULT DILL_ResetSlaveRespData(void);

    /**
     * Call to get descriptive string of the last error occurred
     */
    HRESULT DILL_GetLastErrorString(string& acErrorStr);

    /**
     * Call to set PASS/STOP filter
     */
    /**
     * Call to get controller status. Caller has to give the handle of a
     * event which will set whenever the controller changes the state.
     * #define defCONTROLLER_ACTIVE                   1
     * #define defCONTROLLER_PASSIVE                  2
     * #define defCONTROLLER_BUSOFF                   3
     */
    HRESULT DILL_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);

    /**
     * Call to get Controller parameters. Value will be returned stored in lParam
     * Possible values for ECONTR_PARAM are ...
     */
    HRESULT DILL_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    /**
     *
     *
     */
    virtual HRESULT DILL_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
    /**
     * Call to Get Error Counts
     */
    HRESULT  DILL_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);

    /* HELPER FUNCTIONS START */
    void vSelectInterface_Dummy(void);

private:
    HRESULT (*m_pfPerformInitOperations)(void);
    HRESULT (*m_pfPerformClosureOperations)(void);
    HRESULT (*m_pfGetTimeModeMapping)(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount);
    HRESULT (*m_pfListHwInterfaces)(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount);
    HRESULT (*m_pfSelectHwInterface)(const INTERFACE_HW_LIST& asSelHwInterface, INT nCount);
    HRESULT (*m_pfDeselectHwInterfaces)(void);
    HRESULT (*m_pfDisplayConfigDlg)(PSCONTROLLER_DETAILS InitData, int& Length);
    HRESULT (*m_pfSetConfigData)(PCHAR pInitData, int Length);
    HRESULT (*m_pfStartHardware)(void);
    HRESULT (*m_pfStopHardware)(void);
    HRESULT (*m_pfResetHardware)(void);
    HRESULT (*m_pfGetTxMsgBuffer)(BYTE*& pouTxMsgBuffer);
    HRESULT (*m_pfSendMsg)(DWORD dwClientID, const STLIN_MSG& pouFlxTxMsg);
    HRESULT (*m_pfGetBusConfigInfo)(BYTE* BusInfo);
    HRESULT (*m_pfGetLastErrorString)(CHAR* acErrorStr, int nLength);
    HRESULT (*m_pfFilterFrames)(FILTER_TYPE FilterType, TYPE_CHANNEL Channel, UINT* punMsgIDs, UINT nLength);
    HRESULT (*m_pfManageMsgBuf)(BYTE, DWORD ClientID, CBaseLINBufFSE*);
    HRESULT (*m_pfRegisterClient)(BOOL bRegister, DWORD&, char*);
    HRESULT (*m_pfGetCntrlStatus)(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT (*m_pfGetControllerParams)(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT (*m_pfGetErrorCount)(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);
};

#endif // DIL_LIN_H__INCLUDED_
