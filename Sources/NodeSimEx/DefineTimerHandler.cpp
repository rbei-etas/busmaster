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
 * \file      DefineTimerHandler.cpp
 * \brief     This file contain the definition all member function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition all member function of
 */

// For all standard header file include
#include "NodeSimEx_stdafx.h"
// CDefineTimerHandler class definition
#include "DefineTimerHandler.h"
#include "GlobalObj.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/******************************************************************************/
/*  Function Name    :  CDefineTimerHandler                                   */
/*  Input(s)         :  Pointer to Parent Window                              */
/*  Output           :                                                        */
/*  Functionality    :  This function is defualt constructor.                 */
/*                                                                            */
/*  Member of        :  CDefineTimerHandler                                   */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.03.2002                                            */    
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  28.03.2002, Review comments                           */
/******************************************************************************/
CDefineTimerHandler::CDefineTimerHandler(CFunctionEditorDoc* pDoc, CWnd* pParent /*=NULL*/)
    : CDialog(CDefineTimerHandler::IDD, pParent),
      m_omStrTimerFunctionName(STR_EMPTY)

{
    //{{AFX_DATA_INIT(CDefineTimerHandler)
    m_omEditStrTimerFuncName = _T("");
    m_unEditTimerValue = 1;
    //}}AFX_DATA_INIT
    m_pDoc = pDoc;
}
/******************************************************************************/
/*  Function Name    :  DoDataExchange                                        */    
/*  Input(s)         :                                                        */    
/*  Output           :                                                        */    
/*  Functionality    :  Called by the framework to exchange and validate      */
/*                         dialog data                                        */
/*                                                                            */
/*  Member of        :  CDefineTimerHandler                                   */    
/*  Friend of        :      -                                                 */    
/*  Author(s)        :  Amitesh Bharti                                        */    
/*  Date Created     :  08.03.2002                                            */    
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CDefineTimerHandler::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDefineTimerHandler)
    DDX_Control(pDX, IDC_EDIT_TIMER_VALUE, m_omEditTimerValue);
    DDX_Control(pDX, IDC_EDIT_TIMER_FUNCTION_NAME, m_omEditTimerFuncName);
    DDX_Text(pDX, IDC_EDIT_TIMER_FUNCTION_NAME, m_omEditStrTimerFuncName);
    DDX_Text(pDX, IDC_EDIT_TIMER_VALUE, m_unEditTimerValue);
    //}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CDefineTimerHandler, CDialog)
    //{{AFX_MSG_MAP(CDefineTimerHandler)
    ON_WM_HELPINFO()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
