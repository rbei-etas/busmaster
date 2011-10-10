#ifndef ETAS_OCI_FLEXRAY_H
#define ETAS_OCI_FLEXRAY_H

/**
* @file       ociflx.h
* @brief      FlexRay specific part of the Open Controller Interface (OCI) API.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4788 $
*/

#include "ociflxio.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
* @addtogroup GROUP_OCI_FLEXRAY_CONTROLLER OCI FlexRay
* @brief      FlexRay specific part of the Open Controller Interface (OCI). 
* @{
*/

/**
* @addtogroup GROUP_OCI_FLEXRAY_INITIALIZATION Initialization
* @{
*/

/** Structure containing the FlexRay bus configuration parameters for v1.0 of OCI_FLX. */
typedef struct OCI_FlexRayBusConfiguration 
{
    /** @name Cluster wide protocol relevant parameters: */
    
    /** 
    * Maximum number of times a node in the cluster is permitted to attempt 
    * to start the cluster by initiating schedule synchronization.
    */
    uint32 gColdStartAttempts;
    
    /**
    * Number of macroticks the action point is offset from the beginning of a 
    * static slots or symbol window. 
    */        
    uint32 gdActionPointOffset;
               
    /** Upper limit of the CAS acceptance window. */
    uint32 gdCASRxLowMax;
    
    /** Duration of the idle phase within a dynamic slot. */
    uint32 gdDynamicSlotIdlePhase;
    
    /** Duration of a minislot. */
    uint32 gdMinislot;
    
    /** 
    * Number of macroticks the minislot action point is offset from the 
    * beginning of a minislot.
    */
    uint32 gdMinislotActionPointOffset;
    
    /** Duration of a static slot. */
    uint32 gdStaticSlot;
    
    /** Duration of the symbol window. */
    uint32 gdSymbolWindow;
    
    /** Number of bits in the Transmission Start Sequence. */
    uint32 gdTSSTransmitter;
    
    /** 
    * Number of bits used by the node to test the duration of the 'idle' 
    * portion of a received wakeup symbol.
    */
    uint32 gdWakeupSymbolRxIdle;
    
    /**
    * Number of bits used by the node to test the LOW portion of a received 
    * wakeup symbol.
    */
    uint32 gdWakeupSymbolRxLow;
    
    /** The size of the window used to detect wakeups. */
    uint32 gdWakeupSymbolRxWindow;
    
    /** 
    * Number of bits used by the node to transmit the 'idle' part of a wakeup 
    * symbol.
    */
    uint32 gdWakeupSymbolTxIdle;
    
    /**
    * Number of bits used by the node to transmit the LOW part of a wakeup 
    * symbol.
    */
    uint32 gdWakeupSymbolTxLow;
    
    /** 
    * Upper limit for the start up listen timeout and wake up listen timeout 
    * in the presence of noise.
    */
    uint32 gListenNoise;
    
    /** Number of macroticks in a communication cycle. */
    uint32 gMacroPerCycle;
    
    /** 
    * Defines the number of consecutive even/odd cycle pairs with missing clock 
    * correction terms that will cause the protocol to transition from the 
    * POC:normal active or POC:normal passive state into the POC:haltstate.
    */
    uint32 gMaxWithoutClockCorrectionFatal;

    /** 
    * Defines the number of consecutive even/odd cycle pairs with missing clock 
    * correction terms that will cause the protocol to transition from the 
    * POC:normal active state to the POC:normal passive state. 
    */
    uint32 gMaxWithoutClockCorrectionPassive;
                              
    /** Number of minislots in the dynamic segment. */                     
    uint32 gNumberOfMinislots;
    
    /** Number of static slots in the static segment. */
    uint32 gNumberOfStaticSlots;
    
    /**
    * Start of the offset correction phase within the NIT, expressed as the 
    * number of macroticks from the start of cycle. 
    */
    uint32 gOffsetCorrectionStart;

    /** Payload length of a static frame. */
    uint32 gPayloadLengthStatic;
    
    /**
    * Maximum number of nodes that may send frames with the sync frame 
    * indicator bit set to one. 
    */
    uint32 gSyncNodeMax;
    
    /** @name Local protocol relevant parameters: */
    
    /** 
    * Boolean flag that controls the transition to the POC:halt state due to a 
    * clock synchronization errors. 
    */
    uint32 pAllowHaltDueToClock;
    
    /** 
    * Number of consecutive even/odd cycle pairs that must have valid clock 
    * correction terms before the CC will be allowed to transition from the 
    * POC:normal passive state to POC:normal active state.
    */
    uint32 pAllowPassiveToActive;

    /** 
    * Channels to which the node is connected (A, B or both). 
    * @values  Valid values are @ref OCI_FLEXRAY_CHANNEL_A and @ref OCI_FLEXRAY_CHANNEL_B.
    * @remark  Both values can be bitwise OR-connected to connect the node to both channels.
	*/
    uint32 pChannels;
    
    /**
    * Expanded range of measured clock deviation allowed for startup frames 
    * during integration.
    */
    uint32 pdAcceptedStartupRange;
    
    /** Local cluster drift damping factor used for rate correction. */
    uint32 pClusterDriftDamping;
    
    /**
    * Value used by the receiver to calculate the difference between primary 
    * time reference point and secondary time reference point.
    */
    uint32 pDecodingCorrection;

    /** 
    * Value used to compensate for reception delays on the indicated channel.
    */
    uint32 pDelayCompensationA;
         
    /** 
    * Value used to compensate for reception delays on the indicated channel.
    */
    uint32 pDelayCompensationB;
    
    /** 
    * Upper limit for the start up listen timeout and wake up listen timeout.
    */
    uint32 pdListenTimeout;
    
    /**
    * Maximum drift offset between two nodes that operate with unsynchronized 
    * clocks over one communication cycle.
    */
    uint32 pdMaxDrift;
    
    /**
    * Number of microticks added or subtracted to the NIT to carry out a 
    * host-requested external offset correction.
    */
    uint32 pExternOffsetCorrection;
    
    /**
    * Number of microticks added or subtracted to the cycle to carry out a 
    * host-requested external rate correction.
    */
    uint32 pExternRateCorrection;
    
    /**
    * ID of the slot used to transmit the startup frame, sync frame, or 
    * designated single slot frame.
    */
    uint32 pKeySlotId;
    
    /** 
    * Flag indicating whether the Key Slot is used to transmit a startup frame.
    */
    uint32 pKeySlotUsedForStartup;
    
    /**
    * Flag indicating whether the Key Slot is used to transmit a sync frame.
    */
    uint32 pKeySlotUsedForSync;
    
    /**
    * Number of the last minislot in which a frame transmission can start in 
    * the dynamic segment.
    */
    uint32 pLatestTx;
    
    /**
    * Integer number of macroticks between the static slot boundary and the 
    * following macrotick boundary of the secondary time reference point based 
    * on the nominal macrotick duration.
    */
    uint32 pMacroInitialOffsetA;

    /**
    * Integer number of macroticks between the static slot boundary and the 
    * following macrotick boundary of the secondary time reference point based 
    * on the nominal macrotick duration.
    */
    uint32 pMacroInitialOffsetB;

    /**
    * Number of microticks between the closest macrotick boundary described by 
    * @ref pMacroInitialOffsetA and the secondary time reference point. 
    */
    uint32 pMicroInitialOffsetA;

    /**
    * Number of microticks between the closest macrotick boundary described by 
    * @ref pMacroInitialOffsetB and the secondary time reference point. 
    */
    uint32 pMicroInitialOffsetB;

    /** 
    * Nominal number of microticks in the communication cycle of the local node.
    */
    uint32 pMicroPerCycle;
    
    /** Magnitude of the maximum permissible offset correction value. */
    uint32 pOffsetCorrectionOut;
    
    /** Magnitude of the maximum permissible rate correction value. */
    uint32 pRateCorrectionOut;
    
    /** 
    * Flag indicating whether or not the node shall enter single slot mode 
    * following startup.
    */
    uint32 pSingleSlotEnabled;
    
    /** 
     * Channel used by the node to send a wakeup pattern (A or B). 
     * @values  Valid values are @ref OCI_FLEXRAY_CHANNEL_A or @ref OCI_FLEXRAY_CHANNEL_B.
     */
    uint32 pWakeupChannel;
    
    /** 
    * Number of repetitions of the wakeup symbol that are combined to form 
    * a wakeup pattern when the node enters the POC:wakeup send state.
    */
    uint32 pWakeupPattern;
    

    /** @name Cluster wide protocol related parameters: */
    
    /** Length of the Network Management vector in a cluster. */
    uint32 gNetworkManagementVectorLength;
    
} OCI_FlexRayBusConfiguration;


/** Structure containing the FlexRay bus configuration parameters for v1.1 of OCI_FLX. This is identical to
 * OCI_FlexRayBusConfiguration, except for the additional field pSecondKeySlotId. */
typedef struct OCI_FlexRayBusConfigurationEx
{
    /** @name Cluster wide protocol relevant parameters: */
    
    /** 
    * Maximum number of times a node in the cluster is permitted to attempt 
    * to start the cluster by initiating schedule synchronization.
    */
    uint32 gColdStartAttempts;
    
    /**
    * Number of macroticks the action point is offset from the beginning of a 
    * static slots or symbol window. 
    */        
    uint32 gdActionPointOffset;
               
    /** Upper limit of the CAS acceptance window. */
    uint32 gdCASRxLowMax;
    
    /** Duration of the idle phase within a dynamic slot. */
    uint32 gdDynamicSlotIdlePhase;
    
    /** Duration of a minislot. */
    uint32 gdMinislot;
    
    /** 
    * Number of macroticks the minislot action point is offset from the 
    * beginning of a minislot.
    */
    uint32 gdMinislotActionPointOffset;
    
    /** Duration of a static slot. */
    uint32 gdStaticSlot;
    
    /** Duration of the symbol window. */
    uint32 gdSymbolWindow;
    
    /** Number of bits in the Transmission Start Sequence. */
    uint32 gdTSSTransmitter;
    
    /** 
    * Number of bits used by the node to test the duration of the 'idle' 
    * portion of a received wakeup symbol.
    */
    uint32 gdWakeupSymbolRxIdle;
    
    /**
    * Number of bits used by the node to test the LOW portion of a received 
    * wakeup symbol.
    */
    uint32 gdWakeupSymbolRxLow;
    
    /** The size of the window used to detect wakeups. */
    uint32 gdWakeupSymbolRxWindow;
    
    /** 
    * Number of bits used by the node to transmit the 'idle' part of a wakeup 
    * symbol.
    */
    uint32 gdWakeupSymbolTxIdle;
    
    /**
    * Number of bits used by the node to transmit the LOW part of a wakeup 
    * symbol.
    */
    uint32 gdWakeupSymbolTxLow;
    
    /** 
    * Upper limit for the start up listen timeout and wake up listen timeout 
    * in the presence of noise.
    */
    uint32 gListenNoise;
    
    /** Number of macroticks in a communication cycle. */
    uint32 gMacroPerCycle;
    
    /** 
    * Defines the number of consecutive even/odd cycle pairs with missing clock 
    * correction terms that will cause the protocol to transition from the 
    * POC:normal active or POC:normal passive state into the POC:haltstate.
    */
    uint32 gMaxWithoutClockCorrectionFatal;

    /** 
    * Defines the number of consecutive even/odd cycle pairs with missing clock 
    * correction terms that will cause the protocol to transition from the 
    * POC:normal active state to the POC:normal passive state. 
    */
    uint32 gMaxWithoutClockCorrectionPassive;
                              
    /** Number of minislots in the dynamic segment. */                     
    uint32 gNumberOfMinislots;
    
    /** Number of static slots in the static segment. */
    uint32 gNumberOfStaticSlots;
    
    /**
    * Start of the offset correction phase within the NIT, expressed as the 
    * number of macroticks from the start of cycle. 
    */
    uint32 gOffsetCorrectionStart;

    /** Payload length of a static frame. */
    uint32 gPayloadLengthStatic;
    
    /**
    * Maximum number of nodes that may send frames with the sync frame 
    * indicator bit set to one. 
    */
    uint32 gSyncNodeMax;
    
    /** @name Local protocol relevant parameters: */
    
    /** 
    * Boolean flag that controls the transition to the POC:halt state due to a 
    * clock synchronization errors. 
    */
    uint32 pAllowHaltDueToClock;
    
    /** 
    * Number of consecutive even/odd cycle pairs that must have valid clock 
    * correction terms before the CC will be allowed to transition from the 
    * POC:normal passive state to POC:normal active state.
    */
    uint32 pAllowPassiveToActive;

    /** 
    * Channels to which the node is connected (A, B or both). 
    * @values  Valid values are @ref OCI_FLEXRAY_CHANNEL_A and @ref OCI_FLEXRAY_CHANNEL_B.
    * @remark  Both values can be bitwise OR-connected to connect the node to both channels.
	*/
    uint32 pChannels;
    
    /**
    * Expanded range of measured clock deviation allowed for startup frames 
    * during integration.
    */
    uint32 pdAcceptedStartupRange;
    
    /** Local cluster drift damping factor used for rate correction. */
    uint32 pClusterDriftDamping;
    
    /**
    * Value used by the receiver to calculate the difference between primary 
    * time reference point and secondary time reference point.
    */
    uint32 pDecodingCorrection;

    /** 
    * Value used to compensate for reception delays on the indicated channel.
    */
    uint32 pDelayCompensationA;
         
    /** 
    * Value used to compensate for reception delays on the indicated channel.
    */
    uint32 pDelayCompensationB;
    
    /** 
    * Upper limit for the start up listen timeout and wake up listen timeout.
    */
    uint32 pdListenTimeout;
    
    /**
    * Maximum drift offset between two nodes that operate with unsynchronized 
    * clocks over one communication cycle.
    */
    uint32 pdMaxDrift;
    
    /**
    * Number of microticks added or subtracted to the NIT to carry out a 
    * host-requested external offset correction.
    */
    uint32 pExternOffsetCorrection;
    
    /**
    * Number of microticks added or subtracted to the cycle to carry out a 
    * host-requested external rate correction.
    */
    uint32 pExternRateCorrection;
    
    /**
    * ID of the slot used to transmit the startup frame, sync frame, or 
    * designated single slot frame.
    */
    uint32 pKeySlotId;
    
    /**
    * ID of the slot used as a second startup slot, to enable the bus to be started without the presence
    * of a second Flexray controller. The value in this field is only valid if the busMode field of 
    * OCI_FlexRayAdvancedConfiguration includes the flag OCI_FLEXRAY_BUSMODE_TWOKEYSLOTS.
    */
    uint32 pSecondKeySlotId;
    
    /** 
    * Flag indicating whether the Key Slot is used to transmit a startup frame.
    */
    uint32 pKeySlotUsedForStartup;
    
    /**
    * Flag indicating whether the Key Slot is used to transmit a sync frame.
    */
    uint32 pKeySlotUsedForSync;
    
    /**
    * Number of the last minislot in which a frame transmission can start in 
    * the dynamic segment.
    */
    uint32 pLatestTx;
    
    /**
    * Integer number of macroticks between the static slot boundary and the 
    * following macrotick boundary of the secondary time reference point based 
    * on the nominal macrotick duration.
    */
    uint32 pMacroInitialOffsetA;

    /**
    * Integer number of macroticks between the static slot boundary and the 
    * following macrotick boundary of the secondary time reference point based 
    * on the nominal macrotick duration.
    */
    uint32 pMacroInitialOffsetB;

    /**
    * Number of microticks between the closest macrotick boundary described by 
    * @ref pMacroInitialOffsetA and the secondary time reference point. 
    */
    uint32 pMicroInitialOffsetA;

    /**
    * Number of microticks between the closest macrotick boundary described by 
    * @ref pMacroInitialOffsetB and the secondary time reference point. 
    */
    uint32 pMicroInitialOffsetB;

    /** 
    * Nominal number of microticks in the communication cycle of the local node.
    */
    uint32 pMicroPerCycle;
    
    /** Magnitude of the maximum permissible offset correction value. */
    uint32 pOffsetCorrectionOut;
    
    /** Magnitude of the maximum permissible rate correction value. */
    uint32 pRateCorrectionOut;
    
    /** 
    * Flag indicating whether or not the node shall enter single slot mode 
    * following startup.
    */
    uint32 pSingleSlotEnabled;
    
    /** 
     * Channel used by the node to send a wakeup pattern (A or B). 
     * @values  Valid values are @ref OCI_FLEXRAY_CHANNEL_A or @ref OCI_FLEXRAY_CHANNEL_B.
     */
    uint32 pWakeupChannel;
    
    /** 
    * Number of repetitions of the wakeup symbol that are combined to form 
    * a wakeup pattern when the node enters the POC:wakeup send state.
    */
    uint32 pWakeupPattern;
    

    /** @name Cluster wide protocol related parameters: */
    
    /** Length of the Network Management vector in a cluster. */
    uint32 gNetworkManagementVectorLength;
    
} OCI_FlexRayBusConfigurationEx;


/** 
* Structure containing the FlexRay flow control directives for the 
* communication controller.
*/

typedef struct OCI_FlexRayFlowConfiguration 
{
    /** Enables WUP generation and waiting for WUPs on node start. */
    uint32 wakeupEnabled;
    
    /** 
    * Enables waiting for WUP on channel A before actively participating 
    * after node start.
    */
    uint32 waitForWUPA;
    
    /**
    * Enables waiting for WUP on channel B before actively participating after 
    * node start.
    */
    uint32 waitForWUPB;
    
    /** Inhibits cold-starting mechanisms (non-leading). */
    uint32 coldstartInhibit;
    
    /** Enables infinite cold start attempts - only for debugging purposes. */
    uint32 repeatColdstart;
    
    /** 
    * Makes the FlexRay node resume after reaching the dead-end state "Halt".
    */
    uint32 resumeAfterHalt;
    
} OCI_FlexRayFlowConfiguration;


/** Structure containing the FlexRay receive FIFO configuration. */
typedef struct OCI_FlexRayRxFifoConfiguration 
{
    /** 
    * Payload length reserved for each FIFO entry. Should equal either 
    * gPayloadLengthStatic or largest pPayloadLengthDynMax in cluster, 
    * whichever is greater. 
    */
    uint32 payloadLength;

    /** Determines frame ID, which is not enqueued into the FIFO. */                     
    uint32 rejectFrameIDValue;
    
    /** 
    * Extends number of rejected frames by ignoring matching of given bit 
    * positions.
    */
    uint32 rejectFrameIDMask;
    
    /** 
     * Reception channel (A, B or both). 
     * @values Valid values are @ref OCI_FLEXRAY_CHANNEL_A and @ref OCI_FLEXRAY_CHANNEL_B. 
     * @remark Both values can be bitwise OR-connected to enable reception on both channels.
     */
    uint32 channel;
    
    /** 
    * Reception periodicity. Has to be a power of two. 
    * @values   The following values are valid:
    * @n        1
    * @n        2
    * @n        4
    * @n        8
    * @n        16
    * @n        32
    * @n        64
    */
    uint32 cyclePeriod;
    
    /** Starting offset for periodicity. Valid values are 0..CyclePeriod-1. */
    uint32 cycleOffset;
    
    /** Activates complete rejection of static segment. */
    uint32 rejectStaticSegment;
    
    /** Activates rejection of all null frames. */
    uint32 rejectNullFrames;
    
} OCI_FlexRayRxFifoConfiguration;

/** Supported special modes (like loopback). */
typedef enum OCI_FlexRayControllerBusMode
{
    /** 
    * The FlexRay controller does not receive any data, that is sent by itself. 
    * This is the normal operation mode of any FlexRay communication controller. 
    */
    OCI_FLEXRAY_BUSMODE_NORMAL = 1,

    /** 
    * Every message sent by this controller will be received by every client of 
    * the controller, when a filter condition matches. 
    */
    OCI_FLEXRAY_BUSMODE_SELFRECEPTION = 2,

    /** 
    * The controller does not access the bus system, but every frame is handled 
    * in the local system. 
    */
    OCI_FLEXRAY_BUSMODE_ISOLATEDLOOPBACK = 4,

    /** 
    * The controller has two key slots and so it is meaningful for the client to get/set the field pSecondKeySlotId
    * in OCI_FlexRayBusConfigurationEx.
    */
    OCI_FLEXRAY_BUSMODE_TWOKEYSLOTS = 8

} OCI_FlexRayControllerBusMode;


/** Advanced controller configuration. */
typedef struct OCI_FlexRayAdvancedConfiguration
{
    /** The client requests exclusive access to the controller. */
    uint32 exclusiveAccess;
    
    /**
    * Flags for all bus modes supported by the controller. Any combination of values from @ref OCI_FlexRayControllerBusMode
    * is allowed, but the behaviour is unspecified if both OCI_FLEXRAY_BUSMODE_NORMAL and OCI_FLEXRAY_BUSMODE_SELFRECEPTION are
    * specified simultaneously.
    */
    uint32 busMode;

} OCI_FlexRayAdvancedConfiguration;


/** Structure containing the configuration for generic FlexRay controllers for OCI_FLX v1.0. */
typedef struct OCI_FlexRayConfiguration
{
    /** General bus configuration. */
    OCI_FlexRayBusConfiguration bus;

    /** Flow control. */
    OCI_FlexRayFlowConfiguration flow;

    /** Receive queue configuration. */
    OCI_FlexRayRxFifoConfiguration rxFifo;

    /** Advanced settings. */
    OCI_FlexRayAdvancedConfiguration advanced;

    /** Number of valid buffer configurations in the following array. */
    uint32 validBufferConfigurationCount;

    /** Transmit buffer configurations.*/
    OCI_FlexRayTxBufferConfiguration txBuffer[OCI_FLEXRAY_MAX_BUFFERCOUNT];

} OCI_FlexRayConfiguration;


/** Structure containing the configuration for generic FlexRay controllers for OCI_FLX v1.1. This differs from
 * OCI_FlexRayConfiguration only in the use of OCI_FlexRayBusConfigurationEx instead of OCI_FlexRayBusConfiguration. */
typedef struct OCI_FlexRayConfigurationEx
{
    /** General bus configuration. */
    OCI_FlexRayBusConfigurationEx bus;

    /** Flow control. */
    OCI_FlexRayFlowConfiguration flow;

    /** Receive queue configuration. */
    OCI_FlexRayRxFifoConfiguration rxFifo;

    /** Advanced settings. */
    OCI_FlexRayAdvancedConfiguration advanced;

    /** Number of valid buffer configurations in the following array. */
    uint32 validBufferConfigurationCount;

    /** Transmit buffer configurations.*/
    OCI_FlexRayTxBufferConfiguration txBuffer[OCI_FLEXRAY_MAX_BUFFERCOUNT];

} OCI_FlexRayConfigurationEx;


/**
* Non static properties of the FlexRay controller. The members of this structure
* may change their values during run-time. They can be modified by different 
* functions.
*/
typedef struct OCI_FlexRayControllerProperties
{
    /** 
    * Specifies if the controller interface instance is suspended or running. 
    * @values      The following values are valid:
    * @n           @ref OCI_CONTROLLER_MODE_RUNNING
    * @n           @ref OCI_CONTROLLER_MODE_SUSPENDED 
    * @sa          @ref OCI_ControllerPropertiesMode
    */
    OCI_ControllerPropertiesMode mode;

} OCI_FlexRayControllerProperties;


/**
  Create a new FlexRay controller instance and bind it to the hardware identified by an URI location. 
  
  This is the API used by the application to bind a specific controller. The application can use information 
  delivered by the CSI component to search for accessible controller. The CSI component will transform the 
  (user friendly) URI-Information from this call into a (system friendly) protocol stack. Any driver needs only 
  to implement the OCI_CreateFlexRayControllerBind()-API call. 

  @controllerState This function creates a new controller instance and initializes it to the CREATED state.

  @param[in]  uriLocation   The URI compatible name which identifies the controller. 

  @param[out] controller    The handle to the created FlexRay controller interface instance.
                            This handle must beused in any subsequent API call to access the controller.

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. The @a controller handle is valid and can be used in further calls
             to the API. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_HW_NOT_PRESENT </td> 
        <td> The operation failed because the controller could not be found. Probably the hardware is not
             connected. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td> 
        <td> The application has already bound the controller at this location. A second binding is not possible.
             Use the existing handle to access the controller. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_NO_RESOURCES </td> 
        <td> The driver or CSI component run into a resource problem and can not instantiate a new handle. Destroy
             some unneeded handles to release resources and try again. </td> 
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
   <tr> <td> @ref OCI_ERR_WRONG_BUS_TYPE </td> 
        <td> The driver tries to connect to a different interface type (LIN, FlexRay, ..) of the same hardware.
             This is an implementation error, because the binding process must detect different Interface-UUIDs
             and refuse a binding of the wrong interface. </td> 
   </tr>
  </TABLE>

  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling"
  @n          @ref PAGE_BOA_ADDRESSINGSCHEME  "Addressing scheme of the Basic OpenAPI"

*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_CreateFlexRayController(
    const char* uriLocation, 
    OCI_ControllerHandle* controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_CreateFlexRayController)(
    const char* uriLocation, 
    OCI_ControllerHandle* controller);


/**
  Create a new CAN controller instance and bind it to a hardware identified by a protocol stack. 
  
  This is the API implemented by the driver to bind a specific controller. The CSI component will transform 
  the (user friendly) URI-Information from the CreateCANController() call into this (system friendly) 
  protocol stack. 
 
  @controllerState This function creates a new controller instance and initializes it to the CREATED state.

  @param[in]  protocolStack  The access parameters required to bind to the appropriate controller instance.

  @param[out] controller     The handle to the created controller interface instance.
                             This handle must be used in any subsequent API call to access the controller.

  @return     
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3"> 
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. The controller handle is valid and can be used in further calls
             to the API. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_HW_NOT_PRESENT </td> 
        <td> The operation failed because the controller could not be found. Probably the hardware is not
             connected. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td> 
        <td> The application has already bound the controller at this location. A second binding is not possible.
             Use the existing handle to access the controller. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_NO_RESOURCES </td> 
        <td> The driver or CSI component run into a resource problem and can not instantiate a new handle. Destroy
             some unneeded handles to release resources and try again. </td> 
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
   <tr> <td> @ref OCI_ERR_PROTOCOL_VERSION_NOT_SUPPORTED </td> 
        <td> The binding of the protocol stack failed, because a version check of the requested API or some
             intermediate API in the protocol stack detected some incompatibilities. This is a problem of the
             installed software. A different driver version or a generic version adapter is required to access 
             the controller. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_ACCESS_SYNTAX </td> 
        <td> the parsing of the access parameter failed. The protocol driver or some intermediate driver can not 
             interpret the parameter of the protcolStack. This error should not occur, when the protocol stack was
             build by the CSI component and the CSI-services in the protocol driver are implemented in a correct
             way. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_ACCESS_PARAM </td> 
        <td> The driver or an intermediate layer could not be found. An element of the protocol stack can not be
             resolved. The connection can not be established. This error should not occur, when the protocol stack 
             was build by the CSI component and the CSI-services in the protocol driver are implemented in a 
             correct way. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_TRANSFER_SYNTAX </td> 
        <td> The driver can not access the requested interface. There is a server offering the interface with the
             requested UUID, but the client and server offer only incompatible binding mechanism. This may be a
             software installation problem and some intermediate driver, that maps different binding mechanism may
             be missing.
             This error should not occur, when the protocol stack was build by the CSI component and the 
             CSI-services in the protocol driver are implemented in a correct way. </td> 
   </tr>
  </TABLE>

  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling"

*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_CreateFlexRayControllerBind(
    const BOA_ProtocolStack* protocolStack,
    OCI_ControllerHandle* controller);
#endif 

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_CreateFlexRayControllerBind)(
    const BOA_ProtocolStack* protocolStack,
    OCI_ControllerHandle* controller);


/**
  Destroy an interface instance of a FlexRay controller. 
  
  This function destroys the access to a FlexRay controller instance. It implicitly releases all resources 
  (queues, filter). When the controller is in the @a RUNNING or @a SUSPENDED mode the Controller configuration 
  is unlocked (see also OCI_CloseFlexRayController(). 
  After this call the handle @a controller is invalid and must not be used any more.

  @controllerState CREATED, RUNNING, SUSPENDED, DISCONNECTED. \n
                   The controller instance is destroyed and has no state after successful execution of this call.

  @param[in]  controller  Handle of the FlexRay controller interface instance to destroy.
 
  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. The @a controller handle must not be used any more. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE> 

  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCI_DestroyFlexRayController(
     OCI_ControllerHandle controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_DestroyFlexRayController)(
    OCI_ControllerHandle controller);


/** 
  Open and configure (activate / begin use of) a FlexRay controller interface instance. 
  
  The configuration provided by this call must either be compatible with the setup currently used by the
  physical controller or this instance must be the first logical controller instance that opens the physical
  controller.

  The configuration can only be set once with this call while the controller is in the @a CREATED state. The 
  configuration is used and locked until OCI_CloseFlexRayController() (or OCI_DestroyFlexRayController()) for this 
  controller instance is called.

  The controller may be opened in the @a SUSPENDED mode or @a RUNNING mode. In the @a SUSPENDED mode the 
  controller will not take part in the bus communication. It is possible to configure filter and queues with 
  multiple subsequent API calls. Switching the controller to the @a RUNNING mode by calling 
  OCI_SetFlexRayControllerProperties() will activate all settings at the same time when the controller is activated. 
  In the @a RUNNING mode the controller will immideatly take part in the bus communication. Subsequent API calls 
  will activate queues and filter step by step.

  @controllerState CREATED. \n
                   A transition to the RUNNING or SUSPENDED state will be executed 
                   (depends on the parameter @a properties).

  @param[in]  controller     Handle of the FlexRay controller instance that will be opened. 

  @param[in]  configuration  Pointer to the buffer that contains the configuration for the controller. 

  @param[in]  properties     Pointer to a buffer that contains the properties for the controller.  

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td>
        <td> The controller is not in the @a CREATED state. The controller is already opened and the configuration
             can not be changed without closing it before. </td> 
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
   <tr> <td> @ref OCI_ERR_UNEXPECTED_NULL </td>
        <td> An unexpected implementation error occured. </td> 
   </tr>
  </TABLE> 

  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_OpenFlexRayController(
    OCI_ControllerHandle controller, 
    OCI_FlexRayConfiguration* configuration,
    OCI_FlexRayControllerProperties* properties);
#endif    

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_OpenFlexRayController)(
    OCI_ControllerHandle controller, 
    OCI_FlexRayConfiguration* configuration,
    OCI_FlexRayControllerProperties* properties);

/**
  This method is identical to OCI_OpenFlexRayController, except that it accepts an instance of OCI_FlexRayConfigurationEx
  (instead of OCI_FlexRayConfiguration) as an argument.
 */
#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_OpenFlexRayControllerEx(
    OCI_ControllerHandle controller, 
    OCI_FlexRayConfigurationEx* configuration,
    OCI_FlexRayControllerProperties* properties);
