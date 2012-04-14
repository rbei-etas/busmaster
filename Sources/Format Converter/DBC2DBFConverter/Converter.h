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
 * \brief     Definition of converter class
 * \author    Mahesh B S, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of the converter class.
 */

#pragma once

#include <list>
#include <utility>

#include "Message.h"
#include "Comment.h"
#include "ValueTable.h"
#include "Parameter.h"
#include "ParameterVal.h"

using namespace std;

class CConverter
{
public:
    CConverter();
    virtual ~CConverter();
    static unsigned char ucMsg_DLC;

protected:
    static const char m_accHeader[];
public:
    unsigned int Convert(string &sCanoeFile, string &sCanMonFile);

private:
    unsigned int m_uiResultCode;

public:
    void CreateLogFile(fstream& fileLog);
    bool WriteToOutputFile(fstream& fileOutput);
    void GenerateMessageList(fstream& fileInput);
    void ValidateMessageList();
    void EncryptData(list<string> &m_notProcessed);
    enum {CON_RC_NOERROR,CON_RC_COMPLETED_WITH_ERROR,CON_RC_FILEOPEN_ERROR_INFILE,
          CON_RC_FILEOPEN_ERROR_OUTFILE,CON_RC_FORMAT_ERROR_INFILE,CON_RC_FILEOPEN_ERROR_LOGFILE
         };
    unsigned int SetResultCode(unsigned int uiCode);
    void GetResultString(string& str);
    list<CMessage> m_listMessages;
    list<CMessage> m_unsupList;
    list<CSignal> m_listSignal;
    list<CComment> m_cmMsg;
    list<CComment> m_cmNet;
    list<CComment> m_cmNode;
    list<CComment> m_cmSig;
    list<CValueTable> m_vTab;

    static bool valid_msg;
    string m_omLogFilePath;
    void create_Node_List(char *);
    list<string> m_listNode;
    list<string> m_notProcessed;
    list<CParameters> m_listParameters;
    list<CParameters> m_listParameterArray[6];
    list<CParameterValues> m_listParamValues[6];
    list<string> defList;
};
