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
 * \file      MessageDetailsDlg.cpp
 * \brief     Implementation file for MessageDetailsDlg class
 * \authors   Amarnath Shastry, Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for MessageDetailsDlg class
 */
#include "stdafx.h"
#include "BUSMASTER.h"
#include "MessageDetailsDlg.h"
#include "MainFrm.h"

extern CCANMonitorApp theApp;

/******************************************************************************
  Function Name    :  CMessageDetailsDlg                                    
  Input(s)         :                                                        
  Output           :                                                        
  Functionality    :  Constructor is called when user create an object of   
                      this class. Initialisation of all data members done here   
  Member of        :  CMessageDetailsDlg                                            
  Friend of        :      -                                                 
  Author(s)        :  Amarnath Shastry                                        
  Date Created     :  07-05-2002                                            
*****************************************************************************/
CMessageDetailsDlg::CMessageDetailsDlg(const SDBPARAMS& sDbParams,sMESSAGE *pMsg /*=NULL*/,
                                       CWnd* pParent /*=NULL*/)
    : CDialog(CMessageDetailsDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMessageDetailsDlg)
    m_omStrMessageName = _T("");
    m_omStrMessageCode = _T("");
    m_unMessageLength = 0;
    m_nFrameFormat = -1;
    //m_nDataFormat = -1;
    //}}AFX_DATA_INIT
    m_psMsgStuctPtr = pMsg;
    m_sDbParams = sDbParams;
}


void CMessageDetailsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMessageDetailsDlg)
    DDX_Control(pDX, IDC_SPIN_MSG_LEN, m_omSpinMsgLen);
    DDX_Text(pDX, IDC_EDIT_MSG_NAME, m_omStrMessageName);
    DDX_Text(pDX, IDC_EDIT_MSGCODE, m_omStrMessageCode);
    DDV_MaxChars(pDX, m_omStrMessageCode, 8);
    DDX_Text(pDX, IDC_EDIT_MSG_LENGTH, m_unMessageLength);
    DDX_Radio(pDX, IDC_RBTN_STD, m_nFrameFormat);
    //DDX_Radio(pDX, IDC_RBTN_MOTOROLA, m_nDataFormat);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMessageDetailsDlg, CDialog)
    //{{AFX_MSG_MAP(CMessageDetailsDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************
  Function Name    :  OnInitDialog                                    
  Input(s)         :                                                        
  Output           :                                                        
  Functionality    :  Initialises the dialog with the data if the mode is edit 
                      mode. Otherwise the default values will be initialised.
  Member of        :  CMessageDetailsDlg                                            
  Friend of        :      -                                                 
  Author(s)        :  Amarnath Shastry                                        
  Date Created     :  07-05-2002                                            
*****************************************************************************/
BOOL CMessageDetailsDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();
    //Set the message Id field's text as 'PGN' if J1939,else 'Msg Id'
    GetDlgItem(IDC_STAT_MSG_CODE)->SetWindowText(m_sDbParams.m_omIdFieldName);

    //Select only extended messages
    
    // Set message length range 0-8
    m_omSpinMsgLen.SetBase( BASE_10 );
    m_omSpinMsgLen.SetRange( 0, (SHORT)m_sDbParams.m_unMaxMsgLen );
	m_odMsgNumericEdit.SubclassDlgItem(IDC_EDIT_MSG_NAME,this);
    // If Not NULL, mode is Edit,
    // Fill the dialog with the message details
    if (m_psMsgStuctPtr != NULL)
    {
        m_omStrMessageName  = m_psMsgStuctPtr->m_omStrMessageName;
        m_omStrMessageCode.Format( "%x", m_psMsgStuctPtr->m_unMessageCode );
        m_unMessageLength   = m_psMsgStuctPtr->m_unMessageLength;
        m_nFrameFormat      = m_psMsgStuctPtr->m_bMessageFrameFormat;
    }
    else
    {
        m_nFrameFormat      = 0;
    }
    if (m_sDbParams.m_eBus == J1939)
    {
        CButton* pButton = (CButton*)GetDlgItem(IDC_RBTN_EXTDED);
        pButton->SetCheck(BST_CHECKED);
        pButton = (CButton*)GetDlgItem(IDC_RBTN_STD);
        pButton->EnableWindow(FALSE);
        pButton->SetCheck(BST_UNCHECKED);
        m_nFrameFormat = 1;
    }
    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
