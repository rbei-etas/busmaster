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
 * \file      MsgHandlerDlg.cpp
 * \brief     This file contain the definition all member function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition all member function of
 */
#include "NodeSimEx_stdafx.h"
#include "SimSysManager.h"
#include "MsgHandlerDlg.h"
#include "GlobalObj.h"
#include "MsgHandlerDlg.h"
#include "Utility\MultiLanguageSupport.h"
#include "Utility\UtilFunctions.h"
//#include "../Application/GettextBusmaster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgHandlerDlg dialog

/******************************************************************************/
/*  Function Name    :  CMsgHandlerDlg                                        */
/*  Input(s)         :  CWnd* pParent: Pointer to parent window               */
/*  Output           :                                                        */
/*  Functionality    :  Constructor is called when user create an object of   */
/*                      this class. Initialisation of all data members done   */
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  21.02.2003                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  07.04.2003, review comment incorporated               */
/******************************************************************************/
CMsgHandlerDlg::CMsgHandlerDlg(ETYPE_BUS eBus, CWnd* pParent /*=NULL*/)
    : CDialog(CMsgHandlerDlg::IDD, pParent),
      m_omStrMsgID(""),
      m_omStrMsgIDFrom(""),
      m_omStrMsgIDTo("")
{
    //{{AFX_DATA_INIT(CMsgHandlerDlg)
    //}}AFX_DATA_INIT
    m_eBus = eBus;
}

/******************************************************************************/
/*  Function Name    :  DoDataExchange                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This is called be frame work to exchange the data     */
/*                      between data member and control                       */
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  21.02.2003                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  07.04.2003, review comment incorporated               */
/******************************************************************************/
void CMsgHandlerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMsgHandlerDlg)
    DDX_Control(pDX, IDC_LSTB_MSG_HANDLER_LIST, m_omListMsgName);
    DDX_Control(pDX, IDC_EDIT_MSG_HANDLER_ID_TO, m_odEditMsgIDTo);
    DDX_Control(pDX, IDC_EDIT_MSG_HANDLER_ID_FROM, m_odEditMsgIDFrom);
    DDX_Control(pDX, IDC_EDIT_MSG_HANDLER_ID, m_odEditMsgID);
    DDX_Control(pDX, IDC_CBTN_MSG_HANDLDER_APPLY, m_omButtonApply);
    DDX_Control(pDX, IDC_CBTN__MSG_OK, m_omButtonOK);
    DDX_Text(pDX, IDC_EDIT_MSG_HANDLER_ID, m_omStrMsgID);
    DDX_Text(pDX, IDC_EDIT_MSG_HANDLER_ID_FROM, m_omStrMsgIDFrom);
    DDX_Text(pDX, IDC_EDIT_MSG_HANDLER_ID_TO, m_omStrMsgIDTo);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgHandlerDlg, CDialog)
    //{{AFX_MSG_MAP(CMsgHandlerDlg)
    ON_BN_CLICKED(IDC_CBTN__MSG_CANCEL, OnCbtnMsgCancel)
    ON_BN_CLICKED(IDC_CBTN__MSG_OK, OnCbtnMsgOk)
    ON_BN_CLICKED(IDC_CBTN_MSG_HANDLDER_APPLY, OnCbtnMsgHandlerApply)
    ON_BN_CLICKED(IDC_RBTN_MSG_ALL, OnRbtnMsgAll)
    ON_BN_CLICKED(IDC_RBTN_MSG_ID, OnRbtnMsgId)
    ON_BN_CLICKED(IDC_RBTN_MSG_NAME, OnRbtnMsgName)
    ON_BN_CLICKED(IDC_RBTN_MSG_RANGE, OnRbtnMsgRange)
    ON_LBN_SELCHANGE(IDC_LSTB_MSG_HANDLER_LIST, OnSelchangeLstbMsgHandlerList)
    ON_EN_UPDATE(IDC_EDIT_MSG_HANDLER_ID, OnUpdateEditMsgHandlerId)
    ON_EN_UPDATE(IDC_EDIT_MSG_HANDLER_ID_FROM, OnUpdateEditMsgHandlerIdFrom)
    ON_EN_UPDATE(IDC_EDIT_MSG_HANDLER_ID_TO, OnUpdateEditMsgHandlerIdTo)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************/
