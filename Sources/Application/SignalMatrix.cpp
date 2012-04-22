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
 * \file      SignalMatrix.cpp
 * \brief     Implementation file for CSignalMatrix class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CSignalMatrix class
 */


#include "stdafx.h"             // For Standard Includes
#include "Utility/OffScreenDC.h"        // For Memory DC class Implementation
#include "SignalMatrix.h"       // For CSignalMatrix class declaration

#define HIGHLIGHT_COLOR     RGB(0x20,0x20,0xaa)
#define NO_HIGHLIGHT_COLOR  RGB(255,255,255)
#define DISABLED_COLOR      RGB(212,208,200)

#define defX_DIVISION       2
#define defY_DIVISION       1.35
/*******************************************************************************
  Function Name  : CSignalMatrix
  Description    : Standard default constructor
  Member of      : CSignalMatrix
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 15.4.2005
  Modifications  :
*******************************************************************************/
CSignalMatrix::CSignalMatrix(int nMessageLength)
{
    // Create GDI Objects
    m_pHighlightBrush = new CBrush(HIGHLIGHT_COLOR);
    m_pNoHighlightBrush = new CBrush(NO_HIGHLIGHT_COLOR);
    m_pDisabledBrush = new CBrush(DISABLED_COLOR);

    // Set message Length
    m_unMessageLength = nMessageLength;
    // Clear highlight array
    vResetHighlight();
    // Initialise memory
    for( UINT unIndex = 0; unIndex < MAX_SIGNALS; unIndex++ )
    {
        m_abSignalData[ unIndex ] = 0;
    }
}

/*******************************************************************************
  Function Name  : CSignalMatrix
  Description    : Standard default constructor
  Member of      : CSignalMatrix
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 15.4.2005
  Modifications  :
*******************************************************************************/
CSignalMatrix::CSignalMatrix()
{
    // Create GDI Objects
    m_pHighlightBrush = new CBrush(HIGHLIGHT_COLOR);
    m_pNoHighlightBrush = new CBrush(NO_HIGHLIGHT_COLOR);
    m_pDisabledBrush = new CBrush(DISABLED_COLOR);

    // Set message Length
    m_unMessageLength = 8;
    // Clear highlight array
    vResetHighlight();
    // Initialise memory
    for( UINT unIndex = 0; unIndex < MAX_SIGNALS; unIndex++ )
    {
        m_abSignalData[ unIndex ] = 0;
    }
}

/*******************************************************************************
  Function Name  : ~CSignalMatrix
  Description    : Standard Destructor
  Member of      : CSignalMatrix
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 15.4.2005
  Modifications  :
*******************************************************************************/
CSignalMatrix::~CSignalMatrix()
{
    // Clear GDI Objects and Memory associated with that
    // Highlight brush
    if( m_pHighlightBrush != NULL )
    {
        m_pHighlightBrush->DeleteObject();
        delete m_pHighlightBrush;
        m_pHighlightBrush = NULL;
    }
    // Normal brush
    if( m_pNoHighlightBrush != NULL )
    {
        m_pNoHighlightBrush->DeleteObject();
        delete m_pNoHighlightBrush;
        m_pNoHighlightBrush = NULL;
    }
    // Disabled brush
    if( m_pDisabledBrush != NULL )
    {
        m_pDisabledBrush->DeleteObject();
        delete m_pDisabledBrush;
        m_pDisabledBrush = NULL;
    }
}


BEGIN_MESSAGE_MAP(CSignalMatrix, CStatic)
    //{{AFX_MSG_MAP(CSignalMatrix)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*******************************************************************************
  Function Name  : OnPaint
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called when painting of control is
                   required. This function will draw rectangles and text. This
                   will draw highlighted cells with highlight brush.
  Member of      : CSignalMatrix
  Author(s)      : Raja N
  Date Created   : 15.4.2005
  Modifications  : 
