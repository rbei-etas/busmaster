/*
* Copyright Robert Bosch Engineering and Business Solutions Private Limited. All Rights Reserved.
*/



#pragma once

#include "FlexRayDriverDefines.h"
#include "../../BusmasterDBNetwork/Include/IBMNetWorkGetService.h"
#include "IBusService.h"
#include <list>

class CBaseDIL_FLEXRAY : public IBusService
{
public:
    /**
     * Constructor
     */
    CBaseDIL_FLEXRAY() {};

    /**
     * Destructor
     */
    virtual ~CBaseDIL_FLEXRAY() {};

    /* Based on the parameter this function renders number of the driver interface
    layers supported or available. If 'bAvailable' is true, this returns number of
    the DILs implemented; else the list of the DILs supported by the existing
    license will be returned. If List is nullptr, only number is returned. */
    virtual DWORD DILF_GetDILList(bool bAvailable, DILLIST* List) = 0;

    /* This function selects a driver abstraction layer (DAL). If support for the
    intended one isn't allowed with the present license, this returns NO_LICENSE.
    A list for DALs (or bDriverID) is shown below:
    DRIVER_STUB         : DAL for Stub
    DRIVER_ES520        : DAL for ES520
    DRIVER_TZMCYCLONE   : DAL for TZMCyclone
    DAL_NONE            : Dummy DAL */
    virtual HRESULT DILF_SelectDriver(DWORD dwDriverID, HWND hWndParent) = 0;

    /**
     * DILF_RegisterClient registers/unregisters the client.
     * bRegister = TRUE for registeration
     * bRegister = FALSE for unregisteration
     * Only registered client's buffer will be updated on receive of a msg in the bus.

     * Following are the return values
     * S_OK -->registeration successful
     * ERR_CLIENT_EXISTS -->Client already registered. Client Id will be stored in ClientId.
     * ERR_NO_CLIENT_EXIST --> No client exists. Usually this happens if we try to remove non existing client.
     * ERR_NO_MORE_CLIENT_ALLOWED -->No more clients are allowed to register.
     */
    virtual HRESULT DILF_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName) = 0;

    /**
     * This function manages the target message buffer list. The two combinations
     * are the following:
     * bAction = MSGBUF_ADD, add pBufObj to the target message buffer list.
     * bAction = MSGBUF_CLEAR, clear the list. pBufObj is ignored.
     * At present maximum number of entries in the list is kept as 8.
     */
    virtual HRESULT DILF_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseFLEXBufFSE* pBufObj) = 0;


    /* This returns ID of the driver interface layer presently selected. */
    virtual DWORD DILF_GetSelectedDriver(void) = 0;

    /* Call for all initialisation operations. */
    virtual HRESULT DILF_PerformInitOperations(void) = 0;

    /* Call for all uninitialisation operations */
    virtual HRESULT DILF_PerformClosureOperations(void) = 0;

    /* Call this function to get a system time and the time stamp associated with it*/
    virtual HRESULT DILF_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp) = 0;

    /* Call this function to list the hardware interfaces available and receive
    user's choice. */
    virtual HRESULT DILF_ListHwInterfaces( INTERFACE_HW& sSelHwInterface ) = 0;

    /* Call this function to select a particular hardware interface. */
    virtual HRESULT DILF_SelectHwInterfaces( const INTERFACE_HW_LIST& sSelHwInterface ) = 0;

    /* Call this function to deselect the selected hardware interface. */
    virtual HRESULT DILF_DeselectHwInterfaces(void) = 0;

    /* To set the configuration data for the currently selected DIL. Caller must
    release the memory. */
    //virtual HRESULT DILF_SetConfigData(PCHAR pInitData,  ABS_CLUSTER_INFO   asClusterinfo, int Length, ABS_DEVICE_CONFIG  asDeviceConfig) = 0;
    virtual HRESULT DILF_SetConfigData(IBMNetWorkGetService* ouAbsSFibexContainer) = 0;

    /* Start the controller*/
    virtual HRESULT DILF_StartHardware(void) = 0;

    /* Stop the controller*/
    virtual HRESULT DILF_StopHardware(void) = 0;

    /* Reset Hardware*/
    virtual HRESULT DILF_ResetHardware(void) = 0;

    /* Send Flx messages*/
    virtual HRESULT DILF_SendFlxMsg(DWORD dwClientID, s_FLXDATAMSG* pouFlxTxMsg) = 0;

    /* Send PDU */
    virtual HRESULT DILF_SendPdu(DWORD dwClientID, s_FLXPDUDATAMSG* pouFlxTxMsg, bool bSetUpdateBit) = 0 ;

    /* Send Flx messages*/
    virtual HRESULT DILF_DeleteFlxMsg(DWORD dwClientID, s_FLXDATAMSG* pouFlxTxMsg) = 0;


    /* Call to receive the version informations */
    virtual HRESULT DILF_GetVersionInfo(VERSIONINFO& sVerInfo) = 0;

    /* Call to get descriptive string of the last error occurred */
    virtual HRESULT DILF_GetLastErrorString(std::string acErrorStr, HRESULT& nError ) = 0;

    virtual HRESULT DILF_GetControllerCapabilities(s_FLXControllerCapabilities& ouFlexRayCapabilities) = 0;

    virtual HRESULT DILF_GetControllerParams(std::list<FlexRayControllerParams>&) = 0;

};
