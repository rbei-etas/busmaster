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
 * \file      SelectFunctions.cpp
 * \brief     This file contain definition of all function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */

#include "NodeSimEx_stdafx.h"
#include "SelectFunctions.h"
#include "GlobalObj.h"

/////////////////////////////////////////////////////////////////////////////
// CSelectFunctions dialog

/******************************************************************************/
/*  Function Name    :  CSelectFunctions                                      */
/*                                                                            */
/*  Input(s)         :  Standard Constructor                                  */
/*  Output           :                                                        */
/*  Functionality    :  Constructor
/*  Member of        :  CSelectFunctions                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :
/******************************************************************************/

CSelectFunctions::CSelectFunctions(ETYPE_BUS eBus, CWnd* pParent /*=NULL*/)
    : CDialog(CSelectFunctions::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSelectFunctions)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_omSelectedFunctionPrototype = STR_EMPTY;
    m_eBus = eBus;
}


void CSelectFunctions::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSelectFunctions)
    DDX_Control(pDX, IDC_LSTB_FUNCTIONS, m_omFnListBox);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectFunctions, CDialog)
    //{{AFX_MSG_MAP(CSelectFunctions)
    ON_LBN_DBLCLK(IDC_LSTB_FUNCTIONS, OnDblclkListFunctions)
    ON_BN_CLICKED(IDC_CBTN_OK, OnSelect)
    ON_BN_CLICKED(IDC_CBTN_CANCEL, OnCancel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectFunctions message handlers
/******************************************************************************/
/*  Function Name    :  OnDblclkListFunctions                                                 */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when the double clicks on selected item.

                                                                              */
/*  Member of        :  CSelectFunctions                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :
/******************************************************************************/

void CSelectFunctions::OnDblclkListFunctions()
{
    OnSelect();
}
/******************************************************************************/
/*  Function Name    :  OnSelect                                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user clicks on Ok button.

                                                                              */
/*  Member of        :  CSelectFunctions                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :  06.01.2003, Amarnath S                                */
/*                      Introduced a check for nSelectedIndex to avoid        */
/*                      possible crash.                                       */
/******************************************************************************/

void CSelectFunctions::OnSelect()
{
    // get the selected text
    int nSelectedIndex = m_omFnListBox.GetCurSel();

    if ( nSelectedIndex != -1 )
    {
        m_omFnListBox.GetText( nSelectedIndex, m_omStrSelectedText );
        bFormatFunctionPrototype(m_omSelectedFunctionPrototype);
    }

    CDialog::OnOK();
}
/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Opens wrapper.h file and adds all the function
                        prototypes to the list box.
                                                                              */
/*  Member of        :  CSelectFunctions                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :  06.01.2003, Amarnath S                                */
/*                      Introduced a hashdefine for the registry location     */
/*  Modifications    :  07.01.2003, Amitesh Bharti                            */
/*                      Introduced a hashdefine for error message. Wrapper.h  */
/*                  file is open only if path is read correctly from registry.*/
/******************************************************************************/

BOOL CSelectFunctions::OnInitDialog()
{
    CDialog::OnInitDialog();
    int nCount = (COMMANINT)CGlobalObj::ouGetObj(m_eBus).m_ArrAPIsList.GetSize();

    for (int i = 0; i < nCount; i++)
    {
        m_omFnListBox.AddString( CGlobalObj::ouGetObj(m_eBus).m_ArrAPIsList.GetAt(i) );
    }

    // Set horizontal extent of the list
    // box to max string availaable
    // so that user can scroll
    CSize   sz;
    CString omStrText;
    CDC*  pDC = m_omFnListBox.GetDC();
    int nDx = 0;

    for (int nCount = 0; nCount < m_omFnListBox.GetCount(); nCount++)
    {
        m_omFnListBox.GetText( nCount, omStrText );
        // remove space
        omStrText.TrimRight();
        sz = pDC->GetTextExtent(omStrText);

        if (sz.cx > nDx)
        {
            nDx = sz.cx;
        }
    }

    m_omFnListBox.ReleaseDC(pDC);
    // Set the horizontal extent so every character of all strings
    // can be scrolled to.
    m_omFnListBox.SetHorizontalExtent(nDx);
    return TRUE;
}
/******************************************************************************/
/*  Function Name    :  OnCancel                                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Calls base class function
                                                                              */
/*  Member of        :  CSelectFunctions                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :
/******************************************************************************/

void CSelectFunctions::OnCancel()
{
    CDialog::OnCancel();
}

BOOL CSelectFunctions::bFormatFunctionPrototype(CString& omStrFunctionPrototype)
{
    BOOL bReturn = TRUE;
    CStringArray omStrArrayParaType;
    CString omStrReturnType = "";
    CString omStrFuncName   = "";

    if(m_omStrSelectedText.IsEmpty() == 0 )
    {
        CString omStrTemp =    m_omStrSelectedText;
        omStrTemp = omStrTemp.Right(omStrTemp.GetLength() -
                                    omStrTemp.Find(_T(" ")) - 1 );
        omStrTemp.TrimLeft();
        omStrTemp.TrimRight();
        omStrReturnType = m_omStrSelectedText;
        omStrReturnType.Replace(omStrTemp, "");
        INT nIndex = 0;
        nIndex = omStrTemp.Find(_T("("));
        omStrFuncName = omStrTemp.Left(nIndex);
        omStrTemp = omStrTemp.Right(omStrTemp.GetLength() - nIndex - 1);
        omStrTemp = omStrTemp.Left( omStrTemp.Find(_T(")")));

        while(omStrTemp.IsEmpty() == 0)
        {
            nIndex = omStrTemp.Find(_T(","));

            if(nIndex != -1)
            {
                omStrArrayParaType.Add(omStrTemp.Left(nIndex));
            }
            else
            {
                omStrArrayParaType.Add(omStrTemp);
                omStrTemp = "";
            }

            omStrTemp = omStrTemp.Right(omStrTemp.GetLength() - nIndex - 1 );
        }

        omStrReturnType.TrimLeft();
        omStrReturnType.TrimRight();
        omStrFuncName.TrimLeft();
        omStrFuncName.TrimRight();

        if(omStrReturnType.CompareNoCase(_T("void")) !=0)
        {
            omStrFunctionPrototype  = defTODO_FUNCTION_INSERT;
            omStrFunctionPrototype += defNEW_LINE;
            omStrFunctionPrototype += _T("/* ");
            omStrFunctionPrototype += omStrReturnType;
            omStrFunctionPrototype += _T(" ");
            omStrFunctionPrototype += defRETURN_TYPE_NAME;
            omStrFunctionPrototype += _T(" ;*/");
        }

        UINT unCount = (COMMANUINT)omStrArrayParaType.GetSize();
        omStrFuncName += _T("(");

        for(UINT i = 0; i<unCount ; i++)
        {
            omStrTemp = omStrArrayParaType.GetAt(i);
            omStrTemp.TrimLeft();
            omStrTemp.TrimRight();
            omStrFunctionPrototype += defNEW_LINE;
            omStrFunctionPrototype += _T("/* ");
            omStrFunctionPrototype += omStrTemp;
            omStrTemp               = "";
            omStrTemp.Format(defPARAMETER_NAME,i+1);
            omStrFunctionPrototype += _T(" ");
            omStrFunctionPrototype +=omStrTemp;
            omStrFunctionPrototype += _T(" ;*/");
            omStrFuncName +=omStrTemp;

            if(i != unCount - 1 )
            {
                omStrFuncName += _T(",");
            }
        }

        omStrFuncName += _T(")");

        if(omStrReturnType.CompareNoCase(_T("void")) !=0)
        {
            omStrFunctionPrototype += defNEW_LINE;
            omStrFunctionPrototype += defRETURN_TYPE_NAME;
            omStrFunctionPrototype += _T(" = ");
        }

        omStrFunctionPrototype += omStrFuncName;
        omStrFunctionPrototype += _T(";");
        omStrFunctionPrototype += defNEW_LINE;
    }

    return bReturn;
}
