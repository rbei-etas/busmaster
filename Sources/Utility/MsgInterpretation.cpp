/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/**
* \file MsgInterpretation.cpp
* \brief This file contain the definition member function of
* \author Amitesh Bharti
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*
* This file contain the definition member function of
*/
#include "Utils_stdafx.h" // Standard header definition file
#include "MsgInterpretation.h" // Class definition file
#include "UtilFunctions.h" // For all utility functions implementation
#include "include/utils_macro.h"

//Later put in a different file
#define defSTR_FORMAT_PHY_VALUE_WITH_UNIT_FOR_IPT _T("%-16s %s")
#define defMAX_BITS 64
#define defBITS_IN_BYTE 8
#define defMAX_BYTE 8
#define defBITS_IN_FOUR_BYTE 32
#define defSIZE_OF_ERROR_BUFFER 1024


#define CHAR_BOOL 'B'
#define CHAR_UINT 'U'
#define CHAR_INT 'I'
#define defMSGID_RTR 'r'
#define defMSGID_STD 's'
#define defMSGID_EXTENDED 'x'
#define defEMPTY_CHAR ' '
#define defFORMAT_DATA_DECIMAL _T("%03d")
#define defFORMAT_DATA_HEX _T("%02X")
#define defFORMAT_MSGID_DECIMAL _T("%d")
#define defFORMAT_INT64_DECIMAL _T("%I64d")
#define defFORMAT_INT64_HEX _T("%I64X")
#define defFORMAT_MSGID_HEX _T("0x%X")
#define defFORMAT_MSGID_HEX_STR _T("%X")
#define defFORMAT_DATA_FLOAT _T("%f")
#define defNUMBER_OF_BIT_TO_SHIFT 3
#define defSTR_FORMAT_PHY_VALUE _T("%.3f")
#define STR_EMPTY ""

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/******************************************************************************/
/* Function Name : CMsgInterpretation */
/* */
/* Input(s) : */
/* Output : */
/* Functionality : Constructor
/* Member of : CMsgInterpretation */
/* Friend of : - */
/* */
/* Author(s) : Amarnath Shastry */
/* Date Created : 20.03.2002 */
/* Modifications :
/******************************************************************************/
CMsgInterpretation::CMsgInterpretation()
{
    m_eNumFormat = HEXADECIMAL;
    m_psMsgRoot = NULL;
}
/******************************************************************************/
/* Function Name : ~CMsgInterpretation */
/* */
/* Input(s) : */
/* Output : */
/* Functionality : Destructor
/* Member of : CMsgInterpretation */
/* Friend of : - */
/* */
/* Author(s) : Amarnath Shastry */
/* Date Created : 20.03.2002 */
/* Modifications :
/******************************************************************************/
CMsgInterpretation::~CMsgInterpretation()
{
    m_psMsgRoot = NULL;
}

void CMsgInterpretation::vSetMessageList(SMSGENTRY* psCurrMsgEntry)
{
    m_psMsgRoot = psCurrMsgEntry;
}

int CMsgInterpretation::nGetSignalCount(CString strMsgName)
{
	int iSignalCount = 0;
	SMSGENTRY*  m_psMsgTemp = m_psMsgRoot;
	while(m_psMsgTemp != NULL)
	{
		if(m_psMsgTemp->m_psMsg != NULL)
		{
			//check for same message name
			if(strMsgName.CompareNoCase(m_psMsgTemp->m_psMsg->m_omStrMessageName)==0 )
			{
				sSIGNALS* m_psSignalTemp = m_psMsgTemp->m_psMsg->m_psSignals;
				while(m_psSignalTemp != NULL)
				{
					iSignalCount++; //increment the signal count
					m_psSignalTemp = m_psSignalTemp->m_psNextSignalList;
				}
				break;
			}
			else //go for next message name
			{
				m_psMsgTemp = m_psMsgTemp->m_psNext; 
			}
		}
	}
	//return the number of signals in the given message.
	return iSignalCount;
}

void CMsgInterpretation::vSetCurrNumMode(EFORMAT eNumFormat)
{
    m_eNumFormat = eNumFormat;
}

