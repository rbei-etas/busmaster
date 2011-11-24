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
 * \file      ConfigDetails.cpp
 * \brief     Contains the description of the class CConfigDetails
 * \author    Gopinath Mujumdar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains the description of the class CConfigDetails
 */
#include "stdafx.h" // standard includes present in this header
#include "struct.h" // definitions of the structures used in this application
#include "hashdefines.h" // contains hash defines used across classes
#include "BUSMASTER.h"    // For application class definition
#include "ConfigDetails.h"  // definition of the class CConfigDetails
#include "MainFrm.h"        // For CMainFrame class declaration

// Global App Object
extern CCANMonitorApp theApp;
// Global Tx Mag List
extern PSTXMSG g_psTxMsgBlockList;

//#define ETAS_RELEASE
/******************************************************************************/
/*  Function Name    : CConfigDetails                                         */
/*                                                                            */
/*  Input(s)         :  -                                                     */
/*  Output           :  -                                                     */
/*                                                                            */
/*  Functionality    : Constructor                                            */
/*                                                                            */
/*  Member of        :  -                                                     */
/*  Friend of        :  -                                                     */
/*                                                                            */
/*  Author(s)        : Gopi                                                   */
/*  Date Created     : 18.10.2002                                             */
/*  Modifications on : 16.12.2002, Amitesh Bharti, Added initialisation for   */
/*                                 m_ucCheckSum                               */
/*  Modifications    : Raja N on 05/04/2004                                   */
/*                   : Added init of signal watch list pointer                */
/*  Modifications    : Anish  on 15/12/2006                                   */
/*                   : Added Array pointer creation for DB file names         */
/*  Modifications    : Anish  on 08/02/2007                                   */
/*                   : moved Array pointer creation for DB file names         */
/*                     before calling initdefault values                      */
/******************************************************************************/
CConfigDetails::CConfigDetails() :  m_bConfigInfoLoaded(FALSE),
                                    m_bIsDirty(FALSE),
                                    m_hConfigFile(NULL),
                                    m_fAppVersion(static_cast<FLOAT>(defAPPVERSION)),
                                    m_omStrMruCFile(defEMPTYSTR),
                                    m_omstrConfigFilename(defEMPTYSTR),
                                    m_omstrTempFilename(defEMPTYSTR),
                                    m_omStrLogFilename(defEMPTYSTR),
                                    m_omStrReplayFilename(defEMPTYSTR),
                                    m_omStrMruDLLFilename(defEMPTYSTR),
                                    m_omStrMsgName(defEMPTYSTR),
                                    m_omStrMsgID(defEMPTYSTR),
                                    m_omStrCopyright(defEMPTYSTR),
                                    m_ucCheckSum(0),
                                    m_bIsConfigurationModified(FALSE),
                                    m_unNumberOfMsgBlockCount(0),
                                    m_psMsgBlockList(NULL),
                                    m_psSignalWatchList(NULL),
                                    m_psSimSysArray(NULL)

{
    //m_pSimSysNodeInfo = NULL;
    //Create the CStringArray pointer to load/store database file names
    m_pomaStrDatabaseFilename = new CStringArray;
    m_sMessageAttributes.m_psMsgAttribDetails = NULL;
    m_sMessageAttributes.m_usMsgCount = 0;
    m_sMsgFilterDetails.m_punUndefinedMsgID = NULL;
    vInitDefaultValues();
    m_bConfigInfoLoaded = TRUE;
    m_sFilterConfigured.vClear();
    
}





/******************************************************************************/
/*  Function Name    :  ~CConfigDetails                                       */
/*                                                                            */
/*  Input(s)         :   -                                                    */
/*  Output           :   -                                                    */
/*                                                                            */
/*  Functionality    :  Destructor                                            */
/*                                                                            */
/*  Member of        :   -                                                    */
/*  Friend of        :   -                                                    */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications    :  Raja N on 05/04/02004                                 */
/*                   :  Included release of Signal Watch List memory          */
/******************************************************************************/
CConfigDetails::~CConfigDetails()
{    
    vReleaseMultiMsgInfo(m_psMsgBlockList);
    m_psMsgBlockList = NULL;
    vReleaseSignalWatchListMemory();
    vReleaseSimSysListMemory();
    vReleaseSimSysInfo();
    vReleaseMsgAttrib(&m_sMessageAttributes);
    vReleaseMsgFilterDetails(&m_sMsgFilterDetails);
    if (m_pomaStrDatabaseFilename != NULL)
    {
        delete m_pomaStrDatabaseFilename;
        m_pomaStrDatabaseFilename = NULL; 
    }
}



/******************************************************************************/
/*  Function Name    :  nIsCfgFileFound                                       */
/*  Input(s)         :  omstrFilename : configuration filename                */
/*                                      If none is supplied, then default     */
/*                                      filename is considered.               */
/*  Output           :  defCONFIG_FILE_NOT_FOUND : if the file is not found   */
/*                      defCONFIG_FILE_OPEN_FAILED : if the file could not be */
/*                                                   opened                   */
/*                      defCONFIG_FILE_READ_FAILED : if file read failed      */
/*                      defCONFIG_FILE_INVALID_FILE_EXTN : if invalid         */
/*                                                       extension is provided*/
/*                      defCONFIG_FILE_CORRUPT : if timestamp information     */
/*                                               stored in the input cfg file */
/*                                               does not match with the last */
/*                                               modified information of the  */
/*                                               file                         */
/*                      defCONFIG_FILE_HDR_CORRUPT : if file header is corrupt*/
/*                      defCONFIG_FILE_SUCCESS  : if file is opened           */
/*                                                successfully                */
/*                                                                            */
/*  Functionality    :  This method searches for the specified configuration  */
/*                      file. If no filename is specified, then the default   */
/*                      filename is chosen.                                   */
/*                      It first validates the file extension, if it is       */
/*                      incorrect, then the method exits returning the error  */
/*                      code defCONFIG_FILE_INVALID_FILE_EXTN.                */
/*                      The method then searches of the file, if the file is  */
/*                      not found, then the method exits returning the error  */
/*                      code defCONFIG_FILE_NOT_FOUND.                        */
/*                      If th file is found, and everything is OK with the    */
/*                      file, then a copy of the same is made. This is to     */
/*                      ensure that incase of a failure while archiving the   */
/*                      data, the old data can be restored. This temporary    */
/*                      file is deleted if the operation is successful, or    */
/*                      renamed to the original name if there is a failure.   */
/*                      If all the checks are successful, then the method     */
/*                      returns the defCONFIG_FILE_SUCCESS code.              */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :  13.11.2002, Gopi, No longer appending the current     */
/*                      directory path to the filname                         */
/******************************************************************************/
INT CConfigDetails::nIsCfgFileFound(CString& omStrFilename, BOOL bOpenExisting)
{
    // bOpenExisiting is TRUE, if the callee of the function is 
    // nSaveConfiguration(...), and FALSE if the callee is 
    // nLoadConfiguration.
    // Hence for loading, the file can be opened in GENERIC_READ
    // access and for saving, the file can be opened in GENERIC_WRITE
    // access.
    
    INT nRetVal             = defCONFIG_FILE_SUCCESS;
    
    // validate the extension
    INT nDotPosn = omStrFilename.ReverseFind(defDOT);
    // we found the '.'
    if (nDotPosn != -1)
    {
        CString ostrExtn = omStrFilename.Mid(nDotPosn, defNUMEXTNCHARS + 1);
        if (ostrExtn != defVALIDEXTN)
        {
            nRetVal = defCONFIG_FILE_INVALID_FILE_EXTN;
        }
    }
    
    // is everything OK until now ?
    if (nRetVal == defCONFIG_FILE_SUCCESS)
    {
	    DWORD dwDesiredAccess = 0;
		
		// set the mode of creation..
        DWORD dwCreateMode;
        
        // a copy of the file needs to be created, hence the file is opened
        // in SHARE_READ mode
        DWORD dwShareMode = FILE_SHARE_READ;
        
        // depending on bOpenExisting, the file is either opened or created
        if (bOpenExisting == TRUE)
        {
            dwCreateMode    = OPEN_EXISTING;
            dwDesiredAccess = GENERIC_READ;
        }
        else
        {
            dwCreateMode    = CREATE_ALWAYS;
            dwDesiredAccess = GENERIC_WRITE;
        }
        
        // check if the file exists..
        m_hConfigFile = ::CreateFile(omStrFilename,
            dwDesiredAccess,
            //GENERIC_READ | GENERIC_WRITE,
            dwShareMode,
            NULL,
            dwCreateMode,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        
        // is file not found?
        if (m_hConfigFile == INVALID_HANDLE_VALUE)
        {
            // Most of the values returned by this function 
            // that are related to file operation
            // has been hash defined
            nRetVal = GetLastError();
        }
        else
        {
            if (bOpenExisting == TRUE)
            {
                // make a backup only if an existing file is opened
                // copy the existing file into another so that the same can be 
                // restored incase there are any problems while archiving the 
                // data during the 'store' operation..
                CString oStrTmp(omStrFilename);
                
                INT nPos = oStrTmp.ReverseFind(defDOT);
                
                m_omstrTempFilename = oStrTmp.Left(nPos);
                m_omstrTempFilename += defDOT;
                m_omstrTempFilename += defTMPFILEEXTN;
                
                if (
                    ::CopyFile(omStrFilename, m_omstrTempFilename, FALSE) == TRUE
                    )
                {
                    ::SetFileAttributes(m_omstrTempFilename, defTMPFILEATTRIB);
                }
            } // backup of file
            
        } // open file failed
        
    } // file extension was correct
    
    return nRetVal;
}






/******************************************************************************/
/*  Function Name    :  bGetData                                              */
/*                                                                            */
/*  Input(s)         :  eParam : enumerator for the configuration information */
/*                               that is needed.                              */
/*                      lpData : pointer to the location where the data will  */
/*                               be copied                                    */
/*  Output           :  TRUE : if the data is successfully copied.            */
/*                      FALSE : if value of eParam is invalid, if lpData is   */
/*                              invalid or if data corresponding to eParam is */
/*                              an array                                      */
/*                                                                            */
/*  Functionality    :  This method looks up for the data member corresponding*/
/*                      to eParam and copies the information into the pointer */
/*                      that is passed.                                       */
/*                      For e.g. if eParam is LOG_FILE_NAME, the name of the  */
/*                      log file is copied into lpData.                       */
/*                      This method is rsponsible for the allocation of the   */
/*                      memory. The calling module should release the memory  */
/*                      explicitly.                                           */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  23.03.2003, Changed to take care of acceptance mask,  */
/*                      acceptance filter, controller mode, warning limit etc */
/*  Modifications on :  18.04.2003, Added code for Trace Window. For output   */
/*                                  and replay window the structure           */
/*                                  WINDOWPLACEMENT is used.                  */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  17.06.2003, Some more flag information is stored in,  */
/*                      configuration file:Disp and Log Format and Time Mode  */
/*                      Some name is changed: byBase -> byDisplayHexON        */
/*  Modifications By :  Krishnaswamy B.N                                      */
/*  Modifications on :  19.08.2003, Flag information for m_byLogOverWriteON   */
/*                      and m_byReplayMsgType is obtained                     */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  08.01.2004, The interface for message block           */
/*                      is changed and a new flag is introduced to get the    */
/*                      message count.                                        */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  05.04.2004, Added interface for Signal Watch list data*/
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  22.07.2004, Added interface for Message Buffer Data   */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  03.08.2004, Added Ensure Visble flag value get code   */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  01.12.2004, Added signal graph list pointer get function
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  09.03.2005, Modified controller information get code to
                        support multi channel architecture                    */
/*  Modifications on :  18.03.2005, Implemented code review comments. Changed */
/*                      type case to LPVOID instead of data type cast in the  */
/*                      pointer is already in                                 */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  13.04.2005, Added code to get Tx window Splitter      */
/*                      position                                              */
/*  Modifications on :  13.04.2005, Added code to get Config file version and */
/*                      old & new filter list of message display              */
/*  Modifications on :  06.12.2006, Added code to get Array of  DB Files      */
/*                      instead of single DB file name                        */
/* Modifications By  :  Pradeep Kadoor on 09.05.2008                          */
/*                      Code added to retreive config details of SJW          */
/*                      and PropDelay                                         */
/******************************************************************************/
BOOL CConfigDetails::bGetData(eCONFIGDETAILS  eParam, LPVOID* lpData)
{
    BOOL bRetVal = TRUE;
    
    
    // is the configuration loaded?
    if ((m_bConfigInfoLoaded == TRUE) && (bRetVal == TRUE))
    {
        switch(eParam)
        {
        case MRU_C_FILE_NAME:
            {
                *lpData = NULL;

                *lpData = static_cast<CString*>(new CString);
                
                if (*lpData != NULL)
                {
                    *(static_cast<CString*>(*lpData)) = m_omStrMruCFile;
                }
                else
                {
                    bRetVal = FALSE;
                }
            }
            break;
        case LOG_FILE_NAME:
            {
                *lpData = NULL;
                // copy the log file name into the buffer
                *lpData = static_cast<CString*>(new CString);
                if (*lpData != NULL)
                {
                    *(static_cast<CString*>(*lpData)) = m_omStrLogFilename;
                }
                else
                {
                    // failed to allocate memory
                    bRetVal = FALSE;
                }
            }
            break;
        case REPLAY_FILE_NAME:
            {
                *lpData = NULL;
                // copy the replay file name into the buffer
                *lpData = static_cast<CString*>(new CString);
                if (*lpData != NULL)
                {
                    *(static_cast<CString*>(*lpData)) = m_omStrReplayFilename;
                }
                else
                {
                    // failed to allocate memory
                    bRetVal = FALSE;
                }
            }
            break;
        case DATABASE_FILE_NAME:
            {
                *lpData = NULL;
                // copy the database file name into the buffer
                *lpData = static_cast<CStringArray*>(new CStringArray);
                if (*lpData != NULL && m_pomaStrDatabaseFilename != NULL)
                {
                    static_cast<CStringArray*>(*lpData)->RemoveAll();
                    static_cast<CStringArray*>(*lpData)->
                                         Append(*m_pomaStrDatabaseFilename);
                }
                else
                {
                    // failed to allocate memory
                    bRetVal = FALSE;
                }
            }
            break;
        case MRU_DLL_FILE_NAME:
            {
                
                *lpData = NULL;
                // copy the most recently used DLL file name into the buffer
                *lpData = static_cast<CString*>(new CString);
                if (*lpData != NULL)
                {
                    *(static_cast<CString*>(*lpData)) = m_omStrMruDLLFilename;
                }
                else
                {
                    // failed to allocate memory
                    bRetVal = FALSE;
                }
            }
            break;
        //case MESSAGE_NAME:
        //    {
        //        *lpData = NULL;
        //        // copy the message name into the buffer
        //        *lpData = static_cast<CString*>(new CString);
        //        if (*lpData != NULL)
        //        {
        //            *(static_cast<CString*>(*lpData)) = m_omStrMsgName;
        //        }
        //        else
        //        {
        //            // failed to allocate memory
        //            bRetVal = FALSE;
        //        }
        //    }
        //    break;
        //case MESSAGE_ID:
        //    {
        //        *lpData = NULL;
        //        // copy the message id into the buffer
        //        *lpData = static_cast<CString*>(new CString);
        //        if (*lpData != NULL)
        //        {
        //            *(static_cast<CString*>(*lpData)) = m_omStrMsgID;
        //        }
        //        else
        //        {
        //            // failed to allocate memory
        //            bRetVal = FALSE;
        //        }
        //    }
        //    break;
        case TOOLBAR_DETAILS:
            {
                *lpData = NULL;
                // get the tool bar button status information
                PSTOOLBARINFO pTemp = new STOOLBARINFO;
                
                if (pTemp != NULL)
                {
                    pTemp->m_byMsgFilter      =
                        m_sToolBarButtonStatus.m_byMsgFilter;
                    pTemp->m_byLogFilter      = 
                        m_sToolBarButtonStatus.m_byLogFilter;
                    pTemp->m_byLogging        = 
                        m_sToolBarButtonStatus.m_byLogging;
                    pTemp->m_byMsgInterpret   = 
                        m_sToolBarButtonStatus.m_byMsgInterpret;
                    pTemp->m_byOverwrite      = 
                        m_sToolBarButtonStatus.m_byOverwrite;
                    
                    pTemp->m_byDisplayHexON   = 
                        m_sToolBarButtonStatus.m_byDisplayHexON;
                    pTemp->m_byDisplayTimeMode= 
                        m_sToolBarButtonStatus.m_byDisplayTimeMode;
                    
                    pTemp->m_byLogHexON       =
                        m_sToolBarButtonStatus.m_byLogHexON;
                    pTemp->m_byLogTimeMode    = 
                        m_sToolBarButtonStatus.m_byLogTimeMode;
                    pTemp->m_byLogOverWriteON = 
                        m_sToolBarButtonStatus.m_byLogOverWriteON;
                    pTemp->m_byReplayMsgType  = 
                        m_sToolBarButtonStatus.m_byReplayMsgType;
                    pTemp->m_byEnsureVisible  =
                        m_sToolBarButtonStatus.m_byEnsureVisible;
                    *lpData = static_cast<LPVOID>(pTemp);
                }
                else
                {
                    // failed to allocate memory
                    bRetVal = FALSE;
                }
                
            }
            break;
        case CONTROLLER_DETAILS:
        {
            *lpData = NULL;
            PSCONTROLER_DETAILS  psControllerDetails = 
                new SCONTROLER_DETAILS[ defNO_OF_CHANNELS ];
            // baud rate information
            if (psControllerDetails != NULL)
            {
                // Fill all supported channel information
                for (int nIndex = 0; nIndex < defNO_OF_CHANNELS; nIndex++)
                {
                    // packed value of bit timing register 0 
                    // and bit timing register 1
                    psControllerDetails[ nIndex ].m_nBTR0BTR1 =
                        m_sControllerDetails[ nIndex ].m_nBTR0BTR1;
                    // item number under focus
                    psControllerDetails[ nIndex ].m_nItemUnderFocus =
                        m_sControllerDetails[ nIndex ].m_nItemUnderFocus;
                    // acceptance code information
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrAccCodeByte1[0],
                        m_sControllerDetails[ nIndex ].m_omStrAccCodeByte1[0]);
                    // acceptance code information
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrAccCodeByte2[0],
                        m_sControllerDetails[ nIndex ].m_omStrAccCodeByte2[0]);
                    // acceptance code information
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrAccCodeByte3[0],
                        m_sControllerDetails[ nIndex ].m_omStrAccCodeByte3[0]);
                    // acceptance code information
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrAccCodeByte4[0],
                        m_sControllerDetails[ nIndex ].m_omStrAccCodeByte4[0]);
                    // acceptance mask information
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrAccMaskByte1[0],
                        m_sControllerDetails[ nIndex ].m_omStrAccMaskByte1[0]);
                    // acceptance mask information
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrAccMaskByte2[0],
                        m_sControllerDetails[ nIndex ].m_omStrAccMaskByte2[0]);
                    // acceptance mask information
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrAccMaskByte3[0],
                        m_sControllerDetails[ nIndex ].m_omStrAccMaskByte3[0]);
                    // acceptance mask information
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrAccMaskByte4[0],
                        m_sControllerDetails[ nIndex ].m_omStrAccMaskByte4[0]);
                    // acceptance filter mode
                    psControllerDetails[ nIndex ].m_bAccFilterMode =
                        m_sControllerDetails[ nIndex ].m_bAccFilterMode;
                    // controller mode
                    psControllerDetails[ nIndex ].m_ucControllerMode =
                        m_sControllerDetails[ nIndex ].m_ucControllerMode;
                    // warning limit
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrWarningLimit,
                        m_sControllerDetails[ nIndex ].m_omStrWarningLimit);
                    // baudrate information
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrBaudrate,
                        m_sControllerDetails[ nIndex ].m_omStrBaudrate);
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrCNF1,
                        m_sControllerDetails[ nIndex ].m_omStrCNF1);
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrCNF2,
                        m_sControllerDetails[ nIndex ].m_omStrCNF2);
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrCNF3,
                        m_sControllerDetails[ nIndex ].m_omStrCNF3);
                    // clock information
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrPropagationDelay,
                        m_sControllerDetails[ nIndex ].m_omStrPropagationDelay);
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrSjw,
                        m_sControllerDetails[ nIndex ].m_omStrSjw);
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrClock,
                        m_sControllerDetails[ nIndex ].m_omStrClock);
                    // sampling information
                    _tcscpy(psControllerDetails[ nIndex ].m_omStrSampling,
                        m_sControllerDetails[ nIndex ].m_omStrSampling);
                }
                // Now assign it to the result
                *lpData =
                    static_cast<LPVOID>(psControllerDetails);
            }
            else
            {
                // failed to allocate memory
                bRetVal = FALSE;
            }
        }
            break;
        case SEND_MULTI_MSGS:
            {
                *lpData = NULL;
                PSMSGBLOCKLIST psMsgBlockList  = new SMSGBLOCKLIST;
                if (psMsgBlockList != NULL)
                {
                    vInitialiseMsgBlock(psMsgBlockList);
                    // multiple message 
                    bRetVal = bGetMultiMsgInfo(psMsgBlockList);
                    
                    // release memory if any error occurs
                    if (bRetVal == FALSE)
                    {
                        vReleaseMultiMsgInfo(psMsgBlockList);
                        psMsgBlockList = NULL;
                    }
                }
                else
                {
                    // memory not enough
                    bRetVal = FALSE;
                }
                
                if (bRetVal == TRUE)
                {
                    *lpData = static_cast<LPVOID>(psMsgBlockList);
                }
            }
            break;
        case MSG_BLOCK_COUNT:
            {
                UINT* punCount = static_cast<UINT*>(*lpData);
                if (punCount != NULL)
                {
                    *(punCount) = static_cast<UINT>(m_unNumberOfMsgBlockCount);
                }
                else
                {
                    bRetVal = FALSE;
                }
                
            }
            break;
        case MSG_ATTRIBUTES:
            {
                *lpData = NULL;
                PSMESSAGE_ATTRIB pMsgAttrib = new SMESSAGE_ATTRIB;
                
                if (pMsgAttrib != NULL)
                {
                    // attributes of all the messages
                    bRetVal = bGetMsgAttrib(pMsgAttrib);
                    
                    // release memory if any error occurs
                    if (bRetVal == FALSE)
                    {
                        vReleaseMsgAttrib(pMsgAttrib);
                        pMsgAttrib = NULL;
                    }
                }
                else
                {
                    // failed memory allocation
                    bRetVal = FALSE;
                }
                
                if (bRetVal == TRUE)
                {
                    *lpData = static_cast<LPVOID>(pMsgAttrib);
                }
            }
            break;
        case OLD_FILTER_DETAILS:
            {
                *lpData = NULL;
                PSMESSAGE_FILTER_DETAILS pMsgFilterDetails = 
                    new SMESSAGE_FILTER_DETAILS;
                if (pMsgFilterDetails != NULL)
                {
                    // filter details
                    bRetVal = bGetMsgFilterDetails(pMsgFilterDetails);
                    
                    // release memory if any error occurs
                    if (bRetVal == FALSE)
                    {
                        vReleaseMsgFilterDetails(pMsgFilterDetails);
                        pMsgFilterDetails = NULL;
                    }
                }
                else
                {
                    // failed memory allocation..
                    bRetVal = FALSE;
                }
                
                if (bRetVal == TRUE)
                {
                    *lpData = 
                        static_cast<LPVOID>(pMsgFilterDetails);
                }
            }
            break;
            
        case SIGNAL_WATCH_LIST:
            // Signal Watch List Pointer
            {
                *lpData = 
                    static_cast<LPVOID>(m_psSignalWatchList);
            }
            break;
        case SIMSYS_LIST:
            // Simsys array pointer
            {
                *lpData = 
                    static_cast<LPVOID>(m_psSimSysArray);
            }
            break;
        case SIGNAL_GRAPH_LIST:
            // Signal Graph List Pointer
            {
                // Assign member object pointer
                // This pointer will be used accross to update
                // Graph List
                *lpData = &m_odGraphList;
            }
            break;
            
        case MSG_BUFFER_SIZE:
            // Message Buffer Size
            {
                if (lpData!= NULL && *lpData != NULL)
                {
                    PUINT punSize = static_cast<PUINT>(*lpData);
                    punSize[defAPPEND_DATA_INDEX] = m_unAppendBufferSize;
                    punSize[defOVERWRITE_DATE_INDEX] = m_unOverWriteBufferSize;
                    punSize[defDISPLAY_UPDATE_DATA_INDEX] =
                        m_unDisplayUpdateRate;
                }
                else
                {
                    bRetVal = FALSE;
                }
            }
            break;
            
            // Window placement details
        case REPLAY_WND_PLACEMENT: // Replay window
            {
                WINDOWPLACEMENT * psData = 
                    static_cast<WINDOWPLACEMENT*>(*lpData);
                *psData = m_sReplayWndCoOrd;
            }
            break;
        case OUT_WND_PLACEMENT: // Compile output window
            {
                WINDOWPLACEMENT * psData = 
                    static_cast<WINDOWPLACEMENT*>(*lpData);
                *psData = m_sOutputWndCoOrd;
            }
            break;
        case NOTIFICATION_WND_PLACEMENT: // Trace Window
            {
                WINDOWPLACEMENT * psData = 
                    static_cast<WINDOWPLACEMENT*>(*lpData);
                *psData = m_sNotificWndCoOrd;
            }
            break;
        case MSG_WND_PLACEMENT:     // Message window
            {
                WINDOWPLACEMENT * psData = 
                    static_cast<WINDOWPLACEMENT*>(*lpData);
                *psData = m_sMsgWndCoOrd;
            }
            break;
        case SIGWATCH_WND_PLACEMENT:    // Signal watch window
            {
                WINDOWPLACEMENT * psData = 
                    static_cast<WINDOWPLACEMENT*>(*lpData);
                *psData = m_sSigWatchWindow;
            }
            break;
        case MSGINTERP_WND_PLACEMENT: // Message Interpretation window
            {
                WINDOWPLACEMENT * psData = 
                    static_cast<WINDOWPLACEMENT*>(*lpData);
                *psData = m_sMsgInterpWindow;
            }
            break;
        case GRAPH_WND_PLACEMENT: // Graph window
            {
                WINDOWPLACEMENT * psData = 
                    static_cast<WINDOWPLACEMENT*>(*lpData);
                *psData = m_sGraphWndCoOrd;
            }
            break;
        case TX_WND_PLACEMENT:  // Tx configure window
            {
                WINDOWPLACEMENT * psData = 
                    static_cast<WINDOWPLACEMENT*>(*lpData);
                *psData = m_sTxWndCoOrd;
            }
            break;
        //case SIMSYS_WND_PLACEMENT: // Simulated system configure window
        //    {
        //        WINDOWPLACEMENT * psData = 
        //            static_cast<WINDOWPLACEMENT*>(*lpData);
        //        *psData = m_sSimSysCoOrd;
        //    }
        //    break;
            // Splitter details
        case GRAPH_WND_SPLITTER_DATA: // Graph window splitter postion
            {
                PSGRAPHSPLITTERDATA psData =
                    static_cast<PSGRAPHSPLITTERDATA>(*lpData);
                *psData = m_sGraphWndSplitterPos;
            }
            break;
        case TX_MSG_WND_SPLITTER_DATA: // Tx configure splitter position
            {
                PSTXMSGSPLITTERDATA psData =
                    static_cast<PSTXMSGSPLITTERDATA>(*lpData);
                *psData = m_sTxMsgWndSplitterPos;
            }
            break;
            
        case CONFIG_FILE_VERSION:
            {
                // Assign the file version
                if (lpData != NULL && *lpData != NULL)
                {
                    float * pData = static_cast<float *>(*lpData);
                    *pData = m_fAppVersion;
                }
            }
            break;
            // New vesion of Filter for Message Dispaly
        case MSG_DISPLAY_FILTER_DETAILS:
            {
                // get the pointer and copy the data
                CModuleFilterArray * psData =
                    static_cast<CModuleFilterArray*>(*lpData);
                // Remove elements in the list if any
                if (psData != NULL)
                {
                    psData->RemoveAll();
                    // Take display list and copy all
                    int nSize = m_omMsgDisplayFilter.GetSize();
                    for (int nIndex = 0; nIndex < nSize; nIndex++)
                    {
                        psData->Add( m_omMsgDisplayFilter[ nIndex ]);
                    }
                }
                else
                {
                    bRetVal = FALSE;
                }
            }
            break;
        case FILTER_CONFIG_DETS:
            {
                *lpData = (void*)&m_sFilterConfigured;
                bRetVal = TRUE;
            }
            break;
        case LOG_CONFIG_DETS:
        {
            *lpData = (void*)&m_sLogConfigDets;
            bRetVal = TRUE;
        }
        break;
        case REPLAY_CONFIG_DETS:
        {
            *lpData = (void*)&m_sReplayDetails;
            bRetVal = TRUE;
        }
        break;
        default:
            {
                *lpData = NULL;
                // not in scope
                bRetVal = FALSE;
            }
            break;
        }
    }
    else
    {
        *lpData = NULL;
        // invalid out-param..
        bRetVal = FALSE;
    }
    // Return the result
    return bRetVal;
}