#endif    

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_OpenFlexRayControllerEx)(
    OCI_ControllerHandle controller, 
    OCI_FlexRayConfigurationEx* configuration,
    OCI_FlexRayControllerProperties* properties);


/**
  Close and unlock the configuration of a FlexRay controller instance. 
  
  All queues and filters for this instance are implicitly destroyed with this call. It is undefined if data 
  in transmit queues is flushed or discarded. When this call returns, all resources associated with this instance 
  have been cleaned up and the instance stopped transmitting data on the bus. 

  The state of the controller instance is moved to the @a CREATED state (see @ref GROUP_OCI_CONTROLLER). In a 
  hardware that supports multiple instances on a single physical interface the configuration may be changed by 
  a different instance by calling OCI_OpenFlexRayController() in this state.

  @controllerState RUNNING, SUSPENDED. \n
                   A transition to the CREATED state is executed. \n
                   When the controller is in the DISCONNECTED state the function may be called to do local clean
                   up. Only a limitied plausibility check may be done in this case.  

  @param[in]  controller    Handle of the Controller instance that will be closed.
  
  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td>
        <td> The @a controller was not opened.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_OUT_OF_MEMORY </td>
        <td> The driver run out of memory. This error should only occur on heavy loaded systems with multiple 
             clients and large send and receive queues. The error may indicate a resource leak in the driver 
             implementation. When multiple application access the same resource (interface hardware or physical
             controller) the termination of a different application may return the resources required for this 
             instance. </td> 
   </tr>
  </TABLE> 

  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_CloseFlexRayController(
    OCI_ControllerHandle controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_CloseFlexRayController)(
    OCI_ControllerHandle controller);


/** 
  Query the configuration of the FlexRay controller. 
  
  This call can be executed in any state of the controller instance. Hardware that supports more than one
  logical controller instance on a single physical interface delivers the configuration of the physical
  controller. When the controller instance is in the @a RUNNING or @a SUSPENDED mode, the data will be 
  compatible to the confguration set by OCI_OpenFlexRayController(). When this instance is in the @a CREATED state
  and a different controller instance has already configured the hardware (via OCI_OpenFlexRayController()) the
  configuration used by that instance is returned. When no logical instance accesses the physical controller 
  the call fails and delivers @ref OCI_ERR_NO_CONFIG.

  This call can be used to attach a second application to a single physical controller. 

  @controllerState CREATED, RUNNING, SUSPENDED.

  @param[in]   controller      Handle of the FlexRay controller instance that is queried. 
  
  @param[out]  configuration   Pointer to the buffer that will receive the configuration structure.
                               Must not be @c NULL.

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The configuration paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_NO_CONFIG </td>  
        <td> There is currently no instance accessing the FlexRay Controller and the controller uses no vaild
             configuration. </td> 
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
   <tr> <td> @ref OCI_ERR_UNEXPECTED_NULL </td>
        <td> An unexpected implementation error occured. </td> 
   </tr>
  </TABLE> 
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_GetFlexRayConfiguration(
    OCI_ControllerHandle controller, 
    OCI_FlexRayConfiguration* configuration);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetFlexRayConfiguration)(
    OCI_ControllerHandle controller, 
    OCI_FlexRayConfiguration* configuration);

/**
  This method is identical to OCI_GetFlexRayConfiguration, except that it accepts an instance of OCI_FlexRayConfigurationEx
  (instead of OCI_FlexRayConfiguration) as an argument.
 */
