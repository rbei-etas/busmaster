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
 * \file      ReplayFile.cpp
 * \brief     Implementation file for CReplayFile class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CReplayFile class
 */

#include "Replay_stdafx.h"             // For standard Include
#include "ReplayFile.h"         // For replay file class declaration

/*******************************************************************************
  Function Name  : CReplayFile
  Description    : Standard default constructor
  Member of      : CReplayFile
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
CReplayFile::CReplayFile() 
                           : m_omStrFileName( STR_EMPTY ),
                             m_nTimeMode( 0 ),
                             m_unMsgTimeDelay( 1 ),
                             m_nReplayMode( 0 ),
                             m_unCycleTimeDelay( 1 ),
                             m_bEnabled( TRUE ),
                             m_bInteractive( FALSE )
{
    m_sFilterApplied.vClear();
}

/*******************************************************************************
  Function Name  : CReplayFile
  Description    : Standard Copy constructor
  Member of      : CReplayFile
  Functionality  : This will copy local variables with the reference object data
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
CReplayFile::CReplayFile(const CReplayFile& ouRef)
{
    // Copy the content of the reference object
    vCopyContent( ouRef );
}

/*******************************************************************************
  Function Name  : operator =
  Input(s)       : ouRef - Object that is to be copied
  Output         : CReplayFile& - Self reference for operator cascading
  Functionality  : Overloaded '=' operator. This will copy RHS object data.
  Member of      : CReplayFile
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  : 
*******************************************************************************/
CReplayFile& CReplayFile::operator = ( const CReplayFile& ouRef)
{
    if( this != &ouRef )
    {
        vCopyContent( ouRef );
    }
    else
    {
        // Self Assignment
        ASSERT( FALSE );
    }
    // Return current object pointer
    return *this;
}

/*******************************************************************************
  Function Name  : vCopyContent
  Input(s)       : ouRef - Object that has to be copied
  Output         : -
  Functionality  : To copy the content of the reference object.
  Member of      : CReplayFile
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  : 
*******************************************************************************/
void CReplayFile::vCopyContent(const CReplayFile& ouRef)
{
    // Assign File Name
    m_omStrFileName = ouRef.m_omStrFileName;
    // Time Mode
    m_nTimeMode = ouRef.m_nTimeMode;
    // Specified Delay
    m_unMsgTimeDelay = ouRef.m_unMsgTimeDelay;
    // Repaly Mode
    m_nReplayMode = ouRef.m_nReplayMode;
    // Cyclic Delay
    m_unCycleTimeDelay = ouRef.m_unCycleTimeDelay;
    // Enable Option
    m_bEnabled = ouRef.m_bEnabled;
    // Interactive Option
    m_bInteractive = ouRef.m_bInteractive;
    // Copy Filter
    m_sFilterApplied.bClone(ouRef.m_sFilterApplied);    
}

/*******************************************************************************
  Function Name  : ~CReplayFile
  Description    : Standard Destructor
  Member of      : CReplayFile
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
CReplayFile::~CReplayFile()
{

}

/*******************************************************************************
  Function Name  : bSerialize
  Input(s)       : omArchive - Arcieve to configuration file
  Output         : BOOL - TRUE if successful, FALSE otherwise
  Functionality  : To serialize replay file class data
  Member of      : CReplayFile
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  : 
*******************************************************************************/
BOOL CReplayFile::bSerialize(CArchive& omArchive)
{
    UNREFERENCED_PARAMETER(omArchive);
    BOOL bResult = FALSE;
    /*
    if( omArchive.IsStoring() )
    {
        // Store File Name
        omArchive << m_omStrFileName;
        // Store time mode
        omArchive << m_nTimeMode;
        // Store Message Time delay
        omArchive << m_unMsgTimeDelay;  
        // Store replay mode
        omArchive << m_nReplayMode;
        // Store cycle delay
        omArchive << m_unCycleTimeDelay;
        // Store filter details
        // Store the size
        int nSize = m_sFilter.GetSize();
        omArchive << nSize;
        // Store individual filters
        for( int nIndex = 0; nIndex < nSize; nIndex++ )
        {
            // Serialize filters
            m_sFilter.ElementAt( nIndex ).Serialize( omArchive );
        }
        // Store Enabled Option
        omArchive << m_bEnabled;
        // Store Interactive Option
        omArchive << m_bInteractive;
        // Set the result
        bResult = TRUE;
    }
    else
    {
        // Load File Name
        omArchive >> m_omStrFileName;
        // Load time mode
        omArchive >> m_nTimeMode;
        // Load Message Time delay
        omArchive >> m_unMsgTimeDelay;  
        // Load replay mode
        omArchive >> m_nReplayMode;
        // Load cycle delay
        omArchive >> m_unCycleTimeDelay;
        // Load filter details
        m_sFilter.RemoveAll();
        // Load the size
        int nSize;
        omArchive >> nSize;
        // Load individual filters
        for( int nIndex = 0; nIndex < nSize; nIndex++ )
        {
            SMODULEFILTER sFilter;
            // Serialize filters
            sFilter.Serialize( omArchive );
            // Add the filter in to the list
            m_sFilter.Add( sFilter );
        }
        // Load Enabled Option
        omArchive >> m_bEnabled;
        // Load Interactive Option
        omArchive >> m_bInteractive;
        // Set the result
        bResult = TRUE;
    }
    */
    return bResult;
}

