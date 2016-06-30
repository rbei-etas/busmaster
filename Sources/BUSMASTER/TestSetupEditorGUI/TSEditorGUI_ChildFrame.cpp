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
 * \file      TSEditorGUI_ChildFrame.cpp
 * \author    Venkatanarayana makam
 * \author    GT-Derka
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "TSEditorGUI_stdafx.h"
#include "TreeViewEx.h"
#include "TSEditorGUI_PropertyView.h"
#include "TSEditorGUI_ChildFrame.h"
#include "TestSetupEditorLib/VerifyEntity.h"
#include "TSEditorGUI_resource.h"
#include "TSEditorGUI_Definitions.h"
#include "TSEditorGUI_SettingsDlg.h"
#include "Include\Utils_macro.h"
#include "TSEditorGUI_MDIChildBase.h"
#include "include/XMLDefines.h"
#include "Utility/XMLUtils.h"
#include "Utility\MultiLanguageSupport.h"
#include "TSEditorGUI_PropertyView.h"   /* derka */
//#include "../Application/GettextBusmaster.h"
#include "Utility\UtilFunctions.h"
#include <htmlhelp.h>

#define MSG_GET_CONFIGPATH  10000
#define def_STR_SIGNAL_FORMAT   "%-50s %8d %8d\r\n"
#define def_STR_SIGNAL_HEADING  "%-50s %8s %8s\r\n\r\n"
#define CHECKENTITY(pEntity) if( pEntity == nullptr){return;}
#define CHECKEQ(ID, CONDITION) if(ID == (CONDITION)){return;}
#define CHECKEQRET(ID, CONDITION, RETVAL) if(ID == (CONDITION)){return (RETVAL);}

const UINT g_unMinWindowWidth  = 850;
const UINT g_unMinWindowHeight = 675;
//INT g_iNumberOfActiveChannels = 0; /* derka */

IMPLEMENT_DYNCREATE(CTSEditorChildFrame, CMDIChildWnd)
extern CTSEditorChildFrame* g_pomTSEditorChildWindow;
/******************************************************************************
Function Name  :  CTSEditorChildFrame
Input(s)       :  -
Output         :  -
Functionality  :  Constructor for CTSEditorChildFrame class
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam, GT-Derka
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
CTSEditorChildFrame::CTSEditorChildFrame()
{

    m_odTreeView = nullptr;
    m_hParentTreeItem = nullptr;
    m_omMenu.LoadMenu(IDR_TSEDITORMENU);
    m_hMenuShared = m_omMenu.GetSafeHmenu();
    m_bQueryConfirm = TRUE;
    m_pomImageList = nullptr;
    m_bInit = TRUE;
    m_iNumberOfActiveChannels = 0;
    vInitialise();

    CWnd* pMain = AfxGetMainWnd();
    if (pMain != nullptr)
    {
        m_pMainMenu = pMain->GetMenu()->Detach();
    }
    vSetDefaultWndPlacement();
    m_iNumberOfActiveChannels = CHANNEL_ALLOWED;
}

/******************************************************************************
Function Name  :  ~CTSEditorChildFrame
Input(s)       :  -
Output         :  -
Functionality  :  Destructor of the class CTSEditorChildFrame()
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
CTSEditorChildFrame::~CTSEditorChildFrame()
{
    m_omMenu.DestroyMenu();
    vInitialise();
    //Not rewuired in initialisation
    if(m_pomImageList != nullptr)
    {
        delete m_pomImageList;
    }
    m_pomImageList = nullptr;
}

/******************************************************************************
Function Name  :  vInitialise
Input(s)       :  -
Output         :  void
Functionality  :  Initialises the pointers and initialises the CTestsetupEntity
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vInitialise()
{
    m_bModified = FALSE;
    m_hCurrentTreeItem = nullptr;
    m_pCurrentEntity = nullptr;
    m_bFileSaved = TRUE;
    m_podCopyEntity = nullptr;
    m_bPasted = FALSE;

    m_ouTSEntity.vInitialise();
}


BEGIN_MESSAGE_MAP(CTSEditorChildFrame, CMDIChildWnd)
    ON_WM_DESTROY()
    ON_MESSAGE(WM_TS_SELCHANGED, OnSelectionChanged)
    ON_MESSAGE(WM_TS_SELCHANGING, OnSelectionChanging)
    ON_COMMAND(IDM_FILE_OPEN, OnFileOpen)
    ON_COMMAND(IDM_HELP_ABOUTTESTSETUPEDITOR, OnHelpAbouttestsetupeditor)
    ON_COMMAND(IDM_FILE_SAVE, OnFileSave)
    ON_UPDATE_COMMAND_UI(IDM_FILE_SAVE, OnUpdateFileSave)
    ON_COMMAND(IDM_FILE_NEW, OnFileNew)
    ON_COMMAND(IDM_FILE_SAVEAS, OnFileSaveas)
    ON_UPDATE_COMMAND_UI(IDM_FILE_SAVEAS, OnUpdateFileSaveas)
    ON_COMMAND(IDM_FILE_CLOSE, OnFileClose)
    ON_COMMAND(IDM_EDIT_COPY, OnEditCopy)
    ON_UPDATE_COMMAND_UI(IDM_EDIT_COPY, OnUpdateEditCopy)
    ON_COMMAND(IDM_EDIT_PASTE, OnEditPaste)
    ON_UPDATE_COMMAND_UI(IDM_EDIT_PASTE, OnUpdateEditPaste)
    ON_COMMAND(IDM_EDIT_CUT, OnEditCut)
    ON_UPDATE_COMMAND_UI(IDM_EDIT_CUT, OnUpdateEditCut)
    ON_COMMAND(IDM_DISPLAY_RESET, OnDisplayReset)
    ON_COMMAND(ID_DISPLAY_SETTINGS, OnDisplaySettings)
    ON_COMMAND(IDM_FILE_VALIDATE, OnFileValidate)
    ON_COMMAND(IDM_FILE_EXIT, OnFileExit)
    ON_WM_MDIACTIVATE()
    ON_COMMAND(IDM_HELP_TESTEDITORHELP, OnHelpTesteditorhelp)
    ON_WM_CLOSE()
    ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

/******************************************************************************
Function Name  :  OnCreateClient
Input(s)       :  LPCREATESTRUCT - Create Structure
                  CCreateContext* - Context Pointer
Output         :  BOOL - returns TRUE if success otherwise FALSE
Functionality  :  This Function will create the treeview and listview.
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
BOOL CTSEditorChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
    BOOL bReturn = m_omSplitterWnd.CreateStatic( this, def_NUM_ROWS_TSEDITOR, def_NUM_COLS_TSEDITOR );
    CSize omSize(def_WIDTH_PANE, def_HEIGHT_PANE);
    bReturn = bReturn && m_omSplitterWnd.CreateView(def_ROW_INDEX, def_INDEX_TREEVIEW, RUNTIME_CLASS(CTreeViewEx),omSize, pContext);
    bReturn = bReturn && m_omSplitterWnd.CreateView(def_ROW_INDEX, def_INDEX_PROPVIEW, RUNTIME_CLASS(CPropertyView),omSize, pContext);

    CHECKEQRET(bReturn, FALSE, FALSE);

    m_odTreeView =  (CTreeViewEx*)m_omSplitterWnd.GetPane(def_ROW_INDEX, def_INDEX_TREEVIEW);
    m_odPropertyView = (CPropertyView*)m_omSplitterWnd.GetPane(def_ROW_INDEX, def_INDEX_PROPVIEW);

    //Setting TreeView Item Images
    m_pomImageList = new CImageList();
    m_pomImageList->Create(def_HEIGHT_IMAGE, def_WIDTH_IMAGE, ILC_COLOR32|ILC_MASK, def_NUM_IMAGE, def_NUM_IMAGE);
    m_pomImageList->SetBkColor(def_COLOR_TREE_BKG);
    CBitmap omBitmap;

    for (int nID = IDI_ICONTESTCASE; nID <= IDI_ICONREPLAY; nID++)  // load bitmaps
    {
        HICON hIcon = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(nID));
        m_pomImageList->Add(hIcon);
    }

    m_odTreeView->GetTreeCtrl().SetImageList(m_pomImageList, TVSIL_NORMAL);
    m_bInit = FALSE;

    //SSH + issue #438
    SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_BUSMASTER), FALSE);
    //SSH -
    return TRUE;
}


/******************************************************************************
Function Name  :  vLoadTestSetupFile
Input(s)       :  omFilePath - File Path
                  bEmptyFile - Specifies weather the input file is empty or not
Output         :  void
Functionality  :  Loads the TestSetup file and update the CTestSetup datastructure
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vLoadTestSetupFile(CString omFilePath, BOOL bEmptyFile /*FALSE*/)
{
    vInitialise();
    if(m_odPropertyView != nullptr)
    {
        m_odPropertyView->m_omPropertyList.DeleteAllItems();
    }
    m_bFileSaved = FALSE;
    if(bEmptyFile == FALSE)
    {
        if( m_ouTSEntity.LoadFile(omFilePath) != S_OK)  //if S_OK Then Upadate The Tree
        {
            MessageBox(_("Invalid Test Setup File"), _("Error"), MB_OK|MB_ICONERROR);
            vInitialise();
            return;
        }
        m_bFileSaved = TRUE;
    }

    vSetCurrentFile(omFilePath);
    nUpdateTreeView();
}
void CTSEditorChildFrame::vLoadTestSetupFileTemp(CString omFilePath, BOOL bEmptyFile)
{
    vInitialise();
    if(m_odTreeView != nullptr )
    {
        m_odTreeView->GetTreeCtrl().DeleteAllItems();
    }
    if(m_odPropertyView != nullptr)
    {
        m_odPropertyView->m_omPropertyList.DeleteAllItems();
    }
    m_ouTSEntity.vInitialise();

    if(bEmptyFile == FALSE)
    {
        if( m_ouTSEntity.LoadFile(omFilePath) != S_OK)  //if S_OK Then Upadate The Tree
        {
            MessageBox(_("Invalid Test Setup File"), _("Error"), MB_OK|MB_ICONERROR);
            vInitialise();
            if(m_odTreeView != nullptr )
            {
                m_odTreeView->GetTreeCtrl().DeleteAllItems();
            }
            if(m_odPropertyView != nullptr)
            {
                m_odPropertyView->m_omPropertyList.DeleteAllItems();
            }
            m_ouTSEntity.vInitialise();
            CString omStrTemp = def_EMPTYFILENAME;
            vSetCurrentFile(omStrTemp);
            return;
        }
        m_bFileSaved = TRUE;
    }

    vSetCurrentFile(omFilePath);
    nUpdateTreeView();
}
/******************************************************************************
Function Name  :  nUpdateTreeView
Input(s)       :  -
Output         :  INT, Non zero on failure.
Functionality  :  Deletes the current tree item and re initialises it with the
                  current datastructure.
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
INT CTSEditorChildFrame::nUpdateTreeView()
{
    CHECKEQRET(m_odTreeView, nullptr, -1);
    UINT unCount;
    m_ouTSEntity.GetSubEntryCount(unCount);
    m_odTreeView->GetTreeCtrl().DeleteAllItems();
    m_hParentTreeItem = m_odTreeView->InsertTreeItem(nullptr, m_ouTSEntity.m_omstrTestSetupTitle, nullptr, 0, 0, m_ouTSEntity.GetID());
    if(unCount != 0)
    {
        for(UINT j=0; j<unCount; j++)
        {
            CBaseEntityTA* pTCEntity = nullptr;
            m_ouTSEntity.GetSubEntityObj(j, &pTCEntity);
            HTREEITEM hTCTreeitem = m_odTreeView->InsertTreeItem(m_hParentTreeItem, "", nullptr, 0, 0, pTCEntity->GetID());
            parseTestCaseEntiy(pTCEntity, hTCTreeitem);
        }
    }
    return 0;
}

/******************************************************************************
Function Name  :  parseTestCaseEntiy
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::parseTestCaseEntiy(CBaseEntityTA* pTCEntity, HTREEITEM hTCTreeitem)
{
    CHECKENTITY(pTCEntity);

    UINT unCount;
    CTestCaseData TestCaseData;
    CBaseEntityTA* pEntity = nullptr;
    CTreeCtrl& omTempTreeCtrl = m_odTreeView->GetTreeCtrl();

    pTCEntity->GetEntityData(TEST_CASE, &TestCaseData);
    pTCEntity->GetSubEntryCount(unCount);


    omTempTreeCtrl.SetItemText(hTCTreeitem, TestCaseData.m_omTitle);
    m_odTreeView->vDeleteChildItems(hTCTreeitem);
    int TempConut = unCount;
    for(int i =0; i<TempConut; i++)
    {
        pTCEntity->GetSubEntityObj(i, &pEntity);
        pEntity->GetSubEntryCount(unCount);
        eTYPE_ENTITY eEntityType = pEntity->GetEntityType();

        if(eEntityType == SEND)
        {
            HTREEITEM hSubParent = m_odTreeView->InsertTreeItem(hTCTreeitem, _(def_NAME_SEND), nullptr, def_INDEX_SEND_IMAGE, def_INDEX_SEND_IMAGE, pEntity->GetID());
            parseSendEntity(pEntity, hSubParent);
        }
        else if(eEntityType == VERIFY || eEntityType == VERIFYRESPONSE)
        {
            CString omStrTreeName = _(def_NAME_VERIFY);
            if(eEntityType == VERIFYRESPONSE)
            {
                omStrTreeName = _(def_NAME_VERIFYRESPONSE);
            }
            HTREEITEM hSubParent = m_odTreeView->InsertTreeItem(hTCTreeitem, omStrTreeName, nullptr, def_INDEX_VERIFY_IMAGE, def_INDEX_VERIFY_IMAGE, pEntity->GetID());
            parseVerifyEntity(pEntity, hSubParent);
        }
        else if(eEntityType == WAIT)
        {
            parseWaitEntity(pEntity, hTCTreeitem);
        }
        else if(eEntityType == REPLAY)
        {
            parseReplayEntity(pEntity, hTCTreeitem);
        }
    }
}
/******************************************************************************
Function Name  :  parseSendEntity
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::parseSendEntity(CBaseEntityTA* pEntity, HTREEITEM hSubParent)
{
    CHECKENTITY(pEntity);
    UINT unCount;
    CBaseEntityTA* pSubEntity;  //send_message entity
    CSend_MessageData odData;

    pEntity->GetSubEntryCount(unCount); //Send_Message Count
    m_odTreeView->vDeleteChildItems(hSubParent);

    for(UINT i=0; i<unCount; i++)
    {
        pEntity->GetSubEntityObj(i, &pSubEntity);
        pSubEntity->GetEntityData(SEND_MESSAGE, &odData);
        m_odTreeView->InsertTreeItem(hSubParent, odData.m_omMessageName, nullptr, def_INDEX_SEND_IMAGE, def_INDEX_SEND_IMAGE, pSubEntity->GetID());
    }
    m_odTreeView->RedrawWindow();
}

/******************************************************************************
Function Name  :  parseVerifyEntity
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::parseVerifyEntity(CBaseEntityTA* pEntity, HTREEITEM hSubParent)
{
    CHECKENTITY(pEntity);

    UINT unCount;
    CBaseEntityTA* pSubEntity;
    CVerify_MessageData odData;

    pEntity->GetSubEntryCount(unCount);
    m_odTreeView->vDeleteChildItems(hSubParent);

    for(UINT i=0; i<unCount; i++)
    {
        pEntity->GetSubEntityObj(i, &pSubEntity);
        pSubEntity->GetEntityData(VERIFY_MESSAGE, &odData);
        m_odTreeView->InsertTreeItem(hSubParent, odData.m_omMessageName, nullptr, def_INDEX_VERIFY_IMAGE, def_INDEX_VERIFY_IMAGE, pSubEntity->GetID());
    }
    m_odTreeView->RedrawWindow();
}
/******************************************************************************
Function Name  :  parseWaitEntity
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::parseWaitEntity(CBaseEntityTA* pEntity, HTREEITEM hTCTreeitem)
{
    CHECKENTITY(pEntity);
    CWaitEntityData odData;
    pEntity->GetEntityData(WAIT, &odData);
    m_odTreeView->InsertTreeItem(hTCTreeitem, _("Wait"), nullptr, def_INDEX_WAIT_IMAGE, def_INDEX_WAIT_IMAGE, pEntity->GetID());
}

/******************************************************************************
Function Name  :  parseReplayEntity
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::parseReplayEntity(CBaseEntityTA* pEntity, HTREEITEM hTCTreeitem)
{
    CHECKENTITY(pEntity);
    CString omstrTemp = _("Replay");
    m_odTreeView->InsertTreeItem(hTCTreeitem, omstrTemp, nullptr, def_INDEX_REPLAY_IMGAE, def_INDEX_REPLAY_IMGAE, pEntity->GetID());
}

/******************************************************************************
Function Name  :  OnDestroy
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnDestroy()
{
    //TODO::HANDLE Properly
    CMDIChildWnd::OnDestroy();
    g_pomTSEditorChildWindow = nullptr;
}

/******************************************************************************
Function Name  :  OnFileOpen
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnFileOpen()
{
    INT nRetVal = nPromptForSaveFile();
    CHECKEQ(nRetVal, IDCANCEL);
    CFileDialog omFileOpenDlg(TRUE, ".xml", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
    omFileOpenDlg.m_ofn.lpstrTitle = _("Select A TestSetup File");

    if(IDOK == omFileOpenDlg.DoModal())
    {
        vLoadTestSetupFileTemp(omFileOpenDlg.GetPathName());
    }
}

/******************************************************************************
Function Name  :  OnHelpAbouttestsetupeditor
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnHelpAbouttestsetupeditor()
{
    CString omStrVersion;
    omStrVersion.LoadString(IDS_TSEDITOR_ABOUT);
    CString omStrLicence =  omStrVersion + _("\n\nCopyright(c) 2011, Robert Bosch Engineering and\nBusiness Solutions Ltd.\nAll rights reserved.");
    MessageBox(omStrLicence, nullptr, MB_OK|MB_ICONINFORMATION);
}

/******************************************************************************
Function Name  :  OnSelectionChanged
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
LRESULT CTSEditorChildFrame::OnSelectionChanged(WPARAM pTreeView, LPARAM /*pOldTreeView*/)
{
    m_bModified = FALSE;
    LPNMTREEVIEW pNMTreeView = (LPNMTREEVIEW)pTreeView;
    m_dwCurrentEntityID = (DWORD)m_odTreeView->GetTreeCtrl().GetItemData(pNMTreeView->itemNew.hItem);
    m_hCurrentTreeItem = pNMTreeView->itemNew.hItem;
    return nDisplayEntity(m_dwCurrentEntityID);
}

