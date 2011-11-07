/*----------------------------------------------------------------------------
| File:
|   vxlapi.h
| Project:
|   Multi Bus driver for Win98/WinNT/Win2000/WinXP
|
| Description:
|   Driver Interface Prototypes - customer version
|
|-----------------------------------------------------------------------------
| $Author: vismra $    $Date: 2010-07-14 10:21:56 +0200 (Mi, 14 Jul 2010) $   $Revision: 6794 $
| $Id: vxlapi.h 6794 2010-07-14 08:21:56Z vismra $
|-----------------------------------------------------------------------------
| Copyright (c) 2010 by Vector Informatik GmbH.  All rights reserved.
 ----------------------------------------------------------------------------*/


#ifndef _V_XLAPI_H_                                        
#define _V_XLAPI_H_

#ifdef __cplusplus
extern "C" {
#endif


#define _XL_EXPORT_API   __stdcall
#define _EXPORT_API      __stdcall
  #if defined (DYNAMIC_CANDRIVER_DLL) || defined (DYNAMIC_XLDRIVER_DLL)
    #define _XL_EXPORT_DECL   _XL_EXPORT_API
    #define _XL_EXPORT_DEF    _XL_EXPORT_API
  #else
    // not used for dynamic load of dll
    #define _XL_EXPORT_DECL  __declspec(dllimport) _XL_EXPORT_API
    #define _XL_EXPORT_DEF   __declspec(dllimport) _XL_EXPORT_API
  #endif



// Functions have the following parameters
#define DEFPARAMS XLportHandle portHandle, XLaccess accessMask, XLuserHandle userHandle
// Almost all xlFr... Functions have the following parameters
#define DEFFRPARAM  XLportHandle portHandle, XLaccess accessMask 

// Marcos for interface definition and implementation
#ifdef DYNAMIC_XLDRIVER_DLL

  #ifdef DO_NOT_DEFINE_EXTERN_DECLARATION

    // All DECL_STDXL_FUNC have return type XLstatus
    #define DECL_STDXL_FUNC(apiname, apitype, args)    \
      typedef XLstatus (_XL_EXPORT_API *apitype) args

  #else // DO_NOT_DEFINE_EXTERN_DECLARATION

    #define DECL_STDXL_FUNC(apiname, apitype, args)    \
      typedef XLstatus (_XL_EXPORT_API *apitype) args; \
      extern apitype apiname

  #endif // DO_NOT_DEFINE_EXTERN_DECLARATION

#else // DYNAMIC_XLDRIVER_DLL

  // All DECL_STDXL_FUNC have return type XLstatus
  #define DECL_STDXL_FUNC(apiname, apitype, args)      \
      XLstatus _XL_EXPORT_DECL apiname args
  #define IMPL_STDXL_FUNC(apiname, args)               \
     XLstatus _XL_EXPORT_DEF apiname args 

#endif // DYNAMIC_XLDRIVER_DLL

// the pshpack1.h which is shipped with BCB 6 has
// some problem with one byte alignment. So we use the pragma
#if (__BORLANDC__ >= 0x0560)		
#pragma pack(push,1)
#else
#include <pshpack1.h>
#endif


//------------------------------------------------------------------------------
// Transceiver types
//------------------------------------------------------------------------------
// CAN Cab
#define XL_TRANSCEIVER_TYPE_NONE                 0x0000
#define XL_TRANSCEIVER_TYPE_CAN_251              0x0001
#define XL_TRANSCEIVER_TYPE_CAN_252              0x0002
#define XL_TRANSCEIVER_TYPE_CAN_DNOPTO           0x0003
#define XL_TRANSCEIVER_TYPE_CAN_SWC_PROTO        0x0005  //!< Prototype. Driver may latch-up.
#define XL_TRANSCEIVER_TYPE_CAN_SWC              0x0006
#define XL_TRANSCEIVER_TYPE_CAN_EVA              0x0007
#define XL_TRANSCEIVER_TYPE_CAN_FIBER            0x0008
#define XL_TRANSCEIVER_TYPE_CAN_1054_OPTO        0x000B  //!< 1054 with optical isolation
#define XL_TRANSCEIVER_TYPE_CAN_SWC_OPTO         0x000C  //!< SWC with optical isolation
#define XL_TRANSCEIVER_TYPE_CAN_B10011S          0x000D  //!< B10011S truck-and-trailer
#define XL_TRANSCEIVER_TYPE_CAN_1050             0x000E  //!< 1050
#define XL_TRANSCEIVER_TYPE_CAN_1050_OPTO        0x000F  //!< 1050 with optical isolation
#define XL_TRANSCEIVER_TYPE_CAN_1041             0x0010  //!< 1041
#define XL_TRANSCEIVER_TYPE_CAN_1041_OPTO        0x0011  //!< 1041 with optical isolation
#define XL_TRANSCEIVER_TYPE_LIN_6258_OPTO        0x0017  //!< Vector LINcab 6258opto with transceiver Infineon TLE6258 
#define XL_TRANSCEIVER_TYPE_LIN_6259_OPTO        0x0019  //!< Vector LINcab 6259opto with transceiver Infineon TLE6259
#define XL_TRANSCEIVER_TYPE_DAIO_8444_OPTO       0x001D  //!< Vector IOcab 8444  (8 dig.Inp.; 4 dig.Outp.; 4 ana.Inp.; 4 ana.Outp.)
#define XL_TRANSCEIVER_TYPE_CAN_1041A_OPTO       0x0021  //!< 1041A with optical isolation
#define XL_TRANSCEIVER_TYPE_LIN_6259_MAG         0x0023  //!< LIN transceiver 6259, with transceiver Infineon TLE6259, magnetically isolated, stress functionality
#define XL_TRANSCEIVER_TYPE_LIN_7259_MAG         0x0025  //!< LIN transceiver 7259, with transceiver Infineon TLE7259, magnetically isolated, stress functionality
#define XL_TRANSCEIVER_TYPE_LIN_7269_MAG         0x0027  //!< LIN transceiver 7269, with transceiver Infineon TLE7269, magnetically isolated, stress functionality
#define XL_TRANSCEIVER_TYPE_CAN_1054_MAG         0x0033  //!< TJA1054, magnetically isolated, with selectable termination resistor (via 4th IO line) 
#define XL_TRANSCEIVER_TYPE_CAN_251_MAG          0x0035  //!< 82C250/251 or equivalent, magnetically isolated
#define XL_TRANSCEIVER_TYPE_CAN_1050_MAG         0x0037  //!< TJA1050, magnetically isolated
#define XL_TRANSCEIVER_TYPE_CAN_1040_MAG         0x0039  //!< TJA1040, magnetically isolated
#define XL_TRANSCEIVER_TYPE_CAN_1041A_MAG        0x003B  //!< TJA1041A, magnetically isolated
#define XL_TRANSCEIVER_TYPE_TWIN_CAN_1041A_MAG   0x0080  //!< TWINcab with two TJA1041, magnetically isolated
#define XL_TRANSCEIVER_TYPE_TWIN_LIN_7269_MAG    0x0081  //!< TWINcab with two 7259, Infineon TLE7259, magnetically isolated, stress functionality
#define XL_TRANSCEIVER_TYPE_TWIN_CAN_1041AV2_MAG 0x0082  //!< TWINcab with two TJA1041, magnetically isolated
#define XL_TRANSCEIVER_TYPE_TWIN_CAN_1054_1041A_MAG  0x0083  //!< TWINcab with TJA1054A and TJA1041A with magnetic isolation

// CAN PiggyBack
#define XL_TRANSCEIVER_TYPE_PB_CAN_251           0x0101
#define XL_TRANSCEIVER_TYPE_PB_CAN_1054          0x0103
#define XL_TRANSCEIVER_TYPE_PB_CAN_251_OPTO      0x0105
#define XL_TRANSCEIVER_TYPE_PB_CAN_SWC           0x010B
// 0x010D not supported, 0x010F, 0x0111, 0x0113 reserved for future use!! 
#define XL_TRANSCEIVER_TYPE_PB_CAN_1054_OPTO     0x0115
#define XL_TRANSCEIVER_TYPE_PB_CAN_SWC_OPTO      0x0117
#define XL_TRANSCEIVER_TYPE_PB_CAN_TT_OPTO       0x0119
#define XL_TRANSCEIVER_TYPE_PB_CAN_1050          0x011B
#define XL_TRANSCEIVER_TYPE_PB_CAN_1050_OPTO     0x011D
#define XL_TRANSCEIVER_TYPE_PB_CAN_1041          0x011F
#define XL_TRANSCEIVER_TYPE_PB_CAN_1041_OPTO     0x0121
#define XL_TRANSCEIVER_TYPE_PB_LIN_6258_OPTO     0x0129 //!< LIN piggy back with transceiver Infineon TLE6258
#define XL_TRANSCEIVER_TYPE_PB_LIN_6259_OPTO     0x012B //!< LIN piggy back with transceiver Infineon TLE6259
#define XL_TRANSCEIVER_TYPE_PB_LIN_6259_MAG      0x012D //!< LIN piggy back with transceiver Infineon TLE6259, magnetically isolated, stress functionality
#define XL_TRANSCEIVER_TYPE_PB_CAN_1041A_OPTO    0x012F //!< CAN transceiver 1041A
#define XL_TRANSCEIVER_TYPE_PB_LIN_7259_MAG      0x0131 //!< LIN piggy back with transceiver Infineon TLE7259, magnetically isolated, stress functionality
#define XL_TRANSCEIVER_TYPE_PB_LIN_7269_MAG      0x0133 //!< LIN piggy back with transceiver Infineon TLE7269, magnetically isolated, stress functionality
#define XL_TRANSCEIVER_TYPE_PB_CAN_251_MAG       0x0135 //!< 82C250/251 or compatible, magnetically isolated
#define XL_TRANSCEIVER_TYPE_PB_CAN_1050_MAG      0x0136 //!< TJA 1050, magnetically isolated
#define XL_TRANSCEIVER_TYPE_PB_CAN_1040_MAG      0x0137 //!< TJA 1040, magnetically isolated
#define XL_TRANSCEIVER_TYPE_PB_CAN_1041A_MAG     0x0138 //!< TJA 1041A, magnetically isolated
#define XL_TRANSCEIVER_TYPE_PB_DAIO_8444_OPTO    0x0139 //!< optically isolated IO piggy
#define XL_TRANSCEIVER_TYPE_PB_CAN_1054_MAG      0x013B //!< TJA1054, magnetically isolated, with selectable termination resistor (via 4th IO line) 

// FlexRay PiggyBacks
#define XL_TRANSCEIVER_TYPE_PB_FR_1080           0x0201 //!< TJA 1080
#define XL_TRANSCEIVER_TYPE_PB_FR_1080_MAG       0x0202 //!< TJA 1080 magnetically isolated piggy
#define XL_TRANSCEIVER_TYPE_PB_FR_1080A_MAG      0x0203 //!< TJA 1080A magnetically isolated piggy
#define XL_TRANSCEIVER_TYPE_PB_FR_1082_MAG       0x0204

// IOpiggy 8642
#define XL_TRANSCEIVER_TYPE_PB_DAIO_8642         0x0280 //!< Iopiggy for VN8900

//------------------------------------------------------------------------------
// Transceiver Operation Modes
//------------------------------------------------------------------------------
#define XL_TRANSCEIVER_LINEMODE_NA               ((unsigned int)0x0000)
#define XL_TRANSCEIVER_LINEMODE_TWO_LINE         ((unsigned int)0x0001)
#define XL_TRANSCEIVER_LINEMODE_CAN_H            ((unsigned int)0x0002)
#define XL_TRANSCEIVER_LINEMODE_CAN_L            ((unsigned int)0x0003)
#define XL_TRANSCEIVER_LINEMODE_SWC_SLEEP        ((unsigned int)0x0004)  //!< SWC Sleep Mode.
#define XL_TRANSCEIVER_LINEMODE_SWC_NORMAL       ((unsigned int)0x0005)  //!< SWC Normal Mode.
#define XL_TRANSCEIVER_LINEMODE_SWC_FAST         ((unsigned int)0x0006)  //!< SWC High-Speed Mode.
#define XL_TRANSCEIVER_LINEMODE_SWC_WAKEUP       ((unsigned int)0x0007)  //!< SWC Wakeup Mode.
#define XL_TRANSCEIVER_LINEMODE_SLEEP            ((unsigned int)0x0008)
#define XL_TRANSCEIVER_LINEMODE_NORMAL           ((unsigned int)0x0009)
#define XL_TRANSCEIVER_LINEMODE_STDBY            ((unsigned int)0x000a)  //!< Standby for those who support it
#define XL_TRANSCEIVER_LINEMODE_TT_CAN_H         ((unsigned int)0x000b)  //!< truck & trailer: operating mode single wire using CAN high
#define XL_TRANSCEIVER_LINEMODE_TT_CAN_L         ((unsigned int)0x000c)  //!< truck & trailer: operating mode single wire using CAN low
#define XL_TRANSCEIVER_LINEMODE_EVA_00           ((unsigned int)0x000d)  //!< CANcab Eva 
#define XL_TRANSCEIVER_LINEMODE_EVA_01           ((unsigned int)0x000e)  //!< CANcab Eva 
#define XL_TRANSCEIVER_LINEMODE_EVA_10           ((unsigned int)0x000f)  //!< CANcab Eva 
#define XL_TRANSCEIVER_LINEMODE_EVA_11           ((unsigned int)0x0010)  //!< CANcab Eva 



//------------------------------------------------------------------------------
// Transceiver Capabilities
//------------------------------------------------------------------------------
#define XL_TRANSCEIVER_CAPABILITY_NONE           ((unsigned int)0x0000)
#define XL_TRANSCEIVER_CAPABILITY_HIGHSPEED      ((unsigned int)0x0001)
#define XL_TRANSCEIVER_CAPABILITY_J1708          ((unsigned int)0x0002)

//------------------------------------------------------------------------------
// Transceiver Status Flags
//------------------------------------------------------------------------------
// (not all used, but for compatibility reasons)
#define XL_TRANSCEIVER_STATUS_PRESENT            ((unsigned int)0x0001)
#define XL_TRANSCEIVER_STATUS_POWER              ((unsigned int)0x0002)
#define XL_TRANSCEIVER_STATUS_MEMBLANK           ((unsigned int)0x0004)
#define XL_TRANSCEIVER_STATUS_MEMCORRUPT         ((unsigned int)0x0008)
#define XL_TRANSCEIVER_STATUS_POWER_GOOD         ((unsigned int)0x0010)
#define XL_TRANSCEIVER_STATUS_EXT_POWER_GOOD     ((unsigned int)0x0020)
#define XL_TRANSCEIVER_STATUS_NOT_SUPPORTED      ((unsigned int)0x0040)


////////////////////////////////////////////////////////////////////////////////
// driver status
#define XL_SUCCESS                     0   //=0x0000
#define XL_PENDING                     1   //=0x0001

#define XL_ERR_QUEUE_IS_EMPTY          10  //=0x000A
#define XL_ERR_QUEUE_IS_FULL           11  //=0x000B
#define XL_ERR_TX_NOT_POSSIBLE         12  //=0x000C
#define XL_ERR_NO_LICENSE              14  //=0x000E
#define XL_ERR_WRONG_PARAMETER         101 //=0x0065
#define XL_ERR_TWICE_REGISTER          110 //=0x006E
#define XL_ERR_INVALID_CHAN_INDEX      111 //=0x006F
#define XL_ERR_INVALID_ACCESS          112 //=0x0070
#define XL_ERR_PORT_IS_OFFLINE         113 //=0x0071
#define XL_ERR_CHAN_IS_ONLINE          116 //=0x0074
#define XL_ERR_NOT_IMPLEMENTED         117 //=0x0075
#define XL_ERR_INVALID_PORT            118 //=0x0076
#define XL_ERR_HW_NOT_READY            120 //=0x0078
#define XL_ERR_CMD_TIMEOUT             121 //=0x0079
#define XL_ERR_HW_NOT_PRESENT          129 //=0x0081
#define XL_ERR_NOTIFY_ALREADY_ACTIVE   131 //=0x0083
#define XL_ERR_NO_RESOURCES            152 //=0x0098
#define XL_ERR_WRONG_CHIP_TYPE         153 //=0x0099
#define XL_ERR_WRONG_COMMAND           154 //=0x009A
#define XL_ERR_INVALID_HANDLE          155 //=0x009B
#define XL_ERR_CANNOT_OPEN_DRIVER      201 //=0x00C9
#define XL_ERR_WRONG_BUS_TYPE          202 //=0x00CA
#define XL_ERR_DLL_NOT_FOUND           203 //=0x00CB
// special stream defines
#define XL_ERR_CONNECTION_BROKEN       210 //=0x00D2
#define XL_ERR_CONNECTION_CLOSED       211 //=0x00D3
#define XL_ERR_INVALID_STREAM_NAME     212 //=0x00D4
#define XL_ERR_CONNECTION_FAILED       213 //=0x00D5
#define XL_ERR_STREAM_NOT_FOUND        214 //=0x00D6
#define XL_ERR_STREAM_NOT_CONNECTED    215 //=0x00D7
#define XL_ERR_QUEUE_OVERRUN           216 //=0x00D8
#define XL_ERROR                       255 //=0x00FF

////////////////////////////////////////////////////////////////////////////////
// For the timestamps and the access mask the API use 64 bit values
typedef unsigned __int64 XLuint64;

////////////////////////////////////////////////////////////////////////////////
typedef char *XLstringType;

////////////////////////////////////////////////////////////////////////////////
// accessmask
typedef XLuint64 XLaccess;

////////////////////////////////////////////////////////////////////////////////
// handle for xlSetNotification
typedef HANDLE XLhandle;

////////////////////////////////////////////////////////////////////////////////
// LIN lib
//------------------------------------------------------------------------------
// defines for LIN
//------------------------------------------------------------------------------

// defines for xlLinSetChannelParams
#define XL_LIN_MASTER                             01 //!< channel is a LIN master
#define XL_LIN_SLAVE                              02 //!< channel is a LIN slave
#define XL_LIN_VERSION_1_3                      0x01 //!< LIN version 1.3
#define XL_LIN_VERSION_2_0                      0x02 //!< LIN version 2.0
#define XL_LIN_VERSION_2_1                      0x03 //!< LIN version 2.1

// defines for xlLinSetSlave
#define XL_LIN_CALC_CHECKSUM                   0x100 //!< flag for automatic 'classic' checksum calculation
#define XL_LIN_CALC_CHECKSUM_ENHANCED          0x200 //!< flag for automatic 'enhanced' checksum calculation

// defines for the SleepMode function call
#define XL_LIN_SET_SILENT                       0x01 //!< set hardware into sleep mode
#define XL_LIN_SET_WAKEUPID                     0x03 //!< set hardware into sleep mode and send a request at wake-up

// defines for xlLinSetChecksum. For LIN >= 2.0 there can be used two different Checksum models.
#define XL_LIN_CHECKSUM_CLASSIC                 0x00 //!< Use classic CRC 
#define XL_LIN_CHECKSUM_ENHANCED                0x01 //!< Use enhanced CRC
#define XL_LIN_CHECKSUM_UNDEFINED               0xff //!< Set the checksum calculation to undefined.

// defines for the wake-up events
#define XL_LIN_STAYALIVE                        0x00 //!< flag if nothing changes
#define XL_LIN_SET_SLEEPMODE                    0x01 //!< flag if the hardware is set into the sleep mode
#define XL_LIN_COMESFROM_SLEEPMODE              0x02 //!< flag if the hardware comes from the sleep mode

#define XL_LIN_WAKUP_INTERNAL                   0x01 //!< flag to signal a internal WAKEUP (event)
#define XL_LIN_UNDEFINED_DLC                    0xff //!< set the DLC to undefined (xlLINSetDLC)

// defines for xlLinSwitchSlave
#define XL_LIN_SLAVE_ON                         0xff //!< switch on the LIN slave           
#define XL_LIN_SLAVE_OFF                        0x00 //!< switch off the LIN slave

//------------------------------------------------------------------------------
// structures for LIN
//------------------------------------------------------------------------------
typedef struct {
     unsigned int LINMode;                           //!< XL_LIN_SLAVE | XL_LIN_MASTER
     int          baudrate;                          //!< the baudrate will be calculated within the API. Here: e.g. 9600, 19200
     unsigned int LINVersion;                        //!< define for the LIN version (actual V1.3 of V2.0)
     unsigned int reserved;                          //!< for future use
} XLlinStatPar;


////////////////////////////////////////////////////////////////////////////////
// Defines
//------------------------------------------------------------------------------
// message flags 
#ifndef MAX_MSG_LEN
#define MAX_MSG_LEN 8
#endif

// interface version for our events
#define XL_INTERFACE_VERSION_V2    2                                                                             
#define XL_INTERFACE_VERSION_V3    3 
#define XL_INTERFACE_VERSION_V4    4           
//current version
#define XL_INTERFACE_VERSION          XL_INTERFACE_VERSION_V3                                                     

#define XL_CAN_EXT_MSG_ID             0x80000000                                                 

#define XL_CAN_MSG_FLAG_ERROR_FRAME   0x01
#define XL_CAN_MSG_FLAG_OVERRUN       0x02           //!< Overrun in Driver or CAN Controller 
                                                     //!< special case: OVERRUN combined with TIMER
                                                     //!< means the 32 bit timer has overrun
#define XL_CAN_MSG_FLAG_NERR          0x04           //!< Line Error on Lowspeed
#define XL_CAN_MSG_FLAG_WAKEUP        0x08           //!< High Voltage Message on Single Wire CAN
#define XL_CAN_MSG_FLAG_REMOTE_FRAME  0x10
#define XL_CAN_MSG_FLAG_RESERVED_1    0x20
#define XL_CAN_MSG_FLAG_TX_COMPLETED  0x40           //!< Message Transmitted
#define XL_CAN_MSG_FLAG_TX_REQUEST    0x80           //!< Transmit Message stored into Controller

// LIN flags
#define XL_LIN_MSGFLAG_TX             XL_CAN_MSG_FLAG_TX_COMPLETED   //!< LIN TX flag
#define XL_LIN_MSGFLAG_CRCERROR       0x81                           //!< Wrong LIN CRC

//------------------------------------------------------------------------------
// structure for XL_RECEIVE_MSG, XL_TRANSMIT_MSG 

struct s_xl_can_msg {  /* 32 Bytes */
         unsigned long     id;
         unsigned short    flags;
         unsigned short    dlc;
         XLuint64          res1;
         unsigned char     data [MAX_MSG_LEN];
         XLuint64          res2;
       };




//------------------------------------------------------------------------------
// structure for XL_TRANSMIT_DAIO_DATA

// flags masks
#define XL_DAIO_DATA_GET                     0x8000
#define XL_DAIO_DATA_VALUE_DIGITAL           0x0001
#define XL_DAIO_DATA_VALUE_ANALOG            0x0002
#define XL_DAIO_DATA_PWM                     0x0010

// optional function flags
#define XL_DAIO_MODE_PULSE                   0x0020  // generates pulse in values of PWM 

struct s_xl_daio_data {  /* 32 Bytes */
         unsigned short    flags;                 // 2
         unsigned int      timestamp_correction;  // 4
         unsigned char     mask_digital;          // 1
         unsigned char     value_digital;         // 1
         unsigned char     mask_analog;           // 1
         unsigned char     reserved0;             // 1
         unsigned short    value_analog[4];       // 8
         unsigned int      pwm_frequency;         // 4
         unsigned short    pwm_value;             // 2
         unsigned int      reserved1;             // 4
         unsigned int      reserved2;             // 4
};

//------------------------------------------------------------------------------
// structure for XL_CHIP_STATE 

#define XL_CHIPSTAT_BUSOFF              0x01
#define XL_CHIPSTAT_ERROR_PASSIVE       0x02
#define XL_CHIPSTAT_ERROR_WARNING       0x04
#define XL_CHIPSTAT_ERROR_ACTIVE        0x08

#define XL_CAN_STATE_FLAG_SJA_MODE      0x00000001

struct s_xl_chip_state {
         unsigned char busStatus;
         unsigned char txErrorCounter;
         unsigned char rxErrorCounter;
         unsigned char chipState;           // raw Status Register Value
         unsigned int  flags;               
       };

//------------------------------------------------------------------------------
// structure for XL_SYNC_PULSE 
#define XL_SYNC_PULSE_EXTERNAL      0x00
#define XL_SYNC_PULSE_OUR           0x01
#define XL_SYNC_PULSE_OUR_SHARED    0x02

struct s_xl_sync_pulse {
         unsigned char    pulseCode;                 //!< generated by us
         XLuint64         time;                      //!< 1ns resolution
       };

//------------------------------------------------------------------------------
// structure and defines for XL_TRANSCEIVER 
#define XL_TRANSCEIVER_EVENT_NONE                 0
#define XL_TRANSCEIVER_EVENT_INSERTED             1  //!< cable was inserted
#define XL_TRANSCEIVER_EVENT_REMOVED              2  //!< cable was removed
#define XL_TRANSCEIVER_EVENT_STATE_CHANGE         3  //!< transceiver state changed

struct s_xl_transceiver {
         unsigned char  event_reason;                //!< reason for what was event sent
         unsigned char  is_present;                  //!< allways valid transceiver presence flag
       };


//------------------------------------------------------------------------------
// defines for SET_OUTPUT_MODE                         
#define XL_OUTPUT_MODE_SILENT                     0  //!< switch CAN trx into default silent mode
#define XL_OUTPUT_MODE_NORMAL                     1  //!< switch CAN trx into normal mode
#define XL_OUTPUT_MODE_TX_OFF                     2  //!< switch CAN trx into silent mode with tx pin off
#define XL_OUTPUT_MODE_SJA_1000_SILENT            3  //!< switch CAN trx into SJA1000 silent mode

//------------------------------------------------------------------------------
// Transceiver modes 
#define XL_TRANSCEIVER_EVENT_ERROR                1
#define XL_TRANSCEIVER_EVENT_CHANGED              2

//------------------------------------------------------------------------------
// definitions for the events and commands used by the driver
enum e_XLevent_type {
       XL_NO_COMMAND               = 0,
       XL_RECEIVE_MSG              = 1,
       XL_CHIP_STATE               = 4,
       XL_TRANSCEIVER              = 6,
       XL_TIMER                    = 8,
       XL_TRANSMIT_MSG             =10,
       XL_SYNC_PULSE               =11,
       XL_APPLICATION_NOTIFICATION =15,

       //for LIN we have special events
       XL_LIN_MSG                  =20,
       XL_LIN_ERRMSG               =21,
       XL_LIN_SYNCERR              =22,
       XL_LIN_NOANS                =23,
       XL_LIN_WAKEUP               =24,
       XL_LIN_SLEEP                =25,
       XL_LIN_CRCINFO              =26,

       // for D/A IO bus 
       XL_RECEIVE_DAIO_DATA        =32,                 //!< D/A IO data message

     };

////////////////////////////////////////////////////////////////////////////////
// LIN lib
//------------------------------------------------------------------------------
// LIN event structures 
struct s_xl_lin_msg {
  unsigned char id;
  unsigned char dlc;
  unsigned short flags;
  unsigned char data[8];
  unsigned char crc;
};
struct s_xl_lin_sleep { 
  unsigned char flag;
};

struct s_xl_lin_no_ans {
  unsigned char id;
};

struct s_xl_lin_wake_up {
  unsigned char flag;
};

struct s_xl_lin_crc_info {
  unsigned char id;
  unsigned char flags;
};

//------------------------------------------------------------------------------
// LIN messages structure
union  s_xl_lin_msg_api {
       struct s_xl_lin_msg          linMsg; 
       struct s_xl_lin_no_ans       linNoAns;
       struct s_xl_lin_wake_up      linWakeUp;
       struct s_xl_lin_sleep        linSleep;
       struct s_xl_lin_crc_info     linCRCinfo;
};

//------------------------------------------------------------------------------
// BASIC bus message structure
union s_xl_tag_data {
        struct s_xl_can_msg              msg;
        struct s_xl_chip_state           chipState;
        union  s_xl_lin_msg_api          linMsgApi;    
        struct s_xl_sync_pulse           syncPulse;
        struct s_xl_daio_data            daioData; 
        struct s_xl_transceiver          transceiver; 
      };

typedef unsigned char  XLeventTag;



//------------------------------------------------------------------------------
// XL_EVENT structures
// event type definition 

struct s_xl_event {
         XLeventTag           tag;             // 1                          
         unsigned char        chanIndex;       // 1
         unsigned short       transId;         // 2
         unsigned short       portHandle;      // 2 internal use only !!!!
         unsigned short       reserved;        // 2
         XLuint64             timeStamp;       // 8
         union s_xl_tag_data  tagData;         // 32 Bytes 
       };
                                               // --------
                                               // 48 Bytes
 
typedef struct s_xl_event XLevent;                    
// message name to aquire a unique message id from windows
#define DriverNotifyMessageName  "VectorCanDriverChangeNotifyMessage"

// defines for the supported hardware
#define XL_HWTYPE_NONE                           0
#define XL_HWTYPE_VIRTUAL                        1
#define XL_HWTYPE_CANCARDX                       2
#define XL_HWTYPE_CANAC2PCI                      6
#define XL_HWTYPE_CANCARDY                      12
#define XL_HWTYPE_CANCARDXL                     15
#define XL_HWTYPE_CANCASEXL                     21
#define XL_HWTYPE_CANCASEXL_LOG_OBSOLETE        23
#define XL_HWTYPE_CANBOARDXL                    25
#define XL_HWTYPE_CANBOARDXL_PXI                27
#define XL_HWTYPE_VN2600                        29
#define XL_HWTYPE_VN2610                        XL_HWTYPE_VN2600
#define XL_HWTYPE_VN3300                        37
#define XL_HWTYPE_VN3600                        39
#define XL_HWTYPE_VN7600                        41
#define XL_HWTYPE_CANCARDXLE                    43
#define XL_HWTYPE_VN8900                        45
#define XL_HWTYPE_VN8950                        47
#define XL_MAX_HWTYPE                           51

//------------------------------------------------------------------------------
// build a channels mask from the channels index
#define XL_CHANNEL_MASK(x) (1I64<<(x))

#define XL_MAX_APPNAME 32


//------------------------------------------------------------------------------
// driver status
typedef short XLstatus;


//defines for xlGetDriverConfig structures
#define XL_MAX_LENGTH                  31
#define XL_CONFIG_MAX_CHANNELS         64
 
//activate - channel flags
#define XL_ACTIVATE_NONE                      0 
#define XL_ACTIVATE_RESET_CLOCK               8

//activate - bus types
#define XL_BUS_TYPE_NONE            0x00000000
#define XL_BUS_TYPE_CAN             0x00000001
#define XL_BUS_TYPE_LIN             0x00000002
#define XL_BUS_TYPE_FLEXRAY         0x00000004
#define XL_BUS_TYPE_MOST            0x00000010
#define XL_BUS_TYPE_DAIO            0x00000040 // IO cab
#define XL_BUS_TYPE_HWSYNC          0x00000080
#define XL_BUS_TYPE_J1708           0x00000100

// device capabilities
#define XL_CHCAP_LOG_CAPABLE        0x00002000

#define XL_BUS_COMPATIBLE_CAN                XL_BUS_TYPE_CAN
#define XL_BUS_COMPATIBLE_LIN                XL_BUS_TYPE_LIN
#define XL_BUS_COMPATIBLE_FLEXRAY            XL_BUS_TYPE_FLEXRAY
#define XL_BUS_COMPATIBLE_MOST               XL_BUS_TYPE_MOST
#define XL_BUS_COMPATIBLE_DAIO               XL_BUS_TYPE_DAIO      //io cab
#define XL_BUS_COMPATIBLE_HWSYNC             XL_BUS_TYPE_HWSYNC    //external sync line
#define XL_BUS_COMPATIBLE_J1708              XL_BUS_TYPE_J1708

// the following bus types can be used with the current cab / piggy  
#define XL_BUS_ACTIVE_CAP_CAN                XL_BUS_COMPATIBLE_CAN<<16
#define XL_BUS_ACTIVE_CAP_LIN                XL_BUS_COMPATIBLE_LIN<<16
#define XL_BUS_ACTIVE_CAP_FLEXRAY            XL_BUS_COMPATIBLE_FLEXRAY<<16
#define XL_BUS_ACTIVE_CAP_MOST               XL_BUS_COMPATIBLE_MOST<<16
#define XL_BUS_ACTIVE_CAP_DAIO               XL_BUS_COMPATIBLE_DAIO<<16
#define XL_BUS_ACTIVE_CAP_HWSYNC             XL_BUS_COMPATIBLE_HWSYNC<<16   
#define XL_BUS_ACTIVE_CAP_J1708              XL_BUS_COMPATIBLE_J1708<<16   


//------------------------------------------------------------------------------
// acceptance filter                                                                      

#define XL_CAN_STD 01                                  //!< flag for standard ID's
#define XL_CAN_EXT 02                                  //!< flag for extended ID's                            

//------------------------------------------------------------------------------
// bit timing
typedef struct {
          unsigned long bitRate;
          unsigned char sjw;
          unsigned char tseg1;
          unsigned char tseg2;
          unsigned char sam;  // 1 or 3
        } XLchipParams; 

 
typedef struct {                                                                         
  unsigned int busType;
  union {
    struct {
      unsigned int bitRate;
      unsigned char sjw;
      unsigned char tseg1;
      unsigned char tseg2;
      unsigned char sam;  // 1 or 3
      unsigned char outputMode;
    }can;
    unsigned char raw[32];
  }data;
} XLbusParams; 

// porthandle
#define XL_INVALID_PORTHANDLE (-1)
typedef long XLportHandle, *pXLportHandle;     

// defines for FPGA core types (fpgaCoreCapabilities)
#define XL_FPGA_CORE_TYPE_NONE                        0
#define XL_FPGA_CORE_TYPE_CAN                         1
#define XL_FPGA_CORE_TYPE_LIN                         2
#define XL_FPGA_CORE_TYPE_LIN_RX                      3

//#defines for specialDeviceStatus
#define XL_SPECIAL_DEVICE_STAT_FPGA_UPDATE_DONE       1             //!< automatic driver FPGA flashing done

// structure for xlGetLicenseInfo function
// This structure is returned as an array from the xlGetLicenseInfo. It contains all available licenses on
// the queried channels. The position inside the array is defined by the license itself, e.g. the license for
// the Advanced-Flexray-Library is always at the same array index.
typedef struct s_xl_license_info {
  unsigned char bAvailable;                                         //!< License is available
  char          licName[65];                                        //!< Name of the license as NULL-terminated string
} XL_LICENSE_INFO;
typedef XL_LICENSE_INFO XLlicenseInfo;

// structures for xlGetDriverConfig
typedef struct s_xl_channel_config {
          char                name [XL_MAX_LENGTH + 1];
          unsigned char       hwType;                               //!< HWTYPE_xxxx (see above)
          unsigned char       hwIndex;                              //!< Index of the hardware (same type) (0,1,...)
          unsigned char       hwChannel;                            //!< Index of the channel (same hardware) (0,1,...)
          unsigned short      transceiverType;                      //!< TRANSCEIVER_TYPE_xxxx (see above)
          unsigned int        transceiverState;                     //!< transceiver state (XL_TRANSCEIVER_STATUS...)
          unsigned char       channelIndex;                         //!< Global channel index (0,1,...)
          XLuint64            channelMask;                          //!< Global channel mask (=1<<channelIndex)
          unsigned int        channelCapabilities;                  //!< cap. which can be supported (e.g XXX_BUS_COMPATIBLE_CAN)
          unsigned int        channelBusCapabilities;               //!< what buses are supported and which are possible to be 
                                                                    //!< activated (e.g. XXX_BUS_ACTIVE_CAP_CAN)
                              
          // Channel          
          unsigned char       isOnBus;                              //!< The channel is on bus
          unsigned int        connectedBusType;                     //!< currently selected bus      
          XLbusParams         busParams;                
                                                        
          unsigned int        driverVersion;            
          unsigned int        interfaceVersion;                     //!< version of interface with driver
          unsigned int        raw_data[10];
                              
          unsigned int        serialNumber;
          unsigned int        articleNumber;
                              
          char                transceiverName [XL_MAX_LENGTH + 1];  //!< name for CANcab or another transceiver
                              
          unsigned int        specialCabFlags;                      //!< XL_SPECIAL_CAB_LIN_RECESSIVE_STRESS, XL_SPECIAL_CAB_LIN_DOMINANT_TIMEOUT flags
          unsigned int        dominantTimeout;                      //!< Dominant Timeout in us.
          unsigned char       dominantRecessiveDelay;               //!< Delay in us.
          unsigned char       recessiveDominantDelay;               //!< Delay in us.
          unsigned char       connectionInfo;                       //!< XL_CONNECTION_INFO_XXX
          unsigned char       currentlyAvailableTimestamps;         //!< XL_CURRENTLY_AVAILABLE_TIMESTAMP...
          unsigned short      minimalSupplyVoltage;                 //!< Minimal Supply Voltage of the Cab/Piggy in 1/100 V
          unsigned short      maximalSupplyVoltage;                 //!< Maximal Supply Voltage of the Cab/Piggy in 1/100 V
          unsigned int        maximalBaudrate;                      //!< Maximal supported LIN baudrate
          unsigned char       fpgaCoreCapabilities;                 //!< e.g.: XL_FPGA_CORE_TYPE_XXX
          unsigned char       specialDeviceStatus;                  //!< e.g.: XL_SPECIAL_DEVICE_STAT_XXX
          unsigned short      channelBusActiveCapabilities;         //!< like channelBusCapabilities (but without core dependencies)
          unsigned short      breakOffset;                          //!< compensation for edge asymmetry in ns 
          unsigned short      delimiterOffset;                      //!< compensation for edgdfde asymmetry in ns 
          unsigned int        reserved[3];
        } XL_CHANNEL_CONFIG;

typedef XL_CHANNEL_CONFIG  XLchannelConfig;
typedef XL_CHANNEL_CONFIG  *pXLchannelConfig;

typedef struct s_xl_driver_config {
          unsigned int      dllVersion;
          unsigned int      channelCount;  // total number of channels
          unsigned int      reserved[10];
          XLchannelConfig   channel[XL_CONFIG_MAX_CHANNELS];    // [channelCount]
        } XL_DRIVER_CONFIG;

typedef XL_DRIVER_CONFIG  XLdriverConfig;
typedef XL_DRIVER_CONFIG  *pXLdriverConfig;

///////////////////////////////////////////////////////
// DAIO params definition

// analog and digital port configuration
#define XL_DAIO_DIGITAL_ENABLED                 0x00000001  // digital port is enabled
#define XL_DAIO_DIGITAL_INPUT                   0x00000002  // digital port is input, otherwise it is an output
#define XL_DAIO_DIGITAL_TRIGGER                 0x00000004  // digital port is trigger

#define XL_DAIO_ANALOG_ENABLED                  0x00000001  // analog port is enabled
#define XL_DAIO_ANALOG_INPUT                    0x00000002  // analog port is input, otherwise it is an output
#define XL_DAIO_ANALOG_TRIGGER                  0x00000004  // analog port is trigger
#define XL_DAIO_ANALOG_RANGE_32V                0x00000008  // analog port is in range 0..32,768V, otherwise 0..8,192V

// XL_DAIO trigger mode
#define XL_DAIO_TRIGGER_MODE_NONE               0x00000000  // no trigger configured
#define XL_DAIO_TRIGGER_MODE_DIGITAL            0x00000001  // trigger on preconfigured digital lines
#define XL_DAIO_TRIGGER_MODE_ANALOG_ASCENDING   0x00000002  // trigger on input 3 ascending
#define XL_DAIO_TRIGGER_MODE_ANALOG_DESCENDING  0x00000004  // trigger on input 3 ascending
#define XL_DAIO_TRIGGER_MODE_ANALOG             (XL_DAIO_TRIGGER_MODE_ANALOG_ASCENDING | \
                                                 XL_DAIO_TRIGGER_MODE_ANALOG_DESCENDING)  // trigger on input 3

