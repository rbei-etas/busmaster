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
 * \file      SigGrphWndHandler.cpp
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "stdafx.h"
#include ".\SigGrphWndHandler.h"

//Function Pointer Declarations
typedef HRESULT (*CREATEGRAPHWINDOW)(CMDIFrameWnd* pParentWnd, short eBusType);
typedef BOOL (*ISWINDOWVISIBLE)(short eBusType);
typedef HRESULT (*SHOWGRAPHWINDOW)(short eBusType, BOOL bShow);
typedef HRESULT (*SETSIGNALLISTDETAILS)(short eBusType, CGraphList* pSignalList);
typedef HRESULT (*POSTMESSAGETOSGWND)(short eBusType, UINT msg, WPARAM wParam, LPARAM lParam);
typedef CMsgBufVSE* (*GETGRAPHBUFFER)();
typedef HRESULT (*GETWINDOWSPLITTERPOS)(short eBusType,
                                        WINDOWPLACEMENT& sWndPlacement,
                                        SGRAPHSPLITTERDATA& sGraphSplitter);
typedef HRESULT (*SETWINDOWSPLITTERPOS)(short eBusType,
                                        WINDOWPLACEMENT& sWndPlacement,
                                        SGRAPHSPLITTERDATA& sGraphSplitter);

CREATEGRAPHWINDOW       pfCreateGraphWindow;
ISWINDOWVISIBLE         pfIsWindowVisible;
SHOWGRAPHWINDOW         pfShowGraphWindow;
SETSIGNALLISTDETAILS    pfSetSignalListDetails;
POSTMESSAGETOSGWND      pfPostMessageToSGWnd;
GETGRAPHBUFFER          pfGetGraphBuffer;
GETWINDOWSPLITTERPOS    pfGetWindowSplitterPos;
SETWINDOWSPLITTERPOS    pfSetWindowSplitterPos;

/*******************************************************************************
  Function Name  : CSigGrphHandler
  Input(s)       : -
  Output         : -
  Functionality  : Constructor
  Member of      : CSigGrphHandler
  Author(s)      : ArunKumar K
  Date Created   : 26.10.2010
  Modifications  :
*******************************************************************************/
CSigGrphHandler::CSigGrphHandler(void)
{
    m_hSigGrphHandle = NULL;
}

/*******************************************************************************
  Function Name  : ~CSigGrphHandler
  Input(s)       : -
  Output         : -
  Functionality  : Destructor
  Member of      : CSigGrphHandler
  Author(s)      : ArunKumar K
  Date Created   : 26.10.2010
  Modifications  :
*******************************************************************************/
CSigGrphHandler::~CSigGrphHandler(void)
{
    if ( m_hSigGrphHandle != NULL )
    {
        FreeLibrary(m_hSigGrphHandle);
    }
}

/*******************************************************************************
  Function Name  : vLoadSigGrph_DLL
  Input(s)       : -
  Output         : -
  Functionality  : Load the TxWindow DLL
  Member of      : CSigGrphHandler
  Author(s)      : ArunKumar K
  Date Created   : 26.10.2010
  Modifications  :
*******************************************************************************/
void CSigGrphHandler::vLoadSigGrph_DLL()
{
    if ( m_hSigGrphHandle != NULL )
    {
        FreeLibrary(m_hSigGrphHandle);
        m_hSigGrphHandle = NULL;
    }

    m_hSigGrphHandle = LoadLibrary(_T("SigGrphWnd.dll"));
    vloadFuncPtrAddress();
}

/*******************************************************************************
  Function Name  : vInitializeFuncPtrs
  Input(s)       : -
  Output         : -
  Functionality  : Initialize Function Ptrs to null
  Member of      : CSigGrphHandler
  Author(s)      : ArunKumar K
  Date Created   : 26.10.2010
  Modifications  :
*******************************************************************************/
void CSigGrphHandler::vInitializeFuncPtrs()
{
    pfCreateGraphWindow     = NULL;
    pfIsWindowVisible       = NULL;
    pfShowGraphWindow       = NULL;
    pfSetSignalListDetails  = NULL;
    pfPostMessageToSGWnd    = NULL;
    pfGetGraphBuffer        = NULL;
    pfGetWindowSplitterPos  = NULL;
    pfSetWindowSplitterPos  = NULL;
}

