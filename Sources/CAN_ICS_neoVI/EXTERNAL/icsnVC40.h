////////////////////////////////////////////////////////////////////////////////////
// ICSNVC40.h
// 4/16/2008
// Copyright Intrepid Control Systems, Inc. 2000-2008
// www.intrepidcs.com
////////////////////////////////////////////////////////////////////////////////////


#ifndef ICSNVC40_H
#define ICSNVC40_H

#include "icsSpyDataCStyle.h"

#define		NEOVI_COMMTYPE_RS232			   	0
#define		NEOVI_COMMTYPE_USB_BULK				1
#define		NEOVI_COMMTYPE_TCPIP		    	3
#define		NEOVI_COMMTYPE_FIRE_USB				5

// Driver Type Constants
#define INTREPIDCS_DRIVER_STANDARD		    0
#define	INTREPIDCS_DRIVER_TEST			    1
#define	INTREPIDCS_DRIVER_CYPRESS		    2
#define INTREPIDCS_DRIVER_SYNCTOPC	    	3
#define INTREPIDCS_DRIVER_ENHANCED_3G    	4

// device Type IDs NOTE: These are only valid for the older icsneoOpenPortEx() function
#define INTREPIDCS_DEVICE_NEO4			0
#define INTREPIDCS_DEVICE_VCAN			1
#define INTREPIDCS_DEVICE_NEO6			2
#define INTREPIDCS_DEVICE_UNKNOWN		3
#define INTREPIDCS_DEVICE_VCAN2_DW		4
#define INTREPIDCS_DEVICE_VCAN2_SW		5
#define INTREPIDCS_DEVICE_VCAN2_LSFT	6
#define INTREPIDCS_DEVICE_VCAN2_WIFI	7
#define INTREPIDCS_DEVICE_VCAN3_CANDOC	8
#define INTREPIDCS_DEVICE_NEOFIRE		10
#define INTREPIDCS_DEVICE_VCAN3		    	11
#define INTREPIDCS_DEVICE_YELLOW		12
#define INTREPIDCS_DEVICE_NEORED		INTREPIDCS_DEVICE_NEOFIRE

#define INTREPIDCS_15765_RX_ERR_GLOBAL			0x0001
#define INTREPIDCS_15765_RX_ERR_CFRX_EXP_FF		0x0002
#define INTREPIDCS_15765_RX_ERR_FCRX_EXP_FF		0x0004
#define INTREPIDCS_15765_RX_ERR_SFRX_EXP_CF		0x0008
#define INTREPIDCS_15765_RX_ERR_FFRX_EXP_CF		0x0010
#define INTREPIDCS_15765_RX_ERR_FCRX_EXP_CF		0x0020
#define	INTREPIDCS_15765_RX_ERR_CF_TIME_OUT		0x0040
#define	INTREPIDCS_15765_RX_COMPLETE			0x0080
#define	INTREPIDCS_15765_RX_IN_PROGRESS			0x0100
#define	INTREPIDCS_15765_RX_ERR_SEQ_ERR_CF		0x0200

#define		NETID_DEVICE	                    0
#define		NETID_HSCAN		     	            1
#define		NETID_MSCAN			                2
#define		NETID_SWCAN		    	            3
#define		NETID_LSFTCAN		                4
#define		NETID_FORDSCP		                5
#define		NETID_J1708		    	            6
#define		NETID_AUX			                7
#define		NETID_JVPW		                	8
#define		NETID_ISO			                9
#define		NETID_ISOPIC		               10
#define		NETID_MAIN51		               11
#define		NETID_RED		                   12
#define     NETID_SCI                          13
#define     NETID_ISO2                         14
#define     NETID_ISO14230                     15
#define		NETID_LIN			               16  
//
#define     NETID_ISO3                         41
#define     NETID_HSCAN2                       42
#define     NETID_HSCAN3                       44
#define     NETID_ISO4                         47
#define     NETID_LIN2                         48
#define     NETID_LIN3                         49
#define     NETID_LIN4                         50 
#define     NETID_MOST	                       51
#define	    NETID_RED_APP_ERROR                52
#define	    NETID_CGI			               53
#define	    NETID_3G_RESET_STATUS			   54
#define	   	NETID_3G_FB_STATUS			       55
#define	    NETID_3G_APP_SIGNAL_STATUS		   56
#define	    NETID_3G_READ_DATALINK_CM_TX_MSG   57
#define		NETID_3G_READ_DATALINK_CM_RX_MSG   58  
#define		NETID_3G_LOGGING_OVERFLOW		   59				
#define		NETID_3G_READ_SETTINGS_EX		   60
#define     NETID_SWCAN2                       61
#define     NETID_LSFTCAN2                     62
#define     NETID_RS232          	           63
#define     NETID_UART          	           64
#define     NETID_UART2          	           65
#define     NETID_UART3          	           66
#define     NETID_UART4          	           67
#define     NETID_ANALOG          	           68
#define     NETID_WIZNET                       69
#define     NETID_DATA_TO_HOST		           70
#define     NETID_TEXTAPI_TO_HOST	           71
#define		NETID_I2C1				           71
#define		NETID_SPI1				           72
//#define	    NETID_RED_GET_ISO15765_TX          72
//#define	    NETID_RED_GET_ISO15765_RX          73
#define	    NETID_RED_VBAT		               74
//
#define		NETID_FLEXRAY1A			           80
#define		NETID_FLEXRAY1B			           81
#define		NETID_FLEXRAY2A			           82
#define		NETID_FLEXRAY2B			           83
//                                              
#define     NETID_MOST25	                   90
#define     NETID_MOST50	                   91
#define     NETID_MOST150	                   92

//Upper boundry of net id's
#define NETID_MAX                             100

#define NEODEVICE_UNKNOWN           0
#define NEODEVICE_BLUE			           1
#define NEODEVICE_SW_VCAN		         2
#define NEODEVICE_DW_VCAN		         4
#define NEODEVICE_FIRE		           	8
#define NEODEVICE_VCAN3		         	16
#define NEODEVICE_YELLOW           32
#define NEODEVICE_RED		           	64
#define NEODEVICE_ECU		          	128
#define NEODEVICE_IEVB       	  		256
#define NEODEVICE_PENDANT	       	512
#define NEODEVICE_VIRTUAL_NEOVI   1024
#define NEODEVICE_ECUCHIP_UART		2048
/** Device type for connecting to a neoVI PLASMA.  
  * Currently supported over USB.  Could also be used in Android environment to connect 
  * to the Plasma (amalgamation of several VNETs) */
#define NEODEVICE_PLASMA_1_11				0x1000ul
/** Device type for an attached VNET interface.  
 * In using libicsneo, you can use this to create neoVI style connection to 
 * a particular vnet.  Additionally if someone needs to create a hard neovi connection
 * to one vnet from Windows/Linux, they can icsneoOpenDevice a neoVI that has VNET's
 * like Plasma and open sub connections using VNET device type.*/
#define NEODEVICE_FIRE_VNET					0x2000ul
/**
 *  NeoAnalog, USB peripheral that is instrumentation level analog to CAN.
 */
#define NEODEVICE_NEOANALOG		0x4000ul

#define NEODEVICE_CT_OBD        0x8000ul //FROG TRM - this is ControlTech's version of the ValueCAN3
           
#define NEODEVICE_PLASMA_1_12				0x10000ul
#define NEODEVICE_PLASMA_1_13				0x20000ul
#define NEODEVICE_ION                                   0x40000ul
/*
 * Any plasma hardware rev
 */
#define NEODEVICE_ANY_PLASMA (NEODEVICE_PLASMA_1_11 | NEODEVICE_FIRE_VNET | NEODEVICE_PLASMA_1_12 | NEODEVICE_PLASMA_1_13)
/**
 *  All devices (except for neoAnalog as this is not a Vspy3-able USB device.
 */
#define NEODEVICE_ALL             0xFFFFBFFF

#define NEODEVICE_NEOECUCHIP	NEODEVICE_IEVB
//deprecated netids
#define         NETID_FIRE_HSCAN2           NETID_HSCAN2
#define         NETID_FIRE_MSCAN2           NETID_HSCAN3
#define         NETID_FIRE_HSCAN3           NETID_HSCAN3
#define         NETID_FIRE_LIN2             NETID_LIN2
#define         NETID_FIRE_LIN3             NETID_LIN3
#define         NETID_FIRE_LIN4 			NETID_LIN4
#define	       	NETID_FIRE_CGI				NETID_CGI


#define DEVICE_OPTION_ENABLE_TIMESYNC		(0x00000001)
#define DEVICE_OPTION_DISABLE_AUTO_UPDATE	(0x00000002)
#define DEVICE_OPTION_DONT_ENABLE_NETCOMS   (0x00000004)
#define DEVICE_OPTION_DONT_USE_NEOVISERVER  (0x00000008)

#define ISO15765_2_NETWORK_HSCAN    0x01
#define ISO15765_2_NETWORK_MSCAN    0x02
#define ISO15765_2_NETWORK_HSCAN2   0x04
#define ISO15765_2_NETWORK_HSCAN3   0x08
#define ISO15765_2_NETWORK_SWCAN    0x10

