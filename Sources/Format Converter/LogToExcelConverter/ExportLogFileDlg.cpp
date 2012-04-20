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
 * \file      ExportLogFileDlg.cpp
 * \brief     Definition of CExportLogFileDlg class
 * \authors   Pradeep Kadoor, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the CExportLogFileDlg class.
 */

/* Project includes */
#include "Defines.h"
#include "ExportLogFileDlg.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CExportLogFileDlg, CPropertyPage)
CExportLogFileDlg::CExportLogFileDlg(ETYPE_BUS eBus, CWnd* pParent /*=NULL*/)
	: CPropertyPage(CExportLogFileDlg::IDD)
	, m_omStrLogFileName(_T(""))
	, m_omStrCSVFileName(_T(""))
{
    m_eBus = eBus;
    if (m_eBus == CAN)
    {
        m_pacFields = (CHAR_ARRAY_20*) acFields_CAN;
        m_unNoOfFileds = defNO_OF_FIELDS_CAN;
    }
    else if (m_eBus == J1939)
    {
        m_pacFields = (CHAR_ARRAY_20*)acFields_J1939;
        m_unNoOfFileds = defNO_OF_FIELDS_J1939;
    }
}

CExportLogFileDlg::~CExportLogFileDlg()
{
}

void CExportLogFileDlg::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_LOG_FILE_NAME, m_omStrLogFileName);
    DDX_Text(pDX, IDC_EDIT_CSV_FILE_NAME, m_omStrCSVFileName);
    DDX_Control(pDX, IDC_LST_AVAILABLE, m_omAvailableList);
    DDX_Control(pDX, IDC_LST_SELECTED, m_omSelectedList);
    DDX_Control(pDX, IDC_BUTTON_SELECTALL, m_omSelectAllFields);
    DDX_Control(pDX, IDC_BUTTON_SELECTONE, m_omSelectOneField);
    DDX_Control(pDX, IDC_BUTTON_REMOVEONE, m_omRemoveOneField);
    DDX_Control(pDX, IDC_BUTTON_REMOVEALL, m_omRemoveAllFields);
    DDX_Control(pDX, IDC_COMBO_BUSTYPE, m_omBusType);
}

BEGIN_MESSAGE_MAP(CExportLogFileDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_LOG_BROWSE, OnBnClickedBtnLogBrowse)
	ON_BN_CLICKED(IDC_BTN_CSV_BROWSE, OnBnClickedBtnCsvBrowse)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, OnBnClickedButtonSelectall)
	ON_BN_CLICKED(ID_CONVERT, OnBnClickedConvert)
	ON_BN_CLICKED(IDC_CBTN_CANCELEXPORT, OnBnClickedCbtnCancelexport)
	ON_LBN_SELCHANGE(IDC_LST_AVAILABLE, OnLbnSelchangeLstAvailable)
	ON_LBN_SELCHANGE(IDC_LST_SELECTED, OnLbnSelchangeLstSelected)
	ON_BN_CLICKED(IDC_BUTTON_SELECTONE, OnBnClickedButtonSelectone)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEONE, OnBnClickedButtonRemoveone)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEALL, OnBnClickedButtonRemoveall)
    ON_CBN_SELCHANGE(IDC_COMBO_BUSTYPE, OnCbnSelchangeComboBustype)
END_MESSAGE_MAP()


/**
 * \brief On Button Clicked Button Log Browse
 *
 * To show file selection dialog for log File
 */
void CExportLogFileDlg::OnBnClickedBtnLogBrowse()
{
	 UpdateData( TRUE );

    // Create Save File Dialog with overwrite warning
    CFileDialog omSaveAsDlg( TRUE ,                 // Save File dialog
                             defSTR_LOG_FORMAT,     // Default Extension,
                             NULL,
                             OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                             defSTR_LOG_FORMAT_SPECIFIER,
                             NULL );
    // Set Title to Export
    omSaveAsDlg.m_ofn.lpstrTitle  = defSTR_LOG_DIALOG_CAPTION;
    // Show file save dialog
    if ( omSaveAsDlg.DoModal() == IDOK )
    {
        // Save the selected path
        m_omStrLogFileName = omSaveAsDlg.GetPathName();
        
        INT nIndex = m_omStrLogFileName.ReverseFind('.');
        if ( nIndex >= 0)
        {
            CString omStrOutputFile = m_omStrLogFileName.Left(nIndex);
            omStrOutputFile += ".xls";
            m_omStrCSVFileName = omStrOutputFile;
        }
        UpdateData( FALSE );
    }
}

