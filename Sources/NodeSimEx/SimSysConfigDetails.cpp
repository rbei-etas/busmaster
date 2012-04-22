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
 * \file      SimSysConfigDetails.cpp
 * \brief     Contains the description of the class CSimSysConfigDetails
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains the description of the class CSimSysConfigDetails
 */

#include "NodeSimEx_stdafx.h"	// standard includes present in this header
#include "HashDefines.h"
#include "GlobalObj.h"
#include "SimSysConfigDetails.h"// definition of the class CSimSysConfigDetails
#include "SimSysManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/******************************************************************************/
/*  Function Name    : CSimSysConfigDetails                                   */
/*                                                                            */
/*  Input(s)         :  -                                                     */
/*  Output           :  -                                                     */
/*                                                                            */
/*  Functionality    : Constructor                                            */
/*                                                                            */
/*  Member of        :  -                                                     */
/*  Friend of        :  -                                                     */
/*                                                                            */
/*  Author(s)        : Harika M                                               */
/*  Date Created     : 22.12.2005                                             */
/*  Modifications    : 
/******************************************************************************/
CSimSysConfigDetails::CSimSysConfigDetails(ETYPE_BUS eBus) 

{
    m_eBus = eBus;
}
/******************************************************************************/
/*  Function Name    :  ~CSimSysConfigDetails                                 */
/*                                                                            */
/*  Input(s)         :   -                                                    */
/*  Output           :   -                                                    */
/*                                                                            */
/*  Functionality    :  Destructor                                            */
/*                                                                            */
/*  Member of        :   -                                                    */
/*  Friend of        :   -                                                    */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  22.12.2005                                            */
/*  Modifications    :  
/******************************************************************************/
CSimSysConfigDetails::~CSimSysConfigDetails()
{
}

