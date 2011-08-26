
// icsSpyData.h - Vehicle Spy Data Structures
// Created 9/25/99
// Copyright 1999 Intrepid Control Systems, Inc.

#ifndef VSPY_DATA_HEADER_FILE

#define VSPY_DATA_HEADER_FILE


// timestamp id fields
const unsigned char  HARDWARE_TIMESTAMP_ID_NONE =0;
const unsigned char  HARDWARE_TIMESTAMP_ID_VSI = 1;
const unsigned char  HARDWARE_TIMESTAMP_ID_AVT_716 = 2;
const unsigned char HARDWARE_TIMESTAMP_ID_NI_CAN = 3;
const unsigned char HARDWARE_TIMESTAMP_ID_NEOVI = 4;
const unsigned char HARDWARE_TIMESTAMP_ID_AVT_717= 5;
const unsigned char HARDWARE_TIMESTAMP_ID_NEOv6_VCAN = 6;

// this data file defines data constants used in Vehicle Spy for C/C++ users
// the data is directed from the icsspyHardware COM interface


/*
'// protocols
Public Enum icsspyHardwareProtocol
    icsspyProtocolCustom
    icsspyProtocolCAN
    icsspyProtocolGMLAN
    icsspyProtocolJ1850VPW
    icsspyProtocolJ1850PWM
    icsspyProtocolISO9141
    icsspyProtocolKeyword2000
    icsspyProtocolGM_ALDL_UART
    icsspyProtocolChryslerCCD
    icsspyProtocolChryslerSCI
    icsspyProtocolFordUBP
	icsspyProtocolBean
End Enum */

const BYTE SPY_PROTOCOL_CUSTOM = 0;
const BYTE SPY_PROTOCOL_CAN = 1;
const BYTE SPY_PROTOCOL_GMLAN = 2;
const BYTE SPY_PROTOCOL_J1850VPW = 3;
const BYTE SPY_PROTOCOL_J1850PWM = 4;
const BYTE SPY_PROTOCOL_ISO9141 = 5;
const BYTE SPY_PROTOCOL_Keyword2000 = 6;
const BYTE SPY_PROTOCOL_GM_ALDL_UART = 7;
const BYTE SPY_PROTOCOL_CHRYSLER_CCD = 8;
const BYTE SPY_PROTOCOL_CHRYSLER_SCI = 9;
const BYTE SPY_PROTOCOL_FORD_UBP = 10;
const BYTE SPY_PROTOCOL_BEAN = 11;
const BYTE SPY_PROTOCOL_LIN = 12;
const BYTE SPY_PROTOCOL_J1708 = 13;
const BYTE SPY_PROTOCOL_CHRYSLER_JVPW = 14;
const BYTE SPY_PROTOCOL_J1939 = 15;

/*
'// these are bitmasks for the status bitfield
Public Enum icsSpyDataStatusBitfield
    icsSpyStatusGlobalError = 2 ^ 0
    icsSpyStatusTx = 2 ^ 1
    icsSpyStatusIsCanType = 2 ^ 2
    icsSpyStatusRemoteFrame = 2 ^ 3

    icsSpyStatusErrCRCError = 2 ^ 4
    icsSpyStatusHardwareTimePresent = 2 ^ 5
    icsSpyStatusErrIncompleteFrame = 2 ^ 6
    icsSpyStatusErrLostArbitration = 2 ^ 7

    icsSpyStatusErrUndefined = 2 ^ 8
    icsSpyStatusHasValue = 2 ^ 9
    icsSpyStatusValueIsBoolean = 2 ^ 10
    icsSpyStatusBusShortedPlus = 2 ^ 11

    icsSpyStatusBusShortedGnd = 2 ^ 12
    icsSpyStatusCheckSumError = 2 ^ 13
    icsSpyStatusErrBadMessageBitTimeError = 2 ^ 14
    icsSpyStatusIFRData = 2 ^ 15

    icsSpyStatusCommInOverflow = 2 ^ 16
    icsSpyStatusCommOutOverflow = 2 ^ 17
    icsSpyStatusCommMiscError = 2 ^ 18
    icsSpyStatusBreak = 2 ^ 19

    icsSpyStatusAVT_VSIRecOverflow = 2 ^ 20
	icsSpyStatusTestTrigger = 2 ^ 21
    icsSpyStatusAudioCommentType = 2 ^ 22
    icsSpyStatusGPSDataValue = 2 ^ 23
    
	icsSpyStatusAnalogDigitalInputValue = 2 ^ 24
	icsSpyStatusTextCommentType = 2 ^ 25
    icsSpyStatusNetworkMessageType = 2 ^ 26
    icsSpyStatusVSI_TxUnderRun = 2 ^ 27
    
	icsSpyStatusVSI_IFR_CRCBit = 2 ^ 28
End Enum */

// constants for the status bitfield
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


