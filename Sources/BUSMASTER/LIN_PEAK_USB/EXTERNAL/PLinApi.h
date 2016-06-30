///////////////////////////////////////////////////////////////////////////////
//  PLinApi.h
//  Version 2.2.28.71
//
//  Definition of the PLIN-API.
//
//
//    Principle:
//    ~~~~~~~~~~
//    The driver supports multiple clients (= Windows or DOS programs
//    that communicate with LIN-busses), and multiple LIN-Devices.
//    A LIN-Device represent two LIN-Hardware (each channel is a Hardware)
//    Multiple 'Clients' can be connected to one or more LIN-Hardware, which
//    itself have an interface to a physical LIN-channel of a device.
//
//    Features:
//    ~~~~~~~~~
//     - 1 Client can be connected to multiple Hardware
//     - 1 Hardware supports multiple clients
//     - When a Client sends a message to a Hardware, the message will not be routed
//       to other clients. The response of the Hardware is routed to the connected
//       clients depending on the registered Hardware message filter.
//     - each Client only receives the messages that pass its acceptance filter
//     - each Client has a Receive Queue to buffer received messages
//     - hClient: 'Client handle'. This number is used by the driver to
//                identify and manage a Client
//     - hHw:     'Hardware handle'. This number is used by the driver to
//                identify and manage a Hardware
//     - all handles are 1-based. 0 = illegal handle
//
//     All functions return a value of type TLINError
//
//     Authors: K.Wagner / P.Steil
//
//     -----------------------------------------------------------------------
//     Copyright (C) 2008-2011 by PEAK-System Technik GmbH, Darmstadt/Germany
//     -----------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
#ifndef __LINAPIH__
#define __LINAPIH__