/******************************************************************************/
/*  Function Name    :  OnHelpInfo                                            */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by the framework in response  */
/*                      to the WM_HELPINFO message. This message is sent to   */
/*                      the dialog box during help request from user.         */
/*  Member of        :  CDefineTimerHandler                                   */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  11.03.2002                                            */
/*  Modifications    :  
/******************************************************************************/
BOOL CDefineTimerHandler::OnHelpInfo(HELPINFO* pHelpInfo) 
{
    //theApp.vSetHelpID(pHelpInfo->dwContextId);
    return CDialog::OnHelpInfo(pHelpInfo);
}
/******************************************************************************/
/*  Function Name    :  OnOK                                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by the framework when the     */
/*                      user clicks on the OK button.The validation for name  */
/*                      of timer handler function is done before closing the  */
/*                      the dialog.                                           */
/*  Member of        :  CDefineTimerHandler                                   */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  11.03.2002                                            */
/*  Modifications    :  21.03.2002, Amarnath Shastry                          */
/*                      Added Validation for timer handler name and value     */ 
/*                      28.03.2002, Amitesh Bharti                            */
/*                      Reveiw comments added                                 */
/*                      28.10.2003, Amitesh Bharti                            */
/*                      Bug due to make upper case and comparing the name of  */
/*                      timers is corrected.                                  */
/******************************************************************************/
void CDefineTimerHandler::OnOK() 
{
    BOOL bNoMoreTimer               = FALSE;
    CString omStrTimerHandlerName(STR_EMPTY);
    CString omStrTimerValue(STR_EMPTY);
    m_omEditTimerFuncName.GetWindowText(omStrTimerHandlerName);
    // No special characters allowed 
    if  ( omStrTimerHandlerName.FindOneOf( _T("~!@#$%^&*-+={}[]/\"\\<>`,.? ()") )
        == -1 )
    {
        m_omEditTimerValue.GetWindowText( omStrTimerValue );

        if (m_omEditTimerValue.lGetValue() < 1)
        {
            bNoMoreTimer = TRUE;
            AfxMessageBox(_T("Please enter a value more than or equal\n\
                          to 1 msec!"), MB_OK|MB_ICONINFORMATION);
            m_omEditTimerValue.SetFocus();
        }

        else if ( !omStrTimerHandlerName.IsEmpty() &&
            !omStrTimerValue.IsEmpty()       &&
            omStrTimerValue != "0"         &&
            bNoMoreTimer == FALSE)
        {
            // Construct function name
            m_omStrTimerFunctionName = defTIMER_FN_NAME;
            m_omStrTimerFunctionName.Replace( _T("TIMERNAME"),omStrTimerHandlerName);
            m_omStrTimerFunctionName.Replace( _T("TIMERVALUE"), omStrTimerValue );

            // validate the timer name, document class
            // has array of timer functions defined

            // Get document class object
            if ( m_pDoc != NULL )
            {
                // Get timer array pointer
                CStringArray* pTimerArray = 
                    m_pDoc->omStrGetTimerHandlerPrototypes();

                if ( pTimerArray != NULL )

                {
                    // Check if the handler name already 
                    // exists!
                    for ( INT unCount = 0;
                        ( unCount < pTimerArray->GetSize() ) && 
                        ( bNoMoreTimer == FALSE );
                    unCount++)
                    {
                        CString omStrProto = 
                            pTimerArray->GetAt( unCount );

                        // Get the handler name from the prototype
                        omStrProto = 
                            m_pDoc->omStrExtractFunctionNameFromPrototype( 
                            omStrProto);

                        // Remove the __stdcall keyword from the function
                        int nSpaceIndex = 
                            omStrProto.Find( SPACE );
                        if ( nSpaceIndex != -1 )
                        {
                            omStrProto = 
                                omStrProto.Right( omStrProto.GetLength()-
                                nSpaceIndex );
                            omStrProto.TrimLeft();
                            omStrProto.TrimRight();
                            omStrProto.MakeUpper();

                        }

                        CString omTemp = m_omStrTimerFunctionName;

                        if (  omTemp.CompareNoCase( omStrProto )==0)
                        {
                            // Yes, it is a duplication
                            bNoMoreTimer = TRUE;
                            AfxMessageBox(defMSG_DUP_TIMER_HANDLER,
                                MB_OK|MB_ICONINFORMATION);
                        }
                    }
                }
            }

            if(bNoMoreTimer == TRUE)
            {
                m_omEditTimerFuncName.SetFocus();
                m_omEditTimerFuncName.SetSel(0, -1,FALSE);
            } 
        }
        else
        {
            // If the edit control is empty, don't close the dialog.
            bNoMoreTimer = TRUE;
            // For any invalid input, beep
            MessageBeep( (UINT)-1 );
        }

        if(bNoMoreTimer == FALSE)
        {
            // Call the base class function to close the dialog
            CDialog::OnOK();
        }
    }
    else
    {
        MessageBeep((UINT)-1);
    }
}
/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Sets edit control properties for Timer value          */
/*  Member of        :  CDefineTimerHandler                                   */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  22.03.2002                                            */
/*  Modifications    :  10.02.2004 Raja N                                     */
/*                      Changed the default value to 1                        */
/*                                                                            */  
/******************************************************************************/
BOOL CDefineTimerHandler::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    // Set timer value edit control
    // to accept only unsigned decimal integer
    m_omEditTimerValue.vSetBase( BASE_DECIMAL );
    m_omEditTimerValue.vSetSigned( FALSE );

    // Set the initial value of the timer
    // as 50msec
    //m_omEditTimerValue.vSetValue( defONE );
        
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