#define PLASMA_SLAVE1_OFFSET 100
#define PLASMA_SLAVE2_OFFSET 224
#define PLASMA_SLAVE_NUM 43
/*
VNET Network index matrix.
If above defines are modified you can easily
view/update this table using Excel/OpenOffice...

Network	Main Vnet	Slave1	Slave2
NETID_DEVICE	0	100	224
NETID_HSCAN	1	101	225
NETID_MSCAN	2	102	226
NETID_SWCAN	3	103	227
NETID_LSFTCAN	4	104	228
NETID_FORDSCP	5	105	229
NETID_J1708	6	106	230
NETID_AUX	7	107	231
NETID_JVPW	8	108	232
NETID_ISO	9	109	233
NETID_ISOPIC	10	110	234
NETID_MAIN51	11	111	235
NETID_RED	12	112	236
NETID_SCI	13	113	237
NETID_ISO2	14	114	238
NETID_ISO14230	15	115	239
NETID_LIN	16	116	240
NETID_ISO3	41	117	241
NETID_HSCAN2	42	118	242
NETID_HSCAN3	44	119	243
NETID_ISO4	47	120	244
NETID_LIN2	48	121	245
NETID_LIN3	49	122	246
NETID_LIN4	50	123	247
NETID_MOST	51	124	248
NETID_CGI	53	125	249
NETID_I2C1	71	126	250
NETID_SPI1	72	127	251
NETID_FLEXRAY1A	80	128	252
NETID_FLEXRAY1B	81	140	264
NETID_FLEXRAY2A	82	141	265
NETID_FLEXRAY2B	83	142	266
NETID_MOST25	90	129	253
NETID_MOST50	91	130	254
NETID_MOST150	92	131	255
NETID_SWCAN2	61	132	256
NETID_LSFTCAN2	62	133	257
NETID_RS232	63	134	258
NETID_UART	64	135	259
NETID_UART2	65	136	260
NETID_UART3	66	137	261
NETID_UART4	67	138	262
NETID_ANALOG	68	139	263
*/

typedef struct //_NeoDevice
{
	unsigned long DeviceType;
	int Handle;
	int NumberOfClients;
	int SerialNumber;
	int MaxAllowedClients;

}NeoDevice;

typedef struct _NeoDeviceEx
{
	NeoDevice neoDevice;

	unsigned long FirmwareMajor;
	unsigned long FirmwareMinor;  

	#define CANNODE_STATUS_COREMINI_IS_RUNNING	(0x1)
	#define CANNODE_STATUS_IN_BOOTLOADER		(0x2)
	unsigned long Status;  // Bitfield, see defs above
     
	/** Option bit flags */                         
	#define MAIN_VNET				(0x01)
	#define SLAVE_VNET_A				(0x02)
	#define SLAVE_VNET_B				(0x04)
	unsigned long Options;
	unsigned long Reserved[15];   // may be expanded in future revisions

}NeoDeviceEx;

typedef union tagOptionsOpenNeoEx
{
	struct
	{
		int iNetworkID; //!< Network ID indicating which CAN network to communicate over.
	}CANOptions;
	unsigned long Reserved[16];   //!< may be expanded in future revisions.
}OptionsOpenNeoEx,*POptionsOpenNeoEx;

typedef union tagOptionsFindNeoEx
{
	struct
	{
		int iNetworkID; //!< Network ID indicating which CAN network to communicate over.
	}CANOptions;
	unsigned long Reserved[16];   //!< may be expanded in future revisions.

}OptionsFindNeoEx,*POptionsFindNeoEx;


// Structure for firmware info
#if !defined(linux) && !defined(QNX_OS)

#include <pshpack1.h>

#endif

struct stAPIFirmwareInfo
{
	int iType;  // 1,2,3 for Generation of HW.
	
	// Date and Time (only valid for type 1 and 2
	int  iMainFirmDateDay;
	int  iMainFirmDateMonth;
	int  iMainFirmDateYear;
	int  iMainFirmDateHour;
	int  iMainFirmDateMin;
	int  iMainFirmDateSecond;
	int  iMainFirmChkSum;

	// Version data (only valid for type 3)
	unsigned char iAppMajor;
	unsigned char iAppMinor;

	unsigned char iManufactureDay;
	unsigned char iManufactureMonth;
	unsigned short iManufactureYear;

	unsigned char iBoardRevMajor;
	unsigned char iBoardRevMinor;

	unsigned char iBootLoaderVersionMajor;
	unsigned char iBootLoaderVersionMinor;

};

#if !defined(linux) && !defined(QNX_OS)

#include <poppack.h>

#endif

// HSCAN neoVI or ValueCAN
#define		NEO_CFG_MPIC_HS_CAN_CNF1 (512 + 10)
#define		NEO_CFG_MPIC_HS_CAN_CNF2 (512 + 9)
#define		NEO_CFG_MPIC_HS_CAN_CNF3 (512 + 8)
#define		NEO_CFG_MPIC_HS_CAN_MODE (512 + 54)

// med speed neoVI CAN
#define		NEO_CFG_MPIC_MS_CAN_CNF1 (512 + 22)
#define		NEO_CFG_MPIC_MS_CAN_CNF2 (512 + 21)
#define		NEO_CFG_MPIC_MS_CAN_CNF3 (512 + 20)

// med speed neoVI CAN
#define		NEO_CFG_MPIC_SW_CAN_CNF1 (512 + 34)
#define		NEO_CFG_MPIC_SW_CAN_CNF2 (512 + 33)
#define		NEO_CFG_MPIC_SW_CAN_CNF3 (512 + 32)

// med speed neoVI CAN
#define		NEO_CFG_MPIC_LSFT_CAN_CNF1 (512 + 46)
#define		NEO_CFG_MPIC_LSFT_CAN_CNF2 (512 + 45)
#define		NEO_CFG_MPIC_LSFT_CAN_CNF3 (512 + 44)