/******************************************************************************/
/*  Function Name    :  nIsCfgFileFound                                       */
/*  Input(s)         :  omstrFilename : configuration filename                */
/*                                      If 0 is supplied, then default     */
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
/*  Member of        :  CSimSysConfigDetails                                  */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  
/*  Date Created     :  
/*  Modifications on :									                      */
/******************************************************************************/
INT CSimSysConfigDetails::nIsCfgFileFound(CString omStrFilename)
{
    // bOpenExisiting is TRUE, if the callee of the function is 
    // nSaveConfiguration(...), and FALSE if the callee is 
    // nLoadConfiguration.
    // Hence for loading, the file can be opened in GENERIC_READ
    // access and for saving, the file can be opened in GENERIC_WRITE
    // access.
	
    INT nRetVal             = defCONFIG_FILE_SUCCESS;
    DWORD dwDesiredAccess   = 0;
	
    // validate the extension
    INT nDotPosn = omStrFilename.ReverseFind(defDOT);
    // we found the '.'
    if(nDotPosn != -1)
    {
        CString ostrExtn = omStrFilename.Right(omStrFilename.GetLength() - nDotPosn);
        if(ostrExtn != defSIMVALIDEXTN)
        {
            nRetVal = defCONFIG_FILE_INVALID_FILE_EXTN;
        }
    }
    if (nRetVal != defCONFIG_FILE_INVALID_FILE_EXTN)
    {
        if (_taccess(omStrFilename, 0) == -1)
        {
            nRetVal = defCONFIG_FILE_NOT_FOUND;
        }
    }
	
    return nRetVal;
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
/*                      parameter. If the m_bIsConfigurationModified is TRUE, */
/*                      then the contents are written into the file again.Note*/
/*                      that all the contents are written into irrespective of */
/*                      number of the items that could have changed. The file */
/*                      header information is first written into the file,    */
/*                      followed by the all the configuration items and       */
/*                      finally the last modified date and time information is*/
/*                      written into the file in the format dd:mm:yyyy:hh:mm. */
/*                      The file is then closed.                              */
/*                                                                            */
/*  Member of        :  CSimSysConfigDetails                                  */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  22.12.2005                                            */
/*  Modifications on :  
/******************************************************************************/
INT  CSimSysConfigDetails::nSaveConfiguration(CString omStrCfgFilename, 
                                              PSSIMSYSINFO &psSimSys )

{
    UINT unErrorCode = defCONFIG_FILE_ERROR;
    if (psSimSys != NULL)
    {
        unErrorCode = defCONFIG_FILE_SUCCESS;
        // first check if the file exists
	    unErrorCode = nIsCfgFileFound(omStrCfgFilename); 
	    CString strTempFile;
        if(unErrorCode == defCONFIG_FILE_SUCCESS)
        {
            // if file exists then store data in different temporary file 
            //so that if saving gives error then old file will not be corrupted
            //once the file is saved properly then copy this data to original file
            INT nPos = omStrCfgFilename.ReverseFind(defDOT);	
	        strTempFile = omStrCfgFilename.Left(nPos);
	        strTempFile += defDOT;
	        strTempFile += defTMPFILEEXTN;
            if(nLoadStoreData(CArchive::store , strTempFile, psSimSys) == defCONFIG_FILE_SUCCESS)
            {
                //If the file was existng then copy the temp file data in the 
                //original file and delete temp file
                COPYFILE(strTempFile, omStrCfgFilename);
                ::DeleteFile(strTempFile);
                UCHAR ucCheckSum;
                bSetCheckSum(omStrCfgFilename,&ucCheckSum);
            }
            else
            {
                CString strError; 
                strError.Format(defSAVECONFIGERRSTR, omStrCfgFilename);
                AfxMessageBox(strError, MB_OK);
            }
        }
        else if(unErrorCode == defCONFIG_FILE_NOT_FOUND)
        {
            //If file doesn't exists
            if(nLoadStoreData(CArchive::store , omStrCfgFilename, psSimSys) == defCONFIG_FILE_SUCCESS)
            {
                //If the file was existng then copy the temp file data in the 
                //original file and delete temp file
                UCHAR ucCheckSum;
                bSetCheckSum(omStrCfgFilename,&ucCheckSum);
            }
            else
            {
                CString strError; 
                strError.Format(defCONFIGSAVEERR, omStrCfgFilename);
                AfxMessageBox(strError, MB_OK);
            }
        }
        //Reset the modified flag
        psSimSys->m_bIsSimSysModified = FALSE;
    }
    return unErrorCode;
}
/******************************************************************************/
/*  Function Name    :  nLoadStoreData                                        */
/*                                                                            */
/*  Input(s)         :  nMode : mode in which the CArchive object should      */
/*                              function  
                        CString& omStrSimSysPath                              */
/*                                                                            */
/*  Output           :  TRUE : if the method is successful                    */
/*                      FALSE : if any method throws an exception             */
/*                                                                            */
/*  Functionality    :  This method loads/stores the information from/to the  */
/*                      configuration file. 
/*                                                                            */
/*  Member of        :  CSimSysConfigDetails                                  */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  
/*  Date Created     :  
/*  Modifications on :  
/******************************************************************************/

int CSimSysConfigDetails::nLoadStoreData(UINT unArchiveMode , 
                                         CString omStrSimSysPath,
                                         PSSIMSYSINFO &psCurrSimSys) 

{
	int nRetVal = defCONFIG_FILE_SUCCESS;	
	CFile oConfigFile;
    UINT uFileFlag;
    if (unArchiveMode == CArchive::load)
    {
        //If loading then open existing file as read only
        uFileFlag = CFile::modeRead;
    }
    else
    {
        //If it is saving create the file if not present
        uFileFlag = CFile::modeWrite | CFile::modeCreate;
    }
    try
    {
        CFileException ex;
        if (oConfigFile.Open(omStrSimSysPath, uFileFlag, &ex))
        {
            CArchive oSimCfgArchive(&oConfigFile, unArchiveMode);
			if(oSimCfgArchive.IsLoading())
			{
                CString omCopyRight;    //to support older version
                
				// extract the header information
				oSimCfgArchive >> m_fSimSysVersion;
                oSimCfgArchive >> omCopyRight;
                
                //If loading create new simsysinfo and add it to the list later
                psCurrSimSys = new sSIMSYSINFO;
                if ( psCurrSimSys != NULL)
                {
                    psCurrSimSys->m_omStrSimSysName = omStrSimSysPath;
                }
                if ((m_fSimSysVersion < static_cast<FLOAT> (defSIMSYSVERSION_CAN)))
                {
                    int nCount = 1; //Simsys count is stored in the old version
                    oSimCfgArchive >> nCount;
                }
			}
    		
			if(oSimCfgArchive.IsStoring())
			{
                CString omCopyRight = defSIMSYSCOPYRIGHT; 
				// extract the header information
				m_fSimSysVersion = static_cast<FLOAT> (defSIMSYSVERSION_ALL_BUS);
				oSimCfgArchive << m_fSimSysVersion;
                oSimCfgArchive << omCopyRight;
			}
    		
			if( psCurrSimSys != NULL )
			{
				// Load Sim Sys details
				if( (bLoadStoreSimSysInfo(oSimCfgArchive,psCurrSimSys) != TRUE))
				{
					nRetVal = defCONFIG_FILE_CORRUPT;
				}
			}
		    // close the archive
		    oSimCfgArchive.Close();
	        // close the file
	        oConfigFile.Close();
        }
        else
        {
            nRetVal = defCONFIG_FILE_OPEN_FAILED;
        }
	}
    catch(...)
    {
        AfxMessageBox(_T("An exception occurred while reading Config file"));
    }
	return nRetVal;
}

		
/******************************************************************************/
/*  Function Name    :  bLoadStoreSimSysInfo                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method loads/stores the sim sys info into		  */
/*                      the archive.                                          */
/*  Member of        :  CSimSysConfigDetails                                  */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  
/*  Date Created     :  
/*  Modifications By :  
/******************************************************************************/
BOOL CSimSysConfigDetails:: bLoadStoreSimSysInfo (CArchive& roSimCfgArchive, 
                                                  PSSIMSYSINFO &psSimsysInfo)
{
    BOOL bRetVal = FALSE;
#ifndef _DEBUG
    if (psSimsysInfo != NULL)
    {
        bRetVal = TRUE;
        // loading the data
        CString omTemp;
        if(roSimCfgArchive.IsLoading())
        {	
            if (m_fSimSysVersion < static_cast<FLOAT> (defSIMSYSVERSION_CAN))
            {
                roSimCfgArchive >> omTemp; //Simsys name is stored in the old version
            }            
            roSimCfgArchive >> psSimsysInfo->m_unNumberOfNodesAdded;
            if(psSimsysInfo->m_unNumberOfNodesAdded > 0)
            {
                int nValidNodeCount = 0;
                for( UINT i = 0; i < psSimsysInfo-> m_unNumberOfNodesAdded; i++ )
                {
                    PSNODELIST  psCurrentNodeList = new SNODELIST(m_eBus);
                    if(psCurrentNodeList  != NULL )
                    {
                        roSimCfgArchive >> psCurrentNodeList->m_sNodeInfo.m_omStrDllName;
                        roSimCfgArchive >> psCurrentNodeList->m_sNodeInfo .m_omStrFileName;
                        roSimCfgArchive >> psCurrentNodeList->m_sNodeInfo .m_omStrNodeName;
                        if (m_fSimSysVersion == static_cast<FLOAT>(defSIMSYSVERSION_ALL_BUS))
                        {
                            roSimCfgArchive >> psCurrentNodeList->m_sNodeInfo.m_byPrefAddress;
                            roSimCfgArchive >> psCurrentNodeList->m_sNodeInfo.m_unEcuName;
                        }
                        //Add to the list
                        if ( !psCurrentNodeList->m_sNodeInfo .m_omStrNodeName.IsEmpty())
                        {
                            psSimsysInfo->vAddNodeToList(psCurrentNodeList);
                            CGlobalObj::ouGetObj(m_eBus).RegisterNodeToDIL(TRUE, &(psCurrentNodeList->m_sNodeInfo));
                            ++nValidNodeCount;
                        }
                        else
                        {
                            delete psCurrentNodeList;
                        }
                    }
                    else
                    {
                        bRetVal = FALSE;
                    }
                }
                psSimsysInfo-> m_unNumberOfNodesAdded = nValidNodeCount;
            }
            else
            {
                //Initialize the node list
                psSimsysInfo->m_psNodesList = NULL;
            }
        }
        // store the data
        if(roSimCfgArchive.IsStoring())
        {
            roSimCfgArchive << psSimsysInfo->m_unNumberOfNodesAdded;
            if(psSimsysInfo->m_unNumberOfNodesAdded > 0)
            {
                PSNODELIST psCurrentNodeList  = NULL;
                psCurrentNodeList  = psSimsysInfo->m_psNodesList;
                while(psCurrentNodeList != NULL)
                {
                    roSimCfgArchive << psCurrentNodeList->m_sNodeInfo.m_omStrDllName;
                    roSimCfgArchive << psCurrentNodeList->m_sNodeInfo.m_omStrFileName;
                    roSimCfgArchive << psCurrentNodeList->m_sNodeInfo.m_omStrNodeName;
                    roSimCfgArchive << psCurrentNodeList->m_sNodeInfo.m_byPrefAddress;
                    roSimCfgArchive << psCurrentNodeList->m_sNodeInfo.m_unEcuName;
                    //Go to the next node
                    psCurrentNodeList  = psCurrentNodeList->m_psNextNode;
                }
            }
        }
    }
#endif
    return bRetVal;
}

/******************************************************************************/
/*  Function Name    :  nLoadConfiguration                                    */
/*                                                                            */
/*  Input(s)         :  omStrFilename : name of the configuration file whose  */
/*                      information needs to be loaded                        */
/*  Output           :  All the error codes returned by nIsCfgFileFound(..)   */
/*                      defCONFIG_FILE_CORRUPT : if timestamp information     */
/*                                               stored in the input cfg file */
/*                                               does not match with the last */
/*                                               modified information of the  */
/*                                               file                         */
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
/*  Member of        :  CSimSysConfigDetails                                  */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  27.12.2005                                            */
/*  Modifications on :  
/******************************************************************************/
INT  CSimSysConfigDetails::nLoadConfiguration(CString& omStrFilename, 
                                              PSSIMSYSINFO &psSimSys)
{
    UCHAR ucCheckSum        = 0;
    UCHAR ucCheckSumInFile  = 0;
    BOOL bRet = FALSE;
    // Compute the checksum value in file
    bRet = CComputeCheckSum::bGetCheckSum(omStrFilename, &ucCheckSum,&ucCheckSumInFile);
    // first check if the file exists
    UINT unErrorCode = nIsCfgFileFound(omStrFilename); 
    if(unErrorCode == defCONFIG_FILE_SUCCESS)
    {
        // Now that the configuration file has been found, validate the header
        // and the checksum to ascertain that the file has not been 
        // corrupted after the last time it was saved.
        // extract the information from the file
        if( (ucCheckSum == ucCheckSumInFile ) && (bRet == TRUE))
        {
            // Load the config data
            unErrorCode = nLoadStoreData(CArchive::load , omStrFilename, psSimSys );
            
        }
        else
        {
            // file has been modified by external sources after the last
            // modification by the application.
            unErrorCode = defCONFIG_FILE_CORRUPT;
        }
    }	
    return unErrorCode;
}