/******************************************************************************
Function Name  :  OnSelectionChanging
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
LRESULT CTSEditorChildFrame::OnSelectionChanging(WPARAM /*pTreeView*/, LPARAM /*pOldTreeView*/)
{
    LRESULT lResult = FALSE;
    if(m_bModified == TRUE)
    {
        if(m_bQueryConfirm == FALSE)
        {
            nConfirmCurrentChanges();
            lResult = FALSE;
        }
        else
        {
            m_bQueryConfirm = FALSE;
            INT nSelection = MessageBox(_("Current List is Modified\nDo You Want to save the Changes?"), _("Modified"), MB_YESNOCANCEL|MB_ICONQUESTION);
            switch(nSelection)
            {
                case IDYES:
                    nConfirmCurrentChanges();
                    lResult = FALSE;
                    break;
                case IDNO:
                    lResult = FALSE;
                    break;
                case IDCANCEL:
                    lResult  = TRUE;
                    break;
            }
        }
    }
    return lResult;
}

/******************************************************************************
Function Name  :  nDisplayEntity
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
INT CTSEditorChildFrame::nDisplayEntity(DWORD dwEntityID)
{
    m_ouTSEntity.SearchEntityObject(dwEntityID, &m_pCurrentEntity);
    if(m_pCurrentEntity == nullptr)
    {
        return ERR_INVALID_ENTITY;
    }
    return nDisplayEntity(m_pCurrentEntity);
}

/******************************************************************************
Function Name  :  nDisplayEntity
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
INT CTSEditorChildFrame::nDisplayEntity(CBaseEntityTA* pEntity)
{
    //CBaseEntityTA *pEntity;
    CHECKEQRET(pEntity, nullptr, ERR_INVALID_ENTITY);
    eTYPE_ENTITY eType = m_pCurrentEntity->GetEntityType();
    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;
    omTempListCtrl.DeleteAllItems();
    switch(eType)
    {
        case TEST_SETUP:
            vDisplayHeaderInfo(0);
            break;
        case TEST_CASE:
            vDisplayTestcaseInfo(m_pCurrentEntity);
            break;
        case SEND:
            vDisplaySendInfo(m_pCurrentEntity);
            break;
        case VERIFY:
            vDisplayVerifyInfo(m_pCurrentEntity);
            break;
        case SEND_MESSAGE:
            vDisplaySendMessageInfo(m_pCurrentEntity);
            break;
        case VERIFY_MESSAGE:
            vDisplayVerifyMessageInfo(m_pCurrentEntity);
            break;
        case WAIT:
            vDisplayWaitInfo(m_pCurrentEntity);
            break;
        case REPLAY:
            vDisplayReplayInfo(m_pCurrentEntity);
            break;
        case VERIFYRESPONSE:
            vDisplayVerifyResponseInfo(m_pCurrentEntity);
            break;
        default:
            return ERR_INVALID_ENTITY;
    }
    vShowHelpInfo(eType);
    return S_OK;
}

/******************************************************************************
Function Name  :  vDisplayHeaderInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam, GT-Derka
Date Created   :  04/03/2011
Modifications  :
Codetag        :
******************************************************************************/
void CTSEditorChildFrame::vDisplayHeaderInfo(INT /*nTestSetupIndex*/)
{
    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;

    omTempListCtrl.DeleteAllItems();
    m_odPropertyView->vSetChannelColumn(HIDE);

    //Get Header Info.
    CTestSetupHeader ouHeaderInfo;
    m_ouTSEntity.GetHeaderData(ouHeaderInfo);
    CString omStrTemp;
    //Set List Controls Item Properties.
    SLISTINFO sListInfo;
    sListInfo.m_eType = eText;

    //Title  - ROW0
    omTempListCtrl.InsertItem(def_TS_ROWNUM_TITLE, _("Title"));
    omTempListCtrl.SetItemText(def_TS_ROWNUM_TITLE, def_COLUMN_VALUE, m_ouTSEntity.m_omstrTestSetupTitle);
    omTempListCtrl.vSetColumnInfo(def_TS_ROWNUM_TITLE, def_COLUMN_VALUE, sListInfo);

    //Description - ROW1
    omTempListCtrl.InsertItem(def_TS_ROWNUM_DESCRIPTION, _("Description"));
    omTempListCtrl.SetItemText(def_TS_ROWNUM_DESCRIPTION, def_COLUMN_VALUE, m_ouTSEntity.m_omstrDescription);
    omTempListCtrl.vSetColumnInfo(def_TS_ROWNUM_DESCRIPTION, def_COLUMN_VALUE, sListInfo);

    //Version - ROW2
    omTempListCtrl.InsertItem(def_TS_ROWNUM_VERSION, ouHeaderInfo.m_sVersion.m_omCategory);
    omTempListCtrl.SetItemText(def_TS_ROWNUM_VERSION, def_COLUMN_VALUE, ouHeaderInfo.m_sVersion.m_omValue);
    omTempListCtrl.vSetColumnInfo(def_TS_ROWNUM_VERSION, def_COLUMN_VALUE, sListInfo);

    //Bustype - ROW3
    sListInfo.m_eType = eComboItem;
    sListInfo.m_omEntries.Add("CAN");
    omTempListCtrl.InsertItem(def_TS_ROWNUM_BUSTYPE, _("Bus Type"));
    switch(ouHeaderInfo.m_eBus)
    {
        case CAN:
            omStrTemp = "CAN";
            break;
        default:
            omStrTemp = "CAN";
    }
    omTempListCtrl.SetItemText(def_TS_ROWNUM_BUSTYPE, def_COLUMN_VALUE, omStrTemp);
    omTempListCtrl.vSetColumnInfo(def_TS_ROWNUM_BUSTYPE, def_COLUMN_VALUE, sListInfo);

    //Module - ROW4
    sListInfo.m_eType = eText;
    omTempListCtrl.InsertItem(def_TS_ROWNUM_MODULE, ouHeaderInfo.m_sModuleName.m_omCategory);
    omTempListCtrl.SetItemText(def_TS_ROWNUM_MODULE, def_COLUMN_VALUE, ouHeaderInfo.m_sModuleName.m_omValue);
    omTempListCtrl.vSetColumnInfo(def_TS_ROWNUM_MODULE, def_COLUMN_VALUE, sListInfo);

    //Engineer Name - ROW5
    omTempListCtrl.InsertItem(def_TS_ROWNUM_ENGINEER1, ouHeaderInfo.m_sEngineerInfo1.m_omCategory);
    omTempListCtrl.SetItemText(def_TS_ROWNUM_ENGINEER1, def_COLUMN_VALUE, ouHeaderInfo.m_sEngineerInfo1.m_omValue);
    omTempListCtrl.vSetColumnInfo(def_TS_ROWNUM_ENGINEER1, def_COLUMN_VALUE, sListInfo);

    //Engineer Designation - ROW6
    omTempListCtrl.InsertItem(def_TS_ROWNUM_ENGINEER2, ouHeaderInfo.m_sEngineerInfo2.m_omCategory);
    omTempListCtrl.SetItemText(def_TS_ROWNUM_ENGINEER2, def_COLUMN_VALUE, ouHeaderInfo.m_sEngineerInfo2.m_omValue);
    omTempListCtrl.vSetColumnInfo(def_TS_ROWNUM_ENGINEER2, def_COLUMN_VALUE, sListInfo);

    //DataBase - ROW7
    sListInfo.m_eType = eBrowser;
    sListInfo.m_omEntries.RemoveAll();
    sListInfo.m_omEntries.Add("*.dbf");
    sListInfo.m_omEntries.Add(_("DataBase File (*.dbf)|*.dbf||"));
    omTempListCtrl.InsertItem(def_TS_ROWNUM_DATABASE, _("DataBase Path"));
    omTempListCtrl.SetItemText(def_TS_ROWNUM_DATABASE, def_COLUMN_VALUE, ouHeaderInfo.m_omDatabasePath);
    omTempListCtrl.vSetColumnInfo(def_TS_ROWNUM_DATABASE, def_COLUMN_VALUE, sListInfo);



    //Report File Path - ROW8
    CString omStrDefExt;
    CString omStrFilter;

    omStrDefExt = "*.txt";
    omStrFilter = _("Text Files (*.txt)|*.txt|Html Files (*html)|*.html||");

    sListInfo.m_eType = eBrowser;
    sListInfo.m_omEntries.RemoveAll();
    sListInfo.m_omEntries.Add(omStrDefExt);
    sListInfo.m_omEntries.Add(omStrFilter);
    omTempListCtrl.InsertItem(def_TS_ROWNUM_REPORT, _("Report File Path"));


    omTempListCtrl.SetItemText(def_TS_ROWNUM_REPORT, def_COLUMN_VALUE, ouHeaderInfo.m_sReportFile.m_omPath);
    omTempListCtrl.vSetColumnInfo(def_TS_ROWNUM_REPORT, def_COLUMN_VALUE, sListInfo);


    //Time Mode - ROW9
    sListInfo.m_eType = eComboItem;
    sListInfo.m_omEntries.RemoveAll();
    sListInfo.m_omEntries.Add(_("ABSOLUTE"));
    sListInfo.m_omEntries.Add(_("RELATIVE"));
    omTempListCtrl.InsertItem(def_TS_ROWNUM_TIMEMODE, _("Time Mode"));
    switch(ouHeaderInfo.m_sReportFile.m_eTimeMode)
    {
        case REL:
            omStrTemp = _("RELATIVE");
            break;
        case ABS:
        default:
            omStrTemp = _("ABSOLUTE");
            break;
    }
    omTempListCtrl.SetItemText(def_TS_ROWNUM_TIMEMODE, def_COLUMN_VALUE, omStrTemp);
    omTempListCtrl.vSetColumnInfo(def_TS_ROWNUM_TIMEMODE, def_COLUMN_VALUE, sListInfo);

    //FileFormat - ROW10
    /*sListInfo.m_eType = eComboItem;
    switch(ouHeaderInfo.m_sReportFile.m_eType)
    {
        case HTM:
            omStrTemp = "HTML";
            break;
        case TXT:
        default:
            omStrTemp = "TXT";
            break;
    }
    sListInfo.m_omEntries.RemoveAll();
    sListInfo.m_omEntries.Add("TXT");
    sListInfo.m_omEntries.Add("HTML");
    omTempListCtrl.InsertItem(def_TS_ROWNUM_FILEFORMAT, "File Format");

    omTempListCtrl.SetItemText(def_TS_ROWNUM_FILEFORMAT, def_COLUMN_VALUE, omStrTemp);
    omTempListCtrl.vSetColumnInfo(def_TS_ROWNUM_FILEFORMAT, def_COLUMN_VALUE, sListInfo);*/

}

/******************************************************************************
Function Name  :  vDisplayTestcaseInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam, GT-Derka
Date Created   :  04/03/2011
Modifications  :
Codetag        :
******************************************************************************/
void CTSEditorChildFrame::vDisplayTestcaseInfo(CBaseEntityTA* pTCEntity)
{
    CHECKENTITY(pTCEntity);

    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;
    omTempListCtrl.DeleteAllItems();

    CTestCaseData odData;
    pTCEntity->GetEntityData(TEST_CASE, &odData);

    SLISTINFO sListInfo;
    sListInfo.m_eType = eText;
    CString omstrTemp;

    m_odPropertyView->vSetChannelColumn(HIDE);  // Hide Channel-Column

    //Display Title on first row
    omTempListCtrl.InsertItem(def_TC_ROWNUM_TCID, _("Test Case ID"));
    omTempListCtrl.SetItemText(def_TC_ROWNUM_TCID, def_COLUMN_VALUE, odData.m_omID);
    omTempListCtrl.vSetColumnInfo(def_TC_ROWNUM_TCID, def_COLUMN_VALUE, sListInfo);

    //Display TestCase Name
    omTempListCtrl.InsertItem(def_TC_ROWNUM_TCNAME, _("Test Case Name"));
    omTempListCtrl.SetItemText(def_TC_ROWNUM_TCNAME, def_COLUMN_VALUE, odData.m_omTitle);
    omTempListCtrl.vSetColumnInfo(def_TC_ROWNUM_TCNAME, def_COLUMN_VALUE, sListInfo);

    //Display Exception Handler;
    sListInfo.m_eType = eComboItem;
    sListInfo.m_omEntries.Add(_("CONTINUE"));
    sListInfo.m_omEntries.Add(_("EXIT"));

    omTempListCtrl.InsertItem(def_TC_ROWNUM_TCEXP, _("Exception Handler"));
    switch(odData.m_eExcpAction)
    {
        case EXIT:
            omstrTemp = _("EXIT");
            break;
        case CONTINUE:
        default:
            omstrTemp = _("CONTINUE");
    }
    omTempListCtrl.SetItemText(def_TC_ROWNUM_TCEXP, def_COLUMN_VALUE, omstrTemp);
    omTempListCtrl.vSetColumnInfo( def_TC_ROWNUM_TCEXP, def_COLUMN_VALUE, sListInfo);
}

