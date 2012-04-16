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
 * \author    Mahesh.B.S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the CMessage class.
 */

/* Project includes */
#include "Converter.h"
#include "Message.h"

/**
 * constructor of CMessage
 */
CMessage::CMessage()
{

}

/**
 * destructor of CMessage
 */
CMessage::~CMessage()
{

}

/**
 * \brief operator= overloading
 *
 * Copy the other elements of the new message to this
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
 * Extracts the message data from the given Line and populates
 * the message structure.
 */
int CMessage::Format(char *pcLine)
{
    char* pcToken;
    // get the MSG Name
    pcToken = strtok(pcLine,",");
    m_sName = pcToken;

    // get the MSG ID
    pcToken = strtok(NULL," ,");
    m_uiMsgID = (unsigned int)atoi(pcToken);

    // set the message length
    pcToken = strtok(NULL," ,");
    m_ucLength = (unsigned int)atoi(pcToken);
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
        m_uiMsgID  = m_uiMsgID - 2147483648;
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
 * Overloading of == operator
 */
int CMessage::operator==(const CMessage &message) const
{
    if (message.m_uiMsgID == m_uiMsgID)
        return 1;
    else return 0;
}

/**
 * Overloading of == operator
 */
int CMessage::operator==(const unsigned int uiMsgID) const
{
    if (uiMsgID == m_uiMsgID)
        return 1;
    else return 0;
}

/**
 * Write's the message in the CANoe format and returns false
 * if any of the error signal is not stored in the file
 */
bool CMessage::writeMessageToFile( fstream &fileOutput)
{
    bool bResult = true;

	fileOutput << "BO_ " << dec << m_uiMsgID;
	fileOutput << " " << m_sName.c_str();
	fileOutput << ": " << dec << m_ucLength;
	fileOutput << " " << m_sTxNode.c_str() << endl;
    POSITION possig = m_listSignals.GetHeadPosition();
    while(possig != NULL)
    {
        CSignal& sig = m_listSignals.GetNext(possig);
        bResult = bResult & sig.WriteSignaltofile(fileOutput);
    }
    fileOutput << endl;
    return bResult;
}
