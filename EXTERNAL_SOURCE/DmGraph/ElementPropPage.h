/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//		ElementPropPage.h: Declaration of the CElementPropPage class
//
// CLASS NAME
//		CElementPropPage
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila major re-design of old CElementPropPage
//

#ifndef __ELEMENTPROPPAGE_H_
#define __ELEMENTPROPPAGE_H_

#include "resource.h"       // main symbols
#include "ColPickerBtn.h"
#include "PropPageBaseWnd.h"

EXTERN_C const CLSID CLSID_ElementPropPage;

/////////////////////////////////////////////////////////////////////////////
// CElementPropPage
class ATL_NO_VTABLE CElementPropPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CElementPropPage, &CLSID_ElementPropPage>,
	public IPropertyPageImpl<CElementPropPage>,
	public CDialogImpl<CElementPropPage, CPropPageBaseWnd>
{
public:
	CElementPropPage() 
	{
		m_dwTitleID = IDS_TITLEElementPropPage;
		m_dwHelpFileID = IDS_HELPFILEElementPropPage;
		m_dwDocStringID = IDS_DOCSTRINGElementPropPage;
		m_bInited = FALSE;
	}

	enum {IDD = IDD_ELEMENTPROPPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_ELEMENTPROPPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CElementPropPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CElementPropPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CElementPropPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_ADDBUTTON, BN_CLICKED, OnClickedAddbutton)
	COMMAND_HANDLER(IDC_DELBUTTON, BN_CLICKED, OnClickedDelbutton)
	COMMAND_HANDLER(IDC_REMOVE_SEL, BN_CLICKED, OnClickedRemoveSelbutton)
	NOTIFY_HANDLER(IDC_COLLECTION_LIST, LVN_ITEMCHANGED, OnItemchangedElement_list)
	NOTIFY_HANDLER(IDC_COLLECTION_LIST, LVN_GETDISPINFO, OnGetdispinfoElement_list)
	COMMAND_HANDLER(IDC_LINE_WIDTH, EN_CHANGE, OnChangeLine_width)
	COMMAND_HANDLER(IDC_POINT_WIDTH, EN_CHANGE, OnChangePoint_width)
	COMMAND_HANDLER(IDC_CHECK_SOLID, BN_CLICKED, OnClickedCheck_solid)
	COMMAND_HANDLER(IDC_CHECK_VISIBLE, BN_CLICKED, OnClickedCheck_visible)
	COMMAND_HANDLER(IDC_COMBO_SYMBOL, CBN_CLOSEUP, OnCloseupCombo_symbol)
	COMMAND_HANDLER(IDC_COMBO_TYPE, CBN_CLOSEUP, OnCloseupCombo_type)
	COMMAND_HANDLER(IDC_EDIT_NAME, EN_CHANGE, OnChangeEdit_name)
	MESSAGE_HANDLER(WM_COL_CHANGED, OnColChanged)
	NOTIFY_HANDLER(IDC_SPIN_LINE_WIDTH, UDN_DELTAPOS, OnDeltaposSpin_line_width)
	NOTIFY_HANDLER(IDC_SPIN_POINT_WIDTH, UDN_DELTAPOS, OnDeltaposSpin_point_width)
	REFLECT_NOTIFICATIONS()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

protected:
	STDMETHOD(Apply)(void)
	{
		return S_OK;
	}
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnColChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedAddbutton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedDelbutton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedRemoveSelbutton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnItemchangedElement_list(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnGetdispinfoElement_list(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnChangeLine_width(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangePoint_width(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDeltaposSpin_line_width(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDeltaposSpin_point_width(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnClickedCheck_solid(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCheck_visible(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCloseupCombo_symbol(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCloseupCombo_type(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeEdit_name(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	STDMETHOD(Help)(LPCOLESTR pszHelpDir);

private:
	CColPickerBtn	m_btnLineColPicker,
					m_btnPointColPicker;
	BOOL			m_bInited; // TRUE after WM_INITDIALOG was processed

	HRESULT GetGraphCtrl(IDMGraphCtrl** ppGraphCtrl)
	{
		ATLASSERT(ppGraphCtrl);
		if(m_nObjects == 0)
			return E_FAIL;
		//We use just first object (we should have only one)
	   // I'm assuming there is but one control, ours.
	   // This is a pretty straightforward assumption
	   // Most containers don’t even support multi-control
	   // selection of custom property pages, yet.
		return m_ppUnk[0]->QueryInterface(IID_IDMGraphCtrl, (void**)ppGraphCtrl);
	}

	HRESULT GetCurrentElement(IDMGraphCtrl* pGraphCtrl, IDMGraphElement** ppGraphElement);
	void UpdateControls(IDMGraphCtrl* pGraphCtrl);
};

#endif //__ELEMENTPROPPAGE_H_