// XL_DAIO trigger level
#define XL_DAIO_TRIGGER_LEVEL_NONE                       0  // no trigger level is defined

// periodic measurement setting
#define XL_DAIO_POLLING_NONE                             0  // periodic measurement is disabled 

// structure for the acceptance filter
struct _XLacc_filt {
  unsigned char  isSet;
  unsigned long  code;
  unsigned long  mask; // relevant = 1
};
typedef struct _XLacc_filt  XLaccFilt;

// structure for the acceptance filter of one CAN chip
struct _XLacceptance {
  XLaccFilt   std;
  XLaccFilt   xtd;
};
typedef struct _XLacceptance XLacceptance;

// defines for xlSetGlobalTimeSync
#define XL_SET_TIMESYNC_NO_CHANGE    (unsigned long)  0
#define XL_SET_TIMESYNC_ON           (unsigned long)  1
#define XL_SET_TIMESYNC_OFF          (unsigned long)  2


#include <poppack.h>

////////////////////////////////////////////////////////////////////////////////
// MOST lib
//------------------------------------------------------------------------------
// special MOST defines

#define XLuserHandle               unsigned short

// size of allocation table 
#define MOST_ALLOC_TABLE_SIZE                    64   // size of channel alloctaion table + 4Bytes (MPR, MDR; ?, ?)

