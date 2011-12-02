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
 * \file      CANDBConverterDlg.cpp
 * \brief     Implementation file for the CAboutDlg class.
 * \author    RBIN/EBS1 - Padmaja A
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for the CAboutDlg class.
 */
/**
* \file       CANDBConverterDlg.cpp
* \brief      Implementation file for the CCANDBConverterDlg class.
* \authors    RBIN/EBS1 - Mahesh B S
* \date       17.11.2004 Created
* \copyright  Copyright &copy; 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/
#include "StdAfx.h"
#include "App.h"
#include "ConverterDlg.h"

#include "Converter.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
/**
* \brief      CAboutDlg constructor
* \param[in]  None
* \param[out] None
* \return     None
* \authors
* \date
*/
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}
/**
* \brief      DoDataExchange Function
* \param[in]  CDataExchange*
* \param[out] None
* \return     void
* \authors
* \date
*/
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
// CCANDBConverterDlg dialog
/**
* \brief      CCANDBConverterDlg Constructor
* \param[in]  CDataExchange*
* \param[out] None
* \return     void
* \authors
* \date
*/
CCANDBConverterDlg::CCANDBConverterDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CCANDBConverterDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCANDBConverterDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
/**
* \brief      DoDataExchange Function
* \param[in]  CDataExchange*
* \param[out] None
* \return     void
* \authors
* \date
*/
void CCANDBConverterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCANDBConverterDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCANDBConverterDlg, CDialog)
    //{{AFX_MSG_MAP(CCANDBConverterDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_CANOE_BROWSE_BTN, OnCanoeBrowseBtn)
    ON_BN_CLICKED(IDC_CANMON_BROWSE_BTN, OnCanmonBrowseBtn)
    ON_BN_CLICKED(IDC_CONVERT_BTN, OnConvertBtn)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCANDBConverterDlg message handlers
/**
* \brief      OnInitDialog Function, Called once when the dialog box is created
* \param[in]  None
* \param[out] None
* \return     BOOL, TRUE if dialog has to create otherwise FALSE
* \authors
* \date
*/
BOOL CCANDBConverterDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

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
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // TODO: Add extra initialization here

    SetDlgItemText(IDC_RESULT_STATIC,"Choose files for conversion");
    return TRUE;  // return TRUE  unless you set the focus to a control
}
/**
* \brief      OnSysCommand Function used to handle the ON_WM_SYSCOMMAND
* \param[in]  UINT, ID of the command
              LPARAM, lparam of the message
* \param[out] None
* \return     void
* \authors
* \date
*/
void CCANDBConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

/**
* \brief      OnPaint Message to handle WM_PAINT message
* \param[in]  void
* \param[out]
* \return     void
* \authors
* \date
*/
void CCANDBConverterDlg::OnPaint()
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
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.

/**
* \brief      OnQueryDragIcon to return the icon of the Dialog
* \param[in]  None
* \param[out] None
* \return     HCURSOR
* \authors
* \date
*/
HCURSOR CCANDBConverterDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

/**
* \brief      OnCanoeBrowseBtn is the Handler of ON_BN_CLICKED message for IDC_CANOE_BROWSE_BTN
* \param[in]  None
* \param[out] None
* \return     void
* \authors
* \date
*/
void CCANDBConverterDlg::OnCanoeBrowseBtn()
{
    // Create a file open dlg to select the output file
    CFileDialog fileDlg(FALSE,"dbc",NULL,OFN_OVERWRITEPROMPT,"Output Database Files(*.dbc)|*.dbc||",this);
    fileDlg.m_ofn.lpstrTitle = "Select output database file"; // set the title of dlg

    // if OK clicked, display the path name in the edit box
    if(fileDlg.DoModal() == IDOK)
    {
        SetDlgItemText(IDC_CANOE_EDIT,fileDlg.GetPathName());
    }
}

/**
* \brief      OnCanmonBrowseBtn is the Handler of ON_BN_CLICKED message for IDC_CANMON_BROWSE_BTN
* \param[in]  None
* \param[out] None
* \return     void
* \authors
* \date
*/
void CCANDBConverterDlg::OnCanmonBrowseBtn()
{
    // Create a file save dialog box to select the input file
    CFileDialog fileDlg(TRUE,"dbf",NULL,OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,"Input Database Files(*.dbf)|*.dbf||",this);
    fileDlg.m_ofn.lpstrTitle = "Select input database file"; // set the title

    // if OK clicked, set the path name of input file in the edit box and enable the convert
    // button
    if(fileDlg.DoModal() == IDOK)
    {
        CString sCanoeFilePath, sCanMonFilePath;

        // set if output file was set automatically
        int set_to_default = 0;
        GetDlgItemText(IDC_CANOE_EDIT, sCanoeFilePath);
        if (sCanoeFilePath == "")
        {
            set_to_default = 1;
        }
        else
        {
            sCanoeFilePath = sCanoeFilePath.Left(sCanoeFilePath.GetLength()-4);
            GetDlgItemText(IDC_CANMON_EDIT, sCanMonFilePath);
            sCanMonFilePath = sCanMonFilePath.Left(sCanMonFilePath.GetLength()-4);
            if (sCanoeFilePath == sCanMonFilePath)
            {
                set_to_default = 1;
            }
        }

        sCanMonFilePath = fileDlg.GetPathName();
        SetDlgItemText(IDC_CANMON_EDIT, sCanMonFilePath);

        // set output file automatically to default
        if (set_to_default == 1)
        {
            sCanoeFilePath = sCanMonFilePath.Left(sCanMonFilePath.GetLength()-4);
            sCanoeFilePath += ".dbc";
            SetDlgItemText(IDC_CANOE_EDIT, sCanoeFilePath);
        }

        GetDlgItem(IDC_CONVERT_BTN)->EnableWindow(TRUE);
        SetDlgItemText(IDC_RESULT_STATIC,"Ready for conversion");
    }
}

