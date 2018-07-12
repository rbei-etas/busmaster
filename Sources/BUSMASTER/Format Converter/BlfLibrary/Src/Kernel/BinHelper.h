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
 */

#ifndef BIN_HELPER_H
#define BIN_HELPER_H

//! Aux class to get data from binary file.
class BinHelper
{
public:
    //! Constructor of the object.
    //! \param fileName Full path to desired binary file. For example, "C:\file.blf".
    BinHelper(const std::string& fileName);
    //! Destructor of the object.
    ~BinHelper();

    //! Returns true if this helper can be used (binary file was successfully opened).
    bool IsOk()
    {
        return m_IsOkFlag;
    }

    //! Reads desired length of data into desired structure.
    bool GetData(void* pData, size_t len);
    //! Returns value of the current offset in the file.
    size_t GetOffset()
    {
        return m_Offset;
    }
    //! Set new value of the current offset in the file.
    void SetOffset(size_t offset);
    //! Increases value of the current offset in the file.
    void AddOffset(size_t val);

private:
    //! True if binary file was successfully opened, and this helper class can be used to access to its data.
    bool m_IsOkFlag;
    //! Full path to accessed binary file.
    std::string m_FileName;
    //! All data from binary file.
    std::string m_Data;
    //! Current offset in binary file (or in 'data' container).
    size_t m_Offset;
};

#endif //#ifndef BIN_HELPER_H
