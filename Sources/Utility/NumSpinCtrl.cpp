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
 * \file      NumSpinCtrl.cpp
 * \brief     This file contain implementation of CNumSpinCtrl class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain implementation of CNumSpinCtrl class
 */

// For all standard header file include
#include "Utils_StdAfx.h"         // Standard include
#include "Utility_Structs.h"
#include "NumSpinCtrl.h"    // Class definition file
#include "Utility.h"
//#include "UtilFunctions.h"  // For all utility functions

// Epsilon Value. Used for floating value Comparison
// This has been taken from  ** Float.h **
#define defEPS 2.2204460492503131e-016 /* smallest such that 1.0+DBL_EPSILON != 1.0 */

#define defSTR_DEFAULT_FORMAT_STRING    _T("%g")

/////////////////////////////////////////////////////////////////////////////
// CNumSpinCtrl

/******************************************************************************
 Function Name  :   CNumSpinCtrl

 Description    :   Default Constructor
 Member of      :   CNumSpinCtrl
 Functionality  :   Initialises data members

 Author(s)      :   Raja N
 Date Created   :   22.07.2004
 Modifications  :   Raja N on 02.08.2004, Initialised member variable
******************************************************************************/
CNumSpinCtrl::CNumSpinCtrl()
{
    m_dDelta = 1;            // Init with default Min, Max and Range values
    m_dMinVal = 0;
    m_dMaxVal = 100;
    m_n64MinVal = 0;
    m_n64MaxVal = 100;
    m_n64Delta = 1;
    m_unIntRange = 100;
    m_nDataLength = defMAX_BITS;    // Default is 64 bits
    m_bSigned = TRUE;
    m_bFloatingNumber = FALSE;
    vSetFormatString(NULL);
}

/******************************************************************************
 Function Name  :   ~CNumSpinCtrl

 Description    :   Standard destructor
 Member of      :   CNumSpinCtrl

 Author(s)      :   Raja N
 Date Created   :   22.07.2004
 Modifications  :
******************************************************************************/
CNumSpinCtrl::~CNumSpinCtrl()
{
}


BEGIN_MESSAGE_MAP(CNumSpinCtrl, CSpinButtonCtrl)
    //{{AFX_MSG_MAP(CNumSpinCtrl)
    ON_NOTIFY_REFLECT_EX(UDN_DELTAPOS, OnDeltaPos)
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumSpinCtrl message handlers

/*******************************************************************************
 Function Name  : vSetValueForBuddy
 Input(s)       : val - Double precession value of for the text box
 Output         :  -
 Functionality  : This will be called to set the value to the buddy text box.
                  This will format the value in the user specified format and
                  will set the text to the buddy text control.
 Member of      : CNumSpinCtrl
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  : Raja N on 30.07.2004.
                  Code review changes and Int 64 version implementation to
                  support signals > 52 bits length
*******************************************************************************/
void CNumSpinCtrl::vSetValueForBuddy (double dVal)
{
    // Get the Text window
    CWnd* pomEdit = GetBuddy();

    // If it is valid
    if ( pomEdit != NULL )
    {
        CString omStr;

        // Check for user defined format string
        if (m_omStrFormat.IsEmpty ())
        {
            // Use default format string
            omStr.Format( defSTR_DEFAULT_FORMAT_STRING ,dVal);
        }
        else
        {
            // For decimal mode format the value directly
            if( GetBase() == defBASE_DEC )
            {
                omStr.Format (m_omStrFormat, dVal);
            }
            else
            {
                // For hex double should not be used. Use __int64 member!!
                ASSERT( FALSE );
            }
        }

        // Set the formatted text to buddy control
        pomEdit->SetWindowText (omStr);
    }
}

