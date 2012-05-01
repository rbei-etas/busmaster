/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      FormatPropPage.h: Declaration of the CFormatPropPage class
//
// CLASS NAME
//      CFormatPropPage
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila major re-design of old CFormatPropPage
//

#ifndef __FORMATPROPPAGE_H_
#define __FORMATPROPPAGE_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_FormatPropPage;

/////////////////////////////////////////////////////////////////////////////
// CFormatPropPage
class ATL_NO_VTABLE CFormatPropPage :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CFormatPropPage, &CLSID_FormatPropPage>,
    public IPropertyPageImpl<CFormatPropPage>,
    public CDialogImpl<CFormatPropPage>
{
public:
    CFormatPropPage()
    {
        m_dwTitleID = IDS_TITLEFormatPropPage;
        m_dwHelpFileID = IDS_HELPFILEFormatPropPage;
        m_dwDocStringID = IDS_DOCSTRINGFormatPropPage;
        m_bInited = FALSE;
    }

    enum {IDD = IDD_FORMATPROPPAGE};

    DECLARE_REGISTRY_RESOURCEID(IDR_FORMATPROPPAGE)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CFormatPropPage)
    COM_INTERFACE_ENTRY(IPropertyPage)
    END_COM_MAP()

    BEGIN_MSG_MAP(CFormatPropPage)
    CHAIN_MSG_MAP(IPropertyPageImpl<CFormatPropPage>)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDC_EDIT_FORMAT, EN_CHANGE, OnChangeEdit_format)
    COMMAND_HANDLER(IDC_BUTTON_DEFAULT, BN_CLICKED, OnClickedButton_default)
    COMMAND_HANDLER(IDC_COMBO_TYPE, CBN_SELCHANGE, OnSelChangeCombo_type)
    COMMAND_HANDLER(IDC_COMBO_AXIS, CBN_SELCHANGE, OnSelChangeCombo_axis)
    COMMAND_HANDLER(IDC_FORMAT_LIST, LBN_SELCHANGE, OnSelchangeFormat_list)
    END_MSG_MAP()
    // Handler prototypes:
    //  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    //  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

protected:
    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnChangeEdit_format(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnClickedButton_default(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnSelChangeCombo_type(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnSelChangeCombo_axis(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnSelchangeFormat_list(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

    STDMETHOD(Help)(LPCOLESTR pszHelpDir);

    STDMETHOD(Apply)(void)
    {
        return S_OK;
    }
private:
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
    HRESULT GetSelectedAxis(IDMGraphCtrl* pGraphCtrl, IDMGraphAxis** ppAxis);
    void UpdateControls(IDMGraphCtrl* pGraphCtrl);
    void SetAxisTimeFormat(VARIANT_BOOL b);
};

#endif //__FORMATPROPPAGE_H_
