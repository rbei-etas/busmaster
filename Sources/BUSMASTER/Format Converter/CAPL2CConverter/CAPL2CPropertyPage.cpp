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
 * \file      ConverterDlg.cpp
 * \brief     Implementation file for CCAPL2CPropertyPage class
 * \author    Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CCAPL2CPropertyPage class
 */

// keyoptionDlg.cpp : implementation file
#include "CAPL2CConverter_stdafx.h"
#include "CAPL2CPropertyPage.h"

#include "LexerHandlers.hpp"
#include "ConstString.h"
#include "stdlib.h"
#include "EnvVarHandlerDlg.h"
#include "CAPL2CConverter_Resource.h"
#include "../../Utility/WaitIndicator.h"
#include "CAPL_C_Lexer.h"
////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#define defSTR_BROWSE_FILE_TITLE "Select %s"
#define defSTR_ADD_FILE_STATIC_TXT "Add %s(s)"

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
    //  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCAPL2CPropertyPage dialog

CCAPL2CPropertyPage::CCAPL2CPropertyPage(CWnd* pParent /*=NULL*/)
    : CPropertyPage(CCAPL2CPropertyPage::IDD)
    , m_bConvertDbc2Dbf(FALSE)
{
    //{{AFX_DATA_INIT(CCAPL2CPropertyPage)
    m_eBus = CAN;
    m_savedb = FALSE;
    m_pfGetConverter = NULL;
    m_hDLLModule = NULL;
    m_pouDBC2DBFConverter = NULL;
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    mDllHandle = nullptr;
}

void CCAPL2CPropertyPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCAPL2CPropertyPage)
    //DDX_Check(pDX, IDC_CHKB_OPTN, m_check);
    //DDX_Check(pDX, IDC_CHKB_SAVEDB, m_savedb);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_LIST_DBC_FILES, m_omDBCList);
    DDX_Control(pDX, IDC_COMBO_PROTOCOL, m_omComboBusType);
    DDX_Check(pDX, IDC_CHKB_SAVEDB, m_bConvertDbc2Dbf);
}

BEGIN_MESSAGE_MAP(CCAPL2CPropertyPage, CPropertyPage)
    //{{AFX_MSG_MAP(CCAPL2CPropertyPage)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_CBTN_BINPUT, OnBrowseSource)
    ON_BN_CLICKED(IDC_CBTN_BOUTPUT, OnBrowseDest)
    ON_BN_CLICKED(IDC_CBTN_LOG, OnShowLog)
    ON_BN_CLICKED(IDC_CBTN_CNVRT, OnConvert)
    ON_EN_CHANGE(IDC_EDIT_INPUT, OnChangeEditInput)
    //ON_BN_CLICKED(IDC_CHKB_OPTN, OnChkbOptn)
    ON_BN_CLICKED(IDC_CBTN_BCANOEDB, OnBrowseCANoeDb)
    ON_BN_CLICKED(IDC_CBTN_BBUSMASTERDB, OnBrowseBUSMASTERDb)
    ON_BN_CLICKED(IDC_CBTN_CANCEL, OnCancel)
    ON_BN_CLICKED(IDC_CHKB_SAVEDB, OnChkbSavedb)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_CBTN_BCANOEDB_DEL, OnBnCanoedbDel)
    ON_NOTIFY(NM_CLICK, IDC_LIST_DBC_FILES, OnNMClickListDbcFiles)
    ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_DBC_FILES, OnLvnItemActivateListDbcFiles)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DBC_FILES, OnLvnItemchangedListDbcFiles)
    ON_CBN_SELCHANGE(IDC_COMBO_PROTOCOL, OnCbnSelchangeComboBustype)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCAPL2CPropertyPage message handlers
/*
 * \req      RS_FLX_18
 * \design   DS_FLX_18
 * \codetag  CT_FLX_18
*/
BOOL CCAPL2CPropertyPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // TODO: Add extra initialization here
    CRect omRect;
    m_omDBCList.SetExtendedStyle(m_omDBCList.GetExtendedStyle() | LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

    //Needs to be made generic.
    m_omComboBusType.InsertString(0,"CAN");

    loadFrCapl2CConverter();
    if (nullptr != mFrCapl2CConverterModule)
    {
        m_omComboBusType.InsertString(1, "FlexRay");
    }
    m_omComboBusType.SetCurSel(0);

    //Set Input Database Text;
    SetAddFileStaticText();
    UpdateUI();
    return TRUE;  // return TRUE  unless you set the focus to a control
}
void CCAPL2CPropertyPage::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CPropertyPage::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCAPL2CPropertyPage::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CPropertyPage::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCAPL2CPropertyPage::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}



/*******************************************************************************
 Function Name    : OnBrowseSource
 Input(s)         :     -
 Output           :     -
 Functionality    : This will pop-up a comman dialog box to select the input
                    CAN file.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 27.04.2004
 Modifications    :
*******************************************************************************/

