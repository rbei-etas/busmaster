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
 * \file      BusStatistics.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "stdafx.h"             // Standard include header
#include "BusStatisticCAN.h"
#include "BusStatistics.h"
static CBusStatisticCAN* sg_pouBS_CAN = NULL;

/**
 * \param[in] eBus specifies the Type of bus,Either CAN or FRAME
 * \param[in] ppvInterface on function return it will have the pointer of BusStatistics.
 * \return gives the function return status.
 *
 * This function will return the valid busstatistics class
 * pointer according to the ETYPE_BUS.
 */
HRESULT BS_GetInterface(ETYPE_BUS eBus, void** ppvInterface)
{
    HRESULT hResult = S_OK;

    switch (eBus)
    {
        case CAN:
        {
            if (NULL == sg_pouBS_CAN)
            {
                if ((sg_pouBS_CAN = new CBusStatisticCAN) == NULL)
                {
                    ASSERT(FALSE);
                    hResult = S_FALSE;
                }
            }
            // Else the object has been existing already
            *ppvInterface = (void *) sg_pouBS_CAN; 
        }
        break;
        case MCNET:            
        case J1939:
        {
            ASSERT(FALSE);
        }
        break;
        default: break;
    }

    return hResult;
}
