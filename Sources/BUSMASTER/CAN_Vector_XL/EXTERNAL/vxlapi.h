/*----------------------------------------------------------------------------
| File:
|   vxlapi.h
| Project:
|   Multi Bus driver for Windows 7 / Windows 8 / Windows 10
|
| Description:
|   Driver Interface Prototypes - customer version
|
|-----------------------------------------------------------------------------
| $Author: visstz $    $Date: 2016-06-13 15:31:06 +0200 (Mo, 13 Jun 2016) $   $Revision: 58894 $
| $Id: vxlpubbase.h 58894 2016-06-13 13:31:06Z visstz $
|-----------------------------------------------------------------------------
| Copyright (c) 2016 by Vector Informatik GmbH.  All rights reserved.
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


// Bus types
#define XL_BUS_TYPE_NONE            0x00000000
#define XL_BUS_TYPE_CAN             0x00000001
#define XL_BUS_TYPE_LIN             0x00000002
#define XL_BUS_TYPE_FLEXRAY         0x00000004
#define XL_BUS_TYPE_AFDX            0x00000008 // former BUS_TYPE_BEAN
#define XL_BUS_TYPE_MOST            0x00000010
#define XL_BUS_TYPE_DAIO            0x00000040 // IO cab/piggy
#define XL_BUS_TYPE_J1708           0x00000100
#define XL_BUS_TYPE_ETHERNET        0x00001000
#define XL_BUS_TYPE_A429            0x00002000

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
#define XL_TRANSCEIVER_TYPE_CAN_VIRTUAL          0x0016  //!< Virtual CAN Trasceiver for Virtual CAN Bus Driver
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
#define XL_TRANSCEIVER_TYPE_CAN_1051_CAP_FIX     0x013C //!< TJA1051 - fixed transceiver on e.g. 16xx/8970
#define XL_TRANSCEIVER_TYPE_DAIO_1021_FIX        0x013D //!< Onboard IO of VN1630/VN1640 
#define XL_TRANSCEIVER_TYPE_LIN_7269_CAP_FIX     0x013E //!< TLE7269 - fixed transceiver on 1611
#define XL_TRANSCEIVER_TYPE_PB_CAN_1051_CAP      0x013F //!< TJA 1051, capacitive isolated
#define XL_TRANSCEIVER_TYPE_PB_CAN_SWC_7356_CAP  0x0140 //!< Single Wire NCV7356, capacitive isolated
#define XL_TRANSCEIVER_TYPE_PB_CAN_1055_CAP      0x0141 //!< TJA1055, capacitive isolated, with selectable termination resistor (via 4th IO line) 
#define XL_TRANSCEIVER_TYPE_PB_CAN_1057_CAP      0x0142 //!< TJA 1057, capacitive isolated
#define XL_TRANSCEIVER_TYPE_A429_HOLT8596_FIX    0x0143 //!< Onboard HOLT 8596 TX transceiver on VN0601
#define XL_TRANSCEIVER_TYPE_A429_HOLT8455_FIX    0x0144 //!< Onboard HOLT 8455 RX transceiver on VN0601


// FlexRay PiggyBacks
#define XL_TRANSCEIVER_TYPE_PB_FR_1080           0x0201 //!< TJA 1080
#define XL_TRANSCEIVER_TYPE_PB_FR_1080_MAG       0x0202 //!< TJA 1080 magnetically isolated piggy
#define XL_TRANSCEIVER_TYPE_PB_FR_1080A_MAG      0x0203 //!< TJA 1080A magnetically isolated piggy
#define XL_TRANSCEIVER_TYPE_PB_FR_1082_CAP       0x0204 //!< TJA 1082 capacitive isolated piggy
#define XL_TRANSCEIVER_TYPE_PB_FRC_1082_CAP      0x0205 //!< TJA 1082 capacitive isolated piggy with CANpiggy form factor
#define XL_TRANSCEIVER_TYPE_FR_1082_CAP_FIX      0x0206 //!< TJA 1082 capacitive isolated piggy fixed transceiver - e.g. 7610

#define XL_TRANSCEIVER_TYPE_MOST150_ONBOARD      0x0220 //!< Onboard MOST150 transceiver of VN2640

// Ethernet Phys
#define XL_TRANSCEIVER_TYPE_ETH_BCM54810_FIX        0x0230 //!< Onboard Broadcom Ethernet PHY on VN5610 and VX0312
#define XL_TRANSCEIVER_TYPE_ETH_AR8031_FIX          0x0231 //!< Onboard Atheros Ethernet PHY
#define XL_TRANSCEIVER_TYPE_ETH_BCM89810_FIX        0x0232 //!< Onboard Broadcom Ethernet PHY
#define XL_TRANSCEIVER_TYPE_ETH_TJA1100_FIX         0x0233 //!< Onboard NXP Ethernet PHY
#define XL_TRANSCEIVER_TYPE_ETH_BCM54810_89811_FIX  0x0234 //!< Onboard Broadcom Ethernet PHYs (e.g. VN5610A - BCM54810: RJ45, BCM89811: DSUB)

// IOpiggy 8642
#define XL_TRANSCEIVER_TYPE_PB_DAIO_8642            0x0280 //!< Iopiggy for VN8900
#define XL_TRANSCEIVER_TYPE_DAIO_AL_ONLY            0x028f //!< virtual piggy type for activation line only (e.g. VN8810ini)
#define XL_TRANSCEIVER_TYPE_DAIO_1021_FIX_WITH_AL   0x0290 //!< On board IO with Activation Line (e.g. VN5640) 
#define XL_TRANSCEIVER_TYPE_DAIO_AL_WU              0x0291 //!< virtual piggy type for activation line and WakeUp Line only (e.g. VN5610A)


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
// Transceiver Status Flags
//------------------------------------------------------------------------------
// (not all used, but for compatibility reasons)
#define XL_TRANSCEIVER_STATUS_PRESENT            ((unsigned int)0x0001)
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
#define XL_ERR_CMD_HANDLING            122 //=0x007A
#define XL_ERR_HW_NOT_PRESENT          129 //=0x0081
#define XL_ERR_NOTIFY_ALREADY_ACTIVE   131 //=0x0083
#define XL_ERR_INVALID_TAG             132 //=0x0084
#define XL_ERR_INVALID_RESERVED_FLD    133 //=0x0085
#define XL_ERR_INVALID_SIZE            134 //=0x0086
#define XL_ERR_INSUFFICIENT_BUFFER     135 //=0x0087
#define XL_ERR_ERROR_CRC               136 //=0x0088
#define XL_ERR_BAD_EXE_FORMAT          137 //=0x0089
#define XL_ERR_NO_SYSTEM_RESOURCES     138 //=0x008A
#define XL_ERR_NOT_FOUND               139 //=0x008B
#define XL_ERR_INVALID_ADDRESS         140 //=0x008C
#define XL_ERR_REQ_NOT_ACCEP           141 //=0x008D
#define XL_ERR_INVALID_LEVEL           142 //=0x008E
#define XL_ERR_NO_DATA_DETECTED        143 //=0x008F
#define XL_ERR_INTERNAL_ERROR          144 //=0x0090
#define XL_ERR_UNEXP_NET_ERR           145 //=0x0091
#define XL_ERR_INVALID_USER_BUFFER     146 //=0x0092
#define XL_ERR_NO_RESOURCES            152 //=0x0098
#define XL_ERR_WRONG_CHIP_TYPE         153 //=0x0099
#define XL_ERR_WRONG_COMMAND           154 //=0x009A
#define XL_ERR_INVALID_HANDLE          155 //=0x009B
#define XL_ERR_RESERVED_NOT_ZERO       157 //=0x009D
#define XL_ERR_INIT_ACCESS_MISSING     158 //=0x009E
#define XL_ERR_CANNOT_OPEN_DRIVER      201 //=0x00C9
#define XL_ERR_WRONG_BUS_TYPE          202 //=0x00CA
#define XL_ERR_DLL_NOT_FOUND           203 //=0x00CB
#define XL_ERR_INVALID_CHANNEL_MASK    204 //=0x00CC
#define XL_ERR_NOT_SUPPORTED           205 //=0x00CD
// special stream defines
#define XL_ERR_CONNECTION_BROKEN       210 //=0x00D2
#define XL_ERR_CONNECTION_CLOSED       211 //=0x00D3
#define XL_ERR_INVALID_STREAM_NAME     212 //=0x00D4
#define XL_ERR_CONNECTION_FAILED       213 //=0x00D5
#define XL_ERR_STREAM_NOT_FOUND        214 //=0x00D6
#define XL_ERR_STREAM_NOT_CONNECTED    215 //=0x00D7
#define XL_ERR_QUEUE_OVERRUN           216 //=0x00D8
#define XL_ERROR                       255 //=0x00FF

#define XL_ERR_INVALID_DLC                          0x0201    // DLC with invalid value
#define XL_ERR_INVALID_CANID                        0x0202    // CAN Id has invalid bits set
#define XL_ERR_INVALID_FDFLAG_MODE20                0x0203    // flag set that must not be set when configured for CAN20 (e.g. EDL)
#define XL_ERR_EDL_RTR                              0x0204    // RTR must not be set in combination with EDL
#define XL_ERR_EDL_NOT_SET                          0x0205    // EDL is not set but BRS and/or ESICTRL is
#define XL_ERR_UNKNOWN_FLAG                         0x0206    // unknown bit in flags field is set
///////////////////////////////////////////////////////////////////////////////
// Ethernet API error code (range: 0x1100..0x11FF)
#define XL_ERR_ETH_PHY_ACTIVATION_FAILED            0x1100
#define XL_ERR_ETH_MAC_RESET_FAILED                 0x1101
#define XL_ERR_ETH_MAC_NOT_READY                    0x1102
#define XL_ERR_ETH_PHY_CONFIG_ABORTED               0x1103
#define XL_ERR_ETH_RESET_FAILED                     0x1104
#define XL_ERR_ETH_SET_CONFIG_DELAYED               0x1105  //Requested config was stored but could not be immediately activated
#define XL_ERR_ETH_UNSUPPORTED_FEATURE              0x1106  //Requested feature/function not supported by device
#define XL_ERR_ETH_MAC_ACTIVATION_FAILED            0x1107
#define XL_ERR_ETH_FILTER_INVALID                   0x1108  //Requested filter setting is invalid
#define XL_ERR_ETH_FILTER_UNAVAILABLE               0x1109  //Filtering is not available (not supported by hardware, in use by other application, ...)
#define XL_ERR_ETH_FILTER_NO_INIT_ACCESS            0x110A  //Missing init access for one or more channels referenced by filter
#define XL_ERR_ETH_FILTER_TOO_COMPLEX               0x110B  //Specified filter setting is too complex to be fully supported



enum e_XLevent_type {
  XL_NO_COMMAND               =  0,
  XL_RECEIVE_MSG              =  1,
  XL_CHIP_STATE               =  4,
  XL_TRANSCEIVER              =  6,
  XL_TIMER                    =  8,
  XL_TRANSMIT_MSG             = 10,
  XL_SYNC_PULSE               = 11,
  XL_APPLICATION_NOTIFICATION = 15,


  //for LIN we have special events
  XL_LIN_MSG                  = 20,
  XL_LIN_ERRMSG               = 21,
  XL_LIN_SYNCERR              = 22,
  XL_LIN_NOANS                = 23,
  XL_LIN_WAKEUP               = 24,
  XL_LIN_SLEEP                = 25,
  XL_LIN_CRCINFO              = 26,

  // for D/A IO bus 
  XL_RECEIVE_DAIO_DATA        = 32,                 //!< D/A IO data message

  XL_RECEIVE_DAIO_PIGGY       = 34,                 //!< D/A IO Piggy data message
};




//
// common event tags 
// 
#define XL_RECEIVE_MSG                          ((unsigned short)0x0001)
#define XL_CHIP_STATE                           ((unsigned short)0x0004)
#define XL_TRANSCEIVER_INFO                     ((unsigned short)0x0006)
#define XL_TRANSCEIVER                             (XL_TRANSCEIVER_INFO)
#define XL_TIMER_EVENT                          ((unsigned short)0x0008)
#define XL_TIMER                                        (XL_TIMER_EVENT)
#define XL_TRANSMIT_MSG                         ((unsigned short)0x000A)
#define XL_SYNC_PULSE                           ((unsigned short)0x000B)
#define XL_APPLICATION_NOTIFICATION             ((unsigned short)0x000F)  

//
// LIN event tags 
// 
#define LIN_MSG                                 ((unsigned short)0x0014)
#define LIN_ERRMSG                              ((unsigned short)0x0015)
#define LIN_SYNCERR                             ((unsigned short)0x0016)
#define LIN_NOANS                               ((unsigned short)0x0017)
#define LIN_WAKEUP                              ((unsigned short)0x0018)
#define LIN_SLEEP                               ((unsigned short)0x0019)
#define LIN_CRCINFO                             ((unsigned short)0x001A)

//
// DAIO event tags 
// 
#define RECEIVE_DAIO_DATA                       ((unsigned short)0x0020)    //!< D/A IO data message




//
// FlexRay event tags 
// 
#define XL_FR_START_CYCLE                       ((unsigned short)0x0080)
#define XL_FR_RX_FRAME                          ((unsigned short)0x0081)
#define XL_FR_TX_FRAME                          ((unsigned short)0x0082)
#define XL_FR_TXACK_FRAME                       ((unsigned short)0x0083)
#define XL_FR_INVALID_FRAME                     ((unsigned short)0x0084)
#define XL_FR_WAKEUP                            ((unsigned short)0x0085)
#define XL_FR_SYMBOL_WINDOW                     ((unsigned short)0x0086)
#define XL_FR_ERROR                             ((unsigned short)0x0087)
  #define XL_FR_ERROR_POC_MODE                     ((unsigned char)0x01)
  #define XL_FR_ERROR_SYNC_FRAMES_BELOWMIN         ((unsigned char)0x02)
  #define XL_FR_ERROR_SYNC_FRAMES_OVERLOAD         ((unsigned char)0x03)
  #define XL_FR_ERROR_CLOCK_CORR_FAILURE           ((unsigned char)0x04)
  #define XL_FR_ERROR_NIT_FAILURE                  ((unsigned char)0x05)
  #define XL_FR_ERROR_CC_ERROR                     ((unsigned char)0x06)
#define XL_FR_STATUS                            ((unsigned short)0x0088)
#define XL_FR_NM_VECTOR                         ((unsigned short)0x008A)
#define XL_FR_TRANCEIVER_STATUS                 ((unsigned short)0x008B)
#define XL_FR_SPY_FRAME                         ((unsigned short)0x008E)
#define XL_FR_SPY_SYMBOL                        ((unsigned short)0x008F)


//
// CAPL-On-Board event tags 
// 


//
// MOST25 event tags 
// 
#define XL_MOST_START                                             0x0101
#define XL_MOST_STOP                                              0x0102
#define XL_MOST_EVENTSOURCES                                      0x0103
#define XL_MOST_ALLBYPASS                                         0x0107
#define XL_MOST_TIMINGMODE                                        0x0108
#define XL_MOST_FREQUENCY                                         0x0109
#define XL_MOST_REGISTER_BYTES                                    0x010a
#define XL_MOST_REGISTER_BITS                                     0x010b
#define XL_MOST_SPECIAL_REGISTER                                  0x010c
#define XL_MOST_CTRL_RX_SPY                                       0x010d
#define XL_MOST_CTRL_RX_OS8104                                    0x010e
#define XL_MOST_CTRL_TX                                           0x010f
#define XL_MOST_ASYNC_MSG                                         0x0110
#define XL_MOST_ASYNC_TX                                          0x0111
#define XL_MOST_SYNC_ALLOCTABLE                                   0x0112
#define XL_MOST_SYNC_VOLUME_STATUS                                0x0116
#define XL_MOST_RXLIGHT                                           0x0117
#define XL_MOST_TXLIGHT                                           0x0118
#define XL_MOST_LOCKSTATUS                                        0x0119
#define XL_MOST_ERROR                                             0x011a
#define XL_MOST_CTRL_RXBUFFER                                     0x011c
#define XL_MOST_SYNC_TX_UNDERFLOW                                 0x011d
#define XL_MOST_SYNC_RX_OVERFLOW                                  0x011e
#define XL_MOST_CTRL_SYNC_AUDIO                                   0x011f
#define XL_MOST_SYNC_MUTE_STATUS                                  0x0120
#define XL_MOST_GENLIGHTERROR                                     0x0121
#define XL_MOST_GENLOCKERROR                                      0x0122
#define XL_MOST_TXLIGHT_POWER                                     0x0123
#define XL_MOST_CTRL_BUSLOAD                                      0x0126
#define XL_MOST_ASYNC_BUSLOAD                                     0x0127
#define XL_MOST_CTRL_SYNC_AUDIO_EX                                0x012a
#define XL_MOST_TIMINGMODE_SPDIF                                  0x012b
#define XL_MOST_STREAM_STATE                                      0x012c
#define XL_MOST_STREAM_BUFFER                                     0x012d




//
// MOST150 event tags 
// 
#define XL_START                                ((unsigned short)0x0200)
#define XL_STOP                                 ((unsigned short)0x0201)
#define XL_MOST150_EVENT_SOURCE                 ((unsigned short)0x0203)
#define XL_MOST150_DEVICE_MODE                  ((unsigned short)0x0204)
#define XL_MOST150_SYNC_ALLOC_INFO              ((unsigned short)0x0205)
#define XL_MOST150_FREQUENCY                    ((unsigned short)0x0206)
#define XL_MOST150_SPECIAL_NODE_INFO            ((unsigned short)0x0207)
#define XL_MOST150_CTRL_RX                      ((unsigned short)0x0208)
#define XL_MOST150_CTRL_TX_ACK                  ((unsigned short)0x0209)
#define XL_MOST150_ASYNC_SPY                    ((unsigned short)0x020A)
#define XL_MOST150_ASYNC_RX                     ((unsigned short)0x020B)
#define XL_MOST150_SYNC_VOLUME_STATUS           ((unsigned short)0x020D)
#define XL_MOST150_TX_LIGHT                     ((unsigned short)0x020E)
#define XL_MOST150_RXLIGHT_LOCKSTATUS           ((unsigned short)0x020F)
#define XL_MOST150_ERROR                        ((unsigned short)0x0210)
#define XL_MOST150_CONFIGURE_RX_BUFFER          ((unsigned short)0x0211)
#define XL_MOST150_CTRL_SYNC_AUDIO              ((unsigned short)0x0212)
#define XL_MOST150_SYNC_MUTE_STATUS             ((unsigned short)0x0213)
#define XL_MOST150_LIGHT_POWER                  ((unsigned short)0x0214)
#define XL_MOST150_GEN_LIGHT_ERROR              ((unsigned short)0x0215)
#define XL_MOST150_GEN_LOCK_ERROR               ((unsigned short)0x0216)
#define XL_MOST150_CTRL_BUSLOAD                 ((unsigned short)0x0217)
#define XL_MOST150_ASYNC_BUSLOAD                ((unsigned short)0x0218)
#define XL_MOST150_ETHERNET_RX                  ((unsigned short)0x0219)
#define XL_MOST150_SYSTEMLOCK_FLAG              ((unsigned short)0x021A)
#define XL_MOST150_SHUTDOWN_FLAG                ((unsigned short)0x021B)
#define XL_MOST150_CTRL_SPY                     ((unsigned short)0x021C)
#define XL_MOST150_ASYNC_TX_ACK                 ((unsigned short)0x021D)
#define XL_MOST150_ETHERNET_SPY                 ((unsigned short)0x021E)
#define XL_MOST150_ETHERNET_TX_ACK              ((unsigned short)0x021F)
#define XL_MOST150_SPDIFMODE                    ((unsigned short)0x0220)
#define XL_MOST150_ECL_LINE_CHANGED             ((unsigned short)0x0222)
#define XL_MOST150_ECL_TERMINATION_CHANGED      ((unsigned short)0x0223)
#define XL_MOST150_NW_STARTUP                   ((unsigned short)0x0224)
#define XL_MOST150_NW_SHUTDOWN                  ((unsigned short)0x0225)
#define XL_MOST150_STREAM_STATE                 ((unsigned short)0x0226)
#define XL_MOST150_STREAM_TX_BUFFER             ((unsigned short)0x0227)
#define XL_MOST150_STREAM_RX_BUFFER             ((unsigned short)0x0228)
#define XL_MOST150_STREAM_TX_LABEL              ((unsigned short)0x0229)
#define XL_MOST150_STREAM_TX_UNDERFLOW          ((unsigned short)0x022B)
#define XL_MOST150_GEN_BYPASS_STRESS            ((unsigned short)0x022C)
#define XL_MOST150_ECL_SEQUENCE                 ((unsigned short)0x022D)
#define XL_MOST150_ECL_GLITCH_FILTER            ((unsigned short)0x022E)
#define XL_MOST150_SSO_RESULT                   ((unsigned short)0x022F)


//
// CAN/CAN-FD event tags 
// Rx
#define XL_CAN_EV_TAG_RX_OK                     ((unsigned short)0x0400)
#define XL_CAN_EV_TAG_RX_ERROR                  ((unsigned short)0x0401)
#define XL_CAN_EV_TAG_TX_ERROR                  ((unsigned short)0x0402)
#define XL_CAN_EV_TAG_TX_REQUEST                ((unsigned short)0x0403)
#define XL_CAN_EV_TAG_TX_OK                     ((unsigned short)0x0404)
#define XL_CAN_EV_TAG_CHIP_STATE                ((unsigned short)0x0409)
 
// CAN/CAN-FD event tags 
// Tx
#define XL_CAN_EV_TAG_TX_MSG                    ((unsigned short)0x0440)
//
// Ethernet event tags 
// 
#define XL_ETH_EVENT_TAG_FRAMERX                    ((unsigned short)0x0500)  //Event data type T_XL_ETH_DATAFRAME_RX
#define XL_ETH_EVENT_TAG_FRAMERX_ERROR              ((unsigned short)0x0501)  //Event data type T_XL_ETH_DATAFRAME_RX_ERROR
#define XL_ETH_EVENT_TAG_FRAMETX_ERROR              ((unsigned short)0x0506)  //Event data type T_XL_ETH_DATAFRAME_TX_ERROR
#define XL_ETH_EVENT_TAG_FRAMETX_ERROR_SWITCH       ((unsigned short)0x0507)  //Event data type T_XL_ETH_DATAFRAME_TX_ERR_SW
#define XL_ETH_EVENT_TAG_FRAMETX_ACK                ((unsigned short)0x0510)  //Event data type T_XL_ETH_DATAFRAME_TXACK
#define XL_ETH_EVENT_TAG_FRAMETX_ACK_SWITCH         ((unsigned short)0x0511)  //Event data type T_XL_ETH_DATAFRAME_TXACK_SW
#define XL_ETH_EVENT_TAG_FRAMETX_ACK_OTHER_APP      ((unsigned short)0x0513)  //Event data type T_XL_ETH_DATAFRAME_TXACK_OTHERAPP
#define XL_ETH_EVENT_TAG_FRAMETX_ERROR_OTHER_APP    ((unsigned short)0x0514)  //Event data type T_XL_ETH_DATAFRAME_TX_ERR_OTHERAPP
#define XL_ETH_EVENT_TAG_CHANNEL_STATUS             ((unsigned short)0x0520)  //Event data type T_XL_ETH_CHANNEL_STATUS
#define XL_ETH_EVENT_TAG_CONFIGRESULT               ((unsigned short)0x0530)  //Event data type T_XL_ETH_CONFIG_RESULT
#define XL_ETH_EVENT_TAG_LOSTEVENT                  ((unsigned short)0x05fe)  //Indication that one or more intended events could not be generated. Event data type T_XL_ETH_LOSTEVENT
#define XL_ETH_EVENT_TAG_ERROR                      ((unsigned short)0x05ff)  //Generic error

//
// ARINC429 event tags 
// 
#define XL_A429_EV_TAG_TX_OK                    ((unsigned short)0x0600)
#define XL_A429_EV_TAG_TX_ERR                   ((unsigned short)0x0601)
#define XL_A429_EV_TAG_RX_OK                    ((unsigned short)0x0608)
#define XL_A429_EV_TAG_RX_ERR                   ((unsigned short)0x0609)
#define XL_A429_EV_TAG_BUS_STATISTIC            ((unsigned short)0x060F)

typedef unsigned __int64 XLuint64; 

#include <pshpack8.h>


// defines for XL_APPLICATION_NOTIFICATION_EV::notifyReason 
#define XL_NOTIFY_REASON_CHANNEL_ACTIVATION                   1 
#define XL_NOTIFY_REASON_CHANNEL_DEACTIVATION                 2 
#define XL_NOTIFY_REASON_PORT_CLOSED                          3 

typedef struct s_xl_application_notification { 
  unsigned int  notifyReason;       // XL_NOTIFY_REASON_xxx 
  unsigned int  reserved[7]; 
} XL_APPLICATION_NOTIFICATION_EV; 



// defines for XL_SYNC_PULSE_EV::triggerSource and s_xl_sync_pulse::pulseCode 
#define XL_SYNC_PULSE_EXTERNAL                             0x00 
#define XL_SYNC_PULSE_OUR                                  0x01 
#define XL_SYNC_PULSE_OUR_SHARED                           0x02 

// definition of the sync pulse event for xl interface versions V3 and higher 
// (XL_INTERFACE_VERSION_V3, XL_INTERFACE_VERSION_V4, ..) 
typedef struct s_xl_sync_pulse_ev {
  unsigned int      triggerSource;              //!< e.g. external or internal trigger source
  unsigned int      reserved;
  XLuint64          time;                       //!< internally generated timestamp
} XL_SYNC_PULSE_EV;

// definition of the sync pulse event for xl interface versions V1 and V2 
// (XL_INTERFACE_VERSION_V1, XL_INTERFACE_VERSION_V2) 
#include <pshpack1.h> 
struct s_xl_sync_pulse {
  unsigned char     pulseCode;                  //!< generated by us
  XLuint64          time;                       //!< 1 ns resolution
}; 

#include <poppack.h> 




#include <poppack.h>


//------------------------------------------------------------------------------
// defines for the supported hardware
#define XL_HWTYPE_NONE                           0
#define XL_HWTYPE_VIRTUAL                        1
#define XL_HWTYPE_CANCARDX                       2
#define XL_HWTYPE_CANAC2PCI                      6
#define XL_HWTYPE_CANCARDY                      12
#define XL_HWTYPE_CANCARDXL                     15
#define XL_HWTYPE_CANCASEXL                     21
#define XL_HWTYPE_CANCASEXL_LOG_OBSOLETE        23
#define XL_HWTYPE_CANBOARDXL                    25  // CANboardXL, CANboardXL PCIe 
#define XL_HWTYPE_CANBOARDXL_PXI                27  // CANboardXL pxi 
#define XL_HWTYPE_VN2600                        29
#define XL_HWTYPE_VN2610                        XL_HWTYPE_VN2600
#define XL_HWTYPE_VN3300                        37
#define XL_HWTYPE_VN3600                        39
#define XL_HWTYPE_VN7600                        41
#define XL_HWTYPE_CANCARDXLE                    43
#define XL_HWTYPE_VN8900                        45
#define XL_HWTYPE_VN8950                        47
#define XL_HWTYPE_VN2640                        53
#define XL_HWTYPE_VN1610                        55
#define XL_HWTYPE_VN1630                        57
#define XL_HWTYPE_VN1640                        59
#define XL_HWTYPE_VN8970                        61
#define XL_HWTYPE_VN1611                        63
#define XL_HWTYPE_VN5610                        65
#define XL_HWTYPE_VN7570                        67
#define XL_HWTYPE_IPCLIENT                      69
#define XL_HWTYPE_IPSERVER                      71
#define XL_HWTYPE_VX1121                        73
#define XL_HWTYPE_VX1131                        75
#define XL_HWTYPE_VT6204                        77

#define XL_HWTYPE_VN1630_LOG                    79
#define XL_HWTYPE_VN7610                        81
#define XL_HWTYPE_VN7572                        83
#define XL_HWTYPE_VN8972                        85
#define XL_HWTYPE_VN0601                        87
#define XL_HWTYPE_VX0312                        91
#define XL_HWTYPE_VN8800                        95
#define XL_HWTYPE_IPCL8800                      96
#define XL_HWTYPE_IPSRV8800                     97
#define XL_HWTYPE_CSMCAN                        98
#define XL_HWTYPE_VN5610A                       101
#define XL_HWTYPE_VN7640                        102
#define XL_MAX_HWTYPE                           102
#include <pshpack1.h>
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
#define XL_LIN_MASTER                           (unsigned int) 01 //!< channel is a LIN master
#define XL_LIN_SLAVE                            (unsigned int) 02 //!< channel is a LIN slave
#define XL_LIN_VERSION_1_3                      (unsigned int) 0x01 //!< LIN version 1.3
#define XL_LIN_VERSION_2_0                      (unsigned int) 0x02 //!< LIN version 2.0
#define XL_LIN_VERSION_2_1                      (unsigned int) 0x03 //!< LIN version 2.1

// defines for xlLinSetSlave
#define XL_LIN_CALC_CHECKSUM                    (unsigned short) 0x100 //!< flag for automatic 'classic' checksum calculation
#define XL_LIN_CALC_CHECKSUM_ENHANCED           (unsigned short) 0x200 //!< flag for automatic 'enhanced' checksum calculation

// defines for xlLinSetSleepMode
#define XL_LIN_SET_SILENT                       (unsigned int) 0x01 //!< set hardware into sleep mode
#define XL_LIN_SET_WAKEUPID                     (unsigned int) 0x03 //!< set hardware into sleep mode and send a request at wake-up

// defines for xlLinSetChecksum. For LIN >= 2.0 there can be used two different Checksum models.
#define XL_LIN_CHECKSUM_CLASSIC                 (unsigned char) 0x00 //!< Use classic CRC 
#define XL_LIN_CHECKSUM_ENHANCED                (unsigned char) 0x01 //!< Use enhanced CRC
#define XL_LIN_CHECKSUM_UNDEFINED               (unsigned char) 0xff //!< Set the checksum calculation to undefined.

// defines for the sleep mode event: XL_LIN_SLEEP
#define XL_LIN_STAYALIVE                        (unsigned char) 0x00 //!< flag if nothing changes
#define XL_LIN_SET_SLEEPMODE                    (unsigned char) 0x01 //!< flag if the hardware is set into the sleep mode
#define XL_LIN_COMESFROM_SLEEPMODE              (unsigned char) 0x02 //!< flag if the hardware comes from the sleep mode

// defines for the wake up event: XL_LIN_WAKEUP
#define XL_LIN_WAKUP_INTERNAL                   (unsigned char) 0x01 //!< flag to signal a internal WAKEUP (event)

// defines for xlLINSetDLC
#define XL_LIN_UNDEFINED_DLC                    (unsigned char) 0xff //!< set the DLC to undefined

// defines for xlLinSwitchSlave
#define XL_LIN_SLAVE_ON                         (unsigned char) 0xff //!< switch on the LIN slave           
#define XL_LIN_SLAVE_OFF                        (unsigned char) 0x00 //!< switch off the LIN slave

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
#define XL_CAN_MSG_FLAG_OVERRUN       0x02           //!< Overrun in Driver or CAN Controller, 
                                                     //!< previous msgs have been lost.
#define XL_CAN_MSG_FLAG_NERR          0x04           //!< Line Error on Lowspeed
#define XL_CAN_MSG_FLAG_WAKEUP        0x08           //!< High Voltage Message on Single Wire CAN
#define XL_CAN_MSG_FLAG_REMOTE_FRAME  0x10
#define XL_CAN_MSG_FLAG_RESERVED_1    0x20
#define XL_CAN_MSG_FLAG_TX_COMPLETED  0x40           //!< Message Transmitted
#define XL_CAN_MSG_FLAG_TX_REQUEST    0x80           //!< Transmit Message stored into Controller
#define XL_CAN_MSG_FLAG_SRR_BIT_DOM 0x0200           //!< SRR bit in CAN message is dominant

#define XL_EVENT_FLAG_OVERRUN         0x01           //!< Used in XLevent.flags 

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

typedef struct s_xl_io_digital_data {
  unsigned int digitalInputData;
} XL_IO_DIGITAL_DATA;

typedef struct s_xl_io_analog_data {
  unsigned int measuredAnalogData0;
  unsigned int measuredAnalogData1;
  unsigned int measuredAnalogData2;
  unsigned int measuredAnalogData3;
} XL_IO_ANALOG_DATA;

struct s_xl_daio_piggy_data {  /* xx Bytes */
  unsigned int daioEvtTag;
  unsigned int triggerType;
  union {
    XL_IO_DIGITAL_DATA  digital;
    XL_IO_ANALOG_DATA   analog;
  } data;
};