void CCAPL2CPropertyPage::OnBrowseSource()
{
    CString omStrPath;
    int nIndex = -1;
    CFileDialog cfd(TRUE,"can","*can",OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST|
                    OFN_PATHMUSTEXIST|OFN_EXTENSIONDIFFERENT,_("CAPL Files(*.can)|*.can||"),this);

    cfd.m_ofn.lpstrTitle = _("Select CAPL (.can) File");


    GetDlgItemText( IDC_EDIT_INPUT,omStrPath );
    omStrPath.TrimLeft();
    omStrPath.TrimRight();
    if(omStrPath.IsEmpty())
    {
        omStrPath = AfxGetApp()->GetProfileString(_("Files"),_("CAPL file"),"");
    }
    if( (nIndex = omStrPath.ReverseFind('\\') )!= -1)
    {
        omStrPath = omStrPath.Left(nIndex);
    }

    cfd.m_ofn.lpstrInitialDir = omStrPath;

    if(cfd.DoModal()==IDOK)
    {
        CString strExtName  = cfd.GetFileExt();
        strExtName.MakeLower();
        if(strExtName == "can")
        {
            omStrPath = cfd.GetPathName();
            INT nIndex = omStrPath.ReverseFind('.');
            if ( nIndex >= 0)
            {
                CString omStrOutputFile = omStrPath.Left(nIndex);
                omStrOutputFile += ".cpp";
                SetDlgItemText(IDC_EDIT_OUTPUT,omStrOutputFile);
            }

            SetDlgItemText(IDC_EDIT_INPUT, omStrPath );
            SetDlgItemText(IDC_STAT_RESULT2, "");
        }
        else
        {
            AfxMessageBox(defSTR_ERR_INVALID_FILE ,MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);
        }
    }

}
/*******************************************************************************
 Function Name    : OnBrowseDest
 Input(s)         :     -
 Output           :     -
 Functionality    : This will pop-up a comman dialog box to select the output
                    file.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 27.04.2004
 Modifications    :
*******************************************************************************/
void CCAPL2CPropertyPage::OnBrowseDest()
{
    CFileDialog cfd(TRUE,"cpp",NULL,OFN_OVERWRITEPROMPT|
                    OFN_HIDEREADONLY,_("BUSMASTER  Files(*.cpp)|*.cpp||"),this);

    cfd.m_ofn.lpstrTitle = _("Select BUSMASTER File");

    CString omStrPath;
    int nIndex;

    GetDlgItemText( IDC_EDIT_OUTPUT,omStrPath );
    omStrPath.TrimLeft();
    omStrPath.TrimRight();
    if(omStrPath.IsEmpty())
    {
        omStrPath = AfxGetApp()->GetProfileString(_("Files"),_("CAPL file"),"");
    }
    if( (nIndex = omStrPath.ReverseFind('\\') )!= -1)
    {
        omStrPath = omStrPath.Left(nIndex);
    }

    cfd.m_ofn.lpstrInitialDir = omStrPath;

    if(cfd.DoModal()==IDOK)
    {
        omStrPath = cfd.GetPathName();
        SetDlgItemText(IDC_EDIT_OUTPUT,omStrPath);
        SetDlgItemText(IDC_STAT_RESULT2, "");
    }
}
/*******************************************************************************
 Function Name    : OnShowLog
 Input(s)         :     -
 Output           :     -
 Functionality    : This will show the log file of latest converted CAN file.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 27.04.2004
 Modifications    :
*******************************************************************************/
void CCAPL2CPropertyPage::OnShowLog()
{
    CString cs ;

    cs = getenv("windir");

    cs = cs + "\\notepad.exe " + m_omLogFile;
    WinExec(cs,SW_SHOW);
}

void CCAPL2CPropertyPage::OnCancel()
{
    CPropertyPage::OnCancel();
}

void CCAPL2CPropertyPage::OnChangeEditInput()
{
    //It will check whether the input edit box is blank or not.If nothing is
    //there "Convert" button will be disabled otherwise enabled.
    CString omStrPath;
    GetDlgItemText( IDC_EDIT_INPUT,omStrPath );

    if( omStrPath.GetLength() > 0 )
    {
        GetDlgItem( IDC_CBTN_CNVRT )->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem( IDC_CBTN_CNVRT )->EnableWindow(FALSE);
    }

}

