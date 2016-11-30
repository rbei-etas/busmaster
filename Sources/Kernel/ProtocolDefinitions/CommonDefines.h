#pragma once
//#define   PROTOCOL_EXPORTS 1
#ifdef PROTOCOL_EXPORTS
#define EXPORTS __declspec(dllexport)
#else
#define EXPORTS
#endif

//#define   EXPORTS


typedef unsigned __int64        UID_ELEMENT;
typedef int                     ERRORCODE;
typedef int                     STARTBIT;

#define EC_SUCCESS                  0
#define EC_WARNING_SUCCESS          0x80000000
#define EC_FAILURE                  0x80000001
#define EC_FAIL_ELEMENT_NOT_FOUND   0x80000002
#define EC_DUPLICATE_FILE           0x08000004
#define EC_FILE_NOT_FOUND           0x08000005
#define EC_FILE_INVALID             0x08000006
#define EC_INVALID_PROTOCOL         0x08000007
#define EC_NA                       0x80000008



#define INVALID_UID_ELEMENT         0
#define MAX_LIN_FRAME_LENGTH        8
#define DIRECTION_TX                      0
#define DIRECTION_RX                      1
#define DIRECTION_ALL                     2
#define MAX_SUPPORTED_STRING        256

#define INVALID_DATA            -1
#define EMPTY_STRING            ""
#define defBITS_IN_BYTE         8



enum eDiagType
{
    eLIN_NORMAL_FRAME_ID,
    eLIN_MASTER_FRAME_ID,
    eLIN_SLAVE_FRAME_ID,
    eLIN_SID_ASSIGN_FRAME_ID ,
    eLIN_SID_UNASSIGN_FRAME_ID,
    eLIN_SID_ASSIGN_NAD_ID ,
    eLIN_SID_COND_CHANGE_NAD,
    eLIN_SID_DATA_DUMP,
    eLIN_SID_SAVE_CONFIG ,
    eLIN_SID_ASSIGN_FRAME_RANGE,
    eLIN_SID_FREEFORMAT,
    eLIN_SID_READ_BY_IDENTIFIER,
    eLIN_SID_ALL,
    eLIN_SID_NONE,
};

const std::string m_omDialType[eLIN_SID_ALL] =
{
    "UnConditional",
    "Master Request",
    "Slave Response",
    "AssignFrameId",
    "UnassignFrameId",
    "AssignNAD",
    "ConditionalChangeNAD",
    "DataDump",
    "SaveConfiguration",
    "AssignFrameIdRange",
    "FreeFormat",
    "ReadyByIdentifier"
};

enum eSignalType
{
    eSignalNormal,
    eSignalDiag
};


enum eSignalDataType
{
	Invalid,
    eSigned,
    eUnsigned,
	Float,
	Double,
	ByteArray
};
enum eEndianess
{
    eMotorola,
    eIntel
};


enum eProtocolType
{
    eCANProtocol,
    eLINProtocol,
    eFlexRayProtocol,
    eInvalidProtocol
};


enum eFormat
{
    HEXADECIMAL = 0,
    DEC
};
typedef eFormat EFORMAT;


