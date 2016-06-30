#ifndef _BUSMASTER_COMMAN_H_
#define _BUSMASTER_COMMAN_H_


#ifdef __cplusplus
#define GCC_EXTERN  extern "C" __cdecl
#define GCC_EXPORT  __declspec(dllexport)
#else
#define GCC_EXTERN
#define GCC_EXPORT
#endif

const unsigned int MAX_SEND_TIME = 3; // Millisecond
const int ERR_INVALID_ADDRESS   = -100;

#define BMKEY_BACK          0x80
#define BMKEY_TAB           0x81
#define BMKEY_RETURN        0x82
#define BMKEY_PAGEUP        0x83
#define BMKEY_PAGEDOWN      0x84
#define BMKEY_END           0x85
#define BMKEY_HOME          0x86
#define BMKEY_LEFTARROW     0x87
#define BMKEY_UPARROW       0x88
#define BMKEY_RIGHTARROW    0x89
#define BMKEY_DOWNARROW     0x8a
#define BMKEY_INSERT        0x8b
#define BMKEY_DELETE        0x8c
#define BMKEY_F1            0x8d
#define BMKEY_F2            0x8e
#define BMKEY_F3            0x8f
#define BMKEY_F4            0x90
#define BMKEY_F5            0x91
#define BMKEY_F6            0x92
#define BMKEY_F7            0x93
#define BMKEY_F8            0x94
#define BMKEY_F9            0x95
#define BMKEY_F10           0x96
#define BMKEY_F11           0x97
#define BMKEY_F12           0x98


#endif