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
 * \file      DefConverterPage.h
 * \brief     Implementation of CDefConverterPage dialog
 * \author    Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CDefConverterPage dialog
 */

#include "stdafx.h"
#include "DefConverterPage.h"
#include <strsafe.h>

IMPLEMENT_DYNAMIC(CDefConverterPage, CPropertyPage)

CDefConverterPage::CDefConverterPage()
	: CPropertyPage(CDefConverterPage::IDD)
	, m_pouPluginManager(0)
    , m_omStrInputFilePath(_T(""))
    , m_omStrOutputFilePath(_T(""))
    , m_omstrConversionComment(_T(""))
    , m_omstrEditHelp(_T(""))
{

}

CDefConverterPage::~CDefConverterPage()
{
}

void CDefConverterPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_CONVERSIONS, m_omComboConverterNames);
    DDX_Control(pDX, IDC_EDIT_INPUTFILEPATH, m_omEditInputPath);
    DDX_Control(pDX, IDC_EDIT_OUTPUTFILEPATH, m_omEditOutputPath);
    DDX_Text(pDX, IDC_EDIT_INPUTFILEPATH, CString(m_omStrInputFilePath.c_str()));
    DDX_Text(pDX, IDC_EDIT_OUTPUTFILEPATH, CString(m_omStrOutputFilePath.c_str()));
    DDX_Text(pDX, IDC_EDIT_COMMENT, CString(m_omstrConversionComment.c_str()));
    DDX_Text(pDX, IDC_EDIT_HELP, CString(m_omstrEditHelp.c_str()));
	DDV_MaxChars(pDX, CString(m_omstrEditHelp.c_str()), 1024);
}


BEGIN_MESSAGE_MAP(CDefConverterPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_INPUT, CDefConverterPage::OnBnClickedBtnInput)
    ON_BN_CLICKED(IDC_BTN_OUTPUT, CDefConverterPage::OnBnClickedBtnOutput)
    ON_BN_CLICKED(IDC_BTN_CONVERT, CDefConverterPage::OnBnClickedBtnConvert)
    ON_CBN_SELCHANGE(IDC_COMBO_CONVERSIONS, CDefConverterPage::OnCbnSelchangeComboConversions)
END_MESSAGE_MAP()


// CDefConverterPage message handlers
HRESULT CDefConverterPage::SetPluginManager(CPluginManagerBase *pouPluginManager)
{
    m_pouPluginManager = pouPluginManager;
    return S_FALSE;
}
HRESULT CDefConverterPage::LoadConverters()
{
    HRESULT hResult = S_FALSE;
    if(m_pouPluginManager != NULL)
    {
        INT nCount = m_pouPluginManager->m_ConverterList.GetCount();
        string strConverterName;
        for(INT i = 0; i < nCount; i++)
        {
            POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(i);
            ConverterInfo& ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
            if(ouConverterInfo.m_pouConverter->bHaveOwnWindow() == FALSE)
            {
                ouConverterInfo.m_pouConverter->GetConverterName(strConverterName);
                INT nItem = m_omComboConverterNames.AddString(strConverterName.c_str());
                if(nItem >= 0)
                {
                    m_omComboConverterNames.SetItemData(nItem, i);
                }
            }
        }
        hResult = S_OK;
    }
    return hResult;
}
BOOL CDefConverterPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    // TODO:  Add extra initialization here
    LoadConverters();
    m_omComboConverterNames.SetCurSel(0);
    OnCbnSelchangeComboConversions();
    return TRUE;  // return TRUE unless you set the focus to a control
}

void CDefConverterPage::OnBnClickedBtnInput()
{
    // TODO: Add your control notification handler code here
    string strFileExt;
    string strFileFilters;
    INT nSelected = GetConverterPos();
    if( nSelected < m_pouPluginManager->m_ConverterList.GetCount())
    {
        POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(nSelected);
        ConverterInfo &ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
        if( ouConverterInfo.m_pouConverter != NULL)
        {
            ouConverterInfo.m_pouConverter->GetInputFileFilters(strFileExt, strFileFilters);
        }
    
		CFileDialog fileDlg( TRUE,     // Open File dialog
							 strFileExt.c_str(),     // Default Extension,
							 NULL,     // Initial file name.
							 OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST|
							 OFN_PATHMUSTEXIST,
							 strFileFilters.c_str(),
							 this );
		INT_PTR nRetVal = fileDlg.DoModal();


		if(IDOK == nRetVal)
		{
			m_omEditInputPath.SetWindowText(fileDlg.GetPathName());
			CString omStrOutputFile = (LPCTSTR) fileDlg.GetPathName();
			INT nIndex = omStrOutputFile.ReverseFind('.');
			if ( nIndex >= 0)
			{
				omStrOutputFile = omStrOutputFile.Left(nIndex);
				ouConverterInfo.m_pouConverter->GetOutputFileFilters(strFileExt, strFileFilters);
				omStrOutputFile += ".";
				omStrOutputFile += strFileExt.c_str();
				m_omEditOutputPath.SetWindowText(omStrOutputFile);
			}
			GetDlgItem(IDC_EDIT_COMMENT)->SetWindowText("");
		}
	}
}