//------------------------------------------------------------------------------
// structure for XL_CHIP_STATE 

#define XL_CHIPSTAT_BUSOFF              0x01
#define XL_CHIPSTAT_ERROR_PASSIVE       0x02
#define XL_CHIPSTAT_ERROR_WARNING       0x04
#define XL_CHIPSTAT_ERROR_ACTIVE        0x08


struct s_xl_chip_state {
         unsigned char busStatus;
         unsigned char txErrorCounter;
         unsigned char rxErrorCounter;
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
  unsigned char unused[3];
  unsigned int  startOffs;       // spec >= 2.0 only, else 0
  unsigned int  width;           // spec >= 2.0 only, else 0
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
        struct s_xl_can_msg                    msg;
        struct s_xl_chip_state                 chipState;
        union  s_xl_lin_msg_api                linMsgApi;    
        struct s_xl_sync_pulse                 syncPulse;
        struct s_xl_daio_data                  daioData; 
        struct s_xl_transceiver                transceiver; 
        struct s_xl_daio_piggy_data            daioPiggyData; 
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
         unsigned char        flags;           // 1 (e.g. XL_EVENT_FLAG_OVERRUN)
         unsigned char        reserved;        // 1
         XLuint64             timeStamp;       // 8
         union s_xl_tag_data  tagData;         // 32 Bytes 
       };
                                               // --------
                                               // 48 Bytes
 
typedef struct s_xl_event XLevent;                    
// message name to acquire a unique message id from windows
#define DriverNotifyMessageName  "VectorCanDriverChangeNotifyMessage"

//------------------------------------------------------------------------------
// build a channels mask from the channels index
#define XL_CHANNEL_MASK(x) (1I64<<(x))

#define XL_MAX_APPNAME          32

//------------------------------------------------------------------------------
// driver status
typedef short XLstatus;


//defines for xlGetDriverConfig structures
#define XL_MAX_LENGTH                  31
#define XL_CONFIG_MAX_CHANNELS         64

 
//activate - channel flags
#define XL_ACTIVATE_NONE                      0 
#define XL_ACTIVATE_RESET_CLOCK               8

#define XL_BUS_COMPATIBLE_CAN                XL_BUS_TYPE_CAN
#define XL_BUS_COMPATIBLE_LIN                XL_BUS_TYPE_LIN
#define XL_BUS_COMPATIBLE_FLEXRAY            XL_BUS_TYPE_FLEXRAY
#define XL_BUS_COMPATIBLE_MOST               XL_BUS_TYPE_MOST
#define XL_BUS_COMPATIBLE_DAIO               XL_BUS_TYPE_DAIO          //io cab/piggy
#define XL_BUS_COMPATIBLE_J1708              XL_BUS_TYPE_J1708
#define XL_BUS_COMPATIBLE_ETHERNET           XL_BUS_TYPE_ETHERNET
#define XL_BUS_COMPATIBLE_A429               XL_BUS_TYPE_A429

// the following bus types can be used with the current cab / piggy  
#define XL_BUS_ACTIVE_CAP_CAN                (XL_BUS_COMPATIBLE_CAN<<16)
#define XL_BUS_ACTIVE_CAP_LIN                (XL_BUS_COMPATIBLE_LIN<<16)
#define XL_BUS_ACTIVE_CAP_FLEXRAY            (XL_BUS_COMPATIBLE_FLEXRAY<<16)
#define XL_BUS_ACTIVE_CAP_MOST               (XL_BUS_COMPATIBLE_MOST<<16)
#define XL_BUS_ACTIVE_CAP_DAIO               (XL_BUS_COMPATIBLE_DAIO<<16)
#define XL_BUS_ACTIVE_CAP_J1708              (XL_BUS_COMPATIBLE_J1708<<16)
#define XL_BUS_ACTIVE_CAP_ETHERNET           (XL_BUS_COMPATIBLE_ETHERNET<<16)
#define XL_BUS_ACTIVE_CAP_A429               (XL_BUS_COMPATIBLE_A429<<16)

#define XL_BUS_NAME_NONE            ""
#define XL_BUS_NAME_CAN             "CAN"
#define XL_BUS_NAME_LIN             "LIN"
#define XL_BUS_NAME_FLEXRAY         "FlexRay"
#define XL_BUS_NAME_STREAM          "Stream"
#define XL_BUS_NAME_MOST            "MOST"
#define XL_BUS_NAME_DAIO            "DAIO"
#define XL_BUS_NAME_HWSYNC_KEYPAD   "HWSYNC_KEYPAD"
#define XL_BUS_NAME_J1708           "J1708"
#define XL_BUS_NAME_KLINE           "K-Line"
#define XL_BUS_NAME_ETHERNET        "Ethernet"
#define XL_BUS_NAME_AFDX            "AFDX"
#define XL_BUS_NAME_A429            "ARINC429"


//------------------------------------------------------------------------------
// acceptance filter                                                                      

#define XL_CAN_STD 01                                  //!< flag for standard ID's
#define XL_CAN_EXT 02                                  //!< flag for extended ID's                            

//------------------------------------------------------------------------------
// bit timing


typedef struct { 
  unsigned int  arbitrationBitRate;
  unsigned int  sjwAbr;              // CAN bus timing for nominal / arbitration bit rate
  unsigned int  tseg1Abr;
  unsigned int  tseg2Abr;
  unsigned int  dataBitRate;
  unsigned int  sjwDbr;              // CAN bus timing for data bit rate
  unsigned int  tseg1Dbr;
  unsigned int  tseg2Dbr;
  unsigned int reserved[2];         // has to be zero 
} XLcanFdConf; 


typedef struct {
          unsigned long bitRate;
          unsigned char sjw;
          unsigned char tseg1;
          unsigned char tseg2;
          unsigned char sam;  // 1 or 3
        } XLchipParams; 

// defines for XLbusParams::data::most::activeSpeedGrade and compatibleSpeedGrade
#define  XL_BUS_PARAMS_MOST_SPEED_GRADE_25    0x01
#define  XL_BUS_PARAMS_MOST_SPEED_GRADE_150   0x02


//defines for XLbusParams::data::can/canFD::canOpMode
#define  XL_BUS_PARAMS_CANOPMODE_CAN20                      0x01 //channel operates in CAN20
#define  XL_BUS_PARAMS_CANOPMODE_CANFD                      0x02 //channel operates in CANFD 

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
      unsigned char reserved[7];
      unsigned char canOpMode;
    } can;
    struct {
      unsigned int  arbitrationBitRate;     // CAN bus timing for nominal / arbitration bit rate
      unsigned char sjwAbr;
      unsigned char tseg1Abr;
      unsigned char tseg2Abr;
      unsigned char samAbr;  // 1 or 3
      unsigned char outputMode;
      unsigned char sjwDbr;                 // CAN bus timing for data bit rate 
      unsigned char tseg1Dbr;
      unsigned char tseg2Dbr;
      unsigned int  dataBitRate;
      unsigned char canOpMode;
    } canFD;
    struct {
      unsigned int  activeSpeedGrade;
      unsigned int  compatibleSpeedGrade;
      unsigned int  inicFwVersion;
    } most;
    struct {
      // status and cfg mode are part of xlFrGetChannelConfiguration, too 
      unsigned int  status;                 // XL_FR_CHANNEL_CFG_STATUS_xxx 
      unsigned int  cfgMode;                // XL_FR_CHANNEL_CFG_MODE_xxx 
      unsigned int  baudrate;               // FlexRay baudrate in kBaud 
    } flexray;
    struct {
      unsigned char macAddr[6]; // MAC address (starting with MSB!)
      unsigned char connector;  // XL_ETH_STATUS_CONNECTOR_xxx
      unsigned char phy;        // XL_ETH_STATUS_PHY_xxx
      unsigned char link;       // XL_ETH_STATUS_LINK_xxx
      unsigned char speed;      // XL_ETH_STATUS_SPEED_xxx
      unsigned char clockMode;  // XL_ETH_STATUS_CLOCK_xxx
      unsigned char bypass;     // XL_ETH_BYPASS_xxx
    } ethernet;
    struct {
      unsigned short channelDirection;
      unsigned short res1;
      union {
        struct {
          unsigned int bitrate;
          unsigned int parity;
          unsigned int minGap;
        } tx;
        struct {
          unsigned int bitrate;
          unsigned int minBitrate;
          unsigned int maxBitrate;
          unsigned int parity;
          unsigned int minGap;
          unsigned int autoBaudrate;
        } rx;
        unsigned char raw[24];
      } dir;
    } a429;
    unsigned char raw[28];
  } data;
} XLbusParams; 

// porthandle
#define XL_INVALID_PORTHANDLE (-1)
typedef long XLportHandle, *pXLportHandle;     

// defines for the connectionInfo (only for the USB devices)
#define XL_CONNECTION_INFO_USB_UNKNOWN    0
#define XL_CONNECTION_INFO_USB_FULLSPEED  1    
#define XL_CONNECTION_INFO_USB_HIGHSPEED  2
#define XL_CONNECTION_INFO_USB_SUPERSPEED 3
// defines for FPGA core types (fpgaCoreCapabilities)
#define XL_FPGA_CORE_TYPE_NONE                        0
#define XL_FPGA_CORE_TYPE_CAN                         1
#define XL_FPGA_CORE_TYPE_LIN                         2
#define XL_FPGA_CORE_TYPE_LIN_RX                      3

//#defines for specialDeviceStatus
#define XL_SPECIAL_DEVICE_STAT_FPGA_UPDATE_DONE    0x01             //!< automatic driver FPGA flashing done

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
          unsigned short      transceiverState;                     //!< transceiver state (XL_TRANSCEIVER_STATUS...)
          unsigned short      configError;                          //!< XL_CHANNEL_CONFIG_ERROR_XXX (see above)
          unsigned char       channelIndex;                         //!< Global channel index (0,1,...)
          XLuint64            channelMask;                          //!< Global channel mask (=1<<channelIndex)
          unsigned int        channelCapabilities;                  //!< capabilities which are supported (e.g CHANNEL_FLAG_XXX)
          unsigned int        channelBusCapabilities;               //!< what buses are supported and which are possible to be 
                                                                    //!< activated (e.g. XXX_BUS_ACTIVE_CAP_CAN)
                              
          // Channel          
          unsigned char       isOnBus;                              //!< The channel is on bus
          unsigned int        connectedBusType;                     //!< currently selected bus      
          XLbusParams         busParams;
          unsigned int        _doNotUse;                            //!< introduced for compatibility reasons since EM00056439
                                                        
          unsigned int        driverVersion;            
          unsigned int        interfaceVersion;                     //!< version of interface with driver
          unsigned int        raw_data[10];
                              
          unsigned int        serialNumber;
          unsigned int        articleNumber;
                              
          char                transceiverName [XL_MAX_LENGTH + 1];  //!< name for CANcab or another transceiver
                              
          unsigned int        specialCabFlags;                      //!< XL_SPECIAL_CAB_XXX flags
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

///////////////////////////////////////////////////////
// Remote Api RDNI
#define XL_IPv4 4
#define XL_IPv6 6
#define XL_MAX_REMOTE_DEVICE_INFO 16
#define XL_ALL_REMOTE_DEVICES     0xFFFFFFFF
#define XL_MAX_REMOTE_ALIAS_SIZE  64

#define XL_REMOTE_OFFLINE           1
#define XL_REMOTE_ONLINE            2
#define XL_REMOTE_BUSY              3
#define XL_REMOTE_CONNECION_REFUSED 4

#define XL_REMOTE_ADD_PERMANENT           0x0
#define XL_REMOTE_ADD_TEMPORARY           0x1

#define XL_REMOTE_REGISTER_NONE           0x0
#define XL_REMOTE_REGISTER_CONNECT        0x1
#define XL_REMOTE_REGISTER_TEMP_CONNECT   0x2

#define XL_REMOTE_DISCONNECT_NONE         0x0
#define XL_REMOTE_DISCONNECT_REMOVE_ENTRY 0x1

#define XL_REMOTE_DEVICE_AVAILABLE        0x00000001  // the device is present
#define XL_REMOTE_DEVICE_CONFIGURED       0x00000002  // the device has a configuration entry in registry
#define XL_REMOTE_DEVICE_CONNECTED        0x00000004  // the device is connected too this client
#define XL_REMOTE_DEVICE_ENABLED          0x00000008  // the driver should open a connection to this client
#define XL_REMOTE_DEVICE_BUSY             0x00000010  // the device is used by another client
#define XL_REMOTE_DEVICE_TEMP_CONFIGURED  0x00000020  // the device is temporary configured, it has not entry in registry

#define XL_REMOTE_NO_NET_SEARCH      0
#define XL_REMOTE_NET_SEARCH         1

#define XL_REMOTE_DEVICE_TYPE_UNKNOWN     0
#define XL_REMOTE_DEVICE_TYPE_VN8900      1
#define XL_REMOTE_DEVICE_TYPE_STANDARD_PC 2
#define XL_REMOTE_DEVICE_TYPE_VX          3
#define XL_REMOTE_DEVICE_TYPE_VN8800      4

typedef unsigned int XLremoteHandle;
typedef unsigned int XLdeviceAccess;
typedef unsigned int XLremoteStatus;

#include <pshpack8.h>

typedef struct s_xl_ip_address {       
    union {                     
      unsigned int v4;              
      unsigned int v6[4];
    } ip;
    unsigned int prefixLength;
    unsigned int ipVersion;
    unsigned int configPort;
    unsigned int eventPort;
} XLipAddress;

typedef struct s_xl_remote_location_config {
    char hostName[64];
    char alias[64];
    XLipAddress ipAddress;
    XLipAddress userIpAddress;
    unsigned int deviceType;
    unsigned int serialNumber;
    unsigned int articleNumber;
    XLremoteHandle remoteHandle;
} XLremoteLocationConfig;

typedef struct s_xl_remote_device {
    char deviceName[32];
    unsigned int hwType;
    unsigned int articleNumber;
    unsigned int serialNumber;
    unsigned int reserved;
} XLremoteDevice;

typedef struct s_xl_remote_device_info {
    XLremoteLocationConfig locationConfig;
    unsigned int flags;
    unsigned int reserved;
    unsigned int nbrOfDevices;
    XLremoteDevice deviceInfo[XL_MAX_REMOTE_DEVICE_INFO];
} XLremoteDeviceInfo;



