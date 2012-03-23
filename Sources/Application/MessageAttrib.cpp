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
 * \file      MessageAttrib.cpp
 * \brief     Implementation of the CMessageAttrib class
 * \authors   RBIN/EMC2 - Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the CMessageAttrib class
 */

// For all standard header file include
#include "stdafx.h"
#include <string.h>
// Interface file for the application class
#include "BUSMASTER.h"
// Interface file for CMessageAttrib class
#include "MessageAttrib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


extern CCANMonitorApp theApp;

// Message Attribute singleton
CMessageAttrib CMessageAttrib::sm_ouMsgAttrib[BUS_TOTAL];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/******************************************************************************
 Function Name  :   CMessageAttrib

 Description    :   This is the default constructor
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   21-03-2002
 Modified by    :   Ratnadip Choudhury
 Date modified  :   30-04-2002
******************************************************************************/
CMessageAttrib::CMessageAttrib()
{
    // Initialise the hash table. For best performance, the hash table size 
    // should be a prime number. To minimize collisions the size should be 
    // roughly 20 percent larger than the largest anticipated data set.
    // Hence SIZE_HASH_TABLE
    m_omMsgIDMap.InitHashTable(SIZE_HASH_TABLE);
    m_omMsgIDMapTmp.InitHashTable(SIZE_HASH_TABLE);

}

/******************************************************************************
 Function Name  :   ~CMessageAttrib

 Description    :   This is the default destructor
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   21-03-2002
******************************************************************************/
CMessageAttrib::~CMessageAttrib()
{
    vClearAllEntries();
}

/******************************************************************************
 Function Name  :   ouGetHandle

 Description    :   This is a public static function which should be called to 
                    get hold of the singleton class.
 Input(s)       :   -
 Output         :   CMessageAttrib&
 Functionality  :   Returns reference to the only instance of the class
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   21-03-2002
******************************************************************************/
CMessageAttrib& CMessageAttrib::ouGetHandle(ETYPE_BUS eBus)
{
    return sm_ouMsgAttrib[eBus];
}

/******************************************************************************
 Function Name  :   nGetTotalIDs

 Input(s)       :   -
 Output         :   Number of entries in the data structure
 Functionality  :   Call to get number of entries in the data structure
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   21-03-2002
******************************************************************************/
int CMessageAttrib::nGetTotalIDs()
{
    return m_omMsgIDMap.GetCount();
}

/******************************************************************************
 Function Name  :   vRetrieveMessageAttribData

 Input(s)       :   -
 Output         :   -
 Functionality  :   Reads message attribute data from registry and stores them
                    in a data structure of MFC-CArray type
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   21-03-2002
 Modifications  :   13.11.2002, Gopi
                    Changed to usage of configuration file
******************************************************************************/
void CMessageAttrib::vSetMessageAttribData(PSMESSAGE_ATTRIB pMsgAttributes)
{
    m_omCritSec.Lock();

    vClearAllEntries();

    UINT unMsgID;
    if (pMsgAttributes != NULL)
    {
        int nMsgCount = pMsgAttributes->m_usMsgCount;
        PSMESSAGEATTR pNext = pMsgAttributes->m_psMsgAttribDetails;		

        for (int j = 0; j < nMsgCount; j++)
        {
            m_sIDAttrTmp.omMsgIDName = pNext->omStrMsgname;
            m_sIDAttrTmp.Colour = pNext->sColor;
            unMsgID = pNext->unMsgID;
            pNext++;
			
            // set the information into the map, the key is the Msg ID
            m_omMsgIDMap.SetAt(unMsgID, m_sIDAttrTmp);			
			//store Msg IDs
			m_omIDList.AddTail(unMsgID);
        }
        pMsgAttributes = NULL;
    }

    vMapCopy(m_omMsgIDMapTmp, m_omMsgIDMap);
	vIDCopy(m_omIDListTmp, m_omIDList);


    m_omCritSec.Unlock();
}

/******************************************************************************
 Function Name  :   vIDCopy
 Input(s)       :   omDestMap, omSrcMap of IDs
 Output         :   -
 Functionality  :   Copies the source ID map values into destination. 
 Member of      :   CMessageAttrib

 Author(s)      :   Arunkumar K
 Date Created   :   08-03-2011
 Modifications  :   
******************************************************************************/
void CMessageAttrib::vIDCopy(CList <UINT, UINT&>& omDestMap, 
							 CList <UINT, UINT&>& omSrcMap)
{
	UINT unMsgID;
	omDestMap.RemoveAll();
	

	POSITION psCurrPos = omSrcMap.GetHeadPosition();
    while (psCurrPos != NULL) 
    {
		unMsgID = omSrcMap.GetNext(psCurrPos);
		omDestMap.AddTail(unMsgID);
    }
}

