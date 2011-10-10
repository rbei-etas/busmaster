#ifndef ETAS_OCI_TIME_H
#define ETAS_OCI_TIME_H

/**
* @file       ocitime.h
* @brief      Timer handling functionality of the 
*             Open Controller Interface (OCI) API.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4799 $
*/

#include "ocitypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/** constant large enough to hold the normal string reprsentation of an UUID
    ("00000000-0000-0000-0000-000000000000") */
#define UUID_STRING_SIZE 40

/**
* @addtogroup GROUP_OCI_COMMON OCI Common
* @{
*/

/**
* @addtogroup GROUP_OCI_TIMER_HANDLING Timer Handling
* @{
*
* @todo Extend ETAS ethernet synchronization to support 64Bit 
* timestamping. Check possibilities and effort to transmit additional 
* epoch/sequenceNo information for the 4000s interval. (e.g.\ as 
* virtual serial-number in the hierarchy stack or as an additional 
* private software negotiatiation protocol)
*
* @internal: Existing ETAS system can only be trimmed in a small 
* range by accessing a VXCO. Any modification on an Ethernet port 
* must not exceed 50ppm!
*/

/**
 * Format of the timestamp is 64 Bit binary. 
 */
#define OCI_TIMER_BINARY_64BIT 0x01

/**
 * synchronization is locked to an external reference.
 */
#define SYNC_LOCKED                        (1<<0)

/**
 * indication of a positive leap second at the end of the day.
 */
#define POSITIVE_LEAP_SECOND_AT_END_OF_DAY (1<<1)

/**
 * indication of a negative leap second at the end of the day.
 */
#define NEGATIVE_LEAP_SECOND_AT_END_OF_DAY (1<<2)


/**
 * Reference system that is used by the timer.
 */
typedef enum OCI_TimeReferenceScale
{
   OCI_TimeReferenceScaleUnknown = 0,
   OCI_TimeReferenceScaleTAI     = 1,
   OCI_TimeReferenceScaleUTC     = 2
} OCI_TimeReferenceScale; 


/**
 * Non static read only information of a timer (status). This information
 * may change during the run-time by different events. 
 */
