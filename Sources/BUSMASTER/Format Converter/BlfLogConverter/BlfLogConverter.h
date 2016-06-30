/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      BlfLogConverter.h
 * \brief     Descripton of the BlfLogConverter class and DLLMain Function.
 * \authors   Andrey Oleynikov
 * \copyright Copyright (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Description of the BlfLogConverter class.
 */

#pragma once

/* C++ includes */
#include <string>

/* Project includes */
#include "../FormatConverterApp/BaseConverter.h"

#include <IBlfLibrary.h>

using namespace std;

#define ERR_INPUT_FILE_NOTFOUND          (-1)
#define ERR_OUTPUT_FILE_NOTFOUND         (-2)
#define ERR_UNABLE_TO_GET_LIB_INTERFACE  (-3)
#define ERR_UNABLE_TO_CONVERT            (-4)
#define ERR_PROTOCOL_NOT_SUPPORTED       (-5)

//! Class that care of BlfLibrary, use it when Load blf file to unload automatically
//! in destructor
class CBlfLibraryKeeper
{
public:
    //! Creates keeper object
    //! \param pBlfLib Pointer to library, it is considered, that function Load is called before destroying of keeper object
    CBlfLibraryKeeper(BLF::IBlfLibrary* pBlfLib);
    //! Unload blf file when object destroyed
    ~CBlfLibraryKeeper();
private:
    //! Pointer to Blf library
    BLF::IBlfLibrary* m_pBlfLib;
};

class CBlfLogConverter : public CBaseConverter
{
    //! Conversion state in textual format
    string m_omstrConversionStatus;
    //! Conversion result
    HRESULT m_hResult;
public:
    CBlfLogConverter(void);
    ~CBlfLogConverter(void);
    virtual HRESULT GetInputFileFilters(string&, string& );
    virtual HRESULT GetOutputFileFilters(string&, string& );
    virtual HRESULT ConvertFile(string& chInputFile, string& chOutputFile);
    virtual HRESULT GetConverterName(string& strConverterName);
    virtual HRESULT GetErrorStatus(HRESULT hResult, string& omstrStatus);
    virtual HRESULT GetLastConversionStatus(HRESULT& hResult, string& omstrStatus);
    virtual HRESULT GetHelpText(CString& pchHelpText);
    virtual BOOL bHaveOwnWindow();
    virtual HRESULT GettextBusmaster();
    //! Do nothing, since there are no properties for this converter
    virtual HRESULT GetPropertyPage(CPropertyPage*& pPage)
    {
        return S_FALSE;
    };
private:
    HRESULT WriteToLog(BLF::IBlfLibrary* pBlfLib, std::ofstream& stream) const;
    void AddFunctionHeader(std::ofstream& stream
                           , WORD day
                           , WORD month
                           , WORD year
                           , WORD hour
                           , WORD mins
                           , WORD sec) const;
    void WriteTimestamp(ULONGLONG timestamp, std::ofstream& stream) const;
};