/******************************************************************************
 Function Name  :   vSaveMessageAttribData

 Input(s)       :   -
 Output         :   -
 Functionality  :   Writes the registry to save message attribute data to from
                    the CArray structure. 
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   21-03-2002
 Modifications  :   13.11.2002, Gopi
                    Save the message attribute info into the config file
 Modifications  :   Anish,14.12.2006
                    Chenged to remove Dissociated DB messages
******************************************************************************/
void CMessageAttrib::vSaveMessageAttribData()
{
    m_omCritSec.Lock();
    // write into the configuration file
    SMESSAGE_ATTRIB sMsgAttribDetails;

	UINT unMsgCount = m_omMsgIDMap.GetCount();
    UINT unMsgID;

    // store info into the structure
    sMsgAttribDetails.m_usMsgCount = static_cast<USHORT>(unMsgCount);
    sMsgAttribDetails.m_psMsgAttribDetails = NULL;

    if (unMsgCount != 0)
    {
        // allocate the array
        PSMESSAGEATTR pMsgAttributes = new SMESSAGEATTR [unMsgCount];

        if (pMsgAttributes != NULL)
        {
            sMsgAttribDetails.m_psMsgAttribDetails = pMsgAttributes;
            PSMESSAGEATTR pNext = pMsgAttributes;
            //POSITION posNextPos = m_omMsgIDMap.GetStartPosition();
			POSITION posNextPos = m_omIDList.GetHeadPosition();		

            for (UINT j = 0; j < unMsgCount; j++)
            {
				//m_omMsgIDMap.GetNextAssoc(posNextPos, unMsgID, m_sIDAttrTmp);
				unMsgID = m_omIDList.GetNext(posNextPos);			
				m_omMsgIDMap.Lookup(unMsgID, m_sIDAttrTmp);

                pNext->omStrMsgname = m_sIDAttrTmp.omMsgIDName;
                pNext->sColor = m_sIDAttrTmp.Colour;
                pNext->unMsgID = unMsgID;

                pNext++;
            }
        }// end of if(pMsgAttributes != NULL)
    }// end of if(unMsgCount != 0)

    //theApp.bSetData(MSG_ATTRIBUTES, &sMsgAttribDetails);

    // release memory..
    if (sMsgAttribDetails.m_psMsgAttribDetails != NULL)
    {
        delete[] sMsgAttribDetails.m_psMsgAttribDetails;
        sMsgAttribDetails.m_usMsgCount = 0;
    }

    m_omCritSec.Unlock();
}
void CMessageAttrib::vGetMessageAttribData(SMESSAGE_ATTRIB& sMsgAttribDetails)
{
    m_omCritSec.Lock();

	UINT unMsgCount = m_omMsgIDMap.GetCount();
    UINT unMsgID;

    // store info into the structure
    sMsgAttribDetails.m_usMsgCount = static_cast<USHORT>(unMsgCount);
    sMsgAttribDetails.m_psMsgAttribDetails = NULL;

    if (unMsgCount != 0)
    {
        // allocate the array
        PSMESSAGEATTR pMsgAttributes = new SMESSAGEATTR [unMsgCount];

        if (pMsgAttributes != NULL)
        {
            sMsgAttribDetails.m_psMsgAttribDetails = pMsgAttributes;
            PSMESSAGEATTR pNext = pMsgAttributes;
            //POSITION posNextPos = m_omMsgIDMap.GetStartPosition();			
			POSITION posNextPos = m_omIDList.GetHeadPosition();			

            for (UINT j = 0; j < unMsgCount; j++)
            {
                //m_omMsgIDMap.GetNextAssoc(posNextPos, unMsgID, m_sIDAttrTmp);
				unMsgID = m_omIDList.GetNext(posNextPos);			
				m_omMsgIDMap.Lookup(unMsgID, m_sIDAttrTmp);

                pNext->omStrMsgname = m_sIDAttrTmp.omMsgIDName;
                pNext->sColor = m_sIDAttrTmp.Colour;
                pNext->unMsgID = unMsgID;

                pNext++;				
            }
        }// end of if(pMsgAttributes != NULL)
    }// end of if(unMsgCount != 0)

    m_omCritSec.Unlock();
}