/*******************************************************************************
 Function Name  : vSetValueForBuddy
 Input(s)       : val - Double precession value of for the text box
 Output         :  -
 Functionality  : This will be called to set the value to the buddy text box.
                  This will format the value in the user specified format and
                  will set the text to the buddy text control.
 Member of      : CNumSpinCtrl
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
void CNumSpinCtrl::vSetValueForBuddy (__int64 n64Val)
{
    // Get the Text window
    CWnd* pomEdit = GetBuddy();

    // If it is valid
    if ( pomEdit != NULL )
    {
        CString omStr;

        // Check for user defined format string
        if (m_omStrFormat.IsEmpty ())
        {
            // Use default format string
            omStr.Format( defFORMAT_INT64_DECIMAL ,n64Val);
        }
        else
        {
            // Check the Base value
            // For decima mode (Base 10) negative numbers are represented
            // as - (val). Say -1. But in hex mode '-' is invalid and 2s
            // complement form will be used to represent the negative value.
            // In 2s complement the most significant bit is Sign bit. So data
            // length should be considered to get and set the value if the mode
            // is in Hex

            // For decimal mode format the value directly
            if( GetBase() == defBASE_DEC )
            {
                omStr.Format (m_omStrFormat, n64Val);
            }
            // For hex take the value and mask unwanted portion
            else
            {
                // In Hex change the negative number as - Positive Num
                // Format the min value
                vRemoveUnwantedBits( n64Val, m_nDataLength);
                omStr.Format (m_omStrFormat, n64Val);
            }
        }

        // Set the formatted text to buddy control
        pomEdit->SetWindowText (omStr);
    }
}

/*******************************************************************************
 Function Name  : SetPos
 Input(s)       : val - Double precession value of for the text box
 Output         :  -
 Functionality  : This method sets the current position for a spin button
                  control. This is an interface function to change the position
                  programmatically.
 Member of      : CNumSpinCtrl
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  : Raja N on 30.07.2004
                  Modified the function name as SetPos as this is overriden from
                  base class CSpinButtonCtrl.
*******************************************************************************/
void CNumSpinCtrl::SetPos(double dVal)
{
    // Set the text
    vSetValueForBuddy (dVal);
    // Set the scrollbar position (Int value)
    vSetIntPos (dVal);
}

/*******************************************************************************
 Function Name  : dGetPos
 Input(s)       :   -
 Output         : double - Double precession value of for the text box
 Functionality  : This method get the current virtual double precession value of
                  of the spin button.
 Member of      : CNumSpinCtrl
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  : Raja N on 30.07.2004.
                  Code review changes and Int 64 version implementation to
                  support signals > 52 bits length
*******************************************************************************/
double CNumSpinCtrl::dGetPos()
{
    // Get the text control first to get the value
    CWnd* pomEdit = GetBuddy();
    TCHAR* pDummy = NULL;
    double dVal = 0.0;

    // If the buddy is set then proceed
    if (pomEdit)
    {
        CString omStrText;
        // Get the text
        pomEdit->GetWindowText (omStrText);

        // Check for Base value
        if( GetBase() == defBASE_DEC )
        {
            // In decimal mode floating point values are possible so
            // Use Double Value function to get decimal points also
            dVal = _tcstod( omStrText, &pDummy);
        }
        else
        {
            // Double type should not be used for Hex values
            ASSERT ( FALSE );
        }
    }

    // Return calculated value
    return dVal;
}