UINT static nGetNoOfBytesToRead(UINT nBitNum, UINT nSigLen)
{
    ASSERT(nSigLen > 0);
    UINT nBytesToRead = 1; //Consider first byte
    INT nRemainingLength = nSigLen - (defBITS_IN_BYTE - nBitNum);

    if (nRemainingLength > 0)
    {
        // Add te number of bytes totally it consumes.
        nBytesToRead += (INT)(nRemainingLength / defBITS_IN_BYTE);
        // Check for extra bits which traverse to the next byte.
        INT nTotalBitsConsidered = ((nBytesToRead - 1) * defBITS_IN_BYTE) +
                                   (defBITS_IN_BYTE - nBitNum);

        if (nTotalBitsConsidered < (INT)nSigLen)
        {
            nBytesToRead++;
        }
    }

    return nBytesToRead;
}
/* checks whether signal cross array boundary or not */
static BOOL bValidateSignal(UINT nDLC, UINT nByteNum, UINT nBitNum, UINT nLength, BOOL bDataFormat)
{
    BOOL bValid = TRUE;
    UINT nBytesToRead = nGetNoOfBytesToRead(nBitNum, nLength);
    bValid = (bDataFormat == DATA_FORMAT_INTEL)?
             (INT)(nByteNum + nBytesToRead - 1) <= nDLC :
             (INT)(nByteNum - nBytesToRead) >= 0;
    return bValid;
}
/*******************************************************************************
Function Name : n64GetSignalValue
Input(s) : BYTE byMsgByteVal,
UINT unBitNum,
UINT unLength,
Return : 64 bit signal value
Functionality : Helper Function calculates the signal value represented
in bits. It takes care of motorola and intel byte order.
Member of : CMsgInterpretation
Friend of : -
Author(s) : Pradeep Kadoor.
Date Created : 09/07/2010.
*******************************************************************************/
__int64 static n64GetSignalValueInBits(register CByteArray* pMsgArray,
                                       UINT byteNumber,
                                       UINT unBitNum,
                                       UINT unLength,
                                       EFORMAT_DATA bByteOrder)
{
    __int64 nSigValueInBits = 0;

    if(unLength !=0 && pMsgArray != NULL)
    {
        UINT nBytesToRead = 0;
        /* Find out how many data bytes the signal consumes */
        nBytesToRead = nGetNoOfBytesToRead(unBitNum, unLength);
        /* Whether the format is Intel or Motorola reading bits inside
        a byte is always same */
        UINT CurrBitNum = unBitNum;
        /* If Byte order is motorola then Bytes have to be read in
        reverse order */
        INT nByteOrder = (bByteOrder == DATA_FORMAT_INTEL)? 1: -1;
        BOOL bValid = bValidateSignal((UINT)pMsgArray->GetSize(), byteNumber, unBitNum, unLength, nByteOrder);
        ASSERT(bValid == TRUE);

        if (bValid == TRUE)
        {
            UINT nBitsRead = 0;

            for (register UINT i = 0; i < nBytesToRead; i++)
            {
                /* Please note: If byte number is byteNumber then array index = byteNumber - 1 */
                BYTE byMsgByteVal = pMsgArray->GetAt( (byteNumber - 1) + (nByteOrder * i) );

                if (CurrBitNum != 0)
                {
                    byMsgByteVal >>= CurrBitNum;
                }

                /* Find out how bits to read from the current byte */
                UINT nCurrBitsToRead = min (defBITS_IN_BYTE - CurrBitNum, unLength - nBitsRead);
                /*After the reading first byte reading will be always from
                byte's start index. So reset the CurrBitNum */
                CurrBitNum = 0;
                BYTE byMask = 0;
                byMask = (BYTE)(pow((float) 2.0, (int) nCurrBitsToRead) - 1);
                //Update the signal value
                nSigValueInBits |= (byMsgByteVal & byMask) << nBitsRead;
                nBitsRead += nCurrBitsToRead;
            }
        }
    }

    return nSigValueInBits;
}
/*******************************************************************************
Function Name : n64GetSignalValue
Input(s) : BYTE byMsgByteVal,
UINT unBitNum,
UINT unLength,
BYTE &bySigVal
Output : BYTE &bySigVal
Functionality : Function gets the signal value corresponding to
the byte value and bit passed in the parameter
for given message.
Member of : CMsgInterpretation
Friend of : -
Author(s) :
Date Created :
Modifications : Raja N on 01.08.2004, Removed taking 2's complement and
then inverting bits and adding one. Both with bring the
original value back.
*******************************************************************************/
__int64 CMsgInterpretation::n64GetSignalValue(register CByteArray* pMsgArray,
        UINT byteNumber,
        UINT unBitNum,
        UINT unLength,
        BYTE bySigType,
        EFORMAT_DATA bSigDataFormat)
{
    __int64 n64SigVal = 0;
    n64SigVal = n64GetSignalValueInBits(pMsgArray, byteNumber, unBitNum, unLength, bSigDataFormat);

    //*************************** added on 31/05/2002 ****************
    // check for negative numbers
    if(bySigType == CHAR_INT)
    {
        // Extend the sign bit to get signed value
        // Use Utility class instead of global function
        CUtilFunctions::s_vExtendSignBit( n64SigVal, unLength);
    }

    //**************************************************************
    return n64SigVal;
}
/*******************************************************************************
Function Name : vInterpretMsgs
Input(s) : unMsgCode - Message ID
ucData - Message Data
omStrMsgName - Message Name (o/p)
omStrSigNames - Signal Names
omStrRawValues - CAlculated Raw Values
omStrPhyValues - Physical Value with Unit
bHexON - HEX/DEC format
Output : CStringArray &omStrArrayMsgInterpretArray
Functionality : Function translates the message bytes to meaningful signals
It returns Message Name, Signal Names, Raw Values, Physical
Values. The formatting done based on the hex flag.
Member of : CMsgInterpretation
Friend of : -

Author(s) : Raja N
Date Created : 31.03.2004
Modifications : Raja N on 14.04.2004
: Changed thel logic of iteration to minimize unnecessery
loops and to optimize the function
Modifications : Raja N on 22.07.2004
: Changes due to Signal Descriptor assignment to physical
value
Modifications : Raja N on 01.08.2004
: Changes due to code review: removed hard coded values and
break statements.
Modifications : Raja N
10.08.2004, Removed adding Unit if Signal Descriptor present
*******************************************************************************/
BOOL CMsgInterpretation::vInterpretMsgs(UINT unMsgCode,
                                        const UCHAR* ucData,
                                        CString& omStrMsgName,
                                        CStringArray& omStrSigNames,
                                        CStringArray& omStrRawValues,
                                        CStringArray& omStrPhyValues,//with Unit
                                        BOOL bHexON)
{
    BOOL bSuccess = FALSE;
    // Remove all elements in the string array
    omStrRawValues.RemoveAll();
    omStrPhyValues.RemoveAll();
    omStrSigNames.RemoveAll();
    sMESSAGE* pMsgs = NULL;

    if (SMSGENTRY::bGetMsgPtrFromMsgId(m_psMsgRoot, unMsgCode, pMsgs))
    {
        omStrMsgName = pMsgs->m_omStrMessageName;
        CByteArray omMsgByte;
        register UINT unByteNo = 1; // One based Indexing
        register UINT unBitNumber = 0;
        UINT un_NoOfSignals = 0;
        register UINT unSigLength = 0;

        /*Whether it is |Intel or motorola format, Data is fed from\
        1st byte to DLC */
        for ( register UINT nCount = 0;
                nCount < pMsgs->m_unMessageLength;
                nCount++)
        {
            omMsgByte.Add(ucData[nCount]);
        }

        __int64 n64Vaule = 0;

        // loop thro all the message bytes which signifies signal
        while (unByteNo <= pMsgs->m_unMessageLength &&
                un_NoOfSignals <= pMsgs->m_unNumberOfSignals)
        {
            unBitNumber = unSigLength % defBITS_IN_BYTE;
            // One base index
            // Use shift operator to do division by 8
            // number of bits to shift is 3 here
            unByteNo = ( unSigLength >> defNUMBER_OF_BIT_TO_SHIFT ) + 1;
            // Get signal details in a byte
            register sSIGNALS* psSignal =
                pMsgs->m_psSignals;
            // Get corresponding Signal List pointer
            BOOL bSignalFound = FALSE;

            while (psSignal != NULL && bSignalFound == FALSE)
            {
                if ((psSignal->m_byStartBit == unBitNumber) &&
                        (psSignal->m_unStartByte == unByteNo))
                {
                    bSignalFound = TRUE;
                }
                else
                {
                    psSignal = psSignal->m_psNextSignalList;
                }
            }

            // If Signal found at Bit position
            if (psSignal != NULL)
            {
                __int64 n64SigVal = 0;
                CString omSignalValue(STR_EMPTY), omStrTemp(STR_EMPTY);
                BOOL bFoundSignalDesc = FALSE;
                // Get signal value
                n64SigVal = n64GetSignalValue(&omMsgByte,
                                              unByteNo,
                                              unBitNumber,
                                              psSignal->m_unSignalLength,
                                              psSignal->m_bySignalType,
                                              psSignal->m_eFormat);

                // If the mode is Hex then discard unwanter bits
                if( bHexON == TRUE )
                {
                    n64Vaule = n64SigVal;
                    CUtilFunctions::s_vRemoveUnwantedBits( n64Vaule,
                                                           psSignal->m_unSignalLength );
                    omSignalValue.Format( defFORMAT_INT64_HEX,
                                          n64Vaule );
                }
                else
                {
                    omSignalValue.Format( defFORMAT_INT64_DECIMAL,
                                          n64SigVal );
                }

                //Add the Signal Name First
                omStrSigNames.Add(psSignal->m_omStrSignalName);
                // Add the val to Raw Array
                omStrRawValues.Add(omSignalValue);
                // Replace signal value if any related signal
                // descriptor is defined
                CSignalDescVal* pouCurrSignalDesc =
                    psSignal->m_oSignalIDVal;

                while ( pouCurrSignalDesc != NULL &&
                        bFoundSignalDesc == FALSE )
                {
                    if (pouCurrSignalDesc->m_n64SignalVal == n64SigVal )
                    {
                        omStrTemp = pouCurrSignalDesc->
                                    m_omStrSignalDescriptor;
                        bFoundSignalDesc = TRUE;
                    }
                    else
                    {
                        pouCurrSignalDesc = pouCurrSignalDesc->
                                            m_pouNextSignalSignalDescVal;
                    }
                }

                if( bFoundSignalDesc == FALSE )
                {
                    // Calculate Phy Value
                    double dPhysical =
                        static_cast<double>(n64SigVal);
                    dPhysical *= psSignal->m_fSignalFactor;
                    dPhysical += psSignal->m_fSignalOffset;
                    omSignalValue.Format( defSTR_FORMAT_PHY_VALUE,
                                          dPhysical);

                    // If unit is not enpty then only copy the Unit
                    if( psSignal->m_omStrSignalUnit.IsEmpty() != TRUE )
                    {
                        omStrTemp.Format(
                            defSTR_FORMAT_PHY_VALUE_WITH_UNIT_FOR_IPT,
                            omSignalValue, psSignal->m_omStrSignalUnit );
                    }
                    // Else copy the string directly
                    else
                    {
                        omStrTemp = omSignalValue;
                    }
                }

                // Add it to Phy Array
                omStrPhyValues.Add(omStrTemp);
                // Increment Number of Signals Got
                un_NoOfSignals++;
                // Update Signal Length
                unSigLength += psSignal->m_unSignalLength;

                if (
                    unSigLength >=
                    ( pMsgs->m_unMessageLength <<
                      defNUMBER_OF_BIT_TO_SHIFT )
                )
                {
                    un_NoOfSignals = pMsgs->m_unNumberOfSignals;
                }
            }
            else
            {
                // Signal not found at bit position.
                // So increment the pos.
                unSigLength++;
            }
        } // Main While loop for Bytes

        // Set the Flag to TRUE to denote success
        bSuccess = TRUE;
    }

    return bSuccess;
}


