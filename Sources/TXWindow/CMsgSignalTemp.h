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
 * \file      CMsgSignalTemp.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "include/BaseDefs.h"
#include "Datatypes/MsgSignal_Datatypes.h"



class CMsgSignal
{
public:
    CMsgSignal();
    virtual ~CMsgSignal();

public:
    struct sDBFileStruct
    {
        CString m_omStrDatabasePath; // Database path
        UINT m_unMessageCount;       // Message count in the database
        sMESSAGE* m_psMessages;      // Pointer to the array of message structure
        sDBFileStruct* m_psNextDBFileStruct;
    };

private:
    SDBPARAMS m_sDbParams;
    // Attributes
    CStringArray m_omDatabaseNames;
    //This data member will contain number of messages of active database.
    UINT m_unMessageCount;
    //This data member will contain pointer to sMESSAGE structure that holds active database information.
    sMESSAGE* m_psMessages;
    // This flag indicates if the database is ACTIVE/IN_ACTIVE
    BOOL m_bIsDatabaseActive;
    CString m_strActiveDBFileName;
    //This is pointer to the list of database structures
    sDBFileStruct* m_psDatbaseStructList;
    //CMap to store the all the messages of all the DBs for faster searches
    CMap <UINT, UINT, sMESSAGE*, sMESSAGE*> m_omMsgDetailsIDMap;
    //For shared CMap access and deletion
    CCriticalSection m_omDBMapCritSec;
    // Functions
public:
    //Reset the content of CMap on DB import or dissociate
    void vResetMsgMapContent();
    // Checks if the signal value is out of range
    BOOL bIsSignalValueOutofRange( CString omStrMessageCode,
                                   CString omStrSgName,
                                   CString omStrSignalValue);
    // Checks if the Signal value is duplicate
    BOOL bIsDuplicateSignalValue( CString omStrMessageCode,
                                  CString omStrSgName,
                                  CString omStrSignalValue,
                                  CString omStrDescription,
                                  INT nIndex,
                                  BOOL bMode);
    // Checks if the description is duplicate
    BOOL bIsDuplicateValueDescription( CString omStrMessgaeCode,
                                       CString omStrSgName,
                                       CString omStrSignalValue,
                                       CString omStrDescription,
                                       INT nIndex,
                                       BOOL bMode);
    // Returns the signal names and their indexes
    // to be deleted which are defined after
    // the specified message length
    void vGetSigNamesAndIndexTobeDeleted( UINT unMessageLength,
                                          CString omStrMsgName,
                                          CStringArray& omStrList,
                                          UINT unIndexes[]);
    void unListGetMessageIDs(UINT* omListId);
    // Returns whether a signal can have new
    // Signal description and value
    BOOL bItemCanHaveSignalDesc( CString omStrMsgName,
                                 CString omStrSgName,
                                 unsigned __int64 un64ItemCount );
    // Updates signal matrix
    void vUpdateSignalMatrix(const BYTE* pSrcByte,
                             BYTE* pDestByte,
                             UINT unArrayLen,
                             BOOL bUpdateType);


    // Returns the value of "m_bIsDatabaseSaved" flag
    BOOL bGetModifiedFlag();

    // Gives a list of message names
    void omStrListGetMessageNames(CStringList& );
    // Gives a list of message names for editing database
    void omStrListGetMessageNamesInactive(CStringList& );

    // Set and get active DB flag status
    void vSetDBActiveFlagStatus(BOOL bValue);
    BOOL bGetDBAcitveFlagStatus();

    // Get message pointer for given message
    //sMESSAGE* psGetMessagePointer( CString strMsgName);
    // Get message pointer for given message if called to change inactive Db
    sMESSAGE* psGetMessagePointerInactive( CString strMsgName);

    // Get message pointer for given message ID
    sMESSAGE* psGetMessagePointer( UINT unMsgID);
    // Get message pointer for given message ID for inactive DB
    sMESSAGE* psGetMessagePointerInactive( UINT unMsgID);
    // Convert Extended To Standard Frame Format
    void vConvertExtendedToStandardFrameFormat(int&);

    // Convert Standard To Extended Frame Format
    void vConvertStandardToExtendedFrameFormat(int&);

