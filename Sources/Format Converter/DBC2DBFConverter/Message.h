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
 * \brief     Definition of message class
 * \authors   Mahesh B S, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of the message class.
 */

#pragma once

/* C++ includes */
#include <string>

/* Project includes */
#include "Signal.h"

using namespace std;

class CMessage
{
public:
    int Format(char *pcLine);
    CMessage();
    virtual ~CMessage();
    CMessage& operator=(CMessage& message);
    // data members
    enum MSG_FRAME_FORMAT {MSG_FF_STANDARD = 'S', MSG_FF_EXTENDED = 'X'};

    string          m_acName;
    string          m_txNode;
    unsigned int    m_uiMsgID;
    unsigned int    m_ucLength;
    unsigned int    m_ucNumOfSignals;
    char            m_cDataFormat;
    char            m_cFrameFormat;
    list<CSignal>   m_listSignals;
    bool writeMessageToFile(fstream &fileOutput, list<CMessage> &m_listMessages, bool writeErr);
};