/*******************************************************************************
  Function Name  : unGetConfigSizeOfCommonMembers
  Input(s)       : 
  Output         : size of members common to all object
  Functionality  : To get size of members common to all object
  Member of      : CReplayFile
  Author(s)      : Anish kumar
  Date Created   : 08.07.2010
  Modifications  : 
*******************************************************************************/
UINT CReplayFile::unGetConfigSizeOfCommonMembers()
{
    UINT unSize = 0;
    unSize += sizeof(m_nTimeMode);
    unSize += sizeof(m_unMsgTimeDelay);
    unSize += sizeof(m_nReplayMode);
    unSize += sizeof(m_unCycleTimeDelay);
    unSize += sizeof(m_bEnabled);
    unSize += sizeof(m_bInteractive);
    unSize += (sizeof(TCHAR) * MAX_PATH); // To store the path
    return unSize;
}

UINT CReplayFile::unGetConfigSize()
{
    UINT unSize = 0;    
    unSize += unGetConfigSizeOfCommonMembers();
    unSize += m_sFilterApplied.unGetSize();
    return unSize;
}
/*******************************************************************************
  Function Name  : pbySaveConfig
  Input(s)       : pointer to buffer where the config will be saved
  Output         : End pointer
  Functionality  : To save config data 
  Member of      : CReplayFile
  Author(s)      : Anish kumar
  Date Created   : 08.07.2010
  Modifications  : 
*******************************************************************************/
BYTE* CReplayFile::pbySaveConfig(BYTE* pDesBuffer)
{
    memcpy(pDesBuffer, &m_nTimeMode, sizeof(m_nTimeMode)); 
    pDesBuffer += sizeof(m_nTimeMode);
    memcpy(pDesBuffer, &m_unMsgTimeDelay, sizeof(m_unMsgTimeDelay)); 
    pDesBuffer += sizeof(m_unMsgTimeDelay);
    memcpy(pDesBuffer, &m_nReplayMode, sizeof(m_nReplayMode)); 
    pDesBuffer += sizeof(m_nReplayMode);
    memcpy(pDesBuffer, &m_unCycleTimeDelay, sizeof(m_unCycleTimeDelay)); 
    pDesBuffer += sizeof(m_unCycleTimeDelay);
    memcpy(pDesBuffer, &m_bEnabled, sizeof(m_bEnabled)); 
    pDesBuffer += sizeof(m_bEnabled);
    memcpy(pDesBuffer, &m_bInteractive, sizeof(m_bInteractive)); 
    pDesBuffer += sizeof(m_bInteractive);
    //Save file name size
    TCHAR acName[MAX_PATH] = {_T('\0')};
    _tcscpy(acName, m_omStrFileName.GetBuffer(MAX_PATH));
    memcpy(pDesBuffer, acName, sizeof(TCHAR) * MAX_PATH);
    pDesBuffer += sizeof(TCHAR) * MAX_PATH;
    //To store filters   
    //This function will copy into the destination bufffer and increment the pointer
    pDesBuffer = m_sFilterApplied.pbGetConfigData(pDesBuffer);
    return pDesBuffer;
}