/*******************************************************************************
Function Name : vInterpretMsgs
Input(s) : unMsgCode - Message ID
psWatchList - Pointer to Signal Watch Node
ucData - Message Data
omStrMsgName - Message Name (o/p)
omStrSigNames - Signal Names
omStrRawValues - CAlculated Raw Values
omStrPhyValues - Physical Value with Unit
bHexON - HEX/DEC format
Output : CStringArray &omStrArrayMsgInterpretArray
Functionality : Function translates the message bytes to meaningful signals
It returns Message Name, Signal Names, Raw Values, Physical
Values. The formatting done based on the hex flag. This is
used to calculate only the signals that are in the signal
watch list.
Member of : CMsgInterpretation
Friend of : -

Author(s) : Raja N
Date Created : 31.03.2004
Modifications : Raja N on 22.07.2004
: Changes due to Signal Descriptor assignment to physical
value
Modifications : Raja N on 01.08.2004
: Changes due to code review: removed hard coded values and
break statements.
Modifications : Raja N
10.08.2004, Removed adding Unit if Signal Descriptor present
*******************************************************************************/
BOOL CMsgInterpretation::bInterpretMsgSigList(UINT unMsgCode,
        const UCHAR* ucData,
        CString& omStrMsgName,
        CStringArray& omStrSigNames,
        CStringArray& omStrRawValues,
        CStringArray& omStrPhyValues,//with Unit
        BOOL bHexON)
{
    BOOL bSuccess = FALSE;
    // Remove all elements in the string array
    omStrRawValues.RemoveAll();
    omStrPhyValues.RemoveAll();
    omStrSigNames.RemoveAll();
    // Get message name from msg code
    sMESSAGE* pMsgs = NULL;

    if (SMSGENTRY::bGetMsgPtrFromMsgId(m_psMsgRoot, unMsgCode, pMsgs))
    {
        omStrMsgName = pMsgs->m_omStrMessageName;
        CByteArray omMsgByte;

        /*Whether it is Intel or Motorola format, Data is fed from\
        1st byte to DLC */
        for ( register UINT nCount = 0;
                nCount < pMsgs->m_unMessageLength;
                nCount++)
        {
            omMsgByte.Add(ucData[nCount]);
        }

        register sSIGNALS* psSignal = pMsgs->m_psSignals;

        // Get corresponding Signal List pointer
        if(psSignal != NULL)
        {
            bSuccess = TRUE;
            __int64 n64Vaule = 0;
            register sSIGNALS* psCurrentSignal = psSignal;

            while(psCurrentSignal != NULL)
            {
                __int64 n64SigVal = 0;
                CString omSignalValue(STR_EMPTY);
                CString omStrTemp(STR_EMPTY);
                BOOL bFound = FALSE;
                // Get signal value
                n64SigVal = n64GetSignalValue(&omMsgByte,
                                              psCurrentSignal->m_unStartByte,
                                              psCurrentSignal->m_byStartBit,
                                              psCurrentSignal->m_unSignalLength,
                                              psCurrentSignal->m_bySignalType,
                                              psCurrentSignal->m_eFormat);

                // If the mode is Hex then discard unwanter bits
                if( bHexON == TRUE )
                {
                    n64Vaule = n64SigVal;
                    CUtilFunctions::s_vRemoveUnwantedBits(
                        n64Vaule,
                        psCurrentSignal->m_unSignalLength );
                    omSignalValue.Format( defFORMAT_INT64_HEX,
                                          n64Vaule );
                }
                else
                {
                    omSignalValue.Format(
                        defFORMAT_INT64_DECIMAL,
                        n64SigVal );
                }

                //Add the Signal Name First
                omStrSigNames.Add(psCurrentSignal->m_omStrSignalName);
                // Add the val to Raw Array
                omStrRawValues.Add(omSignalValue);
                // Replace signal value if any related signal
                // descriptor is defined
                register CSignalDescVal* pouCurrSignalDesc =
                    psCurrentSignal->m_oSignalIDVal;

                // Calculate Phy Value
                while ( pouCurrSignalDesc != NULL &&
                        bFound == FALSE )
                {
                    if (pouCurrSignalDesc->m_n64SignalVal ==
                            n64SigVal )
                    {
                        omStrTemp = pouCurrSignalDesc->
                                    m_omStrSignalDescriptor;
                        bFound = TRUE;
                    }
                    else
                    {
                        pouCurrSignalDesc = pouCurrSignalDesc->
                                            m_pouNextSignalSignalDescVal;
                    }
                }

                // IF not found in the list
                if( bFound == FALSE )
                {
                    // Use Factor and offset to calculate the
                    // physical value
                    double dPhysical =
                        static_cast<double>(n64SigVal);
                    dPhysical *= psCurrentSignal->m_fSignalFactor;
                    dPhysical += psCurrentSignal->m_fSignalOffset;
                    omSignalValue.Format(defSTR_FORMAT_PHY_VALUE,
                                         dPhysical );

                    // If unit is not enpty then only copy the Unit
                    if( psCurrentSignal->m_omStrSignalUnit.IsEmpty() != TRUE )
                    {
                        omStrTemp.Format(
                            defSTR_FORMAT_PHY_VALUE_WITH_UNIT_FOR_IPT,
                            omSignalValue, psCurrentSignal->m_omStrSignalUnit );
                    }
                    // Else copy the string directly
                    else
                    {
                        omStrTemp = omSignalValue;
                    }
                }

                // Add it to Phy Array
                omStrPhyValues.Add(omStrTemp);
                // Break the loop
                psCurrentSignal = psCurrentSignal->m_psNextSignalList;
            } // While Loop
        }// IF Signal pointer loop
    }// If Message Name Loop

    return bSuccess;
}


