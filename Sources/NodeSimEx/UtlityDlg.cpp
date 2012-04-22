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
 * \file      UtlityDlg.cpp
 * \brief     This file contain definition of all function of 
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of 
 */

#include "NodeSimEx_stdafx.h"
#include "FunctionEditorDoc.h"  // Get Document
#include "GlobalObj.h"
#include "utlitydlg.h"
#include "SimSysManager.h"
#include "SimSysDetView.h"
#include "UtlityDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CUtlityDlg dialog

/******************************************************************************/
/*  Function Name    :  CUtlityDlg                                            */
/*                                                                            */
/*  Input(s)         :  Standard Constructor                                  */
/*  Output           :                                                        */
/*  Functionality    :  Constructor                                           */
/*  Member of        :  CUtlityDlg                                            */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :  Amitesh Bharti, 23.02.2003                            */
/*                      added initialisation of m_omStrUtilsFunText           */
/******************************************************************************/

CUtlityDlg::CUtlityDlg(CFunctionEditorDoc* pDoc, CWnd* pParent /*=NULL*/)
    : CDialog(CUtlityDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CUtlityDlg)
    m_omStrFunctionName = _T("");
    m_omStrUtilsFunText = _T("");
    //}}AFX_DATA_INIT
	m_bChangeUtilFunc = FALSE;
	m_omStrTempReturnType = STR_EMPTY ;
    m_pDoc = pDoc;
}


void CUtlityDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CUtlityDlg)
    DDX_Control(pDX, IDC_EDIT_UTILS_FN, m_omEditUtilsFunText);
    DDX_Control(pDX, IDC_COMB_FN_RETURN_TYPE, m_omComboRetType);
    DDX_Text(pDX, IDC_EDIT_FN_PROTO, m_omStrFunctionName);
    DDX_Text(pDX, IDC_EDIT_UTILS_FN, m_omStrUtilsFunText);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUtlityDlg, CDialog)
    //{{AFX_MSG_MAP(CUtlityDlg)
    ON_BN_CLICKED(IDC_CBTN_OK, OnOk)
    ON_EN_CHANGE(IDC_EDIT_FN_PROTO, OnChangeEditFnPrototype)
    ON_BN_CLICKED(IDC_CBTN_CANCEL, OnCancel)
    //}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMB_FN_RETURN_TYPE, OnCbnSelchangeCombFnReturnType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUtlityDlg message handlers

