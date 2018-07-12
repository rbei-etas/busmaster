
/**
 * \file      UDS_DataTypes.h
 * \brief     Defines data type class for UDS tool
 * \author    Sanchez Marin Maria Alejandra
 * Designer:  Bentea Radu Mihai
 * Defines data type class for UDS tool
 */


#include "Utility\MultiLanguageSupport.h"
//#include "include/struct_can.h"
#include "CANDriverDefines.h"
#pragma once
#include <locale.h>


typedef enum UDS_INTERFACE
{
    INTERFACE_NORMAL_11,
    INTERFACE_EXTENDED_11,
    INTERFACE_NORMAL_ISO_29,
    INTERFACE_NORMAL_J1939_29,
};

typedef enum DIAGNOSTIC_STANDARD
{
    STANDARD_UDS,
    STANDARD_KWP2000,
};

typedef enum TYPE_OF_FRAME
{
    LONG_RESPONSE,
    CONSECUTIVE_FRAME,
    SIMPLE_RESPONSE,
    FLOW_CONTROL,
};