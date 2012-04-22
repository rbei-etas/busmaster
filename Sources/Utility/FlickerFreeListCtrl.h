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
 * \file      FlickerFreeListCtrl.h
 * \brief     Definition file for CFlickerFreeTree class.
 * \author    Ravi D.
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CFlickerFreeTree class.
 */

#pragma once

// CFlickerFreeListCtrl

class CFlickerFreeListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CFlickerFreeListCtrl)

protected:
	//Added new function declaration
#if 0
     afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	 afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
#endif //These functions have to be added in derived class

	 afx_msg UINT OnGetDlgCode( );
public:
	CFlickerFreeListCtrl();
	virtual ~CFlickerFreeListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CRect m_rectClient;
};
