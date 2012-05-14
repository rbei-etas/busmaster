#pragma once


// CColorSelector
#include "afxwin.h"
#include "afxdlgs.h"

#define WM_CPN_COLORSELECTED             WM_USER + 55 

class CColorSelector : public CButton
{
	//DECLARE_DYNAMIC(CColorSelector)
    COLORREF m_omColorBkg;
public:
	CColorSelector();
	virtual ~CColorSelector();
    COLORREF GetColour(){return m_omColorBkg;};
    void SetColour(COLORREF omColor);
protected:
	DECLARE_MESSAGE_MAP()
    virtual void PreSubclassWindow();
public:
    afx_msg virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnBnClicked();
};


