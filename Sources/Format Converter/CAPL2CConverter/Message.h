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
 * \brief     Interface file for CMessage class
 * \authors   Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CMessage class
 */

#pragma once

/* C++ includes */
#include <string>

/* Project includes */
#include "Signal.h"

#define defMSG_MAX_NAME_LEN     100 // I don't expect a name more than 100 bytes

class CMessage
{
public:
    int Format(char* pcLine);
    CMessage();
    virtual ~CMessage();
    // overload = operator
    CMessage& operator=(CMessage& message);
    CMessage(CMessage& message);

    // data members
    enum MSG_FRAME_FORMAT {MSG_FF_STANDARD = 'S', MSG_FF_EXTENDED = 'X'};

    string          m_acName;
    unsigned int    m_uiMsgID;
    unsigned char   m_ucLength;
    unsigned char   m_ucNumOfSignals;
    char            m_cDataFormat;
    char            m_cFrameFormat;
    CList<CSignal,CSignal&> m_listSignals;
};