#include <poppack.h>

// flags for channelCapabilities
// Time-Sync
#define XL_CHANNEL_FLAG_TIME_SYNC_RUNNING        0x00000001
#define XL_CHANNEL_FLAG_NO_HWSYNC_SUPPORT        0x00000400 //Device is not capable of hardware-based time synchronization via Sync-line
// used to distinguish between VN2600 (w/o SPDIF) and VN2610 (with S/PDIF) 
#define XL_CHANNEL_FLAG_SPDIF_CAPABLE            0x00004000 
#define XL_CHANNEL_FLAG_CANFD_BOSCH_SUPPORT      0x20000000
#define XL_CHANNEL_FLAG_CANFD_ISO_SUPPORT        0x80000000
// max. size of rx fifo for rx event in bytes 
#define RX_FIFO_MOST_QUEUE_SIZE_MAX                  1048576
#define RX_FIFO_MOST_QUEUE_SIZE_MIN                  8192


// defines for xlMostSwitchEventSources
#define XL_MOST_SOURCE_ASYNC_SPY                     0x8000
#define XL_MOST_SOURCE_ASYNC_RX                      0x1000
#define XL_MOST_SOURCE_ASYNC_TX                      0x0800
#define XL_MOST_SOURCE_CTRL_OS8104A                  0x0400
#define XL_MOST_SOURCE_CTRL_SPY                      0x0100
#define XL_MOST_SOURCE_ALLOC_TABLE                   0x0080
#define XL_MOST_SOURCE_SYNC_RC_OVER                  0x0040
#define XL_MOST_SOURCE_SYNC_TX_UNDER                 0x0020
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

typedef XL_SYNC_PULSE_EV XL_MOST_SYNC_PULSE_EV; 

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
#ifdef _MSC_VER
  unsigned char *POINTER_32 pBuffer; // 32bit LSDW of buffer pointer
#else
  unsigned int   pBuffer;            // 32bit LSDW of buffer pointer
#endif
  unsigned int   validBytes;
  unsigned int   status; // // see XL_MOST_STREAM_ERR_...
  unsigned int   pBuffer_highpart;
} XL_MOST_STREAM_BUFFER_EV;


typedef struct s_xl_most_sync_tx_underflow {
  unsigned int streamHandle;
  unsigned int reserved;
} XL_MOST_SYNC_TX_UNDERFLOW_EV;

typedef struct s_xl_most_sync_rx_overflow {
  unsigned int streamHandle;
  unsigned int reserved;
} XL_MOST_SYNC_RX_OVERFLOW_EV;

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
    XL_MOST_SYNC_MUTES_STATUS_EV       mostSyncMuteStatus;
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
    XL_MOST_SYNC_TX_UNDERFLOW_EV       mostSyncTxUnderflow;
    XL_MOST_SYNC_RX_OVERFLOW_EV        mostSyncRxOverflow;
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


#include <poppack.h>
#include <pshpack4.h>  

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// FlexRay XL API
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#define XL_FR_MAX_DATA_LENGTH                                 254


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

// defines for XLfrChannelConfig::status and XLbusParams::data::flexray::status 
#define XL_FR_CHANNEL_CFG_STATUS_INIT_APP_PRESENT   ((unsigned int) 0x01) 
#define XL_FR_CHANNEL_CFG_STATUS_CHANNEL_ACTIVATED  ((unsigned int) 0x02) 
#define XL_FR_CHANNEL_CFG_STATUS_VALID_CLUSTER_CFG  ((unsigned int) 0x04) 
#define XL_FR_CHANNEL_CFG_STATUS_VALID_CFG_MODE     ((unsigned int) 0x08) 

// defines for XLfrChannelConfig::cfgMode and XLbusParams::data::flexray::cfgMode 
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

// defines for XL_FR_SYNC_PULSE_EV::triggerSource 
#define XL_FR_SYNC_PULSE_EXTERNAL          XL_SYNC_PULSE_EXTERNAL 
#define XL_FR_SYNC_PULSE_OUR                    XL_SYNC_PULSE_OUR
#define XL_FR_SYNC_PULSE_OUR_SHARED      XL_SYNC_PULSE_OUR_SHARED 

// defines for xlFrActivateSpy, mode
#define XL_FR_SPY_MODE_ASYNCHRONOUS                          0x01
#include <poppack.h> 

#include <pshpack8.h>

// defines for xlFrSetAcceptanceFilter
//////////////////////////////////////
// filterStatus
#define XL_FR_FILTER_PASS                        0x00000000       //!< maching frame passes the filter
#define XL_FR_FILTER_BLOCK                       0x00000001       //!< maching frame is blocked

// filterTypeMask
#define XL_FR_FILTER_TYPE_DATA                   0x00000001       //!< specifies a data frame
#define XL_FR_FILTER_TYPE_NF                     0x00000002       //!< specifies a null frame in an used cycle
#define XL_FR_FILTER_TYPE_FILLUP_NF              0x00000004       //!< specifies a null frame in an unused cycle

// filterChannelMask
#define XL_FR_FILTER_CHANNEL_A                   0x00000001       //!< specifies FlexRay channel A for the PC
#define XL_FR_FILTER_CHANNEL_B                   0x00000002       //!< specifies FlexRay channel B for the PC
typedef struct  s_xl_fr_acceptance_filter {
  unsigned int  filterStatus;                                     //!< defines if the specified frame should be blocked or pass the filter
  unsigned int  filterTypeMask;                                   //!< specifies the frame type that should be filtered
  unsigned int  filterFirstSlot;                                  //!< beginning of the slot range
  unsigned int  filterLastSlot;                                   //!< end of the slot range (can be the same as filterFirstSlot)
  unsigned int  filterChannelMask;                                //!< channel A, B for PC, channel A, B for COB
} XLfrAcceptanceFilter;
#include <poppack.h> 

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
#define XL_FR_SPY_FRAMEFLAG_BUS_ERROR                 ((unsigned char)0x08)

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
#define XL_FR_PAYLOAD_INCREMENT_NONE                  ((unsigned char)   0) 

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

typedef XL_SYNC_PULSE_EV XL_FR_SYNC_PULSE_EV;

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
#include <pshpack8.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// IO XL API
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// global IO defines
// type defines for XLdaioTriggerMode.portTypeMask 
#define XL_DAIO_PORT_TYPE_MASK_DIGITAL       0x01
#define XL_DAIO_PORT_TYPE_MASK_ANALOG        0x02

// type defines for XLdaioTriggerMode.triggerType 
#define XL_DAIO_TRIGGER_TYPE_CYCLIC          0x01
#define XL_DAIO_TRIGGER_TYPE_PORT            0x02

///////////////////////////////////////////////
// defines for xlIoSetTriggerMode
typedef struct s_xl_daio_trigger_mode {
  unsigned int portTypeMask;    //!< Use defines XL_DAIO_PORT_TYPE_MASK_xxx. Unused for VN1630/VN1640.
  unsigned int triggerType;     //!< Use defines XL_DAIO_TRIGGER_TYPE_xxx from above

  union triggerTypeParams {
    unsigned int cycleTime;     //!< specify time in microseconds
    struct {
      unsigned int portMask;
      unsigned int type;        //!< Use defines XL_DAIO_TRIGGER_TYPE_xxx from below
    } digital;
  } param;

} XLdaioTriggerMode;
#define XL_DAIO_TRIGGER_TYPE_RISING          0x01
#define XL_DAIO_TRIGGER_TYPE_FALLING         0x02
#define XL_DAIO_TRIGGER_TYPE_BOTH            0x03

///////////////////////////////////////////////
// defines for xlIoConfigurePorts 
typedef struct xl_daio_set_port{
  unsigned int portType;        //!< Only one signal group is allowed. One of the defines XL_DAIO_PORT_TYPE_MASK_*
  unsigned int portMask;        //!< Mask of affected ports.
  unsigned int portFunction[8]; //!< Special function of port. One of the defines XL_DAIO_PORT_DIGITAL_* or XL_DAIO_PORT_ANALOG_*
  unsigned int reserved[8];     //!< Set this parameters to zero!
} XLdaioSetPort;

// for digital ports:
#define XL_DAIO_PORT_DIGITAL_IN              0x00
#define XL_DAIO_PORT_DIGITAL_PUSHPULL        0x01
#define XL_DAIO_PORT_DIGITAL_OPENDRAIN       0x02
#define XL_DAIO_PORT_DIGITAL_IN_OUT          0x06 //(only for WakeUp line)

// for analog ports:
#define XL_DAIO_PORT_ANALOG_IN               0x00
#define XL_DAIO_PORT_ANALOG_OUT              0x01
#define XL_DAIO_PORT_ANALOG_DIFF             0x02
#define XL_DAIO_PORT_ANALOG_OFF              0x03

///////////////////////////////////////////////
// defines for xlIoSetDigOutLevel
#define XL_DAIO_DO_LEVEL_0V                  0
#define XL_DAIO_DO_LEVEL_5V                  5
#define XL_DAIO_DO_LEVEL_12V                 12

///////////////////////////////////////////////
// defines for xlIoSetDigitalOutput
typedef struct xl_daio_digital_params{
  unsigned int portMask;     //!< Use defines XL_DAIO_PORT_MASK_DIGITAL_*
  unsigned int valueMask;    //!< Specify the port value (ON/HIGH – 1 | OFF/LOW - 0)
} XLdaioDigitalParams;

// defines for portMask
#define XL_DAIO_PORT_MASK_DIGITAL_D0         0x01
#define XL_DAIO_PORT_MASK_DIGITAL_D1         0x02
#define XL_DAIO_PORT_MASK_DIGITAL_D2         0x04
#define XL_DAIO_PORT_MASK_DIGITAL_D3         0x08
#define XL_DAIO_PORT_MASK_DIGITAL_D4         0x10
#define XL_DAIO_PORT_MASK_DIGITAL_D5         0x20
#define XL_DAIO_PORT_MASK_DIGITAL_D6         0x40
#define XL_DAIO_PORT_MASK_DIGITAL_D7         0x80

///////////////////////////////////////////////
// defines for xlIoSetAnalogOutput  
typedef struct xl_daio_analog_params {
  unsigned int portMask;     //!< Use defines XL_DAIO_PORT_MASK_ANALOG_*
  unsigned int value[8];     //!< 12-bit values
} XLdaioAnalogParams;

///////////////////////////////////////////////
// defines for XLdaioAnalogParams::portMask
#define XL_DAIO_PORT_MASK_ANALOG_A0          0x01
#define XL_DAIO_PORT_MASK_ANALOG_A1          0x02
#define XL_DAIO_PORT_MASK_ANALOG_A2          0x04
#define XL_DAIO_PORT_MASK_ANALOG_A3          0x08

///////////////////////////////////////////////
// event ids
#define XL_DAIO_EVT_ID_DIGITAL               XL_DAIO_PORT_TYPE_MASK_DIGITAL
#define XL_DAIO_EVT_ID_ANALOG                XL_DAIO_PORT_TYPE_MASK_ANALOG
#include <poppack.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ethernet API
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#define XL_ETH_EVENT_SIZE_HEADER       (unsigned short) 32
#define XL_ETH_EVENT_SIZE_MAX          (unsigned int) 2048

#define XL_ETH_RX_FIFO_QUEUE_SIZE_MAX  (unsigned int) (8*1024*1024)  //!< Maximum size of ethernet receive queue: 8 MByte
#define XL_ETH_RX_FIFO_QUEUE_SIZE_MIN  (unsigned int) (64*1024)      //!< Minimum size of ethernet receive queue: 64 KByte

#define XL_ETH_PAYLOAD_SIZE_MAX        (unsigned int) 1500 // maximum payload length for sending an ethernet packet
#define XL_ETH_PAYLOAD_SIZE_MIN        (unsigned int)   46 // minimum payload length for sending an ethernet packet (42 octets with VLAN tag present)
#define XL_ETH_RAW_FRAME_SIZE_MAX      (unsigned int) 1600 // maximum buffer size for storing a "raw" Ethernet frame (including VLAN tags, if present)

////////////////////////////////////////////////
// Infos in the flagsChip parameter of Ethernet events
////////////////////////////////////////////////
#define XL_ETH_CONNECTOR_RJ45          (unsigned short) 0x0001
#define XL_ETH_CONNECTOR_DSUB          (unsigned short) 0x0002
#define XL_ETH_PHY_IEEE                (unsigned short) 0x0004
#define XL_ETH_PHY_BROADR              (unsigned short) 0x0008
#define XL_ETH_FRAME_BYPASSED          (unsigned short) 0x0010 // For Rx and RxError events
#define XL_ETH_QUEUE_OVERFLOW          (unsigned short) 0x0100
#define XL_ETH_BYPASS_QUEUE_OVERFLOW   (unsigned short) 0x8000 //MAC bypass queue full condition occurred, one or more packets dropped

#define XL_ETH_MACADDR_OCTETS          6                     
#define XL_ETH_ETHERTYPE_OCTETS        2
#define XL_ETH_VLANTAG_OCTETS          4

////////////////////////////////////////////////
// Values for xlEthSetConfig
////////////////////////////////////////////////

// speed
#define XL_ETH_MODE_SPEED_AUTO_100         2    /* Set connection speed set to 100 Mbps via auto-negotiation */
#define XL_ETH_MODE_SPEED_AUTO_1000        4    /* Set connection speed to 1000 Mbps via auto-negotiation */
#define XL_ETH_MODE_SPEED_AUTO_100_1000    5    /* Set connection speed automatically to either 100 or 1000 Mbps */
#define XL_ETH_MODE_SPEED_FIXED_100        8    /* Set connection speed to 100 Mbps. Auto-negotiation disabled. */

// duplex
#define XL_ETH_MODE_DUPLEX_DONT_CARE       0    /* Used for BroadR-Reach since only full duplex mode possible. */
#define XL_ETH_MODE_DUPLEX_AUTO            1    /* Duplex mode set via auto-negotiation. Requires connection speed set to an "auto" value. Only for IEEE 802.3*/
#define XL_ETH_MODE_DUPLEX_FULL            3    /* Full duplex mode. Only for IEEE 802.3 */

// connector
#define XL_ETH_MODE_CONNECTOR_RJ45         1    /* Using RJ-45 connector */
#define XL_ETH_MODE_CONNECTOR_DSUB         2    /* Using D-Sub connector */

// phy
#define XL_ETH_MODE_PHY_IEEE_802_3         1    /* Set IEEE 802.3 */
#define XL_ETH_MODE_PHY_BROADR_REACH       2    /* Set BroadR-Reach */

// clockMode
#define XL_ETH_MODE_CLOCK_DONT_CARE        0    /* Used for IEEE 802.3 100 and 10 MBit */
#define XL_ETH_MODE_CLOCK_AUTO             1    /* Clock mode set automatically via auto-negotiation. Only for 1000Base-T if speed mode is one of the "auto" modes */
#define XL_ETH_MODE_CLOCK_MASTER           2    /* Clock mode is master. Only for 1000Base-T or BroadR-Reach */
#define XL_ETH_MODE_CLOCK_SLAVE            3    /* Clock mode is slave. Only for 1000Base-T or BroadR-Reach */

// mdiMode
#define XL_ETH_MODE_MDI_AUTO               1    /* Perform MDI auto detection */
#define XL_ETH_MODE_MDI_STRAIGHT           2    /* Direct MDI (connected to switch) */
#define XL_ETH_MODE_MDI_CROSSOVER          3    /* Crossover MDI (connected to endpoint) */

// brPairs
#define XL_ETH_MODE_BR_PAIR_DONT_CARE      0    /* Used for IEEE 802.3 */
#define XL_ETH_MODE_BR_PAIR_1PAIR          1    /* BR 1-pair connection. Only for BroadR-Reach */


////////////////////////////////////////////////


////////////////////////////////////////////////
// Values for T_XL_ETH_CHANNEL_STATUS event
////////////////////////////////////////////////

// T_XL_ETH_CHANNEL_STATUS.link
#define XL_ETH_STATUS_LINK_UNKNOWN         0    /* The link state could not be determined (e.g. lost connection to board) */
#define XL_ETH_STATUS_LINK_DOWN            1    /* Link is down (no cable attached, no configuration set, configuration does not match) */
#define XL_ETH_STATUS_LINK_UP              2    /* Link is up */
#define XL_ETH_STATUS_LINK_ERROR           4    /* Link is in error state (e.g. auto-negotiation failed) */

// T_XL_ETH_CHANNEL_STATUS.speed
#define XL_ETH_STATUS_SPEED_UNKNOWN        0    /* Connection speed could not be determined (e.g. during auto-negotiation or if link down) */
#define XL_ETH_STATUS_SPEED_100            2    /* Link speed is 100 Mbps */
#define XL_ETH_STATUS_SPEED_1000           3    /* Link speed is 1000 Mbps */

// T_XL_ETH_CHANNEL_STATUS.duplex
#define XL_ETH_STATUS_DUPLEX_UNKNOWN       0    /* Duplex mode could not be determined (e.g. during auto-negotiation or if link down) */
#define XL_ETH_STATUS_DUPLEX_FULL          2    /* Full duplex mode */

// T_XL_ETH_CHANNEL_STATUS.mdiType
#define XL_ETH_STATUS_MDI_UNKNOWN          0    /* MDI mode could not be determined  (e.g. during auto-negotiation or if link down) */
#define XL_ETH_STATUS_MDI_STRAIGHT         1    /* Direct MDI */
#define XL_ETH_STATUS_MDI_CROSSOVER        2    /* Crossover MDI */

// T_XL_ETH_CHANNEL_STATUS.activeConnector
#define XL_ETH_STATUS_CONNECTOR_RJ45       1    /* Using RJ-45 connector */
#define XL_ETH_STATUS_CONNECTOR_DSUB       2    /* Using D-Sub connector */

// T_XL_ETH_CHANNEL_STATUS.activePhy
#define XL_ETH_STATUS_PHY_UNKNOWN          0    /* PHY is currently uhknown (e.g. if link is down) */
#define XL_ETH_STATUS_PHY_IEEE_802_3       1    /* PHY is IEEE 802.3 */
#define XL_ETH_STATUS_PHY_BROADR_REACH     2    /* PHY is BroadR-Reach */

// T_XL_ETH_CHANNEL_STATUS.clockMode
#define XL_ETH_STATUS_CLOCK_DONT_CARE      0    /* Clock mode not relevant. Only for IEEE 802.3 100/10 MBit */
#define XL_ETH_STATUS_CLOCK_MASTER         1    /* Clock mode is master. Only for 1000Base-T or BroadR-Reach */
#define XL_ETH_STATUS_CLOCK_SLAVE          2    /* Clock mode is slave. Only for 1000Base-T or BroadR-Reach */

// T_XL_ETH_CHANNEL_STATUS.brPairs
#define XL_ETH_STATUS_BR_PAIR_DONT_CARE    0    /* No BR pair available. Only for IEEE 802.3 1000/100/10 MBit */
#define XL_ETH_STATUS_BR_PAIR_1PAIR        1    /* BR 1-pair connection. Only for BroadR-Reach */


////////////////////////////////////////////////

// T_XL_ETH_DATAFRAME_RX_ERROR.errorFlags
#define XL_ETH_RX_ERROR_INVALID_LENGTH     ((unsigned int) 0x00000001) /* Invalid length error. Set when the receive frame has an invalid length as defined by IEEE802.3 */
#define XL_ETH_RX_ERROR_INVALID_CRC        ((unsigned int) 0x00000002) /* CRC error. Set when frame is received with CRC-32 error but valid length */
#define XL_ETH_RX_ERROR_PHY_ERROR          ((unsigned int) 0x00000004) /* Corrupted receive frame caused by a PHY error */

// T_XL_ETH_DATAFRAME_TX.flags
// T_XL_ETH_DATAFRAME_TXACK.flags
// T_XL_ETH_DATAFRAME_TXACK_OTHERAPP.flags
// T_XL_ETH_DATAFRAME_TXACK_SW.flags
#define XL_ETH_DATAFRAME_FLAGS_USE_SOURCE_MAC   (unsigned int) 0x00000001 /* Use the given source MAC address (not set by hardware) */

// Bypass values
#define XL_ETH_BYPASS_INACTIVE                  0   /* Bypass inactive (default state) */
#define XL_ETH_BYPASS_PHY                       1   /* Bypass active via PHY loop */
#define XL_ETH_BYPASS_MACCORE                   2   /* Bypass active via L2 switch (using MAC cores) */

// T_XL_ETH_DATAFRAME_TX_ERROR.errorType
// T_XL_ETH_DATAFRAME_TX_ERR_SW.errorType
// T_XL_ETH_DATAFRAME_TX_ERR_OTHERAPP.errorType
#define XL_ETH_TX_ERROR_BYPASS_ENABLED      1   /* Bypass activated */
#define XL_ETH_TX_ERROR_NO_LINK             2   /* No Link */
#define XL_ETH_TX_ERROR_PHY_NOT_CONFIGURED  3   /* PHY not yet configured */
#define XL_ETH_TX_ERROR_INVALID_LENGTH      7   /* Frame with invalid length transmitted */


#include <pshpack1.h>

///////////////////////////////////////////////////////////////////////////////////
// Structures for Ethernet API
///////////////////////////////////////////////////////////////////////////////////

typedef unsigned short   XLethEventTag;

typedef struct s_xl_eth_frame {
  unsigned short     etherType;            /* Ethernet type in network byte order */
  unsigned char      payload[XL_ETH_PAYLOAD_SIZE_MAX];
} T_XL_ETH_FRAME;

typedef union s_xl_eth_framedata {
  unsigned char          rawData[XL_ETH_RAW_FRAME_SIZE_MAX];
  T_XL_ETH_FRAME         ethFrame;
} T_XL_ETH_FRAMEDATA;

typedef struct s_xl_eth_dataframe_rx {
  unsigned int       frameIdentifier;       /* FPGA internal identifier unique to every received frame */
  unsigned int       frameDuration;         /* transmit duration of the Ethernet frame, in nanoseconds */
  unsigned short     dataLen;               /* Overall data length of <frameData> */
  unsigned short     reserved;              /* currently reserved field - not used, ignore */
  unsigned int       reserved2[3];          /* currently reserved field - not used, ignore */
  unsigned int       fcs;                   /* Frame Check Sum */
  unsigned char      destMAC[XL_ETH_MACADDR_OCTETS];            /* Destination MAC address */
  unsigned char      sourceMAC[XL_ETH_MACADDR_OCTETS];          /* Source MAC address */
  T_XL_ETH_FRAMEDATA frameData;
} T_XL_ETH_DATAFRAME_RX;

typedef struct s_xl_eth_dataframe_rxerror {
  unsigned int       frameIdentifier;       /* FPGA internal identifier unique to every received frame */
  unsigned int       frameDuration;         /* transmit duration of the Ethernet frame, in nanoseconds */
  unsigned int       errorFlags;            /* Error information (XL_ETH_RX_ERROR_*) */
  unsigned short     dataLen;               /* Overall data length of <frameData> */
  unsigned short     reserved;              /* currently reserved field - not used, ignore */
  unsigned int       reserved2[3];          /* currently reserved field - not used, ignore */
  unsigned int       fcs;                   /* Frame Check Sum */
  unsigned char      destMAC[XL_ETH_MACADDR_OCTETS];            /* Destination MAC address */
  unsigned char      sourceMAC[XL_ETH_MACADDR_OCTETS];          /* Source MAC address */
  T_XL_ETH_FRAMEDATA frameData;
} T_XL_ETH_DATAFRAME_RX_ERROR;

