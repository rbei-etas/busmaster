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

#ifndef Converter_H
#define Converter_H

#include <list>
#include "../../Utility/utility.h"

#include "DBFMessage.h"
#include "DBFComment.h"
#include "DBFValueTable.h"
#include "DBFParameter.h"
#include "DBFParameterValues.h"
#include "DBCConverterBase.h"

using namespace std;

class CDBFConverter:public CDBCConverterBase
{
	eBUS_TYPE m_eBus;
public:
	CDBFConverter(eBUS_TYPE eBus);
    
    HRESULT LoadDBCFile(string strDBCFile);
    HRESULT GenerateImportList(/* sMESSAGE*& */);
    HRESULT ConvertFile(string strDBFFile);
    HRESULT FindMessage(string strMsgName, CMessage& );
    HRESULT FindMessage(UINT unMsgId, CMessage& );
    HRESULT FindSignalAlias(string& strMsgName, string& strSignalName, string& strSignalAlias);
    HRESULT GetResultString(char*);
    HRESULT ClearMsgList();
	HRESULT GetMessageNameList(std::list<string>& meassageList);
    virtual ~CDBFConverter();
    
    static unsigned char ucMsg_DLC;
private:
    string strDBCFileName;
    //static const char m_accHeader[];
    unsigned int m_uiResultCode;
    static const char *m_pacResultStrings[];
    unsigned int Convert(string sCanoeFile, string sCanMonFile);
    bool bGetSignalAlias(CMessage& ouMsg, string& strSignalName, string& strSignalAlias);
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
    BOOL bHeaderSection;
    static bool valid_msg;
    string m_omLogFilePath;
    void create_Node_List(char *);
    list<string> m_listNode;
    list<string> m_notProcessed;
    list<CParameter> m_listParameters;
    list<CParameter> m_listParameterArray[6];
    list<CParameterValues> m_listParamValues[6];
    list<string> defList;
};

#endif
