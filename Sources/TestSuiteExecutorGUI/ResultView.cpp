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
 * \file      ResultView.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "TSExecutorGUI_stdafx.h"
#include "ResultView.h"

IMPLEMENT_DYNCREATE(CResultView, CListView)

BEGIN_MESSAGE_MAP(CResultView, CListView)
END_MESSAGE_MAP()

// CResultView diagnostics

#ifdef _DEBUG
void CResultView::AssertValid() const
{
    CListView::AssertValid();
}

void CResultView::Dump(CDumpContext& dc) const
{
    CListView::Dump(dc);
}
#endif //_DEBUG

/******************************************************************************
Function Name  :  CResultView
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CResultView
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
CResultView::CResultView(void)
{
}

/******************************************************************************
Function Name  :  ~CResultView
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CResultView
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
CResultView::~CResultView(void)
{
}


/******************************************************************************
Function Name  :  PreCreateWindow
Input(s)       :  CREATESTRUCT& cs
Output         :  BOOL
Functionality  :
Member of      :  CResultView
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
BOOL CResultView::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style = cs.style|LVS_REPORT;
    return CListView::PreCreateWindow(cs);
}

/******************************************************************************
Function Name  :  OnInitialUpdate
Input(s)       :  -
Output         :  void
Functionality  :
Member of      :  CResultView
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
void CResultView::OnInitialUpdate(void)
{
    CListView::OnInitialUpdate();
    //Just To increase the List Item Height
    CRect omRect;
    GetListCtrl().GetWindowRect(&omRect);
    m_omImageList.Create(1, 16, ILC_COLOR24, 4, 1);
    GetListCtrl().SetImageList(&m_omImageList, LVSIL_SMALL);
}
