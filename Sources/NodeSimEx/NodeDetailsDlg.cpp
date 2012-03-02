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
 * \file      NodeDetailsDlg.cpp
 * \brief     Implementation file for CNodeDetailsDlg class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CNodeDetailsDlg class
 */
#include "NodeSimEx_stdafx.h" // Contains standard include header files.
#include "NodeDetailsDlg.h"	  // Class defintion included here
#include "SimSysManager.h"
#include "HashDefines.h"
#include "NodeDetailsDlg.h"
#include "GlobalObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNodeDetailsDlg dialog

/******************************************************************************
Function Name    :  CNodeDetailsDlg                                    
Input(s)         :                                                        
Output           :                                                        
Functionality    :  Constructor is called when user create an object of   
this class. Initialisation of all data members done here   
Member of        :  CNodeDetailsDlg                                            
Friend of        :      -                                                 
Author(s)        :  Harika M                                        
Date Created     :  27-12-2005                                            
*****************************************************************************/
CNodeDetailsDlg::CNodeDetailsDlg(ETYPE_BUS eBus, PSNODEINFO pNode /*=NULL*/,
                                 CWnd* pParent /*=NULL*/)
                                 : CDialog(CNodeDetailsDlg::IDD, pParent)
                                 
{
    //{{AFX_DATA_INIT(CNodeDetailsDlg)
    m_omStrDllPath = _T("");
    m_omStrNodeName = _T("");
    //}}AFX_DATA_INIT
    m_psNodeStuctPtr = pNode;
    m_bIsNodeModified = FALSE;
    m_eBus = eBus;    
}


void CNodeDetailsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNodeDetailsDlg)
    DDX_Text(pDX, IDC_EDIT_DLL_PATH, m_omStrDllPath);
    DDX_Text(pDX, IDC_EDIT_ANODE_NAME, m_omStrNodeName);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_EDIT_ADDRESS, m_omPreferedAddress);
    DDX_Control(pDX, IDC_EDIT_ECU_NAME, m_omEcuName);
}