/*******************************************************************************
 Function Name  : n64GetPos
 Input(s)       :   -
 Output         : __int64 - 64 bit int value of for the text box
 Functionality  : This method get the current 64 bit int value of the spin
                  button.
 Member of      : CNumSpinCtrl
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 30.07.2004
 Modifications  :
*******************************************************************************/
__int64 CNumSpinCtrl::n64GetPos()
{
    __int64 n64Val = 0;
    // Get the text control first to get the value
    CWnd* pomEdit = GetBuddy();

    // If the buddy is set then proceed
    if (pomEdit != NULL )
    {
        CString omStrText;
        // Get the text
        pomEdit->GetWindowText (omStrText);
        // This can be used for both decimal and hex mode. So base check is not
        // required
        // Convert the text in to 64bit integer
        BOOL bSuccess =
            bConvertStringToInt64( omStrText,n64Val, GetBase());

        // Set the default value here incase of failure
        if( bSuccess == FALSE )
        {
            n64Val = 0;
        }

        // Extend the sign bit to get the actual value
        // If the value before is FF ( that is -1 )and data length is 8 bits
        // after extending the sign bit this will become FFFFFFFFFFFFFFFF
        // which is again -1 but the length is 64 bits
        if( m_bSigned == TRUE )
        {
            vExtendSignBit( n64Val, m_nDataLength);
        }
    }

    // Return calculated value
    return n64Val;
}
/*******************************************************************************

*******************************************************************************/
BOOL CNumSpinCtrl::bConvertStringToInt64( CString omStrHexNo,
        __int64& n64RetVal,
        int nBase )
{
    // Initialise local variables
    // Success Flag
    BOOL bSuccess = TRUE;
    // Length of the string
    int nStrLength = 0;
    // Present digit value
    int nCharVal = 0;
    // Negative flag
    BOOL b_IsNegative = FALSE;
    // Initialise the return value
    n64RetVal = 0;
    // Convert in to upper case
    omStrHexNo.MakeUpper();
    // Removes spaces at the begining and at the end
    omStrHexNo.TrimLeft();
    omStrHexNo.TrimRight();
    // Get the String length now after removing spaces.
    nStrLength = omStrHexNo.GetLength();

    // Iterate through elements to calculate values
    for (int nCount = 0; nCount < nStrLength; nCount++)
    {
        // Get the charector
        TCHAR cChar = omStrHexNo.GetAt( nCount);

        // Check for 0 - 9 range
        if( cChar >= '0' && cChar <= '9')
        {
            // Subtract char '0' to get the int value
            // say char '5' - char '0' - int 5
            nCharVal = cChar - '0';

            // If the value is greater then base then
            // the string is invalid. say with base 8, 9 is invalid
            if( nCharVal - nBase >= 0 )
            {
                // Show Error
                ASSERT (FALSE );
                // Stop the loop by setting the max value
                nCount = nStrLength;
                // Indicate Failure
                bSuccess = FALSE;
            }
            else
            {
                // Add current digit with the value
                n64RetVal = n64RetVal * nBase + nCharVal;
            }
        }
        // If the value is Hex
        else if( cChar >= 'A' && cChar <= 'F' )
        {
            // If the base is not hex then show error
            // and stop processing
            if( nBase != defBASE_HEX )
            {
                // Show Error
                ASSERT( FALSE );
                // Stop the loop by setting the max value
                nCount = nStrLength;
                // Indicate Failure
                bSuccess = FALSE;
            }
            // else Valid value
            else
            {
                // Get the int value and add 10 with that
                // Say 'A' - 'A' + 10 = 10
                //     'C' - 'A' + 10 = 12
                nCharVal = cChar - 'A' + defBASE_DEC;
                // Add current digit with the value
                n64RetVal = n64RetVal * nBase + nCharVal;
            }
        }
        else if( cChar == '-' )
        {
            // In Hex mode negative values are not correct
            if ( nBase != defBASE_DEC || nCount != 0 )
            {
                ASSERT( FALSE );
                // Stop the loop by setting the max value
                nCount = nStrLength;
                // Indicate Failure
                bSuccess = FALSE;
            }
            else
            {
                b_IsNegative = TRUE;
            }
        }
        else
        {
            // Invalid input char
            ASSERT( FALSE );
            // Stop the loop by setting the max value
            nCount = nStrLength;
            // Indicate Failure
            bSuccess = FALSE;
        }
    }

    // If negative flag is set then take the negative value
    if ( b_IsNegative == TRUE)
    {
        n64RetVal = -n64RetVal;
    }

    return bSuccess;
}

