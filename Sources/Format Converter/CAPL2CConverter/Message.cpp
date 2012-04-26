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
 * \file      Message.cpp
 * \brief     Implementation file for CMessage class
 * \authors   Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CMessage class
 */

/* Project includes */
#include "Message.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMessage::CMessage()
{
    m_acName[0] = '\0';
    m_cDataFormat = CSignal::SIG_DF_INTEL;
    m_cFrameFormat = MSG_FF_STANDARD;
    m_ucLength = 8;
    m_ucNumOfSignals = 1;
    m_uiMsgID = 0;
    m_listSignals.RemoveAll();
}

CMessage::~CMessage()
{
    if(!m_listSignals.IsEmpty())
    {
        m_listSignals.RemoveAll();
    }
}

CMessage& CMessage::operator=(CMessage& message)
{
    // if there are some elements in the signal list clear them first
    if(!m_listSignals.IsEmpty())
    {
        m_listSignals.RemoveAll();
    }

    // now copy the other elements of the new message to this
    m_acName = message.m_acName;
    m_cDataFormat = message.m_cDataFormat;
    m_cFrameFormat = message.m_cFrameFormat;
    m_ucLength = message.m_ucLength;
    m_ucNumOfSignals = message.m_ucNumOfSignals;
    m_uiMsgID = message.m_uiMsgID;
    m_listSignals.AddTail(&message.m_listSignals);
    return (*this);
}

CMessage::CMessage(CMessage& message)
{
    // if there are some elements in the signal list clear them first
    if(!m_listSignals.IsEmpty())
    {
        m_listSignals.RemoveAll();
    }

    // now copy the other elements of the new message to this
    m_acName = message.m_acName;
    m_cDataFormat = message.m_cDataFormat;
    m_cFrameFormat = message.m_cFrameFormat;
    m_ucLength = message.m_ucLength;
    m_ucNumOfSignals = message.m_ucNumOfSignals;
    m_uiMsgID = message.m_uiMsgID;
    m_listSignals.AddTail(&message.m_listSignals);
}


// extract the message info from the line and store it in the Message object
// return an appropriate error code if something wrong with messageline
int CMessage::Format(char* pcLine)
{
    char* pcToken;
    char *pcNextToken;
    // get the MSG ID
    pcToken = strtok_s(pcLine, " :", &pcNextToken);
    m_uiMsgID = (unsigned int)atoi(pcToken);
    /*
    // set the id and frame format
    // canoe puts MSbit = 1 for extended ID
    if(msg.m_uiMsgID < 0x80000000UL)
    {
        msg.m_cFrameFormat = CMessage::MSG_FF_STANDARD;
    }
    else
    {
        msg.m_cFrameFormat = CMessage::MSG_FF_EXTENDED;
        msg.m_uiMsgID &= 0x7FFFFFFF;
    }
    */
    // get the message name
    pcToken = strtok_s(NULL, " :", &pcNextToken);
    m_acName = pcToken;
    // set the message length
    pcToken = strtok_s(NULL, " :", &pcNextToken);
    m_ucLength = (unsigned char)atoi(pcToken);
    // set the Data format
    m_cDataFormat = CSignal::SIG_DF_INTEL;
    // set the number of signals
    m_ucNumOfSignals = 0;
    return 1;
}
