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
 * \file      StdAfx.cpp
 * \brief     Implementation of the common functions.
 * \authors   RBIN/EBS1 - Mahesh.B.S, Padmaja.A
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the common functions.
 */
/**
* \file       stdafx.Cpp
* \brief      Implementation of the common functions.
* \authors    Mahesh.B.S, Padmaja.A
* \date       15.11.2004
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/
#include "StdAfx.h"
#include "Converter.h"

/**
* \brief      This function will truncate any given string to 32 char.
* \param[in]  char *type,char *pcToken,bool bToLog
* \param[out] None
* \return     void
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
extern void Truncate_str(char *type,char *pcToken,bool bToLog)
{
    string strTemp = pcToken;
    char acLine[100];

    if(bToLog)
    {
        sprintf(acLine,"%s %s ",type,pcToken);
        CConverter::fileLog.WriteString(acLine);
        CConverter::bLOG_ENTERED = true;
    }
    strcpy(pcToken,strTemp.c_str());
    if(bToLog)
    {
        sprintf(acLine," changed to %s\n",pcToken);
        CConverter::fileLog.WriteString(acLine);
    }
    return;
}

/**
* \brief      This function will truncate any given string to 32 char.
* \param[in]  char *type,char *pcToken,bool bToLog
* \param[out] None
* \return     void
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
extern void Truncate_str(char *type,string &strTemp,bool bToLog)
{

    char acLine[100];
    if(bToLog)
    {
        sprintf(acLine,"%s %s ",type,strTemp);
        CConverter::fileLog.WriteString(acLine);
        CConverter::bLOG_ENTERED = true;
    }

    if(bToLog)
    {
        sprintf(acLine," changed to %s\n",strTemp.c_str());
        CConverter::fileLog.WriteString(acLine);
    }
    return;
}
