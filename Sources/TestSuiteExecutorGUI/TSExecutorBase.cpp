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
 * \file      TSExecutorBase.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "TSExecutorGUI_StdAfx.h"
#include "TSExecutorBase.h"

/******************************************************************************
Function Name  :  CTSExecutorBase
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CTSExecutorBase
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
CTSExecutorBase::CTSExecutorBase(void)
{
    m_pomResultDispaly = NULL;
}

/******************************************************************************
Function Name  :  ~CTSExecutorBase
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CTSExecutorBase
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
CTSExecutorBase::~CTSExecutorBase(void)
{
}

/******************************************************************************
Function Name  :  TSX_SetDisplayWnd
Input(s)       :  CListCtrl* pomResultDispaly - List Control display
Output         :  HRESULT
Functionality  :  Sets the display list control
Member of      :  CTSExecutorBase
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorBase::TSX_SetDisplayWnd(CListCtrl* pomResultDispaly)
{
    if(pomResultDispaly != NULL)
    {
        m_pomResultDispaly = pomResultDispaly;
        return S_OK;
    }

    return S_FALSE;
}

/******************************************************************************
Function Name  :  TSX_DisplayMessage
Input(s)       :  CString& omStrMsg - Display Message
Output         :  HRESULT
Functionality  :  Sisplays the Message in new line of list control
Member of      :  CTSExecutorBase
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorBase::TSX_DisplayMessage(CString& omStrMsg)
{
    if(m_pomResultDispaly != NULL)
    {
        int nCount = m_pomResultDispaly->GetItemCount();
        m_pomResultDispaly->InsertItem(nCount, "");
        m_pomResultDispaly->SetItemText(nCount, 1, omStrMsg);
        m_pomResultDispaly->EnsureVisible(nCount, FALSE);
        return S_OK;
    }

    return S_FALSE;
}

/******************************************************************************
Function Name  :  TSX_DisplayResult
Input(s)       :  CString& omStrResult
Output         :  HRESULT
Functionality  :  Displays the message in the result column of current row.
Member of      :  CTSExecutorBase
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorBase::TSX_DisplayResult(CString& omStrResult)
{
    if(m_pomResultDispaly != NULL)
    {
        int nCount = m_pomResultDispaly->GetItemCount();
        m_pomResultDispaly->SetItemText(nCount-1, 2, omStrResult);
        return S_OK;
    }

    return S_FALSE;
}