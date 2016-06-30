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
 * \file      DataBaseMsgList.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "TestSetupEditorLib_stdafx.h"
#include "DataBaseMsgList.h"
#include "Utility\MultiLanguageSupport.h"

//#include "../Application/GettextBusmaster.h"

/******************************************************************************
Function Name  :  CDataBaseMsgList
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CDataBaseMsgList::CDataBaseMsgList(void)
{
    //m_unMessageCount = 0;
    //m_psMessages = nullptr;
    mCurrentCluster = nullptr;
}

/******************************************************************************
Function Name  :  ~CDataBaseMsgList
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CDataBaseMsgList::~CDataBaseMsgList(void)
{
    bFreeMessageMemory();
}

/******************************************************************************
Function Name  :  bFillDataStructureFromDatabaseFile
Input(s)       :  CString strFileName - DataBase File Path
Output         :  BOOL
Functionality  :
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
ERRORCODE CDataBaseMsgList::FillDataStructureFromDatabaseFile( std::string strFileName )
{
    bFreeMessageMemory();
    std::list<ClusterResult> clusterList;
    ERRORCODE ecError = mDbManagerAccess.mParseDbFile( strFileName, CAN, clusterList );
    if ( ecError != EC_SUCCESS && ecError != EC_WARNING_SUCCESS )
    {
        return ecError;
    }
    if ( clusterList.end() == clusterList.begin() )
    {
        mCurrentCluster = nullptr;
        return EC_FAILURE;
    }
    mCurrentCluster = clusterList.begin()->m_pCluster;
    return EC_SUCCESS;
}

/******************************************************************************
Function Name  :  bIsValidMessageID
Input(s)       :  UINT dwMsgId - Message id
Output         :  BOOL
Functionality  :
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
BOOL CDataBaseMsgList::bIsValidMessageID(UINT dwMsgId)
{
    BOOL isValid = FALSE;
    if ( nullptr != mCurrentCluster )
    {
        IFrame* frame;
        mCurrentCluster->GetFrame( dwMsgId, nullptr, &frame );
        if ( nullptr != frame )
        {
            isValid = TRUE;
        }
    }
    return isValid;
}

/******************************************************************************
Function Name  :  nFillMessageList
Input(s)       :  CComboBox& omDbMsgCombo
                  BOOL bEmptyString
Output         :  INT
Functionality  :  Fills The Combo Box With Message List
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
INT CDataBaseMsgList::nFillMessageList(CComboBox& omDbMsgCombo, BOOL bEmptyString)
{
    std::map<UID_ELEMENT, IElement*> frameList;
    mCurrentCluster->GetElementList( eFrameElement, frameList );
    std::string frameName;
for ( auto frame : frameList )
    {
        frame.second->GetName( frameName );
        omDbMsgCombo.AddString( frameName.c_str() );
    }
    if(bEmptyString == TRUE)
    {
        omDbMsgCombo.AddString(_(defDELETE_MSG_SYMBOL));
    }
    return frameList.size();
}

/******************************************************************************
Function Name  :  nFillMessageList
Input(s)       :  CStringArray& omstrEntryList
                  BOOL bEmptyString
Output         :  INT
Functionality  :  Fills The Message List Into String Array
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
INT CDataBaseMsgList::nFillMessageList(CStringArray& omstrEntryList, BOOL bEmptyString)
{
    if ( nullptr == mCurrentCluster )
    {
        return ERR_INVALID_DATABASE;
    }
    std::map<UID_ELEMENT, IElement*> frameList;
    mCurrentCluster->GetElementList( eFrameElement, frameList );
    std::string frameName;
for ( auto frame : frameList )
    {
        frame.second->GetName( frameName );
        omstrEntryList.Add( frameName.c_str() );
    }
    if ( bEmptyString == TRUE )
    {
        omstrEntryList.InsertAt( 0, _( defDELETE_MSG_SYMBOL ) );
    }
    return frameList.size();
}

/******************************************************************************
Function Name  :  bFreeMessageMemory
Input(s)       :
Output         :  BOOL
Functionality  :  Delete the Allocated Memory
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
BOOL CDataBaseMsgList::bFreeMessageMemory(void)
{
    if ( mCurrentCluster != nullptr )
    {
        delete mCurrentCluster;
        mCurrentCluster = nullptr;
    }
    return TRUE;
}

/******************************************************************************
Function Name  :  unGetMessageID
Input(s)       :  CString omstrMsgName - Message Name
Output         :  UINT
Functionality  :  Retrieves the Message ID
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
UINT CDataBaseMsgList::unGetMessageID( CString omstrMsgName )
{
    if ( mCurrentCluster == nullptr )
    {
        return ERR_INVALID_DATABASE;
    }
    std::map<UID_ELEMENT, IElement*> frameList;
    mCurrentCluster->GetElementList( eFrameElement, frameList );
    std::string frameName;
    UINT frameId = (UINT)ERR_WRONG_ID;
for ( auto frame : frameList )
    {
        frame.second->GetName( frameName );
        if ( omstrMsgName == frameName.c_str() )
        {
            ( (IFrame*)frame.second )->GetFrameId( frameId );
            break;
        }
    }
    return frameId;
}

/******************************************************************************
Function Name  :  nGetMessageName
Input(s)       :  UINT unMsgId - Message ID
                  CString& omstrMsgName  - message Name
Output         :  INT
Functionality  :  Returns the Message Name
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
INT CDataBaseMsgList::nGetMessageName(UINT unMsgId, CString& omstrMsgName)
{
    int retVal = ERR_INVALID_DATABASE;

    if ( nullptr != mCurrentCluster )
    {
        IFrame* frame = nullptr;
        omstrMsgName = "";
        mCurrentCluster->GetFrame( unMsgId, nullptr, &frame );
        retVal = ERR_WRONG_ID;
        if ( nullptr != frame )
        {
            std::string frameName;
            frame->GetName( frameName );
            omstrMsgName = frameName.c_str();
            retVal = S_OK;
        }
    }
    return retVal;
}


/******************************************************************************
Function Name  :  nGetMessageInfo
Input(s)       :  CString omstrMsgName
                  sMESSAGE& sMsg
Output         :  INT
Functionality  :  Return the Message Info
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
INT CDataBaseMsgList::nGetMessageInfo( CString omstrMsgName, IFrame** sMsg )
{
    if ( nullptr == mCurrentCluster )
    {
        return ERR_INVALID_DATABASE;
    }
    *sMsg = nullptr;
    std::map<UID_ELEMENT, IElement*> frameList;
    mCurrentCluster->GetElementList( eFrameElement, frameList );
    std::string frameName;
    UINT frameId = (UINT)ERR_WRONG_ID;
for ( auto frame : frameList )
    {
        frame.second->GetName( frameName );
        if ( omstrMsgName == frameName.c_str() )
        {
            *sMsg = ( (IFrame*)frame.second );
            return S_OK;
        }
    }
    return ERR_WRONG_ID;
}

/******************************************************************************
Function Name  :  unGetMsg
Input(s)       :  UINT unMsgId
Output         :  sMESSAGE*
Functionality  :  Returns the message Information
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
IFrame* CDataBaseMsgList::unGetMsg( UINT unMsgId )
{
    IFrame* frame = nullptr;
    if( mCurrentCluster == nullptr )
    {
        return nullptr;
    }
    mCurrentCluster->GetFrame( unMsgId, nullptr, &frame );
    return frame;
}
