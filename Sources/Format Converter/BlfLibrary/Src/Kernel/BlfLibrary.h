/*
 * BLF Library
 * (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Author:      Pavel Boiko
 * Release:     1.0
 * Annotation:  Internal interfaces of the library.
 *              The library goal is providing access to BLF file data.
 *              It is based on a prototype of Tobias Lorenz and some investigation results of Andrey Oleynikov.
 * Revision History :
 *
 *     Date             Author               Detail description
 * ------------- --------------------- ------------------------------
 * May  15, 2014 Pavel Boiko           First version. It supports only CAN messages, and only reading (not writing).
 * May  21, 2014 Andrey Oleynikov      Timestamps, message directions and types (extended or standard) were added.
 * May  22, 2014 Andrey Oleynikov      Compatible with visual studio 2008 toolset.
 * May  23, 2014 Andrey Oleynikov      Message direction attribut was removed since rawFlags already has this information.
 */

#include "../IBlfLibrary.h"
#include "BinHelper.h"
#include "BlfFormat.h"


namespace BLF
{

//! Just to implement IBlfObject - see that interface for details.
class BlfObject : public IBlfObject
{
public:
    virtual BlfObjectKind GetKind()
    {
        return bokUnknown;
    }
    virtual ICanMessage* GetICanMessage()
    {
        return NULL;
    }
};

//! Just to implement ICanMessage - see that interface for details.
class CanMessage : public ICanMessage
{
public:
    //! Constructor
    //! \param channelNo Channel number
    //! \param id Message ID (including extended message bit)
    //! \param dlc Message DLC (data length)
    //! \param pData Pointer to message data
    //! \param timestamp Message timestamp
    //! \param rawFlags Raw flags from CAN message structure
    CanMessage(WORD channelNo, DWORD id, BYTE dlc, const BYTE* pData, ULONGLONG timestamp, BYTE rawFlags);

public:
    virtual BlfObjectKind GetKind()
    {
        return bokCanMessage;
    }
    virtual ICanMessage* GetICanMessage()
    {
        return (ICanMessage*)this;
    }
    virtual bool Dump();

    virtual WORD GetChannelNo()
    {
        return m_ChannelNo;
    }
    virtual DWORD GetId()
    {
        return m_Id;
    }
    virtual BYTE GetDLC()
    {
        return m_DLC;
    }
    virtual const BYTE* GetData()
    {
        return m_Data;
    }
    virtual ULONGLONG GetTimestamp()
    {
        return m_timestamp;
    }
    virtual MessageDirection GetDirection();

private:
    //! Channel number
    WORD m_ChannelNo;
    //! Message ID
    DWORD m_Id;
    //! Message DLC (data length)
    BYTE m_DLC;
    //! Message data
    BYTE m_Data[8];
    //! Message timestamp
    ULONGLONG m_timestamp;
    //! Raw flags from CAN message structure
    BYTE m_rawFlags;
};

//! Just to implement IBlfLibrary - see that interface for details.
class BlfLibrary : public IBlfLibrary
{
public:
    //! Constructor.
    BlfLibrary();
    //! Destructor.
    virtual ~BlfLibrary();

public:
    virtual bool EnableLogging(const std::string& sLogFilePath);

    virtual HRESULT Load(const std::string& sBlfFilePath);
    virtual bool UnLoad();

    virtual size_t GetBlfObjectsCount()
    {
        return m_CanMessages.size();
    }
    virtual IBlfObject* GetBlfObject(size_t index = 0);

    virtual bool Dump();

    virtual SYSTEMTIME GetStartTime()
    {
        return m_StartTime;
    }

private:
    //! Reads header of the BLF file.
    //! \param[in,out] file BLF file access object. Its offset is changed by the method.
    //! \param[out] header Resulting header.
    //! \return false if there was an error.
    bool ReadBlfFileHeader(BinHelper& file, BlfFileHeader& header);
    //! Checks signature of BLF file.
    //! \param signature The signature to check.
    //! \return false if signature doesn't match expected one.
    bool CheckBlfFileSignature(DWORD signature);
    //! Reads all BLF file objects (BLF file contains only header and object sequence).
    //! \param[in,out] file BLF file access object. Its offset is changed by the method.
    //! \param expectedFileSize Expected size of the file (according to the information from BLF file header).
    //! \return false if there was an error.
    bool ReadBlfObjects(BinHelper& file, size_t expectedFileSize);
    //! Reads and uncompress all container objects in BLF file (on the top level BLF file contains only objects "container").
    //! \param[in,out] file BLF file access object. Its offset is changed by the method.
    //! \param expectedFileSize Expected size of the file (according to the information from BLF file header).
    //! \param[out] uncompressedData Result of uncompressing of all "container" objects (they are just appened to the container one by one).
    //! \return false if there was an error.
    bool ReadAndUncompressAllBlfLogContainerObjects(BinHelper& file, size_t expectedFileSize, std::vector<char>& uncompressedData);
    //! Reads base header of BLF objects. This method doesn't change the file offset.
    //! \param file BLF file access object.
    //! \param[out] objectHeader Resulting header.
    //! \return false if there was an error.
    bool PeekBlfObjectHeader(BinHelper& file, BlfObjectHeaderBase& objectHeader);
    //! Reads and uncompress container object in BLF file.
    //! \param[in,out] file BLF file access object. Its offset is changed by the method.
    //! \param[in,out] uncompressedData Early uncompressed data and appended currenly uncompressed one.
    //! \return false if there was an error.
    bool ReadAndUncompressBlfLogContainer(BinHelper& file, std::vector<char>& uncompressedData);
    //! Uncompresses desired data. The data shall be compressed via zlib algorithm.
    //! \param pDataCompressed Compressed data.
    //! \param dataLenCompressed Length of compressed data.
    //! \param[in,out] pDataUnCompressed Buffer for uncompressed data, allocated outside of the method, will be filled here.
    //! \param dataLenUnCompressed Expected length of uncompressed data. It also means length of pDataUnCompressed buffer.
    //! \return false if there was an error.
    bool UnCompress(char* pDataCompressed, size_t dataLenCompressed, char* pDataUnCompressed, size_t dataLenUnCompressed);
    //! Processes whole uncompressed data from BLF file to extract required data - there all "low-level" objects (like CAN message) are located.
    //! \param uncompressedData Uncompressed data to be processed.
    //! \return false if there was an error.
    bool ProcessBlfUncompressedData(const std::vector<char>& uncompressedData);
    //! Processes (extracts) CAN message from the uncompressed data.
    //! \param uncompressedData Uncompressed data to be processed.
    //! \param[in,out] dataPos Position in uncompressed data. The method changes it.
    //! \return false if there was an error.
    bool ProcessBlfCanMessage(const std::vector<char>& uncompressedData, size_t& dataPos);
    //! Processes just extracted CAN message. Stores extracted data into internal representation.
    //! \param canMessage CAN message as it was decoded from BLF.
    //! \return false if there was an error.
    bool ProcessBlfCanMessage(const BlfObject_CanMessage& canMessage);

private:
    //! CAN messages that was decoded from BLF file.
    std::vector<CanMessage> m_CanMessages;
    //! Start time in blf file
    SYSTEMTIME m_StartTime;
};

} // namespace BLF
