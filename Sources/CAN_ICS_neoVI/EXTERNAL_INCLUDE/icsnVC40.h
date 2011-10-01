////////////////////////////////////////////////////////////////////////////////////
// neovi.h
// 4/23/2001
// Header file for using the neoVI API in C/C++
// Copyright Intrepid Control Systems, Inc. 2000-2006
// www.intrepidcs.com
////////////////////////////////////////////////////////////////////////////////////

// include the standard message data structures
#include        "icsSpyData.h"

// comment the following line out for Visual C++
//#define		BORLANDC


//
// If borland c is defined we need to have "extern "c" added
// before each api call and a closing bracket after each api call
#ifdef BORLANDC
	#define BORLANDC_HEADER		extern "C" {
	#define BORLANDC_FOOTER		}
#else
	#define BORLANDC_HEADER	
	#define BORLANDC_FOOTER	
#endif


// OpenPort "OpenType" Argument Constants
#define		NEOVI_COMMTYPE_RS232				0
#define		NEOVI_COMMTYPE_USB_BULK				1
#define		NEOVI_COMMTYPE_USB_ISO_DONT_USE		2
#define		NEOVI_COMMTYPE_TCPIP				3

// Driver Type Constants
#define INTREPIDCS_DRIVER_STANDARD		0
#define	INTREPIDCS_DRIVER_TEST			1

// device Type IDs
#define INTREPIDCS_DEVICE_NEO4			0
#define INTREPIDCS_DEVICE_VCAN			1
#define INTREPIDCS_DEVICE_NEO6			2
#define INTREPIDCS_DEVICE_UNKNOWN		3
#define INTREPIDCS_DEVICE_VCAN2  		4

// neoVI Subsystem ID's
#define		NETID_DEVICE		0
#define		NETID_HSCAN			1
#define		NETID_MSCAN			2
#define		NETID_SWCAN			3
#define		NETID_LSFTCAN		4
#define		NETID_FORDSCP		5
#define		NETID_J1708			6
#define		NETID_AUX			7
#define		NETID_JVPW			8
#define		NETID_ISO			9
#define		NETID_ISOPIC		10
#define		NETID_MAIN51		11
#define		NETID_HOST			12

// ISO15765 Status Flags
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


