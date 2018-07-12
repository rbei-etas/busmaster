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
#include "NodeDetailsDlg.h"   // Class defintion included here
#include "SimSysManager.h"
#include "HashDefines.h"
#include "NodeDetailsDlg.h"
#include "GlobalObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define defNODE_CONFIG_FILTER "All Supported Simulation Files (*.cpp)|*.cpp;*.dll|Cpp File(s) (*.cpp)|*.cpp|Dll File(s) (*.dll)|*.dll||"
#define defSTR_ERR_INVALID_FILE "Invalid File. Please load valid(.cpp/.dll) File."
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
CNodeDetailsDlg::CNodeDetailsDlg(ETYPE_BUS eBus, CGlobalObj* pGlobalObj, CSimSysManager* pSimSysManager, PSNODEINFO pNode /*=nullptr*/,
                                 CWnd* pParent /*=nullptr*/)
    : CDialog(CNodeDetailsDlg::IDD, pParent)

{
    //{{AFX_DATA_INIT(CNodeDetailsDlg)
    m_omStrDllPath = "";
    m_omStrNodeName = "";
    //}}AFX_DATA_INIT
    m_psNodeStuctPtr = pNode;
    m_bIsNodeModified = FALSE;
    m_eBus = eBus;
    m_bEdit = FALSE;
    m_eNodeFileType = NODE_FILE_C_CPP;
    m_pGlobalObj = pGlobalObj;
    m_pSimSysMgr = pSimSysManager;
}


void CNodeDetailsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNodeDetailsDlg)
    DDX_Text(pDX, IDC_EDIT_NODE_FILE_PATH, m_omStrNodeFilePath);
    DDX_Text(pDX, IDC_EDIT_ANODE_NAME, m_omStrNodeName);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_EDIT_ADDRESS, m_omPreferedAddress);
    DDX_Control(pDX, IDC_EDIT_ECU_NAME, m_omEcuName);
}