/*******************************************************************************
  Function Name  : vloadFuncPtrAddress
  Input(s)       : -
  Output         : -
  Functionality  : load lib and get Function pointers
  Member of      : CSigGrphHandler
  Author(s)      : ArunKumar K
  Date Created   : 26.10.2010
  Modifications  :
*******************************************************************************/
void CSigGrphHandler::vloadFuncPtrAddress()
{
    vInitializeFuncPtrs();
    pfCreateGraphWindow     = (CREATEGRAPHWINDOW)
                              GetProcAddress(m_hSigGrphHandle, "SG_CreateGraphWindow");
    pfIsWindowVisible       = (ISWINDOWVISIBLE)
                              GetProcAddress(m_hSigGrphHandle, "SG_IsWindowVisible");
    pfShowGraphWindow       = (SHOWGRAPHWINDOW)
                              GetProcAddress(m_hSigGrphHandle, "SG_ShowGraphWindow");
    pfSetSignalListDetails  = (SETSIGNALLISTDETAILS)
                              GetProcAddress(m_hSigGrphHandle, "SG_SetSignalListDetails");
    pfPostMessageToSGWnd    = (POSTMESSAGETOSGWND)
                              GetProcAddress(m_hSigGrphHandle, "SG_vPostMessageToSGWnd");
    pfGetGraphBuffer        = (GETGRAPHBUFFER)
                              GetProcAddress(m_hSigGrphHandle, "SG_GetGraphBuffer");
    pfGetWindowSplitterPos  = (GETWINDOWSPLITTERPOS)
                              GetProcAddress(m_hSigGrphHandle, "SG_GetWindowSplitterPos");
    pfSetWindowSplitterPos  = (SETWINDOWSPLITTERPOS)
                              GetProcAddress(m_hSigGrphHandle, "SG_SetWindowSplitterPos");
}

/*******************************************************************************
  Function Name  : CreateGraphWindow
  Input(s)       : pParentWnd, eBusType
  Output         : HRESULT
  Functionality  : Enables the creation of Graph Window for a particular bus.
  Member of      : CSigGrphHandler
  Author(s)      : ArunKumar K
  Date Created   : 26.10.2010
  Modifications  :
*******************************************************************************/
HRESULT CSigGrphHandler::CreateGraphWindow(CMDIFrameWnd* pParentWnd,short eBusType)
{
    if(pfCreateGraphWindow!=NULL)
    {
        return pfCreateGraphWindow(pParentWnd, eBusType);
    }
    else
    {
        return S_FALSE;
    }
}

/*******************************************************************************
  Function Name  : bIsWindowVisible
  Input(s)       : eBusType
  Output         : BOOL
  Functionality  : Gets the Graph window's visibility status for a bus.
  Member of      : CSigGrphHandler
  Author(s)      : ArunKumar K
  Date Created   : 29.10.2010
  Modifications  :
*******************************************************************************/
BOOL CSigGrphHandler::bIsWindowVisible(short eBusType)
{
    if(pfIsWindowVisible!=NULL)
    {
        return pfIsWindowVisible(eBusType);
    }
    else
    {
        return FALSE;
    }
}

/*******************************************************************************
  Function Name  : ShowGraphWindow
  Input(s)       : eBusType, bShow
  Output         : HRESULT
  Functionality  : Shows/hides a particular graph window of a particular bus.
  Member of      : CSigGrphHandler
  Author(s)      : ArunKumar K
  Date Created   : 29.10.2010
  Modifications  :
*******************************************************************************/
HRESULT CSigGrphHandler::ShowGraphWindow(short eBusType, BOOL bShow)
{
    if(pfShowGraphWindow!=NULL)
    {
        return pfShowGraphWindow(eBusType, bShow);
    }
    else
    {
        return S_FALSE;
    }
}

