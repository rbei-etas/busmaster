/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      TreeItemParam.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "StdAfx.h"
#include "TreeItemParam.h"
//#include "GettextBusmaster.h"
#include "Utility\MultiLanguageSupport.h"
UINT CTreeItemParam::m_unID = 0x500;

CTreeItemParam::CTreeItemParam(void)
{
    m_aomItemHandle.SetSize(30, 1);
}

CTreeItemParam::~CTreeItemParam()
{
    vRemoveTreeCtrl();
}

CTreeItemParam::CTreeItemParam(CWnd* pomParent,
                               CStringArray& omStringArray,
                               RECT& Rect,
                               COLORREF TextColour)
{
    HTREEITEM ItemHandle;
    int nCount = omStringArray.GetSize();
    m_omRect = Rect;
    //BOOL bCreate =
    m_odTreeCtrl.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | TVS_HASLINES |
                        TVS_LINESATROOT | TVS_SHOWSELALWAYS | WS_CLIPSIBLINGS,
                        m_omRect, pomParent, ++m_unID);
    m_TextColour = TextColour;
    m_odTreeCtrl.SetTextColor(m_TextColour);

    for (int i = 0; i < nCount; i++)
    {
        CString omCurrStr = omStringArray.ElementAt(i);
        ItemHandle =  m_odTreeCtrl.InsertItem(omCurrStr);
        if (ItemHandle != nullptr)
        {
            m_aomItemHandle.Add(ItemHandle);
        }
    }
    VERIFY(m_omFont.CreateFont(
               14,                        // nHeight
               0,                         // nWidth
               0,                         // nEscapement
               0,                         // nOrientation
               FW_LIGHT,                 // nWeight
               FALSE,                     // bItalic
               FALSE,                     // bUnderline
               0,                         // cStrikeOut
               ANSI_CHARSET,              // nCharSet
               OUT_DEFAULT_PRECIS,        // nOutPrecision
               CLIP_DEFAULT_PRECIS,       // nClipPrecision
               DEFAULT_QUALITY,           // nQuality
               DEFAULT_PITCH | FF_ROMAN,  // nPitchAndFamily
               _T(_("Courier New"))));

    m_odTreeCtrl.SetFont(&m_omFont, TRUE);
}

void CTreeItemParam::vUpdateTreeValues(CStringArray& omStringArray, COLORREF TextColour)
{
    CString omCurrStr;
    int nCount = omStringArray.GetSize();
    if(nCount != m_aomItemHandle.GetSize())
    {
        m_aomItemHandle.RemoveAll();
        m_odTreeCtrl.DeleteAllItems();
        HTREEITEM ItemHandle = nullptr;
        for (int i = 0; i < nCount; i++)
        {
            omCurrStr = omStringArray.ElementAt(i);
            ItemHandle=  m_odTreeCtrl.InsertItem(omCurrStr);
            if (ItemHandle != nullptr)
            {
                m_aomItemHandle.Add(ItemHandle);
            }
        }

        m_odTreeCtrl.SetRedraw(TRUE);

        VERIFY(m_omFont.CreateFont(
                   14,                        // nHeight
                   0,                         // nWidth
                   0,                         // nEscapement
                   0,                         // nOrientation
                   FW_LIGHT,                 // nWeight
                   FALSE,                     // bItalic
                   FALSE,                     // bUnderline
                   0,                         // cStrikeOut
                   ANSI_CHARSET,              // nCharSet
                   OUT_DEFAULT_PRECIS,        // nOutPrecision
                   CLIP_DEFAULT_PRECIS,       // nClipPrecision
                   DEFAULT_QUALITY,           // nQuality
                   DEFAULT_PITCH | FF_ROMAN,  // nPitchAndFamily
                   _T(_("Courier New"))));

        m_odTreeCtrl.SetFont(&m_omFont, TRUE);
    }
    if (TextColour != m_TextColour)
    {
        m_TextColour = TextColour;
        m_odTreeCtrl.SetTextColor(m_TextColour);
    }
    HTREEITEM ItemHandle = nullptr;

    if (nCount > m_aomItemHandle.GetSize())
    {
        ASSERT(FALSE);
    }
    omCurrStr = "";
    for (int i = 0; i < nCount; i++)
    {
        omCurrStr = omStringArray.ElementAt(i);
        ItemHandle = nullptr;

        if(m_aomItemHandle.GetSize() > i)
        {
            ItemHandle = m_aomItemHandle.GetAt(i);
        }
        if (ItemHandle != nullptr)
        {
            m_odTreeCtrl.SetItemText(ItemHandle, omCurrStr.GetBuffer(MAX_PATH));
        }
    }
}

void CTreeItemParam::vShift(LONG lExtent)
{
    m_omRect.top += lExtent;
    m_omRect.bottom += lExtent;

    m_odTreeCtrl.MoveWindow(&m_omRect);
}

void CTreeItemParam::vRemoveTreeCtrl(void)
{
    m_aomItemHandle.RemoveAll();
    m_odTreeCtrl.DeleteAllItems();
    if (m_odTreeCtrl.m_hWnd != nullptr)
    {
        m_odTreeCtrl.DestroyWindow();
    }
}

void CTreeItemParam::vShiftWindowTop(LONG lExtent)
{
    int nShift = lExtent - m_omRect.top;
    vShift(nShift);
}

void CTreeItemParam::vUpdateTreeItemRect(RECT& rcNewPos)
{
    m_omRect = rcNewPos;
    m_odTreeCtrl.MoveWindow(&m_omRect);
}