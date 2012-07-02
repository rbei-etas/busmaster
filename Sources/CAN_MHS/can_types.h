#ifndef __CAN_TYPES_H__
#define __CAN_TYPES_H__

#ifdef WIN32
#ifndef __WIN32__
#define __WIN32__
#endif
#endif

#ifdef __WIN32__
#include <windows.h>

#define int32_t __int32
#define uint32_t unsigned __int32
#define int16_t __int16
#define uint16_t unsigned __int16
// Linux & Mac
#else
//#include <unistd.h>
//#include <stdlib.h>
//#include <sys/time.h>
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define INDEX_FIFO_PUFFER_MASK 0x0000FFFF
#define INDEX_SOFT_FLAG        0x02000000
#define INDEX_RXD_TXT_FLAG     0x01000000
#define INDEX_CAN_KANAL_MASK   0x000F0000
#define INDEX_CAN_DEVICE_MASK  0x00F00000

#define INDEX_CAN_KANAL_A      0x00000000
#define INDEX_CAN_KANAL_B      0x00010000

    /***************************************************************/
    /*  Typen                                                      */
    /***************************************************************/

    /******************************************/
    /*            CAN Message Type            */
    /******************************************/
#define MsgFlags Flags.Long
#define MsgLen Flags.Flag.Len
#define MsgRTR Flags.Flag.RTR
#define MsgEFF Flags.Flag.EFF
#define MsgTxD Flags.Flag.TxD
#define MsgData Data.Bytes

    struct TCanFlagsBits
    {
        unsigned Len:4;   // Dlc
        unsigned TxD:1;   // TxD -> 1 = Tx CAN Message, 0 = Rx CAN Message
        unsigned Res:1;   // Reserviert
        unsigned RTR:1;   // remote transmition request bit
        unsigned EFF:1;   // extended frame bit
        unsigned Res2:8;
    };

    union TCanFlags
    {
        struct TCanFlagsBits Flag;
        uint32_t Long;
    };

    union TCanData
    {
        char Chars[8];
        unsigned char Bytes[8];
        uint16_t Words[4];
        uint32_t Longs[2];
    };

    struct TTime
    {
        uint32_t Sec;
        uint32_t USec;
    };

    struct TCanMsg
    {
        uint32_t Id;
        union TCanFlags Flags;
        union TCanData Data;
        struct TTime Time;
    };

    /******************************************/
    /*         CAN Message Filter Type        */
    /******************************************/
#define FilFlags Flags.Long
#define FilEFF Flags.Flag.EFF
#define FilMode Flags.Flag.Mode
#define FilIdMode Flags.Flag.IdMode
#define FilEnable Flags.Flag.Enable


    struct TMsgFilterFlagsBits
    {
        // 1. Byte
        unsigned Len:4;   // Dlc
        unsigned Res:2;   // Reserviert
        unsigned RTR:1;   // remote transmition request bit
        unsigned EFF:1;   // extended frame bit
        // 2. Byte
        unsigned IdMode:2;   // 0 = Maske & Code
        // 1 = Start & Stop
        // 2 = Single Id
        unsigned DLCCheck:1;
        unsigned DataCheck:1;
        unsigned Res1:4;
        // 3. Byte
        unsigned Res2:8;
        // 4. Byte
        unsigned Type:4;   // 0 = Single Puffer
        unsigned Res3:2;
        unsigned Mode:1;   // 0 = Message entfernen
        // 1 = Message nicht entfernen
        unsigned Enable:1;
    };


    union TMsgFilterFlags
    {
        struct TMsgFilterFlagsBits Flag;
        uint32_t  Long;
    };

    struct TMsgFilter
    {
        uint32_t  Maske;
        uint32_t  Code;
        union TMsgFilterFlags Flags;
        union TCanData Data;
    };


#ifdef __cplusplus
}
#endif


#endif