/******************************************************************************
Function Name  :  vDisplaySendInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam, GT-Derka
Date Created   :  04/03/2011
Modifications  :
Codetag        :
******************************************************************************/
void CTSEditorChildFrame::vDisplaySendInfo(CBaseEntityTA* pEntity)
{
    CHECKENTITY(pEntity);

    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;

    m_omSendEntity = *((CSendEntity*)pEntity);

    omTempListCtrl.DeleteAllItems();
    m_odPropertyView->vSetChannelColumn(SHOW);  // Show Channel-Column
    UINT unCount;
    CBaseEntityTA* pSubEntity;  //send_message entity
    CSend_MessageData odData;
    pEntity->GetSubEntryCount(unCount); //Send_Message Count
    CString omstrTemp;
    omstrTemp.Format("%d", unCount);

    SLISTINFO sListInfo;
    sListInfo.m_eType = eNoControl;
    omTempListCtrl.InsertItem(def_SEND_ROWNUM_MSGCNT, _("Message Count"));
    omTempListCtrl.SetItemText(def_SEND_ROWNUM_MSGCNT, def_COLUMN_VALUE, omstrTemp);
    omTempListCtrl.vSetColumnInfo(def_SEND_ROWNUM_MSGCNT, def_COLUMN_VALUE, sListInfo);

    sListInfo.m_eType = eComboItem;
    m_ouTSEntity.m_ouDataBaseManager.nFillMessageList(sListInfo.m_omEntries, TRUE);
    UINT i = 0;

    for( i=0; i<unCount; i++)   // Show all messages belonging to the test case
    {
        pEntity->GetSubEntityObj(i, &pSubEntity);
        pSubEntity->GetEntityData(SEND_MESSAGE, &odData);
        omstrTemp.Format("%d", odData.m_dwMessageID);
        omTempListCtrl.InsertItem(i+1, omstrTemp);
        omTempListCtrl.SetItemText(i+1, def_COLUMN_VALUE, odData.m_omMessageName);
        omTempListCtrl.vSetColumnInfo(i+1, 1, sListInfo);

        vCreateChannelDropdown(omTempListCtrl, odData.m_byChannelNumber, m_iNumberOfActiveChannels, i); // Create channel-drop-down

    }
    omTempListCtrl.InsertItem(i+1, "");
    omTempListCtrl.SetItemText(i+1, def_COLUMN_VALUE, _("[Add Message]")); //Extra Line
    omTempListCtrl.vSetColumnInfo(i+1, def_COLUMN_VALUE, sListInfo);

    sListInfo.m_eType = eNoControl;
    omTempListCtrl.vSetColumnInfo(i+1, def_COLUMN_CHANNEL, sListInfo);  // solves issue #711, 2nd bullet point
}

/******************************************************************************
Function Name  :  vDisplayVerifyInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam, GT-Derka
Date Created   :  04/03/2011
Modifications  :
Codetag        :
******************************************************************************/
void CTSEditorChildFrame::vDisplayVerifyInfo(CBaseEntityTA* pEntity, int nVerifyRowIndex)
{
    CHECKENTITY(pEntity);

    CString omstrTemp;
    UINT unCount;
    m_ouVerifyEntity = *((CVerifyEntity*)pEntity);
    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;
    CBaseEntityTA* pSubEntity;  //Verify_message entity
    m_odPropertyView->vSetChannelColumn(SHOW);      // Show Channel-Column

    pEntity->GetSubEntryCount(unCount); //Send_Message Count
    omstrTemp.Format("%d", unCount);

    SLISTINFO sListInfo;

    sListInfo.m_eType = eNoControl;
    omTempListCtrl.InsertItem(def_VERIFY_ROWNUM_MSGCNT, _("Message Count"));
    omTempListCtrl.SetItemText(def_VERIFY_ROWNUM_MSGCNT, def_COLUMN_VALUE, omstrTemp);
    omTempListCtrl.vSetColumnInfo(def_VERIFY_ROWNUM_MSGCNT, def_COLUMN_VALUE, sListInfo);

    sListInfo.m_eType = eComboItem;
    CVerifyResponseData odVerifyData;
    pEntity->GetEntityData(pEntity->GetEntityType(), &odVerifyData);
    switch(odVerifyData.m_VerifyData.m_eAttributeError)
    {
        case WARNING:
            omstrTemp = _("WARNING");
            break;
        case ERRORS:
            omstrTemp = _("ERROR");
            break;
        case FATAL:
        default:
            omstrTemp = _("FATAL");
            break;
    }
    omTempListCtrl.InsertItem(def_VERIFY_ROWNUM_FAILURE, _("Failure Classification"));
    omTempListCtrl.SetItemText(def_VERIFY_ROWNUM_FAILURE, def_COLUMN_VALUE, omstrTemp);
    sListInfo.m_omEntries.Add(_("WARNING"));
    sListInfo.m_omEntries.Add(_("ERROR"));
    sListInfo.m_omEntries.Add(_("FATAL"));
    omTempListCtrl.vSetColumnInfo(def_VERIFY_ROWNUM_FAILURE, def_COLUMN_VALUE, sListInfo);

    sListInfo.m_eType = eNoControl;
    omTempListCtrl.vSetColumnInfo(def_VERIFY_ROWNUM_FAILURE, def_COLUMN_CHANNEL, sListInfo);  // solves issue #711, 2nd bullet point

    sListInfo.m_eType = eComboItem;
    sListInfo.m_omEntries.RemoveAll();
    m_ouTSEntity.m_ouDataBaseManager.nFillMessageList(sListInfo.m_omEntries, TRUE);

    if( nVerifyRowIndex == -1 )
    {
        nVerifyRowIndex = def_VERIFY_ROWNUM_MSGLIST;
    }
    UINT i;
    for( i=0; i<unCount; i++)
    {
        CVerify_MessageData odData;

        pEntity->GetSubEntityObj(i, &pSubEntity);
        pSubEntity->GetEntityData(VERIFY_MESSAGE, &odData);

        omstrTemp.Format("%d", odData.m_dwMessageID);
        omTempListCtrl.InsertItem(i+nVerifyRowIndex, omstrTemp);

        omTempListCtrl.SetItemText(i+nVerifyRowIndex, def_COLUMN_VALUE, odData.m_omMessageName);
        omTempListCtrl.vSetColumnInfo(i+nVerifyRowIndex, def_COLUMN_VALUE, sListInfo);

        vCreateChannelDropdown(omTempListCtrl, odData.m_byChannelNumber, m_iNumberOfActiveChannels, i+1);   // Create channel-drop-down
    }
    omTempListCtrl.InsertItem(i+nVerifyRowIndex, "");
    omTempListCtrl.SetItemText(i+nVerifyRowIndex, def_COLUMN_VALUE, _("[Add Message]"));
    omTempListCtrl.vSetColumnInfo(i+nVerifyRowIndex, def_COLUMN_VALUE, sListInfo);

    sListInfo.m_eType = eNoControl;
    omTempListCtrl.vSetColumnInfo(i+nVerifyRowIndex, def_COLUMN_CHANNEL, sListInfo);  // solves issue #711, 2nd bullet point
}
/******************************************************************************
Function Name  :  vDisplayVerifyResponseInfo
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CTSEditorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  21/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
void CTSEditorChildFrame::vDisplayVerifyResponseInfo(CBaseEntityTA* pEntity)
{
    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;

    SLISTINFO sListInfo;
    sListInfo.m_eType = eText;
    omTempListCtrl.InsertItem(def_ROWNNUM_WAITFOR, _("Wait Until"));


    //Wait Period
    CString omStrWait;
    CVerifyResponseData ouResponseData;
    pEntity->GetEntityData(VERIFYRESPONSE, &ouResponseData);
    omStrWait.Format("%d", ouResponseData.m_ushDuration);
    vDisplayVerifyInfo(pEntity, 3);
    omTempListCtrl.SetItemText(def_ROWNNUM_WAITFOR, def_COLUMN_VALUE, omStrWait);
    omTempListCtrl.vSetColumnInfo(def_ROWNNUM_WAITFOR, def_COLUMN_VALUE, sListInfo);
}
/******************************************************************************
Function Name  :  vDisplayWaitInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vDisplayWaitInfo(CBaseEntityTA* pEntity)
{
    CHECKENTITY(pEntity);

    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;
    omTempListCtrl.DeleteAllItems();
    m_odPropertyView->vSetChannelColumn(HIDE);  // Hide Channel-Column

    CString omstrTemp;

    CWaitEntityData odData;// = new CSend_MessageData();
    pEntity->GetEntityData(WAIT, &odData);
    SLISTINFO sListInfo;
    sNumericInfo sNumInfo;

    sListInfo.m_eType = eText;
    omTempListCtrl.InsertItem(def_WAIT_ROWNUM_PURPOSE, _("Purpose"));
    omTempListCtrl.SetItemText(def_WAIT_ROWNUM_PURPOSE, def_COLUMN_VALUE, odData.m_omPurpose);
    omTempListCtrl.vSetColumnInfo(def_WAIT_ROWNUM_PURPOSE, def_COLUMN_VALUE, sListInfo);

    sListInfo.m_eType = eNumber;
    omTempListCtrl.InsertItem(def_WAIT_ROWNUM_DELAY, _("Delay (in msec)"));
    omstrTemp.Format("%d", odData.m_ushDuration);
    omTempListCtrl.SetItemText(def_WAIT_ROWNUM_DELAY, def_COLUMN_VALUE, omstrTemp);
    omTempListCtrl.vSetNumericInfo(def_WAIT_ROWNUM_DELAY, def_COLUMN_VALUE, sNumInfo);
    omTempListCtrl.vSetColumnInfo(def_WAIT_ROWNUM_DELAY, def_COLUMN_VALUE, sListInfo);
}

/******************************************************************************
Function Name  :  vDisplayReplayInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vDisplayReplayInfo(CBaseEntityTA* pEntity)
{
    CHECKENTITY(pEntity);
    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;
    omTempListCtrl.DeleteAllItems();

    CString omstrTemp;
    pEntity->GetEntityData(REPLAY, &omstrTemp);

    SLISTINFO sListInfo;
    sListInfo.m_eType = eBrowser;
    omTempListCtrl.InsertItem(def_REPLAY_ROWNUM_FILEPATH, _("File Path"));
    omTempListCtrl.SetItemText(def_REPLAY_ROWNUM_FILEPATH, def_COLUMN_VALUE, omstrTemp);
    omTempListCtrl.vSetColumnInfo(def_REPLAY_ROWNUM_FILEPATH, def_COLUMN_VALUE, sListInfo);
}

/******************************************************************************
Function Name  :  vCreateChannelDropdown
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  GT-Derka
Date Created   :  06/05/2014
Modifications  :
Codetag        :
******************************************************************************/
void CTSEditorChildFrame::vCreateChannelDropdown(CListCtrlEx& omTempListCtrl, BYTE byChannelnumber, INT iNumberOfActiveChannels, INT iTestCaseNumber)
{
    CString omstrTemp;
    SLISTINFO sListInfoChannel;
    omstrTemp.Format("%d", byChannelnumber);    // Get current channelnumber
    omTempListCtrl.SetItemText(def_SMSG_ROWNUM_SDEFVALUE+iTestCaseNumber, def_COLUMN_CHANNEL, omstrTemp);   // Show current channelnumber
    sListInfoChannel.m_eType = eComboItem;  // Configure as DropDown

    for (INT i = 1; i <= iNumberOfActiveChannels; i++ ) // Insert all available channels
    {
        omstrTemp.Format(defFORMAT_MSGID_DECIMAL, i);
        sListInfoChannel.m_omEntries.Add(omstrTemp);
    }
    omTempListCtrl.vSetColumnInfo(def_SMSG_ROWNUM_SDEFVALUE+iTestCaseNumber, def_COLUMN_CHANNEL, sListInfoChannel);
}

/******************************************************************************
Function Name  :  vDisplaySendMessageInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam, GT-Derka
Date Created   :  04/03/2011
Modifications  :
Codetag        :
******************************************************************************/
void CTSEditorChildFrame::vDisplaySendMessageInfo(CBaseEntityTA* pBaseEntity)
{
    //Must Display Database Signals with the signals from the test setup file.
    CHECKENTITY(pBaseEntity);

    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;
    omTempListCtrl.DeleteAllItems();
    m_odPropertyView->vSetChannelColumn(HIDE);  // Hide Channel-Column

    CString omstrTemp;
    INT nSignalCount;
    CSend_MessageData pSubEntity;
    CSend_MessageEntity* pEntity = (CSend_MessageEntity*)pBaseEntity;  //send_message entity
    pEntity->GetEntityData(SEND_MESSAGE, &pSubEntity);


    SLISTINFO sListInfo;
    sNumericInfo sNumInfo;


    if(pSubEntity.m_eSignalUnitType == RAW)
    {
        omstrTemp = _("RAW");
    }
    else if(pSubEntity.m_eSignalUnitType == ENG)
    {
        omstrTemp = _("ENG");
    }

    //Failure Classification Information
    omTempListCtrl.InsertItem(def_SMSG_ROWNUM_SUINT, _("Signal Unit Type"));
    omTempListCtrl.SetItemText(def_SMSG_ROWNUM_SUINT, def_COLUMN_VALUE, omstrTemp);
    sListInfo.m_eType = eComboItem;
    sListInfo.m_omEntries.Add(_("RAW"));
    sListInfo.m_omEntries.Add(_("ENG"));
    omTempListCtrl.vSetColumnInfo(def_SMSG_ROWNUM_SUINT, def_COLUMN_VALUE, sListInfo);

    //Signal Default Value - Hidden Not Required
    /*switch(pSubEntity.m_eSignalUnitType)
    {
        case ENG:
            omstrTemp.Format("%f", pSubEntity.m_uDefaultSignalValue.m_fValue);
            break;
        case RAW:
        default:
            omstrTemp.Format("%d", pSubEntity.m_uDefaultSignalValue.m_u64Value);
            break;
    }
    omTempListCtrl.InsertItem(def_SMSG_ROWNUM_SDEFVALUE, "Signal Default Value");
    omTempListCtrl.SetItemText(def_SMSG_ROWNUM_SDEFVALUE, def_COLUMN_VALUE, omstrTemp);

    omTempListCtrl.vSetNumericInfo(def_SMSG_ROWNUM_SDEFVALUE, def_COLUMN_VALUE, sNumInfo);
    sListInfo.m_eType = eText;
    omTempListCtrl.vSetColumnInfo(def_SMSG_ROWNUM_SDEFVALUE, def_COLUMN_VALUE, sListInfo);*/


    //Signal Display
    //W4 Removal
    nSignalCount = (INT)pSubEntity.m_odSignalDataList.GetCount();
    sListInfo.m_eType = eText;

    for(INT i = 0; i < nSignalCount; i++)
    {
        POSITION  pos = pSubEntity.m_odSignalDataList.FindIndex(i);
        CSignalData& odSignalData = pSubEntity.m_odSignalDataList.GetAt(pos);
        omTempListCtrl.InsertItem(i+def_SMAG_ROWNUM_SVALUE, odSignalData.m_omSigName);
        switch(pSubEntity.m_eSignalUnitType)
        {
            case ENG:
                omstrTemp.Format("%f", odSignalData.m_uValue.m_fValue);
                break;
            case RAW:
            default:
                omstrTemp.Format("%d", odSignalData.m_uValue.m_u64Value);
                break;
        }
        omTempListCtrl.SetItemText(i+def_SMAG_ROWNUM_SVALUE, def_COLUMN_VALUE, omstrTemp);
        omTempListCtrl.vSetColumnInfo(i+def_SMAG_ROWNUM_SVALUE, def_COLUMN_VALUE, sListInfo);
    }
    vDisplaySignalInfo(pSubEntity.m_omMessageName);
}

