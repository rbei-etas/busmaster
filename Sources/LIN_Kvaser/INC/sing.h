/*
**                        Copyright 1997 by KVASER AB
**            P.O Box 4076 S-51104 KINNAHULT, SWEDEN Tel. +46 320 15287
**
** This software is furnished under a license and may be used and copied
** only in accordance with the terms of such license.
**
*/
/*
** This file replaces cancard.h from Softing GmbH.
*/

#ifndef _SING_H_
#define _SING_H_

#if defined(__WIN32__) || defined(_WIN32)

#  if !defined(FAR)
#     define FAR
#  endif
#  if !defined(SINGAPI)
#     define SINGAPI extern int __declspec(dllimport) __stdcall
#  endif

#else

#  if !defined(FAR)
#     define FAR far
#  endif
#  if !defined(SINGAPI)
#     define SINGAPI extern int __export FAR PASCAL
#  endif

#endif

typedef struct  {
    unsigned long   Ident;
    int             DataLength;
    int             RecOverrun_flag;
    int             RCV_fifo_lost_msg;
    unsigned char   RCV_data[8];
    int             AckOverrun_flag;
    int             XMT_ack_fifo_lost_acks;
    int             XMT_rmt_fifo_lost_remotes;
    int             Bus_state;
    int             Error_state;
    int             Can;
    unsigned long   Time;
} param_struct;

