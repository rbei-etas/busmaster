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
 * \file      SimSysDetView.cpp
 * \brief     This file contain definition of all function of 
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of 
 */
#include "NodeSimEx_stdafx.h"	// Contains standard include header files.
#include "SimSysDetView.h"		// Class defintion included here
#include "SimSysManager.h"
#include "ExecuteManager.h"
#include "FunctionEditorDoc.h"
#include "GlobalObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
TCHAR* pcHandlerNames[] =                   {_T("Message Handlers"),
                                             _T("Timer Handlers"),
                                             _T("Key Handlers"),
                                             _T("Error Handlers")};

#define defStrEnabled                    _T("Enabled")
#define defStrDisabled                   _T("Disabled")

/////////////////////////////////////////////////////////////////////////////
// CSimSysDetView

IMPLEMENT_DYNCREATE(CSimSysDetView, CFormView)
/******************************************************************************/
/*  Function Name    :  CSimSysDetView                                        */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor
/*  Member of        :  CSimSysDetView                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  20.12.2005                                            */
/*  Modifications    :  
/******************************************************************************/
CSimSysDetView::CSimSysDetView(): CFormView(CSimSysDetView::IDD)
{
    m_eBus = CSimSysDetView::sm_eBus;
	m_psNodeInfo = NULL;
    m_bIsDllLoadSuccess = FALSE;
	bConnected = FALSE;
    m_omSourceCodeTextList.RemoveAll();

	m_bIsTimerHanEnabled = FALSE;
	m_bIsKeyHanEnabled = FALSE;
	m_bIsMsgHanEnabled = FALSE;
	m_bIsErrorHanEnabled = FALSE;
	
    
	//{{AFX_DATA_INIT(CSimSysDetView)
	m_omStrNodeName = _T("");
	m_omStrCFile = _T("");
	//}}AFX_DATA_INIT
}
/******************************************************************************/
/*  Function Name    :  ~CSimSysDetView                                       */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Destructor
/*  Member of        :  CSimSysDetView                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  20.12.2005                                            */
/*  Modifications    :  
/******************************************************************************/
CSimSysDetView::~CSimSysDetView()
{
}

void CSimSysDetView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimSysDetView)
	DDX_Control(pDX, IDC_LSTC_HANDLER_DETAILS, m_omListCtrlHanDet);
	DDX_Control(pDX, IDC_LSTC_HANVAL, m_omListCtrlHanVal);
    DDX_Control(pDX, IDC_EDIT_PREF_ADRES, m_omPrefAdres);
    DDX_Control(pDX, IDC_EDIT_ECUNAME, m_omECU_NAME);
	DDX_Text(pDX, IDC_EDIT_NODE_NAME, m_omStrNodeName);
	DDV_MaxChars(pDX, m_omStrNodeName, 15);
	DDX_Text(pDX, IDC_EDIT_FILE_NAME, m_omStrCFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSimSysDetView, CFormView)
//{{AFX_MSG_MAP(CSimSysDetView)
ON_BN_CLICKED(IDC_BUTTON_BUILD, OnButtonBuild)
ON_BN_CLICKED(IDC_BUTTON_BUILDANDLOAD, OnButtonBuildandload)
ON_BN_CLICKED(IDC_BUTTON_EDITFILE, OnButtonEditfile)
ON_BN_CLICKED(IDC_BUTTON_ENABLEALLHANDLERS, OnButtonEnableDisableallhandlers)
ON_BN_CLICKED(IDC_BUTTON_ENABLEHANDLER, OnButtonEnableDisablehandler)
ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoadUnload)
ON_BN_CLICKED(IDC_BUTTON_OPENFILE, OnButtonOpenfile)
ON_NOTIFY(NM_CLICK, IDC_LSTC_HANDLER_DETAILS, OnClickLstcHandlerDetails)
ON_NOTIFY(HDN_ITEMCHANGED, IDC_LSTC_HANDLER_DETAILS, OnItemchangedLstcHandlerDetails)

//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimSysDetView diagnostics

#ifdef _DEBUG
void CSimSysDetView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSimSysDetView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSimSysDetView message handlers
/******************************************************************************/
/*  Function Name    :  OnButtonBuild
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Calls  function, Enables the user to 
                        build the C file on button press
/*  Member of        :  CSimSysDetView                                         
/*  Friend of        :      -                                                 
/*                                                                            
/*  Author(s)        :  Anish Kumar                                   
/*  Date Created     :  
/*****************************************************************************/
void CSimSysDetView::OnButtonBuild() 
{
    BOOL bSuccess = FALSE;
    CExecuteManager::ouGetExecuteManager(m_eBus).vClearOutputWnd();
    if( m_psNodeInfo != NULL )
    {
        //If file is opened get its doc pointer and save it before building
        CFunctionEditorDoc* pDoc = CGlobalObj::ouGetObj(m_eBus).pGetDocPtrOfFile(m_psNodeInfo->m_omStrFileName);  
        if (pDoc)
		{
            pDoc->OnSaveDocument(m_psNodeInfo->m_omStrFileName);
		}
        //Build the file
        bSuccess = CExecuteManager::ouGetExecuteManager(m_eBus).bExecuteDllBuild(m_psNodeInfo);
        
        if(bSuccess)
        {
            vUpdateNodeDetailsAndView();
            //Enable the load button
            if( m_psNodeInfo->m_omStrDllName != STR_EMPTY )
            {
                GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(TRUE);
            }
        }
        else  // if the build is unsuccessfull
        {
            AfxMessageBox(_T("The file is not properly built"));
        }
    }
	
}

void CSimSysDetView::vUpdateNodeDetailsAndView()
{
    CString omStrFileName = m_psNodeInfo->m_omStrFileName;
    omStrFileName.Replace( defDOT_SMALL_C , defDOT_DLL );
    m_psNodeInfo->m_omStrDllName = omStrFileName;

    // to indicate to the tree view about the new dlls built.
    CSimSysTreeView* psSimSysTree = 
        CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysTreeView();
    psSimSysTree->vSetSimSysDllName(m_psNodeInfo->m_omStrNodeName , 
                                    omStrFileName , 
                                    m_psNodeInfo->m_bIsDllLoaded);
}

/******************************************************************************/
/*  Function Name    :  OnButtonBuildandload
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Calls  function, Enables the user to 
                        build and load the C file on button press
/*  Member of        :  CSimSysDetView                                         
/*  Friend of        :      -                                                 
/*                                                                            
/*  Author(s)        :  Anish                                   
/*  Date Created     :  
/*****************************************************************************/
void CSimSysDetView::OnButtonBuildandload() 
{
	BOOL bSuccess = FALSE;
    CExecuteManager::ouGetExecuteManager(m_eBus).vClearOutputWnd();
    if(( m_psNodeInfo != NULL) && (m_psNodeInfo->m_omStrFileName != STR_EMPTY ))
                               
    {
        //If file is opened get its doc pointer and save it before building
        CFunctionEditorDoc* pDoc = CGlobalObj::ouGetObj(m_eBus).pGetDocPtrOfFile(m_psNodeInfo->m_omStrFileName);  
        if (pDoc)
		{
            pDoc->OnSaveDocument(m_psNodeInfo->m_omStrFileName);
		}
		bSuccess = CExecuteManager::ouGetExecuteManager(m_eBus).
                                    bExecuteDllBuildLoad(m_psNodeInfo);

		if(bSuccess)// when build and load is successfull
		{
            vUpdateNodeDetailsAndView();           
            m_psNodeInfo->m_bIsDllLoaded = TRUE;
           	vChangeLUButtonText(TRUE);
            GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(TRUE);
            vEnableHandlerDetails(TRUE);
            vDisplayNodeInformation(m_psNodeInfo);
		}
        else   
        {
			AfxMessageBox(_T("The file is either not properly built or Loaded"));
        }
    }
	
}