/*******************************************************************************
 Function Name  : GetPos
 Input(s)       :   -
 Output         : int - default value
 Functionality  : This method alerts the user to use eith dGetPos or n64GetPos
                  method instead of GetPos()
 Member of      : CNumSpinCtrl
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 30.07.2004
 Modifications  :
*******************************************************************************/
int CNumSpinCtrl::GetPos()
{
    /* GetPos function has two definitions dGetPos() and n64GetPos(). The former
    is for double value spin control and later is for int. Later supports 64 bit
    integer. So use either of this. */
    ASSERT( FALSE );
    return -1;
}
/*******************************************************************************
 Function Name  : vSetRangeAndDelta
 Input(s)       : lower - Min Value
                  upper - Max Value
                  delta - Step Value
 Output         :   -
 Functionality  : This method sets the Min,Max and step value of the spin button
 Member of      : CNumSpinCtrl
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  : Raja N on 30.07.2004.
                  Code review changes and Int 64 version implementation to
                  support signals > 52 bits length
*******************************************************************************/
void CNumSpinCtrl::vSetRangeAndDelta(double dLower, double dUpper, double dDelta)
{
    // Assign the values to the members
    m_dMinVal = dLower;
    m_dMaxVal = dUpper;
    m_dDelta = dDelta;

    // Avoid division by zero
    if (m_dDelta != 0.0)
    {
        // Figure out the integer range to use
        // so that acceleration can work properly
        double dRange = fabs ((m_dMaxVal - m_dMinVal) / m_dDelta);

        // Check for maximun range
        if (dRange > static_cast<double>(UD_MAXVAL) )
        {
            // If so use the maximun supported value
            m_unIntRange = UD_MAXVAL;
        }
        else
        {
            // Use the int value as step
            m_unIntRange = static_cast<int>(dRange);
        }

        // Set the range to the spin control
        CSpinButtonCtrl::SetRange32 (0, m_unIntRange);
        // Set integer position
        vSetIntPos (dGetPos());
    }
}

/*******************************************************************************
 Function Name  : vSetRangeAndDelta
 Input(s)       : n64Lower - Min Value
                  n64Upper - Max Value
                  n64Delta - Step Value
 Output         :   -
 Functionality  : This method sets the Min,Max and step value of the spin button
 Member of      : CNumSpinCtrl
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 30.07.2004
 Modifications  :
*******************************************************************************/
void CNumSpinCtrl::vSetRangeAndDelta( __int64 n64Lower, __int64 n64Upper,
                                      __int64 n64Delta )
{
    // Assign the values to the members
    m_n64MinVal = n64Lower;
    m_n64MaxVal = n64Upper;
    m_n64Delta = n64Delta;

    // Avoid division by zero
    if (m_n64Delta != 0)
    {
        // Figure out the integer range to use
        // so that acceleration can work properly
        // No need to change it to int 64 as finally it is going to be an int
        double dRange = fabs ((double)(m_n64MaxVal - m_n64MinVal) / (double)m_n64Delta);

        // Check for maximun range
        if (dRange > static_cast<double>(UD_MAXVAL) )
        {
            // If so use the maximun supported value
            m_unIntRange = UD_MAXVAL;
        }
        else
        {
            // Use the int value as step
            m_unIntRange = static_cast<int>(dRange);
        }

        // Set the range to the spin control
        CSpinButtonCtrl::SetRange32 (0, m_unIntRange);
        // Set integer position
        vSetIntPos (n64GetPos());
    }
}

