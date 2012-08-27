#pragma once


// CColorSelector
#include "afxwin.h"
#include "afxdlgs.h"

#define WM_CPN_COLORSELECTED             WM_USER + 55

class CColorSelector : public CButton
{
    //DECLARE_DYNAMIC(CColorSelector)
    COLORREF m_omColorBkg;
	INT m_nSelIndex;

public:
    CColorSelector();
    virtual ~CColorSelector();
    COLORREF GetColour()
    {
        return m_omColorBkg;
    };
    void SetColour(COLORREF omColor);

	void SetSelectedIndex(INT nIndex);

	CMap<UINT,UINT, COLORREF, COLORREF> m_MapMsgIdToClr;
	
	COLORREF GetColourForSelectedIndex(INT nSelectedIndex);

protected:
    DECLARE_MESSAGE_MAP()
    virtual void PreSubclassWindow();
public:
    afx_msg virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnBnClicked();
};