/******************************************************************************/
/*  Function Name    :  OnOk                                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user clicks on Ok button. This        */
/*                      function gets the selected data type for the variable */
/*                      and packs the variable name with the data type.       */
/*                                                                            */  
/*  Member of        :  CUtlityDlg                                            */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :  27.12.2002, Amarnath S                                */
/*                      Appropriate message box added and validated for all   */ 
/*                      invalid characters                                    */ 
/*  Modifications    :  Amitesh Bharti, 23.02.2003                            */
/*                      added m_omStrUtilsFunText to append the name of       */
/*                      utility function with "Utils_"                        */
/*  Modifications    :  Amitesh Bharti, 23.03.2003                            */
/*                      solved problem of not selecting multiple parameter    */
/*  Modofications    :  Raja N on 03.08.2004, The return type combobox is     */
/*                      editable now. So used get window text to get the text */
/*                      if the selection is -1.                               */
/*  Modofications    :  Anish  on 28.09.2006, On edit Utility function the    */
/*                      function should be replaced if return type is changed */
/*                      while the name is not changed                         */
/******************************************************************************/
void CUtlityDlg::OnOk() 
{
	BOOL bRetVal = TRUE;
    CString omFuncName = _T("");
	UpdateData(TRUE);
	if ( m_omStrFunctionName.GetAt(0) == SPACE )    
	{
		AfxMessageBox(_T("Invalid character found"), MB_OK|MB_ICONINFORMATION);
		GetDlgItem(IDC_EDIT_FN_PROTO)->SetFocus();
	}
	else
	{
		int nIndex =  m_omStrFunctionName.Find( '(');// != -1
		if (  nIndex != -1 )
		{
			CString omStrTemp = m_omStrFunctionName;
			omFuncName = omStrTemp = omStrTemp.Left(nIndex);
			if  ( omStrTemp.FindOneOf( _T("~`!@#$%^&*-+[]{};:\\|'?/><,\" ()"))
				== -1 )
			{
				if ( m_omStrFunctionName.Find( ')') != -1 )
				{
					CString omStrSelDataType = STR_EMPTY;
					// get the data type selected
					int nIndex = m_omComboRetType.GetCurSel();
					// If there is no selection then get the text from editbox
					if( nIndex != -1 )
					{
						m_omComboRetType.GetLBText( nIndex, omStrSelDataType );
					}
					else
					{
						m_omComboRetType.GetWindowText( omStrSelDataType );
					}
					// If the return type is valid
					if (omStrSelDataType.IsEmpty() == FALSE )
					{
						// Insert a space in the end
						omStrSelDataType.Insert( 
							omStrSelDataType.GetLength(),(TCHAR)SPACE);
						// Form prototype
						omStrSelDataType += m_omStrUtilsFunText;
						omStrSelDataType +=m_omStrFunctionName;
						m_omStrFunPrototype = omStrSelDataType;
						if(m_bChangeUtilFunc != TRUE)
						{
							// Check for duplicate selection 
							// Get key handler array from the document
							if (m_pDoc != NULL)
							{
								CStringArray* pArray = 
									m_pDoc->omStrGetUtilityFunctionPrototypes();

								if (pArray)
								{
									CString omStrMsgPro = STR_EMPTY;
									for (int nCount = 0; 
										nCount < pArray->GetSize(); nCount++)
									{
										omStrMsgPro = 
											pArray->GetAt( nCount );

										if ( omStrMsgPro.
											Find(m_omStrFunctionName) != 
											-1 )
										{
											// Form the message to be dispalyed
											omStrMsgPro = 
												defMSG_DUPL_UTIL_FUNC;
											omStrMsgPro.Replace( 
												_T("FUNCTIONNAME"), 
												m_omStrUtilsFunText+m_omStrFunctionName );

											// Duplicate message hanlder, shout
											AfxMessageBox( 
												omStrMsgPro, 
												MB_OK|MB_ICONINFORMATION );
											nCount = (COMMANINT)pArray->GetSize();
											bRetVal = FALSE;
										}
									}
								}
							}
						}
					}
					else
					{
						AfxMessageBox(MSG_RET_TYPE_EMPTY, 
							MB_OK|MB_ICONINFORMATION);
						GetDlgItem(IDC_COMB_FN_RETURN_TYPE)->SetFocus();
						bRetVal = FALSE;
					}

					if ( bRetVal == TRUE )
						CDialog::OnOK();
				}
				else
				{
					// No Closing braces
					AfxMessageBox(_T("No closing paranthesis found!"));
					GetDlgItem(IDC_EDIT_FN_PROTO)->SetFocus();
					bRetVal = FALSE;

				}
			}
			else
			{
				// Invalid character
				AfxMessageBox(_T("Invalid character found!"));
				GetDlgItem(IDC_EDIT_FN_PROTO)->SetFocus();
				bRetVal = FALSE;
			}
		}
		else
		{
			// No white spaces in function name
			AfxMessageBox(_T("No opening paranthesis found!"));
			GetDlgItem(IDC_EDIT_FN_PROTO)->SetFocus();
			bRetVal = FALSE;
		}
	}   
}
/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Adds data type to the combo box control and 
                        disables Ok button                      
                                                                              */
/*  Member of        :  CUtlityDlg                                            */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :  Raja N on 03.08.2004                                  */
/*                      Added code to add database message name to the return */
/*                      type combobox. This combobox is editable now.         */
/*  Modifications    :  Anish  on 28.09.2006                                  */
/*                      Added code to display  the return type in the combobox*/
/*                      if it has been opened for editing                     */
/******************************************************************************/

