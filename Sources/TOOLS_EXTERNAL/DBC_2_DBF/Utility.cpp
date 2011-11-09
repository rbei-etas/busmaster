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
 * \brief     Defines Global Utility functions
 * \author    Venkatanarayana Makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#define     DOUBLEPRECISION       0.5

#include "StdAfx.h"
#include "Utility.h"

/**
 * \brief      Converts the DOUBLE physical values to UINT64 RAW values
 * \param[in]  dMaxVal Maximum physical value
 * \param[in]  dMinVal Minimum physical value
 * \param[in]  dOffset Offset value
 * \param[in]  dScalFactor Scaling Factor
 * \param[out] unMaxVal Maximum RAW value
 * \param[out] unMinVal Minimum RAW value
 *
 * Converts the DOUBLE physical values to UINT64 RAW values.
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
 * \param[in]  dMaxVal Maximum physical value
 * \param[in]  dMinVal Minimum physical value
 * \param[in]  dOffset Offset Value
 * \param[in]  dScalFactor Scaling Factor
 * \param[out] unMaxVal Maximum RAW value
 *             unMinVal Minimum RAW value
 *
 * Converts the DOUBLE physical values to INT64 RAW values.
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
 * \param[in]  unMaxRawVal Maximum raw value
 * \param[in]  unMaxRawVal Minimum raw value
 * \param[in]  dOffset Offset Value
 * \param[in]  dScalFactor Scaling Factor
 * \param[out] dMaxPhyVal Maximum physical vlaue
 * \param[out] dMaxPhyVal Minimum physical vlaue
 *
 * Converts the DOUBLE physical values to INT64 RAW values.
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
 * \param[in]  unMaxRawVal Maximum raw value
 *             unMaxRawVal Minimum raw value
 *             dOffset Offset Value
 *             dScalFactor Scaling Factor
 * \param[out] dMaxPhyVal Maximum physical value
 *             dMaxPhyVal Minimum physical value
 *
 * Converts the DOUBLE physical values to INT64 RAW values.
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
 * \brief      Convert the double value to UINT64.
 * \param[in]  dValue double value
 * \return     value as UINT64
 *
 * This function will convert the double value to UINT64.
 * If the double value precision is more than 0.5 the unit value will be added 1.
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