/*******************************************************************************
  Function Name  : pbyLoadConfig
  Input(s)       : pointer to buffer from where the config will be loaded
  Output         : End pointer
  Functionality  : To load config data 
  Member of      : CReplayFile
  Author(s)      : Anish kumar
  Date Created   : 08.07.2010
  Modifications  : 
*******************************************************************************/
BYTE* CReplayFile::pbyLoadConfig(BYTE* pSrcBuffer)
{
    if (NULL == pSrcBuffer)
    {
        return NULL;
    }
    memcpy(&m_nTimeMode, pSrcBuffer, sizeof(m_nTimeMode)); 
    pSrcBuffer += sizeof(m_nTimeMode);
    memcpy(&m_unMsgTimeDelay, pSrcBuffer, sizeof(m_unMsgTimeDelay)); 
    pSrcBuffer += sizeof(m_unMsgTimeDelay);
    memcpy(&m_nReplayMode, pSrcBuffer, sizeof(m_nReplayMode)); 
    pSrcBuffer += sizeof(m_nReplayMode);
    memcpy(&m_unCycleTimeDelay, pSrcBuffer, sizeof(m_unCycleTimeDelay)); 
    pSrcBuffer += sizeof(m_unCycleTimeDelay);
    memcpy(&m_bEnabled, pSrcBuffer, sizeof(m_bEnabled)); 
    pSrcBuffer += sizeof(m_bEnabled);
    memcpy(&m_bInteractive, pSrcBuffer, sizeof(m_bInteractive)); 
    pSrcBuffer += sizeof(m_bInteractive);
    //Save file name size
    TCHAR acName[MAX_PATH] = {_T('\0')};

    memcpy(acName, pSrcBuffer, sizeof(TCHAR) * MAX_PATH);
    pSrcBuffer += (sizeof(TCHAR) * MAX_PATH);
    //Save file name 
    m_omStrFileName.Format("%s", acName);
    ////To store filters
    bool bRet = false;
    pSrcBuffer = m_sFilterApplied.pbSetConfigData(pSrcBuffer, bRet);
    return pSrcBuffer;
}
/*******************************************************************************
  Function Name  : bisConfigChanged
  Input(s)       : pointer to buffer where the last config was saved
                   it will be incremented to point next replay file
  Output         : BOOL
  Functionality  : To find if config is modified
  Member of      : CReplayFile
  Author(s)      : Anish kumar
  Date Created   : 08.07.2010
  Modifications  : 
*******************************************************************************/
BOOL CReplayFile::bisConfigChanged(BYTE* &pSrcBuffer)
{
    BOOL bReturn = TRUE;
    if (NULL != pSrcBuffer)
    {
        memcpy(&m_nTimeModeTmp, pSrcBuffer, sizeof(m_nTimeModeTmp)); 
        pSrcBuffer += sizeof(m_nTimeModeTmp);
        if (m_nTimeMode != m_nTimeModeTmp)
        {
            bReturn = FALSE;
        }
        if (bReturn)
        {
            memcpy(&m_unTimeDelTmp, pSrcBuffer, sizeof(m_unTimeDelTmp)); 
            pSrcBuffer += sizeof(m_unTimeDelTmp);
            if (m_unMsgTimeDelay != m_unTimeDelTmp)
            {
                bReturn = FALSE;
            }
        }
        if (bReturn)
        {
            memcpy(&m_nRepModeTmp, pSrcBuffer, sizeof(m_nRepModeTmp)); 
            pSrcBuffer += sizeof(m_nRepModeTmp);
            if (m_nReplayMode != m_nRepModeTmp)
            {
                bReturn = FALSE;
            }
        }
        if (bReturn)
        {
            memcpy(&m_unCycleTimeTmp, pSrcBuffer, sizeof(m_unCycleTimeTmp)); 
            pSrcBuffer += sizeof(m_unCycleTimeTmp);
            if (m_unCycleTimeDelay != m_unCycleTimeTmp)
            {
                bReturn = FALSE;
            }
        }
        if (bReturn)
        {
            memcpy(&m_bEnabledTmp, pSrcBuffer, sizeof(m_bEnabledTmp)); 
            pSrcBuffer += sizeof(m_bEnabledTmp);
            if (m_bEnabled != m_bEnabledTmp)
            {
                bReturn = FALSE;
            }
        }
        if (bReturn)
        {
            memcpy(&m_bInteractiveTmp, pSrcBuffer, sizeof(m_bInteractiveTmp)); 
            pSrcBuffer += sizeof(m_bInteractiveTmp);
            if (m_bInteractive != m_bInteractiveTmp)
            {
                bReturn = FALSE;
            }
        }
        int nNameSize = 0;
        if (bReturn)
        {
            memcpy(&nNameSize, pSrcBuffer, sizeof(nNameSize));
            pSrcBuffer += sizeof(nNameSize);
            if (nNameSize != m_omStrFileName.GetLength())
            {
                bReturn = FALSE;
            }
        }
        if (bReturn)
        {
            m_omNameTmp = pSrcBuffer;
            pSrcBuffer += (nNameSize + 1);
            if (m_omStrFileName != m_omNameTmp)
            {
                bReturn = FALSE;
            }
        }
        UINT nFilterSize = 0;
        if (bReturn)
        {
            memcpy(&nFilterSize, pSrcBuffer, sizeof(UINT));
            //To Store no. of filters 
            pSrcBuffer += sizeof (UINT);
            if (nFilterSize != m_sFilterApplied.unGetSize())
            {
                bReturn = FALSE;
            }
        }
        if (bReturn)
        {
            BYTE* pbData = new BYTE[nFilterSize];
            m_sFilterApplied.pbGetConfigData(pbData);
            if (memcmp((void*)pSrcBuffer, (void*)pbData, nFilterSize) != 0)
            {
                bReturn = FALSE;
            }            
        }
    }
    return bReturn;
}