#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_GetFlexRayConfigurationEx(
    OCI_ControllerHandle controller, 
    OCI_FlexRayConfigurationEx* configuration);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetFlexRayConfigurationEx)(
    OCI_ControllerHandle controller, 
    OCI_FlexRayConfigurationEx* configuration);


/**
  Query the properties of the FlexRay controller instance. 
  
  Properties are read/writeable and can be changed using OCI_SetFlexRayControllerProperties(). In the @a SUSPENDED
  state the controller instance will neither send any data on the interface nor receive any data. In the @a RUNNING
  state the controller instance will send data, when the transmit queue is not empty and receive any data
  that passes a configured filter condition.

  In a hardware that supports multiple instances on a single physical interface the state of the physical 
  controller will be @a RUNNING until the last logical instance suspends the logical state. 

  @controllerState CREATED, RUNNING, SUSPENDED.

  @param[in]  controller  Handle of the FlexRay controller instance that is queried.

  @param[out] properties  Pointer to a buffer that will receive the controller interface instance properties. 
                          Must not be @c NULL.
  
  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The @a controller handle is not valid. </td> 
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
OCI_CALL OCI_GetFlexRayControllerProperties(
    OCI_ControllerHandle controller, 
    OCI_FlexRayControllerProperties* properties);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_GetFlexRayControllerProperties)(
    OCI_ControllerHandle controller, 
    OCI_FlexRayControllerProperties* properties);


/**
  Set the FlexRay controller instance properties (suspended / running) of the FlexRay controller instance. 
  
  Properties are read/writeable and can be queried OCI_GetFlexRayControllerProperties().

  These properties are the locigal state of this controller instance. 
  @li In the @a SUSPENDED state the controller instance will neither send any data on the interface nor receive 
      any data. 
  @li In the @a RUNNING state the controller instance will send data, when the transmit queue is not empty and 
      receive any data that passes a configured filter condition.
  
  In a hardware that supports multiple instances on a single physical interface the state of the physical 
  controller will be @a RUNNING until the last instance suspends the logical state. 

  The suspended state is useful to configure a set of filter and activate the complete new set at the
  same time. When the filter are set by (especially a sequence of different) API-calls intermediate states
  exist where some filter already active and other not. This may be critical for specific applications.

  The @a properties parameter describes the controller state after successful execution of the command. When 
  the controller is already in the desired state the driver will ignore the command and return @ref OCI_SUCCESS.

  @controllerState RUNNING, SUSPENDED. \n
                   A transition to the RUNNING or SUSPENDED state may be executed 
                   (depends on the parameter @a properties).

  @param[in]  controller  Handle of the FlexRay controller interface instance for which the properties are set.
  
  @param[in]  properties  FlexRay controller interface instance properties to set. 
                          Must not be @c NULL.

  @return
  <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">  
   <tr> <td> @ref OCI_SUCCESS </td> 
        <td> The operation was successful. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_HANDLE </td>  
        <td> The handle is not valid. </td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_PARAMETER </td>
        <td> The properties paramter is NULL or references an invalid address.</td> 
   </tr>
   <tr> <td> @ref OCI_ERR_INVALID_STATE </td>
        <td> The controller is not in the @a RUNNING or @a SUSPENDED state. When the controller is in the
             @a CREATED state additional configuration information is required and OCI_OpenFlexRayController() must
             be used. </td> 
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

  @sa         @ref GROUP_OCI_CONTROLLER "Controller Handling" 
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCI_SetFlexRayControllerProperties(
    OCI_ControllerHandle controller, 
    OCI_FlexRayControllerProperties* properties);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCI_SetFlexRayControllerProperties)(
    OCI_ControllerHandle controller, 
    OCI_FlexRayControllerProperties* properties);

/** @} */