/******************************************************************************
  Function Name    :  OnOK
  Input(s)         :                                                        
  Output           :                                                        
  Functionality    :  Called when the user presses OK button. 
                      Validation of the fileds.
                      Frame format conversion if mode is edit.
                      Allocate memory for new message.
                      Update the values in the edit to the data structure.
  Member of        :  CMessageDetailsDlg                                            
  Friend of        :      -                                                 
  Author(s)        :  Amarnath Shastry                                        
  Date Created     :  07-05-2002
  Modification     :  10-12-2002, Amarnath Shastry
                      If message length is reduced than previous value,
                      then user is thrown a message which, gets delete
                      confirmation to delete the redundent signals
                      
                      03-03-2003 Rajesh Kumar
                      1. User confirmation for Frame Format change removed
                         as MsgCode conversion is not done now
                      2. Validation of MsgCode when FrameFormat = Standard  
  Modification      : Raja N on 10.03.2004
                      Modified to refer inactive database structure for
                      editor related operation
*****************************************************************************/
void CMessageDetailsDlg::OnOK() 
{
    BOOL bRetVal = TRUE;
    CString omStrDummy;
    UpdateData(TRUE);

    GetDlgItem(IDC_EDIT_MSG_LENGTH)->GetWindowText(omStrDummy);
    GetDlgItem(IDC_EDIT_MSG_NAME)->GetWindowText(m_omStrMessageName);

    if ( omStrDummy.IsEmpty() )
    {
        bRetVal = FALSE;
    }
    else
    {
        UpdateData(TRUE);
        if ( m_unMessageLength > m_sDbParams.m_unMaxMsgLen )
        {
            CString omErr;
            omErr.Format("Please enter integer between 0 and %d", m_sDbParams.m_unMaxMsgLen);
            AfxMessageBox(omErr.GetBuffer(MAX_PATH));
            GetDlgItem(IDC_EDIT_MSG_LENGTH)->SetFocus();
            bRetVal = FALSE;
        }
    }

    if (bRetVal == TRUE &&
        m_omStrMessageName.IsEmpty())
    {
        AfxMessageBox("Message Name cannot be empty!",
            MB_OK|MB_ICONINFORMATION);
        GetDlgItem(IDC_EDIT_MSG_NAME)->SetFocus();
        bRetVal = FALSE;
    }

    if ( bRetVal == TRUE &&
         m_omStrMessageCode.IsEmpty())
    {
        CString omMsg = _T(" cannot be empty!");
        omMsg = m_sDbParams.m_omIdFieldName + omMsg;
        AfxMessageBox(omMsg.GetBuffer(MAX_PATH),
            MB_OK|MB_ICONINFORMATION);
        GetDlgItem(IDC_EDIT_MSGCODE)->SetFocus();
        bRetVal = FALSE;
    }
    
    if (bRetVal)
    {
        // check if the value is valid hex number
        CString omStrMsgCode = m_omStrMessageCode;
        m_omStrMessageCode.MakeUpper();
        
        for ( int nCount = 0; nCount < m_omStrMessageCode.GetLength(); nCount++ )
        {
            TCHAR tChar = m_omStrMessageCode.GetAt( nCount );

            if ( !((tChar >= 'A') && ( tChar <= 'F' ) ||
                 (tChar >= '0' && tChar <= '9') ) )
            {
                AfxMessageBox("Invalid message ID!", 
                    MB_OK|MB_ICONINFORMATION );
                m_omStrMessageCode.Empty();
                GetDlgItem( IDC_EDIT_MSGCODE )->SetFocus();
                UpdateData( FALSE);
                return;
            }
            
        }
        
        // rajesh: 03-03-2003: BEGIN: check range based on FrameFormat
        //CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
        UINT unTempMsgCode =
            static_cast <UINT> (strtol((LPCTSTR )m_omStrMessageCode,NULL, 16) );

        if (m_sDbParams.m_eBus == CAN)
        {
            if((m_nFrameFormat == 0) && (unTempMsgCode >= MAX_LMT_FOR_STD_MSG_ID))    // standard frame
            {
                AfxMessageBox("Invalid message ID!", 
                   MB_OK|MB_ICONINFORMATION );
                m_omStrMessageCode.Empty();
                GetDlgItem( IDC_EDIT_MSGCODE )->SetFocus();
                UpdateData( FALSE);
                return;         
            }
        }
        else if (m_sDbParams.m_eBus == J1939)
        {
            if (unTempMsgCode > MAX_LMT_FOR_PGN)
            {
                AfxMessageBox("Invalid message ID!", 
                   MB_OK|MB_ICONINFORMATION );
                m_omStrMessageCode.Empty();
                GetDlgItem( IDC_EDIT_MSGCODE )->SetFocus();
                UpdateData( FALSE);
                return;  
            }
        }
        // rajesh: 03-03-2003: END: 

        m_omStrMessageCode = omStrMsgCode;

        CMsgSignal* pTempMsgSg = NULL;

        pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);

        // Check if the entered messsage name is a duplacate
        BOOL bDuplicateFound = FALSE;

        if ( m_psMsgStuctPtr != NULL )
        {
            // the following line of code is moved from the outer block to get
            // rid of the crash when m_psMsgStuctPtr is NULL

            if ( m_psMsgStuctPtr->m_omStrMessageName != m_omStrMessageName)
            {
                bDuplicateFound = 
                    pTempMsgSg->bIsDuplicateMessageName( 
                                m_psMsgStuctPtr->m_unMessageCode,
                                m_omStrMessageName );
            }
            // Now that if the user reduces the message length,
            // then delete the signals defined whch are redundent
            if ( m_psMsgStuctPtr->m_unMessageLength > m_unMessageLength)
            {
                // See if signals are defined
                if ( m_psMsgStuctPtr->m_unNumberOfSignals > 0 )
                {
                    // Get the delete confirmation from the user
                    if ( AfxMessageBox(
_T("You have signals defined for this message for previous message length.\n\
Reducing message length will delete redundent signals and associated description\n\
defined for this message.Do you want to delete these signals?\n\n\
Select \"Yes\" to delete the signal(s) and to accept new message length value.\n\
Select \"No\" to retain the previous message length."), MB_YESNO) == IDYES)
                    {
                        // Delete redundent signals and update new message length
                        if ( bDeleteRedundentSignals() == FALSE )
                        {
                            AfxMessageBox(_T("Could not delete the signals..."),
                                MB_OK);
                        }
                    }
                    else
                    {
                        // Retain old message length value
                        m_unMessageLength = m_psMsgStuctPtr->m_unMessageLength;
                    }
                }
            }
        }
        else // 2. Don't know the code
        {
            sMESSAGE* pMsgPtr = 
                pTempMsgSg->psGetMessagePointerInactive( m_omStrMessageName );

            // if this pointer exists, it means that message with 
            // the same name exists
            if ( pMsgPtr != NULL)
            {
                bDuplicateFound = TRUE;
            }
        }

// *** MODIFICATION : END ***

        // Display message if duplicate is found
        if (bDuplicateFound == TRUE)
        {
            AfxMessageBox("Duplicate message name found!", 
                MB_OK|MB_ICONINFORMATION);
            GetDlgItem(IDC_EDIT_MSG_NAME)->SetFocus();
            bRetVal = FALSE;
        }
        else
        {
            // Get form view pointer
            CMainFrame* pMainFrm = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
            int nMsgCode = -1;
            if ( pMainFrm != NULL )
            {
                nMsgCode = 
                    static_cast <INT> (strtol((LPCTSTR )m_omStrMessageCode,
                             NULL, 16) );
            }

            // Validate message code for duplication
            // 1. Edit mode
            if ( m_psMsgStuctPtr != NULL )
            {
                bDuplicateFound = 
                    pTempMsgSg->bIsDuplicateMessageCode( 
                    m_psMsgStuctPtr->m_omStrMessageName,
                    nMsgCode);
            }
            else
            {
                // Add mode, if message name coressponding to the message
                // code is not empty, then their exists a message 
                // with given code
                CString omStr = 
                    pTempMsgSg->omStrGetMessageNameFromMsgCodeInactive( nMsgCode);

                if (!omStr.IsEmpty())
                {
                    bDuplicateFound = TRUE;
                }
            }

            if (bDuplicateFound == TRUE)
            {
                AfxMessageBox("Duplicate message code found!", 
                    MB_OK|MB_ICONINFORMATION);
                GetDlgItem(IDC_EDIT_MSGCODE)->SetFocus();
                bRetVal = FALSE;
            }
            else
            {

                CString omStrPrevMsg = STR_EMPTY;

                // if this is add mode, allocate
                // memory for the new message
                if ( m_psMsgStuctPtr == NULL )
                {
                    // Add message to the data structure
                    pTempMsgSg->bAddMsg();
                }
                else
                {
                    omStrPrevMsg = m_psMsgStuctPtr->m_omStrMessageName;
                }
        
                // In any case u got to update the dialog data
                // to the data structure

                // Get message details
                sMESSAGE od_Temp;

                od_Temp.m_bMessageFrameFormat   = m_nFrameFormat;
                od_Temp.m_nMsgDataFormat        = DATA_FORMAT_INTEL;
                od_Temp.m_omStrMessageName      = m_omStrMessageName;
                od_Temp.m_psSignals             = NULL;
                od_Temp.m_unMessageLength       = m_unMessageLength;


                if ( pMainFrm != NULL )
                {
                    od_Temp.m_unMessageCode         = 
                        static_cast <UINT> ( strtol((LPCTSTR )m_omStrMessageCode,
                                                        NULL, 
                                                        16)      );
                }
                
                // Update the data
                if (( bRetVal = pTempMsgSg->bUpdateMsg( MSG_NAME, 
                                                  omStrPrevMsg,
                                                  &od_Temp)) != FALSE )
                {

                    if (( bRetVal = pTempMsgSg->bUpdateMsg( MSG_CODE, 
                                           m_omStrMessageName,
                                           &od_Temp)) != FALSE )
                    {


                        if ((bRetVal = pTempMsgSg->bUpdateMsg( MSG_LENGTH, 
                                                  m_omStrMessageName,
                                                  &od_Temp)) != FALSE )
                        {
                            if ((bRetVal = pTempMsgSg->bUpdateMsg( MSG_FRAME_FORMAT, 
                                                                   m_omStrMessageName,
                                                                   &od_Temp)) != FALSE )
                            {
                                bRetVal = 
                                    pTempMsgSg->bUpdateMsg( MSG_DATA_FORMAT, 
                                                            m_omStrMessageName,
                                                            &od_Temp);
                            }

                        }
                    }
                }

                if ( !bRetVal )
                {
                    AfxMessageBox("Could not update the changes..!",
                        MB_OK|MB_ICONERROR );
                }
            }
        }
    }

    if ( bRetVal == TRUE )
        CDialog::OnOK();
}
/******************************************************************************
  Function Name    :  OnCancel
  Input(s)         :                                                        
  Output           :                                                        
  Functionality    :  Calls base class functionality.
  Member of        :  CMessageDetailsDlg                                            
  Friend of        :      -                                                 
  Author(s)        :  Amarnath Shastry                                        
  Date Created     :  07-05-2002                                            
*****************************************************************************/
void CMessageDetailsDlg::OnCancel() 
{
    // TODO: Add extra cleanup here
    
    CDialog::OnCancel();
}
/******************************************************************************
  Function Name    :  vConvertToExtended
  Input(s)         :                                                        
  Output           :                                                        
  Functionality    :  Function converts standard to extended frame format.
                      Changes the value of the code if necessary.
  Member of        :  CMessageDetailsDlg                                            
  Friend of        :      -                                                 
  Author(s)        :  Amarnath Shastry                                        
  Date Created     :  09-05-2002                                            
  Modification      : Raja N on 10.03.2004
                      Modified to refer inactive database structure for
                      editor related operations
*****************************************************************************/
void CMessageDetailsDlg::vConvertToExtended()
{
    CString omStrsgCode = m_omStrMessageCode;

    if ( IDYES == AfxMessageBox(MSG_PROMPT_EXTD, MB_YESNO|MB_ICONINFORMATION))
    {
        CMsgSignal* pTempMsgSg = NULL;

        pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);

        // Get message code
        //CMainFrame* pMainFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;

        // Get dec equivalent of hex
        int nMsgCode = atoi( m_omStrMessageCode );

        // convert the message code to extended frame format
        pTempMsgSg->vConvertStandardToExtendedFrameFormat( nMsgCode );

        // return value is in dec, convert to hex
        m_omStrMessageCode.Format( "%x", nMsgCode );

    }
    else
    {
        // retain the old frame format value
        m_nFrameFormat      = 0;
        m_omStrMessageCode  = omStrsgCode;

    }

    // show the new message code in the edit
    UpdateData(FALSE);

}
/******************************************************************************
  Function Name    :  vConvertToStandard
  Input(s)         :                                                        
  Output           :                                                        
  Functionality    :  Function converts extended to standard frame format.
                      Changes the value of the code if necessary.
  Member of        :  CMessageDetailsDlg                                            
  Friend of        :      -                                                 
  Author(s)        :  Amarnath Shastry                                        
  Date Created     :  09-05-2002                                            
  Modification      : Raja N on 10.03.2004
                      Modified to refer inactive database structure for
                      editor related operations
*****************************************************************************/

