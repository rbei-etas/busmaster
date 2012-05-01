/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  GraphExportDlg.cpp
  Description   :  Implementation file for CGraphExportDlg class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphExportDlg.cpv  $

  Author(s)     :  Raja N
  Date Created  :  10/12/2004
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved
*******************************************************************************/

#include "SigGrphWnd_stdafx.h"
// For App declaration
#include "SigGrphWnd_resource.h"
// For Bottom View definition
#include "GraphBottomView.h"
// For Export dialog declaration
#include "GraphExportDlg.h"
#include "SigGrphWnd_Defines.h"

/*******************************************************************************
  Function Name  : CGraphExportDlg
  Description    : Standard default constructor
  Member of      : CGraphExportDlg
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
CGraphExportDlg::CGraphExportDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CGraphExportDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CGraphExportDlg)
    m_omStrCSVFileName = STR_EMPTY;
    m_omStrHTMLFileName = STR_EMPTY;
    m_omStrBMPFileName = STR_EMPTY;
    m_pDMGraphCtrl = NULL;
    //}}AFX_DATA_INIT
}

/*******************************************************************************
  Function Name  : ~CGraphExportDlg
  Description    : Standard Destructor
  Member of      : CGraphExportDlg
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphExportDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGraphExportDlg)
    DDX_Text(pDX, IDC_EDIT_CSV_FILE_NAME, m_omStrCSVFileName);
    DDX_Text(pDX, IDC_EDIT_HTML_FILE_NAME, m_omStrHTMLFileName);
    DDX_Text(pDX, IDC_EDIT_IMAGE_FILE_NAME, m_omStrBMPFileName);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGraphExportDlg, CDialog)
    //{{AFX_MSG_MAP(CGraphExportDlg)
    ON_BN_CLICKED(IDC_BTN_BROWSE_CSV, OnBtnBrowseCsv)
    ON_BN_CLICKED(IDC_BTN_SAVE_CSV, OnBtnSaveCsv)
    ON_BN_CLICKED(IDC_BTN_BROWSE_HTML, OnBtnBrowseHtml)
    ON_BN_CLICKED(IDC_BTN_SAVE_HTML, OnBtnSaveHtml)
    ON_BN_CLICKED(IDC_BTN_BROWSE_BMP, OnBtnBrowseBmp)
    ON_BN_CLICKED(IDC_BTN_SAVE_IMAGE, OnBtnSaveImage)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*******************************************************************************
  Function Name  : OnBtnBrowseCsv
  Input(s)       : -
  Output         : -
  Functionality  : Shows file save dialog to select CSV export file path
  Member of      : CGraphExportDlg
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphExportDlg::OnBtnBrowseCsv()
{
    // Create Save File Dialog with overwrite warning
    CFileDialog omSaveAsDlg( FALSE,                 // Save File dialog
                             defSTR_CSV_FORMAT,     // Default Extension,
                             NULL,
                             OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                             defSTR_CSV_FORMAT_SPECIFIER,
                             NULL );
    // Set Title to Export
    omSaveAsDlg.m_ofn.lpstrTitle  = defSTR_EXPORT_DIALOG_CAPTION;

    // Show file save dialog
    if ( omSaveAsDlg.DoModal() == IDOK )
    {
        // Save the selected path
        m_omStrCSVFileName = omSaveAsDlg.GetPathName();
        UpdateData( FALSE );
    }
}

/*******************************************************************************
  Function Name  : OnBtnSaveCsv
  Input(s)       : -
  Output         : -
  Functionality  : Exports the graph data in to CSV Format
  Member of      : CGraphExportDlg
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 15.12.2004, Added update data before processing the
                   file name
*******************************************************************************/
void CGraphExportDlg::OnBtnSaveCsv()
{
    USES_CONVERSION;
    // Update Data to get file name
    UpdateData();

    // Check for valid entry
    if( m_omStrCSVFileName.IsEmpty() == FALSE )
    {
        if( m_pDMGraphCtrl != NULL )
        {
            // Generate the Report
            m_pDMGraphCtrl->GenerateCSVReport(T2W(m_omStrCSVFileName));
        }
    }
    else
    {
        AfxMessageBox( defSTR_CSV_FILE_PATH_EMPTY );
    }
}