/******************************************************************************
 Function Name  :   nAddNewAttrib

 Input(s)       :   sNewItem : New SCanIDList item to be added
 Output         :   Index of the item added if it is successful
 Functionality  :   Call to add a mew message attribute
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   21-03-2002
******************************************************************************/
int CMessageAttrib::nAddNewAttrib(SCanIDList sNewItem)
{
    int nResult = 0;

    // First check if this particular message ID exists
    if (m_omMsgIDMapTmp.Lookup(sNewItem.nCANID, m_sIDAttrTmp) == TRUE) 
    {
        nResult = MSGID_DUPLICATE;
    }
    else // This message ID is new
    {
        m_sIDAttrTmp.omMsgIDName = sNewItem.omCANIDName;
        m_sIDAttrTmp.Colour = sNewItem.Colour;

        m_omMsgIDMapTmp.SetAt(sNewItem.nCANID, m_sIDAttrTmp);
		m_omIDListTmp.AddTail(sNewItem.nCANID);
    }

    return nResult;
}

/******************************************************************************
 Function Name  :   nRemoveAttrib

 Input(s)       :   unCanID : The CanID of concern
 Output         :   Index of the CanID removed if successful
 Functionality  :   Call to remove a message ID
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   21-03-2002
******************************************************************************/
int CMessageAttrib::nRemoveAttrib(unsigned int unCanID)
{
    int nResult = 0;

    // If this message ID does not exist, send back an appropriate error code
    if (m_omMsgIDMapTmp.Lookup(unCanID, m_sIDAttrTmp) == FALSE) 
    {
        nResult = MSGID_NONEXISTENT;
    }
    else 
    {
        if (m_omMsgIDMapTmp.RemoveKey(unCanID) == FALSE) 
        {
            nResult = MSGID_ERROR_OPRN;
        }
		m_omIDListTmp.RemoveAt(m_omIDListTmp.Find(unCanID));
    }
    return nResult;
}


/******************************************************************************
 Function Name  :   nModifyAttrib

 Input(s)       :   SCanIDList (new data values with an existing CAN identifier)
 Output         :   index of the entry modified if successful, else 
                    MSGID_NONEXISTENT
 Functionality  :   Modifies attribute Reference to a user defined structure 
                    (type SCanIDList) to receive attribute data of a particular
                    CanID which is specified by nCANID member of the SCanIDList 
                    structure
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   21-03-2002
******************************************************************************/
int CMessageAttrib::nModifyAttrib(SCanIDList sNewItem)
{
    int nResult = nRemoveAttrib(sNewItem.nCANID);
    if (nResult == 0) 
    {
        nResult = nAddNewAttrib(sNewItem);
    }
    return nResult;
}

/******************************************************************************
 Function Name  :   vGetCanIDName

 Input(s)       :   unCanID - Message ID
                    CString& - The related message name
 Output         :   -
 Functionality  :   For a given message ID, renders the name associated.
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   21-03-2002
 Modified by    :   Ratnadip Choudhury
 Date modified  :   30-04-2002
 Modifications  :   Krishnaswamy B.N.                                   
                    23.05.2003, Changed formatting of display mode
 Modifications  :   Amitesh Bharti
                    08.03.2004,Changes due to message ID and name is displayed 
                    and logged together side. 
 Modifications  :   Raja N
                    23.06.2004, Modified the return type as BOOL. This will be
                    TRUE for successfullookup. Otherwise FASLE.
******************************************************************************/
BOOL CMessageAttrib::bGetCanIDName(unsigned int unMsgID, CString& omMsgStr)
{
    BOOL bFound = FALSE;
    if (m_omMsgIDMap.Lookup(unMsgID, m_sIDAttrTmp) == TRUE) 
    {
        bFound = TRUE;
        omMsgStr = defMSGID_NAME_DELIMITER;
        omMsgStr += m_sIDAttrTmp.omMsgIDName;
        omMsgStr += defMSG_NAME_END_CHAR;
    }
    return bFound;
}

/******************************************************************************
 Function Name  :   GetCanIDColour

 Input(s)       :   unCanID - Message ID
 Output         :   Colour assiciated to unCanID.
 Functionality  :   Returns colour code associated with a message identifier.
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   21-03-2002
 Modified by    :   Ratnadip Choudhury
 Date modified  :   30-04-2002
******************************************************************************/
COLORREF CMessageAttrib::GetCanIDColour(unsigned int unMsgID)
{
    static COLORREF Colour;

    if (m_omMsgIDMap.Lookup(unMsgID, m_sIDAttrTmp) == TRUE) 
    {
        Colour = m_sIDAttrTmp.Colour;
    }
    else 
    {
        Colour = BLACK_COLOR;
    }

    return Colour;
}

