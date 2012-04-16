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

/* C include */
#include <stdlib.h>

/* Project includes */
#include "CAPL2CPropertyPage.h"
#include "ConstString.h"
#include "Functions.hpp"
#include "List.h"

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
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
{
    //{{AFX_DATA_INIT(CCAPL2CPropertyPage)
    m_check = FALSE;
    m_savedb = FALSE;
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCAPL2CPropertyPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCAPL2CPropertyPage)
    DDX_Check(pDX, IDC_CHKB_OPTN, m_check);
    DDX_Check(pDX, IDC_CHKB_SAVEDB, m_savedb);
    //}}AFX_DATA_MAP
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
    ON_BN_CLICKED(IDC_CHKB_OPTN, OnChkbOptn)
    ON_BN_CLICKED(IDC_CBTN_BCANOEDB, OnBrowseCANoeDb)
    ON_BN_CLICKED(IDC_CBTN_BBUSMASTERDB, OnBrowseBUSMASTERDb)
    ON_BN_CLICKED(IDC_CBTN_CANCEL, OnCancel)
    ON_BN_CLICKED(IDC_CHKB_SAVEDB, OnChkbSavedb)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCAPL2CPropertyPage message handlers

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
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // TODO: Add extra initialization here

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
 Function Name    : OnInitDialog
 Input(s)         :     -
 Output           :     -
 Functionality    : This will populate the first list box with unsupported
                    keys and environment handlers and the second list box
					with available keys.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 22.04.2004
 Modifications    :