// neoVI Error Constants ///////////////////////////////////////////////////////////
#define NEOVI_ERROR_DLL_TX_BUFFER_OVERFLOW   0 
#define NEOVI_ERROR_DLL_ERROR_BUFFER_OVERFLOW   1 
#define NEOVI_ERROR_DLL_USB_SEND_DATA_ERROR   2 
#define NEOVI_ERROR_DLL_ISO_DATA_BUFFER_ALLOC   3 
#define NEOVI_ERROR_DLL_ISO_DATA_READ_BUFFER   4 
#define NEOVI_ERROR_DLL_ISO_DATA_ZERO_PACKETS  5 
#define NEOVI_ERROR_DLL_RX_MSG_BUFFER_OVERFLOW  6 
#define NEOVI_ERROR_DLL_STOP_ISO_STREAM  7 
#define NEOVI_ERROR_DLL_INVALID_NETID  8 
#define NEOVI_ERROR_DLL_PROBLEM_STOPPING_RX_THREAD  9 
#define NEOVI_ERROR_DLL_PROBLEM_STOPPING_TX_THREAD  10 
#define NEOVI_ERROR_DLL_MAIN_PIC_BUFFER_OVERFLOW  11 
#define NEOVI_ERROR_DLL_INVALID_DEVICE_RESPONSE  12 
#define NEOVI_ERROR_DLL_ISOTX_DATA_BUFFER_ALLOC  13 
#define NEOVI_ERROR_DLL_RX_CMD_BUFFER_OVERFLOW 14 
#define NEOVI_ERROR_DLL_RS232_RX_BUFFER_OVERFLOW 15 
#define NEOVI_ERROR_DLL_RS232_ERR_READCOMERR  16 
#define NEOVI_ERROR_DLL_RS232_ERR_READ 17 
#define NEOVI_ERROR_DLL_RS232_BUFFER_ALLOC 18 
#define NEOVI_ERROR_DLL_RS232_TX_BUFFER_OVERFLOW 19 
#define NEOVI_ERROR_DLL_RS232_MISC_ERROR 20 
#define NEOVI_ERROR_DLL_RS232_FIND_WRITE 21 
#define NEOVI_ERROR_DLL_RS232_FIND_BUFFER_ALLOC 22 
#define NEOVI_ERROR_DLL_RS232_FIND_CLEARCOMM 23 
#define NEOVI_ERROR_DLL_RS232_FIND_READCOMM 24 
#define NEOVI_ERROR_DLL_RS232_FIND_TIMEOUT 25 
#define NEOVI_ERROR_DLL_RS232_ERR_BREAK 26 
#define NEOVI_ERROR_DLL_RS232_ERR_FRAME 27 
#define NEOVI_ERROR_DLL_RS232_ERR_IOE 28 
#define NEOVI_ERROR_DLL_RS232_ERR_OVERRUN 29 
#define NEOVI_ERROR_DLL_RS232_ERR_PARITY 30 
#define NEOVI_ERROR_DLL_RS232_TXBUFFER_ALLOC 31  
#define NEOVI_ERROR_DLL_USB_TX_RS232_ERROR 32 
#define NEOVI_ERROR_DLL_RS232_CREATE_FILE 33 
#define NEOVI_ERROR_DLL_RS232_GET_COMM_STATE 34 
#define NEOVI_ERROR_DLL_RS232_SET_COMM_STATE 35 
#define NEOVI_ERROR_DLL_RS232_START_COMM_RX_THREAD 36 
#define NEOVI_ERROR_DLL_RS232_START_COMM_TX_THREAD 37 
#define NEOVI_ERROR_DLL_SYNC_COUNT_ERR 38 
#define NEOVI_ERROR_DLL_RX_MSG_FRAME_ERR 39 
#define NEOVI_ERROR_DLL_RX_MSG_FIFO_OVER 40 
#define NEOVI_ERROR_DLL_RX_MSG_CHK_SUM_ERR 41 
#define NEOVI_ERROR_DLL_PROBLEM_STOPPING_BULKIN_THREAD 42 
#define NEOVI_ERROR_DLL_BULKIN_ERR_READ 43 
#define NEOVI_ERROR_DLL_MAIN51_RX_FIFO_OVERFLOW 44 
#define NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW 45 
#define NEOVI_ERROR_DLL_MAIN51_DEV_FIFO_OVERFLOW 46 
#define NEOVI_ERROR_DLL_RESET_STATUS_CHANGED 47 
#define NEOVI_ERROR_DLL_ISO_LONG_CACHE_OVERFLOW 48 
#define NEOVI_ERROR_DLL_ISORX_LONG_BUFFER_ALLOC 49 
#define NEOVI_ERROR_DLL_J1708_LONG_CACHE_OVERFLOW 50 
#define NEOVI_ERROR_DLL_J1708_LONG_BUFFER_ALLOC 51 
#define NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_DEVICE  52 
#define NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_HSCAN  53 
#define NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_MSCAN  54 
#define NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_SWCAN  55 
#define NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_LSFTCAN  56 
#define NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_FORDSCP  57 
#define NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_J1708  58 
#define NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_AUX  59 
#define NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_JVPW  60 
#define NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_ISO  61 
#define NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_ISOPIC  62 
#define NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_MAIN51  63 
#define NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_HOST  64 
#define NEOVI_ERROR_DLL_READ_ENTIRE_DEEPROM_ERROR  65 
#define  NEOVI_ERROR_DLL_WRITE_ENTIRE_DEEPROM_ERROR 66 
#define  NEOVI_ERROR_DLL_USB_PORT_ALREADY_OPEN 67 
#define  NEOVI_ERROR_DLL_JVPW_TX_REPORT_FIFO_ERR_IN 68 
#define  NEOVI_ERROR_DLL_ISOJ_TX_REPORT_FIFO_ERR_IN 69 
#define  NEOVI_ERROR_DLL_JVPW_TX_REPORT_FIFO_ERR_OUT 70 
#define  NEOVI_ERROR_DLL_ISOJ_TX_REPORT_FIFO_ERR_OUT 71 
#define  NEOVI_ERROR_DLL_MAIN51_TX_IN_FROM_HOST_FIFO 72 
#define  NEOVI_ERROR_DLL_MAIN51_TX_HOST_CHKSUM 73 
#define NEOVI_ERROR_DLL_ISOJ_TX_HOST_MISSED_BYTE 74 
#define NEOVI_ERROR_DLL_NEOVI_NO_RESPONSE 75 
#define NEOVI_ERROR_DLL_RX_SOCKET_FIFO_OVER 76 
#define NEOVI_ERROR_DLL_PROBLEM_STOPPING_TXSOCKET_THREAD 77 
#define NEOVI_ERROR_DLL_PROBLEM_STOPPING_RXSOCKET_THREAD 78 
#define NEOVI_ERROR_DLL_PROBLEM_STOPPING_RXSOCKET_CLIENT_THREAD 79 
#define NEOVI_ERROR_DLL_TCP_CLIENT_TX 80 
#define NEOVI_ERROR_DLL_TCP_CLIENT_RX 81 
#define NEOVI_ERROR_DLL_TCP_CLIENT_RX_SOCK 82 
#define NEOVI_ERROR_DLL_UNABLE_CONNECT_TO_SRVR 83 
#define NEOVI_ERROR_DLL_UNABLE_CREATE_CLIENT_SOCK 84 
#define NEOVI_ERROR_DLL_UNABLE_WSASTARTUP 85 
#define NEOVI_ERROR_DLL_SOCK_CL_RD_BUFFER_ALLOC 86 
#define NEOVI_ERROR_DLL_SOCK_CL_TX_BUFFER_ALLOC 87 
#define NEOVI_ERROR_DLL_SOCK_SRVR_RX_BUFFER_ALLOC 88 
#define NEOVI_ERROR_DLL_SOCK_SRVR_TX_BUFFER_ALLOC 89 
#define NEOVI_ERROR_DLL_ILLEGAL_TX_NETWORK 90 
#define NEOVI_ERROR_DLL_MAIN51_TX_HOST_OVERRUN 91 
#define NEOVI_ERROR_DLL_OPEN_GET_COMM_TIMEOUT 92 
#define NEOVI_ERROR_DLL_OPEN_SET_COMM_TIMEOUT 93 
#define NEOVI_ERROR_DLL_OPEN_READ_DEVICE_TYPE 94 
#define NEOVI_ERROR_DLL_OPEN_READ_DEVICE_TYPE_TOUT 95 
#define NEOVI_ERROR_DLL_CLOSE_PURGE_COMM 96 
#define NEOVI_ERROR_DLL_TX_COM_FIFO_OVERFLOW 97 
#define NEOVI_ERROR_DLL_GET_USBSERIAL_DEVICES 98 
#define  NEOVI_ERROR_DLL_USB_TX_RS232_BCOUNT 99 
#define  NEOVI_ERROR_DLL_OPEN_INBOOTLOADER 100 
#define  NEOVI_ERROR_DLL_TOO_MANY_PERIODICS 101 
#define  NEOVI_ERROR_DLL_PROBLEM_FIRMWARE_INFO 102 
#define  NEOVI_ERROR_DLL_NRED_ODDNUMBYTES 103 
#define  NEOVI_ERROR_DLL_NRED_UNKNOWN_RED_NETID 104 
#define  NEOVI_ERROR_DLL_RED_NOT_SUPPORTED 105 
/* 106 - 116 reserved for neovi red bootloader exceptions */
#define  NEOVI_ERROR_DLL_RED_BL_START_INDEX 106 
#define  NEOVI_ERROR_DLL_3G_BL_FAILURE 107 
#define  NEOVI_ERROR_DLL_RED_BL_END_INDEX 116 
#define  NEOVI_ERROR_DLL_RED_FAILED_TO_ENTER_BL 117 
#define  NEOVI_ERROR_DLL_RED_REQ_SERIAL_NUMBER 118 
#define  NEOVI_ERROR_DLL_RED_AUTHENTICATE 119 
#define  NEOVI_ERROR_DLL_RED_APP_VERSION 120 
#define  NEOVI_ERROR_DLL_RED_SET_BAUD_COMM_FAILURE 121 
#define  NEOVI_ERROR_DLL_RED_INVALID_BAUD_SPECIFIED 122 
#define  NEOVI_ERROR_DLL_RED_READ_BAUD_COMM_FAILURE 123 
#define  NEOVI_ERROR_DLL_RED_FAILED_TO_SAVE_EEPROM 124 
#define  NEOVI_ERROR_DLL_RED_FAILED_TO_UPDATE_WAVEFORM_CHANNEL 125 

#define  NEOVI_ERROR_DLL_RED_RX_MSG_FULL_UNKNOWN_NETWORK 126 
#define  NEOVI_ERROR_DLL_RED_TX_MSG_FULL_UNKNOWN_NETWORK 127 
#define  NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_UNKNOWN_NETWORK 128 

#define  NEOVI_ERROR_DLL_RED_RX_MSG_FULL_HSCAN1 129 
#define  NEOVI_ERROR_DLL_RED_TX_MSG_FULL_HSCAN1 130 
#define  NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_HSCAN1 131 
#define  NEOVI_ERROR_DLL_RED_DRIVER_OVERFLOW_HSCAN1   132 

#define  NEOVI_ERROR_DLL_RED_RX_MSG_FULL_HSCAN2 133 
#define  NEOVI_ERROR_DLL_RED_TX_MSG_FULL_HSCAN2 134 
#define  NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_HSCAN2 135 
#define  NEOVI_ERROR_DLL_RED_DRIVER_OVERFLOW_HSCAN2   136 

#define  NEOVI_ERROR_DLL_RED_RX_MSG_FULL_MSCAN1 137 
#define  NEOVI_ERROR_DLL_RED_TX_MSG_FULL_MSCAN1 138 
#define  NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_MSCAN1 139 
#define  NEOVI_ERROR_DLL_RED_DRIVER_OVERFLOW_MSCAN1   140 

#define  NEOVI_ERROR_DLL_RED_RX_MSG_FULL_HSCAN3 141 
#define  NEOVI_ERROR_DLL_RED_TX_MSG_FULL_HSCAN3 142 
#define  NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_HSCAN3 143 
#define  NEOVI_ERROR_DLL_RED_DRIVER_OVERFLOW_HSCAN3   144 

#define  NEOVI_ERROR_DLL_RED_RX_MSG_FULL_SWCAN 145 
#define  NEOVI_ERROR_DLL_RED_TX_MSG_FULL_SWCAN 146 
#define  NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_SWCAN 147 
#define  NEOVI_ERROR_DLL_RED_DRIVER_OVERFLOW_SWCAN   148 

#define  NEOVI_ERROR_DLL_RED_RX_MSG_FULL_LSFTCAN 149 
#define  NEOVI_ERROR_DLL_RED_TX_MSG_FULL_LSFTCAN 150 
#define  NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_LSFTCAN 151 
#define  NEOVI_ERROR_DLL_RED_DRIVER_OVERFLOW_LSFTCAN   152 

#define  NEOVI_ERROR_DLL_RED_RX_MSG_FULL_LIN1 153 
#define  NEOVI_ERROR_DLL_RED_TX_MSG_FULL_LIN1 154 
#define  NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_LIN1 155 
#define  NEOVI_ERROR_DLL_RED_DRIVER_OVERFLOW_LIN1   156 

#define  NEOVI_ERROR_DLL_RED_RX_MSG_FULL_LIN2 157 
#define  NEOVI_ERROR_DLL_RED_TX_MSG_FULL_LIN2 158 
#define  NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_LIN2 159 
#define  NEOVI_ERROR_DLL_RED_DRIVER_OVERFLOW_LIN2   160 

