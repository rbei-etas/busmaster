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
 * \file      MessageAttrib.h
 * \brief     Interface for the CMessageAttrib class
 * \authors   RBIN/EMC2 - Ratnadip Choudhury, Krishnaswamy B.N.
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface for the CMessageAttrib class
 */

#pragma once

// Developer defined message parameters
#include "datatype.h"

const int MSGID_DUPLICATE       =   -101;
const int MSGID_NONEXISTENT     =   -102;
const int MSGID_ERROR_OPRN      =   -103;
const int ERR_INVALID_ADDRESS   =   -104;


typedef struct tagMsgIDAttr
{
    CString  omMsgIDName;
    COLORREF Colour;
} SMsgIDAttr;


class CMessageAttrib  
{
public:

    // To get its ID, given the message name 
    BOOL bMsgIDFromMsgName(const CString& omMsgName, UINT& unMsgID);
	//To get the Msg Name, from its ID
	BOOL bMessageNameFromMsgCode(UINT unMsgCode, CString& omMsgName);
    // To get total number of message entries
    int nGetTotalIDs(void);
    // To remove all message entries
    void vClearAllEntries();
    // To validate new message ID 
    int nValidateNewID(unsigned int unMsgID);
    // To get all message IDs and their attributes
    int nGetMsgAttribs(SCanIDList* psCanIDItem);
    // To get attribute of a particular message 
    int nGetAttrib(unsigned int unMsgID, SCanIDList& sCanIDItem);
    // To get all messages along with their attributes from registry
    void vSetMessageAttribData(PSMESSAGE_ATTRIB psMsgAttr);
    // To save all messages along with their attributes into registry
    void vSaveMessageAttribData(void);
    void vGetMessageAttribData(SMESSAGE_ATTRIB& sMsgAttr);
    // To confirm updation of message entries
    void vDoCommit();
    // To undo modification of message entries
    void vDoRollback();
    // To add a new message ID
    int nAddNewAttrib(SCanIDList sNewItem);
    // To remove a particular message ID
    int nRemoveAttrib(unsigned int unCanID);
    // To modify a particular message ID
    int nModifyAttrib(SCanIDList sNewItem);
    // To get message string of a particular message ID
    BOOL bGetCanIDName(unsigned int unCanID, CString& omMsgStr);
    // To get associated colour of a particular message ID
    COLORREF GetCanIDColour(unsigned int unCanID);
    // To get hold of instance of the singleton class
    static CMessageAttrib& ouGetHandle(ETYPE_BUS eBus);
    //To modify the format of the display depending on the mode
    void vChangeNumericalMode(BOOL bHexON);
    // Destructor
    virtual ~CMessageAttrib();

private:

    static CMessageAttrib sm_ouMsgAttrib[BUS_TOTAL];
    SMsgIDAttr m_sIDAttrTmp;
    CMap <UINT, UINT, SMsgIDAttr, SMsgIDAttr> m_omMsgIDMap;
    CMap <UINT, UINT, SMsgIDAttr, SMsgIDAttr> m_omMsgIDMapTmp;
	CList<UINT, UINT&> m_omIDList;
	CList<UINT, UINT&> m_omIDListTmp;
    CCriticalSection m_omCritSec;

    // Default constructor
    CMessageAttrib();
    // To copy message entries from one CMap object to the other
    void vMapCopy(CMap <UINT, UINT, SMsgIDAttr, SMsgIDAttr>& omDestMap, 
                  CMap <UINT, UINT, SMsgIDAttr, SMsgIDAttr>& omSrcMap);
    // To copy message entries from one CMap object to the other
    void vIDCopy(CList <UINT, UINT&>& omDestMap, 
                 CList <UINT, UINT&>& omSrcMap);
    // to store the format of display depending on the user mode
    char m_acMsgIDFormat[3];
};
