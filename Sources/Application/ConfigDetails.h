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
 * \file      ConfigDetails.h
 * \brief     This header file contains the definition of the class
 * \authors   Gopinath R. Majumdar, Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the definition of the class
 */

#pragma once

// For Graph Parameter Class
#include "GraphParameters.h"
// For Graph Element class
#include "GraphElement.h"
// For Graphics List class
#include "GraphList.h"
#include "Utility/ComputeCheckSum.h" // For CComputeCheckSum class definition
#include "Datatypes/SigWatch_datatypes.h" // Signal list selected
#include "Datatypes/Log_datatypes.h"

struct SLOGFILEDETS
{
    CString                               m_omStrFileName;
    INT                                   m_nTimeMode;
    INT                                   m_nNumMode;
    INT                                   m_nFileMode;
    BOOL                                  m_bEnabled;
    SLOGTRIGGER                           m_sLogTrigger;
    CModuleFilterArray                    m_omFilter;
};
const int MAX_FILE_COUNT = 0x100;
struct SLOGCONFIGDETS
{
    UINT m_unCount;
    SLOGFILEDETS m_asLogFileDets[MAX_FILE_COUNT];
    BOOL m_bEnableDuringConnect;
};
struct SREPLAYFILE
{
    CString m_omStrFileName;// Replay File Name
    int m_nTimeMode;        // 0 - Retain Delay, 1 - Specific Delay
    UINT m_unMsgTimeDelay;  // Time delay between messages in case time mode = 1
    int m_nReplayMode;      // 0 - Monoshot, 1 - Cyclic
    UINT m_unCycleTimeDelay;// Cyclic delay in case replay mode = 1
    BOOL    m_bEnabled;     // To indicate replay is enabled or not
    BOOL    m_bInteractive; // 0 - Non Interactive, 1 - interactive
    CModuleFilterArray m_omFilter;
};
struct SREPLAYDETS
{
    UINT m_unCount;
    SREPLAYFILE m_asReplayFile[MAX_FILE_COUNT];
};

struct SMSG_FRMT_WND
{
    SHORT m_shColHdrPos[10];
    BOOL  m_bColShowHideStatus[10];
    BOOL  m_bDisplayFormat; //HEX or DEC
    BOOL  m_bDisplayMode; //Overwrite or append
    BOOL  m_bInterpretStatus;
    SHORT  m_shTimeDisplay;// System, relative , absolute
    SMSG_FRMT_WND()
    {
        for (SHORT i = 0; i < 10 ; i++)       //for (INT i = 0; i < 10 ; i++)
        {
            m_shColHdrPos[i] = i;
            m_bColShowHideStatus[i] = TRUE;
        }

        m_bDisplayFormat = TRUE;
        m_bDisplayMode = TRUE;
        m_bInterpretStatus = FALSE;
        m_shTimeDisplay = 0x0;
    }

};
class CConfigDetails : public CComputeCheckSum
{
    // Public Members
public:
    //CSimSysNodeInfo* m_pSimSysNodeInfo;
    // Get message block pointer
    PSMSGBLOCKLIST psReturnMsgBlockPointer();

    PSSIMSYSARRAY psReturnSimsysArrayPtr();

    // To indicate configuration module modification
    void vSetConfigurationModified( BOOL bModified = TRUE );
    // c-tor
    CConfigDetails();
    // d-tor
    ~CConfigDetails();

    // Returns m_bIsDirty flag value
    BOOL    bIsConfigurationModified();
    // allocates and fills the pointer to pointer member with the data
    // corresponding to the eParam.
    BOOL    bGetData(eCONFIGDETAILS  eParam, LPVOID* lpData);
    // reinitializes the data member correponding to eParam with the data
    // as read from lpVoid..
    BOOL    bSetData(eCONFIGDETAILS  eParam, LPVOID lpVoid);

    // initializes all the members with their default values..
    void    vInitDefaultValues();

    // loads a new configuration..
    INT     nNewConfiguration(CString& omStrFilename);
    // loads a configuration info from a file that is already present..
    INT     nLoadConfiguration(CString& omStrFilename);
    // writes the configuration info into the file omStrCfgFilename
    INT     nSaveConfiguration (CString& omStrCfgFilename);

    // releases the memory allocated when sending the information for ePara
    // in the bGetData(..) method
    void    vRelease(eCONFIGDETAILS eParam, LPVOID* lpDataPtr);
    // releases all the memory allocated for the data members..
    void    vResetAll();
    // To get default Window Coordinates
    BOOL bGetDefaultValue( eCONFIGDETAILS eParam,
                           WINDOWPLACEMENT& sPosition );
    // To get default splitter postion based on window size
    BOOL bGetDefaultSplitterPostion( eCONFIGDETAILS eParam,
                                     CRect omWindowSize,
                                     LPVOID* psSplitterData );

