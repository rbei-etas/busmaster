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
 * \file      RefTimeKeeper.cpp
 * \brief     Implementation of CRefTimeKeeper class
 * \author    Anish kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CRefTimeKeeper class
 */

#include "CommonClass_stdafx.h"
#include "reftimekeeper.h"

/**
 * \brief Constructor
 *
 * Constructor
 */
CRefTimeKeeper::CRefTimeKeeper(void)
{
    m_qwRefSysTime  = 0;
    m_qwAbsBaseTime = 0;
}

/**
 * \brief Destructor
 *
 * Destructor
 */
CRefTimeKeeper::~CRefTimeKeeper(void)
{
}

/**
 * \brief Set Time Parameters
 *
 * Save current system time and connection time
 */
void CRefTimeKeeper::vSetTimeParams(SYSTEMTIME& CurrSysTime, UINT64 qwConnectTime)
{
    m_qwRefSysTime = (CurrSysTime.wHour * 3600 + CurrSysTime.wMinute * 60 +
                      + CurrSysTime.wSecond) * 10000 + CurrSysTime.wMilliseconds * 10;
    m_qwAbsBaseTime = qwConnectTime;
}

//void CRefTimeKeeper::vGetTimeParams(UINT64& qwRefSysTime, UINT64& qwAbsBaseTime)
//{
//    qwRefSysTime  = m_qwRefSysTime;
//    qwAbsBaseTime = m_qwAbsBaseTime;
//}