typedef struct OCI_TimerStatus
{
    /** 
     * Pointer to the UTF8 encoded timer ID, that is currently used
     * for timestamping. ETAS recommends to use a string representation 
     * of an UUID for the ID. If two clockIDs are equal, it is assumed 
     * that both designate the same timer / clock source, deliver the same
     * absolute value and do not drift.
     UUIDs should use a ISO/IEC 9834-8:2005 / RFC 4122 compatible format.
     (http://www.itu.int/ITU-T/studygroups/com17/oid/X.667-E.pdf)
     *
     * @internal ETAS can use a "reserved" MAC-Address and map the serial
     * number of the sync generating switch to the timestamp field to
     * generate a UUID from the ETAS sync information.
     * 
     * ==> local-clock: "00000000-0000-1000-8000-MyMacAddress"
     *
     * ==> ETAS-synced: "switchSN-0000-1000-8000-reservedMac"
     */
    char clockID[UUID_STRING_SIZE];

    /**
     * SYNC_LOCKED: 
     * - The synchronization is locked to the @ref clockID and the
     *   absolute offset to the reference is less then @ref refOffsetTicks.
     *   This information is independent of the @ref stratumLevel of the
     *   reference clock. Even when different timer are synchronized
     *   to a clock with a bad stratum level all sync'd timer can offer
     *   a high relative accuracy against each other.
     *   When an external synchronization is lost, the system must
     *   change to a local reference clock and change the @ref clockID
     *   and @ref stratumLevel to the local defaults 
     *   (@ref OCI_TimerCapabilities.localClockID, 
     *    @ref OCI_TimerCapabilities.localStratumLevel).
     *   Nevertheless the timer must maintain a monotonous increasing 
     *   timestamp with the same nominal frequency.
     *
     * POSITIVE_LEAP_SECOND_AT_END_OF_DAY:
     * - The last minute of this day will have 61 seconds. This flag
     *   indicates the insertion of a leap second in the UTC at the
     *   end of the day.
     *
     * NEGATIVE_LEAP_SECOND_AT_END_OF_DAY:
     * - The last minute of this day will have 59 seconds. This flag
     *   indicates a negative leap second in the UTC.
     *
     */
    uint32 flags;

    /**
     * Epoch of the timestamp. Whenever system can not guarantee a monotonous 
     * increment of the time stamp this value must be incremented.
     * In typical cases this will happen when the sync was lost and 
     * re-established or the source of the sync signal has changed. 
     */
    uint32 epoch;

    /**
     * Estimated maximum offset to the time referenced by @ref clockID.
     * This value is an indicator for the quality of the synchronization
     * inside of a cluster, but independent to the @ref stratumLevel of the
     * reference clock for that cluster.
     */
    uint32 refOffsetTicks;

    /** 
     * Stratum is used to describe the quality of a clock used for 
     * synchronization. The ANSI Synchronization Interface Standard 
     * T1.101 defines profiles for clock accuracy at each stratum level, 
     * as does ITU standard G.810, and Telecordia/Bellcore standards GR-253 
     * and GR-1244.
     * Systems with no information about the relation to UTC or TAI must use 
     * stratum level 255.
     *
     * @todo verify and document stratum level.
     */
    uint32 stratumLevel;


    /**
     * The reference scale, which is used for time stamping and for the
     * @ref timeOriginIso8601 reference.
     * 
     */
    OCI_TimeReferenceScale referenceScale;

    /** 
     * For synchronized clocks this string defines the origin of the clock.
     * The binary value 0x0000 0000 0000 0000 is equivalent to this time
     * reference. This member represents a string in the ISO8601:2000 format
     * and references to the @ref referenceScale.
     * The string must use a complete representation in the "Extended Format"
     * (YYYY-MM-DDThh:mm:ss.fffffffff+/-hh:mm). The fractional seconds part
     * may vary in length and may be ommitted entierely, including the dot.
     * An empty string for @ref stratumLevel 255 is allowed.
     *
     */
     char timeOriginIso8601[UUID_STRING_SIZE];

    /**
     * Offset between the TAI system and UTC system valid for the timestamp in
     * @ref currentTime. When flags POSITIVE_LEAP_SECOND_AT_END_OF_DAY or  
     * NEGATIVE_LEAP_SECOND_AT_END_OF_DAY are not set this offset will be constant 
     * for at least 24 hours. When one of the flags is set the value will be incremented 
     * or decremented at the end of the day.
     * systems without relation to TAI / UTC must deliver MIN_SINT32 (-0x8000 0000).
     * Systems with relation to TAI, but no information about the leap seconds may
     * deliver MIN_SINT32, too.
     */
    sint32 tai2utcOffset;

    /**
     * The timestamp of the current time, when this request was served. 
     */
    OCI_Time currentTime;

} OCI_TimerStatus;

/**
 * Flag to indicate the synchronization capabilities to an external reference timer 
 */
#define EXTERNAL_SYNC_SUPPORT (1<<31)

/**
 * Flag to indicate that the timer supports the prorietary synchronization format of
 * ETAS via an Ethernet connection.
 */
#define SYNC_ETAS_ETH         (1<<0)

/**
 * Flag to indicate that the timer supports synchronization to the IEEE1588 (precision
 * time protocol).
 */
#define SYNC_IEEE1588         (1<<1)

/**
 * Flag to indicate that the timer supports synchronization to messages in the 
 * NMEA-0183 ZDA message format 
 */
#define SYNC_GPS_NMEA_ZDA     (1<<2)

/**
 * Flag to indicate that the timer supports synchronization to a one pulse per second
 * (1PPS) singnal of a GPS receiver and the NMEA-0183 ZDA message format. 
 */
#define SYNC_GPS_PPS          (1<<3)

/**
 * Flag to indicate that the timer supports synchronization to IRIG standard
 * 200-2004 code G146. 
 */
#define SYNC_GPS_IRIG_G146    (1<<4)

/** 
 * Static Information about a timer. This information will never change.
 */