/**
 * \brief On Button Clicked Button CSV Browse
 *
 * To show file selection dialog for CSV File
 */
void CExportLogFileDlg::OnBnClickedBtnCsvBrowse()
{
	    UpdateData( TRUE );
    // Create Save File Dialog with overwrite warning
    CFileDialog omSaveAsDlg( FALSE,                 // Save File dialog
                             defSTR_XLS_FORMAT,     // Default Extension,
                             NULL,
                             OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                             defSTR_XLS_FORMAT_SPECIFIER,
                             NULL );
    // Set Title to Export
    omSaveAsDlg.m_ofn.lpstrTitle  = defSTR_CSV_DIALOG_CAPTION;
    // Show file save dialog
    if ( omSaveAsDlg.DoModal() == IDOK )
    {
        // Save the selected path
        m_omStrCSVFileName = omSaveAsDlg.GetPathName();
        UpdateData( FALSE );
    }
}

/**
 * \brief Enable/Disable Controls
 *
 * Enable/Disable Controls
 */
void CExportLogFileDlg::vEnableDisableControls()
{
	BOOL bSelectAllButtonEnable = FALSE;
    BOOL bRemoveAllButtonEnable = FALSE;
    BOOL bSelectOneButtonEnable = FALSE;
    BOOL bRemoveOneButtonEnable = FALSE;

    // Select Button
    if( m_omAvailableList.GetCount() != 0 && m_omAvailableList.GetCurSel() != -1 )
    {
        bSelectAllButtonEnable = TRUE;
		bSelectOneButtonEnable = TRUE;
    }

	if( m_omSelectedList.GetCount() != 0 && m_omSelectedList.GetCurSel() != -1 )
    {
        bRemoveAllButtonEnable = TRUE;
		bRemoveOneButtonEnable = TRUE;
    }
	// update Button Status
    m_omSelectAllFields.EnableWindow( bSelectAllButtonEnable );
    m_omSelectOneField.EnableWindow( bSelectOneButtonEnable );
    m_omRemoveOneField.EnableWindow( bRemoveOneButtonEnable );
    m_omRemoveAllFields.EnableWindow( bRemoveAllButtonEnable );
}

/**
 * \brief Populate Available List
 *
 * Populate Available List
 */
void CExportLogFileDlg::vPopulateAvailableList()
{
    // Insert filed text
    m_omAvailableList.ResetContent();
    m_omSelectedList.ResetContent();
    for( UINT unIndex = 0; unIndex < m_unNoOfFileds; unIndex++)
    {
        m_omAvailableList.InsertString( unIndex, m_pacFields[unIndex]);
        m_omAvailableList.SetItemData( unIndex, unIndex );
    }
}

/**
 * \brief On Button Clicked Button Select All
 *
 * Message handler for select all button
 */
void CExportLogFileDlg::OnBnClickedButtonSelectall()
{
    
	int nSelectedItem = m_omAvailableList.GetCurSel();
	DWORD_PTR nFieldIndex;
	if( nSelectedItem != -1 )
    {
		nSelectedItem=0;
		while(m_omAvailableList.GetCount()!=0)
		{
            nFieldIndex = m_omAvailableList.GetItemData( nSelectedItem );
            m_omAvailableList.DeleteString( nSelectedItem );
            m_omAvailableList.SetCurSel( nSelectedItem );
            // Add it in to the selected list
            int nNewItemIndex = m_omSelectedList.InsertString(
                                           m_omSelectedList.GetCount(),
                                           m_pacFields[ nFieldIndex ] );
            // Set Field index as item data
            m_omSelectedList.SetItemData( nNewItemIndex, nFieldIndex );
            // Set the selection
            m_omSelectedList.SetCurSel( nNewItemIndex );
        }
	}
	// Update UI buttons status
	vEnableDisableControls();
}

/**
 * \brief On Init Dialog
 *
 * Init dialog
 */