#define  NEOVI_ERROR_DLL_RED_RX_MSG_FULL_LIN3 161 
#define  NEOVI_ERROR_DLL_RED_TX_MSG_FULL_LIN3 162 
#define  NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_LIN3 163 
#define  NEOVI_ERROR_DLL_RED_DRIVER_OVERFLOW_LIN3   164 

#define  NEOVI_ERROR_DLL_RED_RX_MSG_FULL_LIN4 165 
#define  NEOVI_ERROR_DLL_RED_TX_MSG_FULL_LIN4 166 
#define  NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_LIN4 167 
#define  NEOVI_ERROR_DLL_RED_DRIVER_OVERFLOW_LIN4   168 

#define  NEOVI_ERROR_DLL_USB_PURGE_FAILED 169 
#define  NEOVI_ERROR_FIRE_COMM_BAD_PACKET 170 
#define  NEOVI_ERROR_FIRE_CGI_COMM_BAD_PACKET 171 

                                                   
#define  NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_HSCAN1 172 
#define  NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_HSCAN2 173 
#define  NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_HSCAN3 174 
#define  NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_MSCAN 175 
#define  NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_SWCAN 176 
#define  NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_LSFTCAN 177 
#define  NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_LIN1 178 
#define  NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_LIN2 179 
#define  NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_LIN3 180 
#define  NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_LIN4 181 
#define  NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_UNKNOWN_NETWORK 182 
#define  NEOVI_ERROR_DLL_RED_RX_MSG_FULL_JVPW 183 
#define  NEOVI_ERROR_DLL_RED_TX_MSG_FULL_JVPW 184 
#define  NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_JVPW 185 
#define  NEOVI_ERROR_DLL_RED_DRIVER_OVERFLOW_JVPW   186 
#define  NEOVI_ERROR_DLL_INTERNAL_SERIAL_NO_DOES_NOT_MATCH_REGISTRY_SERIAL_NO   187 
#define NEOVI_ERROR_DLL_JVPW_LONG_CACHE_OVERFLOW    188 
#define NEOVI_ERROR_DLL_FAILED_TO_SET_LICENSE   189 
#define NEOVI_ERROR_DLL_3G_DEVICE_LICENSE_NEEDS_TO_BE_UPGRADED   190 

#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_HSCAN   191 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_MSCAN    192 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_HSCAN2    193 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_HSCAN3    194 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_LSFT    195 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_SW    196 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_LIN1    197 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_LIN2    198 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_LIN3    199 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_LIN4    200 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_JVPW    201 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_KYW    202 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_J1708    203 
#define NEOVI_ERROR_DLL_MAIN51_RTC_INVALID   204 
#define NEOVI_ERROR_DLL_MAIN51_LOADED_DEFAULT_SETTINGS   205 

#define NEOVI_ERROR_DLL_RED_RX_MSG_FULL_CGI   206 
#define NEOVI_ERROR_DLL_RED_TX_MSG_FULL_CGI   207 
#define NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_CGI   208 
#define NEOVI_ERROR_DLL_RED_DRIVER_OVERFLOW_CGI   209 
#define NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_CGI   210 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_CGI   211 
#define NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_JVPW 212 

//DLL API ERRORS
#define NEOVI_ERROR_DLL_INVALID_SCRIPT_LOCATION   213 
#define NEOVI_ERROR_DLL_SDCARD_NOT_INSERTED   214 
#define NEOVI_ERROR_DLL_SDCARD_NOT_FORMATTED   215 
#define NEOVI_ERROR_DLL_SDCARD_WRITE_ERROR   216 
#define NEOVI_ERROR_DLL_SDCARD_READ_ERROR   217 
#define NEOVI_ERROR_DLL_SCRIPT_START_ERROR   218 
#define NEOVI_ERROR_DLL_SCRIPT_INVALID_FUNCBLOCK_INDEX   219 
#define NEOVI_ERROR_DLL_SCRIPT_ERROR_DOWNLOADING_SCRIPT   220 
#define NEOVI_ERROR_DLL_SCRIPT_ERROR_CLEARING_SCRIPT   221 
                                                                       
#define NEOVI_ERROR_DLL_RED_RX_MSG_FULL_ISO   222 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_ISO   223 
#define NEOVI_ERROR_DLL_SCRIPT_INVALID_MSG_INDEX   224 
#define NEOVI_ERROR_DLL_SCRIPT_INVALID_APPSIG_INDEX   225 
#define NEOVI_ERROR_DLL_SCRIPT_NO_SCRIPT_RUNNING   226 
#define NEOVI_ERROR_DLL_COULD_NOT_SET_SETTINGS_FIRMWARE_MISMATCH   227 
#define NEOVI_ERROR_DLL_FIRE_CGI_TX_NOT_ENABLED   228 
#define NEOVI_ERROR_DLL_SEND_DEVICE_CONFIG_ERROR   229 
#define NEOVI_ERROR_DLL_GET_DEVICE_CONFIG_ERROR   230 
#define NEOVI_ERROR_DLL_UNKNOWN_NEOVI_TYPE   231     
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_ISO2   232 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_ISO3   233 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_ISO4   234 

#define NEOVI_ERROR_DLL_RED_RX_MSG_FULL_ISO2   235 
#define NEOVI_ERROR_DLL_RED_TX_MSG_FULL_ISO2   236 
#define NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_ISO2   237 
#define NEOVI_ERROR_DLL_RED_DRIVER_OVERFLOW_ISO2   238 
#define NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_ISO2   239 
#define NEOVI_ERROR_DLL_RED_RX_MSG_FULL_ISO3   240 
#define NEOVI_ERROR_DLL_RED_TX_MSG_FULL_ISO3   241 
#define NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_ISO3   242 
#define NEOVI_ERROR_DLL_RED_DRIVER_OVERFLOW_ISO3   243 
#define NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_ISO3   244 
#define NEOVI_ERROR_DLL_RED_RX_MSG_FULL_ISO4   245 
#define NEOVI_ERROR_DLL_RED_TX_MSG_FULL_ISO4   246 
#define NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_ISO4   247 
#define NEOVI_ERROR_DLL_RED_DRIVER_OVERFLOW_ISO4   248 
#define NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_ISO4   249 
#define NEOVI_ERROR_DLL_RED_FAILED_TO_CLEAR_LIN_SLAVE_DATA   250 
#define NEOVI_ERROR_DLL_NETWORK_NOT_ENABLED_ISO1   251 
#define NEOVI_ERROR_DLL_RED_RX_MSG_FULL_ISO1   252 
#define NEOVI_ERROR_DLL_RED_TX_MSG_FULL_ISO1   253 
#define NEOVI_ERROR_DLL_RED_TX_REPORT_MSG_FULL_ISO1   254 
#define NEOVI_ERROR_DLL_RED_SETTINGS_NOT_SET_ISO1   255 
#define NEOVI_ERROR_DLL_DROPPED_RTC_CMD   256 
#define NEOVI_ERROR_DLL_J1850_TX_REQUESTS_FLUSHED   257 
#define NEOVI_ERROR_J1708_COMM_BAD_PACKET   258 
#define NEOVI_ERROR_DLL_NETWORK_NOT_SUPPORTED_BY_HARDWARE   259 
#define NEOVI_ERROR_DLL_FEATURE_NOT_UNLOCKED   260     
#define NEOVI_ERROR_DLL_DEVICE_NOT_POWERED   261 
#define NEOVI_ERROR_DLL_3GCANDOWNLOADER_OK 				  262   ////
#define NEOVI_ERROR_DLL_3GCANDOWNLOADER_ERRORBADINIT		  263   // See ENeoVI3GCANDownloaderReturnCode_t in neoVI3GCANDownloader.h
#define NEOVI_ERROR_DLL_3GCANDOWNLOADER_ERRORNOCANPIPE		  264   //
#define NEOVI_ERROR_DLL_3GCANDOWNLOADER_ERRORISONEG7FRESPONSE	  265   //
#define NEOVI_ERROR_DLL_3GCANDOWNLOADER_ERRORTOOLNOTSELECTED	  266   //
#define NEOVI_ERROR_DLL_3GCANDOWNLOADER_ERRORINVALIDDEVICESELECTED	  267   //
#define NEOVI_ERROR_DLL_3GCANDOWNLOADER_ERRORCOULDNOTOPENTOOL	  268   //
#define NEOVI_ERROR_DLL_3GCANDOWNLOADER_ERRORNOFLOWCONTROL		  269   //
#define NEOVI_ERROR_DLL_3GCANDOWNLOADER_ERRORUNSPECIFIC		  270   //
#define NEOVI_ERROR_DLL_3GCANDOWNLOADER_ERRORCOREMININULLPTR		  271   //
#define NEOVI_ERROR_DLL_3GCANDOWNLOADER_ERRORCOREMINIZEROLEN		  272   //
#define NEOVI_ERROR_DLL_3GCANDOWNLOADER_ERRORRESERVED3		  273   //
#define NEOVI_ERROR_DLL_3GCANDOWNLOADER_ERRORRESERVED4		  274   //
#define NEOVI_ERROR_DLL_YELLOW_DEPRECATED		  275 
#define NEOVI_ERROR_DLL_HARDWARE_FAILURE_SRAM   276  //SRAM Problem ERROR report
#define NEOVI_ERROR_ACTIVE_CONNECTION_ALREADY_EXISTS		  277 
#define NEOVI_ERROR_DLL_MAIN51_RTC_FAILED_READ		  278 
#define NEOVI_ERROR_DLL_MAIN51_RTC_AUTO_RTC		  279 
#define NEOVI_ERROR_PLASMA_CORE_RESTARTED		  280 
#define NEOVI_ERROR_REQ_VBATT_FAILED              281 
#define NEOVI_ERROR_TXRX_PAIRS_TIMEOUT            282 
#define NEOVI_ERROR_TXRX_PAIRS_BUSY               283
#define NEOVI_ERROR_COREMINI_IN_USE               284
//For Bug 5589 - Duplicate error number of 78 is removed and added as 285.
#define  NEOVI_ERROR_DLL_PROBLEM_STOPPING_TXSOCKET_CLIENT_THREAD 285 //Bug 5589
#define  NEOVI_ERROR_ABORTED_FOR_SHUTDOWN         286 //Bug 7440 
#define  NEOVI_ERROR_DLL_SEND_DEVICE_CONFIG_NOTPOSSIBLE 287 //Bug 7701
#define  NEOVI_ERROR_CHANNEL_LOCKED_BY_OTHER_CLIENT 288
#define  NEOVI_ERROR_NEOVISERVER_GENERAL_ERROR    289
#define  NEOVI_ERROR_CHANNEL_LOCKING_NOT_SUPPORTED_FOR_DEVICE  290
#define NEOVI_ERROR_DLL_TOO_MANY_TXRXPAIRS 291
#define NEOVI_ERROR_DLL_TXRXPAIRS_NOT_SUPPORTED_FOR_DEVICE 292
#define NEOVI_ERROR_TXRX_PAIRS_FAILED               293
/**
 * So we've been using NEOVI_ERROR_DLL_DEVICE_NOT_POWERED to handle just
 * about all generic failures.  The problem is that now we have hardware
 * that actually can detect if Vehicle power is missing.  So we have
 * this new define to deliniate between the general failure case and
 * ACTUALLY measuring that vehicle power is missing.
 */
