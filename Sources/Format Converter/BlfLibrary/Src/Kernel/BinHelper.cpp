/*
 * BLF Library
 * (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Author:      Pavel Boiko
 * Release:     1.0
 * Annotation:  Aux class to access binary files
 * Revision History :
 *
 *     Date             Author               Detail description
 * ------------- --------------------- ------------------------------
 * Jan. 14, 2010 Pavel Boiko           Prototype
 * May  15, 2014 Pavel Boiko           Adapted for BLF Library
 * May  22, 2014 Andrey Oleynikov      Compatible with visual studio 2008 toolset
 */

#include <iostream>
#include <fstream>
#include <memory>

#include "BinHelper.h"
#include "ErrorManager.h"
#include "Strings.h"

BinHelper::BinHelper(const std::string& fileName)
    : m_IsOkFlag(false), m_FileName(fileName)
{
    m_Offset = 0;

    // Read whole file data into string.
    // All future operations will be performed to this string, not to file.
    // If you will have really big files, probably this way will have to be changed.
    std::ifstream stream(fileName.c_str(), std::ios::binary);
    if (!stream.is_open())
    {
        EM_ERROR("File can't be opened: " + fileName);
    }
    else
    {
        m_IsOkFlag = true;
        m_Data = std::string
                 (
                     (std::istreambuf_iterator<char>(stream)),
                     std::istreambuf_iterator<char>()
                 );
    }
}

BinHelper::~BinHelper()
{
}

bool BinHelper::GetData(void* pData, size_t len)
{
    bool isOk = true;

    // Check - do we have enough data?
    if (m_Offset + len > m_Data.size())
    {
        EM_UNEXPECTED_ERROR("Not enough data in file: " + m_FileName);
        isOk = false;
    }

    if (isOk)
    {
        // Read data
        memcpy(pData, &m_Data[m_Offset], len);
        m_Offset += len;
    }

    return isOk;
}

void BinHelper::SetOffset(size_t offset)
{
    if (offset > m_Data.size())
    {
        EM_UNEXPECTED_ERROR("Not enough data in file: " + m_FileName);
    }

    m_Offset = offset;
}

void BinHelper::AddOffset(size_t val)
{
    if (m_Offset + val > m_Data.size())
    {
        EM_UNEXPECTED_ERROR("Not enough data in file: " + m_FileName);
    }

    m_Offset += val;
}