/*void CCAPL2CPropertyPage::OnChkbOptn()
{
    //this will activate if check box is checked
    UpdateData();
    if( m_check )
    {
        cFlagH = 1;
        GetDlgItem( IDC_EDIT_CANOEDB )->EnableWindow(TRUE);
        GetDlgItem( IDC_CBTN_BCANOEDB )->EnableWindow(TRUE);
        GetDlgItem( IDC_CHKB_SAVEDB )->EnableWindow(TRUE);
    }
    else
    {
        cFlagH = 0;

        SetDlgItemText(IDC_EDIT_CANOEDB," " );
        SetDlgItemText(IDC_EDIT_BUSMASTERDB," " );
        //m_savedb = FALSE;
        GetDlgItem( IDC_EDIT_CANOEDB )->EnableWindow(FALSE);
        GetDlgItem( IDC_CBTN_BCANOEDB )->EnableWindow(FALSE);
        GetDlgItem( IDC_CHKB_SAVEDB )->EnableWindow(FALSE);
        GetDlgItem( IDC_EDIT_BUSMASTERDB )->EnableWindow(FALSE);
        GetDlgItem( IDC_CBTN_BBUSMASTERDB )->EnableWindow(FALSE);

    }
}
*/
/*******************************************************************************
 Function Name    : OnBrowseCANoeDb
 Input(s)         :     -
 Output           :     -
 Functionality    : This will pop-up a comman dialog box to select the input
                    CANoe database file.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 15.05.2004
 Modifications    :
*******************************************************************************/
void CCAPL2CPropertyPage::OnBrowseCANoeDb()
{
    CFileDialog omDBCFileBrowser(TRUE,m_strExtn.c_str(),NULL,OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST|
                                 OFN_PATHMUSTEXIST|OFN_ALLOWMULTISELECT,m_strFileFiler.c_str(),this);

    char chTitle[MAX_PATH] = {'\0'};
    sprintf(chTitle, defSTR_BROWSE_FILE_TITLE, g_strDatabaseFile[m_eBus].c_str());


    omDBCFileBrowser.m_ofn.lpstrTitle = chTitle;

    CString omStrDBFFile;
    if(omDBCFileBrowser.DoModal()==IDOK)
    {
        POSITION pos = omDBCFileBrowser.GetStartPosition();
        while(pos != NULL)
        {

            CString omstrDBCFile = omDBCFileBrowser.GetNextPathName(pos);
            int nIndex = m_omDBCList.GetItemCount();
            if(m_eBus == FLEXRAY && nIndex>0)
            {
                MessageBox(_("Only one FIBEX file is supported."), _("Warning"), MB_OK|MB_ICONWARNING);
                return;
            }
            // Insert at the end
            m_omDBCList.InsertItem( nIndex, omstrDBCFile);

            if(CAN == m_eBus)
            {
                if( omstrDBCFile.Find(".dbc",0) != -1)
                {
                    omStrDBFFile = omstrDBCFile.Left( omstrDBCFile.GetLength() - 4);
                    omStrDBFFile += _T(".dbf");
                    m_omDBCList.SetItemText(nIndex, 1, omStrDBFFile);
                }
                else if( omstrDBCFile.Find(".DBC",0) != -1)
                {
                    omStrDBFFile = omstrDBCFile.Left( omstrDBCFile.GetLength() - 4);
                    omStrDBFFile += _T(".dbf");
                    m_omDBCList.SetItemText(nIndex, 1, omStrDBFFile);
                }
                else
                {
                    MessageBox(_("Invalid CANoe Database File"), _("Error"), MB_OK|MB_ICONERROR);
                    m_omDBCList.DeleteItem(nIndex);
                }
            }
            else if(FLEXRAY == m_eBus)
            {
                omstrDBCFile.MakeLower();
                if( omstrDBCFile.Find(".xml",0) == -1)
                {
                    MessageBox(_("Invalid FIBEX File"), _("Error"), MB_OK|MB_ICONERROR);
                    m_omDBCList.DeleteItem(nIndex);
                }

            }
        }
    }
}

/*******************************************************************************
 Function Name    : OnBrowseBUSMASTERDb
 Input(s)         :     -
 Output           :     -
 Functionality    : This will pop-up a comman dialog box to select the input
                    BUSMASTER database file.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 15.05.2004
 Modifications    :
*******************************************************************************/

void CCAPL2CPropertyPage::OnBrowseBUSMASTERDb()
{
    CFileDialog  omStrDBFFileDialog( TRUE,"dbf",NULL,OFN_OVERWRITEPROMPT|
                                     OFN_PATHMUSTEXIST,_("BUSMASTER Database Files(*.dbf)|*.dbf||"),this);

    omStrDBFFileDialog.m_ofn.lpstrTitle = _("Select BUSMASTER Database File");

    POSITION pos = m_omDBCList.GetFirstSelectedItemPosition();
    int nItem = m_omDBCList.GetNextSelectedItem(pos);

    CString omStrPath = m_omDBCList.GetItemText(nItem, 1);
    omStrPath.TrimLeft();
    omStrPath.TrimRight();
    CString omStrFileName = _T("");
    if( omStrPath.Find(".dbf",0) != -1)
    {
        INT nIndex = omStrPath.ReverseFind('\\');

        if(nIndex >= 0)
        {
            omStrFileName = omStrPath.Right(omStrPath.GetLength() - nIndex - 1);
            strcpy(omStrDBFFileDialog.m_ofn.lpstrFile, omStrFileName.GetBuffer(MAX_PATH));
        }

        omStrPath = omStrPath.Left( omStrPath.GetLength() - 4);
        omStrDBFFileDialog.m_ofn.lpstrInitialDir = omStrPath;
        if(omStrDBFFileDialog.DoModal()==IDOK)
        {
            omStrPath = omStrDBFFileDialog.GetPathName();
            m_omDBCList.SetItemText(nItem, 1, omStrPath);
        }
    }
}

