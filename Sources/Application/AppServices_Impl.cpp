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
 * \file      AppServices_Impl.cpp
 * \brief     Concrete class source for the application service layer
 * \authors   Ratnadip Choudhury, Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Concrete class source for the application service layer
 */

#include "StdAfx.h"
#include "include/Error.h"

#include "TraceWnd/UIThread.h"
#include "AppServices_Impl.h"


/*******************************************************************************
  Function Name  : CAppServices_Impl
  Input(s)       : -
  Output         : -
  Functionality  : Constructor
  Member of      : CAppServices_Impl
  Author(s)      : Anish
  Date Created   : 09.2.2010
  Modifications  : 
*******************************************************************************/
CAppServices_Impl::CAppServices_Impl()
{
    m_podUIThread = NULL;
}


/*******************************************************************************
  Function Name  : ~CAppServices_Impl
  Input(s)       : -
  Output         : -
  Functionality  : Constructor
  Member of      : CAppServices_Impl
  Author(s)      : Anish
  Date Created   : 09.2.2010
  Modifications  : 
*******************************************************************************/
CAppServices_Impl::~CAppServices_Impl()
{
    m_podUIThread = NULL;
}


/*******************************************************************************
  Function Name  : bWriteToTrace
  Input(s)       : Character buffer to be written on trace
  Output         : -
  Functionality  : Write the string on trace window
  Member of      : CAppServices_Impl
  Author(s)      : Anish
  Date Created   : 09.2.2010
  Modifications  : 
*******************************************************************************/
BOOL CAppServices_Impl::bWriteToTrace(TCHAR* pcTraceStr)
{
    BOOL Result = FALSE;

    if (NULL != m_podUIThread)
    {
        CString omStr(pcTraceStr);
        m_podUIThread->vAddString(omStr);
        Result = TRUE;
    }
    return Result;
}
