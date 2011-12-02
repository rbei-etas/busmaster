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
 * \file      Utility.cpp
 * \brief     Implementation of the CValueDescriptor class.
 * \author    Venkatanarayana Makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the CValueDescriptor class.
 */
/**
* \file       Utility.cpp
* \brief      Defines Global Utility functions
* \authors    Venkatanarayana Makam
* \date       27.07.2011
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/
#define     DOUBLEPRECISION       0.5

#include "StdAfx.h"
#include "Utility.h"
/**
* \brief      Converts the DOUBLE physical values to UINT64 RAW values
* \param[in]  DOUBLE dMaxVal, Maximum physical value
              DOUBLE dMinVal, Minimum physical value
              DOUBLE dOffset, Offset value
              DOUBLE dScalFactor, Scaling Factor
* \param[out] UINT64& unMaxVal, UIN64 Maximum vlaue
              UINT64& unMinVal, UINT64 Minimum RAW value
* \return     UINT
* \authors    Venkatanarayana Makam
* \date       27.07.2011
*/
UINT unConvertPhysicalToRaw(DOUBLE dMaxVal, DOUBLE dMinVal, DOUBLE dOffset, DOUBLE dScalFactor, UINT64& unMaxVal, UINT64& unMinVal)
{
    double dTempMaxVal = ((dMaxVal - dOffset) / dScalFactor);
    double dTempMinVal = ((dMinVal - dOffset) / dScalFactor);

    unMaxVal = (UINT64)unRoundOffdoubleValue(dTempMaxVal);
    unMinVal = (UINT64)unRoundOffdoubleValue(dTempMinVal);
    return 0;
}

/**
* \brief      Converts the DOUBLE physical values to INT64 RAW values
* \param[in]  DOUBLE dMaxVal, Maximum physical value
              DOUBLE dMinVal, Minimum physical value
              DOUBLE dOffset, Offset Value
              DOUBLE dScalFactor, Scaling Factor
* \param[out] UINT64& unMaxVal, UIN64 Maximum vlaue
              UINT64& unMinVal, UINT64 Minimum RAW value
* \return     INT
* \authors    Venkatanarayana Makam
* \date       27.07.2011
*/
INT nConvertPhysicalToRaw(DOUBLE dMaxVal, DOUBLE dMinVal, DOUBLE dOffset, DOUBLE dScalFactor, INT64& nMaxVal, INT64& nMinVal)
{
    double dTempMaxVal = ((dMaxVal - dOffset) / dScalFactor);
    double dTempMinVal = ((dMinVal - dOffset) / dScalFactor);

    nMaxVal = (INT64)unRoundOffdoubleValue(dTempMaxVal);
    nMinVal = (INT64)unRoundOffdoubleValue(dTempMinVal);
    return 0;
}
/**
* \brief      Converts the DOUBLE physical values to INT64 RAW values
* \param[in]  UINT64 unMaxRawVal, Maximum raw value
              UINT64 unMaxRawVal, Minimum raw value
              DOUBLE dOffset, Offset Value
              DOUBLE dScalFactor, Scaling Factor
* \param[out] DOUBLE& dMaxPhyVal, Maximum physical vlaue
              DOUBLE& dMaxPhyVal, Minimum physical vlaue
* \return     UINT
* \authors    Venkatanarayana Makam
* \date       27.07.2011
*/
UINT unConvertRawToPhysical(UINT64 unMaxRawVal, UINT64 unMinRawVal, DOUBLE dOffset, DOUBLE dScalFactor, DOUBLE& dMaxPhyVal, DOUBLE& dMinPhyVal)
{
    //Maximum value
    dMaxPhyVal = static_cast<double>(unMaxRawVal);
    dMaxPhyVal *= dScalFactor;
    dMaxPhyVal += dOffset;
    //Minimum value
    dMinPhyVal = static_cast<double>(unMinRawVal);
    dMinPhyVal *= dScalFactor;
    dMinPhyVal += dOffset;
    return 0;
}
/**
* \brief      Converts the DOUBLE physical values to INT64 RAW values
* \param[in]  INT64 unMaxRawVal, Maximum raw value
              INT64 unMaxRawVal, Minimum raw value
              DOUBLE dOffset, Offset Value
              DOUBLE dScalFactor, Scaling Factor
* \param[out] DOUBLE& dMaxPhyVal, Maximum physical vlaue
              DOUBLE& dMaxPhyVal, Minimum physical vlaue
* \return     INT
* \authors    Venkatanarayana Makam
* \date       27.07.2011
*/
INT nConvertRawToPhysical(INT64 unMaxRawVal, INT64 unMinRawVal, DOUBLE dOffset, DOUBLE dScalFactor, DOUBLE& dMaxPhyVal, DOUBLE& dMinPhyVal)
{
    //Maximum value
    dMaxPhyVal = static_cast<double>(unMaxRawVal);
    dMaxPhyVal *= dScalFactor;
    dMaxPhyVal += dOffset;
    //Minimum value
    dMinPhyVal = static_cast<double>(unMinRawVal);
    dMinPhyVal *= dScalFactor;
    dMinPhyVal += dOffset;
    return 0;
}
/**
* \brief      This function will convert the double value to UIN64.
              if the double value precision is more than 0.5 the unit valu will be added 1
* \param[in]  DOUBLE dValue
* \param[out] None
* \return     UINT64
* \authors    Venkatanarayana Makam
* \date       27.07.2011
*/
UINT64 unRoundOffdoubleValue(DOUBLE dValue)
{
    UINT64 unVal = (UINT64)dValue;
    DOUBLE dTempVal = dValue - (DOUBLE)unVal;
    if(dTempVal >= 0.5)
    {
        unVal++;
    }
    return unVal;
}