BEGIN_MESSAGE_MAP(CNodeDetailsDlg, CDialog)
//{{AFX_MSG_MAP(CNodeDetailsDlg)
ON_BN_CLICKED(IDC_BTN_BROWSE, OnBrowse)
ON_BN_CLICKED(IDC_BTN_CLEAR, OnClear)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNodeDetailsDlg message handlers
/******************************************************************************
Function Name    :  OnCancel
Input(s)         :                                                        
Output           :                                                        
Functionality    :  Calls base class functionality.
Member of        :  CNodeDetailsDlg                                            
Friend of        :      -                                                 
Author(s)        :  Harika M                                        
Date Created     :  27-12-2005                                            
*****************************************************************************/
void CNodeDetailsDlg::OnCancel() 
{
    // TODO: Add extra cleanup here
    
    CDialog::OnCancel();
}
/******************************************************************************
Function Name    :  OnInitDialog                                    
Input(s)         :                                                        
Output           :                                                        
Functionality    :  Initialises the dialog with the data if the mode is edit 
                    mode. Otherwise the default values will be initialised.
Member of        :  CNodeDetailsDlg                                            
Friend of        :      -                                                 
Author(s)        :  Harika M                                        
Date Created     :  27-12-2005                                            
*****************************************************************************/
BOOL CNodeDetailsDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
      
    // If Not NULL, mode is Edit,
    // Fill the dialog with the node details
    if (m_psNodeStuctPtr != NULL)
    {
        m_omStrNodeName  = m_psNodeStuctPtr->m_omStrNodeName;
        m_omStrDllPath   = m_psNodeStuctPtr->m_omStrDllName;
        m_omPreferedAddress.vSetValue(m_psNodeStuctPtr->m_byPrefAddress);
        m_omEcuName.vSetValue(m_psNodeStuctPtr->m_unEcuName);
    }
    
    if(!m_omStrDllPath.IsEmpty())
    {
        vEnableClearButton(TRUE);
    }
    if (m_eBus == J1939)
    {
        m_omPreferedAddress.vSetBase(BASE_HEXADECIMAL);
        m_omPreferedAddress.vSetSigned(FALSE);
        m_omPreferedAddress.LimitText(2);
        m_omPreferedAddress.vSetValue(0);

        m_omEcuName.vSetBase(BASE_HEXADECIMAL);
        m_omEcuName.vSetSigned(FALSE);
        m_omEcuName.LimitText(16);
        m_omEcuName.vSetValue(0x8000000000000000);

        CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_ADDRESS);
        pWnd->ShowWindow(TRUE);
        pWnd = (CWnd*)GetDlgItem(IDC_STATIC_ECU_NAME);
        pWnd->ShowWindow(TRUE);
        m_omPreferedAddress.ShowWindow(TRUE);
        m_omEcuName.ShowWindow(TRUE);
    }
    UpdateData(FALSE);

    return TRUE;  
}
/******************************************************************************
Function Name    :  OnOK
Input(s)         :                                                        
Output           :                                                        
Functionality    :  Called when the user presses OK button. 
                    Validation of the fileds.
                                     
Member of        :  CNodeDetailsDlg                                            
Friend of        :      -                                                 
Author(s)        :                                        
Date Created     :  
Modification     :  
*****************************************************************************/
void CNodeDetailsDlg::OnOK() 
{
    BOOL bDuplicateFound = FALSE;
    BOOL bRetVal = TRUE;

    UpdateData(TRUE);
    if (m_omStrNodeName.IsEmpty())
    {
        AfxMessageBox( _T("Node Name cannot be empty!"), MB_OK|MB_ICONINFORMATION );
        
        GetDlgItem( IDC_EDIT_ANODE_NAME )->SetWindowText(m_psNodeStuctPtr->m_omStrNodeName);
        GetDlgItem( IDC_EDIT_ANODE_NAME )->SetFocus();
        bRetVal = FALSE;
    }
    else
    {
        CSimSysNodeInfo* pSimSysNodeInfo = 
            CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
        //Check for duplicate node name if the name is changed
        if ( m_psNodeStuctPtr->m_omStrNodeName != m_omStrNodeName)
        {
            if (pSimSysNodeInfo->bIsDuplicateNode( m_omStrNodeName ))
            {
                AfxMessageBox( _T("Duplicate Node Name found!"), MB_OK|MB_ICONINFORMATION );
                GetDlgItem(IDC_EDIT_ANODE_NAME)->SetFocus();
                bRetVal = FALSE;
            }            
        }
        if (m_eBus == J1939)
        {            
            if (bRetVal == TRUE)
            {
                //Check for duplicate ECU NAME if the current if J1939
                UINT64 unEcuName = (UINT64)m_omEcuName.lGetValue();
                if ( m_psNodeStuctPtr->m_unEcuName != unEcuName)
                {                
                    if (pSimSysNodeInfo->bIsDuplicateEcuName(unEcuName))
                    {
                        AfxMessageBox( _T("Duplicate ECU NAME found!"), MB_OK|MB_ICONINFORMATION );
                        GetDlgItem(IDC_EDIT_ECU_NAME)->SetFocus();
                        bRetVal = FALSE;
                    }
                }
            }
            if (bRetVal == TRUE)
            {
                //Check for duplicate Preffered Address
                BYTE byPrefAdres = (BYTE)m_omPreferedAddress.lGetValue();
                if ( m_psNodeStuctPtr->m_byPrefAddress != byPrefAdres)
                {                
                    if (pSimSysNodeInfo->bIsDuplicateAddress(byPrefAdres))
                    {
                        INT nChoice = AfxMessageBox( _T("Another node is configured to claim the same address.\nDo you want to proceed?"), 
                                                        MB_YESNO|MB_ICONWARNING );
                        if (nChoice == IDNO)
                        {
                            GetDlgItem(IDC_EDIT_ADDRESS)->SetFocus();
                            bRetVal = FALSE;
                        }
                    }
                }
            }
        }
        //Check for duplicate dll
        if( (!m_omStrDllPath.IsEmpty()) && 
            (m_psNodeStuctPtr->m_omStrDllName != m_omStrDllPath))
        {
            if (pSimSysNodeInfo->bIsDuplicateDllName( m_omStrDllPath ))
            {
                AfxMessageBox( _T("Duplicate Dll found!"), MB_OK|MB_ICONINFORMATION );
                GetDlgItem(IDC_EDIT_DLL_PATH)->SetFocus();
                bRetVal = FALSE;
            }
        }
    }
    if ( bRetVal == TRUE )
    {
        //If name is changed
        if (m_psNodeStuctPtr->m_omStrNodeName != m_omStrNodeName)
        {
            m_bIsNodeModified = TRUE;
            m_psNodeStuctPtr->m_omStrNodeName = m_omStrNodeName;
        }
        if (m_psNodeStuctPtr->m_omStrDllName  != m_omStrDllPath)
        {
            //If dll is changed
            m_bIsNodeModified = TRUE;
            //If no error then update the node details and close the dialog box
            m_psNodeStuctPtr->m_omStrDllName  = m_omStrDllPath;
            // Now search for respective *.c file and change it
            CString omStrFilename = m_psNodeStuctPtr->m_omStrDllName;
            omStrFilename.Replace(defDOT_DLL ,defDOT_SMALL_C);
            // file-attribute information
            struct _tfinddata_t fileinfo;
            // Check if file exists
            if (_tfindfirst( omStrFilename.GetBuffer(MAX_PATH), &fileinfo) != -1L)
            {
                m_psNodeStuctPtr->m_omStrFileName = omStrFilename;
            }
        }
        if (m_psNodeStuctPtr->m_byPrefAddress != 
            (BYTE)(m_omPreferedAddress.lGetValue()))
        {
            m_bIsNodeModified = TRUE;
            m_psNodeStuctPtr->m_byPrefAddress = (BYTE)(m_omPreferedAddress.lGetValue());
        }
        if (m_psNodeStuctPtr->m_unEcuName != 
            (UINT64)(m_omEcuName.lGetValue()))
        {
            m_bIsNodeModified = TRUE;
            m_psNodeStuctPtr->m_unEcuName = (UINT64)(m_omEcuName.lGetValue());
        }
        CDialog::OnOK();
    }
}
/******************************************************************************
Function Name    :  PreTranslateMessage
Input(s)         :  MSG* pMsg                                                      
Output           :                                                        
Functionality    :  Prevents the space character in the nameto be processed
                    as it is not recommended.
Member of        :  CNodeDetailsDlg                                            
Friend of        :      -                                                 
Author(s)        :  Harika M                                        
Date Created     :  27-12-2005                                            
*****************************************************************************/
BOOL CNodeDetailsDlg::PreTranslateMessage(MSG* pMsg) 
{
    BOOL bSkip = FALSE;
    if ( pMsg->message == WM_CHAR )
    {
        // If the control is node name
        // do not allow space characters 
        if ( pMsg->wParam == ' ')
        {
            CEdit* omEditCtrlName   = (CEdit*) GetDlgItem(IDC_EDIT_ANODE_NAME);
            CEdit* omEditFocusName  = (CEdit*)GetFocus();
            // Shud not process space characters in Name and Code
            // Edit Control only
            if ( omEditCtrlName == omEditFocusName )  
            {
                bSkip = TRUE;
            }
        }
    }
    if ( bSkip == FALSE)
    {
        bSkip = CDialog::PreTranslateMessage(pMsg);
    }
    return bSkip;
}
/******************************************************************************/
/*  Function Name    :  OnBrowse                                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Displays open file dialog and fills the edit 
                            with selected dll file
/*  Member of        :  CNodeDetailsDlg                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  27-12-2005                                            */
/*  Modifications    :  
/******************************************************************************/
void CNodeDetailsDlg::OnBrowse() 
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    // Display open file dialog
    // which will allow the user to select 
    // only .dll files
    if((m_psNodeStuctPtr != NULL) &&(m_psNodeStuctPtr->m_bIsDllLoaded))
    {
        AfxMessageBox(_T("Dll is already Loaded! Unload it first and try again."), 
                       MB_OK|MB_ICONINFORMATION);
    }
    else
    {
        CFileDialog om_Dlg( TRUE,  //open an existing file
                            NULL,  //extension to file
                            _T(""),  //initial file name
                            OFN_FILEMUSTEXIST| OFN_HIDEREADONLY|
                            OFN_PATHMUSTEXIST, 
                            _T("Dll Files(*.dll)|*.dll||"), 
                            NULL );
        // Set caption text
        om_Dlg.m_ofn.lpstrTitle = _T("Select the Dll");
        
        if ( om_Dlg.DoModal() == IDOK )
        {
            // Display selected file name
            m_omStrDllPath = om_Dlg.GetPathName();
            // to enable the clear button
            vEnableClearButton(TRUE);
            //UpdateData(FALSE);
            SetDlgItemText( IDC_EDIT_DLL_PATH, m_omStrDllPath);
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnClear                                               */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Clears the Dll Path Edit box.
/*  Member of        :  CNodeDetailsDlg                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  16-1-2006                                            */
/*  Modifications    :  
/******************************************************************************/
void CNodeDetailsDlg::OnClear() 
{
    m_omStrDllPath.Empty();
    GetDlgItem(IDC_EDIT_DLL_PATH)->SetWindowText(m_omStrDllPath);
    vEnableClearButton(FALSE);
        
}
/******************************************************************************/
/*  Function Name    :  vEnableClearButton                                    */
/*                                                                            */
/*  Input(s)         :  BOOL bEnable                                          */
/*  Output           :                                                        */
/*  Functionality    :  Enables or disables the Clear button.
/*  Member of        :  CNodeDetailsDlg                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  16-1-2006                                             */
/*  Modifications    :  
/******************************************************************************/
void CNodeDetailsDlg::vEnableClearButton(BOOL bEnable)
{
    GetDlgItem(IDC_BTN_CLEAR)->EnableWindow(bEnable);
}