*******************************************************************************/
void CSignalMatrix::OnPaint() 
{
    // device context for painting
    CPaintDC sdc(this);
    // To get client rectangle
    CRect omClientRect;
    GetClientRect(&omClientRect);
    // Create Buffer DC for offline painting
    COffScreenDC dc(&sdc, &omClientRect);

    // Get X and Y Offset
    // Create 8 X 8 matrix of rectangles
    double xOffset = omClientRect.Width() / (double)defMAX_BYTE;
    double yOffset = omClientRect.Height() / (double)defMAX_BYTE;
    // If some offset is required between rectangles use this gap
    int nGap = 0;

    // Set Text drawing mode
    dc.SetTextAlign(TA_CENTER|TA_BASELINE);
    // Avoid drawing background box
    dc.SetBkMode(TRANSPARENT);
    // Iterate through matrix
    // Note Bits starts from right to left
    // Bytes starts from top to bottom
    for( int nBits = defMAX_BYTE - 1; nBits >= 0; nBits-- )
    {
        for( int nBytes = 0; nBytes < defMAX_BYTE; nBytes++ )
        {
            // Create current bit rectangle
            CRect omCurrentBox( (int)(omClientRect.left + xOffset * nBits + nGap),
                (int)(omClientRect.top + yOffset * nBytes + nGap),
                (int)(omClientRect.left + xOffset * nBits + xOffset - nGap),
                (int)(omClientRect.top + yOffset * nBytes + yOffset - nGap ));
            // Format value text
            CString omStrText;
            // Get Bit Index
            int nBitIndex = (defMAX_BYTE - 1 - nBits) +
                                nBytes * defMAX_BYTE;
            // Format bit text
            omStrText.Format( defFORMAT_MSGID_DECIMAL,
                              m_abSignalData[ nBitIndex ] );
            // Check type of bit it is
            switch( m_aunHighlight[ nBitIndex ] )
            {
            // Not with in message length
            case GRAYED:
                {
                    // Draw Grey Rectangle
                    dc.FillRect(&omCurrentBox,m_pDisabledBrush);
                }
                break;
            // Highlighted bit
            case HIGHLIGHT:
                {
                    // Fill back color
                    dc.FillRect(&omCurrentBox,m_pHighlightBrush);
                    // Set text color
                    dc.SetTextColor( NO_HIGHLIGHT_COLOR );
                    // Draw text
                    dc.TextOut(
                        (int)(omCurrentBox.left + omCurrentBox.Width()/ defX_DIVISION),
                        (int)(omCurrentBox.top + omCurrentBox.Height()/ defY_DIVISION),
                        omStrText );
                }
                break;
            // Non-Highlighted
            case NO_HIGHLIGHT:
                {
                    // Fill back color
                    dc.FillRect( &omCurrentBox ,m_pNoHighlightBrush );
                    // Set text color
                    dc.SetTextColor( HIGHLIGHT_COLOR );
                    dc.TextOut(
                        (int)(omCurrentBox.left + omCurrentBox.Width()/ defX_DIVISION),
                        (int)(omCurrentBox.top + omCurrentBox.Height()/ defY_DIVISION),
                        omStrText );
                }
                break;
            }
            // Draw 3D Rectangle around the rectangle
            dc.DrawEdge( omCurrentBox,
                         EDGE_ETCHED,
                         BF_BOTTOMLEFT );
        }
    }
}

/*******************************************************************************
  Function Name  : vResetHighlight
  Input(s)       : -
  Output         : -
  Functionality  : This function will clear highlight array
  Member of      : CSignalMatrix
  Author(s)      : Raja N
  Date Created   : 15.4.2005
  Modifications  : 
*******************************************************************************/
void CSignalMatrix::vResetHighlight()
{
    // Clear Highlight
    UINT nCount;    //nCount is declared outside the for loop
    for ( nCount = 0;nCount < m_unMessageLength * defBITS_IN_BYTE; nCount++)
    {
        m_aunHighlight[nCount] = NO_HIGHLIGHT;
    }

    // Clear bits that are not part of message
    for ( nCount = m_unMessageLength * defBITS_IN_BYTE;
          nCount < MAX_SIGNALS;
          nCount++ )
    {
        m_aunHighlight[nCount] = GRAYED;
    }
}

