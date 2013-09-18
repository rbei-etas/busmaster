////////////////////////////////////////////////////////////////////////////////////
// icsSpyData.h
// 4/16/2008
// Copyright Intrepid Control Systems, Inc. 2000-2008
// www.intrepidcs.com
////////////////////////////////////////////////////////////////////////////////////

#ifndef VSPY_DATA_C_STYLE_HEADER_FILE
#define VSPY_DATA_C_STYLE_HEADER_FILE

#include "icsSpyDataCommon.h"
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

typedef struct _icsSpyMessage // matching C structure
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
	unsigned char ExtraDataPtrEnabled;			// 1
    unsigned char NumberBytesHeader;// 1
    unsigned char NumberBytesData;  // 1
	short DescriptionID;			// 2
    long ArbIDOrHeader;				// 4
    unsigned char Data[8];
    union
    {
    	struct
        {
	    	unsigned long StatusBitField3;
	    	unsigned long StatusBitField4;
        };
	unsigned char AckBytes[8];
    };
    unsigned int ExtraDataPtr;
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

typedef struct _icsSpyMessageLong// matching C structure
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
	unsigned char ExtraDataPtrEnabled;			// 1
    unsigned char NumberBytesHeader;// 1
    unsigned char NumberBytesData;  // 1
	short DescriptionID;			// 2
    long ArbIDOrHeader;				// 4
    unsigned long DataMsb;
	unsigned long DataLsb;
	unsigned char AckBytes[8];
    unsigned int ExtraDataPtr;
	unsigned char MiscData;

        /*  THIS IS NOT ALLOWED IN C - DAVER
        icsSpyMessageLong_struct() {
                ExtraDataPtrEnabled = false;
                NumberBytesData = 0;
                ExtraDataPtr = 0;
        } */

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

typedef struct _icsSpyMessageJ1850// matching C structure
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
	unsigned char ExtraDataPtrEnabled;			// 1
    unsigned char NumberBytesHeader;// 1
    unsigned char NumberBytesData;  // 1
	short DescriptionID;			// 2
    unsigned char Header[4];		// 4
    unsigned char Data[8];
    union
    {
    	struct
        {
	    	unsigned long StatusBitField3;
	    	unsigned long StatusBitField4;
        };
	unsigned char AckBytes[8];
    };			
    unsigned int ExtraDataPtr;
	unsigned char MiscData;
    }  icsSpyMessageJ1850;





#endif

