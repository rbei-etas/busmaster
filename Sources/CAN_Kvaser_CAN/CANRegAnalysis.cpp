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
 * \file      CANRegAnalysis.cpp
 * \brief     This file contains the function which implements the
 * \authors   Amitesh Bharti, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains the function which implements the
 */

// Include all standard header files
#include "CAN_Kvaser_CAN_stdafx.h"
// hash defines for resource ID's
#include "CAN_Kvaser_CAN_Resource.h"
#include "../include/struct_can.h"
// CChangeRegisters class defination file.

#include "ChangeRegisters.h"
#include "math.h"

/******************************************************************************/
/*  Function Name    :  nListBoxValues                                        */    
/*                                                                            */    
/*  Input(s)         :  Baudrate Number of Sample/bit and Clock Frequency     */    
/*  Output           :  NBT and BRP values                                    */    
/*  Functionality    :  Calculate set of the NBT and BRP value for given      */    
/*                      baudrate and clock frequency                          */    
/*  Member of        :  CChangeRegisters                                      */    
/*  Friend of        :      -                                                 */    
/*                                                                            */    
/*  Author(s)        :  Amitesh Bharti                                        */    
/*  Date Created     :  15.02.2002                                            */    
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  28.05.2002, Type casting changed from BYTE to WORD    */
/******************************************************************************/
BOOL CChangeRegisters::nListBoxValues( struct sCOLUMNS *psColListCtrl,
                                       DOUBLE dBuadRate,WORD wClockFreq,
                                       UINT *puwIndex,INT nSample       ) 
{
  UINT  unProductNbtNBrp    = 0;
  DOUBLE dProductNbtNBrp    = 0; 
  INT   nReturn             = -1;

  // Calcualte the product NBT * BRP = clock/(2.0 * baudrate ). This product
  // should be an integer multiple.
  dProductNbtNBrp  = (wClockFreq/dBuadRate)/2.0 *
                                (defFACT_FREQUENCY/ defFACT_BAUD_RATE) ;
  unProductNbtNBrp = (UINT) (dProductNbtNBrp +0.5);
 //Check if product is integer multiple. Ignore diffrence <= 0.004
  if(fabs((dProductNbtNBrp - unProductNbtNBrp))<=defVALID_DECIMAL_VALUE )
  {
	INT   i = 1;
    WORD wNBT  = (WORD) (unProductNbtNBrp/i);
    FLOAT fNBT = (FLOAT)unProductNbtNBrp/i;

	//Calculate all set of NBT and BRP value for a given product of NBT and BRP.
    while( wNBT >=1 && i<=defMAX_BRP )
    {
      if( (wNBT == fNBT) && (wNBT >= defMIN_NBT) && (wNBT <=defMAX_NBT))
      {
        WORD wBRP    = (WORD)(unProductNbtNBrp/wNBT);
        //Call this function to calculate BTR0, BTR1 regsiter value for one set
        // of NBT and BRP.
        nReturn = nCalculateRegValues(psColListCtrl,wNBT,wBRP,puwIndex,nSample);
      }

      i++;
      wNBT     = (WORD)(unProductNbtNBrp/i);
      fNBT     = (FLOAT)unProductNbtNBrp/ i;
    }

  }
  else
  {
      nReturn = -1;
  }
  return nReturn;
}
/******************************************************************************/
/*  Function Name    : nCalculateRegValues                                    */    
/*                                                                            */    
/*  Input(s)         :  NBT, BRP and Number of Sampling/bit                   */    
/*  Output           :  BTR0 and BTR1 values                                  */    
/*  Functionality    :  Calculates the BTR0 and BTR1 values for a set of NBT  */    
/*                      and BRP values and stores into a structure.           */    
/*  Member of        :  CChangeRegisters                                      */    
/*  Friend of        :      -                                                 */    
/*                                                                            */    
/*  Author(s)        :  Amitesh Bharti                                        */    
/*  Date Created     :  19.02.2002                                            */    
/*  Modifications    :                                                        */    
/*                                                                            */    
/******************************************************************************/
BOOL CChangeRegisters::nCalculateRegValues ( struct sCOLUMNS *psColListCtrl,
                                             WORD wNbt,WORD wBrp,UINT *puwIndex,
                                             INT nSample                          )
{
  WORD  wSJW        = 0;
  WORD  wTSEG1      = 0;
  WORD  wTSEG2      = 0;
  WORD  wTSEG2min   = 0;
  WORD  wTSEG2max   = 0;
  WORD  wSampling   = 0;
  
  INT k = 0;
  for(INT i=0; i<defMAX_SJW;i++)
  {
      wSJW = (BYTE)(i+1);
      k     = 0;
      // Calculate Maximum and Minimum value of TSEG2.
      wTSEG2max = (BYTE)(defmcMIN2(defMAX_TSEG2,wNbt - defMIN_TSEG1 - 1));
      wTSEG2min = (BYTE)(defmcMAX3(defMIN_TSEG2,wSJW,wNbt - defMAX_TSEG1 - 1));
      for (INT j = wTSEG2min; j<wTSEG2max+1;j++)
      {
          
          wTSEG2 = (BYTE)(j);
          k++;
          //Calculate Sampling = ((NBT-TSEG2)/NBT) *100
          wSampling = (BYTE)( ( (FLOAT)( wNbt - wTSEG2) / wNbt)*100 );
          // Only if sampling is greater then 50%, the value is stored in list.
          if(wSampling >=defMIN_SAMPLING)
          {
              // Calculate TSEG1 = NBT-TSEG2-1
              wTSEG1     = (BYTE)(wNbt - wTSEG2 - 1 );
              *puwIndex   +=1;
              //Fill the structure with BRP, Sampling, NBT,SJW values for display
              psColListCtrl[*puwIndex-1].sBRPNBTSampNSJW.usBRP       = wBrp ;
              psColListCtrl[*puwIndex-1].sBRPNBTSampNSJW.usSampling  = wSampling ;
              psColListCtrl[*puwIndex-1].sBRPNBTSampNSJW.usNBT       = wNbt ;             
              psColListCtrl[*puwIndex-1].sBRPNBTSampNSJW.usSJW       = wSJW ;
              //Pack the register BTR0 its bit BRPbit = BRP -1; SJWbit = SJW -1;
              psColListCtrl[*puwIndex-1].uBTRReg0.sBTR0Bit.ucBRPbit  = wBrp - 1 ;
              psColListCtrl[*puwIndex-1].uBTRReg0.sBTR0Bit.ucSJWbit  = wSJW - 1 ;
              //Pack the register BTR1 its bit TESG1bit=TSEG1-1;TSEG2bit=TSEG2 -1;
              psColListCtrl[*puwIndex-1].uBTRReg1.sBTR1Bit.ucTSEG1bit= wTSEG1 - 1;
              psColListCtrl[*puwIndex-1].uBTRReg1.sBTR1Bit.ucTSEG2bit= wTSEG2 - 1;
              //Pack the register BTR1 its bit SAMbit=1 or 0
              if(nSample == 3)
              {
                 psColListCtrl[*puwIndex-1].uBTRReg1.sBTR1Bit.ucSAMbit = 1;
              }
              else
              {
                 psColListCtrl[*puwIndex-1].uBTRReg1.sBTR1Bit.ucSAMbit   = 0;
              }
          }
      }
  }

  return 1;
}