/*******************************************************************************
Function Name : vInterpretMsgs
Input(s) : unMsgCode - Message ID
ucData - Message Data
omStrMsgName - Message Name (o/p)
omStrSigNames - Signal Names
omStrRawValues - CAlculated Raw Values
omStrPhyValues - Physical Value with Unit
bHexON - HEX/DEC format
Output : CStringArray &omStrArrayMsgInterpretArray
Functionality : Function translates the message bytes to meaningful signals
It returns Message Name, Signal Names, Raw Values, Physical
Values. The formatting done based on the hex flag.
Member of : CMsgInterpretation
Friend of : -

Author(s) : Raja N
Date Created : 31.03.2004
Modifications : Raja N on 14.04.2004
: Changed thel logic of iteration to minimize unnecessery
loops and to optimize the function
Modifications : Raja N on 22.07.2004
: Changes due to Signal Descriptor assignment to physical
value
Modifications : Raja N on 01.08.2004
: Changes due to code review: removed hard coded values and
break statements.
Modifications : Raja N
10.08.2004, Removed adding Unit if Signal Descriptor present
*******************************************************************************/
BOOL CMsgInterpretation::vInterpretMsgs(UINT unMsgCode,
                                        const UCHAR* ucData,
                                        CSignalInfoArray& omSignalInfo )
{
    BOOL bSuccess = FALSE;
    // Get message name from msg code
    sMESSAGE* pMsgs = NULL;
    omSignalInfo.RemoveAll();

    if (SMSGENTRY::bGetMsgPtrFromMsgId(m_psMsgRoot, unMsgCode, pMsgs))
    {
        // Get message pointer from message name for future use
        // Use Msg Code instead of Message Name
        CByteArray omMsgByte;
        register UINT unByteNo = 1; // One based Indexing
        register UINT unBitNumber = 0;
        UINT un_NoOfSignals = 0;
        register UINT unSigLength = 0;
        SINTERPRETSIGNALINFO sSigInfo;

        /*Whether it is |Intel or motorola format, Data is fed from\
        1st byte to DLC */
        for ( register UINT nCount = 0;
                nCount < pMsgs->m_unMessageLength;
                nCount++)
        {
            omMsgByte.Add(ucData[nCount]);
        }

        // loop thro all the message bytes which signifies signal
        while (unByteNo <= pMsgs->m_unMessageLength &&
                un_NoOfSignals <= pMsgs->m_unNumberOfSignals)
        {
            unBitNumber = unSigLength % defBITS_IN_BYTE;
            // One base index
            // Use shift operator to do division by 8
            unByteNo = ( unSigLength >> defNUMBER_OF_BIT_TO_SHIFT ) + 1;
            // Get signal details in a byte
            register sSIGNALS* psSignal =
                pMsgs->m_psSignals;
            // Get corresponding Signal List pointer
            BOOL bSignalFound = FALSE;

            while (psSignal != NULL && bSignalFound == FALSE )
            {
                if ((psSignal->m_byStartBit == unBitNumber) &&
                        (psSignal->m_unStartByte == unByteNo))
                {
                    bSignalFound = TRUE;
                }
                else
                {
                    psSignal = psSignal->m_psNextSignalList;
                }
            }

            // If Signal found at Bit position
            if (psSignal != NULL)
            {
                __int64 n64SigVal = 0;
                CString omSignalValue;
                BOOL bFoundSignalDesc = FALSE;
                // Get signal value
                n64SigVal = n64GetSignalValue(&omMsgByte,
                                              unByteNo,
                                              unBitNumber,
                                              psSignal->m_unSignalLength,
                                              psSignal->m_bySignalType,
                                              psSignal->m_eFormat);
                // Store Raw Value
                sSigInfo.m_un64RawValue = n64SigVal;
                // Store Signal Length
                sSigInfo.m_ucSigLength = (UCHAR)(psSignal->m_unSignalLength);
                // Init Signal Description string
                sSigInfo.m_omStrSignalDescValue = STR_EMPTY;
                // Replace signal value if any related signal
                // descriptor is defined
                CSignalDescVal* pouCurrSignalDesc =
                    psSignal->m_oSignalIDVal;

                while (pouCurrSignalDesc != NULL && bFoundSignalDesc == FALSE)
                {
                    if (pouCurrSignalDesc->m_n64SignalVal ==
                            n64SigVal )
                    {
                        // Store Signal Description
                        sSigInfo.m_omStrSignalDescValue =
                            pouCurrSignalDesc->m_omStrSignalDescriptor;
                        bFoundSignalDesc = TRUE;
                    }
                    else
                    {
                        pouCurrSignalDesc = pouCurrSignalDesc->
                                            m_pouNextSignalSignalDescVal;
                    }
                }

                //Store the Signal Name
                sSigInfo.m_omStrSignalName =
                    psSignal->m_omStrSignalName;
                // Calculate Phy Value only if the Signal Descriptor is
                // not defined
                double dPhysical = static_cast<double>(n64SigVal);

                if( bFoundSignalDesc == FALSE )
                {
                    dPhysical *= psSignal->m_fSignalFactor;
                    dPhysical += psSignal->m_fSignalOffset;
                }

                // Store Phy val and Unit
                sSigInfo.m_dPhyValue = dPhysical;
                sSigInfo.m_omStrUnit = psSignal->m_omStrSignalUnit;
                omSignalInfo.Add( sSigInfo );
                // Increment Number of Signals Got
                un_NoOfSignals++;
                // Update Signal Length
                unSigLength += psSignal->m_unSignalLength;

                if (
                    unSigLength >=
                    (pMsgs->m_unMessageLength << defNUMBER_OF_BIT_TO_SHIFT)
                )
                {
                    un_NoOfSignals = pMsgs->m_unNumberOfSignals;
                }
            }
            else
            {
                // Signal not found at bit position.
                // So increment the pos.
                unSigLength++;
            }
        } // Main While loop for Bytes

        // Set the Flag to TRUE to denote success
        bSuccess = TRUE;
    }

    return bSuccess;
}

