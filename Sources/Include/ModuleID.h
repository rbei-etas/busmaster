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

const GUID ID_LICENSE_VALIDATOR = 
{ 0x6a6e4b52, 0x103c, 0x4777, { 0xae, 0xdf, 0x22, 0xc0, 0xda, 0x98, 0x55, 0x89 } };

const GUID ID_DIL = 
{ 0xc8d033ec, 0x5a8d, 0x4bec, { 0xab, 0x1d, 0xbf, 0x88, 0xca, 0x97, 0xf6, 0xbc } };

const GUID ID_FRAME_PROCESSOR = 
{ 0xdd817480, 0x4f0d, 0x45fe, { 0x8e, 0x20, 0xbe, 0x75, 0x7c, 0x37, 0xa8, 0xd4 } };

const GUID ID_PROJECT_CONFIGURATION = 
{ 0xae124d3, 0x2af9, 0x455e, { 0xae, 0x26, 0xfa, 0x86, 0xc8, 0x41, 0xe0, 0x36 } };
