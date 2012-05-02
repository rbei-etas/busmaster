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
 * \file      SetResetTimer.cpp
 * \brief     This file contain definition of all function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */
// Include all standard header files
#include "NodeSimEx_stdafx.h"
// CSetResetTimer class definition
#include "SetResetTimer.h"
//accessin manager class object
#include "ExecuteManager.h"
#include "SimSysManager.h"

CEvent omMainTimerEvent;
//Timer callback for timer handlers
//Bus Type
ETYPE_BUS CSetResetTimer::sm_eBus = CAN;
void CALLBACK TimerProc(UINT uTimerID, UINT uMsg, DWORD* dwUser,
                        DWORD* dw1, DWORD* dw2);

UINT CalcTimersExecTime(LPVOID pParam);

/******************************************************************************/
/*  Function Name    :  CSetResetTimer                                        */
/*  Input(s)         : pointer to timer structure and pointer to CWnd class   */
/*  Output           :                                                        */
/*  Functionality    :  This is default constructor. This will initialise     */
/*                      pointer to structure m_asTimer with address passed    */
/*                      as parameter                                          */
/*                                                                            */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  04.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  08.03.2002                                            */
/******************************************************************************/
CSetResetTimer::CSetResetTimer(ETYPE_BUS eBus, CWnd* pParent /*=NULL*/)
    : CDialog(CSetResetTimer::IDD, pParent),
      m_bDialogGettingInit(TRUE),
      m_bApplyButtonPressed(FALSE),
      m_psFirstNodeTimerList(NULL),
      m_psLastNodeTimerList(NULL),
      m_psSelectedNodeTimerListPtr(NULL),
      m_nSelectedNode(0),
      m_bNodeNameChanged(FALSE),
      m_psCalTimerThreadStruct(NULL),
      m_unTimerID(0)
{
    m_eBus = eBus;
    //{{AFX_DATA_INIT(CSetResetTimer)
    m_omStrNodeName = "";
    //}}AFX_DATA_INIT
    //   m_omApptEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
}

/******************************************************************************/
/*  Function Name    :  DoDataExchange                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by frame work to exchange and */
/*                      validate the values of data members associated with   */
/*                      controls of dialog box.                               */
/*                                                                            */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  28.02.2002                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
void CSetResetTimer::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSetResetTimer)
    DDX_Control(pDX, IDC_COMB_NODENAME, m_omComboNodeName);
    DDX_Control(pDX, IDC_LSTC_TIMER_HANDLER, m_omTimerList);
    DDX_CBString(pDX, IDC_COMB_NODENAME, m_omStrNodeName);
    DDV_MaxChars(pDX, m_omStrNodeName, 20);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetResetTimer, CDialog)
    //{{AFX_MSG_MAP(CSetResetTimer)
    ON_WM_HELPINFO()
    ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
    ON_BN_CLICKED(IDC_CHCK_MONOSHOT, OnChckMonoshot)
    ON_EN_UPDATE(IDC_EDIT_TIMER_VALUE, OnUpdateEditTimerValue)
    ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_TIMER_HANDLER, OnItemchangedLstcTimerHandler)
    ON_CBN_SELCHANGE(IDC_COMB_NODENAME, OnSelchangeNodeList)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by the framework in response  */
