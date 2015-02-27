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
 * \file      BlfLogConverter.cpp
 * \brief     Implementation of the BlfLogConverter class and DLLMain Function.
 * \authors   Andrey Oleynikov
 * \copyright Copyright (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the BlfLogConverter class.
 */

/* Project includes */
#include "BlfLogConverter.h"
#include <fstream>
#include <iomanip>


/**
 * \brief Constructor
 *
 * Constructor of CBlfLogConverter
 */
CBlfLogConverter::CBlfLogConverter(void)
    : m_hResult(S_FALSE)
{
}

/**
 * \brief Destructor
 *
 * Destructor of CBlfLogConverter
 */
CBlfLogConverter::~CBlfLogConverter(void)
{
}

HRESULT CBlfLogConverter::GettextBusmaster(void)
{
    setlocale(LC_ALL,"");
    bindtextdomain("BUSMASTER", getenv("LOCALDIR") );
    textdomain("BUSMASTER");
    return S_OK;
}

/**
 * \brief      Get help text
 * \param[out] pchHelpText Help Text
 * \return     Result code
 *
 * Returns pchHelpText containing the help text.
 */
HRESULT CBlfLogConverter::GetHelpText(CString& pchHelpText)
{
    pchHelpText = _("Converts the CANoe CAN BLF file(.blf) to BUSMASTER CAN log file(.log)");
    return S_OK;
}

/**
 * \brief      Get converter name
 * \param[out] strConverterName Converter Name
 * \return     Result code
 *
 * Returns strConverterName containing the converter name.
 */
HRESULT CBlfLogConverter::GetConverterName(string& strConverterName)
{
    strConverterName = _("BLF TO LOG Conversion");
    return S_OK;
}

/**
 * \brief      Get error status string
 * \param[in]  hResult Error code
 * \param[out] omstrStatus Corresponding error string
 * \return     Result code
 *
 * Returns omstrStatus containing the error string depending on hResult.
 */
HRESULT CBlfLogConverter::GetErrorStatus(HRESULT hResult, string& omstrStatus)
{
    switch( hResult )
    {
        case S_OK:
            m_omstrConversionStatus = _("Conversion success");
            break;

        case S_FALSE:
            m_omstrConversionStatus = _("Conversion failed");
            break;

        default:
            m_omstrConversionStatus = _("Unknown");
            break;
    }

    return S_OK;
}

/**
 * \brief      Get input file filter type and name
 * \param[out] pchInputDefFilters file filter types
 * \param[out] pchInputFilters file filter name
 * \return     Result code
 *
 * Returns strings containing the file extensions and a
 * corresponding filter description.
 */
HRESULT CBlfLogConverter::GetInputFileFilters(string& pchInputDefFilters, string& pchInputFilters)
{
    pchInputDefFilters = "blf";
    pchInputFilters = _("CANoe Log File(s) (*.blf)|*.blf||");
    return S_OK;
}

/**
 * \brief      Get last conversion status
 * \param[out] hResult Last conversion status.
 * \param[out] omstrStatus String describing the last conversion status.
 * \return     Result code
 *
 * Returns a string containing the last conversion status.
 */
HRESULT CBlfLogConverter::GetLastConversionStatus(HRESULT& hResult, string& omstrStatus)
{
    hResult = m_hResult;
    omstrStatus = m_omstrConversionStatus;
    return S_OK;
}

/**
 * \brief      Get output file filter type and name
 * \param[out] pchOutputDefFilters file filter types
 * \param[out] pchOutputFilters file filter name
 * \return     Result code
 *
 * Returns strings containing the file extensions and a
 * corresponding filter description.
 */
HRESULT CBlfLogConverter::GetOutputFileFilters(string& pchOutputDefFilters, string& pchOutputFilters)
{
    pchOutputDefFilters = "log";
    pchOutputFilters = _("BUSMASTER Log File(s) (*.log)|*.log||");
    return S_OK;
}

/**
 * \brief      Write LOG file header to output stream, get the date and time when logging was started
 * \param      stream file filter types
 * \param      day Day (part of date)
 * \param      month Month number
 * \param      year Year
 * \param      hour Hour
 * \param      mins Minutes
 * \param      sec  Seconds
 *
 */
void CBlfLogConverter::AddFunctionHeader(std::ofstream& stream, WORD day, WORD month, WORD year, WORD hour
        , WORD mins, WORD sec) const
{
    stream << "***BUSMASTER Ver 2.6.1***\n";
    stream << "***PROTOCOL CAN***\n";
    stream << "***NOTE: PLEASE DO NOT EDIT THIS DOCUMENT***\n";
    stream << "***[START LOGGING SESSION]***\n";
    stream << "***START DATE AND TIME "<<day<<":"<<month
           << ":" << year << " "  << hour << ":" << mins << ":" << sec << ":000*** \n";
    stream << "***HEX***\n";
    stream << "***ABSOLUTE MODE***\n";
    stream << "***START DATABASE FILES (DBF/DBC)***\n";
    stream << "***END OF DATABASE FILES (DBF/DBC)***\n";
    stream << "***<Time><Tx/Rx><Channel><CAN ID><Type><DLC><DataBytes>***\n";
}

/**
 * \brief     Internal conversion function, writes given timestamp to output file stream
 * \param     timestamp timestamp for current message
 * \param     stream Output file stream
 *
 */
void CBlfLogConverter::WriteTimestamp(ULONGLONG timestamp, std::ofstream& stream) const
{
    // Be careful: in this function milliseconds uses for 10^-4 sec
    ULONGLONG hours = timestamp;
    hours /= 100000;
    ULONGLONG millisecond = hours % 10000;
    hours /= 10000;
    ULONGLONG seconds = hours % 60;
    hours /= 60;
    ULONGLONG minutes = hours % 60;
    hours /= 60;

    stream << std::dec << hours <<":" << minutes << ":" << seconds << ":" ;
    // Outputs milliseconds with adjusting by zero
    stream.setf(ios::right, ios::adjustfield);
    stream.fill('0');
    stream << std::setw(4)<< std::dec << millisecond;
}

/**
 * \brief     Internal conversion function, uses already prepared blf library and output file
 * \param     pBlfLib blf library with loaded input file
 * \param     stream Output file stream
 * \return    Result code
 *
 */
HRESULT CBlfLogConverter::WriteToLog(BLF::IBlfLibrary* pBlfLib, std::ofstream& stream) const
{
    if(pBlfLib == NULL)
    {
        return E_INVALIDARG;
    }

    SYSTEMTIME startTime = pBlfLib->GetStartTime();
    size_t objectsCount = pBlfLib->GetBlfObjectsCount();

    if(objectsCount <= 0)
    {
        return ERR_PROTOCOL_NOT_SUPPORTED;
    }

    AddFunctionHeader(stream, startTime.wDay, startTime.wMonth, startTime.wYear, startTime.wHour, startTime.wMinute, startTime.wSecond);

    for(size_t i = 0; i < objectsCount; i++)
    {
        BLF::IBlfObject* object = pBlfLib->GetBlfObject(i);
        if(object == NULL)
        {
            continue;
        }

        if(object->GetKind() == BLF::bokCanMessage)
        {
            BLF::ICanMessage* canMessage = object->GetICanMessage();
            // It is should be impossible, that object has CanMessage type, but return NULL for GetICanMessage()
            ASSERT(canMessage != NULL);
            if(canMessage == NULL)
            {
                continue;
            }

            WriteTimestamp(canMessage->GetTimestamp(), stream);

            if(canMessage->GetDirection()==BLF::mdRx)
            {
                stream << " Rx ";
            }
            else
            {
                stream << " Tx ";
            }
            stream << canMessage->GetChannelNo() << " ";
            // suppress extended message bit
            stream << std::hex << (canMessage->GetId() & 0x7FFFFFFF ) << " ";

            stream.setf(ios::uppercase);
            // checks extended bit
            if((canMessage->GetId() & 0x80000000) == 0)
            {
                stream << "s ";    // Standard message
            }
            else
            {
                stream << "x ";    // Extended message
            }
            stream << (int)canMessage->GetDLC();
            for (int i = 0 ; (i < canMessage->GetDLC()) ; ++i)
            {
                stream << " ";
                if (canMessage->GetData()[i] <= 0xf)
                {
                    stream << '0';
                }
                stream << std::hex;
                stream <<  (int)canMessage->GetData()[i];
                stream.setf(ios::uppercase);
            }
            stream << "\n";
        }
    }
    stream << "***END DATE AND TIME ***\n";
    // Everythere above \n is used, but here \r\n like in asc to log parser
    stream << "***[STOP LOGGING SESSION]***\r\n";

    return S_OK;
}

/**
 * \brief     Conversion function
 * \param[in] chInputFile Input file name to convert from
 * \param[in] chOutputFile Output file name to convert to
 * \return    Result code
 *
 * This is the actual conversion function with input and output file name.
 */
HRESULT CBlfLogConverter::ConvertFile(string& chInputFile, string& chOutputFile)
{
    HRESULT hResult = S_OK;

    try
    {
        bool isOk = true;

        // Create BLF Library interface
        BLF::IBlfLibrary* pBlfLib = BLF::GetIBlfLibrary();
        isOk = (NULL != pBlfLib);
        if (!isOk)
        {
            m_omstrConversionStatus = _("Unable to get BLF Library interface");
            m_hResult = ERR_UNABLE_TO_GET_LIB_INTERFACE;
            return ERR_UNABLE_TO_GET_LIB_INTERFACE;
        }

        // Load BLF file
        CBlfLibraryKeeper blfKeeper(pBlfLib);
        HRESULT hResult = pBlfLib->Load(chInputFile);

        if (hResult != S_OK)
        {
            m_omstrConversionStatus = _("Error: Invalid BLF file");
            m_hResult = hResult;
            return hResult;
        }

        std::ofstream fout(chOutputFile.c_str());
        // Let the output stream to throw exception on errors
        fout.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        if (!fout.is_open())
        {
            m_omstrConversionStatus = _("Output File path is not found");
            m_hResult = ERR_OUTPUT_FILE_NOTFOUND;
            return ERR_OUTPUT_FILE_NOTFOUND;
        }

        HRESULT hRes = WriteToLog(pBlfLib, fout);
        if (!SUCCEEDED(hRes))
        {
            if(hRes == ERR_PROTOCOL_NOT_SUPPORTED)
            {
                m_omstrConversionStatus = _("Error: No CAN data found for conversion.");
                m_hResult = ERR_PROTOCOL_NOT_SUPPORTED;
                return ERR_PROTOCOL_NOT_SUPPORTED;
            }
            else
            {
                m_omstrConversionStatus = _("Error: Unable to convert file.");
                m_hResult = ERR_OUTPUT_FILE_NOTFOUND;
                return ERR_OUTPUT_FILE_NOTFOUND;
            }
        }
    }
    catch (std::ifstream::failure& e)
    {
        // Process output stream errors (like not enough disk space)
        m_omstrConversionStatus = _("Error while writing output file.");
        m_hResult = E_FAIL;
        return E_FAIL;
    }
    catch (const std::exception& e)
    {
        // Process critical error
        m_omstrConversionStatus = e.what();
        m_hResult = E_FAIL;
        return E_FAIL;
    }

    m_omstrConversionStatus = _("Conversion Completed Successfully");
    m_hResult = hResult;
    return hResult;
}

/**
 * \brief     Returns if it has an own window
 * \return    True, if it has an own window.
 *
 * This returns true, if the converter has an own window, false otherwise.
 */
BOOL CBlfLogConverter::bHaveOwnWindow()
{
    return FALSE;
}



///////////////////////////////////////////////////
//CBlfLibraryKeeper

CBlfLibraryKeeper::CBlfLibraryKeeper(BLF::IBlfLibrary* pBlfLib)
    : m_pBlfLib(pBlfLib)
{
}

CBlfLibraryKeeper::~CBlfLibraryKeeper()
{
    if(m_pBlfLib != NULL)
    {
        m_pBlfLib->UnLoad();
    }
}