#ifdef __cplusplus
extern "C" {
#endif

    // Handle types
    //
    typedef BYTE HLINCLIENT;                        // Client Handle
    typedef WORD HLINHW;                            // Hardware Handle

    // Invalid Handle values
    //
#define INVALID_LIN_HANDLE              0       // Invalid value for all LIN handles (Client, Hardware)

    // Hardware Types
    //
#define  LIN_HW_TYPE_USB                1       // LIN USB type

    // Minimum and Maximum values
    //
#define LIN_MAX_FRAME_ID                63      // Maximum allowed Frame ID (0x3F)
#define LIN_MAX_SCHEDULES               8       // Maximum allowed Schedules per Hardware
#define LIN_MIN_SCHEDULE_NUMBER         0       // Minimum Schedule number
#define LIN_MAX_SCHEDULE_NUMBER         7       // Maximum Schedule number
#define LIN_MAX_SCHEDULE_SLOTS          256     // Maximum allowed Schedule slots per Hardware
#define LIN_MIN_BAUDRATE                1000    // Minimum LIN Baudrate
#define LIN_MAX_BAUDRATE                20000   // Maximum LIN Baudrate
#define LIN_MAX_NAME_LENGTH             48      // Maximum number of bytes for Name / ID of a Hardware or Client
#define LIN_MAX_USER_DATA               24      // Maximum number of bytes that a user can read/write on a Hardware

    // Frame flags for LIN Frame Entries
    //
#define FRAME_FLAG_RESPONSE_ENABLE      0x1     // Slave Enable Publisher Response
#define FRAME_FLAG_SINGLE_SHOT          0x2     // Slave Publisher Single shot
#define FRAME_FLAG_IGNORE_INIT_DATA     0x4     // Ignore InitialData on set frame entry

    // Error flags for LIN Rcv Msgs
    //
#define MSG_ERR_INCONSISTENT_SYNC       0x1     // Error on Synchronization field
#define MSG_ERR_ID_PARITY_BIT0          0x2     // Wrong parity Bit 0
#define MSG_ERR_ID_PARITY_BIT1          0x4     // Wrong parity Bit 1
#define MSG_ERR_SLAVE_NOT_RESPONDING    0x8     // Slave not responding error
#define MSG_ERR_TIMEOUT                 0x10    // A timeout was reached
#define MSG_ERR_CHECKSUM                0x20    // Wrong checksum
#define MSG_ERR_GND_SHORT               0x40    // Bus shorted to ground
#define MSG_ERR_VBAT_SHORT              0x80    // Bus shorted to VBat.
#define MSG_ERR_SLOT_DELAY              0x100   // A slot time (delay) was too small
#define MSG_ERR_OTHER_RESPONSE          0x200   // Response was received from other station
#define TLINMsgErrors                   INT32

    // Client Parameters (GetClientParam Function)
    //
#define clpName                         1       // Client Name
#define clpMessagesOnQueue              2       // Unread messages in the Receive Queue
#define clpWindowHandle                 3       // Registered windows handle (information purpose)
#define clpConnectedHardware            4       // Handles of the connected Hardware
#define clpTransmittedMessages          5       // Number of transmitted messages
#define clpReceivedMessages             6       // Number of received messages
#define clpReceiveStatusFrames          7       // Status of the property "Status Frames"
#define clpOnReceiveEventHandle         8       // Handle of the Receive event
#define clpOnPluginEventHandle          9       // Handle of the Hardware plug-in event
#define TLINClientParam                 WORD

    // Hardware Parameters (GetHardwareParam function)
    //
#define hwpName                         1       // Hardware / Device Name
#define hwpDeviceNumber                 2       // Index of the owner Device
#define hwpChannelNumber                3       // Channel Index on the owner device (0 or 1)
#define hwpConnectedClients             4       // Handles of the connected clients
#define hwpMessageFilter                5       // Message filter
#define hwpBaudrate                     6       // Baudrate
#define hwpMode                         7       // Master status
#define hwpFirmwareVersion              8       // Lin Firmware Version (Text with the form xx.yy where:
    // xx = major version. yy = minor version)
#define hwpBufferOverrunCount           9       // Receive Buffer Overrun Counter
#define hwpBossClient                   10      // Registered master Client
#define hwpSerialNumber                 11      // Serial number of a Hardware
#define hwpVersion                      12      // Version of a Hardware
#define hwpType                         13      // Type of a Hardware
#define hwpQueueOverrunCount            14      // Receive Queue Buffer Overrun Counter
#define hwpIdNumber                     15      // Hardware identification number
#define hwpUserData                     16      // User data on a hardware
#define TLINHardwareParam               WORD

    // LIN Received Message Types
    //
#define mstStandard                     0       // Standard LIN Message
#define mstBusSleep                     1       // Bus Sleep status message
#define mstBusWakeUp                    2       // Bus WakeUp status message
#define mstAutobaudrateTimeOut          3       // Auto-baudrate Timeout status message
#define mstAutobaudrateReply            4       // Auto-baudrate Reply status message
#define mstOverrun                      5       // Bus overrun status message
#define mstQueueOverrun                 6       // Queue overrun status message
#define TLINMsgType                     BYTE

    // LIN Schedule Slot Types
    //
#define sltUnconditional                0       // Unconditional frame
#define sltEvent                        1       // Event frame
#define sltSporadic                     2       // Sporadic frame
#define sltMasterRequest                3       // Diagnostic Master Request frame
#define sltSlaveResponse                4       // Diagnostic Slave Response frame
#define TLINSlotType                    BYTE

    // LIN Message Direction Types
    //
#define dirDisabled                     0       // Direction disabled
#define dirPublisher                    1       // Direction is Publisher
#define dirSubscriber                   2       // Direction is Subscriber
#define dirSubscriberAutoLength         3       // Direction is Subscriber (detect Length)
#define TLINDirection                   BYTE

    // LIN Message checksum Types
    //
#define cstCustom                       0       // Custom checksum
#define cstClassic                      1       // Classic checksum (ver 1.x)
#define cstEnhanced                     2       // Enhanced checksum
#define cstAuto                         3       // Detect checksum
#define TLINChecksumType                BYTE

    // LIN Hardware operation mode
    //
#define modNone                         0       // Hardware is not initialized
#define modSlave                        1       // Hardware working as Slave
#define modMaster                       2       // Hardware working as Master
#define TLINHardwareMode                BYTE

    // LIN Hardware status
    //
#define hwsNotInitialized               0       // Hardware is not initialized
#define hwsAutobaudrate                 1       // Hardware is detecting the baudrate
#define hwsActive                       2       // Hardware (bus) is active
#define hwsSleep                        3       // Hardware (bus) is in sleep mode
#define hwsShortGround                  6       // Hardware (bus-line) shorted to ground
#define TLINHardwareState               BYTE

    // LIN Error Codes
    //
#define errOK                           0       // No error. Success.
#define errXmtQueueFull                 1       // Transmit Queue is full.
#define errIllegalPeriod                2       // Period of time is invalid. 
#define errRcvQueueEmpty                3       // Client Receive Queue is empty.
#define errIllegalChecksumType          4       // Checksum type is invalid.
#define errIllegalHardware              5       // Hardware handle is invalid.
#define errIllegalClient                6       // Client handle is invalid.
#define errWrongParameterType           7       // Parameter type is invalid.
#define errWrongParameterValue          8       // Parameter value is invalid.
#define errIllegalDirection             9       // Direction is invalid.
#define errIllegalLength                10      // Length is outside of the valid range.
#define errIllegalBaudrate              11      // Baudrate is outside of the valid range.
#define errIllegalFrameID               12      // ID is outside of the valid range. 
#define errBufferInsufficient           13      // Buffer parameter is too small.
#define errIllegalScheduleNo            14      // Scheduler Number is outside of the valid range.
#define errIllegalSlotCount             15      // Slots count is bigger than the actual number of available slots.
#define errIllegalIndex                 16      // Array index is outside of the valid range.
#define errIllegalRange                 17      // Range of bytes to be updated is invalid.
#define errOutOfResource                1001    // LIN Manager does not have enough resources for the current task.
#define errManagerNotLoaded             1002    // The LIN Device Manager is not running.
#define errManagerNotResponding         1003    // The communication to the LIN Device Manager was interrupted.
#define errMemoryAccess                 1004    // A "MemoryAccessViolation" exception occurred within an API method.
#define errNotImplemented               0xFFFE  // An API method is not implemented.
#define errUnknown                      0xFFFF  // An internal error occurred within the LIN Device Manager.
#define TLINError                       DWORD

#pragma pack(push, 8)                           // These Records are 8-Bytes aligned!

    // Version Information structure
    //
    typedef struct
    {
        short Major;                                // Major part of a version number
        short Minor;                                // Minor part of a version number
        short Revision;                             // Revision part of a version number
        short Build;                                // Build part of a version number
    } TLINVersion;

    // A LIN Message to be sent
    //
    typedef  struct
    {
        BYTE FrameId;                               // Frame ID (6 bit) + Parity (2 bit)
        BYTE Length;                                // Frame Length (1..8)
        TLINDirection Direction;                    // Frame Direction (see LIN Message Direction Types)
        TLINChecksumType ChecksumType;              // Frame Checksum type (see LIN Message Checksum Types)
        BYTE Data[8];                               // Data bytes (0..7)
        BYTE Checksum;                              // Frame Checksum
    } TLINMsg;

    // A received LIN Message
    //
    typedef  struct
    {
        TLINMsgType Type;                           // Frame type (see LIN Received Message Types)
        BYTE FrameId;                               // Frame ID (6 bit) + Parity (2 bit)
        BYTE Length;                                // Frame Length (1..8)
        TLINDirection Direction;                    // Frame Direction (see LIN Message Direction Types)
        TLINChecksumType ChecksumType;              // Frame Checksum type (see LIN Message Checksum Types)
        BYTE Data[8];                               // Data bytes (0..7)
        BYTE Checksum;                              // Frame Checksum
        TLINMsgErrors ErrorFlags;                   // Frame error flags (see Error flags for LIN Rcv Msgs)
        unsigned __int64 TimeStamp;                 // Timestamp in microseconds
        HLINHW hHw;                                 // Handle of the Hardware which received the message
    } TLINRcvMsg;

    // A LIN Frame Entry
    //
    typedef struct
    {
        BYTE FrameId;                               // Frame ID (without parity)
        BYTE Length;                                // Frame Length (1..8)
        TLINDirection Direction;                    // Frame Direction (see LIN Message Direction Types)
        TLINChecksumType ChecksumType;              // Frame Checksum type (see LIN Message Checksum Types)
        WORD Flags;                                 // Frame flags (see Frame flags for LIN Msgs)
        BYTE InitialData[8];                        // Data bytes (0..7)
    } TLINFrameEntry;

    // A LIN Schedule slot
    //
    typedef struct
    {
        TLINSlotType Type;                          // Slot Type (see LIN Schedule Slot Types)
        WORD Delay;                                 // Slot Delay in Milliseconds
        BYTE FrameId[8];                            // Frame IDs (without parity)
        BYTE CountResolve;                          // ID count for sporadic frames
        // Resolve schedule number for Event frames
        DWORD Handle;                               // Slot handle (read-only)
    } TLINScheduleSlot;

    // LIN Status data
    //
    typedef struct
    {
        TLINHardwareMode Mode;                      // Hardware mode (see LIN Hardware operation mode)
        TLINHardwareState Status;                   // Hardware status (see LIN Hardware status)
        BYTE FreeOnSendQueue;                       // Count of free places in the Transmit Queue
        WORD FreeOnSchedulePool;                    // Free slots in the Schedule pool (see Minimum and Maximum values)
        WORD ReceiveBufferOverrun;                  // USB receive buffer overrun counter
    } TLINHardwareStatus;

#pragma pack(pop)

    ///////////////////////////////////////////////////////////////////////////////
    // Function prototypes

    //-----------------------------------------------------------------------------
    // LIN_RegisterClient()
    //   Registers a Client at the LIN Manager. Creates a Client handle and
    //   allocates the Receive Queue (only one per Client). The hWnd parameter
    //   can be zero for DOS Box Clients. The Client does not receive any
    //   messages until LIN_RegisterFrameId() or LIN_SetClientFilter() is called.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue
    //
    TLINError __stdcall LIN_RegisterClient (
        LPSTR strName,              // Name of the Client
        DWORD hWnd,                 // Window handle of the Client (only for information purposes)
        HLINCLIENT* hClient);       // Pointer to the Client handle buffer


    //-----------------------------------------------------------------------------
    // LIN_RemoveClient()
    //   Removes a Client from the Client list of the LIN Manager. Frees all
    //   resources (receive queues, message counters, etc.). If the Client was
    //   a Boss-Client for one or more Hardware, the Boss-Client property for
    //   those Hardware will be set to INVALID_LIN_HANDLE.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient
    //
    TLINError __stdcall LIN_RemoveClient (
        HLINCLIENT hClient);        // Handle of the Client


    //-----------------------------------------------------------------------------
    // LIN_ConnectClient()
    //   Connects a Client to a Hardware.
    //   The Hardware is assigned by its Handle.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware
    //
    TLINError __stdcall LIN_ConnectClient (
        HLINCLIENT hClient,         // Connect this Client ...
        HLINHW hHw);                // to this Hardware


    //-----------------------------------------------------------------------------
    // LIN_DisconnectClient()
    //   Disconnects a Client from a Hardware. This means: no more messages
    //   will be received by this Client from this Hardware.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware
    //
    TLINError __stdcall LIN_DisconnectClient (
        HLINCLIENT hClient,         // Disconnect this Client ...
        HLINHW hHw);                // from this Hardware.


    //-----------------------------------------------------------------------------
    // LIN_ResetClient()
    //   Flushes the Receive Queue of the Client and resets its counters.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient
    //
    TLINError __stdcall LIN_ResetClient (
        HLINCLIENT hClient);        // Handle of the Client


    //-----------------------------------------------------------------------------
    // LIN_SetClientParam()
    //   Sets a Client parameter to a given value.
    //
    //   Allowed TLINClientParam    Parameter
    //   values in wParam:          type:       Description:
    //   -------------------------  ----------  ------------------------------------
    //   clpReceiveStatusFrames     int         0 = Status Frames deactivated,
    //                                          otherwise active
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterType, errWrongParameterValue, errIllegalClient
    //
    TLINError __stdcall LIN_SetClientParam (
        HLINCLIENT hClient,         // Client Handle
        TLINClientParam wParam,     // TLINClientParam parameter
        DWORD dwValue);             // Parameter value


    //-----------------------------------------------------------------------------
    // LIN_GetClientParam()
    //   Gets a Client parameter.
    //
    //   Allowed TLINClientParam    Parameter
    //   values in wParam:          type:       Description:
    //   -------------------------  ----------  ------------------------------------
    //   clpName                    char[]      Name of the Client
    //   clpMessagesOnQueue         int         Unread messages in the Receive Queue
    //   clpWindowHandle            int         Window handle of the Client application
    //                                          (can be zero for DOS Box Clients)
    //   clpConnectedHardware       HLINHW[]    Array of Hardware Handles connected by a Client
    //                                          The first item in the array refers to the
    //                                          amount of handles. So [*] = Total handles + 1
    //   clpTransmittedMessages     int         Number of transmitted messages
    //   clpReceivedMessages        int         Number of received messages
    //   clpReceiveStatusFrames     int         0 = Status Frames deactivated, otherwise active
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterType, errWrongParameterValue, errIllegalClient,
    //   errBufferInsufficient
    //
    TLINError __stdcall LIN_GetClientParam (
        HLINCLIENT hClient,         // Client Handle
        TLINClientParam wParam,     // TLINClientParam parameter
        void* pBuff,                // Buffer for the parameter value
        WORD wBuffSize);            // Size of the buffer in bytes


    //-----------------------------------------------------------------------------
    // LIN_SetClientFilter()
    //   Sets the filter of a Client and modifies the filter of
    //   the connected Hardware.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware
    //
    TLINError __stdcall LIN_SetClientFilter (
        HLINCLIENT hClient,         // Set for this Client
        HLINHW hHw,                 // within this Hardware
        unsigned __int64 iRcvMask); // this message filter: each bit corresponds
    // to a Frame ID (0..63).


    //-----------------------------------------------------------------------------
    // LIN_GetClientFilter()
    //   Gets the filter corresponding to a given Client-Hardware pair.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware
    //
    TLINError __stdcall LIN_GetClientFilter (
        HLINCLIENT hClient,         // Client Handle
        HLINHW hHw,                 // Hardware Handle
        unsigned __int64* pRcvMask);// Buffer for the message filter: each bit
    // corresponds to a Frame ID (0..63)


    //-----------------------------------------------------------------------------
    // LIN_Read()
    //   Reads the next message/status information from a Client's Receive
    //   Queue. The message will be written to 'pMsg'.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errRcvQueueEmpty
    //
    TLINError __stdcall LIN_Read (
        HLINCLIENT hClient,         // Client Handle
        TLINRcvMsg* pMsg);          // Buffer for the message


    //-----------------------------------------------------------------------------
    // LIN_ReadMulti()
    //   Reads several received messages.
    //   pMsgBuff must be an array of 'iMaxCount' entries (must have at least
    //   a size of iMaxCount * sizeof(TLINRcvMsg) bytes).
    //   The size 'iMaxCount' of the array = max. messages that can be received.
    //   The real number of read messages will be returned in 'pCount'.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errRcvQueueEmpty
    //
    TLINError __stdcall LIN_ReadMulti (
        HLINCLIENT hClient,         // Client Handle
        TLINRcvMsg* pMsgBuff,       // Buffer for the messages
        int iMaxCount,              // Maximum number of messages to read
        int* pCount);               // Buffer for the real number of messages read


    //-----------------------------------------------------------------------------
    // LIN_Write()
    //   The Client 'hClient' transmits a message 'pMsg' to the Hardware 'hHw'.
    //   The message is written into the Transmit Queue of the Hardware.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware,
    //   errIllegalDirection, errIllegalLength
    //
    TLINError __stdcall LIN_Write (
        HLINCLIENT hClient,         // Client Handle
        HLINHW hHw,                 // Hardware Handle
        TLINMsg* pMsg);             // Message Buffer to be written


    //-----------------------------------------------------------------------------
    // LIN_InitializeHardware()
    //   Initializes a Hardware with a given Mode and Baudrate.
    //   REMARK: If the Hardware was initialized by another Client, the function
    //   will re-initialize the Hardware. All connected clients will be affected.
    //   It is the job of the user to manage the setting and/or configuration of
    //   Hardware, e.g. by using the Boss-Client parameter of the Hardware.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware,
    //   errIllegalBaudrate
    //
    TLINError __stdcall LIN_InitializeHardware (
        HLINCLIENT hClient,         // Client Handle
        HLINHW hHw,                 // Hardware Handle
        TLINHardwareMode bMode,     // Hardware Mode (see LIN Hardware Operation Modes)
        WORD wBaudrate);            // LIN Baudrate (see LIN_MIN_BAUDRATE and LIN_MAX_BAUDRATE)


    //-----------------------------------------------------------------------------
    // LIN_GetAvailableHardware()
    //   Gets an array containing the handles of the current Hardware
    //   available in the system.
    //   The count of Hardware handles returned in the array is written in
    //   'pCount'.
    //
    //   REMARK: To ONLY get the count of available Hardware, call this
    //   function using 'pBuff' = NULL and wBuffSize = 0.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errBufferInsufficient
    //
    TLINError __stdcall LIN_GetAvailableHardware(
        HLINHW* pBuff,              // Buffer for the handles
        WORD wBuffSize,             // Size of the buffer in bytes
        int* pCount);               // Number of Hardware available


    //-----------------------------------------------------------------------------
    // LIN_SetHardwareParam()
    //   Sets a Hardware parameter to a given value.
    //
    //   Allowed TLINHardwareParam  Parameter
    //   values in wParam:          type:       Description:
    //   -------------------------  ----------  -----------------------------------
    //   hwpMessageFilter           unsigned    Hardware message filter. Each bit
    //                              _int64      corresponds to a Frame ID (0..63)
    //   hwpBossClient              HLINCLIENT  Handle of the new Boss-Client
    //   hwpIdNumber                int         Identification number for a hardware
    //   hwpUserData                BYTE[]      User data to write on a hardware. See LIN_MAX_USER_DATA
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterType, errWrongParameterValue, errIllegalClient,
    //   errIllegalHardware
    //
    TLINError __stdcall LIN_SetHardwareParam (
        HLINCLIENT hClient,         // Client Handle
        HLINHW hHw,                 // Hardware Handle
        TLINHardwareParam wParam,   // TLINHardwareParam parameter
        void* pBuff,                // Buffer for the parameter value
        WORD wBuffSize);            // Size of the buffer


    //-----------------------------------------------------------------------------
    // LIN_GetHardwareParam()
    //   Gets a Hardware parameter.
    //
    //   Allowed TLINHardwareParam  Parameter
    //   values in wParam:          type:       Description:
    //   -------------------------  ----------  -----------------------------------
    //   hwpName                    char[]      Name of the Hardware. See LIN_MAX_NAME_LENGTH
    //   hwpDeviceNumber            int         Index of the Device owner of the Hardware
    //   hwpChannelNumber           int         Channel Index of the Hardware on the owner device
    //   hwpConnectedClients        BYTE[*]     Array of Client Handles conencted to a Hardware
    //                                          The first item in the array refers to the
    //                                          amount of handles. So [*] = Total handles + 1
    //   hwpMessageFilter           unsigned    Configured message filter. Each bit corresponds
    //                               _int64     to a Frame ID (0..63)
    //   hwpBaudrate                int         Configured baudrate
    //   hwpMode                    int         0 = Slave, otehrwise Master
    //   hwpFirmwareVersion         TLINVersion A TLINVersion structure containing the Firmware Version
    //   hwpBufferOverrunCount      int         Receive Buffer Overrun Counter
    //   hwpBossClient              HLINCLIENT  Handle of the current Boss-Client
    //   hwpSerialNumber            int         Serial number of the Hardware
    //   hwpVersion                 int         Version of the Hardware
    //   hwpType                    int         Type of the Hardware
    //   hwpQueueOverrunCount       int         Receive Queue Buffer Overrun Counter
    //   hwpIdNumber                int         Identification number for a hardware
    //   hwpUserData                BYTE[]      User data saved on the hardware. See LIN_MAX_USER_DATA
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterType, errWrongParameterValue, errIllegalHardware,
    //   errBufferInsufficient
    //
    TLINError __stdcall LIN_GetHardwareParam (
        HLINHW hHw,                 // Hardware Handle
        TLINHardwareParam wParam,   // TLINHardwareParam parameter
        void* pBuff,                // Buffer for the parameter value
        WORD wBuffSize);            // Size of the buffer


    //-----------------------------------------------------------------------------
    // LIN_ResetHardware()
    //   Flushes the queues of the Hardware and resets its counters.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware
    //
    TLINError __stdcall LIN_ResetHardware (
        HLINCLIENT hClient,         // Client Handle
        HLINHW hHw);                // Hardware Handle


    //-----------------------------------------------------------------------------
    // LIN_ResetHardwareConfig()
    //   Deletes the current configuration of the Hardware and sets its defaults.
    //   The Client 'hClient' must be registered and connected to the Hardware to
    //   be accessed.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware
    //
    TLINError __stdcall LIN_ResetHardwareConfig (
        HLINCLIENT hClient,         // Client Handle
        HLINHW hHw);                // Hardware handle

    //-----------------------------------------------------------------------------
    // LIN_IdentifyHardware()
    //   Phisically identifies a LIN Hardware (a channel on a LIN Device) by
    //   blinking its associated LED.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalHardware
    //
    TLINError __stdcall LIN_IdentifyHardware (
        HLINHW hHw);                // Hardware Handle


    //-----------------------------------------------------------------------------
    // LIN_RegisterFrameId()
    //   Modifies the filter of a Client and, eventually, the filter of the
    //   connected Hardware. The messages with FrameID 'bFromFrameId' to
    //   'bToFrameId' will be received.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware,
    //   errIllegalFrameID
    //
    TLINError __stdcall LIN_RegisterFrameId (
        HLINCLIENT hClient,         // Client Handle
        HLINHW hHw,                 // Hardware Handle
        BYTE bFromFrameId,          // First ID of the frame range
        BYTE bToFrameId);           // Last ID of the frame range


    //-----------------------------------------------------------------------------
    // LIN_SetFrameEntry()
    //   Configures a LIN Frame in a given Hardware. The Client 'hClient' must
    //   be registered and connected to the Hardware to be accessed.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware,
    //   errIllegalFrameID, errIllegalLength
    //
    TLINError __stdcall LIN_SetFrameEntry (
        HLINCLIENT hClient,             // Client Handle
        HLINHW hHw,                     // Hardware Handle
        TLINFrameEntry* pFrameEntry);   // Frame entry buffer


    //-----------------------------------------------------------------------------
    // LIN_GetFrameEntry()
    //   Gets the configuration of a LIN Frame from a given Hardware.
    //   The 'pFrameEntry.FrameId' must be set to the ID of the frame, whose
    //   configuration should be returned.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalHardware, errIllegalFrameID
    //
    TLINError __stdcall LIN_GetFrameEntry (
        HLINHW hHw,                     // Hardware Handle
        TLINFrameEntry* pFrameEntry);   // Frame Entry buffer


    //-----------------------------------------------------------------------------
    // LIN_UpdateByteArray()
    //   Updates the data of a LIN Frame for a given Hardware. The Client
    //   'hClient' must be registered and connected to the Hardware to be
    //   accessed. 'pData' must have at least a size of 'bLen'.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware,
    //   errIllegalFrameID, errIllegalLength, errIllegalIndex,
    //   errIllegalRange
    //
    TLINError __stdcall LIN_UpdateByteArray (
        HLINCLIENT hClient,         // Client handle
        HLINHW hHw,                 // Hardware Handle
        BYTE bFrameId,              // Frame ID
        BYTE bIndex,                // Index where the update data Starts (0..7)
        BYTE bLen,                  // Count of Data bytes to be updated.
        BYTE* pData);               // Data buffer


    //-----------------------------------------------------------------------------
    // LIN_StartKeepAlive()
    //   Sets the Frame 'bFrameId' as Keep-Alive frame for the given Hardware and
    //   starts to send it every 'wPeriod' Milliseconds. The Client 'hClient' must
    //   be registered and connected to the Hardware to be accessed.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware,
    //   errIllegalFrameID
    //
    TLINError __stdcall LIN_StartKeepAlive (
        HLINCLIENT hClient,         // Client Handle
        HLINHW hHw,                 // Hardware Handle
        BYTE bFrameId,              // ID of the Keep-Alive Frame
        WORD wPeriod);              // Keep-Alive Interval in Milliseconds


    //-----------------------------------------------------------------------------
    // LIN_SuspendKeepAlive()
    //   Suspends the sending of a Keep-Alive frame in the given Hardware.
    //   The Client 'hClient' must be registered and connected to the Hardware
    //   to be accessed.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware
    //
    TLINError __stdcall LIN_SuspendKeepAlive (
        HLINCLIENT hClient,         // Client Handle
        HLINHW hHw);                // Hardware Handle


    //-----------------------------------------------------------------------------
    // LIN_ResumeKeepAlive()
    //   Resumes the sending of a KeepAlive frame in the given Hardware.
    //   The Client 'hClient' must be registered and connected to the Hardware
    //   to be accessed.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware
    //
    TLINError __stdcall LIN_ResumeKeepAlive (
        HLINCLIENT hClient,         // Client Handle
        HLINHW hHw);                // Hardware Handle


    //-----------------------------------------------------------------------------
    // LIN_SetSchedule()
    //   Configures the slots of a Schedule in a given Hardware. The Client
    //   'hClient' must be registered and connected to the Hardware to be
    //   accessed. The Slot handles will be returned in the parameter
    //   "pSchedule" (Slots buffer), when this function successfully completes.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware,
    //   errIllegalScheduleNo, errIllegalSlotCount
    //
    TLINError __stdcall LIN_SetSchedule (
        HLINCLIENT hClient,             // Client Handle
        HLINHW hHw,                     // Hardware Handle
        int iScheduleNumber,            // Schedule number (see LIN_MIN_SCHEDULE_NUMBER
        // and LIN_MAX_SCHEDULE_NUMBER)
        TLINScheduleSlot* pSchedule,    // Slots buffer
        int iSlotCount);                // Count of Slots in the slots buffer


    //-----------------------------------------------------------------------------
    // LIN_GetSchedule()
    //   Gets the slots of a Schedule from a given Hardware. The count of slots
    //   returned in the array is written in 'pSlotCount'.
    //
    //   REMARK: To ONLY get the count of slots contained in the given Schedule,
    //   call this function using 'pScheduleBuff' = NULL and iMaxSlotCount = 0.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalHardware, errIllegalScheduleNo,
    //   errIllegalSlotCount
    //
    TLINError __stdcall LIN_GetSchedule (
        HLINHW  hHw,                        // Hardware Handle
        int iScheduleNumber,                // Schedule Number (see LIN_MIN_SCHEDULE_NUMBER
        // and LIN_MAX_SCHEDULE_NUMBER)
        TLINScheduleSlot* pScheduleBuff,    // Slots Buffer.
        int iMaxSlotCount,                  // Maximum number of slots to read.
        int* pSlotCount);                   // Real number of slots read.


    //-----------------------------------------------------------------------------
    // LIN_DeleteSchedule()
    //   Removes all slots contained by a Schedule of a given Hardware. The
    //   Client 'hClient' must be registered and connected to the Hardware to
    //   be accessed.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware,
    //   errIllegalScheduleNo
    //
    TLINError __stdcall LIN_DeleteSchedule (
        HLINCLIENT hClient,             // Client Handle
        HLINHW hHw,                     // Hardware Handle
        int iScheduleNumber);           // Schedule Number (see LIN_MIN_SCHEDULE_NUMBER
    // and LIN_MAX_SCHEDULE_NUMBER)


    //-----------------------------------------------------------------------------
    // LIN_SetScheduleBreakPoint()
    //   Sets a 'breakpoint' on a slot from a Schedule in a given Hardware. The
    //   Client 'hClient' must be registered and connected to the Hardware to
    //   be accessed.
    //
    //  REMARK: Giving 'dwHandle' a value of 0 ('NULL'), causes the deletion of
    //  the breakpoint.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware
    //
    TLINError __stdcall LIN_SetScheduleBreakPoint (
        HLINCLIENT hClient,             // Client Handle
        HLINHW hHw,                     // Hardware Handle
        int iBreakPointNumber,          // Breakpoint Number (0 or 1)
        DWORD dwHandle);                // Slot Handle


    //-----------------------------------------------------------------------------
    // LIN_StartSchedule()
    //   Activates a Schedule in a given Hardware. The Client 'hClient' must
    //   be registered and connected to the Hardware to be accessed.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware,
    //   errIllegalScheduleNo
    //
    TLINError __stdcall LIN_StartSchedule (
        HLINCLIENT hClient,             // Client Handle
        HLINHW hHw,                     // Hardware Handle
        int iScheduleNumber);           // Schedule Number (see LIN_MIN_SCHEDULE_NUMBER
    // and LIN_MAX_SCHEDULE_NUMBER)


    //-----------------------------------------------------------------------------
    // LIN_SuspendSchedule()
    //   Suspends an active Schedule in a given Hardware. The Client 'hClient'
    //   must be registered and connected to the Hardware to be accessed.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware
    //
    TLINError __stdcall LIN_SuspendSchedule (
        HLINCLIENT hClient,             // Client Handle
        HLINHW hHw);                    // Hardware Handle


    //-----------------------------------------------------------------------------
    // LIN_ResumeSchedule()
    //   Restarts a configured Schedule in a given Hardware. The Client 'hClient'
    //   must be registered and connected to the Hardware to be accessed.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware
    //
    TLINError __stdcall LIN_ResumeSchedule (
        HLINCLIENT hClient,             // Client Handle
        HLINHW hHw);                    // Hardware Handle


    //-----------------------------------------------------------------------------
    // LIN_XmtWakeUp()
    //   Sends a wake-up impulse (single data byte 0xF0). The Client 'hClient'
    //   must be registered and connected to the Hardware to be accessed.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware
    //
    TLINError __stdcall LIN_XmtWakeUp (
        HLINCLIENT hClient,             // Client Handle
        HLINHW hHw);                    // Hardware Handle


    //-----------------------------------------------------------------------------
    // LIN_StartAutoBaud()
    //   Starts a process to detect the Baud rate of the LIN bus that is
    //   connected to the indicated Hardware.
    //   The Client 'hClient' must be registered and connected to the Hardware
    //   to be accessed. The Hardware must be not initialized in order
    //   to do an Auto-baudrate procedure.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalClient, errIllegalHardware
    //
    TLINError __stdcall LIN_StartAutoBaud (
        HLINCLIENT hClient,             // Client Handle
        HLINHW hHw,                     // Hardware Handle
        WORD wTimeOut);                 // Auto-baudrate Timeout in Milliseconds


    //-----------------------------------------------------------------------------
    // LIN_GetStatus()
    //   Retrieves current status information from the given Hardware.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalHardware
    //
    TLINError __stdcall LIN_GetStatus (
        HLINHW hHw,                         // Hardware handle
        TLINHardwareStatus* pStatusBuff);   // Status data buffer


    //-----------------------------------------------------------------------------
    // LIN_CalculateChecksum()
    //   Calculates the checksum of a LIN Message and writes it into the
    //   'Checksum' field of 'pMsg'.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalLength
    //
    TLINError __stdcall LIN_CalculateChecksum (
        TLINMsg* pMsg);                     // Message buffer


    //-----------------------------------------------------------------------------
    // LIN_GetVersion()
    //   Returns a TLINVersion sttructure containing the PLIN-API DLL version.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue
    //
    TLINError __stdcall LIN_GetVersion (
        TLINVersion* pVerBuff               // Version buffer
    );

    //-----------------------------------------------------------------------------
    // LIN_GetVersionInfo()
    //   Returns a string containing Copyright information.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue
    //
    TLINError __stdcall LIN_GetVersionInfo (
        LPSTR  strTextBuff,                 // String buffer
        WORD wBuffSize);                    // Size in bytes of the buffer


    //-----------------------------------------------------------------------------
    // LIN_GetErrorText()
    //   Converts the error code 'dwError' to a text containing an error
    //   description in the language given as parameter (when available).
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errBufferInsufficient
    //
    TLINError __stdcall LIN_GetErrorText (
        TLINError dwError,                  // A TLINError code
        BYTE bLanguage,                     // Indicates a "Primary language ID"
        LPSTR strTextBuff,                  // Error string buffer
        WORD wBuffSize);                    // Buffer size in bytes


    //-----------------------------------------------------------------------------
    // LIN_GetPID()
    //   Gets the 'FrameId with Parity' corresponding to the given
    //   'pFrameId' and writes the result on it.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalFrameID
    //
    TLINError __stdcall LIN_GetPID(
        BYTE*  pFrameId);                   // Frame ID (0..LIN_MAX_FRAME_ID)


    //-----------------------------------------------------------------------------
    // LIN_GetTargetTime()
    //   Gets the system time used by the LIN-USB adapter.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalHardware
    //
    TLINError __stdcall  LIN_GetTargetTime (
        HLINHW hHw,                         // Hardware Handle
        unsigned __int64*  pTargetTime);    // TargetTime buffer


    //-----------------------------------------------------------------------------
    // LIN_SetResponseRemap()
    //   Sets the Response Remap of a LIN Slave
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalFrameID, errIllegalClient, errIllegalHardware,
    //   errMemoryAccess
    //
    TLINError __stdcall LIN_SetResponseRemap (
        HLINCLIENT  hClient,                // Client Handle
        HLINHW hHw,                         // Hardware Handle
        BYTE* pRemapTab);                   // Remap Response buffer


    //-----------------------------------------------------------------------------
    // LIN_GetResponseRemap()
    //   Gets the Response Remap of a LIN Slave
    //
    //  REMARK: The Buffer must be at least 64 bytes length
    //  the breakpoint.
    //
    // Possible DLL interaction errors:
    //   errManagerNotLoaded, errManagerNotResponse, errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue, errIllegalHardware, errMemoryAccess
    //
    TLINError __stdcall LIN_GetResponseRemap (
        HLINHW hHw,                         // Hardware Handle
        BYTE* pRemapTab);                   // Remap Response buffer


    //-----------------------------------------------------------------------------
    // LIN_GetSystemTime()
    //   Gets the current system time. The system time is returned by
    //   Windows as the elapsed number of microseconds since system start.
    //
    // Possible DLL interaction errors:
    //   errMemoryAccess
    //
    // Possible API errors:
    //   errWrongParameterValue
    //
    TLINError __stdcall LIN_GetSystemTime(
        unsigned __int64*  pSystemTime);

#ifdef __cplusplus
}
#endif
#endif