/******************************************************************************
Function Name  :  vDisplayVerifyMessageInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam, GT-Derka
Date Created   :  04/03/2011
Modifications  :
Codetag        :
******************************************************************************/
void CTSEditorChildFrame::vDisplayVerifyMessageInfo(CBaseEntityTA* pBaseEntity)
{
    CHECKENTITY(pBaseEntity);

    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;
    omTempListCtrl.DeleteAllItems();
    m_odPropertyView->vSetChannelColumn(HIDE);  // Hide Channel-Column

    CString omstrTemp;
    INT nSignalCount;
    CVerify_MessageData pSubEntity;
    CVerify_MessageEntity* pEntity = (CVerify_MessageEntity*)pBaseEntity;  //send_message entity
    pEntity->GetEntityData(VERIFY_MESSAGE, &pSubEntity);


    SLISTINFO sListInfo;
    sNumericInfo sNumInfo;


    if(pSubEntity.m_eSignalUnitType == RAW)
    {
        omstrTemp = _("RAW");
    }
    else if(pSubEntity.m_eSignalUnitType == ENG)
    {
        omstrTemp = _("ENG");
    }

    //Failure ClassiFication Information
    omTempListCtrl.InsertItem(def_VMSG_ROWNUM_SUINT, _("Signal Unit Type"));
    omTempListCtrl.SetItemText(def_VMSG_ROWNUM_SUINT, def_COLUMN_VALUE, omstrTemp);
    sListInfo.m_eType = eComboItem;
    sListInfo.m_omEntries.Add(_("RAW"));
    sListInfo.m_omEntries.Add(_("ENG"));
    omTempListCtrl.vSetColumnInfo(def_VMSG_ROWNUM_SUINT, def_COLUMN_VALUE, sListInfo);

    nSignalCount = (INT)pSubEntity.m_odSignalConditionList.GetCount();
    sListInfo.m_eType = eText;

    for(INT i = 0; i < nSignalCount; i++)
    {
        POSITION  pos = pSubEntity.m_odSignalConditionList.FindIndex(i);
        CSignalCondition& odSignalData = pSubEntity.m_odSignalConditionList.GetAt(pos);
        omTempListCtrl.InsertItem(i+1, odSignalData.m_omSigName);
        omTempListCtrl.SetItemText(i+1, def_COLUMN_VALUE, odSignalData.m_omCondition);
        omTempListCtrl.vSetColumnInfo(i+1, def_COLUMN_VALUE, sListInfo);
    }
    vDisplaySignalInfo(pSubEntity.m_omMessageName);
}

/******************************************************************************
Function Name  :  nCancelCurrentChanges
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
INT CTSEditorChildFrame::nCancelCurrentChanges()
{
    CHECKEQRET(m_pCurrentEntity, nullptr, ERR_INVALID_ENTITY);
    return nDisplayEntity(m_pCurrentEntity);
}

/******************************************************************************
Function Name  :  nConfirmCurrentChanges
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
INT CTSEditorChildFrame::nConfirmCurrentChanges()
{
    CHECKEQRET(m_pCurrentEntity, nullptr, ERR_INVALID_ENTITY);
    switch(m_pCurrentEntity->GetEntityType())
    {
        case TEST_SETUP:
            vSaveHeaderInfo(0);
            break;
        case TEST_CASE:
            vSaveTestcaseInfo(m_pCurrentEntity);
            break;
        case SEND:
            vSaveSendInfo(m_pCurrentEntity);
            break;
        case VERIFY:
            vSaveVerifyInfo(m_pCurrentEntity);
            break;
        case SEND_MESSAGE:
            vSaveSendMessageInfo(m_pCurrentEntity);
            break;
        case VERIFY_MESSAGE:
            vSaveVerifyMessageInfo(m_pCurrentEntity);
            break;
        case WAIT:
            vSaveWaitInfo(m_pCurrentEntity);
            break;
        case REPLAY:
            vSaveReplayInfo(m_pCurrentEntity);
            break;
        case VERIFYRESPONSE:
            vSaveVerfiyReponseInfo(m_pCurrentEntity);
            break;
        default:
            return 0;
    }

    vSetModifiedFlag(TRUE);
    return 0;
}

/******************************************************************************
Function Name  :  vSaveHeaderInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vSaveHeaderInfo(INT /*nTestSetupIndex*/)
{
    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;

    m_ouTSEntity.m_omstrTestSetupTitle = omTempListCtrl.GetItemText(def_TS_ROWNUM_TITLE, 1);
    m_odTreeView->GetTreeCtrl().SetItemText(m_hCurrentTreeItem, m_ouTSEntity.m_omstrTestSetupTitle);

    m_ouTSEntity.m_omstrDescription = omTempListCtrl.GetItemText(def_TS_ROWNUM_DESCRIPTION, 1);

    //Get Header Info.
    /*
    #define def_TS_ROWNUM_FILEFORMAT        10*/
    CTestSetupHeader ouHeaderInfo;
    m_ouTSEntity.GetHeaderData(ouHeaderInfo);

    ouHeaderInfo.m_sModuleName.m_omValue = omTempListCtrl.GetItemText(def_TS_ROWNUM_MODULE, 1);
    ouHeaderInfo.m_sVersion.m_omValue = omTempListCtrl.GetItemText(def_TS_ROWNUM_VERSION, 1);
    ouHeaderInfo.m_sEngineerInfo1.m_omValue = omTempListCtrl.GetItemText(def_TS_ROWNUM_ENGINEER1, 1);
    ouHeaderInfo.m_sEngineerInfo2.m_omValue = omTempListCtrl.GetItemText(def_TS_ROWNUM_ENGINEER2, 1);
    ouHeaderInfo.m_sReportFile.m_omPath = omTempListCtrl.GetItemText(def_TS_ROWNUM_REPORT, 1);

    ouHeaderInfo.m_omDatabasePath = omTempListCtrl.GetItemText(def_TS_ROWNUM_DATABASE, 1);

    //Bus Type
    CString omStrTemp;
    omStrTemp = omTempListCtrl.GetItemText(def_TS_ROWNUM_BUSTYPE, 1);
    if(omStrTemp == "CAN")
    {
        ouHeaderInfo.m_eBus = CAN;
    }
    else    //Currently Only CAN
    {
        ouHeaderInfo.m_eBus = CAN;
    }

    //Time Mode
    omStrTemp = omTempListCtrl.GetItemText(def_TS_ROWNUM_TIMEMODE, 1);
    if(omStrTemp == _("RELATIVE"))
    {
        ouHeaderInfo.m_sReportFile.m_eTimeMode = REL;
    }
    else
    {
        ouHeaderInfo.m_sReportFile.m_eTimeMode = ABS;
    }

    //File Format
    char* pchExten = PathFindExtension(ouHeaderInfo.m_sReportFile.m_omPath);

    //omStrTemp = omTempListCtrl.GetItemText(def_TS_ROWNUM_FILEFORMAT, 1);
    if((strcmp(pchExten, ".HTML") == 0)||(strcmp(pchExten, _(".html")) == 0))
    {
        ouHeaderInfo.m_sReportFile.m_eType = HTM;
    }
    else
    {
        ouHeaderInfo.m_sReportFile.m_eType = TXT;
    }
    m_ouTSEntity.SetHeaderData(ouHeaderInfo);
}

/******************************************************************************
Function Name  :  vSaveTestcaseInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vSaveTestcaseInfo(CBaseEntityTA* pEntity)
{
    CHECKENTITY(pEntity);

    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;

    CTestCaseEntity* pTCEntity = (CTestCaseEntity*)pEntity;

    CTestCaseData odData;

    CString omstrTemp;

    //Title on first row
    odData.m_omID = omTempListCtrl.GetItemText(def_TC_ROWNUM_TCID, def_COLUMN_VALUE);

    //TestCase Name in Second row
    odData.m_omTitle = omTempListCtrl.GetItemText(def_TC_ROWNUM_TCNAME, def_COLUMN_VALUE);

    //Exception Handler in Third Row
    omstrTemp= omTempListCtrl.GetItemText(def_TC_ROWNUM_TCEXP, def_COLUMN_VALUE);
    if(omstrTemp == _("EXIT"))
    {
        odData.m_eExcpAction = EXIT;
    }
    else        //Take anything as CONTINUE;
    {
        odData.m_eExcpAction = CONTINUE;
    }
    pTCEntity->SetTestCaseDetails(odData.m_omTitle, odData.m_omID, odData.m_eExcpAction);

    //The Selection will be exactly on our Testcase item
    HTREEITEM hItem = m_hCurrentTreeItem;
    m_odTreeView->GetTreeCtrl().SetItemText(hItem, odData.m_omTitle);

}

/******************************************************************************
Function Name  :  vSaveSendInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam, GT-Derka
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vSaveSendInfo(CBaseEntityTA* pEntity)
{
    CHECKENTITY(pEntity);

    UINT unCount;
    CSend_MessageData odData;
    pEntity->GetSubEntryCount(unCount);
    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;
    CString omstrTemp = omTempListCtrl.GetItemText(def_SEND_ROWNUM_MSGCNT, 1);
    unCount = atoi(omstrTemp);
    CSendData odSendData;
    m_omSendEntity.GetEntityData(SEND, &odSendData);

    for(UINT i = 0; i < unCount; i++)   // Store Channel-Data
    {
        omstrTemp = omTempListCtrl.GetItemText(def_SEND_ROWNUM_MSGLIST+i, def_COLUMN_CHANNEL);
        BYTE byChannel = BYTE(atoi(omstrTemp));
        odSendData.SetChannel(i, byChannel);
    }

    pEntity->SetEntityData(SEND, &odSendData);
    parseSendEntity(pEntity, m_hCurrentTreeItem);
}

/******************************************************************************
Function Name  :  vSaveVerifyInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam, GT-Derka
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vSaveVerifyInfo(CBaseEntityTA* pEntity)
{
    CHECKENTITY(pEntity);

    UINT unCount;
    CVerifyData odVerifyData;

    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;

    CString omstrTemp = omTempListCtrl.GetItemText(def_VERIFY_ROWNUM_MSGCNT, 1);    // Get number of messages
    unCount = atoi(omstrTemp);

    omstrTemp = omTempListCtrl.GetItemText(def_VERIFY_ROWNUM_FAILURE, 1);


    m_ouVerifyEntity.GetEntityData(VERIFY, &odVerifyData);


    if(omstrTemp == _("WARNING"))
    {
        odVerifyData.m_eAttributeError = WARNING;
    }
    else if(omstrTemp == _("ERROR"))
    {
        odVerifyData.m_eAttributeError = ERRORS;
    }
    else
    {
        odVerifyData.m_eAttributeError = FATAL;
    }

    for(UINT i = 0; i < unCount; i++)   // Save all Channel-numbers
    {
        omstrTemp = omTempListCtrl.GetItemText(def_VERIFY_ROWNUM_MSGLIST+i, def_COLUMN_CHANNEL);
        BYTE byChannel = BYTE(atoi(omstrTemp));
        odVerifyData.SetChannel(i, byChannel);
    }

    pEntity->SetEntityData(VERIFY, &odVerifyData);

    parseVerifyEntity(pEntity, m_hCurrentTreeItem);
}

void CTSEditorChildFrame::vSaveVerfiyReponseInfo(CBaseEntityTA* pEntity)
{
    CVerifyResponseData ouVerifyResponseData;

    //Message List
    m_ouVerifyEntity.GetEntityData(VERIFY, &ouVerifyResponseData.m_VerifyData);
    CListCtrl& omTempListCtrl = m_odPropertyView->m_omPropertyList;

    //Wait Period
    CString omStrWiatFor = omTempListCtrl.GetItemText(def_ROWNNUM_WAITFOR, def_COLUMN_VALUE);
    ouVerifyResponseData.m_ushDuration = (USHORT)atoi(omStrWiatFor);

    CString omstrTemp = omTempListCtrl.GetItemText(def_VERIFY_ROWNUM_FAILURE, def_COLUMN_VALUE);


    if(omstrTemp == _("WARNING"))
    {
        ouVerifyResponseData.m_VerifyData.m_eAttributeError = WARNING;
    }
    else if(omstrTemp == _("ERROR"))
    {
        ouVerifyResponseData.m_VerifyData.m_eAttributeError = ERRORS;
    }
    else
    {
        ouVerifyResponseData.m_VerifyData.m_eAttributeError = FATAL;
    }

    //Saving
    pEntity->SetEntityData(VERIFYRESPONSE, &ouVerifyResponseData);
    parseVerifyEntity(pEntity, m_hCurrentTreeItem);
}
/******************************************************************************
Function Name  :  vSaveSendMessageInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam, GT-Derka
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vSaveSendMessageInfo(CBaseEntityTA* pEntity)
{
    CHECKENTITY(pEntity);

    CListCtrl& omTempListCtrl = m_odPropertyView->m_omPropertyList;

    CString omstrTemp;
    UINT unSignalCount;
    CSend_MessageData ouSendMsgEntity;
    ((CSend_MessageEntity*)pEntity)->GetEntityData(SEND_MESSAGE, &ouSendMsgEntity);

    omstrTemp = omTempListCtrl.GetItemText(def_SMSG_ROWNUM_SUINT, def_COLUMN_VALUE);

    if(omstrTemp == _("ENG"))
    {
        ouSendMsgEntity.m_eSignalUnitType = ENG;
    }
    else                          //Take Anything as RAW
    {
        ouSendMsgEntity.m_eSignalUnitType = RAW;
    }

    //ouSendMsgEntity.m_byChannelNumber = BYTE(atoi(omTempListCtrl.GetItemText(def_SMSG_ROWNUM_SUINT, def_COLUMN_CHANNEL)));  // store Channel-Number

    /*//Default Signal Vlaue
    omstrTemp = omTempListCtrl.GetItemText(def_SMSG_ROWNUM_SVALUE, def_COLUMN_VALUE);
    switch(ouSendMsgEntity.m_eSignalUnitType)
    {
        case ENG:
            ouSendMsgEntity.m_uDefaultSignalValue.m_fValue = (float)atof(omstrTemp);
            break;
        case RAW:
        default:
            ouSendMsgEntity.m_uDefaultSignalValue.m_u64Value = _atoi64(omstrTemp);
            break;
    }*/

    CSignalData ouSignalData;
    unSignalCount = omTempListCtrl.GetItemCount() - def_SMAG_ROWNUM_SVALUE;
    ouSendMsgEntity.m_odSignalDataList.RemoveAll();
    for(UINT i = 0 ; i < unSignalCount; i++)
    {
        ouSignalData.m_omSigName = omTempListCtrl.GetItemText(i+def_SMAG_ROWNUM_SVALUE, def_COLUMN_CATEGORY);
        omstrTemp = omTempListCtrl.GetItemText(i+def_SMAG_ROWNUM_SVALUE, def_COLUMN_VALUE);
        if(ouSendMsgEntity.m_eSignalUnitType == ENG)
        {
            ouSignalData.m_uValue.m_fValue = (float)atof(omstrTemp);
        }
        else
        {
            ouSignalData.m_uValue.m_u64Value = _atoi64(omstrTemp.GetBuffer(MAX_PATH));
        }
        ouSendMsgEntity.m_odSignalDataList.AddTail(ouSignalData);
    }

    ((CSend_MessageEntity*)pEntity)->SetEntityData(SEND_MESSAGE, &ouSendMsgEntity);
}

