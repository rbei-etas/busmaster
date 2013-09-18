#ifndef ETAS_OCI_BASE_H
#define ETAS_OCI_BASE_H

/**
* @file       ocibase.h
* @brief      I/O medium-independent part of the Open Controller Interface (API).
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4509 $
*/

#include "..\Common\boaservice.h"
#include "ocitime.h"
#include "ocierror.h"
#include "ociqueue.h"
#include "ocictrl.h"

/**
* @addtogroup GROUP_OCI_COMMON OCI Common
* @brief      Common part of the Open Controller Interface (OCI).
* @{
*
* @defgroup GROUP_OCI_CONTROLLER Controller Handling
* @ingroup  GROUP_OCI_COMMON
*
* The Open Controller Interface (OCI) implements a state machine 
* for the Controller (CAN-Controller, LIN-Controller, FlexRay-
* Controller).\ All functions use bus interface specific names and 
* parameter, but have a common structure.
*
* The API use three states of a Controller:
* @li Created 
*      - The controller is created and a basic set of functions
*        can be executed. The controller is not configured. So
*        it is not possible to take part in the bus communication
* @li Suspended 
*      - The controller is configured, but set into a suspended
*        state. It is possible to use any function that uses a
*        hController, but the controller will not interact
*        with the bus system. This state allows the configuration
*        of several filter conditions. Switching to the Running 
*        state will activate all new settings at the same time.
* @li Running 
*      - The controller is configured and takes part in the bus
*        communication. It is possible to use any function that
*        uses a hController. Any change will immediately be
*        active.  
*
* The following figure gives an overview of the supported functions
* for a controller. A detailed description of the different data 
* structures and functions for the different Bus Interfaces can be 
* found here:
*
* @sa    @ref GROUP_OCI_CAN_CONTROLLER, 
*        @ref GROUP_OCI_LIN_CONTROLLER, 
*        @ref GROUP_OCI_FLEXRAY_CONTROLLER
*
* <HR>
* @image html "figure_oci_controllerstates.gif"
*
*
* @defgroup GROUP_OCI_EVENT_HANDLING Event Handling
* @ingroup  GROUP_OCI_COMMON
*
* The event handling in the Open Controller Interface allows an
* in band handling of events by inserting events into the receive
* data stream and fast reaction based on a callback mechanism.
* 
* Any event can be registered to occur in the rxQueue. In this case
* the event will be inserted at the corresponding position in the
* sequence of received frames.
*
* Any event can be registered to trigger the (optional) callback of
* the queue, too. In this case the the event will trigger the callback
* and the software can immediately react on the event. The event code,
* timestamp, filter tag and event specific data is passed to the callback.
*
* It is possible to enable both paths at the same time.
*
* <HR>
* @image html "figure_oci_eventhandling.gif"
*
* @}
*/

#endif