/******************************************************************************/
/*  Function Name    :  bSetData                                              */
/*                                                                            */
/*  Input(s)         :  eParam : enumerator corresponding to the configuration*/
/*                               information                                  */
/*                      lpVoid : pointer to the data that needs to be stored  */
/*  Output           :  TRUE : if the method succeeds                         */
/*                      FALSE : if the method fails                           */
/*                                                                            */
/*  Functionality    :  This method searches for the element that corresponds */
/*                      to the enumerator value. If the element is found, then*/
/*                      its value is updated with the new value.              */
/*                      If the element is not found or if there is source     */
/*                      pointer is invalid, then the method exits.            */
/*                      This method does not archive the data.                */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :  27.11.2002, Update data only if m_bConfigInfoLoaded is*/
/*                                  TRUE                                      */
/*  Modifications on :  Amitesh Bharti                                        */
/*  Modifications on :  23.03.2003, Changed to take care of acceptance mask,  */
/*                      acceptance filter, controller mode, warning limit etc */
/*  Modifications on :  18.04.2003, Added code for Trace Window. For output   */
/*                                  and replay window the structure           */
/*                                  WINDOWPLACEMENT is used.                  */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  17.06.2003, Some more flag information is stored in,  */
/*                      configuration file:Disp and Log Format and Time Mode  */
/*                      Some name is changed: byBase -> byDisplayHexON        */
/*  Modifications By :  Krishnaswamy B.N                                      */
/*  Modifications on :  19.08.2003, Flag information for m_byLogOverWriteON   */
/*                      and m_byReplayMsgType is stored in.                   */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  05.04.2004, Added interface for Signal Watch list data*/
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  05.04.2004, Added code to save Signal Watch window and*/
/*                      Interpretation Window status                          */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  22.07.2004, Added interface for Message Buffer Data   */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  03.08.2004, Added code to set Ensure Visible Flag data*/
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  01.12.2004, Added signal graph list pointer Set function
/*  Modifications on :  09.03.2005, Modified controller information Set code to
/*                      support multi channel architecture                    */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  13.04.2005, Added code to set Tx window Splitter      */
/*                      position                                              */
/*  Modifications on :  13.04.2005, Added code to get Config file version and */
/*                      old & new filter list of message display              */
/*  Modifications By :  Pradeep Kadoor on 09.05.2008                          */
/*                      Code added to set values of SJW                       */
/*                      and PropDelay in Configuration file                   */
/*  Modifications By :  Pradeep Kadoor on 28.05.2009                          */
/*                      Modified code for 'case SIMSYS_LIST:'.                */
/*                      Coded added to copy the whole SSIMSYSARRAY linked     */
/*                      list into a new linked list instead of directly       */
/*                      copying the SSIMSYSARRAY pointer.                     */                        
/******************************************************************************/
BOOL CConfigDetails::bSetData(eCONFIGDETAILS  eParam, LPVOID lpVoid)
{
    ASSERT(FALSE);
    BOOL bRetVal = TRUE;
    
    // check input param
    if (lpVoid == NULL)
    {
        bRetVal = FALSE;
    }
    
    // is the configuration loaded?
    // only then go into the process of setting the data to the new
    // values..
    if (m_bConfigInfoLoaded == TRUE)
    {
        if (bRetVal == TRUE)
        {
            switch(eParam)
            {
            case MRU_C_FILE_NAME:
                {
                    m_omStrMruCFile.Empty();
                    m_omStrMruCFile = *(static_cast<CString*>(lpVoid));
                    m_dwModifiedVals |= defCHANGEDMRUCFILE;
                    m_bIsConfigurationModified = TRUE;
                }
                break;
            case LOG_FILE_NAME:
                {
                    // copy the log file name into the data member
                    m_omStrLogFilename.Empty();
                    m_omStrLogFilename = *(static_cast<CString*>(lpVoid));
                    m_dwModifiedVals |= defCHANGEDLOGFILE;
                    m_bIsConfigurationModified = TRUE;
                }
                break;
            case REPLAY_FILE_NAME:
                {
                    // copy the replay file name into the buffer
                    m_omStrReplayFilename.Empty();
                    m_omStrReplayFilename = *(static_cast<CString*>(lpVoid));
                    m_dwModifiedVals |= defCHANGEDREPLAYFILE;
                    m_bIsConfigurationModified = TRUE;
                }
                break;
            case DATABASE_FILE_NAME:
                {
                    if (m_pomaStrDatabaseFilename != NULL)
                    {
                        // copy the database file name into the buffer
                        m_pomaStrDatabaseFilename->RemoveAll();
                        m_pomaStrDatabaseFilename->
                            Append(*(static_cast<CStringArray*>(lpVoid)));
                        m_dwModifiedVals |= defCHANGEDDATABASEFILE;
                        m_bIsConfigurationModified = TRUE;
                    }
                }
                break;
            case MRU_DLL_FILE_NAME:
                {
                    // copy the most recently used DLL file name into the buffer
                    m_omStrMruDLLFilename.Empty();
                    m_omStrMruDLLFilename = *(static_cast<CString*>(lpVoid));
                    m_dwModifiedVals |= defCHANGEDDLLFILE;
                    m_bIsConfigurationModified = TRUE;
                }
                break;
            /*case MESSAGE_NAME:
                {
                    m_omStrMsgName.IsEmpty();
                    m_omStrMsgName = *(static_cast<CString*>(lpVoid));
                    m_dwModifiedVals |= defCHANGEDMSGNAME;
                    m_bIsConfigurationModified = TRUE;
                }
                break;*/
            /*case MESSAGE_ID:
                {
                    m_omStrMsgID.IsEmpty();
                    m_omStrMsgID = *(static_cast<CString*>(lpVoid));
                    m_dwModifiedVals |= defCHANGEDMSGID;
                    m_bIsConfigurationModified = TRUE;
                }
                break;*/
            case TOOLBAR_DETAILS:
                {
                    // get the tool bar button status information
                    PSTOOLBARINFO pSrc = static_cast<PSTOOLBARINFO>(lpVoid);
                    m_sToolBarButtonStatus.m_byMsgFilter = pSrc->m_byMsgFilter;
                    m_sToolBarButtonStatus.m_byLogFilter = pSrc->m_byLogFilter;
                    m_sToolBarButtonStatus.m_byLogging = pSrc->m_byLogging;
                    m_sToolBarButtonStatus.m_byMsgInterpret = 
                        pSrc->m_byMsgInterpret;
                    m_sToolBarButtonStatus.m_byOverwrite = pSrc->m_byOverwrite;
                    m_sToolBarButtonStatus.m_byDisplayHexON   = 
                        pSrc->m_byDisplayHexON;
                    m_sToolBarButtonStatus.m_byDisplayTimeMode = 
                        pSrc->m_byDisplayTimeMode;
                    m_sToolBarButtonStatus.m_byLogHexON       =
                        pSrc->m_byLogHexON;
                    m_sToolBarButtonStatus.m_byLogTimeMode    = 
                        pSrc->m_byLogTimeMode;
                    m_dwModifiedVals |= defCHAGNEDTOOLBARINF;
                    m_bIsConfigurationModified = TRUE;
                    
                    m_sToolBarButtonStatus.m_byLogOverWriteON = 
                        pSrc->m_byLogOverWriteON;
                    m_sToolBarButtonStatus.m_byReplayMsgType = 
                        pSrc->m_byReplayMsgType;
                    m_sToolBarButtonStatus.m_byEnsureVisible =
                        pSrc->m_byEnsureVisible;
                    
                }
                break;
            case CONTROLLER_DETAILS:
            {
                // baud rate information
                PSCONTROLER_DETAILS pSrc = 
                    static_cast<PSCONTROLER_DETAILS>(lpVoid);
                // packed value of bit timing register 0 
                // and bit timing register 1
                for (int nIndex = 0; nIndex < defNO_OF_CHANNELS; nIndex++)
                {
                    m_sControllerDetails[ nIndex ].m_nBTR0BTR1 = 
                        pSrc[ nIndex ].m_nBTR0BTR1;
                    // item number under focus
                    m_sControllerDetails[ nIndex ].m_nItemUnderFocus = 
                        pSrc[ nIndex ].m_nItemUnderFocus;
                    // acceptance code information
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrAccCodeByte1[0],
                        pSrc[ nIndex ].m_omStrAccCodeByte1[0]);
                    // acceptance code information
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrAccCodeByte2[0],
                        pSrc[ nIndex ].m_omStrAccCodeByte2[0]);
                    // acceptance code information
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrAccCodeByte3[0],
                        pSrc[ nIndex ].m_omStrAccCodeByte3[0]);
                    // acceptance code information
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrAccCodeByte4[0],
                        pSrc[ nIndex ].m_omStrAccCodeByte4[0]);
                    // acceptance mask information
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrAccMaskByte1[0],
                        pSrc[ nIndex ].m_omStrAccMaskByte1[0]);
                    // acceptance mask information
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrAccMaskByte2[0],
                        pSrc[ nIndex ].m_omStrAccMaskByte2[0]);
                    // acceptance mask information
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrAccMaskByte3[0],
                        pSrc[ nIndex ].m_omStrAccMaskByte3[0]);
                    // acceptance mask information
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrAccMaskByte4[0],
                        pSrc[ nIndex ].m_omStrAccMaskByte4[0]);
                    m_sControllerDetails[ nIndex ].m_bAccFilterMode = 
                        pSrc[ nIndex ].m_bAccFilterMode;
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrWarningLimit,
                        pSrc[ nIndex ].m_omStrWarningLimit);
                    m_sControllerDetails[ nIndex ].m_ucControllerMode = 
                        pSrc[ nIndex ].m_ucControllerMode;
                    // baudrate information
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrBaudrate,
                        pSrc[ nIndex ].m_omStrBaudrate);
                    // bit timing register 0 information
                    //m_sControllerDetails[ nIndex ].m_omStrBTR0 = 
                    //  pSrc[ nIndex ].m_omStrBTR0 ;
                    // bit timing register 1 information
                    //m_sControllerDetails[ nIndex ].m_omStrBTR1 = 
                    //  pSrc[ nIndex ].m_omStrBTR1;
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrCNF1,
                        pSrc[ nIndex ].m_omStrCNF1);
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrCNF2,
                        pSrc[ nIndex ].m_omStrCNF2);
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrCNF3,
                        pSrc[ nIndex ].m_omStrCNF3);
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrCNF3, 
                        pSrc[ nIndex ].m_omStrCNF3);
                    //PropDelay  and SJW
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrPropagationDelay, 
                        pSrc[ nIndex ].m_omStrPropagationDelay);
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrSjw,
                        pSrc[ nIndex ].m_omStrSjw);
                    // clock information
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrClock, 
                        pSrc[ nIndex ].m_omStrClock);
                    // sampling information
                    _tcscpy(m_sControllerDetails[ nIndex ].m_omStrSampling,
                        pSrc[ nIndex ].m_omStrSampling );
                }
                m_dwModifiedVals |= defCHANGEDCONTROLLERDETAILS;
                m_bIsConfigurationModified = TRUE;
            }
                break;
            case SEND_MULTI_MSGS:
                {
                    PSMSGBLOCKLIST psMsgBlockInfo = 
                        static_cast<PSMSGBLOCKLIST>(lpVoid);
                    // multiple message 
                    bRetVal = bSetMultiMsgInfo(psMsgBlockInfo);
                    
                    if (bRetVal == FALSE)
                    {
                        // set the default values
                        vReleaseMultiMsgInfo(m_psMsgBlockList);
                        m_psMsgBlockList = NULL;
                    }
                    
                    m_dwModifiedVals |= defCHANGEDMULTIMSG;
                    m_bIsConfigurationModified = TRUE;
                }
                break;
            case MSG_BLOCK_COUNT:
                {
                    UINT* punCount;
                    punCount = static_cast<UINT*>(lpVoid);
                    if (punCount != NULL)
                    {
                        m_unNumberOfMsgBlockCount = *(punCount);
                    }
                    else
                    {
                        bRetVal = FALSE;
                    }
                    if (bRetVal == FALSE)
                    {
                        m_unNumberOfMsgBlockCount = 0;
                    }
                    
                    m_dwModifiedVals |= defCHANGEDMSGBLOCKCOUNT;
                    m_bIsConfigurationModified = TRUE;
                }
                break;
            case MSG_ATTRIBUTES:
                {
                    PSMESSAGE_ATTRIB pMsgAttrib = 
                        static_cast<PSMESSAGE_ATTRIB>(lpVoid);
                    
                    // attributes of all the messages
                    bRetVal = bSetMsgAttrib(pMsgAttrib);
                    
                    if (bRetVal == FALSE)
                    {
                        // set the default values
                        vReleaseMsgAttrib(&m_sMessageAttributes);
                        vInitMsgAttributes();
                    }
                    m_dwModifiedVals |= defCHANGEDMSGATTRIBS;
                    m_bIsConfigurationModified = TRUE;
                }
                break;
            case SIGNAL_WATCH_LIST:
                {
                    PSSIGNALWATCHLIST psTemp = 
                        static_cast<PSSIGNALWATCHLIST>(lpVoid);
                    // Work around to pass NULL value
                    if ((const int)psTemp == -1)
                    {
                        // Empty Lsit
                        //m_psSignalWatchList = NULL;
                    }
                    else
                    {
                        // Modified List
                       // m_psSignalWatchList = psTemp;
                    }
                    m_bIsConfigurationModified = TRUE;
                }                   
                break;
            case SIMSYS_LIST:
                {
                    PSSIMSYSARRAY psSimsysTemp = 
                        static_cast<PSSIMSYSARRAY>(lpVoid);
                    // Work around to pass NULL value
                    if ((const int)psSimsysTemp == -1)
                    {
                        // Empty Lsit
                        m_psSimSysArray = NULL;
                    }
                    else
                    {
                        ///////////////////////////// Modified List/////////////////////////////////////////////////
                        //Copy the entire linked list into a new linked list "m_psSimSysArray"
                        if (m_psSimSysArray == NULL)
                        {
                            m_psSimSysArray = new SSIMSYSARRAY; // Create Head node
                        }
                        *(m_psSimSysArray) = *(psSimsysTemp); // 'operator=' is defined                        
                    }
                    m_bIsConfigurationModified = TRUE;
                }
                break;
                // Graph List
            case SIGNAL_GRAPH_LIST:
                {
                    // Graph List is accessed by the object pointer. So no need
                    // to copy object Unnecessarily. Just set the modified flag
                    // to popup save dialog while closing
                    m_bIsConfigurationModified = TRUE;
                }                   
                break;         
                
            case MSG_BUFFER_SIZE:
                {
                    PUINT punSize = static_cast<PUINT>(lpVoid);
                    m_unAppendBufferSize = punSize[defAPPEND_DATA_INDEX];
                    m_unOverWriteBufferSize = punSize[defOVERWRITE_DATE_INDEX];
                    m_unDisplayUpdateRate =
                        punSize[defDISPLAY_UPDATE_DATA_INDEX];
                    m_bIsConfigurationModified = TRUE;
                }
                break;
                // Window placement details
            case REPLAY_WND_PLACEMENT: // Replay window
                {
                    m_sReplayWndCoOrd = *(static_cast<WINDOWPLACEMENT *>(lpVoid));
                    m_bIsConfigurationModified = TRUE;
                }
                break;
                
            case OUT_WND_PLACEMENT: // Compile output window
                {
                    m_sOutputWndCoOrd = *(static_cast<WINDOWPLACEMENT *>(lpVoid));
                    m_bIsConfigurationModified = TRUE;
                }
                break;
                
            case NOTIFICATION_WND_PLACEMENT: // Trace Window
                {
                    m_sNotificWndCoOrd = *(static_cast<WINDOWPLACEMENT *>(lpVoid));
                    m_bIsConfigurationModified = TRUE;
                }
                break;
                
            case MSG_WND_PLACEMENT:     // Message window
                {
                    m_sMsgWndCoOrd = *(static_cast<WINDOWPLACEMENT *>(lpVoid));
                    m_bIsConfigurationModified = TRUE;
                }
                break;
                
            case SIGWATCH_WND_PLACEMENT:    // Signal watch window
                {
                    m_sSigWatchWindow = *(static_cast<WINDOWPLACEMENT *>(lpVoid));
                    m_bIsConfigurationModified = TRUE;
                }
                break;
                
            case MSGINTERP_WND_PLACEMENT: // Message Interpretation window
                {
                    m_sMsgInterpWindow = *(static_cast<WINDOWPLACEMENT *>(lpVoid));
                    m_bIsConfigurationModified = TRUE;
                }
                break;
                
            case GRAPH_WND_PLACEMENT: // Graph window
                {
                    m_sGraphWndCoOrd = *(static_cast<WINDOWPLACEMENT *>(lpVoid));
                    m_bIsConfigurationModified = TRUE;
                }
                break;
                
            case TX_WND_PLACEMENT:  // Tx configure window
                {
                    m_sTxWndCoOrd = *(static_cast<WINDOWPLACEMENT *>(lpVoid));
                    m_bIsConfigurationModified = TRUE;
                }
                break;
                
                // Splitter details
            case GRAPH_WND_SPLITTER_DATA: // Graph window splitter postion
                {
                    PSGRAPHSPLITTERDATA psData =
                        static_cast<PSGRAPHSPLITTERDATA>(lpVoid);
                    m_sGraphWndSplitterPos = *psData;
                    m_bIsConfigurationModified = TRUE;
                }
                break;
            case TX_MSG_WND_SPLITTER_DATA: // Tx configure splitter position
                {
                    PSTXMSGSPLITTERDATA psData =
                        static_cast<PSTXMSGSPLITTERDATA>(lpVoid);
                    m_sTxMsgWndSplitterPos = *psData;
                    m_bIsConfigurationModified = TRUE;
                }
                break;
                
                // New vesion of Filter for Message Dispaly
            case MSG_DISPLAY_FILTER_DETAILS:
                {
                    // get the pointer and copy the data
                    CModuleFilterArray * psData =
                        static_cast<CModuleFilterArray*>(lpVoid);
                    // Remove elements in the list if any
                    if (psData != NULL)
                    {
                        m_omMsgDisplayFilter.RemoveAll();
                        // Take display list and copy all
                        int nSize = psData->GetSize();
                        for (int nIndex = 0; nIndex < nSize; nIndex++)
                        {
                            m_omMsgDisplayFilter.Add( psData->GetAt( nIndex));
                        }
                        // Update Modification Flag
                        m_bIsConfigurationModified = TRUE;
                    }
                    else
                    {
                        bRetVal = FALSE;
                    }
                }
                break;
            default:
                {
                    // not in scope
                    bRetVal = FALSE;
                }
                break;
            }
        }
        else
        {
            // invalid out-param..
            bRetVal = FALSE;
        }
        
        if (bRetVal == TRUE)
        {
            m_bIsDirty = TRUE;
        }
    }
    
    if (bRetVal == FALSE)
    {
        if (theApp.m_bFromAutomation == FALSE)
        AfxMessageBox(_T("Error while updating data"), MB_OK);
    }
    return bRetVal;
}





