#include <wtypes.h>

// BLF object types (see BlfObjectHeader::mObjectType)
#define BLF_OBJECT_TYPE_CAN_MESSAGE 1
#define BLF_OBJECT_TYPE_LOG_CONTAINER 10

//! Structure that describes common information in BLF file (start part of BLF file).
struct BlfFileHeader
{
    //! BLF file signature ("LOGG").
    DWORD       m_Signature;
    //! Size of "virtual" structure that may be formed by data from this one and used in binlog.dll (VBLFileStatisticsEx).
    DWORD       m_StructureSize;
    //! Application ID.
    BYTE        m_ApplicationID;
    //! Application major number (usually 0).
    BYTE        m_ApplicationMajor;
    //! Application minor number (usually 0).
    BYTE        m_ApplicationMinor;
    //! Application build number (usually 0).
    BYTE        m_ApplicationBuildNo;
    //! BL API major number (BL_MAJOR_NUMBER).
    BYTE        m_BinLogMajor;
    //! BL API minor number (BL_MINOR_NUMBER).
    BYTE        m_BinLogMinor;
    //! BL API build number (BL_BUILD_NUMBER).
    BYTE        m_BinLogBuild;
    //! BL API patch number (BL_PATCH_NUMBER).
    BYTE        m_BinLogPatch;
    //! File size.
    ULONGLONG   m_FileSize;
    //! Uncompressed file size.
    ULONGLONG   m_FileSizeUncompressed;
    //! Count of objects in the file.
    DWORD       m_CountOfObjects;
    //! Count of objects read.
    DWORD       m_CountOfObjectsRead;
    //! Start time.
    SYSTEMTIME  m_TimeStart;
    //! End time.
    SYSTEMTIME  m_TimeEnd;
    //! Not used.
    DWORD       m_NotUSed[18];
};

//! Base part of BLF file object.
struct BlfObjectHeaderBase
{
    //! Signature (BLF_OBJECT_SIGNATURE).
    DWORD m_Signature;
    //! Size of this structure.
    WORD  m_HeaderSize;
    //! Object header version (1).
    WORD  m_HeaderVersion;
    //! Object size.
    DWORD m_ObjectSize;
    //! Object type (BLF_OBJECT_TYPE_XXX).
    DWORD m_ObjectType;
};

//! Header of BLF object.
struct BlfObjectHeader
{
    //! Base header.
    BlfObjectHeaderBase m_Header;
    //! Flags.
    DWORD               m_Flags;
    //! Reserved.
    WORD                m_NotUsed;
    //! Object version.
    WORD                m_Version;
    //! Object timestamp.
    ULONGLONG           m_TimeStamp;
};

//! Structure that describes log container in BLF file.
struct BlfObject_LogContainer
{
    //! BLF object header.
    BlfObjectHeaderBase m_Header;
    //! Flags (usually BL_OBJ_FLAG_TIME_ONE_NANS).
    DWORD               m_Flags;
    //! Not used.
    WORD                m_NotUsed;
    //! Object version (usually 0).
    WORD                m_Version;
    //! Uncompressed size of the object.
    ULONGLONG           m_SizeUncompressed;
};

//! Structure that describes CAN message in BLF file.
struct BlfObject_CanMessage
{
    //! BLF object header.
    BlfObjectHeader m_Header;
    //! Channel no.
    WORD            m_Channel;
    //! CAN dir & rtr.
    BYTE            m_Flags;
    //! CAN message data length.
    BYTE            m_DLC;
    //! CAN message ID.
    DWORD           m_ID;
    //! CAN message data.
    BYTE            m_Data[8];
};