typedef struct s_xl_eth_dataframe_tx {
  unsigned int       frameIdentifier;       /* FPGA internal identifier unique to every frame sent */
  unsigned int       flags;                 /* Flags to specify whether to use the given source MAC and FCS (see XL_ETH_DATAFRAME_FLAGS_) */
  unsigned short     dataLen;               /* Overall data length of <frameData> */
  unsigned short     reserved;              /* currently reserved field - must be set to "0" */
  unsigned int       reserved2[4];          /* reserved field - must be set to "0" */
  unsigned char      destMAC[XL_ETH_MACADDR_OCTETS];            /* Destination MAC address */
  unsigned char      sourceMAC[XL_ETH_MACADDR_OCTETS];          /* Source MAC address */
  T_XL_ETH_FRAMEDATA frameData;
} T_XL_ETH_DATAFRAME_TX;

typedef struct s_xl_eth_dataframe_tx_event {
  unsigned int       frameIdentifier;       /* FPGA internal identifier unique to every frame sent */
  unsigned int       flags;                 /* Flags signalizing whether the given source MAC and FCS have been used by the send request (see XL_ETH_DATAFRAME_FLAGS_) */
  unsigned short     dataLen;               /* Overall data length of <frameData> */
  unsigned short     reserved;              /* currently reserved field - not used, ignore */
  unsigned int       frameDuration;         /* transmit duration of the Ethernet frame, in nanoseconds */
  unsigned int       reserved2[2];          /* currently reserved field - not used, ignore */
  unsigned int       fcs;                   /* Frame Check Sum */
  unsigned char      destMAC[XL_ETH_MACADDR_OCTETS];            /* Destination MAC address */
  unsigned char      sourceMAC[XL_ETH_MACADDR_OCTETS];          /* Source MAC address */
  T_XL_ETH_FRAMEDATA frameData;
} T_XL_ETH_DATAFRAME_TX_EVENT;

typedef T_XL_ETH_DATAFRAME_TX_EVENT T_XL_ETH_DATAFRAME_TXACK;
typedef T_XL_ETH_DATAFRAME_TX_EVENT T_XL_ETH_DATAFRAME_TXACK_SW;
typedef T_XL_ETH_DATAFRAME_TX_EVENT T_XL_ETH_DATAFRAME_TXACK_OTHERAPP;

typedef struct s_xl_eth_dataframe_txerror {
  unsigned int                 errorType;         /* Error information */
  T_XL_ETH_DATAFRAME_TX_EVENT  txFrame;
} T_XL_ETH_DATAFRAME_TX_ERROR;

typedef T_XL_ETH_DATAFRAME_TX_ERROR T_XL_ETH_DATAFRAME_TX_ERR_SW;
typedef T_XL_ETH_DATAFRAME_TX_ERROR T_XL_ETH_DATAFRAME_TX_ERR_OTHERAPP;

#include <poppack.h>
#include <pshpack4.h>

typedef struct s_xl_eth_config_result {
  unsigned int       result;
} T_XL_ETH_CONFIG_RESULT;

typedef struct s_xl_eth_channel_status {
  unsigned int       link;           /* (XL_ETH_STATUS_LINK_*)      Ethernet connection status */
  unsigned int       speed;          /* (XL_ETH_STATUS_SPEED_*)     Link connection speed */
  unsigned int       duplex;         /* (XL_ETH_STATUS_DUPLEX_*)    Ethernet duplex mode. 1000Base-T always uses full duplex. */
  unsigned int       mdiType;        /* (XL_ETH_STATUS_MDI_*)       Currently active MDI-mode */
  unsigned int       activeConnector;/* (XL_ETH_STATUS_CONNECTOR_*) Connector (plug) to use (BroadR-REACH or RJ-45). */
  unsigned int       activePhy;      /* (XL_ETH_STATUS_PHY_*)       Currently active physical layer */
  unsigned int       clockMode;      /* (XL_ETH_STATUS_CLOCK_*)     When in 1000Base-T or BroadR-mode, currently active mode */
  unsigned int       brPairs;        /* (XL_ETH_STATUS_BR_PAIR_*)   When in BroadR-mode, number of used cable pairs */
} T_XL_ETH_CHANNEL_STATUS;

typedef struct s_xl_eth_lostevent {
  XLethEventTag      eventTypeLost;   /* Type of event lost */
  unsigned short     reserved;        /* currently reserved field - not used */
  unsigned int       reason;          /* Reason code why the events were lost (0 means unknown) */
  union {
    struct {
      unsigned int   frameIdentifier;       /* FPGA internal identifier unique to every frame sent */
      unsigned int   fcs;                   /* Frame Check Sum */
      unsigned char  sourceMAC[XL_ETH_MACADDR_OCTETS]; /* Source MAC address */
      unsigned char  reserved[2];           /* currently reserved field - not used */
    } txAck, txAckSw;
    struct {
      unsigned int   errorType;
      unsigned int   frameIdentifier;       /* FPGA internal identifier unique to every frame sent */
      unsigned int   fcs;                   /* Frame Check Sum */
      unsigned char  sourceMAC[XL_ETH_MACADDR_OCTETS]; /* Source MAC address */
      unsigned char  reserved[2];           /* currently reserved field - not used */
    } txError, txErrorSw;
    unsigned int     reserved[20];
  } eventInfo;
} T_XL_ETH_LOSTEVENT;

typedef struct s_xl_eth_event {
  unsigned int       size;             // 4 - overall size of the complete event, depending on event type and piggybacked data
  XLethEventTag      tag;              // 2 - type of the event 
  unsigned short     channelIndex;     // 2 
  unsigned int       userHandle;       // 4 
  unsigned short     flagsChip;        // 2
  unsigned short     reserved;         // 2
  XLuint64           reserved1;        // 8 
  XLuint64           timeStampSync;    // 8 - timestamp which is synchronized by the driver
                                       // --------- 
                                       // 32 bytes -> XL_ETH_EVENT_SIZE_HEADER

  union s_xl_eth_tag_data {
    unsigned char                      rawData[XL_ETH_EVENT_SIZE_MAX]; 
    T_XL_ETH_DATAFRAME_RX              frameRxOk;      //(tag==XL_ETH_EVENT_TAG_FRAMERX)              Frame received from network
    T_XL_ETH_DATAFRAME_RX_ERROR        frameRxError;   //(tag==XL_ETH_EVENT_TAG_FRAMERX_ERROR)        Erroneous frame received from network
    T_XL_ETH_DATAFRAME_TXACK           frameTxAck;     //(tag==XL_ETH_EVENT_TAG_FRAMETX_ACK)          ACK for frame sent by application
    T_XL_ETH_DATAFRAME_TXACK_SW        frameTxAckSw;   //(tag==XL_ETH_EVENT_TAG_FRAMETX_ACK_SWITCH)   ACK for frame sent by switch
    T_XL_ETH_DATAFRAME_TXACK_OTHERAPP  frameTxAckOtherApp;//(tag==XL_ETH_EVENT_TAG_FRAMETX_ERROR_OTHER_APP) ACK for frame sent by another application
    T_XL_ETH_DATAFRAME_TX_ERROR        frameTxError;   //(tag==XL_ETH_EVENT_TAG_FRAMETX_ERROR)        NACK for frame sent by application (frame could not be transmitted)
    T_XL_ETH_DATAFRAME_TX_ERR_SW       frameTxErrorSw; //(tag==XL_ETH_EVENT_TAG_FRAMETX_ERROR_SWITCH) NACK for frame sent by switch. May indicate internal processing failure (e.g. queue full condition)
    T_XL_ETH_DATAFRAME_TX_ERR_OTHERAPP frameTxErrorOtherApp;//(tag==XL_ETH_EVENT_TAG_FRAMETX_ERROR_OTHER_APP) NACK for frame sent by another application
    T_XL_ETH_CONFIG_RESULT             configResult;
    T_XL_ETH_CHANNEL_STATUS            channelStatus;
    XL_SYNC_PULSE_EV                   syncPulse;
    T_XL_ETH_LOSTEVENT                 lostEvent;        //(tag==XL_ETH_EVENT_TAG_LOSTEVENT)           Indication that one or more events have been lost
  } tagData;
} T_XL_ETH_EVENT;

typedef struct {
  unsigned int       speed;          /* (XL_ETH_MODE_SPEED_*)       Connection speed setting */
  unsigned int       duplex;         /* (XL_ETH_MODE_DUPLEX_*)      Duplex mode setting. Not relevant for BroadR-REACH mode, set to "nochange" or "auto". */
  unsigned int       connector;      /* (XL_ETH_MODE_CONNECTOR_*)   Connector to use  */
  unsigned int       phy;            /* (XL_ETH_MODE_PHY_*)         Physical interface to enable  */
  unsigned int       clockMode;      /* (XL_ETH_MODE_CLOCK_*)       Master or slave clock mode setting (1000Base-T/BroadR-REACH mode only). */
  unsigned int       mdiMode;        /* (XL_ETH_MODE_MDI_*)         Currently active MDI-mode */
  unsigned int       brPairs;        /* (XL_ETH_MODE_BR_PAIR_*)     Number of cable pairs to use (BroadR-REACH mode only). */  
} T_XL_ETH_CONFIG;

#include <poppack.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MOST150 XL API
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#define XL_MOST150_RX_EVENT_HEADER_SIZE       (unsigned short) 32
#define XL_MOST150_MAX_EVENT_DATA_SIZE        (unsigned int) 2048
#define MOST150_SYNC_ALLOC_INFO_SIZE          (unsigned int) 372

#define XL_MOST150_CTRL_PAYLOAD_MAX_SIZE          (unsigned short) 45
#define XL_MOST150_ASYNC_PAYLOAD_MAX_SIZE         (unsigned short) 1524 // maximum valid length (s. INIC User Manual)
#define XL_MOST150_ETHERNET_PAYLOAD_MAX_SIZE      (unsigned short) 1506 // maximum valid length (s. INIC User Manual)
#define XL_MOST150_ASYNC_SEND_PAYLOAD_MAX_SIZE    (unsigned short) 1600 // maximum length for sending a MDP
#define XL_MOST150_ETHERNET_SEND_PAYLOAD_MAX_SIZE (unsigned short) 1600 // maximum length for sending a MEP


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Flags for the flagsChip parameter
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#define XL_MOST150_VN2640                         (unsigned short) 0x0001  //!< common VN2640 event
#define XL_MOST150_INIC                           (unsigned short) 0x0002  //!< event was generated by INIC
#define XL_MOST150_SPY                            (unsigned short) 0x0004  //!< event was generated by spy
#define XL_MOST150_QUEUE_OVERFLOW                 (unsigned short) 0x0100  //!< queue overflow occured (some events are lost)

// XL_MOST150_EVENT_SOURCE_EV.mask
#define XL_MOST150_SOURCE_SPECIAL_NODE     (unsigned int) 0x00000001
#define XL_MOST150_SOURCE_SYNC_ALLOC_INFO  (unsigned int) 0x00000004
#define XL_MOST150_SOURCE_CTRL_SPY         (unsigned int) 0x00000008
#define XL_MOST150_SOURCE_ASYNC_SPY        (unsigned int) 0x00000010
#define XL_MOST150_SOURCE_ETH_SPY          (unsigned int) 0x00000020
#define XL_MOST150_SOURCE_SHUTDOWN_FLAG    (unsigned int) 0x00000040
#define XL_MOST150_SOURCE_SYSTEMLOCK_FLAG  (unsigned int) 0x00000080
#define XL_MOST150_SOURCE_LIGHTLOCK_SPY    (unsigned int) 0x00000200
#define XL_MOST150_SOURCE_LIGHTLOCK_INIC   (unsigned int) 0x00000400
#define XL_MOST150_SOURCE_ECL_CHANGE       (unsigned int) 0x00000800
#define XL_MOST150_SOURCE_LIGHT_STRESS     (unsigned int) 0x00001000
#define XL_MOST150_SOURCE_LOCK_STRESS      (unsigned int) 0x00002000
#define XL_MOST150_SOURCE_BUSLOAD_CTRL     (unsigned int) 0x00004000
#define XL_MOST150_SOURCE_BUSLOAD_ASYNC    (unsigned int) 0x00008000
#define XL_MOST150_SOURCE_CTRL_MLB         (unsigned int) 0x00010000
#define XL_MOST150_SOURCE_ASYNC_MLB        (unsigned int) 0x00020000
#define XL_MOST150_SOURCE_ETH_MLB          (unsigned int) 0x00040000
#define XL_MOST150_SOURCE_TXACK_MLB        (unsigned int) 0x00080000
#define XL_MOST150_SOURCE_STREAM_UNDERFLOW (unsigned int) 0x00100000
#define XL_MOST150_SOURCE_STREAM_OVERFLOW  (unsigned int) 0x00200000
#define XL_MOST150_SOURCE_STREAM_RX_DATA   (unsigned int) 0x00400000
#define XL_MOST150_SOURCE_ECL_SEQUENCE     (unsigned int) 0x00800000

// XL_MOST150_DEVICE_MODE_EV.deviceMode
#define XL_MOST150_DEVICEMODE_SLAVE                     (unsigned char) 0x00
#define XL_MOST150_DEVICEMODE_MASTER                    (unsigned char) 0x01
#define XL_MOST150_DEVICEMODE_STATIC_MASTER             (unsigned char) 0x03
#define XL_MOST150_DEVICEMODE_RETIMED_BYPASS_SLAVE      (unsigned char) 0x04
#define XL_MOST150_DEVICEMODE_RETIMED_BYPASS_MASTER     (unsigned char) 0x05

// XL_MOST150_FREQUENCY_EV.frequency
#define XL_MOST150_FREQUENCY_44100                       (unsigned int) 0x00000000
#define XL_MOST150_FREQUENCY_48000                       (unsigned int) 0x00000001
#define XL_MOST150_FREQUENCY_ERROR                       (unsigned int) 0x00000002

// XL_MOST150_SPECIAL_NODE_INFO_EV.changeMask
#define XL_MOST150_NA_CHANGED                            (unsigned int) 0x00000001
#define XL_MOST150_GA_CHANGED                            (unsigned int) 0x00000002
#define XL_MOST150_NPR_CHANGED                           (unsigned int) 0x00000004
#define XL_MOST150_MPR_CHANGED                           (unsigned int) 0x00000008
#define XL_MOST150_SBC_CHANGED                           (unsigned int) 0x00000010
#define XL_MOST150_CTRL_RETRY_PARAMS_CHANGED             (unsigned int) 0x00000060
#define XL_MOST150_ASYNC_RETRY_PARAMS_CHANGED            (unsigned int) 0x00000180
#define XL_MOST150_MAC_ADDR_CHANGED                      (unsigned int) 0x00000200
#define XL_MOST150_NPR_SPY_CHANGED                       (unsigned int) 0x00000400
#define XL_MOST150_MPR_SPY_CHANGED                       (unsigned int) 0x00000800
#define XL_MOST150_SBC_SPY_CHANGED                       (unsigned int) 0x00001000
#define XL_MOST150_INIC_NISTATE_CHANGED                  (unsigned int) 0x00002000
#define XL_MOST150_SPECIAL_NODE_MASK_CHANGED             (unsigned int) 0x00003FFF

// Retry Parameters
#define XL_MOST150_CTRL_RETRY_TIME_MIN                   (unsigned int) 3  // Time Unit: 16 MOST Frames
#define XL_MOST150_CTRL_RETRY_TIME_MAX                   (unsigned int) 31
#define XL_MOST150_CTRL_SEND_ATTEMPT_MIN                 (unsigned int) 1
#define XL_MOST150_CTRL_SEND_ATTEMPT_MAX                 (unsigned int) 16
#define XL_MOST150_ASYNC_RETRY_TIME_MIN                  (unsigned int) 0  // Time Unit: 1 MOST Frame
#define XL_MOST150_ASYNC_RETRY_TIME_MAX                  (unsigned int) 255
#define XL_MOST150_ASYNC_SEND_ATTEMPT_MIN                (unsigned int) 1  // For both MDP and MEP
#define XL_MOST150_ASYNC_SEND_ATTEMPT_MAX                (unsigned int) 16

// NIStates
#define XL_MOST150_INIC_NISTATE_NET_OFF                  (unsigned int) 0x00000000
#define XL_MOST150_INIC_NISTATE_NET_INIT                 (unsigned int) 0x00000001
#define XL_MOST150_INIC_NISTATE_NET_RBD                  (unsigned int) 0x00000002
#define XL_MOST150_INIC_NISTATE_NET_ON                   (unsigned int) 0x00000003
#define XL_MOST150_INIC_NISTATE_NET_RBD_RESULT           (unsigned int) 0x00000004

// XL_MOST150_CTRL_TX_ACK_EV.status
#define XL_MOST150_TX_OK                                 (unsigned int) 0x00000001
#define XL_MOST150_TX_FAILED_FORMAT_ERROR                (unsigned int) 0x00000002
#define XL_MOST150_TX_FAILED_NETWORK_OFF                 (unsigned int) 0x00000004
#define XL_MOST150_TX_FAILED_TIMEOUT                     (unsigned int) 0x00000005
#define XL_MOST150_TX_FAILED_WRONG_TARGET                (unsigned int) 0x00000008
#define XL_MOST150_TX_OK_ONE_SUCCESS                     (unsigned int) 0x00000009
#define XL_MOST150_TX_FAILED_BAD_CRC                     (unsigned int) 0x0000000C
#define XL_MOST150_TX_FAILED_RECEIVER_BUFFER_FULL        (unsigned int) 0x0000000E

// XL_MOST150_CTRL_SPY_EV.validMask
// XL_MOST150_ASYNC_SPY_EV.validMask
// XL_MOST150_ETHERNET_SPY_EV.validMask
#define XL_MOST150_VALID_DATALENANNOUNCED                (unsigned int) 0x00000001
#define XL_MOST150_VALID_SOURCEADDRESS                   (unsigned int) 0x00000002
#define XL_MOST150_VALID_TARGETADDRESS                   (unsigned int) 0x00000004
#define XL_MOST150_VALID_PACK                            (unsigned int) 0x00000008
#define XL_MOST150_VALID_CACK                            (unsigned int) 0x00000010
#define XL_MOST150_VALID_PINDEX                          (unsigned int) 0x00000020
#define XL_MOST150_VALID_PRIORITY                        (unsigned int) 0x00000040
#define XL_MOST150_VALID_CRC                             (unsigned int) 0x00000080
#define XL_MOST150_VALID_CRCCALCULATED                   (unsigned int) 0x00000100
#define XL_MOST150_VALID_MESSAGE                         (unsigned int) 0x80000000

// XL_MOST150_CTRL_SPY_EV.pAck
// XL_MOST150_ASYNC_SPY_EV.pAck
// XL_MOST150_ETHERNET_SPY_EV.pAck
#define XL_MOST150_PACK_OK                               (unsigned int) 0x00000004
#define XL_MOST150_PACK_BUFFER_FULL                      (unsigned int) 0x00000001
#define XL_MOST150_PACK_NO_RESPONSE                      (unsigned int) 0x00000000 // maybe spy before receiver

// XL_MOST150_CTRL_SPY_EV.cAck
// XL_MOST150_ASYNC_SPY_EV.cAck
// XL_MOST150_ETHERNET_SPY_EV.cAck
#define XL_MOST150_CACK_OK                               (unsigned int) 0x00000004
#define XL_MOST150_CACK_CRC_ERROR                        (unsigned int) 0x00000001
#define XL_MOST150_CACK_NO_RESPONSE                      (unsigned int) 0x00000000 // maybe spy before receiver

//XL_MOST150_ASYNC_RX_EV.length
#define XL_MOST150_ASYNC_INVALID_RX_LENGTH               (unsigned int) 0x00008000 // flag indicating a received MDP with length > XL_MOST150_ASYNC_PAYLOAD_MAX_SIZE

//XL_MOST150_ETHERNET_RX_EV.length
#define XL_MOST150_ETHERNET_INVALID_RX_LENGTH            (unsigned int) 0x80000000 // flag indicating a received MEP with length > XL_MOST150_ETHERNET_PAYLOAD_MAX_SIZE

// XL_MOST150_TX_LIGHT_EV.light
#define XL_MOST150_LIGHT_OFF                             (unsigned int) 0x00000000
#define XL_MOST150_LIGHT_FORCE_ON                        (unsigned int) 0x00000001
#define XL_MOST150_LIGHT_MODULATED                       (unsigned int) 0x00000002

// XL_MOST150_RXLIGHT_LOCKSTATUS_EV.status
//#define XL_MOST150_LIGHT_OFF                             (unsigned int) 0x00000000
#define XL_MOST150_LIGHT_ON_UNLOCK                       (unsigned int) 0x00000003
#define XL_MOST150_LIGHT_ON_LOCK                         (unsigned int) 0x00000004
#define XL_MOST150_LIGHT_ON_STABLE_LOCK                  (unsigned int) 0x00000005
#define XL_MOST150_LIGHT_ON_CRITICAL_UNLOCK              (unsigned int) 0x00000006

// XL_MOST150_ERROR_EV.errorCode
#define XL_MOST150_ERROR_ASYNC_TX_ACK_HANDLE             (unsigned int) 0x00000001
#define XL_MOST150_ERROR_ETH_TX_ACK_HANDLE               (unsigned int) 0x00000002

// XL_MOST150_CONFIGURE_RX_BUFFER_EV.bufferType
#define XL_MOST150_RX_BUFFER_TYPE_CTRL                   (unsigned int) 0x00000001
#define XL_MOST150_RX_BUFFER_TYPE_ASYNC                  (unsigned int) 0x00000002

// XL_MOST150_CONFIGURE_RX_BUFFER_EV.bufferMode
#define XL_MOST150_RX_BUFFER_NORMAL_MODE                 (unsigned int) 0x00000000
#define XL_MOST150_RX_BUFFER_BLOCK_MODE                  (unsigned int) 0x00000001

// XL_MOST150_CTRL_SYNC_AUDIO_EV.device
#define XL_MOST150_DEVICE_LINE_IN                        (unsigned int) 0x00000000
#define XL_MOST150_DEVICE_LINE_OUT                       (unsigned int) 0x00000001
#define XL_MOST150_DEVICE_SPDIF_IN                       (unsigned int) 0x00000002
#define XL_MOST150_DEVICE_SPDIF_OUT                      (unsigned int) 0x00000003
#define XL_MOST150_DEVICE_ALLOC_BANDWIDTH                (unsigned int) 0x00000004

// XL_MOST150_CTRL_SYNC_AUDIO_EV.mode
#define XL_MOST150_DEVICE_MODE_OFF                       (unsigned int) 0x00000000
#define XL_MOST150_DEVICE_MODE_ON                        (unsigned int) 0x00000001
#define XL_MOST150_DEVICE_MODE_OFF_BYPASS_CLOSED         (unsigned int) 0x00000002
#define XL_MOST150_DEVICE_MODE_OFF_NOT_IN_NETON          (unsigned int) 0x00000003
#define XL_MOST150_DEVICE_MODE_OFF_NO_MORE_RESOURCES     (unsigned int) 0x00000004
#define XL_MOST150_DEVICE_MODE_OFF_NOT_ENOUGH_FREE_BW    (unsigned int) 0x00000005
#define XL_MOST150_DEVICE_MODE_OFF_DUE_TO_NET_OFF        (unsigned int) 0x00000006
#define XL_MOST150_DEVICE_MODE_OFF_DUE_TO_CFG_NOT_OK     (unsigned int) 0x00000007
#define XL_MOST150_DEVICE_MODE_OFF_COMMUNICATION_ERROR   (unsigned int) 0x00000008
#define XL_MOST150_DEVICE_MODE_OFF_STREAM_CONN_ERROR     (unsigned int) 0x00000009
#define XL_MOST150_DEVICE_MODE_OFF_CL_ALREADY_USED       (unsigned int) 0x0000000A
#define XL_MOST150_DEVICE_MODE_CL_NOT_ALLOCATED          (unsigned int) 0x000000FF