/*******************************************************************************
 Function Name  : vSetIntPos
 Input(s)       : pos -
 Output         :   -
 Functionality  : This method sets the pos of the spin button. It derives the
                  int value from the double pos and sets the scroll pos.
 Member of      : CNumSpinCtrl
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
void CNumSpinCtrl::vSetIntPos (double dPos)
{
    // avoid division by zero
    if (m_dMaxVal != m_dMinVal)
    {
        int nInt_Pos;

        // Check for bundary condition
        if (dPos < m_dMinVal)
        {
            // Take the min int value. Should be zero
            nInt_Pos = 0;
        }
        else if (dPos > m_dMaxVal)
        {
            // Take the Max value
            nInt_Pos = m_unIntRange;
        }
        else
        {
            // Get the current postion in the range
            // Range could be like 5 - 15. If the position is 10 then we are in
            // the exact middle. So subtract min value from pos
            // Pos = ( Current Val - Min ) / ( Max - Min )
            double dPosInRange = (dPos - m_dMinVal) / (m_dMaxVal - m_dMinVal);
            // Now multiply this pos by int step to get actual int value
            // In the above example if the step is 2 then the val is 5 * 2 = 10
            nInt_Pos = (int)(m_unIntRange * dPosInRange + 0.5);
        }

        // Set the position of the scroll bar
        CSpinButtonCtrl::SetPos (nInt_Pos);
    }
}

/*******************************************************************************
 Function Name  : vSetIntPos
 Input(s)       : n64Pos - Spin control positon
 Output         :   -
 Functionality  : This method sets the pos of the spin button. It derives the
                  int value from the double pos and sets the scroll pos.
 Member of      : CNumSpinCtrl
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 30.07.2004
 Modifications  :
*******************************************************************************/
void CNumSpinCtrl::vSetIntPos (__int64 n64Pos)
{
    // avoid division by zero
    if (m_n64MaxVal != m_n64MinVal)
    {
        int nInt_Pos;

        // Check for bundary condition
        if (n64Pos < m_n64MinVal)
        {
            // Take the min int value. Should be zero
            nInt_Pos = 0;
        }
        else if (n64Pos > m_n64MaxVal)
        {
            // Take the Max value
            nInt_Pos = m_unIntRange;
        }
        else
        {
            // Get the current postion in the range
            // Range could be like 5 - 15. If the position is 10 then we are in
            // the exact middle. So subtract min value from pos
            // Pos = ( Current Val - Min ) / ( Max - Min )
            double dPosInRange =
                (double)((n64Pos - m_n64MinVal) / (m_n64MaxVal - m_n64MinVal));
            // Now multiply this pos by int step to get actual int value
            // In the above example if the step is 2 then the val is 5 * 2 = 10
            nInt_Pos = (int)(m_unIntRange * dPosInRange + 0.5);
        }

        // Set the position of the scroll bar
        CSpinButtonCtrl::SetPos (nInt_Pos);
    }
}

/*******************************************************************************
 Function Name  : vGetRangeAndDelta
 Input(s)       :   -
 Output         : lower - Min Value
                  upper - Max Value
                  delta - Step Value
 Functionality  : This method gets the Min,Max and step value of the spin button
 Member of      : CNumSpinCtrl
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
void CNumSpinCtrl::vGetRangeAndDelta(double& lower,double& upper,double& delta)
{
    // Update the values
    lower = m_dMinVal;
    upper = m_dMaxVal;
    delta = m_dDelta;
}

/*******************************************************************************
 Function Name  : vGetRangeAndDelta
 Input(s)       :   -
 Output         : n64Lower - Min Value
                  n64Upper - Max Value
                  n64Delta - Step Value
 Functionality  : This method gets the Min,Max and step value of the spin button
 Member of      : CNumSpinCtrl
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 30.07.2004
 Modifications  :
*******************************************************************************/
void CNumSpinCtrl::vGetRangeAndDelta( __int64& n64Lower, __int64& n64Upper,
                                      __int64& n64Delta)
{
    // Update the values
    n64Lower = m_n64MinVal;
    n64Upper = m_n64MaxVal;
    n64Delta = m_n64Delta;
}