    // releases memory allocated for the simulated system Information
    void vReleaseSimSysInfo();
    // Protected memebers
protected:

    // Private Members
private:
    SREPLAYDETS m_sReplayDetails;
    SFILTERAPPLIED_CAN m_sFilterConfigured;
    SLOGCONFIGDETS m_sLogConfigDets;
    // flag to store the status of the data except window co-ord
    BOOL                    m_bIsConfigurationModified;
    // flag to hold the status of the load of configuration file
    BOOL                    m_bConfigInfoLoaded;
    // flag to store the status of the data
    BOOL                    m_bIsDirty;
    // handle to the configuration file currently loaded
    HANDLE                  m_hConfigFile;
    // version information
    float                   m_fAppVersion;
    // message name that is written/read from configuration
    CString                 m_omStrMsgName;
    // message id that is written/ read from configuration
    CString                 m_omStrMsgID;
    // name of the configuration file currently loaded
    CString                 m_omstrConfigFilename;
    // name of the file that is a copy of the currently loaded config file
    CString                 m_omstrTempFilename;
    // last used C File written/read from configuration
    CString                 m_omStrMruCFile;
    // last used log file written/read from configuration
    CString                 m_omStrLogFilename;
    // last used replay file written/read from configuration
    CString                 m_omStrReplayFilename;
    // last used database file written/read from configuration
    CStringArray*           m_pomaStrDatabaseFilename;
    // last used DLL file written/read from configuration
    CString                 m_omStrMruDLLFilename;
    // copyright info written/read from configuration
    CString                 m_omStrCopyright;
    // toolbar info written/read from configuration
    STOOLBARINFO            m_sToolBarButtonStatus;
    // message attribute written/read from configuration
    SMESSAGE_ATTRIB         m_sMessageAttributes;
    // ouput, replay and message window coords
    SWND_CO_ORDINATES       m_sWindowCoOrdinates;
    // baud rate details written/read from configuration
    SCONTROLLER_DETAILS      m_sControllerDetails[ defNO_OF_CHANNELS ];
    // send multiple msgs written/read from configuration
    PSMSGBLOCKLIST          m_psMsgBlockList;
    UINT                    m_unNumberOfMsgBlockCount;
    // message filter details written/read from configuration
    SMESSAGE_FILTER_DETAILS m_sMsgFilterDetails;
    // Filter for Message Display
    CModuleFilterArray      m_omMsgDisplayFilter;
    // dword val, whose each bit represent the modified state of a
    // particular configuration info
    DWORD m_dwModifiedVals;
    // check sum of the bytes store in the file.
    UCHAR m_ucCheckSum;
    // Pointer to Signal Watch List data structure
    PSSIGNALWATCHLIST       m_psSignalWatchList;
    // Init message Buffer Parameters
    UINT m_unAppendBufferSize, m_unOverWriteBufferSize;
    // Display Refresh Rate
    UINT m_unDisplayUpdateRate;
    // Object for Graph List
    CGraphList  m_odGraphList;

    // Pointer to Simulated system List data structure
    PSSIMSYSARRAY m_psSimSysArray;


    // Window placement structures
    WINDOWPLACEMENT m_sReplayWndCoOrd,
                    m_sOutputWndCoOrd,
                    m_sNotificWndCoOrd,
                    m_sMsgWndCoOrd,
                    m_sSigWatchWindow,
                    m_sMsgInterpWindow,
                    m_sGraphWndCoOrd,
                    m_sTxWndCoOrd,
                    m_sSimSysCoOrd;

    // Splitter information
    // Tx Msg Splitter Position
    STXMSGSPLITTERDATA m_sTxMsgWndSplitterPos;
    // Graph window splitter postion
    SGRAPHSPLITTERDATA m_sGraphWndSplitterPos;

    // To Initialise graph parameters after configuration load/create
    void vInitGraphParamenters();
    VOID vInitialiseMsgDetails(PSTXCANMSGLIST& psMsgDetails);
    VOID vInitialiseMsgBlock(PSMSGBLOCKLIST& psMsgBlockList);


