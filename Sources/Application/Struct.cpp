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
 * @author Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#include "stdafx.h"
#include "Struct.h"

sSIMSYSARRAY::sSIMSYSARRAY()
{
    psNextSimsys = nullptr;
}

sSIMSYSARRAY& sSIMSYSARRAY::operator=(const sSIMSYSARRAY& RefObj)
{
    m_omStrSimSysPath = RefObj.m_omStrSimSysPath;
    m_omStrSimSysName = RefObj.m_omStrSimSysName;
    if (RefObj.psNextSimsys != nullptr)
    {
        if (psNextSimsys == nullptr)
        {
            psNextSimsys = new sSIMSYSARRAY;
        }
        *(psNextSimsys) = *(RefObj.psNextSimsys);
    }
    else if ((RefObj.psNextSimsys == nullptr) && (psNextSimsys != nullptr))
    {
        sSIMSYSARRAY* psTemp = nullptr;
        while (psNextSimsys != nullptr)
        {
            psTemp = psNextSimsys;
            psNextSimsys = psNextSimsys->psNextSimsys;
            delete psTemp;
            psTemp = nullptr;
        }
    }
    return *this;
}