/*                      to the WM_INITDIALOG message. This message is sent to */
/*                      the dialog box during DoModal calls,which occur       */
/*                      immediately before the dialog box is displayed.       */
/*                      All controls of dialog are initialised in this func.  */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  04.03.2002                                            */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      20.10.2003, Modified to make this dialog modaless and */
/*                      added new controls.                                   */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      03.11.2003, The loop breaking flag bEndLoop is introd */
/*                      uced to ensure array index going beyond limit.        */
/*  Modifications    :  Raja N.                                               */
/*                      14.01.2004, The loop breaking at boundry condition was*/
/*                      modified to ensure no empty items are shown.          */
/*  Modifications    :  Anish                                                 */
/*                      14.01.2006, Node name combo box is populated          */
/*                      Changed are made for populating timer list for        */
/*                      multinode                                             */
/******************************************************************************/
BOOL CSetResetTimer::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_bDialogGettingInit = TRUE;
    m_omComboNodeName.EnableWindow(TRUE);
    m_omComboNodeName.ResetContent();

    if(FALSE==m_bSimSysNodes)
    {
        PSNODETIMERLIST psCurrTimerListPtr=m_psFirstNodeTimerList;

        while(psCurrTimerListPtr!=NULL)
        {
            m_omComboNodeName.AddString(psCurrTimerListPtr->omNodeName);
            psCurrTimerListPtr=psCurrTimerListPtr->psNextTimerListPtr;
        }
    }
    else
    {
        int nNoOfNodes=(COMMANINT)m_omSysNodeName.GetSize();

        //if the no. of nodes is more than one populate the combo box
        //and leave it enabled
        if(nNoOfNodes > 1)
        {
            int nCurrNode=0;

            while(nCurrNode<nNoOfNodes)
            {
                CString omNodeName=m_omSysNodeName.GetAt(nCurrNode);
                m_omComboNodeName.AddString(omNodeName);
                nCurrNode++;
            }
        }
        else
        {
            CString omNodeName=m_omSysNodeName.GetAt(0);
            m_omComboNodeName.AddString(omNodeName);
            m_omComboNodeName.SetCurSel(0);
            m_omComboNodeName.EnableWindow(FALSE);
        }
    }

    // Set the style and insert column.
    m_omTimerList.SetExtendedStyle(LVS_EX_CHECKBOXES);
    m_omTimerList.InsertColumn(0,defTIMER_HEADER_NAME,LVCFMT_CENTER,-1,-1);

    if(m_psSelectedNodeTimerListPtr==NULL&&m_psFirstNodeTimerList!=NULL)
    {
        CString omNodeName;
        m_omComboNodeName.GetLBText(0,omNodeName);
        //get the corressponding node's timer list pointer
        BOOL bFound=FALSE;
        PSNODETIMERLIST psCurrTimerListPtr=m_psFirstNodeTimerList;

        while(psCurrTimerListPtr!=NULL&&!bFound)
        {
            if(omNodeName==psCurrTimerListPtr->omNodeName)
            {
                bFound=TRUE;
            }
            else
            {
                psCurrTimerListPtr=psCurrTimerListPtr->psNextTimerListPtr;
            }
        }

        if(bFound)
        {
            m_psSelectedNodeTimerListPtr= psCurrTimerListPtr->psTimerListPtr;
            m_nSelectedNode=0;
        }

        if(TRUE==m_bSimSysNodes)
        {
            BOOL bFound=FALSE;
            CString omNodeName=m_omSysNodeName.GetAt(0);
            PSNODETIMERLIST psCurrTimerListPtr=m_psFirstNodeTimerList;

            while(psCurrTimerListPtr!=NULL&&!bFound)
            {
                if(omNodeName==psCurrTimerListPtr->omNodeName)
                {
                    bFound=TRUE;
                }
                else
                {
                    psCurrTimerListPtr=psCurrTimerListPtr->psNextTimerListPtr;
                }
            }

            m_psSelectedNodeTimerListPtr= psCurrTimerListPtr->psTimerListPtr;
            m_nSelectedNode=0;
        }

        m_omComboNodeName.SetCurSel(0);
    }

    PSTIMERHANDLERLIST ps_CurrTimeHandler=m_psSelectedNodeTimerListPtr;
    m_omComboNodeName.SetCurSel(m_nSelectedNode);
    INT nCountTimer = 0;
    // If there is timer handler function defined, display the function name
    // and set the selected to default or last selected. If not then disable
    // the subsequent controls for timer and its type.
    INT nColumnWidth = 0;
    INT nTemp = 0;

    while(ps_CurrTimeHandler!=NULL)
    {
        m_omTimerList.InsertItem(nCountTimer,
                                 ps_CurrTimeHandler->sTimerHandler.omStrTimerHandlerName);
        m_omTimerList.SetCheck(nCountTimer,
                               ps_CurrTimeHandler->sTimerHandler.bTimerSelected);

        // If it is Mono shot timer, make the timer ID as NULL.
        if(ps_CurrTimeHandler->sTimerHandler.bTimerType !=FALSE)
        {
            ps_CurrTimeHandler->sTimerHandler.bTimerType = FALSE;
        }

        // get the width of cloumn from length of item string inserted.
        nTemp = m_omTimerList.GetStringWidth(
                    ps_CurrTimeHandler->sTimerHandler.omStrTimerHandlerName);

        // Get the maximum colunm width.
        if(nTemp >nColumnWidth)
        {
            nColumnWidth = nTemp;
        }

        nCountTimer++;
        ps_CurrTimeHandler=ps_CurrTimeHandler->psNextTimer;
    }

    // Set the clumn width to maximum string size + check box image size.
    m_omTimerList.SetColumnWidth(0,nColumnWidth + defSIZE_OF_IMAGE);
    // Set the focus and selection to first item.
    int nReturn = m_omTimerList.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED,
                  LVIS_SELECTED|LVIS_FOCUSED);

    // Update the controls with selected timer attributes.
    if(nReturn >0)
    {
        vUpdateTimerInformation(m_psSelectedNodeTimerListPtr,0);
    }

    // Remove the selection from list control items and disable
    // the Apply button only if the timer is cyclic and
    nReturn = m_omTimerList.GetNextItem(-1, LVIS_SELECTED) ;

    if( nReturn != -1)
    {
        PSTIMERHANDLER ps_CurrTimeHandler=GetPointerOfHandler(
                                              m_psSelectedNodeTimerListPtr,nReturn);

        //if found
        if(ps_CurrTimeHandler!=NULL&&ps_CurrTimeHandler->bTimerType!=FALSE)
        {
            GetDlgItem(IDC_BTN_APPLY)->EnableWindow(FALSE);
        }
    }
    else
    {
        GetDlgItem(IDC_BTN_APPLY)->EnableWindow(TRUE);
    }

    m_bDialogGettingInit = FALSE;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
