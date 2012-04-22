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
 * \file      BUSMASTER.h
 * \brief     This header file for CCANMonitorApp class
 * \authors   Amitesh Bharti, Ratnadip Choudhury, Anish kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file for CCANMonitorApp class
 */

#pragma once

#include "resource.h"       // main symbols
#include "MsgSignal.h"      // Added by ClassView
#include "Flags.h"      // For CFlags
#include "Struct.h" // Added by ClassView
#include "ConfigDetails.h"  // Added by ClassView, reqd for CConfigDetails obj


/////////////////////////////////////////////////////////////////////////////
// CCANMonitorApp:
// See BUSMASTER.cpp for the implementation of this class
//

class CCANMonitorApp : public CWinApp
{
public:
    // Wrapper around CConfigDetails::vSetConfigurationModified()
    void vSetConfigurationModified(BOOL bModified = TRUE);
    // Wrapper around CConfigDetails::bIsConfigurationModified()
    BOOL bIsConfigurationModified();
    // wrapper around CConfigDetails::nSaveConfiguration(..)
    //INT nSaveConfiguration (CString omStrCfgFilename/*=defDEFAULTCFGFILE*/);
    // wrapper around CConfigDetails::nLoadConfiguration(..)
    //INT nLoadConfiguration(CString omStrFilename ,BOOL bFromCom);
    // wrapper around CConfigDetails::vRelease(..)
    void vRelease1(eCONFIGDETAILS eParam, LPVOID* lpDataPtr);
    // wrapper around CConfigDetails::bGetData(..)
    BOOL bGetData1(eCONFIGDETAILS  eParam, LPVOID* lpData);
    // wrapper around CConfigDetails::bSetData(..)
    BOOL bSetData1(eCONFIGDETAILS  eParam, LPVOID lpVoid);
    PSMSGBLOCKLIST psReturnMsgBlockPointer();
    // To get Default window size
    BOOL bGetDefaultValue( eCONFIGDETAILS eParam, WINDOWPLACEMENT& sPosition );
    // To get default splitter window position
    BOOL bGetDefaultSplitterPostion(eCONFIGDETAILS eParam, CRect omWindowSize,
                                    LPVOID* pData);

	//PSSIMSYSARRAY psReturnSimsysArrayPtr();

public:
	//CExecutefunc
    STHREADINFO m_asUtilThread[defEVENT_TOTAL-defOFFSET_TXMSG];
    CEvent m_aomState[defEVENT_TOTAL];
    VOID vDestroyUtilThreads(UINT unMaxWaitTime, BYTE byThreadCode);
    void GetLoadedConfigFilename(CString& roStrCfgFile);
    //INT nNewConfiguration(CString omStrFilename);
    CString omStrGetUnionFilePath(CString omStrTemp);
    CMsgSignal* m_pouMsgSignal;
    CMsgSignal* m_pouMsgSgInactive;
    CHAR m_acApplicationDirectory[MAX_PATH];// Specifies the application directory


    CCANMonitorApp();
    void vSetHelpID(DWORD dwHelpID);
    void vPopulateCANIDList();
    CFlags* pouGetFlagsPtr();
    //CString m_omStrUnionPath;
    BOOL m_bFromAutomation;

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCANMonitorApp)
    public:
    virtual BOOL InitInstance();
    virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
    virtual int ExitInstance();
    virtual CWnd* GetMainWnd();
    //}}AFX_VIRTUAL

// Implementation
    //{{AFX_MSG(CCANMonitorApp)
    afx_msg void OnAppAbout();
    afx_msg void OnFileOpen();
    afx_msg void OnFileNew();
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
	BOOL bInitialiseConfiguration(BOOL bFromCom);
    void vSetFileStorageInfo(CString oCfgFilename);
    void vDisplayConfigErrMsgbox(UINT unErrorCode, BOOL bOperation);
    
    CString m_ostrConfigFilename;
    CString m_omConfigErr;
    BOOL m_bIsConfigFileLoaded;
    DWORD m_dwHelpID;
    CFlags* m_pouFlags;
    BOOL m_bIsMRU_CreatedInOpen;

    sTOOLBAR_BUTTON_STATUS m_sToolbarInfo;
    CString m_omMRU_C_FileName;
    WINDOWPLACEMENT m_sNotificWndPlacement;

protected:
    //CConfigDetails m_oConfigDetails;
public:
    INT COM_nSaveConfiguration(CString omStrCfgFilename);
    BOOL bWriteIntoTraceWnd(char* omText);
};
