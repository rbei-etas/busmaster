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
 * \file      ExploreMsgSg.cpp
 * \brief     This file contain definition of all function of
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */

#include "NodeSimEx_stdafx.h"             // Standard include header file
#include "NodeSimEx_resource.h"
#include "FunctionEditorDoc.h"  // Document defintion file
#include "ExploreMsgSg.h"       // Class defintion file
#include "Utility\MultiLanguageSupport.h"
//#include "../Application/GettextBusmaster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/******************************************************************************/
/*  Function Name    :  CExploreMsgSg                                         */
/*                                                                            */
/*  Input(s)         :  enumerator to indicate selection of message or signal */
/*  Output           :                                                        */
/*  Functionality    :  Constructor
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :
/******************************************************************************/

CExploreMsgSg::CExploreMsgSg( CFunctionEditorDoc* pDoc,
                              CMsgNameMsgCodeListDataBase& odMsgNameCodeListDb, //CAPL_DB_NAME_CHANGE
                              BOOL bCheckRequired,
                              eMESSAGEFROM eWindow,
                              eSELTYPE eSelType,
                              CGlobalObj* pGlobalObj,
                              CWnd* pParent /*=nullptr*/)
    : CDialog(CExploreMsgSg::IDD, pParent),
      m_odMsgNameCodeListDb(odMsgNameCodeListDb)            //CAPL_DB_NAME_CHANGE
{
    //{{AFX_DATA_INIT(CExploreMsgSg)
    m_bWantStructure = FALSE;
    //}}AFX_DATA_INIT
    m_eSelectType       = eSelType;
    m_bWantStructure    = FALSE;
    m_bCheckRequired    = bCheckRequired;
    m_eWindow           = eWindow;
    m_omStrMessageName  = "";
    m_pDoc              = pDoc;
    m_bIsPduFound       = true;
    m_pGlobalObj = pGlobalObj;
}


void CExploreMsgSg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CExploreMsgSg)
    DDX_Control(pDX, IDC_LSTB_SIGNAL, m_omSignalListBox);
    DDX_Control(pDX, IDC_LSTC_MSGS, m_omMsgList);
    DDX_Check(pDX, IDC_CHKB_WANT_STRUCTURE, m_bWantStructure);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExploreMsgSg, CDialog)
    //{{AFX_MSG_MAP(CExploreMsgSg)
    ON_NOTIFY(NM_DBLCLK, IDC_LSTC_MSGS, OnDblclkListMsgs)
    ON_LBN_DBLCLK(IDC_LSTB_SIGNAL, OnDblclkListSignals)
    ON_BN_CLICKED(IDC_CBTN_OK, OnSelect)
    ON_BN_CLICKED(IDC_CBTN_CANCEL, OnCbtnCancel)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_MSGS, OnItemchangedLstcMsgs)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_MSGS, OnClickLstcMsgs)
    //ON_CBN_SELCHANGE(IDC_CMB_CHANNEL_SEL, OnSelChangeChnlCombo)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExploreMsgSg message handlers

/******************************************************************************/
/*  Function Name    :  OnDblclkListMsgs                                      */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user double clicks on a message,      */
/*                      and calls OnSelect() function                         */
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :  Amitesh Bharti,                                       */
/*                      20.05.2003, call OnSelect to close the dialog only if */
/*                      user wants to select message.                         */
/*  Modifications    :  Raja N                                                */
/*                      11.03.2004, Set the focus to the last selected item   */
/*                      if the user double clicks outside                     */
/*  Modifications    :  Raja N on 31.05.2004                                  */
/*                      Added check for message list view dbl click on msg for*/
/*                      Global variable addition                              */
/******************************************************************************/
void CExploreMsgSg::OnDblclkListMsgs(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    if(m_omMsgList.GetNextItem(-1, LVNI_SELECTED) == -1)
    {
        m_omMsgList.SetItemState(m_nMsgIndex, LVNI_SELECTED | LVNI_FOCUSED,
                                 LVNI_SELECTED | LVNI_FOCUSED);
    }

    if ( m_eSelectType == SEL_MESSAGE || m_eSelectType == SEL_GLOBAL_MESSAGE)
    {
        OnSelect();
    }
    *pResult = 0;
}
/******************************************************************************/
/*  Function Name    :  OnDblclkListSignals                                   */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user double clicks on a signal,       */
/*                      and calls OnSelect() function                         */
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :  Amitesh Bharti,                                       */
/*                      20.05.2003, call OnSelect to close the dialog only if */
/*                      user wants to select singal.                          */
/******************************************************************************/

