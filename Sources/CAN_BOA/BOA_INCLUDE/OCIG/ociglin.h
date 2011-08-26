#ifndef ETAS_OCIG_LIN_H
#define ETAS_OCIG_LIN_H

/**
* @file       ocilin.h
* @brief      LIN specific part of the Generic Open Contoller Interface (OCIG) API.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*/


#include "ocigctrl.h"
#include "..\OCI\ocilinio.h"

#include "..\Common\pshpack1.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
* @addtogroup GROUP_OCIG_LIN_CONTROLLER OCIG LIN
* @brief      LIN specific part of the Open Controller Interface (OCIG). 
* @{
*/

/**
* @addtogroup GROUP_OCIG_LIN_INITIALIZATION Initialization
* @{
*/

/**
* Create a new LIN controller instance which is bound to the specified
* URI location. 
* @param[in]  uriLocation  is a URI compatible name which identifies
*                          the controller. 
* @param[out] controller   receives the handle to the created LIN controller 
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
* @todo      Document the syntax of the uriLocation parameter.
* @todo      Document error codes returned by this function.
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCIG_CreateLINController(
    const char* uriLocation, 
    OCI_ControllerHandle* controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCIG_CreateLINController)(
    const char* uriLocation, 
    OCI_ControllerHandle* controller);


/**
* Create a new LIN controller instance. Binding parameters are
* specified in the form of a protocol stack.
* @param[in]  protocolStack  Contains access parameters required to bind
*                            to the appropriate controller instance.
* @param[out] controller     receives the handle to the created controller 
*                            interface instance.
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
OCI_CALL OCIG_CreateLINControllerBind(
    const BOA_ProtocolStack* protocolStack,
    OCI_ControllerHandle* controller);
#endif    

typedef OCI_ErrorCode
(OCI_CALL *PF_OCIG_CreateLINControllerBind)(
    const BOA_ProtocolStack* protocolStack,
    OCI_ControllerHandle* controller);

/** @} */

/**
* @addtogroup GROUP_OCI_LIN_INTERNAL Internal Structures
* @brief      Additional definitions relevant for OCI-Generic implementors. 
* @{
*/

/**
* Internal identifier for the OCI-Generic LIN interface.
*/

#define UUID_OCIGLIN {0xAE,0xC9,0xFD,0x57,0x8B,0x8C,0x4a,0x8a,0xBD,0xD4,0x74,0x6D,0x2B,0x10,0x75,0x7B}

/**
* Internal v-table for the OCI-Generic LIN interface.
*/

typedef struct OCIG_LIN_VTable
{
    // ocitime.h

    OCI_Time_VTable timeVTable;

    // ocierror.h

    OCI_Error_VTable errorVTable;

    // ociqueue.h

    OCI_Queue_VTable queueVTable;

    // ocigctrl.h

    OCIG_Ctrl_VTable ctrlVTable;

    // ocilinio.h

	struct OCI_LINIO_VTable linioVTable;

    // ociglin.h

    PF_OCIG_CreateLINController createLINController;
    PF_OCIG_CreateLINControllerBind createLINControllerBind;

} OCIG_LIN_VTable;

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#include "..\Common\poppack.h"

#endif