// Maximum number of CL that can be allocated for device XL_MOST150_DEVICE_ALLOC_BANDWIDTH
#define XL_MOST150_ALLOC_BANDWIDTH_NUM_CL_MAX            10

// special CL for xlMost150CtrlSyncAudio to de-allocate all CLs for device XL_MOST150_DEVICE_ALLOC_BANDWIDTH
#define XL_MOST150_CL_DEALLOC_ALL                        (unsigned int) 0x00000FFF

// XL_MOST150_SYNC_VOLUME_STATUS_EV.volume
#define XL_MOST150_VOLUME_MIN                            (unsigned int) 0x00000000
#define XL_MOST150_VOLUME_MAX                            (unsigned int) 0x000000FF

// XL_MOST150_SYNC_MUTE_STATUS_EV.mute
#define XL_MOST150_NO_MUTE                               (unsigned int) 0x00000000
#define XL_MOST150_MUTE                                  (unsigned int) 0x00000001

// XL_MOST150_LIGHT_POWER_EV.lightPower
#define XL_MOST150_LIGHT_FULL                            (unsigned int) 0x00000064
#define XL_MOST150_LIGHT_3DB                             (unsigned int) 0x00000032

// XL_MOST150_SYSTEMLOCK_FLAG_EV.state
#define XL_MOST150_SYSTEMLOCK_FLAG_NOT_SET               (unsigned int) 0x00000000
#define XL_MOST150_SYSTEMLOCK_FLAG_SET                   (unsigned int) 0x00000001

// XL_MOST150_SHUTDOWN_FLAG_EV.state
#define XL_MOST150_SHUTDOWN_FLAG_NOT_SET                 (unsigned int) 0x00000000
#define XL_MOST150_SHUTDOWN_FLAG_SET                     (unsigned int) 0x00000001

// ECL
// xlMost150SetECLLine
#define XL_MOST150_ECL_LINE_LOW                          (unsigned int) 0x00000000
#define XL_MOST150_ECL_LINE_HIGH                         (unsigned int) 0x00000001
// xlMost150SetECLTermination
#define XL_MOST150_ECL_LINE_PULL_UP_NOT_ACTIVE           (unsigned int) 0x00000000
#define XL_MOST150_ECL_LINE_PULL_UP_ACTIVE               (unsigned int) 0x00000001

// xlMost150EclConfigureSeq
// Maximum number of states that can be configured for a sequence
#define XL_MOST150_ECL_SEQ_NUM_STATES_MAX               200
// Value range for duration of ECL sequence states
#define XL_MOST150_ECL_SEQ_DURATION_MIN                  1      // -> 100 µs
#define XL_MOST150_ECL_SEQ_DURATION_MAX                  655350 // -> 65535 ms

// xlMost150EclSetGlitchFilter
// Value range for setting the glitch filter
#define XL_MOST150_ECL_GLITCH_FILTER_MIN                 50      // -> 50 µs
#define XL_MOST150_ECL_GLITCH_FILTER_MAX                 50000   // -> 50 ms

// XL_MOST150_GEN_LIGHT_ERROR_EV.stressStarted
// XL_MOST150_GEN_LOCK_ERROR_EV.stressStarted
// XL_MOST150_CTRL_BUSLOAD.busloadStarted
// XL_MOST150_ASYNC_BUSLOAD.busloadStarted
// XL_MOST150_ECL_SEQUENCE_EV.sequenceStarted
#define XL_MOST150_MODE_DEACTIVATED                       0
#define XL_MOST150_MODE_ACTIVATED                         1

// busloadType for xlMost150AsyncConfigureBusload
#define XL_MOST150_BUSLOAD_TYPE_DATA_PACKET               0
#define XL_MOST150_BUSLOAD_TYPE_ETHERNET_PACKET           1

// counterType for the xlMost150****ConfigureBusload function
#define XL_MOST150_BUSLOAD_COUNTER_TYPE_NONE              0x00
#define XL_MOST150_BUSLOAD_COUNTER_TYPE_1_BYTE            0x01
#define XL_MOST150_BUSLOAD_COUNTER_TYPE_2_BYTE            0x02
#define XL_MOST150_BUSLOAD_COUNTER_TYPE_3_BYTE            0x03
#define XL_MOST150_BUSLOAD_COUNTER_TYPE_4_BYTE            0x04


// XL_MOST150_SPDIF_MODE_EV.spdifMode
#define XL_MOST150_SPDIF_MODE_SLAVE                      (unsigned int) 0x00000000
#define XL_MOST150_SPDIF_MODE_MASTER                     (unsigned int) 0x00000001

// XL_MOST150_SPDIF_MODE_EV.spdifError
#define XL_MOST150_SPDIF_ERR_NO_ERROR                    (unsigned int) 0x00000000
#define XL_MOST150_SPDIF_ERR_HW_COMMUNICATION            (unsigned int) 0x00000001

// XL_MOST150_NW_STARTUP_EV.error
#define XL_MOST150_STARTUP_NO_ERROR                      (unsigned int) 0x00000000
// XL_MOST150_NW_STARTUP_EV.errorInfo
#define XL_MOST150_STARTUP_NO_ERRORINFO                  (unsigned int) 0xFFFFFFFF

// XL_MOST150_NW_SHUTDOWN_EV.error
#define XL_MOST150_SHUTDOWN_NO_ERROR                     (unsigned int) 0x00000000
// XL_MOST150_NW_SHUTDOWN_EV.errorInfo
#define XL_MOST150_SHUTDOWN_NO_ERRORINFO                 (unsigned int) 0xFFFFFFFF

/// Values for synchronous streaming API
#define XL_MOST150_STREAM_RX_DATA                        0 // RX streaming: MOST -> PC
#define XL_MOST150_STREAM_TX_DATA                        1 // TX streaming: PC -> MOST

#define XL_MOST150_STREAM_INVALID_HANDLE                 0  

// stream states
#define XL_MOST150_STREAM_STATE_CLOSED                   0x01
#define XL_MOST150_STREAM_STATE_OPENED                   0x02
#define XL_MOST150_STREAM_STATE_STARTED                  0x03
#define XL_MOST150_STREAM_STATE_STOPPED                  0x04
#define XL_MOST150_STREAM_STATE_START_PENDING            0x05 // waiting for result from hw
#define XL_MOST150_STREAM_STATE_STOP_PENDING             0x06 // waiting for result from hw
#define XL_MOST150_STREAM_STATE_OPEN_PENDING             0x07 // waiting for result from hw
#define XL_MOST150_STREAM_STATE_CLOSE_PENDING            0x08 // waiting for result from hw

// TX Streaming: Maximum number of bytes that can be streamed per MOST frame
#define XL_MOST150_STREAM_TX_BYTES_PER_FRAME_MIN         1
#define XL_MOST150_STREAM_TX_BYTES_PER_FRAME_MAX         152

// RX Streaming: Maximum number of connection labels that can be streamed
#define XL_MOST150_STREAM_RX_NUM_CL_MAX                  8

// valid connection label range
#define XL_MOST150_STREAM_CL_MIN                         (unsigned int) 0x000C
#define XL_MOST150_STREAM_CL_MAX                         (unsigned int) 0x017F

// XL_MOST150_STREAM_STATE_EV.streamError
// XL_MOST150_STREAM_TX_LABEL_EV.errorInfo
#define XL_MOST150_STREAM_STATE_ERROR_NO_ERROR           0
#define XL_MOST150_STREAM_STATE_ERROR_NOT_ENOUGH_BW      1
#define XL_MOST150_STREAM_STATE_ERROR_NET_OFF            2
#define XL_MOST150_STREAM_STATE_ERROR_CONFIG_NOT_OK      3
#define XL_MOST150_STREAM_STATE_ERROR_CL_DISAPPEARED     4
#define XL_MOST150_STREAM_STATE_ERROR_INIC_SC_ERROR      5
#define XL_MOST150_STREAM_STATE_ERROR_DEVICEMODE_BYPASS  6
#define XL_MOST150_STREAM_STATE_ERROR_NISTATE_NOT_NETON  7
#define XL_MOST150_STREAM_STATE_ERROR_INIC_BUSY          8
#define XL_MOST150_STREAM_STATE_ERROR_CL_MISSING         9
#define XL_MOST150_STREAM_STATE_ERROR_NUM_BYTES_MISMATCH 10
#define XL_MOST150_STREAM_STATE_ERROR_INIC_COMMUNICATION 11

// XL_MOST150_STREAM_TX_BUFFER_EV.status
#define XL_MOST150_STREAM_BUFFER_ERROR_NO_ERROR          0
#define XL_MOST150_STREAM_BUFFER_ERROR_NOT_ENOUGH_DATA   1
#define XL_MOST150_STREAM_BUFFER_TX_FIFO_CLEARED         2

// XL_MOST150_STREAM_RX_BUFFER_EV.status
//#define XL_MOST150_STREAM_BUFFER_ERROR_NO_ERROR          0
#define XL_MOST150_STREAM_BUFFER_ERROR_STOP_BY_APP       1
#define XL_MOST150_STREAM_BUFFER_ERROR_MOST_SIGNAL_OFF   2
#define XL_MOST150_STREAM_BUFFER_ERROR_UNLOCK            3
#define XL_MOST150_STREAM_BUFFER_ERROR_CL_MISSING        4
#define XL_MOST150_STREAM_BUFFER_ERROR_ALL_CL_MISSING    5
#define XL_MOST150_STREAM_BUFFER_ERROR_OVERFLOW          128 // overflow bit

// latency values
#define XL_MOST150_STREAM_LATENCY_VERY_LOW               0
#define XL_MOST150_STREAM_LATENCY_LOW                    1
#define XL_MOST150_STREAM_LATENCY_MEDIUM                 2
#define XL_MOST150_STREAM_LATENCY_HIGH                   3
#define XL_MOST150_STREAM_LATENCY_VERY_HIGH              4


// bypass stress maximum/minimum timing parameter in msec
#define XL_MOST150_BYPASS_STRESS_TIME_MIN                10
#define XL_MOST150_BYPASS_STRESS_TIME_MAX                65535

// XL_MOST150_GEN_BYPASS_STRESS_EV.stressStarted
#define XL_MOST150_BYPASS_STRESS_STOPPED                 0
#define XL_MOST150_BYPASS_STRESS_STARTED                 1
#define XL_MOST150_BYPASS_STRESS_STOPPED_LIGHT_OFF       2
#define XL_MOST150_BYPASS_STRESS_STOPPED_DEVICE_MODE     3


// xlMost150SetSSOResult
// XL_MOST150_SSO_RESULT_EV.status
#define XL_MOST150_SSO_RESULT_NO_RESULT                  (unsigned int) 0x00000000
#define XL_MOST150_SSO_RESULT_NO_FAULT_SAVED             (unsigned int) 0x00000001
#define XL_MOST150_SSO_RESULT_SUDDEN_SIGNAL_OFF          (unsigned int) 0x00000002
#define XL_MOST150_SSO_RESULT_CRITICAL_UNLOCK            (unsigned int) 0x00000003


#include <pshpack1.h>

//////////////////////////////////////////////////////////////
//  Structures for MOST150 events
//////////////////////////////////////////////////////////////

typedef unsigned short    XLmostEventTag; 


typedef struct s_xl_most150_event_source{
  unsigned int sourceMask;
} XL_MOST150_EVENT_SOURCE_EV;

typedef struct s_xl_most150_device_mode {
  unsigned int deviceMode;
} XL_MOST150_DEVICE_MODE_EV;

typedef struct s_xl_most150_frequency {
  unsigned int frequency;
} XL_MOST150_FREQUENCY_EV;

typedef struct s_xl_most150_special_node_info{
  unsigned int   changeMask;
  unsigned short nodeAddress;
  unsigned short groupAddress;
  unsigned char  npr;
  unsigned char  mpr;
  unsigned char  sbc;
  unsigned char  ctrlRetryTime;
  unsigned char  ctrlSendAttempts;
  unsigned char  asyncRetryTime;
  unsigned char  asyncSendAttempts;
  unsigned char  macAddr[6];
  unsigned char  nprSpy;
  unsigned char  mprSpy;
  unsigned char  sbcSpy;
  unsigned char  inicNIState;
  unsigned char  reserved1[3];
  unsigned int   reserved2[3];
} XL_MOST150_SPECIAL_NODE_INFO_EV;

typedef struct s_xl_most150_ctrl_rx {
  unsigned short targetAddress;
  unsigned short sourceAddress;
  unsigned char  fblockId;
  unsigned char  instId;
  unsigned short functionId;
  unsigned char  opType;
  unsigned char  telId;
  unsigned short telLen;
  unsigned char  ctrlData[45];
} XL_MOST150_CTRL_RX_EV;

typedef struct s_xl_most150_ctrl_spy{
  unsigned int   frameCount;
  unsigned int   msgDuration; // duration of message transmission in [ns]
  unsigned char  priority;
  unsigned short targetAddress;
  unsigned char  pAck;
  unsigned short ctrlDataLenAnnounced;
  unsigned char  reserved0;
  unsigned char  pIndex;
  unsigned short sourceAddress;
  unsigned short reserved1;
  unsigned short crc;
  unsigned short crcCalculated;
  unsigned char  cAck;
  unsigned short ctrlDataLen; // number of bytes contained in ctrlData[]
  unsigned char  reserved2;
  unsigned int   status; // currently not used
  unsigned int   validMask;
  unsigned char  ctrlData[51];
} XL_MOST150_CTRL_SPY_EV;

typedef struct s_xl_most150_async_rx_msg {
  unsigned short length;
  unsigned short targetAddress;
  unsigned short sourceAddress;
  unsigned char  asyncData[1524];
} XL_MOST150_ASYNC_RX_EV;

typedef struct s_xl_most150_async_spy_msg {
  unsigned int   frameCount;
  unsigned int   pktDuration; // duration of data packet transmission in [ns]
  unsigned short asyncDataLenAnnounced; 
  unsigned short targetAddress;
  unsigned char  pAck;
  unsigned char  pIndex;
  unsigned short sourceAddress;
  unsigned int   crc;
  unsigned int   crcCalculated;
  unsigned char  cAck;
  unsigned short asyncDataLen; // number of bytes contained in asyncData[]
  unsigned char  reserved;
  unsigned int   status; // currently not used
  unsigned int   validMask;
  unsigned char  asyncData[1524];
} XL_MOST150_ASYNC_SPY_EV;

typedef struct s_xl_most150_ethernet_rx {
  unsigned char sourceAddress[6];
  unsigned char targetAddress[6];
  unsigned int  length;
  unsigned char ethernetData[1510];
} XL_MOST150_ETHERNET_RX_EV;

typedef struct s_xl_most150_ethernet_spy {
  unsigned int   frameCount;
  unsigned int   pktDuration; // duration of ethernet packet transmission in [ns]
  unsigned short ethernetDataLenAnnounced; 
  unsigned char  targetAddress[6];
  unsigned char  pAck;
  unsigned char  sourceAddress[6];
  unsigned char  reserved0;
  unsigned int   crc;
  unsigned int   crcCalculated;
  unsigned char  cAck;
  unsigned short ethernetDataLen; // number of bytes contained in ethernetData[]
  unsigned char  reserved1;
  unsigned int   status; // currently not used
  unsigned int   validMask;
  unsigned char  ethernetData[1506];
} XL_MOST150_ETHERNET_SPY_EV;

typedef struct s_xl_most150_cl_info {
  unsigned short label;
  unsigned short channelWidth;
} XL_MOST150_CL_INFO;

typedef struct s_xl_most150_sync_alloc_info {
  XL_MOST150_CL_INFO  allocTable[MOST150_SYNC_ALLOC_INFO_SIZE];
} XL_MOST150_SYNC_ALLOC_INFO_EV;


typedef struct s_xl_most150_sync_volume_status {
 unsigned int  device;
 unsigned int  volume;
} XL_MOST150_SYNC_VOLUME_STATUS_EV;

typedef struct s_xl_most150_tx_light {
  unsigned int light;
} XL_MOST150_TX_LIGHT_EV;

typedef struct s_xl_most150_rx_light_lock_status {
  unsigned int status;
} XL_MOST150_RXLIGHT_LOCKSTATUS_EV;

typedef struct s_xl_most150_error {
  unsigned int errorCode;
  unsigned int parameter[3];
} XL_MOST150_ERROR_EV;

typedef struct s_xl_most150_configure_rx_buffer {
  unsigned int bufferType;
  unsigned int bufferMode;
} XL_MOST150_CONFIGURE_RX_BUFFER_EV;

typedef struct s_xl_most150_ctrl_sync_audio {
  unsigned int label; 
  unsigned int width; 
  unsigned int device; 
  unsigned int mode;
} XL_MOST150_CTRL_SYNC_AUDIO_EV;

typedef struct s_xl_most150_sync_mute_status {
  unsigned int device;
  unsigned int mute;
} XL_MOST150_SYNC_MUTE_STATUS_EV;

typedef struct s_xl_most150_tx_light_power {
  unsigned int lightPower;
} XL_MOST150_LIGHT_POWER_EV;

typedef struct s_xl_most150_gen_light_error {
  unsigned int stressStarted;
} XL_MOST150_GEN_LIGHT_ERROR_EV;

typedef struct s_xl_most150_gen_lock_error {
  unsigned int stressStarted;
} XL_MOST150_GEN_LOCK_ERROR_EV;

typedef struct s_xl_most150_ctrl_busload {
  unsigned long busloadStarted; 
} XL_MOST150_CTRL_BUSLOAD_EV;

typedef struct s_xl_most150_async_busload {
  unsigned long busloadStarted; 
} XL_MOST150_ASYNC_BUSLOAD_EV;

typedef struct s_xl_most150_systemlock_flag {
  unsigned int state;
} XL_MOST150_SYSTEMLOCK_FLAG_EV;

typedef struct s_xl_most150_shutdown_flag {
  unsigned int state;
} XL_MOST150_SHUTDOWN_FLAG_EV;

typedef struct s_xl_most150_spdif_mode {
  unsigned int spdifMode;
  unsigned int spdifError;
} XL_MOST150_SPDIF_MODE_EV;

typedef struct s_xl_most150_ecl {
  unsigned int eclLineState;
} XL_MOST150_ECL_EV;

typedef struct s_xl_most150_ecl_termination {
  unsigned int resistorEnabled;
} XL_MOST150_ECL_TERMINATION_EV;

typedef struct s_xl_most150_nw_startup {
  unsigned int error;
  unsigned int errorInfo;
} XL_MOST150_NW_STARTUP_EV;

typedef struct s_xl_most150_nw_shutdown {
  unsigned int error;
  unsigned int errorInfo;
} XL_MOST150_NW_SHUTDOWN_EV;

typedef struct s_xl_most150_stream_state {
  unsigned int streamHandle;
  unsigned int streamState;
  unsigned int streamError;
} XL_MOST150_STREAM_STATE_EV;

typedef struct s_xl_most150_stream_tx_buffer {
  unsigned int streamHandle;
  unsigned int numberOfBytes;
  unsigned int status;
} XL_MOST150_STREAM_TX_BUFFER_EV;

typedef struct s_xl_most150_stream_rx_buffer {
  unsigned int streamHandle;
  unsigned int numberOfBytes;
  unsigned int status;
  unsigned int labelInfo;
} XL_MOST150_STREAM_RX_BUFFER_EV;

typedef struct s_xl_most150_stream_tx_underflow {
  unsigned int streamHandle;
  unsigned int reserved;
} XL_MOST150_STREAM_TX_UNDERFLOW_EV;

typedef struct s_xl_most150_stream_tx_label {
  unsigned int streamHandle;
  unsigned int errorInfo;
  unsigned int connLabel;
  unsigned int width;
} XL_MOST150_STREAM_TX_LABEL_EV;

typedef struct s_xl_most150_gen_bypass_stress {
  unsigned int stressStarted;
} XL_MOST150_GEN_BYPASS_STRESS_EV;

typedef struct s_xl_most150_ecl_sequence {
  unsigned int sequenceStarted;
} XL_MOST150_ECL_SEQUENCE_EV;

typedef struct s_xl_most150_ecl_glitch_filter {
  unsigned int duration;
} XL_MOST150_ECL_GLITCH_FILTER_EV;

typedef struct s_xl_most150_sso_result {
  unsigned int status;
} XL_MOST150_SSO_RESULT_EV;


typedef struct s_xl_most150_ctrl_tx_ack {
  unsigned short targetAddress;
  unsigned short sourceAddress;
  unsigned char  ctrlPrio;
  unsigned char  ctrlSendAttempts;
  unsigned char  reserved[2];
  unsigned int   status;

  // ctrlData structure:
  // -----------------------------------------------------------------------
  // FBlockID | InstID | FunctionID | OpType | TelID | TelLen | Payload
  // -----------------------------------------------------------------------
  //  8 bit   | 8 bit  |   12 bit   | 4 bit  | 4 bit | 12 bit | 0 .. 45 byte
  // -----------------------------------------------------------------------
  // ctrlData[0]:	FBlockID
  // ctrlData[1]:	InstID
  // ctrlData[2]:	FunctionID (upper 8 bits)
  // ctrlData[3]:	FunctionID (lower 4 bits) + OpType (4 bits) 
  // ctrlData[4]:	TelId (4 bits) + TelLen (upper 4 bits)
  // ctrlData[5]:	TelLen (lower 8 bits)
  // ctrlData[6..50]: Payload
  unsigned char  ctrlData[51];
} XL_MOST150_CTRL_TX_ACK_EV;

typedef struct s_xl_most150_async_tx_ack {
  unsigned char  priority;
  unsigned char  asyncSendAttempts;
  unsigned short length; 
  unsigned short targetAddress;
  unsigned short sourceAddress;
  unsigned int   status;
  unsigned char  asyncData[1524];
} XL_MOST150_ASYNC_TX_ACK_EV;

typedef struct s_xl_most150_ethernet_tx {
  unsigned char priority;
  unsigned char ethSendAttempts;
  unsigned char sourceAddress[6];
  unsigned char targetAddress[6];
  unsigned char reserved[2];
  unsigned int  length;
  unsigned char ethernetData[1510];
} XL_MOST150_ETHERNET_TX_ACK_EV;

typedef struct s_xl_most150_hw_sync {
  unsigned int pulseCode;
} XL_MOST150_HW_SYNC_EV;

