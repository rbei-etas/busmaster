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
 * \file      ResultView.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

// CResultView view

class CResultView : public CListView
{
//Member Variables
private:
    CImageList m_omImageList;

//Member functions
private:
	DECLARE_DYNCREATE(CResultView)
protected:
    DECLARE_MESSAGE_MAP()
    // protected constructor used by dynamic creation
    CResultView(void);           
    //Pre Create window handles
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //Proeteced Constructor
    virtual ~CResultView(void);

public:
    virtual void OnInitialUpdate(void);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
