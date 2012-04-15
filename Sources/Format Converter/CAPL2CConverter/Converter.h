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
 * \file      Converter.h
 * \brief     Interface file for CConverter class
 * \author    Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CConverter class
 */

#pragma once

#include "Message.h"

class CConverter
{
public:
    CConverter();
    virtual ~CConverter();

protected:
    static const char m_accHeader[];
public:
    unsigned int Convert(CString sCanoeFile);

private:
    unsigned int m_uiResultCode;
    static const char *m_pacResultStrings[];
public:
    CMessage ouFindMessage(UINT nMsgID);
    CMessage ouFindMessage(CString omStrMsgName);
    void CreateLogFile(CStdioFile& fileLog);
    bool WriteToOutputFile(CString fileOutput);
    void GenerateMessageList(CStdioFile& fileInput);
    void ValidateMessageList();
    enum {CON_RC_NOERROR,CON_RC_COMPLETED_WITH_ERROR,CON_RC_FILEOPEN_ERROR_INFILE,
          CON_RC_FILEOPEN_ERROR_OUTFILE,CON_RC_FORMAT_ERROR_INFILE,CON_RC_FILEOPEN_ERROR_LOGFILE
         };
    unsigned int SetResultCode(unsigned int uiCode);
    const char* GetResultString();
    CList<CMessage,CMessage&> m_listMessages;

};