typedef struct s_xl_event_most150 {

  unsigned int                          size;             // 4 - overall size of the complete event 
  XLmostEventTag                        tag;              // 2 - type of the event 
  unsigned short                        channelIndex;     // 2 
  unsigned int                          userHandle;       // 4 
  unsigned short                        flagsChip;        // 2
  unsigned short                        reserved;         // 2
  XLuint64                              timeStamp;        // 8 - raw timestamp
  XLuint64                              timeStampSync;    // 8 - timestamp which is synchronized by the driver
                                                          // --------- 
                                                          // 32 bytes -> XL_MOST_EVENT_HEADER_SIZE 
  union {
    unsigned char                     rawData[XL_MOST150_MAX_EVENT_DATA_SIZE]; 
    XL_MOST150_EVENT_SOURCE_EV        mostEventSource;
    XL_MOST150_DEVICE_MODE_EV         mostDeviceMode;
    XL_MOST150_FREQUENCY_EV           mostFrequency;
    XL_MOST150_SPECIAL_NODE_INFO_EV   mostSpecialNodeInfo;
    XL_MOST150_CTRL_RX_EV             mostCtrlRx;
    XL_MOST150_CTRL_TX_ACK_EV         mostCtrlTxAck;
    XL_MOST150_ASYNC_SPY_EV           mostAsyncSpy;
    XL_MOST150_ASYNC_RX_EV            mostAsyncRx;
    XL_MOST150_SYNC_ALLOC_INFO_EV     mostSyncAllocInfo;
    XL_MOST150_SYNC_VOLUME_STATUS_EV  mostSyncVolumeStatus;
    XL_MOST150_TX_LIGHT_EV            mostTxLight;
    XL_MOST150_RXLIGHT_LOCKSTATUS_EV  mostRxLightLockStatus;
    XL_MOST150_ERROR_EV               mostError;
    XL_MOST150_CONFIGURE_RX_BUFFER_EV mostConfigureRxBuffer;
    XL_MOST150_CTRL_SYNC_AUDIO_EV     mostCtrlSyncAudio;
    XL_MOST150_SYNC_MUTE_STATUS_EV    mostSyncMuteStatus;
    XL_MOST150_LIGHT_POWER_EV         mostLightPower;
    XL_MOST150_GEN_LIGHT_ERROR_EV     mostGenLightError;
    XL_MOST150_GEN_LOCK_ERROR_EV      mostGenLockError;
    XL_MOST150_CTRL_BUSLOAD_EV        mostCtrlBusload;
    XL_MOST150_ASYNC_BUSLOAD_EV       mostAsyncBusload;
    XL_MOST150_ETHERNET_RX_EV         mostEthernetRx;
    XL_MOST150_SYSTEMLOCK_FLAG_EV     mostSystemLockFlag;
    XL_MOST150_SHUTDOWN_FLAG_EV       mostShutdownFlag;
    XL_MOST150_SPDIF_MODE_EV          mostSpdifMode;
    XL_MOST150_ECL_EV                 mostEclEvent;
    XL_MOST150_ECL_TERMINATION_EV     mostEclTermination;
    XL_MOST150_CTRL_SPY_EV            mostCtrlSpy;
    XL_MOST150_ASYNC_TX_ACK_EV        mostAsyncTxAck;
    XL_MOST150_ETHERNET_SPY_EV        mostEthernetSpy;
    XL_MOST150_ETHERNET_TX_ACK_EV     mostEthernetTxAck;
    XL_MOST150_HW_SYNC_EV             mostHWSync;
    XL_MOST150_NW_STARTUP_EV          mostStartup;
    XL_MOST150_NW_SHUTDOWN_EV         mostShutdown;
    XL_MOST150_STREAM_STATE_EV        mostStreamState;
    XL_MOST150_STREAM_TX_BUFFER_EV    mostStreamTxBuffer;
    XL_MOST150_STREAM_RX_BUFFER_EV    mostStreamRxBuffer;
    XL_MOST150_STREAM_TX_UNDERFLOW_EV mostStreamTxUnderflow;
    XL_MOST150_STREAM_TX_LABEL_EV     mostStreamTxLabel;
    XL_MOST150_GEN_BYPASS_STRESS_EV   mostGenBypassStress;
    XL_MOST150_ECL_SEQUENCE_EV        mostEclSequence;
    XL_MOST150_ECL_GLITCH_FILTER_EV   mostEclGlitchFilter;
    XL_MOST150_SSO_RESULT_EV          mostSsoResult;
  } tagData;
} XLmost150event;


///////////////////////////////////////////////////////////////////////////////////
// Structures for MOST150 API commands
///////////////////////////////////////////////////////////////////////////////////

//XLstatus xlMost150SetSpecialNodeInfo(DEFPARAMS, XLmost150SetSpecialNodeInfo *specialodeInfo);
typedef struct s_xl_set_most150_special_node_info {
  unsigned int  changeMask; // see XL_MOST150_SPECIAL_NODE_MASK_CHANGED
  unsigned int  nodeAddress;
  unsigned int  groupAddress;
  unsigned int  sbc;
  unsigned int  ctrlRetryTime;
  unsigned int  ctrlSendAttempts;
  unsigned int  asyncRetryTime;
  unsigned int  asyncSendAttempts;
  unsigned char  macAddr[6];
} XLmost150SetSpecialNodeInfo;

//XLstatus xlMost150CtrlTransmit(DEFPARAMS, XLmost150CtrlTxMsg *pCtrlTxMsg);
typedef struct s_xl_most150_ctrl_tx_msg {
  unsigned int   ctrlPrio;         // Prio: Currently fixed to 0x01 for Control Messages
  unsigned int   ctrlSendAttempts; // 1..16 attempts, set an invalid value to use the default value set by xlMost150SetCtrlRetryParameters
  unsigned int   targetAddress;

  // ctrlData structure:
  // -----------------------------------------------------------------------
  // FBlockID | InstID | FunctionID | OpType | TelID | TelLen | Payload
  // -----------------------------------------------------------------------
  //  8 bit   | 8 bit  |   12 bit   | 4 bit  | 4 bit | 12 bit | 0 .. 45 byte
  // -----------------------------------------------------------------------
  // ctrlData[0]:	FBlockID
  // ctrlData[1]:	InstID
  // ctrlData[2]:	FunctionID (upper 8 bits)
  // ctrlData[3]:	FunctionID (lower 4 bits) + OpType (4 bits) 
  // ctrlData[4]:	TelId (4 bits) + TelLen (upper 4 bits)
  // ctrlData[5]:	TelLen (lower 8 bits)
  // ctrlData[6..50]: Payload
  unsigned char  ctrlData[51];
} XLmost150CtrlTxMsg;

//XLstatus xlMost150AsyncTransmit(DEFPARAMS, XLmost150AsyncTxMsg *pAsyncTxMsg);
typedef struct s_xl_most150_async_tx_msg {
  unsigned int   priority;          // Prio: Currently fixed to 0x00 for MDP /MEP
  unsigned int   asyncSendAttempts; // 1..16 attempts,set an invalid value to use the default value set by xlMost150SetAsyncRetryParameters
  unsigned int   length;            // max. 1600 bytes
  unsigned int   targetAddress;
  unsigned char  asyncData[XL_MOST150_ASYNC_SEND_PAYLOAD_MAX_SIZE];
} XLmost150AsyncTxMsg;

//XLstatus xlMost150EthernetTransmit(DEFPARAMS, XLmost150EthernetTxMsg  *pEthernetTxMsg);
typedef struct s_xl_most150_ethernet_tx_msg {
  unsigned int   priority;           // Prio: Currently fixed to 0x00 for MDP /MEP
  unsigned int   ethSendAttempts;    // 1..16 attempts, set an invalid value to use the default value set by xlMost150SetAsyncRetryParameters
  unsigned char  sourceAddress[6];
  unsigned char  targetAddress[6];
  unsigned int   length;             // max. 1600 bytes
  unsigned char  ethernetData[XL_MOST150_ETHERNET_SEND_PAYLOAD_MAX_SIZE];
} XLmost150EthernetTxMsg;

//XLstatus xlMost150CtrlSyncAudio(DEFPARAMS, XLmost150SyncAudioParameter *syncAudioParameter);
typedef struct s_xl_most150_sync_audio_parameter {
  unsigned int  label;
  unsigned int  width;
  unsigned int  device;
  unsigned int  mode;
} XLmost150SyncAudioParameter;

//XLstatus xlMost150CtrlConfigureBusload(DEFPARAMS, XLmost150CtrlBusloadConfig  *pCtrlBusloadConfig);
typedef struct s_xl_most150_ctrl_busload_config {
  unsigned int         transmissionRate;
  unsigned int         counterType;
  unsigned int         counterPosition; // counter can be only be set in the payload -> position 0 means first payload byte!
  XLmost150CtrlTxMsg   busloadCtrlMsg;
} XLmost150CtrlBusloadConfig;

//XLstatus xlMost150AsyncConfigureBusload(DEFPARAMS, XLmost150AsyncBusloadConfig  *pAsyncBusloadConfig);
typedef struct s_xl_most150_async_busload_config {
  unsigned int           busloadType;
  unsigned int           transmissionRate;
  unsigned int           counterType;
  unsigned int           counterPosition;
  union {
    unsigned char          rawBusloadPkt[1540]; 
    XLmost150AsyncTxMsg    busloadAsyncPkt;
    XLmost150EthernetTxMsg busloadEthernetPkt;
  } busloadPkt;
} XLmost150AsyncBusloadConfig;

//XLstatus xlMost150StreamOpen(DEFPARAMS, XLmost150StreamOpen*  pStreamOpen);
typedef struct s_xl_most150_stream_open {
  unsigned int* pStreamHandle;
  unsigned int  direction;
  unsigned int  numBytesPerFrame;
  unsigned int  reserved;
  unsigned int  latency;
} XLmost150StreamOpen;

//XLstatus xlMost150StreamGetInfo(DEFPARAMS, XLmost150StreamInfo*  pStreamInfo);
typedef struct s_xl_most150_stream_get_info {
  unsigned int  streamHandle;
  unsigned int  numBytesPerFrame;
  unsigned int  direction;
  unsigned int  reserved;
  unsigned int  latency;
  unsigned int  streamState;
  unsigned int  connLabels[XL_MOST150_STREAM_RX_NUM_CL_MAX];
} XLmost150StreamInfo;


#include <poppack.h>

#include <pshpack8.h>

///////////////////////////////////////////////////////////////////////
// CAN / CAN-FD types and definitions
///////////////////////////////////////////////////////////////////////

#define XL_CAN_MAX_DATA_LEN                  64
#define XL_CANFD_RX_EVENT_HEADER_SIZE        32 
#define XL_CANFD_MAX_EVENT_SIZE              128

////////////////////////////////////////////////////////////////////////
// get the number of databytes from dlc/edl/rtr in received events
#define CANFD_GET_NUM_DATABYTES(dlc, edl, rtr)   \
  ((rtr)?0:       \
  (dlc)<9?(dlc):  \
  !(edl)?8:       \
  (dlc)== 9?12:   \
  (dlc)==10?16:   \
  (dlc)==11?20:   \
  (dlc)==12?24:   \
  (dlc)==13?32:   \
  (dlc)==14?48:64)

// to be used with XLcanTxEvent::XL_CAN_TX_MSG::msgFlags
#define XL_CAN_TXMSG_FLAG_EDL                0x0001    // extended data length
#define XL_CAN_TXMSG_FLAG_BRS                0x0002    // baud rate switch
#define XL_CAN_TXMSG_FLAG_RTR                0x0010    // remote transmission request
#define XL_CAN_TXMSG_FLAG_HIGHPRIO           0x0080    // high priority message - clears all send buffers - then transmits
#define XL_CAN_TXMSG_FLAG_WAKEUP             0x0200    // generate a wakeup message


// to be used with
// XLcanRxEvent::XL_CAN_EV_RX_MSG::msgFlags
// XLcanRxEvent::XL_CAN_EV_TX_REQUEST::msgFlags
// XLcanRxEvent::XL_CAN_EV_RX_MSG::msgFlags
// XLcanRxEvent::XL_CAN_EV_TX_REMOVED::msgFlags
// XLcanRxEvent::XL_CAN_EV_ERROR::msgFlags
#define XL_CAN_RXMSG_FLAG_EDL                0x0001    // extended data length
#define XL_CAN_RXMSG_FLAG_BRS                0x0002    // baud rate switch
#define XL_CAN_RXMSG_FLAG_ESI                0x0004    // error state indicator
#define XL_CAN_RXMSG_FLAG_RTR                0x0010    // remote transmission request
#define XL_CAN_RXMSG_FLAG_EF                 0x0200    // error frame (only posssible in XL_CAN_EV_TX_REQUEST/XL_CAN_EV_TX_REMOVED)
#define XL_CAN_RXMSG_FLAG_ARB_LOST           0x0400    // Arbitration Lost
                                                       // set if the receiving node tried to transmit a message but lost arbitration process
#define XL_CAN_RXMSG_FLAG_WAKEUP             0x2000    // high voltage message on single wire CAN
#define XL_CAN_RXMSG_FLAG_TE                 0x4000    // 1: transceiver error detected



////////////////////////////////////////////////////////////////////////
// CAN / CAN-FD tx event definitions
////////////////////////////////////////////////////////////////////////

typedef struct {
  unsigned int       canId;
  unsigned int       msgFlags;
  unsigned char      dlc;
  unsigned char      reserved[7];
  unsigned char      data[XL_CAN_MAX_DATA_LEN];
} XL_CAN_TX_MSG;

typedef struct {
  unsigned short     tag;              //  2 - type of the event
  unsigned short     transId;          //  2
  unsigned char      channelIndex;     //  1 - internal has to be 0
  unsigned char      reserved[3];      //  3 - has to be zero 

  union {
    XL_CAN_TX_MSG   canMsg;
  } tagData;
} XLcanTxEvent;



////////////////////////////////////////////////////////////////////////
// CAN / CAN-FD rx event definitions
////////////////////////////////////////////////////////////////////////

// used with XL_CAN_EV_TAG_RX_OK, XL_CAN_EV_TAG_TX_OK
typedef struct {
  unsigned int    canId;
  unsigned int    msgFlags;
  unsigned int    crc;
  unsigned char   reserved1[12];
  unsigned short  totalBitCnt;
  unsigned char   dlc;        
  unsigned char   reserved[5];
  unsigned char   data[XL_CAN_MAX_DATA_LEN];
} XL_CAN_EV_RX_MSG;

typedef struct {
  unsigned int    canId;
  unsigned int    msgFlags;
  unsigned char   dlc;
  unsigned char   reserved1;
  unsigned short  reserved;
  unsigned char   data[XL_CAN_MAX_DATA_LEN];
} XL_CAN_EV_TX_REQUEST;


// to be used with XL_CAN_EV_TAG_CHIP_STATE
typedef struct {
  unsigned char   busStatus;
  unsigned char   txErrorCounter;
  unsigned char   rxErrorCounter;
  unsigned char   reserved;
  unsigned int    reserved0;
} XL_CAN_EV_CHIP_STATE;


typedef XL_SYNC_PULSE_EV XL_CAN_EV_SYNC_PULSE;

// to be used with XL_CAN_EV_ERROR::errorCode
#define XL_CAN_ERRC_BIT_ERROR              1 
#define XL_CAN_ERRC_FORM_ERROR             2
#define XL_CAN_ERRC_STUFF_ERROR            3
#define XL_CAN_ERRC_OTHER_ERROR            4
#define XL_CAN_ERRC_CRC_ERROR              5
#define XL_CAN_ERRC_ACK_ERROR              6
#define XL_CAN_ERRC_NACK_ERROR             7
#define XL_CAN_ERRC_OVLD_ERROR             8
#define XL_CAN_ERRC_EXCPT_ERROR            9

//to be used with XL_CAN_EV_TAG_RX_ERROR/XL_CAN_EV_TAG_TX_ERROR
typedef struct {
  unsigned char  errorCode;
  unsigned char  reserved[95];
} XL_CAN_EV_ERROR;

// to be used with XLcanRxEvent::flagsChip
#define XL_CAN_QUEUE_OVERFLOW          0x100

// max./min size of application rx fifo (bytes)
#define RX_FIFO_CANFD_QUEUE_SIZE_MAX                         524288   // 0,5 MByte
#define RX_FIFO_CANFD_QUEUE_SIZE_MIN                         8192     // 8 kByte


//------------------------------------------------------------------------------
// General RX Event
typedef struct {
  unsigned int         size;             // 4 - overall size of the complete event
  unsigned short       tag;              // 2 - type of the event
  unsigned short       channelIndex;     // 2        
  unsigned int         userHandle;       // 4 (lower 12 bit available for CAN)
  unsigned short       flagsChip;        // 2 queue overflow (upper 8bit)
  unsigned short       reserved0;        // 2
  XLuint64             reserved1;        // 8 
  XLuint64             timeStampSync;    // 8 - timestamp which is synchronized by the driver

  union {
    unsigned char             raw[XL_CANFD_MAX_EVENT_SIZE - XL_CANFD_RX_EVENT_HEADER_SIZE];
    XL_CAN_EV_RX_MSG          canRxOkMsg;
    XL_CAN_EV_RX_MSG          canTxOkMsg;
    XL_CAN_EV_TX_REQUEST      canTxRequest;

    XL_CAN_EV_ERROR           canError;
    XL_CAN_EV_CHIP_STATE      canChipState;
    XL_CAN_EV_SYNC_PULSE      canSyncPulse;
  } tagData;
} XLcanRxEvent;
#include <poppack.h>

#include <pshpack8.h>

/* ============================================================================== */
/*                                                                                */
/* (2)   DEFINES / MACROS                                                         */
/*                                                                                */
/* ============================================================================== */

///////////////////////////////////////////////////////////////////////
// ARINC429 types and definitions
///////////////////////////////////////////////////////////////////////

// defines for XL_A429_PARAMS::channelDirection
#define XL_A429_MSG_CHANNEL_DIR_TX                  0x01
#define XL_A429_MSG_CHANNEL_DIR_RX                  0x02

// to be used with XL_A429_PARAMS::data::tx::bitrate
#define XL_A429_MSG_BITRATE_SLOW_MIN               10500
#define XL_A429_MSG_BITRATE_SLOW_MAX               16000
#define XL_A429_MSG_BITRATE_FAST_MIN               90000
#define XL_A429_MSG_BITRATE_FAST_MAX              110000

// to be used with XL_A429_PARAMS::data::tx/rx::minGap
#define XL_A429_MSG_GAP_4BIT                          32

// to be used with XL_A429_PARAMS::rx::minBitrate/maxBitrate
#define XL_A429_MSG_BITRATE_RX_MIN                 10000
#define XL_A429_MSG_BITRATE_RX_MAX                120000

// to be used with XL_A429_PARAMS::rx::autoBaudrate
#define XL_A429_MSG_AUTO_BAUDRATE_DISABLED             0
#define XL_A429_MSG_AUTO_BAUDRATE_ENABLED              1

// to be used with XL_A429_TX_MSG::flags
#define XL_A429_MSG_FLAG_ON_REQUEST           0x00000001
#define XL_A429_MSG_FLAG_CYCLIC               0x00000002
#define XL_A429_MSG_FLAG_DELETE_CYCLIC        0x00000004

// to be used with XL_A429_TX_MSG::cycleTime
#define	XL_A429_MSG_CYCLE_MAX                 0x3FFFFFFF

// to be used with XL_A429_TX_MSG::gap
#define	XL_A429_MSG_GAP_DEFAULT                        0 // get minGap config from set channel params
#define	XL_A429_MSG_GAP_MAX                   0x000FFFFF

// to be used with XL_A429_PARAMS::data::parity
// to be used with XL_A429_TX_MSG::parity
#define XL_A429_MSG_PARITY_DEFAULT                     0 // get parity config from set channel params
#define XL_A429_MSG_PARITY_DISABLED                    1 // tx: get parity config from transmit data - rx: check disabled
#define XL_A429_MSG_PARITY_ODD                         2
#define XL_A429_MSG_PARITY_EVEN                        3

// to be used with XLa429RxEvent::XL_A429_EV_TX_OK::msgCtrl
#define XL_A429_EV_TX_MSG_CTRL_ON_REQUEST              0
#define XL_A429_EV_TX_MSG_CTRL_CYCLIC                  1

// to be used with XLa429RxEvent::XL_A429_EV_TX_ERR::errorReason
#define XL_A429_EV_TX_ERROR_ACCESS_DENIED              0
#define XL_A429_EV_TX_ERROR_TRANSMISSION_ERROR         1

// to be used with XLa429RxEvent::XL_A429_EV_RX_ERR::errorReason
#define XL_A429_EV_RX_ERROR_GAP_VIOLATION              0
#define XL_A429_EV_RX_ERROR_PARITY                     1
#define	XL_A429_EV_RX_ERROR_BITRATE_LOW                2
#define XL_A429_EV_RX_ERROR_BITRATE_HIGH               3
#define XL_A429_EV_RX_ERROR_FRAME_FORMAT               4
#define	XL_A429_EV_RX_ERROR_CODING_RZ                  5
#define	XL_A429_EV_RX_ERROR_DUTY_FACTOR                6
#define	XL_A429_EV_RX_ERROR_AVG_BIT_LENGTH             7

// to be used with XLa429RxEvent::flagsChip
#define XL_A429_QUEUE_OVERFLOW                     0x100

// max./min size of application rx fifo (bytes)
#define XL_A429_RX_FIFO_QUEUE_SIZE_MAX            524288 // 0,5 MByte
#define XL_A429_RX_FIFO_QUEUE_SIZE_MIN              8192 // 8 kByte

/* ============================================================================== */
/*                                                                                */
/* (3)  TYPE DEFINITIONS                                                          */
/*                                                                                */
/* ============================================================================== */

////////////////////////////////////////////////////////////////////////
// ARINC429 paramter configuration definitions
////////////////////////////////////////////////////////////////////////

typedef struct s_xl_a429_params {
  unsigned short channelDirection;
  unsigned short res1;
  union {
    struct {
      unsigned int bitrate;
      unsigned int parity;
      unsigned int minGap;
    } tx;
    struct {
      unsigned int bitrate;
      unsigned int minBitrate;
      unsigned int maxBitrate;
      unsigned int parity;
      unsigned int minGap;
      unsigned int autoBaudrate;
    } rx;
    unsigned char raw[28];
  } data;
} XL_A429_PARAMS;

////////////////////////////////////////////////////////////////////////
// ARINC429 tx event definitions
////////////////////////////////////////////////////////////////////////

typedef struct s_xl_a429_msg_tx {
  unsigned short     userHandle;
  unsigned short     res1;
  unsigned int       flags;
  unsigned int       cycleTime;
  unsigned int       gap;
  unsigned char      label;
  unsigned char      parity;
  unsigned short     res2;
  unsigned int       data;
} XL_A429_MSG_TX;

////////////////////////////////////////////////////////////////////////
// ARINC429 rx event definitions
////////////////////////////////////////////////////////////////////////

// used with XL_A429_EV_TAG_TX_OK
typedef struct s_xl_a429_ev_tx_ok {
  unsigned int       frameLength;
  unsigned int       bitrate;
  unsigned char      label;
  unsigned char      msgCtrl;
  unsigned short     res1;
  unsigned int       data;
} XL_A429_EV_TX_OK;

// used with XL_A429_EV_TAG_TX_ERR
typedef struct s_xl_a429_ev_tx_err {
  unsigned int       frameLength;
  unsigned int       bitrate;
  unsigned char      errorPosition;
  unsigned char      errorReason;
  unsigned char      label;
  unsigned char      res1;
  unsigned int       data;
} XL_A429_EV_TX_ERR;

// used with XL_A429_EV_TAG_RX_OK
typedef struct s_xl_a429_ev_rx_ok {
  unsigned int       frameLength;
  unsigned int       bitrate;
  unsigned char      label;
  unsigned char      res1[3]; 
  unsigned int       data;
} XL_A429_EV_RX_OK;

