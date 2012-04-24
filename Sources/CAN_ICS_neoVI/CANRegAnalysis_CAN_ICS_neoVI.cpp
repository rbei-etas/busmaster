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
 * \file      CANRegAnalysis_CAN_ICS_neoVI.cpp
 * \brief     This file contains the function which implements the
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains the function which implements the
 */

// Include all standard header files
#include "CAN_ICS_NeoVI_stdafx.h"
// hash defines for resource ID's
#include "CAN_ICS_NeoVI_resource.h"
// CChangeRegisters class defination file.
#include "ChangeRegisters_CAN_ICS_neoVI.h"

/******************************************************************************/
/*  Function Name    :  nListBoxValues                                        */
/*                                                                            */
/*  Input(s)         :  Baudrate Number of Sample/bit and Clock Frequency     */
/*  Output           :  NBT and BRP values                                    */
/*  Functionality    :  Calculate set of the NBT and BRP value for given      */
/*                      baudrate and clock frequency                          */
/*  Member of        :  CChangeRegisters_CAN_ICS_neoVI                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.02.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  28.05.2002, Type casting changed from BYTE to WORD    */
/******************************************************************************/
BOOL CChangeRegisters_CAN_ICS_neoVI::nListBoxValues(sCOLUMNS* /*psColListCtrl*/,
        DOUBLE dBuadRate, WORD wClockFreq,
        UINT* puwIndex, INT nSample)
{
    UINT    unProductNbtNBrp    = 0;
    DOUBLE  dProductNbtNBrp     = 0;
    INT     nReturn             = -1;
    // Calcualte the product NBT * BRP = clock/(2.0 * baudrate ). This product
    // should be an integer multiple.
    dProductNbtNBrp  = (wClockFreq / dBuadRate) / 2.0 *
                       (defFACT_FREQUENCY / defFACT_BAUD_RATE);
    unProductNbtNBrp = (UINT) (dProductNbtNBrp + 0.5);

    //Check if product is integer multiple. Ignore diffrence <= 0.004
    if (fabs((dProductNbtNBrp - unProductNbtNBrp)) <= defVALID_DECIMAL_VALUE)
    {
        INT   i = 1;
        WORD wNBT  = (WORD) (unProductNbtNBrp / i);
        FLOAT fNBT = (FLOAT) unProductNbtNBrp / i;

        //Calculate all set of NBT and BRP value for a given product of NBT and BRP.
        while (wNBT >= 1 && i <= defMAX_BRP)
        {
            if ((wNBT == fNBT) && (wNBT >= defMIN_NBT) && (wNBT <=defMAX_NBT))
            {
                WORD wBRP = (WORD)(unProductNbtNBrp / wNBT);

                //Call this function to calculate BTR0, BTR1 regsiter value
                // for one set of NBT and BRP.
                if (m_bOption == NO_DEF)
                {
                    nReturn = bCalculateICSneoVIRegValues(wNBT, wBRP, puwIndex, nSample);
                }
                else
                {
                    sBRP_NBT_SAMP_n_SJW CurrEntry;
                    CurrEntry.usBRP = (WORD)(unProductNbtNBrp / wNBT);
                    CurrEntry.usNBT = wNBT;
                    CurrEntry.usSample = (USHORT)nSample;
                    CurrEntry.usPropDelay = (USHORT)m_nPropDelay;
                    CurrEntry.usSJW = (USHORT)m_nSJWCurr;
                    nReturn = bCalculateICSneoVIParams(CurrEntry, *puwIndex, m_bOption);
                }
            }

            i++;
            wNBT = (WORD)(unProductNbtNBrp / i);
            fNBT = (FLOAT)unProductNbtNBrp / i;
        }

        nReturn = 1; // Whatever the return value is, the calculating operation
        // is always successful
    }
    else
    {
        nReturn = -1;
    }

    return nReturn;
}