void CExploreMsgSg::OnDblclkListSignals()
{
    if ( m_eSelectType == SEL_SIGNAL || m_eSelectType == SEL_PDU)
    {
        OnSelect();
    }
}
/******************************************************************************/
/*  Function Name    :  OnSelect                                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user clicks on Select button. This
                        function gets the selected message or signal.
                        Associates signal with the message if the selection
                        mode is SEL_SIGNAL. Also forms the structure variable
                        if that option is checked for that message
                                                                              */
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :  Amitesh Bharti
                        13.06.2002 While making message structure string,
                        the message name is replace with MSG_STRUCT_VAR for
                        both message selection and signal selection if user
                        choose to create message structure.Space and Semi Colon
                        is removed from #define MSG_STRUCT_VAR and put
                        seperatly.
/*  Modifications    :  Raja N                                                */
/*                      10/03/2004 Modified the message and signal structure  */
/*                      creation to include init of database messages.        */
/*  Modifications    :  Raja N on 31.05.2004                                  */
/*                      Added code to insert message variable with structure  */
/*                      for Global variable addition                          */
/******************************************************************************/

void CExploreMsgSg::OnSelect()
{
    UpdateData(TRUE);


    // Get Main Frame Window Pointer
    CFunctionEditorDoc* pDoc = nullptr;
    // For global variable addition structure is required
    if( m_eSelectType == SEL_GLOBAL_MESSAGE)
    {
        // Same as select message
        m_eSelectType = SEL_MESSAGE;
        // Enable structure definition
        m_bWantStructure = TRUE;
    }


    // Get the Document Object
    if (m_bWantStructure == TRUE)
    {
        pDoc = m_pDoc;
    }

    if ( m_eSelectType == SEL_MESSAGE )
    {
        // Get selected message text
        m_omStrMessageName = m_omStrSelectedItemText =
                                 m_omMsgList.GetItemText( m_nMsgIndex, 0);
        UINT unMsgID = (COMMANUINT)m_omMsgList.GetItemData(m_nMsgIndex);

        if (m_bWantStructure == TRUE)
        {
            // Get the Initialised string from document
            if (pDoc != nullptr)
            {
                //To pass the actual name of message
                int nIndex = m_omStrMessageName.ReverseFind(defMSGID_NAME_START_CHAR);
                if(nIndex != -1)
                {
                    m_omStrSelectedItemText = m_omStrMessageName.Left(nIndex);
                    m_omStrMessageName = m_omStrSelectedItemText;
                }



                CString omStrMsgStructure =
                    pDoc->omStrGetInitialisedMessage(unMsgID,
                                                     m_omStrSelectedItemText,
                                                     MSG_STRUCT_VAR,TRUE,nGetSelChannel(), nGetSelChannel());
                m_omStrSelectedItemText = omStrMsgStructure;
            }
        }
    }
    else
    {
        // User wants signal to be selected
        CString omStrMsg = m_omMsgList.GetItemText(m_nMsgIndex, 0);
        UINT unMsgID = (COMMANUINT)m_omMsgList.GetItemData(m_nMsgIndex);
        int nSgIndex = (COMMANUINT)m_omSignalListBox.GetCurSel();

        if ( nSgIndex != -1 )
        {
            // Get selected signal text
            CString omStrSgName = "";
            m_omSignalListBox.GetText( nSgIndex, omStrSgName );

            if ( m_bWantStructure )
            {
                omStrSgName.Insert( 0, (char)PERIOD );

                ////To pass the actual name of message
                int nIndex = omStrMsg.ReverseFind(defMSGID_NAME_START_CHAR);
                if( nIndex!=0 )
                {
                    omStrMsg = omStrMsg.Left(nIndex);
                }

                if(nullptr != pDoc)
                {
                    // Get the Initialised string from document
                    CString omStrMsgStructure =
                        pDoc->omStrGetInitialisedMessage(unMsgID,
                                                         omStrMsg,
                                                         MSG_STRUCT_VAR,TRUE,nGetSelChannel());
                    // Form the declaration and signal access statements
                    m_omStrSelectedItemText.Format(defFNS_INIT_SIG_FORMAT,
                                                   omStrMsgStructure,
                                                   MSG_STRUCT_VAR,
                                                   defSIGNALMEMBER);
                }
            }
            m_omStrSelectedItemText += omStrSgName;
        }
    }
    CDialog::OnOK();
}

