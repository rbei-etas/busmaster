/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//		DMGraphPropPage.h: Declaration of the CDMGraphPropPage class
//
// CLASS NAME
//		CDMGraphPropPage
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila major re-design of old CDMGraphPropPage
//

#ifndef __DMGRAPHPROPPAGE_H_
#define __DMGRAPHPROPPAGE_H_

#include "resource.h"       // main symbols
#include "ColPickerBtn.h"

EXTERN_C const CLSID CLSID_DMGraphPropPage;

/////////////////////////////////////////////////////////////////////////////
// CDMGraphPropPage
class ATL_NO_VTABLE CDMGraphPropPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDMGraphPropPage, &CLSID_DMGraphPropPage>,
	public IPropertyPageImpl<CDMGraphPropPage>,
	public CDialogImpl<CDMGraphPropPage>
{
public:
	CDMGraphPropPage() 
	{
		m_dwTitleID = IDS_TITLEDMGraphPropPage;
		m_dwHelpFileID = IDS_HELPFILEDMGraphPropPage;
		m_dwDocStringID = IDS_DOCSTRINGDMGraphPropPage;
		m_bInited = FALSE;
	}

	enum {IDD = IDD_DMGRAPHPROPPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_DMGRAPHPROPPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDMGraphPropPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CDMGraphPropPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CDMGraphPropPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_CAPTION, EN_CHANGE, OnChangeCaption)
	COMMAND_HANDLER(IDC_CHECK_SHOWGRID_X, BN_CLICKED, OnClickedCheck_showgridX)
	COMMAND_HANDLER(IDC_CHECK_SHOWGRID_Y, BN_CLICKED, OnClickedCheck_showgridY)
	COMMAND_HANDLER(IDC_CHECK_XLOG, BN_CLICKED, OnClickedCheck_xlog)
	COMMAND_HANDLER(IDC_CHECK_YLOG, BN_CLICKED, OnClickedCheck_ylog)
	COMMAND_HANDLER(IDC_COMBO_FRAME, CBN_CLOSEUP, OnCloseupCombo_frame)
	COMMAND_HANDLER(IDC_COMBO_MODE, CBN_CLOSEUP, OnCloseupCombo_mode)
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
	LRESULT OnChangeCaption(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCheck_showgridX(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCheck_showgridY(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCheck_xlog(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCheck_ylog(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCloseupCombo_frame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCloseupCombo_mode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	STDMETHOD(Help)(LPCOLESTR pszHelpDir);

	STDMETHOD(Apply)(void)
	{
		return S_OK;
	}
private:
	CColPickerBtn	m_btnFrameColPicker,
					m_btnPlotColPicker,
					m_btnAxisColPicker,
					m_btnGridColPicker,
					m_btnLabelColPicker;
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
	void UpdateControls(IDMGraphCtrl* pGraphCtrl);
};

#endif //__DMGRAPHPROPPAGE_H_
