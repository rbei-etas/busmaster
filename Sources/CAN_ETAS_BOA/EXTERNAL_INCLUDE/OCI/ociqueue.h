#ifndef ETAS_OCI_QUEUE_H
#define ETAS_OCI_QUEUE_H

/**
* @file       ociqueue.h
* @brief      Generic queue part definitions of the 
*             Open Controller Interface (API).
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4810 $
*/

#include "ocitypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @addtogroup GROUP_OCI_COMMON OCI Common
* @{
*/

/**
* @addtogroup GROUP_OCI_QUEUE Queue Handling
* 
* The Open Controller interface uses Queues to transmit and receive
* any data. When a Controller is in the state Suspended or Running
* it is possible to create a Rx-Queue and one or more Tx-Queues. The
* number of supported Tx-Queues is dependent from the bus interface
* type.
*
* <HR>
* @image html "figure_oci_queuehandling.gif"
* @{
*/

/**
* @anchor  ANCHOR_OCI_EVENT_DESTINATION_FLAGS
* @name    OCI event destination flags.
* @brief   Constants to specify the destination of a generated queue 
*          event. These constants are valid for 
*          @ref OCI_QueueEventMessage.destination.
*/

/** @{ */

/** 
* The destination of an enabled event is the event handler of the read 
* queue. May be used in combination with @ref OCI_EVENT_DESTINATION_CALLBACK
* when the event shall trigger a callback and the event shall be stored 
* in-band.
* @sa @ref PAGE_OCI_RECEIVE_MESSAGES "Receive Messages"
*/
#define OCI_EVENT_DESTINATION_INBAND      (0x01u)

/** 
* The destination of an enabled event is the receive queue. May be 
* used in combination with @ref OCI_EVENT_DESTINATION_INBAND when the 
* event shall trigger a callback and the event shall be stored in-band.
* @sa @ref PAGE_OCI_RECEIVE_MESSAGES "Receive Messages"
*/
#define OCI_EVENT_DESTINATION_CALLBACK    (0x02u)

/** @} */


/**
* Non static read only information of a queue (status). This 
* information may change during the run-time by different events. 
*/

typedef struct OCI_QueueStatus
{
    /** 
    * Current number of used entries.
    */
    uint32 used;  

    /** 
    * Maximum number of used entries since last reset. 
    * @sa @ref OCI_ResetQueue().
    */
    uint32 maxUsed;  

} OCI_QueueStatus;


/**
* Non static information of a queue, that can be access and modified 
* by the @ref OCI_SetQueueProperties() function.
*/

typedef struct OCI_QueueProperties
{
   /**
   * @todo  Comments are missing!
   */
   uint32 lowWatermark;
   
   /**
   * @todo  Comments are missing!
   */
   uint32 highWatermark;

} OCI_QueueProperties;


/** 
* Static information about a queue. This information will never change. Note that this information refers only
* to the queue implemented by the OCD. Other queues may exist below the OCD, e.g. in the bus controller hardware.
*/

typedef struct OCI_QueueCapabilities
{
    /** 
    * Capacity, i.e.\ maximum available entries. This must be a 
    * guaranteed capacity.
    */ 
    uint32 minCapacity;
    
    /**
    * Maximum capacity of the queue in entries. When this value it 
    * equal to @ref minCapacity the queue has a static size. When the
    * queue supports dynamic sizing, but can not specify any upper 
    * specific limit (e.g.\ because if sharing RAM with other dynamic 
    * sized queues) the value should be MAX_UINT32. 
    */
    uint32 maxCapacity;

} OCI_QueueCapabilities;


/** 
  Discard all entries of a specific queue. 

  In case of a receive queue the filter conditions are not affected by this call. Any white filter that was added 
  to this queue will stay active. 
  
  In case of a transmit queue all queued entries will be removed from the queue. Frames already stored in the
  controller, but stalled by by higher prior frames from other controller (CAN) or stalled by the scheduling 
  algorithm (LIN, FlexRay) are not affected and may be send after completion of this call. 

  In case of a transmit queue of a LIN slave, the LIN slave is set into passive mode which means 
  the LIN slave is only listening. The LIN slave will be activated again, when data is written into
  the transmit queue.
  
  Because of the asynchronous character of the call and to avoid racing conditions it is recommended to use this
  call only in the @a SUSPENDED state of the controller. Otherwise queue events that have fired before the call 
  is executed may be delivered to the application, even when they become invalid by the actions of the call 
  (e.g.\ there is no longer the promised number of frames waiting to be read). This may require a complex handling
  in the application.

  @controllerState RUNNING(not recommended), SUSPENDED. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.

  @param[in]  queue   Handle of the queue that is reset. 

  @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyCANController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE>
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_ResetQueue(
    OCI_QueueHandle queue);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_ResetQueue)(
    OCI_QueueHandle queue);

/** 
  Read the current status information of a specific queue. 
  
  The status information of a queue instance may change by asynchronous events.

  When a receive queue is configured to generate callbacks for received frames and events 
  the @a status will return a filling level and a maximum filling level of 0.

  @controllerState RUNNING, SUSPENDED. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.

  @param[in]  queue    Handle of the queue to be queried. 
  @param[out] status   Pointer to a buffer that will receive the read @a queue statistics. 
                       Must not be @c NULL.

  @todo ???For write-only links, all members of the @ref OCI_QueueStatus structure will be 0.

  @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a status paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyCANController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE>
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_GetQueueStatus(
    OCI_QueueHandle queue, 
    OCI_QueueStatus* status);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetQueueStatus)(
    OCI_QueueHandle queue, 
    OCI_QueueStatus* status);


/**
  Read the capability information of a specific queue.

  Capablities are static information that will never change for specific queue instance.

  @controllerState RUNNING, SUSPENDED. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.

  @param[in]  queue         Handle of the queue to be queried.
  @param[out] capabilities  Pointer to a buffer that receives the capabilities of the queue. 
                            Must not be @c NULL.

  @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a capabilities paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyCANController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE> 
*/
#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_GetQueueCapabilities(
    OCI_QueueHandle queue, 
    OCI_QueueCapabilities* capabilities);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetQueueCapabilities)(
    OCI_QueueHandle queue, 
    OCI_QueueCapabilities* capabilities);


