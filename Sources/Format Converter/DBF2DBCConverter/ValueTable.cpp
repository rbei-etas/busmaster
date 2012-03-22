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
 * \file      ValueTable.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
/*********************************************************************
Project       :  CANDb convertor
FileName      :  ValueTable.Cpp
Description   :  Implementation file for the value table.This is atable containing value discriptors.

$Log:   X:/Archive/Sources/Ext_tools_DBF_2_DBC/ValueTable.cpv  $

      Rev 1.1   04 Aug 2011 19:55:50   CANMNTTM


Rev 1.0   03 Aug 2011 15:47:52   rac2kor


Rev 1.0   17 Aug 2005 13:35:02   amb2kor
Initial Version.

Author(s)     :  RBIN/EBS1 - Padmaja.A
Date Created  :  16-11-2004
Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*********************************************************************/
/**
* \file       ValueTable.cpp
* \brief      Implementation file for the value table.
* \authors    Padmaja.A.
* \date       16.11.2004 Craeted
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/

#include "StdAfx.h"
#include <string.h>
#include "ValueTable.h"
#include "Signal.h"
#include "Converter.h"

/**
* \brief      Constructor of CValueTable
* \param[in]  None
* \param[out] None
* \return     None
* \authors    Padmaja.A.
* \date       16.11.2004
*/
CValueTable::CValueTable()
{
    m_TableName[0] = '\0';
}

/**
* \brief      destructor of CValueTable
* \param[in]  None
* \param[out] None
* \return     None
* \authors    Padmaja.A.
* \date       16.11.2004
*/
CValueTable::~CValueTable()
{

}

/**
* \brief      overloaded operator =
* \param[in]  CValueTable&
* \param[out] None
* \return     CValueTable&
* \authors    Padmaja.A.
* \date       16.11.2004
*/
CValueTable& CValueTable::operator=(CValueTable& Tab)
{
    strncpy(m_TableName, Tab.m_TableName, sizeof(m_TableName));
    POSITION posMsg = Tab.m_values.GetHeadPosition();
    while(posMsg != NULL)
    {
        CValueDescriptor& vDisp = Tab.m_values.GetNext(posMsg);
        m_values.AddTail(vDisp );
    }
    return (*this);
}

/**
* \brief      extracts Values and value descriptors from the line
* \param[in]  String having the value table name followed by the
value,value descriptor as in canoe format
* \param[out] None
* \return     void
* \authors    Padmaja.A.
* \date       16.11.2004
*/

void CValueTable::Format_ValueTable(char *pcLine,CStdioFile &fileInput)
{
    char *pcToken;
    char acLine[defCON_MAX_LINE_LEN] = {'\0'};
    //skip all balnk spaces.
    while(*pcLine && *pcLine==' ')
    {
        *pcLine++;
    }
    pcToken=strtok(pcLine,"\n");
    //get value table name.
    strncpy(m_TableName, pcToken, sizeof(m_TableName));
    //parses value table.
    while(strcmp((fileInput.ReadString(acLine,defCON_MAX_LINE_LEN)),"[END_TABLE]\n")!=0)
    {

        pcLine=acLine;
        //skip white spaces
        while(*pcLine && *pcLine==' ')
        {
            *pcLine++;
        }
        //get value descriptor.
        pcToken=strtok(pcLine," ");
        if(strcmp(pcToken,"[VALUE_DESCRIPTION]")==0)
        {
            pcToken=strtok(NULL,"\"");
            CValueDescriptor valDesc;
            //get the descriptor value.
            valDesc.m_sDescriptor =pcToken;
            if(    valDesc.m_sDescriptor.GetLength() > defCON_MAX_MSGN_LEN + 2)
            {
                char logmsg[defCON_MAX_LINE_LEN];
                sprintf(logmsg,"value Descriptor %s changed as %s\"\n",valDesc.m_sDescriptor,valDesc.m_sDescriptor.Left(defCON_MAX_MSGN_LEN));
                CConverter::fileLog.WriteString(logmsg);
                CConverter::bLOG_ENTERED = true;

                valDesc.m_sDescriptor = valDesc.m_sDescriptor.Left(defCON_MAX_MSGN_LEN);
                valDesc.m_sDescriptor += "\"";
            }

            pcToken=strtok(NULL,",");
            valDesc.m_value.i64Value = atoi(pcToken);
            m_values.AddTail(valDesc);
        }
    }

}

/**
* \brief      writes the value tebles in the given list to the output file.
* \param[in]  Pointer to the Output file,List of Value tables
* \param[out] None
* \return     int
* \authors    Padmaja.A.
* \date       17.11.2004
*/
void CValueTable::writeValueTabToFile(CStdioFile &fileOutput,CList<CValueTable,CValueTable&> &vTab)
{
    //get value table.
    POSITION pos = vTab.GetHeadPosition();
    char acLine[defCON_MAX_LINE_LEN];
    //repeats till value tables exists in the list.
    while(pos != NULL)
    {
        //write value table name to the o/p file.
        CValueTable &tab = vTab.GetNext(pos);
        sprintf(acLine,"VAL_TABLE_ %s ",tab.m_TableName);
        fileOutput.WriteString(acLine);
        //writes descriptors values to the o/p file.
        CValueDescriptor desc;
        desc.writeValuDescToFile (fileOutput,CSignal::SIG_TYPE_INT64,tab.m_values);
    }
}