/*******************************************************************************
 Function Name  : OnDeltaPos
 Description    : Notification Message Handler
 Functionality  : This function will be called by the framework during the press
                  of spin button in the edit control. This will update the buddy
                  text as per scroll position
 Member of      : CNumSpinCtrl
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
BOOL CNumSpinCtrl::OnDeltaPos(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pUD = (NM_UPDOWN*)pNMHDR;
    // Get the new value
    // Current Val + Step Value * Steps
    double dVal = 0.0;
    __int64 n64Val = 0;
    BOOL bInvalid = FALSE;

    if( m_bFloatingNumber == TRUE )
    {
        dVal = dGetPos () + m_dDelta * pUD->iDelta;
    }
    else
    {
        n64Val = n64GetPos () + m_n64Delta * pUD->iDelta;
    }

    // Check the property to wrap to min value incase of maximun value exceed
    // and vice versa
    BOOL bCanWrap = UDS_WRAP & GetStyle();

    // spin down
    if (pUD->iDelta < 0)
    {
        if( m_bFloatingNumber == TRUE )
        {
            // Floating point numbers cannot be compared directly. Because of
            // precession difference this will give wrong result. Epsilon value
            // will be used as a tolerence to nullify this error.
            // Calculate floating point correction value
            double dAbsEps = fabs(defEPS * max (dVal, m_dMinVal));

            if (dAbsEps < defEPS)
            {
                dAbsEps = defEPS;
            }

            // If the value is lesser then the minimun value
            if (m_dMinVal - dVal > dAbsEps) // ~ if (val < m_dMinVal)
            {
                if(bCanWrap == TRUE)
                {
                    // Go to Max End - Wrap Up
                    dVal = m_dMaxVal;
                }
                else
                {
                    // Set the allowed Min value
                    dVal = m_dMinVal;
                }
            }
        }
        else
        {
            // For unsigned number cast it to Unsigned mode
            bInvalid = FALSE;

            if( n64Val < m_n64MinVal )
            {
                if ( bCanWrap == TRUE)
                {
                    // Go to Max End - Wrap Up
                    n64Val = m_n64MaxVal;
                }
                else
                {
                    // Set the allowed Min value
                    n64Val = m_n64MinVal;
                }
            }
        }
    }
    else  // spin up
    {
        if( m_bFloatingNumber == TRUE )
        {
            // Calculate Tolerence value
            double dAbsEps = fabs(defEPS * max (dVal, m_dMaxVal));

            if (dAbsEps < defEPS)
            {
                dAbsEps = defEPS;
            }

            // If the value is greater then the maximum value
            if (dVal - m_dMaxVal > dAbsEps)   //if (val > m_dMaxVal)
            {
                if( bCanWrap == TRUE )
                {
                    dVal = m_dMinVal;
                }
                else
                {
                    dVal = m_dMaxVal;
                }
            }
        }
        else
        {
            // For unsigned number cast it to Unsigned mode
            bInvalid = FALSE;

            if( n64Val > m_n64MaxVal )
            {
                if( bCanWrap == TRUE )
                {
                    n64Val = m_n64MinVal;
                }
                else
                {
                    n64Val = m_n64MaxVal;
                }
            }
        }
    }

    // Set the text value in Buddy control
    if( m_bFloatingNumber == TRUE )
    {
        vSetValueForBuddy (dVal);
    }
    else
    {
        vSetValueForBuddy( n64Val );
    }

    *pResult = 0;
    // let parent process this notification too.
    return FALSE;
}

/******************************************************************************
 Function Name    : OnCreate
 Input(s)         : LPCREATESTRUCT
 Output           : int
 Functionality    : Called by the framework while a window is being created.
                    This will init the spin control
 Member of        : CNumSpinCtrl
 Friend of        : -
 Author(s)        : Raja N
 Date Created     : 22.07.2004
*******************************************************************************/
int CNumSpinCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    int nReturn;
    nReturn = CSpinButtonCtrl::OnCreate(lpCreateStruct);

    if( nReturn != -1)
    {
        // Update the spin control parameters
        vInitSpinCtrl();
    }

    return nReturn;
}