/**
 Query property information of a specific queue.
 
 The properties are static information, that will only change when new properties are set by calling 
 OCI_SetQueueProperties().

 @controllerState RUNNING, SUSPENDED. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.

 @param[in]  queue        Handle of the queue instance for which the properties are requested.
 @param[out] properties   Pointer to a buffer receiving the requested property information of the queue.
 
 @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a capabilities paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyCANController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE> 
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_GetQueueProperties(
    OCI_QueueHandle queue, 
    OCI_QueueProperties* properties);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetQueueProperties)(
    OCI_QueueHandle queue, 
    OCI_QueueProperties* properties);



/**
  Modify the properties of a queue.

  Properties are read/writeable and can be queried OCI_GetQueueProperties().

  @controllerState RUNNING, SUSPENDED. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.

  @param[in]  queue       Handle of the queue to query.

  @param[out] properties  Pointer to a buffer that will receive the queue properties. 
                          Must not be @c NULL.
  
  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a queue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a properties paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyCANController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE> 

*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_SetQueueProperties(
    OCI_QueueHandle queue, 
    OCI_QueueProperties* properties);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_SetQueueProperties)(
    OCI_QueueHandle queue, 
    OCI_QueueProperties* properties);


/**
* Kinds of queue events. 
* These are valid values for @ref OCI_QueueEventMessage.eventCode.
*/
typedef enum OCI_QueueEvent
{
   /** 
   * An underrun condition has occurred in the queue. 
   */
   OCI_QUEUE_UNDERRUN = 0,
   
   /** 
   * The queue has been emptied. 
   */
   OCI_QUEUE_EMPTY = 1,
   
   /** 
   * The status of the queue has changed from empty to not empty. 
   */
   OCI_QUEUE_NOT_EMPTY = 2,
   
   /** 
   * The filling level of the queue has dropped above its low water 
   * mark. 
   */
   OCI_QUEUE_LOW_WATERMARK  = 3,

   /** 
   * The filling level of the queue has risen above its high water 
   * mark. */
   OCI_QUEUE_HIGH_WATERMARK = 4,

   /** 
   * The queue is now full 
   */
   OCI_QUEUE_FULL = 5,
   
   /** 
   * The queue has overflown and some message could not be stored in 
   * the queue anymore. In case of a receive queue, the extra messages 
   * have been discarded. 
   */
   OCI_QUEUE_OVERFLOW = 6

} OCI_QueueEvent;

/**
* Message that is generated when a queue event fires and a filter
* for that event was set by calling @ref OCI_AddQueueEventFilter.
* @sa @ref OCI_AddQueueEventFilter, @ref OCI_RemoveQueueEventFilter. 
*/