/******************************************************************************/
/*  Function Name    :  nLoadConfiguration                                    */
/*                                                                            */
/*  Input(s)         :  omStrFilename : name of the configuration file whose  */
/*                      information needs to be loaded                        */
/*  Output           :  All the error codes returned by nIsCfgFileFound(..)   */
/*                      defCONFIG_FILE_READ_FAILED - if a file read operation */
/*                                                   fails                    */
/*                      defCONFIG_FILE_SUCCESS - if the method succeeds       */
/*                                                                            */
/*  Functionality    :  This method initializes all the members of the        */
/*                      CConfigDeatils object. The method nIsCfgFileFound is  */
/*                      first called to validate the input file. If the       */
/*                      validation succeeds, then the members are initialized */
/*                      with the values from the configuration file.          */
/*                      The method validates the file header and on failure it*/
/*                      exits returning the error code                        */
/*                      defCONFIG_FILE_HDR_CORRUPT.                           */
/*                      If the file header is correct, then the last modified */
/*                      stamp is verfied against the last modified information*/
/*                      stored in the file. If this check fails, then the     */
/*                      method exits returning the code defCONFIG_FILE_CORRUPT*/
/*                      The CArchive Serialization operation is used here to  */
/*                      extract the values. If a member has array elements,   */
/*                      then the same is allocated and then the information   */
/*                      extracted from the file. If at anytime the CArchive   */
/*                      operation throws an exception, then the method exits  */
/*                      at that point giving defCONFIG_FILE_READ_FAILED return*/
/*                      value.                                                */
/*                      All the members are reset and the allocated memory is */
/*                      released.                                             */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.12.2002                                            */
/*  Modifications on :  13.11.2002, Gopi, error code if nCfgFileFound(..)     */
/*                      added. Added code to release data members before      */
/*                      loading the new values.                               */
/*  Modification Date:  28.11.2002                                            */
/*  Modification By  :  Amarnath S                                            */
/*                      Previous configuration temporary file is deleted      */
/*                      before loading next configuration file                */
/*  Modification Date:  02.01.2003                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*                      Checksum validation is done before loading the file   */
/*  Modification Date:  19.01.2004                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*                      Close the handle in case there is mismatch in checksum*/
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  05.04.2004, Added release of Signal Watch list memory */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  02.08.2004, Removed the version check as it has been  */
/*                      moved inside nLoadStoreData.                          */
/*  Modifications on :  Pradeep Kadoor on 12.06.2009.                         */
/*                      Avoided messsage box by first checking whether        */  
/*                      file exists or not.                                   */
/******************************************************************************/
INT  CConfigDetails::
nLoadConfiguration(CString& omStrFilename/*= defDEFAULTCFGFILE*/)
{
    CMainFrame* pMainFrm = (CMainFrame*)theApp.m_pMainWnd;
    //CSimSysWnd* pSimSysWnd = NULL;
    //CSimSysTreeView* pSimSysTree = NULL;
    // If the user loads configuration file,
    // make sure the prev config temp file is deleted
    if (m_omstrTempFilename.IsEmpty() == FALSE)
    {
        // delete the temporary file
        ::DeleteFile(m_omstrTempFilename);
        m_omstrTempFilename = defEMPTYSTR;
    }
    
    UCHAR ucCheckSum        = 0;
    UCHAR ucCheckSumInFile  = 0;
    BOOL bRet = FALSE;
    UINT unErrorCode = defCONFIG_FILE_NOT_FOUND;
    //Check if Configuration file is present or not.
    struct _finddata_t fileinfo;
    if (_findfirst( omStrFilename, &fileinfo) != -1L)
	{
        // file exists
        // Compute the checksum value in file
		bRet = CComputeCheckSum::bGetCheckSum(omStrFilename, &ucCheckSum,&ucCheckSumInFile);
        // first check if the file exists
        unErrorCode = nIsCfgFileFound(omStrFilename, TRUE); 
	}
    if (unErrorCode == defCONFIG_FILE_SUCCESS)
    {
         m_bConfigInfoLoaded = FALSE;
        // release the existing members
        vReleaseMultiMsgInfo(m_psMsgBlockList);
        m_psMsgBlockList = NULL;
        // Release signal Watch list memory
        vReleaseSignalWatchListMemory();
        // Release simulated system list memory
        vReleaseSimSysListMemory();
        //m_pSimSysNodeInfo = theApp.pomGetSimSysNodeInfo();
        //// Release simulated system info memory
        //if (m_pSimSysNodeInfo != NULL)
        //{
        //    m_pSimSysNodeInfo->vReleaseSimSysInfo();
        //}
        vReleaseMsgAttrib(&m_sMessageAttributes);
        vReleaseMsgFilterDetails(&m_sMsgFilterDetails);
        
        // Now that the configuration file has been found, validate the header
        // and the checksum to ascertain that the file has not been 
        // corrupted after the last time it was saved.
        // extract the information from the file
        if ((ucCheckSum == ucCheckSumInFile) && (bRet == TRUE))
        {
            // Load the config data
            unErrorCode = nLoadStoreData(CArchive::load);
            // Check for success. If it is success then store the name and
            // set valid configuration flag
            if (unErrorCode == defCONFIG_FILE_SUCCESS)
            {
                m_bConfigInfoLoaded = TRUE;
                m_omstrConfigFilename = omStrFilename;
               /* if (m_pSimSysNodeInfo != NULL)
                {
                    m_pSimSysNodeInfo->bPopulateSimSysInfo();
                }*/
                
            }
            if (pMainFrm != NULL)
            {/*
                pSimSysWnd = pMainFrm->pomGetSimSysWnd();
                pSimSysTree = pMainFrm->podGetSimSysTreeView();*/
            }
            // to indicate to the tree view about the new dlls built.
            /*if (pSimSysWnd != NULL)
            {
                
                if (pSimSysTree != NULL)
                    pSimSysTree->bPopulateTree();
            }
            CExecuteManager::ouGetExecuteManager().vClearOutputWnd();*/
            
        }
        else
        {
            // file has been modified by external sources after the last
            // modification by the application.
            unErrorCode = defCONFIG_FILE_CORRUPT;
            if (m_hConfigFile != NULL)
            {
                ::CloseHandle(m_hConfigFile);
                m_hConfigFile = NULL;
            }
        }
    }
    return unErrorCode;
}

/******************************************************************************/
/*  Function Name    :  nSaveConfiguration                                    */
/*                                                                            */
/*  Input(s)         :  omStrCfgFilename : name of the configuration file in  */
/*                      which the information needs to be saved.              */
/*  Output           :  defCONFIG_FILE_OPEN_FAILED - if file cannot be opened */
/*                      defCONFIG_FILE_WRITE_FAILED - if the write operation  */
/*                                                    fails                   */
/*                      defCONFIG_FILE_CLOSE_FAILED - if the file close fails */
/*                                                                            */
/*  Functionality    :  This method opens the file as given in the input      */
/*                      parameter. If the m_bIsDirty is TRUE, then the        */
/*                      contents are written into the file again. Note that   */
/*                      all the contents are written into irrespective of the */
/*                      number of the items that could have changed. The file */
/*                      header information is first written into the file,    */
/*                      followed by the all the configuration items and       */
/*                      finally the last modified date and time information is*/
/*                      written into the file in the format dd:mm:yyyy:hh:mm. */
/*                      The file is then closed.                              */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :  Gopi, 12.11.2002, Added checking for an empty filename*/
/*                      for the temporary file before copying from or         */
/*                      deleting it.                                          */
/*                      Gopi, 13.11.2002, Close the file handle               */
/*  Modifications    :  Raja N on 01.08.2004, Modified the return value check */
/*                      of function nLoadStoreData from True/Flase to int as  */
/*                      that function's return type changed to int from BOOL  */
/******************************************************************************/
INT  CConfigDetails::nSaveConfiguration(CString& omStrCfgFilename)
{
    UINT unErrorCode = defCONFIG_FILE_SUCCESS;
    
    // save this file only if the file is saved as a new one, or there has been 
    // an updation in the data
    if ((omStrCfgFilename != m_omstrConfigFilename) || (m_bIsDirty == TRUE))
    {
        // if filename is empty, then it means that the data should be saved
        // into the last loaded file...
        if (omStrCfgFilename.IsEmpty() == TRUE)
        {
            omStrCfgFilename = m_omstrConfigFilename;
        }
        // first check if the file exists
        unErrorCode = nIsCfgFileFound(omStrCfgFilename, FALSE); 
        
        if (unErrorCode == defCONFIG_FILE_SUCCESS)
        {
            // Now that the configuration file has been found, validate the 
            // header and the last modified time to ascertain that the file has
            // not been corrupted after the last time it was saved.
            // extract the information from the file
            if (nLoadStoreData(CArchive::store) != defCONFIG_FILE_SUCCESS)
            {
                if (theApp.m_bFromAutomation == FALSE)
                {
                    AfxMessageBox(defSAVECONFIGERRSTR, MB_OK);
                }
                if (m_omstrTempFilename.IsEmpty() == FALSE)
                {
                    COPYFILE(m_omstrTempFilename, m_omstrConfigFilename);
                }
            }
            else
            {
                // reset the dirty flag.
                m_bIsDirty = FALSE;
                m_bIsConfigurationModified = FALSE;
                CString strError;
                // Set the checksum as computed.
                if (CComputeCheckSum::COM_bSetCheckSum(omStrCfgFilename,&m_ucCheckSum, strError) == FALSE)
                {
                    if (m_omstrTempFilename.IsEmpty() == FALSE)
                    {
                        COPYFILE(m_omstrTempFilename, m_omstrConfigFilename);
                    }
                }
            }
        }
    }
    
    if (m_omstrTempFilename.IsEmpty() == FALSE)
    {
        // delete the temporary file
        ::DeleteFile(m_omstrTempFilename);
    }
    
    return unErrorCode;
}





/******************************************************************************/
/*  Function Name    :  nNewConfiguration                                     */
/*                                                                            */
/*  Input(s)         :  omStrFilename : name of the configuration file which  */
/*                                      should be created.                    */
/*  Output           :  defCONFIG_FILE_OPEN_FAILED - if file cannot be opened */
/*                      defCONFIG_FILE_WRITE_FAILED - if the write operation  */
/*                                                    fails                   */
/*                      defCONFIG_FILE_CLOSE_FAILED - if the file close fails */
/*                                                                            */
/*  Functionality    :  This method initializes all the members of the        */
/*                      CConfigDetails object with default values. This method*/
/*                      is called when user wants to create a fresh set of    */
/*                      data. Note that the window coordinates are all -1 and */
/*                      hence the calculation of the coordinates is the       */
/*                      responsibility of the particular class needing that   */
/*                      information. Also, no entries for messages are present*/
/*                      All data structures are first released, and only then */
/*                      the default values are entered.                       */
/*                      As a new file is being created, no backup is possible.*/
/*                      Hence, the temporary file is not created.             */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  12.11.2002                                            */
/*  Modifications on :                                                        */
/******************************************************************************/
INT  CConfigDetails::nNewConfiguration(CString& omStrFilename)
{
    int nError = defCONFIG_FILE_SUCCESS;
    //m_pSimSysNodeInfo = theApp.pomGetSimSysNodeInfo();
    //CMainFrame* pMainFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
    //CSimSysWnd* pSimSysWnd = NULL;
    //CSimSysTreeView* pSimSysTree = NULL;

    // pre caution - the structures already could be containing values, hence
    // release them..
    vReleaseMultiMsgInfo(m_psMsgBlockList);
    m_psMsgBlockList = NULL;
    vReleaseSignalWatchListMemory();
    // Release simulated system list memory
    //vReleaseSimSysListMemory();
    // Release simulated system info memory
    //m_pSimSysNodeInfo->vReleaseSimSysInfo();
    vReleaseMsgAttrib(&m_sMessageAttributes);
    vReleaseMsgFilterDetails(&m_sMsgFilterDetails);
    
    vInitDefaultValues();
    
    // data has changed.
    m_bIsDirty = TRUE;
    // a new file is being created, hence no temp file can be created
    m_omstrTempFilename = defEMPTYSTR;
    
    nError = nSaveConfiguration(omStrFilename);
    
    if (nError == defCONFIG_FILE_SUCCESS)
    {
        // this is the name of the configuration file whose information is 
        // currently loaded...
        m_omstrConfigFilename = omStrFilename;
        //if (m_pSimSysNodeInfo != NULL)
        //    m_pSimSysNodeInfo->bPopulateSimSysInfo();
        //
        //
        //if (pMainFrm != NULL)
        //{
        //    pSimSysWnd = pMainFrm->pomGetSimSysWnd();
        //    pSimSysTree = pMainFrm->podGetSimSysTreeView();
        //}
        //// to indicate to the tree view about the new dlls built.
        //if (pSimSysWnd != NULL)
        //{
        //    
        //    if (pSimSysTree != NULL)
        //        pSimSysTree->bPopulateTree();
        //}
        //CExecuteManager::ouGetExecuteManager().vClearOutputWnd();
    }
    
    return nError;
}






/******************************************************************************/
/*  Function Name    :  nLoadStoreData                                        */
/*                                                                            */
/*  Input(s)         :  nMode : mode in which the CArchive object should      */
/*                              function                                      */
/*                                                                            */
/*  Output           :  TRUE : if the method is successful                    */
/*                      FALSE : if any method throws an exception             */
/*                                                                            */
/*  Functionality    :  This method loads/stores the information from/to the  */
/*                      configuration file. The data is extracted in the      */
/*                      following order, header information, Toolbar Button   */
/*                      Status, Log filename, Replay filename, Database       */
/*                      filename, DLL filename, Message Attriutes, Window     */
/*                      Co-ordinates, Baudrate Details, Send Multiple Message */
/*                      Information, Message Filter Details, Last modified    */
/*                      date and time.                                        */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :  13.11.2002, Gopi, Validate m_hConfigFile before using */
/*                      it. Moved construction of CArchive object into        */
/*                      try-catch block.                                      */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  17.06.2003, Some more flag information is stored in,  */
/*                      configuration file:Disp and Log Format and Time Mode  */
/*                      Some name is changed: byBase -> byDisplayHexON        */
/*  Modifications By :  Krishnaswamy B.N                                      */
/*  Modifications on :  19.08.2003, Flag information for m_byLogOverWriteON   */
/*                      and m_byReplayMsgType added                           */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  05.04.2004, Added code to save Signal Watch list data */
/*                      and modified App version check. The previous config   */
/*                      version is assuemed as 2.2 and the current version is */
/*                      2.3. 2.3 will have SW list, window position of Signal */
/*                      Watch window and Interpretation window                */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  22.07.2004, Added interface for Message Buffer Data   */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  01.08.2004, Modified the function return type as int. */
/*                      and added checks for wrong config file before loading */
/*                      the configuration file. Removed basic features version*/
/*                      check before load/store.                              */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  01.12.2004, Added serialisation of Graph list data    */
/*  Modifications    :  Raja N on 02.05.2005, Added code to serialize splitter*/
/*                      window positions                                      */
/*  Modifications    :  Raja N on 20.07.2005, Added code to serialize filter  */
/*                      log and replay file details                           */
/*  Modifications    :  Anish,for storing Multiple file names as CStringArray
                        Pointer
/*  Modifications    :  Anish on 24.01.07
                        deleted the database name array pointer after loading 
                        to remove memory leak.
/******************************************************************************/
int CConfigDetails::nLoadStoreData(UINT nMode)
{
    //It will be used in other functions called from here
    //m_pSimSysNodeInfo = theApp.pomGetSimSysNodeInfo();
    int nRetVal = defCONFIG_FILE_SUCCESS;
    BOOL bIsLoading = FALSE;
    
    // associate the file handle with an archive object. 
    // Set the operation of the archive object to load
    
    // create the archive for the CFile object...
    // The casting is in contradiction to the rules of coding because
    // 1. The CreateFile(...) method used to get the handle of the file
    //    returns a HANDLE to the file and not HFILE.
    // 2. CFile constructor takes a HFILE as its input parameter, while MSDN
    //    documentation mentions that the value as returned by CreateFile(...) 
    //    SDK method should be used. Usage of the return value by 
    //    CreateFile(...) without casting results in a compiler error. 
    // 3. The recommended usage of static_cast<T>(v) also results in a 
    //    compilation error.
    // 4. MSDN documentation recommends the explicit conversion in these cases.
    //    Compiler error C2664
    
    if (m_hConfigFile != NULL)
    {
#if _MFC_VER <= 0x0600
        CFile   oConfigFile((INT_PTR)m_hConfigFile);
#else
        CFile   oConfigFile(m_hConfigFile);
#endif
        
        try
        {
            // the buffer size is assumed to be 4096 bytes.If need be this value
            // should be increased.
            CArchive oCfgArchive(&oConfigFile, nMode);
            
            if (oCfgArchive.IsLoading())
            {
                bIsLoading = TRUE;
                // extract the header information
                oCfgArchive >> m_fAppVersion;
                oCfgArchive >> m_omStrCopyright;
                
                // From this version onwards all successor
                //config files are supported
                FLOAT fSupportedVersion =
                    static_cast <FLOAT> ( defBASE_CONF_VERSION);
                // verify the header 
                // If the config version is not latest version and
                // not previous version too or the header is not correct
                // then declare it as unsupported or coppupted header file
                if ((m_fAppVersion < fSupportedVersion)
                    || m_omStrCopyright != defCOPYRIGHT)
                {
                    // there is a corruption in th header information
                    nRetVal = defCONFIG_FILE_HDR_CORRUPT;
                }
                else
                {
                    oCfgArchive >> m_sToolBarButtonStatus.m_byMsgFilter;
                    oCfgArchive >> m_sToolBarButtonStatus.m_byLogFilter;
                    oCfgArchive >> m_sToolBarButtonStatus.m_byLogging;
                    oCfgArchive >> m_sToolBarButtonStatus.m_byMsgInterpret;
                    oCfgArchive >> m_sToolBarButtonStatus.m_byOverwrite;
                    oCfgArchive >> m_sToolBarButtonStatus.m_byDisplayTimeMode;
                    oCfgArchive >> m_sToolBarButtonStatus.m_byDisplayHexON;
                    oCfgArchive >> m_sToolBarButtonStatus.m_byLogTimeMode;
                    oCfgArchive >> m_sToolBarButtonStatus.m_byLogHexON;
                    
                    oCfgArchive >> m_sToolBarButtonStatus.m_byLogOverWriteON;
                    oCfgArchive >> m_sToolBarButtonStatus.m_byReplayMsgType;
                    // Get Ensure Visible flag status
                    oCfgArchive >> m_sToolBarButtonStatus.m_byEnsureVisible;
                    
                    oCfgArchive >> m_omStrMruCFile;
                    oCfgArchive >> m_omStrLogFilename;
                    oCfgArchive >> m_omStrReplayFilename;
                    if (m_fAppVersion >= defMULTI_DATABASE_VERSION)
                    {
                        CStringArray *pouTempArray ;
                        oCfgArchive >> pouTempArray;
                        m_pomaStrDatabaseFilename->RemoveAll();
                        m_pomaStrDatabaseFilename->Append(*pouTempArray);
                        delete pouTempArray;
                    }
                    else
                    {
                        CString omStrDBPath;
                        oCfgArchive >> omStrDBPath;
                        m_pomaStrDatabaseFilename->RemoveAll();
                        m_pomaStrDatabaseFilename->Add(omStrDBPath);
                    }
                    oCfgArchive >> m_omStrMruDLLFilename;
                    oCfgArchive >> m_omStrMsgName;
                    oCfgArchive >> m_omStrMsgID;
                    //  m_pSimSysNodeInfo->bPopulateSimSysInfo();
                }
            }
            
            if (oCfgArchive.IsStoring())
            {
                // extract the header information
                m_fAppVersion = static_cast<FLOAT> (defAPPVERSION);
                oCfgArchive << m_fAppVersion;
                oCfgArchive << m_omStrCopyright;
                
                oCfgArchive << m_sToolBarButtonStatus.m_byMsgFilter;
                oCfgArchive << m_sToolBarButtonStatus.m_byLogFilter;
                oCfgArchive << m_sToolBarButtonStatus.m_byLogging;
                oCfgArchive << m_sToolBarButtonStatus.m_byMsgInterpret;
                oCfgArchive << m_sToolBarButtonStatus.m_byOverwrite;
                oCfgArchive << m_sToolBarButtonStatus.m_byDisplayTimeMode;
                oCfgArchive << m_sToolBarButtonStatus.m_byDisplayHexON;
                oCfgArchive << m_sToolBarButtonStatus.m_byLogTimeMode;
                oCfgArchive << m_sToolBarButtonStatus.m_byLogHexON;
                
                oCfgArchive << m_sToolBarButtonStatus.m_byLogOverWriteON;
                oCfgArchive << m_sToolBarButtonStatus.m_byReplayMsgType;
                // Update Ensure Visible Flag
                oCfgArchive << m_sToolBarButtonStatus.m_byEnsureVisible;
                oCfgArchive << m_omStrMruCFile;
                oCfgArchive << m_omStrLogFilename;
                oCfgArchive << m_omStrReplayFilename;
                oCfgArchive << m_pomaStrDatabaseFilename;
                oCfgArchive << m_omStrMruDLLFilename;
                oCfgArchive << m_omStrMsgName;
                oCfgArchive << m_omStrMsgID;
                
            }
            if (nRetVal == defCONFIG_FILE_SUCCESS)
            {
                // Load Controller configuration
                vLoadStoreBaudRateDetails(oCfgArchive);
                // Load Window postion info
                vLoadStoreWndCoords(oCfgArchive);
                // Load splitter info only if the app version is > 2.4
                // That is from Multi channel version onwards
                vLoadStoreSplitterPostion( oCfgArchive);
                
                // Load Message Attributes, Tx Message info and Filter
                // Details
                if ((bLoadStoreMsgAttributes(oCfgArchive) != TRUE) ||
                    (bLoadStoreMultiMsgInfo(oCfgArchive) != TRUE)  ||
                    (bLoadStoreMsgFilterDetails(oCfgArchive) != TRUE))
                {
                    nRetVal = defCONFIG_FILE_CORRUPT;
                }
                // Serialize Display Filter info
                if ( nRetVal == defCONFIG_FILE_SUCCESS &&
                    bLoadStoreDisplayFilterInfo( oCfgArchive) != TRUE)
                {
                    
                    nRetVal = defCONFIG_FILE_CORRUPT;
                }
                // Serialize Log File Info
                if ( nRetVal == defCONFIG_FILE_SUCCESS &&
                    bLoadStoreLogFileInfo( oCfgArchive) != TRUE)
                {
                    
                    nRetVal = defCONFIG_FILE_CORRUPT;
                }
                
                // Serialize Replay File Info
                if ( nRetVal == defCONFIG_FILE_SUCCESS &&
                    bLoadStoreReplayFileInfo( oCfgArchive) != TRUE)
                {
                    
                    nRetVal = defCONFIG_FILE_CORRUPT;
                }
                
                // Get the Signal Watch Information
                if (nRetVal == defCONFIG_FILE_SUCCESS &&
                    bLoadStoreSignalWatchList(oCfgArchive) != TRUE)
                {
                    nRetVal = defCONFIG_FILE_CORRUPT;
                }
                
                // Get the simulated system Information
                
                if (nRetVal == defCONFIG_FILE_SUCCESS &&
                    bLoadStoreSimSysList(oCfgArchive) != TRUE)
                {
                    nRetVal = defCONFIG_FILE_CORRUPT;
                }
                               
                // Get Message Buffer Parameters
                if (oCfgArchive.IsLoading())
                {
                    oCfgArchive >> m_unAppendBufferSize;
                    oCfgArchive >> m_unOverWriteBufferSize;
                    oCfgArchive >> m_unDisplayUpdateRate;
                }
                else
                {
                    oCfgArchive << m_unAppendBufferSize;
                    oCfgArchive << m_unOverWriteBufferSize;
                    oCfgArchive << m_unDisplayUpdateRate;
                }
                
                // Serialize Graph List Information
                if ( oCfgArchive.IsLoading())
                {
                    // Load graph details only if the conf file version > 2.3
                    if (m_fAppVersion >= 
                        static_cast<float>( defSIGNAL_GRAPH_VERSION))
                    {
                        // Load it from the config file
                        m_odGraphList.nSerialize( oCfgArchive);
                        // Reset the channel number if it is older version
                        // configuration file (V2.4)
                        // Channel support comes from V2.5 onwards
                        if (m_fAppVersion == 
                            static_cast<float>( defSIGNAL_GRAPH_VERSION))
                        {
                            int nSize =
                                m_odGraphList.m_omElementList.GetSize();
                            for (int nIndex = 0; nIndex < nSize; nIndex++)
                            {
                                // Reset the channel to 1
                                m_odGraphList.m_omElementList[ nIndex ].
                                    m_nFrameFormat = 1;
                            }
                        }
                    }
                    else
                    {
                        // Use default values
                        vInitGraphParamenters();
                    }
                }
                else
                {
                    // Save Graph details irrespective of conf file
                    // version
                    m_odGraphList.nSerialize( oCfgArchive);
                }                
                
                // get the checksum information.
                if (oCfgArchive.IsLoading())
                {
                    oCfgArchive >> m_ucCheckSum;
                }
                
                if (oCfgArchive.IsStoring())
                {
                    // Store the chechsum as zero.
                    oCfgArchive << m_ucCheckSum;
                }
            }
            // close the archive
            oCfgArchive.Close();
        }
        catch(CArchiveException* poArchExcep)
        {
            nRetVal = defCONFIG_FILE_CORRUPT;
            poArchExcep->Delete();
        }
        catch(CFileException* poFileExcep)
        {
            nRetVal = defCONFIG_FILE_CORRUPT;
            poFileExcep->Delete();
        }
        catch(CMemoryException* poMemExcep)
        {
            nRetVal  = defCONFIG_FILE_OPEN_FAILED;
            poMemExcep->Delete();
        }
        // close the file
        oConfigFile.Close();
        
        m_hConfigFile = NULL;
    }
    else
    {
        nRetVal = defCONFIG_FILE_OPEN_FAILED;
    }
    return nRetVal;
}
/******************************************************************************/
/*  Function Name    :  vInitDefaultValues                                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  All the members of the class are initialized with     */
/*                      default values. This method is called when the user   */
/*                      wants to create a new configuration file.             */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  12.11.2002                                            */
/*  Modifications on :  01.01.2003, Initialise                                */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  17.06.2003, some data members are initialised for     */
/*                      problem when the loading fails, all data members get  */
/*                      re-initialised to default other then initialisation in*/
/*                      constructor list                                      */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  22.07.2004, Added interface for Message Buffer Data   */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  03.08.2004, Changed default log file name from empty  */
/*                      string to default file name                           */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  10.12.2004, Added default value init for Graph data   */
/*  Modifications    :  Raja N on 02.05.2005, Added code to init splitter     */
/*                      window positions                                      */
/*  Modifications    :  Raja N on 20.07.2005, Added code to init filter, log  */
/*                      and replay managers                                   */
/*  Modifications    :  Anish on 08.02.2007, to init datatbase file string    */
/*                      array                                                 */
/******************************************************************************/
void CConfigDetails::vInitDefaultValues()
{
    // always clean the members..
    vResetAll();
    
    CString ostrTemp = defEMPTYSTR;
    // Default log file name
    m_omStrLogFilename = def_DEFAULT_LOGFILENAME;
    // MRU C file name
    m_omStrMruCFile      = defEMPTYSTR;
    // replay file name
    m_omStrReplayFilename = defEMPTYSTR;
    
    // database file name
    m_pomaStrDatabaseFilename->RemoveAll();
    
    // mru dll name
    m_omStrMruDLLFilename = defEMPTYSTR;
    
    // copy right info
    m_omStrCopyright = defCOPYRIGHT;
    
    // version info
    m_fAppVersion = static_cast <float> (defAPPVERSION);
    
    
    m_bIsDirty  = FALSE;
    
    m_hConfigFile = NULL ;
    
    m_omstrConfigFilename = defEMPTYSTR ;
    
    m_omstrTempFilename = defEMPTYSTR;
    
    m_omStrMsgName = defEMPTYSTR ;
    
    m_omStrMsgID = defEMPTYSTR;
    
    m_ucCheckSum = 0;
    
    // Initialise window postion members
    vInitWndCoords();
    // initialise Splitter position members
    vInitSplitterPostions();
    // baud rate defaults
    vInitBaudRateDetails();
    
    // message attributes defaults
    vInitMsgAttributes();
    
    // filter defauls
    vInitFilterDetails();
    // Init Log Manager
    //CLogManager::ouGetLogManager().vInitLogManager();
    // Init Replay Manager
    //CReplayManager::ouGetReplayManager().vInitReplayManager();
    // tool bar defaults 
    vInitToolbarInfo();
    // Initialise the count of message block
    m_unNumberOfMsgBlockCount = 0;
    // values are loaded...
    m_bConfigInfoLoaded = TRUE;
    // Init default value of the buffer size
    m_unAppendBufferSize = defDEF_APPEND_BUFFER_SIZE;
    m_unOverWriteBufferSize = defDEF_OVERWRITE_BUFFER_SIZE;
    m_unDisplayUpdateRate = defDEF_DISPLAY_UPDATE_TIME;
    // Update Graph Parameters
    vInitGraphParamenters();
}
/******************************************************************************/
/*  Function Name    :  vInitWndCoords                                        */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  All the members of the class are initialized with     */
/*                      default values. This method is called when the user   */
/*                      wants to create a new configuration file.             */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  12.11.2002                                            */
/*  Modifications on :  18.04.2003, Added code for Trace Window. For output   */
/*                                  and replay window the structure           */
/*                                  WINDOWPLACEMENT is used.                  */
/*  Modifications on :  08.05.2003, Initialisation of window coordinate       */
/*                                  ptMaxPosition.x is corrected.             */
/*  Modifications on :  13.04.2004, Added initialisation of Signal Watch and  */
/*                                  Mesage Interpretation Windows             */
/*  Modifications    : Raja N on 02.05.2005, Added code to init graph and Tx  */
/*                     message window postions. Remomed fixed position of     */
/*                     windows and assigned -1 to denote initial value        */
/******************************************************************************/
void CConfigDetails::vInitWndCoords()
{
    // window coords
    // window placement structure info for msg wnd
    m_sMsgWndCoOrd.length = sizeof(WINDOWPLACEMENT);
    m_sMsgWndCoOrd.flags = defMSGWNDFLAGS;
    m_sMsgWndCoOrd.showCmd = defWNDSHOWCMD;
    m_sMsgWndCoOrd.ptMinPosition.x = -1;
    m_sMsgWndCoOrd.ptMinPosition.y = -1;
    m_sMsgWndCoOrd.ptMaxPosition.x = -1;
    m_sMsgWndCoOrd.ptMaxPosition.y = -1;
    m_sMsgWndCoOrd.rcNormalPosition.bottom = -1;
    m_sMsgWndCoOrd.rcNormalPosition.left = -1;
    m_sMsgWndCoOrd.rcNormalPosition.top = -1;
    m_sMsgWndCoOrd.rcNormalPosition.right = -1;
    
    // output window
    m_sOutputWndCoOrd.length = sizeof(WINDOWPLACEMENT);
    m_sOutputWndCoOrd.flags = defMSGWNDFLAGS;
    m_sOutputWndCoOrd.showCmd = defWNDSHOWCMD;
    m_sOutputWndCoOrd.ptMinPosition.x = -1;
    m_sOutputWndCoOrd.ptMinPosition.y = -1;
    m_sOutputWndCoOrd.ptMaxPosition.x = -1;
    m_sOutputWndCoOrd.ptMaxPosition.y = -1;
    
    m_sOutputWndCoOrd.rcNormalPosition.bottom = -1;
    m_sOutputWndCoOrd.rcNormalPosition.left = -1;
    m_sOutputWndCoOrd.rcNormalPosition.top = -1;
    m_sOutputWndCoOrd.rcNormalPosition.right = -1;
    
    // replay window
    m_sReplayWndCoOrd.length = sizeof(WINDOWPLACEMENT);
    m_sReplayWndCoOrd.flags = defMSGWNDFLAGS;
    m_sReplayWndCoOrd.showCmd = defWNDSHOWCMD;
    m_sMsgWndCoOrd.ptMinPosition.x = -1;
    m_sReplayWndCoOrd.ptMinPosition.y = -1;
    m_sReplayWndCoOrd.ptMaxPosition.x = -1;
    m_sReplayWndCoOrd.ptMaxPosition.y = -1;
    
    m_sReplayWndCoOrd.rcNormalPosition.bottom = -1;
    m_sReplayWndCoOrd.rcNormalPosition.left = -1;
    m_sReplayWndCoOrd.rcNormalPosition.top = -1;
    m_sReplayWndCoOrd.rcNormalPosition.right = -1;
    
    // trace window
    m_sNotificWndCoOrd.length = sizeof(WINDOWPLACEMENT);
    m_sNotificWndCoOrd.flags = defMSGWNDFLAGS;
    m_sNotificWndCoOrd.showCmd = defWNDSHOWCMD;
    m_sNotificWndCoOrd.ptMinPosition.x = -1;
    m_sNotificWndCoOrd.ptMinPosition.y = -1;
    m_sNotificWndCoOrd.ptMaxPosition.x = -1;
    m_sNotificWndCoOrd.ptMaxPosition.y = -1;
    
    m_sNotificWndCoOrd.rcNormalPosition.bottom = -1;
    m_sNotificWndCoOrd.rcNormalPosition.left = -1;
    m_sNotificWndCoOrd.rcNormalPosition.top = -1;
    m_sNotificWndCoOrd.rcNormalPosition.right = -1;
    
    // Signal Watch Window
    m_sSigWatchWindow.length = sizeof(WINDOWPLACEMENT);
    m_sSigWatchWindow.flags = WPF_SETMINPOSITION;
    m_sSigWatchWindow.showCmd = defWNDSHOWCMD;
    m_sSigWatchWindow.ptMinPosition.x = -1;
    m_sSigWatchWindow.ptMinPosition.y = -1;
    m_sSigWatchWindow.ptMaxPosition.x = -1;
    m_sSigWatchWindow.ptMaxPosition.y = -1;
    
    m_sSigWatchWindow.rcNormalPosition.bottom = -1;
    m_sSigWatchWindow.rcNormalPosition.left = -1;
    m_sSigWatchWindow.rcNormalPosition.top = -1;
    m_sSigWatchWindow.rcNormalPosition.right = -1;
    
    // Message Interpretation Window
    m_sMsgInterpWindow.length = sizeof(WINDOWPLACEMENT);
    m_sMsgInterpWindow.flags = WPF_SETMINPOSITION;
    m_sMsgInterpWindow.showCmd = defWNDSHOWCMD;
    m_sMsgInterpWindow.ptMinPosition.x = -1;
    m_sMsgInterpWindow.ptMinPosition.y = -1;
    m_sMsgInterpWindow.ptMaxPosition.x = -1;
    m_sMsgInterpWindow.ptMaxPosition.y = -1;
    
    m_sMsgInterpWindow.rcNormalPosition.bottom = -1;
    m_sMsgInterpWindow.rcNormalPosition.left = -1;
    m_sMsgInterpWindow.rcNormalPosition.right = -1;
    m_sMsgInterpWindow.rcNormalPosition.top = -1;
    
    // Init Graph Window Coordinates
    m_sGraphWndCoOrd.length = sizeof(WINDOWPLACEMENT);
    m_sGraphWndCoOrd.flags = WPF_SETMINPOSITION;
    m_sGraphWndCoOrd.showCmd = defWNDSHOWCMD;
    m_sGraphWndCoOrd.ptMinPosition.x = -1;
    m_sGraphWndCoOrd.ptMinPosition.y = -1;
    m_sGraphWndCoOrd.ptMaxPosition.x = -1;
    m_sGraphWndCoOrd.ptMaxPosition.y = -1;
    // Assign Boundary Rect
    m_sGraphWndCoOrd.rcNormalPosition.top = -1;
    m_sGraphWndCoOrd.rcNormalPosition.left = -1;
    m_sGraphWndCoOrd.rcNormalPosition.right = -1;
    m_sGraphWndCoOrd.rcNormalPosition.bottom = -1;
    
    // Tx Window Postion
    m_sTxWndCoOrd.length = sizeof(WINDOWPLACEMENT);
    m_sTxWndCoOrd.flags = WPF_SETMINPOSITION;
    m_sTxWndCoOrd.showCmd = defWNDSHOWCMD;
    m_sTxWndCoOrd.ptMinPosition.x = -1;
    m_sTxWndCoOrd.ptMinPosition.y = -1;
    m_sTxWndCoOrd.ptMaxPosition.x = -1;
    m_sTxWndCoOrd.ptMaxPosition.y = -1;
    
    m_sTxWndCoOrd.rcNormalPosition.top = -1;
    m_sTxWndCoOrd.rcNormalPosition.left = -1;
    m_sTxWndCoOrd.rcNormalPosition.right = -1;
    m_sTxWndCoOrd.rcNormalPosition.bottom = -1;
}