/*******************************************************************************
Function Name : bInterpretMsgs
Input(s) : unMsgCode - Message ID
ucData - Message Data
omStrMsgName - Message Name (o/p)
omStrSigNames - Signal Names
omStrRawValues - CAlculated Raw Values
omStrPhyValues - Physical Value with Unit
Output : CStringArray &omStrArrayMsgInterpretArray
Functionality : Function translates the message bytes to meaningful signals
It returns Message Name, Signal Names, Raw Values, Physical
Values. The formatting done based on the hex flag.
Member of : CMsgInterpretation
Friend of : -

Author(s) : Raja N
Date Created : 31.03.2004
Modifications : Raja N on 14.04.2004
: Changed thel logic of iteration to minimize unnecessery
loops and to optimize the function
Modifications : Raja N on 22.07.2004
: Changes due to Signal Descriptor assignment to physical
value
Modifications : Raja N on 01.08.2004
: Changes due to code review: removed hard coded values and
break statements.
Modifications : Raja N
10.08.2004, Removed adding Unit if Signal Descriptor present
*******************************************************************************/
BOOL CMsgInterpretation::bInterpretMsgs(UINT unMsgCode,
                                        const UCHAR* ucData,
                                        CString& omStrMsgName,
                                        CStringArray& omStrSigNames,
                                        CStringArray& omStrRawValues,
                                        CStringArray& omStrPhyValues)
{
    // Remove all elements in the string array
    omStrRawValues.RemoveAll();
    omStrPhyValues.RemoveAll();
    omStrSigNames.RemoveAll();
    sMESSAGE* pMsgs = NULL;

    for (SMSGENTRY* psTmp = m_psMsgRoot; NULL != psTmp; psTmp = psTmp->m_psNext)
    {
        if (psTmp->m_psMsg->m_unMessageCode == unMsgCode)
        {
            pMsgs = psTmp->m_psMsg;
            break;
        }
    }

    if (NULL == pMsgs)
    {
        return FALSE; // Return if the message entry isn't found
    }

    omStrMsgName = pMsgs->m_omStrMessageName;
    CByteArray omMsgByte;

    /*Whether it is |Intel or motorola format, Data is fed from\
    1st byte to DLC */
    for ( register UINT nCount = 0;
            nCount < pMsgs->m_unMessageLength;
            nCount++)
    {
        omMsgByte.Add(ucData[nCount]);
    }

    __int64 n64SigVal = 0;
    sSIGNALS* psCurrSignal = pMsgs->m_psSignals;

    while (NULL != psCurrSignal)
    {
        // Add the Signal Name First
        omStrSigNames.Add(psCurrSignal->m_omStrSignalName);
        // Get signal value
        n64SigVal = n64GetSignalValue(&omMsgByte, psCurrSignal->m_unStartByte,
                                      psCurrSignal->m_byStartBit, psCurrSignal->m_unSignalLength,
                                      psCurrSignal->m_bySignalType, psCurrSignal->m_eFormat);
        CString omSignalValue;
        UINT unSigLen = psCurrSignal->m_unSignalLength;
        //Calculate the character width required to represent the raw value.
        //1 character means 1 nibble
        UINT unWidth = (unSigLen % 4 == 0) ? (unSigLen/4) : (unSigLen/4 + 1);
        //Ignore extra FFs incase of a negative number.
        double dblVal = 16;
        __int64 nWidthMask = 0 | (__int64)(pow(dblVal, (int)unWidth) - 1);
        n64SigVal = n64SigVal & nWidthMask;
        omSignalValue.Format(_T("0x%X"), n64SigVal);
        // Add the value to Raw Array
        omStrRawValues.Add(omSignalValue);
        // Replace signal value if any related signal descriptor is defined
        CSignalDescVal* pouCurrSigDesc = psCurrSignal->m_oSignalIDVal;
        BOOL bFoundDesc = FALSE;
        CString omStrTemp;

        while ((NULL != pouCurrSigDesc) && !bFoundDesc)
        {
            if (pouCurrSigDesc->m_n64SignalVal == n64SigVal)
            {
                omStrTemp = pouCurrSigDesc->m_omStrSignalDescriptor;
                bFoundDesc = TRUE;
            }
            else
            {
                pouCurrSigDesc = pouCurrSigDesc->m_pouNextSignalSignalDescVal;
            }
        }

        if (bFoundDesc == FALSE) // Signal value not defined
        {
            // Calculate Phy Value
            double dPhysical = static_cast<double>(n64SigVal);
            dPhysical *= psCurrSignal->m_fSignalFactor;
            dPhysical += psCurrSignal->m_fSignalOffset;
            omStrTemp.Format(defSTR_FORMAT_PHY_VALUE, dPhysical);

            // If unit is not enpty then only copy the Unit
            if (psCurrSignal->m_omStrSignalUnit.IsEmpty() != TRUE)
            {
                omStrTemp += _T(" ");
                omStrTemp += psCurrSignal->m_omStrSignalUnit;
            }
        }

        omStrPhyValues.Add(omStrTemp); // Add it to physical value Array
        psCurrSignal = psCurrSignal->m_psNextSignalList;
    }

    return TRUE;
}

