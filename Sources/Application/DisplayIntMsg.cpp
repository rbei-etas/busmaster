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
 * \file      DisplayIntMsg.cpp
 * \brief     This file contain the definition all member function of
 * \author    Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition all member function of
 */
// For all standard header file include
#include "stdafx.h"
// Definition of App class
#include "BUSMASTER.h"
// Definition of CDisplayIntMsg class.
#include "DisplayIntMsg.h"

/******************************************************************************/
/*  Function Name    :  CDisplayIntMsg                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor is called when user create an object of   */
/*                      this class. m_omInterpretedMsg data member will be    */
/*                      assing the value passed as parameter omStrStringArray */
/*  Member of        :  CDisplayIntMsg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.03.2002                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
CDisplayIntMsg::CDisplayIntMsg(CONST CStringArray& omStrStringArray)
    : CDialog(CDisplayIntMsg::IDD, NULL)
{
    m_omInterpretedMsg.RemoveAll();
    m_omInterpretedMsg.Copy(omStrStringArray);
}
/******************************************************************************/
/*  Function Name    :  CDisplayIntMsg                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor is called when user create an object of   */
/*                      this class. m_omInterpretedMsg data member will be    */
/*                      assing the value passed as parameter omStrStringArray */
/*  Member of        :  CDisplayIntMsg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.03.2002                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
void CDisplayIntMsg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDisplayIntMsg)
    DDX_Control(pDX, IDC_EDIT_INTERPRETED_MSG, m_omEditControl);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDisplayIntMsg, CDialog)
    //{{AFX_MSG_MAP(CDisplayIntMsg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will be called by frame work after      */
/*                       DoModal() function call. The edit control attached to*/
/*                      this dialog will be initialied with text in item of   */
/*                      data member m_omInterpretedMsg after inserting the new*/
/*                      charactors.                                           */
/*  Member of        :  OnInitDialog                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.03.2002                                            */
/*  Modifications    :  28.03.2002, Amitesh Bharti                            */
/*                       Review comment                                       */
/******************************************************************************/
BOOL CDisplayIntMsg::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Get the content of CStringArray object and add it to CString object
    // after inserting new line.
    CString omStr           = "";
    CString omStrDisplay    = "";
    INT nTotalItem = m_omInterpretedMsg.GetSize();

    for(INT nCount=0; nCount<nTotalItem; nCount++)
    {
        omStr.Format("%s\r\n",m_omInterpretedMsg.GetAt(nCount));
        omStrDisplay  += omStr;
    }

    // Set the edit box text.
    m_omEditControl.SetWindowText(omStrDisplay);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
