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
 * \author    RBIN/EBS1 - Mahesh B S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CConverter class.
 */
/**
* \file       Converter.h
* \brief      CConverter class.
* \authors    Mahesh B S
* \date       14.11.2004
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/

#if !defined(AFX_CONVERTER_H__A66CF773_FD95_4EC8_AA35_8C230C34F8C2__INCLUDED_)
#define AFX_CONVERTER_H__A66CF773_FD95_4EC8_AA35_8C230C34F8C2__INCLUDED_
#include "Message.h"
#include "Parameter.h"
#include "ValueTable.h"
#include "Comment.h"
#include "ParameterVal.h"

#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CConverter
{
public:
    CConverter();
    virtual ~CConverter();

protected:
    static const char m_accHeader[];
public:
    unsigned int Convert(CString sCanoeFile,CString sCanMonFile);
    const char* GetResultString();
    static CStdioFile fileLog;
    static bool bLOG_ENTERED;
    static unsigned char ucMsg_DLC;
    CString m_omLogFilePath;
private:

    enum {CON_RC_NOERROR,CON_RC_COMPLETED_WITH_ERROR,CON_RC_FILEOPEN_ERROR_INFILE,
          CON_RC_FILEOPEN_ERROR_OUTFILE,CON_RC_FORMAT_ERROR_INFILE,CON_RC_FILEOPEN_ERROR_LOGFILE
         };

    unsigned int SetResultCode(unsigned int uiCode);
    void GenerateMessageList(CStdioFile& fileInput);
    void ValidateMessageList();
    bool WriteToOutputFile(CStdioFile& fileOutput);
    void CreateLogFile(CStdioFile &fileLog);
    void create_Node_List(char *);
    void DecryptData(CList<CString,CString& > &m_notProcessed);

    unsigned int m_uiResultCode;
    static const char *m_pacResultStrings[];
    CList<CString,CString& > m_notProcessed;
    CList<CString,CString& > m_listNode;
    CList<CMessage,CMessage&> m_listMessages;
    CList<CParameters,CParameters&> m_listParameterArray[6];
    CList<CValueTable,CValueTable&> m_vTab;
    CList<CComment,CComment&> m_listComments[4];

};

#endif // !defined(AFX_CONVERTER_H__A66CF773_FD95_4EC8_AA35_8C230C34F8C2__INCLUDED_)