BOOL CMsgInterpretation::bInterpretMsgs(EFORMAT eNumFormat,
                                        SMSGENTRY* psMsgRoot,
                                        UINT unMsgCode,
                                        const UCHAR* ucData,
                                        CString& omStrMsgName,
                                        SSignalInfoArray& SigInfoArray)
{
    //BOOL bReturn = TRUE;
    // Remove all elements in the target array
    SigInfoArray.RemoveAll();
    sMESSAGE* pMsgs = NULL;

    for (SMSGENTRY* psTmp = psMsgRoot; NULL != psTmp; psTmp = psTmp->m_psNext)
    {
        if (psTmp->m_psMsg->m_unMessageCode == unMsgCode)
        {
            pMsgs = psTmp->m_psMsg;
            break;
        }
    }

    if (NULL == pMsgs)
    {
        return FALSE; // Return if the message entry isn't found
    }

    omStrMsgName = pMsgs->m_omStrMessageName;
    bInterpretMsgs(eNumFormat, pMsgs, ucData, SigInfoArray);
    return TRUE;
}
BOOL CMsgInterpretation::bInterpretMsgs(EFORMAT /*eNumFormat*/,
                                        const sMESSAGE* pMsg,
                                        const UCHAR* ucData,
                                        CSignalInfoArray& omSigInfoArray)
{
    BOOL bReturn = FALSE;
    omSigInfoArray.RemoveAll();

    if ((pMsg != NULL) && (ucData != NULL))
    {
        CByteArray omMsgByte;

        /*Whether it is |Intel or motorola format, Data is fed from\
        1st byte to DLC */
        for ( register UINT nCount = 0;
                nCount < pMsg->m_unMessageLength;
                nCount++)
        {
            omMsgByte.Add(ucData[nCount]);
        }

        SINTERPRETSIGNALINFO sInterSigInfo;
        __int64 n64SigVal = 0;
        sSIGNALS* psCurrSignal = pMsg->m_psSignals;

        while (NULL != psCurrSignal)
        {
            // Add the Signal Name First
            sInterSigInfo.m_omStrSignalName = psCurrSignal->m_omStrSignalName;
            // Get signal value
            n64SigVal = n64GetSignalValue(&omMsgByte, psCurrSignal->m_unStartByte,
                                          psCurrSignal->m_byStartBit, psCurrSignal->m_unSignalLength,
                                          psCurrSignal->m_bySignalType, psCurrSignal->m_eFormat);
            // Calculate the raw value
            UINT unSigLen = psCurrSignal->m_unSignalLength;
            //Calculate the character width required to represent the raw value.
            //1 character means 1 nibble
            //venkat
            /*UINT unWidth = (unSigLen % 4 == 0) ? (unSigLen/4) : (unSigLen/4 + 1);
            //calculate extra FFs incase of a negative number.
            double dblVal = 16;
            __int64 nWidthMask = 0 | (__int64)(pow(dblVal, (int)unWidth) - 1);
            //Mask extra FFs with signal value
            n64SigVal = n64SigVal &  nWidthMask;*/
            sInterSigInfo.m_un64RawValue = n64SigVal;
            sInterSigInfo.m_omStrUnit = psCurrSignal->m_omStrSignalUnit;
            sInterSigInfo.m_ucSigLength = (UCHAR)(psCurrSignal->m_unSignalLength);
            // Replace signal value if any related signal descriptor is defined
            CSignalDescVal* pouCurrSigDesc = psCurrSignal->m_oSignalIDVal;
            BOOL bFoundDesc = FALSE;

            while ((NULL != pouCurrSigDesc) && !bFoundDesc)
            {
                if (pouCurrSigDesc->m_n64SignalVal == n64SigVal)
                {
                    sInterSigInfo.m_omStrSignalDescValue = pouCurrSigDesc->m_omStrSignalDescriptor;
                    bFoundDesc = TRUE;
                }
                else
                {
                    pouCurrSigDesc = pouCurrSigDesc->m_pouNextSignalSignalDescVal;
                }
            }

            // Calculate engineering value
            double dPhysical = static_cast<double>(n64SigVal);
            dPhysical *= psCurrSignal->m_fSignalFactor;
            dPhysical += psCurrSignal->m_fSignalOffset;
            sInterSigInfo.m_dPhyValue = dPhysical;
            omSigInfoArray.Add(sInterSigInfo); // Finally add it to the target array
            psCurrSignal = psCurrSignal->m_psNextSignalList;
        }
    }

    if (0 < omSigInfoArray.GetSize())
    {
        bReturn = TRUE;
    }

    return bReturn;
}

BOOL CMsgInterpretation::bInterpretMsgs(EFORMAT eNumFormat,
                                        const sMESSAGE* pMsg,
                                        const UCHAR* ucData,
                                        SSignalInfoArray& SigInfoArray)
{
    BOOL bReturn = FALSE;
    SigInfoArray.RemoveAll();

    if ((pMsg != NULL) && (ucData != NULL))
    {
        CByteArray omMsgByte;

        /*Whether it is |Intel or motorola format, Data is fed from\
        1st byte to DLC */
        for ( register UINT nCount = 0;
                nCount < pMsg->m_unMessageLength;
                nCount++)
        {
            omMsgByte.Add(ucData[nCount]);
        }

        SSignalInfo sSigInfoTmp;
        __int64 n64SigVal = 0;
        sSIGNALS* psCurrSignal = pMsg->m_psSignals;

        while (NULL != psCurrSignal)
        {
            // Add the Signal Name First
            sSigInfoTmp.m_omSigName = psCurrSignal->m_omStrSignalName;
            // Get signal value
            n64SigVal = n64GetSignalValue(&omMsgByte, psCurrSignal->m_unStartByte,
                                          psCurrSignal->m_byStartBit, psCurrSignal->m_unSignalLength,
                                          psCurrSignal->m_bySignalType, psCurrSignal->m_eFormat);
            // Calculate the raw value
            UINT unSigLen = psCurrSignal->m_unSignalLength;
            //Calculate the character width required to represent the raw value.
            //1 character means 1 nibble
            /*UINT unWidth = (unSigLen % 4 == 0) ? (unSigLen/4) : (unSigLen/4 + 1);
            //calculate extra FFs incase of a negative number.
            double dblVal = 16;
            __int64 nWidthMask = 0 | (__int64)(pow(dblVal, (int)unWidth) - 1);
            //Mask extra FFs with signal value
            n64SigVal = n64SigVal &  nWidthMask;*/

            if(eNumFormat == HEXADECIMAL)
            {
                sSigInfoTmp.m_omRawValue.Format(_T("0x%X"), n64SigVal);
            }
            else
            {
                sSigInfoTmp.m_omRawValue.Format(_T("%d"), n64SigVal);
            }

            // Assign the unit, if there is any
            sSigInfoTmp.m_omUnit = psCurrSignal->m_omStrSignalUnit;
            // Replace signal value if any related signal descriptor is defined
            CSignalDescVal* pouCurrSigDesc = psCurrSignal->m_oSignalIDVal;
            BOOL bFoundDesc = FALSE;

            while ((NULL != pouCurrSigDesc) && !bFoundDesc)
            {
                if (pouCurrSigDesc->m_n64SignalVal == n64SigVal)
                {
                    sSigInfoTmp.m_omEnggValue = pouCurrSigDesc->m_omStrSignalDescriptor;
                    bFoundDesc = TRUE;
                }
                else
                {
                    pouCurrSigDesc = pouCurrSigDesc->m_pouNextSignalSignalDescVal;
                }
            }

            if (bFoundDesc == FALSE) // Signal value not defined
            {
                // Calculate engineering value
                double dPhysical = static_cast<double>(n64SigVal);
                dPhysical *= psCurrSignal->m_fSignalFactor;
                dPhysical += psCurrSignal->m_fSignalOffset;
                sSigInfoTmp.m_omEnggValue.Format(defSTR_FORMAT_PHY_VALUE, dPhysical);
            }

            SigInfoArray.Add(sSigInfoTmp); // Finally add it to the target array
            psCurrSignal = psCurrSignal->m_psNextSignalList;
        }
    }

    if (0 < SigInfoArray.GetSize())
    {
        bReturn = TRUE;
    }

    return bReturn;
}