BOOL CExportLogFileDlg::OnInitDialog()
{   
    CPropertyPage::OnInitDialog();
    CString omStrBus;
    if (m_eBus == CAN)
    {
        omStrBus = "CAN";
    }
    else if (m_eBus == J1939)
    {
        omStrBus = "J1939";
    }
    m_omBusType.SelectString(0, omStrBus);
    vPopulateAvailableList();
	vEnableDisableControls();
	return TRUE;  
}

/**
 * \brief On Button Clicked Convert
 *
 * Message handler for convert button
 */
void CExportLogFileDlg::OnBnClickedConvert()
{
    
	// Log File name and Excel File name

	
	CString strLogFileName = m_omStrLogFileName;  
	CString strCSVFileName = m_omStrCSVFileName;  


	// filename have been entered

	
	if( strLogFileName != "" && strCSVFileName != "" )
	{
		if( m_omSelectedList.GetCount() != 0 ) 
		{	
			//fields have been selected
			//open the files
            CLogToExcel oExport( strLogFileName.GetBuffer(MAX_PATH),strCSVFileName.GetBuffer(MAX_PATH), m_unNoOfFileds, m_pacFields);
			//set the selected fields
			oExport.fnSetSelectedFields( &(this->m_omSelectedList) );
			//convert log file to excel file
			if( !oExport.bConvert() )
            {
				MessageBox(EXPORTTOEXCEL_SYNTAXERROR,APPLICATION_NAME,MB_OK);
            }
			else
			{
				MessageBox(EXPORTTOEXCEL_SUCCESSMSG,APPLICATION_NAME,MB_OK);
				//OnOK();
			}
		}
		else
			    MessageBox(EXPORTTOEXCEL_FIELDSELECTIONERROR,APPLICATION_NAME,MB_OK);
	}
	else
				MessageBox(EXPORTTOEXCEL_FILEERROR,APPLICATION_NAME,MB_OK);
}

/**
 * \brief On Button Clicked Button Cancel Export
 *
 * Message handler for cancel export button
 */
void CExportLogFileDlg::OnBnClickedCbtnCancelexport()
{
	OnCancel(); 
}

/**
 * \brief On Listbutton Select Change List Available
 *
 * Message handler for selection change on availbale list
 */
void CExportLogFileDlg::OnLbnSelchangeLstAvailable()
{
	// TODO: Add your control notification handler code here
	vEnableDisableControls();
}

/**
 * \brief On Listbutton Select Change List Selected
 *
 * Message handler for selection change
 */
void CExportLogFileDlg::OnLbnSelchangeLstSelected()
{
	vEnableDisableControls();
}

/**
 * \brief On Button Clicked Button Select One
 *
 * Message handler for select one button
 */
void CExportLogFileDlg::OnBnClickedButtonSelectone()
{
	// Get the selected item index
    int nSelectedItem = m_omAvailableList.GetCurSel();
    // If it is valid
    if( nSelectedItem != -1 )
    {
        // Get the filed index
        DWORD_PTR nFieldIndex = m_omAvailableList.GetItemData( nSelectedItem );
        if( nFieldIndex >= 0 && nFieldIndex < m_unNoOfFileds )
        {
            // Remove that item
            m_omAvailableList.DeleteString( nSelectedItem );
            // Set the focus to the next item
            if( nSelectedItem == m_omAvailableList.GetCount() )
            {
                nSelectedItem--;
            }
            m_omAvailableList.SetCurSel( nSelectedItem );
            // Add it in to the selected list
            int nNewItemIndex = m_omSelectedList.InsertString(
                                           m_omSelectedList.GetCount(),
                                           m_pacFields[ nFieldIndex ] );
            // Set Field index as item data
            m_omSelectedList.SetItemData( nNewItemIndex, nFieldIndex );
            // Set the selection
            m_omSelectedList.SetCurSel( nNewItemIndex );
            // Update UI buttons status
            vEnableDisableControls();
        }
    }

}

/**
 * \brief On Button Clicked Button Remove One
 *
 * Message handler for remove one button
 */