/*******************************************************************************
  Function Name  : SetSignalListDetails
  Input(s)       : eBusType, pSignalList
  Output         : HRESULT
  Functionality  : Sets the configured signal list details for a bus.
  Member of      : CSigGrphHandler
  Author(s)      : ArunKumar K
  Date Created   : 02.11.2010
  Modifications  :
*******************************************************************************/
HRESULT CSigGrphHandler::SetSignalListDetails(short eBusType, CGraphList* pSignalList)
{
    if(pfSetSignalListDetails!=NULL)
    {
        return pfSetSignalListDetails(eBusType, pSignalList);
    }
    else
    {
        return S_FALSE;
    }
}

/*******************************************************************************
  Function Name  : vPostMessageToSGWnd
  Input(s)       : short eBusType, UINT msg, WPARAM wParam, LPARAM lParam
  Output         : -
  Functionality  : Posts the Message to SigGrphWnd Dll.
  Member of      : CSigGrphHandler
  Author(s)      : ArunKumar K
  Date Created   : 09.11.2010
  Modifications  :
*******************************************************************************/
void CSigGrphHandler::vPostMessageToSGWnd(short eBusType, UINT msg,
        WPARAM wParam, LPARAM lParam)
{
    if(pfPostMessageToSGWnd != NULL)
    {
        pfPostMessageToSGWnd(eBusType, msg, wParam, lParam);
    }
}

/*******************************************************************************
  Function Name  : vGetGraphBuffer
  Input(s)       : -
  Output         : CMsgBufVSE*
  Functionality  : Gets the CBaseMsgBufVSE pointer initialized.
  Member of      : CSigGrphHandler
  Author(s)      : ArunKumar K
  Date Created   : 25.11.2010
  Modifications  :
*******************************************************************************/
CMsgBufVSE* CSigGrphHandler::vGetGraphBuffer()
{
    if(pfGetGraphBuffer != NULL)
    {
        return pfGetGraphBuffer();
    }
    else
    {
        return NULL;
    }
}

/*******************************************************************************
  Function Name  : GetWindowSplitterPos
  Input(s)       : eBusType, sWndPlacement, sGraphSplitter
  Output         : HRESULT
  Functionality  : Gets the Graph window position and splitter details.
  Member of      : CSigGrphHandler
  Author(s)      : ArunKumar K
  Date Created   : 08.12.2010
  Modifications  :
*******************************************************************************/
HRESULT CSigGrphHandler::GetWindowSplitterPos(short eBusType,
        WINDOWPLACEMENT& sWndPlacement,
        SGRAPHSPLITTERDATA& sGraphSplitter)
{
    if(pfGetWindowSplitterPos!=NULL)
    {
        return pfGetWindowSplitterPos(eBusType, sWndPlacement, sGraphSplitter);
    }
    else
    {
        return S_FALSE;
    }
}

/*******************************************************************************
  Function Name  : SetWindowSplitterPos
  Input(s)       : eBusType, sWndPlacement, sGraphSplitter
  Output         : HRESULT
  Functionality  : Sets the Graph window position and splitter details.
  Member of      : CSigGrphHandler
  Author(s)      : ArunKumar K
  Date Created   : 08.12.2010
  Modifications  :
*******************************************************************************/
HRESULT CSigGrphHandler::SetWindowSplitterPos(short eBusType,
        WINDOWPLACEMENT& sWndPlacement,
        SGRAPHSPLITTERDATA& sGraphSplitter)
{
    if(pfSetWindowSplitterPos!=NULL)
    {
        return pfSetWindowSplitterPos(eBusType, sWndPlacement, sGraphSplitter);
    }
    else
    {
        return S_FALSE;
    }
}