/******************************************************************************
Function Name  :  vSaveVerifyMessageInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vSaveVerifyMessageInfo(CBaseEntityTA* pEntity)
{
    CHECKENTITY(pEntity);

    CListCtrl& omTempListCtrl = m_odPropertyView->m_omPropertyList;

    CString omstrTemp;
    UINT unSignalCount;
    CVerify_MessageData odVerifyMsgEntity;
    ((CVerify_MessageEntity*)pEntity)->GetEntityData(VERIFY_MESSAGE, &odVerifyMsgEntity);
    //Failure Classification Information
    omstrTemp = omTempListCtrl.GetItemText(def_VMSG_ROWNUM_SUINT, def_COLUMN_VALUE);

    if(omstrTemp == _("ENG"))
    {
        odVerifyMsgEntity.m_eSignalUnitType = ENG;
    }
    else                          //Take Anything as RAW
    {
        odVerifyMsgEntity.m_eSignalUnitType = RAW;
    }

    CSignalCondition ouSignalCondition;
    unSignalCount = omTempListCtrl.GetItemCount() - 1;
    odVerifyMsgEntity.m_odSignalConditionList.RemoveAll();
    for(UINT i = 0 ; i < unSignalCount; i++)
    {
        ouSignalCondition.m_omSigName = omTempListCtrl.GetItemText(i+1, def_COLUMN_CATEGORY);
        omstrTemp = omTempListCtrl.GetItemText(i+1, def_COLUMN_VALUE);
        ouSignalCondition.m_omCondition = omstrTemp;
        odVerifyMsgEntity.m_odSignalConditionList.AddTail(ouSignalCondition);
    }

    ((CVerify_MessageEntity*)pEntity)->SetEntityData(VERIFY_MESSAGE, &odVerifyMsgEntity);
}

/******************************************************************************
Function Name  :  vSaveWaitInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vSaveWaitInfo(CBaseEntityTA* pEntity)
{
    CHECKENTITY(pEntity);
    CString omstrTemp;
    CWaitEntityData odData;
    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;

    pEntity->GetEntityData(WAIT, &odData);
    odData.m_omPurpose = omTempListCtrl.GetItemText(def_WAIT_ROWNUM_PURPOSE, 1);
    omstrTemp = omTempListCtrl.GetItemText(def_WAIT_ROWNUM_DELAY, 1);
    odData.m_ushDuration = (USHORT)atoi(omstrTemp);
    pEntity->SetEntityData(WAIT, &odData);
}

/******************************************************************************
Function Name  :  vSaveReplayInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vSaveReplayInfo(CBaseEntityTA* pEntity)
{
    CHECKENTITY(pEntity);
    CString omstrTemp = m_odPropertyView->m_omPropertyList.GetItemText(def_REPLAY_ROWNUM_FILEPATH, 1);
    pEntity->SetEntityData(REPLAY, &omstrTemp);
}

/******************************************************************************
Function Name  :  vSetModifiedFlag
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vSetModifiedFlag(BOOL isModified)
{
    m_bModified = isModified;
    if(isModified == TRUE)
    {
        vSetFileSavedFlag(FALSE);
    }
}

/******************************************************************************
Function Name  :  vSetFileSavedFlag
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vSetFileSavedFlag(BOOL isModified)
{
    m_bFileSaved = isModified;
}

/******************************************************************************
Function Name  :  vSetNumberOfActiveChannels
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  GT-Derka
Date Created   :  05/05/2014
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vSetNumberOfActiveChannels(INT number)
{
    m_iNumberOfActiveChannels = number;
}


/******************************************************************************
Function Name  :  vListCtrlItemChanged
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vListCtrlItemChanged(LPNMLISTVIEW pNMLV)
{
    if(m_pCurrentEntity->GetEntityType() == TEST_SETUP)
    {
        vHandleTestSetup(pNMLV);
    }
    if(m_pCurrentEntity->GetEntityType() == SEND)
    {
        vHandleSendEntity(pNMLV);
    }
    else if(m_pCurrentEntity->GetEntityType() == VERIFY)
    {
        vHandleVerifyEntity(pNMLV);
    }
    else if(m_pCurrentEntity->GetEntityType() == VERIFYRESPONSE)
    {
        vHandleVerifyResponseEntity(pNMLV);
    }
}

/******************************************************************************
Function Name  :  vHandleTestSetup
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vHandleTestSetup(LPNMLISTVIEW pNMLV)
{
    if(pNMLV->iItem == def_TS_ROWNUM_DATABASE)
    {
        CString omstrDatabaseName = m_odPropertyView->m_omPropertyList.GetItemText(def_TS_ROWNUM_DATABASE, 1);
        CTestSetupHeader ouHeaderInfo;
        m_ouTSEntity.GetHeaderData(ouHeaderInfo);

        if(ouHeaderInfo.m_omDatabasePath == "")
        {
            ouHeaderInfo.m_omDatabasePath = omstrDatabaseName;
            if ( m_ouTSEntity.m_ouDataBaseManager.FillDataStructureFromDatabaseFile( omstrDatabaseName.GetBuffer( 0 ) ) == FALSE )
            {
                // Remove the entry from the list box
                CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;
                omTempListCtrl.SetItemText(def_TS_ROWNUM_DATABASE, def_COLUMN_VALUE, "");
            }
        }
        else
        {
            if(omstrDatabaseName != ouHeaderInfo.m_omDatabasePath)
            {
                if(omstrDatabaseName == "")
                {
                    m_ouTSEntity.vDeleteAllSubMessages();
                    OnDisplayReset();
                    return;
                }
                int nRetVal = MessageBox(_("Database Path Is Changed.All Old messages of Test Setup File will be Deleted.\nDo you Want To Continue"), _("Database Path Changed"),MB_YESNO||MB_ICONWARNING);
                if(nRetVal == IDOK)
                {
                    ouHeaderInfo.m_omDatabasePath = omstrDatabaseName;
                    ERRORCODE ecCode = m_ouTSEntity.m_ouDataBaseManager.FillDataStructureFromDatabaseFile( omstrDatabaseName.GetBuffer( 0 ) );
                    if ( ecCode != EC_SUCCESS )
                    {
                        if ( EC_INVALID_PROTOCOL == ecCode )
                        {
                            MessageBox( DATABASE_INV_SELECT );
                        }
                        // Remove the entry from the list box
                        CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;
                        omTempListCtrl.SetItemText(def_TS_ROWNUM_DATABASE, def_COLUMN_VALUE, "");
                    }
                    m_ouTSEntity.vDeleteAllSubMessages();
                    OnDisplayReset();
                }
                else
                {
                    if(m_pCurrentEntity->GetEntityType() == TEST_SETUP)
                    {
                        vDisplayHeaderInfo(0);
                    }
                }
            }
        }
    }
}


/******************************************************************************
Function Name  :  vHandleSendEntity
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CTSEditorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam, GT-Derka
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
void CTSEditorChildFrame::vHandleSendEntity(LPNMLISTVIEW pNMLV)
{
    //CTreeCtrl &omTempTreeCtrl = m_odTreeView->GetTreeCtrl();
    SLISTINFO sListInfo;
    SLISTINFO sListInfoChannel;
    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;
    CString omstrTemp;
    if(m_pCurrentEntity->GetEntityType() == SEND)
    {
        int item = pNMLV->iItem;
        if(item >= defLIST_SEND_ST_ROW)      //item is with in message list     /* derka-comment: Presumably: Get Item that was updated */
        {
            omstrTemp  = omTempListCtrl.GetItemText(item, def_COLUMN_VALUE);    /* derka-comment: Get value (=message-name) of entry... */
            if(omstrTemp == _(defDELETE_MSG_SYMBOL))                            /* derka-comment: ... and check if Item should become deleted */
            {
                if(item != omTempListCtrl.GetItemCount()-1) // Last Item Check.Still This
                {
                    // Message is not added in List;
                    m_omSendEntity.DeleteSubEntry(item - defLIST_SEND_ST_ROW);      //Message Index
                    omTempListCtrl.DeleteItem(item);
                }
                else
                {
                    omTempListCtrl.SetItemText(item, def_COLUMN_VALUE, _("[Add Message]"));
                }
            }
            else    // if item should not become deleted
            {
                sListInfo.m_eType = eComboItem;
                m_ouTSEntity.m_ouDataBaseManager.nFillMessageList(sListInfo.m_omEntries, TRUE);
                CSend_MessageData odSendMsgData;
                if(item == omTempListCtrl.GetItemCount()-1) //Last Row - ("[Add Message]" was selected)
                {
                    CSend_MessageEntity odNewSendEnity;
                    odSendMsgData.m_omMessageName = omstrTemp;
                    odSendMsgData.m_dwMessageID = m_ouTSEntity.m_ouDataBaseManager.unGetMessageID(omstrTemp);
                    odNewSendEnity.SetEntityData(SEND_MESSAGE, &odSendMsgData);
                    m_omSendEntity.AddSubEntry((CBaseEntityTA*)&odNewSendEnity);

                    vCreateChannelDropdown(omTempListCtrl, odSendMsgData.m_byChannelNumber, m_iNumberOfActiveChannels, item-1); // Create Channel-Dropdown

                    omTempListCtrl.InsertItem(item+1, "");
                    omTempListCtrl.SetItemText(item+1, def_COLUMN_VALUE, _("[Add Message]"));
                    omTempListCtrl.vSetColumnInfo(item+1, def_COLUMN_VALUE, sListInfo);
                }
                else    // message selected
                {
                    CBaseEntityTA* pEntity;

                    CSend_MessageData odOrgMsgData;
                    m_omSendEntity.GetSubEntityObj(item-1, &pEntity);
                    pEntity->GetEntityData(SEND_MESSAGE, &odOrgMsgData);
                    odSendMsgData.m_omMessageName = omstrTemp;
                    odSendMsgData.m_dwMessageID = m_ouTSEntity.m_ouDataBaseManager.unGetMessageID(omstrTemp);
                    if(odOrgMsgData.m_omMessageName != omstrTemp)
                    {
                        pEntity->SetEntityData(SEND_MESSAGE, &odSendMsgData);
                    }

                    omstrTemp  = omTempListCtrl.GetItemText(item, def_COLUMN_CHANNEL);  // Read selected Channel-Number
                    odSendMsgData.m_byChannelNumber = BYTE(atoi( omstrTemp ));  //  Save Channel-Number
                }
                omstrTemp.Format("%d", odSendMsgData.m_dwMessageID);
                omTempListCtrl.DeleteItem(item);
                omTempListCtrl.InsertItem(item, omstrTemp);
                omTempListCtrl.SetItemText(item, def_COLUMN_VALUE, odSendMsgData.m_omMessageName);
                omTempListCtrl.vSetColumnInfo(item, def_COLUMN_VALUE, sListInfo);

                omstrTemp.Format("%d", odSendMsgData.m_byChannelNumber);
                omTempListCtrl.SetItemText(item, def_COLUMN_CHANNEL, omstrTemp);    // Show Channel-Number


            }
            omTempListCtrl.RedrawWindow();  // Redraw window to avoid wrong line coloring, Issue #698
            INT nCount = omTempListCtrl.GetItemCount();
            omstrTemp.Format("%d", nCount-2);
            omTempListCtrl.SetItemText(0, def_COLUMN_VALUE, omstrTemp);
        }
        omTempListCtrl.EnsureVisible(item+1, FALSE);
    }
}