#define NEOVI_ERROR_DLL_DEVICE_USB_CONNECTED_BUT_NOT_POWER   294

// Constants for describing error severity
#define icsspyErrCritical    0x10
#define icsspyErrQuestion    0x20
#define icsspyErrExclamation 0x30
#define icsspyErrInformation 0x40


// Constants used to calculate the timestamp
#define NEOVI_TIMESTAMP_2   0.1048576 
#define NEOVI_TIMESTAMP_1   0.0000016 

#define NEOVIPRO_VCAN_TIMESTAMP_2   0.065536 
#define NEOVIPRO_VCAN_TIMESTAMP_1   0.000001 

#define NEOVI6_VCAN_TIMESTAMP_2   0.065536 
#define NEOVI6_VCAN_TIMESTAMP_1   0.000001 

#define NEOVI_RED_TIMESTAMP_2_25NS   107.3741824 
#define NEOVI_RED_TIMESTAMP_1_25NS   0.000000025 

typedef struct _SPerformanceStats
{
	unsigned long average_main_loop_us;
	unsigned long min_main_loop_us;
	unsigned long max_main_loop_us;
}SPerformanceStats;

#ifdef __C30
    #include "icsglb.h"
#else 
	// win32 types
        #define icscm_int16     short
	#define icscm_uint16    unsigned short
	#define icscm_uint32    unsigned int
	#define icscm_uint8    unsigned char
        #define icscm_float32    float
        
	#pragma pack(push)
	#pragma pack(2)

#endif


#ifdef USING_STUDIO_8
	#define VS_MODIFIER __declspec(align(2))
#else
	#define VS_MODIFIER
#endif
/* product ids */                             
#define NEOVI_RED_PROD_ID       0
#define NEOVI_FIRE_PROD_ID      1
#define NEOVI_VCAN3_PROD_ID     2
#define NEOVI_YELLOW_PROD_ID    3
#define NEOVI_ECU_PROD_ID       4
#define NEOVI_IEVB_PROD_ID      5
#define NEOVI_PENDANT_PROD_ID   6
#define NEOVI_NEOANALOG_PROD_ID 7
#define NEOVI_PLASMA_PROD_ID   10
#define NEOVI_ION_PROD_ID      11
/* chip ids */
#define NEOVI_FIRE_MCHIP_ID     0
#define NEOVI_FIRE_LCHIP_ID     1
#define NEOVI_FIRE_UCHIP_ID     2
#define NEOVI_FIRE_JCHIP_ID     3
                                  
#define NEOVI_VCAN3_MCHIP_ID 	4

#define NEOVI_YELLOW_MCHIP_ID 	5  
#define NEOVI_YELLOW_UCHIP_ID    NEOVI_FIRE_UCHIP_ID

#define NEOVI_ECU_MPIC_ID       6

#define NEOVI_IEVB_MPIC_ID       7

#define NEOVI_PENDANT_MPIC_ID    8
                                          
#define NEOVI_VNETFIRE_MCHIP_ID     	9
#define NEOVI_VNETFIRE_LCHIP_ID     	10
#define NEOVI_PLASMA_CORE_ID    	11
#define NEOVI_PLASMA_HID_ID    		12

#define NEOVI_ANALOG_MPIC_ID		13

#define NEOVI_PLASMA_ANALOG_CORE_ID		14
#define NEOVI_PLASMA_FLEXRAY_CORE_ID		15     
#define NEOVI_PLASMA_CORE_ID_1_12    	16

#define NEOVI_SLAVE_VNETFIRE_MCHIP_ID   17
#define NEOVI_SLAVE_VNETFIRE_LCHIP_ID   18

#define NEOVI_ION_CORE_ID	19
#define NEOVI_ION_HID_ID	20
#define NEOVI_ION_CORE_LOADER_ID    21
#define NEOVI_ION_HID_LOADER_ID     22
#define NEOVI_ION_FPGA_BIT_ID       23
     
#define NEOVI_VNETFIRE_EP_MCHIP_ID   	24
#define NEOVI_VNETFIRE_EP_LCHIP_ID   	25
#define NEOVI_AOUT_MCHIP   		26
#define NEOVI_MOST25_MCHIP   		27
#define NEOVI_MOST50_MCHIP   		28
#define NEOVI_MOST150_MCHIP   		29

// --- CAN Settings
typedef VS_MODIFIER struct {
	icscm_uint8 Mode;
	icscm_uint8 SetBaudrate;
	icscm_uint8 Baudrate;
	icscm_uint8 transceiver_mode;
	icscm_uint8 TqSeg1;
	icscm_uint8 TqSeg2;
	icscm_uint8 TqProp;
	icscm_uint8 TqSync;
	icscm_uint16 BRP;
	icscm_uint8 auto_baud;
	icscm_uint8 innerFrameDelay25us;
	//Actual use in firmware
	//Visual Studio does compile bitfields in structs correctly
	//unsigned innerFrameDelay25us:4; // 0 - 375us, 25us resolution
	//unsigned :4;
} CAN_SETTINGS;
                       
        enum { AUTO, USE_TQ };                                                                              //SetBaudrate
        enum { BPS20, BPS33, BPS50, BPS62, BPS83,  BPS100, BPS125, BPS250, BPS500, BPS800,  BPS1000, BPS666 /*Bug 7896*/ };        // CAN baudrates
        enum { NORMAL=0, DISABLE=1, LOOPBACK=2, LISTEN_ONLY=3, LISTEN_ALL=7 };        // CAN Modes
        enum { MSCAN2, LSFTCAN2, SWCAN2 }; 						// CAN2 Network Type

// --- SWCAN Settings
typedef VS_MODIFIER struct {
	icscm_uint8 Mode;
	icscm_uint8 SetBaudrate;
	icscm_uint8 Baudrate;
	icscm_uint8 transceiver_mode;
	icscm_uint8 TqSeg1;
	icscm_uint8 TqSeg2;
	icscm_uint8 TqProp;
	icscm_uint8 TqSync;
	icscm_uint16 BRP;
        icscm_uint16 high_speed_auto_switch; 
	icscm_uint8 auto_baud;
	icscm_uint8 RESERVED;
} SWCAN_SETTINGS;    

enum { SWCAN_AUTOSWITCH_DISABLED, SWCAN_AUTOSWITCH_NO_RESISTOR, SWCAN_AUTOSWITCH_WITH_RESISTOR, SWCAN_AUTOSWITCH_DISABLED_RESISTOR_ENABLED}; 

// --- LIN Settings
typedef VS_MODIFIER struct _LIN_SETTINGS{
	icscm_uint32 Baudrate;
	icscm_uint16 spbrg;
	icscm_uint8 brgh;
	icscm_uint8 numBitsDelay;
	icscm_uint8 MasterResistor;
	icscm_uint8 Mode;
} LIN_SETTINGS;

enum { BPS5000, BPS10400, BPS33333, BPS50000, BPS62500,  BPS71429, BPS83333, BPS100000, BPS117647 }; 	// baud rate
enum { RESISTOR_ON, RESISTOR_OFF };                             					//master resistor
enum { SLEEP_MODE, SLOW_MODE, NORMAL_MODE, FAST_MODE };    

typedef VS_MODIFIER struct {
	icscm_uint16 time_500us;
	icscm_uint16 k;
	icscm_uint16 l;
} ISO9141_KEYWORD2000__INIT_STEP;


