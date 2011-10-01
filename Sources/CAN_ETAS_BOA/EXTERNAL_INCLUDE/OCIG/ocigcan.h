#ifndef ETAS_OCIG_CAN_H
#define ETAS_OCIG_CAN_H

/**
* @file       ocican.h
* @brief      CAN specific part of the Open Contoller Interface (OCIG) API.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*/


#include "ocigctrl.h"
#include "..\OCI\ocicanio.h"

#include "..\Common\pshpack1.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @addtogroup GROUP_OCIG_CAN_CONTROLLER OCIG CAN
* @brief      CAN specific part of the Generic Open Controller Interface (OCIG). 
* @details    The CAN controller interface instance supports a single 
*             TxQueue and a single RxQueue. All CAN frames will be 
*             queued and transmitted in the same order as they are 
*             delivered via @ref OCIG_WriteCANData() to the TxQueue.
* @{
*/

/**
* @addtogroup GROUP_OCIG_CAN_INITIALIZATION Initialization
* @{
*/

/**
* Create a new CAN controller instance which is bound to the specified
* URI location. 
* @param[in]  uriLocation  is a URI compatible name which identifies
*                          the controller. 
* @param[out] controller   receives the handle to the created controller 
*                          interface instance.
* @return     If the operation was successful and the controller could
*             be found, @ref OCI_SUCCESS will be returned. If the 
*             controller could not be found, @ref OCI_ERR_HW_NOT_PRESENT 
*             will be returned. If the controller is already created, 
*             @ref OCI_ERR_INVALID_STATE will be returned.
*             Otherwise an specific error code will be returned.
* @sa         @ref GROUP_OCIG_CONTROLLER 
*             "Controller Handling"
* @n          @ref PAGE_BOA_ADDRESSINGSCHEME 
*             "Addressing scheme of the Basic OpenAPI" 
*
* @todo       Document error codes returned by this function.
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCIG_CreateCANController(
    const char* uriLocation, 
    OCI_ControllerHandle* controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCIG_CreateCANController)(
    const char* uriLocation, 
    OCI_ControllerHandle* controller);


/**
* Create a new CAN controller instance. Binding parameters are
* specified in the form of a protocol stack.
* @param[in]  protocolStack  Contains access parameters required to bind
*                            to the appropriate controller instance
* @param[out] controller     receives the handle to the created controller 
*                            interface instance
* @return     If the operation was successful and the controller could
*             be found, @ref OCI_SUCCESS will be returned. If the 
*             controller could not be found, @ref OCI_ERR_HW_NOT_PRESENT 
*             will be returned. If the controller is already created, 
*             @ref OCI_ERR_INVALID_STATE will be returned.
*             Otherwise an specific error code will be returned.
* @sa         @ref GROUP_OCIG_CONTROLLER "Controller Handling"
*
* @todo       Document error codes returned by this function
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCIG_CreateCANControllerBind(
    const BOA_ProtocolStack* protocolStack,
    OCI_ControllerHandle* controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCIG_CreateCANControllerBind)(
    const BOA_ProtocolStack* protocolStack,
    OCI_ControllerHandle* controller);


/** @} */


/**
* @addtogroup GROUP_OCIG_CAN_INTERNAL Internal Structures
* @brief      Additional definitions relevant for OCI-Generic implementors. 
* @{
*/

/**
* Internal identifier for the OCI-Generic CAN interface.
*/

#define UUID_OCIGCAN {0x42,0xDB,0xF2,0x2F,0x14,0x8C,0x40,0x3f,0xB0,0x89,0x03,0x29,0xAF,0xBC,0xF0,0xAA}


/**
* Internal v-table for the OCI-Generic CAN interface.
*/

typedef struct OCIG_CAN_VTable
{
    /* ocitime.h */

    OCI_Time_VTable timeVTable;

    /* ocierror.h */

    OCI_Error_VTable errorVTable;

    /* ociqueue.h */

    OCI_Queue_VTable queueVTable;

    /* ocigctrl.h */

    OCIG_Ctrl_VTable ctrlVTable;

    /* ocicanio.h */

    OCI_CANIO_VTable canioVTable;

    /* ocigcan.h */

    PF_OCIG_CreateCANController createCANController;
    PF_OCIG_CreateCANControllerBind createCANControllerBind;

} OCIG_CAN_VTable;

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#include "..\Common\poppack.h"

#endif