void CCAPL2CPropertyPage::OnChkbSavedb()
{
    UpdateData();
    if( m_savedb )
    {
        cFlagLog = 2;//to check whether to save converted database or not
    }
    else
    {
        cFlagLog = 0;
    }

}

void CCAPL2CPropertyPage::SaveSettings()
{
    //this function will save the settings in the registry.
    CString strSection       = _("Files");
    CString strStringItem    = _("CAPL File");

    CWinApp* pApp = AfxGetApp();
    CString omStrCAPLName;
    CWnd* pCAPL = GetDlgItem(IDC_EDIT_INPUT);
    pCAPL->GetWindowText(omStrCAPLName);
    pApp->WriteProfileString(strSection, strStringItem,omStrCAPLName);
}



void CCAPL2CPropertyPage::OnBnCanoedbDel()
{
    // TODO: Add your control notification handler code here
    POSITION pos = m_omDBCList.GetFirstSelectedItemPosition();
    if(pos != NULL)
    {
        int nItem = m_omDBCList.GetNextSelectedItem(pos);
        m_omDBCList.DeleteItem(nItem);
    }

}

/*
 * \req      RS_FLX_9
 * \design   DS_FLX_9
 * \codetag  CT_FLX_9
 */
void CCAPL2CPropertyPage::OnConvert()
{
    CString omStrCFileName , omStrCAPLFileName ;    //to store the destination file and source file
    CFile omCAPLFile, omCFile, omLogFile;           //to check files
    CString omExtension;                                   //to store the extension

    BOOL bValidInputs = TRUE;

    for( int m = 0; m < 254; m++)
    {
        //initialize array with -1.
        acAltKey[m]= -1;
    }

    GetDlgItem(IDC_CBTN_LOG)->EnableWindow(FALSE);
    GetDlgItemText( IDC_EDIT_INPUT,omStrCAPLFileName );//omStrCAPLFileName will have input file path
    GetDlgItemText( IDC_EDIT_OUTPUT,omStrCFileName );//omStrCFileName will have output file path

    omStrCAPLFileName.TrimRight();
    omExtension = omStrCAPLFileName.Right(4);
    omExtension.MakeLower();

    if( omExtension != ".can" )
    {
        //if file doesn't exist or in write mode
        MessageBox( defSTR_Warning5,MB_ICONWARNING|MB_OK );
        SetDlgItemText( IDC_EDIT_INPUT,"" );
        GetDlgItem( IDC_EDIT_INPUT)->SetFocus();
        bValidInputs = FALSE;//to avoid conversion
    }

    else if( omCAPLFile.Open( omStrCAPLFileName,CFile::modeRead ) == NULL )
    {
        //if file doesn't exist or in write mode
        MessageBox( defSTR_Warning2,MB_ICONWARNING|MB_OK );
        SetDlgItemText( IDC_EDIT_INPUT,"" );
        GetDlgItem( IDC_EDIT_INPUT)->SetFocus();
        bValidInputs = FALSE;//to avoid conversion
    }
    else
    {
        //close omCAPLFile file
        omCAPLFile.Close();

    }

    if(omStrCFileName == "")
    {
        //for default output file name
        omStrCFileName = omStrCAPLFileName.Left( omStrCAPLFileName.GetLength() - 4 );
        omStrCFileName += ".cpp";
        SetDlgItemText( IDC_EDIT_OUTPUT,omStrCFileName);
    }
    else
    {
        omStrCFileName.TrimRight();
        int nIndex = omStrCFileName.ReverseFind('.');
        omExtension = omStrCFileName.Right(omStrCFileName.GetLength()-nIndex);
        omExtension.MakeLower();

        if ( omExtension != ".cpp")
        {
            MessageBox(defSTR_Warning1,MB_ICONWARNING|MB_OK );
            SetDlgItemText( IDC_EDIT_OUTPUT,"" );
            GetDlgItem( IDC_EDIT_OUTPUT)->SetFocus();
            bValidInputs = FALSE;//to avoid conversion
        }
    }
    if(TRUE == bValidInputs)
    {
        if(omCFile.Open(omStrCFileName,CFile::modeCreate|CFile::modeWrite)==NULL)
        {
            //if output file is in read mode
            MessageBox( defSTR_Warning3,MB_ICONWARNING|MB_OK );
            SetDlgItemText( IDC_EDIT_OUTPUT,"" );
            GetDlgItem( IDC_EDIT_OUTPUT)->SetFocus();
            bValidInputs = FALSE;//to avoid conversion
        }
        else
        {
            //close omCFile file
            omCFile.Close();

        }
    }
    m_omLogFile = omStrCFileName;
    m_omLogFile = omStrCFileName.Left( omStrCFileName.GetLength() -  4 );
    m_omLogFile +=  defSTR_LogName;//default output file name
    if(omLogFile.Open(m_omLogFile,CFile::modeCreate|CFile::modeWrite)== NULL)
    {
        MessageBox( defSTR_Warning4,MB_ICONWARNING|MB_OK );
        bValidInputs = FALSE;//to avoid conversion
    }
    else
    {
        omLogFile.Close();

    }
    CWaitIndicator ouWaitIndicator;

    if( TRUE == bValidInputs )
    {
        INT_PTR nCount = m_omDBCList.GetItemCount();
        CStringArray omStrDbcFileArray;
        if( 0 == nCount )
        {
            INT nChoice = MessageBox(defSTR_Warning9, MB_YESNO|MB_ICONWARNING);
            if( IDNO == nChoice )
            {
                return;
            }
            ouWaitIndicator.DisplayWindow ( _ ( "" ), this );
        }
        else
        {
            ouWaitIndicator.DisplayWindow (_("Loading The Database File(s)... Please Wait"), this);
        }

        g_ouGlobalVariables.vInitialise();
        m_pouDBC2DBFConverter = nullptr;
        CString omStrResult;
        ouWaitIndicator.SetWindowText(_("CAPL Conversion is Going on... Please Wait"));

        if(CAN==m_eBus)
        {
            //LOad DBC Files
            m_hDLLModule = LoadLibrary("DBC2DBFConverterLibrary.dll");
            m_pfGetConverter = (GETCONVERTER)GetProcAddress(m_hDLLModule, "GetDBCConverter");
            if( NULL != m_pfGetConverter)
            {
                m_pfGetConverter(m_pouDBC2DBFConverter, CAN);
                if( NULL == m_pouDBC2DBFConverter )
                {
                    MessageBox(defSTR_Warning8, MB_OK|MB_ICONWARNING);
                }
            }
            if(0!=nCount)
            {
                for( int i = 0; i < nCount; i++)
                {
                    CString omStrDBCFile = m_omDBCList.GetItemText(i, 0).GetBuffer(MAX_PATH);
                    omStrDbcFileArray.Add(m_omDBCList.GetItemText(i, 0));
                    if ( nullptr != m_pouDBC2DBFConverter )
                    {
                        m_pouDBC2DBFConverter->LoadDBCFile(omStrDBCFile);
                    }
                }
            }
            nConvert(m_eBus, omStrCAPLFileName, omStrCFileName, m_omLogFile, omStrDbcFileArray, m_pouDBC2DBFConverter, omStrResult);
        }
        else if(m_eBus == FLEXRAY)
        {
            loadFrCapl2CConverter();
            if (nullptr != mFrCapl2CConverterFun)
            {
                std::list<std::string> fibexFiles;
                INT_PTR nCount = m_omDBCList.GetItemCount();
                for (int i = 0; i < nCount; i++)
                {
                    std::string omStrDBCFile = m_omDBCList.GetItemText(i, 0).GetBuffer(MAX_PATH);
                    fibexFiles.push_back((LPCSTR)m_omDBCList.GetItemText(i, 0));
                }
                std::string result;
                mFrCapl2CConverterFun((LPCSTR)omStrCAPLFileName, (LPCSTR)omStrCFileName, (LPCSTR)m_omLogFile, fibexFiles, result);
                omStrResult = result.c_str();
            }
            else
            {
                //TODO::
            }
        }

        UpdateData();


        if(m_eBus == CAN)
        {
            if(m_bConvertDbc2Dbf == TRUE)
            {
                INT_PTR nCount = m_omDBCList.GetItemCount();
                ouWaitIndicator.SetWindowText(_("Database Conversion is Going on... Please Wait"));
                for( int i = 0; i < nCount; i++)
                {
                    CString omStrDBCFile = m_omDBCList.GetItemText(i, 0).GetBuffer(MAX_PATH);
                    if ( nullptr != m_pouDBC2DBFConverter )
                    {
                        m_pouDBC2DBFConverter->ClearMsgList();
                        m_pouDBC2DBFConverter->LoadDBCFile(omStrDBCFile);
                        m_pouDBC2DBFConverter->ConvertFile(m_omDBCList.GetItemText(i, 1).GetBuffer(MAX_PATH));
                    }
                }
            }
            if(m_pouDBC2DBFConverter != NULL)
            {
                delete m_pouDBC2DBFConverter;
            }
        }
        ouWaitIndicator.SetWindowText(_("Conversion Completed Successfully"));
        Sleep(520);
        ouWaitIndicator.CloseWindow();

        CWnd* pResultWnd = GetDlgItem(IDC_STAT_RESULT2);
        if( pResultWnd)
        {
            //to display the result
            pResultWnd->SetWindowText(omStrResult);
            SaveSettings();
        }
        ouUnSptdKey.vClearArray();
        GetDlgItem( IDC_CBTN_LOG )->EnableWindow(TRUE);

    }
}
void CCAPL2CPropertyPage::OnNMClickListDbcFiles(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here

    *pResult = 0;
}