// Configuration Array addresses

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
const unsigned long NEOVI_ERROR_DLL_TX_BUFFER_OVERFLOW = 0;
const unsigned long NEOVI_ERROR_DLL_ERROR_BUFFER_OVERFLOW = 1;
const unsigned long NEOVI_ERROR_DLL_USB_SEND_DATA_ERROR = 2;
const unsigned long NEOVI_ERROR_DLL_ISO_DATA_BUFFER_ALLOC = 3;
const unsigned long NEOVI_ERROR_DLL_ISO_DATA_READ_BUFFER = 4;
const unsigned long NEOVI_ERROR_DLL_ISO_DATA_ZERO_PACKETS =5;
const unsigned long NEOVI_ERROR_DLL_RX_MSG_BUFFER_OVERFLOW =6;
const unsigned long NEOVI_ERROR_DLL_STOP_ISO_STREAM =7;
const unsigned long NEOVI_ERROR_DLL_INVALID_NETID =8;
const unsigned long NEOVI_ERROR_DLL_PROBLEM_STOPPING_RX_THREAD =9;
const unsigned long NEOVI_ERROR_DLL_PROBLEM_STOPPING_TX_THREAD =10;
const unsigned long NEOVI_ERROR_DLL_MAIN_PIC_BUFFER_OVERFLOW =11;
const unsigned long NEOVI_ERROR_DLL_INVALID_DEVICE_RESPONSE =12;
const unsigned long NEOVI_ERROR_DLL_ISOTX_DATA_BUFFER_ALLOC =13;
const unsigned long NEOVI_ERROR_DLL_RX_CMD_BUFFER_OVERFLOW=14;
const unsigned long NEOVI_ERROR_DLL_RS232_RX_BUFFER_OVERFLOW=15;
const unsigned long NEOVI_ERROR_DLL_RS232_ERR_READCOMERR =16;
const unsigned long NEOVI_ERROR_DLL_RS232_ERR_READ=17;
const unsigned long NEOVI_ERROR_DLL_RS232_BUFFER_ALLOC=18;
const unsigned long NEOVI_ERROR_DLL_RS232_TX_BUFFER_OVERFLOW=19;
const unsigned long NEOVI_ERROR_DLL_RS232_MISC_ERROR=20;
const unsigned long NEOVI_ERROR_DLL_RS232_FIND_WRITE=21;
const unsigned long NEOVI_ERROR_DLL_RS232_FIND_BUFFER_ALLOC=22;
const unsigned long NEOVI_ERROR_DLL_RS232_FIND_CLEARCOMM=23;
const unsigned long NEOVI_ERROR_DLL_RS232_FIND_READCOMM=24;
const unsigned long NEOVI_ERROR_DLL_RS232_FIND_TIMEOUT=25;
const unsigned long NEOVI_ERROR_DLL_RS232_ERR_BREAK=26;
const unsigned long NEOVI_ERROR_DLL_RS232_ERR_FRAME=27;
const unsigned long NEOVI_ERROR_DLL_RS232_ERR_IOE=28;
const unsigned long NEOVI_ERROR_DLL_RS232_ERR_OVERRUN=29;
const unsigned long NEOVI_ERROR_DLL_RS232_ERR_PARITY=30;
const unsigned long NEOVI_ERROR_DLL_RS232_TXBUFFER_ALLOC=31; 
const unsigned long NEOVI_ERROR_DLL_USB_TX_RS232_ERROR=32;
const unsigned long NEOVI_ERROR_DLL_RS232_CREATE_FILE=33;
const unsigned long NEOVI_ERROR_DLL_RS232_GET_COMM_STATE=34;
const unsigned long NEOVI_ERROR_DLL_RS232_SET_COMM_STATE=35;
const unsigned long NEOVI_ERROR_DLL_RS232_START_COMM_RX_THREAD=36;
const unsigned long NEOVI_ERROR_DLL_RS232_START_COMM_TX_THREAD=37;
const unsigned long NEOVI_ERROR_DLL_SYNC_COUNT_ERR=38;
const unsigned long NEOVI_ERROR_DLL_RX_MSG_FRAME_ERR=39;
const unsigned long NEOVI_ERROR_DLL_RX_MSG_FIFO_OVER=40;
const unsigned long NEOVI_ERROR_DLL_RX_MSG_CHK_SUM_ERR=41;
const unsigned long NEOVI_ERROR_DLL_PROBLEM_STOPPING_BULKIN_THREAD=42;
const unsigned long NEOVI_ERROR_DLL_BULKIN_ERR_READ=43;
const unsigned long NEOVI_ERROR_DLL_MAIN51_RX_FIFO_OVERFLOW=44;
const unsigned long NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW=45;
const unsigned long NEOVI_ERROR_DLL_MAIN51_DEV_FIFO_OVERFLOW=46;
const unsigned long NEOVI_ERROR_DLL_RESET_STATUS_CHANGED=47;
const unsigned long NEOVI_ERROR_DLL_ISO_LONG_CACHE_OVERFLOW=48;
const unsigned long NEOVI_ERROR_DLL_ISORX_LONG_BUFFER_ALLOC=49;
const unsigned long NEOVI_ERROR_DLL_J1708_LONG_CACHE_OVERFLOW=50;
const unsigned long NEOVI_ERROR_DLL_J1708_LONG_BUFFER_ALLOC=51;
const unsigned long NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_DEVICE =52;
const unsigned long NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_HSCAN =53;
const unsigned long NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_MSCAN =54;
const unsigned long NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_SWCAN =55;
const unsigned long NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_LSFTCAN =56;
const unsigned long NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_FORDSCP =57;
const unsigned long NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_J1708 =58;
const unsigned long NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_AUX =59;
const unsigned long NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_JVPW =60;
const unsigned long NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_ISO =61;
const unsigned long NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_ISOPIC =62;
const unsigned long NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_MAIN51 =63;
const unsigned long NEOVI_ERROR_DLL_MAIN51_TX_FIFO_OVERFLOW_HOST =64;
const unsigned long NEOVI_ERROR_DLL_READ_ENTIRE_DEEPROM_ERROR =65;
const unsigned long  NEOVI_ERROR_DLL_WRITE_ENTIRE_DEEPROM_ERROR=66;
const unsigned long  NEOVI_ERROR_DLL_USB_PORT_ALREADY_OPEN=67;
const unsigned long  NEOVI_ERROR_DLL_JVPW_TX_REPORT_FIFO_ERR_IN=68;
const unsigned long  NEOVI_ERROR_DLL_ISOJ_TX_REPORT_FIFO_ERR_IN=69;
const unsigned long  NEOVI_ERROR_DLL_JVPW_TX_REPORT_FIFO_ERR_OUT=70;
const unsigned long  NEOVI_ERROR_DLL_ISOJ_TX_REPORT_FIFO_ERR_OUT=71;
const unsigned long  NEOVI_ERROR_DLL_MAIN51_TX_IN_FROM_HOST_FIFO=72;
const unsigned long  NEOVI_ERROR_DLL_MAIN51_TX_HOST_CHKSUM=73;
const unsigned long NEOVI_ERROR_DLL_ISOJ_TX_HOST_MISSED_BYTE=74;
const unsigned long NEOVI_ERROR_DLL_NEOVI_NO_RESPONSE=75;
const unsigned long NEOVI_ERROR_DLL_RX_SOCKET_FIFO_OVER=76;
const unsigned long NEOVI_ERROR_DLL_PROBLEM_STOPPING_TXSOCKET_THREAD=77;
const unsigned long NEOVI_ERROR_DLL_PROBLEM_STOPPING_RXSOCKET_THREAD=78;
const unsigned long NEOVI_ERROR_DLL_PROBLEM_STOPPING_TXSOCKET_CLIENT_THREAD=78;
const unsigned long NEOVI_ERROR_DLL_PROBLEM_STOPPING_RXSOCKET_CLIENT_THREAD=79;
const unsigned long NEOVI_ERROR_DLL_TCP_CLIENT_TX=80;
const unsigned long NEOVI_ERROR_DLL_TCP_CLIENT_RX=81;
const unsigned long NEOVI_ERROR_DLL_TCP_CLIENT_RX_SOCK=82;
const unsigned long NEOVI_ERROR_DLL_UNABLE_CONNECT_TO_SRVR=83;
const unsigned long NEOVI_ERROR_DLL_UNABLE_CREATE_CLIENT_SOCK=84;
const unsigned long NEOVI_ERROR_DLL_UNABLE_WSASTARTUP=85;
const unsigned long NEOVI_ERROR_DLL_SOCK_CL_RD_BUFFER_ALLOC=86;
const unsigned long NEOVI_ERROR_DLL_SOCK_CL_TX_BUFFER_ALLOC=87;
const unsigned long NEOVI_ERROR_DLL_SOCK_SRVR_RX_BUFFER_ALLOC=88;
const unsigned long NEOVI_ERROR_DLL_SOCK_SRVR_TX_BUFFER_ALLOC=89;
const unsigned long NEOVI_ERROR_DLL_ILLEGAL_TX_NETWORK=90;
const unsigned long NEOVI_ERROR_DLL_MAIN51_TX_HOST_OVERRUN=91;
const unsigned long NEOVI_ERROR_DLL_OPEN_GET_COMM_TIMEOUT=92;
const unsigned long NEOVI_ERROR_DLL_OPEN_SET_COMM_TIMEOUT=93;
const unsigned long NEOVI_ERROR_DLL_OPEN_READ_DEVICE_TYPE=94;
const unsigned long NEOVI_ERROR_DLL_OPEN_READ_DEVICE_TYPE_TOUT=95;
const unsigned long NEOVI_ERROR_DLL_CLOSE_PURGE_COMM=96;
const unsigned long NEOVI_ERROR_DLL_TX_COM_FIFO_OVERFLOW=97;
const unsigned long NEOVI_ERROR_DLL_GET_USBSERIAL_DEVICES=98;