/******************************************************************************/
/*  Function Name    :  OnButtonLoadUnload
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Calls  function, Enables the user to 
						load/unload the dll on button press
/*  Member of        :  CSimSysDetView                                         
/*  Friend of        :      -                                                 
/*                                                                            
/*  Author(s)        :  Harika M                                      
/*  Date Created     :  21.12.2005
/*  Modification By  :  Anish kr                                     
/*  Date		     :  18.10.2006,to solve bug when multiple files are opened 
/*						and this Fn is called
/*  Modification By  :  Anish kr                                     
/*  Date		     :  31.10.2006,Remove code for changing Dll name(File's name.dll)
/*                      in node's detail struct on simply loading
/*  Modification By  :  Anish kr                                     
/*  Date		     :  21.12.2006,Added code to reset the handler's button status 
/*                      on loading of any dll
/*****************************************************************************/
void CSimSysDetView::OnButtonLoadUnload() 
{
	int nReturnVal = IDNO;
	CString omStrButtonText = STR_EMPTY;
    CString omStrFileName = STR_EMPTY;
    //Clear oytput window
    CExecuteManager::ouGetExecuteManager(m_eBus).vClearOutputWnd();
    //Get the button text to check whether to load or unload
	GetDlgItem(IDC_BUTTON_LOAD)->GetWindowText(omStrButtonText);

    m_omListCtrlHanVal.DeleteAllItems();
	if(omStrButtonText == "Load")  // when user wants to load
	{
        //If file is opened get its doc pointer 
        CFunctionEditorDoc* pDoc = CGlobalObj::ouGetObj(m_eBus).pGetDocPtrOfFile(m_psNodeInfo->m_omStrFileName);
		if ( pDoc != NULL)
        {
		    if(pDoc->IsModified())
		    {			
			    nReturnVal =AfxMessageBox(_T("The file has been opened for editing.\n")
				                          _T("Would you like to build them? "), 
				                          MB_YESNOCANCEL|MB_ICONQUESTION);
		    }
        }
		// Load without building if either the file is not modified or 
        // thg user doesn't want to build
        if( nReturnVal == IDNO ) 
        {
            BOOL bSuccess = FALSE;
            if(( m_psNodeInfo != NULL) && (m_psNodeInfo->m_omStrDllName != STR_EMPTY ) )
            {
                bSuccess = CExecuteManager::ouGetExecuteManager(m_eBus).
                                            bExecuteDllLoad( m_psNodeInfo );
                
                if(bSuccess)
                {
                    // to indicate to the tree view about the new dlls built.
                    CSimSysTreeView* psSimSysTree = 
                        CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysTreeView();
                    psSimSysTree->vSetSimSysDllName(m_psNodeInfo->m_omStrNodeName , 
                                                    m_psNodeInfo->m_omStrDllName , TRUE);
                    
                    //Save the loading info
                    m_psNodeInfo->m_bIsDllLoaded = TRUE;
                    //Change the text of load button
                    vChangeLUButtonText(TRUE);
                    GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(TRUE);
                    vEnableHandlerDetails(TRUE);                
                    
                    CSimSysManager::ouGetSimSysManager(m_eBus).ouGetFlags().vSetFlagStatus(H_ALL_HANDLER, FALSE);
                    CSimSysManager::ouGetSimSysManager(m_eBus).ouGetFlags().vSetFlagStatus(H_ERROR_HANDLER, FALSE);
                    CSimSysManager::ouGetSimSysManager(m_eBus).ouGetFlags().vSetFlagStatus(H_KEY_HANDLER_ON, FALSE);
                    CSimSysManager::ouGetSimSysManager(m_eBus).ouGetFlags().vSetFlagStatus(H_MSGHANDLERBUTTON, FALSE);
                    // to make the status of the handlers disabled
                    m_psNodeInfo->m_bDllHandlersEnabled = FALSE;
                    m_psNodeInfo->m_bErrorHandlersEnabled = FALSE;
                    m_psNodeInfo->m_bIsAllHandlersEnabled = FALSE;
                    m_psNodeInfo->m_bKeyHandlersEnabled = FALSE;
                    m_psNodeInfo->m_bMsgHandlersEnabled = FALSE;
                    m_psNodeInfo->m_bTimerHandlersEnabled = FALSE;
                    //Display the node details
                    vDisplayNodeInformation(m_psNodeInfo);  
                }
                else  
                { 
                    AfxMessageBox(_T("The file is not properly Loaded"));
                }
            }
        }
        
        else if( nReturnVal == IDYES )  // Build and load
        {
            BOOL bSuccess = FALSE;
            if(( m_psNodeInfo != NULL) && (m_psNodeInfo->m_omStrFileName != STR_EMPTY ) )
            {
                if (pDoc)
                {
                    //If file is opened then save it
                    pDoc->OnSaveDocument(m_psNodeInfo->m_omStrFileName);
                }
				bSuccess = CExecuteManager::ouGetExecuteManager(m_eBus).bExecuteDllBuildLoad(
					                                              m_psNodeInfo);

				if(!bSuccess)
				{
					AfxMessageBox(_T("The file is either not properly built or Loaded"));
				}
				else  
				{
					vUpdateNodeDetailsAndView();

					m_psNodeInfo->m_bIsDllLoaded = TRUE;
					vChangeLUButtonText(TRUE);
					GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(TRUE);
					vEnableHandlerDetails(TRUE);
					vDisplayNodeInformation(m_psNodeInfo);                     
				}
			}
		}

	}
	else if(omStrButtonText == "UnLoad")  // when user wants to unload
	{
		BOOL bSuccess = FALSE;
		if(( m_psNodeInfo != NULL) && (m_psNodeInfo->m_omStrDllName != STR_EMPTY ) )
		{
			bSuccess = CExecuteManager::ouGetExecuteManager(m_eBus).
                                        bExecuteDllUnload(m_psNodeInfo);

			if(!bSuccess)
			{
				AfxMessageBox(_T("The file is not properly unloaded"));
			}
			else  
			{
               
                // to indicate to the tree view 
                CSimSysTreeView* psSimSysTree = 
                    CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysTreeView();
                psSimSysTree->vSetSimSysDllName(m_psNodeInfo->m_omStrNodeName , 
                                                m_psNodeInfo->m_omStrDllName , FALSE);
                
                // update the member m_bIsDllLoaded of the struct sNodeInfo 
                m_psNodeInfo->m_bIsDllLoaded = FALSE;
                vChangeLUButtonText(FALSE);
                
                GetDlgItem(IDC_BUTTON_BUILDANDLOAD)->EnableWindow(TRUE);
                GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(TRUE);
                vEnableHandlerDetails(FALSE);

                // to make the status of the handlers disabled
                m_psNodeInfo->m_bDllHandlersEnabled = FALSE;
                m_psNodeInfo->m_bErrorHandlersEnabled = FALSE;
                m_psNodeInfo->m_bIsAllHandlersEnabled = FALSE;
                m_psNodeInfo->m_bKeyHandlersEnabled = FALSE;
                m_psNodeInfo->m_bMsgHandlersEnabled = FALSE;
                m_psNodeInfo->m_bTimerHandlersEnabled = FALSE;
                 
                vDisplayNodeInformation(m_psNodeInfo);            
                
            }
        }        
    }
}
/******************************************************************************/
/*  Function Name    :  OnButtonEditfile
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Calls  function, Enables the user to 
                        edit the C file on button press
/*  Member of        :  CSimSysDetView                                         
/*  Friend of        :      -                                                 
/*                                                                            
/*  Author(s)        :                                      
/*  Date Created     :  
/*****************************************************************************/
void CSimSysDetView::OnButtonEditfile() 
{	
    CFunctionEditorDoc* pDoc = CGlobalObj::ouGetObj(m_eBus).pGetDocPtrOfFile(m_omStrCFile);
    if (pDoc != NULL)
    {
        //If file is opened then get its frame and activate it
        {
            POSITION pos = pDoc->GetFirstViewPosition();
            if (pos)
            {
                pDoc->GetNextView(pos)->GetParentFrame()->ActivateFrame();
            }
        }
    }
    else
    {
        //If file is not opened then open it
        if ( !CGlobalObj::ouGetObj(m_eBus).bOpenFunctioneditorfile(m_omStrCFile) )
        {
            AfxMessageBox(_T("Specified filename not found!"), 
                MB_OK|MB_ICONINFORMATION);
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnButtonEnableDisableallhandlers
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Calls  function, Enables the user to 
						enable/disable all the handlers on button press
/*  Member of        :  CSimSysDetView                                         
/*  Friend of        :      -                                                 
/*                                                                            
/*  Author(s)        :                                      
/*  Date Created     :  
/*****************************************************************************/
void CSimSysDetView::OnButtonEnableDisableallhandlers() 
{
    CSimSysNodeInfo* pSimSysNodeInf = 
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    if (pSimSysNodeInf)
    {
	    CString omStrButtonText = STR_EMPTY;
	    CButton* psButton = (CButton*)GetDlgItem(IDC_BUTTON_ENABLEALLHANDLERS);
	    psButton->GetWindowText(omStrButtonText);

        BOOL bEnableAll = !omStrButtonText.CompareNoCase(_T("Enable All Handlers"));
	    pSimSysNodeInf->vSetEnableNodeAllHandlers(STR_EMPTY , 
                                                m_psNodeInfo->m_omStrNodeName , 
                                                bEnableAll);
	    vChangeEDAllHanButtonText(bEnableAll);
	    vDisplayNodeInformation(m_psNodeInfo);

		// Set the flag status about the enable / disable handler
		vUpdateHandlerEnableDisableStatus(-1, bEnableAll);
    }
}
/******************************************************************************/
/*  Function Name    :  OnButtonEnableDisablehandler
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Calls  function, Enables the user to 
						enable/disable the selected handler on button press
/*  Member of        :  CSimSysDetView                                         
/*  Friend of        :      -                                                 
/*                                                                            
/*  Author(s)        :                                     
/*  Date Created     :  
/*****************************************************************************/
void CSimSysDetView::OnButtonEnableDisablehandler() 
{
	POSITION pos = m_omListCtrlHanDet.GetFirstSelectedItemPosition();
    if (pos)
    {
	    int nItem = m_omListCtrlHanDet.GetNextSelectedItem(pos);
	    // pass it to the vChangeEDHanButtonText..   	
	    CString omStrButtonText = STR_EMPTY;
	    GetDlgItem(IDC_BUTTON_ENABLEHANDLER)->GetWindowText(omStrButtonText);
        BOOL bEnableHandler = !omStrButtonText.CompareNoCase(_T("Enable Handler"));
        //If button is for enabling then enable the handler and vice versa
        vSetNodeInfoEDHanStatus(nItem , bEnableHandler );
        vDisplayNodeInformation(m_psNodeInfo);

		// Set the flag status about the enable / disable handler
		vUpdateHandlerEnableDisableStatus(nItem, bEnableHandler);
    }
}
/******************************************************************************/
/*  Function Name    :  OnButtonOpenfile
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Calls  function, Enables the user to (associate)
						open an existing C file or create a new file on button press
/*  Member of        :  CSimSysDetView                                         
/*  Friend of        :      -                                                 
/*                                                                            
/*  Author(s)        :  Harika M                                      
/*  Date Created     :  21.12.2005
/*****************************************************************************/
void CSimSysDetView::OnButtonOpenfile() 
{
    int nReturnVal = IDYES;
    if( m_psNodeInfo->m_bIsDllLoaded)
    {
        AfxMessageBox(_T("Unload the dll and try again!!"));
    }
    else
    {
        if(m_psNodeInfo->m_omStrFileName != STR_EMPTY)
        {
            nReturnVal = AfxMessageBox(ALL_NODE_EDIT_CONFMN, MB_YESNO|MB_ICONQUESTION);
        }
        if ( nReturnVal == IDYES )
        {
            CString omStrDefCFileName(STR_EMPTY);
            // Display open dialog box with *.c filter
            // and select the C file by default
            CFileDialog fileDlg( TRUE,      // Open File dialog
                                _T("c"),        // Default Extension,
                                omStrDefCFileName,                        
                                OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                                _T("C File(s)(*.c)|*.c||"),
                                NULL );
            
            // Set Title
            fileDlg.m_ofn.lpstrTitle  = _T("Select FRAME Source Filename...");
            
            if ( IDOK == fileDlg.DoModal() )
            {
                CString strExtName  = fileDlg.GetFileExt();
                CString omStrNewCFileName = fileDlg.GetPathName();
                CString omStrTitle = STR_EMPTY;
                omStrTitle = fileDlg.GetFileTitle();
            
/*
                // file-attribute information
                _findfirst fileinfo;
                // Check if file exists
                if (_findfirst( omStrNewCFileName, &fileinfo) != -1L)
                {*/
                    // update the struct info with new file name and check if the 
                    // corresponding dll is present in the same directory
                    // if present update the dll name else make it STR_EMPTY.
                    BOOL bIsSuccess = bUpdateNodeInfoFile(omStrNewCFileName);
                    if( bIsSuccess )
                    {
                        CGlobalObj::ouGetObj(m_eBus).bOpenFunctioneditorfile(omStrNewCFileName);
                        m_omStrCFile =  omStrNewCFileName;   
                        UpdateData(FALSE);
/*                        
                        GetDlgItem(IDC_BUTTON_EDITFILE)->EnableWindow(TRUE);
                        GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(TRUE);
                        GetDlgItem(IDC_BUTTON_BUILDANDLOAD)->EnableWindow(TRUE);
                        if(m_psNodeInfo->m_omStrDllName != STR_EMPTY)
                            GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(TRUE);
                        else
                            GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(FALSE);
                    }
                    
                    
                }
                else  // new doc should be created if the file name is not found.
                { 
                    BOOL bIsSuccess = bUpdateNodeInfoFile(omStrNewCFileName);
                    if( bIsSuccess )
                    {
                        BOOL bSuccess = bCreateNewFile(omStrNewCFileName );
                        if (bSuccess)
                        {
                            CGlobalObj::bOpenFunctioneditorfile(omStrNewCFileName);
                            CFunctionEditorDoc *pDoc = CGlobalObj::pGetDocPtrOfFile(omStrNewCFileName);
                            if (pDoc)
                            {
                                pDoc->SetModifiedFlag();
                            }
                            m_omStrCFile =  omStrNewCFileName;   
                            UpdateData(FALSE);*/
                            GetDlgItem(IDC_BUTTON_EDITFILE)->EnableWindow(TRUE);
                            GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(TRUE);
                            GetDlgItem(IDC_BUTTON_BUILDANDLOAD)->EnableWindow(TRUE);
                            if(m_psNodeInfo->m_omStrDllName != STR_EMPTY)
                            {
                                GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(TRUE);
                            }
                            else
                            {
                                GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(FALSE);
                            }
                        }
                    }  
                }
            }
        }
    //}   
//}
/******************************************************************************/
/*  Function Name    :  bCreateNewFile                                        */
/*                                                                            */
/*  Input(s)         :   CString omStrFileName                                */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  Constructs the new document and updates the source 
                        list. 
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  21.02.2006                                            */
/*  Modifications    :  Anish kumar,31.08.06
						bug fixed,XXX_Unions.h was not included as header
/******************************************************************************/
//BOOL CSimSysDetView::bCreateNewFile( CString omStrFileName )
//{
//    BOOL bSuccess = FALSE;
//    FILE * pCFile = _tfopen( omStrFileName, _T("at"));
//    CString omStrCFileName = STR_EMPTY;
//    CString omStr = STR_EMPTY;
//    if( pCFile != NULL )
//    {
//        bSuccess = TRUE;
//       
//         TCHAR buffer[2500] = BUS_INCLUDE_HDR;
//         wcscat(buffer ,_T("\n#include <Windows.h>"));
//         if (CGlobalObj::m_omMsgStructFile.IsEmpty())
//         {
//             ASSERT(FALSE);
//         }
//         CString omStr;
//         omStr.Format(STR_INCLUDE_FILE, CGlobalObj::m_omMsgStructFile);
//		 wcscat(buffer ,omStr);
//		 // Get file path of unions.h from app class
//		 // and if valid insert into the header namespace
//        //COMMENTED BY AK****************
//		/*CStringArray aomstrDBFiles;
//		theApp.m_pouMsgSignal->vGetDataBaseNames(&aomstrDBFiles);
//		int nTotalCount = aomstrDBFiles.GetCount();
//		for(int nCount = 0 ; nCount < nTotalCount ; nCount++)
//		{
//			CString omStrTemp = aomstrDBFiles.GetAt(nCount);
//			omStr = CSimSysManager::ouGetSimSysManager().omStrGetUnionFilePath(omStrTemp);
//			if ( !omStr.IsEmpty())
//			{
//				omStr.Insert( 0,"#include \"");
//				omStr += "\"";
//			}
//			wcscat(buffer ,omStr);
//			wcscat(buffer ,"\n");
//		}*/
//		 wcscat(buffer ,_T("\n/* End FRAME include header */"));
//         wcscat(buffer ,_T("\n\n\n"));
//         wcscat(buffer ,BUS_VAR_HDR);
//         wcscat(buffer ,_T("\n\n"));
//         wcscat(buffer ,BUS_VAR_FOOTER);
//         wcscat(buffer ,_T("\n\n\n"));
//         wcscat(buffer ,BUS_FN_PROTOTYPE_HDR);
//         wcscat(buffer ,_T("\n"));
//         wcscat(buffer ,BUS_FN_PROTOTYPE_FOOTER);
//         wcscat(buffer ,_T("\n\n"));
//         int tcsLength = _tcslen(buffer); 
//      
//       int numwritten = fwrite( buffer, sizeof( TCHAR ), tcsLength, pCFile );
//
//       fclose(pCFile);
//        pCFile = NULL;
//    }
//    else
//        AfxMessageBox(_T("Error in creating c file"));
//
//    return bSuccess;
//}

/******************************************************************************/
/*  Function Name    :  bUpdateNodeInfoFile
/*  Input(s)         :  CString omStrCFile
/*  Output           :  -
/*  Functionality    :  Updates the Node details with the new C file name selected 
                        by "Open File" and calls a function to update the dll name 
                        if present and also intimates the tree view to update its 
                        details.
/*  Member of        :  CSimSysDetView                                         
/*  Friend of        :      -                                                 
/*                                                                            
/*  Author(s)        :  Harika M                                      
/*  Date Created     :  21.12.2005
/*****************************************************************************/
BOOL CSimSysDetView::bUpdateNodeInfoFile(CString omStrCFile)
{
    INT nIsFileFound = 0;
    BOOL bIsSuccess = FALSE;
    BOOL bDupDllFound = FALSE;
    BOOL bDupCfileFound = FALSE;
    CSimSysTreeView* pSimSysTreeView = 
        CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysTreeView();
    CSimSysNodeInfo *pSimSysNodeInf = 
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    
    CSimSysConfigDetails* pSimSysConfig = 
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysConfig();
    if (pSimSysTreeView == NULL || pSimSysNodeInf == NULL || pSimSysConfig == NULL)
    {
        return false;
    }
    //Check if the .c file is already present
    bDupCfileFound = pSimSysNodeInf->bIsDuplicateCFile( omStrCFile );
    if (bDupCfileFound)
    {
        AfxMessageBox(_T("Duplicate C file found!"));
    }
    else
    {
        //Check if the corressponding dll is already present
        CString omStrDllName = omStrCFile;
        omStrDllName.Replace( defDOT_SMALL_C , defDOT_DLL );
        bDupDllFound = pSimSysNodeInf->bIsDuplicateDllName( omStrDllName );
        if(bDupDllFound)
        {
            AfxMessageBox(_T("Duplicate dll found!"));
        }
    }
    //If there is no duplication then update the node info
    if ((!bDupCfileFound) && (!bDupDllFound))
    {
        m_psNodeInfo->m_omStrFileName = omStrCFile;
        //Get the corressponding dll name and check if it is already
        //present in the folder
        omStrCFile.Replace( defDOT_SMALL_C , defDOT_DLL );
         // file-attribute information
        _tfinddata_t fileinfo;
        // Check if file exists
        if (_tfindfirst( omStrCFile.GetBuffer(MAX_PATH), &fileinfo) != -1L)
        {
            //In order to find where to update in treeview
            CString omStrPrevDllName = m_psNodeInfo->m_omStrDllName;
            //If present then update the dll name also
            m_psNodeInfo->m_omStrDllName = omStrCFile;
            // to indicate the change in the tree view
            pSimSysTreeView->vSetDllName( omStrPrevDllName ,
                                          m_psNodeInfo->m_omStrDllName );
        }
        //Set the modification flag as file is changed
        PSSIMSYSINFO psSimSys = pSimSysTreeView->psGetCurrentSimSysInfo();
        if (psSimSys != NULL)
        {
            psSimSys->m_bIsSimSysModified = TRUE;
        }
        bIsSuccess = TRUE;
    } 
    return bIsSuccess;
}


/******************************************************************************/
/*  Function Name    :  OnClickLstcHandlerDetails                             */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  displays handler description and value
						for the selected handler from handler list control

/*  Member of        :  CSimSysDetView                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M											  */
/*  Date Created     :  20.12.2005                                            */
/*. Modifications    :  
/******************************************************************************/
void CSimSysDetView::OnClickLstcHandlerDetails(NMHDR* pNMHDR, LRESULT* /*pResult*/) 
{
	UINT unItemStateMask = LVNI_SELECTED|LVNI_FOCUSED;
    int nItemCount = m_omListCtrlHanDet.GetItemCount();
    int nSel = m_omListCtrlHanDet.GetNextItem(-1, LVNI_SELECTED  );
    if(nSel == -1)
    {
        m_omListCtrlHanDet.SetItemState(nItemCount - 1,
                                        unItemStateMask ,
                                        unItemStateMask ) ;
    }
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    vUpDownArrowKeySelection(pNMListView->iItem);    
}


/******************************************************************************/
/*  Function Name    :  OnInitialUpdate                                       */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the frame work to update the view. 
						Inserts columns to the list control

/*  Member of        :  CSimSysDetView                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M			                                  */
/*  Date Created     :  20.12.2005                                            */
/*  Modifications    :  
/******************************************************************************/

void CSimSysDetView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

    CSimSysManager::ouGetSimSysManager(m_eBus).podSetSimSysDetView( this );
    
    // Create a font for the main header
    CFont om_tFont;
    LOGFONT LF;
    memset(&LF, 0, sizeof(LF));
	
    // Setting the log font structure values to set the font
    LF.lfHeight = 10;
    LF.lfWeight = FW_HEAVY;
    LF.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
    _tcscpy(LF.lfFaceName, _T("Ms Sans Serif"));
    if (!om_tFont.CreateFontIndirect(&LF))
    {
        TRACE(_T("MMI: CMainFrame::OnCreate() Could not create font for Combo Box\n"));
    }
    
    // Insert column header in the signal details list control
    RECT rRect;
    GetDlgItem(IDC_LSTC_HANDLER_DETAILS)->GetWindowRect( &rRect );
	
    ScreenToClient(&rRect);
	
    CSize om_Coor(0,0);
    
    om_Coor.cx = rRect.right/3;
	
    // Insert column to handler details list control
    m_omListCtrlHanDet.InsertColumn( 0,
		_T("Name"),
		LVCFMT_CENTER, 
		om_Coor.cx, 
		0);
    
    m_omListCtrlHanDet.InsertColumn( 1,
		_T("Handler Count"),
		LVCFMT_CENTER, 
		om_Coor.cx, 
		1);
	
    
    m_omListCtrlHanDet.InsertColumn( 2,
		_T("Status"),
		LVCFMT_CENTER, 
		om_Coor.cx, 
		2);

    // dll is unloaded then insert these values
    if(!m_bIsDllLoadSuccess)
    {
        UpdateData(TRUE);
        for(int i=0; i<4; i++)
        {
            m_omListCtrlHanDet.InsertItem(i,pcHandlerNames[i]);
            m_omListCtrlHanDet.SetItemText(i,1,_T("-"));
            m_omListCtrlHanDet.SetItemText(i,2,defStrDisabled);            
        }
    }
    
    
    // if dll is loaded then insert the values along with the no of 
    // handlers present n the status
    if(m_bIsDllLoadSuccess)
    {
        UpdateData(TRUE);
        vEnableHandlerDetails(TRUE);
        LVITEM sItem;
        sItem.mask      = LVIF_STATE;
        sItem.iItem     = 0;
        sItem.state     = LVIS_FOCUSED|LVIS_SELECTED;
        sItem.stateMask = LVIS_SELECTED |LVIS_FOCUSED;
        sItem.iSubItem  = 0;
        m_omListCtrlHanDet.SetItem(&sItem );
        if (m_psNodeInfo->m_bIsAllHandlersEnabled)
            vChangeEDAllHanButtonText(TRUE);
        
    }
    
    
    // Insert columns to the handler value list control
    m_omListCtrlHanVal.GetWindowRect( &rRect );
    
    ScreenToClient(&rRect);
    
    m_omListCtrlHanVal.InsertColumn( 0,
        _T("Handler Description"),
        LVCFMT_CENTER, 
        rRect.right/2, 
        0);
    
    
    // let list control select full row
    m_omListCtrlHanDet.SetExtendedStyle( LVS_EX_FULLROWSELECT );
    m_omListCtrlHanVal.SetExtendedStyle( LVS_EX_FULLROWSELECT );
    
    //
    m_omECU_NAME.vSetSigned(FALSE);
    m_omPrefAdres.vSetSigned(FALSE);
    //
    vHideControls(SW_HIDE);	
}

/******************************************************************************/
/*  Function Name    :  vDisplayNodeInformation                              
/*                                                                            
/*  Input(s)         :  CSimSysDetView::sMESSAGE* pMsg                            
/*  Output           :                                                        
/*  Functionality    :  Displays the message details on the form view

  /*  Member of        :  CSimSysDetView                                         
  /*  Friend of        :      -                                                 
  /*                                                                            
  /*  Author(s)        :  Harika M                                      
  /*  Date Created     :  20.12.2005                                            
  /*  Modifications    :  
/******************************************************************************/

void CSimSysDetView::vDisplayNodeInformation(PSNODEINFO pNodeInfo)

{
    if (pNodeInfo != NULL)
    {
        m_psNodeInfo = pNodeInfo;
        // Get node name
        m_omStrNodeName = pNodeInfo->m_omStrNodeName;
        m_omPrefAdres.vSetValue(pNodeInfo->m_byPrefAddress);
        m_omECU_NAME.vSetValue(pNodeInfo->m_unEcuName);
        // Get C file path
        m_omStrCFile = pNodeInfo->m_omStrFileName;
        BOOL bEnableButton = !pNodeInfo->m_omStrFileName.IsEmpty();
        vEnableButtons(bEnableButton);
        // Enable/disable the filegrp static frame depending upon presence of file 
        GetDlgItem(IDC_STAT_FILEGRP)->EnableWindow(bEnableButton); 		
	    UpdateData(FALSE);
        //Enable load button if there is dll attached to the node
        BOOL bEnableDllButton = !pNodeInfo->m_omStrDllName.IsEmpty();
        GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(bEnableDllButton);
    
        if (pNodeInfo->m_bIsDllLoaded)
        {
            m_bIsDllLoadSuccess = TRUE;
            
            // enable the handler details n show the handler details.  
            vEnableHandlerDetails(TRUE);  
            // Get The Connection Status and update the controls
            bConnected = CSimSysManager::ouGetSimSysManager(m_eBus).
                            ouGetFlags().nGetFlagStatus(H_CONNECTED);
            // only when the tool is connected enable the button.
            if(bConnected)
            {
                GetDlgItem(IDC_BUTTON_ENABLEHANDLER)->EnableWindow(TRUE);
                GetDlgItem(IDC_BUTTON_ENABLEALLHANDLERS)->EnableWindow(TRUE);
            }
            else
            {
                GetDlgItem(IDC_BUTTON_ENABLEHANDLER)->EnableWindow(FALSE);
                GetDlgItem(IDC_BUTTON_ENABLEALLHANDLERS)->EnableWindow(FALSE);
            }
            // toggle text to "Unload".
            vChangeLUButtonText(TRUE);
            // call a func here only to populate all the handler function names.
            vAddItemToHanDetList( pNodeInfo );
            if (pNodeInfo->m_bIsAllHandlersEnabled)
            {
                vChangeEDAllHanButtonText(TRUE);
            }
                       
        }
        else
        {
            // toggle text to "Load".
            vChangeLUButtonText(FALSE);
            // call a func here only to populate all the handler function names.
            vAddItemToHanDetList( pNodeInfo );

			// disable the whole handler details frame.
            vEnableHandlerDetails(FALSE);
            			
        }
        if(pNodeInfo->m_bIsAllHandlersEnabled)
        {
            vChangeEDAllHanButtonText(TRUE);
        }
        else
        {
            vChangeEDAllHanButtonText(FALSE);
        }
    }
}

/******************************************************************************/
/*  Function Name    :  vEnableHandlerDetails                                 */
/*                                                                            */
/*  Input(s)         :  BOOL bIsEnabled                                       */
/*  Output           :                                                        */
/*  Functionality    :  Enables or disables the list controls
/*  Member of        :  CSimSysDetView                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M											  */
/*  Date Created     :  20.12.2005                                            */
/*  Modifications    :  
/******************************************************************************/

void CSimSysDetView::vEnableHandlerDetails(BOOL bIsEnabled)
{
    GetDlgItem(IDC_LSTC_HANDLER_DETAILS)->EnableWindow(bIsEnabled);
    GetDlgItem(IDC_LSTC_HANVAL)->EnableWindow(bIsEnabled);
    GetDlgItem(IDC_BUTTON_ENABLEHANDLER)->EnableWindow(bIsEnabled);
    GetDlgItem(IDC_BUTTON_ENABLEALLHANDLERS)->EnableWindow(bIsEnabled);
    
}
/******************************************************************************/
/*  Function Name    :  vEnableHandlerButtons                                 */
/*                                                                            */
/*  Input(s)         :  BOOL bIsEnabled                                       */
/*  Output           :                                                        */
/*  Functionality    :  Enables or disables the buttons
/*  Member of        :  CSimSysDetView                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M											  */
/*  Date Created     :  20.12.2005                                            */
/*  Modifications    :  Anish,13.02.07
						Added code to check if dll is loaded before enabling 
						the handler buttons
/******************************************************************************/
void CSimSysDetView::vEnableHandlerButtons( BOOL bIsEnabled )
{
	if( bIsEnabled == TRUE && m_psNodeInfo != NULL )
	{
		if(m_psNodeInfo->m_bIsDllLoaded == FALSE)
		{
			bIsEnabled = FALSE;
		}
	}
	GetDlgItem(IDC_BUTTON_ENABLEHANDLER)->EnableWindow(bIsEnabled);
	GetDlgItem(IDC_BUTTON_ENABLEALLHANDLERS)->EnableWindow(bIsEnabled);


}
/******************************************************************************/
/*  Function Name    :  vHideControls                                         */
/*                                                                            */
/*  Input(s)         :  UINT unEnableHide                                     */
/*  Output           :                                                        */
/*  Functionality    :  Hides or unhides the controls
/*  Member of        :  CSimSysDetView                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M											  */
/*  Date Created     :  20.12.2005                                            */
/*  Modifications    :  
/******************************************************************************/
void CSimSysDetView::vHideControls(UINT unEnableHide)
{
    GetDlgItem(IDC_STAT_NODE_DETAILS)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_STAT_FILEGRP)->ShowWindow(unEnableHide);

    
    GetDlgItem(IDC_STAT_NODE_NAME)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_EDIT_NODE_NAME)->ShowWindow(unEnableHide);
	
    BOOL bShowHide =  FALSE;
    {
        GetDlgItem(IDC_STATIC_PREF_ADRES)->ShowWindow(bShowHide);
        GetDlgItem(IDC_EDIT_PREF_ADRES)->ShowWindow(bShowHide);

        GetDlgItem(IDC_STATIC_ECU_NAME)->ShowWindow(bShowHide);
        GetDlgItem(IDC_EDIT_ECUNAME)->ShowWindow(bShowHide);
    }
    GetDlgItem(IDC_STAT_FILE_NAME)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_EDIT_FILE_NAME)->ShowWindow(unEnableHide);
    
    GetDlgItem(IDC_LSTC_HANDLER_DETAILS)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_LSTC_HANVAL)->ShowWindow(unEnableHide);
	
    GetDlgItem(IDC_STAT_HANGROUP)->ShowWindow(unEnableHide);
	
    GetDlgItem(IDC_STAT_HANDESC)->ShowWindow(unEnableHide);
  
    GetDlgItem(IDC_BUTTON_OPENFILE)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_BUTTON_EDITFILE)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_BUTTON_BUILD)->ShowWindow(unEnableHide);
	GetDlgItem(IDC_BUTTON_BUILDANDLOAD)->ShowWindow(unEnableHide);
	GetDlgItem(IDC_BUTTON_LOAD)->ShowWindow(unEnableHide);
	
	GetDlgItem(IDC_BUTTON_ENABLEHANDLER)->ShowWindow(unEnableHide);
	GetDlgItem(IDC_BUTTON_ENABLEALLHANDLERS)->ShowWindow(unEnableHide);

    
    if ( unEnableHide == SW_HIDE )
    {
        m_bAreControlsHidden = TRUE;
    }
    else
    {
        m_bAreControlsHidden = FALSE;
    }
}
/******************************************************************************/
/*  Function Name    :  vChangeLUButtonText                                   */
/*                                                                            */
/*  Input(s)         :  BOOL bIsLoaded                                        */
/*  Output           :                                                        */
/*  Functionality    :  Changes the Text of the button to "Load" or "UnLoad"
/*  Member of        :  CSimSysDetView                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  
/*  Date Created     :  
/*  Modifications    :  
/******************************************************************************/
void CSimSysDetView::vChangeLUButtonText(BOOL bIsLoaded)
{
	LPCTSTR StrUnloadTxt = _T("Load");
    if(bIsLoaded)
    {
        StrUnloadTxt = _T("UnLoad");
    }
    GetDlgItem(IDC_BUTTON_LOAD)->SetWindowText(StrUnloadTxt);
	
}
void CSimSysDetView::vSetNodeAddress(BYTE byAddress)
{
    if (m_psNodeInfo != NULL)
    {
        m_psNodeInfo->m_byPrefAddress = byAddress;
        m_omPrefAdres.vSetValue(byAddress);
    }
}
/******************************************************************************/
/*  Function Name    :  vSetNodeInfoEDHanStatus                               */
/*                                                                            */
/*  Input(s)         :  int nSelItem , BOOL bIsEnabled                        */
/*  Output           :                                                        */
/*  Functionality    :  Sets the Node Info structure with the bIsEnabled to enable/
                        disable the handler corresponding to the selected item.
/*  Member of        :  CSimSysDetView                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M											  */
/*  Date Created     :  12.1.2006                                             */
/*  Modifications    :  
/******************************************************************************/


