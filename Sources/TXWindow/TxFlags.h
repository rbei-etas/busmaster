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
 * @brief Contains CFlags class definition
 * @author Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains CFlags class definition
 */

#pragma once

/** enumeration for all flags */
typedef enum eTXWNDFLAG
{
    TX_HEX,
    TX_SENDMESG,
    TX_CONNECTED
};

class CFlags
{
public:
    CFlags();
    virtual ~CFlags();

    /**
     * This function returns the state of flag The eWhichFlag
     * identified the flag.
     *
     * @param[in] WhichFlag Flag identifer
     * @return State of flag
     */
    int nGetFlagStatus(eTXWNDFLAG WhichFlag);

    /**
     * This function set the correspoding flag whose
     * with value passed as nValue parameter.The eWhichFlag
     * identified the flag to be set.
     *
     * @param[in] WhichFlag Flag identifer
     * @param[in] nValue value to set that flag
     */
    void vSetFlagStatus(eTXWNDFLAG WhichFlag, int nValue);

private:
    BOOL m_bHex;
    BOOL m_bSendMsgOn;
    BOOL m_bConnected;

    CCriticalSection m_omFlagCritSec;
};