/******************************************************************************
Function Name  :  vHandleVerifyEntity
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam, GT-Derka
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vHandleVerifyEntity(LPNMLISTVIEW pNMLV)
{
    //CTreeCtrl &omTempTreeCtrl = m_odTreeView->GetTreeCtrl();
    SLISTINFO sListInfo;
    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;
    CString omstrTemp;
    int item = pNMLV->iItem;
    if(item >= def_VERIFY_ROWNUM_MSGLIST)      //item is with in message list
    {
        omstrTemp  = omTempListCtrl.GetItemText(item, def_COLUMN_VALUE);
        if(omstrTemp == _(defDELETE_MSG_SYMBOL))
        {
            if(item != omTempListCtrl.GetItemCount()-1) // Lase Item Check.Still This
            {
                // Message is not added in List;
                m_ouVerifyEntity.DeleteSubEntry(item - def_VERIFY_ROWNUM_MSGLIST);      //Message Index
                omTempListCtrl.DeleteItem(item);
            }
            else
            {
                omTempListCtrl.SetItemText(item, def_COLUMN_VALUE, _("[Add Message]"));
            }
        }
        else
        {
            //CVERIFY_MESSAGEData odVerifyMsgData;
            sListInfo.m_eType = eComboItem;
            m_ouTSEntity.m_ouDataBaseManager.nFillMessageList(sListInfo.m_omEntries, TRUE);
            CVerify_MessageData odVerifyMsgData;
            if(item == omTempListCtrl.GetItemCount()-1) //Last Row
            {
                CVerify_MessageEntity odNewVerifyEnity;
                odVerifyMsgData.m_omMessageName = omstrTemp;

                odVerifyMsgData.m_dwMessageID = m_ouTSEntity.m_ouDataBaseManager.unGetMessageID(omstrTemp);
                odNewVerifyEnity.SetEntityData(VERIFY_MESSAGE, &odVerifyMsgData);
                m_ouVerifyEntity.AddSubEntry((CBaseEntityTA*)&odNewVerifyEnity);

                vCreateChannelDropdown(omTempListCtrl, odVerifyMsgData.m_byChannelNumber, m_iNumberOfActiveChannels, item-1);   // Create Channel-Dropdown

                omTempListCtrl.InsertItem(item+1, "");
                omTempListCtrl.SetItemText(item+1, def_COLUMN_VALUE, _("[Add Message]"));
                omTempListCtrl.vSetColumnInfo(item+1, def_COLUMN_VALUE, sListInfo);
            }
            else
            {
                CBaseEntityTA* pEntity; // A message was selected

                CVerify_MessageData odOrgMsgData;
                m_ouVerifyEntity.GetSubEntityObj(item-def_VERIFY_ROWNUM_MSGLIST, &pEntity);
                pEntity->GetEntityData(VERIFY_MESSAGE, &odOrgMsgData);
                odVerifyMsgData.m_omMessageName = omstrTemp;
                odVerifyMsgData.m_dwMessageID = m_ouTSEntity.m_ouDataBaseManager.unGetMessageID(omstrTemp);
                if(odOrgMsgData.m_omMessageName != omstrTemp)
                {
                    pEntity->SetEntityData(VERIFY_MESSAGE, &odVerifyMsgData);
                }
                omstrTemp  = omTempListCtrl.GetItemText(item, def_COLUMN_CHANNEL);  // Read selected Channel-Number
                odVerifyMsgData.m_byChannelNumber = BYTE(atoi( omstrTemp ));    // Save Channel-Number
            }
            omstrTemp.Format("%d", odVerifyMsgData.m_dwMessageID);
            omTempListCtrl.DeleteItem(item);
            omTempListCtrl.InsertItem(item, omstrTemp);
            omTempListCtrl.SetItemText(item, def_COLUMN_VALUE, odVerifyMsgData.m_omMessageName);
            omTempListCtrl.vSetColumnInfo(item, def_COLUMN_VALUE, sListInfo);

            omstrTemp.Format("%d", odVerifyMsgData.m_byChannelNumber);
            omTempListCtrl.SetItemText(item, def_COLUMN_CHANNEL, omstrTemp);    // Show Channel-Number
        }
        UINT unCount;
        omTempListCtrl.RedrawWindow();  // Redraw window to avoid wrong line coloring, Issue #698
        m_ouVerifyEntity.GetSubEntryCount(unCount);
        omstrTemp.Format("%d", unCount);
        omTempListCtrl.SetItemText(0, def_COLUMN_VALUE, omstrTemp);
    }
    omTempListCtrl.EnsureVisible(item+1, FALSE);
}

/******************************************************************************
Function Name  :  vHandleVerifyResponseEntity
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CTSEditorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam, GT-Derka
Date Created   :  21/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
void CTSEditorChildFrame::vHandleVerifyResponseEntity(LPNMLISTVIEW pNMLV)
{
    //CTreeCtrl &omTempTreeCtrl = m_odTreeView->GetTreeCtrl();
    SLISTINFO sListInfo;
    CListCtrlEx& omTempListCtrl = m_odPropertyView->m_omPropertyList;
    CString omstrTemp;
    int item = pNMLV->iItem;
    if(item >= def_VERIFYRES_ROWNUM_MSGLIST)      //item is with in message list
    {
        omstrTemp  = omTempListCtrl.GetItemText(item, def_COLUMN_VALUE);
        if(omstrTemp == _(defDELETE_MSG_SYMBOL))
        {
            if(item != omTempListCtrl.GetItemCount()-1) // Lase Item Check.Still This
            {
                // Message is not added in List;
                m_ouVerifyEntity.DeleteSubEntry(item - def_VERIFYRES_ROWNUM_MSGLIST);      //Message Index
                omTempListCtrl.DeleteItem(item);
            }
            else
            {
                omTempListCtrl.SetItemText(item, def_COLUMN_VALUE, _("[Add Message]"));
            }
        }
        else
        {
            //CVERIFY_MESSAGEData odVerifyMsgData;
            sListInfo.m_eType = eComboItem;
            m_ouTSEntity.m_ouDataBaseManager.nFillMessageList(sListInfo.m_omEntries, TRUE);
            CVerify_MessageData odVerifyMsgData;
            if(item == omTempListCtrl.GetItemCount()-1) //Last Row
            {
                CVerify_MessageEntity odNewVerifyEnity;
                odVerifyMsgData.m_omMessageName = omstrTemp;

                odVerifyMsgData.m_dwMessageID = m_ouTSEntity.m_ouDataBaseManager.unGetMessageID(omstrTemp);
                odNewVerifyEnity.SetEntityData(VERIFY_MESSAGE, &odVerifyMsgData);
                m_ouVerifyEntity.AddSubEntry((CBaseEntityTA*)&odNewVerifyEnity);

                omTempListCtrl.InsertItem(item+1, "");
                omTempListCtrl.SetItemText(item+1, def_COLUMN_VALUE, _("[Add Message]"));
                omTempListCtrl.vSetColumnInfo(item+1, def_COLUMN_VALUE, sListInfo);
            }
            else
            {
                CBaseEntityTA* pEntity;

                CVerify_MessageData odOrgMsgData;
                m_ouVerifyEntity.GetSubEntityObj(item-def_VERIFYRES_ROWNUM_MSGLIST, &pEntity);
                pEntity->GetEntityData(VERIFY_MESSAGE, &odOrgMsgData);
                odVerifyMsgData.m_omMessageName = omstrTemp;
                odVerifyMsgData.m_dwMessageID = m_ouTSEntity.m_ouDataBaseManager.unGetMessageID(omstrTemp);
                if(odOrgMsgData.m_omMessageName != omstrTemp)
                {
                    pEntity->SetEntityData(VERIFY_MESSAGE, &odVerifyMsgData);
                }
            }
            omstrTemp.Format("%d", odVerifyMsgData.m_dwMessageID);
            omTempListCtrl.DeleteItem(item);
            omTempListCtrl.InsertItem(item, omstrTemp);
            omTempListCtrl.SetItemText(item, def_COLUMN_VALUE, odVerifyMsgData.m_omMessageName);
            omTempListCtrl.vSetColumnInfo(item, def_COLUMN_VALUE, sListInfo);
        }
        UINT unCount;
        omTempListCtrl.RedrawWindow();  // Redraw window to avoid wrong line coloring, Issue #698
        m_ouVerifyEntity.GetSubEntryCount(unCount);//omTempListCtrl.GetItemCount();
        omstrTemp.Format("%d", unCount);
        omTempListCtrl.SetItemText(0, def_COLUMN_VALUE, omstrTemp);
    }
    omTempListCtrl.EnsureVisible(item+1, FALSE);
}
/******************************************************************************
Function Name  :  unRepisitonEntry
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
UINT CTSEditorChildFrame::unRepisitonEntry(DWORD dwRepositionItemID, DWORD dwInsertAfterItemID, DWORD dwParentID)
{
    CBaseEntityTA* ouRepositionItem;
    CBaseEntityTA* ouInsertAfterItem;
    CBaseEntityTA* ouParentItem;

    m_ouTSEntity.SearchEntityObject(dwRepositionItemID, &ouRepositionItem);
    m_ouTSEntity.SearchEntityObject(dwInsertAfterItemID, &ouInsertAfterItem);
    m_ouTSEntity.SearchEntityObject(dwParentID, &ouParentItem);

    HRESULT hResult = S_FALSE;
    if( nullptr != ouParentItem)
    {
        hResult = ouParentItem->RepositionSubEntity(ouRepositionItem, ouInsertAfterItem);
    }

    return hResult;
}

/******************************************************************************
Function Name  :  eGetCurrentEntityType
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
eTYPE_ENTITY CTSEditorChildFrame::eGetCurrentEntityType()
{
    return m_pCurrentEntity->GetEntityType();
}

/******************************************************************************
Function Name  :  nAddNewEntity
Input(s)       :  DWORD dwId - Parent ID
                  eTYPE_ENTITY eEntityType - Entity Type;
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :  CS007
******************************************************************************/
INT CTSEditorChildFrame::nAddNewEntity(DWORD dwId, eTYPE_ENTITY eEntityType)
{
    CTreeViewEx& omTempTreeCtrl = (CTreeViewEx&)m_odTreeView->GetTreeCtrl();
    CBaseEntityTA* pParentEntity = nullptr, *pNewEntity;
    m_ouTSEntity.SearchEntityObject(dwId, &pParentEntity);
    CString omStrNewItem;
    UINT unImageIndex;
    BOOL bFileModified = FALSE;
    switch(eEntityType)
    {
        case TEST_CASE:
        {
            CTestCaseEntity ouTestCaseEntity;
            m_ouTSEntity.AddSubEntry(&ouTestCaseEntity);
            CBaseEntityTA* pouTestCaseEntity;
            m_ouTSEntity.GetSubEntityObj(m_ouTSEntity.GetSubEntryCount()-1, &pouTestCaseEntity);
            omTempTreeCtrl.InsertTreeItem(m_hParentTreeItem, _("Untitled TestCase"), nullptr, 0, 0, pouTestCaseEntity->GetID());
            omTempTreeCtrl.RedrawWindow();
            vSetModifiedFlag(TRUE);
            return S_OK;
        }
        case SEND:
        {
            pNewEntity = new CSendEntity();
            omStrNewItem = _("Send");
            unImageIndex = def_INDEX_SEND_IMAGE;
            vSetModifiedFlag(TRUE);
            break;
        }
        case VERIFY:
        {
            pNewEntity = new CVerifyEntity();
            omStrNewItem = _("Verify");
            unImageIndex = def_INDEX_VERIFY_IMAGE;
            vSetModifiedFlag(TRUE);
            break;
        }
        case WAIT:
        {
            pNewEntity = new CWaitEntity();
            omStrNewItem = _("Wait");
            unImageIndex = def_INDEX_WAIT_IMAGE;
            vSetModifiedFlag(TRUE);
            break;
        }
        case REPLAY:
        {
            pNewEntity = new CReplayEntity();
            omStrNewItem = _("Replay");
            unImageIndex = def_INDEX_REPLAY_IMGAE;
            vSetModifiedFlag(TRUE);
            break;
        }
        case VERIFYRESPONSE:
        {
            pNewEntity = new CVerifyResponse();
            ((CVerifyResponse*)pNewEntity)->m_ushDuration = 0;
            omStrNewItem = _("VerfiyResponse");
            unImageIndex = def_INDEX_VERIFY_IMAGE;
            vSetModifiedFlag(TRUE);
            break;
        }
        default:
            return S_FALSE;
    }

    if(nullptr != pParentEntity)
    {
        pParentEntity->AddSubEntry(pNewEntity);
    }

    m_odTreeView->InsertTreeItem(m_hCurrentTreeItem, omStrNewItem, nullptr, unImageIndex, unImageIndex, pNewEntity->GetID());
    m_odTreeView->RedrawWindow();
    return S_OK;
}

/******************************************************************************
Function Name  :  nChangeEntityTitle
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
INT CTSEditorChildFrame::nChangeEntityTitle(CBaseEntityTA* pEntity, CString& omstrName)
{
    if(pEntity == nullptr)               //Add to Current Entity;
    {
        pEntity = m_pCurrentEntity;
    }
    eTYPE_ENTITY eEntityType = pEntity->GetEntityType();
    switch(eEntityType)
    {
        case TEST_SETUP:
        {
            if(omstrName.IsEmpty() == FALSE)
            {
                m_ouTSEntity.m_omstrTestSetupTitle = omstrName;
            }
            vDisplayHeaderInfo(0);
            vSetFileSavedFlag(FALSE);
            break;
        }
        case TEST_CASE:
        {
            CTestCaseData ouTestCaseData;
            ((CTestCaseEntity*)pEntity)->GetTestCaseDetails(ouTestCaseData.m_omTitle, ouTestCaseData.m_omID, ouTestCaseData.m_eExcpAction);
            if(omstrName.IsEmpty() == FALSE)
            {
                ouTestCaseData.m_omTitle = omstrName;
            }
            ((CTestCaseEntity*)pEntity)->SetTestCaseDetails(ouTestCaseData.m_omTitle, ouTestCaseData.m_omID, ouTestCaseData.m_eExcpAction);
            vDisplayTestcaseInfo(pEntity);
            vSetFileSavedFlag(FALSE);
            break;
        }
    }
    return 0;
}

/******************************************************************************
Function Name  :  nDeleteItem
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :  CS010
******************************************************************************/
INT CTSEditorChildFrame::nDeleteItem(DWORD dwId, DWORD dwParentId)
{
    //CTreeCtrl &omTempTreeCtrl = m_odTreeView->GetTreeCtrl();
    CBaseEntityTA* pouDelEntity, *pouParentItem;
    m_ouTSEntity.SearchEntityObject(dwId, &pouDelEntity);
    if (nullptr != pouDelEntity)
    {
        if(pouDelEntity->GetEntityType() != TEST_CASE)
        {
            m_ouTSEntity.SearchEntityObject(dwParentId, &pouParentItem);
            if (nullptr != pouParentItem)
            {
                pouParentItem->DeleteSubEntry(pouDelEntity);
            }
        }
        else if(pouDelEntity->GetEntityType() == TEST_CASE)
        {
            m_ouTSEntity.DeleteSubEntry(pouDelEntity);
        }
    }
    return 0;
}

