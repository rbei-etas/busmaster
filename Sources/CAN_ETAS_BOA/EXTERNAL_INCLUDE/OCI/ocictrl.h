#ifndef ETAS_OCI_CTRL_H
#define ETAS_OCI_CTRL_H

/**
* @file       ocictrl.h
* @brief      Public declaration of Block Device Driver API.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4636 $
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
* @addtogroup GROUP_OCI_COMMON OCI Common
* @{
*/

/**
* @addtogroup GROUP_OCI_CONTROLLER Controller Handling
* @{
*/

/**
* OCI property values for the mode parameter of the properties 
* structure of any controller. These are possible values for: 
* @li @ref OCI_CANControllerProperties.mode
* @li @ref OCI_LINControllerProperties.mode
* @li @ref OCI_FlexRayControllerProperties.mode
*/

typedef enum OCI_ControllerPropertiesMode
{
    /** 
    * Specifies that the controller interface instance is running. 
    */
    OCI_CONTROLLER_MODE_RUNNING    = 0,
    
    /** 
    * Specifies that the controller interface instance is suspended. 
    * RX acquisition is suspended. The transmit scheduler does not 
    * service the instance's TX queue(s).
    */
    OCI_CONTROLLER_MODE_SUSPENDED  = 1

} OCI_ControllerPropertiesMode;

/**
* OCI property values for the selfReceptionMode parameter of the 
* controller and RX queue configurations. FlexRay supports a more
* elaborate scheme for the controller, see @ref OCI_FlexRayControllerBusMode.
* These are possible values for: 
* @li @ref OCI_CANConfiguration.selfReceptionMode
* @li @ref OCI_LINConfiguration.selfReceptionMode
* @li @ref OCI_CANRxQueueConfiguration.selfReceptionMode
*/

typedef enum OCI_SelfReceptionMode
{
    /** 
    * Specifies that the controller will not put frames into the
    * receive queue if they were sent though the same port. 
    */
    OCI_SELF_RECEPTION_OFF  = 0,
    
    /** 
    * Specifies that the controller will put frames into the
    * receive queue if they were sent though the same port. 
    * Some controllers may not support that in HW. Therefore,
    * the frames will be looped back from the TX queue into the
    * RX queue in software. This may procude a slightly inaccurate
    * frame order and timestamping.
    */
    OCI_SELF_RECEPTION_ON    = 1

} OCI_SelfReceptionMode;

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_GetUnmappedHandle(
	OCI_ControllerHandle handle,
    OCI_ControllerHandle* unmappedHandle);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetUnmappedHandle)(
	OCI_ControllerHandle handle,
    OCI_ControllerHandle* unmappedHandle);

/** @} */

/** @} */


#ifdef __cplusplus
}
#endif

#endif