// used with XL_A429_EV_TAG_RX_ERR
typedef struct s_xl_a429_ev_rx_err {
  unsigned int       frameLength;
  unsigned int       bitrate;
  unsigned int       bitLengthOfLastBit;
  unsigned char      errorPosition;
  unsigned char      errorReason;
  unsigned char      label;
  unsigned char      res1; 
  unsigned int       data;
} XL_A429_EV_RX_ERR;

// used with XL_A429_EV_TAG_BUS_STATISTIC
typedef struct s_xl_a429_ev_bus_statistic {
  unsigned int        busLoad; // 0.00-100.00%
  unsigned int        res1[3];
} XL_A429_EV_BUS_STATISTIC;

typedef XL_SYNC_PULSE_EV XL_A429_EV_SYNC_PULSE;

typedef struct {
  unsigned int         size;             // 4 - overall size of the complete event
  unsigned short       tag;              // 2 - type of the event
  unsigned char        channelIndex;     // 1
  unsigned char        reserved;         // 1        
  unsigned int         userHandle;       // 4 (lower 12 bit available for CAN)
  unsigned short       flagsChip;        // 2 queue overflow (upper 8bit)
  unsigned short       reserved0;        // 1 
  XLuint64             timeStamp;        // 8 - raw timestamp
  XLuint64             timeStampSync;    // 8 - timestamp which is synchronized by the driver

  union {
    XL_A429_EV_TX_OK           a429TxOkMsg;
    XL_A429_EV_TX_ERR          a429TxErrMsg;
    XL_A429_EV_RX_OK           a429RxOkMsg;
    XL_A429_EV_RX_ERR          a429RxErrMsg;
    XL_A429_EV_BUS_STATISTIC   a429BusStatistic;
    XL_A429_EV_SYNC_PULSE      a429SyncPulse;
  } tagData;
} XLa429RxEvent;

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
This is used to unload the driver, if no more application is using it.
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
The timer of the port will be activated/deactivated and the
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
for timer event generation out of the channels specified by parameter 
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
Setup an event to notify the application if there are messages in the
ports receive queue.
queueLevel specifies the number of messages that triggers the event.
Note that the event is triggered only once, when the queueLevel is
reached. An application should read all available messages by xlReceive
to be sure to re enable the event. The API generates the handle by
itself. For LIN the queueLevel is fix to one.
*/