// max. size of rx fifo for rx event in bytes 
#define RX_FIFO_MOST_QUEUE_SIZE_MAX                  1048576 

// defines for the event tags
#define XL_MOST_START                                0x0101
#define XL_MOST_STOP                                 0x0102
#define XL_MOST_EVENTSOURCES                         0x0103
#define XL_MOST_ALLBYPASS                            0x0107
#define XL_MOST_TIMINGMODE                           0x0108
#define XL_MOST_FREQUENCY                            0x0109
#define XL_MOST_REGISTER_BYTES                       0x010a
#define XL_MOST_REGISTER_BITS                        0x010b
#define XL_MOST_SPECIAL_REGISTER                     0x010c
#define XL_MOST_CTRL_RX_SPY                          0x010d
#define XL_MOST_CTRL_RX_OS8104                       0x010e
#define XL_MOST_CTRL_TX                              0x010f
#define XL_MOST_ASYNC_MSG                            0x0110
#define XL_MOST_ASYNC_TX                             0x0111
#define XL_MOST_SYNC_ALLOCTABLE                      0x0112
#define XL_MOST_SYNC_VOLUME_STATUS                   0x0116
#define XL_MOST_RXLIGHT                              0x0117
#define XL_MOST_TXLIGHT                              0x0118
#define XL_MOST_LOCKSTATUS                           0x0119
#define XL_MOST_ERROR                                0x011a
#define XL_MOST_CTRL_RXBUFFER                        0x011c
#define XL_MOST_CTRL_SYNC_AUDIO                      0x011f
#define XL_MOST_SYNC_MUTE_STATUS                     0x0120
#define XL_MOST_GENLIGHTERROR                        0x0121
#define XL_MOST_GENLOCKERROR                         0x0122
#define XL_MOST_TXLIGHT_POWER                        0x0123
#define XL_MOST_CTRL_BUSLOAD                         0x0126
#define XL_MOST_ASYNC_BUSLOAD                        0x0127
#define XL_MOST_CTRL_SYNC_AUDIO_EX                   0x012a
#define XL_MOST_TIMINGMODE_SPDIF                     0x012b
#define XL_MOST_STREAM_STATE                         0x012c
#define XL_MOST_STREAM_BUFFER                        0x012d

// defines for xlMostSwitchEventSources
#define XL_MOST_SOURCE_ASYNC_SPY                     0x8000
#define XL_MOST_SOURCE_ASYNC_RX                      0x1000
#define XL_MOST_SOURCE_ASYNC_TX                      0x0800
#define XL_MOST_SOURCE_CTRL_OS8104A                  0x0400
#define XL_MOST_SOURCE_CTRL_SPY                      0x0100
#define XL_MOST_SOURCE_SYNCLINE                      0x0010
#define XL_MOST_SOURCE_ASYNC_RX_FIFO_OVER            0x0008

// data for XL_MOST_ERROR:
#define XL_MOST_OS8104_TX_LOCK_ERROR             0x00000001
#define XL_MOST_OS8104_SPDIF_LOCK_ERROR          0x00000002
#define XL_MOST_OS8104_ASYNC_BUFFER_FULL         0x00000003
#define XL_MOST_OS8104_ASYNC_CRC_ERROR           0x00000004
#define XL_MOST_ASYNC_TX_UNDERRUN                0x00000005
#define XL_MOST_CTRL_TX_UNDERRUN                 0x00000006
#define XL_MOST_MCU_TS_CMD_QUEUE_UNDERRUN        0x00000007
#define XL_MOST_MCU_TS_CMD_QUEUE_OVERRUN         0x00000008
#define XL_MOST_CMD_TX_UNDERRUN                  0x00000009
#define XL_MOST_SYNCPULSE_ERROR                  0x0000000A
#define XL_MOST_OS8104_CODING_ERROR              0x0000000B
#define XL_MOST_ERROR_UNKNOWN_COMMAND            0x0000000C
#define XL_MOST_ASYNC_RX_OVERFLOW_ERROR          0x0000000D
#define XL_MOST_FPGA_TS_FIFO_OVERFLOW            0x0000000E
#define XL_MOST_SPY_OVERFLOW_ERROR               0x0000000F
#define XL_MOST_CTRL_TYPE_QUEUE_OVERFLOW         0x00000010
#define XL_MOST_ASYNC_TYPE_QUEUE_OVERFLOW        0x00000011
#define XL_MOST_CTRL_UNKNOWN_TYPE                0x00000012
#define XL_MOST_CTRL_QUEUE_UNDERRUN              0x00000013
#define XL_MOST_ASYNC_UNKNOWN_TYPE               0x00000014
#define XL_MOST_ASYNC_QUEUE_UNDERRUN             0x00000015
 
// data for demanded timstamps
#define XL_MOST_DEMANDED_START                   0x00000001

#define XL_MOST_RX_DATA_SIZE                           1028
#define XL_MOST_TS_DATA_SIZE                             12
#define XL_MOST_RX_ELEMENT_HEADER_SIZE                   32
#define XL_MOST_CTRL_RX_SPY_SIZE                         36
#define XL_MOST_CTRL_RX_OS8104_SIZE                      28
#define XL_MOST_SPECIAL_REGISTER_CHANGE_SIZE             20
#define XL_MOST_ERROR_EV_SIZE_4                           4  // dwords
#define XL_MOST_ERROR_EV_SIZE                            16  // bytes


// defines for the audio devices
#define XL_MOST_DEVICE_CASE_LINE_IN                       0
#define XL_MOST_DEVICE_CASE_LINE_OUT                      1
#define XL_MOST_DEVICE_SPDIF_IN                           7
#define XL_MOST_DEVICE_SPDIF_OUT                          8
#define XL_MOST_DEVICE_SPDIF_IN_OUT_SYNC                 11

// defines for xlMostCtrlSyncAudioEx, mode
#define XL_MOST_SPDIF_LOCK_OFF                            0
#define XL_MOST_SPDIF_LOCK_ON                             1

// defines for the XL_MOST_SYNC_MUTES_STATUS event
#define XL_MOST_NO_MUTE                                   0
#define XL_MOST_MUTE                                      1

// defines for the event sources in XLmostEvent
#define XL_MOST_VN2600                                 0x01
#define XL_MOST_OS8104A                                0x02
#define XL_MOST_OS8104B                                0x04
#define XL_MOST_SPY                                    0x08

// defines for xlMostSetAllBypass and XL_MOST_ALLBYPASS
#define XL_MOST_MODE_DEACTIVATE                           0
#define XL_MOST_MODE_ACTIVATE                             1
#define XL_MOST_MODE_FORCE_DEACTIVATE                     2

#define XL_MOST_RX_BUFFER_CLEAR_ONCE                      2

// defines for xlMostSetTimingMode and the XL_MOST_TIMINGMODE(_SPDIF)_EV event.
#define XL_MOST_TIMING_SLAVE                              0
#define XL_MOST_TIMING_MASTER                             1
#define XL_MOST_TIMING_SLAVE_SPDIF_MASTER                 2
#define XL_MOST_TIMING_SLAVE_SPDIF_SLAVE                  3
#define XL_MOST_TIMING_MASTER_SPDIF_MASTER                4
#define XL_MOST_TIMING_MASTER_SPDIF_SLAVE                 5
#define XL_MOST_TIMING_MASTER_FROM_SPDIF_SLAVE            6


// defines for xlMostSetFrequency and the XL_MOST_FREQUENCY_EV event.
#define XL_MOST_FREQUENCY_44100                           0
#define XL_MOST_FREQUENCY_48000                           1
#define XL_MOST_FREQUENCY_ERROR                           2

// defines for xlMostSetTxLight 
#define XL_MOST_LIGHT_OFF                                 0
#define XL_MOST_LIGHT_FORCE_ON                            1   // unmodulated on
#define XL_MOST_LIGHT_MODULATED                           2   // modulated light

//defines for xlMostSetTxLightPower and the XL_MOST_TXLIGHT_POWER_EV event.
#define XL_MOST_LIGHT_FULL                                100
#define XL_MOST_LIGHT_3DB                                 50

// defines for the XL_MOST_LOCKSTATUS event 
#define XL_MOST_UNLOCK                                    5
#define XL_MOST_LOCK                                      6
#define XL_MOST_STATE_UNKNOWN                             9

// defines for the XL_MOST_CTRL_RX_OS8104 event (tx event)
#define XL_MOST_TX_WHILE_UNLOCKED                0x80000000  
#define XL_MOST_TX_TIMEOUT                       0x40000000  
#define XL_MOST_DIRECTION_RX                              0
#define XL_MOST_DIRECTION_TX                              1

#define XL_MOST_NO_QUEUE_OVERFLOW                    0x0000 // No rx-queue overflow occured
#define XL_MOST_QUEUE_OVERFLOW                       0x8000 // Overflow of rx-queue in firmware when trying to add a rx-event
#define XL_MOST_COMMAND_FAILED                       0x4000
#define XL_MOST_INTERNAL_OVERFLOW                    0x2000 // Overflow of command-timestamp-queue in firmware
#define XL_MOST_MEASUREMENT_NOT_ACTIVE               0x1000
#define XL_MOST_QUEUE_OVERFLOW_ASYNC                 0x0800 // Overflow of async rx-queue in firmware when trying to add a packet
#define XL_MOST_QUEUE_OVERFLOW_CTRL                  0x0400 // Overflow of rx-queue in firmware when trying to add a message
#define XL_MOST_NOT_SUPPORTED                        0x0200
#define XL_MOST_QUEUE_OVERFLOW_DRV                   0x0100 // Overflow occured when trying to add an event to application rx-queue 

#define XL_MOST_NA_CHANGED                           0x0001 // node address changed 
#define XL_MOST_GA_CHANGED                           0x0002 // group address changed 
#define XL_MOST_APA_CHANGED                          0x0004 // alternative packet address changed 
#define XL_MOST_NPR_CHANGED                          0x0008 // node position register changed 
#define XL_MOST_MPR_CHANGED                          0x0010 // max position register changed 
#define XL_MOST_NDR_CHANGED                          0x0020 // node delay register changed 
#define XL_MOST_MDR_CHANGED                          0x0040 // max delay register changed 
#define XL_MOST_SBC_CHANGED                          0x0080 // 
#define XL_MOST_XTIM_CHANGED                         0x0100 // 
#define XL_MOST_XRTY_CHANGED                         0x0200 // 

// defines for the MOST register (xlMostWriteRegister)
#define XL_MOST_bGA                                    0x89  // Group Address
#define XL_MOST_bNAH                                   0x8A  // Node Address High
#define XL_MOST_bNAL                                   0x8B  // Node Address Low
#define XL_MOST_bSDC2                                  0x8C  // Source Data Control 2
#define XL_MOST_bSDC3                                  0x8D  // Source Data Control 3
#define XL_MOST_bCM2                                   0x8E  // Clock Manager 2
#define XL_MOST_bNDR                                   0x8F  // Node Delay
#define XL_MOST_bMPR                                   0x90  // Maximum Position
#define XL_MOST_bMDR                                   0x91  // Maximum Delay
#define XL_MOST_bCM4                                   0x93  // Clock Manager 4
#define XL_MOST_bSBC                                   0x96  // Synchronous Bandwidth Control
#define XL_MOST_bXSR2                                  0x97  // Transceiver Status 2

#define XL_MOST_bRTYP                                  0xA0  // Receive Message Type
#define XL_MOST_bRSAH                                  0xA1  // Source Address High
#define XL_MOST_bRSAL                                  0xA2  // Source Address Low
#define XL_MOST_bRCD0                                  0xA3  // Receive Control Data 0 --> bRCD16 = bRCD0+16

#define XL_MOST_bXTIM                                  0xBE  // Transmit Retry Time
#define XL_MOST_bXRTY                                  0xBF  // Transmit Retries

#define XL_MOST_bXPRI                                  0xC0  // Transmit Priority
#define XL_MOST_bXTYP                                  0xC1  // Transmit Message Type
#define XL_MOST_bXTAH                                  0xC2  // Target Address High
#define XL_MOST_bXTAL                                  0xC3  // Target Address Low
#define XL_MOST_bXCD0                                  0xC4  // Transmit Control Data 0 --> bXCD16 = bXCD0+16

#define XL_MOST_bXTS                                   0xD5  // Transmit Transfer Status

#define XL_MOST_bPCTC                                  0xE2  // Packet Control
#define XL_MOST_bPCTS                                  0xE3  // Packet Status

// defines 
#define XL_MOST_SPY_RX_STATUS_NO_LIGHT                 0x01
#define XL_MOST_SPY_RX_STATUS_NO_LOCK                  0x02
#define XL_MOST_SPY_RX_STATUS_BIPHASE_ERROR            0x04
#define XL_MOST_SPY_RX_STATUS_MESSAGE_LENGTH_ERROR     0x08
#define XL_MOST_SPY_RX_STATUS_PARITY_ERROR             0x10
#define XL_MOST_SPY_RX_STATUS_FRAME_LENGTH_ERROR       0x20
#define XL_MOST_SPY_RX_STATUS_PREAMBLE_TYPE_ERROR      0x40
#define XL_MOST_SPY_RX_STATUS_CRC_ERROR                0x80

// defines for status of async frames
#define XL_MOST_ASYNC_NO_ERROR                         0x00
#define XL_MOST_ASYNC_SBC_ERROR                        0x0C
#define XL_MOST_ASYNC_NEXT_STARTS_TO_EARLY             0x0D
#define XL_MOST_ASYNC_TO_LONG                          0x0E

#define XL_MOST_ASYNC_UNLOCK                           0x0F // unlock occured within receiption of packet

// defines for XL_MOST_SYNC_PULSE_EV member trigger_source 
#define SYNC_PULSE_EXTERNAL                            0x00 
#define SYNC_PULSE_OUR                                 0x01 

// ctrlType value within the XL_CTRL_SPY event 
#define XL_MOST_CTRL_TYPE_NORMAL                       0x00
#define XL_MOST_CTRL_TYPE_REMOTE_READ                  0x01
#define XL_MOST_CTRL_TYPE_REMOTE_WRITE                 0x02
#define XL_MOST_CTRL_TYPE_RESOURCE_ALLOCATE            0x03
#define XL_MOST_CTRL_TYPE_RESOURCE_DEALLOCATE          0x04
#define XL_MOST_CTRL_TYPE_GET_SOURCE                   0x05

// counterType for the xlMost****GenerateBusload function
#define XL_MOST_BUSLOAD_COUNTER_TYPE_NONE              0x00
#define XL_MOST_BUSLOAD_COUNTER_TYPE_1_BYTE            0x01
#define XL_MOST_BUSLOAD_COUNTER_TYPE_2_BYTE            0x02
#define XL_MOST_BUSLOAD_COUNTER_TYPE_3_BYTE            0x03
#define XL_MOST_BUSLOAD_COUNTER_TYPE_4_BYTE            0x04

// selection bits for xlMostGetDeviceStates / CMD_GET_DEVICE_STATE->selection_mask 
#define XL_MOST_STATESEL_LIGHTLOCK                   0x0001
#define XL_MOST_STATESEL_REGISTERBUNCH1              0x0002 
#define XL_MOST_STATESEL_BYPASSTIMING                0x0004
#define XL_MOST_STATESEL_REGISTERBUNCH2              0x0008
#define XL_MOST_STATESEL_REGISTERBUNCH3              0x0010
#define XL_MOST_STATESEL_VOLUMEMUTE                  0x0020
#define XL_MOST_STATESEL_EVENTSOURCE                 0x0040
#define XL_MOST_STATESEL_RXBUFFERMODE                0x0080
#define XL_MOST_STATESEL_ALLOCTABLE                  0x0100
#define XL_MOST_STATESEL_SUPERVISOR_LOCKSTATUS       0x0200
#define XL_MOST_STATESEL_SUPERVISOR_MESSAGE          0x0400

// defines for sync data streaming
#define XL_MOST_STREAM_RX_DATA                             0 // RX streaming: MOST -> PC
#define XL_MOST_STREAM_TX_DATA                             1 // TX streaming: PC -> MOST

#define XL_MOST_STREAM_ADD_FRAME_HEADER                    1 // only for RX: additionally the orig. TS + status information are reported

// stream states
#define XL_MOST_STREAM_STATE_CLOSED                     0x01
#define XL_MOST_STREAM_STATE_OPENED                     0x02
#define XL_MOST_STREAM_STATE_STARTED                    0x03
#define XL_MOST_STREAM_STATE_STOPPED                    0x04
#define XL_MOST_STREAM_STATE_START_PENDING              0x05 // waiting for result from hw
#define XL_MOST_STREAM_STATE_STOP_PENDING               0x06 // waiting for result from hw
#define XL_MOST_STREAM_STATE_UNKNOWN                    0xFF 

// stream modes
#define XL_MOST_STREAM_ACTIVATE                            0
#define XL_MOST_STREAM_DEACTIVATE                          1

#define XL_MOST_STREAM_INVALID_HANDLE                      0  

// latency values
#define XL_MOST_STREAM_LATENCY_VERY_LOW                    0
#define XL_MOST_STREAM_LATENCY_LOW                         1
#define XL_MOST_STREAM_LATENCY_MEDIUM                      2
#define XL_MOST_STREAM_LATENCY_HIGH                        3
#define XL_MOST_STREAM_LATENCY_VERY_HIGH                   4

// error defines for sync data streaming
#define XL_MOST_STREAM_ERR_NO_ERROR                     0x00
#define XL_MOST_STREAM_ERR_INVALID_HANDLE               0x01
#define XL_MOST_STREAM_ERR_NO_MORE_BUFFERS_AVAILABLE    0x02
#define XL_MOST_STREAM_ERR_ANY_BUFFER_LOCKED            0x03
#define XL_MOST_STREAM_ERR_WRITE_RE_FAILED              0x04
#define XL_MOST_STREAM_ERR_STREAM_ALREADY_STARTED       0x05
#define XL_MOST_STREAM_ERR_TX_BUFFER_UNDERRUN           0x06
#define XL_MOST_STREAM_ERR_RX_BUFFER_OVERFLOW           0x07
#define XL_MOST_STREAM_ERR_INSUFFICIENT_RESOURCES       0x08


#include <pshpack8.h>
// -------------------------------------------------------------
//                    Structures for MOST events
// -------------------------------------------------------------