/*******************************************************************************
  Function Name  : OnBtnBrowseHtml
  Input(s)       : -
  Output         : -
  Functionality  : Shows file save dialog to select HTML report file path
  Member of      : CGraphExportDlg
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphExportDlg::OnBtnBrowseHtml()
{
    // Create Save File Dialog with overwrite warning
    CFileDialog omSaveAsDlg( FALSE,      // Save File dialog
                             defSTR_HTML_FORMAT,     // Default Extension,
                             NULL,
                             OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                             defSTR_HTML_FORMAT_SPECIFIER,
                             NULL );
    // Set Title to Export
    omSaveAsDlg.m_ofn.lpstrTitle  = defSTR_REPORT_DIALOG_CAPTION;

    // Show the dialog and save the path on OK
    if ( omSaveAsDlg.DoModal() == IDOK )
    {
        // Save the file path
        m_omStrHTMLFileName = omSaveAsDlg.GetPathName();
        UpdateData( FALSE );
    }
}

/*******************************************************************************
  Function Name  : OnBtnSaveHtml
  Input(s)       : -
  Output         : -
  Functionality  : Exports the graph data in to HTML Format. If print option is
                   selected then prints the generated report using web
                   browser control
  Member of      : CGraphExportDlg
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 14.12.2004, Added pointer to parameters
  Modifications  : Raja N on 15.12.2004, Added update data before processing the
                   file name
  Modifications  : Raja N on 9.3.2005
                   Added code to include multi channel information in the HTML
                   report
*******************************************************************************/
void CGraphExportDlg::OnBtnSaveHtml()
{
    // Update Data to get file name
    /*UpdateData();
    // Check for valid file name
    if( m_omStrHTMLFileName.IsEmpty() == FALSE )
    {
        if( m_podGraphControl != NULL )
        {
            CStringArray omParams;
            // Get The configuration File Name
            CString omStr = STR_EMPTY;
            // Get the Name from the app module
            //theApp.GetLoadedConfigFilename( omStr );
            // Add Config File
            //omParams.Add( omStr );
    #if 0
            // Get the Baud Rate
            PSCONTROLLER_DETAILS  pBaudDetails = NULL;
            // Get the Baud Rate
            theApp.bGetData( CONTROLLER_DETAILS, (void**)&pBaudDetails);
            if( pBaudDetails != NULL )
            {
                CString omStrBaudRate(STR_EMPTY);
                // Get the actual hardware available
                UINT unActualHw = 0;
                LONG lParam = 0;
                if (g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK)
                {
                    unActualHw = (UINT)lParam;
                }
                for( UINT unIndex = 0; unIndex < defNO_OF_CHANNELS; unIndex++ )
                {
                    // Format and Add Baud Rate
                    omStr.Format( defSTR_BAUD_RATE_FORMAT,
                                  unIndex + 1,
                                  pBaudDetails[unIndex].m_omStrBaudrate );
                    // Add to the baud rate string
                    omStrBaudRate += omStr;
                    // Use * to denote the channel where the hardware is missing
                    if( unIndex > ( unActualHw - 1 ) )
                    {
                        omStrBaudRate += defASSETRIC;
                    }
                    // Add one space after that
                    omStrBaudRate += defSPACE_STR;
                }
                // Add baud rate string
                omParams.Add( omStrBaudRate );
                // Now release the memory
                theApp.vRelease( CONTROLLER_DETAILS, (void **) &pBaudDetails );
            }
    #endif
            // On Success show the message box and print if selected
            if( m_podGraphControl->GenerateHTMLReport( m_omStrHTMLFileName,
                                           (LPUNKNOWN*)&omParams) == 0 )
            {
                // Show Success message
                CString omStrMsg;
                omStrMsg.Format( defSTR_REPORT_CREATE_SUCCESS ,
                                 m_omStrHTMLFileName );
                AfxMessageBox( omStrMsg );
                // Check for Print enable option
                CButton * pWnd = (CButton *)GetDlgItem( IDC_CHECK_PRINT_REPORT );
                if( pWnd != NULL )
                {
                    // Get the check box status
                    int bPrintReport = pWnd->GetCheck();
                    // If selected
                    if( bPrintReport == TRUE )
                    {
                        // Update Browser Control
                        m_omBrowser.Navigate( m_omStrHTMLFileName, NULL, NULL,
                                              NULL, NULL );
                        // Call Page Setuo
                        m_omBrowser.ExecWB( OLECMDID_PAGESETUP,
                                            OLECMDEXECOPT_DODEFAULT, NULL, NULL);
                        // Print the document
                        m_omBrowser.ExecWB( OLECMDID_PRINT,
                                            OLECMDEXECOPT_DODEFAULT, NULL, NULL);
                    }
                }
            }
        }
    }
    else
    {
        AfxMessageBox( defSTR_HTML_FILE_PATH_EMPTY );
    }   */
}

/*******************************************************************************
  Function Name  : OnBtnBrowseBmp
  Input(s)       : -
  Output         : -
  Functionality  : Shows file save dialog to select BMP file path
  Member of      : CGraphExportDlg
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphExportDlg::OnBtnBrowseBmp()
{
    CFileDialog omSaveAsDlg( FALSE,      // Save File dialog
                             defSTR_BMP_FORMAT,     // Default Extension,
                             NULL,
                             OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                             defSTR_BMP_FORMAT_SPECIFIER,
                             NULL );
    // Set Title
    omSaveAsDlg.m_ofn.lpstrTitle  = defSTR_BITMAP_DIALOG_CAPTION;

    // Show the dialog and save on OK
    if ( omSaveAsDlg.DoModal() == IDOK )
    {
        m_omStrBMPFileName = omSaveAsDlg.GetPathName();
        UpdateData( FALSE );
    }
}


/*******************************************************************************
  Function Name  : OnBtnSaveCsv
  Input(s)       : -
  Output         : -
  Functionality  : Exports the graph data in to BMP Format
  Member of      : CGraphExportDlg
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 15.12.2004, Added update data before processing the
                   file name
*******************************************************************************/
void CGraphExportDlg::OnBtnSaveImage()
{
    // Update Data to get file name
    UpdateData();

    // Check for valid file name
    if( m_omStrBMPFileName.IsEmpty() == FALSE )
    {
        // Save the image to the given path
        if( m_pDMGraphCtrl != NULL )
        {
            _bstr_t bstrBMPFileName = m_omStrBMPFileName;
            m_pDMGraphCtrl->SaveAs( bstrBMPFileName.GetBSTR() );
        }
    }
    else
    {
        AfxMessageBox(defSTR_bmp_FILE_PATH_EMPTY);
    }
}

/*******************************************************************************
  Function Name  : OnInitDialog
  Input(s)       : -
  Output         : FALSE - If focus is set to Any UI control explicitly
  Functionality  : Initialises dialog's UI components
  Member of      : CGraphExportDlg
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
BOOL CGraphExportDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect;
    GetClientRect(&rect);
    ClientToScreen(&rect);
    rect.bottom -= 90;
    SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
    return TRUE;
}