/******************************************************************************
 Function Name    : PreSubclassWindow
 Input(s)         : -
 Output           : -
 Functionality    : Called by the framework while the control is beeing created
                    through dialog. This will init the sping control
 Member of        : CNumSpinCtrl
 Friend of        : -
 Author(s)        : Raja N
 Date Created     : 22.07.2004
*******************************************************************************/
void CNumSpinCtrl::PreSubclassWindow()
{
    CSpinButtonCtrl::PreSubclassWindow();
    // Update the spin control parameters
    vInitSpinCtrl();
}

/******************************************************************************
 Function Name    : vInitSpinCtrl
 Input(s)         : -
 Output           : -
 Functionality    : This will init the sping control. Also will check the
                    SETBUDDYINT property
 Member of        : CNumSpinCtrl
 Friend of        : -
 Author(s)        : Raja N
 Date Created     : 22.07.2004
*******************************************************************************/
void CNumSpinCtrl::vInitSpinCtrl()
{
    // SETBUDDYINT option sets the text directly to the text box. This will
    // overlap with customized texxt set by this control
    ASSERT ((GetStyle () & UDS_SETBUDDYINT) != UDS_SETBUDDYINT);

    // set default values
    // Call approp. initialiser based on float support
    if( m_bFloatingNumber == TRUE )
    {
        vSetRangeAndDelta (m_dMinVal, m_dMaxVal, m_dDelta);
    }
    else
    {
        vSetRangeAndDelta (m_n64MinVal, m_n64MaxVal, m_n64Delta);
    }
}

/******************************************************************************
 Function Name    : vSetFormatString
 Input(s)         : lpszFormatString - Format Specifier String
 Output           : -
 Functionality    : This sets the format of the text. This follows the standard
                    format specifier representation like %d, %g,...
 Member of        : CNumSpinCtrl
 Friend of        : -
 Author(s)        : Raja N
 Date Created     : 22.07.2004
*******************************************************************************/
void CNumSpinCtrl::vSetFormatString (LPCTSTR lpszFormatString /*= NULL*/)
{
    // Clear the format specifier string
    if (lpszFormatString == NULL)
    {
        m_omStrFormat.Empty ();
    }
    else
    {
        m_omStrFormat = lpszFormatString;
    }
}

/******************************************************************************
 Function Name    : vSetDataSize
 Input(s)         : nSize - Size of the data in Bits
 Output           : -
 Functionality    : This sets the size of the data in bits. This is required if
                    hex mode is used and negative numbers are displayed. This is
                    used to get 2s complement value and to mask unwanted portion
                    of data.
 Member of        : CNumSpinCtrl
 Friend of        : -
 Author(s)        : Raja N
 Date Created     : 22.07.2004
*******************************************************************************/
void CNumSpinCtrl::vSetDataSize(short nSize)
{
    m_nDataLength = nSize;
}

/*******************************************************************************
 Function Name    : vSetSigned
 Input(s)         : bIsSigned - Type of the date Signed or Unsigned
 Output           : -
 Functionality    : This sets the data type. Data type will be refered to get
                    the value from the text.
 Member of        : CNumSpinCtrl
 Friend of        : -
 Author(s)        : Raja N
 Date Created     : 30.07.2004
*******************************************************************************/
void CNumSpinCtrl::vSetSigned(BOOL bIsSigned)
{
    m_bSigned = bIsSigned;
}

/*******************************************************************************
 Function Name    : vSetFloatNumber
 Input(s)         : bIsFloat - Type of the date integer(Int64) or float(double)
 Output           : -
 Functionality    : This sets the data type. Data type will be refered to get
                    the value from the text.
 Member of        : CNumSpinCtrl
 Friend of        : -
 Author(s)        : Raja N
 Date Created     : 30.07.2004
*******************************************************************************/
void CNumSpinCtrl::vSetFloatNumber(BOOL bIsFloat)
{
    m_bFloatingNumber = bIsFloat;
}