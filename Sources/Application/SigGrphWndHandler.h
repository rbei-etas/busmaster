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
 * \file      SigGrphWndHandler.h
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "SigGrphWnd_Extern.h"
#include "DataTypes/MsgBufVSE.h"
#include "struct.h"

class CSigGrphHandler
{
private:
    HMODULE m_hSigGrphHandle;
    void vInitializeFuncPtrs();
    void vloadFuncPtrAddress();
public:
	CSigGrphHandler(void);
	~CSigGrphHandler(void);
	void vLoadSigGrph_DLL();

	HRESULT CreateGraphWindow(CMDIFrameWnd* pParentWnd,short eBusType);
	BOOL	bIsWindowVisible(short eBusType);
	HRESULT ShowGraphWindow(short eBusType, BOOL bShow);
	HRESULT SetSignalListDetails(short eBusType, CGraphList * pSignalList);
	void	vPostMessageToSGWnd(short eBusType, UINT msg, WPARAM wParam, LPARAM lParam);
	CMsgBufVSE* vGetGraphBuffer();
	HRESULT GetWindowSplitterPos(short eBusType, 
								 WINDOWPLACEMENT& sWndPlacement, 
								 SGRAPHSPLITTERDATA& sGraphSplitter);
	HRESULT SetWindowSplitterPos(short eBusType, 
								 WINDOWPLACEMENT& sWndPlacement, 
								 SGRAPHSPLITTERDATA& sGraphSplitter);
};
