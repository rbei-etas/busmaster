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
 * \file      ChangeRegDefines.h
 * \brief     This header file contains the hash define and micro     
 * \authors   Amitesh Bharti, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the hash define and micro     
 */
#if !defined Include_Change_Register_Defines_Header
#define Include_Change_Register_Defines_Header

#define  defSECTION_CHANGE_REG       "Change Registers"
#define  defITEM_BAUD_RATE           "Baud Rate"
#define  defITEM_ACCEPTANCE_MASK     "Acceptance Mask"
#define  defITEM_ACCEPTANCE_CODE     "Acceptance Code"
#define  defITEM_BTR0                "Bus Timer Register0"
#define  defITEM_BTR1                "Bus Timer Register1"
#define  defITEM_SAMPLING            "Sampling"
#define  defITEM_CLOCK               "Clock Frequency"
#define  defITEM_BRP                 "Baud Rate Prescalar"
#define  defITEM_INDEX_UNDERFOCUS    "Item Number Under Focus"

#define  defITEM_BTR                 "BTR0 and BTR1"

#define  defDEFAUT_BAUDRATE           0x4716

#define  defHEX_STRING                _T("0x")
#define  defEMPTY_STRING              _T("")
#define  defBAUD_RATE_MESSAGE         _T("Nearest Valid Baud Rate is %s kBit/s. Do You want to change it ?")
#define  defVALIDATION_MSG_BAUD_RATE  _T("Not a valid Decimal Numerals or out of range. !!! \n\nEnter only positive Decimal Numerals upto 1000kbps.")
#define  defVALIDATION_MESSAGE        _T("Not a valid value. !!! \n\nEnter only Hexadecimal Value.")
#define  defERRORMSG_INSERT           _T("Error In Inserting SubItems")

#define  defBRT0_COLUNM_POS              1
#define  defBRT1_COLUNM_POS              2
#define  defSAMPLING_COLUNM_POS          3
#define  defNBT_COLUNM_POS               4
#define  defSJW_COLUNM_POS               5

#define  defSUBITEM_NOT_SELECTED         0
#define  defZERO                         0
#define  defVALID_LENGTH_BTR             2
#define  defVALID_DECIMAL_DIGIT          3
#define  defVALID_LENGTH_ACCEPTANCE      4

#define  defSUCCESS                      1
#define  defERROR                       -1

#define  defFromKillFocusBTR            -2

#define  defFACT_BAUD_RATE               1000.0
#define  defFACT_FREQUENCY               1000000.0
#define  defNUMBER_OF_COLUMNS_CAN_ICS_neoVI 6
#define  defNUMBER_OF_COLUMNS            5
// Maximum list of values can be displayed in list box
#define  defREG_VALUE_LIST_COUNT_MAX     200
#define  defSHIFT_LEFT_SJW_POS            6
#define  defSHIFT_LEFT_TSEG2_POS          4
#define  defSHIFT_LEFT_SAM_POS            7

#define  defSET                           1
#define  defRESET                         0

#define  defHEXADECIMAL                   16
#define  defWARNING_LIMIT_MIN             1
#define  defWARNING_LIMIT_MAX             127
#define defWARNINGLIMIT_MSG _T("The Warning limit \"%s\" is beyond the limit %d - %d")
// Allowed valid decimal values in NBT and BRP product
#define  defVALID_DECIMAL_VALUE          0.004
// Maximum size of Column Headers Displayed in list box
#define  defMAX_COLUMN_SIZE           100  
                                                     
// Maximum value of SJW    = SJWbit + 1 ( BTR0 :6-7 )
#define  defMAX_SJW                    4  
// Maximum value of NBT    = TSEG2+TSEG1+1
#define  defMAX_NBT                    25 
// Maximum value of BRP    = BRPbit + 1 ( BTR0 :0-5 )
#define  defMAX_BRP                    64 


// Maximum value of TSEG1   = TSEG1bit + 1 ( BTR1 : 0-3)
#define  defMAX_TSEG1                      16 
#define defMAX_TSEG1_ICSneoVI               8
#define defMIN_TSEG1_ICSneoVI               1
#define  defMAX_SAMPLING                   50 

// Maximum value of TSEG2   = TSEG2bit + 1 ( BTR1 : 4-6)
#define  defMAX_TSEG2                      8  
  // Minimum value of SJW   = SJWbit + 1 ( BTR0 :6-7 )
#define  defMIN_SJW                    1
 // Minimum value of NBT    = TSEG2+TSEG1+1
#define  defMIN_NBT                    8 
// Minimum value of   BRP   = BRPbit + 1 ( BTR0 :0-5 )
#define  defMIN_BRP                       1
// Minimum value of TSEG1   = TSEG1bit + 1 ( BTR1 : 0-3)
#define  defMIN_TSEG1                      3  
// Minimum vaue of Sampling point = ( (NBT-TSEG2)/NBT ) * 100
#define  defMIN_SAMPLING                   50
 // Minimum value of TSEG2  = TSEG2bit + 1 ( BTR1 : 4-6)
#define  defMIN_TSEG2                  2 

 // Find Max of two numbers
#define  defmcMAX2(X,Y)    ( ( X>Y ) ? X:Y ) 
 // Find Min of two numbers
#define  defmcMIN2(X,Y)    ( ( X<Y ) ? X:Y ) 
// Find Max of three numbers
#define  defmcMAX3(X,Y,Z)  ( ( ( ( X>Y ) ? X:Y ) > Z ) ? ( ( X>Y ) ? X:Y ) : Z ) 
// Find Min of three numbers
#define  defmcMIN3(X,Y,Z)  ( ( ( ( X<Y ) ? X:Y ) < Z ) ? ( ( X<Y ) ? X:Y ) : Z )
// Round to next interger value if decimal value is >=0.5 else remove decimal value.
#define  defmcROUND5(X)        X - (unsigned int)X >= 0.5 ? (unsigned int)(X+0.5):(unsigned int)(X)



#endif