/******************************************************************************/
/*  Function Name    :  bCalculateICSneoVIRegValues

    Input(s)         :  NBT, BRP and Number of Sampling/bit
    Output           :  true if process of calculation is complete. In case
                        there are more values remaining, false is returned.
    Functionality    :  Given a value of NBT, BRP and sampling rate, this
                        calculates set of values of CNF1, CNF2, CNF3, PD and
                        SJW saving them in the array
                        and BRP values and stores into a structure.
    Member of        :  CChangeRegisters_CAN_ICS_neoVI
    Friend of        :  -

    Author(s)        :  Pradeep Kadoor
    Date Created     :  03.04.2008
/******************************************************************************/
bool CChangeRegisters_CAN_ICS_neoVI::bCalculateICSneoVIRegValues(WORD wNbt, WORD wBrp,
        UINT* puwIndex, INT nSample)
{
    bool bContinue = (defREG_VALUE_LIST_COUNT_MAX > *puwIndex);
    WORD  wMinPropDelay = defPropDelayICSneoVI;
    WORD  wMaxPropDelay = defmcMIN2(defMAXPropDelay,
                                    (wNbt - defMIN_TSEG1_ICSneoVI - defMIN_TSEG2 - 1));

    for (INT i = wMinPropDelay; (i <= wMaxPropDelay) && bContinue; i++)
    {
        sBRP_NBT_SAMP_n_SJW CurrEntry;
        CurrEntry.usBRP = wBrp;
        CurrEntry.usNBT = wNbt;
        CurrEntry.usPropDelay = (USHORT)i;
        CurrEntry.usSample = (USHORT)nSample;
        bContinue = bCalculateICSneoVIParams(CurrEntry, *puwIndex, SJW_TS1_TS2);
    }

    return bContinue;
}

