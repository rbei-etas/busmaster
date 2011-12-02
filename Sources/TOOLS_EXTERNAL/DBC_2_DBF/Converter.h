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
 * \brief     CConverter class.
 * \author    Mahesh B S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CConverter class.
 */

#if !defined(AFX_CONVERTER_H__A66CF773_FD95_4EC8_AA35_8C230C34F8C2__INCLUDED_)
#define AFX_CONVERTER_H__A66CF773_FD95_4EC8_AA35_8C230C34F8C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Message.h"
#include "Comment.h"
#include "ValueTable.h"
#include "Parameter.h"
#include "ParameterVal.h"

#include <utility>
using namespace std;
typedef struct pair<CString,CString> PAIR_CM;


class CConverter
{
public:
    CConverter();
    virtual ~CConverter();
    static unsigned char ucMsg_DLC;

protected:
    static const char m_accHeader[];
public:
    unsigned int Convert(CString sCanoeFile,CString sCanMonFile);

private:
    unsigned int m_uiResultCode;

    static const char *m_pacResultStrings[];
public:
    void CreateLogFile(CStdioFile& fileLog);
    bool WriteToOutputFile(CStdioFile& fileOutput);
    void GenerateMessageList(CStdioFile& fileInput);
    void ValidateMessageList();
    void EncryptData(CList<CString,CString& > &m_notProcessed);
    enum {CON_RC_NOERROR,CON_RC_COMPLETED_WITH_ERROR,CON_RC_FILEOPEN_ERROR_INFILE,
          CON_RC_FILEOPEN_ERROR_OUTFILE,CON_RC_FORMAT_ERROR_INFILE,CON_RC_FILEOPEN_ERROR_LOGFILE
         };
    unsigned int SetResultCode(unsigned int uiCode);
    const char* GetResultString();
    CList<CMessage,CMessage&> m_listMessages;
    CList<CMessage,CMessage&> m_unsupList;
    CList<CSignal,CSignal&> m_listSignal;
    CList<CComment,CComment&> m_cmMsg;
    CList<CComment,CComment&> m_cmNet;
    CList<CComment,CComment&> m_cmNode;
    CList<CComment,CComment&> m_cmSig;
    CList<CValueTable,CValueTable&> m_vTab;

    static bool valid_msg;
    CString m_omLogFilePath;
    void create_Node_List(char *);
    CList<CString,CString& > m_listNode;
    CList<CString,CString& > m_notProcessed;
    CList<CParameters,CParameters&> m_listParameters;
    CList<CParameters,CParameters&> m_listParameterArray[6];
    CList<CParameterValues,CParameterValues&> m_listParamValues[6];
    CList<CString,CString&>defList;

};

#endif // !defined(AFX_CONVERTER_H__A66CF773_FD95_4EC8_AA35_8C230C34F8C2__INCLUDED_)