// Constants for describing error severity
const unsigned long icsspyErrCritical=0x10;
const unsigned long icsspyErrExclamation=0x30;
const unsigned long icsspyErrInformation=0x40;
const unsigned long icsspyErrQuestion=0x20;

// Constants used to calculate the timestamp
const double NEOVI_TIMESTAMP_2 = 0.1048576;
const double NEOVI_TIMESTAMP_1 = 0.0000016;

// Constants used to calculate the timestamp
const double NEOVI6_VCAN_TIMESTAMP_2 = 0.065536;
const double NEOVI6_VCAN_TIMESTAMP_1 = 0.000001;

// Win32 DLL prototypes 
BORLANDC_HEADER int _stdcall  icsneoOpenPort(int lPortNumber, int lPortType,
                        int lDriverType,unsigned char * bNetworkID,
                        unsigned char * bSCPIDs,  int * hObject); BORLANDC_FOOTER

BORLANDC_HEADER int _stdcall icsneoOpenPortEx( int lPortNumber, int lPortType, 
						int lDriverType, int lIPAddressMSB, int lIPAddressLSBOrBaudRate, 
						int bConfigRead, unsigned char * bNetworkID, int * hObject); BORLANDC_FOOTER


BORLANDC_HEADER int _stdcall icsneoClosePort(int hObject, int * pNumberOfErrors); BORLANDC_FOOTER


