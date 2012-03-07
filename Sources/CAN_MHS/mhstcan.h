#ifndef __MHSTCAN_H__
#define __MHSTCAN_H__

#include "can_types.h"

#ifdef __cplusplus
  extern "C" {
#endif

/***************************************************************/
/* Define Makros                                               */
/***************************************************************/
// CAN Übertragungsgeschwindigkeit
#define CAN_10K_BIT   10    // 10 kBit/s
#define CAN_20K_BIT   20    // 20 kBit/s
#define CAN_50K_BIT   50    // 50 kBit/s
#define CAN_100K_BIT  100   // 100 kBit/s
#define CAN_125K_BIT  125   // 125 kBit/s
#define CAN_250K_BIT  250   // 250 kBit/s
#define CAN_500K_BIT  500   // 500 kBit/s
#define CAN_800K_BIT  800   // 800 kBit/s
#define CAN_1M_BIT    1000  // 1 MBit/s

// CAN Bus Mode
#define OP_CAN_NO_CHANGE         0  // Aktuellen Zustand nicht ändern
#define OP_CAN_START             1  // Startet den CAN-Bus
#define OP_CAN_STOP              2  // Stopt den CAN-Bus
#define OP_CAN_RESET             3  // Reset CAN Controller (BusOff löschen)
#define OP_CAN_START_LOM         4  // Startet den CAN-Bus im Silent Mode (Listen Only Mode)
#define OP_CAN_START_NO_RETRANS  5  // Startet den CAN-Bus im Automatic Retransmission disable Mode


#define CAN_CMD_NONE                0x0000
#define CAN_CMD_RXD_OVERRUN_CLEAR   0x0001
#define CAN_CMD_RXD_FIFOS_CLEAR     0x0002
#define CAN_CMD_TXD_OVERRUN_CLEAR   0x0004
#define CAN_CMD_TXD_FIFOS_CLEAR     0x0008
#define CAN_CMD_HW_FILTER_CLEAR     0x0010
#define CAN_CMD_SW_FILTER_CLEAR     0x0020
#define CAN_CMD_TXD_PUFFERS_CLEAR   0x0040

#define CAN_CMD_FIFOS_ERROR_CLEAR   0x000F
#define CAN_CMD_ALL_CLEAR           0x0FFF


// DrvStatus
#define DRV_NOT_LOAD              0  // Die Treiber DLL wurde noch nicht geladen
#define DRV_STATUS_NOT_INIT       1  // Treiber noch nicht Initialisiert
#define DRV_STATUS_INIT           2  // Treiber erfolgrich Initialisiert
#define DRV_STATUS_PORT_NOT_OPEN  3  // Die Schnittstelle wurde geöffnet
#define DRV_STATUS_PORT_OPEN      4  // Die Schnittstelle wurde nicht geöffnet
#define DRV_STATUS_DEVICE_FOUND   5  // Verbindung zur Hardware wurde Hergestellt
#define DRV_STATUS_CAN_OPEN       6  // Device wurde geöffnet und erfolgreich Initialisiert
#define DRV_STATUS_CAN_RUN_TX     7  // CAN Bus RUN nur Transmitter (wird nicht verwendet !)
#define DRV_STATUS_CAN_RUN        8  // CAN Bus RUN

// CanStatus
#define CAN_STATUS_OK          0     // CAN-Controller: Ok
#define CAN_STATUS_ERROR       1     // CAN-Controller: CAN Error
#define CAN_STATUS_WARNING     2     // CAN-Controller: Error warning
#define CAN_STATUS_PASSIV      3     // CAN-Controller: Error passiv
#define CAN_STATUS_BUS_OFF     4     // CAN-Controller: Bus Off
#define CAN_STATUS_UNBEKANNT   5     // CAN-Controller: Status Unbekannt

// Fifo Status
#define FIFO_OK                 0 // Fifo-Status: Ok
#define FIFO_HW_OVERRUN         1 // Fifo-Status: Hardware Fifo Überlauf
#define FIFO_SW_OVERRUN         2 // Fifo-Status: Software Fifo Überlauf
#define FIFO_HW_SW_OVERRUN      3 // Fifo-Status: Hardware & Software Fifo Überlauf
#define FIFO_STATUS_UNBEKANNT   4 // Fifo-Status: Unbekannt

// Makros für SetEvent
#define EVENT_ENABLE_PNP_CHANGE          0x0001
#define EVENT_ENABLE_STATUS_CHANGE       0x0002
#define EVENT_ENABLE_RX_FILTER_MESSAGES  0x0004
#define EVENT_ENABLE_RX_MESSAGES         0x0008
#define EVENT_ENABLE_ALL                 0x00FF

#define EVENT_DISABLE_PNP_CHANGE         0x0100
#define EVENT_DISABLE_STATUS_CHANGE      0x0200
#define EVENT_DISABLE_RX_FILTER_MESSAGES 0x0400
#define EVENT_DISABLE_RX_MESSAGES        0x0800
#define EVENT_DISABLE_ALL                0xFF00


/***************************************************************/
/*  Typen                                                      */
/***************************************************************/

/******************************************/
/*             Device Status              */
/******************************************/
struct TDeviceStatus
  {
  int32_t DrvStatus;
  unsigned char CanStatus;
  unsigned char FifoStatus;
  };


/***************************************************************/
/*  Tiny-CAN API Funktionen                                    */
/***************************************************************/
__declspec(dllimport) int32_t WINAPI CanInitDriver(char *options);
__declspec(dllimport) void WINAPI CanDownDriver(void);
__declspec(dllimport) int32_t WINAPI CanSetOptions(char *options);
__declspec(dllimport) int32_t WINAPI CanDeviceOpen(uint32_t  index, char *parameter);
__declspec(dllimport) int32_t WINAPI CanDeviceClose(uint32_t  index);
__declspec(dllimport) int32_t WINAPI CanApplaySettings(uint32_t  index);

__declspec(dllimport) int32_t WINAPI CanSetMode(uint32_t  index, unsigned char can_op_mode, uint16_t  can_command);
__declspec(dllimport) int32_t WINAPI CanSet(uint32_t  index, uint16_t  obj_index, uint16_t  obj_sub_index, void *data, int32_t size);
__declspec(dllimport) int32_t WINAPI CanGet(uint32_t  index, uint16_t  obj_index, uint16_t  obj_sub_index, void *data, int32_t size);

__declspec(dllimport) int32_t WINAPI CanTransmit(uint32_t  index, struct TCanMsg *msg, int32_t count);
__declspec(dllimport) void WINAPI CanTransmitClear(uint32_t  index);
__declspec(dllimport) uint32_t  WINAPI CanTransmitGetCount(uint32_t  index);
__declspec(dllimport) int32_t WINAPI CanTransmitSet(uint32_t  index, uint16_t  cmd,
  uint32_t  time);
__declspec(dllimport) int32_t WINAPI CanReceive(uint32_t  index, struct TCanMsg *msg, int32_t count);
__declspec(dllimport) void WINAPI CanReceiveClear(uint32_t  index);
__declspec(dllimport) uint32_t  WINAPI CanReceiveGetCount(uint32_t  index);

__declspec(dllimport) int32_t WINAPI CanSetSpeed(uint32_t  index, uint16_t  speed);
__declspec(dllimport) int32_t WINAPI CanSetSpeedUser(uint32_t  index, uint32_t  value);
__declspec(dllimport) char * WINAPI CanDrvInfo(void);
__declspec(dllimport) char * WINAPI CanDrvHwInfo(uint32_t  index);
__declspec(dllimport) int32_t WINAPI CanSetFilter(uint32_t  index, struct TMsgFilter *msg_filter);

__declspec(dllimport) int32_t WINAPI CanGetDeviceStatus(uint32_t  index, struct TDeviceStatus *status);

__declspec(dllimport) void WINAPI CanSetPnPEventCallback(void (__stdcall *event)(uint32_t  index, int32_t status));
__declspec(dllimport) void WINAPI CanSetStatusEventCallback(void (__stdcall *event)
   (uint32_t  index, struct TDeviceStatus *device_status));
__declspec(dllimport) void WINAPI CanSetRxEventCallback(void (__stdcall *event)
   (uint32_t  index, struct TCanMsg *msg, int32_t count));

__declspec(dllimport) void WINAPI CanSetEvents(uint16_t  events);
__declspec(dllimport) uint32_t  WINAPI CanEventStatus(void);


#ifdef __cplusplus
  }
#endif

#endif