/******************************************************************************/
/*  Function Name    :  vInitBaudRateDetails                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  All the members of the class are initialized with     */
/*                      default values. This method is called when the user   */
/*                      wants to create a new configuration file.             */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  12.11.2002                                            */
/*  Modifications on :  Amitesh Bharti                                        */
/*  Modifications on :  23.03.2003, Changed to take care of acceptance mask,  */
/*                      acceptance filter, controller mode, warning limit etc */
/******************************************************************************/
void CConfigDetails::vInitBaudRateDetails()
{
}
/******************************************************************************/
/*  Function Name    :  vInitMsgAttributes                                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  All the members of the class are initialized with     */
/*                      default values. This method is called when the user   */
/*                      wants to create a new configuration file.             */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  12.11.2002                                            */
/*  Modifications on :                                                        */
/******************************************************************************/
void CConfigDetails::vInitMsgAttributes()
{
    m_sMessageAttributes.m_usMsgCount = 0;
    m_sMessageAttributes.m_psMsgAttribDetails = NULL;
}





/******************************************************************************/
/*  Function Name    :  vInitFilterDetails                                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  All the members of the class are initialized with     */
/*                      default values. This method is called when the user   */
/*                      wants to create a new configuration file.             */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  12.11.2002                                            */
/*  Modifications on :  Raja N on 20.07.2005, Intialised filter manager and   */
/*                      Display filter list.                                  */
/******************************************************************************/
void CConfigDetails::vInitFilterDetails()
{
    m_sMsgFilterDetails.m_byFilterType = defMSGFILTERTYPE;
    m_sMsgFilterDetails.m_omMsgNameArray.RemoveAll();
    m_sMsgFilterDetails.m_punUndefinedMsgID = NULL;
    m_sMsgFilterDetails.m_unCount = 0;
    m_sMsgFilterDetails.m_nRangeFrom = -1;
    m_sMsgFilterDetails.m_nRangeTo = -1;
    // Clear existing filter list
    //CFilterManager::ouGetFilterManager().vInitFilterList();
    // Clear Message Filter List
    m_omMsgDisplayFilter.RemoveAll();
}

/******************************************************************************/
/*  Function Name    :  vInitToolbarInfo                                      */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  All the members of the class are initialized with     */
/*                      default values. This method is called when the user   */
/*                      wants to create a new configuration file.             */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  12.11.2002                                            */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  17.06.2003, Some more flag information is stored in,  */
/*                      configuration file:Disp and Log Format and Time Mode  */
/*                      Some name is changed: byBase -> byDisplayHexON        */
/*  Modifications By :  Krishnaswamy B.N                                      */
/*  Modifications on :  19.08.2003, Flag information for m_byLogOverWriteON   */
/*                      and m_byReplayMsgType is initialised                  */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  03.08.2004, Added init of Ensure Visible flag         */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  11.08.2004, Modified inti value of Ensure Visible flag*/
/*                      to True                                               */
/******************************************************************************/
void CConfigDetails::vInitToolbarInfo()
{
    // toolbar information
    m_sToolBarButtonStatus.m_byMsgFilter = 0;
    m_sToolBarButtonStatus.m_byLogFilter =  0;
    m_sToolBarButtonStatus.m_byLogging =  0;
    m_sToolBarButtonStatus.m_byMsgInterpret =  0;
    m_sToolBarButtonStatus.m_byOverwrite =  1;
    m_sToolBarButtonStatus.m_byDisplayTimeMode = eSYSTEM_MODE ;
    m_sToolBarButtonStatus.m_byDisplayHexON = FALSE;
    m_sToolBarButtonStatus.m_byLogTimeMode = eSYSTEM_MODE;
    m_sToolBarButtonStatus.m_byLogHexON = TRUE;
    m_sToolBarButtonStatus.m_byLogOverWriteON = TRUE;
    m_sToolBarButtonStatus.m_byReplayMsgType = eALL_MESSAGE;
    m_sToolBarButtonStatus.m_byEnsureVisible = TRUE;
}

/******************************************************************************/
/*  Function Name    :  vLoadStoreBaudRateDetails                             */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method loads/stores the baud rate information    */
/*                      from/to the configuration file. The load/store        */
/*                      operation done depending if the archive is loading or */
/*                      storing data. The baud rate structure is as defined   */
/*                      struct sBAUDRATEDETAILS                               */
/*                      {                                                     */
/*                          INT     m_nItemUnderFocus;                        */
/*                          INT     m_nBTR0BTR1;                              */
/*                                                                            */
/*                          CString m_omStrBTR0;                              */
/*                          CString m_omStrBTR1;                              */
/*                          CString m_omStrBaudrate;                          */
/*                          CString m_omStrClock;                             */
/*                          CString m_omStrSampling;                          */
/*                          CString m_omStrAccCode;                           */
/*                          CString m_omStrAccMask;                           */
/*                                                                            */
/*                      };                                                    */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :  Amitesh Bharti                                        */
/*  Modifications on :  23.03.2003, Changed to take care of acceptance mask,  */
/*                      acceptance filter, controller mode, warning limit etc */
/*  Modifications    :  Raja N on 31.08.2004                                  */
/*                      Added USB code to check filtering mode and controller */
/*                      Warning limit. In case of unsupported option default  */
/*                      value will be used and user will be informed about    */
/*                      change.                                               */
/*  Modifications    :  Raja N on 22.03.2005                                  */
/*                      Modified code to use Message Box rathen then using    */
/*                      using AfxMessageBox as the window itself will be created
/*                      during this function call                             */
/******************************************************************************/

static void vReadControllerDetails(CArchive& roCfgArchive, int& nIndex,
                                   PSCONTROLER_DETAILS psControllerDetails)
{
    // the item under focus
    roCfgArchive >> psControllerDetails[ nIndex ].m_nItemUnderFocus;
    // packed value of bit timing register 0 and bit timing register 1
    roCfgArchive >> psControllerDetails[ nIndex ].m_nBTR0BTR1;
    CString omTemp;
    roCfgArchive >> omTemp;
    _tcscpy(psControllerDetails[nIndex].m_omStrCNF1, omTemp.GetBuffer(MAX_PATH));
    roCfgArchive >> omTemp;
    _tcscpy(psControllerDetails[nIndex].m_omStrCNF2, omTemp.GetBuffer(MAX_PATH));
    // baudrate information
    roCfgArchive >> omTemp;
    _tcscpy(psControllerDetails[nIndex].m_omStrBaudrate, omTemp.GetBuffer(MAX_PATH));
    // clock information
    roCfgArchive >> omTemp;
    _tcscpy(psControllerDetails[nIndex].m_omStrClock, omTemp.GetBuffer(MAX_PATH));
    // sampling information
    roCfgArchive >> omTemp;
    _tcscpy(psControllerDetails[nIndex].m_omStrSampling, omTemp.GetBuffer(MAX_PATH));
    
    // acceptance code information
    roCfgArchive >> omTemp;
    _tcscpy(psControllerDetails[nIndex].m_omStrAccCodeByte1[0], omTemp.GetBuffer(MAX_PATH));
    roCfgArchive >> omTemp;
    _tcscpy(psControllerDetails[nIndex].m_omStrAccCodeByte2[0], omTemp.GetBuffer(MAX_PATH));
    roCfgArchive >> omTemp;
    _tcscpy(psControllerDetails[nIndex].m_omStrAccCodeByte3[0], omTemp.GetBuffer(MAX_PATH));
    roCfgArchive >> omTemp;
    _tcscpy(psControllerDetails[nIndex].m_omStrAccCodeByte4[0], omTemp.GetBuffer(MAX_PATH));
    roCfgArchive >> omTemp;
    _tcscpy(psControllerDetails[nIndex].m_omStrAccMaskByte1[0], omTemp.GetBuffer(MAX_PATH));
    roCfgArchive >> omTemp;
    _tcscpy(psControllerDetails[nIndex].m_omStrAccMaskByte2[0], omTemp.GetBuffer(MAX_PATH));
    roCfgArchive >> omTemp;
    _tcscpy(psControllerDetails[nIndex].m_omStrAccMaskByte3[0], omTemp.GetBuffer(MAX_PATH));
    roCfgArchive >> omTemp;
    _tcscpy(psControllerDetails[nIndex].m_omStrAccMaskByte4[0], omTemp.GetBuffer(MAX_PATH));
    
    // Acceptance Filter mode
    roCfgArchive >> psControllerDetails[ nIndex ].m_bAccFilterMode;
    // warning limit information
    roCfgArchive >> omTemp;
    _tcscpy(psControllerDetails[nIndex].m_omStrWarningLimit, omTemp.GetBuffer(MAX_PATH));
    // controller mode information
    roCfgArchive >> psControllerDetails[ nIndex ].m_ucControllerMode;
}
void CConfigDetails::vLoadStoreBaudRateDetails(CArchive& roCfgArchive)
{
    // the archive is now loading data to the members
    if (roCfgArchive.IsLoading())
    {
        // First load default values
        vInitBaudRateDetails();
        // number of channels in the config file
        int nChannels = 1;
        // For dummy reading
        BOOL bDummyRead = FALSE;
        int nTimes = 0;
        
        // Check the Config file version
        if (m_fAppVersion >= defMULTI_CHANNEL_VERSION)
        {
            // Get the number of channels from the configuration file
            roCfgArchive >> nChannels;
            // If more number of channels found
            if (nChannels > defNO_OF_CHANNELS)
            {
                bDummyRead = TRUE;
                // Get the extra information count
                nTimes = nChannels - defNO_OF_CHANNELS;
                // Assign to supported channels
                nChannels = defNO_OF_CHANNELS;
            }
        }
        // Retrive each channel information
        for (int nIndex = 0; nIndex < nChannels; nIndex++)
        {
            vReadControllerDetails(roCfgArchive, nIndex, m_sControllerDetails);
        }
        
        
        // Read the additional information so that the flow will not be
        // distrubed
        if (bDummyRead == TRUE)
        {
            sCONTROLERDETAILS sDummy;
            
            for (int nIndex = 0; nIndex < nTimes; nIndex++)
            {
                vReadControllerDetails(roCfgArchive, nIndex, m_sControllerDetails);
            }            
        }
    }
}


/******************************************************************************/
/*  Function Name    :  bLoadStoreMsgAttributes                               */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method loads/stores the message attributes       */
/*                      structure from the configuration file. The load/store */
/*                      operation is done depending on whether the archive is */
/*                      loading/storing the data. The message attribute       */
/*                      structure is defined as follows:                      */
/*                      struct sMESSAGEATTRIBUTES                             */
/*                      {                                                     */
/*                          USHORT          m_usMsgCount;                     */
/*                          PSMESSAGEATTR   m_psMsgAttribDetails;             */
/*                      };                                                    */
/*                      and PSMESSAGEATTR is pointer to a structure of type   */
/*                                                                            */
/*                      struct sMESSAGEATTR                                   */
/*                      {                                                     */
/*                          CString     omStrMsgname;                         */
/*                          UINT        unMsgID;                              */
/*                          COLORREF    sColor ;                              */
/*                      };                                                    */
/*                      In the loading mode, only if the m_usMsgCount has a   */
/*                      non-zero value the pointer m_psMsgAttribDetails is    */
/*                      allocated.                                            */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :                                                        */
/******************************************************************************/
BOOL CConfigDetails::bLoadStoreMsgAttributes(CArchive& roCfgArchive)
{
    BOOL bRetVal = TRUE;
    
    if (roCfgArchive.IsLoading())
    {
        // m_sMessageAttributes
        roCfgArchive >> m_sMessageAttributes.m_usMsgCount;
        USHORT usCount = m_sMessageAttributes.m_usMsgCount;
        
        if (usCount != 0)
        {
            // allocate memory
            PSMESSAGEATTR   pMsgAttr = new SMESSAGEATTR[usCount];
            
            // did allocation succeed
            if (pMsgAttr != NULL)
            {
                PSMESSAGEATTR   pHead = pMsgAttr;
                
                for (int i = 0; i < usCount; i++, pMsgAttr++)
                {
                    roCfgArchive >> pMsgAttr->omStrMsgname;
                    roCfgArchive >> pMsgAttr->unMsgID;
                    roCfgArchive >> pMsgAttr->sColor;
                }
                m_sMessageAttributes.m_psMsgAttribDetails = pHead;
            }
            else
            {
                // failed to allocate memory
                bRetVal = FALSE;
            }
        }
    }
    
    if (roCfgArchive.IsStoring())
    {
        roCfgArchive << m_sMessageAttributes.m_usMsgCount;
        
        PSMESSAGEATTR pMsgAttr = m_sMessageAttributes.m_psMsgAttribDetails;
        
        for (int i = 0; i < m_sMessageAttributes.m_usMsgCount; 
        i++, pMsgAttr++)
        {
            roCfgArchive << pMsgAttr->omStrMsgname;
            roCfgArchive << pMsgAttr->unMsgID;
            roCfgArchive << pMsgAttr->sColor;
        }
    }
    
    return bRetVal;
}





/******************************************************************************/
/*  Function Name    :  vLoadStoreWndCoords                                   */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method loads/stores from the configuration file  */
/*                      the replay window coordinates, output window coords   */
/*                      and message window placement information. Depending on*/
/*                      the mode of the archive, the data is loaded/stored.   */
/*                      The structure is as defined                           */
/*                      struct sWINDOWCOORDINATES                             */
/*                      {                                                     */
/*                          WINDOWPLACEMENT    m_sReplayWndCoOrd;             */
/*                          WINDOWPLACEMENT    m_sOutputWndCoOrd;             */
/*                          WINDOWPLACEMENT    m_sMsgWndCoOrd;                */
/*                          WINDOWPLACEMENT    m_sNoticWndCoOrd;              */
/*                      };                                                    */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :  18.04.2003, Added code for Trace Window. For output   */
/*                                  and replay window the structure           */
/*                                  WINDOWPLACEMENT is used.                  */
/*  Modifications    :  Raja N on 05.04.2004                                  */
/*                      Included window placenemt of Signal Watch and Message */
/*                      Interpretation window
/*  Modifications    :  Raja N on 05.04.2004                                  */
/*                      Included window placenemt of Tx Msg window and graph  */
/*                      window                                                */
/******************************************************************************/
void CConfigDetails::vLoadStoreWndCoords(CArchive& roCfgArchive)
{
    // Set All values to default
    if (roCfgArchive.IsLoading() &&
        m_fAppVersion < defMULTI_CHANNEL_VERSION)
    {
        // Load Default values
        vInitWndCoords();
    }
    // window placement values of the Replay window
    vLoadStoreWindowInfo( roCfgArchive, m_sReplayWndCoOrd);
    // window placement values of the Out window
    vLoadStoreWindowInfo( roCfgArchive, m_sOutputWndCoOrd);
    // window placement values of the trace window
    vLoadStoreWindowInfo( roCfgArchive, m_sNotificWndCoOrd);
    // window placement values of the message window
    vLoadStoreWindowInfo( roCfgArchive, m_sMsgWndCoOrd);
    // placement structure of the Signal Watch Window
    vLoadStoreWindowInfo( roCfgArchive, m_sSigWatchWindow);
    // placement structure of the Interpretation Window
    vLoadStoreWindowInfo( roCfgArchive, m_sMsgInterpWindow);
    // Load Graph window and Tx Window information only if the 
    // version is >=2.5 that is Multi channel version
    // Save irrespective of app version
    if (m_fAppVersion >= defMULTI_CHANNEL_VERSION ||
        roCfgArchive.IsStoring())
    {
        // Placement structure of Graph window
        vLoadStoreWindowInfo( roCfgArchive, m_sGraphWndCoOrd);
        // Placement structure of Tx window
        vLoadStoreWindowInfo( roCfgArchive, m_sTxWndCoOrd);
    }
}

