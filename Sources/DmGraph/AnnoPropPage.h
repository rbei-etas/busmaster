/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      AnnoPropPage.h: Declaration of the CAnnoPropPage class
//
// CLASS NAME
//      CAnnoPropPage
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila major re-design of old CAnnoPropPage
//

#ifndef __ANNOPROPPAGE_H_
#define __ANNOPROPPAGE_H_

#include "resource.h"       // main symbols
#include "ColPickerBtn.h"
#include "PropPageBaseWnd.h"

EXTERN_C const CLSID CLSID_AnnoPropPage;

/////////////////////////////////////////////////////////////////////////////
// CAnnoPropPage
class ATL_NO_VTABLE CAnnoPropPage :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CAnnoPropPage, &CLSID_AnnoPropPage>,
    public IPropertyPageImpl<CAnnoPropPage>,
    public CDialogImpl<CAnnoPropPage, CPropPageBaseWnd>
{
public:
    CAnnoPropPage()
    {
        m_dwTitleID = IDS_TITLEAnnoPropPage;
        m_dwHelpFileID = IDS_HELPFILEAnnoPropPage;
        m_dwDocStringID = IDS_DOCSTRINGAnnoPropPage;
        m_bInited = FALSE;
    }

    enum {IDD = IDD_ANNOPROPPAGE};

    DECLARE_REGISTRY_RESOURCEID(IDR_ANNOPROPPAGE)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CAnnoPropPage)
    COM_INTERFACE_ENTRY(IPropertyPage)
    END_COM_MAP()

protected:
    BEGIN_MSG_MAP(CAnnoPropPage)
    CHAIN_MSG_MAP(IPropertyPageImpl<CAnnoPropPage>)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDC_ADDBUTTON, BN_CLICKED, OnClickedAddbutton)
    COMMAND_HANDLER(IDC_DELBUTTON, BN_CLICKED, OnClickedDelbutton)
    COMMAND_HANDLER(IDC_REMOVE_SEL, BN_CLICKED, OnClickedRemoveSelbutton)
    NOTIFY_HANDLER(IDC_COLLECTION_LIST, LVN_ITEMCHANGED, OnItemchangedElement_list)
    NOTIFY_HANDLER(IDC_COLLECTION_LIST, LVN_GETDISPINFO, OnGetdispinfoElement_list)
    COMMAND_HANDLER(IDC_CHECK_VISIBLE, BN_CLICKED, OnClickedCheck_visible)
    COMMAND_HANDLER(IDC_COMBO_ORIENTATION, CBN_CLOSEUP, OnCloseupCombo_orientation)
    COMMAND_HANDLER(IDC_EDIT_CAPTION, EN_CHANGE, OnChangeEdit_caption)
    COMMAND_HANDLER(IDC_EDIT_X, EN_CHANGE, OnChangeEdit_x)
    COMMAND_HANDLER(IDC_EDIT_Y, EN_CHANGE, OnChangeEdit_y)
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
    LRESULT OnChangeEdit_caption(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnChangeEdit_x(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnChangeEdit_y(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnCloseupCombo_orientation(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnClickedCheck_visible(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

    STDMETHOD(Help)(LPCOLESTR pszHelpDir);

    STDMETHOD(Apply)(void)
    {
        return S_OK;
    }
private:
    CColPickerBtn   m_btnLabelTxtColPicker,
                    m_btnLabelBkColPicker;
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

#endif //__ANNOPROPPAGE_H_
