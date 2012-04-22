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
 * \file      OffScreenDC.cpp
 * \brief     This file contain the definition all member function of
 * \author    Amitesh Bharti & Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition all member function of
 */

// Standard include header
#include "Utils_stdafx.h"
// OffScreenDC class definition File
#include "OffScreenDC.h"

// For common definitions
#define WHITE_COLOR             RGB(255,255,255)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*******************************************************************************
 Function Name  : COffScreenDC
 Description    : Standard default constructor
 Member of      : COffScreenDC
 Functionality  : Initialises data members and creates offscreen (buffer) Bitmap
 Author(s)      : Raja N
 Date Created   : 23.06.2004
 Modifications  : Raja N on 06.08.2004
                  Added code to check the failure condition
*******************************************************************************/
COffScreenDC::COffScreenDC(CDC* pDC, const CRect& rcBounds) : CDC()
{
    CreateCompatibleDC(pDC);
    
    // Store the screen DC
    m_pDC = pDC;
    // Store Screen Area Rectangle
    m_rcBounds = rcBounds;
    // Create a new bitmap for offscreen
    m_bCreateSuccess = m_omBitmap.CreateCompatibleBitmap( pDC, rcBounds.Width(),
                                                          rcBounds.Height());
    // If the creation is success
    if( m_bCreateSuccess == TRUE )
    {
        // Select the Bit map and
        // Save the old bitmap
        m_pOldBitmap = SelectObject( &m_omBitmap );
        // Fill the background with white color
    }
    // Create failed use the CDC directly
    else
    {
        // Delete the created DC
        DeleteDC();
        // Use the Screen DC directly
        m_hDC = pDC->m_hDC;
        // Copy m_hAttribDC
        // This data member contains the attribute device context for this
        // CDC object
        m_hAttribDC = pDC->m_hAttribDC;
    }
    // To change the background replace it with approp. background color
    FillSolidRect( &rcBounds, WHITE_COLOR );
}

/*******************************************************************************
 Function Name  : ~COffScreenDC
 Description    : Standard Destructor
 Member of      : COffScreenDC
 Functionality  : Copies the offscreen buffer in to the screen and deletes the
                  offscreen buffer.
 Author(s)      : Raja N
 Date Created   : 23.06.2004
 Modifications  : Raja N on 06.08.2004
                  Added code to check the failure condition
*******************************************************************************/
COffScreenDC::~COffScreenDC()
{
    // Copy the data in to the screen
    // only if the bitmap creation is successful
    if( m_bCreateSuccess == TRUE )
    {
        m_pDC->BitBlt( m_rcBounds.left,
                       m_rcBounds.top,
                       m_rcBounds.Width(),
                       m_rcBounds.Height(),
                       this,    // This as source
                       m_rcBounds.left,
                       m_rcBounds.top,
                       SRCCOPY );
        // Restore old bitmap
        SelectObject(m_pOldBitmap);
    }
    else
    {
        // As the painting is direct no need to do anything here
    }
}

/*******************************************************************************
 Function Name  : operator ->
 Description    : Overloaded operator ->
 Member of      : COffScreenDC
 Functionality  : Returns the self pointer to use the operator ->
 Author(s)      : Raja N
 Date Created   : 23.06.2004
*******************************************************************************/
COffScreenDC * COffScreenDC::operator->()
{
    return this;
}