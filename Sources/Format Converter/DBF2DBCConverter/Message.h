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
 * \author    RBIN/EBS1 - Mahesh B S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CMessage class.
 */
/**
* \file       Message.h
* \brief      CMessage class.
* \authors    Mahesh B S
* \date       14.11.2004
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/

#pragma once

#include <afxtempl.h>
#include "Signal.h"

class CMessage
{
public:
    CMessage();
    virtual ~CMessage();
    int Format(char *pcLine);

    CMessage& operator=(CMessage& message);
    int operator==(const CMessage &message) const;
    int operator==(const unsigned int uiMsgID) const;
    bool writeMessageToFile( fstream &fileOutput);

    // data members
    enum MSG_FRAME_FORMAT {MSG_FF_STANDARD = 'S', MSG_FF_EXTENDED = 'X'};
    string			m_sName;
    string			m_sTxNode;
    unsigned int	m_uiMsgID;
    unsigned char	m_ucLength;
    char			m_cFrameFormat;
    CList<CSignal,CSignal&> m_listSignals;
};