typedef struct OCI_QueueEventMessage
{
    /** 
    * Global receive timestamp. Depending on the implementation, the timestamp may be based on a
    * hardware-internal timer with no global synchronization, or the timestamp may be completely
    * unavailable (in which case the value OCI_NO_TIME is used). In particular, the epoch
    * (i.e.\ value 0) may be the point in time the hardware was switched on.
    */
    OCI_Time timeStamp;

    /**  
    * Application supplied value, that is set by the registration of 
    * the filter condition @ref OCI_AddQueueEventFilter().
    */
    uint32 tag;

    /** code for the event that generated this message */
    OCI_QueueEvent eventCode;

    /** 
    * Destination of the event. This parameter is a combination of 
    * flags. The value matches the value of the first event 
    * filter that caused this event to be received. 
    * @flags       The following flags are valid:
    * @n           @ref OCI_EVENT_DESTINATION_INBAND
    * @n           @ref OCI_EVENT_DESTINATION_CALLBACK
    * @sa          ANCHOR_OCI_EVENT_DESTINATION_FLAGS
    *              "OCI event destination flags" 
    */
    uint32 destination;

} OCI_QueueEventMessage;


/**
* Definition of an event filter for queue events. The filter matches 
* when an event with the given @ref eventCode is generated.
*/
typedef struct OCI_QueueEventFilter
{
    /** 
      This handle specifies the queue which is the source of the events which are matched by this filter.

      There are two possible use cases for this filter:
        -# using this filter to specify how to handle queue events which occur on an RX queue;
        -# using this filter to specify how to handle queue events which occur on a TX queue. For example it is possible to
           implement an event driver filling routine for a transmit queue (e.g. refill queue, when low watermark is reached
           to avoid a pause on the related bus system).

      In both cases, the queue events must be added to an RX queue, namely the RX queue to which this filter is added via
      OCI_AddQueueEventFilter(). In both cases the queue events are added to the RX queue if they match this filter.

      In the first use case, the RX queue to which the events are added is the same queue on which the events occurred.
      Therefore @ref sourceQueue must identify the same RX queue to which this filter is added via OCI_AddQueueEventFilter().
      Alternatively, @ref sourceQueue can be set to @ref OCI_NO_HANDLE to achieve the same result.

      In the second use case, the events occur on a different queue from the one to which this filter is added via
      OCI_AddQueueEventFilter(). @ref sourceQueue must match the TX queue on which the events occur.
    */
    OCI_QueueHandle sourceQueue;
    
    /** 
    * Kind of the event which this filter shall match. 
    */
    OCI_QueueEvent eventCode;

    /** 
    * Application defined value with which an event is tagged with 
    * when it matches this filter. This value will be written into 
    * @ref OCI_QueueEventMessage.tag.
    */
    uint32 tag;

    /** 
    * Combination of flags that specify the destination of the event. 
    * @flags    The following flags are valid:
    * @n        @ref OCI_EVENT_DESTINATION_CALLBACK. If this flag is 
    *           set, the application defined callback that has been 
    *           registered with the receive queue for which this filter 
    *           is registered is called.
    * @n        @ref OCI_EVENT_DESTINATION_INBAND. If this flag is 
    *           set, the event is stored in-band into the receive 
    *           queue for which this filter is registered.
    *           This flag @a must @a not be set if @ref sourceQueue is 
    *           equal to the read Queue for which this filter is added. 
    * @n        The combination of the above flags causes the event to 
    *           be duplicated. 
    * @sa       ANCHOR_OCI_EVENT_DESTINATION_FLAGS
    *           "OCI event destination flags"
    * @sa       @ref OCI_FlexRayRxQueueConfiguration
    * @sa       @ref OCI_CANRxQueueConfiguration
    * @sa       @ref OCI_LINRxQueueConfiguration
    */
    uint32 destination;

} OCI_QueueEventFilter;


