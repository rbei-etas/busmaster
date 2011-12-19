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
 * \file      Utility.h
 * \brief     Definition of global utility functions
 * \author    venkatanarayana makam, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of the global utility functions.
 */

#ifndef Utility_H
#define Utility_H

//Physical to value
unsigned int unConvertPhysicalToRaw(double dMaxVal, double dMinVal, double dOffset, double dScalFactor, unsigned long long int& unMaxVal, unsigned long long int& unMinVal);
int nConvertPhysicalToRaw(double dMaxVal, double dMinVal, double dOffset, double dScalFactor, long long int& nMaxVal, long long int& nMinVal);

//Raw to Physical value
unsigned int unConvertRawToPhysical(unsigned long long int unMaxRawVal, unsigned long long int unMinRawVal, double dOffset, double dScalFactor, double& dMaxPhyVal, double& dMinPhyVal);
int nConvertRawToPhysical(long long int unMaxRawVal, long long int unMinRawVal, double dOffset, double dScalFactor, double& dMaxPhyVal, double& dMinPhyVal);

//Helper function
unsigned long long int unRoundOffdoubleValue(double dValue);

#endif