*******************************************************************************/
BOOL List::OnInitDialog()
{
    try
    {
        int Flag = 0;
        int m = 0;

        char allkey[]= {'a','b','c','d','e','f','g','h','i','j','k','l','m','n',
                        'o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E',
                        'F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V',
                        'W','X','Y','Z','0','1','2','3','4','5','6','7','8','9',0
                       };
        CDialog::OnInitDialog();
        for(m = 0; m < ouUnSptdKey.nGetSize(); m++)
        {
            m_UnSupKeys.AddString( ouUnSptdKey.omGetAt(m) );
        }

        for( m = 0; m < defINT_SizeofacAltKeyArray ; m++ )
        {
            Flag = 0;
            for( int p = 0; p <= cIndex2-1; p++)
            {
                if(allkey[m] == acSptdKey[p])
                {
                    Flag = 1;
                    break;
                }
            }
            if( Flag == 0)
            {
                m_AltKeys.AddString( CString( allkey[m] ) );
            }
        }

        m_UnSupKeys.SetCurSel(0);
        m_AltKeys.SetCurSel(0);

        if( m_AltKeys.GetCount() == 0)
        {   //if no key is available
            GetDlgItem( IDC_CBTN_ADD )->EnableWindow(FALSE);
            ShowWindow(SW_SHOW);
            MessageBox("No Key is avilable",NULL,MB_OK);

        }
    }
    catch(...)
    {
        CString cs;

        cs.Format(ExceptionFormat,"\"OnInitDialog\"",__FILE__,__LINE__);
        MessageBox(cs);
        exit(0);
    }

    return TRUE;

}
/*******************************************************************************
 Function Name    : OnAdd
 Input(s)         :     -
 Output           :     -
 Functionality    : This will take selected element from first two list and add
                    it to the third one.The element selected from the second
					list will be added to acAltKey array at the place where the
					first list element is stored in acUnSptdKey array.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 23.04.2004
 Modifications    :
*******************************************************************************/
void List::OnAdd()
{
    int m = 0;

    CString uns,alt,uns1;

    m = m_UnSupKeys.GetCurSel();

    m_UnSupKeys.GetText( m,uns );//store selected element from first list
    m_UnSupKeys.GetText( m,uns1 );
    m_UnSupKeys.DeleteString( m );//delete it
    m = m_AltKeys.GetCurSel();

    m_AltKeys.GetText( m,alt );//store selected element from second list
    m_AltKeys.DeleteString( m );

    uns = uns+" --->  ";
    uns = uns + alt;
    m_EquiKeys.AddString( uns );//add both to third list

    acAltKey[ouUnSptdKey.nFind(uns1)] = alt[0];//add the key to acAltKeylist

    m_UnSupKeys.SetCurSel(0);//set first item selected
    m_AltKeys.SetCurSel(0);
    m_EquiKeys.SetCurSel(0);
    if(m_UnSupKeys.GetCount() == 0 || m_AltKeys.GetCount() == 0 )
    {
        GetDlgItem( IDC_CBTN_ADD )->EnableWindow(FALSE);
    }
    else
    {
        GetDlgItem( IDC_CBTN_ADD )->EnableWindow(TRUE);
    }
    GetDlgItem( IDC_CBTN_RMV )->EnableWindow(TRUE);
}
/*******************************************************************************
 Function Name    : OnRemove
 Input(s)         :     -
 Output           :     -
 Functionality    : This will will remove the selected item from the third list
                    and will put the first element in the first list and second
					element in the second list.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 23.04.2004
 Modifications    :
*******************************************************************************/
void List::OnRemove()
{
    CString ekey;
    if((m = m_EquiKeys.GetCurSel()) != LB_ERR)
    {
        GetDlgItem( IDC_CBTN_ADD )->EnableWindow(TRUE);
        m_EquiKeys.GetText( m,ekey );
        int k = ekey.Find(' ',0);
        if( k != -1)
        {
            m_UnSupKeys.AddString(ekey.Left(k));
            m_AltKeys.AddString(ekey.Right(1));
        }

        acAltKey [ ouUnSptdKey.nFind( ekey.Left( k)) ] = -1;

        m_EquiKeys.DeleteString(m);

        m_UnSupKeys.SetCurSel(0);
        m_AltKeys.SetCurSel(0);
        m_EquiKeys.SetCurSel(0);
    }

    if(m_EquiKeys.GetCount()==0)
    {
        GetDlgItem(IDC_CBTN_RMV)->EnableWindow(FALSE);
    }
}
/*******************************************************************************
 Function Name    : OnTerminate
 Input(s)         :     -
 Output           :     -
 Functionality    : This will take first item from list one and first item from
                    list two to make the default selection of available keys for
					the unsupported keys.This will write the user intervention
					for the alternate keys in the log file also.
 Member of        :     -
 Friend of        :     -
 Author(s)        : Amit Ranjan
 Date Created     : 27.04.2004
 Modifications    :
*******************************************************************************/
void List::OnTerminate()
{
    CString buffer;
    CString uns,alt,uns1;

    if(m_UnSupKeys.GetCount()>0)
    {
        if ( m_AltKeys.GetCount() > 0 )
        {
            MessageBox(defSTR_Warning6,MB_OK);
        }
        while(m_UnSupKeys.GetCount()!=0)
        {
            if(m_AltKeys.GetCount()>0)
            {
                m_UnSupKeys.GetText(0,uns);
                m_UnSupKeys.GetText(0,uns1);
                m_UnSupKeys.DeleteString(0);

                m_AltKeys.GetText(0,alt);
                m_AltKeys.DeleteString(0);

                uns = uns+" --->  ";
                uns = uns + alt;
                m_EquiKeys.AddString(uns);

                acAltKey[ ouUnSptdKey.nFind(uns1) ] = alt[0];
            }
            else
            {
                m_UnSupKeys.DeleteString(0);
            }
        }
    }

    if( m_EquiKeys.GetCount() > 0 )
    {
        fprintf(logfile,"---***********************************---");
        fprintf(logfile,"\n---*****List Of User Intervention*****---");
        fprintf(logfile,"\n---***********************************---");
        fprintf(logfile,"\n Unsupported Key -->Equivalent Key");
        fprintf(logfile,"\n ---------------    ---------------\n");
    }

    for(int m = 0; m <= m_EquiKeys.GetCount()-1; m++ )
    {
        m_EquiKeys.GetText( m,buffer );
        fprintf(logfile,"\t%s\n",buffer);
    }

    m_UnSupKeys.ResetContent();
    m_EquiKeys.ResetContent();
    m_AltKeys.ResetContent();
    List::OnOK();
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
    CFileDialog cfd(TRUE,"can",NULL,OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST|
                    OFN_PATHMUSTEXIST,"CAN Files(*.can)|*.can||",this);

    cfd.m_ofn.lpstrTitle = "Select CAN File";


    GetDlgItemText( IDC_EDIT_INPUT,omStrPath );
    omStrPath.TrimLeft();
    omStrPath.TrimRight();
    if(omStrPath.IsEmpty())
    {
        omStrPath = AfxGetApp()->GetProfileString("Files","CAPL file","");
    }
    if( (nIndex = omStrPath.ReverseFind('\\') )!= -1)
    {
        omStrPath = omStrPath.Left(nIndex);
    }

    cfd.m_ofn.lpstrInitialDir = omStrPath;

    if(cfd.DoModal()==IDOK)
    {
        omStrPath = cfd.GetPathName();
        SetDlgItemText(IDC_EDIT_INPUT, omStrPath );
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
    CFileDialog cfd(TRUE,"c",NULL,OFN_OVERWRITEPROMPT|
                    OFN_HIDEREADONLY,"BUSMASTER  Files(*.c)|*.c||",this);

    cfd.m_ofn.lpstrTitle = "Select BUSMASTER File";

    CString omStrPath;
    int nIndex;

    GetDlgItemText( IDC_EDIT_OUTPUT,omStrPath );
    omStrPath.TrimLeft();
    omStrPath.TrimRight();
    if(omStrPath.IsEmpty())
    {
        omStrPath = AfxGetApp()->GetProfileString("Files","CAPL file","");
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

    cs = cs + "\\notepad.exe " + dest1;
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

void CCAPL2CPropertyPage::OnChkbOptn()
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
    CString omStrPath;

    CFileDialog cfd(TRUE,"dbc",NULL,OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST|
                    OFN_PATHMUSTEXIST,"CANoe Database Files(*.dbc)|*.dbc||",this);

    cfd.m_ofn.lpstrTitle = "Select CANoe Database File";

    GetDlgItemText( IDC_EDIT_CANOEDB,omStrPath );
    omStrPath.TrimLeft();
    omStrPath.TrimRight();

    if( omStrPath.Find(".dbc",0) != -1)
    {
        omStrPath = omStrPath.Left( omStrPath.GetLength() - 4);
    }

    cfd.m_ofn.lpstrInitialDir = omStrPath;



    if(cfd.DoModal()==IDOK)
    {
        omStrPath = cfd.GetPathName();
        SetDlgItemText(IDC_EDIT_CANOEDB, omStrPath );
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
    CString omStrPath;

    CFileDialog cfd( TRUE,"dbf",NULL,OFN_OVERWRITEPROMPT|
                     OFN_PATHMUSTEXIST,"BUSMASTER Database Files(*.dbf)|*.dbf||",this);

    cfd.m_ofn.lpstrTitle = "Select BUSMASTER Database File";

    GetDlgItemText( IDC_EDIT_BUSMASTERDB,omStrPath );
    omStrPath.TrimLeft();
    omStrPath.TrimRight();

    if( omStrPath.Find(".dbc",0) != -1)
    {
        omStrPath = omStrPath.Left( omStrPath.GetLength() - 4);
    }

    cfd.m_ofn.lpstrInitialDir = omStrPath;

    if(cfd.DoModal()==IDOK)
    {
        omStrPath = cfd.GetPathName();
        SetDlgItemText(IDC_EDIT_BUSMASTERDB, omStrPath );
    }

}

void CCAPL2CPropertyPage::OnChkbSavedb()
{
    UpdateData();
    if( m_savedb )
    {
        cFlagLog = 2;//to check whether to save converted database or not
        GetDlgItem( IDC_EDIT_BUSMASTERDB )->EnableWindow(TRUE);
        GetDlgItem( IDC_CBTN_BBUSMASTERDB )->EnableWindow(TRUE);
    }
    else
    {
        cFlagLog = 0;
        SetDlgItemText(IDC_EDIT_BUSMASTERDB," " );
        GetDlgItem( IDC_EDIT_BUSMASTERDB )->EnableWindow(FALSE);
        GetDlgItem( IDC_CBTN_BBUSMASTERDB )->EnableWindow(FALSE);

    }

}

void CCAPL2CPropertyPage::SaveSettings()
{
    //this function will save the settings in the registry.
    CString strSection       = "Files";
    CString strStringItem    = "CAPL File";

    CWinApp* pApp = AfxGetApp();
    CString omStrCAPLName;
    CWnd * pCAPL = GetDlgItem(IDC_EDIT_INPUT);
    pCAPL->GetWindowText(omStrCAPLName);
    pApp->WriteProfileString(strSection, strStringItem,omStrCAPLName);
}

void List::OnCancel()
{
    List::OnTerminate();
}

void List::OnClose()
{
    OnTerminate();
    CDialog::OnClose();
}
