/*
 * BLF Library
 * (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Author:      Pavel Boiko
 * Release:     1.0
 * Annotation:  Implementation of internal interfaces of the library. Main mechanisms of the library are here.
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
 * May  29, 2014 Andrey Oleynikov      Cosmetic changes.
 */

#include <fstream>
#include <vector>

#include "zlib.h"

#include "BlfLibrary.h"
#include "ErrorManager.h"

#define RAW_CANMESSAGE_FLAGS_DIRECTION_MASK 0xF
#define ERR_INPUT_FILE_OPEN              (-1)
#define ERR_INVALID_HEADER               (-2)
#define ERR_INVALID_BLF_SIGNATURE        (-3)
#define ERR_UNSUPPORTED_BLF_OBJ          (-4)

namespace BLF
{

BLFLIBRARY_API IBlfLibrary* GetIBlfLibrary()
{
    static BlfLibrary lib;
    return &lib;
}

//////////////////////////////////////////////////////
// BlfLibrary
//////////////////////////////////////////////////////

BlfLibrary::BlfLibrary()
{
    // Nothing
}

BlfLibrary::~BlfLibrary()
{
    UnLoad();
}

bool BlfLibrary::EnableLogging(const std::string& sLogFilePath)
{
    // Enable error/warning/info messages logging
    EM_ENABLE_LOGGING(sLogFilePath);
    EM_INFO("BLF Library log:");
    EM_LOG_DEPTH_INC();
    return true;
}

HRESULT BlfLibrary::Load(const std::string& sBlfFilePath)
{
    EM_INFO("BLF file loading - start");
    EM_LOG_DEPTH_INC();
    HRESULT isOk = S_OK;

    // Call unload function for the case when this method wasn't correctly called manually
    isOk = UnLoad();

    // Create aux object to have an access to the binary file
    if (isOk)
    {
        EM_INFO("Create aux object to access BLF file");
        BinHelper file(sBlfFilePath);
        isOk = file.IsOk();
        if(isOk == false)
        {
            return ERR_INPUT_FILE_OPEN;
        }

        // Read BLF file header
        BlfFileHeader blfFileHeader;
        if (isOk)
        {
            isOk = ReadBlfFileHeader(file, blfFileHeader);
        }

        if(isOk == false)
        {
            return ERR_INVALID_HEADER;
        }
        // Check BLF file signature
        if (isOk)
        {
            isOk = CheckBlfFileSignature(blfFileHeader.m_Signature);
        }

        if(isOk == false)
        {
            return ERR_INVALID_BLF_SIGNATURE;
        }

        // Read BLF file objects
        if (isOk)
        {
            isOk = ReadBlfObjects(file, (size_t)blfFileHeader.m_FileSize);
        }

        if(isOk == false)
        {
            return ERR_UNSUPPORTED_BLF_OBJ;
        }

        if (isOk)
        {
            m_StartTime = blfFileHeader.m_TimeStart;
        }
    }

    EM_LOG_DEPTH_DEC();
    EM_INFO("BLF file loading - finish");
    return S_OK;
}

bool BlfLibrary::ReadBlfFileHeader(BinHelper& file, BlfFileHeader& header)
{
    EM_INFO("Read BLF file header");

    bool isOk = file.GetData(&header, sizeof(BlfFileHeader));

    return isOk;
}

bool BlfLibrary::CheckBlfFileSignature(DWORD signature)
{
    EM_INFO("Check BLF file signature");
    bool isOk = true;

    // The first four bytes shall be "LOGG", in 'GGOL' below we just changed byte order, because mFileSignature is DWORD, not a string
    isOk = (signature == 'GGOL');
    if (!isOk)
    {
        EM_ERROR(std::string("Unexpected BLF file signature (") + (int)signature + ").");
    }

    return isOk;
}

bool BlfLibrary::ReadBlfObjects(BinHelper& file, size_t expectedFileSize)
{
    EM_INFO("Reading BLF file objects - start");
    EM_LOG_DEPTH_INC();
    bool isOk = true;

    // Find and uncompress all "log container" objects
    std::vector<char> uncompressedData;
    if (isOk)
    {
        isOk = ReadAndUncompressAllBlfLogContainerObjects(file, expectedFileSize, uncompressedData);
    }

    // Process uncompressed data
    if (isOk)
    {
        isOk = ProcessBlfUncompressedData(uncompressedData);
    }

    EM_LOG_DEPTH_DEC();
    EM_INFO("Reading BLF file objects - finish");
    return isOk;
}

bool BlfLibrary::ReadAndUncompressAllBlfLogContainerObjects(BinHelper& file, size_t expectedFileSize, std::vector<char>& uncompressedData)
{
    EM_INFO("Reading and uncompressing all \"log container\" objects - start");
    EM_LOG_DEPTH_INC();
    bool isOk = true;

    int objectNo = 0;
    while (isOk && (file.GetOffset() < expectedFileSize))
    {
        ++objectNo;
        EM_INFO(std::string("Reading BLF file object #") + objectNo + " - start");
        EM_LOG_DEPTH_INC();

        // Read BLF object header
        BlfObjectHeaderBase objectHeader;
        if (isOk)
        {
            isOk = PeekBlfObjectHeader(file, objectHeader);
        }

        // Read known BLF objects and skip unknown ones
        if (isOk)
        {
            switch(objectHeader.m_ObjectType)
            {
                case BLF_OBJECT_TYPE_LOG_CONTAINER:
                    isOk = ReadAndUncompressBlfLogContainer(file, uncompressedData);
                    break;

                default:
                    // Skip not supported object
                    EM_WARNING(std::string("Unexpected BLF object (code: ") + (int)objectHeader.m_ObjectType + ") is found and skipped.");
                    file.AddOffset(objectHeader.m_ObjectSize);
            }
        }

        EM_LOG_DEPTH_DEC();
        EM_INFO(std::string("Reading BLF file object #") + objectNo + " - end");
        EM_LOG_DEPTH_INC();
    }

    EM_LOG_DEPTH_DEC();
    EM_INFO("Reading and uncompressing all \"log container\" objects - finish");
    return isOk;
}

bool BlfLibrary::PeekBlfObjectHeader(BinHelper& file, BlfObjectHeaderBase& objectHeader)
{
    EM_INFO("Read BLF object header");

    size_t offset = file.GetOffset();
    bool isOk = file.GetData(&objectHeader, sizeof(BlfObjectHeaderBase));
    file.SetOffset(offset);

    return isOk;
}

bool BlfLibrary::ReadAndUncompressBlfLogContainer(BinHelper& file, std::vector<char>& uncompressedData)
{
    EM_INFO("Reading and uncompressing BLF log container - start");
    EM_LOG_DEPTH_INC();

    // Read container data
    EM_INFO("Read container data");
    BlfObject_LogContainer logContainer;
    memset(&logContainer, 0, sizeof(BlfObject_LogContainer));
    bool isOk = file.GetData(&logContainer.m_Header, sizeof(BlfObject_LogContainer));

    // Get compressed data
    size_t sizeCompressed = 0;
    char* dataCompressed = NULL;
    if (isOk)
    {
        EM_INFO("Get comressed data");
        sizeCompressed = (size_t) logContainer.m_Header.m_ObjectSize - sizeof(BlfObject_LogContainer);
        dataCompressed = new char[sizeCompressed];
        isOk = file.GetData(dataCompressed, sizeCompressed);
    }

    // Uncompress data
    size_t sizeUnCompressed = 0;
    char* dataUnCompressed = NULL;
    if (isOk)
    {
        EM_INFO("Uncomress data");
        sizeUnCompressed = (size_t) logContainer.m_SizeUncompressed;
        dataUnCompressed = new char[sizeUnCompressed];
        isOk = UnCompress(dataCompressed, sizeCompressed, dataUnCompressed, sizeUnCompressed);
        if (dataCompressed)
        {
            delete[] dataCompressed;
            dataCompressed = NULL;
        }
    }

    // Append just uncompressed data to the result container
    if (isOk)
    {
        EM_INFO("Append just uncompressed data to the result container");
        // Yes, it looks not so fast to intially allocate array, and then convert it to vector, but the perfomance is still enough
        // - if for some reason it will not be enough, you may rewrite the mechanism to avoid extra memory allocation/copying.
        std::vector<char> uncompressedDataTmp(dataUnCompressed, dataUnCompressed + sizeUnCompressed);
        uncompressedData.insert(uncompressedData.end(), uncompressedDataTmp.begin(), uncompressedDataTmp.end());
        if (dataUnCompressed)
        {
            delete[] dataUnCompressed;
            dataUnCompressed = NULL;
        }
    }

    // Read container padding bytes (if need)
    if (isOk)
    {
        int paddingBytes = sizeCompressed % 4;
        if (paddingBytes)
        {
            EM_INFO("Read container padding bytes");
            file.AddOffset(paddingBytes);
        }
    }

    if (NULL != dataCompressed)
    {
        delete[] dataCompressed;
        dataCompressed = NULL;
    }

    EM_LOG_DEPTH_DEC();
    EM_INFO("Reading and uncompressing BLF log container - finish");
    return isOk;
}

bool BlfLibrary::ProcessBlfUncompressedData(const std::vector<char>& uncompressedData)
{
    EM_INFO("Processing BLF uncompressed data - start");
    EM_LOG_DEPTH_INC();
    bool isOk = true;

    int objectNo = 0;
    size_t dataPos = 0;
    size_t dataLen = uncompressedData.size();

    while(isOk && (dataPos + sizeof(BlfObjectHeaderBase) <= dataLen))
    {
        ++objectNo;
        EM_INFO(std::string("Processing BLF object #") + objectNo + " - start");
        EM_LOG_DEPTH_INC();

        // Get BLF object header
        const BlfObjectHeaderBase& objectHeader = *(BlfObjectHeaderBase*)&uncompressedData[dataPos];

        // Read known BLF objects and skip unknown ones
        switch(objectHeader.m_ObjectType)
        {
            case BLF_OBJECT_TYPE_CAN_MESSAGE:
                isOk = ProcessBlfCanMessage(uncompressedData, dataPos);
                break;

            default:
                // Skip not supported object
                // It is "info", not "warning" because we know that in BLF are much more types than we process.
                EM_INFO(std::string("Not supported BLF object (code: ") + (int)objectHeader.m_ObjectType + ") is found and skipped.");
                dataPos += objectHeader.m_ObjectSize;
        }

        // Skip padding bytes (if need)
        if (isOk)
        {
            dataPos += objectHeader.m_ObjectSize % 4;
        }

        EM_LOG_DEPTH_DEC();
        EM_INFO(std::string("Processing BLF object #") + objectNo + " - end");
    }

    EM_LOG_DEPTH_DEC();
    EM_INFO("Processing BLF uncompressed data - finish");
    return isOk;
}

bool BlfLibrary::ProcessBlfCanMessage(const std::vector<char>& uncompressedData, size_t& dataPos)
{
    EM_INFO("Processing BLF uncompressed data for CAN message - start");
    EM_LOG_DEPTH_INC();
    bool isOk = true;

    size_t dataLen = uncompressedData.size();
    isOk = (dataPos + sizeof(BlfObject_CanMessage) <= dataLen);
    if (!isOk)
    {
        EM_ERROR("Not enough data for CAN message.");
    }

    if (isOk)
    {
        const BlfObject_CanMessage& canMessage = *(BlfObject_CanMessage*)&uncompressedData[dataPos];
        isOk = ProcessBlfCanMessage(canMessage);
        dataPos += sizeof(BlfObject_CanMessage);
    }

    EM_LOG_DEPTH_DEC();
    EM_INFO("Processing BLF uncompressed data for CAN message - finish");
    return isOk;
}

bool BlfLibrary::ProcessBlfCanMessage(const BlfObject_CanMessage& canMessage)
{
    m_CanMessages.push_back(CanMessage(canMessage.m_Channel
                                       , canMessage.m_ID
                                       , canMessage.m_DLC
                                       , canMessage.m_Data
                                       , canMessage.m_Header.m_TimeStamp
                                       , canMessage.m_Flags));
    return true;
}

bool BlfLibrary::UnCompress(char* pDataCompressed, size_t dataLenCompressed, char* pDataUnCompressed, size_t dataLenUnCompressed)
{
    int err;
    z_stream d_stream; // Decompression stream

    d_stream.zalloc = NULL;
    d_stream.zfree = NULL;
    d_stream.opaque = NULL;

    d_stream.next_in  = (Bytef*)pDataCompressed;
    d_stream.avail_in = 0;
    d_stream.next_out = (Bytef*)pDataUnCompressed;

    err = inflateInit(&d_stream);

    while (d_stream.total_out < dataLenUnCompressed && d_stream.total_in < dataLenCompressed)
    {
        d_stream.avail_in = d_stream.avail_out = 1; // Force small buffers
        err = inflate(&d_stream, Z_NO_FLUSH);
        if (err == Z_STREAM_END)
        {
            break;
        }
    }

    err = inflateEnd(&d_stream);

    if (Z_OK != err)
    {
        EM_ERROR(std::string("Unable to uncompress the data with zlib, error code: ") + err + ".");
    }

    return (Z_OK == err);
}

bool BlfLibrary::UnLoad()
{
    m_CanMessages.clear();
    return true;
}

IBlfObject* BlfLibrary::GetBlfObject(size_t index/* = 0*/)
{
    if (m_CanMessages.empty() || (m_CanMessages.size() <= index))
    {
        return NULL;
    }

    return &m_CanMessages[index];
}

bool BlfLibrary::Dump()
{
    bool isOk = true;

    // Outputs measurement start time in format like:
    // measurementStartTime: 2008-10-23 4 18:50:17.137
    SYSTEMTIME startTime = GetStartTime();
    std::cout << "measurementStartTime: " << startTime.wYear << "-" << startTime.wMonth << "-" << startTime.wDay
              << " " << startTime.wDayOfWeek << " " << startTime.wHour << ":" << startTime.wMinute << ":" << startTime.wSecond
              << "." << startTime.wMilliseconds << std::endl;

    for (std::vector<CanMessage>::iterator iCanMessage = m_CanMessages.begin(); iCanMessage != m_CanMessages.end() ; ++iCanMessage)
    {
        isOk = (*iCanMessage).Dump();
        if (!isOk)
        {
            break;
        }
    }

    return isOk;
}

//////////////////////////////////////////////////////
// CanMessage
//////////////////////////////////////////////////////

CanMessage::CanMessage(WORD channelNo, DWORD id, BYTE dlc, const BYTE* pData, ULONGLONG timestamp, BYTE rawFlags)
    : m_ChannelNo(channelNo), m_Id(id), m_DLC(dlc), m_timestamp(timestamp), m_rawFlags(rawFlags)
{
    memcpy(m_Data, pData, min(dlc, 8));
}

bool CanMessage::Dump()
{
    std::cout
            << "CANMessage:"
            << " Channel=" << m_ChannelNo
            << " Flags=" << (int)m_rawFlags
            << " DLC=" << (int)m_DLC
            << " ID=0x" << std::hex << m_Id
            << " Data=";

    for (int i = 0 ; (i < m_DLC) ; ++i)
    {
        if (m_Data[i] <= 0xf)
        {
            std::cout << '0';
        }
        std::cout << std::hex;
        std::cout << (int)m_Data[i];
        std::cout << " ";
    }
    std::cout << std::endl;

    return true;
}

MessageDirection CanMessage::GetDirection()
{
    MessageDirection direction;
    if((m_rawFlags & RAW_CANMESSAGE_FLAGS_DIRECTION_MASK) == 0)
    {
        direction = mdRx;
    }
    else
    {
        direction = mdTx;
    }
    return direction;
}

} // namespace BLF
