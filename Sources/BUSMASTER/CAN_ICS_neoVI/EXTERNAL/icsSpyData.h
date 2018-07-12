////////////////////////////////////////////////////////////////////////////////////
// icsSpyData.h
// 4/16/2008
// Copyright Intrepid Control Systems, Inc. 2000-2008
// www.intrepidcs.com
////////////////////////////////////////////////////////////////////////////////////

#ifndef VSPY_DATA_HEADER_FILE
#define VSPY_DATA_HEADER_FILE


#include "icsSpyDataCStyle.h"

/*#include "icsSpyDataCommon.h"
struct _icsSpyMessage;
typedef struct _icsSpyMessage icsSpyMessage;
struct _icsSpyMessageLong;
typedef struct _icsSpyMessageLong icsSpyMessageLong;
struct _icsSpyMessageJ1850;
typedef struct _icsSpyMessageJ1850 icsSpyMessageJ1850;  */

class CSpyMessage
{
public:
    CSpyMessage();
    CSpyMessage(const icsSpyMessage*);
    CSpyMessage(const icsSpyMessage&);
    CSpyMessage(const icsSpyMessageLong&);
    CSpyMessage(const CSpyMessage&);

    ~CSpyMessage();
    icsSpyMessageJ1850* ToRawJ1850();
    icsSpyMessage* ToRaw();
    const icsSpyMessageJ1850* ToRawJ1850() const;
    const icsSpyMessage* ToRaw() const;
    void Clear();
    CSpyMessage& operator = (const CSpyMessage&);
    CSpyMessage& operator = (const icsSpyMessage&);
    CSpyMessage& operator = (const icsSpyMessageJ1850& msg);
    CSpyMessage& operator = (const icsSpyMessageLong& msg);

    operator icsSpyMessage();
    operator icsSpyMessageLong();
    operator icsSpyMessageJ1850();
    operator const icsSpyMessage() const;
    operator const icsSpyMessageLong() const;
    operator const icsSpyMessageJ1850() const;



    //public interface

    // icsSpyMessage
    unsigned long& StatusBitField;
    unsigned long& StatusBitField2;
    unsigned long& TimeHardware;
    unsigned long& TimeHardware2;
    unsigned long& TimeSystem;
    unsigned long& TimeSystem2;
    unsigned char& TimeStampHardwareID;
    unsigned char& TimeStampSystemID;
    unsigned char& NetworkID;
    unsigned char& NodeID;
    unsigned char& Protocol;
    unsigned char& MessagePieceID;
    unsigned char& ExtraDataPtrEnabled;
    unsigned char& NumberBytesHeader;
    unsigned char& NumberBytesData;
    short& DescriptionID;
    long& ArbIDOrHeader;
    unsigned char* Data;
    unsigned char* AckBytes;
    unsigned int& ExtraDataPtr;
    unsigned char& MiscData;
    // icsSpyMessageJ1850
    unsigned char* Header;
    //icsSpyMessageLong
    unsigned long& DataMsb;
    unsigned long& DataLsb;

    unsigned long& StatusBitField3;
    unsigned long& StatusBitField4;

private:
    void SanityCheck();

    icsSpyMessage raw;

};

typedef CSpyMessage* PSpyMessage;

#endif