/******************************************************************************/
/*  Function Name    : nCalculateRegParams                                    */
/*                                                                            */
/*  Input(s)         :  NBT, BRP and Number of Sampling/bit                   */
/*  Output           :  Possible values of TSEG1 and TSEG2 values             */
/*  Functionality    :  Calculates the TSEG1 and TSEG2 values for a set of NBT*/
/*                      and BRP values and stores into a structure.           */
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Pradeep Kadoor                                        */
/*  Date Created     :  11.04.2008                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
bool CChangeRegisters_CAN_ICS_neoVI::bCalculateICSneoVIParams(sBRP_NBT_SAMP_n_SJW& CurEntry,
        UINT& unCurrIndex, BYTE bOption)
{
    bool bContinue    = true;

    switch (bOption)
    {
        case TS1_TS2:
        {
            WORD  wNbt        = CurEntry.usNBT;
            WORD  wBrp        = CurEntry.usBRP;
            INT   nSample     = CurEntry.usSample;
            WORD  wSJW        = CurEntry.usSJW;
            WORD  wPropDelay  = CurEntry.usPropDelay;
            WORD  wTSEG1      = 0;
            WORD  wTSEG2      = 0;
            WORD  wTSEG2min   = 0;
            WORD  wTSEG2max   = 0;
            WORD  wSampling   = 0;
            bContinue = (defREG_VALUE_LIST_COUNT_MAX > unCurrIndex);
            wTSEG2max = (BYTE)(defmcMIN2(defMAX_TSEG2,
                                         wNbt - defMIN_TSEG1_ICSneoVI - 1 - wPropDelay));
            wTSEG2min = (BYTE)(defmcMAX3(defMIN_TSEG2, 0,
                                         wNbt - defMAX_TSEG1_ICSneoVI - 1 - wPropDelay));

            for (INT j = wTSEG2min; (j < wTSEG2max + 1) && bContinue; j++)
            {
                wTSEG2 = (BYTE)(j);
                //Calculate Sampling = ((NBT-TSEG2)/NBT) *100
                wSampling = (BYTE)(((FLOAT)(wNbt - wTSEG2) / wNbt)*100);
                //calculate TSEG1
                wTSEG1 = (BYTE)(wNbt - wTSEG2 - 1 - wPropDelay);

                // Only if sampling is greater then 50%, the value is stored in list. &
                // having the condition (TSEG1 + PD) > TSEG2
                if ((wSampling >= defMIN_SAMPLING) && ((wTSEG1 + wPropDelay) > wTSEG2))
                {
                    // First initialize all
                    m_asColListCtrl[unCurrIndex].uCNFReg1.ucCNF1 = 0x0;
                    m_asColListCtrl[unCurrIndex].uCNFReg2.ucCNF2 = 0x0;
                    m_asColListCtrl[unCurrIndex].uCNFReg3.ucCNF3 = 0x0;
                    // Calculate TSEG1 = NBT-TSEG2-1
                    //Fill the structure with BRP, Sampling, NBT,SJW values for display
                    m_asColListCtrl[unCurrIndex].sBRPNBTSampNSJW.usBRP       = wBrp;
                    m_asColListCtrl[unCurrIndex].sBRPNBTSampNSJW.usSampling  = wSampling;
                    m_asColListCtrl[unCurrIndex].sBRPNBTSampNSJW.usNBT       = wNbt;
                    m_asColListCtrl[unCurrIndex].sBRPNBTSampNSJW.usSJW       = wSJW;
                    //Pack the register CNF1 its bit BRPbit = BRP -1; SJWbit = SJW -1;
                    m_asColListCtrl[unCurrIndex].uCNFReg1.sCNF1Bit.ucBRPbit  = wBrp - 1;
                    m_asColListCtrl[unCurrIndex].uCNFReg1.sCNF1Bit.ucSJWbit  = wSJW - 1;
                    //Pack the register CNF2 its bit TESG1bit=TSEG1-1;TSEG2bit=TSEG2 -1;
                    m_asColListCtrl[unCurrIndex].uCNFReg2.sCNF2Bit.ucFLAGbit = 0x1;
                    m_asColListCtrl[unCurrIndex].uCNFReg2.sCNF2Bit.ucSAMbit = (nSample == 3);
                    m_asColListCtrl[unCurrIndex].uCNFReg2.sCNF2Bit.ucTSEG1bit = wTSEG1 - 1;
                    m_asColListCtrl[unCurrIndex].uCNFReg2.sCNF2Bit.ucPropDelaybit = wPropDelay - 1;
                    //Pack the register CNF3 its bit TESG2bit=TSEG2-1;
                    m_asColListCtrl[unCurrIndex].uCNFReg3.sCNF3Bit.ucTSEG2bit = wTSEG2 - 1;
                    unCurrIndex += 1;
                    bContinue = (defREG_VALUE_LIST_COUNT_MAX > unCurrIndex);
                }
            }
        }
        break;

        case PD_TS1_TS2:
        {
            WORD wMinPropDelay = defPropDelayICSneoVI;
            WORD wMaxPropDelay = defmcMIN2(defMAXPropDelay,
                                           (CurEntry.usNBT - defMIN_TSEG1_ICSneoVI - defMIN_TSEG2 - 1));

            for (INT i = wMinPropDelay; (i <= wMaxPropDelay) && bContinue; i++)
            {
                CurEntry.usPropDelay = (USHORT)i;
                bContinue = bCalculateICSneoVIParams(CurEntry, unCurrIndex, TS1_TS2);
            }
        }
        break;

        case SJW_TS1_TS2:
        {
            for (INT i = 0; (i < defMAX_SJW) && bContinue; i++)
            {
                CurEntry.usSJW = (BYTE)(i + 1);
                bContinue = bCalculateICSneoVIParams(CurEntry, unCurrIndex, TS1_TS2);
            }
        }
        break;

        default:
        {
            bContinue = false;
        }
        break;
    }

    return bContinue;
}

/* function ends*/