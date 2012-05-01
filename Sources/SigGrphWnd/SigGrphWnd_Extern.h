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
 * \file      SigGrphWnd_Extern.h
 * \brief     Exported function prototypes
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Exported function prototypes
 */

#pragma once

#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif

class CGraphList;
class CMsgBufVSE;

////Graph window splitter details
struct tagGraphWndSplitterData
{
    int m_nRootSplitterData[2][2];
    int m_nRightViewSplitterData[2][2];
};

typedef tagGraphWndSplitterData sGraphWndSplitterData;
typedef sGraphWndSplitterData SGRAPHWNDSPLITTERDATA;

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

    /*USAGEMODE HRESULT SG_CreateGraphWindow([in] CMDIFrameWnd* pParentWnd, [in] short eBusType);
    USAGEMODE HRESULT SG_ShowGraphWindow([in] short eBusType, [in] BOOL bShow = TRUE);
    USAGEMODE BOOL    SG_IsWindowVisible([in] short eBusType);
    USAGEMODE HRESULT SG_SetRefreshRate( [in] UINT unRefreshRate);
    USAGEMODE HRESULT SG_SetSignalListDetails( [in] short eBusType, [in] CGraphList * pSignalList);
    USAGEMODE HRESULT SG_vPostMessageToSGWnd([in] short eBusType, UINT msg, WPARAM wParam, LPARAM lParam);
    USAGEMODE CMsgBufVSE* SG_GetGraphBuffer();
    USAGEMODE HRESULT SG_GetWindowSplitterPos([in] short eBusType,
                                            WINDOWPLACEMENT& sWndPlacement,
                                            SGRAPHWNDSPLITTERDATA& sGraphSplitter);
    USAGEMODE HRESULT SG_SetWindowSplitterPos([in] short eBusType,
                                            WINDOWPLACEMENT& sWndPlacement,
                                            SGRAPHWNDSPLITTERDATA& sGraphSplitter);*/
    USAGEMODE HRESULT SG_CreateGraphWindow( CMDIFrameWnd* pParentWnd, short eBusType);
    USAGEMODE HRESULT SG_ShowGraphWindow( short eBusType, BOOL bShow = TRUE);
    USAGEMODE BOOL    SG_IsWindowVisible( short eBusType);
    USAGEMODE HRESULT SG_SetRefreshRate( UINT unRefreshRate);
    USAGEMODE HRESULT SG_SetSignalListDetails( short eBusType, CGraphList* pSignalList);
    USAGEMODE HRESULT SG_vPostMessageToSGWnd( short eBusType, UINT msg, WPARAM wParam, LPARAM lParam);
    USAGEMODE CMsgBufVSE* SG_GetGraphBuffer();
    USAGEMODE HRESULT SG_GetWindowSplitterPos( short eBusType,
            WINDOWPLACEMENT& sWndPlacement,
            SGRAPHWNDSPLITTERDATA& sGraphSplitter);
    USAGEMODE HRESULT SG_SetWindowSplitterPos( short eBusType,
            WINDOWPLACEMENT& sWndPlacement,
            SGRAPHWNDSPLITTERDATA& sGraphSplitter);

#ifdef __cplusplus
}
#endif
