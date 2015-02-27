/*!
 * \file
 *
 * \version
 * 1.7.1
 *
 * \brief
 * VSCAN API implementation (header file)
 *
 * This is the implementation of the VSCAN API - a wrapper for the specialized ASCII command set.
 * The library is available for Windows and Linux.
 *
 * Copyright (c) 2014 by VScom
 */
#ifndef _VS_CAN_API_H_
#define _VS_CAN_API_H_


#ifdef WIN32
#include <windows.h>
#else
#include <semaphore.h>
typedef void VOID;
typedef char CHAR;
typedef unsigned long DWORD;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
#endif


#ifndef UINT8
#define UINT8   UCHAR
#endif
#ifndef UINT16
#define UINT16  USHORT
#endif
#ifndef UINT32
#define UINT32  ULONG
#endif


typedef int VSCAN_HANDLE;
typedef int VSCAN_STATUS;

// Hardware Parameter Structure
typedef struct
{
    UINT32 SerialNr;
    UINT8 HwVersion;
    UINT8 SwVersion;
    UINT8 HwType;
} VSCAN_HWPARAM;

// Message Structure
typedef struct
{
    UINT32 Id;
    UINT8 Size;
    UINT8 Data[8];
    UINT8 Flags;
    UINT16 Timestamp;
} VSCAN_MSG;

// Bit Timing Register Structure
typedef struct
{
    UINT8 Btr0;
    UINT8 Btr1;
} VSCAN_BTR;

// Acceptance Code and Mask Structure
typedef struct
{
    UINT32 Code;
    UINT32 Mask;
} VSCAN_CODE_MASK;

// API Version Structure
typedef struct
{
    UINT8 Major;
    UINT8 Minor;
    UINT8 SubMinor;
} VSCAN_API_VERSION;


#define VSCAN_FIRST_FOUND               NULL

// Debug Mode
#define VSCAN_DEBUG_MODE_CONSOLE        (void*)1
#define VSCAN_DEBUG_MODE_FILE           (void*)2
// Debug Level
#define VSCAN_DEBUG_NONE                (void*)0
#define VSCAN_DEBUG_LOW                 (void*)-1
#define VSCAN_DEBUG_MID                 (void*)-51
#define VSCAN_DEBUG_HIGH                (void*)-101

// Status / Errors
#define VSCAN_ERR_OK                    0
#define VSCAN_ERR_ERR                   ((int)VSCAN_DEBUG_LOW)          // Debug Level Low
#define VSCAN_ERR_NO_DEVICE_FOUND       ((int)VSCAN_DEBUG_LOW - 1)
#define VSCAN_ERR_SUBAPI                ((int)VSCAN_DEBUG_LOW - 2)
#define VSCAN_ERR_NOT_ENOUGH_MEMORY     ((int)VSCAN_DEBUG_LOW - 3)
#define VSCAN_ERR_NO_ELEMENT_FOUND      ((int)VSCAN_DEBUG_LOW - 4)
#define VSCAN_ERR_INVALID_HANDLE        ((int)VSCAN_DEBUG_LOW - 5)
#define VSCAN_ERR_IOCTL                 ((int)VSCAN_DEBUG_LOW - 6)
#define VSCAN_ERR_MUTEX                 ((int)VSCAN_DEBUG_LOW - 7)
#define VSCAN_ERR_CMD                   ((int)VSCAN_DEBUG_LOW - 8)
#define VSCAN_ERR_LISTEN_ONLY           ((int)VSCAN_DEBUG_LOW - 9)
#define VSCAN_ERR_NOT_SUPPORTED         ((int)VSCAN_DEBUG_LOW - 10)
#define VSCAN_ERR_GOTO_ERROR            ((int)VSCAN_DEBUG_HIGH)         // Debug Level High

// Mode
#define VSCAN_MODE_NORMAL               0
#define VSCAN_MODE_LISTEN_ONLY          1
#define VSCAN_MODE_SELF_RECEPTION       2

// Speed
#define VSCAN_SPEED_1M                  (void*)8
#define VSCAN_SPEED_800K                (void*)7
#define VSCAN_SPEED_500K                (void*)6
#define VSCAN_SPEED_250K                (void*)5
#define VSCAN_SPEED_125K                (void*)4
#define VSCAN_SPEED_100K                (void*)3
#define VSCAN_SPEED_50K                 (void*)2
#define VSCAN_SPEED_20K                 (void*)1
// generally not possible with the TJA1050
//#define VSCAN_SPEED_10K               (void*)0