/******************************************************************************
 Function Name  :   vDoCommit

 Input(s)       :   - 
 Output         :   -
 Functionality  :   Confirms change in the message attribute list.
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   03-05-2002 
******************************************************************************/
void CMessageAttrib::vDoCommit()
{
    vMapCopy(m_omMsgIDMap, m_omMsgIDMapTmp);
	vIDCopy(m_omIDList, m_omIDListTmp);
}

/******************************************************************************
 Function Name  :   vDoRollback

 Input(s)       :   - 
 Output         :   -
 Functionality  :   Undoes change in the message attribute list.
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   03-05-2002 
******************************************************************************/
void CMessageAttrib::vDoRollback()
{
    vMapCopy(m_omMsgIDMapTmp, m_omMsgIDMap);
	vIDCopy(m_omIDListTmp, m_omIDList);
}

/******************************************************************************
 Function Name  :   vMapCopy

 Input(s)       :   omDestMap - Destination Map object.
                    omSrcMap - Map object which should be copied.
 Output         :   -
 Functionality  :   Copies a CMap object.
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   03-05-2002 
******************************************************************************/
void CMessageAttrib::vMapCopy(CMap <UINT, UINT, SMsgIDAttr, SMsgIDAttr>&
                                                                    omDestMap,
                           CMap <UINT, UINT, SMsgIDAttr, SMsgIDAttr>& omSrcMap)
{
    UINT unMsgID;

    omDestMap.RemoveAll();

    POSITION psCurrPos = omSrcMap.GetStartPosition();
    while (psCurrPos != NULL) 
    {
        omSrcMap.GetNextAssoc(psCurrPos, unMsgID, m_sIDAttrTmp);
        omDestMap.SetAt(unMsgID, m_sIDAttrTmp);
    }
}

/******************************************************************************
 Function Name  :   vClearAllEntries

 Input(s)       :   -
 Output         :   -
 Functionality  :   Removes all the entries both in the two map classes
 Member of      :   CMessageAttrib7

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   03-05-2002 
******************************************************************************/
void CMessageAttrib::vClearAllEntries()
{
    m_omCritSec.Lock();

    m_omMsgIDMap.RemoveAll();
    m_omMsgIDMapTmp.RemoveAll();
	m_omIDList.RemoveAll();
	m_omIDListTmp.RemoveAll();

    m_omCritSec.Unlock();
}

/******************************************************************************
 Function Name  :   bIsNewIDExisting

 Input(s)       :   unMsgID - New message ID to be validated
 Output         :   TRUE ID already exists, else FALSE
 Functionality  :   Validates a new message ID to check if it is already 
                    existing
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   03-05-2002 
******************************************************************************/
int CMessageAttrib::nValidateNewID(unsigned int unMsgID)
{
   int nResult = 0;
   if (m_omMsgIDMapTmp.Lookup(unMsgID, m_sIDAttrTmp) == TRUE) 
   {
       nResult = MSGID_DUPLICATE;
   }
   return nResult;
}

/******************************************************************************
 Function Name  :   nGetMsgAttribs

 Input(s)       :   psMsgIDList - Destination list of message attributes.
 Output         :   Zero if sucessful, else ERR_INVALID_ADDRESS.
 Functionality  :   Called to get message attributes.
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   03-05-2002 
******************************************************************************/
int CMessageAttrib::nGetMsgAttribs(SCanIDList* psMsgIDList)
{
    int nResult = 0, nCount = 0;
    SCanIDList *psMsgIDListTmp = NULL;
    UINT unMsgID;

    POSITION psCurrPos = m_omMsgIDMap.GetStartPosition();

    while ((psCurrPos != NULL) && (nResult == 0))
    {
        // Traverse through the map and save current key and associated entry
        // in unMsgID and m_sIDAttrTmp respectively.
        m_omMsgIDMap.GetNextAssoc(psCurrPos, unMsgID, m_sIDAttrTmp);

        // Indirect way to initialise destination message attribute list. The 
        // current destination is (psMsgIDList + nCount)
        psMsgIDListTmp = psMsgIDList + nCount;
        if (psMsgIDListTmp != NULL) 
        {
            psMsgIDListTmp->nCANID = unMsgID;
            psMsgIDListTmp->Colour = m_sIDAttrTmp.Colour;
            psMsgIDListTmp->omCANIDName = m_sIDAttrTmp.omMsgIDName;
            nCount++;
        }
        else 
        {
            nResult = ERR_INVALID_ADDRESS;
        }
    }

    return nResult;
}