void CSimSysDetView::vSetNodeInfoEDHanStatus( int nSelItem , BOOL bIsEnabled )
{
    // here check the string name omStrSelText,to know which handler is E/D and 
    // set the corresponding flag to the bIsEnabled.

    CSimSysNodeInfo *pSimSysNodeInf = 
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    switch(nSelItem)
    {
        case 0 : 
        {
            m_bIsMsgHanEnabled = bIsEnabled;
            pSimSysNodeInf->vSetEnableNodeMsgHandlers(STR_EMPTY , 
                m_psNodeInfo->m_omStrNodeName , bIsEnabled);
            m_psNodeInfo->m_bMsgHandlersEnabled = bIsEnabled;

            vChangeEDHanButtonText( bIsEnabled );


        }
        break;
        case 1 : 
        {
            m_bIsTimerHanEnabled = bIsEnabled;
            m_psNodeInfo->m_bTimerHandlersEnabled = bIsEnabled;
            pSimSysNodeInf->vSetEnableNodeTimerHandlers(STR_EMPTY , 
                m_psNodeInfo->m_omStrNodeName , bIsEnabled);
            vChangeEDHanButtonText( bIsEnabled );

        }
        break;
        case 2 : 
        {
            m_bIsKeyHanEnabled = bIsEnabled;
            m_psNodeInfo->m_bKeyHandlersEnabled = bIsEnabled;
            pSimSysNodeInf->vSetEnableNodeKeyHandlers(STR_EMPTY , 
                m_psNodeInfo->m_omStrNodeName , bIsEnabled);
            vChangeEDHanButtonText( bIsEnabled );

        }
        break;
        case 3 : 
        {
            m_bIsErrorHanEnabled = bIsEnabled;
            m_psNodeInfo->m_bErrorHandlersEnabled = bIsEnabled;
            pSimSysNodeInf->vSetEnableNodeErrorHandlers(STR_EMPTY , 
                m_psNodeInfo->m_omStrNodeName , bIsEnabled);
            vChangeEDHanButtonText( bIsEnabled );

        }
        break;
        default : AfxMessageBox(_T("Error in handler details of Node info"));
    }	
}
/******************************************************************************/
/*  Function Name    :  vChangeEDHanButtonText                                */
/*                                                                            */
/*  Input(s)         :  BOOL bIsLoaded                                        */
/*  Output           :                                                        */
/*  Functionality    :  Changes the Text of the button to "Enable Handler"  
                        or "Disable Handler" 
/*  Member of        :  CSimSysDetView                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M											  */
/*  Date Created     :  12.1.2006                                             */
/*  Modifications    :  
/******************************************************************************/
void CSimSysDetView::vChangeEDHanButtonText(  BOOL bIsEnabled )
{
    LPCTSTR StrBtnHanTxt = _T("Enable Handler");
    if(bIsEnabled)
    {
        StrBtnHanTxt = _T("Disable Handler");      
    }
    GetDlgItem(IDC_BUTTON_ENABLEHANDLER)->SetWindowText(StrBtnHanTxt);  
}
/******************************************************************************/
/*  Function Name    :  vChangeEDAllHanButtonText                             */
/*                                                                            */
/*  Input(s)         :  BOOL bIsLoaded                                        */
/*  Output           :                                                        */
/*  Functionality    :  Changes the Text of the button to "Enable Handler" 
                        or "Disable Handler"
/*  Member of        :  CSimSysDetView                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  
/*  Date Created     :  
/*  Modifications    :  
/******************************************************************************/
void CSimSysDetView::vChangeEDAllHanButtonText(BOOL bIsEnabled)
{
    LPCTSTR StrBtnHanTxt = _T("Enable All Handlers");
    if(bIsEnabled)
    {
        StrBtnHanTxt = _T("Disable All Handlers");      
    }
    GetDlgItem(IDC_BUTTON_ENABLEALLHANDLERS)->SetWindowText(StrBtnHanTxt);  
}

