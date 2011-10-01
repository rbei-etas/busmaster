#ifndef ETAS_OCIG_FLEXRAY_H
#define ETAS_OCIG_FLEXRAY_H

/**
* @file       ocigflx.h
* @brief      FlexRay specific part of the Open Controller Interface (OCIG) API.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*/


#include "ocigctrl.h"
#include "..\OCI\ociflxio.h"

#include "..\Common\pshpack1.h" 

#ifdef __cplusplus
extern "C" {
#endif


/**
* @addtogroup GROUP_OCIG_FLEXRAY_CONTROLLER OCIG FlexRay
* @brief      FlexRay specific part of the Generic Open Controller Interface (OCIG). 
* @{
*/

/**
* @addtogroup GROUP_OCIG_FLEXRAY_INITIALIZATION Initialization
* @{
*/

/**
* Create a new FlexRay controller instance which is bound to the 
* specified URI location. 
* @param[in]  uriLocation   is a URI compatible name which identifies
*                           the controller. 
* @param[out] controller    receives the handle to the created 
*                           FlexRay controller interface instance.
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
OCI_CALL OCIG_CreateFlexRayController(
    const char* uriLocation, 
    OCI_ControllerHandle* controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCIG_CreateFlexRayController)(
    const char* uriLocation, 
    OCI_ControllerHandle* controller);


/**
* Create a new FlexRay controller instance. Binding parameters are
* specified in the form of a protocol stack.
* @param[in]  protocolStack  contains access parameters required to bind
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
OCI_CALL OCIG_CreateFlexRayControllerBind(
    const BOA_ProtocolStack* protocolStack,
    OCI_ControllerHandle* controller);
#endif 

typedef OCI_ErrorCode
(OCI_CALL *PF_OCIG_CreateFlexRayControllerBind)(
    const BOA_ProtocolStack* protocolStack,
    OCI_ControllerHandle* controller);


/** @} */

/**
* @addtogroup GROUP_OCIG_FLEXRAY_INTERNAL Internal Structures
* @brief      Additional definitions relevant for OCI-Generic implementors. 
* @{
*/

/**
* Internal identifier for the OCI-Generic FlexRay interface.
*/

#define UUID_OCIGFLX {0x9B,0xE6,0xC1,0x40,0xA2,0x80,0x44,0x2b,0x90,0x53,0xC5,0x79,0x04,0x50,0x47,0xA6}


/**
* Internal v-table for the OCI-Generic FlexRay interface.
*/

typedef struct OCIG_FLEXRAY_VTable
{
    /* ocitime.h */

	OCI_Time_VTable timeVTable;

    /* ocierror.h */

    OCI_Error_VTable errorVTable;

    /* ociqueue.h */

	OCI_Queue_VTable queueVTable;

    /* ocigctrl.h */

    OCIG_Ctrl_VTable ctrlVTable;

    /* ociflxio.h */

	OCI_FLEXRAYIO_VTable flexRayIOVTable;

    /* ocigflx.h */

    PF_OCIG_CreateFlexRayController createFlexRayController;
    PF_OCIG_CreateFlexRayControllerBind createFlexRayControllerBind;

} OCIG_FLEXRAY_VTable;

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#include "..\Common\poppack.h"

#endif