/**
  Add white filter condition(s) to enable the reception of Queue events for a specific receive queue.

  In the default configuration of a receive queue no data will be received because any data is blocked by
  some filter condition. The application must enable the reception of the data by setting white filter 
  (defining pass conditions). 

  This function allows the reception of specific events of a Queue. In principle the events may be delivered 
  in-band in the normal data stream or generate a (separate) callback. The signalization path to the application 
  is defined by the creation of the receive queue. (See also OCI_CreateCANRxQueue(), 
  OCI_CreateLINRxQueue(), OCI_CreateFlexRayRxQueue).

  This function is useful to trigger a filling level based reading of a queue. In this case the queue should
  be initialized without callback routines for frame recption. The filling level should fire events and a callback 
  calls OCI_ReadCANData(), OCI_ReadLINData() or OCI_ReadFlexRayData(), when the @a OCI_QUEUE_HIGH_WATERMARK event
  is identified. On a Bus with low traffic or on a queue with strict filter conditions waiting for a specific
  filling level may cause a high latency in data processing.

  @controllerState RUNNING, SUSPENDED. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.

  @param[in]  rxQueue  Handle of the receive queue that will receive the event(s).
  @param[in]  filter   Pointer to an array of @a count entries of type @ref OCI_QueueEventFilter,
                       which will generate events in future.
  @param[in]  count    Number of entries in the @a filter array.

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a rxQueue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a filter paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td>
        <td> One or more @a filter objects are invalid or already exist.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_NO_RESPONSE </td>
        <td> The driver for a removable hardware received no response to a request. The hardware may be removed. 
             It is not known wether the request is lost and the command not exectuted or the response is lost and
             the command was executed. The controller enters the @a DISCONNECTED state. All subsequent calls will 
             return @ref OCI_ERR_DRIVER_DISCONNECTED. 
             Internal resources are locked until OCI_DestroyCANController() for the controller is called. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_DRIVER_DISCONNECTED </td>
        <td> The driver for a removable hardware detected a communication problem (in asynchromous communication, 
             by event signalling or during a previous call to the same hardware) and is not able to communicate to 
             the hardware any more. 
             The state of the controller is @a DISCONNECTED. The command is not executed. All subsequent calls to 
             the same Controller will return @ref OCI_ERR_DRIVER_DISCONNECTED. Internal resources are locked until 
             OCI_DestroyCANController() for the controller is called.  </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>

  </TABLE> 

  @ingroup    GROUP_OCI_EVENT_HANDLING
  
  @sa         GROUP_OCI_EVENT_HANDLING   
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_AddQueueEventFilter(
    OCI_QueueHandle rxQueue,
    OCI_QueueEventFilter filter[], 
    uint32 count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_AddQueueEventFilter)(
    OCI_QueueHandle rxQueue,
    OCI_QueueEventFilter filter[], 
    uint32 count);


/**
  Remove a white filter (defining pass conditions) for some read queue event(s).

  In the default configuration of a receive queue no data and no event will be received because any data 
  is blocked by some filter condition. The application must enable the recpetion of the data or event by 
  setting white filter (defining pass conditions). 

  This function allows to withdraw one or more white filter conditions set by OCI_AddQueueEventFilter(). Each
  @ref OCI_QueueEventFilter of the @a filter array must exact match to a previously added filter. It is
  not possible to restrict an existing white filter, without redrawing it and adding a new more restrictive
  one.
  
  @controllerState RUNNING, SUSPENDED. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.  \n
                   When the controller is in the DISCONNECTED state the function may be called to do local clean
                   up. Only a limitied plausibility check may be done in this case.  

  @param[in]  rxQueue  Handle of the receive queue that will not longer receive the event(s).
  @param[in]  filter   Pointer to an array of @a count entries of @ref OCI_QueueEventFilter, 
                       which will be remove from the filter list.
  @param[in]  count    Number of entries in the @a filter array.

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a rxQueue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a filter paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td>
        <td> One or more filter objects are not set and can not be removed. The processing of the
             filter array is undefined. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE> 

  @ingroup    GROUP_OCI_EVENT_HANDLING
 
  @sa         GROUP_OCI_EVENT_HANDLING   
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_RemoveQueueEventFilter(
    OCI_QueueHandle rxQueue,
    OCI_QueueEventFilter filter[],
    uint32 count);
#endif    

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_RemoveQueueEventFilter)(
    OCI_QueueHandle rxQueue,
    OCI_QueueEventFilter filter[],
    uint32 count);


typedef struct OCI_Queue_VTable
{
    PF_OCI_ResetQueue resetQueue;
    PF_OCI_GetQueueStatus getQueueStatus;
    PF_OCI_GetQueueCapabilities getQueueCapabilities;
    PF_OCI_GetQueueProperties getQueueProperties;
    PF_OCI_SetQueueProperties setQueueProperties;
    PF_OCI_AddQueueEventFilter addQueueEventFilter;
    PF_OCI_RemoveQueueEventFilter removeQueueEventFilter;
} OCI_Queue_VTable;

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