/******************************************************************************
 Function Name  :   nGetAttrib

 Input(s)       :   unMsgID - Message ID whose attribute is to be fetched.
                    sCanIDItem - Where current message attribute is to be 
                    copied on.
 Output         :   Zero if successful, else MSGID_NONEXISTENT
 Functionality  :   Called to get attributes of a particular message ID.
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   03-05-2002 
******************************************************************************/
int CMessageAttrib::nGetAttrib(unsigned int unMsgID, SCanIDList& sCanIDItem)
{
    int nResult = 0;

    if (m_omMsgIDMapTmp.Lookup(unMsgID, m_sIDAttrTmp) == FALSE) 
    {
        nResult = MSGID_NONEXISTENT;
    }
    else 
    {
        sCanIDItem.nCANID = unMsgID;
        sCanIDItem.Colour = m_sIDAttrTmp.Colour;
        sCanIDItem.omCANIDName = m_sIDAttrTmp.omMsgIDName;
    }
    return nResult;
}

/******************************************************************************
 Function Name  :   bMsgIDFromMsgName

 Input(s)       :   omMsgName - Message name whose ID is to be retrieved
                    unMsgID - Reference to an UINT to get the message ID
 Output         :   TRUE if message name is found, else FALSE
 Functionality  :   Call this function to get the identifier of the message
                    from its name.
 Member of      :   CMessageAttrib

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   12-06-2002 
 Modification By:   Amitesh Bharti,
                    17.06.2003, m_acMsgIDFormat is used to format message ID
******************************************************************************/
BOOL CMessageAttrib::bMsgIDFromMsgName(const CString &omMsgName, UINT &unMsgID
                                       )
{
    BOOL bFound = FALSE;

    POSITION psCurrPos = m_omMsgIDMap.GetStartPosition();
    while ((psCurrPos != NULL) && (bFound == FALSE))
    {
        m_omMsgIDMap.GetNextAssoc(psCurrPos, unMsgID, m_sIDAttrTmp);
        if (m_sIDAttrTmp.omMsgIDName == omMsgName) 
        {
            bFound = TRUE;
        }
    }

    if (bFound == FALSE) 
    {
        if (sscanf((LPCTSTR) omMsgName, m_acMsgIDFormat, &unMsgID) == 1)
        {
            bFound = TRUE;
        }
    }

    return bFound;
}

/******************************************************************************
  Function Name    :  bMessageNameFromMsgCode
                                                                            
  Input(s)         :  UINT unMsgCode
  Output           :  TRUE or FALSE  
  Functionality    :  Returns TRUE if message name for message code unMsgCode 
                      is found,otherwise FALSE. The message name will be 
                      returned through omMsgName.
  Member of        :  CMessageAttrib                                        
  Friend of        :      -                                                 
                                                                            
  Author(s)        :  Arunkumar K
  Date Created     :  01.06.2011                                            
  Modifications    :  
******************************************************************************/
BOOL CMessageAttrib::bMessageNameFromMsgCode(UINT unMsgCode, CString& omMsgName)
{
    BOOL bResult = FALSE;
	if (unMsgCode >= 0)
	{
		if (m_omMsgIDMapTmp.Lookup(unMsgCode, m_sIDAttrTmp) == TRUE) 						
		{
			omMsgName = m_sIDAttrTmp.omMsgIDName;
			bResult = TRUE;
		}
	}
    return bResult;
}

/******************************************************************************
 Function Name  :   vChangeNumericalMode

 Input(s)       :   bHexON- Type of display 
                
 Output         :   void 
 Functionality  :   Call this function to format the string based on the mode.
 Member of      :   CMessageAttrib

 Author(s)      :   Krishnaswamy .B.N
 Date Created   :   24-05-2003 
******************************************************************************/
void CMessageAttrib::vChangeNumericalMode(BOOL bHexON)
{
    if(bHexON == TRUE)
    {
        strncpy(m_acMsgIDFormat, "%X", sizeof(m_acMsgIDFormat));
    }
    else
    {
        strncpy(m_acMsgIDFormat, "%d", sizeof(m_acMsgIDFormat));
    }
}
