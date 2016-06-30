/*********************************************************************
* Multi-Column Tree View, version 1.4 (July 7, 2005)
* Copyright (C) 2003-2005 Michal Mecinski.
*
* You may freely use and modify this code, but don't remove
* this copyright note.
*
* THERE IS NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, FOR
* THIS CODE. THE AUTHOR DOES NOT TAKE THE RESPONSIBILITY
* FOR ANY DAMAGE RESULTING FROM THE USE OF IT.
*
* E-mail: mimec@mimec.org
* WWW: http://www.mimec.org
********************************************************************/

#pragma once

class CColumnTreeCtrl : public CTreeCtrl
{
    friend class CColumnTreeView;
    friend class CColumnTreeWnd;

public:
    CColumnTreeCtrl();
    virtual ~CColumnTreeCtrl();

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

protected:
    void HandleMouse(UINT message, UINT nFlags, CPoint point);

    int m_cxFirstCol;
    int m_cxTotal;

    DECLARE_MESSAGE_MAP()

private:
    DECLARE_DYNAMIC(CColumnTreeCtrl)
};