/**
* \brief      OnCanmonBrowseBtn is the Handler of ON_BN_CLICKED message for IDC_CONVERT_BTN.
This function converts the CANoe Database to BUSMASTER Database
* \param[in]  None
* \param[out] None
* \return     void
* \authors
* \date
*/
void CCANDBConverterDlg::OnConvertBtn()
{

    CString sCanoeFilePath,sCanMonFilePath;
    CString sExtension;

    // Get the input path name from the dialog box
    GetDlgItemText(IDC_CANMON_EDIT,sCanMonFilePath);
    sExtension = sCanMonFilePath.Right(4);
    sExtension.MakeLower();

    // validate extension, if invalid clear the edit box
    // and disable the convert button
    if(sExtension != ".dbf")
    {
        AfxMessageBox("Invalid input file");
        SetDlgItemText(IDC_CANMON_EDIT,"");
        GetDlgItem(IDC_CONVERT_BTN)->EnableWindow(FALSE);
        return;
    }

    // Get output file path name
    GetDlgItemText(IDC_CANOE_EDIT,sCanoeFilePath);
    sExtension = sCanoeFilePath.Right(4);
    sExtension.MakeLower();

    if(sExtension != ".dbc")
    {
        AfxMessageBox("Invalid ouput file");
        SetDlgItemText(IDC_CANOE_EDIT,"");
    }

    // create a converter module and pass the file names to the conversion module
    CConverter *pConverter = new CConverter;
    if(pConverter == NULL)
    {
        AfxMessageBox("Error: Insufficient memory");
        return;
    }
    else
    {
        INT nRetVal = pConverter->Convert(sCanMonFilePath,sCanoeFilePath);
        // display final result
        SetDlgItemText(IDC_RESULT_STATIC, pConverter->GetResultString());
        if(nRetVal != 0)
        {
            SetDlgItemText(IDC_EDIT_LOGPATH, pConverter->m_omLogFilePath);
        }
        delete pConverter;
    }
}

/**
* \brief      OnInitDialog Function, Called once when the CAboutDlg dialog box is created
* \param[in]  None
* \param[out]
* \return     BOOL, TRUE if dialog has to create otherwise FALSE
* \authors
* \date
*/
BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    CString sVer,sCopyright;

    TCHAR sPath[MAX_PATH];
    DWORD dwVerInfoSize;
    DWORD dwHandle;
    LPVOID pVerInfo = NULL;

    LPTSTR pCopyright;
    LPTSTR pCompanyName;
    LPTSTR pProductName;
    LPTSTR pProductVer;
    UINT uLen;


    ::GetModuleFileName(NULL,sPath,MAX_PATH);
    dwVerInfoSize = ::GetFileVersionInfoSize(sPath,&dwHandle);

    pVerInfo = (LPVOID) new BYTE[dwVerInfoSize];
    ::GetFileVersionInfo(sPath,dwHandle,dwVerInfoSize,pVerInfo);

    // language code page = US English
    VerQueryValue(pVerInfo,_T("\\StringFileInfo\\040904b0\\LegalCopyright"),(LPVOID*)&pCopyright,&uLen);

    VerQueryValue(pVerInfo,_T("\\StringFileInfo\\040904b0\\CompanyName"),(LPVOID*)&pCompanyName,&uLen);

    VerQueryValue(pVerInfo,_T("\\StringFileInfo\\040904b0\\ProductName"),(LPVOID*)&pProductName,&uLen);

    VerQueryValue(pVerInfo,_T("\\StringFileInfo\\040904b0\\ProductVersion"),(LPVOID*)&pProductVer,&uLen);


    sVer.Format("%s %s",pProductName,pProductVer);
    sCopyright.Format("%s\n%s",pCopyright,pCompanyName);

    delete pVerInfo;
    pVerInfo = NULL;


    SetDlgItemText(IDC_VERSION_STATIC,sVer);
    SetDlgItemText(IDC_COPYRIGHT_STATIC,sCopyright);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