void CExploreMsgSg::vPopulateMsgList(std::map<std::string, unsigned int>& mapMsgNameAndCode)
{
    //GetMsgList for channel - 0.
    vGetMsgList(m_eBus, 0, mapMsgNameAndCode);
}

/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Initialises the dialog with all the messages of active*/
/*                      database. Disables signal ist if the user asks for the*/
/*                      message name.                                         */
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :  Raja N on 11.03.2004                                  */
/*                      Included init of m_nMsgIndex to the selected item     */
/*  Modifications    :  Raja N on 31.05.2004                                  */
/*                      Added new type to have message list view alone for    */
/*                      Global variable addition                              */
/*  Modifications    :  Raja N on 04.06.2004                                  */
/*                      Corrected the positioning problem with control in     */
/*                      case of font change.                                  */
/******************************************************************************/
BOOL CExploreMsgSg::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Disable the chck if not required
    if ( m_bCheckRequired )
    {
        GetDlgItem(IDC_CHKB_WANT_STRUCTURE)->EnableWindow( TRUE );
    }
    else
    {
        GetDlgItem(IDC_CHKB_WANT_STRUCTURE)->EnableWindow( FALSE );
    }


    // Disable the signal list box
    // if the user wants message list only
    if ( m_eSelectType == SEL_MESSAGE )
    {
        GetDlgItem(IDC_LSTB_SIGNAL)->ShowWindow(FALSE);
        vResizeDialog();
    }
    else if( m_eSelectType == SEL_GLOBAL_MESSAGE)
    {
        CRect omDRect, omLRect, omBRect;
        CWnd* pWnd = GetDlgItem(IDC_CHKB_WANT_STRUCTURE);
        if( pWnd != nullptr )
        {
            // Hide want structure item
            pWnd->ShowWindow(SW_HIDE);
        }
        vResizeDialog();
    }


    //  Get the msg list rectangle
    RECT rRect;

    GetDlgItem(IDC_LSTC_MSGS)->GetWindowRect(&rRect);

    ScreenToClient(&rRect);

    // Insert a column to the list control
    m_omMsgList.InsertColumn( 0, "", LVCFMT_LEFT, rRect.bottom );

    m_omMsgList.DeleteAllItems();

    // Init last selection to -1
    m_nMsgIndex = -1;

    //Populate channel
    vPopulateChannel( nGetChannelCount( m_eBus ) );

    std::map<std::string,unsigned int> mapMsgNameAndCode;

    vPopulateMsgList(mapMsgNameAndCode);
    vPopulateListCtrl(m_omMsgList,mapMsgNameAndCode,true);

    //Set Window Text
    SetWindowText(g_strArrWindowText[m_eSelectType].c_str());

    return FALSE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Calls base class OnCanCel function                    */
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.04.2002                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CExploreMsgSg::OnCbtnCancel()
{
    CDialog::OnCancel();
}

void CExploreMsgSg::vPopulateSignalList(INT unChannelNum, DWORD dwMsgCode, std::list<std::string>& lstSigNames)
{
    vGetSignalList(m_eBus, unChannelNum, dwMsgCode, lstSigNames);
    vPopulateListBox(m_omSignalListBox, lstSigNames);
}

