
#include "string.h"
#include <string>


// OpenPort "OpenType" Argument Constants
#define	NEOVI_COMMTYPE_RS232  0
#define	NEOVI_COMMTYPE_USB_BULK	1
#define	NEOVI_COMMTYPE_USB_ISO_DONT_USE	2
#define	NEOVI_COMMTYPE_TCPIP 3

//hardware constants
#define NEODEVICE_BLUE  1
#define NEODEVICE_DW_VCAN  4
#define NEODEVICE_FIRE  8
#define NEODEVICE_VCAN3  16
#define NEODEVICE_ANY  65535

//device ID's
const int NETID_DEVICE  = 0;
const int NETID_HSCAN = 1;
const int NETID_MSCAN = 2;
const int NETID_SWCAN = 3;
const int NETID_LSFTCAN = 4;
const int NETID_FORDSCP = 5;
const int NETID_J1708 = 6;
const int NETID_AUX = 7;
const int NETID_JVPW = 8;
const int NETID_ISO = 9;
const int NETID_LIN = 9;
const int NETID_ISOPIC= 10;
const int NETID_MAIN51 = 11;
const int NETID_SCI = 13;
const int NETID_ISO2 = 14;
const int NETID_FIRE_LIN1 = 16;
const int NETID_FIRE_HSCAN1 = 41;
const int NETID_FIRE_HSCAN2 = 42;
const int NETID_FIRE_MSCAN1 = 43;
const int NETID_FIRE_MSCAN2 = 44;
const int NETID_FIRE_HSCAN3 = 44;
const int NETID_FIRE_SWCAN = 45;
const int NETID_FIRE_LSFT = 46;
const int NETID_FIRE_LIN2 = 48;
const int NETID_FIRE_LIN3 = 49;
const int NETID_FIRE_LIN4 = 50;

const long SPY_STATUS_GLOBAL_ERR = 0x01;
const long SPY_STATUS_TX_MSG = 0x02;
const long SPY_STATUS_XTD_FRAME = 0x04;
const long SPY_STATUS_REMOTE_FRAME = 0x08;

const long SPY_STATUS_CRC_ERROR = 0x10;
const long SPY_STATUS_CAN_ERROR_PASSIVE = 0x20;
const long SPY_STATUS_INCOMPLETE_FRAME = 0x40;
const long SPY_STATUS_LOST_ARBITRATION = 0x80;

const long SPY_STATUS_UNDEFINED_ERROR = 0x100;
const long SPY_STATUS_CAN_BUS_OFF = 0x200;
const long SPY_STATUS_CAN_ERROR_WARNING = 0x400;
const long SPY_STATUS_BUS_SHORTED_PLUS = 0x800;

const long SPY_STATUS_BUS_SHORTED_GND = 0x1000;
const long SPY_STATUS_CHECKSUM_ERROR = 0x2000;
const long SPY_STATUS_BAD_MESSAGE_BIT_TIME_ERROR = 0x4000;
const long SPY_STATUS_IFR_DATA = 0x8000;

const long SPY_STATUS_COMM_IN_OVERFLOW = 0x10000;
const long SPY_STATUS_COMM_OUT_OVERFLOW = 0x20000;
const long SPY_STATUS_COMM_MISC_ERROR = 0x40000;
const long SPY_STATUS_BREAK = 0x80000;

const long SPY_STATUS_AVSI_REC_OVERFLOW = 0x100000;
const long SPY_STATUS_TEST_TRIGGER = 0x200000;
const long SPY_STATUS_AUDIO_COMMENT = 0x400000;
const long SPY_STATUS_GPS_DATA = 0x800000;

const long SPY_STATUS_ANALOG_DIGITAL_INPUT = 0x1000000;
const long SPY_STATUS_TEXT_COMMENT = 0x2000000;
const long SPY_STATUS_NETWORK_MESSAGE_TYPE = 0x4000000;
const long SPY_STATUS_VSI_TX_UNDERRUN = 0x8000000;

const long SPY_STATUS_VSI_IFR_CRC_BIT = 0x10000000;
const long SPY_STATUS_INIT_MESSAGE = 0x20000000;
const long SPY_STATUS_HIGH_SPEED = 0x40000000;

