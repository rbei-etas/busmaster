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
 * \file      ModuleID.h
 * \brief     This contains identifier of various modules.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains identifier of various modules.
 */

#pragma once

#include <guiddef.h>

typedef enum eID_COMPONENT
{
    COMPONENT_UNDEFINED     = 0,
    PROJECTCONFIG_GEN,
    DIL_CAN_STUB,
    DIL_CAN_PEAK_USB,
    DIL_CAN_PEAK_PP,
    DIL_CAN_ICS_NEOVI,
    DIL_CAN_ETAS_BOA,
    DIL_CAN_VECTOR_XL,
    DIL_J1939,
    DIL_MCNET_PEAK_USB,
    FRAMEPROC_CAN,
    FRAMEPROC_MCNET,
    FRAMEPROC_J1939,
    CONVERTER_DBC_2_DBF,
    CONVERTER_DBF_2_DBC,
    CONVERTER_CAPL_2_C,
    COMPONENT_DEFAULT,
    COMPONENTS_ALL
};