// The second status bitfield
/*
Public Enum icsSpyDataStatusBitfield2
    icsSpyStatusHasValue = 2 ^ 0
    icsSpyStatusValueIsBoolean = 2 ^ 1
End Enum */

const long SPY_STATUS2_HAS_VALUE = 1;
const long SPY_STATUS2_VALUE_IS_BOOLEAN = 2;
const long SPY_STATUS2_HIGH_VOLTAGE = 4;
const long SPY_STATUS2_LONG_MESSAGE = 8;

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


/*
Public Type icsSpyMessage
    StatusBitField As Long '4
    StatusBitField2 As Long 'new '4
    TimeHardware As Long ' 4
    TimeHardware2 As Long 'new ' 4
    TimeSystem As Long ' 4
    TimeSystem2 As Long
    TimeStampID As Byte 'new ' 1
    TimeStampSystemID As Byte
    NetworkID As Byte 'new ' 1
    NodeID As Byte
    Protocol As Byte
    MessagePieceID As Byte ' 1
    ColorID As Byte '1
    NumberBytesHeader As Byte ' 1
    NumberBytesData As Byte ' 1
    DescriptionID As Integer ' 2
    ArbIDOrHeader As Long    '// Holds (up to 3 byte 1850 header or 29 bit CAN header) '4
    Data(1 To 8) As Byte '8
    AckBytes(1 To 8) As Byte 'new '8
    Value As Single ' 4
    MiscData As Byte
  
End Type */

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

/*
Public Type icsSpyMessageLong
    StatusBitField As Long ' 4
    StatusBitField2 As Long 'new '4
    TimeHardware As Long
    TimeHardware2 As Long 'new ' 4
    TimeSystem As Long '4
    TimeSystem2 As Long
    TimeStampID As Byte 'new ' 1
    TimeStampSystemID As Byte
    NetworkID As Byte 'new ' 1
    NodeID As Byte
    Protocol As Byte
    MessagePieceID As Byte ' 1
    ColorID As Byte ' 1
    NumberBytesHeader As Byte '
    NumberBytesData As Byte '2
    DescriptionID As Integer '2
    ArbIDOrHeader As Long    '// Holds (up to 3 byte 1850 header or 29 bit CAN header)
    DataMsb As Long
    DataLsb As Long
    AckBytes(1 To 8) As Byte 'new '8
    Value As Single
    MiscData As Byte
    
End Type*/

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
    long ArbIDOrHeader;				// 4
    unsigned long DataMsb;
	unsigned long DataLsb;
	unsigned char AckBytes[8];			
    float Value;
	unsigned char MiscData;
    }  icsSpyMessageLong;



/*
Public Type icsSpyMessageJ1850
    StatusBitField As Long '4
    StatusBitField2 As Long 'new '4
    TimeHardware As Long '4
    TimeHardware2 As Long 'new ' 4
    TimeSystem As Long '4
    TimeSystem2 As Long
    TimeStampID As Byte 'new ' 1
    TimeStampSystemID As Byte
    NetworkID As Byte 'new ' 1
    NodeID As Byte
    Protocol As Byte
    MessagePieceID As Byte ' 1 new
    ColorID As Byte ' 1
    NumberBytesHeader As Byte '1
    NumberBytesData As Byte '1
    DescriptionID As Integer '2
    Header(1 To 4) As Byte  '4  '// Holds (up to 3 byte 1850 header or 29 bit CAN header)
    Data(1 To 8) As Byte '8
    AckBytes(1 To 8) As Byte 'new '8
    Value As Single '4
    MiscData As Byte
End Type */

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

/*
Public Type icsSpyMessageGPS
    StatusBitField As Long  '4
    StatusBitField2 As Long 'new '4
    TimeHardware As Long    '4
    TimeHardware2 As Long 'new ' 4
    TimeSystem As Long '4
    TimeSystem2 As Long
    TimeStampID As Byte 'new ' 1
    TimeStampSystemID As Byte
    NetworkID As Byte 'new ' 1
    NodeID As Byte
    Protocol As Byte
    MessagePieceID As Byte ' 1
    ColorID As Byte ' 1
    GPSDataID As Integer    '2
    DescriptionID As Integer '2
    AckBytes(1 To 8) As Byte 'new '8
    Value1 As Double '8
    Value2 As Double '8
    MiscData As Byte
End Type */

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
	unsigned char NodeID;		// 1
	unsigned char Protocol;		// 1
	unsigned char MessagePieceID;	// 1
	unsigned char ColorID;			// 1
	short GPSDataID;
	short DescriptionID;			// 2
	unsigned char AckBytes[8];			
    double Value1; // 8
    double Value2;// 8
	unsigned char MiscData;
    }  icsSpyMessageGPS;


