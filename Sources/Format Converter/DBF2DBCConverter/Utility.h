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
 * \brief     Utility functions declarations
 * \author    venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Utility functions declarations
 */

//Physical to value
UINT unConvertPhysicalToRaw(DOUBLE dMaxVal, DOUBLE dMinVal, DOUBLE dOffset, DOUBLE dScalFactor, UINT64& unMaxVal, UINT64& unMinVal);
INT nConvertPhysicalToRaw(DOUBLE dMaxVal, DOUBLE dMinVal, DOUBLE dOffset, DOUBLE dScalFactor, INT64& nMaxVal, INT64& nMinVal);

//Raw to Physical value
UINT unConvertRawToPhysical(UINT64 unMaxRawVal, UINT64 unMinRawVal, DOUBLE dOffset, DOUBLE dScalFactor, DOUBLE& dMaxPhyVal, DOUBLE& dMinPhyVal);
INT nConvertRawToPhysical(INT64 unMaxRawVal, INT64 unMinRawVal, DOUBLE dOffset, DOUBLE dScalFactor, DOUBLE& dMaxPhyVal, DOUBLE& dMinPhyVal);


//Helper function
UINT64 unRoundOffdoubleValue(DOUBLE dValue);