BOOL CUtlityDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    // Add default data types to the combo box
//  m_omComboRetType.AddString( BOOLEAN );  
    m_omComboRetType.AddString( defUNSIGNED_CHAR );
    m_omComboRetType.AddString( defUNSIGNED_LONG );
    m_omComboRetType.AddString( defFLOAT );
    m_omComboRetType.AddString( defSIGNED_INT );
    m_omComboRetType.AddString( defUNSIGNED_INT );
    m_omComboRetType.AddString( TVOID );

    
    // Get all the message names of active DB
   // CMsgSignal &ouDatabase = CMsgSignal::ouGetMsgSignal();
    // If the database pointer is valid
    //if( ouDatabase != NULL )
    //{
    //UINT unNoOfMessages = ouDatabase.unGetNumerOfMessages();
    // If there are some messages then get the names and add
    //if ( unNoOfMessages > 0 )
    //{
        // To strore database message names
        //CStringList omMessageNames;
        //// Get names
        //ouDatabase.omStrListGetMessageNames( omMessageNames );
        //// Get the starting position first
        //POSITION pos = omMessageNames.GetHeadPosition();
        //// Insert every message name into the message list box
        //CString omStrMsgName = STR_EMPTY;
        //// Iterate through the list
        //while ( pos != NULL )
        //{
        //    omStrMsgName = omMessageNames.GetNext(pos);
        //    // Addition check to ensure there will not be any empty string
        //    if( omStrMsgName.IsEmpty() == FALSE )
        //    {
        //        m_omComboRetType.AddString( omStrMsgName );
        //    }
        //}
    //}
    //}
    //if the Function prototype has to change display the return type
    m_omComboRetType.SelectString(-1,m_omStrTempReturnType);
    // Disable Ok button initially.
    GetDlgItem( IDC_CBTN_OK )->EnableWindow(FALSE);
    // Set the utility function prefix 
    m_omEditUtilsFunText.SetWindowText(defUTILS_FUNCTION_FN);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
/******************************************************************************/
/*  Function Name    :  OnChangeEditFnPrototype                               */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Enables or disables the Ok button depending on the 
                        number of characters on the edit control.
                                                                              */  
/*  Member of        :  CUtlityDlg                                            */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :  
/******************************************************************************/

void CUtlityDlg::OnChangeEditFnPrototype() 
{
    UpdateData(TRUE);

    // Enable or disable Ok button
    if ( !m_omStrFunctionName.IsEmpty() )
    {
        GetDlgItem( IDC_CBTN_OK )->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem( IDC_CBTN_OK )->EnableWindow(FALSE);
    }
    
}
/******************************************************************************/
/*  Function Name    :  OnCancel                                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Calls the base class OnCanCel
                                                                              */  
/*  Member of        :  CUtlityDlg                                            */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  11.03.2002                                            */
/*  Modifications    :  
/******************************************************************************/
void CUtlityDlg::OnCancel() 
{
    CDialog::OnCancel();
}

/******************************************************************************/
/*  Function Name    :  vSetUtilFuncChange                                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Set m_bChangeUtilFunc if user wants to edit the       */ 
/*                      function name                                         */
/*  Member of        :  CUtlityDlg                                            */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  27.09.2006                                            */
/*  Modifications    :  
/******************************************************************************/
void CUtlityDlg::vSetUtilFuncChange(CString omStrReturnType)
{
	m_bChangeUtilFunc = TRUE;
	m_omStrTempReturnType = omStrReturnType;
}
/******************************************************************************/
/*  Function Name    :  OnCbnSelchangeCombFnReturnType                        */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Enable OK BUtton even if return type is changed       */ 
/*  Member of        :  CUtlityDlg                                            */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  27.09.2006                                            */
/*  Modifications    :  
/******************************************************************************/
void CUtlityDlg::OnCbnSelchangeCombFnReturnType()
{
	 UpdateData(TRUE);

    // Enable or disable Ok button
    if ( !m_omStrFunctionName.IsEmpty() )
    {
        GetDlgItem( IDC_CBTN_OK )->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem( IDC_CBTN_OK )->EnableWindow(FALSE);
    }
}