/******************************************************************************/
/*  Function Name    :  bGetControlStatus                                     */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Returns "m_bAreControlsHidden"
/*  Member of        :  CSimSysDetView                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M											  */
/*  Date Created     :  20.12.2005                                            */
/*  Modifications    :  
/******************************************************************************/
BOOL CSimSysDetView::bGetControlStatus()
{
    return (m_bAreControlsHidden);
}

/*****************************************************************************/
/*  Function Name    :  vEnableButtons
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Enables/Disables "New File", "Edit File", "Build"
                        "Build and Load" and "Load" buttons
/*  Member of        :  CSimSysDetView                                         
/*  Friend of        :      -                                                 
/*                                                                            
/*  Author(s)        :  Harika M											  */
/*  Date Created     :  20.12.2005                                            */
/*  Modifications    :  
/******************************************************************************/
void CSimSysDetView::vEnableButtons(BOOL bEnable)
{
    //GetDlgItem(IDC_BUTTON_OPENFILE)->EnableWindow(bEnable);
    GetDlgItem(IDC_BUTTON_EDITFILE)->EnableWindow(bEnable);
    GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(bEnable);
    GetDlgItem(IDC_BUTTON_BUILDANDLOAD)->EnableWindow(bEnable);
    GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(bEnable);
}
/*****************************************************************************/
/*  Function Name    :  OnItemchangedLstcSignalDetails
/*  Input(s)         :  Item State & Result Pointer from Message Dispatch
/*  Output           :  -
/*  Functionality    :  Updates the Handler Descriptor List as per the selection 
/*  Member of        :  CSimSysDetView                                         
/*  Friend of        :      -                                                 
/*                                                                            
/*  Author(s)        :  Harika M
/*  Date Created     :  20.01.2006
/*****************************************************************************/
void CSimSysDetView::OnItemchangedLstcHandlerDetails(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    UINT unItemStateMask = LVIS_SELECTED|LVIS_FOCUSED;
    
    if(pNMListView->uNewState == unItemStateMask)
    {
        vUpDownArrowKeySelection(pNMListView->iItem);
    }
	*pResult = 0;
}

