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
#include "RefTimeKeeper.h"


UINT64 CRefTimeKeeper::m_qwRefSysTime  = 0;
UINT64 CRefTimeKeeper::m_qwAbsBaseTime = 0;

void CRefTimeKeeper::vSetTimeParams(SYSTEMTIME& currentSystemTime, UINT64 connectTime)
{
    m_qwRefSysTime = (currentSystemTime.wHour * 3600 + currentSystemTime.wMinute * 60 +
                      + currentSystemTime.wSecond) * 10000 + currentSystemTime.wMilliseconds * 10;
    m_qwAbsBaseTime = connectTime;
}

void CRefTimeKeeper::vGetTimeParams(UINT64& referenceSystemTime_, UINT64& absoluteBaseTime_)
{
    referenceSystemTime_ = m_qwRefSysTime;
    absoluteBaseTime_ = m_qwAbsBaseTime;
}
//}