void CMsgInterpretation::vClear()
{
    if(NULL != m_psMsgRoot)
         SMSGENTRY::vClearMsgList(m_psMsgRoot);
}

void CMsgInterpretation::vCopy(CMsgInterpretation* pDest) const
{
    if ( pDest != NULL)
    {
        pDest->vClear();
        pDest->m_eNumFormat = m_eNumFormat;
        //Update the Msg list
        SMSGENTRY* pDestRoot = NULL;
        SMSGENTRY* pTempSrc = m_psMsgRoot;

        while (pTempSrc != NULL)
        {
            SMSGENTRY::bUpdateMsgList(pDestRoot, pTempSrc->m_psMsg);
            pTempSrc = pTempSrc->m_psNext;
        }

        pDest->vSetMessageList(pDestRoot);
    }
}

/* IMPLEMENTATION CLASS CMsgInterpretationJ1939 STARTS */

CMsgInterpretationJ1939::CMsgInterpretationJ1939()
{
    m_psMsgRoot = NULL;
}

CMsgInterpretationJ1939::~CMsgInterpretationJ1939()
{
    if(NULL != m_psMsgRoot)
         SMSGENTRY::vClearMsgList(m_psMsgRoot);
}

BOOL CMsgInterpretationJ1939::bInterPretJ1939_MSGS(
    EFORMAT eNumFormat,
    UINT32 unPGN,
    UINT unDLC,
    BYTE* pbyData,
    CString& omMsgName,
    SSignalInfoArray& odSigInfoArray)
{
    BOOL bReturn = FALSE;
    sMESSAGE* pMsg = NULL;
    odSigInfoArray.RemoveAll();
    SMSGENTRY::bGetMsgPtrFromMsgId(m_psMsgRoot, unPGN, pMsg);

    if ((pMsg != NULL) && (pbyData != NULL) && (unDLC >= pMsg->m_unMessageLength))
    {
        omMsgName = pMsg->m_omStrMessageName;
        CByteArray omMsgByte;

        /*Whether it is |Intel or motorola format, Data is fed from\
        1st byte to DLC */
        for ( register UINT nCount = 0;
                nCount < pMsg->m_unMessageLength;
                nCount++)
        {
            omMsgByte.Add(pbyData[nCount]);
        }

        SSignalInfo sSigInfoTmp;
        __int64 n64SigVal = 0;
        sSIGNALS* psCurrSignal = pMsg->m_psSignals;

        while (NULL != psCurrSignal)
        {
            // Add the Signal Name First
            sSigInfoTmp.m_omSigName = psCurrSignal->m_omStrSignalName;
            // Get signal value
            n64SigVal = n64GetSignalValue(&omMsgByte, psCurrSignal->m_unStartByte,
                                          psCurrSignal->m_byStartBit, psCurrSignal->m_unSignalLength,
                                          psCurrSignal->m_bySignalType, psCurrSignal->m_eFormat);
            // Calculate the raw value
            UINT unSigLen = psCurrSignal->m_unSignalLength;
            //Calculate the character width required to represent the raw value.
            //1 character means 1 nibble
            UINT unWidth = (unSigLen % 4 == 0) ? (unSigLen/4) : (unSigLen/4 + 1);
            //calculate extra FFs incase of a negative number.
            __int64 nWidthMask = 0 | (__int64)(pow((float) 16.0, (int) unWidth) - 1);
            //Mask extra FFs with signal value
            n64SigVal = n64SigVal & nWidthMask;

            if(eNumFormat == HEXADECIMAL)
            {
                sSigInfoTmp.m_omRawValue.Format(_T("0x%X"), n64SigVal);
            }
            else
            {
                sSigInfoTmp.m_omRawValue.Format(_T("%d"), n64SigVal);
            }

            // Assign the unit, if there is any
            sSigInfoTmp.m_omUnit = psCurrSignal->m_omStrSignalUnit;
            // Replace signal value if any related signal descriptor is defined
            CSignalDescVal* pouCurrSigDesc = psCurrSignal->m_oSignalIDVal;
            BOOL bFoundDesc = FALSE;

            while ((NULL != pouCurrSigDesc) && !bFoundDesc)
            {
                if (pouCurrSigDesc->m_n64SignalVal == n64SigVal)
                {
                    sSigInfoTmp.m_omEnggValue = pouCurrSigDesc->m_omStrSignalDescriptor;
                    bFoundDesc = TRUE;
                }
                else
                {
                    pouCurrSigDesc = pouCurrSigDesc->m_pouNextSignalSignalDescVal;
                }
            }

            if (bFoundDesc == FALSE) // Signal value not defined
            {
                // Calculate engineering value
                double dPhysical = static_cast<double>(n64SigVal);
                dPhysical *= psCurrSignal->m_fSignalFactor;
                dPhysical += psCurrSignal->m_fSignalOffset;
                sSigInfoTmp.m_omEnggValue.Format(defSTR_FORMAT_PHY_VALUE, dPhysical);
            }

            odSigInfoArray.Add(sSigInfoTmp); // Finally add it to the target array
            psCurrSignal = psCurrSignal->m_psNextSignalList;
        }
    }

    if (0 < odSigInfoArray.GetSize())
    {
        bReturn = TRUE;
    }

    return bReturn;
}

void CMsgInterpretationJ1939::vSetJ1939Database(const SMSGENTRY* psCurrMsgEntry)
{
    //go for updated message configuration.
    while (m_psMsgRoot != NULL)
    {
        m_psMsgRoot = m_psMsgRoot->m_psNext;
    }
    if (psCurrMsgEntry != NULL)
    {
        const SMSGENTRY* psMsgEntry = psCurrMsgEntry;

        while (psMsgEntry != NULL)
        {
            SMSGENTRY::bUpdateMsgList(m_psMsgRoot, psMsgEntry->m_psMsg);
            psMsgEntry = psMsgEntry->m_psNext;
        }
    }
}

int CMsgInterpretationJ1939::nGetJ1939SignalCount(CString strMsgName)
{
	int iSignalCount = 0;
	SMSGENTRY*  m_psMsgTemp = m_psMsgRoot;
	while(m_psMsgTemp != NULL)
	{
		if(m_psMsgTemp->m_psMsg != NULL)
		{
			//check for same message name
			if(strMsgName.CompareNoCase(m_psMsgTemp->m_psMsg->m_omStrMessageName)==0 )
			{
				sSIGNALS* m_psSignalTemp = m_psMsgTemp->m_psMsg->m_psSignals;
				while(m_psSignalTemp != NULL)
				{
					iSignalCount++; //increment the signal count
					m_psSignalTemp = m_psSignalTemp->m_psNextSignalList;
				}
				break;
			}
			else
			{
				//go for next message name
				m_psMsgTemp = m_psMsgTemp->m_psNext; 
			}
		}
	}
	//return the number of signals for the given message.
	return iSignalCount;
}

