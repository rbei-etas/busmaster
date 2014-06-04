#pragma once
#include "..\TxWindowFlexRay_stdafx.h"
#include "hexedit.h"

BEGIN_MESSAGE_MAP(CHexEdit, CEdit)
    //{{AFX_MSG_MAP(CCellEdit)
    ON_WM_CHAR()
    ON_WM_CTLCOLOR_REFLECT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CHexEdit::CHexEdit(void)
{
    m_brush = nullptr;
    m_nValidChar = nullptr;
    SetValidChar("0123456789abcdefABCDEF ");
}

CHexEdit::~CHexEdit(void)
{
    if (m_nValidChar)
    {
        delete [] m_nValidChar;
    }
    m_fntText.DeleteObject();
    if (m_brush != nullptr)
    {
        delete m_brush;
    }
}

void CHexEdit::SetValidChar(char* pChar)                             // Set the array with valid characters
{
    if (m_nValidChar)
    {
        delete [] m_nValidChar;
    }
    m_nValidChar = new char[_tcsclen(pChar) + 1];
    _tcscpy(m_nValidChar, pChar);
}

int CHexEdit::CountNoOfChars()
{
    CString s;
    GetWindowText(s);
    return(s.GetLength());
}

void CHexEdit::ResetColors(COLORREF newtext,COLORREF newback)
{
    m_textcolor = newtext;
    m_backcolor = newback;

    if (m_brush!=nullptr)
    {
        delete m_brush;
    }

    m_brush=new (CBrush);

    m_brush->CreateSolidBrush(m_backcolor);
    Invalidate(TRUE);
}

void CHexEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (!_istprint(nChar))
    {
        if (nChar == 13)
        {
            GetParent()->PostMessage(WM_KEYDOWN, nChar, 0);
        }
        else
        {
            CEdit::OnChar(nChar, nRepCnt, nFlags); // let the base class handle it
        }
    }
    else if (((m_nValidChar != nullptr)                    // do I have to check if the char is valid ?
              &&  (_tcschr(m_nValidChar,nChar) != nullptr)))
    {
        // is it a 'valid' character ?
        CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
    }
}

void CHexEdit::SetMaxChars(int nMax)
{
    m_nMaxChar = nMax;
    LimitText(nMax);
}

HBRUSH CHexEdit::CtlColor(CDC * pDC, UINT /* nCtlColor */)
{
    // TODO: Change any attributes of the DC here
    pDC->SetTextColor(m_textcolor);
    pDC->SetBkColor(m_backcolor);
    return *m_brush;
}


BOOL CHexEdit::PreTranslateMessage(MSG* pMsg)
{
    InitToolTip();
    m_tt.RelayEvent(pMsg);
    return CEdit::PreTranslateMessage(pMsg);
}

bool CHexEdit::InitToolTip()
{
    if (m_tt.m_hWnd == nullptr)
    {
        m_tt.Create(this);
        m_tt.Activate(true);
        m_tt.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
    }
    return true;
}

bool CHexEdit::SetTextFont( const LOGFONT& lgfnt )
{
    m_fntText.DeleteObject();
    m_fntText.CreateFontIndirect( &lgfnt );
    SetFont( &m_fntText, TRUE );
    return true;
}

bool CHexEdit::SetTextFont( LONG nHeight, bool bBold, bool bItalic, const CString& sFaceName)
{
    LOGFONT lgfnt;

    lgfnt.lfHeight          = -MulDiv(nHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY), 72);
    lgfnt.lfWidth           = 0;
    lgfnt.lfEscapement      = 0;
    lgfnt.lfOrientation     = 0;
    lgfnt.lfWeight          = bBold?FW_BOLD:FW_DONTCARE;
    lgfnt.lfItalic          = bItalic?TRUE:FALSE;
    lgfnt.lfUnderline       = FALSE;
    lgfnt.lfStrikeOut       = FALSE;
    lgfnt.lfCharSet         = DEFAULT_CHARSET;
    lgfnt.lfOutPrecision    = OUT_DEFAULT_PRECIS;
    lgfnt.lfClipPrecision   = CLIP_DEFAULT_PRECIS;
    lgfnt.lfQuality         = DEFAULT_QUALITY;
    lgfnt.lfPitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
    strcpy( lgfnt.lfFaceName, sFaceName );
    return SetTextFont( lgfnt );
    return true;
}


bool CHexEdit::SetFontBold( bool bBold )
{
    LOGFONT lgfnt;

    GetTextFont( &lgfnt );
    lgfnt.lfWeight  = bBold?FW_BOLD:FW_DONTCARE;

    SetTextFont( lgfnt );

    return true;
}


bool CHexEdit::SetFontItalic( bool bItalic )
{
    LOGFONT lgfnt;

    GetTextFont( &lgfnt );
    lgfnt.lfItalic  = bItalic ? TRUE : FALSE;

    SetTextFont( lgfnt );
    return true;
}

bool CHexEdit::SetFontHeight( int nHeight )
{
    LOGFONT lgfnt;
    GetTextFont( &lgfnt );
    lgfnt.lfHeight  = -MulDiv(nHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY), 72);
    lgfnt.lfWidth   = 0;
    SetTextFont( lgfnt );
    return true;
}

bool CHexEdit::SetFontFaceName( const CString& sFaceName )
{
    LOGFONT lgfnt;

    GetTextFont( &lgfnt );
    strcpy( lgfnt.lfFaceName, sFaceName );
    SetTextFont( lgfnt );
    return true;
}

bool CHexEdit::GetTextFont( LOGFONT* plgfnt)
{
    GetFont()->GetObject( sizeof(LOGFONT), (void*)plgfnt);
    return true;
}

bool CHexEdit::SetToolTipText(const CString& sText, bool bActivate)
{
    InitToolTip();
    // If there is no tooltip defined then add it
    if (m_tt.GetToolCount() == 0)
    {
        CRect rect;
        GetClientRect(rect);
        m_tt.AddTool(this, sText, rect, 1);
    }

    m_tt.UpdateTipText(sText, this, 1);
    m_tt.Activate(bActivate);

    return true;
}

bool CHexEdit::ActivateToolTip(bool bActivate)
{
    if (m_tt.GetToolCount() == 0)
    {
        return false;
    }

    // Activate tooltip
    m_tt.Activate(bActivate);

    return true;
}

bool CHexEdit::SetTextClrFocus(COLORREF clr)
{
    m_clrTextFocus = clr;
    Invalidate();

    return true;
}