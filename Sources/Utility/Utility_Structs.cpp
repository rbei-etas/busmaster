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
 * \file      Utility_Structs.cpp
 * \brief     This file contains implementations of utility structures
 * \authors   Ravi D., Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains implementations of utility structures
 */

#include "Utils_stdafx.h"
#include "Utility_Structs.h"

/* sListInfo --- STARTS */
sListInfo::sListInfo()
{
}

sListInfo::sListInfo(sListInfo& sCopy)
{
    m_eType = sCopy.m_eType;
    m_omEntries.Copy(sCopy.m_omEntries);
}

void sListInfo::operator = (sListInfo& sCopy)
{
    m_eType = sCopy.m_eType;
    m_omEntries.Copy(sCopy.m_omEntries);
}
/* sListInfo --- ENDS */
