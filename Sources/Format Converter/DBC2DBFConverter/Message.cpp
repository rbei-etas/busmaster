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
 * \brief     Implementation of message class
 * \author    Mahesh B S, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the message class.
 */

#include "Converter.h"
#include "Message.h"
#include "Tag.h"


/**
 * \brief Constructor
 */
CMessage::CMessage()
{
    m_acName = "";
    m_txNode = "";
    m_cDataFormat = CSignal::SIG_DF_INTEL;
    m_cFrameFormat = MSG_FF_STANDARD;
    m_ucLength = 8;
    m_ucNumOfSignals = 1;
    m_uiMsgID = 0;
    m_listSignals.clear();
}


/**
 * \brief Destructor
 */
CMessage::~CMessage()
{
    m_listSignals.clear();
}


/**
 * \brief overloaded operator =
 * \param[in] message Message to copy content from
 *
 * Copy the elements of the given message to this
 */
CMessage& CMessage::operator=(CMessage& message)
{
    m_acName = message.m_acName;
    m_txNode = message.m_txNode;
    m_cDataFormat = message.m_cDataFormat;
    m_cFrameFormat = message.m_cFrameFormat;
    m_ucLength = message.m_ucLength;
    m_ucNumOfSignals = message.m_ucNumOfSignals;
    m_uiMsgID = message.m_uiMsgID;
    m_listSignals = message.m_listSignals;
    return (*this);
}


/**
 * \brief Extracts the message data
 *
 * Extracts the message data from the given Line and populates
 * the message structure.
 */
int CMessage::Format(char *pcLine)
{
    char *pcToken, *pcTok;
    // get the MSG ID
    pcToken = strtok_s(pcLine, " :", &pcTok);
    m_uiMsgID = strtoul(pcToken, NULL, 0);

    // get the message name
    pcToken = strtok_s(NULL, " :", &pcTok);
    m_acName = pcToken;

    // set the message length
    pcToken = strtok_s(NULL, " :", &pcTok);
    m_ucLength = (unsigned char)atoi(pcToken);
    CConverter::ucMsg_DLC = m_ucLength;

    //get the Tx'ing Node Name
    pcToken = strtok_s(NULL, " :", &pcTok);
    if(strcmp(pcToken, "Vector__XXX"))
        m_txNode = pcToken;
    else
        m_txNode = "";

    // set the Data format
    m_cDataFormat = CSignal::SIG_DF_INTEL;

    // set the number of signals
    m_ucNumOfSignals = 0;

    return 1;
}


/**
 * \brief      writes the Messages in the given list to the output file
 * \param[in]  fileOutput Pointer to the Output file
 * \param[in]  m_listMessages List of Message
 * \param[in]  writeErr If true write error signals also else write only correct signals
 *             associated with the message
 *
 * Writes the Messages in the given list to the output file.
 */
bool CMessage::writeMessageToFile(fstream &fileOutput, list<CMessage> &m_listMessages, bool writeErr)
{
    bool bResult = true;

    //Write all the message
    list<CMessage>::iterator msg;
    for(msg=m_listMessages.begin(); msg!=m_listMessages.end(); ++msg)
    {
        fileOutput << T_START_MSG;
        fileOutput << " ";
        fileOutput << msg->m_acName.c_str();
        fileOutput << ",";
        fileOutput << dec << msg->m_uiMsgID;
        fileOutput << ",";
        fileOutput << dec << (int) msg->m_ucLength;
        fileOutput << ",";
        fileOutput << dec << (int) msg->m_ucNumOfSignals;
        fileOutput << ",";
        fileOutput << msg->m_cDataFormat;
        fileOutput << ",";
        fileOutput << msg->m_cFrameFormat;
        fileOutput << ",";
        fileOutput << msg->m_txNode.c_str();
        fileOutput << endl;

        CSignal sig;
        //write all related signals to the messages
        bResult &= sig.WriteSignaltofile(fileOutput,
                                         msg->m_listSignals,
                                         msg->m_ucLength,
                                         msg->m_cDataFormat,
                                         writeErr);
        fileOutput << T_END_MSG << endl;
        fileOutput << endl;
    }
    return bResult;
}
