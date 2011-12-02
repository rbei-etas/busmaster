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
 * \file      Message.h
 * \brief     CMessage class.
 * \author    Mahesh B S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CMessage class.
 */

#if !defined(AFX_MESSAGE_H__F7C7DBC8_F82B_4799_80EF_1C7CDD5DCA42__INCLUDED_)
#define AFX_MESSAGE_H__F7C7DBC8_F82B_4799_80EF_1C7CDD5DCA42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxtempl.h>
#include "Signal.h"
//#include"definitions.h"
class CMessage
{
public:
    int Format(char *pcLine);
    CMessage();
    virtual ~CMessage();
    CMessage& operator=(CMessage& message);
    // data members
    enum MSG_FRAME_FROMAT {MSG_FF_STANDARD = 'S', MSG_FF_EXTENDED = 'X'};

    char			m_acName[defMSG_MAX_NAME_LEN];
    CString			m_txNode;
    unsigned int	m_uiMsgID;
    unsigned char	m_ucLength;
    unsigned char	m_ucNumOfSignals;
    char			m_cDataFormat;
    char			m_cFrameFormat;
    CList<CSignal,CSignal&> m_listSignals;
    bool writeMessageToFile( CStdioFile &fileOutput,CList<CMessage,CMessage&> &m_listMessages,bool writeErr);
};

#endif // !defined(AFX_MESSAGE_H__F7C7DBC8_F82B_4799_80EF_1C7CDD5DCA42__INCLUDED_)