typedef struct OCI_TimerCapabilities
{
    /** 
     * Pointer to the UTF8 encoded timer ID of the local system.
     * This string should be unique in the network. ETAS recommends 
     * to use a string representation of an UUID.
     * The system may generate a UUID in the boot phase. In case of 
     * an ethernet based system, which is not used to generate other 
     * UUIDs, the system can simply extend a template by the 
     * MAC-Address to generate a UUID Version 1 (e.g.\ used by DCE V1.1).
     *
     * Example: 
     *  - Timestamp (low,mid,high) = 0, 
     *  - Clock Sequence = 0,
     *  - UUID-Version = 1, DCE variant, 
     *  - MAC-Address of the system = 00-60-34-00-14-00 
     *
     *  ==> uuid = "00000000-0000-1000-8000-006034001400"
     */
    char localClockID[UUID_STRING_SIZE];

    /**
    * Format of the 64-Bit timestamp generated by this timer. Currently
    * only @ref OCI_TIMER_BINARY_64BIT is supported. Future implementation 
    * may support different timestamp formats.
    */
    uint32 format;

    /**
    * Nominal number of clock ticks per second. 
    * i.e.\ the value of OCI_Time (t+1s) - OCI_Time(t) for this clock.
    * Must not be 0.
    */
    uint32 tickFrequency;

    /**
    * Number of clock ticks between two updates. For instance, 
    * a 15.2 Hz clock could be encoded with a @ref tickFrequency of 152
    * and @ref ticksPerIncrement of 10.
    * Must not be 0.
    */
    uint32 ticksPerIncrement;

    /** 
    * Stratum is used to describe the quality of a clock used for 
    * synchronization. The ANSI Synchronization Interface Standard 
    * T1.101 defines profiles for clock accuracy at each stratum level, 
    * as does ITU standard G.810, and Telecordia/Bellcore standards GR-253 
    * and GR-1244.
    * Systems with no information about the relation to UTC or TAI must use 
    * stratum Level 255.
    *
    * @todo verify and document stratum level.
    */
    uint32 localStratumLevel;

    /**
     * The reference scale, which is used for time stamping and for the
     * @ref localTimeOriginIso8601 reference.
     * 
     */
    OCI_TimeReferenceScale localReferenceScale;

    /** 
    * For synchronized clocks this string defines the origin of the clock as
    * a date and time in the Gregorian calendar and International Atomic Time (TAI)
    * or date and time in the Gregorian calendar and Coordinated Universal Time (UTC).
    * The binary value 0x0000 0000 0000 0000 is equivalent to this time
    * reference. This member represents a string in the ISO8601:2000 format.
    * The string must use a complete representation in the "Extended Format"
    * (YYYY-MM-DDThh:mm:ss+/-hh:mm) or an empty string for @ref localStratumLevel 255.
    *
    */
    char localTimeOriginIso8601[UUID_STRING_SIZE];

    /**
     * The timer supports the synchronization to an external reference clock.
     *
     * EXTERNAL_SYNC_SUPPORT:
     * - The clock supports one or more mechnism to synchronize to an external clock reference.
     *
     * SYNC_ETAS_ETH:
     * - The timer can synchronize to the ETAS propritary synchronization via ethernet.
     *
     * SYNC_IEEE1588:
     * - The timer can act as a slave clock according to the IEEE1588 protocol.
     *
     * SYNC_GPS_NMEA_ZDA:
     * - The timer supports synchronization to the NMEA-0183 ZDA message of a GPS-signal.
     *
     * SYNC_GPS_PPS:
     * - The timer supports synchronization to a 1 pulse per second signal of a GPS receiver.
     *
     * SYNC_GPS_IRIG_G146:
     * -The timer can synchronize to a IRIG standard 200-2004 conform G146 signal. 
     */
    uint32 syncSlave; 

    /**
     * The timer can generate a reference for an external timer / system. 
     *
     * EXTERNAL_SYNC_SUPPORT:
     * - The timer supports one or more mechnism to generate a reference signal for an external clock.
     *
     * SYNC_ETAS_ETH:
     * - The timer can generate the ETAS propritary synchronization via ethernet.
     *
     * SYNC_IEEE1588:
     * - The timer can act as a master clock in an IEEE1588 network.
     *
     * SYNC_GPS_NMEA_ZDA:
     * - The timer can generate a NMEA-0183 conform ZDA-message.
     *
     * SYNC_GPS_PPS:
     * - The time can generate a 1 pulse per second signal.
     *
     * SYNC_GPS_IRIG_G146
     * -The timer can generate an IRIG standard 200-2004 conform G146 signal. 
     */
    uint32 syncMaster; 

} OCI_TimerCapabilities;