/*
Public Type icsSpyMessageAnalog
    StatusBitField As Long  '4
    StatusBitField2 As Long 'new '4
    TimeHardware As Long    '4
    TimeHardware2 As Long 'new ' 4
    TimeSystem As Long '4
    TimeSystem2 As Long
    TimeStampID As Byte 'new ' 1
    TimeStampSystemID As Byte
    NetworkID As Byte 'new ' 1
    NodeID As Byte
    Protocol As Byte
    MessagePieceID As Byte ' 1
    ColorID As Byte ' 1
    AIDataID As Integer    '2
    DescriptionID As Integer '2
    Value As Double '8
    HardwareIndex As Integer '2
    AckBytes(1 To 8) As Byte 'new '8
    iDummy As Integer '2
    lDummy As Long '4
    MiscData As Byte
End Type
 */

typedef struct // matching C structure
    {
    unsigned long StatusBitField;	// 4
	unsigned long StatusBitField2;	// 4
    unsigned long TimeHardware;		// 4
	unsigned long TimeHardware2;	// 4
    unsigned long TimeSystem;		// 4
	unsigned long TimeSystem2;		// 4
	unsigned char TimeStampHardwareID;		// 1
	unsigned char TimeStampSystemID;		// 1
	unsigned char NetworkID;		// 1
	unsigned char NodeID;		// 1
	unsigned char Protocol;
	unsigned char MessagePieceID;	// 1
	unsigned char ColorID;			// 1
	short AIDataID;
	short DescriptionID;			// 2
	double Value;
	short HardwareIndex;
	unsigned char AckBytes[8];
	short iDummy;
	long lDummy;
	unsigned char MiscData;
    }  icsSpyMessageAnalog;

/*
Public Enum icsspyHardwareTimeStampID
    icsspyTimeStampNone
    icsspyTimeStampVSI
    icsspyTimeStampAVT_716
    icsspyTimeStampNICAN
End Enum */



/*
Public Enum icsspySystemTimeStampID
    icsspyTimeStampNone
    icsspyTimeStampTimeGetTimeAPI
End Enum*/

const unsigned char SYSTEM_TIMESTAMP_ID_NONE = 0;
const unsigned char SYSTEM_TIMESTAMP_ID_TIMEGETTIME_API = 1;


/*
Public Type icsspyMsgTime
    SystemTimeStampID As Long
    SystemTime1 As Long
    SystemTime2 As Long
    HardwareTimeStampID As Long
    HardwareTime1 As Long
    HardwareTime2 As Long
End Enum */

typedef struct // matching C structure
    {
    unsigned long SystemTimeStampID;	// 4
	unsigned long SystemTime1;			// 4
    unsigned long SystemTime2;			// 4
	unsigned long HardwareTimeStampID;	// 4
    unsigned long HardwareTime1;		// 4
	unsigned long HardwareTime2;		// 4
	
    }  icsSpyMsgTime;


/* Public Type spyFilterBytes
    StatusValue As Long
    StatusMask As Long
    Status2Value As Long
    Status2Mask As Long
    Header(1 To 4) As Byte
    HeaderMask(1 To 4) As Byte
    MiscData As Long
    MiscDataMask As Long
    ByteData(1 To 8) As Byte
    ByteDataMask(1 To 8) As Byte
    HeaderLength As Long
    ByteDataLength As Long
    NetworkID As Long
    FrameMaster As Boolean
    bStuff1 As Byte
    bStuff2 As Byte
    ExpectedLength As Long
    NodeID As Long
End Type */ 


typedef struct // matching C structure
    {
	unsigned long StatusValue;  // 4
	unsigned long StatusMask;  // 4
	unsigned long Status2Value;  // 4
	unsigned long Status2Mask;  // 4
	unsigned char Header[4];	// 4
	unsigned char HeaderMask[4];  // 4
	unsigned long MiscData; // 4
	unsigned long MiscDataMask;  // 4 
	unsigned char ByteData[8];	// 4
	unsigned char ByteDataMask[8];  // 4
	unsigned long HeaderLength; // 4 
	unsigned long ByteDataLength; // 4
	unsigned long NetworkID;	// 4
	unsigned short FrameMaster;	// 2
	unsigned char bUseArbIdRangeFilter;
	unsigned char bStuff2;
	unsigned long ExpectedLength;
	unsigned long NodeID;
	}  spyFilterBytes;

/*Public Type spyFilterLong
    StatusValue As Long
    StatusMask As Long
    Status2Value As Long
    Status2Mask As Long
    Header As Long
    HeaderMask As Long
    MiscData As Long
    MiscDataMask As Long
    ByteDataMsb As Long
    ByteDataLsb As Long
    ByteDataMaskMsb As Long
    ByteDataMaskLsb As Long
    HeaderLength As Long
    ByteDataLength As Long
    NetworkID As Long
    FrameMaster As Boolean
    bStuff1 As Byte
    bStuff2 As Byte
    ExpectedLength As Long
    NodeID As Long
End Type */

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



#endif