// Configuration Array constants
// high speed CAN neoVI / valuecan baud rate constants
const long NEO_CFG_MPIC_HS_CAN_CNF1 = 512 + 10;
const long NEO_CFG_MPIC_HS_CAN_CNF2 = 512 + 9;
const long NEO_CFG_MPIC_HS_CAN_CNF3 = 512 + 8;
const long NEO_CFG_MPIC_HS_CAN_MODE = 512 + 54;
// med speed CAN
const long NEO_CFG_MPIC_MS_CAN_CNF1 = 512 + 22;
const long NEO_CFG_MPIC_MS_CAN_CNF2 = 512 + 21;
const long NEO_CFG_MPIC_MS_CAN_CNF3 = 512 + 20;
//SW CAN
const long NEO_CFG_MPIC_SW_CAN_CNF1 = 512 + 34;
const long NEO_CFG_MPIC_SW_CAN_CNF2 = 512 + 33;
const long NEO_CFG_MPIC_SW_CAN_CNF3 = 512 + 32;
//LSFT CAN
const long NEO_CFG_MPIC_LSFT_CAN_CNF1 = 512 + 46;
const long NEO_CFG_MPIC_LSFT_CAN_CNF2 = 512 + 45;
const long NEO_CFG_MPIC_LSFT_CAN_CNF3 = 512 + 44;

// The second status bitfield
const long SPY_STATUS2_HAS_VALUE = 1;
const long SPY_STATUS2_VALUE_IS_BOOLEAN = 2;
const long SPY_STATUS2_HIGH_VOLTAGE = 4;
const long SPY_STATUS2_LONG_MESSAGE = 8;

const int SPY_PROTOCOL_CUSTOM = 0;
const int SPY_PROTOCOL_CAN = 1;
const int SPY_PROTOCOL_GMLAN = 2;
const int SPY_PROTOCOL_J1850VPW = 3;
const int SPY_PROTOCOL_J1850PWM = 4;
const int SPY_PROTOCOL_ISO9141 = 5;
const int SPY_PROTOCOL_Keyword2000 = 6;
const int SPY_PROTOCOL_GM_ALDL_UART = 7;
const int SPY_PROTOCOL_CHRYSLER_CCD = 8;
const int SPY_PROTOCOL_CHRYSLER_SCI = 9;
const int SPY_PROTOCOL_FORD_UBP = 10;
const int SPY_PROTOCOL_BEAN = 11;
const int SPY_PROTOCOL_LIN = 12;
const int SPY_PROTOCOL_J1708 = 13;
const int SPY_PROTOCOL_CHRYSLER_JVPW = 14;
const int SPY_PROTOCOL_J1939 = 15;


// these are used in status2 for Vehicle Spy 3
const long icsspystatusChangeLength =0x10;
const long icsspystatusChangeBitH1 = 0x20;
const long icsspystatusChangeBitH2 = 0x40;
const long icsspystatusChangeBitH3 = 0x80 ;
const long icsspystatusChangeBitB1 = 0x100;
const long icsspystatusChangeBitB2 = 0x200 ;
const long icsspystatusChangeBitB3 = 0x400 ;
const long icsspystatusChangeBitB4 = 0x800 ;
const long icsspystatusChangeBitB5 = 0x1000;
const long icsspystatusChangeBitB6 = 0x2000;
const long icsspystatusChangeBitB7 = 0x4000;
const long icsspystatusChangeBitB8 = 32768  ;
const long icsspystatusChangedGlobal = 65536 ;

const long SCRIPT_STATUS_STOPPED  = 0;
const long SCRIPT_STATUS_RUNNING = 1;

const long SCRIPT_LOCATION_FLASH_MEM = 0;		//(Valid only on a neoVI Fire or neoVI Red)
const long SCRIPT_LOCATION_SDCARD = 1;		//(Valid only on a neoVI Fire or neoVI Red)
const long SCRIPT_LOCATION_VCAN3_MEM = 2;	 //(Valid only on a ValueCAN 3 device)

//Structure for neoVI device types


typedef struct 
{
	int DeviceType;
	int Handle;
	int NumberOfClients;
	int SerialNumber;
	int MaxAllowedClients;
} NeoDevice;

typedef struct
{
    unsigned char Mode;
    unsigned char SetBaudrate;
    unsigned char Baudrate;
    unsigned char NetworkType;
    unsigned char TqSeg1;
    unsigned char TqSeg2;
    unsigned char TqProp;
    unsigned char TqSync;
    unsigned short BRP;
    unsigned short auto_baud;
} CAN_SETTINGS;

typedef struct
{
    unsigned short time_500us;
    unsigned short k;
    unsigned short l;
}ISO9141_KEYWORD2000__INIT_STEP;