/******************************************************************************/
/*  Function Name    :  OnItemchangedLstcMsgs                                 */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user changes selection on a message,  */
/*                      and displays corresponding signals in signal list     */
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  20.01.2004                                            */
/*  Modifications    :  Raja N on 11.03.2004                                  */
/*                      Included assignment of selected item variable while   */
/*                      the item selection got changed                        */
/******************************************************************************/
void CExploreMsgSg::OnItemchangedLstcMsgs(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // User wants the signal details
    if ( m_eSelectType == SEL_SIGNAL || m_eSelectType == SEL_PDU)
    {
        if(pNMListView->uNewState == (LVIS_FOCUSED |LVIS_SELECTED) )
        {
            // Delete every other item from the list box.
            m_omSignalListBox.ResetContent();

            // Get handle to selected message item
            int hSelItem = pNMListView->iItem;

            if ( (hSelItem != -1) )
            {
                CStringList omSignalNames;

                omSignalNames.RemoveAll();

                // Store last selection
                m_nMsgIndex = hSelItem;

                // Get selected message name
                m_omStrMessageName = m_omMsgList.GetItemText( hSelItem, 0 );

                DWORD dwMsgCode = (COMMANDWORD)m_omMsgList.GetItemData(hSelItem);
                CString strChnlNum;
                ((CComboBox*)GetDlgItem(IDC_CMB_CHANNEL_SEL))->GetWindowTextA(strChnlNum);
                INT unChannelNum = atoi(strChnlNum) - 1;
                std::list<std::string> lstSigNames;

                vPopulateSignalList(unChannelNum, dwMsgCode, lstSigNames);

                m_omSignalListBox.SetCurSel(0);
                SetHorizExtentListBox(m_omSignalListBox);
            }

        }
    }
    else
    {
        // Store last selection
        m_nMsgIndex = pNMListView->iItem ;
    }
    *pResult = 0;
}

/******************************************************************************/
/*  Function Name    :  OnClickLstcMsgs                                       */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user changes selection on a message,  */
/*                      and this will store the selection. If the user clicks */
/*                      clicks outside this will set the focus to the last    */
/*                      selected item                                         */
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Raja N                                                */
/*  Date Created     :  11.03.2004                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CExploreMsgSg::OnClickLstcMsgs(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    int nIndex = m_omMsgList.GetNextItem(-1, LVNI_SELECTED);

    if( nIndex == -1)
    {
        m_omMsgList.SetItemState(m_nMsgIndex, LVNI_SELECTED | LVNI_FOCUSED,
                                 LVNI_SELECTED | LVNI_FOCUSED);
    }
    else
    {
        m_nMsgIndex = nIndex;
    }
    *pResult = 0;
}

void CExploreMsgSg::vGetSigNamesFromMsgCode(DWORD dwMsgCode, CStringList& omSignalNames)
{
    SMSG_NAME_CODE sMsgCodeName;
    omSignalNames.RemoveAll();
    sMsgCodeName.m_dwMsgCode = dwMsgCode;
    for(int nItr =0; nItr < m_odMsgNameCodeListDb.GetCount(); nItr++)
    {
        POSITION pos = m_odMsgNameCodeListDb.FindIndex(nItr);
        if( nullptr != pos )
        {
            SDB_NAME_MSG& sDbNameMsg = m_odMsgNameCodeListDb.GetAt(pos);
            POSITION pos1 = sDbNameMsg.m_oMsgNameMsgCodeList.Find(sMsgCodeName);
            if (pos1 != nullptr)
            {
                sMsgCodeName = sDbNameMsg.m_oMsgNameMsgCodeList.GetAt(pos1);
                omSignalNames.AddTail(&(sMsgCodeName.m_omSignalNames));
            }
        }
    }
}

void CExploreMsgSg::vSetFrameList(IBMNetWorkGetService* ouClusterConfig, ETYPE_BUS eBus)
{
    m_ouClusterConfig = ouClusterConfig;
    m_eBus = eBus;
}
/**************************************************************************************
    Function Name    :  vGetMsgList
    Input(s)         :  ETYPE_BUS eBus,int nChannel,std::map<std::string,unsigned int>& mapMsgNameAndCode
    Output           :  void
    Functionality    :  Gets map of message name and code, in the database.
    Member of        :  CExploreMsgSg
    Author(s)        :  Robin G.K.
    Date Created     :  27.10.2015
    Requirement ID   :  RS_FLX_08_09_10
***************************************************************************************/
void CExploreMsgSg::vGetMsgList(ETYPE_BUS eBus,int nChannel,std::map<std::string,unsigned int>& mapMsgNameAndCode)
{
    std::list<IFrame*> ouFrameList;
    IBMNetWorkGetService* pouClusterConfig = m_pGlobalObj->m_ouClusterConfig;
    if(nullptr!=pouClusterConfig)
    {
        pouClusterConfig->GetFrameList(eBus, nChannel, ouFrameList);
    }

    std::list<IFrame*>::iterator itrLstMsg = ouFrameList.begin();
    int nIndex = 0;
	LinFrameProps props;
    while(itrLstMsg != ouFrameList.end())
    {
        CString omMsg;
        std::string omFrameName;
        (*itrLstMsg)->GetName(omFrameName);

        (*itrLstMsg)->GetProperties(props);
        unsigned int unFrameId = 0;
        switch(eBus)
        {
            case LIN:
				if (props.m_eLinFrameType != eLinUnconditionalFrame && props.m_eLinFrameType != eLinDiagnosticFrame)
                {
                    itrLstMsg++;
                    continue;
                }
                (*itrLstMsg)->GetFrameId(unFrameId);
                omMsg.Format("%s[0x%x]", omFrameName.c_str(), (unFrameId));
                break;
            case CAN:
            case J1939:
                ( *itrLstMsg )->GetFrameId( unFrameId );
                omMsg.Format( "%s[0x%x]", omFrameName.c_str(), ( unFrameId ) );
                break;
        }
        mapMsgNameAndCode.insert(std::pair<std::string,unsigned int>(std::string(omMsg),unFrameId));
        itrLstMsg++;
    }
}