/******************************************************************************
Function Name  :  OnFileSave
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnFileSave()
{
    //OnSelectionChanging(0, 0);
    nConfirmCurrentChanges();
    if(m_omCurrentTSFile != _(def_EMPTYFILENAME))      //We have a path
    {
        m_ouTSEntity.SaveFileAs(m_omCurrentTSFile);
        m_bFileSaved = TRUE;
        m_bModified = FALSE;
    }
}

/******************************************************************************
Function Name  :  OnUpdateFileSave
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnUpdateFileSave(CCmdUI* pCmdUI)
{
    if((m_bFileSaved == FALSE)||(m_bModified == TRUE))
    {
        pCmdUI->Enable(TRUE);
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
}


/******************************************************************************
Function Name  :  OnFileNew
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CTSEditorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       : CS001
******************************************************************************/
void CTSEditorChildFrame::OnFileNew()
{
    INT nRetVal = nPromptForSaveFile();
    CHECKEQ(nRetVal, IDCANCEL);
    vInitialise();
    if(m_odPropertyView != nullptr)
    {
        m_odPropertyView->m_omPropertyList.DeleteAllItems();
        m_odTreeView->GetTreeCtrl().DeleteAllItems();
    }

    CString omstrFileName(def_EMPTYFILENAME);
    vSetCurrentFile(omstrFileName);


    CFileDialog omFileOpenDlg(FALSE, ".xml", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
    omFileOpenDlg.m_ofn.lpstrTitle = "New TestSetup File";
    if(omFileOpenDlg.DoModal() == IDOK)
    {
        CString omstrFileName = omFileOpenDlg.GetPathName();
        CStdioFile om_File;
        // create the selected file
        if (om_File.Open(omstrFileName, CFile::modeCreate | CFile::modeRead | CFile::typeText))
        {
            om_File.Close();
        }
        //Since Empty File
        vLoadTestSetupFile(omFileOpenDlg.GetPathName(), TRUE);
    }
}


/******************************************************************************
Function Name  :  nPromptForSaveFile
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
INT CTSEditorChildFrame::nPromptForSaveFile()
{
    INT nRetVal = IDYES;
    if(m_bFileSaved == FALSE)
    {
        CString omstrMsg;
        omstrMsg.Format(_("The file '%s' has been modified\nDo you Want to save?"), m_omCurrentTSFile.GetBuffer(MAX_PATH));
        nRetVal = MessageBox(omstrMsg, _("BUSMASTER TestSetup Editor"), MB_YESNOCANCEL|MB_ICONQUESTION);

        if(nRetVal == IDYES)
        {
            OnFileSave();
        }
    }
    return nRetVal;
}

/******************************************************************************
Function Name  :  OnFileSaveas
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnFileSaveas()
{
    CFileDialog omSaveAsFileDlg(FALSE, ".xml", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
    if(IDOK == omSaveAsFileDlg.DoModal())
    {
        CString omSelectedFile = omSaveAsFileDlg.GetPathName();
        m_ouTSEntity.SaveFileAs(omSelectedFile);
        vSetCurrentFile(omSelectedFile);
    }
}

/******************************************************************************
Function Name  :  OnFileClose
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnFileClose()
{
    INT nRetVal = nPromptForSaveFile();
    CHECKEQ(nRetVal, IDCANCEL);
    vInitialise();
    if(m_odPropertyView != nullptr)
    {
        m_odPropertyView->m_omPropertyList.DeleteAllItems();
    }
    CString omstrFileName(_(def_EMPTYFILENAME));
    vSetCurrentFile(omstrFileName);
    CHECKENTITY(m_odTreeView);
    m_odTreeView->GetTreeCtrl().DeleteAllItems();
}

/******************************************************************************
Function Name  :  OnUpdateFileSaveas
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnUpdateFileSaveas(CCmdUI* pCmdUI)
{
    if(m_omCurrentTSFile == _(def_EMPTYFILENAME) || m_omCurrentTSFile == "")
    {
        pCmdUI->Enable(FALSE);
    }
    else
    {
        pCmdUI->Enable(TRUE);
    }
}

/******************************************************************************
Function Name  :  vSetCurrentFile
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vSetCurrentFile(CString& omNewFilePath)
{
    m_omCurrentTSFile = omNewFilePath;
    if(omNewFilePath == _(def_EMPTYFILENAME) || m_omCurrentTSFile.IsEmpty())
    {
        SetWindowText(_("Test Automation Editor"));
    }
    else
    {
        SetWindowText(_("Test Automation Editor:: ") + m_omCurrentTSFile);
    }
}

/******************************************************************************
Function Name  :  OnUpdateFrameTitle
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
    vSetCurrentFile(m_omCurrentTSFile);
    CMDIChildWnd::OnUpdateFrameTitle(bAddToTitle);
}

/******************************************************************************
Function Name  :  OnEditCopy
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnEditCopy()
{
    if(bEnableCopy() == FALSE)
    {
        return;
    }
    if((m_bPasted == TRUE)&&(m_pCurrentEntity != nullptr))
    {
        delete m_podCopyEntity;
        m_podCopyEntity = nullptr;
    }
    vCopyTreeItem(&m_podCopyEntity, m_pCurrentEntity);
}

/******************************************************************************
Function Name  :  vCopyTreeItem
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vCopyTreeItem(CBaseEntityTA** podCopyEntity, CBaseEntityTA* pCurrentEntity)
{
    if(pCurrentEntity->GetEntityType() != TEST_SETUP)
    {
        switch(pCurrentEntity->GetEntityType())
        {
            case TEST_CASE:
                *podCopyEntity = new CTestCaseEntity();
                //((CTestCaseEntity*)pCurrentEntity)->CopySubEntry(podCopyEntity);
                *((CTestCaseEntity*)*podCopyEntity) = *((CTestCaseEntity*)pCurrentEntity);
                break;
            case SEND:
                *podCopyEntity = new CSendEntity();
                *((CSendEntity*)*podCopyEntity) = *((CSendEntity*)pCurrentEntity);
                break;
            case VERIFY:
                *podCopyEntity = new CVerifyEntity();
                *((CVerifyEntity*)*podCopyEntity) = *((CVerifyEntity*)pCurrentEntity);
                break;
            case SEND_MESSAGE:
                *podCopyEntity = new CSend_MessageEntity();
                *((CSend_MessageEntity*)*podCopyEntity) = *((CSend_MessageEntity*)pCurrentEntity);
                break;
            case VERIFY_MESSAGE:
                *podCopyEntity = new CVerify_MessageEntity();
                *((CVerify_MessageEntity*)*podCopyEntity) = *((CVerify_MessageEntity*)pCurrentEntity);
                break;
            case WAIT:
                *podCopyEntity = new CWaitEntity();
                *((CWaitEntity*)*podCopyEntity) = *((CWaitEntity*)pCurrentEntity);
                break;
            case REPLAY:
                *podCopyEntity = new CReplayEntity();
                *((CReplayEntity*)*podCopyEntity) = *((CReplayEntity*)pCurrentEntity);
                break;
            case VERIFYRESPONSE:
                *podCopyEntity = new CVerifyResponse();
                *((CVerifyResponse*)*podCopyEntity) = *((CVerifyResponse*)pCurrentEntity);
        }
    }
}

/******************************************************************************
Function Name  :  OnUpdateEditCopy
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(bEnableCopy());
}
/******************************************************************************
Function Name  :  bEnableCopy
Input(s)       :
Output         :  BOOL
Functionality  :
Member of      :  CTSEditorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  25/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
BOOL CTSEditorChildFrame::bEnableCopy()
{
    BOOL bRetVal;
    if(m_pCurrentEntity == nullptr)
    {
        bRetVal = FALSE;
        return bRetVal;
    }
    //TODO::Copy paste of TEST_CASE was pending.
    if(m_pCurrentEntity->GetEntityType() == TEST_SETUP||m_pCurrentEntity->GetEntityType() == TEST_CASE)
    {
        bRetVal = FALSE;
    }
    else
    {
        bRetVal = TRUE;
    }
    return bRetVal;
}
/******************************************************************************
Function Name  :  OnEditPaste
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnEditPaste()
{
    //CTreeViewEx &omTempTreeCtrl = (CTreeViewEx&)m_odTreeView->GetTreeCtrl();
    if(bEnablePase() == FALSE)
    {
        MessageBox(_("Wrong Item Selected"));
        return;
    }

    CBaseEntityTA* podCopyEntity = nullptr;
    vCopyTreeItem(&podCopyEntity, m_podCopyEntity);

    switch(podCopyEntity->GetEntityType())
    {
        case TEST_CASE:
        {
            m_ouTSEntity.AddSubEntry(m_podCopyEntity);
            nUpdateTreeView();
            break;
        }
        case SEND:
        case VERIFY:
        case WAIT:
        case REPLAY:
        case VERIFYRESPONSE:
            m_pCurrentEntity->AddSubEntry(podCopyEntity);
            parseTestCaseEntiy(m_pCurrentEntity, m_hCurrentTreeItem);
            nDisplayEntity(m_pCurrentEntity);
            break;
        case SEND_MESSAGE:
            m_pCurrentEntity->AddSubEntry(podCopyEntity);
            parseSendEntity(m_pCurrentEntity, m_hCurrentTreeItem);
            nDisplayEntity(m_pCurrentEntity);
            break;
        case VERIFY_MESSAGE:
            m_pCurrentEntity->AddSubEntry(podCopyEntity);
            parseVerifyEntity(m_pCurrentEntity, m_hCurrentTreeItem);
            nDisplayEntity(m_pCurrentEntity);
            break;
        default:
            break;
    }
    m_bPasted = TRUE;
    vSetModifiedFlag(TRUE);
}

/******************************************************************************
Function Name  :  OnUpdateEditPaste
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(bEnablePase());
}
/******************************************************************************
Function Name  :  bEnablePase
Input(s)       :
Output         :  BOOL
Functionality  :
Member of      :  CTSEditorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  25/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
BOOL CTSEditorChildFrame::bEnablePase()
{
    BOOL bRetVal;
    if(m_podCopyEntity == nullptr || m_pCurrentEntity == nullptr)
    {
        bRetVal = FALSE;
        return bRetVal;
    }

    if(isParentChild(m_pCurrentEntity->GetEntityType(), m_podCopyEntity->GetEntityType()) ==TRUE)
    {
        bRetVal = TRUE;
    }
    else
    {
        bRetVal  = FALSE;
    }
    return bRetVal;
}
/******************************************************************************
Function Name  :  isParentChild
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
BOOL CTSEditorChildFrame::isParentChild(eTYPE_ENTITY eParent, eTYPE_ENTITY eChild)
{
    BOOL bRetVal = FALSE;
    switch(eChild)
    {
        case TEST_CASE:         //Currently disabled
        {
            bRetVal = TRUE;
        }
        break;
        case SEND:
        case VERIFY:
        case WAIT:
        case REPLAY:
        case VERIFYRESPONSE:
        {
            if(eParent == TEST_CASE)
            {
                bRetVal = TRUE;
            }
        }
        break;
        case SEND_MESSAGE:
        {
            if(eParent == SEND)
            {
                bRetVal = TRUE;
            }

            break;
        }
        case VERIFY_MESSAGE:
        {
            if(eParent == VERIFY || eParent == VERIFYRESPONSE)
            {
                bRetVal = TRUE;
            }

            break;
        }
        default:
            break;
    }
    return bRetVal;
}

/******************************************************************************
Function Name  :  OnEditCut
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnEditCut()
{
    OnEditCopy();
    CTreeCtrl& omTempTreeCtrl = m_odTreeView->GetTreeCtrl();

    HTREEITEM hParentItem = nullptr;

    if(m_hCurrentTreeItem != nullptr)
    {
        hParentItem = omTempTreeCtrl.GetParentItem(m_hCurrentTreeItem);
    }

    if(hParentItem != nullptr)
    {
        DWORD dwParentId = (DWORD)omTempTreeCtrl.GetItemData(hParentItem);

        if(m_pCurrentEntity != nullptr)
        {
            nDeleteItem(m_pCurrentEntity->GetID(), dwParentId);
        }
    }

    m_pCurrentEntity = nullptr;

    if(m_odPropertyView != nullptr)
    {
        m_odPropertyView->m_omPropertyList.DeleteAllItems();
    }

    if(m_hCurrentTreeItem != nullptr)
    {
        omTempTreeCtrl.DeleteItem(m_hCurrentTreeItem);
    }
    vSetModifiedFlag(TRUE);
}

/******************************************************************************
Function Name  :  OnUpdateEditCut
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnUpdateEditCut(CCmdUI* pCmdUI)
{
    OnUpdateEditCopy(pCmdUI);
}

/******************************************************************************
Function Name  :  OnDisplayReset
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnDisplayReset()
{
    nUpdateTreeView();
}

/******************************************************************************
Function Name  :  vShowHelpInfo
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vShowHelpInfo(eTYPE_ENTITY eEntityType)
{
    CString omHelpString;
    switch(eEntityType)
    {
        case TEST_SETUP:
            omHelpString.LoadString(IDS_HELP_TESTSETUP);
            break;
        case TEST_CASE:
            omHelpString.LoadString(IDS_HELP_TESTCASE);
            break;
        case SEND:
            omHelpString.LoadString(IDS_HELP_SEND);
            break;
        case VERIFY:
            omHelpString.LoadString(IDS_HELP_VERIFY);
            break;
        case SEND_MESSAGE:
            return;
            break;
        case VERIFY_MESSAGE:
            return;
            break;
        case WAIT:
            omHelpString.LoadString(IDS_HELP_WAIT);
            break;
        case REPLAY:
            omHelpString.LoadString(IDS_HELP_REPLAY);
            break;
        default:
            break;
    }
    m_odPropertyView->vShowHelpInfo(omHelpString);
}

/******************************************************************************
Function Name  :  OnDisplaySettings
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::OnDisplaySettings()
{
    CTSEditorSettingsDlg ouSettingsDlg;
    COLORREF omBkColor, omTextColor, omRow1Color, omRow2Color;
    m_odTreeView->vGetTreeCtrlColor(omBkColor, omTextColor);
    m_odPropertyView->m_omPropertyList.vGetRowColors(omRow1Color, omRow2Color);

    ouSettingsDlg.m_ouBkColorBtn.SetColour(omBkColor);
    ouSettingsDlg.m_ouTxtColorBtn.SetColour(omTextColor);
    ouSettingsDlg.m_Row1Color.SetColour(omRow1Color);
    ouSettingsDlg.m_Row2Color.SetColour(omRow2Color);
    ouSettingsDlg.m_bQueryConfirm = !m_bQueryConfirm;
    if(ouSettingsDlg.DoModal() == IDOK)
    {
        m_odTreeView->vSetTreeCtrlColor(ouSettingsDlg.m_ouBkColorBtn.GetColour(),
                                        ouSettingsDlg.m_ouTxtColorBtn.GetColour());
        m_odPropertyView->m_omPropertyList.vSetRowColors(ouSettingsDlg.m_Row1Color.GetColour(),
                ouSettingsDlg.m_Row2Color.GetColour());
        m_bQueryConfirm = !ouSettingsDlg.m_bQueryConfirm;
    }

}

/******************************************************************************
Function Name  :  OnFileValidate
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CTSEditorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  13/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
void CTSEditorChildFrame::OnFileValidate()
{
    CString omStrResult;
    HRESULT hResult = m_ouTSEntity.ValidateEntity(omStrResult);
    m_odPropertyView->vShowHelpInfo(omStrResult);
    if(hResult != ERR_VALID_SUCCESS)
    {
        MessageBox(_("Found Some Errors\\Warnings"), _("Validation Failed"), MB_OK|MB_ICONERROR);
    }
    else
    {
        MessageBox(_("No Errors\\Warnings are Found"), _("Validation Success"), MB_OK|MB_ICONINFORMATION);
    }
}
/******************************************************************************
Function Name  :  PreTranslateMessage
Input(s)       :
Output         :
Functionality  :
Member of      :  CBusStatisticCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  04/03/2011
Modifications  :
******************************************************************************/
BOOL CTSEditorChildFrame::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN)
    {
        if(GetKeyState(VK_LCONTROL)<0)
        {
            switch(pMsg->wParam)
            {
                case 'C':
                case 'c':
                    OnEditCopy();
                    return TRUE;
                case 'V':
                case 'v':
                    // If F7 is not down
                    if(!(GetKeyState(VK_F7) < 0))
                    {
                        OnEditPaste();
                        return TRUE;
                    }
                    else
                    {
                        return FALSE;
                    }
                case 'X':
                case 'x':
                    OnEditCut();
                    return TRUE;
                case 'N':
                case 'n':
                    OnFileNew();
                    return TRUE;
                case 'O':
                case 'o':
                    OnFileOpen();
                    return TRUE;
                case 'S':
                case 's':
                    // If F4 is not down
                    if(!(GetKeyState(VK_F4) < 0))
                    {
                        OnFileSave();
                        return TRUE;
                    }
                    else
                    {
                        return FALSE;
                    }
                default:
                    break;
            }
        }
    }
    return CMDIChildWnd::PreTranslateMessage(pMsg);
}
/******************************************************************************
Function Name  :  GetConfigurationData
Input(s)       :  BYTE*& pDesBuffer - Confiduration data
                  UINT& nBuffSize - Buffer Size
Output         :  HRESULT - S_OK if success
Functionality  :
Member of      :  CTSEditorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTSEditorChildFrame::GetConfigurationData(BYTE*& pDesBuffer, UINT& nBuffSize)
{
    nBuffSize = (4*sizeof(COLORREF) + sizeof(BOOL) + sizeof(WINDOWPLACEMENT) + 2*sizeof(int));
    pDesBuffer = new BYTE[nBuffSize];
    COLORREF omCol1, omCol2;

    BYTE* pTemp = pDesBuffer;

    m_odTreeView->vGetTreeCtrlColor(omCol1, omCol2);
    COPY_DATA(pTemp, &omCol1, sizeof(COLORREF));
    COPY_DATA(pTemp, &omCol2, sizeof(COLORREF));

    m_odPropertyView->m_omPropertyList.vGetRowColors(omCol1, omCol2);
    COPY_DATA(pTemp, &omCol1, sizeof(COLORREF));
    COPY_DATA(pTemp, &omCol2, sizeof(COLORREF));

    //Window position
    WINDOWPLACEMENT wndPlacement;
    GetWindowPlacement(&wndPlacement);

    //TreeView Window Position
    INT nCxCur, nCxMin;
    m_omSplitterWnd.GetColumnInfo(0, nCxCur, nCxMin);

    COPY_DATA(pTemp, &m_bQueryConfirm, sizeof(BOOL));

    COPY_DATA(pTemp, &wndPlacement, sizeof(WINDOWPLACEMENT));
    COPY_DATA(pTemp, &nCxCur, sizeof(INT));
    COPY_DATA(pTemp, &nCxMin, sizeof(INT));
    return S_OK;
}
bool CTSEditorChildFrame::GetConfigurationData(xmlNodePtr& pxmlNodePtr)
{
    const char* omcVarChar ;

    pxmlNodePtr = xmlNewNode(nullptr, BAD_CAST DEF_TS_EDITOR);

    if(m_omCurrentTSFile.IsEmpty() == FALSE && m_omCurrentTSFile != def_EMPTYFILENAME)
    {

        std::string omPath, omStrConfigFolder;
        char configPath[MAX_PATH]= {0};
        AfxGetMainWnd()->SendMessage(MSG_GET_CONFIGPATH, (WPARAM)configPath, 0);
        CUtilFunctions::nGetBaseFolder(configPath, omStrConfigFolder );
        CUtilFunctions::MakeRelativePath(omStrConfigFolder.c_str(), (char*)m_omCurrentTSFile.GetBuffer(MAX_PATH), omPath);
        omcVarChar = omPath.c_str();

        xmlNodePtr pFilePath = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_TS_XML_FILE_PATH, BAD_CAST omcVarChar);
        xmlAddChild(pxmlNodePtr, pFilePath);
    }
    //<Tree_Bkg_Color>ff0000</Tree_Bkg_Color>
    COLORREF omCol1, omCol2;
    m_odTreeView->vGetTreeCtrlColor(omCol1, omCol2);
    CString csClor;
    csClor.Format("%x", omCol1);
    omcVarChar = csClor;
    xmlNodePtr pClr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_TREE_BKG_CLR,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pClr);

    //<Tree_Text_Color>ff0000</Tree_Text_Color>
    csClor.Format("%x", omCol2);
    omcVarChar = csClor;
    pClr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_TREE_TXT_COLOR,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pClr);

    //<List_Row1_Color>ff0000</List_Row1_Color>
    m_odPropertyView->m_omPropertyList.vGetRowColors(omCol1, omCol2);
    csClor.Format("%x", omCol1);
    omcVarChar = csClor;
    pClr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_LST_ROW1_CLR,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pClr);

    //<List_Row2_Color>ff0000</List_Row2_Color>
    csClor.Format("%x", omCol2);
    omcVarChar = csClor;
    pClr = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_LST_ROW2_CLR,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pClr);

    //Window position
    WINDOWPLACEMENT wndPlacement;
    GetWindowPlacement(&wndPlacement);

    xmlNodePtr pNodeWndPos = xmlNewNode(nullptr, BAD_CAST DEF_WND_POS);
    xmlAddChild(pxmlNodePtr, pNodeWndPos);

    if(IsWindowVisible() == FALSE)
    {
        wndPlacement.showCmd = SW_HIDE;
    }

    xmlUtils::CreateXMLNodeFrmWindowsPlacement(pNodeWndPos,wndPlacement);

    //TreeView Window Position
    INT nCxCur, nCxMin;
    m_omSplitterWnd.GetColumnInfo(0, nCxCur, nCxMin);

    xmlNodePtr pNodeSW = xmlNewNode(nullptr, BAD_CAST DEF_SPLITTER_WINDOW);
    xmlAddChild(pxmlNodePtr, pNodeSW);

    //<CxIdeal />
    CString  csCxIdeal;
    csCxIdeal.Format("%d", nCxCur);
    omcVarChar = csCxIdeal;
    xmlNodePtr pCxIdeal = xmlNewChild(pNodeSW, nullptr, BAD_CAST DEF_CX_IDEAL, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSW, pCxIdeal);

    // <CxMin />
    CString  csCxMin;
    csCxMin.Format("%d",nCxMin);
    omcVarChar = csCxMin;
    xmlNodePtr pcsCxMin = xmlNewChild(pNodeSW, nullptr, BAD_CAST DEF_CX_MIN, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSW, pcsCxMin);

    return true;
}
/******************************************************************************
Function Name  :  SetConfigurationData
Input(s)       :  BYTE*& pDesBuffer - Confiduration data
                  UINT& nBuffSize - Buffer Size
Output         :  HRESULT - S_OK if success
Functionality  :
Member of      :  CTSEditorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTSEditorChildFrame::SetConfigurationData(BYTE* pSrcBuffer, UINT unBuffSize)
{
    // On New config creation close the file
    OnFileClose();
    if(unBuffSize != 0)
    {
        COLORREF omCol1, omCol2;

        BYTE* pTemp = pSrcBuffer;

        COPY_DATA_2(&omCol1, pTemp, sizeof(COLORREF));
        COPY_DATA_2(&omCol2, pTemp, sizeof(COLORREF));
        m_odTreeView->vSetTreeCtrlColor(omCol1, omCol2);

        m_odPropertyView->m_omPropertyList.vGetRowColors(omCol1, omCol2);
        COPY_DATA_2(&omCol1, pTemp, sizeof(COLORREF));
        COPY_DATA_2(&omCol2, pTemp, sizeof(COLORREF));
        m_odPropertyView->m_omPropertyList.vSetRowColors(omCol1, omCol2);

        COPY_DATA_2(&m_bQueryConfirm, pTemp, sizeof(BOOL));

        WINDOWPLACEMENT wndPlacement;
        COPY_DATA_2(&wndPlacement, pTemp,sizeof(WINDOWPLACEMENT));
        SetWindowPlacement(&wndPlacement);

        INT nCxCur, nCxMin;
        COPY_DATA_2(&nCxCur, pTemp, sizeof(INT));
        COPY_DATA_2(&nCxMin, pTemp, sizeof(INT));
        m_omSplitterWnd.SetColumnInfo(0, nCxCur, nCxMin);
        m_omSplitterWnd.RecalcLayout();

    }
    else
    {

        m_odTreeView->vSetDefaultColors();
        m_odPropertyView->m_omPropertyList.vSetDefaultColors();
        WINDOWPLACEMENT omTempWnd = m_sTSDefPlacement;
        omTempWnd.showCmd = SW_HIDE;
        SetWindowPlacement(&omTempWnd);
        m_bQueryConfirm = TRUE;
    }
    return S_OK;
}

HRESULT CTSEditorChildFrame::SetConfigurationData(xmlNodePtr pXmlNode)
{
    OnFileClose();

    m_odTreeView->vSetDefaultColors();
    m_odPropertyView->m_omPropertyList.vSetDefaultColors();
    SetWindowPlacement(&m_sTSDefPlacement);
    m_bQueryConfirm = TRUE;


    xmlXPathObjectPtr pTempNode = nullptr;

    BOOL bWindowPos = FALSE;
    BOOL bIsFileFound = FALSE;

    if( nullptr != pXmlNode)
    {
        COLORREF omCol1, omCol2;
        //Get Default Tree Colors
        m_odTreeView->vGetTreeCtrlColor(omCol1, omCol2);
        CString strxmlFilePath = "";

        pTempNode = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*) "XML_File_Path");
        if( nullptr != pTempNode )
        {
            xmlNodePtr pNode = pTempNode->nodesetval->nodeTab[0]->children;
            if(pNode != nullptr)
            {
                xmlChar* pText = xmlNodeListGetString(pNode->doc, pNode, 1);

                if( nullptr != pText)
                {
                    if(PathIsRelative((char*)pText) == TRUE)
                    {
                        std::string omStrConfigFolder;
                        std::string omPath;
                        char configPath[MAX_PATH]= {0};
                        AfxGetMainWnd()->SendMessage(MSG_GET_CONFIGPATH, (WPARAM)configPath, 0);
                        CUtilFunctions::nGetBaseFolder(configPath, omStrConfigFolder );
                        char chAbsPath[MAX_PATH];
                        PathCombine(chAbsPath, omStrConfigFolder.c_str(), (char*)pText);
                        strxmlFilePath = chAbsPath;
                    }
                    else
                    {
                        strxmlFilePath = (char*)pText;
                    }
                    xmlFree(pText);
                }
                if(strxmlFilePath.IsEmpty() == FALSE)
                {
                    vSetCurrentFile(strxmlFilePath);
                    bIsFileFound = TRUE;
                }
            }
        }

        pTempNode = nullptr;
        pTempNode = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*) "Tree_Bkg_Color");
        if( nullptr != pTempNode )
        {
            xmlNodePtr pNode = pTempNode->nodesetval->nodeTab[0]->children;
            if(pNode != nullptr)
            {
                xmlChar* pText = xmlNodeListGetString(pNode->doc, pNode, 1);
                int nVal = strtol((char*)pText, nullptr, 16);
                omCol1 = (COLORREF)nVal;
            }
        }

        pTempNode = nullptr;
        pTempNode = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*) "Tree_Text_Color");
        if( nullptr != pTempNode )
        {
            xmlNodePtr pNode = pTempNode->nodesetval->nodeTab[0]->children;
            if(pNode != nullptr)
            {
                xmlChar* pText = xmlNodeListGetString(pNode->doc, pNode, 1);
                int nVal = strtol((char*)pText, nullptr, 16);
                omCol2 = (COLORREF)nVal;
            }
        }
        m_odTreeView->vSetTreeCtrlColor(omCol1, omCol2);

        //GetDefault List Control Color
        m_odPropertyView->m_omPropertyList.vGetRowColors(omCol1, omCol2);
        pTempNode = nullptr;
        pTempNode = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*) "List_Row1_Color");
        if( nullptr != pTempNode )
        {
            xmlNodePtr pNode = pTempNode->nodesetval->nodeTab[0]->children;
            if(pNode != nullptr)
            {
                xmlChar* pText = xmlNodeListGetString(pNode->doc, pNode, 1);
                int nVal = strtol((char*)pText, nullptr, 16);
                omCol1 = (COLORREF)nVal;
            }

        }

        pTempNode = nullptr;
        pTempNode = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*) "List_Row2_Color");
        if( nullptr != pTempNode )
        {
            xmlNodePtr pNode = pTempNode->nodesetval->nodeTab[0]->children;
            if(pNode != nullptr)
            {
                xmlChar* pText = xmlNodeListGetString(pNode->doc, pNode, 1);
                int nVal = strtol((char*)pText, nullptr, 16);
                omCol2 = (COLORREF)nVal;
            }
        }
        m_odPropertyView->m_omPropertyList.vSetRowColors(omCol1, omCol2);

        //AutoSave
        pTempNode = nullptr;
        pTempNode = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*) "EnableAutoSave");
        if( nullptr != pTempNode )
        {
            xmlNodePtr pNode = pTempNode->nodesetval->nodeTab[0]->children;
            m_bQueryConfirm = xmlUtils::bGetBooleanValue((char*)xmlNodeListGetString(pNode->doc, pNode, 1));
        }

        //Window PlaceMent
        pTempNode = nullptr;
        pTempNode = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*) "Window_Position");
        if( nullptr != pTempNode )
        {
            xmlNodePtr pNode = pTempNode->nodesetval->nodeTab[0];
            WINDOWPLACEMENT wndPlacement;
            if( S_OK == xmlUtils::ParseWindowsPlacement(pNode, wndPlacement))
            {
                bWindowPos = TRUE;
                SetWindowPlacement(&wndPlacement);
            }
        }

        //Splitter Window
        pTempNode = nullptr;
        pTempNode = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*) "Splitter_Window");
        if( nullptr != pTempNode )
        {
            xmlNodePtr pNode = pTempNode->nodesetval->nodeTab[0];
            int nCxCur, nCxMax;
            if( S_OK == xmlUtils::ParseSplitterWindow(pNode, nCxCur, nCxMax))
            {
                m_omSplitterWnd.SetColumnInfo(0, nCxCur, nCxMax);
                m_omSplitterWnd.RecalcLayout();
            }
        }

    }
    else
    {

    }

    if(bWindowPos == FALSE)
    {
        WINDOWPLACEMENT wndPlacement;
        wndPlacement.rcNormalPosition.top = 55;
        wndPlacement.rcNormalPosition.bottom = 629;
        wndPlacement.rcNormalPosition.left = 581;
        wndPlacement.rcNormalPosition.right = 1468;
        wndPlacement.showCmd = SW_NORMAL;
        SetWindowPlacement(&wndPlacement);
    }

    if(bIsFileFound == TRUE)
    {
        if(m_omCurrentTSFile.IsEmpty() == FALSE)
        {
            vLoadTestSetupFile(m_omCurrentTSFile);
        }
    }

    return S_OK;
}
/******************************************************************************
Function Name  :  vDisplaySignalInfo
Input(s)       :  CBaseEntityTA* pBaseEntity - Send or Verify Message entity
                  CString& omStrMsg - Signal Info
Output         :  void
Functionality  :
Member of      :  CTSEditorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
void CTSEditorChildFrame::vDisplaySignalInfo(CString& omStrMsg)
{
    IFrame* sMsg = nullptr;
    if( S_OK == m_ouTSEntity.m_ouDataBaseManager.nGetMessageInfo(omStrMsg, &sMsg))
    {
        UINT unSignalCount = sMsg->GetSignalCount();
        std::map<ISignal*, SignalInstanse> signalList;
        sMsg->GetSignalList( signalList );
        std::string signalName;
        unsigned int signalLength;
        CString omStrText;
        omStrText.Format(def_STR_SIGNAL_HEADING, _("Signal"), _("StartBit"), _("Length"));
for (auto signal : signalList )
        {
            CString omStrSignal;
            signal.first->GetName( signalName );
            signal.first->GetLength( signalLength );
            omStrSignal.Format( def_STR_SIGNAL_FORMAT, signalName.c_str(), signal.second.m_nStartBit, signalLength);
            omStrText+=omStrSignal;
        }
        m_odPropertyView->vShowHelpInfo(omStrText);
    }
}

/******************************************************************************
Function Name  :  OnFileExit
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CTSEditorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  14/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
void CTSEditorChildFrame::OnFileExit(void)
{
    INT nRetVal = nPromptForSaveFile();
    CHECKEQ(nRetVal, IDCANCEL);
    m_omMenu.DestroyMenu();
    AfxGetMainWnd()->SetMenu(CMenu::FromHandle(m_pMainMenu));

    /* Make the next available MDI window active */
    CWnd* pActivateWnd =   GetNextWindow();
    pActivateWnd->SetForegroundWindow();
    pActivateWnd->SetFocus();
    ShowWindow(SW_HIDE);
    //OnFileClose();
    //OnClose();
}
/******************************************************************************
Function Name  :  OnMDIActivate
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CTSEditorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  20/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
void CTSEditorChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
    if(bActivate == TRUE)
    {
        m_hMenuShared = m_omMenu.GetSafeHmenu();
    }

    if(bActivate == FALSE)
    {
        AfxGetMainWnd()->SetMenu(CMenu::FromHandle(m_pMainMenu));
    }

    CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}

/******************************************************************************
Function Name  :  SetTSEditorMenu
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CTSEditorChildFrame
Friend of      :  -
Author(s)      :  Prathiba
Date Created   :  11/09/2012
Modifications  :
Code Tag       :
******************************************************************************/
void CTSEditorChildFrame::SetTSEditorMenu()
{
    if(m_omMenu == nullptr)
    {
        m_omMenu.LoadMenu(IDR_TSEDITORMENU);
        //m_hMenuShared = m_omMenu.GetSafeHmenu();
    }
    else
    {
        AfxGetMainWnd()->SetMenu(&m_omMenu);
        CString strTestSetupfile = m_omCurrentTSFile;
        m_bModified = FALSE;
        m_bFileSaved = TRUE;
        OnFileClose();

        if(strTestSetupfile.IsEmpty() == FALSE && strTestSetupfile != def_EMPTYFILENAME)
        {
            m_omCurrentTSFile = strTestSetupfile;
            vLoadTestSetupFile(strTestSetupfile);
        }
    }
}

