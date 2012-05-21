/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      ColPickerBtn.h: Declaration of the CColPickerBtn class
//
// CLASS NAME
//      CColPickerBtn
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila major re-design of old ColourPicker
//

#ifndef __COL_PICKER_BTN__H__
#define __COL_PICKER_BTN__H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//sent to the parent when color was changed
//wParam: COLORREF
//lParam: ID of this control
#define WM_COL_CHANGED (WM_USER+1000)

/////////////////////////////////////////////////////////////////////////////
// CColPickerBtn window


class CColPickerBtn : public CWindowImpl<CColPickerBtn>
{
    // Construction
public:
    CColPickerBtn();

protected:
    BEGIN_MSG_MAP(CAboutDlg)
    MESSAGE_HANDLER(OCM_COMMAND, OnCommand)
    MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem)
    DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

    // Attributes
public:
    COLORREF GetColor() const
    {
        return m_col;
    }

    void     SetColor(COLORREF crColour)
    {
        m_col = crColour;
        Invalidate(FALSE);
    }

    LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
    COLORREF m_col;
};


#endif // __COL_PICKER_BTN__H__