void CMessageDetailsDlg::vConvertToStandard()
{
    CString omStrsgCode = m_omStrMessageCode;

    if ( IDYES == AfxMessageBox(MSG_PROMPT_STD, MB_YESNO|MB_ICONINFORMATION))
    {
        CMsgSignal* pTempMsgSg = NULL;

        pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);

        // Get message code
        //CMainFrame* pMainFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;

        // Get dec equivalent of hex
        int nMsgCode = atoi( m_omStrMessageCode );

        // convert the message code to extended frame format
        pTempMsgSg->vConvertExtendedToStandardFrameFormat( nMsgCode );

        // return value is in dec, convert to hex
        m_omStrMessageCode.Format( "%x", nMsgCode );

    }
    else
    {
        m_nFrameFormat      = 1;
        m_omStrMessageCode  = omStrsgCode;
    }

    // show the new message code in the edit
    UpdateData(FALSE);

}
/******************************************************************************
  Function Name    :  PreTranslateMessage
  Input(s)         :  MSG* pMsg                                                      
  Output           :                                                        
  Functionality    :  Prevents the space character in the name and code 
                      edit control to be processed as it is not recommended
  Member of        :  CMessageDetailsDlg                                            
  Friend of        :      -                                                 
  Author(s)        :  Amarnath Shastry                                        
  Date Created     :  09-05-2002                                            
*****************************************************************************/
BOOL CMessageDetailsDlg::PreTranslateMessage(MSG* pMsg) 
{
    BOOL bSkip = FALSE;
    if ( pMsg->message == WM_CHAR )
    {
        // if the control is message length,
        // allow only numbersfrom 0-8 and 
        // backspace only
        CEdit* omEditCtrlLen   = (CEdit*) GetDlgItem(IDC_EDIT_MSG_LENGTH);
        CEdit* omEditFocusLen  = (CEdit*)GetFocus();
        if ( omEditCtrlLen == omEditFocusLen)
        {
            if ( (pMsg->wParam >= '0' && pMsg->wParam <= '8') ||
                  pMsg->wParam == 0x08)
            {
                bSkip = FALSE;
            }
            else
            {
                bSkip = TRUE;
            }
        }

        // If the control is message name and code
        // do not allow space characters 
        if ( pMsg->wParam == ' ')
        {
            CEdit* omEditCtrlName   = (CEdit*) GetDlgItem(IDC_EDIT_MSG_NAME);
            CEdit* omEditFocusName  = (CEdit*)GetFocus();

            CEdit* omEditCtrlCode   = (CEdit*) GetDlgItem(IDC_EDIT_MSGCODE);
            CEdit* omEditFocusCode  = (CEdit*)GetFocus();

            // Shud not process space characters in Name and Code
            // Edit Control only
            if ( omEditCtrlName == omEditFocusName ||
                 omEditCtrlCode == omEditFocusCode)
            {
                bSkip = TRUE;
            }
        }
    }

    if ( bSkip == FALSE)
        bSkip = CDialog::PreTranslateMessage(pMsg);

    return bSkip;
}
/******************************************************************************
  Function Name    :  bDeleteRedundentSignals
  Input(s)         :  -                                                      
  Output           :  BOOL                                                     
  Functionality    :  Deletes redundent signals if the user reduces the message 
                      length and if he  has defined signals in that range
  Member of        :  CMessageDetailsDlg                                            
  Friend of        :      -                                                 
  Author(s)        :  Amarnath Shastry                                        
  Date Created     :  10-12-2002                                            
  Modification      : Raja N on 10.03.2004
                      Modified to refer inactive database structure for
                      editor related operations
*****************************************************************************/
BOOL CMessageDetailsDlg::bDeleteRedundentSignals()
{
    BOOL bReturnValue = TRUE;
    // Here suppose that prev message length = 5,
    // and now he has reduced to 4, then 
    // only the redundent signal(s) that was defined in
    // index 5 will be deleted

    // Get appropriate structure pointer
    CMsgSignal* pTempMsgSg = NULL;
    pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);

    // List has signal names to be deleted
    CStringArray strSignalList;
    // List has signal index to be deleted
    UINT unIndexes[64];

    if ( pTempMsgSg != NULL )
    {
        // Get signal names and their indexes to be deleted
        pTempMsgSg->vGetSigNamesAndIndexTobeDeleted( m_unMessageLength,
            m_omStrMessageName,
            strSignalList,
            unIndexes);

        for ( int unCount = 0;
        unCount < strSignalList.GetSize(); unCount++)
        {
            bReturnValue = 
                pTempMsgSg->bDeleteSignalFromMsg(
                    unIndexes[unCount], // Zero based position of signal
                    m_omStrMessageName,
                    strSignalList.GetAt( unCount) );
        }
    }

    return (bReturnValue);

}