/**************************************************************************************
    Function Name    :  vGetSignalList
    Input(s)         :  ETYPE_BUS eBus,int nChannel, DWORD dwMsgCode,std::list<std::string>& lstSigNames
    Output           :  void
    Functionality    :  Gets list of Signal names for the given Message Code.
    Member of        :  CExploreMsgSg
    Author(s)        :  Robin G.K.
    Date Created     :  27.10.2015
    Requirement ID   :  RS_FLX_08_09_10
***************************************************************************************/
void CExploreMsgSg::vGetSignalList(ETYPE_BUS eBus,int nChannel, DWORD dwMsgCode,std::list<std::string>& lstSigNames)
{
    ICluster* pCluster = nullptr;
    IBMNetWorkGetService* pouClusterConfig = m_pGlobalObj->m_ouClusterConfig;
    if(nullptr!=pouClusterConfig)
    {
        pouClusterConfig->GetDBService(eBus, nChannel, 0, &pCluster);
    }
    IFrame* ouframeStrct = nullptr;
    unsigned int unFrameId = dwMsgCode;
    switch (eBus)
    {
        case CAN:
        case LIN:
        case J1939:
            if (nullptr != pCluster)
            {
                pCluster->GetFrame(unFrameId, nullptr, &ouframeStrct);
            }
            break;
    }
	
	std::map< ISignal*, SignalInstanse> mapSignals;

    if(nullptr != pCluster && nullptr != ouframeStrct)
    {
        ouframeStrct->GetSignalList(mapSignals);
    }

    std::string omSignalName;
	for(auto itrSignal : mapSignals)
    {
        (itrSignal.first)->GetName(omSignalName);
        lstSigNames.push_back(omSignalName);
    }

}
//
///**************************************************************************************
//    Function Name    :  vGetSignalList
//    Input(s)         :  ETYPE_BUS eBus,int nChannel,std::string strPduName, std::list<std::string>& lstSignalNames
//    Output           :  void
//    Functionality    :  Gets list of Signal names for the given Pdu Name.
//    Member of        :  CExploreMsgSg
//    Author(s)        :  Robin G.K.
//    Date Created     :  27.10.2015
//    Requirement ID   :  RS_FLX_08_09_10
//***************************************************************************************/
//void CExploreMsgSg::vGetSignalList(ETYPE_BUS eBus,int nChannel,std::string strPduName, std::list<std::string>& lstSignalNames)
//{
//    ICluster* pCluster = nullptr;
//    std::map<ISignal*, SignalInstanse> mapSignals;
//    IBMNetWorkGetService* pouClusterConfig = m_pGlobalObj->m_ouClusterConfig;
//    if(nullptr!=pouClusterConfig)
//    {
//        pouClusterConfig->GetDBService(eBus, nChannel, 0, &pCluster);
//    }
//    IPdu* pPdu = nullptr;
//    pCluster->GetElementByName(ePduElement, strPduName,  reinterpret_cast<IElement**>(&pPdu));
//    if (nullptr != pPdu )
//    {
//        pPdu->GetSignalList(mapSignals);
//    }
//    std::string omSignalName;
//for(auto itrSignal:mapSignals)
//    {
//        (itrSignal.first)->GetName(omSignalName);
//        lstSignalNames.push_back(omSignalName);
//    }
//}
/**************************************************************************************
    Function Name    :  vPopulateListCtrl
    Input(s)         :  CListCtrl& omListCtrl,const std::map<std::string,unsigned int>& mapNames,bool bSetItemData
    Output           :  void
    Functionality    :  Populates ListCtrl for the given map of string and dataitem.
    Member of        :  CExploreMsgSg
    Author(s)        :  Robin G.K.
    Date Created     :  27.10.2015
    Requirement ID   :  RS_FLX_08_09_10
***************************************************************************************/
void CExploreMsgSg::vPopulateListCtrl(CListCtrl& omListCtrl,const std::map<std::string,unsigned int>& mapNames,bool bSetItemData)
{
    int nIndex = 0;
for(auto itr: mapNames)
    {
        int nInsertedIndex = omListCtrl.InsertItem(nIndex++, itr.first.c_str());
        if(true == bSetItemData)
        {
            omListCtrl.SetItemData(nInsertedIndex,itr.second);
        }
    }
}
/**************************************************************************************
    Function Name    :  vPopulateListBox
    Input(s)         :  CListBox& omListBox,const std::list<std::string> lstItems
    Output           :  void
    Functionality    :  Populates ListBox for the given list of string items.
    Member of        :  CExploreMsgSg
    Author(s)        :  Robin G.K.
    Date Created     :  27.10.2015
    Requirement ID   :  RS_FLX_08_09_10
***************************************************************************************/
void CExploreMsgSg::vPopulateListBox(CListBox& omListBox,const std::list<std::string> lstItems)
{
    int nIndex = 0;
    std::string strItem;
for(auto itr:lstItems)
    {
        omListBox.InsertString(nIndex++,itr.c_str());
    }
}
/**************************************************************************************
    Function Name    :  SetHorizExtentListBox
    Input(s)         :  CListBox& omListBox
    Output           :  void
    Functionality    :  Sets the Listbox scroll to an extent so as to view the longest
                        string that is inserted.
    Member of        :  CExploreMsgSg
    Author(s)        :  Robin G.K.
    Date Created     :  27.10.2015
    Requirement ID   :  RS_FLX_08_09_10
***************************************************************************************/
void  CExploreMsgSg::SetHorizExtentListBox(CListBox& omListBox)
{
    // Set horizontal extent of the list
    // box to max string availaable
    // so that user can scroll
    CSize   sz;
    CString omStrText;
    CDC*  pDC = omListBox.GetDC();
    if ( pDC != nullptr)
    {
        int nDx = 0;
        for (int nCount = 0;
                nCount < omListBox.GetCount();
                nCount++)
        {
            omListBox.GetText( nCount, omStrText );
            // remove space
            omStrText.TrimRight();
            sz = pDC->GetTextExtent(omStrText);
            if (sz.cx > nDx)
            {
                nDx = sz.cx;
            }
        }
        omListBox.ReleaseDC(pDC);
        // Set the horizontal extent so every character of all
        // strings can be scrolled to.
        omListBox.SetHorizontalExtent(nDx);
    }
}
/**************************************************************************************
    Function Name    :  vPopulateChannel
    Input(s)         :  int nChannelCount
    Output           :  void
    Functionality    :  Populates the Channel ComboBox.
    Member of        :  CExploreMsgSg
    Author(s)        :  Robin G.K.
    Date Created     :  27.10.2015
    Requirement ID   :  RS_FLX_08_09_10
***************************************************************************************/
void CExploreMsgSg::vPopulateChannel(int nChannelCount)
{
    char chString[MAX_PATH] = "";
    for(INT unIndex = 0; unIndex < nChannelCount; unIndex++)
    {
        sprintf(chString, "%d", unIndex+1);
        ((CComboBox*)GetDlgItem(IDC_CMB_CHANNEL_SEL))->AddString(chString);
    }
    if(((CComboBox*)GetDlgItem(IDC_CMB_CHANNEL_SEL))->GetCount() > 0)
    {
        ((CComboBox*)GetDlgItem(IDC_CMB_CHANNEL_SEL))->SetCurSel(0);
    }
}
/**************************************************************************************
    Function Name    :  nGetChannelCount
    Input(s)         :  ETYPE_BUS eBus
    Output           :  int
    Functionality    :  Gets the Channel count for the given protocol.
    Member of        :  CExploreMsgSg
    Author(s)        :  Robin G.K.
    Date Created     :  27.10.2015
    Requirement ID   :  RS_FLX_08_09_10
***************************************************************************************/
int CExploreMsgSg::nGetChannelCount(ETYPE_BUS eBus)
{
    int nChannelCount=0;
    CBaseDIL_CAN* pBaseDIL_CAN=nullptr;
    IBMNetWorkGetService* pouClusterConfig = m_pGlobalObj->m_ouClusterConfig;
    if ( nullptr != pouClusterConfig )
    {
        pouClusterConfig->GetChannelCount( eBus, nChannelCount );
    }
    return nChannelCount;
}
/**************************************************************************************
    Function Name    :  nGetSelChannel
    Input(s)         :  void
    Output           :  int
    Functionality    :  Gets the Channel selected by the user.
    Member of        :  CExploreMsgSg
    Author(s)        :  Robin G.K.
    Date Created     :  27.10.2015
    Requirement ID   :  RS_FLX_08_09_10
***************************************************************************************/
int CExploreMsgSg::nGetSelChannel()
{
    CString strChnlSel = "";
    ((CComboBox*)GetDlgItem(IDC_CMB_CHANNEL_SEL))->GetWindowText(strChnlSel);
    return atoi(strChnlSel);
}

/**************************************************************************************
    Function Name    :  vResizeDialog
    Input(s)         :  void
    Output           :  void
    Functionality    :  Resizes the Dialog
    Member of        :  CExploreMsgSg
    Author(s)        :  Robin G.K.
    Date Created     :  27.10.2015
    Requirement ID   :  RS_FLX_08_09_10
***************************************************************************************/

void CExploreMsgSg::vResizeDialog()
{
    CRect omDRect, omLRect;
    // Get Dialog size
    GetWindowRect(&omDRect);
    CWnd* pWnd;
    // Get list control size
    pWnd = GetDlgItem(IDC_LSTC_MSGS);
    if( pWnd != nullptr )
    {
        // Convert it in to client co ordiantes
        pWnd->GetWindowRect(&omLRect);
        ScreenToClient(&omLRect);

        pWnd->MoveWindow(omLRect);

        // Change the postion of select button
        pWnd = GetDlgItem(IDC_CBTN_OK);
        if( pWnd != nullptr )
        {
            // Reduce the size by half and add right margin
            int nDialogWidth = (omDRect.Width() / 2) + omLRect.left - 1;
            MoveWindow( omDRect.left, omDRect.top,
                        nDialogWidth, omDRect.Height());
        }
    }

}