// --- ISO (KEYWORD) Settings
typedef VS_MODIFIER struct {
	icscm_uint32 Baudrate;
	icscm_uint16 spbrg;
	icscm_uint16 brgh;
	
	ISO9141_KEYWORD2000__INIT_STEP init_steps[16];
	icscm_uint8 init_step_count;
	
	icscm_uint16 p2_500us;
	icscm_uint16 p3_500us;
	icscm_uint16 p4_500us;
	icscm_uint16 chksum_enabled;
} ISO9141_KEYWORD2000_SETTINGS;

// --- RS232 Settings
/*typedef VS_MODIFIER struct {
	icscm_uint16 Baudrate;
	icscm_uint16 spbrg;
	icscm_uint16 brgh;
} RS232_SETTINGS;*/

// --- UART Settings
typedef VS_MODIFIER struct _UART_SETTINGS{
	icscm_uint16 Baudrate;
	icscm_uint16 spbrg;
	icscm_uint16 brgh;
	icscm_uint16 parity;
	icscm_uint16 stop_bits;   
	icscm_uint8 flow_control; // 0- off, 1 - Simple CTS RTS,
	icscm_uint8 reserved_1;

	union
	{
		icscm_uint32 bOptions;
		struct
		{
			unsigned invert_tx : 1;
			unsigned invert_rx : 1;
			unsigned half_duplex : 1;
			unsigned reserved_bits : 13;
			unsigned reserved_bits2 : 16;
		};	
	};
} UART_SETTINGS;

// --- J1708 Settings
typedef VS_MODIFIER struct {
	icscm_uint16 enable_convert_mode;
} J1708_SETTINGS;

typedef VS_MODIFIER struct _SRedSettings
{
	//CAN stuff
	CAN_SETTINGS can1;
	CAN_SETTINGS can2;
	// lin stuff
	LIN_SETTINGS lin1;
	LIN_SETTINGS lin2;
}SRedSettings;

typedef VS_MODIFIER struct _STextAPISettings
{
	//CAN stuff
	icscm_uint32 can1_tx_id;
	icscm_uint32 can1_rx_id;
	union
	{
		struct
		{
			unsigned bExtended:1;
			unsigned :15;
		};
		icscm_uint32 DWord;
	}can1_options;
	
	icscm_uint32 can2_tx_id;
	icscm_uint32 can2_rx_id;
	union
	{
		struct
		{
			unsigned bExtended:1;
			unsigned :15;
		};
		icscm_uint32 DWord;
	}can2_options;

        icscm_uint32 network_enables;

	icscm_uint32 can3_tx_id;
	icscm_uint32 can3_rx_id;
	union
	{
		struct
		{
			unsigned bExtended:1;
			unsigned :15;
		};
		icscm_uint32 DWord;
	}can3_options;

	icscm_uint32 can4_tx_id;
	icscm_uint32 can4_rx_id;
	union
	{
		struct
		{
			unsigned bExtended:1;
			unsigned :15;
		};
		icscm_uint32 DWord;
	}can4_options;

	icscm_uint32 reserved[5];
	
}STextAPISettings;

typedef VS_MODIFIER union _stChipVersions
{
 struct
 {

  icscm_uint8 mpic_maj;
  icscm_uint8 mpic_min;
  icscm_uint8 upic_maj;
  icscm_uint8 upic_min;
  icscm_uint8 lpic_maj;
  icscm_uint8 lpic_min;
  icscm_uint8 jpic_maj;
  icscm_uint8 jpic_min;
 } fire_versions;
 
 struct
 {
  icscm_uint8 mpic_maj;
  icscm_uint8 mpic_min;
  icscm_uint8 core_maj;
  icscm_uint8 core_min;
  icscm_uint8 lpic_maj;
  icscm_uint8 lpic_min;
  icscm_uint8 hid_maj;
  icscm_uint8 hid_min;
 } plasma_fire_vnet;

 struct
 {
  icscm_uint8 mpic_maj;
  icscm_uint8 mpic_min;
 } vcan3_versions;

 struct
 {
  icscm_uint8 mpic_maj;
  icscm_uint8 mpic_min;
  icscm_uint8 upic_maj;
  icscm_uint8 upic_min;
 } yellow_versions;

 
} stChipVersions;

typedef VS_MODIFIER struct _SNeoMostGatewaySettings
{
	icscm_uint16 netId;		//!< Netid of CAN network to use.
	icscm_uint8 zero0;
	unsigned enabled:1;		//!< set to 1 to enable neoMOST network.
	unsigned miscIoIndex:3; //1< index of which miscio to use for timestamp sync. 0 => MISC1.
	unsigned echoOnCan:1;
	unsigned zero1:3;
}SNeoMostGatewaySettings;

enum
{
	REPORT_ON_PERIODIC,
	REPORT_ON_MISC1,
	REPORT_ON_MISC2,
	REPORT_ON_MISC3,
	REPORT_ON_MISC4,
	REPORT_ON_MISC5,
	REPORT_ON_MISC6,
	REPORT_ON_LED1,
	REPORT_ON_LED2,
	REPORT_ON_KLINE,
	REPORT_ON_MISC3_AIN,
	REPORT_ON_MISC4_AIN,
	REPORT_ON_MISC5_AIN,
	REPORT_ON_MISC6_AIN,
	REPORT_ON_PWM_IN1, 	/* send PWM 0x101 on change */
	REPORT_ON_GPS, 		/* send GPS 0x110-0x116 on change */
};


typedef VS_MODIFIER struct _SFireSettings
{
        CAN_SETTINGS can1;
        CAN_SETTINGS can2;
        CAN_SETTINGS can3;
        CAN_SETTINGS can4;
 
        SWCAN_SETTINGS swcan;
        CAN_SETTINGS lsftcan;
 
        LIN_SETTINGS lin1;
        LIN_SETTINGS lin2;
        LIN_SETTINGS lin3;
        LIN_SETTINGS lin4; 

        icscm_uint16 cgi_enable_reserved;
        icscm_uint16 cgi_baud;
        icscm_uint16 cgi_tx_ifs_bit_times;
        icscm_uint16 cgi_rx_ifs_bit_times;  
        icscm_uint16 cgi_chksum_enable;
 
        icscm_uint16 network_enables;
        icscm_uint16 network_enabled_on_boot;
 
        icscm_uint32 pwm_man_timeout;
        icscm_uint16 pwr_man_enable;
 
        icscm_uint16 misc_io_initial_ddr;  
        icscm_uint16 misc_io_initial_latch;
        icscm_uint16 misc_io_analog_enable;
        icscm_uint16 misc_io_report_period;
        icscm_uint16 misc_io_on_report_events;
        icscm_uint16 ain_sample_period;
        icscm_uint16 ain_threshold;
 
        //ISO 15765-2 Transport Layer
        icscm_int16 iso15765_separation_time_offset;
 
        //ISO9141 - KEYWORD 2000
        icscm_int16 iso9141_kwp_enable_reserved;
        ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings;
 
        //Performance Test
        icscm_uint16 perf_en;

        //ISO9141 - Parity
	icscm_uint16 iso_parity; // 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination; // 0 - use inner frame time, 1 - GME CIM-SCL
	icscm_uint16 iso_tester_pullup_enable;

	//Additional network enables
    icscm_uint16 network_enables_2;     
	
	ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings_2;
	icscm_uint16 iso_parity_2; 							// 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination_2; 				// 0 - use inner frame time, 1 - GME CIM-SCL
	
	ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings_3;
	icscm_uint16 iso_parity_3; 							// 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination_3; 				// 0 - use inner frame time, 1 - GME CIM-SCL
	
	ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings_4;
	icscm_uint16 iso_parity_4; 							// 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination_4; 				// 0 - use inner frame time, 1 - GME CIM-SCL    
	
	icscm_uint16 fast_init_network_enables_1;
	icscm_uint16 fast_init_network_enables_2;
	
	UART_SETTINGS uart;
	UART_SETTINGS uart2;
	
	STextAPISettings text_api;    
	
	SNeoMostGatewaySettings neoMostGateway;
	
	#define VNETBITS_FEATURE_ANDROID_MSGS (1)
	/**
	 * Unfortuntely I haven't gone thru the trouble
	 * of splitting the FIRE VNET and FIRE settings
	 * structures. So until I can do so and reserve 
	 * some time to test it, add a member that only
	 * VNET looks at for VNET features (like 
	 * Android CoreMiniMsg pump).
	 * Defaults to zero.
	 * @see VNETBITS_FEATURE_ANDROID_MSGS
	 */
	icscm_uint16 vnetBits;
}SFireSettings;

