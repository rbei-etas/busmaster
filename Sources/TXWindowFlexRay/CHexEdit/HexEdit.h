#pragma once
#include "afxwin.h"

class CHexEdit : public CEdit
{
public:
    CHexEdit(void);
    ~CHexEdit(void);

    void ResetColors(COLORREF,COLORREF);

    //Anupam Kumar
    void SetMaxChars(int nMax = -1);
    void SetValidChar(char* pChar);
    int  CountNoOfChars();
    bool SetToolTipText(const CString& sText, bool bActivate = true);
    bool ActivateToolTip(bool bEnable = true);
    bool GetTextFont( LOGFONT* plgfnt);
    bool GetTextClrFocus(COLORREF& clr);
    bool SetTextClrFocus( COLORREF clr );
    bool SetTextFont( LONG nHeight, bool bBold, bool bItalic, const CString& sFaceName );
    bool SetTextFont( const LOGFONT& lgfnt );
    bool SetFontBold( bool bBold = true);
    bool SetFontItalic( bool bItalic = true);
    bool SetFontHeight( int nHeight );
    bool SetFontFaceName( const CString& sFaceName );

private:
    bool InitToolTip();

    CBrush* m_brush;
    COLORREF m_textcolor, m_backcolor, m_clrTextFocus;
    CToolTipCtrl m_tt;
    CFont m_fntText;

    //Anupam
    int m_nMaxChar;     // -1 unused, > 0 leaves the field automatically
    char* m_nValidChar; // nullptr or a array of valid keys, eg. "0123456789" fo numeric only !

    // Generated message map functions
protected:
    //{{AFX_MSG(CCellEdit)
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg HBRUSH CHexEdit::CtlColor(CDC* pDC, UINT nCtlColor);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};