typedef struct s_xl_most_ctrl_spy {
  unsigned int arbitration;
  unsigned short targetAddress;
  unsigned short sourceAddress;
  unsigned char ctrlType;
  unsigned char ctrlData[17];
  unsigned short crc;
  unsigned short txStatus;
  unsigned short ctrlRes;
  unsigned int spyRxStatus;
} XL_MOST_CTRL_SPY_EV;

typedef struct s_xl_most_ctrl_msg {
  unsigned char ctrlPrio;
  unsigned char ctrlType;
  unsigned short targetAddress;
  unsigned short sourceAddress;
  unsigned char ctrlData[17];
  unsigned char direction;           // transmission or real receiption
  unsigned int status;               // unused for real rx msgs
} XL_MOST_CTRL_MSG_EV;

typedef struct s_xl_most_async_msg {
  unsigned int status;               // read as last data from PLD but stored first
  unsigned int crc;                  // not used
  unsigned char arbitration;
  unsigned char length;              // real length of async data in quadlets
  unsigned short targetAddress;
  unsigned short sourceAddress;
  unsigned char asyncData[1018];     // max size but only used data is transmitted to pc
} XL_MOST_ASYNC_MSG_EV;

typedef struct s_xl_most_async_tx {
  unsigned char arbitration;
  unsigned char length;              // real length of async data in quadlets
  unsigned short targetAddress;
  unsigned short sourceAddress;
  unsigned char asyncData[1014];     // worst case
} XL_MOST_ASYNC_TX_EV;

typedef struct s_xl_most_special_register {
  unsigned int changeMask;          // see defines "XL_MOST_..._CHANGED" 
  unsigned int lockStatus;
  unsigned char register_bNAH;
  unsigned char register_bNAL;
  unsigned char register_bGA;
  unsigned char register_bAPAH;
  unsigned char register_bAPAL;
  unsigned char register_bNPR;
  unsigned char register_bMPR;
  unsigned char register_bNDR;
  unsigned char register_bMDR;
  unsigned char register_bSBC;
  unsigned char register_bXTIM; 
  unsigned char register_bXRTY; 
} XL_MOST_SPECIAL_REGISTER_EV;

typedef struct s_xl_most_event_source {
  unsigned int mask;
  unsigned int state;
} XL_MOST_EVENT_SOURCE_EV;

typedef struct s_xl_most_all_bypass {
  unsigned int bypassState;
} XL_MOST_ALL_BYPASS_EV;

typedef struct s_xl_most_timing_mode {
  unsigned int timingmode;
} XL_MOST_TIMING_MODE_EV;

typedef struct s_xl_most_timing_mode_spdif {
  unsigned int timingmode;
} XL_MOST_TIMING_MODE_SPDIF_EV;

typedef struct s_xl_most_frequency {
  unsigned int frequency;
} XL_MOST_FREQUENCY_EV;

typedef struct s_xl_most_register_bytes {
  unsigned int number;
  unsigned int address;
  unsigned char value[16];
} XL_MOST_REGISTER_BYTES_EV;

typedef struct s_xl_most_register_bits {
  unsigned int address;
  unsigned int value;
  unsigned int mask;
} XL_MOST_REGISTER_BITS_EV;

typedef struct s_xl_most_sync_alloc {
  unsigned char allocTable[MOST_ALLOC_TABLE_SIZE];
} XL_MOST_SYNC_ALLOC_EV;

typedef struct s_xl_most_ctrl_sync_audio { 
  unsigned int channelMask[4]; 
  unsigned int device; 
  unsigned int mode; 
} XL_MOST_CTRL_SYNC_AUDIO_EV; 

typedef struct s_xl_most_ctrl_sync_audio_ex { 
  unsigned int channelMask[16]; 
  unsigned int device; 
  unsigned int mode; 
} XL_MOST_CTRL_SYNC_AUDIO_EX_EV; 

typedef struct s_xl_most_sync_volume_status {
  unsigned int device;
  unsigned int volume;
} XL_MOST_SYNC_VOLUME_STATUS_EV;

typedef struct s_xl_most_sync_mutes_status {
  unsigned int device;
  unsigned int mute;
} XL_MOST_SYNC_MUTES_STATUS_EV;

typedef struct s_xl_most_rx_light {
  unsigned int light;
} XL_MOST_RX_LIGHT_EV;

typedef struct s_xl_most_tx_light {
  unsigned int light;
} XL_MOST_TX_LIGHT_EV;

typedef struct s_xl_most_light_power {
  unsigned int lightPower;
} XL_MOST_LIGHT_POWER_EV;

typedef struct s_xl_most_lock_status {
  unsigned int lockStatus;
} XL_MOST_LOCK_STATUS_EV;

typedef struct s_xl_most_supervisor_lock_status {
  unsigned int supervisorLockStatus;
} XL_MOST_SUPERVISOR_LOCK_STATUS_EV;

typedef struct s_xl_most_gen_light_error {
  unsigned int lightOnTime;
  unsigned int lightOffTime;
  unsigned int repeat;
} XL_MOST_GEN_LIGHT_ERROR_EV;

typedef struct s_xl_most_gen_lock_error {
  unsigned int lockOnTime;
  unsigned int lockOffTime;
  unsigned int repeat;
} XL_MOST_GEN_LOCK_ERROR_EV;

typedef struct s_xl_most_rx_buffer { 
  unsigned int mode;
} XL_MOST_RX_BUFFER_EV; 

typedef struct s_xl_most_error { 
  unsigned int errorCode; 
  unsigned int parameter[3]; 
} XL_MOST_ERROR_EV; 

typedef struct s_xl_most_sync_pulse { 
  unsigned int     triggerSource;              // e.g. external or internal trigger source
  unsigned __int64 time;                       // internal generated timestamp
} XL_MOST_SYNC_PULSE_EV;

typedef struct s_xl_most_ctrl_busload {
  unsigned int busloadCtrlStarted; 
} XL_MOST_CTRL_BUSLOAD_EV;

typedef struct s_xl_most_async_busload {
  unsigned int busloadAsyncStarted; 
} XL_MOST_ASYNC_BUSLOAD_EV; 

typedef struct s_xl_most_stream_state {
  unsigned int streamHandle; 
  unsigned int streamState; // see XL_MOST_STREAM_STATE_...
  unsigned int streamError; // see XL_MOST_STREAM_ERR_...
  unsigned int reserved;
} XL_MOST_STREAM_STATE_EV;

typedef struct s_xl_most_stream_buffer {
  unsigned int   streamHandle;
  unsigned char* pBuffer;
  unsigned int   validBytes;
  unsigned int   status; // // see XL_MOST_STREAM_ERR_...
  unsigned int   reserved;
} XL_MOST_STREAM_BUFFER_EV;


#define XL_MOST_EVENT_HEADER_SIZE                                                          32 
#define XL_MOST_EVENT_MAX_DATA_SIZE                                                      1024 
#define XL_MOST_EVENT_MAX_SIZE      (XL_MOST_EVENT_HEADER_SIZE + XL_MOST_EVENT_MAX_DATA_SIZE) 

// rx event definition 
union s_xl_most_tag_data {
    XL_MOST_CTRL_SPY_EV                mostCtrlSpy;
    XL_MOST_CTRL_MSG_EV                mostCtrlMsg;
    XL_MOST_ASYNC_MSG_EV               mostAsyncMsg;            // received async frame 
    XL_MOST_ASYNC_TX_EV                mostAsyncTx;             // async frame tx acknowledge 
    XL_MOST_SPECIAL_REGISTER_EV        mostSpecialRegister;
    XL_MOST_EVENT_SOURCE_EV            mostEventSource;
    XL_MOST_ALL_BYPASS_EV              mostAllBypass;
    XL_MOST_TIMING_MODE_EV             mostTimingMode;
    XL_MOST_TIMING_MODE_SPDIF_EV       mostTimingModeSpdif;
    XL_MOST_FREQUENCY_EV               mostFrequency;
    XL_MOST_REGISTER_BYTES_EV          mostRegisterBytes;
    XL_MOST_REGISTER_BITS_EV           mostRegisterBits;
    XL_MOST_SYNC_ALLOC_EV              mostSyncAlloc;
    XL_MOST_CTRL_SYNC_AUDIO_EV         mostCtrlSyncAudio;
    XL_MOST_CTRL_SYNC_AUDIO_EX_EV      mostCtrlSyncAudioEx;
    XL_MOST_SYNC_VOLUME_STATUS_EV      mostSyncVolumeStatus;
    XL_MOST_SYNC_MUTES_STATUS_EV       mostSyncMutesStatus;
    XL_MOST_RX_LIGHT_EV                mostRxLight;
    XL_MOST_TX_LIGHT_EV                mostTxLight;
    XL_MOST_LIGHT_POWER_EV             mostLightPower;
    XL_MOST_LOCK_STATUS_EV             mostLockStatus;
    XL_MOST_GEN_LIGHT_ERROR_EV         mostGenLightError;
    XL_MOST_GEN_LOCK_ERROR_EV          mostGenLockError;
    XL_MOST_RX_BUFFER_EV               mostRxBuffer; 
    XL_MOST_ERROR_EV                   mostError; 
    XL_MOST_SYNC_PULSE_EV              mostSyncPulse; 
    XL_MOST_CTRL_BUSLOAD_EV            mostCtrlBusload; 
    XL_MOST_ASYNC_BUSLOAD_EV           mostAsyncBusload; 
    XL_MOST_STREAM_STATE_EV            mostStreamState;
    XL_MOST_STREAM_BUFFER_EV           mostStreamBuffer;
};


typedef unsigned short    XLmostEventTag; 

struct s_xl_most_event { 
  unsigned int        size;             // 4 - overall size of the complete event 
  XLmostEventTag      tag;              // 2 - type of the event 
  unsigned short      channelIndex;     // 2 
  unsigned int        userHandle;       // 4 - internal use only 
  unsigned short      flagsChip;        // 2 
  unsigned short      reserved;         // 2 
  XLuint64            timeStamp;        // 8 
  XLuint64            timeStampSync;    // 8 
                                        // --------- 
                                        // 32 bytes -> XL_MOST_EVENT_HEADER_SIZE 
  union s_xl_most_tag_data tagData; 
}; 

typedef struct s_xl_most_event XLmostEvent; 

typedef XL_MOST_CTRL_MSG_EV                 XLmostCtrlMsg;
typedef XL_MOST_ASYNC_TX_EV                 XLmostAsyncMsg; 

typedef struct s_xl_most_ctrl_busload_configuration {
  unsigned int        transmissionRate;
  unsigned int        counterType;
  unsigned int        counterPosition;
  XL_MOST_CTRL_MSG_EV busloadCtrlMsg;
} XL_MOST_CTRL_BUSLOAD_CONFIGURATION; 


typedef struct s_xl_most_async_busload_configuration {
  unsigned int        transmissionRate;
  unsigned int        counterType;
  unsigned int        counterPosition;
  XL_MOST_ASYNC_TX_EV busloadAsyncMsg;
} XL_MOST_ASYNC_BUSLOAD_CONFIGURATION; 

typedef XL_MOST_CTRL_BUSLOAD_CONFIGURATION  XLmostCtrlBusloadConfiguration; 
typedef XL_MOST_ASYNC_BUSLOAD_CONFIGURATION XLmostAsyncBusloadConfiguration; 

typedef struct s_xl_most_device_state { 
  unsigned int  selectionMask; 
  // XL_MOST_STATESEL_LIGHTLOCK 
  unsigned int  lockState;                      // see XL_MOST_LOCK_STATUS_EV 
  unsigned int  rxLight;                        // see XL_MOST_RX_LIGHT_EV 
  unsigned int  txLight;                        // see XL_MOST_TX_LIGHT_EV 
  unsigned int  txLightPower;                   // see XL_MOST_LIGHT_POWER_EV 
  // XL_MOST_STATESEL_REGISTERBUNCH1             
  unsigned char registerBunch1[16];             // 16 OS8104 registers (0x87...0x96 -> NPR...SBC) 
  // XL_MOST_STATESEL_BYPASSTIMING              
  unsigned int  bypassState;                    // see XL_MOST_ALL_BYPASS_EV 
  unsigned int  timingMode;                     // see XL_MOST_TIMING_MODE_EV 
  unsigned int  frequency;                      // frame rate (if master); see XL_MOST_FREQUENCY_EV 
  // XL_MOST_STATESEL_REGISTERBUNCH2             
  unsigned char registerBunch2[2];              // 2 OS8104 registers (0xBE, 0xBF -> XTIM, XRTY) 
  // XL_MOST_STATESEL_REGISTERBUNCH3             
  unsigned char registerBunch3[2];              // 2 OS8104 registers (0xE8, 0xE9 -> APAH, APAL) 
  // XL_MOST_STATESEL_VOLUMEMUTE                
  unsigned int  volume[2];                      // volume state for DEVICE_CASE_LINE_IN, DEVICE_CASE_LINE_OUT 
  unsigned int  mute[2];                        // mute state for DEVICE_CASE_LINE_IN, DEVICE_CASE_LINE_OUT 
  // XL_MOST_STATESEL_EVENTSOURCE               
  unsigned int  eventSource;                    // see XL_MOST_EVENT_SOURCE_EV 
  // XL_MOST_STATESEL_RXBUFFERMODE              
  unsigned int  rxBufferMode;                   // see XL_MOST_RX_BUFFER_EV 
  // XL_MOST_STATESEL_ALLOCTABLE 
  unsigned char allocTable[MOST_ALLOC_TABLE_SIZE]; // see XL_MOST_SYNC_ALLOC_EV 
   // XL_MOST_STATESEL_SUPERVISOR_LOCKSTATUS
  unsigned int supervisorLockStatus;
  // XL_MOST_STATESEL_SUPERVISOR_MESSAGE
  unsigned int broadcastedConfigStatus;
  unsigned int adrNetworkMaster;
  unsigned int abilityToWake;
} XL_MOST_DEVICE_STATE; 

typedef XL_MOST_DEVICE_STATE                XLmostDeviceState; 

typedef struct s_xl_most_stream_open {
  unsigned int* pStreamHandle;
  unsigned int  numSyncChannels;
  unsigned int  direction;
  unsigned int  options;
  unsigned int  latency;
} XL_MOST_STREAM_OPEN;

typedef XL_MOST_STREAM_OPEN                 XLmostStreamOpen;

typedef struct s_xl_most_stream_info {
  unsigned int  streamHandle;
  unsigned int  numSyncChannels;
  unsigned int  direction;
  unsigned int  options;
  unsigned int  latency;
  unsigned int  streamState;
  unsigned int  reserved;
  unsigned char syncChannels[60];
} XL_MOST_STREAM_INFO;

typedef XL_MOST_STREAM_INFO                   XLmostStreamInfo;


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// FlexRay XL API
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#define XL_FR_MAX_DATA_LENGTH                                 254

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// event tags
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#define XL_FR_START_CYCLE                       ((unsigned short)0x0080)
#define XL_FR_RX_FRAME                          ((unsigned short)0x0081)
#define XL_FR_TX_FRAME                          ((unsigned short)0x0082)
#define XL_FR_TXACK_FRAME                       ((unsigned short)0x0083)
#define XL_FR_INVALID_FRAME                     ((unsigned short)0x0084)
#define XL_FR_WAKEUP                            ((unsigned short)0x0085)
#define XL_FR_SYMBOL_WINDOW                     ((unsigned short)0x0086)
#define XL_FR_ERROR                             ((unsigned short)0x0087)
  #define XL_FR_ERROR_POC_MODE                  ((unsigned char)0x01)
  #define XL_FR_ERROR_SYNC_FRAMES_BELOWMIN      ((unsigned char)0x02)
  #define XL_FR_ERROR_SYNC_FRAMES_OVERLOAD      ((unsigned char)0x03)
  #define XL_FR_ERROR_CLOCK_CORR_FAILURE        ((unsigned char)0x04)
  #define XL_FR_ERROR_NIT_FAILURE               ((unsigned char)0x05)
  #define XL_FR_ERROR_CC_ERROR                  ((unsigned char)0x06)
#define XL_FR_STATUS                            ((unsigned short)0x0088)
#define XL_FR_NM_VECTOR                         ((unsigned short)0x008A)
#define XL_FR_TRANCEIVER_STATUS                 ((unsigned short)0x008B)
#define XL_FR_SPY_FRAME				                  ((unsigned short)0x008E)
#define XL_FR_SPY_SYMBOL			                  ((unsigned short)0x008F)


// Attention: The timer event type has to have the same number as CANcardXL, CANboardXL, ... 

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// function structures
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// structure for xlFrSetConfiguration
typedef struct s_xl_fr_cluster_configuration {

  unsigned int      busGuardianEnable;                              
  unsigned int	    baudrate;                         
  unsigned int	    busGuardianTick;                       
  unsigned int	    externalClockCorrectionMode;                                   
  unsigned int	    gColdStartAttempts;               
  unsigned int	    gListenNoise;                     
  unsigned int	    gMacroPerCycle;                   
  unsigned int	    gMaxWithoutClockCorrectionFatal;       
  unsigned int	    gMaxWithoutClockCorrectionPassive;     
  unsigned int	    gNetworkManagementVectorLength;        
  unsigned int	    gNumberOfMinislots;               
  unsigned int	    gNumberOfStaticSlots;             
  unsigned int	    gOffsetCorrectionStart;           
  unsigned int	    gPayloadLengthStatic;             
  unsigned int	    gSyncNodeMax;                     
  unsigned int	    gdActionPointOffset;              
  unsigned int	    gdDynamicSlotIdlePhase;           
  unsigned int	    gdMacrotick;                           
  unsigned int	    gdMinislot;                       
  unsigned int	    gdMiniSlotActionPointOffset;      
  unsigned int	    gdNIT;                            
  unsigned int	    gdStaticSlot;                     
  unsigned int	    gdSymbolWindow;                        
  unsigned int	    gdTSSTransmitter;                 
  unsigned int	    gdWakeupSymbolRxIdle;             
  unsigned int	    gdWakeupSymbolRxLow;              
  unsigned int	    gdWakeupSymbolRxWindow;           
  unsigned int	    gdWakeupSymbolTxIdle;             
  unsigned int      gdWakeupSymbolTxLow;              
  unsigned int	    pAllowHaltDueToClock;             
  unsigned int	    pAllowPassiveToActive;            
  unsigned int	    pChannels;                        
  unsigned int	    pClusterDriftDamping;             
  unsigned int	    pDecodingCorrection;              
  unsigned int	    pDelayCompensationA;              
  unsigned int	    pDelayCompensationB;                            
  unsigned int	    pExternOffsetCorrection;          
  unsigned int	    pExternRateCorrection;            
  unsigned int	    pKeySlotUsedForStartup;           
  unsigned int	    pKeySlotUsedForSync;              
  unsigned int	    pLatestTx;                        
  unsigned int	    pMacroInitialOffsetA;             
  unsigned int	    pMacroInitialOffsetB;             
  unsigned int	    pMaxPayloadLengthDynamic;              
  unsigned int	    pMicroInitialOffsetA;             
  unsigned int	    pMicroInitialOffsetB;             
  unsigned int	    pMicroPerCycle;                   
  unsigned int	    pMicroPerMacroNom;                                                      
  unsigned int      pOffsetCorrectionOut;                 
  unsigned int      pRateCorrectionOut;                   
  unsigned int      pSamplesPerMicrotick;                      
  unsigned int      pSingleSlotEnabled;                   
  unsigned int      pWakeupChannel;                       
  unsigned int      pWakeupPattern;                        
  unsigned int      pdAcceptedStartupRange;               
  unsigned int      pdListenTimeout;                      
  unsigned int      pdMaxDrift;                           
  unsigned int      pdMicrotick;                               
  unsigned int      gdCASRxLowMax;                        
  unsigned int      gChannels;        
  unsigned int      vExternOffsetControl;                 
  unsigned int      vExternRateControl;                   
  unsigned int      pChannelsMTS;

  unsigned int      framePresetData;          //!< 16-bit value with data for pre-initializing the Flexray payload data words

  unsigned int      reserved[15]; 
} XLfrClusterConfig;
               