typedef VS_MODIFIER struct _SFireVnetSettings
{
	CAN_SETTINGS can1;
	CAN_SETTINGS can2;
	CAN_SETTINGS can3;
	CAN_SETTINGS can4;
	
	SWCAN_SETTINGS swcan;
	CAN_SETTINGS lsftcan;
	
	LIN_SETTINGS lin1;
	LIN_SETTINGS lin2;
	LIN_SETTINGS lin3;
	LIN_SETTINGS lin4; 
	
	icscm_uint16 cgi_enable_reserved;
	icscm_uint16 cgi_baud;
	icscm_uint16 cgi_tx_ifs_bit_times;
	icscm_uint16 cgi_rx_ifs_bit_times;
	icscm_uint16 cgi_chksum_enable;
	
	icscm_uint16 network_enables;
	icscm_uint16 network_enabled_on_boot;
	
	icscm_uint32 pwm_man_timeout;
	icscm_uint16 pwr_man_enable; // 0 - off, 1 - sleep enabled, 2- idle enabled (fast wakeup)
	
	icscm_uint16 misc_io_initial_ddr;  
	icscm_uint16 misc_io_initial_latch;
	icscm_uint16 misc_io_analog_enable;
	icscm_uint16 misc_io_report_period;
	icscm_uint16 misc_io_on_report_events;
	icscm_uint16 ain_sample_period;
	icscm_uint16 ain_threshold;
	
	//ISO 15765-2 Transport Layer
	icscm_int16 iso15765_separation_time_offset;
	
	//ISO9141 - KEYWORD 2000 1
	icscm_int16 iso9141_kwp_enable_reserved;
	ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings;
	
	//Performance Test
	icscm_uint16 perf_en;
	
	//ISO9141 - Parity
	icscm_uint16 iso_parity; // 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination; // 0 - use inner frame time, 1 - GME CIM-SCL
	icscm_uint16 iso_tester_pullup_enable;
	
	//Additional network enables
	icscm_uint16 network_enables_2;
	
	ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings_2;
	icscm_uint16 iso_parity_2; 							// 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination_2; 				// 0 - use inner frame time, 1 - GME CIM-SCL
	
	ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings_3;
	icscm_uint16 iso_parity_3; 							// 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination_3; 				// 0 - use inner frame time, 1 - GME CIM-SCL
	
	ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings_4;
	icscm_uint16 iso_parity_4; 							// 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination_4; 				// 0 - use inner frame time, 1 - GME CIM-SCL
	
	icscm_uint16 fast_init_network_enables_1;
	icscm_uint16 fast_init_network_enables_2;
	
	UART_SETTINGS uart;
	UART_SETTINGS uart2;
	
	STextAPISettings text_api;
	
	SNeoMostGatewaySettings neoMostGateway;
	
	#define VNETBITS_FEATURE_ANDROID_MSGS (1)
	/**
	 * VNET options bitfield.
	 * Defaults to zero.
	 * @see VNETBITS_FEATURE_ANDROID_MSGS
	 */
	icscm_uint16 vnetBits;
}SFireVnetSettings;


typedef VS_MODIFIER struct _SVCAN3Settings
{
        CAN_SETTINGS can1;
        CAN_SETTINGS can2;

        icscm_uint16 network_enables; 
        icscm_uint16 network_enabled_on_boot;

        //ISO 15765-2 Transport Layer
        icscm_int16 iso15765_separation_time_offset;

        //Performance Test
        icscm_uint16 perf_en;

        //MISC IO
	icscm_uint16 misc_io_initial_ddr;
	icscm_uint16 misc_io_initial_latch;
	icscm_uint16 misc_io_report_period;
	icscm_uint16 misc_io_on_report_events;
}SVCAN3Settings;

typedef VS_MODIFIER struct _SYellowSettings
{
	CAN_SETTINGS can1;
	CAN_SETTINGS can2;

	//RS232_SETTINGS rs232;
	icscm_uint16 rs232_deprecated[3];
		
	icscm_uint16 hs2_j1708_enable;		// Use hs2 + J1708
	icscm_uint16 iso_j1850_enable;		// Select ISO + J1850
	icscm_uint16 j1708_j1850_enable;	// Use J1708 + J1850
	
	icscm_uint16 network_enables;
	icscm_uint16 network_enables_2;  
	icscm_uint16 network_enabled_on_boot;
	
	icscm_uint32 pwm_man_timeout;
	icscm_uint16 pwr_man_enable; // 0 - off, 1 - sleep enabled, 2- idle enabled (fast wakeup)
	
	icscm_uint16 misc_io_initial_ddr;  
	icscm_uint16 misc_io_initial_latch;
	icscm_uint16 misc_io_analog_enable;
	icscm_uint16 misc_io_report_period;
	icscm_uint16 misc_io_on_report_events;
	icscm_uint16 ain_sample_period;
	icscm_uint16 ain_threshold;

	//ISO 15765 Transport Layer
	icscm_int16 iso15765_separation_time_offset;
	
	//ISO9141 - KEYWORD 2000 1
	icscm_int16 iso9141_kwp_enable_reserved;
	ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings;
	LIN_SETTINGS lin1;
	
	//ISO9141 - KEYWORD 2000 2 // remove later
	icscm_int16 iso9141_kwp_enable_reserved_2; // remove later
	ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings_2; // remove later
	LIN_SETTINGS lin2;

	//J1708
	J1708_SETTINGS j1708;

	//Performance Test
	icscm_uint16 perf_en;
	
	//ISO9141 - Parity
	icscm_uint16 iso_parity; // 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination; // 0 - use inner frame time, 1 - GME CIM-SCL
	icscm_uint16 iso_tester_pullup_enable;
	
	//remove later
	icscm_uint16 iso_parity_2; 							// 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination_2; 				// 0 - use inner frame time, 1 - GME CIM-SCL
	
	
	icscm_uint16 fast_init_network_enables_1;
	icscm_uint16 fast_init_network_enables_2;
	
	UART_SETTINGS uart;
	UART_SETTINGS uart2;
	
	STextAPISettings text_api;

}SYellowSettings;

typedef VS_MODIFIER struct _SECUSettings
{
	// ECU ID used in CAN communications.  
	// TX ID = ECU ID with bit28 cleared, 
	// RX ID = ECUID with bit28 set, 
	// ECU ID = 0 implies ECU ID = serial no with bit 27 set
	icscm_uint32 ecu_id;
	
	icscm_uint16 selected_network; // not supported yet - default to HSCAN
	
	CAN_SETTINGS can1;
	CAN_SETTINGS can2;

	LIN_SETTINGS lin1;
	LIN_SETTINGS lin2;
	
	//ISO 15765-2 Transport Layer
	icscm_int16 iso15765_separation_time_offset;

	//ISO9141 - KEYWORD 2000 1
	ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings;
	icscm_uint16 iso_parity; // 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination; // 0 - use inner frame time, 1 - GME CIM-SCL
	
	ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings_2;
	icscm_uint16 iso_parity_2; 							// 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination_2; 				// 0 - use inner frame time, 1 - GME CIM-SCL
	
	icscm_uint16 network_enables;
	icscm_uint16 network_enables_2;
	icscm_uint16 network_enabled_on_boot;
		
	icscm_uint32 pwr_man_timeout;
	icscm_uint16 pwr_man_enable; // 0 - off, 1 - sleep enabled, 2- idle enabled (fast wakeup)
	
	icscm_uint16 misc_io_initial_ddr;  
	icscm_uint16 misc_io_initial_latch;
	icscm_uint16 misc_io_analog_enable;
	icscm_uint16 misc_io_report_period;
	icscm_uint16 misc_io_on_report_events;
	icscm_uint16 ain_sample_period;
	icscm_uint16 ain_threshold;
	
	SWCAN_SETTINGS swcan;
	SWCAN_SETTINGS swcan2;
	CAN_SETTINGS lsftcan;
	CAN_SETTINGS lsftcan2;
	
	UART_SETTINGS uart;
	UART_SETTINGS uart2;
	
	STextAPISettings text_api;
}SECUSettings;

typedef VS_MODIFIER struct _SPendantSettings
{
	// ECU ID used in CAN communications.  
	// TX ID = ECU ID with bit28 cleared, 
	// RX ID = ECUID with bit28 set, 
	// ECU ID = 0 implies ECU ID = serial no with bit 27 set
	icscm_uint32 ecu_id; 
	
	icscm_uint16 selected_network; // not supported yet - default to HSCAN
	
	CAN_SETTINGS can1;
	CAN_SETTINGS can2;

	LIN_SETTINGS lin1;
	LIN_SETTINGS lin2;

	//ISO 15765-2 Transport Layer
	icscm_int16 iso15765_separation_time_offset;

	//ISO9141 - KEYWORD 2000 1
	ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings;
	icscm_uint16 iso_parity; // 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination; // 0 - use inner frame time, 1 - GME CIM-SCL
	
	ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings_2;
	icscm_uint16 iso_parity_2; 							// 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination_2; 				// 0 - use inner frame time, 1 - GME CIM-SCL
	
	icscm_uint16 network_enables;
	icscm_uint16 network_enables_2;
	icscm_uint16 network_enabled_on_boot;
		
	icscm_uint32 pwr_man_timeout;
	icscm_uint16 pwr_man_enable; // 0 - off, 1 - sleep enabled, 2- idle enabled (fast wakeup)
	
	icscm_uint16 misc_io_initial_ddr;  
	icscm_uint16 misc_io_initial_latch;
	icscm_uint16 misc_io_analog_enable;
	icscm_uint16 misc_io_report_period;
	icscm_uint16 misc_io_on_report_events;
	icscm_uint16 ain_sample_period;
	icscm_uint16 ain_threshold;
	
	SWCAN_SETTINGS swcan;
	SWCAN_SETTINGS swcan2;
	CAN_SETTINGS lsftcan;
	CAN_SETTINGS lsftcan2;
	
	UART_SETTINGS uart;
	UART_SETTINGS uart2;

	STextAPISettings text_api;
	
}SPendantSettings;