#ifdef __cplusplus
extern "C" {
#endif

SINGAPI INIPC_initialize_board(int address);

SINGAPI CANPC_reset_board(void);

SINGAPI CANPC_reset_chip(void);

SINGAPI CANPC_initialize_chip (int presc, int sjw, int tseg1,
                               int tseg2, int sam);

SINGAPI CANPC_initialize_chip2(int presc, int sjw, int tseg1,
                               int tseg2, int sam);

SINGAPI CANPC_set_mode (int SleepMode, int SpeedMode);

SINGAPI CANPC_set_mode2(int SleepMode, int SpeedMode);

SINGAPI CANPC_set_acceptance (unsigned int AccCodeStd,
                              unsigned int AccMaskStd,
                              unsigned long AccCodeXtd,
                              unsigned long AccMaskXtd);

SINGAPI CANPC_set_acceptance2(unsigned int AccCodeStd,
                              unsigned int AccMaskStd,
                              unsigned long AccCodeXtd,
                              unsigned long AccMaskXtd);

SINGAPI CANPC_set_output_control (int OutputControl);

SINGAPI CANPC_set_output_control2(int OutputControl);

SINGAPI CANPC_initialize_interface(int ReceiveFifoEnable,
                                   int ReceivePollAll,
                                   int ReceiveEnableAll,
                                   int ReceiveIntEnableAll,
                                   int AutoRemoteEnableAll,
                                   int TransmitReqFifoEnable,
                                   int TransmitPollAll,
                                   int TransmitAckEnableAll,
                                   int TransmitAckFifoEnableAll,
                                   int TransmitRmtFifoEnableAll);

SINGAPI CANPC_enable_fifo(void);

SINGAPI CANPC_optimize_rcv_speed(void);

SINGAPI CANPC_enable_timestamps(void);

SINGAPI CANPC_enable_fifo_transmit_ack (void);

SINGAPI CANPC_enable_fifo_transmit_ack2(void);

SINGAPI CANPC_get_version(int * sw_version,
                          int *  fw_version,
                          int * hw_version,
                          int * license,
                          int * can_chip_type);

SINGAPI CANPC_start_chip(void);

SINGAPI CANPC_read_ac(param_struct * param);

SINGAPI CANPC_send_data (unsigned long Ident, int Xtd,
                         int DataLength, unsigned char * pData);

SINGAPI CANPC_send_data2(unsigned long Ident, int Xtd,
                         int DataLength, unsigned char * pData);

SINGAPI CANPC_send_remote (unsigned long Ident, int Xtd, int DataLength);

SINGAPI CANPC_send_remote2(unsigned long Ident, int Xtd, int DataLength);

SINGAPI CANPC_reinitialize(void);

SINGAPI CANPC_reset_rcv_fifo(void);

SINGAPI CANPC_get_time(unsigned long * time);

SINGAPI INIPC_close_board(void);

SINGAPI CANPC_set_interrupt_event(HANDLE hEvent);

SINGAPI CANPC_get_serial_number(unsigned long * ser_number);

SINGAPI CANPC_get_trigger (int * level);

SINGAPI CANPC_get_trigger2(int * level);

SINGAPI CANPC_set_trigger (int level);

SINGAPI CANPC_set_trigger2(int level);

SINGAPI CANPC_get_bus_state(int Can);

SINGAPI CANPC_reset_xmt_fifo(void);

SINGAPI CANPC_reset_lost_msg_counter(void);

SINGAPI CANPC_read_rcv_fifo_level(void);

SINGAPI CANPC_read_xmt_fifo_level(void);

SINGAPI CANPC_set_path(char * path);

SINGAPI CANPC_enable_error_frame_detection(void);

// Dummy functions.
SINGAPI CANPC_define_cyclic (int ObjectNumber,
                             unsigned int Rate,
                             unsigned int Cycles);
SINGAPI CANPC_define_cyclic2(int ObjectNumber,
                             unsigned int Rate,
                             unsigned int Cycles);
SINGAPI CANPC_define_object (unsigned long Handle,
                             int FAR * ObjectNumber,
                             int Type,
                             int ReceiveIntEnable,
                             int AutoRemoteEnable,
                             int TransmitAckEnable);
SINGAPI CANPC_define_object2(unsigned long Handle,
                             int FAR * ObjectNumber,
                             int Type,
                             int ReceiveIntEnable,
                             int AutoRemoteEnable,
                             int TransmitAckEnable);
SINGAPI CANPC_enable_dyn_obj_buf(void);
SINGAPI CANPC_initialize_interface(int ReceiveFifoEnable,
                                   int ReceivePollAll,
                                   int ReceiveEnableAll,
                                   int ReceiveIntEnableAll,
                                   int AutoRemoteEnable,
                                   int TransmitReqFifoEnable,
                                   int TransmitPollAll,
                                   int TransmitAckEnableAll,
                                   int TransmitAckFifoEnable,
                                   int TransmitRmtFifoEnable);
SINGAPI CANPC_read_rcv_data (int  ObjectNumber,
                             unsigned char FAR * pRCV_Data,
                             unsigned long FAR * Time);
SINGAPI CANPC_read_rcv_data2(int  ObjectNumber,
                             unsigned char FAR * pRCV_Data,
                             unsigned long FAR * Time);
SINGAPI CANPC_read_xmt_data (int  ObjectNumber,
                             int FAR * pDataLength,
                             unsigned char FAR * pXMT_Data);
SINGAPI CANPC_read_xmt_data2(int  ObjectNumber,
                             int FAR * pDataLength,
                             unsigned char FAR * pXMT_Data);
SINGAPI CANPC_send_object (int  ObjectNumber,
                           int  DataLength);
SINGAPI CANPC_send_object2(int  ObjectNumber,
                           int  DataLength);
SINGAPI CANPC_send_remote_object (int ObjectNumber,
                                  int DataLength);
SINGAPI CANPC_send_remote_object2(int ObjectNumber,
                                  int DataLength);
SINGAPI CANPC_supply_object_data (int  ObjectNumber,
                                  int  DataLength,
                                  unsigned char FAR * pData);
SINGAPI CANPC_supply_object_data2(int  ObjectNumber,
                                  int  DataLength,
                                  unsigned char FAR * pData);
SINGAPI CANPC_supply_rcv_object_data (int  ObjectNumber,
                                      int  DataLength,
                                      unsigned char FAR * pData);
SINGAPI CANPC_supply_rcv_object_data2(int  ObjectNumber,
                                      int  DataLength,
                                      unsigned char FAR * pData);
SINGAPI CANPC_write_object (int  ObjectNumber,
                            int  DataLength,
                            unsigned char FAR * pData);
SINGAPI CANPC_write_object2(int  ObjectNumber,
                            int  DataLength,
                            unsigned char FAR * pData);




// Additions.
SINGAPI CANPC_GetEventHandle(HANDLE *h);
SINGAPI CANPC_SetTimerRate(unsigned int rate);

#ifdef __cplusplus
}
#endif

#endif