/*******************************************************************************
Function Name  : vLoadStoreSplitterPostion
Input(s)       : oCfgArchive - Reference to Configuration file archive
Output         : -
Functionality  : This function will load/save splitter positions in to the
configuration file
Member of      : CConfigDetails
Author(s)      : Raja N
Date Created   : 26.4.2005
Modifications  : 
*******************************************************************************/
void CConfigDetails::vLoadStoreSplitterPostion( CArchive& oCfgArchive)
{
    // Loading
    if (oCfgArchive.IsLoading())
    {
        if (m_fAppVersion >= defMULTI_CHANNEL_VERSION)
        {
            // Read Graph Window Splitter Postion
            // Root splitter information
            oCfgArchive >> m_sGraphWndSplitterPos.m_nRootSplitterData[0][0]; 
            oCfgArchive >> m_sGraphWndSplitterPos.m_nRootSplitterData[0][1]; 
            oCfgArchive >> m_sGraphWndSplitterPos.m_nRootSplitterData[1][0]; 
            oCfgArchive >> m_sGraphWndSplitterPos.m_nRootSplitterData[1][1]; 
            
            // Column splitter information
            oCfgArchive >> m_sGraphWndSplitterPos.m_nRightViewSplitterData[0][0];
            oCfgArchive >> m_sGraphWndSplitterPos.m_nRightViewSplitterData[0][1];
            oCfgArchive >> m_sGraphWndSplitterPos.m_nRightViewSplitterData[1][0];
            oCfgArchive >> m_sGraphWndSplitterPos.m_nRightViewSplitterData[1][1];
            
            // Tx Window Splitter Postion
            // Main Splitter postion
            oCfgArchive >> m_sTxMsgWndSplitterPos.m_nRootSplitterData[0][0];
            oCfgArchive >> m_sTxMsgWndSplitterPos.m_nRootSplitterData[0][1];
            oCfgArchive >> m_sTxMsgWndSplitterPos.m_nRootSplitterData[1][0];
            oCfgArchive >> m_sTxMsgWndSplitterPos.m_nRootSplitterData[1][1];
            
            // Second Level Splitter
            oCfgArchive >> m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[0][0];
            oCfgArchive >> m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[0][1];
            oCfgArchive >> m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[1][0];
            oCfgArchive >> m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[1][1];
            
            // Third level splitter data
            oCfgArchive >> m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[0][0];
            oCfgArchive >> m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[0][1];
            oCfgArchive >> m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[1][0];
            oCfgArchive >> m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[1][1];
        }
        else
        {
            // Initialise splitter postions with default values
            vInitSplitterPostions();
        }
    }
    // Storing
    else
    {
        // Store Graph Window Splitter Postion
        // Root splitter information
        // Row 0
        oCfgArchive << m_sGraphWndSplitterPos.m_nRootSplitterData[0][0]; 
        oCfgArchive << m_sGraphWndSplitterPos.m_nRootSplitterData[0][1]; 
        oCfgArchive << m_sGraphWndSplitterPos.m_nRootSplitterData[1][0]; 
        oCfgArchive << m_sGraphWndSplitterPos.m_nRootSplitterData[1][1]; 
        
        // Column splitter information
        oCfgArchive << m_sGraphWndSplitterPos.m_nRightViewSplitterData[0][0];
        oCfgArchive << m_sGraphWndSplitterPos.m_nRightViewSplitterData[0][1];
        oCfgArchive << m_sGraphWndSplitterPos.m_nRightViewSplitterData[1][0];
        oCfgArchive << m_sGraphWndSplitterPos.m_nRightViewSplitterData[1][1];
        
        
        // Tx Window Splitter Postion
        
        // Main Splitter postion
        // Row 0
        oCfgArchive << m_sTxMsgWndSplitterPos.m_nRootSplitterData[0][0];
        oCfgArchive << m_sTxMsgWndSplitterPos.m_nRootSplitterData[0][1];
        oCfgArchive << m_sTxMsgWndSplitterPos.m_nRootSplitterData[1][0];
        oCfgArchive << m_sTxMsgWndSplitterPos.m_nRootSplitterData[1][1];
        
        // Second Level Splitter
        oCfgArchive << m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[0][0];
        oCfgArchive << m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[0][1];
        oCfgArchive << m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[1][0];
        oCfgArchive << m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[1][1];
        
        // Third level splitter data
        oCfgArchive << m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[0][0];
        oCfgArchive << m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[0][1];
        oCfgArchive << m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[1][0];
        oCfgArchive << m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[1][1];
    }
}

/******************************************************************************/
/*  Function Name    :  bLoadStoreMultiMsgInfo                                */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method loads/stores the multiple message info    */
/*                      structure. The structure is as defined                */
/*                      struct sSENDMULTIMSGINFO                              */
/*                      {                                                     */
/*                          UINT unNoOfMsgsToBeSent;                          */
/*                          CStringArray omMsgFormatList;                     */
/*                          BOOL bChecked;                                    */
/*                          INT nTimeDelay;                                   */
/*                          PSSENDMSGINFO pSendMsgInfo;                       */
/*                      };                                                    */
/*                      This structure has an array of SSENDMSGINFO structures*/
/*                      The array is allocated only if the count              */
/*                      unNoOfMsgsToBeSent is non zero.                       */
/*                      The structure is as defined                           */
/*                                                                            */
/*                      struct sSENDMSGINFO                                   */
/*                      {                                                     */
/*                          INT nMsgId;                                       */
/*                          char* omStrMsgName;                               */
/*                          char* omStrMsgDetails;                            */
/*                          UINT acBinaryData[64];                            */
/*                          int  anSignalValues[64];                          */
/*                          INT nDLC;                                         */
/*                          PSSENDSIGINFO pSendSgInfo;                        */
/*                      };                                                    */
/*                      This structure has an array of the SENDSIGINFO. This  */
/*                      is allocated only if nDLC is non zero.                */
/*                      strucutres.                                           */
/*                                                                            */
/*                      struct sSENDSIGINFO                                   */
/*                      {                                                     */
/*                          INT nValue;                                       */
/*                      };                                                    */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :                                                        */
/******************************************************************************/
BOOL CConfigDetails::bLoadStoreMultiMsgInfo(CArchive& roCfgArchive)
{
    BOOL bRetVal = TRUE;
    // loading the data..
    if (roCfgArchive.IsLoading())
    {
        // get the number of the messages present in the array..
        roCfgArchive >> m_unNumberOfMsgBlockCount;
        
        m_psMsgBlockList = NULL;
        PSMSGBLOCKLIST psMsgCurrentBlockList = NULL;
        // are there any elements ?
        if (m_unNumberOfMsgBlockCount  != 0)
        {
            if (m_psMsgBlockList == NULL)
            {
                m_psMsgBlockList = new SMSGBLOCKLIST;
                vInitialiseMsgBlock(m_psMsgBlockList);
                if (m_psMsgBlockList == NULL)
                {
                    bRetVal = FALSE;
                }
            }
            psMsgCurrentBlockList = m_psMsgBlockList;
            for (UINT i = 0;
            (i < m_unNumberOfMsgBlockCount) && (bRetVal == TRUE);
            i++)
            {
                bRetVal = bLoadStoreMsgInfo(roCfgArchive,psMsgCurrentBlockList);
                if (i+1 <m_unNumberOfMsgBlockCount)
                {
                    psMsgCurrentBlockList->m_psNextMsgBlocksList =
                        new SMSGBLOCKLIST;
                    vInitialiseMsgBlock(
                        psMsgCurrentBlockList->m_psNextMsgBlocksList);
                    if (psMsgCurrentBlockList->m_psNextMsgBlocksList == NULL)
                    {
                        bRetVal = FALSE;
                    }
                }
                psMsgCurrentBlockList = 
                    psMsgCurrentBlockList->m_psNextMsgBlocksList ;
            }
        } 
    }
    
    if (roCfgArchive.IsStoring())
    {
        // store the number of the messages present in the array..
        roCfgArchive << m_unNumberOfMsgBlockCount ;
        
        PSMSGBLOCKLIST psMsgCurrentBlockList = NULL;
        // are there any elements ?
        if (m_unNumberOfMsgBlockCount  != 0)
        {
            
            psMsgCurrentBlockList = m_psMsgBlockList;
            for (UINT i = 0;
            (i < m_unNumberOfMsgBlockCount) && (bRetVal == TRUE);
            i++)
            {
                bRetVal = bLoadStoreMsgInfo(roCfgArchive,psMsgCurrentBlockList);
                psMsgCurrentBlockList = 
                    psMsgCurrentBlockList->m_psNextMsgBlocksList ;
            }
        } 
    }
    return bRetVal;
}
//This struct is only required to load old configuration
struct tagFilterOld
{
    UCHAR m_ucFilterType;   // 0 - Message ID and 1 - ID Range
    UINT  m_unMsgIDFrom;    // From Message ID incase of range.
                            // Msg ID in case of Single ID
    UINT  m_unMsgIDTo  ;    // To Message ID incase of range.
    UCHAR m_ucDirection;    // 2 - Rx Msg, 1 - Tx Msg, 0 - For All
    UCHAR m_ucEXTENDED;     // 0 - Standard, 1 - Extended, 2 - For All
    UCHAR m_ucRTR;          // 0 - Non RTR, 1 - RTR Message, 2 - For All
    UCHAR m_ucChannel;      // Channel Number, 0 - for all
};

typedef tagFilterOld sFilterOld;
typedef sFilterOld SFILTER_OLD;

static void vPopulateFilterCan(SFILTER_CAN& Dest, const SFILTER_OLD& Src)
{
    Dest.m_dwMsgIDFrom = Src.m_unMsgIDFrom;
    Dest.m_dwMsgIDTo   = Src.m_unMsgIDTo;

    if (Src.m_ucChannel == 0)
    {
        Dest.m_eChannel = CAN_CHANNEL_ALL;
    }
    else if ((Src.m_ucChannel >= 1) && (Src.m_ucChannel <= 2))
    {
        Dest.m_eChannel = Src.m_ucChannel;
    }
    else
    {
        ASSERT(FALSE);
    }

    switch (Src.m_ucDirection)
    {
        case 0:
        {
            Dest.m_eDrctn = DIR_RX;
        }
        break;
        case 1:
        {
            Dest.m_eDrctn = DIR_TX;
        }
        break;
        case 2:
        {
            Dest.m_eDrctn = DIR_ALL;
        }
        break;
        default:
        {
            ASSERT(FALSE);
        }
        break;
    }
    Dest.m_ucFilterType = Src.m_ucFilterType;

    switch(Src.m_ucRTR)
    {
        case 0:
        {
            Dest.m_byMsgType = TYPE_MSG_CAN_NON_RTR;
        }
        break;
        case 1:
        {
            Dest.m_byMsgType = TYPE_MSG_CAN_RTR;
        }
        break;
        case 2:
        {
            Dest.m_byMsgType = TYPE_MSG_CAN_ALL;
        }
        break;
        default:
        {
            ASSERT(FALSE);
        }
        break;
    }

    switch(Src.m_ucEXTENDED)
    {
        case 0:
        {
            Dest.m_byIDType = TYPE_ID_CAN_STANDARD;
        }
        break;
        case 1:
        {
            Dest.m_byIDType = TYPE_ID_CAN_EXTENDED;
        }
        break;
        case 2:
        {
            Dest.m_byIDType = TYPE_ID_CAN_ALL;
        }
        break;
        default:
        {
            ASSERT(FALSE);
        }
        break;
    }    
}

/******************************************************************************/
/*  Function Name    :  bLoadStoreMsgFilterDetails                            */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method loads/stores the message filter details   */
/*                      into the archive. The structure that needs to be      */
/*                      stored is                                             */
/*                      struct sMESSAGEFILTERDETAILS                          */
/*                      {                                                     */
/*                          UINT    *m_punUndefinedMsgID;                     */
/*                          CStringArray    m_omMsgNameArray;                 */
/*                          UINT    m_unRangeFrom;                            */
/*                          UINT    m_unRangeTo;                              */
/*                          UINT    m_unCount;                                */
/*                          BYTE    m_byFilterType;                           */
/*                          BOOL    m_bCheckTxFlag;                           */
/*                      };                                                    */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :  Gopi, 12.11.2002, Extended to store and load filter   */
/*                      type, flag to indicate if filter messages should be   */
/*                      transmitted                                           */
/*  Modifications    :  Raja N on 20.07.2005, Added code to support serialise */
/*                      of new filter architecture and to support previous    */
/*                      version cfg file                                      */
/******************************************************************************/
static BOOL bLoadFilterDets(CArchive& romArchive, SFILTERAPPLIED_CAN& sFilterConfigured)
{
    BOOL bResult = FALSE;
    sFilterConfigured.vClear();
    // Load number of filters in the list first
    int nSize = 0;
    romArchive >> nSize;
    sFilterConfigured.m_psFilters = new SFILTERSET[nSize];
    sFilterConfigured.m_ushTotal = (USHORT)nSize;
    sFilterConfigured.m_bEnabled = FALSE;
    // Take each filter and store it Name followed by the filter
    for( int nIndex = 0; nIndex < nSize; nIndex++ )
    {
        // Get Filter Name
        CString omStrFilterName = STR_EMPTY;
        romArchive >> omStrFilterName;
        _tcscpy(sFilterConfigured.m_psFilters[nIndex].m_sFilterName.m_acFilterName, 
                                                                    omStrFilterName);
        // Read Filter Type first
        romArchive >> sFilterConfigured.m_psFilters[nIndex].m_sFilterName.m_bFilterType;
        // Serialize filter array
        CArray<SFILTER_OLD, SFILTER_OLD&> omFilterList;
        omFilterList.Serialize( romArchive );
		INT nFilterCount = omFilterList.GetSize();
        sFilterConfigured.m_psFilters[nIndex].m_bEnabled = TRUE;
        sFilterConfigured.m_psFilters[nIndex].m_eCurrBus = CAN;
        sFilterConfigured.m_psFilters[nIndex].m_ushFilters = (USHORT)nFilterCount;
        sFilterConfigured.m_psFilters[nIndex].m_psFilterInfo = new SFILTER_CAN[nFilterCount];
        SFILTER_CAN* psTemp = (SFILTER_CAN*)(sFilterConfigured.m_psFilters[nIndex].m_psFilterInfo);
        for (INT i = 0; i < nFilterCount; i++)
        {
            SFILTER_OLD sFilterOld = omFilterList.GetAt(i);
            vPopulateFilterCan(psTemp[i], sFilterOld);            
        }
        bResult = TRUE;        
    }
    return bResult;
}
BOOL CConfigDetails::bLoadStoreMsgFilterDetails(CArchive& roCfgArchive)
{
    BOOL bRetVal =TRUE;

    if(roCfgArchive.IsLoading())
    {
        // Latest version file
        if( m_fAppVersion >= static_cast<float>( defMULTI_FILTER_VERSION ))
        {
            bLoadFilterDets(roCfgArchive, m_sFilterConfigured);
            //CFilterManager::ouGetFilterManager().Serialize( roCfgArchive );
        }
        else
        {
            //CFilterManager::ouGetFilterManager().vInitFilterList();
            // Get Message Filter Details
            // Get Array length. This is always 1 or 0!! Hardcoded value as of now
            UINT unUnknownIDArraySize;
            roCfgArchive >> unUnknownIDArraySize;
            // Read Unknown msg ID only if it is present
            if( unUnknownIDArraySize > 0 )
            {
                // Read unknown message ID
                m_sMsgFilterDetails.m_punUndefinedMsgID =
                                        new UINT[unUnknownIDArraySize + 1];
                if( m_sMsgFilterDetails.m_punUndefinedMsgID != NULL )
                {
                    m_sMsgFilterDetails.m_punUndefinedMsgID[0] =
                                                        unUnknownIDArraySize;
                    roCfgArchive >> m_sMsgFilterDetails.m_punUndefinedMsgID[1];
                }
                else
                {
                    bRetVal = FALSE;
                }
            }
            if( bRetVal == TRUE )
            {
                // range start
                roCfgArchive >> m_sMsgFilterDetails.m_nRangeFrom;
                // range end
                roCfgArchive >> m_sMsgFilterDetails.m_nRangeTo;
                // count info of message names
                roCfgArchive >> m_sMsgFilterDetails.m_unCount;
                // filter type
                roCfgArchive >> m_sMsgFilterDetails.m_byFilterType;
                // transmit filter messages?
                roCfgArchive >> m_sMsgFilterDetails.m_bChekTxMsg;
                // Serialize msg names
                m_sMsgFilterDetails.m_omMsgNameArray.Serialize(roCfgArchive);
            }
        }
    }
    else
    {
        ASSERT(FALSE);
    }
    
    return bRetVal;
}
/******************************************************************************/
/*  Function Name    :  bLoadStoreSendMsgInfo                                 */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method loads/stores the Send message info into   */
/*                      the archive. The structure that needs to be           */
/*                      stored is                                             */
/*                      struct sMSGBLOCKLIST                                  */
/*                      {                                                     */
/*                          CString m_omStrBlockName;                         */
/*                          eTXMSGTRIGGERTYPE m_eTrigger;                     */
/*                          BOOL m_bType;                                     */
/*                          BOOL m_bActive;                                   */
/*                          BOOL m_bTxAllFrame;                               */
/*                          UCHAR m_ucKeyValue;                               */
/*                          UINT m_unTimeInterval;                            */
/*                          UINT m_unMsgCount;                                */
/*                          PSTXCANMSGLIST m_psTxCANMsgList;                  */
/*                          struct sMSGBLOCKLIST* m_psNextMsgBlocksList;      */
/*                      };                                                    */
/*                      This structure contains another list of for  message  */
/*                      details PSTXCANMSGLIST                                */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications On :  05.06.2003, the name  an64SignalValues is changed to  */
/*                      __int64 data type.                                    */
/*  Modifications on :  08.01.2004, The interface for message block           */
/*                      is changed                                            */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  22.07.2004, Added code to load old version cfg data   */
/*  Modifications    :  Raja N on 02.05.2005, Removed code to reset trigger   */
/*                      condition in case of old configuration data and added */
/*                      code to load message enable flag and channel info     */
/*  Modifications    :  Raja N on 10.05.2005, Modified conditional statement as
per code review comments                              */
/******************************************************************************/
BOOL CConfigDetails::bLoadStoreMsgInfo(CArchive& roCfgArchive, 
                                       PSMSGBLOCKLIST psMsgBlockList)
{
    BOOL bRetVal = TRUE;
    // loading the data
    UINT unDataLength = 0;
    if (roCfgArchive.IsLoading())
    {
        roCfgArchive >> psMsgBlockList->m_bActive;
        roCfgArchive >> psMsgBlockList->m_bType;
        roCfgArchive >> psMsgBlockList->m_bTxAllFrame;
        roCfgArchive >> psMsgBlockList->m_ucTrigger;
        roCfgArchive >> psMsgBlockList->m_omStrBlockName;
        roCfgArchive >> psMsgBlockList->m_ucKeyValue;
        roCfgArchive >> psMsgBlockList->m_unTimeInterval;
        roCfgArchive >> psMsgBlockList->m_unMsgCount;
        if (psMsgBlockList->m_unMsgCount > 0)
        {
            PSTXCANMSGLIST psCurrentMsgList = NULL;
            STXCANMSGDETAILS sTxMsgDetails;
            psMsgBlockList->m_psTxCANMsgList = new STXCANMSGLIST;
            vInitialiseMsgDetails(psMsgBlockList->m_psTxCANMsgList);
            if (psMsgBlockList->m_psTxCANMsgList == NULL)
            {
                bRetVal = FALSE;
            }
            else
            {
                psCurrentMsgList = psMsgBlockList->m_psTxCANMsgList;
            }
            for (UINT i = 0; ( i < psMsgBlockList->m_unMsgCount) &&
                ( bRetVal == TRUE);
                i++)
            {
                if (psCurrentMsgList != NULL)
                {
                    // Load Dirty Flag
                    roCfgArchive >> sTxMsgDetails.m_bIsMsgDirty;
                    
                    // Load Enable Option only for latest configuration file
                    // That is version > 2.4
                    if (m_fAppVersion >= defTX_MSG_ENABLE_OPTION_VERISION)
                    {
                        roCfgArchive >> sTxMsgDetails.m_bEnabled;
                    }
                    else
                    {
                        // Set Default Value
                        sTxMsgDetails.m_bEnabled = TRUE;
                    }
                    
                    // Load CAN Attributes
                    roCfgArchive >> sTxMsgDetails.m_sTxMsg.m_unMsgID;
                    roCfgArchive >> sTxMsgDetails.m_sTxMsg.m_ucDataLen;
                    roCfgArchive >> sTxMsgDetails.m_sTxMsg.m_ucEXTENDED;
                    roCfgArchive >> sTxMsgDetails.m_sTxMsg.m_ucRTR;
                    
                    // Read Channel Info only for Multi channel version
                    if (m_fAppVersion >= defMULTI_CHANNEL_VERSION)
                    {
                        roCfgArchive >> sTxMsgDetails.m_sTxMsg.m_ucChannel;
                    }
                    else
                    {
                        // Set Default Value
                        sTxMsgDetails.m_sTxMsg.m_ucChannel = 1;
                    }
                    unDataLength = sTxMsgDetails.m_sTxMsg.m_ucDataLen;
                    
                    for (UINT j  =0 ; j<defMAX_BYTE ; j++)
                    {
                        if (j<unDataLength)
                        {
                            roCfgArchive >> sTxMsgDetails.m_sTxMsg.m_ucData[j];
                        }
                        else
                        {
                            sTxMsgDetails.m_sTxMsg.m_ucData[j] = 0;
                        }
                    }
                    memcpy(&(psCurrentMsgList->m_sTxMsgDetails),
                        &sTxMsgDetails,
                        sizeof(sTxMsgDetails));
                    if (i+1 <psMsgBlockList->m_unMsgCount)
                    {
                        psCurrentMsgList->m_psNextMsgDetails = 
                            new STXCANMSGLIST;
                        vInitialiseMsgDetails(
                            psCurrentMsgList->m_psNextMsgDetails);
                    }
                    else
                    {
                        psCurrentMsgList->m_psNextMsgDetails = NULL;
                    }
                    psCurrentMsgList = psCurrentMsgList->m_psNextMsgDetails;
                }
                else
                {
                    bRetVal = FALSE;
                }
            }
        }
        else
        {
            psMsgBlockList->m_psTxCANMsgList = NULL;
        }
        psMsgBlockList->m_psNextMsgBlocksList = NULL;
    }
    // store the data
    if (roCfgArchive.IsStoring())
    {
        roCfgArchive << psMsgBlockList->m_bActive;
        roCfgArchive << psMsgBlockList->m_bType;
        roCfgArchive << psMsgBlockList->m_bTxAllFrame;
        roCfgArchive << psMsgBlockList->m_ucTrigger;
        roCfgArchive << psMsgBlockList->m_omStrBlockName;
        roCfgArchive << psMsgBlockList->m_ucKeyValue;
        roCfgArchive << psMsgBlockList->m_unTimeInterval;
        roCfgArchive << psMsgBlockList->m_unMsgCount;
        if (psMsgBlockList->m_unMsgCount > 0)
        {
            PSTXCANMSGLIST psCurrentMsgList = NULL;
            STXCANMSGDETAILS sTxMsgDetails;
            psCurrentMsgList = psMsgBlockList->m_psTxCANMsgList;
            sTxMsgDetails = psCurrentMsgList->m_sTxMsgDetails;
            for (UINT i =0; i<psMsgBlockList->m_unMsgCount;i++)
            {
                roCfgArchive << sTxMsgDetails.m_bIsMsgDirty;
                roCfgArchive << sTxMsgDetails.m_bEnabled;
                
                roCfgArchive << sTxMsgDetails.m_sTxMsg.m_unMsgID;
                roCfgArchive << sTxMsgDetails.m_sTxMsg.m_ucDataLen;
                roCfgArchive << sTxMsgDetails.m_sTxMsg.m_ucEXTENDED;
                roCfgArchive << sTxMsgDetails.m_sTxMsg.m_ucRTR;
                roCfgArchive << sTxMsgDetails.m_sTxMsg.m_ucChannel;
                
                unDataLength = sTxMsgDetails.m_sTxMsg.m_ucDataLen;
                
                for (UINT j  =0 ; j<unDataLength ; j++)
                {
                    roCfgArchive << sTxMsgDetails.m_sTxMsg.m_ucData[j];
                }
                psCurrentMsgList = psCurrentMsgList->m_psNextMsgDetails;
                if (psCurrentMsgList != NULL)
                {
                    sTxMsgDetails    = psCurrentMsgList->m_sTxMsgDetails; 
                }
            }
        }
    }
    return bRetVal;
}