/******************************************************************************
Function Name  :  vSetDefaultWndPlacement
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CTSEditorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  20/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
void CTSEditorChildFrame::vSetDefaultWndPlacement(void)
{
    POINT MaxPosition, MinPosition;
    MaxPosition.x = 0;
    MaxPosition.y = 0;
    MinPosition.x = 0;
    MinPosition.y = 0;

    RECT NormalPosition;
    NormalPosition.left = 63;
    NormalPosition.top  = 4;
    NormalPosition.right = 913;
    NormalPosition.bottom = 596;

    m_sTSDefPlacement.length = 44;
    m_sTSDefPlacement.flags = 1;
    m_sTSDefPlacement.showCmd = 1;
    m_sTSDefPlacement.ptMaxPosition = MaxPosition;
    m_sTSDefPlacement.ptMinPosition = MinPosition;
    m_sTSDefPlacement.rcNormalPosition = NormalPosition;
}
/******************************************************************************
Function Name  :  OnHelpTesteditorhelp
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CTSEditorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  28/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
void CTSEditorChildFrame::OnHelpTesteditorhelp()
{
    // Get Application Help File Path
    CString omStrPath = AfxGetApp()->m_pszHelpFilePath;
    // Replace .hlp with .chm
    int nIndex = omStrPath.ReverseFind( PERIOD );
    // Extract string before the extension
    omStrPath = omStrPath.Mid(0, nIndex );
    // Add New Extension
    omStrPath = omStrPath + ".chm"+"::/html/TestAutomation.htm";
    // Make it as content display always
    ::HtmlHelp(nullptr, omStrPath, HH_DISPLAY_TOPIC, 0);
}

void CTSEditorChildFrame::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    /*OnFileClose();
    CMDIChildWnd::OnClose();*/

    INT nRetVal = nPromptForSaveFile();
    CHECKEQ(nRetVal, IDCANCEL);

    AfxGetMainWnd()->SetMenu(CMenu::FromHandle(m_pMainMenu));

    /* Make the next available MDI window active */
    CWnd* pActivateWnd =   GetNextWindow();
    pActivateWnd->SetForegroundWindow();
    pActivateWnd->SetFocus();
    ShowWindow(SW_HIDE);
}

/**
* \brief         Handler for GetMinMaxInfo event where default window size is validated.
* \param         void
* \return        S_OK if CAN_ResetHardware call is success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          31.01.2013 Created
*/
void CTSEditorChildFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    if(!m_bInit)
    {
        lpMMI->ptMinTrackSize.x = g_unMinWindowWidth;
        lpMMI->ptMinTrackSize.y = g_unMinWindowHeight;
    }

    CWnd::OnGetMinMaxInfo(lpMMI);
}