/*****************************************************************************/
/*  Function Name    :  vUpDownArrowKeySelection
/*  Input(s)         :  Selected list item
/*  Output           :  -
/*  Functionality    :  Updates the handler value list control with selected  
                        handler                                
/*  Member of        :  CSimSysDetView                                         
/*  Friend of        :      -                                                 
/*                                                                            
/*  Author(s)        :  Harika M											  
/*  Date Created     :  20.12.2005                                            
/*  Modifications    :  
/******************************************************************************/
void CSimSysDetView::vUpDownArrowKeySelection(int nSel )// selected item index 
{
    BOOL bConnected = CSimSysManager::ouGetSimSysManager(m_eBus).
                        ouGetFlags().nGetFlagStatus(H_CONNECTED);
	    
    if (nSel != -1 )
    {
        // Enable "Enable/Disable All handlers button" and "Enable/Disable handler button'
        if(bConnected)
        {
            GetDlgItem(IDC_BUTTON_ENABLEALLHANDLERS)->EnableWindow(TRUE);
            GetDlgItem(IDC_BUTTON_ENABLEHANDLER)->EnableWindow(TRUE);
        }
        else
        {
            GetDlgItem(IDC_BUTTON_ENABLEALLHANDLERS)->EnableWindow(FALSE);
            GetDlgItem(IDC_BUTTON_ENABLEHANDLER)->EnableWindow(FALSE);
        }
        
        m_omListCtrlHanVal.DeleteAllItems();
        
        // Toggle enable-disable handler button accordingly 
        // and update the handler details in the Handler value list
        CString omStrTemp = STR_EMPTY;
        switch(nSel)
        {
        case 0 : 
            {
                int i=0;
                m_bIsMsgHanEnabled = m_psNodeInfo->m_bMsgHandlersEnabled ;
                vChangeEDHanButtonText( m_bIsMsgHanEnabled );
                // fill the handler value list with the fun names and its value
                int nMsgCount = (COMMANINT)m_psNodeInfo->m_omStrArrayMsgHandlers.GetSize();
                for ( i=0 ; i < nMsgCount ; i++ )
                {
                    omStrTemp = m_psNodeInfo->m_omStrArrayMsgHandlers.ElementAt(i);
                    m_omListCtrlHanVal.InsertItem( i , omStrTemp);
                }
              
            }
            break;
        case 1 : 
            {
                int i = 0;
                m_bIsTimerHanEnabled = m_psNodeInfo->m_bTimerHandlersEnabled;
                // fill the handler value list with the fun names and its value
                int nTimerCount = (COMMANINT)m_psNodeInfo->m_omStrArrayTimerHandlers.GetSize();
                for ( i=0 ; i < nTimerCount ; i++ )
                {
                    omStrTemp = m_psNodeInfo->m_omStrArrayTimerHandlers.ElementAt(i);
                    m_omListCtrlHanVal.InsertItem( i , omStrTemp);
                }
                vChangeEDHanButtonText( m_bIsTimerHanEnabled );
                
            }
            break;
        case 2 : 
            {
                int i = 0;
                m_bIsKeyHanEnabled = m_psNodeInfo->m_bKeyHandlersEnabled;
                // fill the handler value list with the fun names and its value
                int nKeyCount = (COMMANINT)m_psNodeInfo->m_omStrArrayKeyHandlers.GetSize();
                for ( i=0 ; i < nKeyCount ; i++ )
                {
                    omStrTemp = m_psNodeInfo->m_omStrArrayKeyHandlers.ElementAt(i);
                    m_omListCtrlHanVal.InsertItem( i , omStrTemp);
                }
                vChangeEDHanButtonText( m_bIsKeyHanEnabled );
            }
            break;
        case 3 : 
            {
                int i = 0;
                m_bIsErrorHanEnabled = m_psNodeInfo->m_bErrorHandlersEnabled;
                // fill the handler value list with the fun names and its value
                int nErrorCount = (COMMANINT)m_psNodeInfo->m_omStrArrayErrorHandlers.GetSize();
                for ( i=0 ; i < nErrorCount ; i++ )
                {
                    omStrTemp = m_psNodeInfo->m_omStrArrayErrorHandlers.ElementAt(i);
                    m_omListCtrlHanVal.InsertItem( i , omStrTemp);
                }
                vChangeEDHanButtonText( m_bIsErrorHanEnabled );
            }
            break;
        default : AfxMessageBox(_T("Error in handler details of Node info"));
            break;
        }
        
        
    }
    if( nSel == -1 )
    {
        // Disable handler manipulation buttons
        GetDlgItem(IDC_BUTTON_ENABLEALLHANDLERS)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_ENABLEHANDLER)->EnableWindow(FALSE);
        //empty the handler description list as well
        m_omListCtrlHanVal.DeleteAllItems();
    } 
}