/******************************************************************************/
/*  Function Name    :  OnBtnOk                                                  */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by frame work when user press */
/*                      OK button. The selection information is stored in the */
/*                      element of structure.                                 */
/*                                                                            */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  04.03.2002                                            */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      20.10.2003, Modified to make this dialog modaless and */
/*                      added new controls.                                   */
/******************************************************************************/
void CSetResetTimer::OnBtnOk()
{
    CExecuteManager::ouGetExecuteManager(m_eBus).
    vUpdateTimersFromUser(m_psFirstNodeTimerList);
    DestroyWindow();
    CSimSysManager::ouGetSimSysManager(m_eBus).
    ouGetFlags().vSetFlagStatus(H_TIMERBUTTON,FALSE);
}
/******************************************************************************/
/*  Function Name    :  OnHelpInfo                                            */
/*  Input(s)         :  Pointer to object of structure for help request info. */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by the framework in response  */
/*                      to the WM_HELPINFO message. This message is sent to   */
/*                      the dialog box during help request from user.         */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  04.03.2002                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
BOOL CSetResetTimer::OnHelpInfo(HELPINFO* pHelpInfo)
{
    //theApp.vSetHelpID(pHelpInfo->dwContextId);
    return CDialog::OnHelpInfo(pHelpInfo);
}
/******************************************************************************/
/*  Function Name    :  OnBtnApply                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will be called by frame work when Apply */
/*                      button is pressed.                                    */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  06.10.2003                                            */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      19.01.2004, removed Enable/Disable controls on Apply  */
/******************************************************************************/
void CSetResetTimer::OnBtnApply()
{
    vSaveUpdatedTimerInformation();
    CExecuteManager::ouGetExecuteManager(m_eBus).
    vUpdateTimersFromUser(m_psFirstNodeTimerList);
    m_bApplyButtonPressed = TRUE;
    INT  nTimerIndex = 0;
    // Remove the selection from list control items and disable
    // the Apply button.
    nTimerIndex = m_omTimerList.GetNextItem(-1,LVIS_SELECTED);

    if(nTimerIndex != -1 )
    {
        m_omTimerList.SetItemState(nTimerIndex, 0,LVIS_SELECTED) ;
    }

    GetDlgItem(IDC_BTN_APPLY)->EnableWindow(FALSE);
}
/******************************************************************************/
/*  Function Name    :  OnChckMonoshot                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :   This function will be called by frame work when user */
/*                       changes the check/uncheck condition of check box.    */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  06.10.2003                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
void CSetResetTimer::OnChckMonoshot()
{
    vSaveUpdatedTimerInformation();
    GetDlgItem(IDC_BTN_APPLY)->EnableWindow(TRUE);
}
/******************************************************************************/
/*  Function Name    :  OnUpdateEditTimerValue                                */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    : This will be called by framework whenever user changes */
/*                     edit box value. If the value is valid it is saved else */
/*                     focus is shifted to same edit box with default value.  */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  06.10.2003                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
void CSetResetTimer::OnUpdateEditTimerValue()
{
    UINT unTimerVal = GetDlgItemInt(IDC_EDIT_TIMER_VALUE, 0, TRUE);

    // If value is greater then zero save it
    if(unTimerVal>=1)
    {
        vSaveUpdatedTimerInformation();
    }
    else// Set the focus to edit control and initialise it with last saved value
    {
        CString omNodeName;
        //Get the node selected in combobox
        int nCurSel = m_omComboNodeName.GetCurSel();

        if (nCurSel != CB_ERR)
        {
            m_omComboNodeName.GetLBText(nCurSel,omNodeName);
        }

        //get the corressponding node's timer list pointer
        BOOL bFound=FALSE;
        PSNODETIMERLIST psCurrTimerListPtr=m_psFirstNodeTimerList;

        while(psCurrTimerListPtr!=NULL&&!bFound)
        {
            if(omNodeName==psCurrTimerListPtr->omNodeName)
            {
                bFound=TRUE;
            }
            else
            {
                psCurrTimerListPtr=psCurrTimerListPtr->psNextTimerListPtr;
            }
        }

        CEdit* pomTimerValue = (CEdit*)GetDlgItem(IDC_EDIT_TIMER_VALUE);

        if(pomTimerValue != NULL)
        {
            pomTimerValue->SetFocus();
            int nSelectedItem = m_omTimerList.GetNextItem(-1, LVNI_SELECTED);

            if(nSelectedItem != -1&&bFound)
            {
                PSTIMERHANDLER ps_CurrTimeHandler=GetPointerOfHandler(
                                                      psCurrTimerListPtr->psTimerListPtr,nSelectedItem);

                if(ps_CurrTimeHandler!=NULL)
                {
                    SetDlgItemInt(IDC_EDIT_TIMER_VALUE,
                                  ps_CurrTimeHandler->unTimerVal,
                                  FALSE );
                }
            }
        }
    }

    GetDlgItem(IDC_BTN_APPLY)->EnableWindow(TRUE);
}
/******************************************************************************/
/*  Function Name    :  vSaveUpdatedTimerInformation                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  It will save user changes to timer attribute in member*/
/*                      variable m_asTimer. The index of item selected will be*/
/*                      taken as index for updation.                          */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  06.10.2003                                            */
/*  Modification by  :  Anish kumar                                           */
/*  Modifications    :  14.01,06 ,changes due to node combo box               */
/******************************************************************************/
void CSetResetTimer::vSaveUpdatedTimerInformation()
{
    PSTIMERHANDLERLIST psTimerList=m_psSelectedNodeTimerListPtr;

    if(psTimerList!=NULL)
    {
        CButton* pomTimerType;
        int nSelectedItem = m_omTimerList.GetNextItem(-1, LVNI_SELECTED);

        if( nSelectedItem != -1)
        {
            pomTimerType    = (CButton*)GetDlgItem(IDC_CHCK_MONOSHOT);
            PSTIMERHANDLER ps_CurrTimeHandler=GetPointerOfHandler(
                                                  psTimerList,nSelectedItem);

            if(ps_CurrTimeHandler!=NULL)
            {
                ps_CurrTimeHandler->unTimerVal =
                    GetDlgItemInt(IDC_EDIT_TIMER_VALUE, 0, FALSE);

                if( pomTimerType != NULL)
                {
                    ps_CurrTimeHandler->bTimerType = pomTimerType->GetCheck();
                }

                ps_CurrTimeHandler->bTimerSelected =
                    m_omTimerList.GetCheck(nSelectedItem);
                ps_CurrTimeHandler->unCurrTime=0;
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  vUpdateTimerInformation                               */
/*  Input(s)         :  int : nSelectedItem : index of selected item          */
/*  Output           :                                                        */
/*  Functionality    : This function will update the timer attribute to       */
/*                     and enable or disable it based on if timer is running  */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  06.10.2003                                            */
/*  Modifications    :   Anish kumar                                          */
/*                       23.12.05 remove condition to check whether timer is  */
/*                       started or not                                       */
/******************************************************************************/
void CSetResetTimer::vUpdateTimerInformation(PSTIMERHANDLERLIST ps_CurrTimeHandl
        ,int nSelectedItem)
{
    if(ps_CurrTimeHandl!=NULL)
    {
        CButton* pomTimerValue = (CButton*)GetDlgItem(IDC_EDIT_TIMER_VALUE);
        CButton* pomTimerType =  (CButton*)GetDlgItem(IDC_CHCK_MONOSHOT);
        PSTIMERHANDLER ps_CurrTimeHandler;

        if(pomTimerType  != NULL)
        {
            ps_CurrTimeHandler=GetPointerOfHandler(ps_CurrTimeHandl,
                                                   nSelectedItem);
            pomTimerType ->SetCheck(ps_CurrTimeHandler->bTimerType);
            // If timer timer ID is not NULL -> timer is
            // running or waiting for trigger. Don't enable the controls.
            pomTimerType ->EnableWindow(TRUE);
        }

        if(pomTimerValue != NULL)
        {
            SetDlgItemInt(IDC_EDIT_TIMER_VALUE,
                          ps_CurrTimeHandler->unTimerVal,
                          TRUE);
            // If timer timer ID is not NULL -> timer is
            // running or waiting for trigger. Don't enable the controls.
            //ani1
            pomTimerValue->EnableWindow(FALSE);
        }
    }
}


/******************************************************************************/
/*  Function Name    :  OnCancel                                              */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :   This function is called when user press cancel button*/
/*                       or click on 'X'.                                     */
/*                                                                            */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  06.10.2003                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
void CSetResetTimer::OnCancel()
{
    DestroyWindow();
}
/******************************************************************************/
/*  Function Name    :  DestroyWindow                                         */
/*  Input(s)         :                                                        */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  This is called when the dialog is destroyed.          */
/*                                                                            */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  06.10.2003                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
BOOL CSetResetTimer::DestroyWindow()
{
    m_psSelectedNodeTimerListPtr=NULL;
    m_nSelectedNode=0;
    // Set the flag to FALSE to indicate the dialog is closed.
    CSimSysManager::ouGetSimSysManager(m_eBus).
    ouGetFlags().vSetFlagStatus(H_TIMERBUTTON,FALSE);
    return CDialog::DestroyWindow();
}

/******************************************************************************/
/*  Function Name    :  OnItemchangedLstcTimerHandler                         */
/*  Input(s)         :  NMHDR* pNMHDR,                                        */
/*  Output           :  LRESULT* pResult                                      */
/*  Functionality    :  This is message mapped function assocaited with list  */
/*                      control. The function is called br framework when ever*/
/*                      there is change in any if items in list control.      */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.10.2003                                            */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      19.01.2004,  Enable/Disable controls if the selection */
/*                                   is removed from items.                   */
/*  Modifications    :  Anish                                                 */
/*                      14.01.2006, Node name combo box is populated          */
/*                      Changed are made for populating timer list for        */
/*                      multinode                                             */
/******************************************************************************/
void CSetResetTimer::OnItemchangedLstcTimerHandler(NMHDR* pNMHDR,
        LRESULT* pResult)
{
    PSTIMERHANDLERLIST psTimerList=m_psSelectedNodeTimerListPtr;

    // If dialog is getting initialised and list control item is inserted
    // then don't execute the code. In that case also this handler will be
    // called
    if(m_bDialogGettingInit == FALSE && m_bNodeNameChanged == FALSE)
    {
        NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
        UINT unCurrentState = 0;

        // If Apply button is pressed, initialised the member list
        // with the timer list of all nodes,they
        // will have updated information about the controls.
        if(m_bApplyButtonPressed == TRUE)
        {
            CExecuteManager::ouGetExecuteManager(m_eBus).
            vCopyTimersInSetResetMemory(m_psFirstNodeTimerList);
            m_bApplyButtonPressed = FALSE;
        }

        // Get the current state i.e new state of the list control items
        // and check for if it is checked, unchecked, selected and focused.
        unCurrentState = pNMListView->uNewState & defMASK_LIST_CTRL;

        //  unCurrentState=unCurrentState&defMASK_NODE_COMBO;
        if(  unCurrentState != FALSE
                &&  pNMListView->uChanged  == LVIF_STATE )
        {
            // Enable apply button.
            GetDlgItem(IDC_BTN_APPLY)->EnableWindow(TRUE);
            // check is the item is focused and selected.
            unCurrentState = pNMListView->uNewState & (LVIS_FOCUSED |LVIS_SELECTED);
            // If yes, update the timer information for newly selected item.

            if(unCurrentState != FALSE)
            {
                vUpdateTimerInformation(psTimerList,pNMListView->iItem);
            }

            // if it is checked or unchecked, get the condition of the item
            // and store it in m_asTimer member variable.
            unCurrentState = pNMListView->uNewState & defMASK_CHECK_UNCHECK;

            //unCurrentState=unCurrentState&defMASK_NODE_COMBO;
            if(unCurrentState != FALSE )
            {
                PSTIMERHANDLER ps_CurrTimeHandler=
                    GetPointerOfHandler(psTimerList,pNMListView->iItem);

                if(ps_CurrTimeHandler!=NULL)
                {
                    ps_CurrTimeHandler->bTimerSelected =
                        m_omTimerList.GetCheck(pNMListView->iItem);
                    ps_CurrTimeHandler->unCurrTime=0;
                }
            }
        }

        CButton* pomTimerValue = (CButton*)GetDlgItem(IDC_EDIT_TIMER_VALUE);
        CButton* pomTimerType =  (CButton*)GetDlgItem(IDC_CHCK_MONOSHOT);

        if(pNMListView->uNewState == 0 )
        {
            if(pomTimerType  != NULL && pomTimerValue  != NULL )
            {
                pomTimerType->EnableWindow(TRUE);
                pomTimerValue->EnableWindow(TRUE);
            }
        }
        else
        {
            INT nIndex = m_omTimerList.GetNextItem(-1,LVIS_SELECTED);
            PSTIMERHANDLER ps_CurrTimeHandler=GetPointerOfHandler(
                                                  psTimerList,nIndex);

            if( nIndex != -1 && ps_CurrTimeHandler!=NULL&&
                    ps_CurrTimeHandler->bTimerType == FALSE )
            {
                if(pomTimerType  != NULL && pomTimerValue  != NULL )
                {
                    pomTimerType->EnableWindow(TRUE);
                    pomTimerValue->EnableWindow(TRUE);
                }
            }
        }
    }

    *pResult = 0;
}
/******************************************************************************/
/*  Function Name    :  vTimerStatusChanged                                   */
/*  Input(s)         :  void                                                  */
/*  Output           :  void                                                  */
/*  Functionality    :  This function is will called when the state of timer  */
/*                      is changed. It will change the state of control to    */
/*                      enable or disable it.                                 */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  30.01.2004                                            */
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :  18.01.06,Changed psTimerList to get the currently     */
/*                      selected node's timer list                            */
/******************************************************************************/
void CSetResetTimer::vTimerStatusChanged()
{
    PSTIMERHANDLERLIST psTimerList=m_psSelectedNodeTimerListPtr;
    INT nIndex = m_omTimerList.GetNextItem(-1,LVIS_SELECTED);
    CButton* pomTimerValue = (CButton*)GetDlgItem(IDC_EDIT_TIMER_VALUE);
    CButton* pomTimerType  =  (CButton*)GetDlgItem(IDC_CHCK_MONOSHOT);
    CButton* pomButtonApply = (CButton*)GetDlgItem(IDC_BTN_APPLY);
    PSTIMERHANDLER ps_CurrTimeHandler=GetPointerOfHandler(psTimerList,nIndex);

    if( nIndex != -1 &&
            ps_CurrTimeHandler->bTimerType == FALSE )
    {
        if(    pomTimerType   != NULL
                && pomTimerValue  != NULL
                && pomButtonApply != NULL )
        {
            pomTimerType->EnableWindow(TRUE);
            pomTimerValue->EnableWindow(TRUE);
            pomButtonApply->EnableWindow(TRUE);
        }
    }
    else
    {
        if(    pomTimerType   != NULL
                && pomTimerValue  != NULL
                && pomButtonApply != NULL )
        {
            pomTimerType->EnableWindow(FALSE);
            pomTimerValue->EnableWindow(FALSE);
            pomButtonApply->EnableWindow(FALSE);
        }
    }
}

/******************************************************************************/
/*  Function Name    :  vSetResetAllTimers                                    */
/*  Input(s)         :  BOOL                                                  */
/*  Output           :  void                                                  */
/*  Functionality    :  This function will be called to Set/Reset all valid   */
/*                      timers in the user dll. This will change the global   */
/*                      structure and will start/stop timers using the func   */
/*                      vStartStopTimers().                                   */
/*  Member of        :  CSetResetTimer                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Raja N                                                */
/*  Date Created     :  04.02.2004                                            */
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :   23.12.05,populated the timer list box with selected  */
/*                       node's timer list                                    */
/******************************************************************************/
void CSetResetTimer::vSetResetAllTimers(CString om_StrNode,BOOL bEnable)
{
    PSNODETIMERLIST ps_CurrNodeTrLst=m_psFirstNodeTimerList;
    BOOL bFound=FALSE;

    while(ps_CurrNodeTrLst!=NULL&&!bFound)
    {
        if(om_StrNode==ps_CurrNodeTrLst->omNodeName)
        {
            bFound=TRUE;
        }
        else
        {
            ps_CurrNodeTrLst=ps_CurrNodeTrLst->psNextTimerListPtr;
        }
    }

    if(ps_CurrNodeTrLst!=NULL)
    {
        PSTIMERHANDLERLIST psTimerLst=ps_CurrNodeTrLst->psTimerListPtr;

        while(psTimerLst!=NULL)
        {
            psTimerLst->sTimerHandler.bTimerSelected=bEnable;
            psTimerLst->sTimerHandler.unCurrTime=0;
            psTimerLst=psTimerLst->psNextTimer;
        }
    }

    if(bFound&&(ps_CurrNodeTrLst->psTimerListPtr==m_psSelectedNodeTimerListPtr))
    {
        if(IsWindow(GetSafeHwnd()) == TRUE)
        {
            // Preserve previously selected item index
            register INT rnIndex = m_omTimerList.GetNextItem(-1,LVIS_SELECTED);
            // Delete all items and columns
            m_omTimerList.DeleteAllItems();
            m_omTimerList.DeleteColumn(0);
            // Init the dialog with latest value
            SendMessage(WM_INITDIALOG, 0, 0);
            // Restore the Selection
            m_omTimerList.SetItemState(rnIndex, LVIS_SELECTED | LVIS_FOCUSED,
                                       LVIS_SELECTED);
        }
    }
}

/******************************************************************************/
/*  Function Name    :  GetPointerOfHandler                                   */
/*  Input(s)         :  Timer Handler List and index                          */
/*  Output           :  Pointer to the structure of the timer                 */
/*  Functionality    :  This function will be called to search the stucture   */
/*                      pointer of the timer at the given index of ctrl list  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  23.12.05                                              */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
PSTIMERHANDLER CSetResetTimer::GetPointerOfHandler(
    PSTIMERHANDLERLIST psTimerHandl,int nSelectedItem)
{
    CString om_TimerName=m_omTimerList.GetItemText(nSelectedItem,0);
    PSTIMERHANDLERLIST ps_CurrTimeHandler=psTimerHandl;

    while(ps_CurrTimeHandler!=NULL&&
            om_TimerName!=ps_CurrTimeHandler->sTimerHandler.omStrTimerHandlerName)
    {
        ps_CurrTimeHandler=ps_CurrTimeHandler->psNextTimer;
    }

    if(ps_CurrTimeHandler!=NULL)
    {
        return &ps_CurrTimeHandler->sTimerHandler;
    }

    return NULL;
}


/******************************************************************************/
/*  Function Name    :  vAddNewNodeTimerList                                  */
/*  Input(s)         :  NodeName,timer list of node                           */
/*  Output           :                                                        */
/*  Functionality    :  This function adds a node's timer list to local list  */
/*                      of timer and node structure                           */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  14.01.05                                              */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
const int SIZE_TIMER_STRUCT = sizeof(STIMERHANDLER);
void CSetResetTimer::vAddNewNodeTimerList(CString om_NodeName,
        PSTIMERHANDLERLIST psTimerStrList)
{
    //Make a local timer list for the node
    PSTIMERHANDLERLIST psFirstIndex=NULL;
    PSTIMERHANDLERLIST psLastIndex=NULL;
    PSTIMERHANDLERLIST ps_CurrTimer=psTimerStrList;

    //copy the timer list
    while(ps_CurrTimer!=NULL)
    {
        PSTIMERHANDLERLIST psTimerLst=new STIMERHANDLERLIST;
        psTimerLst->psNextTimer=NULL;
        psTimerLst->sTimerHandler.omStrTimerHandlerName =
            ps_CurrTimer->sTimerHandler.omStrTimerHandlerName ;
        psTimerLst->sTimerHandler.bTimerType =
            ps_CurrTimer->sTimerHandler.bTimerType;
        psTimerLst->sTimerHandler.bTimerSelected =
            ps_CurrTimer->sTimerHandler.bTimerSelected;
        psTimerLst->sTimerHandler.unTimerVal =
            ps_CurrTimer->sTimerHandler.unTimerVal;
        psTimerLst->sTimerHandler.pFTimerHandler =
            ps_CurrTimer->sTimerHandler.pFTimerHandler;
        psTimerLst->sTimerHandler.unTimerID =
            ps_CurrTimer->sTimerHandler.unTimerID;
        psTimerLst->sTimerHandler.unCurrTime =
            ps_CurrTimer->sTimerHandler.unCurrTime;
        psTimerLst->sTimerHandler.hDllHandle =
            ps_CurrTimer->sTimerHandler.hDllHandle;
        psTimerLst->sTimerHandler.pomThreadHandle =
            ps_CurrTimer->sTimerHandler.pomThreadHandle;

        if(psFirstIndex!=NULL)
        {
            psLastIndex->psNextTimer=psTimerLst;
            psLastIndex=psTimerLst;
        }
        else
        {
            psFirstIndex=psTimerLst;
            psLastIndex=psTimerLst;
        }

        ps_CurrTimer=ps_CurrTimer->psNextTimer;
    }

    //After the node's timer list is prepared add it to node timer list
    PSNODETIMERLIST psNodeTimer=new SNODETIMERLIST;

    if(psNodeTimer!=NULL)
    {
        psNodeTimer->omNodeName=om_NodeName;
        //Assign the first pointer of the list to NodeTimer structure
        psNodeTimer->psTimerListPtr=psFirstIndex;
        psNodeTimer->psNextTimerListPtr=NULL;

        //Add to NodeTimer List
        if(m_psFirstNodeTimerList!=NULL)
        {
            m_psLastNodeTimerList->psNextTimerListPtr=psNodeTimer;
            m_psLastNodeTimerList=psNodeTimer;
        }
        else
        {
            m_psFirstNodeTimerList=psNodeTimer;
            m_psLastNodeTimerList=m_psFirstNodeTimerList;
        }
    }
}


/******************************************************************************/
/*  Function Name    :  vDeleteNodeTimerList                                  */
/*  Input(s)         :  NodeName                                              */
/*  Output           :                                                        */
/*  Functionality    :  This function deletes a node's timer list of local    */
/*                      list of timer and node structure                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  14.01.05                                              */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CSetResetTimer::vDeleteNodeTimerList(CString om_NodeName)
{
    if(m_psFirstNodeTimerList!=NULL)
    {
        PSNODETIMERLIST psPrevNodeList,psCurrNodeList;

        //If it is first structure
        if(om_NodeName==m_psFirstNodeTimerList->omNodeName)
        {
            psCurrNodeList=m_psFirstNodeTimerList;

            if(m_psFirstNodeTimerList==m_psLastNodeTimerList)
            {
                m_psLastNodeTimerList=NULL;
            }

            m_psFirstNodeTimerList=m_psFirstNodeTimerList->psNextTimerListPtr;

            if(psCurrNodeList->psTimerListPtr!=NULL)
            {
                vDeleteTimerListOfNode(psCurrNodeList->psTimerListPtr);
            }

            delete psCurrNodeList;
            psCurrNodeList=NULL;
        }
        else
        {
            psPrevNodeList=NULL;
            psCurrNodeList=m_psFirstNodeTimerList;

            while(psCurrNodeList!=NULL && om_NodeName!=psCurrNodeList->omNodeName)
            {
                psPrevNodeList=psCurrNodeList;
                psCurrNodeList=psCurrNodeList->psNextTimerListPtr;
            }

            if(psCurrNodeList!=NULL)
            {
                psPrevNodeList->psNextTimerListPtr=psCurrNodeList->psNextTimerListPtr;

                if(psCurrNodeList==m_psLastNodeTimerList)
                {
                    m_psLastNodeTimerList=psPrevNodeList;
                }

                if(psCurrNodeList->psTimerListPtr!=NULL)
                {
                    vDeleteTimerListOfNode(psCurrNodeList->psTimerListPtr);
                }

                delete psCurrNodeList;
                psCurrNodeList=NULL;
            }
        }
    }
}

/*************************************************************************
    Function Name    :  vStartTimer
    Input(s)         :
    Output           :
    Functionality    :  Starts the only multimedia timer for all timers
    Member of        :  CSetResetTimer
    Author(s)        :  Anish kumar
    Date Created     :  14.01.06
**************************************************************************/
void CSetResetTimer::vStartTimer()
{
    omMainTimerEvent.ResetEvent();

    if(m_psCalTimerThreadStruct == NULL)
    {
        m_psCalTimerThreadStruct = new SCALCEXECTIMETHREAD;
        m_psCalTimerThreadStruct->m_pomThreadPtr=NULL;
    }

    if(m_psCalTimerThreadStruct->m_pomThreadPtr == NULL)
    {
        CSetResetTimer::sm_eBus = m_eBus;
        m_psCalTimerThreadStruct->m_bThreadStop = FALSE;
        m_psCalTimerThreadStruct->m_omExitThreadEvent.ResetEvent();
        m_psCalTimerThreadStruct->m_pomThreadPtr
            = AfxBeginThread(CalcTimersExecTime,m_psCalTimerThreadStruct);
    }

    if(m_unTimerID == 0)
    {
        m_unTimerID = timeSetEvent(1, 0, (LPTIMECALLBACK)TimerProc, NULL,
                                   TIME_CALLBACK_FUNCTION|TIME_PERIODIC );
    }
}



/*************************************************************************
    Function Name    :  vStopTimer
    Input(s)         :
    Output           :
    Functionality    :  Stop the multimedia timer
    Member of        :  CSetResetTimer
    Author(s)        :  Anish kumar
    Date Created     :  14.01.06
    Modification By  :  Anish Kumar
    Modification on  :  20.11.06,set the event for timerproc before
                        terminating it
**************************************************************************/
void CSetResetTimer::vStopTimer()
{
    if(m_psCalTimerThreadStruct != NULL)
    {
        m_psCalTimerThreadStruct->m_bThreadStop = TRUE;
        omMainTimerEvent.SetEvent();
        WaitForSingleObject(m_psCalTimerThreadStruct->m_omExitThreadEvent,20);
        timeKillEvent(m_unTimerID);
        m_unTimerID = 0;

        if(m_psCalTimerThreadStruct->m_pomThreadPtr != NULL)
        {
            TerminateThread(m_psCalTimerThreadStruct->m_pomThreadPtr->m_hThread,0);
            m_psCalTimerThreadStruct->m_pomThreadPtr=NULL;
        }

        delete m_psCalTimerThreadStruct;
        m_psCalTimerThreadStruct=NULL;
    }
}

/*************************************************************************
    Function Name    :  OnSelchangeNodeList
    Input(s)         :
    Output           :
    Functionality    :  It populate the list box according to the node
                         selected in nodetimer combo box
    Member of        :  CSetResetTimer
    Author(s)        :  Anish kumar
    Date Created     :  14.01.06
**************************************************************************/


void CSetResetTimer::OnSelchangeNodeList()
{
    CString omNodeName;
    m_bNodeNameChanged = TRUE;
    //Get the node selected in combobox
    int nCurSel = m_omComboNodeName.GetCurSel();

    if (nCurSel != CB_ERR)
    {
        m_omComboNodeName.GetLBText(nCurSel,omNodeName);
        //get the corressponding node's timer list pointer
        BOOL bFound=FALSE;
        PSNODETIMERLIST psCurrTimerListPtr=m_psFirstNodeTimerList;

        while(psCurrTimerListPtr!=NULL&&!bFound)
        {
            if(omNodeName==psCurrTimerListPtr->omNodeName)
            {
                bFound=TRUE;
            }
            else
            {
                psCurrTimerListPtr=psCurrTimerListPtr->psNextTimerListPtr;
            }
        }

        if(bFound)
        {
            m_psSelectedNodeTimerListPtr= psCurrTimerListPtr->psTimerListPtr;
            m_nSelectedNode=nCurSel;
        }
    }

    if(m_psSelectedNodeTimerListPtr!=NULL)
    {
        //Get
        m_omTimerList.DeleteAllItems();
        ASSERT(m_omTimerList.GetItemCount() == 0);
        PSTIMERHANDLERLIST ps_CurrTimeHandl=m_psSelectedNodeTimerListPtr;
        INT nCountTimer = 0;
        INT nColumnWidth = 0;

        while(ps_CurrTimeHandl!=NULL)
        {
            m_omTimerList.InsertItem(nCountTimer,
                                     ps_CurrTimeHandl->sTimerHandler.omStrTimerHandlerName);
            m_omTimerList.SetCheck(nCountTimer,
                                   ps_CurrTimeHandl->sTimerHandler.bTimerSelected);

            // If it is Mono shot timer, make the timer type False
            if(ps_CurrTimeHandl->sTimerHandler.bTimerType !=FALSE)
            {
                ps_CurrTimeHandl->sTimerHandler.bTimerType = FALSE;
            }

            // get the width of cloumn from length of item string inserted.
            INT nTemp = m_omTimerList.GetStringWidth(
                            ps_CurrTimeHandl->sTimerHandler.omStrTimerHandlerName);

            // Get the maximum colunm width.
            if(nTemp >nColumnWidth)
            {
                nColumnWidth = nTemp;
            }

            nCountTimer++;
            ps_CurrTimeHandl=ps_CurrTimeHandl->psNextTimer;
        }

        // Set the clumn width to maximum string size + check box image size.
        m_omTimerList.SetColumnWidth(0,nColumnWidth + defSIZE_OF_IMAGE);
        // Set the focus and selection to first item.
        int nReturn = m_omTimerList.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED,
                      LVIS_SELECTED|LVIS_FOCUSED);

        // Update the controls with selected timer attributes.
        if(nReturn >0)
        {
            vUpdateTimerInformation(m_psSelectedNodeTimerListPtr,0);
        }

        // Remove the selection from list control items and disable
        // the Apply button only if the timer is cyclic and
        nReturn = m_omTimerList.GetNextItem(-1, LVIS_SELECTED) ;

        if( nReturn != -1)
        {
            PSTIMERHANDLER ps_CurrTimerHandler=GetPointerOfHandler(
                                                   m_psSelectedNodeTimerListPtr,nReturn);

            //if found
            if(ps_CurrTimerHandler!=NULL&&ps_CurrTimerHandler->bTimerType!=FALSE)
            {
                GetDlgItem(IDC_BTN_APPLY)->EnableWindow(FALSE);
            }
        }
        else
        {
            GetDlgItem(IDC_BTN_APPLY)->EnableWindow(TRUE);
        }
    }
    else
    {
        m_omTimerList.DeleteAllItems();
    }

    m_bNodeNameChanged = FALSE;
    m_bDialogGettingInit = FALSE;
}


/*************************************************************************
    Function Name    :  vSetSimSysNodeArray
    Input(s)         :  Array of Simsys node names/single node and BOOL
                        to set whether it is called from mainframe or
                        simsys
    Output           :
    Functionality    :  It copy the nodename array to setreset class local
                        memory and set the flag value
    Member of        :  CSetResetTimer
    Author(s)        :  Anish kumar
    Date Created     :  24.01.06
**************************************************************************/
void CSetResetTimer::vSetSimSysNodeArray(const CStringArray& omSysNode,
        BOOL bFromSimSys)
{
    m_omSysNodeName.Copy(omSysNode);
    m_bSimSysNodes = bFromSimSys;
}


/******************************************************************************/
/*  Function Name    :  vDeleteTimerListOfNode                                */
/*  Input(s)         :  Tmer List pointer                                     */
/*  Output           :                                                        */
/*  Functionality    :  This function deletes a node's timer list             */
/*  Member of        :  CSetResetTimer                                        */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  07.02.06                                              */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CSetResetTimer::vDeleteTimerListOfNode(PSTIMERHANDLERLIST psListOfTimer)
{
    if(psListOfTimer!=NULL)
    {
        PSTIMERHANDLERLIST psCurrTimerLst;

        while(psListOfTimer!=NULL)
        {
            psCurrTimerLst=psListOfTimer;
            psListOfTimer=psListOfTimer->psNextTimer;
            delete psCurrTimerLst;
            psCurrTimerLst=NULL;
        }
    }
}

/******************************************************************************/
/*  Function Name    :  vRestoreTimerStatus                                   */
/*  Input(s)         :  Tmer List pointer,Node Namme                          */
/*  Output           :                                                        */
/*  Functionality    :  This function Restore a node's timer status on B&L    */
/*  Member of        :  CSetResetTimer                                        */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  24.02.06                                              */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CSetResetTimer::vRestoreTimerStatus(CString omNodeName,
        PSTIMERHANDLERLIST psTimerListPtr)
{
    if(m_psFirstNodeTimerList!=NULL)
    {
        PSNODETIMERLIST psCurrNodeTimerLst=m_psFirstNodeTimerList;
        BOOL bFound=FALSE;

        while(psCurrNodeTimerLst!=NULL&&!bFound)
        {
            if(omNodeName==psCurrNodeTimerLst->omNodeName)
            {
                bFound=TRUE;
            }
            else
            {
                psCurrNodeTimerLst=psCurrNodeTimerLst->psNextTimerListPtr;
            }
        }

        if(bFound)
        {
            PSTIMERHANDLERLIST psCurrTimerLst=psCurrNodeTimerLst->psTimerListPtr;

            while(psCurrTimerLst!=NULL)
            {
                PSTIMERHANDLERLIST psLocalTLst=psTimerListPtr;
                bFound=FALSE;

                while(psLocalTLst!=NULL&&!bFound)
                {
                    if(psLocalTLst->sTimerHandler.omStrTimerHandlerName==
                            psCurrTimerLst->sTimerHandler.omStrTimerHandlerName)
                    {
                        psLocalTLst->sTimerHandler.bTimerSelected=
                            psCurrTimerLst->sTimerHandler.bTimerSelected;
                        psLocalTLst->sTimerHandler.bTimerType=
                            psCurrTimerLst->sTimerHandler.bTimerType;
                        bFound=TRUE;
                    }
                    else
                    {
                        psLocalTLst=psLocalTLst->psNextTimer;
                    }
                }

                psCurrTimerLst=psCurrTimerLst->psNextTimer;
            }
        }
    }
}





/*************************************************************************
    Function Name    : CalcTimersExecTime
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :
    Author(s)        :  Anish kumar
    Date Created     :
**************************************************************************/

UINT CalcTimersExecTime(LPVOID pParam)
{
    PSCALCEXECTIMETHREAD psThreadInfo = (PSCALCEXECTIMETHREAD)pParam;

    while( !psThreadInfo->m_bThreadStop )
    {
        WaitForSingleObject(omMainTimerEvent,INFINITE);
        omMainTimerEvent.ResetEvent();

        if(!psThreadInfo->m_bThreadStop)
        {
            CExecuteManager::ouGetExecuteManager(CSetResetTimer::sm_eBus).vManageTimerExecution();
        }
    }

    psThreadInfo->m_pomThreadPtr = NULL;
    psThreadInfo->m_omExitThreadEvent.SetEvent();
    return 0;
}
/*************************************************************************
    Function Name    :  TimerProc
    Input(s)         :
    Output           :
    Functionality    :  Timer call back function
    Member of        :  Global timer thread function
    Author(s)        :  Anish kumar
    Date Created     :  16.12.05
**************************************************************************/
void CALLBACK TimerProc(UINT /*m_unTimerId*/, UINT /*uMsg*/, DWORD* /*dwUser*/,
                        DWORD* /*dw1*/, DWORD* /*dw2*/)
{
    omMainTimerEvent.SetEvent();
}