void CDefConverterPage::OnBnClickedBtnOutput()
{
    string strFileExt;
    string strFileFilters;
    INT nSelected = GetConverterPos();
    if( nSelected < m_pouPluginManager->m_ConverterList.GetCount())
    {
        POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(nSelected);
        ConverterInfo &ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
        if( ouConverterInfo.m_pouConverter != NULL)
        {
            ouConverterInfo.m_pouConverter->GetOutputFileFilters(strFileExt, strFileFilters);
        }
    }
    CFileDialog fileDlg( TRUE,     // Open File dialog
                         strFileExt.c_str(),     // Default Extension,
                         NULL,     // Initial file name.
                         OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                         strFileFilters.c_str(),
                         this );
    INT_PTR nRetVal = fileDlg.DoModal();
    
    if(IDOK == nRetVal)
    {
        m_omEditOutputPath.SetWindowText(fileDlg.GetPathName());
    }
}

void CDefConverterPage::OnBnClickedBtnConvert()
{
    // TODO: Add your control notification handler code here
    INT nSelected = GetConverterPos();
	TRY
	{
		if( (nSelected >= 0 ) && (nSelected < m_pouPluginManager->m_ConverterList.GetCount()))
		{
			POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(nSelected);
			ConverterInfo &ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
			if( ouConverterInfo.m_pouConverter != NULL)
			{
				UpdateData();
				HRESULT hResult = ouConverterInfo.m_pouConverter->ConvertFile(m_omStrInputFilePath, m_omStrOutputFilePath);
				ouConverterInfo.m_pouConverter->GetLastConversionStatus(hResult, m_omstrConversionComment);
				UpdateData(FALSE);
			}
		}
	}
	CATCH(CException, e)
	{
		MessageBox("Unexpected Error Occured.Conversion Oberted", "Error", MB_OK|MB_ICONERROR);
	}
	END_CATCH
}

void CDefConverterPage::OnCbnSelchangeComboConversions()
{
    // TODO: Add your control notification handler code here
    m_omStrInputFilePath = _T("");
    m_omStrOutputFilePath = _T("");
    m_omstrConversionComment = _T("");
    string m_omstrConversionName;
    INT nSelectedItemIndex = GetConverterPos();
    if( nSelectedItemIndex >= 0 )
    {
        string chHelpText;
        string chConversionName;
        POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(nSelectedItemIndex);
        ConverterInfo &ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
        ouConverterInfo.m_pouConverter->GetHelpText(chHelpText);
        ouConverterInfo.m_pouConverter->GetConverterName(chConversionName);
        
        m_omstrEditHelp = chHelpText.c_str();
        m_omstrConversionName = chConversionName.c_str();
        //TODO::Require some more knowledge on how to change 
        //propertypage title.
        TC_ITEM tcItem;
        tcItem.mask = TCIF_TEXT;
        tcItem.pszText = (LPTSTR)(m_omstrConversionName.c_str());
        ((CPropertySheet*)GetParent())->GetTabControl()->SetItem(0, &tcItem );
		SetWindowText(m_omstrConversionName.c_str());
    }
    UpdateData(FALSE);
}


INT CDefConverterPage::GetConverterPos()
{
    INT nComboBoxIndex = m_omComboConverterNames.GetCurSel();
    INT nListIndex = m_omComboConverterNames.GetItemData(nComboBoxIndex);
    if( (nListIndex >= 0 ) && (nListIndex < m_pouPluginManager->m_ConverterList.GetCount()))
    {
        return nListIndex;
    }
    else
    {
        return -1;
    }
}