/*******************************************************************************
Function Name    :  PostNcDestroy
Input(s)         :  -
Output           :  -
Functionality    :  This function will be called by framework after destroying
                    the window. This will initialise CmainFrame's view pointer
                    to null
Member of        :  CSimSysDetView                                         
Friend of        :      -                                                 

Author(s)        :  Harika M											  
Date Created     :  20.12.2005                                            
Modifications    :  
/******************************************************************************/
void CSimSysDetView::PostNcDestroy() 
{
	  CSimSysManager::ouGetSimSysManager(m_eBus).podSetSimSysDetView( NULL );
	  CFormView::PostNcDestroy();
}



/******************************************************************************/
/*  Function Name    :  vAddItemToHanDetList                                  */
/*                                                                            */
/*  Input(s)         :  sNODEINFO* pNode                     */
/*  Output           :                                                        */
/*  Functionality    :  Adds the handler details into the list 
/*  Member of        :  CSimSysDetView                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Anish Kr.                                     
/*  Date Created     :                                         
/*  Modifications    :  
/******************************************************************************/
void CSimSysDetView::vAddItemToHanDetList( sNODEINFO* pNode )
{
    
    CString omStrTemp = STR_EMPTY;
    // get the CStringArray of all handlers and update the handler handler details 
    // with the no. of handlers present for each type n status.
    // by default the first handler " Message handlers" has to be selected.
    
    if( pNode != NULL )
    {
        // for Msg handlers
        vSetHandlerDetailRowText(0,
                                 (COMMANINT)pNode->m_omStrArrayMsgHandlers.GetSize(),
                                 pNode->m_bMsgHandlersEnabled);
        
                
        // for Timer handlers
        vSetHandlerDetailRowText(1,
                                 (COMMANINT)pNode->m_omStrArrayTimerHandlers.GetSize(),
                                 pNode->m_bTimerHandlersEnabled);
               
               
        // for Key handlers
        vSetHandlerDetailRowText(2,
                                 (COMMANINT)pNode->m_omStrArrayKeyHandlers.GetSize(),
                                 pNode->m_bKeyHandlersEnabled);
       
        
                
        // for Error handlers
        vSetHandlerDetailRowText(3,
                                 (COMMANINT)pNode->m_omStrArrayErrorHandlers.GetSize(),
                                 pNode->m_bErrorHandlersEnabled);
                     
    }
    m_omListCtrlHanDet.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
    vUpDownArrowKeySelection(0);
    
    //UpdateData(FALSE);
    
}