/*******************************************************************************
Function Name : n64GetSignalValue
Input(s) : BYTE byMsgByteVal,
UINT unBitNum,
UINT unLength,
BYTE &bySigVal
Output : BYTE &bySigVal
Functionality : Function gets the signal value corresponding to
the byte value and bit passed in the parameter
for given message.
Member of : CMsgInterpretationJ1939
Friend of : -
Author(s) : Pradeep Kadoor
Date Created :
Modifications :
*******************************************************************************/
__int64 CMsgInterpretationJ1939::n64GetSignalValue(register CByteArray* pMsgArray,
        UINT byteNumber,
        UINT unBitNum,
        UINT unLength,
        BYTE bySigType,
        EFORMAT_DATA bSigDataFormat)
{
    __int64 n64SigVal = 0;
    n64SigVal = n64GetSignalValueInBits(pMsgArray, byteNumber, unBitNum, unLength, bSigDataFormat);

    //*************************** added on 31/05/2002 ****************
    // check for negative numbers
    if(bySigType == CHAR_INT)
    {
        // Extend the sign bit to get signed value
        // Use Utility class instead of global function
        CUtilFunctions::s_vExtendSignBit( n64SigVal, unLength);
    }

    //**************************************************************
    return n64SigVal;
}
/******************************************************************************
Function Name : vClear
Input(s) :
Output :
Functionality :
Member of : CMsgInterpretationJ1939
Friend of : -
Author(s) : Pradeep Kadoor
Date Created : 16/02/2011
Modifications :
******************************************************************************/
void CMsgInterpretationJ1939::vClear()
{
    if(NULL != m_psMsgRoot)
         SMSGENTRY::vClearMsgList(m_psMsgRoot);
}
/******************************************************************************
Function Name : vCopy
Input(s) :
Output :
Functionality :
Member of : CMsgInterpretationJ1939
Friend of : -
Author(s) : Pradeep Kadoor
Date Created : 16/02/2011
Modifications :
******************************************************************************/
void CMsgInterpretationJ1939::vCopy(CMsgInterpretationJ1939* pDest) const
{
    if ( pDest != NULL)
    {
        pDest->vClear();
        pDest->m_eNumFormat = m_eNumFormat;
        //Update the Msg list
        SMSGENTRY* pDestRoot = NULL;
        SMSGENTRY* pTempSrc = m_psMsgRoot;

        while (pTempSrc != NULL)
        {
            SMSGENTRY::bUpdateMsgList(pDestRoot, pTempSrc->m_psMsg);
            pTempSrc = pTempSrc->m_psNext;
        }

        pDest->vSetJ1939Database(pDestRoot);
    }
}


/* IMPLEMENTATION CLASS CMsgInterpretationJ1939 ENDS */
//venkat
BOOL CMsgInterpretation::bInterpretMsgs(UINT unMsgCode,
                                        const UCHAR* ucData,
                                        CSignalInfoArray& omSignalInfo )
{
    BOOL bSuccess = FALSE;
    // Get message name from msg code
    sMESSAGE* pMsgs = NULL;
    omSignalInfo.RemoveAll();

    if (SMSGENTRY::bGetMsgPtrFromMsgId(m_psMsgRoot, unMsgCode, pMsgs))
    {
        // Get message pointer from message name for future use
        // Use Msg Code instead of Message Name
        CByteArray omMsgByte;
        //register UINT unByteNo = 1; // One based Indexing
        //register UINT unBitNumber = 0;
        UINT un_NoOfSignals = 0;
        register UINT unSigLength = 0;
        SINTERPRETSIGNALINFO sSigInfo;

        /*Whether it is |Intel or motorola format, Data is fed from\
        1st byte to DLC */
        for ( register UINT nCount = 0;
                nCount < pMsgs->m_unMessageLength;
                nCount++)
        {
            omMsgByte.Add(ucData[nCount]);
        }

        // Get signal details in a byte
        register sSIGNALS* psSignal = pMsgs->m_psSignals;

        // If Signal found at Bit position
        while (psSignal!= NULL)
        {
            //if (psSignal != NULL)
            //{
            __int64 n64SigVal = 0;
            CString omSignalValue;
            BOOL bFoundSignalDesc = FALSE;
            // Get signal value
            n64SigVal = n64GetSignalValue(&omMsgByte,
                                          psSignal->m_unStartByte,
                                          psSignal->m_byStartBit,
                                          psSignal->m_unSignalLength,
                                          psSignal->m_bySignalType,
                                          psSignal->m_eFormat);
            // Store Raw Value
            sSigInfo.m_un64RawValue = n64SigVal;
            // Store Signal Length
            sSigInfo.m_ucSigLength = (UCHAR)(psSignal->m_unSignalLength);
            // Init Signal Description string
            sSigInfo.m_omStrSignalDescValue = STR_EMPTY;
            // Replace signal value if any related signal
            // descriptor is defined
            CSignalDescVal* pouCurrSignalDesc =
                psSignal->m_oSignalIDVal;

            while (pouCurrSignalDesc != NULL && bFoundSignalDesc == FALSE)
            {
                if (pouCurrSignalDesc->m_n64SignalVal ==
                        n64SigVal )
                {
                    // Store Signal Description
                    sSigInfo.m_omStrSignalDescValue =
                        pouCurrSignalDesc->m_omStrSignalDescriptor;
                    bFoundSignalDesc = TRUE;
                }
                else
                {
                    pouCurrSignalDesc = pouCurrSignalDesc->
                                        m_pouNextSignalSignalDescVal;
                }
            }

            //Store the Signal Name
            sSigInfo.m_omStrSignalName =
                psSignal->m_omStrSignalName;
            // Calculate Phy Value only if the Signal Descriptor is
            // not defined
            double dPhysical = static_cast<double>(n64SigVal);

            if( bFoundSignalDesc == FALSE )
            {
                dPhysical *= psSignal->m_fSignalFactor;
                dPhysical += psSignal->m_fSignalOffset;
            }

            // Store Phy val and Unit
            sSigInfo.m_dPhyValue = dPhysical;
            sSigInfo.m_omStrUnit = psSignal->m_omStrSignalUnit;
            omSignalInfo.Add( sSigInfo );
            // Increment Number of Signals Got
            un_NoOfSignals++;
            // Update Signal Length
            unSigLength += psSignal->m_unSignalLength;

            if (
                unSigLength >=
                (pMsgs->m_unMessageLength << defNUMBER_OF_BIT_TO_SHIFT)
            )
            {
                un_NoOfSignals = pMsgs->m_unNumberOfSignals;
            }

            psSignal = psSignal->m_psNextSignalList;
        }

        bSuccess = TRUE;
    }

    return bSuccess;
}