// structure and defines for function xlFrGetChannelConfig 
typedef struct s_xl_fr_channel_config { 
  unsigned int      status;             // XL_FR_CHANNEL_CFG_STATUS_xxx 
  unsigned int      cfgMode; 	          // XL_FR_CHANNEL_CFG_MODE_xxx 
  unsigned int      reserved[6]; 
  XLfrClusterConfig xlFrClusterConfig;  // same as used in function xlFrSetConfig
} XLfrChannelConfig; 

// defines for XLfrChannelConfig::status 
#define XL_FR_CHANNEL_CFG_STATUS_INIT_APP_PRESENT   ((unsigned int) 0x01) 
#define XL_FR_CHANNEL_CFG_STATUS_CHANNEL_ACTIVATED  ((unsigned int) 0x02) 
#define XL_FR_CHANNEL_CFG_STATUS_VALID_CLUSTER_CFG  ((unsigned int) 0x04) 
#define XL_FR_CHANNEL_CFG_STATUS_VALID_CFG_MODE     ((unsigned int) 0x08) 

// defines for XLfrChannelConfig::cfgMode 
#define XL_FR_CHANNEL_CFG_MODE_SYNCHRONOUS                             1 
#define XL_FR_CHANNEL_CFG_MODE_COMBINED                                2 
#define XL_FR_CHANNEL_CFG_MODE_ASYNCHRONOUS                            3 


// defines for xlFrSetMode (frModes)
#define XL_FR_MODE_NORMAL                                    0x00 //!< setup the VN3000 (eRay) normal operation mode. (default mode)
#define XL_FR_MODE_COLD_NORMAL                               0x04 //!< setup the VN3000 (Fujitsu) normal operation mode. (default mode)

// defines for xlFrSetMode (frStartupAttributes)
#define XL_FR_MODE_NONE                                      0x00 //!< for normal use
#define XL_FR_MODE_WAKEUP                                    0x01 //!< for wakeup
#define XL_FR_MODE_COLDSTART_LEADING                         0x02 //!< Coldstart path initiating the schedule synchronization
#define XL_FR_MODE_COLDSTART_FOLLOWING                       0x03 //!< Coldstart path joining other coldstart nodes
#define XL_FR_MODE_WAKEUP_AND_COLDSTART_LEADING              0x04 //!< Send Wakeup and Coldstart path initiating the schedule synchronization
#define XL_FR_MODE_WAKEUP_AND_COLDSTART_FOLLOWING            0x05 //!< Send Wakeup and Coldstart path joining other coldstart nodes

// structure for xlFrSetMode
typedef struct s_xl_fr_set_modes {
  unsigned int 	    frMode;
  unsigned int 	    frStartupAttributes;
  unsigned int 	    reserved[30];
} XLfrMode;

// defines for xlFrSetupSymbolWindow
#define XL_FR_SYMBOL_MTS                                     0x01 //!< defines a MTS (Media Access Test Symbol)
#define XL_FR_SYMBOL_CAS                                     0x02 //!< defines a CAS (Collision Avoidance Symbol)


// FR transceiver xlFrSetTransceiverMode modes
#define XL_FR_TRANSCEIVER_MODE_SLEEP                         0x01
#define XL_FR_TRANSCEIVER_MODE_NORMAL                        0x02
#define XL_FR_TRANSCEIVER_MODE_RECEIVE_ONLY                  0x03
#define XL_FR_TRANSCEIVER_MODE_STANDBY                       0x04

// defines for XL_FR_SYNC_PULSE_EV member triggerSource 
#define XL_FR_SYNC_PULSE_EXTERNAL                            0x00 
#define XL_FR_SYNC_PULSE_OUR                                 0x01 
#define XL_FR_SYNC_PULSE_OUR_SHARED                          0x02 

// defines for xlFrActivateSpy, mode
#define XL_FR_SPY_MODE_ASYNCHRONOUS                          0x01

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Flags for the flagsChip parameter
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#define XL_FR_CHANNEL_A                               ((unsigned short)0x01)
#define XL_FR_CHANNEL_B                               ((unsigned short)0x02)
#define XL_FR_CHANNEL_AB ((unsigned short)(XL_FR_CHANNEL_A|XL_FR_CHANNEL_B))
#define XL_FR_CC_COLD_A                               ((unsigned short)0x04) //!< second CC channel A to initiate the coldstart
#define XL_FR_CC_COLD_B                               ((unsigned short)0x08) //!< second CC channel B to initiate the coldstart
#define XL_FR_CC_COLD_AB ((unsigned short)(XL_FR_CC_COLD_A|XL_FR_CC_COLD_B))
#define XL_FR_SPY_CHANNEL_A                           ((unsigned short)0x10) //!< Spy mode flags
#define XL_FR_SPY_CHANNEL_B                           ((unsigned short)0x20) //!< Spy mode flags

#define XL_FR_QUEUE_OVERFLOW                        ((unsigned short)0x0100) //!< driver queue overflow  


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_FLEXRAY_FRAME structure flags / defines 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines for T_FLEXRAY_FRAME member flags 
#define XL_FR_FRAMEFLAG_STARTUP                   ((unsigned short) 0x0001)   //!< indicates a startup frame
#define XL_FR_FRAMEFLAG_SYNC                      ((unsigned short) 0x0002)   //!< indicates a sync frame
#define XL_FR_FRAMEFLAG_NULLFRAME                 ((unsigned short) 0x0004)   //!< indicates a nullframe
#define XL_FR_FRAMEFLAG_PAYLOAD_PREAMBLE          ((unsigned short) 0x0008)   //!< indicates a present payload preamble bit
#define XL_FR_FRAMEFLAG_FR_RESERVED               ((unsigned short) 0x0010)   //!< reserved by Flexray protocol

#define XL_FR_FRAMEFLAG_REQ_TXACK                 ((unsigned short) 0x0020)   //!< used for Tx events only 
#define XL_FR_FRAMEFLAG_TXACK_SS                  XL_FR_FRAMEFLAG_REQ_TXACK   //!< indicates TxAck of SingleShot; used for TxAck events only 
#define XL_FR_FRAMEFLAG_RX_UNEXPECTED             XL_FR_FRAMEFLAG_REQ_TXACK   //!< indicates unexpected Rx frame; used for Rx events only 

#define XL_FR_FRAMEFLAG_NEW_DATA_TX               ((unsigned short) 0x0040)   //!< flag used with TxAcks to indicate first TxAck after data update 
#define XL_FR_FRAMEFLAG_DATA_UPDATE_LOST          ((unsigned short) 0x0080)   //!< flag used with TxAcks indicating that data update has been lost 

#define XL_FR_FRAMEFLAG_SYNTAX_ERROR              ((unsigned short) 0x0200) 
#define XL_FR_FRAMEFLAG_CONTENT_ERROR             ((unsigned short) 0x0400) 
#define XL_FR_FRAMEFLAG_SLOT_BOUNDARY_VIOLATION   ((unsigned short) 0x0800) 
#define XL_FR_FRAMEFLAG_TX_CONFLICT               ((unsigned short) 0x1000) 
#define XL_FR_FRAMEFLAG_EMPTY_SLOT                ((unsigned short) 0x2000) 
#define XL_FR_FRAMEFLAG_FRAME_TRANSMITTED         ((unsigned short) 0x8000)   //!< Only used with TxAcks: Frame has been transmitted. If not set after transmission, an error has occurred.

// XL_FR_SPY_FRAME_EV event: frameError value
#define XL_FR_SPY_FRAMEFLAG_FRAMING_ERROR             ((unsigned char)0x01)
#define XL_FR_SPY_FRAMEFLAG_HEADER_CRC_ERROR          ((unsigned char)0x02)
#define XL_FR_SPY_FRAMEFLAG_FRAME_CRC_ERROR           ((unsigned char)0x04)

// XL_FR_SPY_FRAME_EV event: frameCRC value
#define XL_FR_SPY_FRAMEFLAG_FRAME_CRC_NEW_LAYOUT ((unsigned int)0x80000000)

// XL_FR_TX_FRAME event: txMode flags
#define XL_FR_TX_MODE_CYCLIC                          ((unsigned char)0x01)   //!< 'normal' cyclic mode
#define XL_FR_TX_MODE_SINGLE_SHOT                     ((unsigned char)0x02)   //!< sends only a single shot
#define XL_FR_TX_MODE_NONE                            ((unsigned char)0xff)   //!< switch off TX

// XL_FR_TX_FRAME event: incrementSize values
#define XL_FR_PAYLOAD_INCREMENT_8BIT                  ((unsigned char)   8)   
#define XL_FR_PAYLOAD_INCREMENT_16BIT                 ((unsigned char)  16) 
#define XL_FR_PAYLOAD_INCREMENT_32BIT                 ((unsigned char)  32) 

// XL_FR_STATUS event: statusType (POC status)
#define XL_FR_STATUS_DEFAULT_CONFIG                          0x00 //!< indicates the actual state of the POC in operation control 
#define XL_FR_STATUS_READY                                   0x01 //!< ...
#define XL_FR_STATUS_NORMAL_ACTIVE                           0x02 //!< ...
#define XL_FR_STATUS_NORMAL_PASSIVE                          0x03 //!< ...
#define XL_FR_STATUS_HALT                                    0x04 //!< ...
#define XL_FR_STATUS_MONITOR_MODE                            0x05 //!< ...
#define XL_FR_STATUS_CONFIG                                  0x0f //!< ...
                                                    
#define XL_FR_STATUS_WAKEUP_STANDBY                          0x10 //!< indicates the actual state of the POC in the wakeup path 
#define XL_FR_STATUS_WAKEUP_LISTEN                           0x11 //!< ...
#define XL_FR_STATUS_WAKEUP_SEND                             0x12 //!< ...
#define XL_FR_STATUS_WAKEUP_DETECT                           0x13 //!< ...
                                                    
#define XL_FR_STATUS_STARTUP_PREPARE                         0x20 //!< indicates the actual state of the POC in the startup path 
#define XL_FR_STATUS_COLDSTART_LISTEN                        0x21 //!< ...
#define XL_FR_STATUS_COLDSTART_COLLISION_RESOLUTION          0x22 //!< ...
#define XL_FR_STATUS_COLDSTART_CONSISTENCY_CHECK             0x23 //!< ...
#define XL_FR_STATUS_COLDSTART_GAP                           0x24 //!< ...
#define XL_FR_STATUS_COLDSTART_JOIN                          0x25 //!< ...
#define XL_FR_STATUS_INTEGRATION_COLDSTART_CHECK             0x26 //!< ...
#define XL_FR_STATUS_INTEGRATION_LISTEN                      0x27 //!< ... 
#define XL_FR_STATUS_INTEGRATION_CONSISTENCY_CHECK           0x28 //!< ...
#define XL_FR_STATUS_INITIALIZE_SCHEDULE                     0x29 //!< ...
#define XL_FR_STATUS_ABORT_STARTUP                           0x2a //!< ...
#define XL_FR_STATUS_STARTUP_SUCCESS                         0x2b //!< ...

// XL_FR_ERROR event: XL_FR_ERROR_POC_MODE, errorMode
#define XL_FR_ERROR_POC_ACTIVE                               0x00 //!< Indicates the actual error mode of the POC: active (green)
#define XL_FR_ERROR_POC_PASSIVE                              0x01 //!< Indicates the actual error mode of the POC: passive (yellow)
#define XL_FR_ERROR_POC_COMM_HALT                            0x02 //!< Indicates the actual error mode of the POC: comm-halt (red)

// XL_FR_ERROR event: XL_FR_ERROR_NIT_FAILURE, flags
#define XL_FR_ERROR_NIT_SENA                                0x100 //!< Syntax Error during NIT Channel A
#define XL_FR_ERROR_NIT_SBNA                                0x200 //!< Slot Boundary Violation during NIT Channel B
#define XL_FR_ERROR_NIT_SENB                                0x400 //!< Syntax Error during NIT Channel A
#define XL_FR_ERROR_NIT_SBNB                                0x800 //!< Slot Boundary Violation during NIT Channel B

// XL_FR_ERROR event: XL_FR_ERROR_CLOCK_CORR_FAILURE, flags
#define XL_FR_ERROR_MISSING_OFFSET_CORRECTION          0x00000001 //!< Set if no sync frames were received. -> no offset correction possible.
#define XL_FR_ERROR_MAX_OFFSET_CORRECTION_REACHED      0x00000002 //!< Set if max. offset correction limit is reached.   
#define XL_FR_ERROR_MISSING_RATE_CORRECTION            0x00000004 //!< Set if no even/odd sync frames were received -> no rate correction possible.
#define XL_FR_ERROR_MAX_RATE_CORRECTION_REACHED        0x00000008 //!< Set if max. rate correction limit is reached.	
     	
// XL_FR_ERROR event: XL_FR_ERROR_CC_ERROR, erayEir
#define XL_FR_ERROR_CC_PERR                            0x00000040 //!< Parity Error, data from MHDS (internal ERay error)
#define XL_FR_ERROR_CC_IIBA                            0x00000200 //!< Illegal Input Buffer Access (internal ERay error)  
#define XL_FR_ERROR_CC_IOBA                            0x00000400 //!< Illegal Output Buffer Access (internal ERay error)
#define XL_FR_ERROR_CC_MHF                             0x00000800 //!< Message Handler Constraints Flag data from MHDF (internal ERay error)
#define XL_FR_ERROR_CC_EDA                             0x00010000 //!< Error Detection on channel A, data from ACS
#define XL_FR_ERROR_CC_LTVA                            0x00020000 //!< Latest Transmit Violation on channel A
#define XL_FR_ERROR_CC_TABA                            0x00040000 //!< Transmit Across Boundary on Channel A
#define XL_FR_ERROR_CC_EDB                             0x01000000 //!< Error Detection on channel B, data from ACS
#define XL_FR_ERROR_CC_LTVB                            0x02000000 //!< Latest Transmit Violation on channel B    
#define XL_FR_ERROR_CC_TABB                            0x04000000 //!< Transmit Across Boundary on Channel B

// XL_FR_WAKEUP event: wakeupStatus 
#define XL_FR_WAKEUP_UNDEFINED                               0x00 //!< No wakeup attempt since CONFIG state was left. (e.g. when a wakeup pattern A|B is received)
#define XL_FR_WAKEUP_RECEIVED_HEADER                         0x01 //!< Frame header without coding violation received. 
#define XL_FR_WAKEUP_RECEIVED_WUP                            0x02 //!< Wakeup pattern on the configured wakeup channel received.
#define XL_FR_WAKEUP_COLLISION_HEADER                        0x03 //!< Detected collision during wakeup pattern transmission received. 
#define XL_FR_WAKEUP_COLLISION_WUP                           0x04 //!< Collision during wakeup pattern transmission received.
#define XL_FR_WAKEUP_COLLISION_UNKNOWN                       0x05 //!< Set when the CC stops wakeup.
#define XL_FR_WAKEUP_TRANSMITTED                             0x06 //!< Completed the transmission of the wakeup pattern.
#define XL_FR_WAKEUP_EXTERNAL_WAKEUP                         0x07 //!< wakeup comes from external
#define XL_FR_WAKEUP_WUP_RECEIVED_WITHOUT_WUS_TX             0x10 //!< wakeupt pattern received from flexray bus
#define XL_FR_WAKEUP_RESERVED                                0xFF

// XL_FR_SYMBOL_WINDOW event: flags
#define XL_FR_SYMBOL_STATUS_SESA                             0x01 //!< Syntax Error in Symbol Window Channel A
#define XL_FR_SYMBOL_STATUS_SBSA                             0x02 //!< Slot Boundary Violation in Symbol Window Channel A 
#define XL_FR_SYMBOL_STATUS_TCSA                             0x04 //!< Transmission Conflict in Symbol Window Channel A
#define XL_FR_SYMBOL_STATUS_SESB                             0x08 //!< Syntax Error in Symbol Window Channel B
#define XL_FR_SYMBOL_STATUS_SBSB                             0x10 //!< Slot Boundary Violation in Symbol Window Channel B 
#define XL_FR_SYMBOL_STATUS_TCSB                             0x20 //!< Transmission Conflict in Symbol Window Channel B
#define XL_FR_SYMBOL_STATUS_MTSA                             0x40 //!< MTS received in Symbol Window Channel A
#define XL_FR_SYMBOL_STATUS_MTSB                             0x80 //!< MTS received in Symbol Window Channel B


#include <pshpack8.h>

#define XL_FR_RX_EVENT_HEADER_SIZE       32 
#define XL_FR_MAX_EVENT_SIZE            512  



/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structures for FlexRay events
/////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct s_xl_fr_start_cycle {
  unsigned int                          cycleCount;
  int                                   vRateCorrection;
  int                                   vOffsetCorrection;
  unsigned int                          vClockCorrectionFailed;
  unsigned int                          vAllowPassivToActive;
  unsigned int                          reserved[3]; 
} XL_FR_START_CYCLE_EV;

typedef struct s_xl_fr_rx_frame {
  unsigned short                        flags;
  unsigned short                        headerCRC; 
  unsigned short                        slotID;
  unsigned char                         cycleCount;
  unsigned char                         payloadLength; 
  unsigned char	                        data[XL_FR_MAX_DATA_LENGTH]; 
} XL_FR_RX_FRAME_EV;

typedef struct s_xl_fr_tx_frame {
  unsigned short                        flags;
  unsigned short                        slotID;
  unsigned char                         offset; 
  unsigned char	                        repetition;
  unsigned char                         payloadLength;
  unsigned char	                        txMode;
  unsigned char                         incrementSize;
  unsigned char                         incrementOffset;
  unsigned char                         reserved0;
  unsigned char                         reserved1;
  unsigned char	                        data[XL_FR_MAX_DATA_LENGTH]; 
} XL_FR_TX_FRAME_EV;

typedef struct s_xl_fr_wakeup { 
  unsigned char                         cycleCount;              //!< Actual cyclecount.
  unsigned char                         wakeupStatus;            //!< XL_FR_WAKEUP_UNDEFINED, ...
  unsigned char                         reserved[6];
} XL_FR_WAKEUP_EV;

typedef struct s_xl_fr_symbol_window {
  unsigned int                          symbol;                  //!< XL_FR_SYMBOL_MTS, ...
  unsigned int                          flags;                   //!< XL_FR_SYMBOL_STATUS_SESA, ...
  unsigned char                         cycleCount;              //!< Actual cyclecount.
  unsigned char                         reserved[7];
} XL_FR_SYMBOL_WINDOW_EV;

typedef struct s_xl_fr_status {
  unsigned int                          statusType;              //!< POC status XL_FR_STATUS_ defines like, normal, active...
  unsigned int                          reserved;
} XL_FR_STATUS_EV;

typedef struct s_xl_fr_nm_vector {
  unsigned char                         nmVector[12];
  unsigned char                         cycleCount;              //!< Actual cyclecount.
  unsigned char                         reserved[3];
} XL_FR_NM_VECTOR_EV;

typedef struct s_xl_fr_sync_pulse {
  unsigned int                          triggerSource;           //!< e.g. external or internal trigger source
  XLuint64                              time;                    //!< internal generated timestamp
} XL_FR_SYNC_PULSE_EV;

typedef struct s_xl_fr_error_poc_mode {
  unsigned char                         errorMode;               //!< error mode like: active, passive, comm_halt
  unsigned char                         reserved[3];
} XL_FR_ERROR_POC_MODE_EV;

typedef struct s_xl_fr_error_sync_frames {
  unsigned short                        evenSyncFramesA;         //!< valid RX/TX sync frames on frCh A for even cycles
  unsigned short                        oddSyncFramesA;          //!< valid RX/TX sync frames on frCh A for odd cycles
  unsigned short                        evenSyncFramesB;         //!< valid RX/TX sync frames on frCh B for even cycles
  unsigned short                        oddSyncFramesB;          //!< valid RX/TX sync frames on frCh B for odd cycles
  unsigned int                          reserved;
} XL_FR_ERROR_SYNC_FRAMES_EV;