/** 
  Query the status of the timer that will be used for generating time-stamps. More than one system 
  or controller may share the same timer / clock source. All frames and events will be stamped with
  this timer, when they are stored in the read queue(s).

  The status information of the timer for a specific controller instance may change, when some events occur.

  @controllerState CREATED, RUNNING, SUSPENDED.
  @todo verify supported states!

  @param[in]  controller Handle of the controller to query. 
  @param[out] status     pointer to the @ref OCI_TimerStatus structure that will receive the specification.
                         Must not be NULL.

  @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. The @a status information is filled with valid information.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
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
OCI_CALL OCI_GetTimerStatus(
    OCI_ControllerHandle controller, 
    OCI_TimerStatus *status);
#endif    

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetTimerStatus)(
    OCI_ControllerHandle controller, 
    OCI_TimerStatus *status);


/** 
  Query the capabilities of the local timer that will be used for generating time-stamps. More than one system 
  or controller may share the same timer / clock source.

  The capabilities are static information, which will never change for a specific controller instance.

  @controllerState CREATED, RUNNING, SUSPENDED.
  @todo verify supported states!

  @param[in]  controller    controller to query. 
  @param[out] capabilities  pointer to the @ref OCI_TimerCapabilities structure that will receive the timer
                            capabilities. Must not be @c NULL.

  @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. The @a capabilities are filled with valid information. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a capabilities paramter is NULL or points to an invalid address.</td> 
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
OCI_CALL OCI_GetTimerCapabilities(
    OCI_ControllerHandle controller, 
    OCI_TimerCapabilities *capabilities);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetTimerCapabilities)(
    OCI_ControllerHandle controller, 
    OCI_TimerCapabilities *capabilities);

/** 
  Query the current value of the timer / clock for this controller. The result will have the same precision, 
  resolution and offset as the time stamps used in messages received by the controller. When additional 
  information, like the clockID and stratumLevel is required the function OCI_GetTimerStatus() can be used 
  instead.

  @controllerState CREATED, RUNNING, SUSPENDED.
  @todo verify supported states!

  @param[in]  controller  controller to query. 
  @param[out] time        pointer to the @ref OCI_Time read from the timer used by the controller for time-stamping.

  @return
   <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a time paramter is NULL or references an invalid address.</td> 
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
OCI_CALL OCI_GetTimerValue(
    OCI_ControllerHandle controller, 
    OCI_Time *time);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetTimerValue)(
    OCI_ControllerHandle controller, 
    OCI_Time *time);


/**
 *  codes for event generation. 
 */
typedef enum OCI_TimerEvent
{
    /**
    * Timer event - the timer has locked to an external reference. There
    * may a non monotony in the timestamps.
    */
    OCI_TIMER_EVENT_SYNC_LOCK = 0, 

    /**
    * Timer event - the timer has lost the contact to an external reference.
    */
    OCI_TIMER_EVENT_SYNC_LOSS = 1,

    /**
    * Timer event - A leap second occurs. The offset between TAI and UTC
    * has changed by one second. The change may be positive or negative. 
    */
    OCI_TIMER_EVENT_LEAP_SECOND = 2

} OCI_TimerEvent;

/**
 * Definition of an event filter for timer events
 */
typedef struct OCI_TimerEventFilter
{
    /**
    * Code for the event, that shall generate a event message.
    */
    OCI_TimerEvent eventCode;

    /**
    * Tag that is used for an in-band message when the event fired.
    */
    uint32 tag;

    /** 
    * Destination of the event. When @ref OCI_EVENT_DESTINATION_INBAND 
    * is set the event will be delivered inband to the application.
    * When the @ref OCI_EVENT_DESTINATION_CALLBACK is set the event 
    * will trigger the eventHandler of the rxQueue for that event.
    * When both flags are combined the event is duplicated and the
    * callback is triggered and the event is stored inband in the queue.
    */
    uint32 destination;


} OCI_TimerEventFilter;