// Device Types
#define VSCAN_HWTYPE_UNKNOWN            0
#define VSCAN_HWTYPE_SERIAL             1
#define VSCAN_HWTYPE_USB                2
#define VSCAN_HWTYPE_NET                3
#define VSCAN_HWTYPE_BUS                4

#define VSCAN_IOCTL_OFF                 (void*)0
#define VSCAN_IOCTL_ON                  (void*)1

// Timestamp
#define VSCAN_TIMESTAMP_OFF             VSCAN_IOCTL_OFF
#define VSCAN_TIMESTAMP_ON              VSCAN_IOCTL_ON

// Filter Mode
#define VSCAN_FILTER_MODE_SINGLE        (void*)1
#define VSCAN_FILTER_MODE_DUAL          (void*)2

// Ioctls
#define VSCAN_IOCTL_SET_DEBUG           1
#define VSCAN_IOCTL_GET_HWPARAM         2
#define VSCAN_IOCTL_SET_SPEED           3
#define VSCAN_IOCTL_SET_BTR             4
#define VSCAN_IOCTL_GET_FLAGS           5
#define VSCAN_IOCTL_SET_ACC_CODE_MASK   6
#define VSCAN_IOCTL_SET_TIMESTAMP       7
#define VSCAN_IOCTL_SET_DEBUG_MODE      8
#define VSCAN_IOCTL_SET_BLOCKING_READ   9
#define VSCAN_IOCTL_SET_FILTER_MODE     10
#define VSCAN_IOCTL_GET_API_VERSION     11

// Bits for VSCAN_IOCTL_GET_FLAGS
#define VSCAN_IOCTL_FLAG_RX_FIFO_FULL       (1 << 0)
#define VSCAN_IOCTL_FLAG_TX_FIFO_FULL       (1 << 1)
#define VSCAN_IOCTL_FLAG_ERR_WARNING        (1 << 2)
#define VSCAN_IOCTL_FLAG_DATA_OVERRUN       (1 << 3)
#define VSCAN_IOCTL_FLAG_UNUSED             (1 << 4)
#define VSCAN_IOCTL_FLAG_ERR_PASSIVE        (1 << 5)
#define VSCAN_IOCTL_FLAG_ARBIT_LOST         (1 << 6)
#define VSCAN_IOCTL_FLAG_BUS_ERROR          (1 << 7)
#define VSCAN_IOCTL_FLAG_API_RX_FIFO_FULL   (1 << 16)

// Masks for VSCAN_IOCTL_SET_ACC_CODE_MASK
#define VSCAN_IOCTL_ACC_CODE_ALL        0x00000000
#define VSCAN_IOCTL_ACC_MASK_ALL        0xFFFFFFFF

// Flags
#define VSCAN_FLAGS_STANDARD            (1 << 0)
#define VSCAN_FLAGS_EXTENDED            (1 << 1)
#define VSCAN_FLAGS_REMOTE              (1 << 2)
#define VSCAN_FLAGS_TIMESTAMP           (1 << 3)


#ifdef __cplusplus
extern "C" {
#endif

    // If the function succeeds, the return value is greater zero (handle)
    // If the function fails, the return value is one of VSCAN_STATUS
    VSCAN_HANDLE VSCAN_Open(CHAR* SerialNrORComPortORNet, DWORD Mode);
    // The return value is one of VSCAN_STATUS
    VSCAN_STATUS VSCAN_Close(VSCAN_HANDLE Handle);
    // The return value is one of VSCAN_STATUS
    VSCAN_STATUS VSCAN_Ioctl(VSCAN_HANDLE Handle, DWORD Ioctl, VOID* Param);
    // The return value is one of VSCAN_STATUS
    VSCAN_STATUS VSCAN_Flush(VSCAN_HANDLE Handle);
    // The return value is one of VSCAN_STATUS
    VSCAN_STATUS VSCAN_Write(VSCAN_HANDLE Handle, VSCAN_MSG* Buf, DWORD Size, DWORD* Written);
    // The return value is one of VSCAN_STATUS
    VSCAN_STATUS VSCAN_Read(VSCAN_HANDLE Handle, VSCAN_MSG* Buf, DWORD Size, DWORD* Read);
    // The return value is one of VSCAN_STATUS
#ifdef WIN32
    VSCAN_STATUS VSCAN_SetRcvEvent(VSCAN_HANDLE Handle, HANDLE Event);
#else
    VSCAN_STATUS VSCAN_SetRcvEvent(VSCAN_HANDLE Handle, sem_t* Event);
#endif
    // No return value for this function
    VOID VSCAN_GetErrorString(VSCAN_STATUS Status, CHAR* String, DWORD MaxLen);

#ifdef __cplusplus
}
#endif

#endif