typedef struct s_xl_fr_error_clock_corr_failure {
  unsigned short                        evenSyncFramesA;         //!< valid RX/TX sync frames on frCh A for even cycles
  unsigned short                        oddSyncFramesA;          //!< valid RX/TX sync frames on frCh A for odd cycles
  unsigned short                        evenSyncFramesB;         //!< valid RX/TX sync frames on frCh B for even cycles
  unsigned short                        oddSyncFramesB;          //!< valid RX/TX sync frames on frCh B for odd cycles
  unsigned int                          flags;                   //!< missing/maximum rate/offset correction flags.   
  unsigned int                          clockCorrFailedCounter;  //!< E-Ray: CCEV register (CCFC value)
  unsigned int                          reserved;    
} XL_FR_ERROR_CLOCK_CORR_FAILURE_EV;

typedef struct s_xl_fr_error_nit_failure {
  unsigned int                          flags;                   //!< flags for NIT boundary, syntax error...
  unsigned int                          reserved;
} XL_FR_ERROR_NIT_FAILURE_EV;

typedef struct s_xl_fr_error_cc_error {
  unsigned int                          ccError;                 //!< internal CC errors (Transmit Across Boundary, Transmit Violation...)
  unsigned int                          reserved;
} XL_FR_ERROR_CC_ERROR_EV;

union s_xl_fr_error_info {
  XL_FR_ERROR_POC_MODE_EV               frPocMode;               //!< E-RAY: EIR_PEMC
  XL_FR_ERROR_SYNC_FRAMES_EV            frSyncFramesBelowMin;    //!< E-RAY: EIR_SFBM 
  XL_FR_ERROR_SYNC_FRAMES_EV            frSyncFramesOverload;    //!< E-RAY: EIR_SFO
  XL_FR_ERROR_CLOCK_CORR_FAILURE_EV     frClockCorrectionFailure;//!< E-RAY: EIR_CCF
  XL_FR_ERROR_NIT_FAILURE_EV            frNitFailure;            //!< NIT part of the E_RAY: SWNIT register
  XL_FR_ERROR_CC_ERROR_EV               frCCError;               //!< internal CC error flags (E-RAY: EIR)
};

typedef struct s_xl_fr_error {
  unsigned char                         tag;
  unsigned char                         cycleCount;
  unsigned char                         reserved[6];
  union s_xl_fr_error_info              errorInfo;
} XL_FR_ERROR_EV;

typedef struct s_xl_fr_spy_frame {
  unsigned int                          frameLength;
  unsigned char                         frameError;	             //!< XL_FR_SPY_FRAMEFLAG_XXX values
  unsigned char                         tssLength;	
  unsigned short                        headerFlags;	 
  unsigned short                        slotID;
  unsigned short                        headerCRC;
  unsigned char                         payloadLength; 
  unsigned char                         cycleCount;
  unsigned short                        reserved;
  unsigned int                          frameCRC;  
  unsigned char                         data[XL_FR_MAX_DATA_LENGTH];
} XL_FR_SPY_FRAME_EV;

typedef struct s_xl_fr_spy_symbol {
  unsigned short                        lowLength;
  unsigned short                        reserved;
 } XL_FR_SPY_SYMBOL_EV;


#define XL_NOTIFY_REASON_CHANNEL_ACTIVATION       1 
#define XL_NOTIFY_REASON_CHANNEL_DEACTIVATION     2 
#define XL_NOTIFY_REASON_PORT_CLOSED              3 

typedef struct s_xl_application_notification { 
  unsigned int 	notifyReason; 			// XL_NOTIFY_REASON_xxx 
  unsigned int	reserved[7]; 
} XL_APPLICATION_NOTIFICATION_EV; 



/////////////////////////////////////////////////////////////////////////////////////////////////////////
// rx event definition 
/////////////////////////////////////////////////////////////////////////////////////////////////////////

union s_xl_fr_tag_data {
  XL_FR_START_CYCLE_EV                  frStartCycle;
  XL_FR_RX_FRAME_EV                     frRxFrame;   
  XL_FR_TX_FRAME_EV                     frTxFrame;
  XL_FR_WAKEUP_EV                       frWakeup; 
  XL_FR_SYMBOL_WINDOW_EV                frSymbolWindow;
  XL_FR_ERROR_EV                        frError; 
  XL_FR_STATUS_EV                       frStatus;
  XL_FR_NM_VECTOR_EV                    frNmVector;  
  XL_FR_SYNC_PULSE_EV                   frSyncPulse;
  XL_FR_SPY_FRAME_EV                    frSpyFrame;
  XL_FR_SPY_SYMBOL_EV                   frSpySymbol;

  XL_APPLICATION_NOTIFICATION_EV        applicationNotification;

  unsigned char                         raw[XL_FR_MAX_EVENT_SIZE - XL_FR_RX_EVENT_HEADER_SIZE]; 
};

typedef unsigned short                  XLfrEventTag; 

struct s_xl_fr_event { 
  unsigned int                          size;             // 4 - overall size of the complete event 
  XLfrEventTag                          tag;              // 2 - type of the event 
  unsigned short                        channelIndex;     // 2 
  unsigned int                          userHandle;       // 4 
  unsigned short                        flagsChip;        // 2 - frChannel e.g. XL_FR_CHANNEL_A (lower 8 bit), queue overflow (upper 8bit)
  unsigned short                        reserved;         // 2 
  XLuint64                              timeStamp;        // 8 - raw timestamp
  XLuint64                              timeStampSync;    // 8 - timestamp which is synchronized by the driver
                                                          // --------- 
                                                          // 32 bytes -> XL_FR_RX_EVENT_HEADER_SIZE
  union s_xl_fr_tag_data                tagData; 
}; 

typedef struct s_xl_fr_event            XLfrEvent; 


#include <poppack.h>
///////////////////////////////////////////////////////////////////////////////
// Function calls
////////////////////////////////////////////////////////////////////////////////
// common functions
/*------------------------------------------------------------------------------
xlOpenDriver():
--------------------------------------------------------------------------------
The Application calls this function to get access to the driver.
*/

#ifdef DYNAMIC_XLDRIVER_DLL
  // in case of dynamic loading the application defines this function
  typedef XLstatus (_XL_EXPORT_API *XLOPENDRIVER) (void);
#else
  XLstatus _XL_EXPORT_DECL xlOpenDriver(void);
#endif

/*------------------------------------------------------------------------------
xlCloseDriver ():
--------------------------------------------------------------------------------
The driver is closed.
This is used to unload the driver, if no more application is useing it.
Does not close the open ports !!!
*/

#ifdef DYNAMIC_XLDRIVER_DLL
  typedef XLstatus (_XL_EXPORT_API *XLCLOSEDRIVER) (void);
#else
  XLstatus _XL_EXPORT_DECL xlCloseDriver(void);
#endif

/*------------------------------------------------------------------------------
xlSetApplConfig():
xlGetApplConfig():
--------------------------------------------------------------------------------
Handle the application configuration for VCANCONF.EXE
*/

/*
Returns the hwIndex, hwChannel and hwType for a specific Application and application channel.
This gives the ability to register own applications into the Vector
CAN DRIVER CONFIGURATION.
AppName: Zero terminated string containing the Name of the Application.
AppChannel: Channel of the application
hwType, hwIndex, hwChannel: contains the the hardware information on success.
This values can be used in a subsequent call to xlGetChannelMask or xlGetChannelIndex.
*/

DECL_STDXL_FUNC(  xlGetApplConfig, XLGETAPPLCONFIG, (
                  char            *appName,        //<! Name of Application
                  unsigned int     appChannel,     //<! 0,1
                  unsigned int    *pHwType,        //<! HWTYPE_xxxx
                  unsigned int    *pHwIndex,       //<! Index of the hardware (slot) (0,1,...)
                  unsigned int    *pHwChannel,     //<! Index of the channel (connector) (0,1,...)
                  unsigned int     busType         //<! Bus type of configuration, should be BUS_TYPE_NONE when no bus type is set
                ));

DECL_STDXL_FUNC(  xlSetApplConfig, XLSETAPPLCONFIG, (
                  char            *appName,        //<! Name of Application
                  unsigned int     appChannel,     //<! 0,1
                  unsigned int     hwType,         //<! HWTYPE_xxxx
                  unsigned int     hwIndex,        //<! Index of the hardware (slot) (0,1,...)
                  unsigned int     hwChannel,      //<! Index of the channel (connector) (0,1,...)
                  unsigned int     busType         //<! Bus type of configuration, should be BUS_TYPE_NONE when no bus type is set
    ));

/*------------------------------------------------------------------------------
xlGetDriverConfig():
--------------------------------------------------------------------------------
The application gets the information, which
channels are available in the system. The user
must provide the memory (pointer to XLdriverConfig structure).
*/

DECL_STDXL_FUNC( xlGetDriverConfig, XLGETDRIVERCONFIG, (XLdriverConfig *pDriverConfig));

/*------------------------------------------------------------------------------
xlGetChannelIndex():
xlGetChannelMask():
--------------------------------------------------------------------------------

Get the channel index for a channel of a certain hardware.
Parameter -1 means "don't care"
Result -1 (xlGetChannelIndex) or 0 (xlGetChannelMask) means "not found"
*/

#ifdef DYNAMIC_XLDRIVER_DLL
  typedef int (_XL_EXPORT_API *XLGETCHANNELINDEX) (
    int hwType,     // [-1,HWTYPE_CANCARDX,HWTYPE_VIRTUAL,...]
    int hwIndex,    // [-1,0,1]
    int hwChannel   // [-1,0,1]
  );

  typedef XLaccess (_XL_EXPORT_API *XLGETCHANNELMASK) (
    int hwType,     // [-1,HWTYPE_CANCARDX,HWTYPE_VIRTUAL,...]
    int hwIndex,    // [-1,0,1]
    int hwChannel   // [-1,0,1]
  );
#else
  int _XL_EXPORT_DECL xlGetChannelIndex(
    int hwType,     // [-1,HWTYPE_CANCARDX,HWTYPE_VIRTUAL,...]
    int hwIndex,    // [-1,0,1]
    int hwChannel   // [-1,0,1]
  );

  XLaccess _XL_EXPORT_DECL xlGetChannelMask(
    int hwType,     // [-1,HWTYPE_CANCARDX,HWTYPE_VIRTUAL,...]
    int hwIndex,    // [-1,0,1]
    int hwChannel   // [-1,0,1]
  );
#endif


/*------------------------------------------------------------------------------
xlOpenPort():
--------------------------------------------------------------------------------
The application tells the driver to which channels
it wants to get access to and which of these channels
it wants to get the permission to initialize the channel (on input must be
in variable where pPermissionMask points).
Only one port can get the permission to initialize a channel.
The port handle and permitted init access is returned.
*/

DECL_STDXL_FUNC( xlOpenPort, XLOPENPORT, (   
                XLportHandle   *pPortHandle,
                char           *userName,
                XLaccess       accessMask,
                XLaccess       *pPermissionMask,
                unsigned int   rxQueueSize,
                unsigned int   xlInterfaceVersion,
                unsigned int   busType)
                );

/*------------------------------------------------------------------------------
xlSetTimerRate():
--------------------------------------------------------------------------------
The timer of the port will be activated/deacticated and the
rate for cyclic timer events is set.
The resolution of the parameter 'timerRate' is 10us.
The accepted values for this parameter are 100, 200, 300,... resulting
in an effective timerrate of 1000us, 2000us, 3000us,...
*/

DECL_STDXL_FUNC ( xlSetTimerRate, XLSETTIMERRATE, (
                  XLportHandle   portHandle,
                  unsigned long  timerRate)
                  );

/*------------------------------------------------------------------------------
xlSetTimerRateAndChannel():
--------------------------------------------------------------------------------
This function sets the timerrate for timerbased-notify feature using out from
the specified channels the one which is best suitable for exact timestamps.
If only one channel is specified, this channel is used for timer event generation.
Only channels that are assigned to the port specified by parameter portHandle may be used. 
  Parameter timerRate specifies the requested timer event's cyclic rate; passed back is
the timer rate used by the driver. The resolution of this parameter is 10us.
A value timerRate=0 would disable timer events.
  Returns in parameter timerChannelMask the channel that is best suitable 
for timer event generation out of the channels specified by paramter 
timerChannelMask. The timer rate value may be below 1ms, but is limited to the following
discrete values (with 'x' as unsigned decimal value):
CAN/LIN     : 250 us ... (250 us + x * 250 us)
Flexray(USB): 250 us ... (250 us + x *  50 us)
Flexray(PCI): 100 us ... (100 us + x *  50 us)

Example: timerRate=25  ==> Used timerrate would be 250us.
*/

DECL_STDXL_FUNC ( xlSetTimerRateAndChannel, XLSETTIMERRATEANDCHANNEL, (
                  XLportHandle   portHandle, 
                  XLaccess       *timerChannelMask, 
                  unsigned long  *timerRate)
                  );


/*------------------------------------------------------------------------------
xlResetClock():
--------------------------------------------------------------------------------
The clock generating timestamps for the port will be reset
*/

DECL_STDXL_FUNC ( xlResetClock, XLRESETCLOCK, (XLportHandle portHandle));

/*------------------------------------------------------------------------------
xlSetNotification():
--------------------------------------------------------------------------------
Setup a event to notify the application if there are messages in the
ports receive queue.
queueLevel specifies the number of messages that triggeres the event.
Note that the event is triggered only once, when the queueLevel is
reached. An application should read all available messages by xlReceive
to be sure to reenable the event. The API generates the handle by
itself. For LIN the queueLevel is fix to one.
*/

DECL_STDXL_FUNC ( xlSetNotification, XLSETNOTIFICATION, (
                  XLportHandle  portHandle,
                  XLhandle      *handle,
                  int           queueLevel)
                );


/*------------------------------------------------------------------------------
xlSetTimerBasedNotifiy():
--------------------------------------------------------------------------------
Setup a event to notify the application based on the timerrate which can
be set by xlSetTimerRate()/xlSetTimerRateAndChannel().
*/

DECL_STDXL_FUNC ( xlSetTimerBasedNotify, XLSETTIMERBASEDNOTIFY, (
                  XLportHandle portHandle, 
                  XLhandle     *pHandle)
                  );

/*------------------------------------------------------------------------------
xlFlushReceiveQueue():
--------------------------------------------------------------------------------
The receive queue of the port will be flushed.
*/

DECL_STDXL_FUNC ( xlFlushReceiveQueue, XLFLUSHRECEIVEQUEUE, (XLportHandle portHandle));

/*------------------------------------------------------------------------------
xlGetReceiveQueueLevel():
--------------------------------------------------------------------------------
The count of events in the receive queue of the port will be returned.
*/

DECL_STDXL_FUNC ( xlGetReceiveQueueLevel, XLGETRECEIVEQUEUELEVEL, (
                  XLportHandle portHandle,
                  int       *level)
                );

/*------------------------------------------------------------------------------
xlActivateChannel():
--------------------------------------------------------------------------------
The selected channels go 'on the bus'. Type of the bus is specified by busType parameter.
Additional parameters can be specified by flags parameter.
*/

DECL_STDXL_FUNC ( xlActivateChannel, XLACTIVATECHANNEL, (
                  XLportHandle  portHandle, 
                  XLaccess      accessMask, 
                  unsigned int  busType, 
                  unsigned int  flags)
                  );


/*------------------------------------------------------------------------------
xlReceive():
--------------------------------------------------------------------------------
The driver is asked to retrieve burst of Events from the
application's receive queue. This function is optimized
for speed. pEventCount on start must contain size of the buffer in
messages, on return it sets number of realy received messages (messages
written to pEvents buffer).
Application must allocate pEvents buffer big enough to hold number of
messages requested by pEventCount parameter.
It returns VERR_QUEUE_IS_EMPTY and *pEventCount=0 if no event
was received.
The function only works for CAN, LIN, DAIO. For MOST there is a different
function
*/

DECL_STDXL_FUNC ( xlReceive, XLRECEIVE, (
                  XLportHandle  portHandle,
                  unsigned int  *pEventCount,
                  XLevent       *pEvents)
                  );

/*------------------------------------------------------------------------------
xlGetErrorString():
xlGetEventString(): 
--------------------------------------------------------------------------------
Utility Functions
*/
#ifdef DYNAMIC_XLDRIVER_DLL
  typedef XLstringType (_XL_EXPORT_API *XLGETERRORSTRING) ( XLstatus err );
  typedef XLstringType (_XL_EXPORT_API *XLGETEVENTSTRING)( XLevent *pEv );
#else
  XLstringType _XL_EXPORT_DECL xlGetErrorString( XLstatus err );
  XLstringType _XL_EXPORT_DECL xlGetEventString( XLevent *pEv );
#endif

/*------------------------------------------------------------------------------
xlOemContact():                                              
--------------------------------------------------------------------------------
*/

DECL_STDXL_FUNC ( xlOemContact, XLOEMCONTACT, (XLportHandle portHandle, unsigned long Channel, XLuint64 context1, XLuint64 *context2));
                  
/*------------------------------------------------------------------------------
xlGetSyncTime():
--------------------------------------------------------------------------------
Function is reading high precision (1ns) card time used for time synchronization
of Party Line trigger (sync line).
*/

DECL_STDXL_FUNC ( xlGetSyncTime,      XLGETSYNCTIME, (
                  XLportHandle        portHandle, 
                  XLuint64            *pTime )
    );

/*------------------------------------------------------------------------------
xlGenerateSyncPulse():
--------------------------------------------------------------------------------
Activates short sync pulse on desired channel. Channels mask should not
define two channels of one hardware.

*/

DECL_STDXL_FUNC ( xlGenerateSyncPulse, XLGENERATESYNCPULSE, (
                  XLportHandle portHandle,
                  XLaccess    accessMask)
                 );

/*------------------------------------------------------------------------------
xlPopupHwConfig():
--------------------------------------------------------------------------------
*/

DECL_STDXL_FUNC ( xlPopupHwConfig, XLPOPUPHWCONFIG, (
                  char *callSign, 
                  unsigned int waitForFinish)
                  );

/*------------------------------------------------------------------------------
xlDeactivateChannel():
--------------------------------------------------------------------------------
The selected channels go 'off the bus'.
Its now possible to initialize
*/

DECL_STDXL_FUNC ( xlDeactivateChannel, XLDEACTIVATECHANNEL, (
                  XLportHandle portHandle,
                  XLaccess    accessMask)
                 );

/*------------------------------------------------------------------------------
xlClosePort():
--------------------------------------------------------------------------------
The port is closed, channels are deactivated.
*/

DECL_STDXL_FUNC ( xlClosePort, XLCLOSEPORT, (
                  XLportHandle portHandle)
                 );

////////////////////////////////////////////////////////////////////////////////
// CAN functions
////////////////////////////////////////////////////////////////////////////////
/*------------------------------------------------------------------------------
xlCanFlushTransmitQueue():
// TODO: fr MOST nutzen
--------------------------------------------------------------------------------
The transmit queue of the selected channel will be flushed.
*/

DECL_STDXL_FUNC ( xlCanFlushTransmitQueue, XLCANFLUSHTRANSMITQUEUE, (
                  XLportHandle portHandle, XLaccess    accessMask)
                  );
                 

/*------------------------------------------------------------------------------
xlCanSetChannelOutput():
--------------------------------------------------------------------------------
The output mode for the CAN chips of the channels defined by accessMask, is set
to OUTPUT_MODE_NORMAL or OUTPUT_MODE_SILENT.
The port must have init access to the channels.
*/

DECL_STDXL_FUNC ( xlCanSetChannelOutput, XLCANSETCHANNELOUTPUT,  (
                  XLportHandle portHandle,
                  XLaccess   accessMask,
                  int        mode)
                  );


