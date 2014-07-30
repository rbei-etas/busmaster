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
 * \file      OffScreenDC.h
 * \brief     This file contain the interface for COffScreenDC class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the interface for COffScreenDC class
 */

#pragma once

class COffScreenDC : public CDC
{
public:
    // Public constructor with screen DC and Screen Rectangle
    COffScreenDC(CDC* pDC, const CRect& rcBounds);
    // Default destructor
    virtual ~COffScreenDC();
    // Type casting operator
    inline COffScreenDC* operator->();
    // Flag to indicate Creation status
    BOOL        m_bCreateSuccess;
protected:

private:
    // Pointer to hold the bitmap data
    CBitmap     m_omBitmap;
    // Pointer to store old bitmap address
    CBitmap*    m_pOldBitmap;
    // Pointer to store actual screen DC
    CDC*        m_pDC;
    // Rectangle of the screen or Drawing area
    CRect       m_rcBounds;
};