BEGIN_MESSAGE_MAP(CNodeDetailsDlg, CDialog)
    //{{AFX_MSG_MAP(CNodeDetailsDlg)
    ON_BN_CLICKED(IDC_BTN_BROWSE, OnBrowse)
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
        if(m_bEdit == TRUE)
        {
            m_omPreferedAddress.EnableWindow(FALSE);
            m_omEcuName.EnableWindow(FALSE);
        }
    }

    // If Not nullptr, mode is Edit,
    // Fill the dialog with the node details
    if (m_psNodeStuctPtr != nullptr && m_bEdit )
    {
        m_omStrNodeName  = m_psNodeStuctPtr->m_omStrNodeName;
        m_omStrDllPath   = m_psNodeStuctPtr->m_omStrDllName;
        m_omStrCFile     = m_psNodeStuctPtr->m_omStrCFileName;
        m_omPreferedAddress.vSetValue(m_psNodeStuctPtr->m_byPrefAddress);
        m_omEcuName.vSetValue(m_psNodeStuctPtr->m_unEcuName);


        if(m_psNodeStuctPtr->m_eNodeFileType == NODE_FILE_DLL && !m_omStrDllPath.IsEmpty())
        {
            m_omStrNodeFilePath =  m_omStrDllPath;
            m_eNodeFileType = NODE_FILE_DLL;
        }
        else if((m_psNodeStuctPtr->m_eNodeFileType == NODE_FILE_C_CPP) && !m_omStrCFile.IsEmpty())
        {
            m_omStrNodeFilePath =  m_omStrCFile;
        }
        else
        {
            m_omStrNodeFilePath = "";
        }
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
Modification     :  Robin G.K.
                    21.10.14, Moved updation of node info and check for duplication
                    to different function.
*****************************************************************************/
void CNodeDetailsDlg::OnOK()
{

    BOOL bDuplicateFound = FALSE;
    BOOL bRetVal = TRUE;

    UpdateData(TRUE);

    if (m_omStrNodeName.IsEmpty())
    {
        AfxMessageBox( "Node Name cannot be empty!", MB_OK|MB_ICONINFORMATION );

        GetDlgItem( IDC_EDIT_ANODE_NAME )->SetWindowText(m_psNodeStuctPtr->m_omStrNodeName);
        GetDlgItem( IDC_EDIT_ANODE_NAME )->SetFocus();
        bRetVal = FALSE;
    }
    else
    {
        CSimSysNodeInfo* pSimSysNodeInfo =
            m_pSimSysMgr->pomGetSimSysNodeInfo();
        //Check for duplicate node name if the name is changed
        if ( m_psNodeStuctPtr->m_omStrNodeName != m_omStrNodeName)
        {
            if (pSimSysNodeInfo->bIsDuplicateNode( m_omStrNodeName ))
            {
                AfxMessageBox( "Duplicate Node Name found!", MB_OK|MB_ICONINFORMATION );
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
                        AfxMessageBox( "Duplicate ECU NAME found!", MB_OK|MB_ICONINFORMATION );
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
                        INT nChoice = AfxMessageBox( "Another node is configured to claim the same address.\nDo you want to proceed?",
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
        if ((!m_omStrCFile.IsEmpty() && m_psNodeStuctPtr->m_omStrCFileName != m_omStrCFile && pSimSysNodeInfo->bIsDuplicateCFile( m_omStrCFile ))
                || (!m_omStrDllPath.IsEmpty() && m_psNodeStuctPtr->m_omStrDllName != m_omStrDllPath && pSimSysNodeInfo->bIsDuplicateDllName( m_omStrDllPath )))
        {
            AfxMessageBox( "Duplicate File found!", MB_OK|MB_ICONINFORMATION );
            GetDlgItem(IDC_EDIT_NODE_FILE_PATH)->SetFocus();
            bRetVal = FALSE;
        }
        else
        {
            m_psNodeStuctPtr->m_eNodeFileType = m_eNodeFileType;
        }
    }
    if ( bRetVal == TRUE)
    {
        if (m_psNodeStuctPtr->m_omStrNodeName != m_omStrNodeName)
        {
            m_bIsNodeModified = TRUE;
            m_psNodeStuctPtr->m_omStrNodeName = m_omStrNodeName;
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
        if(!m_omStrNodeFilePath.IsEmpty())
        {
            int nReturnVal = IDYES;

            if(m_psNodeStuctPtr->m_omStrCFileName != m_omStrCFile && !m_psNodeStuctPtr->m_omStrCFileName.IsEmpty())
            {
                nReturnVal = AfxMessageBox(ALL_NODE_EDIT_CONFMN, MB_YESNO|MB_ICONQUESTION);
            }
            if(nReturnVal)
            {
                bRetVal = FALSE;
                if(m_eNodeFileType == NODE_FILE_C_CPP  && !m_omStrCFile.IsEmpty())
                {
                    if(bOpenfile())
                    {
                        bUpdateNodeInfoFile();
                        bRetVal = TRUE;
                    }
                }
                else
                {
                    bUpdateNodeInfoFile();
                    bRetVal = TRUE;
                }
            }
        }
    }
    if(bRetVal == TRUE)
    {
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
/*  Modifications    :  Robin G.K.
                        21.10.2014, Add support for .cpp,.c and .dll.
                                    Removed unused "Clear" button enabling.   */
/******************************************************************************/
void CNodeDetailsDlg::OnBrowse()
{
    CFileDialog om_Dlg( TRUE,  //open an existing file
                        "cpp",  //extension to file
                        nullptr,  //initial file name
                        OFN_HIDEREADONLY|
                        OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT,
                        defNODE_CONFIG_FILTER,
                        nullptr );
    // Set caption text
    om_Dlg.m_ofn.lpstrTitle = "Select the File";

    if ( om_Dlg.DoModal() == IDOK )
    {
        CString strExtName  = om_Dlg.GetFileExt();
        strExtName.MakeLower();
        if(strExtName == "cpp" || strExtName == "dll")
        {
            // Display selected file name
            if(strExtName == "dll")
            {
                m_omStrCFile = "";
                m_omStrDllPath ="";
                CString omStrFilePath = om_Dlg.GetPathName();
                omStrFilePath.Replace(".dll",".cpp");
                CFileFind omFileFind;
                /*if the .cpp file corresponding to the .dll file is found in the folder
                containing the .dll file then we consider it as .cpp file and the file(.cpp)
                is associated to the .dll and the .cpp file can be edited as well as built.
                Else the .dll is simply used for loading
                and cant be edited or build*/
                if(omFileFind.FindFile(omStrFilePath) == 0)
                {
                    m_omStrDllPath = om_Dlg.GetPathName();
                    m_eNodeFileType = NODE_FILE_DLL;
                }
                else
                {
                    m_omStrCFile = omStrFilePath;
                    m_eNodeFileType = NODE_FILE_C_CPP;
                }
                SetDlgItemText( IDC_EDIT_NODE_FILE_PATH, om_Dlg.GetPathName());
            }
            else if(strExtName == "cpp")
            {
                m_eNodeFileType = NODE_FILE_C_CPP;
                m_omStrCFile = om_Dlg.GetPathName();;
                SetDlgItemText( IDC_EDIT_NODE_FILE_PATH, m_omStrCFile);

            }
        }
        else
        {
            AfxMessageBox(defSTR_ERR_INVALID_FILE ,MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);
        }

    }

}

bool CNodeDetailsDlg::bCheckIfOldCppFile(CString& omText)
{
    CString omTemp = C_FILE_VERSION_VALUE;
    float nCurrentVersion = atof(omTemp);
    CString omVersion;

    int nIndex = omText.Find('[');
    omVersion = omText.Mid(nIndex + 1, omText.ReverseFind('.') - nIndex + 1);
    CString omTemp1 = omVersion;

    float nVersion = atof(omTemp1);

    // If Cpp file version is less than the current version
    if (nVersion < nCurrentVersion)
    {
        omText.Replace(omVersion, omTemp);
        return true;
    }

    return false;
}

/******************************************************************************/
/*  Function Name    :  bOpenfile
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Calls  function, Enables the user to (associate)
                        open an existing C file or create a new file on button press
/*  Member of        :  CSimSysDetView
/*  Friend of        :      -
/*
/*  Author(s)        :  Harika M
/*  Date Created     :  21.12.2005
/*  Modification     :  Robin G.K.
                        21.10.14, Moved this functionality from CSimSysDetView
                        to CNodeDetailsDlg.
/*****************************************************************************/
bool CNodeDetailsDlg::bOpenfile()
{
    if(m_eBus == J1939)
    {
        if(m_pGlobalObj->bJ1939Activated == false)
        {
            AfxMessageBox("J1939 is not Activated, activate it and then try.");
            return false;
        }
    }
    if (nullptr != m_pGlobalObj->pGetDocPtrOfFile(m_omStrCFile)) //If function editor is already open.
    {
        return true;
    }

    /*Getting the Protocol from the .c file*/
    // For File I/O
    CStdioFile o_File;

    // Open File
    BOOL bIsFileOpen = FALSE;
    bool bIsOldCppFileFormat = false;
    // Opening the file
    bIsFileOpen = o_File.Open(
                      m_omStrCFile, CFile::modeRead|CFile::typeText );
    if(bIsFileOpen != FALSE )
    {
        // read subsequent info from the file
        /* Read Database version number*/
        int nIndex = -1;
        CString strProtocolInfo;

        CString omstrBusName;

        omstrBusName.Empty();

        if(m_eBus == CAN)
        {
            // If CAN
            omstrBusName = "CAN";
        }
        else if(m_eBus == J1939)
        {
            // If J1939
            omstrBusName = "J1939";
        }
        else if(m_eBus == LIN)
        {
            // If LIN
            omstrBusName = "LIN";
        }

        // If Bus is selected
        if(omstrBusName.IsEmpty() == FALSE)
        {
            nIndex = -1;
            CString omstrProtocolValue;

            // Check if the PROTOCOL tag exists in the .c file
            while( nIndex == -1 && o_File.ReadString( omstrProtocolValue ))
            {
                // Checking if the .c file is of which Protocol
                nIndex = omstrProtocolValue.Find(PROTOCOL_TAB);
                if (-1 == omstrProtocolValue.Find(BUSMASTER_VERSION_TAG) && -1 != omstrProtocolValue.Find(VERSION_TAG))
                {
                    bIsOldCppFileFormat = bCheckIfOldCppFile(omstrProtocolValue);
                }
            }

            if(nIndex != -1)
            {
                int nPlaceLeftParanth = omstrProtocolValue.Find('[');
                int nPlaceRightParanth = omstrProtocolValue.Find(']');
                if(nPlaceLeftParanth != -1 && nPlaceRightParanth != -1)
                {
                    omstrProtocolValue = omstrProtocolValue.Mid(nPlaceLeftParanth+1, (nPlaceRightParanth - nPlaceLeftParanth -1));

                    // If CAN protocol is selected
                    if(omstrProtocolValue == "CAN" || omstrProtocolValue == "LIN")
                    {
                        if(m_eBus == J1939)
                        {
                            // If the .c file is not related to J1939
                            AfxMessageBox("File " + m_omStrCFile + " s not created for J1939.\r\nPlease open the .cpp file created for J1939.");
                            return false;
                        }
                    }

                    // If J1939 protocol is selected
                    if(omstrProtocolValue == "J1939" || omstrProtocolValue == "LIN")
                    {
                        // If CAN protocol is selected
                        if(m_eBus == CAN)
                        {
                            // If the .c file is not related to CAN
                            AfxMessageBox("File " + m_omStrCFile + " is not created for CAN.\r\nPlease open the .cpp file created for CAN.");
                            return false;
                        }
                    }

                    // If LIN protocol is selected
                    if(omstrProtocolValue == "CAN" || omstrProtocolValue == "J1939")
                    {
                        // If Lin protocol is slected
                        if(m_eBus == LIN)
                        {
                            // If the .c file is not related to LIN
                            AfxMessageBox("File " + m_omStrCFile + " is not created for LIN.\r\nPlease open the .cpp file created for LIN.");
                            return false;
                        }
                    }
                }

                // Closing the file opened
                o_File.Close();
            }
            else
            {
                o_File.SeekToBegin();

                CString omTemp1 = BUS_INCLUDE_HDR;
                // Preparing the header to be searched for in .c file
                // to get the Protocol name
                omTemp1.Replace("PLACE_HODLER_FOR_BUSNAME", omstrBusName);

                nIndex = -1;

                while( nIndex == -1 && o_File.ReadString( strProtocolInfo ))
                {
                    // Checking if the .c file is of which Protocol
                    nIndex = strProtocolInfo.Find(omTemp1);
                }

                // Closing the file opened
                o_File.Close();

                if (nIndex == -1)
                {
                    // If CAN protocol is selected
                    if(m_eBus == CAN)
                    {
                        // If the .cpp file is not related to CAN
                        AfxMessageBox("File " + m_omStrCFile + " is not created for CAN.\r\nPlease open the .cpp file created for CAN.");
                        return false;
                    }
                    // If J1939 protocol is selected
                    else if(m_eBus == J1939)
                    {
                        // If the .cpp file is not related to J1939
                        AfxMessageBox("File " + m_omStrCFile + " s not created for J1939.\r\nPlease open the .cpp file created for J1939.");
                        return false;
                    }
                    // If LIN protocol is selected
                    else if(m_eBus == LIN)
                    {
                        // If the .cpp file is not related to LIN
                        AfxMessageBox("File " + m_omStrCFile + " s not created for LIN.\r\nPlease open the .cpp file created for LIN.");
                        return false;
                    }
                }
            }
        }
    }

    if (!m_omStrCFile.IsEmpty())
    {
        m_pGlobalObj->bOpenFunctioneditorfile(m_omStrCFile, bIsOldCppFileFormat);
    }
    return true;
}
/*********************************************************************************/
/*  Function Name    :  bUpdateNodeInfoFile
/*  Input(s)         :  CString omStrCFile
/*  Output           :  -
/*  Functionality    : If <FilePath>.dll is entered in Node dialog then it associates
                        <FilePath>.cpp(or <FilePath>.c) to the node, and vice versa.
/*  Member of        :  CSimSysDetView
/*  Friend of        :      -
/*  Author(s)        :  Harika M
/*  Date Created     :  21.12.2005
/*  Modification     :  Robin G.K.
                        21.10.14, Moved this from CSimSysDetView to CNodeDetailsDlg.
                        Optimisied the functionality.                           */
/********************************************************************************/
void CNodeDetailsDlg::bUpdateNodeInfoFile()
{
    CString omStrFileName = m_omStrNodeFilePath;
    CSimSysTreeView* pSimSysTreeView =
        m_pSimSysMgr->podGetSimSysTreeView();
    CSimSysNodeInfo* pSimSysNodeInf =
        m_pSimSysMgr->pomGetSimSysNodeInfo();

    int nIndex = omStrFileName.ReverseFind('.');
    if( nIndex >= 0 )
    {
        omStrFileName = omStrFileName.Left(nIndex);

    }
    CString omStrDllFile = omStrFileName + defDOT_DLL;
    // file-attribute information
    _tfinddata_t fileinfo;
    if (_tfindfirst( omStrDllFile.GetBuffer(MAX_PATH), &fileinfo) != -1L)
    {
        m_psNodeStuctPtr->m_omStrDllName = omStrDllFile;
    }

    CString omStrCPPFile = omStrFileName + defDOT_CPP;
    if(_tfindfirst( omStrCPPFile.GetBuffer(MAX_PATH), &fileinfo) != -1L)
    {
        m_psNodeStuctPtr->RegisterFileToMonitor(std::string(m_psNodeStuctPtr->m_omStrCFileName),false);
        m_psNodeStuctPtr->m_omStrCFileName = omStrCPPFile;
        m_psNodeStuctPtr->RegisterFileToMonitor(std::string(m_psNodeStuctPtr->m_omStrCFileName),true);
    }

    CString omStrCFile = omStrFileName + defDOT_C;
    if(_tfindfirst( omStrCFile.GetBuffer(MAX_PATH), &fileinfo) != -1L)
    {
        m_psNodeStuctPtr->m_omStrCFileName = omStrCFile;
    }
}