typedef VS_MODIFIER struct _SIEVBSettings
{
	// ECU ID used in CAN communications.
	// TX ID = ECU ID with bit28 cleared,
	// RX ID = ECUID with bit28 set,
	// ECU ID = 0 implies ECU ID = serial no with bit 27 set
	icscm_uint32 ecu_id; 

	icscm_uint16 selected_network; // not supported yet - default to HSCAN
	
	CAN_SETTINGS can1;
	LIN_SETTINGS lin1;
	
	icscm_uint16 network_enables;
	icscm_uint16 network_enables_2;
	icscm_uint16 network_enabled_on_boot;
	
	icscm_uint16 misc_io_initial_ddr;  
	icscm_uint16 misc_io_initial_latch;
	icscm_uint16 misc_io_analog_enable;
	icscm_uint16 misc_io_report_period;
	icscm_uint16 misc_io_on_report_events;
	icscm_uint16 ain_sample_period;
	icscm_uint16 ain_threshold;
	
	//ISO 15765-2 Transport Layer
	icscm_int16 iso15765_separation_time_offset;   
	
	CAN_SETTINGS can2;
	LIN_SETTINGS lin2;
			
	UART_SETTINGS uart;
	UART_SETTINGS uart2;
	
	STextAPISettings text_api;
	
	icscm_uint32 reserved_1;
	icscm_uint32 reserved_2;
	
	icscm_uint32 pwr_man_timeout;
	icscm_uint16 pwr_man_enable; // 0 - off, 1 - sleep enabled, 2- idle enabled (fast wakeup)
	
	icscm_uint16 idle_wakeup_network_enables_1;
	icscm_uint16 idle_wakeup_network_enables_2;
			
	icscm_uint16 misc_io_analog_enable_2;
	
	//ISO9141 - KEYWORD 2000 1
	ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings;
	//ISO9141 - Parity
	icscm_uint16 iso_parity; // 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination; // 0 - use inner frame time, 1 - GME CIM-SCL
	icscm_uint16 iso_tester_pullup_enable;
	
	//ISO9141 - KEYWORD 2000 2
	ISO9141_KEYWORD2000_SETTINGS iso9141_kwp_settings_2;
	icscm_uint16 iso_parity_2; 							// 0 - no parity, 1 - event, 2 - odd
	icscm_uint16 iso_msg_termination_2; 				// 0 - use inner frame time, 1 - GME CIM-SCL

}SIEVBSettings;

#define GS_VERSION 5
typedef VS_MODIFIER struct _GLOBAL_SETTINGS
{
	icscm_uint16 version;

	icscm_uint16 len;

	icscm_uint16 chksum;

	union
	{
		SRedSettings red;
	
		SFireSettings fire;  
		SFireVnetSettings firevnet;
	
		SVCAN3Settings vcan3;
		
		SYellowSettings yellow;

                SECUSettings ecu;
                	
		SIEVBSettings ievb;

                SPendantSettings pendant;
	};                   
} GLOBAL_SETTINGS;  

#define NEOVI_3G_MAX_SETTINGS_SIZE  sizeof(GLOBAL_SETTINGS)

enum
{
	LL_DEAD,
	LL_PRODUCTION,
	LL_FIRE_DEMO,        
	LL_FIRE,
	LL_RED_LIMITED,
	LL_RED,
	LL_YELLOW_LIMITED,
	LL_YELLOW,
	LL_VCAN3_LIMITED,
	LL_VCAN3,
	LL_VCAN3_PRODUCTION, // limited number of transmits and then hardware trap
	LL_FIRE_PRODUCTION, // limited number of transmits and then hardware trap, cgi enabled until power off or hardware trap
	LL_ECU_40,
	LL_IEVB,
	LL_ECU_20,	// limit USB communication
	LL_NEO_HDA,
    LL_NEOOBD_LOGGER,
};

typedef struct _SReflashRequest
{
	char fire_mpic_ief[1024];
	unsigned long fire_mpic_enable;

	char fire_lpic_ief[1024];
	unsigned long fire_lpic_enable;

	char fire_yellow_upic_ief[1024];
	unsigned long fire_yellow_upic_enable;

	char fire_yellow_jpic_ief[1024];
	unsigned long fire_yellow_jpic_enable;

	char yellow_mpic_ief[1024];
	unsigned long yellow_mpic_enable;

	char vcan3_mpic_ief[1024];
	unsigned long vcan3_mpic_enable;

}SReflashRequest;

#define icscm_int16     short
#define icscm_uint16    unsigned short
#define icscm_uint32    unsigned int
#define icscm_uint8    unsigned char
#define icscm_float32    float

#define SCRIPT_LOCATION_FLASH_MEM  0
#define SCRIPT_LOCATION_SDCARD     1
#define SCRIPT_LOCATION_VCAN3_MEM  4

#define ICS_NEODEVICE_OLD_VERSION(x)  (x == NEODEVICE_DW_VCAN || x == NEODEVICE_BLUE)
#define ICS_NEODEVICE_NEW_VERSION(x)  (x == NEODEVICE_FIRE || x == NEODEVICE_VCAN3 || x == NEODEVICE_CT_OBD || x == NEODEVICE_YELLOW)

typedef struct _stCM_ISO157652_TxMessage
{
	//transmit message
	icscm_uint16 vs_netid;	///< The netid of the message (determines which network to transmit on),  not supported

	icscm_uint8 padding;	///< The padding byte to use to fill the unused portion of
							///  transmitted CAN frames (single frame, first frame, consecutive frame).
	
	icscm_uint8		reserved2;

	icscm_uint32    id;	///< arbId of transmitted frames (CAN id to transmit to).
	icscm_uint32    fc_id;		///< flow control arb id filter value (response id from receiver).
    icscm_uint32    fc_id_mask;	///< The flow control arb filter mask (response id from receiver).

	icscm_uint8		stMin;  ///< Overrides the stMin that the receiver reports, see overrideSTmin. Set to J2534's STMIN_TX if <= 0xFF.
	icscm_uint8		blockSize;  ///< Overrides the block size that the receiver reports, see overrideBlockSize.   Set to J2534's BS_TX if <= 0xFF.
	icscm_uint8		flowControlExtendedAddress;	///< Expected Extended Address byte of response from receiver.  see fc_ext_address_enable, not supported.
	icscm_uint8		extendedAddress;			///< Extended Address byte of transmitter. see ext_address_enable, not supported.

	//flow control timeouts
	icscm_uint16    fs_timeout;				///< max timeout (ms) for waiting on flow control respons. Set this to N_BS_MAX's value if J2534.
	icscm_uint16    fs_wait;				///< max timeout (ms) for waiting on flow control response after receiving flow control
											///with flow status set to WAIT.   Set this to N_BS_MAX's value if J2534.

	icscm_uint8 data[4*1024];	///< The data

	icscm_uint32 num_bytes;	///< Number of data bytes

	//option bits
	union
	{
		struct
		{
			unsigned id_29_bit_enable:1;	///< Enables 29 bit arbId for transmitted frames.  Set to 1 so transmitted frames use 29 bit ids, not supported.
			unsigned fc_id_29_bit_enable:1;	///< Enables 29 bit arbId for Flow Control filter.  Set to 1 if receiver response uses 29 bit ids, not supported.
			unsigned ext_address_enable:1;	///< Enables Extended Addressing, Set to 1 if transmitted frames should have extended addres byte, not supported.
			unsigned fc_ext_address_enable:1;	///< Enables Extended Addressing for Flow Control filter.  Set to 1 if receiver responds with extended address byte, not supported.
			
			unsigned overrideSTmin:1;   ///< Uses member stMin and not receiver's flow control's stMin.
			unsigned overrideBlockSize:1;   ///< Uses member BlockSize and not receiver's flow control's BlockSize.
			unsigned paddingEnable:1; ///< Enable's padding
		};
		icscm_uint16 flags;
	};
        
}stCM_ISO157652_TxMessage;

typedef struct _stCM_ISO157652_RxMessage
{
	//transmit message
	icscm_uint16 vs_netid;	///< The netid of the message (determines which network to decode receives),  not supported

	icscm_uint8 padding;	///< The padding byte to use to fill the unused portion of
							///  transmitted CAN frames (flow control), see paddingEnable.

	icscm_uint32    id;			///< ArbId filter value for frames from transmitter (from ECU to neoVI).
	icscm_uint32    id_mask;	///< ArbId filter mask for frames from transmitter (from ECU to neoVI).
	icscm_uint32    fc_id;		///< flow control arbId to transmit in flow control (from neoVI to ECU).

	icscm_uint8		flowControlExtendedAddress;	///< Extended Address byte used in flow control (from neoVI to ECU). see fc_ext_address_enable.
	icscm_uint8		extendedAddress;	///< Expected Extended Address byte of frames sent by transmitter (from ECU to neoVI).  see ext_address_enable.

	icscm_uint8		blockSize;		///< Block Size to report in flow control response.
	icscm_uint8		stMin;		///< Minimum seperation time (between consecutive frames) to report in flow control response.

	//flow control timeouts
	icscm_uint16    cf_timeout;				///< max timeout (ms) for waiting on consecutive frame.  Set this to N_CR_MAX's value in J2534.

	//option bits
	union
	{
		struct
		{
			unsigned id_29_bit_enable:1;	///< Enables 29 bit arbId filter for frames (from ECU to neoVI).
			unsigned fc_id_29_bit_enable:1;	///< Enables 29 bit arbId for Flow Control (from neoVI to ECU).
			unsigned ext_address_enable:1;	///< Enables Extended Addressing (from ECU to neoVI).
			unsigned fc_ext_address_enable:1;	///< Enables Extended Addressing (from neoVI to ECU).
			unsigned enableFlowControlTransmission:1;	///< Enables Flow Control frame transmission (from neoVI to ECU).
			unsigned paddingEnable:1; ///< Enable's padding
		};
		icscm_uint32 flags;
	};

	icscm_uint8 reserved[16];
        
}stCM_ISO157652_RxMessage;

#ifdef __C30
#else
	#pragma pack(pop)
#endif








#endif