    // Delete signal description and value
    BOOL bDeleteSgDescVal( int nIndexOfItem,
                           CString strMsgName,
                           CString strSgName,
                           CString omStrDesc);

    // Update signal description and value
    BOOL bUpdateSgDescVal( CString strMsgCode,
                           CString strSgName,
                           CString omStrPrevDesc,
                           CSignalDescVal*);

    // Add signal description and value
    BOOL bAddSgDescVal( CString strMsgCode, CString strSgName);

    // Updates signal details
    void vUpdateSignalDetails ( CString strMsgName,
                                CString omStrSignalName,
                                sSIGNALS*);

    // Deletes signal
    BOOL bDeleteSignalFromMsg( int nIndexOfWhichItem,
                               CString strMsgName,
                               CString omStrSignalName);

    // Add signal
    BOOL bAddSignalToMsg( CString strMsgName );

    // Delete message
    BOOL bDeleteMsg( CString );

    // Update message details
    //BOOL bUpdateMsg( eMSG_CHANGES_IN eNum, CString, sMESSAGE* );

    // Add message
    BOOL bAddMsg();

    // Return no of messages
    UINT unGetNumerOfMessages();

    // Save into DB file
    BOOL bWriteIntoDatabaseFileFromDataStructure( CString strFileName);

    // Read from the DB file
    BOOL bFillDataStructureFromDatabaseFile( CString strFileName);

    // Get all signal names
    void vGetSignalNames( UINT unMsgID, CStringList& strSignalList);

    // Get index from message name
    int nGetMessageIndexFromMsgName( CString strMsgName);

    // Create new DB
    BOOL bCreateDataBase(CString strFilename);

    // Return message name from code
    CString omStrGetMessageNameFromMsgCode( UINT unMsgCode);

    // Return message name from code when called from Inactive Database
    CString omStrGetMessageNameFromMsgCodeInactive( UINT unMsgCode);

    // Get message name from code
    BOOL bMessageNameFromMsgCode(UINT unMsgCode, CString& omMsgName);

    // Get message code from name
    int nGetMessageCode( CString strMsgName);

    // Free reserved memory
    BOOL bDeAllocateMemory(CString strDBName);
    //Delete the memory allocated to a particular Database
    BOOL bDeAllocateDBMemory(sDBFileStruct* psDatbaseStructList);
    //Delete the memory allocated to a Inactive Database
    BOOL bDeAllocateMemoryInactive();
    // Return the array of DB file
    void vGetDataBaseNames(CStringArray* pastrDBnames);

    // Validate dupliacte start bit value
    BOOL bIsDuplicateSignalStartBitValue( CString omStrMsgName,
                                          UINT unByteNum,
                                          UINT unSignalLength,
                                          UINT unStartBitValue,
                                          BOOL bDataFormat);

    // Validate duplicate message code
    BOOL bIsDuplicateMessageCode( CString omStrMsgName,
                                  UINT unMsgCode);

    // Validate dupliacte signal name
    BOOL bIsDuplicateSignalName( CString omStrMsgName, CString omStrSgName);

    // Validate dupliacte message name
    BOOL bIsDuplicateMessageName( int nMsgId,CString strMsgName);

    // Creates and Updates "Unions.h" file
    CString bWriteDBHeader(CString omStrActiveDataBase);

    static BOOL bCalcBitMaskForSig(BYTE* pbyMaskByte, UINT unArrayLen,
                                   UINT nByteNum, UINT nBitNum, UINT nLength, EFORMAT_DATA bDataFormat);
    static BOOL bValidateSignal(UINT nDLC, UINT nByteNum, UINT nBitNum,
                                UINT nLength, EFORMAT_DATA bDataFormat);



private:
    BOOL bFormSigNameAndLength( UINT* punLength,
                                const UINT* punStartBit,
                                CStringArray& omStrArraySigName,
                                const INT nIndex );
    BOOL bSortSignalStartBitAscend(UINT* punSigStartBit ,UINT unCount);
    BOOL m_bIsDatabaseSaved;

    // Reserves memory for new message
    BOOL bAllocateMemory(BOOL nMode);

    // Validate Db file
    BOOL bValidateDatabaseFile(CString strFileName);

};