/**
* @addtogroup GROUP_OCI_FLEXRAY_INTERNAL Internal Structures
* @brief      Additional definitions relevant for OCI implementors. 
* @{
*/

/**
* Internal identifier for the OCI FlexRay interface.
*/

#define UUID_OCIFLX {0xDD,0xD9,0xEB,0xF4,0x94,0x67,0x4b,0x81,0xA3,0x4B,0xF7,0xDF,0xAE,0x3C,0x65,0x66}


/**
* Internal v-table for the OCI FlexRay interface.
*/

typedef struct OCI_FLEXRAY_VTable
{
    /* ocitime.h */

    OCI_Time_VTable timeVTable;

    /* ocierror.h */

    OCI_Error_VTable errorVTable;

    /* ociqueue.h */

    OCI_Queue_VTable queueVTable;

    /* ociflxio.h */

	OCI_FLEXRAYIO_VTable flexRayIOVTable;

    /* ociflx.h */

    PF_OCI_CreateFlexRayController     createFlexRayController;
    PF_OCI_CreateFlexRayControllerBind createFlexRayControllerBind;
    PF_OCI_DestroyFlexRayController    destroyFlexRayController;

    PF_OCI_OpenFlexRayController   openFlexRayController;
    PF_OCI_CloseFlexRayController  closeFlexRayController;
    PF_OCI_GetFlexRayConfiguration getFlexRayConfiguration;

    PF_OCI_GetFlexRayControllerProperties getFlexRayControllerProperties;
    PF_OCI_SetFlexRayControllerProperties setFlexRayControllerProperties;

    PF_OCI_OpenFlexRayControllerEx   openFlexRayControllerEx;       /* This method was added for OCI_FLX v1.1 */
    PF_OCI_GetFlexRayConfigurationEx getFlexRayConfigurationEx;     /* This method was added for OCI_FLX v1.1 */

} OCI_FLEXRAY_VTable;

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
