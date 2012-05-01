/**
 * \file      memdc.h
 * \author    Keith Rule
  */

#if !defined MEMDC_H__INCLUDED_
#define MEMDC_H__INCLUDED_

//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-2002, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History - 10/3/97 Fixed scrolling bug.
//               Added print support. - KR
//
//       11/3/99 Fixed most common complaint. Added
//            background color fill. - KR
//
//       11/3/99 Added support for mapping modes other than
//            MM_TEXT as suggested by Lee Sang Hun. - KR
//
//       02/11/02 Added support for CScrollView as supplied
//             by Gary Kirkham. - KR
//
// This class implements a memory Device Context which allows

// flicker free drawing.
//Code Project Reference: http://www.codeproject.com/KB/GDI/flickerfree.aspx?msg=1737391


class CMemDC : public CDC
{
private:
    CBitmap*    m_bitmap;
    CBitmap*    m_oldBitmap;
    CDC*        m_pDC;
    CRect       m_rcBounds;
public:
    CMemDC(CDC* pDC, const CRect& rcBounds) : CDC()
    {
        /*
        CreateCompatibleDC(pDC);
        m_bitmap = new CBitmap;
        m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
        m_oldBitmap = SelectObject(m_bitmap);
        m_pDC = pDC;
        m_rcBounds = rcBounds;
        */
        CreateCompatibleDC(pDC);
        m_bitmap = new CBitmap;
        BOOL bSuccess = m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());

        if (bSuccess == TRUE)
        {
            m_oldBitmap = SelectObject(m_bitmap);
        }
        else
        {
            // Delete the created DC
            DeleteDC();
            // Use the Screen DC directly
            m_hDC = pDC->m_hDC;
            // Copy m_hAttribDC
            // This data member contains the attribute device context for this
            // CDC object
            m_hAttribDC = pDC->m_hAttribDC;
        }

        m_pDC = pDC;
        m_rcBounds = rcBounds;
    }
    ~CMemDC()
    {
        m_pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(),
                      this, m_rcBounds.left, m_rcBounds.top, SRCCOPY);
        SelectObject(m_oldBitmap);

        if (m_bitmap != NULL)
        {
            delete m_bitmap;
        }
    }
    CMemDC* operator->()
    {
        return this;
    }
};

#endif // MEMDC_H__INCLUDED_