/******************************************************************************/
/*  Function Name    :  bGetMultiMsgInfo                                      */
/*                                                                            */
/*  Input(s)         :  pSendMultMsgInfo : out-param structure                */
/*                                                                            */
/*  Output           :  TRUE : if the method succeeds                         */
/*                      FALSE : if memory allocation fails                    */
/*                                                                            */
/*  Functionality    :  This method gets the information for the multi message*/
/*                      information. The memory for the strucutre is already  */
/*                      allocated in the bGetData(..) method. This method     */
/*                      allocates memory as required by the other elements in */
/*                      this structure.                                       */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications On :  05.06.2003, the name  an64SignalValues is changed to  */
/*                      __int64 data type.                                    */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications On :  24.12.2003, the structure for holding message details */
/*                      is changed. So new structure is adopted               */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  08.01.2004, The interface for message block           */
/*                      is changed and a new flag is introduced to get the    */
/*                      message count.                                        */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  22.07.2004, The trigger value type got changed as char*/
/*                      and removed the trigger to def from enum              */
/******************************************************************************/
BOOL CConfigDetails::bGetMultiMsgInfo(PSMSGBLOCKLIST psDestMsgBlockList)
{
    BOOL bRetVal = TRUE;
    PSMSGBLOCKLIST psSrcMsgBlockList = m_psMsgBlockList; 
    PSTXCANMSGLIST psSrcTxMsgList    = NULL;
    PSTXCANMSGLIST psDestTxMsgList   = NULL;
    if (psSrcMsgBlockList != NULL && psDestMsgBlockList != NULL)
    {
        for (UINT i =0; (i<m_unNumberOfMsgBlockCount) && (bRetVal == TRUE);i++)
        {
            psDestMsgBlockList->m_bActive        = 
                psSrcMsgBlockList->m_bActive;
            psDestMsgBlockList->m_bType          = psSrcMsgBlockList->m_bType;
            psDestMsgBlockList->m_bTxAllFrame     = 
                psSrcMsgBlockList->m_bTxAllFrame;
            psDestMsgBlockList->m_ucTrigger       =
                psSrcMsgBlockList->m_ucTrigger;
            psDestMsgBlockList->m_omStrBlockName = 
                psSrcMsgBlockList->m_omStrBlockName;
            psDestMsgBlockList->m_ucKeyValue     = 
                psSrcMsgBlockList->m_ucKeyValue;
            psDestMsgBlockList->m_unMsgCount     = 
                psSrcMsgBlockList->m_unMsgCount;
            psDestMsgBlockList->m_unTimeInterval = 
                psSrcMsgBlockList->m_unTimeInterval;
            if (psSrcMsgBlockList->m_unMsgCount > 0)
            {
                
                psSrcTxMsgList = psSrcMsgBlockList->m_psTxCANMsgList;
                psDestMsgBlockList->m_psTxCANMsgList = new STXCANMSGLIST;
                psDestTxMsgList = psDestMsgBlockList->m_psTxCANMsgList;
                if (psDestTxMsgList != NULL)
                {
                    for (UINT j =0;
                    (j<psSrcMsgBlockList->m_unMsgCount)&&(bRetVal == TRUE);
                    j++)
                    {
                        
                        memcpy(&(psDestTxMsgList->m_sTxMsgDetails),
                            &(psSrcTxMsgList->m_sTxMsgDetails),
                            sizeof(psSrcTxMsgList->m_sTxMsgDetails));
                        if (psSrcTxMsgList->m_psNextMsgDetails != NULL && 
                            j+1 <psSrcMsgBlockList->m_unMsgCount)
                        {
                            psDestTxMsgList->m_psNextMsgDetails = 
                                new STXCANMSGLIST;
                            vInitialiseMsgDetails(
                                psDestTxMsgList->m_psNextMsgDetails);
                            if (psDestTxMsgList->m_psNextMsgDetails != NULL)
                            {
                                psDestTxMsgList = 
                                    psDestTxMsgList->m_psNextMsgDetails;
                                psSrcTxMsgList  = 
                                    psSrcTxMsgList->m_psNextMsgDetails;
                            }
                            else
                            {
                                bRetVal = FALSE;
                            }
                        }
                        else
                        {
                            psDestTxMsgList->m_psNextMsgDetails = NULL;;
                        }
                    }
                }
                else
                {
                    bRetVal = FALSE;
                }
            }
            // Check the message count and don't allocate if the assignment for
            // last node is done i + 1 Because i is not incremented here 
            
            if (psSrcMsgBlockList->m_psNextMsgBlocksList != NULL &&
                (i + 1) < m_unNumberOfMsgBlockCount)
            {
                psDestMsgBlockList->m_psNextMsgBlocksList = new SMSGBLOCKLIST;
                vInitialiseMsgBlock(psDestMsgBlockList->m_psNextMsgBlocksList);
                if (psDestMsgBlockList->m_psNextMsgBlocksList != NULL)
                {
                    psDestMsgBlockList = 
                        psDestMsgBlockList->m_psNextMsgBlocksList;
                    psDestMsgBlockList->m_psNextMsgBlocksList = NULL;
                    psDestMsgBlockList->m_psTxCANMsgList = NULL;
                    psSrcMsgBlockList = 
                        psSrcMsgBlockList->m_psNextMsgBlocksList;
                }       
                else
                {
                    bRetVal = FALSE;
                }
            }
            else
            {
                psDestMsgBlockList->m_psNextMsgBlocksList = NULL;
            }
        }
    }
    else
    {
        bRetVal = FALSE;
    }
    return bRetVal;
}





/******************************************************************************/
/*  Function Name    :  bSetMultiMsgInfo                                      */
/*                                                                            */
/*  Input(s)         :  pSendMultMsgInfo : in param structure                 */
/*                                                                            */
/*  Output           :  TRUE : if the method succeeds                         */
/*                      FALSE : if memory allocation fails                    */
/*                                                                            */
/*  Functionality    :  This method sets the information from the input       */
/*                      structure into the data member. This updates the      */
/*                      information.                                          */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications On :  05.06.2003, the name  an64SignalValues is changed to  */
/*                      __int64 data type.                                    */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  08.01.2004, The interface for message block           */
/*                      is changed and a new flag is introduced to get the    */
/*                      message count.                                        */
/*  Modofication By  :  Raja N                                                */
/*  Modifications on :  22.07.2004, The trigger value type got changed as char*/
/*                      and removed the trigger to def from enum              */
/******************************************************************************/
BOOL CConfigDetails::bSetMultiMsgInfo(PSMSGBLOCKLIST psMsgBlockList)
{
    BOOL bRetVal = TRUE;
    PSMSGBLOCKLIST psSrcMsgBlockList = psMsgBlockList;
    PSMSGBLOCKLIST psDesMsgBlockList = m_psMsgBlockList;
    PSTXMSG psTxMsg = g_psTxMsgBlockList;
    if (psDesMsgBlockList == NULL)
    {
        m_psMsgBlockList = new SMSGBLOCKLIST;
        vInitialiseMsgBlock(m_psMsgBlockList);
        if (m_psMsgBlockList == NULL)
        {
            bRetVal = FALSE;
        }
        else
        {
            psDesMsgBlockList = m_psMsgBlockList;
            m_psMsgBlockList->m_psNextMsgBlocksList = NULL;
            m_psMsgBlockList->m_psTxCANMsgList = NULL;
        }
    }
    if (psSrcMsgBlockList != NULL && 
        psDesMsgBlockList != NULL &&
        bRetVal == TRUE)
    {
        do
        {
            psDesMsgBlockList->m_bActive        = psSrcMsgBlockList->m_bActive;
            psDesMsgBlockList->m_bType          = psSrcMsgBlockList->m_bType;
            psDesMsgBlockList->m_bTxAllFrame     = 
                psSrcMsgBlockList->m_bTxAllFrame;
            psDesMsgBlockList->m_ucTrigger       = 
                psSrcMsgBlockList->m_ucTrigger;
            psDesMsgBlockList->m_omStrBlockName =
                psSrcMsgBlockList->m_omStrBlockName;
            
            psDesMsgBlockList->m_ucKeyValue  = psSrcMsgBlockList->m_ucKeyValue;
            psDesMsgBlockList->m_unMsgCount  = psSrcMsgBlockList->m_unMsgCount;
            
            psDesMsgBlockList->m_unTimeInterval = 
                psSrcMsgBlockList->m_unTimeInterval;
            
            if (psSrcMsgBlockList->m_unMsgCount>0)
            {
                PSTXCANMSGLIST psSrcTxMsgList = 
                    psSrcMsgBlockList->m_psTxCANMsgList;
                PSTXCANMSGLIST psDesTxMsgList = NULL;
                if (psDesMsgBlockList->m_psTxCANMsgList == NULL)
                {
                    psDesTxMsgList = new STXCANMSGLIST;
                    psDesMsgBlockList->m_psTxCANMsgList = psDesTxMsgList;
                    psDesTxMsgList->m_psNextMsgDetails  = NULL;
                }
                else
                {
                    psDesTxMsgList = psDesMsgBlockList->m_psTxCANMsgList;
                }
                if (psDesTxMsgList != NULL && psSrcTxMsgList != NULL)
                {
                    LPLONG lpPreviousCount = NULL;
                    BOOL   bTxON = FALSE;
                    CFlags* pouFlag = theApp.pouGetFlagsPtr();
                    if (pouFlag != NULL)
                    {
                        bTxON = pouFlag->nGetFlagStatus(SENDMESG);
                    }
                    do
                    {
                        if (psTxMsg != NULL && bTxON == TRUE)
                        {
                            //EnterCriticalSection(
                            //          &psTxMsg->m_sMsgBlocksCriticalSection);
                            //Wait for twice to block both Key and Timer threads
                            WaitForSingleObject(psTxMsg->m_hSemaphore, 
                                INFINITE);
                            WaitForSingleObject(psTxMsg->m_hSemaphore, 
                                INFINITE);
                        }
                        psDesTxMsgList->m_sTxMsgDetails = 
                            psSrcTxMsgList->m_sTxMsgDetails;
                        if (psTxMsg != NULL && bTxON == TRUE)
                        {
                            // LeaveCriticalSection(
                            //           &psTxMsg->m_sMsgBlocksCriticalSection);
                            //Release twice to block both Key and Timer threads
                            ReleaseSemaphore(psTxMsg->m_hSemaphore,
                                1,
                                lpPreviousCount);
                            ReleaseSemaphore(psTxMsg->m_hSemaphore,
                                1,
                                lpPreviousCount);
                        }
                        psSrcTxMsgList = psSrcTxMsgList->m_psNextMsgDetails;
                        if (psSrcTxMsgList != NULL)
                        {
                            if (psDesTxMsgList->m_psNextMsgDetails == NULL)
                            {
                                psDesTxMsgList->m_psNextMsgDetails = 
                                    new STXCANMSGLIST;
                                if (psDesTxMsgList->m_psNextMsgDetails == NULL)
                                {
                                    bRetVal = FALSE;
                                }
                                else
                                {
                                    psDesTxMsgList = 
                                        psDesTxMsgList->m_psNextMsgDetails;
                                    psDesTxMsgList->m_psNextMsgDetails = NULL;
                                }
                            }
                            else
                            {
                                psDesTxMsgList = 
                                    psDesTxMsgList->m_psNextMsgDetails;
                            }
                        }
                    }while(psSrcTxMsgList!= NULL  && bRetVal == TRUE);
                    // Delete the extra element if user has reduced the size.
                    if (psDesTxMsgList->m_psNextMsgDetails != NULL)
                    {
                        PSTXCANMSGLIST psDelTxMsgList = 
                            psDesTxMsgList->m_psNextMsgDetails;
                        psDesTxMsgList->m_psNextMsgDetails = NULL;
                        while(psDesTxMsgList != NULL && psDelTxMsgList != NULL)
                        {
                            psDesTxMsgList = psDelTxMsgList->m_psNextMsgDetails;
                            delete psDelTxMsgList ;
                            psDelTxMsgList  = NULL;
                            if (psDesTxMsgList != NULL)
                            {
                                psDelTxMsgList = 
                                    psDesTxMsgList->m_psNextMsgDetails;
                            }
                        }
                    }
                }
                else
                {
                    bRetVal = FALSE;
                }
            }
            if (psSrcMsgBlockList->m_psNextMsgBlocksList != NULL)
            {
                if (psDesMsgBlockList->m_psNextMsgBlocksList == NULL)
                {
                    psDesMsgBlockList->m_psNextMsgBlocksList = 
                        new SMSGBLOCKLIST;
                    vInitialiseMsgBlock(
                        psDesMsgBlockList->m_psNextMsgBlocksList);
                    if (psDesMsgBlockList->m_psNextMsgBlocksList == NULL)
                    {
                        bRetVal = FALSE;
                    }
                    else
                    {
                        psDesMsgBlockList = 
                            psDesMsgBlockList->m_psNextMsgBlocksList;
                        psDesMsgBlockList->m_psTxCANMsgList = NULL;
                        psDesMsgBlockList->m_psNextMsgBlocksList = NULL;
                    }
                }
                else
                {
                    psDesMsgBlockList = 
                        psDesMsgBlockList->m_psNextMsgBlocksList;
                }
                
            }
            psSrcMsgBlockList = psSrcMsgBlockList->m_psNextMsgBlocksList;
            if (psTxMsg != NULL)
            {
                psTxMsg = psTxMsg->m_psNextTxMsgInfo;
            }
        }while( psSrcMsgBlockList != NULL 
            && bRetVal == TRUE);
        
    }
    else
    {
        bRetVal = FALSE;
    }
    return bRetVal;
}





/******************************************************************************/
/*  Function Name    :  vReleaseMultiMsgInfo                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method releases all the memory allocated for the */
/*                      send multiple message info structure.                 */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  08.01.2004, The interface for message block           */
/*                      is changed                                            */
/******************************************************************************/
void CConfigDetails::vReleaseMultiMsgInfo(PSMSGBLOCKLIST psMsgBlockList)
{
    PSMSGBLOCKLIST psNextMsgBlockList    = NULL;
    PSTXCANMSGLIST psNextTxCANMsgList    = NULL;
    PSMSGBLOCKLIST psCurrentMsgBlockList = NULL;
    PSTXCANMSGLIST psCurrentTxCANMsgList = NULL;
    if (psMsgBlockList != NULL)
    {
        psCurrentMsgBlockList = psMsgBlockList;
        do
        {
            if (psCurrentMsgBlockList->m_unMsgCount > 0)
            {
                psCurrentTxCANMsgList = 
                    psCurrentMsgBlockList->m_psTxCANMsgList;
                do
                {
                    psNextTxCANMsgList    = 
                        psCurrentTxCANMsgList->m_psNextMsgDetails;
                    delete psCurrentTxCANMsgList;
                    psCurrentTxCANMsgList = NULL;
                    psCurrentTxCANMsgList = psNextTxCANMsgList;
                }while(psNextTxCANMsgList != NULL);
                
            }
            psNextMsgBlockList = psCurrentMsgBlockList->m_psNextMsgBlocksList;
            delete psCurrentMsgBlockList;
            psCurrentMsgBlockList = NULL;
            psCurrentMsgBlockList = psNextMsgBlockList;
        }while(psNextMsgBlockList != NULL);
    }
}

/******************************************************************************/
/*  Function Name    :  vReleaseSimSysInfo                                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method releases all the memory allocated for the */
/*                      sim sys info structure.                              */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  22.12.2005                                            */
/*  Modifications By :  
/******************************************************************************/

void CConfigDetails::vReleaseSimSysInfo()
{
   // CSimSysNodeInfo::PSSIMSYSINFO psNextSimSysInfo    = NULL;
   // CSimSysNodeInfo::PSNODELIST psNextNodeList    = NULL;
   // CSimSysNodeInfo::PSSIMSYSINFO psCurrentSimSysInfo = NULL;
   // CSimSysNodeInfo::PSNODELIST psCurrentNodeList = NULL;
   // CSimSysNodeInfo::PSSIMSYSINFO psTempSimSysInfo = theApp.psReturnSimsysInfoPtr();
   // if (psTempSimSysInfo != NULL)
   // {
   //     psCurrentSimSysInfo = psTempSimSysInfo;
   //     do
   //     {
   //         if (psCurrentSimSysInfo->m_unNumberOfNodesAdded > 0)
   //         {
   //             psCurrentNodeList = 
   //                 psCurrentSimSysInfo->m_psNodesList;
   //             do
   //             {
   //                 psNextNodeList    = 
   //                     psCurrentNodeList->m_psNextNode;
   //                 delete psCurrentNodeList;
   //                 psCurrentNodeList = NULL;
   //                 psCurrentNodeList = psNextNodeList;
   //             }while(psNextNodeList != NULL);
   //             
   //         }
   //         psNextSimSysInfo = psCurrentSimSysInfo->m_psSimsysNext;
   //         delete psCurrentSimSysInfo;
   //         psCurrentSimSysInfo = NULL;
   //         psCurrentSimSysInfo = psNextSimSysInfo;
   //     }while(psNextSimSysInfo != NULL);
   // }
   // psTempSimSysInfo = NULL;
   //// m_psSimSysInfo = NULL;
   //// m_unSimSysCount = 0;
}

/******************************************************************************/
/*  Function Name    :  bGetMsgAttrib                                         */
/*                                                                            */
/*  Input(s)         :  pMsgAttrib : out param structure                      */
/*                                                                            */
/*  Output           :  TRUE : if the method succeeds                         */
/*                      FALSE : if memory allocation fails                    */
/*                                                                            */
/*  Functionality    :  This method gets the information of the message       */
/*                      attribute structure.                                  */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :                                                        */
/******************************************************************************/
BOOL CConfigDetails::bGetMsgAttrib(PSMESSAGE_ATTRIB pMsgAttrib)
{
    BOOL bRetVal = TRUE;
    PSMESSAGE_ATTRIB pDest = pMsgAttrib;
    
    
    // count of the messages in the structure..
    pDest->m_usMsgCount = m_sMessageAttributes.m_usMsgCount;
    pDest->m_psMsgAttribDetails = NULL;
    
    if (m_sMessageAttributes.m_usMsgCount != 0)
    {
        pDest->m_psMsgAttribDetails = new SMESSAGEATTR[pDest->m_usMsgCount];
        
        if (pDest->m_psMsgAttribDetails != NULL)
        {
            PSMESSAGEATTR pDestPos = pDest->m_psMsgAttribDetails;
            PSMESSAGEATTR pSrcPos = m_sMessageAttributes.m_psMsgAttribDetails;
            
            for (int i = 0; i < pDest->m_usMsgCount; i++, 
                pDestPos++, pSrcPos++)
            {
                pDestPos->omStrMsgname = pSrcPos->omStrMsgname;
                pDestPos->sColor = pSrcPos->sColor;
                pDestPos->unMsgID = pSrcPos->unMsgID;
            }
        }
        else
        {
            // failed memory allocation
            bRetVal = FALSE;
        }
    }// no elements 
    
    return bRetVal;
}





/******************************************************************************/
/*  Function Name    :  bSetMsgAttrib                                         */
/*                                                                            */
/*  Input(s)         :  pMsgAttrib : in param structure                       */
/*                                                                            */
/*  Output           :  TRUE : if the method succeeds                         */
/*                      FALSE : if memory allocation fails                    */
/*                                                                            */
/*  Functionality    :  This method sets the information of the message       */
/*                      attribute structure.                                  */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :                                                        */
/******************************************************************************/
BOOL CConfigDetails::bSetMsgAttrib(PSMESSAGE_ATTRIB pMsgAttrib)
{
    BOOL bRetVal = TRUE;
    
    // release the current allocation
    vReleaseMsgAttrib(&m_sMessageAttributes);
    
    // count of the messages in the structure..
    m_sMessageAttributes.m_usMsgCount = pMsgAttrib->m_usMsgCount;
    m_sMessageAttributes.m_psMsgAttribDetails = NULL;
    
    if (pMsgAttrib->m_usMsgCount != 0)
    {
        m_sMessageAttributes.m_psMsgAttribDetails = 
            new SMESSAGEATTR[m_sMessageAttributes.m_usMsgCount];
        
        if (m_sMessageAttributes.m_psMsgAttribDetails != NULL)
        {
            PSMESSAGEATTR pSrcPos = pMsgAttrib->m_psMsgAttribDetails;
            PSMESSAGEATTR pDestPos = m_sMessageAttributes.m_psMsgAttribDetails;
            
            for (int i = 0; (i < pMsgAttrib->m_usMsgCount) 
                && (pDestPos != NULL) && (pSrcPos != NULL); i++)
            {
                pDestPos->omStrMsgname = pSrcPos->omStrMsgname;
                pDestPos->sColor = pSrcPos->sColor;
                pDestPos->unMsgID = pSrcPos->unMsgID;
                
                pDestPos++;
                pSrcPos++;
            }
        }
        else
        {
            // failed memory allocation
            bRetVal = FALSE;
        }
    } // no elements
    return bRetVal;
}





/******************************************************************************/
/*  Function Name    :  vReleaseMsgAttrib                                     */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method releases all the memory allocated for the */
/*                      message attribute structure.                          */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :                                                        */
/******************************************************************************/
void CConfigDetails::vReleaseMsgAttrib(PSMESSAGE_ATTRIB pData)
{
    if (pData->m_psMsgAttribDetails != NULL)
    {
        delete [] pData->m_psMsgAttribDetails;
        pData->m_psMsgAttribDetails = NULL;
    }
}





/******************************************************************************/
/*  Function Name    :  bGetMsgFilterDetails                                  */
/*                                                                            */
/*  Input(s)         :  pMsgFilterDetails : out param structure               */
/*                                                                            */
/*  Output           :  TRUE : if the method succeeds                         */
/*                      FALSE : if memory allocation fails                    */
/*                                                                            */
/*  Functionality    :  This method gets the information of the message filter*/
/*                      details                                               */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :                                                        */
/******************************************************************************/
BOOL CConfigDetails::bGetMsgFilterDetails(PSMESSAGE_FILTER_DETAILS 
                                          pMsgFilterDetails)
{
    BOOL bRetVal = TRUE;
    PSMESSAGE_FILTER_DETAILS pDest = pMsgFilterDetails;
    
    // get the information
    pDest->m_unCount = m_sMsgFilterDetails.m_unCount;
    
    // get the string array elements..
    pDest->m_omMsgNameArray.Copy(m_sMsgFilterDetails.m_omMsgNameArray);
    
    pDest->m_nRangeFrom = m_sMsgFilterDetails.m_nRangeFrom;
    pDest->m_nRangeTo = m_sMsgFilterDetails.m_nRangeTo;
    pDest->m_byFilterType = m_sMsgFilterDetails.m_byFilterType;
    pDest->m_bChekTxMsg = m_sMsgFilterDetails.m_bChekTxMsg;
    
    // always initialize...
    pDest->m_punUndefinedMsgID = NULL;
    
    if (m_sMsgFilterDetails.m_punUndefinedMsgID != NULL)
    {
        int nCount = m_sMsgFilterDetails.m_punUndefinedMsgID[0];
        
        // the first element holds the info of the number of undefined msg ids
        pDest->m_punUndefinedMsgID = new UINT [nCount + 1];
        
        pDest->m_punUndefinedMsgID[0] = nCount;
        
        if (pDest->m_punUndefinedMsgID != NULL)
        {
            for (int j = 1; j <= nCount; j++)
            {
                pDest->m_punUndefinedMsgID[j] = 
                    m_sMsgFilterDetails.m_punUndefinedMsgID[j];
            }
        }
        else
        {
            // failed memory allocation
            bRetVal = FALSE;
        }
    } // no undefined msg ids..
    return bRetVal;
}