void CExportLogFileDlg::OnBnClickedButtonRemoveone()
{
    
	// Get the selected item index
    int nSelectedItem = m_omSelectedList.GetCurSel();
    // If it is a valid index
    if( nSelectedItem != -1 )
    {
        // Get the selected item data. That is index of fields array
        DWORD_PTR nFieldIndex = m_omSelectedList.GetItemData( nSelectedItem );
        // Remove the item fr the selected list
        m_omSelectedList.DeleteString( nSelectedItem );
        // Set the item selection to next
        if( nSelectedItem == m_omSelectedList.GetCount() )
        {
            nSelectedItem--;
        }
        m_omSelectedList.SetCurSel( nSelectedItem );

        // Insert this item in to the available list
        int nNewItemIndex = m_omAvailableList.GetCount();
        for( int nIndex = 0; nIndex < m_omAvailableList.GetCount(); nIndex++ )
        {
            if( m_omAvailableList.GetItemData( nIndex ) > nFieldIndex )
            {
                nNewItemIndex = nIndex;
                // Break the loop
                nIndex = m_omAvailableList.GetCount();
            }
        }
        if( nNewItemIndex != -1 )
        {
            m_omAvailableList.InsertString( nNewItemIndex,
                                            m_pacFields[ nFieldIndex ] );
        }
        else
        {
            nNewItemIndex = m_omAvailableList.AddString( 
                                        m_pacFields[ nFieldIndex ] );
        }
        // Set item data
        m_omAvailableList.SetItemData( nNewItemIndex, nFieldIndex );
        // Set the new item as selected
        m_omAvailableList.SetCurSel( nNewItemIndex );
        // Update UI Controls
        vEnableDisableControls();
    }
}

/**
 * \brief On Button Clicked Button Remove All
 *
 * Message handler for remove all button
 */
void CExportLogFileDlg::OnBnClickedButtonRemoveall()
{
    
	int nSelectedItem = m_omSelectedList.GetCurSel();
	DWORD_PTR nFieldIndex;
	if( nSelectedItem != -1 )
    {
		nSelectedItem=0;
		while(m_omSelectedList.GetCount()!=0)
		{
			// Get the selected item data. That is index of fields array
			nFieldIndex = m_omSelectedList.GetItemData( nSelectedItem );
			// Remove the item fr the selected list
			m_omSelectedList.DeleteString( nSelectedItem );
			// Set the item selection to next
			m_omSelectedList.SetCurSel( nSelectedItem );
			// Insert this item in to the available list
			int nNewItemIndex = m_omAvailableList.GetCount();
			for( int nIndex = 0; nIndex < m_omAvailableList.GetCount(); nIndex++ )
			{
				if( m_omAvailableList.GetItemData( nIndex ) > nFieldIndex )
				{
					nNewItemIndex = nIndex;
					// Break the loop
					nIndex = m_omAvailableList.GetCount();
				}
			}
			if( nNewItemIndex != -1 )
			{
				m_omAvailableList.InsertString( nNewItemIndex,
												m_pacFields[ nFieldIndex ] );
			}
			else
			{
				nNewItemIndex = m_omAvailableList.AddString( 
											m_pacFields[ nFieldIndex ] );
			}
			// Set item data
			m_omAvailableList.SetItemData( nNewItemIndex, nFieldIndex );
			// Set the new item as selected
			m_omAvailableList.SetCurSel( nNewItemIndex );
			// Update UI Controls
		}
    }
	vEnableDisableControls();
}

/**
 * \brief On ComboBox Selection Change Combo Bus Type
 *
 * Message handler for selection change in bus type combo box.
 */
void CExportLogFileDlg::OnCbnSelchangeComboBustype()
{
    CString omStrBus;
    m_omBusType.GetWindowText(omStrBus);
    CString omTitle("Export Log File - ");
    if( omStrBus == "CAN")
    {
        omTitle += _T("CAN");
        m_eBus = CAN;
        m_pacFields = (CHAR_ARRAY_20*) acFields_CAN;
        m_unNoOfFileds = defNO_OF_FIELDS_CAN;
    }
    else if ( omStrBus == "J1939")
    {
        omTitle += _T("J1939");
        m_eBus = J1939;
        m_pacFields = (CHAR_ARRAY_20*)acFields_J1939;
        m_unNoOfFileds = defNO_OF_FIELDS_J1939;
    }
    SetWindowText(omTitle);
	vPopulateAvailableList();
	vEnableDisableControls();
}