DECL_STDXL_FUNC ( xlSetNotification, XLSETNOTIFICATION, (
                  XLportHandle  portHandle,
                  XLhandle      *pHandle,
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
messages, on return it sets number of really received messages (messages
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
xlCanGetEventString(): 
--------------------------------------------------------------------------------
Utility Functions
*/
#ifdef DYNAMIC_XLDRIVER_DLL
  typedef XLstringType (_XL_EXPORT_API *XLGETERRORSTRING) ( XLstatus err );
  typedef XLstringType (_XL_EXPORT_API *XLGETEVENTSTRING)( XLevent *pEv );
  typedef XLstringType (_XL_EXPORT_API *XLCANGETEVENTSTRING)( XLcanRxEvent *pEv );
#else
  XLstringType _XL_EXPORT_DECL xlGetErrorString( XLstatus err );
  XLstringType _XL_EXPORT_DECL xlGetEventString( XLevent *pEv );
  XLstringType _XL_EXPORT_DECL xlCanGetEventString( XLcanRxEvent *pEv );
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
xlGetChannelTime():
--------------------------------------------------------------------------------
Function reads the 64-bit PC-based card time.
*/

DECL_STDXL_FUNC ( xlGetChannelTime,   XLGETCHANNELTIME, (
                  XLportHandle        portHandle, 
                  XLaccess            accessMask,
                  XLuint64            *pChannelTime )
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
 *  \param type           [IN] Reserved. Should always be set to zero!
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

//------------------------------------------------------------------------------
// xlCanFdSetConfiguration
//--------------------------------------------------------------------------------
// configures CAN-FD
DECL_STDXL_FUNC ( xlCanFdSetConfiguration, XLCANFDSETCONFIGURATION, (
                  XLportHandle portHandle,
                  XLaccess     accessMask,
                  XLcanFdConf* pCanFdConf)
                 );

//------------------------------------------------------------------------------
// xlCanReceive
//--------------------------------------------------------------------------------
// receives a CAN/CAN-FD event from the applications receive queue
DECL_STDXL_FUNC ( xlCanReceive, XLCANRECEIVE, (
                  XLportHandle  portHandle,
                  XLcanRxEvent* pXlCanRxEvt)
                 );

//------------------------------------------------------------------------------
// xlCanTransmitEx
//--------------------------------------------------------------------------------
// transmits a number of CAN / CAN-FD events
DECL_STDXL_FUNC ( xlCanTransmitEx, XLCANTRANSMITEX, (
                  XLportHandle  portHandle,
                  XLaccess      accessMask,
                  unsigned int  msgCnt,
                  unsigned int* pMsgCntSent,
                  XLcanTxEvent* pXlCanTxEvt)
                 );

/*------------------------------------------------------------------------------
xlCanSetAcceptance():
--------------------------------------------------------------------------------
Set the acceptance filter
Filters for std and ext ids are handled independent in the driver.
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
Usually pEvents is a pointer to XLevent array. pEvents points to variable
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
To query and change the global time sync setting 
*/
DECL_STDXL_FUNC ( xlSetGlobalTimeSync, XLSETGLOBALTIMESYNC, (unsigned long newValue, 
                                                             unsigned long *previousValue));


/*------------------------------------------------------------------------------
xlCheckLicense():
--------------------------------------------------------------------------------
For all channels the port wants to use it is checked whether
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
 *                              XL_SUCCESS if no error occurred.
 *                              XL_ERR_NO_RESOURCES if the given array size is too small to copy all available licenses into it.
 *                              XL_ERROR if general error occurred.
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
// DAIO Function Declarations (IOcab)
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
    extern XLCANGETEVENTSTRING        xlCanGetEventString;
    extern XLGETERRORSTRING           xlGetErrorString;
    
#  endif
#endif

////////////////////////////////////////////////////////////////////////////////
// MOST Function Declarations
////////////////////////////////////////////////////////////////////////////////

//  apiname, apitype, parameters in parenthesis

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
 *                                         successfully launched or e.g. whether a command queue overflow happened
 */

/** \brief fetching events from driver queue.
 *  This method is used to fetch events, either bus events or acknowledgments 
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

/** \brief Programming the routing engine (RE) for audio channels.
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

/** \brief Programming the routing engine (RE) for audio channels.
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
 *  \param pCtrlBusloadConfiguration  [IN] structure containing the ctrl msg used for busload generation and configuration,  
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
 *  \param numberCtrlFrames           [IN] number of busload ctrl messages (0xFFFFFFFF indicates infinite number of messages) 
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
 *  \param pAsyncBusloadConfiguration [IN] structure containing the async msg used for busload generation and configuration,  
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
 *  \param numberAsyncFrames          [IN] number of busload async messages (0xFFFFFFFF indicates infinite number of messages) 
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
 *  \param pStreamOpen                [IN] structure containing the stream parameters - 
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
 *                                         has to be released by calling xlMostStreamBufferDeallocate
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


/** \brief Function to set the filter type for a range of slots.
 *  ResponseEvent:                         
 *  \param pAcceptanceFilter          [IN] type and ranges of slots
 *  \return XLstatus                       general status information 
 */ 

DECL_STDXL_FUNC (xlFrSetAcceptanceFilter, XLFP_FRSETACCEPTANCEFILTER, (DEFFRPARAM,
                                                                       XLfrAcceptanceFilter *pAcceptanceFilter));

/** \brief The application gets the information, which remote channels are available in the system. The user
 *         must provide the memory (pointer to XLdriverConfig structure).
 *  \param pDriverConfig              [OUT] The remote driver configuration structure.
 *  \return XLstatus                        general status information
 */
DECL_STDXL_FUNC( xlGetRemoteDriverConfig, XLGETREMOTEDRIVERCONFIG, (XLdriverConfig *pDriverConfig));


/** \brief The application gets the available RDNI network devices. The buffer is allocated by DLL and can be 
 *         freed by a call of xlReleaseRemoteDeviceInfo.
 *  \param deviceList                 [OUT] Pointer receiving the address of the buffer containing the device configuration information.
 *  \param nbrOfRemoteDevices         [OUT] The number of available network devices.
 *  \param netSearch                  [IN] One of the defines: XL_REMOTE_NET_SEARCH or XL_REMOTE_NO_NET_SEARCH
 *
 *  \return XLstatus                        general status information
 */
DECL_STDXL_FUNC (xlGetRemoteDeviceInfo, XLGETREMOTEDEVICEINFO, (XLremoteDeviceInfo  **deviceList, unsigned int *nbrOfRemoteDevices, unsigned int netSearch));

/** \brief Frees the buffer allocated by a call to xlGetRemoteDeviceInfo.
 *  \param deviceList                 [IN] Pointer containing the address of the buffer.
 *  \return XLstatus                        general status information
 */
DECL_STDXL_FUNC (xlReleaseRemoteDeviceInfo, XLRELEASEREMOTEDEVICEINFO, (XLremoteDeviceInfo  **deviceList));

/** \brief The application establishes a connection to a RDNI network device.
 *  \param remoteHandle               [IN] DLL internal handle of the device retrieved by a call to xlGetRemoteDeviceInfo.
 *  \param deviceMask                 [IN] not used
 *  \param flags                      [IN] specify if the connection shall be permanent or temporary (until next reboot)
 *                                         One of the defines: XL_REMOTE_ADD_PERMANENT, XL_REMOTE_ADD_TEMPORARY
 *
 *  \return XLstatus                        general status information
 */
DECL_STDXL_FUNC (xlAddRemoteDevice, XLADDREMOTEDEVICE, (XLremoteHandle remoteHandle, XLdeviceAccess deviceMask, unsigned int flags));

/** \brief The application closes a connection to a RDNI network device.
 *  \param remoteHandle               [IN] DLL internal handle of the device retrieved by a call to xlGetRemoteDeviceInfo.
 *  \param deviceMask                 [IN] not used
 *  \param flags                      [IN] Server entry removal flag.
 *
 *  \return XLstatus                        general status information
 */
DECL_STDXL_FUNC (xlRemoveRemoteDevice, XLREMOVEREMOTEDEVICE, (XLremoteHandle remoteHandle, XLdeviceAccess deviceMask, unsigned int flags));

/** \brief Updates a list of remote device information objects
 *  \param deviceList                 [IN] An array of device information objects to be updated
 *  \param nbrOfRemoteDevices         [IN] Number of elements in the list. 

 *
 *  \return XLstatus                        general status information
 */
DECL_STDXL_FUNC (xlUpdateRemoteDeviceInfo, XLUPDATEREMOTEDEVICEINFO, (XLremoteDeviceInfo *deviceList, unsigned int nbrOfRemoteDevices));

/** \brief Retrieves the hardware type and hardware index of the channels in driver config structure that belong to the remote device.
 *  \param remoteHandle               [IN]  Handle of the remote device.
 *  \param hwType                     [OUT] Hardware type of the channels.
 *  \param hwIndex                    [OUT] Hardware index of the channels.

 *
 *  \return XLstatus                        general status information
 */
DECL_STDXL_FUNC (xlGetRemoteHwInfo, XLGETREMOTEHWINFO, (XLremoteHandle remoteHandle, int *hwType, int *hwIndex, int *isPresent));

/** \brief Registers a manual configured network node.
 *  \param hwType                     [IN]  Hardware type of the device to be registered.
 *  \param ipAddress                  [IN]  IP address of the device.
 *  \param flags                      [IN]  Specify one of the defines:
 *                                          XL_REMOTE_REGISTER_NONE, XL_REMOTE_REGISTER_CONNECT, XL_REMOTE_REGISTER_TEMP_CONNECT
 *
 *  \return XLstatus                        general status information
 */
DECL_STDXL_FUNC (xlRegisterRemoteDevice, XLREGISTERREMOTEDEVICE, (int hwType, XLipAddress *ipAddress, unsigned int flags));


///////////////////////////////////////////////////////////
// IOpiggy API functions (Public)
///////////////////////////////////////////////////////////

/** \brief Setup the DAIO trigger for the analog, digital and the pwm ports. A port group must not
 *         have more than one trigger source.
 *  \param pxlDaioTriggerMode      [IN] Pointer to the trigger mode structure
 *  \return XLstatus                       general status information 
 */

DECL_STDXL_FUNC ( xlIoSetTriggerMode, XLIOSETTRIGGERMODE, (DEFFRPARAM, XLdaioTriggerMode* pxlDaioTriggerMode));

/** \brief Sets the values of digital outputs.
 *  \param pxlDaioDigitalParams    [IN] Pointer to the digital parameter structure
 *  \return XLstatus                       general status information 
 */

DECL_STDXL_FUNC(xlIoSetDigitalOutput, XLIOSETDIGITALOUTPUT, (DEFFRPARAM, XLdaioDigitalParams *pxlDaioDigitalParams));


/** \brief Setup the DAIO ports.
 *  \param pxlDaioSetPort          [IN] Pointer to the XLdaioSetPort structure.
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC ( xlIoConfigurePorts, XLIOCONFIGUREPORTS, (DEFFRPARAM, XLdaioSetPort *pxlDaioSetPort));

/** \brief Defines the voltage level where a digital signal is measured as logical high and where it is measured as logical low.
 *  \param level                   [IN] 10bit value which defines voltage level [mV] for input threshold of digital ports.
 *  \return XLstatus                       general status information 
 */

DECL_STDXL_FUNC( xlIoSetDigInThreshold, XLIOSETDIGINTHRESHOLD, (DEFFRPARAM, unsigned int level));

/** \brief Set the voltage of the high level of the digital outputs in push-pull mode.
 *  \param level                   [IN] One of the defines XL_DAIO_DO_LEVEL_*
 *  \return XLstatus                       general status information 
 */

DECL_STDXL_FUNC( xlIoSetDigOutLevel, XLIOSETDIGOUTLEVEL, (DEFFRPARAM, unsigned int level));

/** \brief Set the values for the analog outputs.
 *  \param pxlDaioAnalogParams     [IN] Pointer to the analog parameter structure
 *  \return XLstatus                       general status information 
 */

DECL_STDXL_FUNC(xlIoSetAnalogOutput, XLIOSETANALOGOUTPUT, (DEFFRPARAM, XLdaioAnalogParams *pxlDaioAnalogParams));

/** \brief Start measurements.
 *  \param portTypeMask            [IN] Port types on which to start the measurements. Use defines XL_DAIO_PORT_TYPE_MASK_*.
 *  \return XLstatus                       general status information 
 */

DECL_STDXL_FUNC(xlIoStartSampling, XLIOSTARTSAMPLING, (DEFFRPARAM, unsigned int portTypeMask));

////////////////////////////////////////////////////////////////////////////////
// MOST150 Function Declarations
////////////////////////////////////////////////////////////////////////////////

//  apiname, apitype, parameters in parenthesis

/**
 *  Common principles:
 *    If not mentioned otherwise, all APIs are asynchronous and will trigger an action in VN2640.
 *    Results are delivered in events which can be fetched by xlMost150Receive.
 *    
 *  Common Parameters: DEFPARAMS
 *    XLportHandle portHandle:             was previously fetched by xlOpenPort API.
 *    XLaccess accessMask:                 determines on which channels an API should work.
 *    XLuserHandle userHandle:             used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously, e.g. MPR changed then the userHandle == 0
 *  Common Return Value:
 *    XLstatus:                            common return value of most APIs which indicates whether a command was 
 *                                         successfully launched or e.g. whether a command queue overflow happened
 */

/** \brief fetching events from driver queue.
 *  This method is used to fetch events, either bus events or acknowledgments 
 *  for commands from the driver queue. Each call delivers only one event (if an event is available). \n
 *  It is a synchronous mode and either delivers event data immediately, or
 *  indicates an error condition with its return value.
 *  \param  XLportHandle portHandle:  [IN] was previously fetched by xlOpenPort API
 *  \param  XLaccess accessMask:      [IN] determines on which channels an API should work
 *  \param  pEventBuffer              [IN] This parameter must point to a buffer to which the driver can copy
 *                                         the next event of the receive queue
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150Receive,         XLFP_MOST150RECEIVE,           (XLportHandle portHandle, XLmost150event* pEventBuffer));

/** \brief Twinkle the power led from the VN2640.
 *  ResponseEvent:                         none
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150TwinklePowerLed,       XLFP_MOST150TWINKLEPOWERLED,      (DEFPARAMS)); 

/** \brief Activates or deactivates the different event sources of VN2640.
 *  This method is used to select which bus events should be delivered by VN2640.
 *  ResponseEvent:                         XL_MOST150_EVENT_SOURCE
 *  \param sourceMask                 [IN] each bit stands for an event source and can separately be set. 
 *                                         Use the definitions of the source mask (see XL_MOST150_SOURCE_...).
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150SwitchEventSources, XLFP_MOST150SWITCHEVENTSOURCES,(DEFPARAMS, unsigned int sourceMask));


/** \brief Sets the device mode.
 *  This method is used to switch the device mode to either Master, Slave or bypass \n
 *  ResponseEvent:                         XL_MOST150_DEVICE_MODE
 *  \param  deviceMode                [IN] device mode (see XL_MOST150_DEVICEMODE_...)
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150SetDeviceMode,   XLFP_MOST150SETDEVICEMODE,  (DEFPARAMS, unsigned int deviceMode));

/** \brief Requests the current device mode.
 *  This method is asynchronous and requests the event used to switch device mode.
 *  ResponseEvent:                         XL_MOST150_DEVICE_MODE
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150GetDeviceMode,   XLFP_MOST150GETDEVICEMODE,  (DEFPARAMS));

/** \brief Switches the SPDIF mode to slave or master mode.
 *  This method is used to switch into the SPDIF master or SPDIF slave mode \n
 *  ResponseEvent:                         XL_MOST150_SPDIFMODE
 *  \param  spdifMode                 [IN] MOST master/slave, ...
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150SetSPDIFMode,    XLFP_MOST150SETSPDIFMODE,  (DEFPARAMS, unsigned int spdifMode));

/** \brief Requests the current SPDIF mode.
 *  This method is used to trigger the event XL_MOST150_SPDIFMODE, which will deliver
 *  information whether the SPDIF is configured in slave or master mode.\n
 *  ResponseEvent:                         XL_MOST150_SPDIFMODE
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150GetSPDIFMode,    XLFP_MOST150GETSPDIFMODE,  (DEFPARAMS));


/** \brief Set one or more parameters of the special node info at once.
 *  ResponseEvent:                         XL_MOST150_SPECIAL_NODE_INFO
 *  \param  pSpecialNodeInfo          [IN] contains the parameter to set
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150SetSpecialNodeInfo, XLFP_MOST150SETSPECIALNODEINFO, (DEFPARAMS, XLmost150SetSpecialNodeInfo *pSpecialNodeInfo));

/** \brief Requests one or more parameters of the special node info at once.
 *  ResponseEvent:                         XL_MOST150_SPECIAL_NODE_INFO
 *  \param  requestMask               [IN] contains a mask of parameter to get (see XL_MOST150_SPECIAL_NODE_MASK_CHANGED)
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150GetSpecialNodeInfo, XLFP_MOST150GETSPECIALNODEINFO, (DEFPARAMS, unsigned int requestMask));

/** \brief Set the frequency of the MOST150 ring.
 *  ResponseEvent:                         XL_MOST150_FREQUENCY
 *  \param  frequency                 [IN] contains the frequency to be set. Only as timing master! (see XL_MOST150_FREQUENCY_...)
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150SetFrequency,    XLFP_MOST150SETFREQUENCY,    (DEFPARAMS, unsigned int frequency));

/** \brief Requests the frequency of the MOST150 ring.
 *  ResponseEvent:                         XL_MOST150_FREQUENCY
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150GetFrequency,    XLFP_MOST150GETFREQUENCY,    (DEFPARAMS));

/** \brief Transmit a control message on the MOST150 ring.
 *  ResponseEvent:                         XL_MOST150_CTRL_TX
 *  \param  pCtrlTxMsg                [IN] pointer to structure that contains the control message to be sent
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150CtrlTransmit,    XLFP_MOST150CTRLTRANSMIT,    (DEFPARAMS, XLmost150CtrlTxMsg *pCtrlTxMsg));

/** \brief Transmit a data packet (MDP) on the MOST150 ring.
 *  ResponseEvent:                         XL_MOST150_ASYNC_TX
 *  \param  pAsyncTxMsg               [IN] pointer to structure that contains the MOST Data Packet (MDP) to be sent
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150AsyncTransmit,   XLFP_MOST150ASYNCTRANSMIT,   (DEFPARAMS, XLmost150AsyncTxMsg *pAsyncTxMsg));

/** \brief Transmit a Ethernet packet (MEP) on the MOST150 ring.
 *  ResponseEvent:                         XL_MOST150_ETHERNET_TX
 *  \param  pEthernetTxMsg            [IN] pointer to structure that contains the MOST Ethernet Packet (MEP) to be sent
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150EthernetTransmit, XLFP_MOST150ETHTRANSMIT,    (DEFPARAMS, XLmost150EthernetTxMsg *pEthernetTxMsg));

/** \brief Requests the state of the system lock flag.
 *  ResponseEvent:                         XL_MOST150_SYSTEMLOCK_FLAG
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150GetSystemLockFlag, XLFP_MOST150GETSYSTEMLOCK, (DEFPARAMS));

/** \brief Requests the state of the shutdown flag.
 *  ResponseEvent:                         XL_MOST150_SHUTDOWN_FLAG
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150GetShutdownFlag, XLFP_MOST150GETSHUTDOWN,     (DEFPARAMS));

/** \brief Shutdown the MOST150 ring.
 *  ResponseEvent:                         
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150Shutdown,        XLFP_MOST150SHUTDOWN,       (DEFPARAMS));

/** \brief Startup the MOST150 ring.
 *  ResponseEvent:                         
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150Startup,          XLFP_MOST150STARTUP,        (DEFPARAMS));

/** \brief Requests the current allocation information.
 *  \n
 *  ResponseEvent:                         XL_MOST150_SYNC_ALLOC_INFO
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150SyncGetAllocTable, XLFP_MOST150GETALLOCTABLE, (DEFPARAMS));

/** \brief Set the parameters for audio functions.
 *  ResponseEvent:                         XL_MOST150_CTRL_SYNC_AUDIO
 *  \param  pSyncAudioParameter       [IN] pointer to structure that contains the data
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150CtrlSyncAudio,   XLFP_MOST150CTRLSYNCAUDIO,   (DEFPARAMS, XLmost150SyncAudioParameter *pSyncAudioParameter));

/** \brief Set the volume of Line In/Out audio device.
 *  ResponseEvent:                         XL_MOST150_SYNC_VOLUME_STATUS
 *  \param  device                    [IN] specifies the device (see XL_MOST150_DEVICE_LINE_IN, ...)
 *  \param  volume                    [IN] specifies the volume
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150SyncSetVolume,   XLFP_MOST150SYNCSETVOLUME,   (DEFPARAMS, unsigned int  device, unsigned int  volume));

/** \brief Requests the volume of Line In/Out audio device.
 *  \n
 *  ResponseEvent:                         XL_MOST150_SYNC_VOLUME_STATUS
 *  \param  device                    [IN] specifies the device (see XL_MOST150_DEVICE_LINE_IN, ...)
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150SyncGetVolume,   XLFP_MOST150SYNCGETVOLUME,   (DEFPARAMS, unsigned int  device));

/** \brief Set mute state of Line In/Out or S/PDIF In/Out audio device.
 *  ResponseEvent:                         XL_MOST150_SYNC_MUTE_STATUS
 *  \param  device                    [IN] specifies the device (see XL_MOST150_DEVICE_...)
 *  \param  mute                      [IN] specifies the mute status (on / off)
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150SyncSetMute,     XLFP_MOST150SYNCSETMUTE,     (DEFPARAMS, unsigned int  device, unsigned int  mute));

/** \brief Requests mute state of Line In/Out or S/PDIF In/Out audio device.
 *  ResponseEvent:                         XL_MOST150_SYNC_MUTE_STATUS
 *  \param  device                    [IN] specifies the device (see XL_MOST150_DEVICE_LINE_IN, ...)
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150SyncGetMute,     XLFP_MOST150SYNCGETMUTE,     (DEFPARAMS, unsigned int  device));

/** \brief Requests the FOR and lock status either from the spy or from INIC.
 *  ResponseEvent:                         XL_MOST150_RXLIGHT_LOCKSTATUS
 *  \param  fromSpy                   [IN] defines the source, to get the light & lock status from
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150GetRxLightLockStatus, XLFP_MOST150GETLIGHTLOCKSTATUS, (DEFPARAMS, unsigned int fromSpy));

/** \brief Set the FOT output mode.
 *  \n
 *  ResponseEvent:                         XL_MOST150_TX_LIGHT
 *  \param  txLight                   [IN] mode of the output (modulated (on) or off)
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150SetTxLight,      XLFP_MOST150SETTXLIGHT,       (DEFPARAMS, unsigned int txLight));

/** \brief Requests the FOT output mode.
 *  ResponseEvent:                         XL_MOST150_TX_LIGHT
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150GetTxLight,      XLFP_MOST150GETTXLIGHT,       (DEFPARAMS));

/** \brief Set the FOT output power.
 *  \n
 *  ResponseEvent:                         XL_MOST150_LIGHT_POWER
 *  \param  attenuation               [IN] tx light power (no attenuation / -3dB attenuation)
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150SetTxLightPower, XLFP_MOST150SETTXLIGHTPOWER,  (DEFPARAMS, unsigned int  attenuation));

/** \brief Controls the light error generation.
 *  \n
 *  ResponseEvent:                         XL_MOST150_GEN_LIGHT_ERROR
 *  \param  lightOffTime              [IN] duration of light off in [ms]
 *  \param  lightOnTime               [IN] duration of light on in [ms]
 *  \param  repeat                    [IN] number of error intervals
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150GenerateLightError, XLFP_MOST150GENLIGHTERROR, (DEFPARAMS, unsigned int lightOffTime, unsigned int lightOnTime, unsigned int repeat));

/** \brief Control the lock error generation.
 *  \n
 *  ResponseEvent:                         XL_MOST150_GEN_LOCK_ERROR
 *  \param  unlockTime                [IN] duration of unlock in [ms]
 *  \param  lockTime                  [IN] duration of lock in [ms]
 *  \param  repeat                    [IN] number of error intervals
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150GenerateLockError, XLFP_MOST150GENLOCKERROR, (DEFPARAMS, unsigned int unlockTime, unsigned int lockTime, unsigned int repeat));

/** \brief Configures the receive buffer for control messages and packets of the INIC.
 *  \n
 *  ResponseEvent:                         XL_MOST150_CONFIGURE_RX_BUFFER
 *  \param  bufferType                [IN] Bit mask for receive buffer type (control messages and/or packets (MDP/MEP)).
 *  \param  bufferMode                [IN] Block or un-block receive buffer
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150ConfigureRxBuffer, XLFP_MOST150CONFIGURERXBUFFER,  (DEFPARAMS, unsigned int bufferType, unsigned int  bufferMode));

/** \brief Defines the control message which should be transmitted with xlMost150CtrlGenerateBusload().
 *  ResponseEvent:                         
 *  \param  pCtrlBusLoad              [IN] pointer to structure that contains the control message
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150CtrlConfigureBusload,   XLFP_MOST150CTRLCONFIGLOAD,   (DEFPARAMS, XLmost150CtrlBusloadConfig *pCtrlBusLoad));

/** \brief Starts or stops the control message busload by sending the control message defined with xlMost150CtrlConfigureBusload().
 *  ResponseEvent:                        XL_MOST150_CTRL_BUSLOAD 
 *  \param  numberCtrlFrames         [IN] number of control messages to be sent
 *                                        0:            stop sending
 *                                        < 0xFFFFFFFF: number of messages to be sent
 *                                        0xFFFFFFFF:   send continuously
 *  \return XLstatus                      general status information
 */
DECL_STDXL_FUNC( xlMost150CtrlGenerateBusload,   XLFP_MOST150CTRLGENLOAD,   (DEFPARAMS, unsigned long numberCtrlFrames));

/** \brief Define the data or Ethernet packet that should be transmitted with xlMost150AsyncGenerateBusload().
 *  ResponseEvent:                         
 *  \param  pAsyncBusLoad             [IN] pointer to structure that contains either the data or the Ethernet packet
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150AsyncConfigureBusload,   XLFP_MOST150ASYNCCONFIGLOAD,   (DEFPARAMS, XLmost150AsyncBusloadConfig *pAsyncBusLoad));

/** \brief Starts or stops the packet busload by sending either the data or Ethernet packet defined with xlMost150AsyncConfigureBusload().
 *  ResponseEvent:                         XL_MOST150_ASYNC_BUSLOAD
 *  \param  numberAsyncPackets        [IN] number of data or Ethernet packets to be sent
 *                                         0:            stop sending
 *                                         < 0xFFFFFFFF: number of packets to be sent
 *                                         0xFFFFFFFF:   send continuously
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150AsyncGenerateBusload,   XLFP_MOST150ASYNCGENLOAD,   (DEFPARAMS, unsigned long numberAsyncPackets));


/** \brief Set the ECL state.
 *  ResponseEvent:                         XL_MOST150_ECL_LINE_CHANGED
 *  \param  eclLineState              [IN] ECL state to be set: XL_MOST150_ECL_LINE_LOW, XL_MOST150_ECL_LINE_HIGH
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150SetECLLine,      XLFP_MOST150SETECLLINE,   (DEFPARAMS, unsigned int  eclLineState));


/** \brief Set the ECL termination resistor state.
 *  ResponseEvent:                         XL_MOST150_ECL_TERMINATION_CHANGED
 *  \param  eclLineTermination        [IN] ECL line termination resistor state to be set:
 *                                         XL_MOST150_ECL_LINE_PULL_UP_NOT_ACTIVE, XL_MOST150_ECL_LINE_PULL_UP_ACTIVE
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150SetECLTermination, XLFP_MOST150SETECLTERMINATION,   (DEFPARAMS, unsigned int  eclLineTermination));

/** \brief Requests the current ECL state and settings.
 *  This method is asynchronous and requests the event used to get the ECL line state, termination and glitch filter setting.
 *  ResponseEvent:                         XL_MOST150_ECL_LINE_CHANGED, XL_MOST150_ECL_TERMINATION_CHANGED, XL_MOST150_ECL_GLITCH_FILTER
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlMost150GetECLInfo,      XLFP_MOST150GETECLINFO,  (DEFPARAMS));

/** \brief Opens a stream (Rx / Tx) for routing synchronous data to or from the MOST bus (synchronous channel). 
 *  ResponseEvent:                         XL_MOST150_STREAM_STATE 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param pStreamOpen                [IN] structure containing the stream parameters - 
 *                                         it's storage has has to be supplied by the caller 
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMost150StreamOpen,      XLFP_MOST150STREAMOPEN,  (DEFPARAMS, XLmost150StreamOpen* pStreamOpen)); 


/** \brief Closes an opened a stream (Rx / Tx) used for routing synchronous data to or from the MOST bus (synchronous channel). 
 *  ResponseEvent:                         XL_MOST150_STREAM_STATE 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param streamHandle               [IN] stream handle returned by xlMostStreamOpen
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMost150StreamClose,     XLFP_MOST150STREAMCLOSE,  (DEFPARAMS, unsigned int streamHandle)); 


/** \brief Starts the streaming (Rx / Tx) of synchronous data to or from the MOST bus (synchronous channel). 
 *  Attention: Has to be called after XL_MOST150_STREAM_STATE "Opened" was received.
 *  ResponseEvent:                         XL_MOST150_STREAM_STATE 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param streamHandle               [IN] stream handle returned by xlMostStreamOpen
 *  \param numConnLabels              [IN] Number of connection labels to stream (only used for Rx streaming, max. 8 labels can be streamed!)
 *  \param pConnLabels                [IN] connection label(s) (only used for Rx streaming)
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMost150StreamStart,     XLFP_MOST150STREAMSTART,  (DEFPARAMS, unsigned int streamHandle,
                                                                      unsigned int  numConnLabels,
                                                                      unsigned int* pConnLabels)); 


/** \brief Stops the streaming (Rx / Tx) of synchronous data to or from the MOST bus (synchronous channel). 
 *  Attention: Has to be called after XL_MOST150_STREAM_STATE "Started" was received.
 *  ResponseEvent:                         XL_MOST150_STREAM_STATE 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param streamHandle               [IN] stream handle returned by xlMostStreamOpen
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMost150StreamStop,      XLFP_MOST150STREAMSTOP,  (DEFPARAMS, unsigned int streamHandle)); 

/** \brief Provides further streaming data to be sent to the MOST bus (synchronous channel). 
 *  ResponseEvent:                         XL_MOST150_STREAM_TX_BUFFER as soon as further data is required. 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param streamHandle               [IN] stream handle returned by xlMostStreamOpen
 *  \param pBuffer                    [IN] pointer to the buffer used for streaming
 *  \param pNumberOfBytes       [IN]/[OUT] number of bytes contained in the buffer to be used for streaming.
 *                                         In case of not all bytes could be stored, this parameters contains the adjusted 
 *                                         number of bytes stored and the function returns an error (XL_ERR_QUEUE_IS_FULL).
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMost150StreamTransmitData, XLFP_MOST150STREAMTRANSMITDATA, (DEFPARAMS, unsigned int streamHandle, unsigned char* pBuffer, unsigned int* pNumberOfBytes)); 

/** \brief Clears the content of the driver's Tx FIFO.
 *  This method is used to clear the content of the driver's TX streaming FIFO which has not been sent yet.\n
 *  ResponseEvent:                         None 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel from where to get the device's state 
 *  \param userHandle                 [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously 
 *  \param streamHandle               [IN] stream handle returned by xlMostStreamOpen.
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMost150StreamClearTxFifo,  XLFP_MOST150STREAMCLEARTXFIFO,  (DEFPARAMS, unsigned int streamHandle)); 

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
DECL_STDXL_FUNC (xlMost150StreamGetInfo, XLFP_MOST150STREAMGETINFO, (DEFPARAMS, XLmost150StreamInfo* pStreamInfo));

/** \brief Initializes the Rx Streaming FIFO.
 *  This method is used to initialize the FIFO for storing the received streaming data.\n
 *  ResponseEvent:                         None 
 *  \param portHandle                 [IN] handle to port from which the information is requested 
 *  \param accessMask                 [IN] mask specifying the port's channel
 *  \return XLstatus                       general status information 
 */
DECL_STDXL_FUNC (xlMost150StreamInitRxFifo, XLFP_MOST150STREAMINITRXFIFO, (XLportHandle portHandle, XLaccess accessMask));

/** \brief Fetches streaming data from the driver queue.
 *  This method is used to fetch received streaming data. The application is triggered by
 *  a XL_MOST150_STREAM_RX_BUFFER event to call this method.
 *  It is a synchronous mode and either delivers streaming data immediately, or
 *  indicates an error condition with its return value.
 *  \param  XLportHandle portHandle:  [IN] was previously fetched by xlOpenPort API
 *  \param  XLaccess accessMask:      [IN] determines on which channel an API should work
 *  \param  pBuffer                   [IN] Pointer to a buffer to which the driver can copy
 *                                         the streaming data of the receive queue
 *  \param  pBufferSize               [IN] Determines the maximum buffer size
 *                                   [OUT] The number of actually copied data bytes
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC (xlMost150StreamReceiveData, XLFP_MOST150STREAMRECEIVEDATA, (XLportHandle portHandle, XLaccess accessMask,
                                                                             unsigned char* pBuffer, unsigned int* pBufferSize));

/** \brief Controls the bypass stress generation.
 *  \param  XLportHandle portHandle:  [IN] was previously fetched by xlOpenPort API
 *  \param  XLaccess accessMask:      [IN] determines on which channel an API should work
 *  \param  XLuserHandle userHandle:  [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously
 *  \param  bypassCloseTime           [IN] duration of bypass close time in [ms] (minimum value: 10 ms)
 *  \param  bypassOpenTime            [IN] duration of bypass open time in [ms] (minimum value: 10 ms)
 *  \param  repeat                    [IN] number of error intervals
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC (xlMost150GenerateBypassStress, XLFP_MOST150GENERATEBYPASSSTRESS, (DEFPARAMS, unsigned int bypassCloseTime, 
                                                                                   unsigned int bypassOpenTime, unsigned int repeat));

/** \brief Configures a sequence for the ECL.
 *  \param  XLportHandle portHandle:  [IN] was previously fetched by xlOpenPort API
 *  \param  XLaccess accessMask:      [IN] determines on which channel an API should work
 *  \param  XLuserHandle userHandle:  [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously
 *  \param  numStates                 [IN] Number of states during the sequence (max. XL_MOST150_ECL_SEQ_NUM_STATES_MAX)
 *  \param  pEclStates                [IN] Pointer to a buffer containing the ECL sequence states (1: High, 0: Low)
 *  \param  pEclStatesDuration        [IN] Pointer to a buffer containing the ECL sequence states duration in multiple of 100 µs (max. value XL_MOST150_ECL_SEQ_DURATION_MAX)
 *                                         NOTE: Both buffers have to have at least the size <numStates> DWORDS!
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC (xlMost150EclConfigureSeq, XLFP_MOST150ECLCONFIGURESEQ, (DEFPARAMS, unsigned int numStates, 
                                                                         unsigned int* pEclStates,
                                                                         unsigned int* pEclStatesDuration));

/** \brief Starts or stops the previously configured ECL sequence.
 *  \param  XLportHandle portHandle:  [IN] was previously fetched by xlOpenPort API
 *  \param  XLaccess accessMask:      [IN] determines on which channel an API should work
 *  \param  XLuserHandle userHandle:  [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously
 *  \param  start                     [IN] Starts (1) or stops (0) the configured ECL sequence
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC (xlMost150EclGenerateSeq, XLFP_MOST150ECLGENERATESEQ, (DEFPARAMS, unsigned int start));

/** \brief Configures the glitch filter for detecting ECL state changes.
 *  \param  XLportHandle portHandle:  [IN] was previously fetched by xlOpenPort API
 *  \param  XLaccess accessMask:      [IN] determines on which channel an API should work
 *  \param  XLuserHandle userHandle:  [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously
 *  \param  duration                  [IN] Duration (in µs) of glitches to be filtered. Value range: 50 µs .. 50 ms (Default: 1 ms)
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC (xlMost150SetECLGlitchFilter, XLFP_MOST150SETECLGLITCHFILTER, (DEFPARAMS, unsigned int duration));


/** \brief Sets the SSOResult value - needed for resetting the value to 0x00 (No Result) after Shutdown Result analysis has been done.
 *  ResponseEvent:                         XL_MOST150_SSO_RESULT. 
 *  \param  XLportHandle portHandle:  [IN] was previously fetched by xlOpenPort API
 *  \param  XLaccess accessMask:      [IN] determines on which channel an API should work
 *  \param  XLuserHandle userHandle:  [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously
 *  \param  ssoCUStatus               [IN] SSOCUStatus (currently only the value <XL_MOST150_SSO_RESULT_NO_RESULT> is allowed!)
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC (xlMost150SetSSOResult, XLFP_MOST150SETSSORESULT, (DEFPARAMS, unsigned int ssoCUStatus));

/** \brief Requests the SSOResult value.
 *  ResponseEvent:                         XL_MOST150_SSO_RESULT. 
 *  \param  XLportHandle portHandle:  [IN] was previously fetched by xlOpenPort API
 *  \param  XLaccess accessMask:      [IN] determines on which channel an API should work
 *  \param  XLuserHandle userHandle:  [IN] used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC (xlMost150GetSSOResult, XLFP_MOST150GETSSORESULT, (DEFPARAMS));

////////////////////////////////////////////////////////////////////////////////
// Ethernet Function Declarations
////////////////////////////////////////////////////////////////////////////////

//  apiname, apitype, parameters in parenthesis

/**
 *  Common principles:
 *    If not mentioned otherwise, all APIs are asynchronous and will trigger an action in VN56xx.
 *    Results are delivered in events which can be fetched by xlEthReceive.
 *    
 *  Common Parameters: DEFPARAMS
 *    XLportHandle portHandle:             was previously fetched by xlOpenPort API.
 *    XLaccess accessMask:                 determines on which channels an API should work.
 *    XLuserHandle userHandle:             used to match the response of the driver to the requests of the application
 *                                         if an event is received spontaneously, e.g. MPR changed then the userHandle == 0
 *  Common Return Value:
 *    XLstatus:                            common return value of most APIs which indicates whether a command was 
 *                                         successfully launched or e.g. whether a command queue overflow happened
 */

/** \brief Configures basic Ethernet settings.
 *  This method is used to configure the basic Ethernets settings like speed, connector, etc. \n
 *  ResponseEvent:                         XL_ETH_CONFIG_RESULT
 *  \param  config                    [IN] new configuration to set
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC(xlEthSetConfig, XLFP_ETHSETCONFIG, (DEFPARAMS, const T_XL_ETH_CONFIG *config));

/** \brief Synchronously read the last Ethernet configuration settings.
 *  This allows an application to detect if a change in configuration is necessary.\n
 *  ResponseEvent:                         none
 *  \param  config                   [OUT] current configuration
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC(xlEthGetConfig, XLFP_ETHGETCONFIG, (DEFPARAMS, T_XL_ETH_CONFIG *config));

/** \brief Fetching events from driver queue.
 *  This method is used to fetch events, either bus events or acknowledgments 
 *  for commands from the driver queue. Each call delivers only one event (if an event is available). \n
 *  It is a synchronous mode and either delivers event data immediately, or
 *  indicates an error condition with its return value.
 *  \param  XLportHandle portHandle:  [IN] was previously fetched by xlOpenPort API
 *  \param  ethEventBuffer            [IN] This parameter must point to a buffer to which the driver can copy
 *                                         the next event of the receive queue. The "size" member of this struct
 *                                         specifies the maximum size of the buffer (header plus receive data); upon return,
 *                                         it holds the actual size.
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlEthReceive, XLFP_ETHRECEIVE, (XLportHandle portHandle,	T_XL_ETH_EVENT *ethEventBuffer));

/** \brief Configures the bypass of two channels.
 *  This method is used to enable the bypass of two channels or to disable the bypass for several channels\n
 *  ResponseEvent:                         XL_ETH_CONFIG
 *  \param  mode                      [IN] Bypass state (one of XL_ETH_BYPASS_INACTIVE, XL_ETH_BYPASS_PHY, XL_ETH_BYPASS_MACCORE)
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC(xlEthSetBypass, XLFP_ETHSETBYPASS, (DEFPARAMS, unsigned int mode));

/** \brief Twinkle the Status led from the VN5610.
 *  ResponseEvent:                         none
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlEthTwinkleStatusLed,       XLFP_ETHTWINKLESTATUSLED,      (DEFPARAMS)); 

/** \brief Transmit a data frame to the network.
 *  This method is asynchronous; a confirmation of the transmit is received via an XL_ETH_TX_OK/XL_ETH_TX_ERROR event.
 *  ResponseEvent:                         XL_ETH_TX_OK, XL_ETH_TX_ERROR
 *  \param  data:                     [IN] pointer to an Ethernet data frame to be sent
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC( xlEthTransmit, XLFP_ETHTRANSMIT, (DEFPARAMS, const T_XL_ETH_DATAFRAME_TX *data));

////////////////////////////////////////////////////////////////////////////////
// ARINC429 Function Declarations
////////////////////////////////////////////////////////////////////////////////
//  apiname, apitype, parameters in parenthesis

/**
 *  Common Parameters: DEFPARAMS
 *    XLportHandle portHandle:             was previously fetched by xlOpenPort API.
 *    XLaccess accessMask:                 determines on which channels an API should work.
 *  Common Return Value:
 *    XLstatus:                            common return value of most APIs which indicates whether a command was 
 *                                         successfully launched or e.g. whether a command queue overflow happened
 */

/** \brief Fetching events from driver queue.
 *  This method is used to fetch events, either bus events or acknowledgments 
 *  for commands from the driver queue. Each call delivers only one event (if an event is available). \n
 *  It is a synchronous mode and either delivers event data immediately, or
 *  indicates an error condition with its return value.
 *  \param  portHandle:               [IN] was previously fetched by xlOpenPort API
 *  \param  pXlA429RxEvt:             [IN] This parameter must point to a buffer to which the driver can copy
 *                                         the next event of the receive queue. The "size" member of this struct
 *                                         specifies the maximum size of the buffer (header plus receive data); upon return,
 *                                         it holds the actual size.
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC(xlA429Receive, XLFP_A429RECEIVE, (XLportHandle portHandle, XLa429RxEvent* pXlA429RxEvt));

/** \brief Configures basic Arinc429 settings.
 *  This method is used to configure the basic Arinc429 settings like parity, bitrate, etc. \n
 *  \param  pXlA429Params:            [IN] new Arinc429 parameter to set
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC(xlA429SetChannelParams, XLFP_A429SETCHANNELPARAMS, (DEFFRPARAM, XL_A429_PARAMS* pXlA429Params));

/** \brief Transmit a Arinc429 word to the network.
 *  This method is asynchronous; a confirmation of the transmit is received via an XL_A429_EV_TAG_TX_OK/XL_A429_EV_TAG_TX_ERR event.
 *  \param  pXlA429MsgTx:             [IN] pointer to an Arinc429 frame to be sent
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC(xlA429Transmit, XLFP_A429TRANSMIT, (DEFFRPARAM, unsigned int msgCnt, unsigned int* pMsgCntSent, XL_A429_MSG_TX* pXlA429MsgTx));

////////////////////////////////////////////////////////////////////////////////
// Vector Keyman Function Declarations
////////////////////////////////////////////////////////////////////////////////
//  apiname, apitype, parameters in parenthesis

/** \brief Returns connected Keyman license dongles.
 *  This method returns the connected Keyman license dongles
 *  \param  boxCount:                 [IN] number of connected Keyman license Dongles
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC(xlGetKeymanBoxes, XLFP_GETKEYMANBOXES, (unsigned int* boxCount));

/** \brief Returns serial number and license info.
 *  This method returns serial number and license info (license bits) of selected Keyman License dongle
 *  \param  boxIndex:                 [IN] index of found Keyman License Dongle (zero based)
 *  \param  boxMask:                  [OUT] mask of Keyman Dongle
 *  \param  boxSerial:                [OUT] serial number of Keyman Dongle
 *  \param  licInfo:                  [OUT] license Info (license bits in license array)
 *  \return XLstatus                       general status information
 */
DECL_STDXL_FUNC(xlGetKeymanInfo, XLFP_GETKEYMANINFO, (unsigned int boxIndex, unsigned int* boxMask, unsigned int* boxSerial, XLuint64* licInfo));


#ifdef __cplusplus
}
#endif   // _cplusplus

#endif // _V_XLAPI_H_        