/******************************************************************************/
/*  Function Name    :  bSetMsgFilterDetails                                  */
/*                                                                            */
/*  Input(s)         :  pMsgFilterDetails : in param structure                */
/*                                                                            */
/*  Output           :  TRUE : if the method succeeds                         */
/*                      FALSE : if memory allocation fails                    */
/*                                                                            */
/*  Functionality    :  This method sets the information of the message filter*/
/*                      details                                               */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :                                                        */
/******************************************************************************/
BOOL CConfigDetails::bSetMsgFilterDetails(PSMESSAGE_FILTER_DETAILS 
                                          pMsgFilterDetails)
{
    BOOL bRetVal = TRUE;
    PSMESSAGE_FILTER_DETAILS pSrc = pMsgFilterDetails;
    
    // release the source..
    vReleaseMsgFilterDetails(&m_sMsgFilterDetails);
    
    // get the information
    m_sMsgFilterDetails.m_unCount = pSrc->m_unCount;
    
    // get the string array elements..
    m_sMsgFilterDetails.m_omMsgNameArray.Copy(pSrc->m_omMsgNameArray);
    
    m_sMsgFilterDetails.m_nRangeFrom = pSrc->m_nRangeFrom;
    m_sMsgFilterDetails.m_nRangeTo = pSrc->m_nRangeTo;
    m_sMsgFilterDetails.m_byFilterType = pSrc->m_byFilterType;
    m_sMsgFilterDetails.m_bChekTxMsg = pSrc->m_bChekTxMsg;
    m_sMsgFilterDetails.m_punUndefinedMsgID = NULL;
    
    if (pSrc->m_punUndefinedMsgID != NULL)
    {
        int nCount = pSrc->m_punUndefinedMsgID[0];
        
        // the first element holds the info of the number of undefined msg ids
        m_sMsgFilterDetails.m_punUndefinedMsgID = new UINT [nCount + 1];
        
        if (m_sMsgFilterDetails.m_punUndefinedMsgID != NULL)
        {
            m_sMsgFilterDetails.m_punUndefinedMsgID[0] = nCount;
            
            for (int j = 1; j <= nCount; j++)
            {
                m_sMsgFilterDetails.m_punUndefinedMsgID[j] = 
                    pSrc->m_punUndefinedMsgID[j];
            }
        }
        else
        {
            // failed memory allocation
            bRetVal = FALSE;
        }
    } // no undefined msg ids
    return bRetVal;
}





/******************************************************************************/
/*  Function Name    :  vReleaseMsgFilterDetails                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method releases all the memory allocated for the */
/*                      message filter details structure.                     */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :                                                        */
/******************************************************************************/
void CConfigDetails::vReleaseMsgFilterDetails(PSMESSAGE_FILTER_DETAILS pData)
{
    pData->m_omMsgNameArray.RemoveAll();
    
    if (pData->m_punUndefinedMsgID != NULL)
    {
        delete [] pData->m_punUndefinedMsgID;
        pData->m_punUndefinedMsgID = NULL;
    }
}





/******************************************************************************/
/*  Function Name    :  vRelease                                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method releases all the memory allocated in the  */
/*                      bGetData(...) method                                  */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications    :  Raja N on 05/04/2004                                  */
/*                   :  Added an interface to release Signal Watch List memory*/
/*                      checking NULL of the pointer to pointer to avoid crash*/
/*                      if the parameter is NULL                              */
/*  Modifications    :  Raja N on 09.03.2005, Modified delete code to use     */
/*                      delete[] as controller information is now an array    */
/*  Modifications    :  Raja N on 02.05.2005, Deleted code to free window     */
/*                      position info. Window postion info is now structure   */
/*                      no need to free any memory                            */
/*  Modifications    :  Raja N on 20.07.2005, Changed emun for old version    */
/*                      filter list                                           */
/******************************************************************************/
void CConfigDetails::vRelease(eCONFIGDETAILS eParam, LPVOID* lpDataPtr)
{
    switch(eParam)
    {
    case LOG_FILE_NAME:
    case REPLAY_FILE_NAME:
    case    DATABASE_FILE_NAME:
        {
            if (*lpDataPtr != NULL)
            {
                CStringArray*  paomDBFiles =
                    static_cast<CStringArray*>( *lpDataPtr);
                delete paomDBFiles;
                *lpDataPtr = NULL;
            }
        }
        break;
    case MRU_DLL_FILE_NAME:
    case TOOLBAR_DETAILS:
        break;
    case CONTROLLER_DETAILS:
        {
            if (*lpDataPtr != NULL)
            {
                PSCONTROLER_DETAILS  psControllerDetails =
                    static_cast<PSCONTROLER_DETAILS>( *lpDataPtr);
                delete [] psControllerDetails;
                *lpDataPtr = NULL;
            }
        }
        break;
    case SEND_MULTI_MSGS:
        {
            if (*lpDataPtr != NULL)
            {
                PSMSGBLOCKLIST psMsgBlockList = 
                    static_cast<PSMSGBLOCKLIST>(*lpDataPtr);
                vReleaseMultiMsgInfo(psMsgBlockList);
                *lpDataPtr = NULL;
            }
        }
        break;
    case MSG_ATTRIBUTES:
        {
            if (*lpDataPtr != NULL)
            {
                PSMESSAGE_ATTRIB pData = 
                    static_cast<PSMESSAGE_ATTRIB>(*lpDataPtr);
                vReleaseMsgAttrib(pData);
            }
        }
        break;
    case OLD_FILTER_DETAILS:
        {
            if (*lpDataPtr != NULL)
            {
                PSMESSAGE_FILTER_DETAILS pData = 
                    static_cast<PSMESSAGE_FILTER_DETAILS>(*lpDataPtr);
                vReleaseMsgFilterDetails(pData);
            }
        }
        break;
    case SIGNAL_WATCH_LIST:
        {
            vReleaseSignalWatchListMemory();
        }
        break;
    case SIMSYS_LIST:
        {
            vReleaseSimSysListMemory();
        }
        break;
        
    default:
        {
        }
        break;
    }
    
    if (lpDataPtr != NULL && *lpDataPtr != NULL)
    {
        delete *lpDataPtr;
        *lpDataPtr = NULL;
    }
    
}

/******************************************************************************/
/*  Function Name    :  vResetAll                                             */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method releases the memory allocated towards     */
/*                      every member in the bGetData(...) method              */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications on :                                                        */
/******************************************************************************/
void CConfigDetails::vResetAll()
{
    vReleaseMultiMsgInfo(m_psMsgBlockList);
    m_psMsgBlockList = NULL;
    vReleaseMsgAttrib(&m_sMessageAttributes);
    vReleaseMsgFilterDetails(&m_sMsgFilterDetails);
}
/******************************************************************************/
/*  Function Name    :  bIsConfigurationModified                              */
/*  Input(s)         :                                                        */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  This method returns the value of                      */
/*                      m_bIsConfigurationModified                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath S                                            */
/*  Date Created     :  29.10.2002                                            */
/*  Modifications on :                                                        */
/******************************************************************************/
BOOL CConfigDetails::bIsConfigurationModified()
{
    return m_bIsConfigurationModified;
}
/******************************************************************************/
/*  Function Name    :  vSetConfigurationModified                             */
/*  Input(s)         :                                                        */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  This method sets the value of                         */
/*                      m_bIsConfigurationModified                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath S                                            */
/*  Date Created     :  02.12.2002                                            */
/*  Modifications on :                                                        */
/******************************************************************************/
void CConfigDetails::vSetConfigurationModified(BOOL bModified /* = TRUE */)
{
    m_bIsConfigurationModified = bModified;
}
/******************************************************************************/
/*  Function Name    :  bGetCheckSum                                          */
/*  Input(s)         :  omStrConfigFileName : reference to File name          */
/*                      pucCheckSum : Returns computed check sum              */
/*                      pucCheckSumInFile : Returns the chechsum in file      */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  This method computes the checksum for a given bytes   */
/*                      in a file. The file is opened and checksum is computed*/
/*                      for all bytes except the last byte. Returns TRUE for  */
/*                      sucess and FALSE for failure.                         */
/*                      chechsum value.                                       */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  16.12.2002                                            */
/*  Modifications on :  16.12.2002, review comment incorporated               */
/*  Modifications on :  02.01.2003, Get the check sum stored in file in third */
/*                                  parameter.                                */
/******************************************************************************/
/*BOOL CConfigDetails::bGetCheckSum(CString &omStrConfigFileName, 
UCHAR* pucCheckSum, UCHAR* pucCheckSumInFile)
{
CStdioFile omStdiofile;
CFileException   omException  ;
DWORD dwSize                 = 0;
DWORD dwRead                 = 0;
BOOL bReturn                 = FALSE;
BOOL bFileOpen               = FALSE;
CString omStrErrorMessage    = _T("");
TCHAR  acErrorMsg[defSIZE_OF_ERROR_BUFFER];

  // Open the configration file.
  TRY
  {
  bFileOpen = omStdiofile.Open(omStrConfigFileName,
  CFile::modeRead | CFile::typeBinary, &omException);
  if (bFileOpen!=FALSE)
  {
  UCHAR *pucBuff   = NULL;
  // Get the size of file
  dwSize = omStdiofile.GetLength(); 
  if (dwSize > 0)
  {
  pucBuff = static_cast<UCHAR*> (new UCHAR[dwSize]);
  if (pucBuff!=NULL)
  {
  // Read the whole file and put the content to pucBuff;
  dwRead = omStdiofile.Read(pucBuff,dwSize);
  // Compute the checksum
  bReturn = bComputeCheckSum(pucBuff,dwSize - 1,
  pucCheckSum);
  // Get the check sum stored in file ( Last byte)
  *pucCheckSumInFile  = pucBuff[dwSize-1];
  delete [] pucBuff;
  pucBuff = NULL;
  }
  }
  omStdiofile.Close();
  }
  else
  {
  // Get the exception error message
  omException.GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
  // configuration file  open error notification 
  AfxMessageBox(acErrorMsg ,MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);
  
    }
    
      }
      CATCH_ALL(pomE)
      {
      if (pomE != NULL)
      {
      // Get the exception error message
      pomE->GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
      //Display the error
      AfxMessageBox(acErrorMsg ,MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);
      pomE->Delete();
      }
      bReturn = FALSE;          
      }
      END_CATCH_ALL
      
        return bReturn;
}*/
/******************************************************************************/
/*  Function Name    :  bSetCheckSum                                          */
/*  Input(s)         :  omStrConfigFileName : reference to File name          */
/*                      pucCheckSum : Returns computed check sum              */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  This method computes the checksum for a given bytes   */
/*                      in a file. The file is opened and checksum is computed*/
/*                      for all bytes except the last byte. Returns TRUE for  */
/*                      sucess and FALSE for failure. The same checksum is    */
/*                      overwritten at the last byte in file.                 */
/*                      chechsum value.                                       */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  16.12.2002                                            */
/*  Modifications on :  16.12.2002, review comment incorporated               */
/******************************************************************************/
/*BOOL CConfigDetails::bSetCheckSum(CString &omStrConfigFileName,
UCHAR* pucCheckSum)
{
CStdioFile omStdiofile;
CFileException omException ;
DWORD dwSize                 = 0;
DWORD dwRead                 = 0;
BOOL bReturn                 = FALSE;
BOOL bFileOpen               = FALSE;
CString omStrStrErrorMessage = _T("");
TCHAR  acErrorMsg[defSIZE_OF_ERROR_BUFFER];

  // Open the configration file template.
  TRY
  {
  bFileOpen = omStdiofile.Open(omStrConfigFileName,
  CFile::modeReadWrite| CFile::typeBinary,
  &omException);
  if (bFileOpen!=FALSE)
  {
  UCHAR *pucBuff   = NULL;
  UCHAR  ucCheckSum = 0;
  // Get the size of file
  dwSize = omStdiofile.GetLength();        
  pucBuff = static_cast<UCHAR*> (new UCHAR[dwSize]);
  if (pucBuff!=NULL)
  {
  // Read the whole file and put the content to pucBuff;
  dwRead = omStdiofile.Read(pucBuff,dwSize);
  // compute the checksum
  bReturn = bComputeCheckSum(pucBuff,dwSize-1, &ucCheckSum);
  if (bReturn == TRUE)
  {
  // Seek to the last byte to store checksum
  omStdiofile.Seek(dwSize-1,CFile::begin);
  // Write one byte checksum
  omStdiofile.Write(&ucCheckSum,1);
  // return the checksum
  if (pucCheckSum!= NULL)
  {
  *pucCheckSum = ucCheckSum;
  }
  else
  {
  bReturn = FALSE;
  }
  }
  delete [] pucBuff;
  pucBuff = NULL;
  }
  omStdiofile.Close();
  }
  else
  {
  // Get the exception error message
  omException.GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
  // configuration file open error notification 
  AfxMessageBox(acErrorMsg ,MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);
  
    }
    
      }
      CATCH_ALL(pomE)
      {
      if (pomE != NULL)
      {
      // Get the exception error message
      pomE->GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
      //Display the error
      AfxMessageBox(acErrorMsg ,MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);
      pomE->Delete();
      }
      }
      END_CATCH_ALL
      
        return bReturn;
}*/
/******************************************************************************/
/*  Function Name    :  bComputeCheckSum                                      */
/*  Input(s)         :  pucBuff : pointer to buffer,                          */
/*                      pucCheckSum :pointer to checksum byte                 */
/*                      dwSize : number of bytes for which checksum to be     */
/*                               be computed                                  */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  This method computes the checksum for a give bytes    */
/*                      of data passed as pointer pucBuff. It return TRUE on  */
/*                      success. The paramter pucCheckSum contains computed   */
/*                      chechsum value.                                       */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  16.12.2002                                            */
/*  Modifications on :                                                        */
/******************************************************************************/
/*BOOL CConfigDetails::bComputeCheckSum(UCHAR* pucBuff,
DWORD dwSize, 
UCHAR* pucCheckSum)
{
BOOL  bReturn     = FALSE;
UCHAR ucTempByte  = 0;
// check for pointer and size to be more then zero.
if (pucBuff != NULL && dwSize >0)
{
for (UINT unIndex = 0; unIndex < dwSize; unIndex++)
{
// XOR the bytes with result and update the result. First time 
// the result will be zero and XOR will return the byte itself.
ucTempByte =  static_cast<UCHAR>(ucTempByte^ pucBuff[unIndex]);
}
// check for the pointer to be NULL. If not NULL  assign the 
// computed checksum
if (pucCheckSum != NULL)
{
*pucCheckSum = ucTempByte;
bReturn = TRUE;
}
}

  return bReturn;
}*/
/******************************************************************************/
/*  Function Name    :  psReturnMsgBlockPointer                               */
/*  Input(s)         :                                                        */
/*  Output           :  PSMSGBLOCKLISt                                        */
/*  Functionality    :  This method return the message block pointer          */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modifications on :                                                        */
/******************************************************************************/


PSMSGBLOCKLIST CConfigDetails::psReturnMsgBlockPointer()
{
    return m_psMsgBlockList;
}

PSSIMSYSARRAY CConfigDetails::psReturnSimsysArrayPtr()
{
    return m_psSimSysArray;
}

/******************************************************************************/
/*  Function Name    :  vInitialiseMsgBlock                                   */
/*  Input(s)         :  reference to pointer of  SMSGBLOCKLIST                */
/*  Output           :                                                        */
/*  Functionality    :  This method will intialise psMsgBlockList with default*/
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.01.2004                                            */
/*  Modofication By  :  Raja N                                                */
/*  Modifications on :  22.07.2004, Added init of mew members                 */
/******************************************************************************/

VOID CConfigDetails::vInitialiseMsgBlock(PSMSGBLOCKLIST& psMsgBlockList)
{
    if (psMsgBlockList != NULL)
    {
        psMsgBlockList->m_psNextMsgBlocksList = NULL;
        psMsgBlockList->m_unMsgCount          = 0;
        psMsgBlockList->m_bActive             = FALSE;
        psMsgBlockList->m_bType               = TRUE;
        psMsgBlockList->m_bTxAllFrame         = FALSE;
        psMsgBlockList->m_ucTrigger           = defTIME_TRIGGER;
        psMsgBlockList->m_omStrBlockName      = defDEFAULT_MSG_BLOCK_NAME;
        psMsgBlockList->m_psTxCANMsgList      = NULL;
        psMsgBlockList->m_ucKeyValue          = defDEFAULT_KEY_VAL;
        psMsgBlockList->m_unTimeInterval      = defDEFAULT_TIME_VAL;
    }
}
/******************************************************************************/
/*  Function Name    :  vInitialiseMsgDetails                                 */
/*  Input(s)         :  reference to pointer of  STXCANMSGLIST                */
/*  Output           :                                                        */
/*  Functionality    :  This method will intialise psMsgDetailsr with default */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.01.2004                                            */
/*  Modifications on :                                                        */
/******************************************************************************/

VOID CConfigDetails::vInitialiseMsgDetails(PSTXCANMSGLIST& psMsgDetails)
{
    if (psMsgDetails != NULL)
    {
        psMsgDetails->m_psNextMsgDetails    = NULL;
    }
    
}

/******************************************************************************/
/*  Function Name    :  bLoadStoreSignalWatchList                             */
/*                                                                            */
/*  Input(s)         :  CArchive &  - Arcihve to or from sotre/read           */
/*                                                                            */
/*  Output           :  BOOL - Success/Failure                                */
/*                                                                            */
/*  Functionality    :  This method loads/stores the Signal Watch List info   */
/*                      Store: The number of nodes in the list will be stored */
/*                      first and then each nodes will be dumped.             */
/*                      Load: The number of nodes will be read and then each  */
/*                      nodes will be read one by one                         */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Raja N                                                */
/*  Date Created     :  30.03.2004                                            */
/*  Modifications on :                                                        */
/******************************************************************************/
BOOL CConfigDetails::bLoadStoreSignalWatchList(CArchive& roCfgArchive)
{
    BOOL bSuccess = TRUE;
    
    if (roCfgArchive.IsStoring())
    {
        ASSERT(FALSE);
    }
    // Loading
    else
    {
        UINT unLength = 0;
        //Get the length of Signal Watch List
        roCfgArchive >> unLength;
        // Get the data from the archive
        if (unLength > 0)
        {
            try
            {
                // Allocate the first node
                PSSIGNALWATCHLIST  psTemp = NULL;
                psTemp = /*m_psSignalWatchList = */new sSignalWatchList;
                // Iterate through list
                for (register UINT index = 0; index < unLength; index++)
                {
                    // Get the Message id
                    roCfgArchive >> psTemp->unMsgID;
                    // Get the Signal list
                    psTemp->omCSASignals.Serialize(roCfgArchive);
                    // Create next node. Eliminate creation for the last node.
                    if (index != (unLength - 1))
                    {
                        psTemp->psNextMessageNode = new sSignalWatchList;
                        psTemp = psTemp->psNextMessageNode;
                    }
                }
            }
            catch(...)
            {
                // Exception occuered
                if (theApp.m_bFromAutomation == FALSE)
                {
                    AfxMessageBox("Error while loading Signal Watch List!!",
                        MB_ICONSTOP);
                }
                bSuccess = FALSE;
            }
        }
    }
    return bSuccess;
}


/******************************************************************************/
/*  Function Name    :  vReleaseSignalWatchListMemory                         */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This method releases all the memory allocated for the */
/*                      Signal WAtch List                   .                 */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Raja N                                                */
/*  Date Created     :  30.03.2004                                            */
/******************************************************************************/
void CConfigDetails::vReleaseSignalWatchListMemory()
{
}
//{
//    PSSIGNALWATCHLIST psTemp = NULL;
//    try
//    {
//        while( m_psSignalWatchList != NULL)
//        {
//            psTemp = m_psSignalWatchList->psNextMessageNode;
//            delete m_psSignalWatchList;
//            m_psSignalWatchList = psTemp;
//        }
//        // Initialise to zero
//        m_psSignalWatchList = NULL;
//    }
//    catch(...)
//    {
//        if (theApp.m_bFromAutomation == FALSE)
//        AfxMessageBox("Error while deleting Signal Watch List",MB_ICONSTOP);
//    }
//    
//}


/******************************************************************************/
/*  Function Name    :  bLoadStoreSimSysList                                  */
/*                                                                            */
/*  Input(s)         :  CArchive &  - Arcihve to or from sotre/read           */
/*                                                                            */
/*  Output           :  BOOL - Success/Failure                                */
/*                                                                            */
/*  Functionality    :  This method loads/stores the Simulated systemList info*/
/*                      Store: The name and path of the simulated systems are */
/*                      stored first and then each node will be dumped.       */
/*                      Load: The name and path of the simulated systems will */
/*                      be read one by one                                    */
/*                                                                            */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  19.12.2005                                            */
/*  Modifications    : Anish,15.12.2006                                       */
/*                     Changed code to compare version with defMulti_NODE     */
/******************************************************************************/
BOOL CConfigDetails::bLoadStoreSimSysList(CArchive& roCfgArchive)
{
    BOOL bSuccess = TRUE;
    
    if (roCfgArchive.IsStoring())
    {
        UINT unLength = 0;
        PSSIMSYSARRAY psSimsysTemp;
        // Take care about CS
        // Calculate Length
        try
        {
            psSimsysTemp = m_psSimSysArray;
            while( psSimsysTemp != NULL)
            {
                unLength++;
                psSimsysTemp = psSimsysTemp->psNextSimsys;
            }
            // Save the length First
            // Even there is no member in the list like 0
            roCfgArchive << unLength;
            // Processing is required only if there are some elements 
            // in the list
            if (unLength > 0)
            {
                // Reset the pointer to the begining of the list
                psSimsysTemp = m_psSimSysArray;
                // Iterate through the list
                for (register UINT index = 0;
                psSimsysTemp != NULL && index < unLength;
                index++)
                {
                    // Store the simulated system path
                    roCfgArchive << psSimsysTemp->m_omStrSimSysPath;
                    // Store the simulated system name
                    roCfgArchive << psSimsysTemp->m_omStrSimSysName;  
                    
                    // Get the next element of the list
                    psSimsysTemp = psSimsysTemp->psNextSimsys;
                }
                
            }
        }
        catch(...)
        {
            // Exception occuered
            if (theApp.m_bFromAutomation == FALSE)
            {
                AfxMessageBox("Error while saving Simulated system List!!",MB_ICONSTOP);
            }
            bSuccess = FALSE;
        }
    }
    // Loading
    else
    {
        if (m_fAppVersion >= static_cast<FLOAT>(defMULTI_NODE_VERSION))
        {
            
            UINT unLength = 0;
            //Get the length of Simulated system List
            roCfgArchive >> unLength;
            // Get the data from the archive
            if (unLength > 0)
            {
                try
                {
                    // Allocate the first node
                    PSSIMSYSARRAY psSimsysTemp = NULL;
                    psSimsysTemp = m_psSimSysArray = new SSIMSYSARRAY;
                    // Iterate through list
                    for (register UINT index = 0; index < unLength; index++)
                    {
                        // Get the simulated system path
                        roCfgArchive >> psSimsysTemp->m_omStrSimSysPath;
                        // Get the simulated system name
                        roCfgArchive >> psSimsysTemp->m_omStrSimSysName;  
                        psSimsysTemp->psNextSimsys = NULL;
                        // Create next node. Eliminate creation for the last node.
                        if (index != (unLength - 1))
                        {
                            psSimsysTemp->psNextSimsys = new SSIMSYSARRAY;
                            psSimsysTemp = psSimsysTemp->psNextSimsys;
                        }
                    }
                    
                    
                }
                catch(...)
                {
                    if (m_psSimSysArray != NULL)
                    {
                        delete m_psSimSysArray;
                        m_psSimSysArray = NULL;
                    }
                    // Exception occuered
                    if (theApp.m_bFromAutomation == FALSE)
                    {
                        AfxMessageBox("Error while loading Simulated system List!!",
                                       MB_ICONSTOP);
                    }
                    bSuccess = FALSE;
                }
            }
        }
    }
    return bSuccess;
}


/******************************************************************************/
/*  Function Name    :  vReleaseSimSysListMemory                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This method releases all the memory allocated for the */
/*                      Simulated system List                   .             */
/*  Member of        :  CConfigDetails                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  19.12.2005                                            */
/******************************************************************************/
void CConfigDetails::vReleaseSimSysListMemory()
{
    PSSIMSYSARRAY psSimsysTemp = NULL;
    while( m_psSimSysArray != NULL)
    {
        psSimsysTemp = m_psSimSysArray->psNextSimsys;
        delete m_psSimSysArray;
        m_psSimSysArray = psSimsysTemp;
    }
}


/*******************************************************************************
Function Name  : vInitGraphParamenters
Input(s)       : -
Output         : -
Functionality  : Initialised Graph List for a new configuration or while
loading old configuration file
Member of      : CConfigDetails
Author(s)      : Raja N
Date Created   : 10/12/2004
Modifications  : 
*******************************************************************************/
void CConfigDetails::vInitGraphParamenters()
{
    // Init Env Parameters
    CGraphParameters odDefaultParams;
    m_odGraphList.m_odGraphParameters = odDefaultParams;
    // Clear graph signal list
	if(m_odGraphList.m_omElementList.GetSize()>0)
		m_odGraphList.m_omElementList.RemoveAll();
}