void CCAPL2CPropertyPage::OnLvnItemActivateListDbcFiles(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: Add your control notification handler code here

    *pResult = 0;
}

void CCAPL2CPropertyPage::OnLvnItemchangedListDbcFiles(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    if (pNMListView->uChanged != LVIF_TEXT)
    {
        // Selected & Focused
        if (pNMListView->uNewState == (LVIS_FOCUSED | LVIS_SELECTED))
        {
            // Update selection
            //m_nLogIndexSel = pNMListView->iItem;
            // Update selected Log file details
            GetDlgItem(IDC_CBTN_BCANOEDB_DEL)->EnableWindow(TRUE);
            GetDlgItem(IDC_CBTN_BBUSMASTERDB)->EnableWindow(TRUE);
        }
        else
        {
            GetDlgItem(IDC_CBTN_BCANOEDB_DEL)->EnableWindow(FALSE);
            GetDlgItem(IDC_CBTN_BBUSMASTERDB)->EnableWindow(FALSE);
        }
    }

    *pResult = 0;
}
/**
 * \brief On ComboBox Selection Change Combo Bus Type
 *
 * Message handler for selection change in bus type combo box.
 */
void CCAPL2CPropertyPage::OnCbnSelchangeComboBustype()
{
    CString omStrBus;
    m_omComboBusType.GetWindowText(omStrBus);
    if( omStrBus == "CAN")
    {
        m_eBus = CAN;
    }
    else if ( omStrBus == "FlexRay")
    {
        m_eBus = FLEXRAY;
    }
    SetAddFileStaticText();
    UpdateUI();
    UpdateData(FALSE);
}