void CSimSysDetView::vSetHandlerDetailRowText(int nRow, int nNoOfHandler, BOOL bEnabled)
{
    if (nRow >= 0)
    {
        CString strHandlerCount = _T("");
        CString strEnabled = defStrDisabled;
        if (bEnabled)
        {
            strEnabled = defStrEnabled; 
        }
        strHandlerCount.Format(_T("%d"), nNoOfHandler);
        //Handler counts in 2nd row(0 based)
        m_omListCtrlHanDet.SetItemText(nRow, 1, strHandlerCount);
        //State in 3rd row
        m_omListCtrlHanDet.SetItemText(nRow, 2, strEnabled);
    }
}
void CSimSysDetView::vGetNodeInfo(PSNODEINFO psNodeInfo)
{
    psNodeInfo->m_omStrNodeName = m_psNodeInfo->m_omStrNodeName;
    psNodeInfo->m_omStrDllName = m_psNodeInfo->m_omStrDllName;
    //psNodeInfo->m_CurrLCN = m_psNodeInfo->m_CurrLCN;
}

void CSimSysDetView::vUpdateHandlerList()
{
    if (NULL != m_psNodeInfo)
    {
        vAddItemToHanDetList( m_psNodeInfo );
    }
}

void CSimSysDetView::vUpdateHandlerEnableDisableStatus(int nItem, BOOL bEnableHandler)
{	
	CFlags &ouFlag =  CSimSysManager::ouGetSimSysManager(m_eBus).ouGetFlags();
	switch (nItem)
	{
	case -1:
		ouFlag.vSetFlagStatus(H_ALL_HANDLER, bEnableHandler);
		ouFlag.vSetFlagStatus(H_MSGHANDLERBUTTON, bEnableHandler);
		ouFlag.vSetFlagStatus(H_TIMERBUTTON, bEnableHandler);
		ouFlag.vSetFlagStatus(H_KEY_HANDLER_ON, bEnableHandler);
		ouFlag.vSetFlagStatus(H_ERROR_HANDLER, bEnableHandler);
		break;
	case 0:	// Message Handler			
		ouFlag.vSetFlagStatus(H_MSGHANDLERBUTTON, bEnableHandler);
		break;
	case 1:	// Timer Handler			
		ouFlag.vSetFlagStatus(H_TIMERBUTTON, bEnableHandler);
		break;
	case 2:	// Key Handler
		ouFlag.vSetFlagStatus(H_KEY_HANDLER_ON, bEnableHandler);
		break;
	case 3:	// Error Handler
		ouFlag.vSetFlagStatus(H_ERROR_HANDLER, bEnableHandler);
		break;
	}
}