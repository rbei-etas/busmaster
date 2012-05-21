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
 * \file      Struct.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "stdafx.h"
#include "Struct.h"

sSIMSYSARRAY::sSIMSYSARRAY()
{
    psNextSimsys = NULL;
}
/*******************************************************************************
 Function Name  : operator=
 Functionality  : Copies the contents of the destination(Param) linked list into
                  the source(Member) linked list. Source linked list will be
                  reduced or increased if necessary.
 Member of      : sSIMSYSARRAY
 Author(s)      : Pradeep Kadoor.
 Date Created   : 28.05.2009
*******************************************************************************/
sSIMSYSARRAY& sSIMSYSARRAY::operator=(const sSIMSYSARRAY& RefObj)
{
    m_omStrSimSysPath = RefObj.m_omStrSimSysPath;
    m_omStrSimSysName = RefObj.m_omStrSimSysName;

    if (RefObj.psNextSimsys != NULL)
    {
        if (psNextSimsys == NULL)
        {
            psNextSimsys = new sSIMSYSARRAY;
        }

        *(psNextSimsys) = *(RefObj.psNextSimsys);
    }
    else if ((RefObj.psNextSimsys == NULL) && (psNextSimsys != NULL))
    {
        sSIMSYSARRAY* psTemp = NULL;

        while (psNextSimsys != NULL)
        {
            psTemp = psNextSimsys;
            psNextSimsys = psNextSimsys->psNextSimsys;
            delete psTemp;
            psTemp = NULL;
        }
    }

    return *this;
}
