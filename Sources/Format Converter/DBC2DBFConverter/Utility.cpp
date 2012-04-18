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
 * \brief     Implementation of global utility functions
 * \authors   Venkatanarayana Makam, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of global utility functions.
 */

/* Project includes */
#include "Utility.h"

/**
 * \brief      Converts the DOUBLE physical values to UINT64 RAW values
 * \param[in]  dMaxVal Maximum physical value
 * \param[in]  dMinVal Minimum physical value
 * \param[in]  dOffset Offset value
 * \param[in]  dScaleFactor Scaling Factor
 * \param[out] unMaxVal Maximum RAW value
 * \param[out] unMinVal Minimum RAW value
 *
 * Converts the DOUBLE physical values to UINT64 RAW values.
 */
unsigned int unConvertPhysicalToRaw(double dMaxVal, double dMinVal, double dOffset, double dScaleFactor, unsigned long long int& unMaxVal, unsigned long long int& unMinVal)
{
    double dTempMaxVal = ((dMaxVal - dOffset) / dScaleFactor);
    double dTempMinVal = ((dMinVal - dOffset) / dScaleFactor);

    unMaxVal = (unsigned long long int)unRoundOffdoubleValue(dTempMaxVal);
    unMinVal = (unsigned long long int)unRoundOffdoubleValue(dTempMinVal);
    return 0;
}

/**
 * \brief      Converts the DOUBLE physical values to INT64 RAW values
 * \param[in]  dMaxVal Maximum physical value
 * \param[in]  dMinVal Minimum physical value
 * \param[in]  dOffset Offset Value
 * \param[in]  dScaleFactor Scaling Factor
 * \param[out] nMaxVal Maximum RAW value
 * \param[out] nMinVal Minimum RAW value
 *
 * Converts the DOUBLE physical values to INT64 RAW values.
 */
int nConvertPhysicalToRaw(double dMaxVal, double dMinVal, double dOffset, double dScaleFactor, long long int& nMaxVal, long long int& nMinVal)
{
    double dTempMaxVal = ((dMaxVal - dOffset) / dScaleFactor);
    double dTempMinVal = ((dMinVal - dOffset) / dScaleFactor);

    nMaxVal = (long long int)unRoundOffdoubleValue(dTempMaxVal);
    nMinVal = (long long int)unRoundOffdoubleValue(dTempMinVal);
    return 0;
}

/**
 * \brief      Converts the DOUBLE physical values to INT64 RAW values
 * \param[in]  unMaxRawVal Maximum raw value
 * \param[in]  unMinRawVal Minimum raw value
 * \param[in]  dOffset Offset Value
 * \param[in]  dScaleFactor Scaling Factor
 * \param[out] dMaxPhyVal Maximum physical vlaue
 * \param[out] dMinPhyVal Minimum physical vlaue
 *
 * Converts the DOUBLE physical values to INT64 RAW values.
 */
unsigned int unConvertRawToPhysical(unsigned long long int unMaxRawVal, unsigned long long int unMinRawVal, double dOffset, double dScaleFactor, double& dMaxPhyVal, double& dMinPhyVal)
{
    //Maximum value
    dMaxPhyVal = static_cast<double>(unMaxRawVal);
    dMaxPhyVal *= dScaleFactor;
    dMaxPhyVal += dOffset;
    //Minimum value
    dMinPhyVal = static_cast<double>(unMinRawVal);
    dMinPhyVal *= dScaleFactor;
    dMinPhyVal += dOffset;
    return 0;
}

/**
 * \brief      Converts the DOUBLE physical values to INT64 RAW values
 * \param[in]  unMaxRawVal Maximum raw value
 * \param[in]  unMinRawVal Minimum raw value
 * \param[in]  dOffset Offset Value
 * \param[in]  dScaleFactor Scaling Factor
 * \param[out] dMaxPhyVal Maximum physical value
 * \param[out] dMinPhyVal Minimum physical value
 *
 * Converts the DOUBLE physical values to INT64 RAW values.
 */
int nConvertRawToPhysical(long long int unMaxRawVal, long long int unMinRawVal, double dOffset, double dScaleFactor, double& dMaxPhyVal, double& dMinPhyVal)
{
    //Maximum value
    dMaxPhyVal = static_cast<double>(unMaxRawVal);
    dMaxPhyVal *= dScaleFactor;
    dMaxPhyVal += dOffset;
    //Minimum value
    dMinPhyVal = static_cast<double>(unMinRawVal);
    dMinPhyVal *= dScaleFactor;
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
unsigned long long int unRoundOffdoubleValue(double dValue)
{
    unsigned long long int unVal = (unsigned long long int)dValue;
    double dTempVal = dValue - (double)unVal;
    if(dTempVal >= 0.5)
    {
        unVal++;
    }
    return unVal;
}
