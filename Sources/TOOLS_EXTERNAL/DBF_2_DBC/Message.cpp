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
 * \brief     Implementation of the CMessage class.
 * \author    RBIN/EBS1 - Mahesh.B.S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the CMessage class.
 */
/**
* \file       Message.cpp
* \brief      Implementation file for the Message class.
* \authors    Mahesh B S
* \date       15-11-2004 Created
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/

#include "StdAfx.h"
#include "Message.h"
#include "Converter.h"

/**
* \brief      constructor of CMessage
* \param[in]  None
* \param[out] None
* \return     None
* \authors    Mahesh.B.S
* \date       15.11.2002
*/
CMessage::CMessage()
{

}

/**
* \brief      destructor of CMessage
* \param[in]  None
* \param[out] None
* \return     None
* \authors    Mahesh.B.S
* \date       15.11.2002
*/
CMessage::~CMessage()
{

}

/**
* \brief      operator= overloading
* \param[in]  CMessage& message
* \param[out] None
* \return     CMessage& CMessage
* \authors    Mahesh.B.S
* \date       15.11.2002
*/
CMessage& CMessage::operator=(CMessage& message)
{
    // if there are some elements in the signal list clear them first
    if(!m_listSignals.IsEmpty())
    {
        m_listSignals.RemoveAll();
    }

    // now copy the other elements of the new message to this
    m_sName = message.m_sName;
    m_sTxNode = message.m_sTxNode;
    m_cFrameFormat = message.m_cFrameFormat;
    m_ucLength = message.m_ucLength;
    m_uiMsgID = message.m_uiMsgID;
    m_listSignals.AddTail(&message.m_listSignals);
    return (*this);
}

/**
* \brief      Extracts the message data from the given Line and populates
the message structure.
* \param[in]  char *pcLine
* \param[out] None
* \return     int
* \authors    Mahesh.B.S
* \date       15.11.2002
*/

int CMessage::Format(char *pcLine)
{
    char* pcToken;
    // get the MSG Name
    pcToken = strtok(pcLine,",");
    m_sName = pcToken;
    if(m_sName.GetLength() > defCON_MAX_MSGN_LEN)
        Truncate_str("Message name",m_sName,true);

    // get the MSG ID
    pcToken = strtok(NULL," ,");
    m_uiMsgID = (unsigned int)atoi(pcToken);

    // set the message length
    pcToken = strtok(NULL," ,");
    m_ucLength = (unsigned char)atoi(pcToken);
    CConverter::ucMsg_DLC = m_ucLength;
    //no.. of signals.
    pcToken = strtok(NULL," ,");

    //data format.
    pcToken = strtok(NULL," ,");

    //frame format.
    pcToken = strtok(NULL," ,");
    m_cFrameFormat = pcToken[0];
    if(strcmp(pcToken,"X") == 0)
    {
        m_uiMsgID  = m_uiMsgID + 2147483648;
    }

    //get the Tx'ing Node Name
    pcToken = strtok(NULL,"\n");
    if(pcToken)
    {
        m_sTxNode = pcToken;
    }
    else
    {
        m_sTxNode = "Vector__XXX";
    }

    return 1;
}

/**
* \brief      Overloading of == operator
* \param[in]  const CMessage &message
* \param[out] None
* \return     int
* \authors    Mahesh.B.S
* \date       15.11.2002
*/
int CMessage::operator==(const CMessage &message) const
{
    if (message.m_uiMsgID == m_uiMsgID)
        return 1;
    else return 0;
}

/**
* \brief      Overloading of == operator
* \param[in]  const CMessage &message
* \param[out] None
* \return     int
* \authors    Mahesh.B.S
* \date       15.11.2002
*/
int CMessage::operator==(const unsigned int uiMsgID) const
{
    if (uiMsgID == m_uiMsgID)
        return 1;
    else return 0;
}

/**
* \brief      Write's the message in the CANoe format and returns false
if any of the error signal is not stored in the file
* \param[in]  CStdioFile &fileOutput
* \param[out] None
* \return     bool
* \authors    Mahesh.B.S
* \date       15.11.2002
*/

bool CMessage::writeMessageToFile( CStdioFile &fileOutput)
{
    bool bResult = true;
    char acLine[defCON_MAX_LINE_LEN];

    sprintf(acLine,"BO_ %u %s: %u %s\n",m_uiMsgID,m_sName,m_ucLength,m_sTxNode);
    fileOutput.WriteString(acLine);
    POSITION possig = m_listSignals.GetHeadPosition();
    while(possig != NULL)
    {
        CSignal& sig = m_listSignals.GetNext(possig);
        bResult = bResult & sig.WriteSignaltofile(fileOutput);
    }
    fileOutput.WriteString("\n");
    return bResult;
}