/**
* Message that is generated when a timer event fires and a filter
* for that event was set.
*/
typedef struct OCI_TimerEventMessage
{
    /** 
    * Global receive time stamp. Depending on the implementation, 
    * this may be based on a hardware-internal timer with no global 
    * synchronization. In particular, the epoch (i.e.\ value 0) may
    * be the point in time the hardware was switched on.
    */
    OCI_Time timeStamp;

    /**  
    * Application supplied value, that is set by the registration of the 
    * filter condition OCI_AddTimerEventFilter().
    */
    uint32 tag;

    /** code for the event that generated this message */
    OCI_TimerEvent eventCode;
    
    /** 
    * Destination of the event. The event message will use the 
    * combination of @ref OCI_EVENT_DESTINATION_INBAND and 
    * @ref OCI_EVENT_DESTINATION_CALLBACK that was registered for 
    * this event by calling OCI_AddTimerEventFilter(). */
    uint32 destination;

} OCI_TimerEventMessage;


/**
  Add white filter condition(s) to enable the reception of Timer events for a specific receive queue.

  In the default configuration of a receive queue no data will be received because any data is blocked by
  some filter condition. The application must enable the reception of the data by setting white filter 
  (defining pass conditions). 

  This function allows the reception of specific Timer events. The events may be delivered in-band
  in the normal data stream or generate a (separate) callback. The signalization path to the application is
  defined by the creation of the receive queue. (See also OCI_CreateLINRxQueue(), OCI_CreateCANRxQueue(),
  OCI_CreateFlexRayRxQueue()).

  @controllerState RUNNING, SUSPENDED. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.

  @param[in]  rxQueue  Handle of the receive queue that will receive the timer events. 
  @param[in]  filter   Pointer to an array of @a count entries of type @ref OCI_TimerEventFilter.
                       If @a count is 0, @a filter may be NULL.
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
  
  @ingroup GROUP_OCI_EVENT_HANDLING

  @sa GROUP_OCI_EVENT_HANDLING   
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_AddTimerEventFilter(
    OCI_QueueHandle rxQueue, 
    OCI_TimerEventFilter filter[], 
    uint32 count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_AddTimerEventFilter)(
    OCI_QueueHandle rxQueue, 
    OCI_TimerEventFilter filter[], 
    uint32 count);

/**
  Remove white filter condition(s) do disable reception of timer events for a specific receive queue. 

  This function allows to withdraw one or more white filter conditions set by OCI_AddTimerEventFilter(). Each
  @ref OCI_TimerEventFilter of the @a filter array must exact match to a previously added filter. 

  @controllerState RUNNING, SUSPENDED. \n
                   When the controller is in the CREATED state no valid Queue can exist and the call will return
                   @ref OCI_ERR_INVALID_HANDLE.  \n
                   When the controller is in the DISCONNECTED state the function may be called to do local clean
                   up. Only a limitied plausibility check may be done in this case.  

  @param[in]  rxQueue Handle of the receive queue to modify. 
  @param[in]  filter  Pointer to an array of @a count entries of type @ref OCI_TimerEventFilter. 
                      If @a count is 0, @a filters may be NULL.
  @param[in]  count   Number of entries in the @a filters array. 

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a rxQueue handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The @a filter paramter is NULL or references an invalid object.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INCONSISTENT_PARAMETER_SET </td>
        <td> One or more @a filter objects are not set and can not be removed. The processing of the rest of the
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

  @ingroup GROUP_OCI_EVENT_HANDLING
  
  @sa GROUP_OCI_EVENT_HANDLING   
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_RemoveTimerEventFilter(
    OCI_QueueHandle rxQueue, 
    OCI_TimerEventFilter filter[], 
    uint32 count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_RemoveTimerEventFilter)(
    OCI_QueueHandle rxQueue, 
    OCI_TimerEventFilter filter[], 
    uint32 count);

typedef struct OCI_Time_VTable
{
    PF_OCI_GetTimerStatus getTimerStatus;
    PF_OCI_GetTimerCapabilities getTimerCapabilities;
    PF_OCI_GetTimerValue getTimerValue;
    PF_OCI_AddTimerEventFilter addTimerEventFilter;
    PF_OCI_RemoveTimerEventFilter removeTimerEventFilter;
} OCI_Time_VTable;

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