BORLANDC_HEADER void  _stdcall  icsneoFreeObject(int hObject); BORLANDC_FOOTER


BORLANDC_HEADER int _stdcall icsneoGetMessages(int hObject, icsSpyMessage * pMsg,
                        int * pNumberOfMessages,
                        int * pNumberOfErrors); BORLANDC_FOOTER


BORLANDC_HEADER int _stdcall icsneoTxMessages(int hObject,icsSpyMessage * pMsg,
                        int lNetworkID, int lNumMessages); BORLANDC_FOOTER


BORLANDC_HEADER int _stdcall icsneoGetErrorMessages(int hObject, int * pErrorMsgs,
                        int * pNumberOfErrors); BORLANDC_FOOTER


BORLANDC_HEADER int _stdcall icsneoGetErrorInfo(int lErrorNumber,
						TCHAR * szErrorDescriptionShort,TCHAR * szErrorDescriptionLong,
						int *lMaxLengthShort,int *lMaxLengthLong,
						int * lErrorSeverity,
						int * lRestartNeeded); BORLANDC_FOOTER



BORLANDC_HEADER int _stdcall icsneoSendConfiguration(int hObject, 
						unsigned char * pData, 
						int lNumBytes); BORLANDC_FOOTER


BORLANDC_HEADER int _stdcall icsneoFindAllUSBDevices(int lDriverType,
						int iGetSerialNumbers,
						int *iDevices, 
						int *iSerialNumbers,
						int *iOpenedStates,
						int * iNumDevices); BORLANDC_FOOTER


BORLANDC_HEADER int _stdcall icsneoFindAllCOMDevices(int lDriverType,
						int lGetSerialNumbers,
						int lStopAtFirst,
						int lUSBCommOnly,
						int *p_lDeviceTypes,
						int *p_lComPorts,
						int *p_lSerialNumbers,
						int *lNumDevices); BORLANDC_FOOTER


BORLANDC_HEADER int _stdcall icsneoStartSockServer(int hObject, int iPort); BORLANDC_FOOTER


BORLANDC_HEADER int _stdcall icsneoStopSockServer(int hObject); BORLANDC_FOOTER


BORLANDC_HEADER int _stdcall icsneoGetDLLVersion(); BORLANDC_FOOTER


BORLANDC_HEADER  void _stdcall icsneoSetISO15765RxParameters(int hObject, 
                        int lNetwork,
					    int lEnable, 
					    spyFilterLong * pFF_CFMsgFilter, 
					    icsSpyMessage * pFlowCTxMsg, 
					    int lCFTimeOutMs,
					    int lFlowCBlockSize,
					    int lUsesExtendedAddressing, 
                        int lUseHardwareIfPresent); BORLANDC_FOOTER


BORLANDC_HEADER   void _stdcall icsneoGetISO15765Status(int hObject,
                        int lNetwork, int lClearTxStatus, int lClearRxStatus,
                        int * lTxStatus, 
                        int * lRxStatus); BORLANDC_FOOTER

									 
BORLANDC_HEADER	int _stdcall icsneoGetConfiguration(int hObject, 
						unsigned char * pData, int * lNumBytes); BORLANDC_FOOTER
													 
							
BORLANDC_HEADER	int _stdcall icsneoSendConfiguration(int hObject, unsigned char * pData, int lNumBytes); BORLANDC_FOOTER	

BORLANDC_HEADER	int _stdcall icsneoEnableNetworkCom(int hObject,int lEnable); BORLANDC_FOOTER	


BORLANDC_HEADER	int _stdcall icsneoGetPerformanceParameters(int hObject, int * iBufferCount, 
						int * iBufferMax, int * iOverFlowCount, int * iReserved1, 
						int * iReserved2,int * iReserved3,int * iReserved4,int * iReserved5); BORLANDC_FOOTER	 