/*******************************************************************************
Function Name  : vInitSplitterPostions
Input(s)       : -
Output         : -
Functionality  : Initialised splitter postion for a new configuration or while
loading old configuration file
Member of      : CConfigDetails
Author(s)      : Raja N
Date Created   : 19.04.2005
Modifications  : 
*******************************************************************************/
void CConfigDetails::vInitSplitterPostions()
{
    int nDefValue = -1;
    // Init Graph window splitter postion
    m_sGraphWndSplitterPos.m_nRootSplitterData[0][0] = nDefValue;
    m_sGraphWndSplitterPos.m_nRootSplitterData[0][1] = nDefValue;
    m_sGraphWndSplitterPos.m_nRootSplitterData[1][0] = nDefValue;
    m_sGraphWndSplitterPos.m_nRootSplitterData[1][1] = nDefValue;
    
    m_sGraphWndSplitterPos.m_nRightViewSplitterData[0][0] = nDefValue;
    m_sGraphWndSplitterPos.m_nRightViewSplitterData[0][1] = nDefValue;
    
    m_sGraphWndSplitterPos.m_nRightViewSplitterData[1][0] = nDefValue;
    m_sGraphWndSplitterPos.m_nRightViewSplitterData[1][1] = nDefValue;
    
    // Init Tx Window postion
    m_sTxMsgWndSplitterPos.m_nRootSplitterData[0][0] = nDefValue;
    m_sTxMsgWndSplitterPos.m_nRootSplitterData[0][1] = nDefValue;
    m_sTxMsgWndSplitterPos.m_nRootSplitterData[1][0] = nDefValue;
    m_sTxMsgWndSplitterPos.m_nRootSplitterData[1][1] = nDefValue;
    
    m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[0][0] = nDefValue;
    m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[0][1] = nDefValue;
    m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[1][0] = nDefValue;
    m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[1][1] = nDefValue;
    
    m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[0][0] = nDefValue;
    m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[0][1] = nDefValue;
    m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[1][0] = nDefValue;
    m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[1][1] = nDefValue;
}

/*******************************************************************************
Function Name  : bGetDefaultSplitterPostion
Input(s)       : eParam - Window Identity parameter
sWindowSize - Window Size
psSplitterData - Pointer to Poiinter of Splitter data struct
Output         : BOOL - Result
Functionality  : Returns default postion of splitter window based on window
size
Member of      : CConfigDetails
Author(s)      : Raja N
Date Created   : 19.04.2005
Modifications  : 
*******************************************************************************/
BOOL CConfigDetails::bGetDefaultSplitterPostion( eCONFIGDETAILS eParam,
                                                CRect omWndSize,
                                                LPVOID* psSplitterData)
{
    // Result of the operation
    BOOL bWindowFound = TRUE;
    // Select approp window
    switch( eParam)
    {
    case GRAPH_WND_SPLITTER_DATA:   // Graph Window
        {
            // Cast the data to SGRAPHSPLITTERDATA type
            PSGRAPHSPLITTERDATA pData = static_cast<PSGRAPHSPLITTERDATA>
                (*psSplitterData);
            // Init Graph window splitter postion
            if (pData != NULL)
            {
                pData->m_nRootSplitterData[0][0] =
                    (int)(omWndSize.Width() * defLEFT_VIEW_PROPOTION);
                pData->m_nRootSplitterData[0][1] = 0;
                
                pData->m_nRootSplitterData[1][0] =
                    (int)(omWndSize.Width() * defRIGHT_VIEW_PROPOTION);
                pData->m_nRootSplitterData[1][1] = 0;
                
                pData->m_nRightViewSplitterData[0][0] =
                    (int)(omWndSize.Height() * defGRAPH_VIEW_PROPOSION);
                pData->m_nRightViewSplitterData[0][1] = 0;
                
                pData->m_nRightViewSplitterData[1][0] =
                    (int)(omWndSize.Height() * defGRAPH_VIEW_PROPOSION);
                pData->m_nRightViewSplitterData[1][1] = 0;
            }
            else
            {
                // Set the result to false
                bWindowFound = FALSE;
            }
        }
        break;
    case TX_MSG_WND_SPLITTER_DATA:      // Tx Msg Configuration Window
        {
            // Init Tx Window postion
            // Cast the data to STXMSGSPLITTERDATA type
            STXMSGSPLITTERDATA * pData = static_cast<STXMSGSPLITTERDATA*>
                (*psSplitterData);
            // Init Graph window splitter postion
            if (pData != NULL)
            {
                // Root Splitter information
                pData->m_nRootSplitterData[0][0] =
                    (int)(omWndSize.Height() * defTX_WND_ROOT_SPLITTER_RATIO);
                pData->m_nRootSplitterData[0][1] = 0;
                
                pData->m_nRootSplitterData[1][0] =
                    (int)(omWndSize.Height() * ( 1 - defTX_WND_ROOT_SPLITTER_RATIO));
                pData->m_nRootSplitterData[1][1] = 0;
                
                // Left splitter information
                pData->m_nLeftViewSplitterData[0][0] =
                    (int)(omWndSize.Width() * defTX_WND_LEFT_SPLITTER_RATIO);
                pData->m_nLeftViewSplitterData[0][1] = 0;
                
                pData->m_nLeftViewSplitterData[1][0] =
                    (int)(omWndSize.Width() * (1 - defTX_WND_LEFT_SPLITTER_RATIO));
                pData->m_nLeftViewSplitterData[1][1] = 0;
                
                // Take Right view height as size
                pData->m_nRightViewSplitterData[0][0] =
                    (int)(pData->m_nRootSplitterData[0][0] *
                                 defTX_WND_RIGHT_SPLITTER_RATIO);
                pData->m_nRightViewSplitterData[0][1] = 0;
                
                pData->m_nRightViewSplitterData[1][0] = 
                    (int)(pData->m_nRootSplitterData[0][0] *
                          ( 1- defTX_WND_RIGHT_SPLITTER_RATIO));
                pData->m_nRightViewSplitterData[1][1] = 0;
            }
            else
            {
                // Set the result to false
                bWindowFound = FALSE;
            }
        }
        break;
    default:    // Invalid window type or functionality not implemented
        ASSERT( FALSE);
        bWindowFound = FALSE;
    }
    // Return the result
    return bWindowFound;
}

/*******************************************************************************
Function Name  : vLoadStoreWindowInfo
Input(s)       : roCfgArchive - Reference to CArchive object
sPlacement - Reference to WINDOWPLACEMENT structure variable
Output         : -
Functionality  : This function will read/ store window placement data from or
to CArchive
Member of      : CConfigDetails
Author(s)      : Raja N
Date Created   : 27.4.2005
Modifications  : 
*******************************************************************************/
void CConfigDetails::vLoadStoreWindowInfo( CArchive& roCfgArchive,
                                          WINDOWPLACEMENT& sPlacement)
{
    // If it is loading
    if (roCfgArchive.IsLoading())
    {
        // Read data in to the placement structure
        roCfgArchive >> sPlacement.length;
        roCfgArchive >> sPlacement.flags;
        roCfgArchive >> sPlacement.showCmd;
        roCfgArchive >> sPlacement.ptMaxPosition;
        roCfgArchive >> sPlacement.ptMinPosition;
        roCfgArchive >> sPlacement.rcNormalPosition;
    }
    else
    {
        // Save data to archive from the placement structure
        roCfgArchive << sPlacement.length;
        roCfgArchive << sPlacement.flags;
        roCfgArchive << sPlacement.showCmd;
        roCfgArchive << sPlacement.ptMaxPosition;
        roCfgArchive << sPlacement.ptMinPosition;
        roCfgArchive << sPlacement.rcNormalPosition;
    }
}

/*******************************************************************************
Function Name  : bGetDefaultValue
Input(s)       : eParam - Window Identity
sPosition - Reference to Window Position structure
Output         : -
Functionality  : This function will assign default value of window position
with respect to main frame size
Member of      : CConfigDetails
Author(s)      : Raja N
Date Created   : 2.5.2005
Modifications  : Raja N on 10.05.2005, Added break to default statement.
*******************************************************************************/
BOOL CConfigDetails::bGetDefaultValue(eCONFIGDETAILS eParam,
                                      WINDOWPLACEMENT& sPosition)
{
    BOOL bSizeFound = FALSE;
    CRect omMainFrameSize;
    CRect omToolBarRect;
    // Get the width of frame border and multiply with a factor
    int nBorderWidth = defBORDER_WIDTH_FACTOR *
        GetSystemMetrics( SM_CXBORDER);
    // Get Main Frame window size and toolbar size
    CMainFrame * pMainFrame = NULL;
    pMainFrame = (CMainFrame *)theApp.m_pMainWnd;
    // If it is a valid window pointer
    if (pMainFrame != NULL &&
        IsWindow(pMainFrame->m_hWnd))
    {
        // Get Main Frame Size
        pMainFrame->GetClientRect( &omMainFrameSize);
        omMainFrameSize.NormalizeRect();
        
        // Get Toolbar size
        CToolBarCtrl& omToolBarCtrl = 
            pMainFrame->vGetReferenceToToolBarCtrl();
        omToolBarCtrl.GetWindowRect(&omToolBarRect);
        bSizeFound = TRUE;
    }
    // If successfully calculated Mainframe and toolbar size
    if (bSizeFound == TRUE)
    {
        
        switch( eParam)
        {
        case REPLAY_WND_PLACEMENT:
            {
                CRect omRect = omMainFrameSize;
                omRect.top   += static_cast<LONG> ( omRect.Height() * 
                    defCHILD_FRAME_PROP_H);
                omRect.right = static_cast<LONG> ( omRect.right * 
                    defREPLAY_WND_PROP_X);
                
                sPosition.rcNormalPosition = omRect;
                sPosition.rcNormalPosition.top -= 2 * omToolBarRect.Height();
                sPosition.rcNormalPosition.bottom -= 2 * omToolBarRect.Height();
            }
            break;
        case OUT_WND_PLACEMENT:
            {
                CRect omRect = omMainFrameSize;
                omRect.top   += static_cast<LONG> ( omRect.Height() * 
                    defCHILD_FRAME_PROP_H);
                omRect.right -= static_cast<LONG> ( omRect.right *
                    defOUTPUT_WND_PROP_X);
                sPosition.rcNormalPosition = omRect;
                sPosition.rcNormalPosition.top -= 2 * omToolBarRect.Height();
                sPosition.rcNormalPosition.bottom -= 2 * omToolBarRect.Height();
            }
            break;
        case NOTIFICATION_WND_PLACEMENT:
            {
                CRect omRect = omMainFrameSize;
                omRect.top   += static_cast<LONG> ( omRect.Height() * 
                    defCHILD_FRAME_PROP_H);
                omRect.left += static_cast<LONG> ( omRect.right *
                    defNOTIFIC_WND_PROP_X);
                omRect.right -= nBorderWidth;
                sPosition.rcNormalPosition = omRect;
                sPosition.rcNormalPosition.top -= 2 * omToolBarRect.Height();
                sPosition.rcNormalPosition.bottom -= 2 * omToolBarRect.Height();
            }
            break;
        case MSG_WND_PLACEMENT:
            {
                CRect omRect = omMainFrameSize;
                omRect.bottom = static_cast<LONG> ( omRect.left +
                    omRect.Height() * defMSG_WND_HEIGHT_PROPOTION);
                // Reduce the border size
                omRect.right -= nBorderWidth;
                sPosition.rcNormalPosition = omRect;
            }
            break;
        case SIGWATCH_WND_PLACEMENT:   // Non - MDI Child
            {
                CRect omRect = omMainFrameSize;
                // Make the height 35% pf main frame height
                omRect.bottom = static_cast<LONG> ( omRect.top +
                    omRect.Height() * defSIGNAL_WATCH_WND_HEIGHT_RATIO);
                // Make the width 65% of main frame
                omRect.left += (LONG)( omRect.Width() *
                    defSIGNAL_WATCH_WND_WIDTH_RATIO);
                // Shift the window down by tool bar size
                omRect.top += omToolBarRect.Height();
                omRect.bottom += omToolBarRect.Height();
                // Reduce the border size
                omRect.right -= nBorderWidth;
                // Convert into screen
                pMainFrame->ClientToScreen(&omRect);
                // Assign the rect
                sPosition.rcNormalPosition = omRect;
            }
            break;
        case MSGINTERP_WND_PLACEMENT:   // Non - MDI Child
            {
                CRect omRect = omMainFrameSize;
                // Make the top starts from 35% of main frame height
                omRect.top += static_cast<LONG> ( omRect.Height() *
                    defMSG_IPET_WND_HEIGHT_RATIO);
                // Make the width 65% of main frame
                omRect.left += (LONG)( omRect.Width() * defMSG_IPET_WND_WIDTH_RATIO);
                // Shift the top down to avoid overlap with toolbar
                omRect.top += omToolBarRect.Height();
                // Reduce the border size
                omRect.right -= nBorderWidth;
                // Convert in to Screen coordinates
                pMainFrame->ClientToScreen( &omRect);
                // Assign the rect
                sPosition.rcNormalPosition = omRect;
            }
            break;
        case GRAPH_WND_PLACEMENT:
            {
                CRect omRect = omMainFrameSize;
                // Reduce Tool bar Height
                omRect.top += omToolBarRect.Height();
                // Reduce the dimention propotionally
                omRect.top += (LONG)(omRect.Height() * defGRAPH_WND_TOP_MARGIN);
                omRect.bottom -= (LONG)(omRect.Height() * defGRAPH_WND_BOTTOM_MARGIN);
                omRect.left += (LONG)(omRect.Width() * defGRAPH_WND_LEFT_MARGIN);
                omRect.right -= (LONG)(omRect.Width() * defGRAPH_WND_RIGHT_MARGIN);
                // Update the rectangle
                sPosition.rcNormalPosition = omRect;
            }
            break;
        case TX_WND_PLACEMENT:
            {
                CRect omRect = omMainFrameSize;
                // Reduce the size propotionally
                omRect.bottom -= (LONG)(omRect.Height() * defTX_MSG_WND_BOTTOM_MARGIN);
                omRect.right -= (LONG)(omRect.Width() * defTX_MSG_WND_RIGHT_MARGIN);
                // Update the size
                sPosition.rcNormalPosition = omRect;
            }
            break;
        case SIMSYS_WND_PLACEMENT:
            {
                CRect omRect = omMainFrameSize;
                // Reduce the size propotionally
                omRect.bottom -= (LONG)(omRect.Height() * defTX_MSG_WND_BOTTOM_MARGIN);
                omRect.right -= (LONG)(omRect.Width() * defTX_MSG_WND_RIGHT_MARGIN);
                // Update the size
                sPosition.rcNormalPosition = omRect;
            }
            break;
        default:
            ASSERT( FALSE);
            bSizeFound = FALSE;
            break;
        }
    }
    // Return the result
    return bSizeFound;
}

/*******************************************************************************
Function Name  : bLoadStoreDisplayFilterInfo
Input(s)       : oCfgArchive - Reference to Configuration file archive
Output         : -
Functionality  : To serialise display filter information
Member of      : CConfigDetails
Author(s)      : Raja N
Date Created   : 20.7.2005
Modifications  : 
*******************************************************************************/
BOOL CConfigDetails::bLoadStoreDisplayFilterInfo( CArchive& oCfgArchive)
{
    BOOL bResult = FALSE;
    // Loading Filter Info
    if (oCfgArchive.IsLoading())
    {
        // Only If supported in the config file
        if (m_fAppVersion >= static_cast<float>( defMULTI_FILTER_VERSION))
        {
            m_omMsgDisplayFilter.RemoveAll();
            // Get Message Filter List size
            int nSize = 0;
            oCfgArchive >> nSize;
            // Iterate through the list
            for (int nIndex = 0; nIndex < nSize; nIndex++)
            {
            	SMODULEFILTER sFilter = { "", FALSE };
                //oCfgArchive >> sFilter.m_omFilterName;
                //oCfgArchive >> sFilter.m_bEnabled;
                m_omMsgDisplayFilter.Add( sFilter);
            }
            bResult = TRUE;
        }
        else
        {
            // Clear existing items
            m_omMsgDisplayFilter.RemoveAll();
            // Set the result
            bResult = TRUE;
        }
    }
    else
    {
        ASSERT(FALSE);
    }
    //return the result
    return bResult;
}
BOOL static bLogFileSerialize(CArchive& omArchive, SLOGFILEDETS& sLogFileDets)
{
    BOOL bResult = FALSE;
    if( omArchive.IsLoading())
    {
        // Assign File Name
	    omArchive >> sLogFileDets.m_omStrFileName;
	    // Time Mode
	    omArchive >> sLogFileDets.m_nTimeMode;
	    // Numeric Mode
	    omArchive >> sLogFileDets.m_nNumMode;
	    // File Mode
	    omArchive >> sLogFileDets.m_nFileMode;
	    // Enable Option
	    omArchive >> sLogFileDets.m_bEnabled;
	    // Copy Trigger List
	    omArchive >> sLogFileDets.m_sLogTrigger.m_unStartID;
        omArchive >> sLogFileDets.m_sLogTrigger.m_unStopID;
        int unTrigType = 0;
        omArchive >> unTrigType;
        switch (unTrigType)
        {
            case 0:
            {
                sLogFileDets.m_sLogTrigger.m_unTriggerType = NONE;
            }
            break;
            case 1:
            {
                sLogFileDets.m_sLogTrigger.m_unTriggerType = START;
            }
            break;
            case 2:
            {
                sLogFileDets.m_sLogTrigger.m_unTriggerType = STOP;
            }
            break;
            case 3:
            {
                sLogFileDets.m_sLogTrigger.m_unTriggerType = BOTH;
            }
            break;
            case 4:
            {
                sLogFileDets.m_sLogTrigger.m_unTriggerType = STOPPED;
            }
            break;
        }
	    // Copy Filter
        // Get the size
        sLogFileDets.m_omFilter.RemoveAll();
        int nSize;
        omArchive >> nSize;
        // Get the filters
        for( int nIndex = 0; nIndex < nSize; nIndex++ )
        {
            SMODULEFILTER sFilter = {_T(""), FALSE};
            if( omArchive.IsLoading() )
            {
                CString omTemp;
                omArchive >> omTemp;
                strcpy (sFilter.m_omFilterName, omTemp.GetBuffer(MAX_PATH));
                omArchive >> sFilter.m_bEnabled;
            }
            sLogFileDets.m_omFilter.Add( sFilter );
        }
        bResult = TRUE;
    }
    return bResult;
}
//A function to load and store log details
BOOL static bLoadSaveLogInfo(CArchive& omArchive, SLOGCONFIGDETS& sLogConfigDets)
{
    BOOL bResult = FALSE;
    // Loading
    if( omArchive.IsLoading() )
    {
        // Serialise Log Files
        int nSize = 0;
        omArchive >> nSize;
        sLogConfigDets.m_unCount = nSize;
        // Serialise each log files
        for( int nIndex = 0; nIndex < nSize; nIndex++ )
        {
            bLogFileSerialize( omArchive, sLogConfigDets.m_asLogFileDets[nIndex]);
        }
        // Get Enable during connect option
        omArchive >> sLogConfigDets.m_bEnableDuringConnect;
        // Set the result to true
        bResult = TRUE;
    }
    //Storing
    else
    {
        ASSERT(FALSE);
    }
    return bResult;
}
/*******************************************************************************
Function Name  : bLoadStoreLogFileInfo
Input(s)       : omArchive - Reference to configuration file archive
Output         : -
Functionality  : To serialise log file info
Member of      : CConfigDetails
Author(s)      : Raja N
Date Created   : 20.7.2005
Modifications  : 
*******************************************************************************/
BOOL CConfigDetails::bLoadStoreLogFileInfo(CArchive& omArchive)
{
    BOOL bResult = TRUE;
    // Loading file
    if (omArchive.IsLoading())
    {
        if (m_fAppVersion >= static_cast<float>( defMULTI_FILTER_VERSION))
        {
            bLoadSaveLogInfo(omArchive, m_sLogConfigDets);
        }
        else
        {
            ASSERT(FALSE);
        }
    }
    else
    {
        ASSERT(FALSE);
    }
    return bResult;
}

static BOOL bLoadReplayFile(CArchive& omArchive, SREPLAYFILE& sRepalyFile)
{
    // Load File Name
    omArchive >> sRepalyFile.m_omStrFileName;
    // Load time mode
    omArchive >> sRepalyFile.m_nTimeMode;
    // Load Message Time delay
    omArchive >> sRepalyFile.m_unMsgTimeDelay;  
    // Load replay mode
    omArchive >> sRepalyFile.m_nReplayMode;
    // Load cycle delay
    omArchive >> sRepalyFile.m_unCycleTimeDelay;
    // Load filter details
    sRepalyFile.m_omFilter.RemoveAll();
    // Load the size
    int nSize;
    omArchive >> nSize;
    // Load individual filters
    for( int nIndex = 0; nIndex < nSize; nIndex++ )
    {
        SMODULEFILTER sFilter;
        // Serialize filters
        CString omFilterName;
        omArchive >> omFilterName;
        omArchive >> sFilter.m_bEnabled;
        
        strcpy(sFilter.m_omFilterName, omFilterName.GetBuffer(MAX_PATH));
        // Add the filter in to the list
        sRepalyFile.m_omFilter.Add( sFilter );
    }
    // Load Enabled Option
    omArchive >> sRepalyFile.m_bEnabled;
    // Load Interactive Option
    omArchive >> sRepalyFile.m_bInteractive;
    // Set the result
    return TRUE;
}
static BOOL bLoadReplayDetails(CArchive& omArchive, SREPLAYDETS& sRepalyDets)
{
    BOOL bResult = FALSE;
    // Loading
    if( omArchive.IsLoading() )
    {
        int nSize = 0;
        omArchive >> nSize;
        sRepalyDets.m_unCount = nSize;
        // Serialise each log files
        for( int nIndex = 0; nIndex < nSize; nIndex++ )
        {
           bResult = bLoadReplayFile(omArchive, sRepalyDets.m_asReplayFile[nIndex]);
        }
        // Set the result to true
        bResult = TRUE;
    }
    //Storing
    else
    {
        ASSERT(FALSE);
    }
    return bResult;
}

/*******************************************************************************
Function Name  : bLoadStoreReplayFileInfo
Input(s)       : omArchive - Reference to configuration file archive
Output         : -
Functionality  : To serialise replay file info
Member of      : CConfigDetails
Author(s)      : Raja N
Date Created   : 20.7.2005
  Modifications  : 
*******************************************************************************/
BOOL CConfigDetails::bLoadStoreReplayFileInfo(CArchive& omArchive)
{
    BOOL bResult = TRUE;
    // Loading file
    if (omArchive.IsLoading())
    {
        if (m_fAppVersion >= static_cast<float>( defMULTI_FILTER_VERSION))
        {
            bResult =
                bLoadReplayDetails(omArchive, m_sReplayDetails);
        }
        else
        {
            //CReplayManager::ouGetReplayManager().vInitReplayManager();
        }
    }
    else
    {
        ASSERT(FALSE);
    }
    return bResult;
}

INT  CConfigDetails::COM_nSaveConfiguration(CString& omStrCfgFilename)
{
    UINT unErrorCode = defCONFIG_FILE_SUCCESS;
    
    // save this file only if the file is saved as a new one, or there has been 
    // an updation in the data
    if ((omStrCfgFilename != m_omstrConfigFilename) || (m_bIsDirty == TRUE))
    {
        // if filename is empty, then it means that the data should be saved
        // into the last loaded file...
        if (omStrCfgFilename.IsEmpty() == TRUE)
        {
            omStrCfgFilename = m_omstrConfigFilename;
        }
        // first check if the file exists
        unErrorCode = nIsCfgFileFound(omStrCfgFilename, FALSE); 
        
        if (unErrorCode == defCONFIG_FILE_SUCCESS)
        {
            // Now that the configuration file has been found, validate the 
            // header and the last modified time to ascertain that the file has
            // not been corrupted after the last time it was saved.
            // extract the information from the file
            if (nLoadStoreData(CArchive::store) != defCONFIG_FILE_SUCCESS)
            {
                if (m_omstrTempFilename.IsEmpty() == FALSE)
                {
                    COPYFILE(m_omstrTempFilename, m_omstrConfigFilename);
                }
            }
            else
            {
                // reset the dirty flag.
                m_bIsDirty = FALSE;
                m_bIsConfigurationModified = FALSE;
                // Set the checksum as computed.
                if (CComputeCheckSum::bSetCheckSum(omStrCfgFilename,&m_ucCheckSum) == FALSE)
                {
                    if (m_omstrTempFilename.IsEmpty() == FALSE)
                    {
                        COPYFILE(m_omstrTempFilename, m_omstrConfigFilename);
                    }
                }
            }
        }
    }
    
    if (m_omstrTempFilename.IsEmpty() == FALSE)
    {
        // delete the temporary file
        ::DeleteFile(m_omstrTempFilename);
    }
    
    return unErrorCode;
}