/*******************************************************************************
  Function Name  : vSetHighlight
  Input(s)       : pbySigMask  : Signal mask
                   unArrayLen : length of the byte array
  Output         : -
  Functionality  : This function will set signal matrix according
                   to the signal mask.
  Member of      : CSignalMatrix
  Author(s)      : Pradeep Kadoor
  Date Created   : 10.07.2010
*******************************************************************************/
void CSignalMatrix::vSetHighlight(const BYTE* pbySigMask, UINT unArrayLen)
{
    //Reset all the Highlight bits
    vResetHighlight();

    //Set the highlight bits
    UINT BitIndex = 0;
    for (UINT i = 0; i < unArrayLen; i++)
    {   
        for (UINT nShift = 0; nShift < defBITS_IN_BYTE; nShift++)
        {
            m_aunHighlight[BitIndex] = ((pbySigMask[i] >> nShift) & 0x1)? HIGHLIGHT : NO_HIGHLIGHT;
            BitIndex++;
        }
    }
    // Update UI to reflect the changes
    Invalidate(FALSE);
}

/*******************************************************************************
  Function Name  : vResetValues
  Input(s)       : -
  Output         : -
  Functionality  : This function resets all cells with value '0'
  Member of      : CSignalMatrix
  Author(s)      : Raja N
  Date Created   : 15.4.2005
  Modifications  : 
*******************************************************************************/
void CSignalMatrix::vResetValues()
{
    for( UINT nIndex = 0; nIndex < m_unMessageLength * defBITS_IN_BYTE; nIndex++)
    {
        m_abSignalData[ nIndex] = 0;
    }
}

/*******************************************************************************
  Function Name  : vSetValue
  Input(s)       : Pointer to an array of value to be set in the matrix
  Output         : -
  Functionality  : This function sets values to the cells in the signal matrix
  Member of      : CSignalMatrix
  Author(s)      : Raja N
  Date Created   : 15.4.2005
  Modifications  : 
*******************************************************************************/
void CSignalMatrix::vSetValue(UINT* punValues)
{
    // If it is valid pointer
    if ( punValues != NULL )
    {
        for ( UINT unIndex = 0;
              unIndex < m_unMessageLength * defBITS_IN_BYTE;
              unIndex++ )
        {
            //Get value at unIndex
            m_abSignalData[ unIndex ] = punValues[ unIndex ];
        }
        // Update UI
        Invalidate();
    }
}

/*******************************************************************************
  Function Name  : vSetMessageLength
  Input(s)       : Length of the message
  Output         : -
  Functionality  : This function sets the length of the message
  Member of      : CSignalMatrix
  Author(s)      : Raja N
  Date Created   : 15.4.2005
  Modifications  : 
*******************************************************************************/
void CSignalMatrix::vSetMessageLength(UINT unMsgLength)
{
    // Init Message Length
    m_unMessageLength = unMsgLength;
    // Clear Highlight to avoid confusion
    vResetHighlight();
    // Update UI
    Invalidate();
}

/*******************************************************************************
  Function Name  : vSetByteValue
  Input(s)       : Pointer to char array of value to be set in the matrix
  Output         : -
  Functionality  : This function sets values to the cells in the signal matrix
  Member of      : CSignalMatrix
  Author(s)      : Raja N
  Date Created   : 15.4.2005
  Modifications  : 
*******************************************************************************/
void CSignalMatrix::vSetByteValue(UCHAR* punValues)
{
    // If the pointer is valid
    if ( punValues != NULL )
    {
        int nByte, nBit;
        UINT unValue;
        // Iterate through the array
        for ( UINT unIndex = 0;
              unIndex < m_unMessageLength * defBITS_IN_BYTE;
              unIndex++ )
        {
            //Get value at unIndex
            nByte = unIndex / defBITS_IN_BYTE;
            nBit = unIndex % defBITS_IN_BYTE;
            unValue = punValues[ nByte ] &
                            ( 1 << nBit ) ? 1 : 0;
            if ( unValue == 1 )
                m_abSignalData[ unIndex ] = 1;
            else
                m_abSignalData[ unIndex ] = 0;
        }
    // Update UI
    Invalidate();
    }
}

/*******************************************************************************
 Function Name    : OnEraseBkgnd
 Input(s)         : pDC - Pointer to Device Context
 Output           :
 Functionality    : This function will be called by the framework to erase the
                    background. Return TRUE to show that background has been
                    erased
 Member of        : CSigWatchDlg
 Friend of        :   -
 Author(s)        : Raja N
 Date Created     : 15.04.2005
 Modified by      :
 Modification     :
*******************************************************************************/
BOOL CSignalMatrix::OnEraseBkgnd(CDC* /*pDC*/) 
{
    // Return TRUE. Paint function will take care of erasing the background
    return TRUE;
}