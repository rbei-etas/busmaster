#if !defined(AFX_BASEDIALOG_H__DF4DE489_4474_4759_A14E_EB3FF0CDFBDA__INCLUDED_)
#define AFX_BASEDIALOG_H__DF4DE489_4474_4759_A14E_EB3FF0CDFBDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

class CResizableDialog : public CDialog
{
// Construction
public:
    CResizableDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor

    void AutoMove(int iID, double dXMovePct, double dYMovePct, double dXSizePct, double dYSizePct);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CResizableDialog)
protected:
    //}}AFX_VIRTUAL

protected:
    //{{AFX_MSG(CResizableDialog)
    virtual BOOL OnInitDialog();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

public:
    bool            m_bShowGripper;         // ignored if not WS_THICKFRAME

private:
    struct SMovingChild
    {
        HWND        m_hWnd;
        double      m_dXMoveFrac;
        double      m_dYMoveFrac;
        double      m_dXSizeFrac;
        double      m_dYSizeFrac;
        CRect       m_rcInitial;
    };
    typedef std::vector<SMovingChild>   MovingChildren;

    MovingChildren  m_MovingChildren;
    CSize           m_szInitial;
    CSize           m_szMinimum;
    HWND            m_hGripper;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEDIALOG_H__DF4DE489_4474_4759_A14E_EB3FF0CDFBDA__INCLUDED_)
