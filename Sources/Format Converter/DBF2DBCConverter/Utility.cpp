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
 *
 * Defines Global Utility functions
 */

/* Project includes */
#include "Utility.h"

#define     doublePRECISION       0.5

/**
* \brief      Converts the double physical values to unsigned long long int RAW values
* \param[in]  double dMaxVal, Maximum physical value
              double dMinVal, Minimum physical value
              double dOffset, Offset value
              double dScalFactor, Scaling Factor
* \param[out] unsigned long long int& unMaxVal, UIN64 Maximum vlaue
              unsigned long long int& unMinVal, unsigned long long int Minimum RAW value
* \return     unsigned int
* \authors    Venkatanarayana Makam
* \date       27.07.2011
*/
unsigned int unConvertPhysicalToRaw(double dMaxVal, double dMinVal, double dOffset, double dScalFactor, unsigned long long int& unMaxVal, unsigned long long int& unMinVal)
{
    double dTempMaxVal = ((dMaxVal - dOffset) / dScalFactor);
    double dTempMinVal = ((dMinVal - dOffset) / dScalFactor);

    unMaxVal = (unsigned long long int)unRoundOffdoubleValue(dTempMaxVal);
    unMinVal = (unsigned long long int)unRoundOffdoubleValue(dTempMinVal);
    return 0;
}

/**
* \brief      Converts the double physical values to long long int RAW values
* \param[in]  double dMaxVal, Maximum physical value
              double dMinVal, Minimum physical value
              double dOffset, Offset Value
              double dScalFactor, Scaling Factor
* \param[out] unsigned long long int& unMaxVal, UIN64 Maximum vlaue
              unsigned long long int& unMinVal, unsigned long long int Minimum RAW value
* \return     int
* \authors    Venkatanarayana Makam
* \date       27.07.2011
*/
int nConvertPhysicalToRaw(double dMaxVal, double dMinVal, double dOffset, double dScalFactor, long long int& nMaxVal, long long int& nMinVal)
{
    double dTempMaxVal = ((dMaxVal - dOffset) / dScalFactor);
    double dTempMinVal = ((dMinVal - dOffset) / dScalFactor);

    nMaxVal = (long long int)unRoundOffdoubleValue(dTempMaxVal);
    nMinVal = (long long int)unRoundOffdoubleValue(dTempMinVal);
    return 0;
}
/**
* \brief      Converts the double physical values to long long int RAW values
* \param[in]  unsigned long long int unMaxRawVal, Maximum raw value
              unsigned long long int unMaxRawVal, Minimum raw value
              double dOffset, Offset Value
              double dScalFactor, Scaling Factor
* \param[out] double& dMaxPhyVal, Maximum physical vlaue
              double& dMaxPhyVal, Minimum physical vlaue
* \return     unsigned int
* \authors    Venkatanarayana Makam
* \date       27.07.2011
*/
unsigned int unConvertRawToPhysical(unsigned long long int unMaxRawVal, unsigned long long int unMinRawVal, double dOffset, double dScalFactor, double& dMaxPhyVal, double& dMinPhyVal)
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
* \brief      Converts the double physical values to long long int RAW values
* \param[in]  long long int unMaxRawVal, Maximum raw value
              long long int unMaxRawVal, Minimum raw value
              double dOffset, Offset Value
              double dScalFactor, Scaling Factor
* \param[out] double& dMaxPhyVal, Maximum physical vlaue
              double& dMaxPhyVal, Minimum physical vlaue
* \return     int
* \authors    Venkatanarayana Makam
* \date       27.07.2011
*/
int nConvertRawToPhysical(long long int unMaxRawVal, long long int unMinRawVal, double dOffset, double dScalFactor, double& dMaxPhyVal, double& dMinPhyVal)
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
* \param[in]  double dValue
* \param[out] None
* \return     unsigned long long int
* \authors    Venkatanarayana Makam
* \date       27.07.2011
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