/*------------------------------------------------------------------------------
xlCanSetChannelMode():
--------------------------------------------------------------------------------
For the CAN channels defined by AccessMask is set
whether the caller will get a TX and/or a TXRQ
receipt for transmitted messages.
The port must have init access to the channels.
*/

DECL_STDXL_FUNC ( xlCanSetChannelMode, XLCANSETCHANNELMODE, (
                  XLportHandle    portHandle,
                  XLaccess        accessMask,
                  int             tx,
                  int             txrq)
                 );

/*------------------------------------------------------------------------------
xlCanSetReceiveMode():
--------------------------------------------------------------------------------
*/

DECL_STDXL_FUNC (xlCanSetReceiveMode, XLCANSETRECEIVEMODE, (
                 XLportHandle    Port,        // Port Handle
                 unsigned char   ErrorFrame,  // suppress Error Frames
                 unsigned char   ChipState    // suppress Chip States
                 )
                 );


/*------------------------------------------------------------------------------*/
/** xlCanSetChannelTransceiver():
 *\brief The transceiver mode is set for all channels defined by accessMask.
 *  The port must have init access to the channels.
 *  \param portHandle     [IN] handle to port from which the information is requested
 *  \param accessMask     [IN] mask specifying the port's channel from where to get the device state
 *  \param type           [IN] Transceiver type (specified by defines 'XL_TRANSCEIVER_TYPE_*')
 *  \param lineMode       [IN] Transceiver operation mode (specified by defines 'XL_TRANSCEIVER_LINEMODE_*')
 *  \param resNet         [IN] Reserved. Should always be set to zero!
 */

DECL_STDXL_FUNC ( xlCanSetChannelTransceiver, XLCANSETCHANNELTRANSCEIVER, (
                  XLportHandle  portHandle,
                  XLaccess      accessMask,
                  int           type,
                  int           lineMode,
                  int           resNet)
                  );

/*------------------------------------------------------------------------------
xlCanSetChannelParams():
xlCanSetChannelParamsC200():
xlCanSetChannelBitrate():
--------------------------------------------------------------------------------
The channels defined by accessMask will be initialized with the
given parameters.
The port must have init access to the channels.
*/                

DECL_STDXL_FUNC ( xlCanSetChannelParams, XLCANSETCHANNELPARAMS, (
                  XLportHandle  portHandle,
                  XLaccess      accessMask,
                  XLchipParams* pChipParams)
                  );

DECL_STDXL_FUNC ( xlCanSetChannelParamsC200, XLCANSETCHANNELPARAMSC200, (
                  XLportHandle  portHandle,
                  XLaccess      accessMask,
                  unsigned char btr0,
                  unsigned char btr1)
                  );

DECL_STDXL_FUNC ( xlCanSetChannelBitrate, XLCANSETCHANNELBITRATE, (
                  XLportHandle  portHandle,
                  XLaccess      accessMask,
                  unsigned long bitrate)
                  );

/*------------------------------------------------------------------------------
xlCanSetAcceptance():
--------------------------------------------------------------------------------
Set the acceptance filter
Filters for std and ext ids are handled independant in the driver.
Use mask=0xFFFF,code=0xFFFF or mask=0xFFFFFFFF,code=0xFFFFFFFF to fully close
the filter.
*/     

DECL_STDXL_FUNC ( xlCanSetChannelAcceptance, XLCANSETCHANNELACCEPTANCE, (
                  XLportHandle portHandle,
                  XLaccess        accessMask,
                  unsigned long   code, 
                  unsigned long   mask,
                  unsigned int    idRange)
                 );

/*------------------------------------------------------------------------------
xlCanAddAcceptanceRange():
xlCanRemoveAcceptanceRange():
xlCanResetAcceptance():
--------------------------------------------------------------------------------
*/

DECL_STDXL_FUNC ( xlCanAddAcceptanceRange,    XLCANADDACCEPTANCERANGE, (
                  XLportHandle    portHandle,
                  XLaccess        accessMask,
                  unsigned long   first_id,
                  unsigned long   last_id)
                  );
DECL_STDXL_FUNC ( xlCanRemoveAcceptanceRange, XLCANREMOVEACCEPTANCERANGE, (
                  XLportHandle    portHandle,
                  XLaccess        accessMask,
                  unsigned long   first_id,
                  unsigned long   last_id)
                  );
DECL_STDXL_FUNC ( xlCanResetAcceptance,       XLCANRESETACCEPTANCE, (
                  XLportHandle     portHandle,
                  XLaccess        accessMask,
                  unsigned int    idRange);
                  );

/*------------------------------------------------------------------------------
xlCanRequestChipState():
--------------------------------------------------------------------------------
The state of the selected channels is requested.
The answer will be received as an event (XL_CHIP_STATE).
*/

DECL_STDXL_FUNC ( xlCanRequestChipState, XLCANREQUESTCHIPSTATE, (
                  XLportHandle portHandle,
                  XLaccess    accessMask)
                );

/*------------------------------------------------------------------------------
xlCanTransmit():                                                                            
--------------------------------------------------------------------------------
This function is designed to send different messages to supported bus.
Usualy pEvents is a pointer to XLevent array. pEvents points to variable
which contains information about how many messages should be transmitted
to desired channels. It must be less or same as pEventCount buffer size
in messages. On return function writes number of transmitted messages
(moved to device queue for transmitting). 

*/

DECL_STDXL_FUNC ( xlCanTransmit, XLCANTRANSMIT, (
                  XLportHandle  portHandle,
                  XLaccess      accessMask,
                  unsigned int  *pEventCount,
                  void          *pEvents)
                  );


/*------------------------------------------------------------------------------
xlSetGlobalTimeSync():
--------------------------------------------------------------------------------
To query and change the globale time sync setting 
*/
DECL_STDXL_FUNC ( xlSetGlobalTimeSync, XLSETGLOBALTIMESYNC, (unsigned long newValue, 
                                                             unsigned long *previousValue));


/*------------------------------------------------------------------------------
xlCheckLicense():
--------------------------------------------------------------------------------
For all channels the port wants to use it is checked wether
the hardware is licensed for the type of application.
If not the application should terminate.
*/

DECL_STDXL_FUNC ( xlCheckLicense, XLCHECKLICENSE, (
                  XLportHandle    portHandle,
                  XLaccess        accessMask,
                  unsigned long   protectionCode)
                  );


/********************************************************************/
/** xlGetLicenseInfo()
 *\brief Function to get available licenses from Vector devices.
 *  This function returns the available licenses in an array of XLlicenseInfo structures. This array contains all available licenses on
 *  the queried channels. The position inside the array is defined by the license itself, e.g. the license for
 *  the Advanced-Flexray-Library is always at the same array index.
 *  \param channelMask      [IN] : Channelmask for which to query the available licenses
 *  \param *pLicInfoArray   [OUT]: Array with license overview
 *  \param licInfoArraySize [IN] : Size of array pointed to with 'pLicInfo' (number of array entries)
 *  \return XLstatus            General status information is returned.
 *                              XL_SUCCESS if no error occured.
 *                              XL_ERR_NO_RESOURCES if the given array size is too small to copy all available licenses into it.
 *                              XL_ERROR if general error occured.
*/
DECL_STDXL_FUNC ( xlGetLicenseInfo, XLGETLICENSEINFO, (
                  XLaccess      channelMask,
                  XLlicenseInfo *pLicInfoArray,
                  unsigned int  licInfoArraySize)
                  );


////////////////////////////////////////////////////////////////////////////////
// LIN functions
////////////////////////////////////////////////////////////////////////////////
 
DECL_STDXL_FUNC( xlLinSetChannelParams, XLLINSETCHANNELPARAMS,  (XLportHandle portHandle, XLaccess accessMask, XLlinStatPar vStatPar));
DECL_STDXL_FUNC( xlLinSetDLC,           XLLINSETDLC,            (XLportHandle portHandle, XLaccess accessMask, unsigned char dlc[64]));
DECL_STDXL_FUNC( xlLinSetSlave,         XLLINSETSLAVE,          (XLportHandle portHandle, XLaccess accessMask, unsigned char linId, unsigned char data[8], unsigned char dlc, unsigned short checksum));
DECL_STDXL_FUNC( xlLinSendRequest,      XLLINSENDREQUEST,       (XLportHandle portHandle, XLaccess accessMask, unsigned char linId, unsigned int flags));
DECL_STDXL_FUNC( xlLinSetSleepMode,     XLLINSETSLEEPMODE,      (XLportHandle portHandle, XLaccess accessMask, unsigned int flags, unsigned char linId));
DECL_STDXL_FUNC( xlLinWakeUp,           XLLINWAKEUP,            (XLportHandle portHandle, XLaccess accessMask));
DECL_STDXL_FUNC( xlLinSetChecksum,      XLLINSETCHECKSUM,       (XLportHandle portHandle, XLaccess accessMask, unsigned char checksum[60]));
DECL_STDXL_FUNC( xlLinSwitchSlave,      XLLINSWITCHSLAVE,       (XLportHandle portHandle, XLaccess accessMask, unsigned char linID, unsigned char mode));

////////////////////////////////////////////////////////////////////////////////
// DAIO Function Declarations
////////////////////////////////////////////////////////////////////////////////

DECL_STDXL_FUNC (xlDAIOSetPWMOutput             , XLDAIOSETPWMOUTPUT,             (XLportHandle portHandle, XLaccess accessMask, unsigned int frequency,   unsigned int value)); 
DECL_STDXL_FUNC (xlDAIOSetDigitalOutput         , XLDAIOSETDIGITALOUTPUT,         (XLportHandle portHandle, XLaccess accessMask, unsigned int outputMask,  unsigned int valuePattern));
DECL_STDXL_FUNC (xlDAIOSetAnalogOutput          , XLDAIOSETANALOGOUTPUT,          (XLportHandle portHandle, XLaccess accessMask, unsigned int analogLine1,  unsigned int analogLine2,  unsigned int analogLine3,  unsigned int analogLine4));
DECL_STDXL_FUNC (xlDAIORequestMeasurement       , XLDAIOREQUESTMEASUREMENT,       (XLportHandle portHandle, XLaccess accessMask));
DECL_STDXL_FUNC (xlDAIOSetDigitalParameters     , XLDAIOSETDIGITALPARAMETERS,     (XLportHandle portHandle, XLaccess accessMask, unsigned int inputMask,   unsigned int outputMask));
DECL_STDXL_FUNC (xlDAIOSetAnalogParameters      , XLDAIOSETANALOGPARAMETERS,      (XLportHandle portHandle, XLaccess accessMask, unsigned int inputMask,   unsigned int outputMask,   unsigned int highRangeMask));
DECL_STDXL_FUNC (xlDAIOSetAnalogTrigger         , XLDAIOSETANALOGTRIGGER,         (XLportHandle portHandle, XLaccess accessMask, unsigned int triggerMask, unsigned int triggerLevel, unsigned int triggerEventMode));
DECL_STDXL_FUNC (xlDAIOSetMeasurementFrequency  , XLDAIOSETMEASUREMENTFREQUENCY,  (XLportHandle portHandle, XLaccess accessMask, unsigned int measurementInterval)); 
DECL_STDXL_FUNC (xlDAIOSetDigitalTrigger        , XLDAIOSETDIGITALTRIGGER,        (XLportHandle portHandle, XLaccess accessMask, unsigned int triggerMask));

////////////////////////////////////////////////////////////////////////////////
// extern declaration for dynamically linking... for functions without the macro
////////////////////////////////////////////////////////////////////////////////

#ifdef DYNAMIC_XLDRIVER_DLL
#  ifndef DO_NOT_DEFINE_EXTERN_DECLARATION

    XLstatus xlOpenDriver(void);
    XLstatus xlCloseDriver(void);

    extern XLGETCHANNELINDEX          xlGetChannelIndex;
    extern XLGETCHANNELMASK           xlGetChannelMask;
    
    extern XLGETEVENTSTRING           xlGetEventString;
    extern XLGETERRORSTRING           xlGetErrorString;
    
#  endif
#endif

////////////////////////////////////////////////////////////////////////////////
// MOST Function Declarations
////////////////////////////////////////////////////////////////////////////////

//  apiname, apitype, parameters in paranthesis

/**
 *  Common principles:
 *    If not mentioned otherwise, all APIs are asynchronous and will trigger an action in VN2600.
 *    Results are delivered in events which can be fetched by xlMostReceive.
 *    
 *  Common Parameters: DEFPARAMS
 *    XLportHandle portHandle:             was previously fetched by xlOpenPort API.
 *    XLaccess accessMask:                 determines on which channels an API should work.
 *    XLuserHandle userHandle:             used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously, e.g. MPR changed then the userHandle == 0
 *  Common Return Value:
 *    XLstatus:                            common return value of most APIs which indicates whether a command was 
 *                                         successfully launched or e.g. whether a command queue overflow happend
 */

/** \brief fetching events from driver queue.
 *  This method is used to fetch events, either bus events or acknowledgements 
 *  for commands from the driver queue. Each call delivers only one event (if an event is available). \n
 *  It is a synchronous mode and either delivers event data immediately, or
 *  indicates an error condition with its return value.
 *  \param  XLportHandle portHandle:  [IN] was previously fetched by xlOpenPort API (TODO)
 *  \param  XLaccess accessMask:      [IN] determines on which channels an API should work (TODO)
 *  \param  pEventBuffer              [IN] This parameter must point to a buffer to which the driver can copy
 *                                         the next event of the receive queue
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostReceive,            XLFP_MOSTRECEIVE,           (XLportHandle portHandle, XLmostEvent* pEventBuffer));

/** \brief Activates or deactivates the different event sources of VN2600.
 *  This method is used to select which bus events should be delivered by VN2600.
 *  Either CtrlNode, CtrlSpy, AsyncNode or AsyncSpy messages \n
 *  ResponseEvent:                         XL_MOST_EVENTSOURCES
 *  \param sourceMask                 [IN] each bit stands for an event source and can separately be set. 
 *                                         Use the definitions of the sourcemask...
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostSwitchEventSources, XLFP_MOSTSWITCHEVENTSOURCES,(DEFPARAMS, unsigned short sourceMask));

/** \brief Activates or deactivates the bypass of the OS8104.
 *  This method is used to switch the Bypass OS8104 (register TODO:) on and off \n
 *  ResponseEvent:                         XL_MOST_ALLBYPASS
 *  \param  bypassMode                [IN] bypass open/close
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostSetAllBypass,       XLFP_MOSTSETALLBYPASS,      (DEFPARAMS, unsigned char bypassMode));

/** \brief Reads out the bypass mode of the OS8104.
 *  This method is asynchronous and requests the event used to switch the Bypass OS8104.
 *  ResponseEvent:                         XL_MOST_ALLBYPASS
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostGetAllBypass,       XLFP_MOSTGETALLBYPASS,      (DEFPARAMS));

/** \brief Switches the OS8104 into slave or master mode.
 *  This method is used to switch the OS8104 into the timing master or slave mode\n
 *  ResponseEvent:                         XL_MOST_TIMINGMODE
 *  \param  timingMode                [IN] MOST master/slave
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostSetTimingMode,      XLFP_MOSTSETTIMINGMODE,     (DEFPARAMS, unsigned char timingMode));

/** \brief Triggers the event XL_MOST_TIMINGMODE.
 *  This method is used to trigger the event XL_MOST_TIMINGMODE, which will deliver
 *  information whether the OS8104 is configured in slave or master mode.\n
 *  ResponseEvent:                         XL_MOST_TIMINGMODE
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostGetTimingMode,      XLFP_MOSTGETTIMINGMODE,     (DEFPARAMS));

/** \brief Selects the MOST frequency either to 44.1 kHz or 48 kHz.
 *  This method is used to select either 44.1 kHz or 48 kHz as 
 *  bus clock when the OS8104 of VN2600 acts as timing master \n
 *  ResponseEvent:                         XL_MOST_FREQUENCY
 *  \param  frequency                 [IN] 44,1kHz, 48kHz
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostSetFrequency,       XLFP_MOSTSETFREQUENCY,      (DEFPARAMS, unsigned short frequency));

/** \brief Triggers the event XL_MOST_FREQUENCY.
 *  This method is used to trigger the event XL_MOST_FREQUENCY, which will deliver
 *  information whether the OS8104 of VN2600 as timing master 
 *  generates 44.1 kHz or 48 kHz as bus clock.\n
 *  ResponseEvent:                         XL_MOST_FREQUENCY
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostGetFrequency,       XLFP_MOSTGETFREQUENCY,      (DEFPARAMS));

/** \brief Allows to write up to 16 byte register in the OS8104.
 *  This method is used to write numbyte (up to 16) bytes into the registers of the OS8104 
 *  beginning from adr. \n
 *  ResponseEvent:                         XL_MOST_REGISTER_BYTES
 *  \param  adr                       [IN] address (MAP) of register to which the first byte is written
 *  \param  numBytes                  [IN] number of successive bytes to be written to the registers
 *  \param  data                      [IN] bytes to be written 
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostWriteRegister,      XLFP_MOSTWRITEREGISTER,     (DEFPARAMS, unsigned short adr, unsigned char numBytes, unsigned char data[16]));

/** \brief Triggers the event XL_MOST_REGISTER_BYTES.
 *  This method is used to read out registers of the OS8104. 
 *  The results will be delivered in the event XL_MOST_REGISTER_BYTES\n
 *  ResponseEvent:                         XL_MOST_REGISTER_BYTES
 *  \param  adr                       [IN] address (MAP) of register from which the first byte is read
 *  \param  numBytes                  [IN] number of successive bytes to be read
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostReadRegister,       XLFP_MOSTREADREGISTER,      (DEFPARAMS, unsigned short adr, unsigned char numBytes));

/** \brief Allows to write single or multiple bits of one byte register in the OS8104.
 *  This method is used to write bits into a register of the OS8104 \n
 *  ResponseEvent:                         XL_MOST_REGISTER_BYTES
 *  \param  adr                       [IN] address (MAP) of the register
 *  \param  mask                      [IN] each bit in mask corresponds to a bit in the register. 
 *                                         1 means this bit will be written, 0 means that the bit is not influenced
 *  \param  value                     [IN] the byte to be written respecting the parameter mask
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostWriteRegisterBit,   XLFP_MOSTWRITEREGISTERBIT,  (DEFPARAMS, unsigned short adr, unsigned char mask, unsigned char value));

/** \brief Sending a MOST Ctrl Message.
 *  This method is used to send a ctrl message to the MOST ring. 
 *  The members ctrlType, targetAdr, ctrlData[17], TODO: prio of pCtrlMsg will be used,
 *  all other members don't care for the transmit request. 
 *  A XL_MOST_CTRL_MSG event will be delivered with dir==Tx and txStatus set to 
 *  report success or failure of the transmission.\n
 *  ResponseEvent:                         XL_MOST_CTRL_MSG
 *  \param  pCtrlMsg                  [IN] structure with all relevant data needed for a transmit request
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostCtrlTransmit,       XLFP_MOSTCTRLTRANSMIT,      (DEFPARAMS, XLmostCtrlMsg* pCtrlMsg));

/** \brief Sending a MOST Async Message (Packet).
 *  This method is used to send an asynchronous message (packet) to the MOST ring. 
 *  The members arbitration, targetAdr, asyncData[1014], length, TODO: prio of pAsyncMsg will be used,
 *  all other members don't care for the transmit request. 
 *  TODO: arbitration has to be calculated by the sender or will be calculated by the driver/firmware?
 *  A XL_MOST_ASYNC_MSG event will be delivered with dir==Tx and txStatus set to 
 *  report success or failure of the transmission.\n
 *  ResponseEvent:                         XL_MOST_ASYNC_MSG
 *  \param  pAsyncMsg                 [IN] structure with all relevant data needed for a transmit request
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostAsyncTransmit,      XLFP_MOSTASYNCTRANSMIT,     (DEFPARAMS, XLmostAsyncMsg* pAsyncMsg));

/** \brief Triggers the event XL_MOST_SYNC_ALLOCTABLE.
 *  This method is used to trigger the event XL_MOST_SYNC_ALLOCTABLE,
 *  which delivers the complete allocation table of the OS8104.\n
 *  ResponseEvent:                         XL_MOST_SYNC_ALLOCTABLE
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostSyncGetAllocTable,  XLFP_MOSTSYNCGETALLOCTABLE, (DEFPARAMS));

/** \brief Programming the routing egine (RE) for audio channels.
 *  This method is used to program the routing engine (RE) of the OS8104 in order
 *  to either stream audio data from the line in of VN2600 to certain MOST channels allocated before, 
 *  or to stream audio data from certain MOST channels to the headphone output of VN2600. \n
 *  ResponseEvent:                         XL_MOST_CTRL_SYNC_AUDIO
 *  \param  channel[4]                [IN] channel numbers to be routed
 *  \param  device                    [IN] device, e.g.: audio line in/audio line out
 *  \param  mode                      [IN] audio mode
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostCtrlSyncAudio,      XLFP_MOSTCTRLSYNCAUDIO,     (DEFPARAMS, unsigned int channel[4], unsigned int device, unsigned int mode));

/** \brief Programming the routing egine (RE) for audio channels.
 *  This method is used to program the routing engine (RE) of the OS8104 in order
 *  to either stream audio data from the line in of VN2600 to certain MOST channels allocated before, 
 *  or to stream audio data from certain MOST channels to the headphone output of VN2600. \n
 *  ResponseEvent:                         XL_MOST_CTRL_SYNC_AUDIO_EX
 *  \param  channel[16]               [IN] channel numbers to be routed (including SPDIF)
 *  \param  device                    [IN] device, e.g.: audio line in/audio line out, SPDIF in/out
 *  \param  mode                      [IN] audio mode
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostCtrlSyncAudioEx,      XLFP_MOSTCTRLSYNCAUDIOEX, (DEFPARAMS, unsigned int channel[16], unsigned int device, unsigned int mode));

/** \brief Setting the volume/attenuation for line in and line out.
 *  This method is used to set the volume/attenuation of the line in or line out of VN2600.\n
 *  ResponseEvent:                         XL_MOST_SYNC_VOLUME_STATUS
 *  \param  device                    [IN] device, e.g.: audio line in/audio line out
 *  \param  volume                    [IN] 0..255: 0..100% of volume
 *  \return XLstatus general status information
*/
DECL_STDXL_FUNC( xlMostSyncVolume,         XLFP_MOSTSYNCVOLUME,        (DEFPARAMS, unsigned int device, unsigned char volume));