void CCAPL2CPropertyPage::UpdateUI()
{
    int nColumnCount = m_omDBCList.GetHeaderCtrl()->GetItemCount();
    m_omDBCList.DeleteAllItems();
    // Delete all of the columns.
    for (int i=0; i < nColumnCount; i++)
    {
        m_omDBCList.DeleteColumn(0);
    }
    CRect omRect;
    m_omDBCList.GetWindowRect( &omRect );
    if(m_eBus == CAN)
    {
        GetDlgItem( IDC_CHKB_SAVEDB )->ShowWindow(TRUE);
        GetDlgItem(IDC_CBTN_BBUSMASTERDB)->ShowWindow(TRUE);
        GetDlgItem(IDC_CBTN_BCANOEDB)->SetWindowText("Add DBC File");
        GetDlgItem(IDC_CBTN_BCANOEDB_DEL)->SetWindowText("Delete DBC File");
        m_omDBCList.InsertColumn(0, _T(_("DBC File Path")));
        m_omDBCList.InsertColumn(1, _T(_("DBF File Path")));
        m_omDBCList.SetColumnWidth( 0, omRect.Width()/2 );
        m_omDBCList.SetColumnWidth( 1, omRect.Width()/2 );
        m_strFileFiler = "CANoe Database Files(*.dbc)|*.dbc||";
        m_strExtn = ".dbc";
    }
    else if(m_eBus == FLEXRAY)
    {
        GetDlgItem( IDC_CHKB_SAVEDB )->ShowWindow(FALSE);
        GetDlgItem(IDC_CBTN_BBUSMASTERDB)->ShowWindow(FALSE);
        GetDlgItem(IDC_CBTN_BCANOEDB)->SetWindowText("Add FIBEX File");
        GetDlgItem(IDC_CBTN_BCANOEDB_DEL)->SetWindowText("Delete FIBEX File");
        m_omDBCList.InsertColumn(0, _T(_("Fibex File Path")));
        m_omDBCList.SetColumnWidth( 0, omRect.Width());
        m_strFileFiler = "FIBEX Files (*.xml)|*.xml||";
        m_strExtn = ".xml";
    }
}
/**************************************************************************************
    Function Name    :  SetAddFileStaticText
    Input(s)         :  void
    Output           :  void
    Functionality    :  Sets Add File Static Text
    Member of        :  CCAPL2CPropertyPage
    Author(s)        :  Robin G.K.
    Date Created     :  28.10.2015
    Requirement ID   :
***************************************************************************************/
void CCAPL2CPropertyPage::SetAddFileStaticText()
{
    CWnd* pWnd = GetDlgItem(IDC_STATIC_ADD_FILE);
    if(nullptr != pWnd)
    {
        char chText[MAX_PATH] = {'\0'};
        sprintf(chText, defSTR_ADD_FILE_STATIC_TXT, g_strDatabaseFile[m_eBus].c_str());
        pWnd->SetWindowTextA(chText);
    }
}