    /*    // compute the check for a buffer pass as parameter
    BOOL bComputeCheckSum(UCHAR *pcBuff,DWORD dwSize, UCHAR* pucCheckSum);
    // Get the computed checksum of file whose name is passed as parameter
    // Also the computed checksum and stored checksum is returned.
    BOOL bGetCheckSum(CString &omStrConfigFileName,
    UCHAR* pucCheckSum, UCHAR* pucCheckSumInFile);
    // Sets the checksum at last byte location after computing it.
    BOOL bSetCheckSum(CString& omStrConfigFileName,UCHAR* pucCheckSum);
    // method to search and open/create the configuration file. */


    INT     nIsCfgFileFound (CString& omStrFilename, BOOL bOpenExisting);
    // formats the last modified time into the format as specified
    // gets the system time

    // initialize the window coords to the default values
    void    vInitWndCoords();
    // To initialize splitter postions
    void vInitSplitterPostions();
    // initialize the baud rate structure with default values
    void    vInitBaudRateDetails();
    // initialize the message attribute structure with default values
    void    vInitMsgAttributes();
    // initialize the message filter structure with default values
    void    vInitFilterDetails();
    // initialize the toolbar info structure with default values
    void    vInitToolbarInfo();
    // method to desrialize/serialize data into the file/data members
    BOOL    nLoadStoreData(UINT unMode);
    // called from bLoadStoreData(..) for baud rate details
    void    vLoadStoreBaudRateDetails(CArchive& roCfgArchive);
    // called from bLoadStoreData(..) for window coords
    void    vLoadStoreWndCoords(CArchive& roCfgArchive);
    // To load/store splitter postion
    void    vLoadStoreSplitterPostion(CArchive& roCfgArchive);
    // called from bLoadStoreData(..) for message attributes
    BOOL    bLoadStoreMsgAttributes(CArchive& roCfgArchive);
    // called from bLoadStoreData(..) for multiple message info
    BOOL    bLoadStoreMultiMsgInfo(CArchive& roCfgArchive);
    // called from bLoadStoreData(..) for message filter details..
    BOOL    bLoadStoreMsgFilterDetails(CArchive& roCfgArchive);
    // called from bLoadStoreData(..) for message info
    BOOL    bLoadStoreMsgInfo(CArchive& roCfgArchive,
                              PSMSGBLOCKLIST psMsgBlockList);
    // gets the multiple message info ..
    BOOL    bGetMultiMsgInfo(PSMSGBLOCKLIST psDestMsgBlockList);
    // gets the message attribute info
    BOOL    bGetMsgAttrib(PSMESSAGE_ATTRIB pMsgAttrib);
    // gets the message filter details..
    BOOL    bGetMsgFilterDetails(PSMESSAGE_FILTER_DETAILS pMsgFilterDetails);

    // sets the multiple message structure data member with the info
    BOOL    bSetMultiMsgInfo(PSMSGBLOCKLIST psMsgBlockList);
    // sets the message attribute structure with the info..
    BOOL    bSetMsgAttrib(PSMESSAGE_ATTRIB pMsgAttrib);
    // sets the message filter structure with the info..
    BOOL    bSetMsgFilterDetails(PSMESSAGE_FILTER_DETAILS pMsgFilterDetails);
    // Load/Store Signal Watch List
    BOOL bLoadStoreSignalWatchList(CArchive& roCfgArchive);
    // releases the multiple message structure pointer that is passed..
    void    vReleaseMultiMsgInfo(PSMSGBLOCKLIST psMsgBlockList);
    // releases the message attribute structure that is passed..
    void    vReleaseMsgAttrib(PSMESSAGE_ATTRIB pData);
    // releases the message filter details structure pointer that is passed..
    void    vReleaseMsgFilterDetails(PSMESSAGE_FILTER_DETAILS pData);
    // releases memory allocated for the signal watch list
    void vReleaseSignalWatchListMemory();
    // To load/Store a window postion
    void vLoadStoreWindowInfo( CArchive& roCfgArchive,
                               WINDOWPLACEMENT& sPlacement );
    // To Load/Store Message Display Filter List
    BOOL bLoadStoreDisplayFilterInfo( CArchive& roCfgArchive );
    // To Load/Store Log File Infor
    BOOL bLoadStoreLogFileInfo(CArchive& omArchive);
    // To Load/Store Replay File Info
    BOOL bLoadStoreReplayFileInfo(CArchive& omArchive);


    // To Load/Store simulated system list File Info
    BOOL bLoadStoreSimSysList(CArchive& roCfgArchive);
    // releases memory allocated for the simulated system list
    void vReleaseSimSysListMemory();
public:
    INT COM_nSaveConfiguration(CString& omStrCfgFilename);


};