/** \brief Setting mute for line in and line out.
 *  This method is used to switch mute on or off for the line in or line out of VN2600.\n
 *  ResponseEvent:                         XL_MOST_SYNC_VOLUME_STATUS
 *  \param  device                    [IN] device, e.g.: audio line in/audio line out  
 *  \param  mute                      [IN] mute on/mute off
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostSyncMute,           XLFP_MOSTSYNCMUTE,          (DEFPARAMS, unsigned int device, unsigned char mute));

/** \brief Triggers the event XL_MOST_SYNC_VOLUME_STATUS.
 *  This method is used to trigger the event XL_MOST_SYNC_VOLUME_STATUS,
 *  which delivers the information about volume status of line in and line out.\n
 *  ResponseEvent:                         XL_MOST_SYNC_VOLUME_STATUS
 *  \param  device                    [IN] device, e.g.: audio line in/audio line out  
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostSyncGetVolumeStatus,XLFP_MOSTSYNCGETVOLUMESTATUS,(DEFPARAMS, unsigned int device));

/** \brief Triggers the event XL_MOST_SYNC_MUTE_STATUS.
 *  This method is used to trigger the event XL_MOST_SYNC_MUTE_STATUS,
 *  which delivers the information about mute status of line in and line out.\n
 *  ResponseEvent:                         XL_MOST_SYNC_MUTE_STATUS
 *  \param  device                    [IN] device, e.g.: audio line in/audio line out    
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostSyncGetMuteStatus,  XLFP_MOSTSYNCGETMUTESTATUS,(DEFPARAMS, unsigned int device));

/** \brief Triggers the event XL_MOST_SYNC_MUTE_STATUS.
 *  This method delivers the recent light status at the Rx Pin of the OS8104.\n
 *  ResponseEvent:                         XL_MOST_SYNC_MUTE_STATUS
 *  \param  device                    [IN] device, e.g.: audio line in/audio line out    
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostGetRxLight,         XLFP_MOSTGETRXLIGHT,        (DEFPARAMS));

/** \brief Switching the Tx light of VN2600.
 *  This method is used to switch the Tx light of VN2600 off, to normal or to constant on\n
 *  ResponseEvent:                         XL_MOST_TXLIGHT
 *  \param  txLight                   [IN] tx light on, off or modulated
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostSetTxLight,         XLFP_MOSTSETTXLIGHT,        (DEFPARAMS, unsigned char txLight));

/** \brief Triggers the event XL_MOST_TXLIGHT.
 *  This method is used to trigger the event XL_MOST_TXLIGHT,
 *  which delivers the recent light status at the Tx Pin of the OS8104.\n
 *  ResponseEvent:                         XL_MOST_TXLIGHT
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostGetTxLight,         XLFP_MOSTGETTXLIGHT,        (DEFPARAMS));

/** \brief Switching the Tx light power of the FOT.
 *  This method is used to switch the Tx light power of the FOT to normal or -3 dB\n
 *  ResponseEvent:                         XL_MOST_TXLIGHT
 *  \param  attenuation               [IN] tx power
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostSetLightPower,      XLFP_MOSTSETLIGHTPOWER,     (DEFPARAMS, unsigned char attenuation));

// TODO: GetLightPower??

/** \brief Triggers the event XL_MOST_LOCKSTATUS.
 *  This method is used to trigger the event XL_MOST_LOCKSTATUS,
 *  which delivers the recent lock status at the Rx Pin of the OS8104.\n
 *  ResponseEvent:                         XL_MOST_LOCKSTATUS
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostGetLockStatus,      XLFP_MOSTGETLOCKSTATUS,     (DEFPARAMS));

/** \brief Starts and stops the light error generator.
 *  This method is used to start (repeat>0) or stop (repeat==0) the light error generator
 *  which switches the Tx light on and off or configured periods.\n
 *  ResponseEvent:                         XL_MOST_GENLIGHTERROR
 *  \param  lightOffTime              [IN] duration of light off in ms
 *  \param  lightOnTime               [IN] duration of modulated light on in ms
 *  \param  repeat                    [IN] repetition of light on light off sequence, or repeat==0: stop the generation
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostGenerateLightError, XLFP_MOSTGENERATELIGHTERROR,(DEFPARAMS, unsigned long lightOffTime, unsigned long lightOnTime, unsigned short repeat));

/** \brief Starts and stops the lock error generator.
 *  This method is used to start (repeat>0) or stop (repeat==0) the lock error generator
 *  which switches the Tx light between modulated on and permanent on for configured periods.\n
 *  ResponseEvent:                         XL_MOST_GENLOCKERROR 
 *  \param  unmodTime                 [IN] duration of light off in ms
 *  \param  modTime                   [IN] duration of modulated light on in ms
 *  \param  repeat                    [IN] repetition of sequence, or repeat==0: stop the generation
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostGenerateLockError,  XLFP_MOSTGENERATELOCKERROR, (DEFPARAMS, unsigned long unmodTime, unsigned long modTime, unsigned short repeat));

/** \brief prevent firmware from emptying the Rx buffer of the OS8104
 *  This method is used to Switch the stress mode on or off, where the 
 *  Rx buffer of the OS8104 is not emptied by the firmware
 *  which switches the Tx light between modulated on and permanent on for configured periods.\n
 *  ResponseEvent:                         XL_MOST_CTRL_RXBUFFER
 *  \param  bufferMode                [IN] specifies the buffer mode
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostCtrlRxBuffer,       XLFP_MOSTCTRLRXBUFFER,      (DEFPARAMS, unsigned short bufferMode));

/** \brief Twinkle the power led from the VN2600.
 *  ResponseEvent:                         none
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMostTwinklePowerLed,       XLFP_MOSTTWINKLEPOWERLED,      (DEFPARAMS)); 

/** \brief Prepares and configures busload generation with MOST control frames. 
 *  Attention: Has to be called before "xlMostCtrlGenerateBusload". 
 *  ResponseEvent:                         none 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param pCtrlBusloadConfiguration  [IN] structure containg the ctrl msg used for busload generation and configuration,  
 *                                         it's storage has has to be supplied by the caller 
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMostCtrlConfigureBusload, XLFP_MOSTCTRLCONFIGUREBUSLOAD, (DEFPARAMS, 
                                                                             XLmostCtrlBusloadConfiguration* pCtrlBusloadConfiguration)); 

/** \brief Starts busload generation with MOST control frames. 
 *  Attention: "xlMostCtrlConfigureBusload" has to be called before. 
 *  ResponseEvent:                         XL_MOST_CTRL_BUSLOAD 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param numberCtrlFrames           [IN] number of busload ctrl messages (0xFFFFFFFF indicates infinite number of msgs) 
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMostCtrlGenerateBusload, XLFP_MOSTCTRLGENERATEBUSLOAD, (DEFPARAMS, unsigned long numberCtrlFrames)); 

/** \brief Prepares and configures busload generation of MOST asynchronous frames. 
 *  Attention: Has to be called before "xlMostAsyncGenerateBusload". 
 *  ResponseEvent:                         none 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param pAsyncBusloadConfiguration [IN] structure containg the async msg used for busload generation and configuration,  
 *                                         it's storage has has to be supplied by the caller 
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMostAsyncConfigureBusload, XLFP_MOSTASYNCCONFIGUREBUSLOAD, (DEFPARAMS, 
                                                                               XLmostAsyncBusloadConfiguration* pAsyncBusloadConfiguration)); 

/** \brief Starts busload generation with MOST asynchronous frames. 
 *  Attention: "xlMostAsyncConfigureBusload" has to be called before. 
 *  ResponseEvent:                         XL_MOST_ASYNC_BUSLOAD 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param numberAsyncFrames          [IN] number of busload async messages (0xFFFFFFFF indicates infinite number of msgs) 
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMostAsyncGenerateBusload, XLFP_MOSTASYNCGENERATEBUSLOAD, (DEFPARAMS, unsigned long numberAsyncFrames)); 


/** \brief Opens a stream (Rx / Tx) for routing synchronous data to or from the MOST bus (synchronous channel). 
 *  Attention: Has to be called before "xlMostStreamBufferAllocate". 
 *  ResponseEvent:                         none 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param pStreamOpen                [IN] structure containg the stream parameters - 
 *                                         it's storage has has to be supplied by the caller 
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMostStreamOpen,           XLFP_MOSTSTREAMOPEN,           (DEFPARAMS, XLmostStreamOpen* pStreamOpen)); 


/** \brief Closes an opened a stream (Rx / Tx) used for routing synchronous data to or from the MOST bus (synchronous channel). 
 *  ResponseEvent:                         none 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param streamHandle               [IN] stream handle returned by xlMostStreamOpen
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMostStreamClose,          XLFP_MOSTSTREAMCLOSE,           (DEFPARAMS, unsigned int streamHandle)); 


/** \brief Starts the streaming (Rx / Tx) of synchronous data to or from the MOST bus (synchronous channel). 
 *  Attention: Has to be called after "xlMostStreamOpen and xlMostStreamBufferAllocate" were called. 
 *  ResponseEvent:                         none 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param streamHandle               [IN] stream handle returned by xlMostStreamOpen
 *  \param syncChannels               [IN] synchronous channels (bytes) used for streaming.
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMostStreamStart,          XLFP_MOSTSTREAMSTART,          (DEFPARAMS, unsigned int streamHandle, unsigned char syncChannels[60])); 


/** \brief Stops the streaming (Rx / Tx) of synchronous data to or from the MOST bus (synchronous channel). 
 *  Attention: Has to be called before "xlMostStreamBufferDeallocate". 
 *  ResponseEvent:                         none 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param streamHandle               [IN] stream handle returned by xlMostStreamOpen
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMostStreamStop,           XLFP_MOSTSTREAMSTOP,           (DEFPARAMS, unsigned int streamHandle)); 


/** \brief Allocates a buffer for streaming (RX / Tx) of synchronous data to or from the MOST bus (synchronous channel). 
 *  Attention: Has to be called before "xlMostStreamStart". 
 *  ResponseEvent:                         none 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param streamHandle               [IN] stream handle returned by xlMostStreamOpen
 *  \param ppBuffer                  [OUT] pointer to the buffer used for streaming
 *                                         memory allocation is done by the driver 
 *                                         has to be realeased by calling xlMostStreamBufferDeallocate
 *  \param pBufferSize               [OUT] buffer size.
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMostStreamBufferAllocate, XLFP_MOSTSTREAMBUFFERALLOCATE, (DEFPARAMS, unsigned int streamHandle, unsigned char** ppBuffer, unsigned int* pBufferSize)); 


/** \brief Deallocates any buffer allocated with "xlMostStreamBufferAllocate". 
 *  Attention: Has to be called before "xlMostStreamClose". Afterwards no buffer must be accessed!
 *  ResponseEvent:                         none 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param streamHandle               [IN] stream handle returned by xlMostStreamOpen
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMostStreamBufferDeallocateAll, XLFP_MOSTSTREAMBUFFERDEALLOCATEALL, (DEFPARAMS, unsigned int streamHandle)); 


/** \brief Notifies the driver the next buffer to be used for streaming synchronous data to or from the MOST bus (synchronous channel). 
 *  ResponseEvent:                         none 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param streamHandle               [IN] stream handle returned by xlMostStreamOpen
 *  \param pBuffer                    [IN] pointer to the next buffer used for streaming
 *  \param filledBytes                [IN] size of  the next buffer to be used for streaming
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMostStreamBufferSetNext, XLFP_MOSTSTREAMBUFFERSETNEXT, (DEFPARAMS, unsigned int streamHandle, unsigned char* pBuffer, unsigned int filledBytes)); 


/** \brief Retrieves the stream information.
 *  This method is used to gather the recent stream state information.\n
 *  ResponseEvent:                         None 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param pStreamInfo               [OUT] Pointer to the stream information.
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMostStreamGetInfo, XLFP_MOSTSTREAMGETINFO, (DEFPARAMS, XLmostStreamInfo* pStreamInfo)); 


/** \brief Clears the content of the buffer(s) which are not already sent.
 *  This method is used to clear the content of any TX streaming buffer which has not been sent yet.\n
 *  ResponseEvent:                         None 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param streamHandle               [IN] stream handle returned by xlMostStreamOpen.
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMostStreamBufferClearAll, XLFP_MOSTSTREAMBUFFERCLEARALL, (DEFPARAMS, unsigned int streamHandle)); 


////////////////////////////////////////////////////////////////////////////////
// FlexRay Function Declarations
////////////////////////////////////////////////////////////////////////////////

/** \brief Setup the FlexRay node
 *  ResponseEvent:                         
 *  \param portHandle                 [IN] handle specifying the port to be configured 
 *  \param accessMask                 [IN] mask specifying the port's channel 
 *  \param pxlClusterConfig           [IN] structure to the cluster config structure
 *  \return XLstatus                       general status information 
 */ 
 
DECL_STDXL_FUNC (xlFrSetConfiguration, XLFP_FRSETCONFIGURATION, (DEFFRPARAM, XLfrClusterConfig	*pxlClusterConfig));

/** \brief Get configuration of a FlexRay channel 
 *  ResponseEvent:                         
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the information
 *  \param XLfrChannelConfig         [OUT] pointer to the FlexRay channel configuration structure 
 *  \return XLstatus                       general status information 
 */ 
 
DECL_STDXL_FUNC (xlFrGetChannelConfiguration, XLFP_FRGETCHANNELCONFIGURATION, (DEFFRPARAM, XLfrChannelConfig* pxlFrChannelConfig));

/** \brief Setup the FlexRay mode
 *  ResponseEvent:                         
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param pxlFrMode                  [IN] structure to the FlexRay mode structure (e.g.: normal-, monitor-, clusterScan mode).
 *  \return XLstatus                       general status information 
 */ 

DECL_STDXL_FUNC (xlFrSetMode, XLFP_FRSETMODE, (DEFFRPARAM, XLfrMode	*pxlFrMode));

/** \brief Initialize the cold start and define the sync event
 *  ResponseEvent:                         
 *  \param portHandle                 [IN] handle to port from which the information is requested
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state   
 *  \param pEventBuffer               [IN] pointer to the startup and sync frame
 *  \return XLstatus                       general status information 
 */ 

DECL_STDXL_FUNC (xlFrInitStartupAndSync, XLFP_FRINITSTARTUPANDSYNC, (DEFFRPARAM, XLfrEvent *pEventBuffer));

/** \brief setup the symbol window. 
 *  ResponseEvent:                         
 *  \param portHandle                 [IN] handle to port from which the information is requested
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state   
 *  \param frChannel                  [IN] FlexRay channel, like A,B, both...
 *  \param symbolWindowMask           [IN] symbol window mask like MTS.
 *  \return XLstatus                       general status information 
 */ 

DECL_STDXL_FUNC (xlFrSetupSymbolWindow, XLFP_FRSETUPSYMBOLWINDOW, (DEFFRPARAM, unsigned int frChannel, 
                                                                               unsigned int symbolWindowMask));
 
/** \brief Reads the FlexRay events
 *  ResponseEvent:                         
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param pEventBuffer              [OUT] pointer to the FlexRay RX event
 *  \return XLstatus                       general status information 
 */ 

DECL_STDXL_FUNC (xlFrReceive, XLFP_FRRECEIVE, (XLportHandle portHandle, XLfrEvent *pEventBuffer)); 

/** \brief Transmit a FlexRay event
 *  ResponseEvent:                         
 *  \param portHandle                 [IN] handle to port from which the information is requested
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state   
 *  \param pEventBuffer               [IN] pointer to the FlexRay TX event
 *  \return XLstatus                       general status information 
 */ 

DECL_STDXL_FUNC (xlFrTransmit, XLFP_FRTRANSMIT, (DEFFRPARAM, XLfrEvent *pEventBuffer));

/** \brief 
 *  ResponseEvent:                         
 *  \param portHandle                 [IN] handle to port from which the information is requested
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state   
 *  \param frChannel                  [IN] FlexRay channel. e.g. CHANNEL_A...
 *  \param mode                       [IN] transceiver mode. e.g. sleep
 *  \return XLstatus                       general status information 
 */ 

DECL_STDXL_FUNC (xlFrSetTransceiverMode, XLFP_FRSETTRANSCEIVERMODE, (DEFFRPARAM, unsigned int frChannel, unsigned int mode));

/** \brief 
 *  ResponseEvent:                         
 *  \param portHandle                 [IN] handle to port from which the information is requested
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param symbolWindow               [IN] defines the symbol window (e.g. MTS).
 *  \return XLstatus                       general status information 
 */ 

DECL_STDXL_FUNC (xlFrSendSymbolWindow, XLFP_FRSENDSYMBOLWINDOW, (DEFFRPARAM, unsigned int symbolWindow));

/** \brief 
 *  ResponseEvent:                         
 *  \param portHandle                 [IN] handle to port from which the information is requested
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state   
 *  \param mode                       [IN] specifies the spy mode: XL_FR_SPY_MODE_***
 *  \return XLstatus                       general status information 
 */ 

DECL_STDXL_FUNC (xlFrActivateSpy, XLFP_FRACTIVATESPY, (DEFFRPARAM, unsigned int mode));


#if (__BORLANDC__ >= 0x0560)
#pragma pack(pop)
#else
#include <poppack.h>
#endif

#ifdef __cplusplus
}
#endif   // _cplusplus

#endif // _V_XLAPI_H_        