/**
* \brief
* \param[in]
* \return        void
* \authors       Venkatanarayana Makam
* \date
*/

int CCAPL2CPropertyPage::nConvert(ETYPE_BUS eBus, CString omStrCAPLFile, CString omStrCFile, CString omStrLogFile, CStringArray& omStrDbcFileArray, CDBCConverterBase* ouDBC2DBFConverter, CString& omStrResult)
{
    m_eBus = eBus;
    g_ouGlobalVariables.m_eBus = m_eBus;
    g_bBusConnectGenerated = false;
    g_bGlobalVariable = false;
    g_MsgInitialize.clear();

    if (NULL == ouDBC2DBFConverter && m_eBus == CAN)
    {
        omStrResult = "Loading Dbc Converter Failed";
        return S_FALSE;
    }

    g_ouGlobalVariables.g_ouDBC2DBFConverter = ouDBC2DBFConverter;
    cIndex2 = 0;
    bPreStart = FALSE;

    //FirstParse Starts Here::Unsupported keys anf function definitions are processed
    CString omStrTempFile = omStrCFile + ".temp";
    yyin = fopen(omStrCAPLFile, "r");
    yyout = fopen(omStrTempFile, "w+");
    if (NULL == yyin || NULL == yyout)
    {
        return S_FALSE;
    }

    BEGIN(FirstParse); //start first parse
    yylex();

    pFileLog = fopen(omStrLogFile, "w");
    if (NULL != pFileLog)
    {
        fprintf(pFileLog, defSTR_InputFileInfo, omStrCAPLFile);//writing to log file
        fprintf(pFileLog, defSTR_OutputFileInfo, omStrCFile);
    }

    if (ouUnSptdKey.nGetSize() > 0)
    {
        //if there is any unsuppoted key then
        CEnvVarHandlerDlg d;
        d.DoModal();
    }
    CString omTemp = "";

    //SecondParse Starts Here::
    fseek(yyin, 0, SEEK_SET);
    BEGIN(SecondParse);
    yylex();
    if (g_ouGlobalVariables.omStrLastEvent.GetLength() > 0)
    {
        if (g_ouGlobalVariables.omStrLastEvent == defSTR_GlobalVar)
        {
            omTemp = defSTR_UsingGlobalVarNameSpace;
            omTemp += defSTR_GlobalVarEnd;
        }
        else
        {
            omTemp = defSTR_EndComment;
        }
        omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
        fprintf(yyout, "\n%s%s\n", (LPCSTR)omTemp, (LPCSTR)g_ouGlobalVariables.omStrLastEvent);
        g_ouGlobalVariables.omStrLastEvent.Empty();
    }

    fclose(yyin);
    fclose(yyout);

    //ThirdParse Starts Here::
    yyin = fopen(omStrTempFile, "r");
    yyout = fopen(omStrCFile, "w+");
    if (NULL == yyin || NULL == yyout)
    {
        return S_FALSE;
    }

    BEGIN(ThirdParse);
    yylex();
    fclose(yyin);
    fclose(yyout);
    //Fourth Phase Starts Here::
    yyin = fopen(omStrCFile, "r");
    yyout = fopen(omStrTempFile, "w+");
    if (NULL == yyin || NULL == yyout)
    {
        return S_FALSE;
    }

    BEGIN(FourthParse);
    yylex();
    fclose(yyin);
    fclose(yyout);

    //Fifth Phase Starts Here::
    yyin = fopen(omStrTempFile, "r");
    yyout = fopen(omStrCFile, "w+");
    if (NULL == yyin || NULL == yyout)
    {
        return S_FALSE;
    }

    BEGIN(FifthParse);
    yylex();

    fclose(yyin);
    fclose(yyout);
    DeleteFile(omStrTempFile);

    //Include Headers,Wrapper,Utility Function definition, Function Declarations
    CString omFun;
    CString omStrTemp;
    INT_PTR nCount = g_ouGlobalVariables.g_omStrFunCalls.GetSize();
    for (INT_PTR i = 0; i < nCount; i++)
    {
        omStrTemp = g_ouGlobalVariables.g_omStrFunCalls.GetAt(i);
        bAddExport(omStrTemp);
        omFun += omStrTemp + "\n";
    }
    CString omWrapperFun;
    nCount = g_ouGlobalVariables.g_omStrFunWrappers.GetSize();
    for (INT_PTR i = 0; i < nCount; i++)
    {
        omWrapperFun += g_ouGlobalVariables.g_omStrFunWrappers.GetAt(i) + "\n";
    }

    std::fstream filestr;
    filestr.open(omStrCFile.GetBuffer(MAX_PATH), fstream::in | fstream::out);
    INT nDBCCount = omStrDbcFileArray.GetSize();
    std::string str((std::istreambuf_iterator<char>(filestr)),
                    std::istreambuf_iterator<char>());
    filestr.seekp(0);

    filestr << COPYWRITE_INFORMATION;
    filestr << C_FILE_VERSION;
    filestr << BUSMASTER_VERSION;
    omTemp = PROTOCOL;

    if (eBus == CAN)
    {
        omTemp.Replace(defSTR_PlaceHolderForBusname, "CAN");
    }

    filestr << omTemp;

    filestr << endl << endl;

    omTemp = defSTR_StartIncludeHeader;
    omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
    filestr << omTemp;
    filestr << "#include <windows.h>\n";
    omTemp = defSTR_ProtocolIncludeHeader;
    if (eBus == CAN)
    {
        omTemp.Replace(defSTR_PlaceHolderForBusname, "CAN");
    }

    filestr << omTemp;

    omTemp = defSTR_EndIncludeHeader;
    omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
    filestr << omTemp << endl;//footer for include header

    omTemp = defSTR_FuncProHeader;
    omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
    filestr << omTemp;
    filestr.write(omFun.GetBuffer(MAX_PATH), omFun.GetLength());
    if (FALSE == bPreStart)
    {
        filestr << defUtils_PrestartDec;
    }

    if (CAN == m_eBus && g_bBusConnectGenerated == false && g_MsgInitialize.empty() == false)
    {
        filestr << defSTR_OnBusConnectDecl;
    }

    omTemp = defSTR_FuncProFooter;
    omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
    filestr << omTemp << endl;
    filestr << "\n";

    omTemp = defSTR_FuncProWrapperHeader;
    omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
    filestr << omTemp;
    filestr.write(omWrapperFun.GetBuffer(MAX_PATH), omWrapperFun.GetLength());
    omTemp = defSTR_FuncProWrapperFooter;
    omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
    filestr << omTemp << endl;


    filestr << defSTR_RXMessageDec << defSTR_TXMessageDec;
    filestr << defSSTR_TYPEDEFFOR << defSSTR_TYPEDEFSWITCH << defSSTR_TYPEDEFIF << endl;
    filestr << str;

    if (FALSE == bPreStart)
    {
        omTemp = defUtils_StartComment;
        omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
        filestr << omTemp;
        filestr << defUtils_PrestartDef << endl << '}';
        omTemp = defUtils_EndComment;
        omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[eBus].c_str());
        filestr << omTemp;
    }


    if (CAN == m_eBus && g_bBusConnectGenerated == false && g_MsgInitialize.empty() == false)
    {
        // Generating Bus Connect if Msg Handlers are initialized
        std::string omBusConnect = "";

        CString omTemp = "";
        omTemp = defSTR_Header;
        omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[m_eBus].c_str());

        omBusConnect += omTemp + defSTR_OnBusConnectHeder;
        omBusConnect += defSTR_OnBusConnect + g_MsgInitialize;

        omTemp = defSTR_Footer;
        omTemp.Replace(defSTR_PlaceHolderForBusname, strBUS_TYPE[m_eBus].c_str());
        omTemp += defSTR_OnBusConnectHeder;

        omBusConnect += omTemp;

        filestr << omBusConnect << endl;
    }
    fclose(yyin);

    //Result File Generation
    vWriteLogFile();
    if (NULL != pFileLog)
    {
        fclose(pFileLog);
    }

    //Return Value Updation
    if ((g_ouGlobalVariables.g_ouErrorStrings.size() == 0) && g_ouGlobalVariables.g_ouWarningStrings.size() == 0)
    {
        omStrResult.Format("Conversion Completed Successfully");
    }
    else
    {
        omStrResult.Format("Conversion is Completed With  %d Error(s) and %d Warning(s)", g_ouGlobalVariables.g_ouErrorStrings.size(), g_ouGlobalVariables.g_ouWarningStrings.size());
    }
    return 0;

}

int CCAPL2CPropertyPage::loadFrCapl2CConverter()
{
    if (nullptr == mFrCapl2CConverterFun)
    {
        mFrCapl2CConverterModule = LoadLibrary("CAPL2CConverterFlexRay.dll");
        if (mFrCapl2CConverterModule != nullptr)
        {
            mFrCapl2CConverterFun = (CONVERTFLEXRAYCAPLFILES)GetProcAddress(mFrCapl2CConverterModule, "ConvertFlexRayCaplFiles");
            return 1;
        }
    }
    return 0;
}