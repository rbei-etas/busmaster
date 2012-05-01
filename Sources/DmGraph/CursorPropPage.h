/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      CursorPropPage.h: Declaration of the CCursorPropPage class
//
// CLASS NAME
//      CCursorPropPage
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila major re-design of old CCursorPropPage
//

#ifndef __CURSORPROPPAGE_H_
#define __CURSORPROPPAGE_H_

#include "resource.h"       // main symbols
#include "ColPickerBtn.h"
#include "PropPageBaseWnd.h"

EXTERN_C const CLSID CLSID_CursorPropPage;

/////////////////////////////////////////////////////////////////////////////
// CCursorPropPage
class ATL_NO_VTABLE CCursorPropPage :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CCursorPropPage, &CLSID_CursorPropPage>,
    public IPropertyPageImpl<CCursorPropPage>,
    public CDialogImpl<CCursorPropPage, CPropPageBaseWnd>
{
public:
    CCursorPropPage()
    {
        m_dwTitleID = IDS_TITLECursorPropPage;
        m_dwHelpFileID = IDS_HELPFILECursorPropPage;
        m_dwDocStringID = IDS_DOCSTRINGCursorPropPage;
        m_bInited = FALSE;
    }

    enum {IDD = IDD_CURSORPROPPAGE};

    DECLARE_REGISTRY_RESOURCEID(IDR_CURSORPROPPAGE)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CCursorPropPage)
    COM_INTERFACE_ENTRY(IPropertyPage)
    END_COM_MAP()

    BEGIN_MSG_MAP(CCursorPropPage)
    CHAIN_MSG_MAP(IPropertyPageImpl<CCursorPropPage>)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDC_ADDBUTTON, BN_CLICKED, OnClickedAddbutton)
    COMMAND_HANDLER(IDC_DELBUTTON, BN_CLICKED, OnClickedDelbutton)
    COMMAND_HANDLER(IDC_REMOVE_SEL, BN_CLICKED, OnClickedRemoveSelbutton)
    NOTIFY_HANDLER(IDC_COLLECTION_LIST, LVN_ITEMCHANGED, OnItemchangedElement_list)
    NOTIFY_HANDLER(IDC_COLLECTION_LIST, LVN_GETDISPINFO, OnGetdispinfoElement_list)
    COMMAND_HANDLER(IDC_EDIT_X, EN_CHANGE, OnChangeEdit_x)
    COMMAND_HANDLER(IDC_EDIT_Y, EN_CHANGE, OnChangeEdit_y)
    COMMAND_HANDLER(IDC_EDIT_NAME, EN_CHANGE, OnChangeEdit_name)
    COMMAND_HANDLER(IDC_CHECK_VISIBLE, BN_CLICKED, OnClickedCheck_visible)
    COMMAND_HANDLER(IDC_COMBO_STYLE, CBN_CLOSEUP, OnCloseupCombo_style)
    COMMAND_HANDLER(IDC_COMBO_SNAP, CBN_CLOSEUP, OnCloseupCombo_snap)
    MESSAGE_HANDLER(WM_COL_CHANGED, OnColChanged)
    REFLECT_NOTIFICATIONS()
    END_MSG_MAP()
    // Handler prototypes:
    //  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    //  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

protected:
    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnColChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnClickedAddbutton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnClickedDelbutton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnClickedRemoveSelbutton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnItemchangedElement_list(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
    LRESULT OnGetdispinfoElement_list(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
    LRESULT OnChangeEdit_x(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnChangeEdit_y(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnChangeEdit_name(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnClickedCheck_visible(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnCloseupCombo_style(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnCloseupCombo_snap(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

    STDMETHOD(Help)(LPCOLESTR pszHelpDir);

    STDMETHOD(Apply)(void)
    {
        return S_OK;
    }
private:
    CColPickerBtn   m_btnColPicker;
    BOOL            m_bInited; // TRUE after WM_INITDIALOG was processed

    HRESULT GetGraphCtrl(IDMGraphCtrl** ppGraphCtrl)
    {
        ATLASSERT(ppGraphCtrl);

        if(m_nObjects == 0)
        {
            return E_FAIL;
        }

        //We use just first object (we should have only one)
        // I'm assuming there is but one control, ours.
        // This is a pretty straightforward assumption
        // Most containers don’t even support multi-control
        // selection of custom property pages, yet.
        return m_ppUnk[0]->QueryInterface(IID_IDMGraphCtrl, (void**)ppGraphCtrl);
    }
    void UpdateControls(IDMGraphCtrl* pGraphCtrl);
};

#endif //__CURSORPROPPAGE_H_