/*  Function Name    :  OnCbtnMsgCancel                                       */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called when user selects to cancel   */
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  21.02.2003                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  07.04.2003, review comment incorporated               */
/******************************************************************************/
void CMsgHandlerDlg::OnCbtnMsgCancel()
{
    CDialog::OnCancel();
}
/******************************************************************************/
/*  Function Name    :  OnCbtnMsgOk                                           */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called when user selects OK button.  */
/*                      The validation of user selection is done in this func.*/
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  21.02.2003                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  07.04.2003, review comment incorporated               */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  17.04.2003,remove selection of listbox after pressing */
/*                                 OK button and if validation fails. Found   */
/*                                 during unit testing.                       */
/******************************************************************************/
void CMsgHandlerDlg::OnCbtnMsgOk()
{
    OnCbtnMsgHandlerApply();
    CDialog::OnOK();
}
/******************************************************************************/
/*  Function Name    :  OnCbtnMsgHandlerApply                                 */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called when user selects Apply button*/
/*                      The user selection will be validated and selected     */
/*                      will be added to editor.                              */
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  21.02.2003                                            */
/******************************************************************************/
void CMsgHandlerDlg::OnCbtnMsgHandlerApply()
{

    BOOL bValidateSelection = FALSE;
    UpdateData(TRUE);
    m_omButtonApply.EnableWindow(FALSE);
    m_omButtonOK.EnableWindow(FALSE);
    // Get document pointer
    CFunctionEditorDoc* pDoc = CGlobalObj::ouGetObj(m_eBus).podGetFunctionEditorDoc();
    if (NULL != pDoc)
    {
        SBUS_SPECIFIC_INFO sBusSpecInfo;
        pDoc->bGetBusSpecificInfo(sBusSpecInfo);
        // Validate user selections
        bValidateSelection = bValidateUserSelection(pDoc);
        if (bValidateSelection == TRUE)
        {
            CString omFunc = CGlobalObj::omGetBusSpecMsgHndlrName(m_eBus);;
            // Add to function editor
            CString omSelectedText = "";

            omSelectedText = BUS_FN_HDR; // Start comment section: init
            omSelectedText.Replace("PLACE_HODLER_FOR_BUSNAME",
                                   sBusSpecInfo.m_omBusName); // Replace the bus name

            omFunc += m_omStrSelectedItemText;
            omSelectedText.Replace( "PLACE_HODLER_FOR_FUNCTIONNAME",
                                    omFunc );
            pDoc->m_omSourceCodeTextList.AddTail( omSelectedText );
            // Form the function prototype
            omSelectedText  = m_omStrSelectedItemText;
            int nIndex = -1;


            // Get the type and set the parameter type
            BOOL bIsMsgSpecificHandler = FALSE;
            //Find out whether the Msg handler is DatabaseMsgName type or ID type
            CString omMsgHandlerType = CGlobalObj::ouGetObj(m_eBus).m_omMsgStructName;
            nIndex = omSelectedText.Find(defSTR_MSG_SPECIFIC_HANDLER);

            if( nIndex != -1 )
            {
                bIsMsgSpecificHandler = TRUE;
                // For database message type is equal same as msg name
                omMsgHandlerType = omSelectedText.Mid( nIndex +
                                                       defMESSAGE_NAME_INDEX );
            }
            CString omStrParamtype;

            if (bIsMsgSpecificHandler == TRUE && (m_eBus == CAN || m_eBus == LIN))
            {
                omStrParamtype = omMsgHandlerType;
            }
            else// For Msg ID, Range and generic messages the type is sTCANDATA
            {
                omStrParamtype = CGlobalObj::ouGetObj(m_eBus).m_omMsgStructName;
            }
            CString omFormatString = "";

            if(m_eBus == CAN || m_eBus == LIN)
            {
                omFormatString = defDEFAULT_MSG_HANDLER_CODE_CAN;
            }
            else
            {
                omFormatString = defDEFAULT_MSG_HANDLER_CODE;
            }

            omSelectedText.Format(  omFormatString,
                                    CGlobalObj::omGetBusSpecMsgHndlrName(sBusSpecInfo.m_eBus),
                                    omSelectedText,   // Fun name
                                    omStrParamtype ); // Parameter type

            pDoc->m_omSourceCodeTextList.AddTail( omSelectedText );
            pDoc->bAddFunctionPrototype(omSelectedText, TRUE);

            CString omStrPrototype = omSelectedText;
            // Add to tree view
            CFnsTreeView* pomTreeView = CGlobalObj::ouGetObj(m_eBus).podGetFuncsTreeViewPtr();
            if(pomTreeView != NULL )
            {
                // Add the prototype to the tree view
                CTreeCtrl& omTree = pomTreeView->GetTreeCtrl();
                HTREEITEM hItem = omTree.GetSelectedItem();
                HTREEITEM hNew =
                    omTree.InsertItem( omSelectedText, hItem);
                omTree.SetItemImage( hNew, 5, 5 );
                omTree.SelectItem( hNew );
                // Form the body of the function
                omSelectedText = "{";
                pDoc->m_omSourceCodeTextList.AddTail( omSelectedText );
                if (CGlobalObj::ouGetObj(m_eBus).m_omMsgStructName.IsEmpty())
                {
                    ASSERT(FALSE);
                }
                omSelectedText = defTODO;
                pDoc->m_omSourceCodeTextList.AddTail( omSelectedText );

                // Form the function footer
                omSelectedText = BUS_FN_FOOTER;
                omSelectedText.Replace("PLACE_HODLER_FOR_BUSNAME", sBusSpecInfo.m_omBusName);
                omSelectedText.Replace( "PLACE_HODLER_FOR_FUNCTIONNAME",
                                        omFunc );

                pDoc->m_omSourceCodeTextList.AddTail( omSelectedText );

                CStringArray* pMsgArray =
                    pDoc->omStrGetMessageHandlerPrototypes();
                if ( pMsgArray != NULL )
                {
                    pMsgArray->Add( omStrPrototype );
                }
                pDoc->UpdateAllViews( NULL );
                pDoc->SetModifiedFlag( TRUE );
            }
        }
    }
    m_omStrSelectedItemText = "";
    BOOL bButtonChecked  = FALSE;
    bButtonChecked =  IsDlgButtonChecked(IDC_RBTN_MSG_NAME);
    if(bButtonChecked != FALSE )
    {
        m_omListMsgName.SetCurSel(-1);
    }
}
/******************************************************************************/
/*  Function Name    :  OnRbtnMsgAll                                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called be framework when user selects*/
/*                      this radio button. Repective controll will be enable  */
/*                      or disable  depending upon the selection              */
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  21.02.2003                                            */
/******************************************************************************/
void CMsgHandlerDlg::OnRbtnMsgAll()
{
    m_odEditMsgIDTo.EnableWindow(FALSE);
    m_omListMsgName.EnableWindow(FALSE);
    m_odEditMsgIDFrom.EnableWindow(FALSE);
    m_odEditMsgID.EnableWindow(FALSE);
    m_omButtonApply.EnableWindow(TRUE);
    m_omButtonOK.EnableWindow(TRUE);
}
/******************************************************************************/
/*  Function Name    :  OnRbtnMsgId                                           */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called be framework when user selects*/
/*                      this radio button. Repective controll will be enable  */
/*                      or disable  depending upon the selection              */
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  21.02.2003                                            */
/******************************************************************************/
void CMsgHandlerDlg::OnRbtnMsgId()
{
    m_odEditMsgIDTo.EnableWindow(FALSE);
    m_omListMsgName.EnableWindow(FALSE);
    m_odEditMsgIDFrom.EnableWindow(FALSE);
    m_odEditMsgID.EnableWindow(TRUE);
    m_omButtonApply.EnableWindow(FALSE);
    m_omButtonOK.EnableWindow(FALSE);
}
/******************************************************************************/
/*  Function Name    :  OnRbtnMsgName                                         */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called be framework when user selects*/
/*                      this radio button. Repective controll will be enable  */
/*                      or disable  depending upon the selection              */
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  21.02.2003                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  07.04.2003, review comment incorporated               */
/******************************************************************************/
void CMsgHandlerDlg::OnRbtnMsgName()
{
    m_odEditMsgIDTo.EnableWindow(FALSE);
    m_omListMsgName.EnableWindow(TRUE);
    m_odEditMsgIDFrom.EnableWindow(FALSE);
    m_odEditMsgID.EnableWindow(FALSE);
    m_omButtonApply.EnableWindow(FALSE);
    m_omButtonOK.EnableWindow(FALSE);
    m_omButtonApply.EnableWindow(FALSE);

    m_omListMsgName.SetCurSel(-1);

    if(m_eBus == LIN)
    {
        GetDlgItem(IDC_CMB_CHANNEL)->EnableWindow(TRUE);

        CString strChnlNum;
        ((CComboBox*)GetDlgItem(IDC_CMB_CHANNEL))->GetWindowTextA(strChnlNum);
        list<FRAME_STRUCT> lstMsgNames;
        CGlobalObj::ouGetObj(m_eBus).m_ouClusterConfig->m_ouFlexChannelConfig[atoi(strChnlNum)].m_ouClusterInfo.GetFrames(lstMsgNames);
        CStringArray* pomStrArray = NULL;
        list<FRAME_STRUCT>::iterator itrLstMsg;
        for(itrLstMsg = lstMsgNames.begin(); itrLstMsg != lstMsgNames.end(); itrLstMsg++)
        {
            bAddMessageNameInListBox(pomStrArray, itrLstMsg->m_strFrameName.c_str());
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnRbtnMsgRange                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called be framework when user selects*/
/*                      this radio button. Repective controll will be enable  */
/*                      or disable  depending upon the selection              */
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  21.02.2003                                            */
/******************************************************************************/
void CMsgHandlerDlg::OnRbtnMsgRange()
{
    m_odEditMsgIDTo.EnableWindow(TRUE);
    m_omListMsgName.EnableWindow(FALSE);
    m_odEditMsgIDFrom.EnableWindow(TRUE);
    m_odEditMsgID.EnableWindow(FALSE);
    m_omButtonApply.EnableWindow(FALSE);
    m_omButtonOK.EnableWindow(FALSE);
}
void CMsgHandlerDlg::vInitDlgWithBusSpecNames(void)
{
    CString omMsgFieldName = CGlobalObj::omGetBusSpecMsgFieldName(m_eBus);

    //Set Message Id radio button name
    CString omTemp = omMsgFieldName + " ID";
    GetDlgItem(IDC_RBTN_MSG_ID)->SetWindowText(omTemp.GetBuffer(MAX_PATH));

    //Set Message Id range radio button name
    omTemp = omMsgFieldName + " ID Range";
    GetDlgItem(IDC_RBTN_MSG_RANGE)->SetWindowText(omTemp.GetBuffer(MAX_PATH));

    //Set ALL Rx message radio button name
    omTemp = "ALL Rx " + omMsgFieldName + "(s)";
    GetDlgItem(IDC_RBTN_MSG_ALL)->SetWindowText(omTemp.GetBuffer(MAX_PATH));

    //Set Message name radio button name
    omTemp = omMsgFieldName + " Name";
    GetDlgItem(IDC_RBTN_MSG_NAME)->SetWindowText(omTemp.GetBuffer(MAX_PATH));

    //Set static text Message Id: name
    omTemp = omMsgFieldName + " ID: ";
    GetDlgItem(IDC_STATIC_MSG_ID)->SetWindowText(omTemp.GetBuffer(MAX_PATH));

    //Set static text Message Id range: name
    omTemp = omMsgFieldName + " ID Range";
    GetDlgItem(IDC_STATIC_MSG_ID_RANGE)->SetWindowText(omTemp.GetBuffer(MAX_PATH));

    //Set static text Message DB list name
    omTemp = "Database " + omMsgFieldName + " List";
    GetDlgItem(IDC_STATIC_DB_MSG_LIST)->SetWindowText(omTemp.GetBuffer(MAX_PATH));

}
/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  All the controls are initialised in this function.    */
/*                      This function is called by frame work after DoModal() */
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  21.02.2003                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  07.04.2003, review comment incorporated               */
/******************************************************************************/
BOOL CMsgHandlerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    vInitDlgWithBusSpecNames();
    m_odEditMsgIDTo.EnableWindow(FALSE);
    m_omListMsgName.EnableWindow(FALSE);
    m_odEditMsgIDFrom.EnableWindow(FALSE);
    m_odEditMsgID.EnableWindow(TRUE);
    m_omButtonApply.EnableWindow(FALSE);
    m_omButtonOK.EnableWindow(FALSE);
    //VENKATNARAYANA
    //    m_odEditMsgID.vSetSigned(FALSE);

    m_odEditMsgIDTo.vSetSigned(FALSE);
    m_odEditMsgIDFrom.vSetSigned(FALSE);
    CheckDlgButton(IDC_RBTN_MSG_ID,BST_CHECKED);
    // Get all the message names of active DB

    CFunctionEditorDoc* pDoc = CGlobalObj::ouGetObj(m_eBus).podGetFunctionEditorDoc();
    CStringArray* pomStrArray = NULL;
    if (pDoc != NULL)
    {
        pomStrArray = pDoc->omStrGetMessageHandlerPrototypes();
    }

    if(m_eBus != LIN)
    {
        if(pomStrArray != NULL )
        {
            POSITION pos = CGlobalObj::ouGetObj(m_eBus).m_odMsgNameMsgCodeListDb.GetHeadPosition();
            //UINT unNoOfMessages = ouGetMsgSignal().unGetNumerOfMessages();
            while (pos != NULL)
            {
                SDB_NAME_MSG&  oDbNameMsg = CGlobalObj::ouGetObj(m_eBus).m_odMsgNameMsgCodeListDb.GetNext(pos);
                POSITION posMess = oDbNameMsg.m_oMsgNameMsgCodeList.GetHeadPosition();
                while (posMess != NULL)
                {
                    SMSG_NAME_CODE& sMsgNameCode = oDbNameMsg.m_oMsgNameMsgCodeList.GetNext(posMess);
                    bAddMessageNameInListBox(pomStrArray, sMsgNameCode.m_omMsgName);
                }
            }
        }
    }

    if(m_eBus == LIN)
    {
        INT unChnlConfig = CGlobalObj::ouGetObj(m_eBus).m_ouClusterConfig->m_nChannelsConfigured;
        char chString[MAX_PATH] = "";
        for(INT unIndex = 0; unIndex < unChnlConfig; unIndex++)
        {
            sprintf(chString, "%d", unIndex+1);
            ((CComboBox*)GetDlgItem(IDC_CMB_CHANNEL))->AddString(chString);
        }

        if(((CComboBox*)GetDlgItem(IDC_CMB_CHANNEL))->GetCount() > 0)
        {
            ((CComboBox*)GetDlgItem(IDC_CMB_CHANNEL))->SetCurSel(0);
        }

        CString strChnlNum;
        ((CComboBox*)GetDlgItem(IDC_CMB_CHANNEL))->GetWindowTextA(strChnlNum);
        list<FRAME_STRUCT> lstMsgNames;
        INT unChannelNum = atoi(strChnlNum) - 1;
        CGlobalObj::ouGetObj(m_eBus).m_ouClusterConfig->m_ouFlexChannelConfig[unChannelNum].m_ouClusterInfo.GetFrames(lstMsgNames);

        list<FRAME_STRUCT>::iterator itrLstMsg = lstMsgNames.begin();
        while(itrLstMsg != lstMsgNames.end())
        {
            bAddMessageNameInListBox(pomStrArray, itrLstMsg->m_strFrameName.c_str());
            itrLstMsg++;
        }
    }

    //if ( unNoOfMessages > 0 )
    {

        //COMMENTED BY AK******************
        //ouGetMsgSignal().omStrListGetMessageNames(omMessageNames);

        //CMainFrame* pMainFrame = NULL;
        //pMainFrame             = (CMainFrame*)AfxGetMainWnd();
        //if(pMainFrame != NULL )
        //{

        //    CFunctionEditorDoc* pDoc = pMainFrame->CGlobalObj::podGetFunctionEditorDoc();
        //    if(pDoc != NULL )
        //    {

        //        CStringArray* pomStrArray = NULL;
        //        pomStrArray = pDoc->omStrGetMessageHandlerPrototypes();
        //        if(pomStrArray != NULL )
        //        {
        //            POSITION pos = omMessageNames.GetHeadPosition();
        //            // Insert every message name into the message list box
        //            CString omStrMsgName = _T("");

        //            while ( pos != NULL )
        //            {
        //
        //                omStrMsgName = omMessageNames.GetNext(pos);
        //                bAddMessageNameInListBox(pomStrArray,omStrMsgName);
        //
        //            }
        //        }
        //    }
        //}
    }

    if(m_eBus != LIN )
    {
        // Disable channel selection combo for other protocols
        // other than LIN
        GetDlgItem(IDC_STATIC_CHANNEL)->ShowWindow(FALSE);
        GetDlgItem(IDC_CMB_CHANNEL)->ShowWindow(FALSE);
    }
    else
    {
        GetDlgItem(IDC_STATIC_CHANNEL)->ShowWindow(TRUE);
        GetDlgItem(IDC_CMB_CHANNEL)->ShowWindow(TRUE);
        GetDlgItem(IDC_CMB_CHANNEL)->EnableWindow(FALSE);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
/******************************************************************************/
/*  Function Name    :  OnSelchangeLstbMsgHandlerList                         */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Enable OK and APPLY button                            */
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  21.02.2003                                            */
/*  Modification By  :                                                        */
/*  Modifications on :                                                        */
/******************************************************************************/
void CMsgHandlerDlg::OnSelchangeLstbMsgHandlerList()
{
    m_omButtonApply.EnableWindow(TRUE);
    m_omButtonOK.EnableWindow(TRUE);
}
/******************************************************************************/
/*  Function Name    :  bValidateUserSelection                                */
/*  Input(s)         :  CFunctionEditorDoc* pDoc : pointer to function editor */
/*  Output           :  TRUE/FALSE                                            */
/*  Functionality    :  This function will validate user selection. Based on  */
/*                      type of message handler selected, its corresponding   */
/*                      handlers selection is validated. It calls             */
/*                      bValidateMessageNameAndID for validating message name */
/*                      and message ID hanlders. message name handlers is not */
/*                      allowed to be written if corresponding message Id     */
/*                      handler is already added and vice versa.              */
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :
/*  Date Created     :
/******************************************************************************/
BOOL CMsgHandlerDlg::bValidateUserSelection(CFunctionEditorDoc* pDoc)

{
    BOOL bReturn              = TRUE;
    BOOL bButtonChecked       = FALSE;
    CString omStrMsgNameForID = "";
    CString omStrMsgIDForName = "";
    CList<UINT, UINT&> omMsgList;
    if(pDoc != NULL )
    {
        CString omStrText = "";
        CString omStrHandlerType = "";
        bButtonChecked =  IsDlgButtonChecked(IDC_RBTN_MSG_NAME);

        CStringArray* pMsgArray = pDoc->omStrGetMessageHandlerPrototypes();

        if(bButtonChecked)
        {
            //COMMENTED BY AK **************
            int nSelection = 0;
            UINT unMsgIDForName = 0;
            CString omStrMsgName = "";
            nSelection = m_omListMsgName.GetCurSel();
            m_omListMsgName.GetText(nSelection,omStrMsgName);
            // get the corresponding message ID from the database
            /* unMsgIDForName=CMsgSignal::ouGetMsgSignal().nGetSlotIDFromName(omStrMsgName);
             if(unMsgIDForName != -1)
             {
                 omStrMsgIDForName.Format("%x",unMsgIDForName);
                 bReturn = bValidateMessageNameAndID( pMsgArray,
                                                      defMSG_NAME_HANDLER,
                                                      omStrMsgIDForName);
             }*/
            m_omStrSelectedItemText = defMSG_NAME_HANDLER;
            m_omStrSelectedItemText += omStrMsgName;
            omStrText = "\""+ omStrMsgName + "\"";
            omStrHandlerType = _(defMSG_NAME_TEXT);
        }
        else
        {
            bButtonChecked =  IsDlgButtonChecked(IDC_RBTN_MSG_ID);
            if(bButtonChecked != 0)
            {
                //VENKATNARAYANA
                //unMsgID = (UINT)m_odEditMsgID.lGetValue();
                // Get the corresponding message name from the database
                /*omStrMsgNameForID = CMsgSignal::ouGetMsgSignal().
                            omStrGetMessageNameFromSlotID(unMsgID);
                bReturn = bValidateMessageNameAndID( pMsgArray,
                                                     defMSG_ID_HANDLER,
                                                     omStrMsgNameForID);*/

                //TODO::Venkatanarayana
                CString omStrTempId = "";
                CString omStrTemp;
                int nCurPos= 0;

                /*omStrTemp= m_omStrMsgID.Tokenize(",",nCurPos);*/
                vTokenize(m_omStrMsgID, ",", omStrTemp, nCurPos);
                while (omStrTemp != "")
                {
                    CHAR* pchTemp;
                    UINT unMsgId = strtoul(omStrTemp, &pchTemp, 16);
                    /*omStrTemp = m_omStrMsgID.Tokenize(",",nCurPos);*/
                    vTokenize(m_omStrMsgID, ",", omStrTemp, nCurPos);
                    omMsgList.AddTail(unMsgId);
                };

                UINT unMsgCount = omMsgList.GetCount();
                if( unMsgCount == 1 )
                {
                    m_omStrSelectedItemText = defMSG_ID_HANDLER;
                    POSITION pos = omMsgList.FindIndex(0);
                    if(pos != NULL)
                    {
                        UINT unVal = omMsgList.GetAt(pos);
                        m_omStrMsgID.Format("%x", unVal);
                        omStrText = _T("\"")+ m_omStrMsgID + _T("\"");
                    }
                    m_omStrSelectedItemText += m_omStrMsgID;
                    omStrText = "\""+ m_omStrMsgID + "\"";
                    omStrHandlerType = _(defMSG_ID_TEXT);
                }
                else if(unMsgCount > 1)
                {
                    m_omStrSelectedItemText = defMSG_IDLIST_HANDLER;

                    for(UINT i = 0; i < unMsgCount; i++)
                    {
                        POSITION pos = omMsgList.FindIndex(i);
                        UINT unVal = omMsgList.GetAt(pos);
                        m_omStrMsgID.Format("%x_", unVal);
                        omStrText  += m_omStrMsgID;

                    }
                    omStrText.TrimRight("_");
                    m_omStrSelectedItemText += omStrText;
                    omStrText = _T("\"")+ omStrText + _T("\"");
                    omStrHandlerType = _(defMSG_IDLIST_TEXT);
                }
            }
            else
            {
                bButtonChecked =  IsDlgButtonChecked(IDC_RBTN_MSG_RANGE);
                if(bButtonChecked != 0)
                {
                    UINT unMsgIDFrom = 0;
                    UINT unMsgIDTo   = 0;
                    m_omStrSelectedItemText = defMSG_IDRANGE_HANDLER;
                    m_omStrSelectedItemText += m_omStrMsgIDFrom;
                    m_omStrSelectedItemText += "_";
                    m_omStrSelectedItemText += m_omStrMsgIDTo;
                    unMsgIDTo    = (UINT)m_odEditMsgIDTo.lGetValue();
                    unMsgIDFrom  =(UINT) m_odEditMsgIDFrom.lGetValue();
                    if(unMsgIDFrom < unMsgIDTo)
                    {
                        omStrText = "\""+ m_omStrMsgIDFrom;
                        omStrText += "-";
                        omStrText += m_omStrMsgIDTo;
                        omStrText += "\"";
                        omStrHandlerType = _(defMSG_IDRANGE_TEXT);
                    }
                    else
                    {
                        bReturn = FALSE;
                        AfxMessageBox(_(defSTRMSG_RANGE_INVALID));
                        m_odEditMsgIDFrom.SetFocus();
                    }
                }
                else
                {
                    bButtonChecked =  IsDlgButtonChecked(IDC_RBTN_MSG_ALL);
                    if(bButtonChecked != 0)
                    {
                        m_omStrSelectedItemText = defMSG_ALL;
                        omStrHandlerType = _(defMSG_ALL_TEXT);
                        omStrText = _("message");
                    }
                }
            }

        }

        if(bReturn == TRUE)
        {
            if (pMsgArray)
            {
                CString omStrMsgPro = STR_EMPTY;
                CString omStrMsgCommon = CGlobalObj::omGetBusSpecMsgHndlrName(m_eBus);
                for (int nCount = 0; nCount<pMsgArray->GetSize(); nCount++ )
                {
                    omStrMsgPro = pMsgArray->GetAt( nCount );
                    omStrMsgPro = omStrMsgPro.Right(omStrMsgPro.GetLength()
                                                    - omStrMsgPro.Find(" ") - 1);
                    omStrMsgPro = omStrMsgPro.Right(omStrMsgPro.GetLength()
                                                    - omStrMsgCommon.GetLength());
                    omStrMsgPro = omStrMsgPro.Left( omStrMsgPro.Find("(") );
                    omStrMsgPro.TrimLeft();
                    omStrMsgPro.TrimRight();
                    if(omStrHandlerType == _(defMSG_IDLIST_TEXT) )
                    {
                        if (omStrMsgPro.Find(defMSG_IDLIST_HANDLER) >= 0 )
                        {
                            omStrMsgPro = omStrMsgPro.Right(omStrMsgPro.GetLength()
                                                            - omStrMsgPro.Find(defMSG_IDLIST_HANDLER) - strlen(defMSG_IDLIST_HANDLER));

                            CString omStrTemp;
                            int nCurPos= 0;
                            /*omStrTemp= omStrMsgPro.Tokenize("_",nCurPos);*/
                            vTokenize(omStrMsgPro, "_", omStrTemp, nCurPos);
                            while (omStrTemp != "")
                            {
                                CHAR* pchTemp;
                                UINT unMsgId = strtoul(omStrTemp, &pchTemp, 16);
                                if (omMsgList.Find(unMsgId, 0) != NULL)
                                {
                                    omStrMsgPro = _(defMSG_DUPL_MSG_HANDLER);
                                    omStrMsgPro.Replace( _T(_("TYPE")),
                                                         omStrHandlerType );
                                    omStrMsgPro.Replace( _T(_("MESSAGENAME")),
                                                         omStrText );

                                    omStrMsgPro += _T("In\"") + pMsgArray->GetAt( nCount ) +_T("\" Handler");
                                    // Duplicate message hanlder, shout
                                    AfxMessageBox( omStrMsgPro,
                                                   MB_OK|MB_ICONINFORMATION );
                                    nCount = (COMMANINT)pMsgArray->GetSize();
                                    bReturn = FALSE;
                                    break;
                                }
                                /*omStrTemp = omStrMsgPro.Tokenize("_",nCurPos);*/
                                vTokenize(omStrMsgPro, "_", omStrTemp, nCurPos);
                                //omMsgList.AddTail(unMsgID);
                            };
                            //break;
                        }
                    }
                    ///////////////////////////////////////////
                    else if (omStrMsgPro.Compare(m_omStrSelectedItemText) == 0)
                    {
                        // Form the message to be dispalyed
                        omStrMsgPro = defMSG_DUPL_MSG_HANDLER;
                        omStrMsgPro.Replace( "TYPE",
                                             omStrHandlerType );
                        omStrMsgPro.Replace( "MESSAGENAME",
                                             omStrText );
                        // Duplicate message hanlder, shout
                        AfxMessageBox( omStrMsgPro,
                                       MB_OK|MB_ICONINFORMATION );
                        nCount = (COMMANINT)pMsgArray->GetSize();
                        bReturn = FALSE;
                        if(omStrHandlerType.Compare(defMSG_NAME_TEXT) ==0 )
                        {
                            m_omListMsgName.SetFocus();
                        }
                        else if(omStrHandlerType.Compare(defMSG_ID_TEXT) == 0 )
                        {
                            m_odEditMsgID.SetFocus();
                        }
                        else if(
                            omStrHandlerType.Compare(defMSG_IDRANGE_TEXT) == 0 )
                        {
                            m_odEditMsgIDFrom.SetFocus();
                        }
                        break;
                    }
                }
            }
            else
            {
                bReturn = FALSE;
            }
        }
    }
    else
    {
        bReturn = FALSE;
    }
    // Remove the selected item from the list
    if( bReturn == TRUE && bButtonChecked == TRUE)
    {
        int nIndex = m_omListMsgName.GetCurSel();
        if( nIndex != -1 )
        {
            m_omListMsgName.DeleteString( nIndex );
        }
    }

    return bReturn;
}

/******************************************************************************/
/*  Function Name    :  OnUpdateEditMsgHandlerId                              */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called when selection is changed in  */
/*                      message ID handler edit box control.Enable apply and  */
/*                      OK button                                             */
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  21.02.2003                                            */
/*  Modification By  :                                                        */
/*  Modifications on :                                                        */
/******************************************************************************/
void CMsgHandlerDlg::OnUpdateEditMsgHandlerId()
{
    m_omButtonApply.EnableWindow(TRUE);
    m_omButtonOK.EnableWindow(TRUE);
}
/******************************************************************************/
/*  Function Name    :  OnUpdateEditMsgHandlerIdFrom                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called when selection is changed in  */
/*                      message ID range (From )handler edit box control.     */
/*                      Enable apply and OK button                            */
/*  Member of        :  CMsgFilter                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  21.02.2003                                            */
/*  Modification By  :                                                        */
/*  Modifications on :                                                        */
/******************************************************************************/
void CMsgHandlerDlg::OnUpdateEditMsgHandlerIdFrom()
{
    m_omButtonApply.EnableWindow(TRUE);
    m_omButtonOK.EnableWindow(TRUE);
}
/******************************************************************************/
/*  Function Name    :  OnUpdateEditMsgHandlerIdTo                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called when selection is changed in  */
/*                      message ID range (TO )handler edit box control. Enable*/
/*                      apply and OK button                                   */
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  21.02.2003                                            */
/*  Modification By  :                                                        */
/*  Modifications on :                                                        */
/******************************************************************************/
void CMsgHandlerDlg::OnUpdateEditMsgHandlerIdTo()
{
    m_omButtonApply.EnableWindow(TRUE);
    m_omButtonOK.EnableWindow(TRUE);
}
/******************************************************************************/
/*  Function Name    :  bAddMessageNameInListBox                              */
/*  Input(s)         :  const CStringArray *pomStrArray : All handlers        */
/*                      const CString &omStrMessageName : current message name*/
/*  Output           :  TRUE/FALSE                                            */
/*  Functionality    :  Message name is checked for corresponding message name*/
/*                      and message Id handler already written. If it is      */
/*                      written, it will not added in the message list of     */
/*                      in the list box controll.                             */
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  06.03.2003                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  07.04.2003, review comment incorporated               */
/******************************************************************************/
BOOL CMsgHandlerDlg::bAddMessageNameInListBox(const CStringArray* pomStrArray,
        const CString& omStrMessageName)
{
    //return true;
    BOOL bReturn = TRUE;
    if(pomStrArray != NULL )
    {
        UINT nCount = 0;
        nCount = (COMMANUINT)pomStrArray->GetSize();
        CString omStrFunction = "";
        CString omStrMsgNameHandler = "";
        int nIndex = 0;
        for ( UINT i = 0; i<nCount; i++)
        {
            omStrFunction = pomStrArray->GetAt(i);
            omStrMsgNameHandler  = CGlobalObj::omGetBusSpecMsgHndlrName(m_eBus);
            omStrMsgNameHandler += defMSG_NAME_HANDLER;
            omStrMsgNameHandler +=  omStrMessageName;
            nIndex = omStrFunction.Find(omStrMsgNameHandler);
            if(nIndex != -1)
            {
                bReturn = FALSE;
                i = nCount;
            }
            else
            {
                CString omStrTemp = CGlobalObj::omGetBusSpecMsgHndlrName(m_eBus);
                omStrTemp += defMSG_ID_HANDLER;
                nIndex = omStrFunction.Find(omStrTemp);
                if(nIndex != -1 )
                {
                    CHAR* pcStopStr = NULL;
                    UINT unMsgID = 0;
                    omStrTemp = omStrFunction.Left(omStrFunction.Find('('));
                    omStrTemp.TrimLeft();
                    omStrTemp.TrimRight();
                    omStrTemp = omStrTemp.Right(omStrTemp.GetLength() -
                                                omStrTemp.ReverseFind('_') -1);
                    unMsgID = strtol((LPCTSTR )omStrTemp,&pcStopStr,16);
                    if(unMsgID>0)
                    {

                        /*omStrTemp = CMsgSignal::ouGetMsgSignal().
                            omStrGetMessageNameFromSlotID(unMsgID);*/
                        if(omStrMessageName == omStrTemp)
                        {
                            bReturn = FALSE;
                            i = nCount;
                        }
                    }
                }
            }
        }
        if(bReturn == TRUE )
        {
            m_omListMsgName.AddString(omStrMessageName);
        }
    }
    else // if pomStrArray is NULL
    {
        bReturn = FALSE;
    }

    return bReturn;
}
/******************************************************************************/
/*  Function Name    :  bValidateMessageNameAndID                             */
/*  Input(s)         :  const CStringArray* pomStrArray : all message handler */
/*                                                        already written     */
/*                      const CString& omStrHandlerType : Type of message     */
/*                                                         handler            */
/*                      CString omStrMessageNameOrID: Corresponding message   */
/*                                                    name or message Id      */
/*  Output           :  TRUE/FALSE                                            */
/*  Functionality    :  This function will validate the duplicate message     */
/*                      handler. If messsage name handler is written, it will */
/*                      validate if corresponding message Id handler is       */
/*                      already written and vice versa.                       */
/*  Member of        :  CMsgHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  12.03.2003                                            */
/*  Modification By  :                                                        */
/*  Modifications on :                                                        */
/******************************************************************************/
BOOL CMsgHandlerDlg::bValidateMessageNameAndID(const CStringArray* pomStrArray,
        const CString& omStrHandlerType,
        CString omStrMessageNameOrID)
{
    BOOL bReturn = TRUE;
    CString omStrMakeHandler = "";
    if(pomStrArray != NULL )
    {
        omStrMakeHandler = CGlobalObj::omGetBusSpecMsgHndlrName(m_eBus);
        // compare if the handler written is message name or message ID.
        if(omStrHandlerType.Compare(defMSG_ID_HANDLER) == 0)
        {
            omStrMakeHandler += defMSG_NAME_HANDLER;
        }
        else if(omStrHandlerType.Compare(defMSG_NAME_HANDLER) == 0)
        {
            omStrMakeHandler += defMSG_ID_HANDLER;
        }
        // Make the current handler by getting its ID from name and vice
        // versa.
        omStrMakeHandler += omStrMessageNameOrID;
        UINT unCount = (COMMANUINT)pomStrArray->GetSize();
        CString omStrMsgPro = "";
        for(UINT i = 0; i<unCount; i++)
        {
            omStrMsgPro = pomStrArray->GetAt( i );
            omStrMsgPro = omStrMsgPro.Right(omStrMsgPro.GetLength()
                                            - omStrMsgPro.Find(" ") - 1);
            omStrMsgPro = omStrMsgPro.Left( omStrMsgPro.Find("(" ));
            omStrMsgPro.TrimLeft();
            omStrMsgPro.TrimRight();
            if (omStrMsgPro.Compare(omStrMakeHandler) == 0)
            {
                // Form the message to be dispalyed
                omStrMsgPro = _(defDUPL_CORR_MSG_HANDLER);
                if(omStrHandlerType.Compare(defMSG_ID_HANDLER) == 0)
                {
                    omStrMsgPro.Replace( "TYPE", _(defMSG_NAME_TEXT));
                    m_odEditMsgID.SetFocus();
                }
                else if(omStrHandlerType.Compare(defMSG_NAME_HANDLER) == 0)
                {
                    omStrMsgPro.Replace( "TYPE", _(defMSG_ID_TEXT));
                    omStrMessageNameOrID.Insert(0,"0x");
                    m_omListMsgName.SetFocus();
                }
                omStrMsgPro.Replace( "MESSAGENAME",omStrMessageNameOrID );
                // Duplicate message hanlder, shout
                AfxMessageBox( omStrMsgPro,MB_OK|MB_ICONINFORMATION );
                unCount = (COMMANUINT)pomStrArray->GetSize();
                bReturn = FALSE;
            }
        }
    }
    else
    {
        bReturn = FALSE;
    }
    return bReturn ;
}

/**
* \brief         This function finds the next token in the target string

* \param[in]     None Takes input from yytext
* \return        Void
* \authors       Saravanan K S
* \date
*/
void CMsgHandlerDlg::vTokenize(CString strInput, CString strToken, CString& strOutput, int& nStart)
{
    int nIdx = strInput.Find(strToken, nStart);
    int nCount = 0;
    strOutput.Empty();

    if (nIdx != -1)
    {
        nCount = nIdx - nStart; // Calculate the number of characters to extract
        strOutput = strInput.Mid(nStart, nCount); //Extract the charatcers
    }
    else if( nStart < strInput.GetLength() )
    {
        strOutput = strInput.Right(strInput.GetLength() - nStart);
        nIdx = strInput.GetLength()-1;
    }
    else
    {
        strOutput.Empty();
    }
    nStart = nIdx + 1;
}