typedef struct
{
    unsigned int Baudrate;
    unsigned short spbrg;
    unsigned short brgh;
    ISO9141_KEYWORD2000__INIT_STEP init_steps[16]; //See the ISO9141_KW2000__INIT_STEP structure
    unsigned char init_step_count;
    unsigned short p2_500us;
    unsigned short p3_500us;
    unsigned short p4_500us;
    unsigned short chksum_enabled;
} ISO9141_KW2000SETTINGS;


typedef struct
{
    unsigned char Mode;
    unsigned char SetBaudrate;
    unsigned char Baudrate;
    unsigned char NetworkType;
    unsigned char TqSeg1;
    unsigned char TqSeg2;
    unsigned char TqProp;
    unsigned char TqSync;
    unsigned short BRP;
    unsigned short high_speed_auto_switch;
    unsigned short auto_baud;
} SWCAN_SETTINGS;

typedef struct _LIN_SETTINGS
{
    unsigned int Baudrate;
    unsigned short spbrg;
    unsigned short brgh;
    unsigned char MasterResistor;
    unsigned char Mode;
} LIN_SETTINGS;

typedef struct 
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
    unsigned short cgi_enable;
    unsigned short cgi_baud;
    unsigned short cgi_tx_ifs_bit_times;
    unsigned short cgi_rx_ifs_bit_times;
    unsigned short cgi_chksum_enable;
    unsigned short network_enables;
    unsigned short network_enabled_on_boot;
    unsigned short pwm_man_timeout;
    unsigned short pwr_man_enable;
    unsigned short misc_io_initial_ddr;
    unsigned short misc_io_initial_latch;
    unsigned short misc_io_analog_enable;
    unsigned short misc_io_report_period;
    unsigned short misc_io_on_report_events;
    unsigned short ain_sample_period;
    unsigned short ain_threshold;
    unsigned short iso15765_separation_time_offset; 
    unsigned short iso9141_kwp_enable;
    ISO9141_KW2000SETTINGS iso9141_kwp_settings; 
    unsigned short perf_en;
    unsigned short iso_parity; 
    unsigned short iso_msg_termination; 
}SFireSettings; 




typedef struct 
{
    CAN_SETTINGS can1;
    CAN_SETTINGS can2;
    unsigned short  network_enables;
    unsigned short network_enabled_on_boot;
    short iso15765_separation_time_offset;
    unsigned short perf_en;
    unsigned short misc_io_initial_ddr;
    unsigned short misc_io_initial_latch;
    unsigned short misc_io_report_period;
    unsigned short misc_io_on_report_events;
} SVCAN3Settings;



typedef struct // matching C structure
    {
	unsigned long StatusValue;  // 4
	unsigned long StatusMask;  // 4
	unsigned long Status2Value;  // 4
	unsigned long Status2Mask;  // 4
	unsigned long Header;	// 4
	unsigned long HeaderMask;  // 4
	unsigned long MiscData; // 4
	unsigned long MiscDataMask;  // 4 
	unsigned long ByteDataMSB;	// 4
	unsigned long ByteDataLSB;	// 4
	unsigned long ByteDataMaskMSB;  // 4
	unsigned long ByteDataMaskLSB;  // 4
	unsigned long HeaderLength; // 4 
	unsigned long ByteDataLength; // 4
	unsigned long NetworkID;	// 4
	unsigned short FrameMaster;	// 2
	unsigned char bUseArbIdRangeFilter;
	unsigned char bStuff2;
	unsigned long ExpectedLength;
	unsigned long NodeID;
	}  spyFilterLong;

typedef struct // matching C structure
    {
    unsigned long StatusBitField;	// 4
	unsigned long StatusBitField2;	// 4
    unsigned long TimeHardware;		// 4
	unsigned long TimeHardware2;	// 4
    unsigned long TimeSystem;		// 4
	unsigned long TimeSystem2;		// 4
	unsigned char TimeStampHardwareID;		// 1
	unsigned char TimeStampSystemID;
	unsigned char NetworkID;		// 1
	unsigned char NodeID;
	unsigned char Protocol;
	unsigned char MessagePieceID;	// 1
	unsigned char ColorID;			// 1
    unsigned char NumberBytesHeader;// 1
    unsigned char NumberBytesData;  // 1
	short DescriptionID;			// 2
    long ArbIDOrHeader;				// 4
    unsigned char Data[8];			
	unsigned char AckBytes[8];			
    float Value;
	unsigned char MiscData;
    }  icsSpyMessage;

typedef struct // matching C structure
    {
    unsigned long StatusBitField;	// 4
	unsigned long StatusBitField2;	// 4
    unsigned long TimeHardware;		// 4
	unsigned long TimeHardware2;	// 4
    unsigned long TimeSystem;		// 4
	unsigned long TimeSystem2;		// 4
	unsigned char TimeStampHardwareID;		// 1
	unsigned char TimeStampSystemID;	// 1
	unsigned char NetworkID;		// 1
	unsigned char NodeID;			// 1
	unsigned char Protocol;			// 1
	unsigned char MessagePieceID;	// 1
	unsigned char ColorID;			// 1
    unsigned char NumberBytesHeader;// 1
    unsigned char NumberBytesData;  // 1
	short DescriptionID;			// 2
    unsigned char Header[4];		// 4
    unsigned char Data[8];
	unsigned char AckBytes[8];			
    float Value;
	unsigned char MiscData;
    }  icsSpyMessageJ1850;



typedef struct _stCM_ISO157652_TxMessage
{
	//transmit message
	unsigned short vs_netid;	///< The netid of the message (determines which network to transmit on),  not supported

	unsigned char padding;	///< The padding byte to use to fill the unused portion of
							///  transmitted CAN frames (single frame, first frame, consecutive frame),
							///  not supported as it is always 0xAA.
	
	unsigned char	reserved;

	unsigned int    id;	///< arbId of transmitted frames (CAN id to transmit to).
	unsigned int    fc_id;		///< flow control arb id filter value (response id from receiver).
    unsigned int    fc_id_mask;	///< The flow control arb filter mask (response id from receiver).

	unsigned char		reserved1;
	unsigned char		reserved2;
	unsigned char		flowControlExtendedAddress;	///< Expected Extended Address byte of response from receiver.  see fc_ext_address_enable, not supported.
	unsigned char		extendedAddress;			///< Extended Address byte of transmitter. see ext_address_enable, not supported.

	//flow control timeouts
	unsigned short    fs_timeout;				///< max timeout (ms) for waiting on flow control respons, not supported.
	unsigned short    fs_wait;				///< max timeout (ms) for waiting on flow control response that does not have 
											///flow status set to WAIT, not supported.

	unsigned char data[4*1024];	///< The data

	unsigned int num_bytes;	///< Number of data bytes

	//option bits
	union
	{
		struct
		{
			unsigned id_29_bit_enable:1;	///< Enables 29 bit arbId for transmitted frames.  Set to 1 so transmitted frames use 29 bit ids, not supported.
			unsigned fc_id_29_bit_enable:1;	///< Enables 29 bit arbId for Flow Control filter.  Set to 1 if receiver response uses 29 bit ids, not supported.
			unsigned ext_address_enable:1;	///< Enables Extended Addressing, Set to 1 if transmitted frames should have extended addres byte, not supported.
			unsigned fc_ext_address_enable:1;	///< Enables Extended Addressing for Flow Control filter.  Set to 1 if receiver responds with extended address byte, not supported.
		};
		unsigned short flags;
	};
      
	
}stCM_ISO157652_TxMessage;

typedef struct _stCM_ISO157652_RxMessage
{
	//transmit message
	unsigned short vs_netid;	///< The netid of the message (determines which network to decode receives),  not supported

	unsigned char padding;	///< The padding byte to use to fill the unused portion of
							///  transmitted CAN frames (flow control),
							///  not supported as it is always 0xAA.

	unsigned int    id;			///< ArbId filter value for frames from transmitter (from ECU to neoVI).
	unsigned int    id_mask;	///< ArbId filter mask for frames from transmitter (from ECU to neoVI).
	unsigned int    fc_id;		///< flow control arbId to transmit in flow control (from neoVI to ECU).

	unsigned char		flowControlExtendedAddress;	///< Extended Address byte used in flow control (from neoVI to ECU). see fc_ext_address_enable.
	unsigned char		extendedAddress;	///< Expected Extended Address byte of frames sent by transmitter (from ECU to neoVI).  see ext_address_enable.

	unsigned char		blockSize;	
	unsigned char		stMin;				

	//flow control timeouts
	unsigned short    cf_timeout;				///< max timeout (ms) for waiting on consecutive frame. not supported.

	unsigned char dataValues[4*1024];	///< The filter data byte values
	unsigned char dataMask[4*1024];	///< The filter data byte mask

	unsigned int num_bytes;	///< Number of data bytes

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
		};
		unsigned int flags;
	};

	unsigned char reserved[12];
        
}stCM_ISO157652_RxMessage;