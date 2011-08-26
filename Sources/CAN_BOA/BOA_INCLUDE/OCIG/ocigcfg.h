#ifndef ETAS_OCIG_CFG_H
#define ETAS_OCIG_CFG_H

/**
* @file       ocigcfg.h
* @brief      Configuration stuctures portion of the 
*             Genric Open Controller Interface (OCIG) API.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*/

#include "..\OCI\ocitypes.h"

#include "..\Common\pshpack1.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @addtogroup GROUP_OCIG_COMMON OCIG Common
* @{
*/

/**
* @addtogroup GROUP_OCIG_CONFIGURATION Configuration Structures
* @{
*/

/**
* @anchor ANCHOR_OCIG_PARAMETER_TYPE_VALUES
* @name   OCIG Configuration Parameter Type Values
* @brief  List of configuration parameter types.
*         The values are used to select a specific part of a suitable 
*         union data structure. For that reason the relevant values 
*         must be continuous enumerated beginning with 1. This is 
*         important when code is automatically generated.
* @{
*/

/** 
* Parameter is a signed integer. 
*/
#define OCIG_PARAMETER_TYPE_INT          1

/** 
* Parameter is a unsigned integer.
*/
#define OCIG_PARAMETER_TYPE_UNSIGNED     2

/** 
* Parameter is a 32 bit float 
*/
#define OCIG_PARAMETER_TYPE_FLOAT        3

/** 
* Parameter is a 0-terminated UTF-8 string. 
*/
#define OCIG_PARAMETER_TYPE_STRING       4

/** 
* Parameter is a fixed-length byte (unsigned char) array. 
*/
#define OCIG_PARAMETER_TYPE_BINARY       5

/** 
 The Parameter is a record / structure. 
 OCIG_DataDictionaryEntry.structure references the first member of the record. 
 OCIG_DataDictionaryEntry.size is the number of members of the record 
 (defined at offsets @a structure .. @a structure + @a size-1).
*/
#define OCIG_PARAMETER_TYPE_RECORD       240

/** 
 The Parameter is an array with variable length.
 OCIG_DataDictionaryEntry.structure references the type definition of the elements. 
 OCIG_DataDictionaryEntry.size is the number of array elements (size_is()).
 OCIG_DataDictionaryEntry.value is interpreted as an integer and REFERENCES the lentgh_is() element of the array.
 The value "" or "-1" is used for fix sized arrays.
*/
#define OCIG_PARAMETER_TYPE_ARRAY        241

/**
 The Parameter describes a tagged union.
 OCIG_DataDictionaryEntry.structure references the first type definition of the union elements. 
 OCIG_DataDictionaryEntry.size is the number of union elements (located at structure .. structure + size-1).
 OCIG_DataDictionaryEntry.value is interpreted as unsigned and REFERENCES the tag of the union. The valid range
 for the tag is 0 .. size-1.
 */
#define OCIG_PARAMETER_TYPE_UNION        242

/**
 The type of the parameter is unknown. 
 This value must not be used in the DataDicitionary, but may be used by incomplete filled elements derived 
 from the DataDictionary.
 */
#define OCIG_PARAMETER_TYPE_UNKNWON     (OCI_NO_VLAUE)
/** @} */

/**
* @anchor ANCHOR_OCIG_CONFIG_PARAMETER_FLAGS
* @name   OCIG Configuration Parameter Flags
* @brief  List of configuration parameter types.
* @{
*/

/** 
  The parameter can be read. 
  Exotic settings may not be available once they have been set, i.e.\ they cannot be read again 
  from controller registers etc.
*/
#define OCIG_CONFIG_PARAMETER_READABLE     0x01

/** 
 The Parameter value may be modified at all. 
*/
#define OCIG_CONFIG_PARAMETER_WRITEABLE    0x02

/** 
 The Parameter value is optional. 
 Optional parameter can only occur at the end of a configuration structure. In this case the configuration
 can use a smaller structure. The usage of optional parameter is deprecated. The preferred solution resets
 this flag and sets a default value for the element. This is exact the same mechanism, that the controller
 has to do with optional parameter.
*/
#define OCIG_CONFIG_PARAMETER_OPTIONAL     0x04

/**
 The Parameter default value is valid. 
 For several Parameter there may be a vaild default configuration. Other Parameter may have no default
 value. Parameter, that have no valid default must be set by the application before assigning the configuration 
 to a controller instance. A formal validation of a configuration container can reject a container with invalid
 parameter for initialization. A parameter marked as invalid should additionally use a default value, that will
 probably cause an error in the OCIG_OpenController() call.
*/
#define OCIG_CONFIG_PARAMETER_INVALID  0x08

/**
 Flag for objects derived from the Data Dictionary. 
 The value of the prameter was set by the Data Dictionary.
 */
#define OCIG_CONFIG_PARAMETER_SET_BY_DD  0x0100

/**
 Flag for objects derived from the Data Dictionary. 
 The value of the prameter was set by the Application.
 */
#define OCIG_CONFIG_PARAMETER_SET_BY_APP 0x0200
/** @} */


/** 
* Maximum string length of parameter name (including terminating character)
* in @ref OCIG_DataDictionaryEntry.
* @todo Verify the value of @ref OCIG_DICTIONARY_IDENT_LENGTH.
*/
#define OCIG_DICTIONARY_IDENT_LENGTH       64u

/** 
* Maximum string length of parameter names (including terminating character)
* in @ref OCIG_DataDictionaryEntry.
* @todo Verify the value of @ref OCIG_DICTIONARY_NAME_LENGTH.
*/
#define OCIG_DICTIONARY_NAME_LENGTH        64u

/** 
* Maximum string length of default values (including terminating character)
* in @ref OCIG_DataDictionaryEntry.
*/
#define OCIG_DICTIONARY_VALUE_LENGTH       32u


/**
* Structure returned by the OCIG to represent the properties of a 
* single driver configuration parameter.
*/
typedef struct OCIG_DataDictionaryEntry
{
    /**
    * Parameter name. This could be a part of an identifier, which could be a 
    * concatenation of multiple parameter names. The parameter name must not 
    * be empty and must not contain dots.
    * @remarks  Use UTF8 to encode non-ASCII7 strings.
    * @par
    * <b>Naming convention:</b>
    * @n
    * The complete identifier is a string with @ref OCIG_DICTIONARY_IDENT_LENGTH 
    * alphanumerical characters including points and brackets, interpreted as 
    * hierarchical concatenation of partial strings separated by points. Every 
    * partial string represents a dictionary entry and may not exceed 
    * @ref OCIG_DICTIONARY_NAME_LENGTH characters, including the length of an 
    * optional array index (numeric or as a symbolic string) in brackets at the 
    * end of the partial string. The number of partial strings within complete 
    * identifier is not limited. The character chain must correspond with the 
    * identifier laws defined in programming language C. Identifiers can 
    * represent instances of array elements or instances of elements of complex 
    * C types or nested combinations of these. An instance of the element of a 
    * struct type would be represented by the concatenation of the instance 
    * name, a point and the element name. An instance of an array element would 
    * be represented by an instance name followed by a pair of brackets which 
    * contain either a numeric value or a symbolic string which is defined as 
    * an enumerator of an ENUM definition of the C program. Identifiers are 
    * random names which may contain characters A through Z, a through z, 
    * underscore (_), numerals 0 through 9, points ('.') and brackets 
    * ( '[',']' ). However, the following limitations apply: the first 
    * character must be a letter or an underscore, brackets must occur in pairs 
    * at the end of a partial string and must contain a number or an alphabetic 
    * string (description of the index of an array element).
    * @par
    * <b>Examples of complete identifiers:</b>
    * @li  MyIdentifier1
    * @li  _MyIdentifier2
    * @li  MyIdentifier3[1]
    * @li  MyIdentifier4[high]
    * @li  MyIdentifier5.MyIdentifier6
    * @li  MyIdentifier7.MyIdentifier8[2]
    * @li  MyIdentifier9[0].MyIdentifier10[100].MyIdentifier11    
    */
    
    char name[OCIG_DICTIONARY_NAME_LENGTH];

    /**
    * Default value. Should be suitable for standard
    * conversion into the respective type.
    * Empty strings are allowed to indicate a non-value.
    */

    char defaultValue[OCIG_DICTIONARY_VALUE_LENGTH];

    /**
    * Parameter data type.
    * @values   The following values are valid:
    * @n        @ref OCIG_PARAMETER_TYPE_INT
    * @n        @ref OCIG_PARAMETER_TYPE_UNSIGNED
    * @n        @ref OCIG_PARAMETER_TYPE_FLOAT
    * @n        @ref OCIG_PARAMETER_TYPE_STRING
    * @n        @ref OCIG_PARAMETER_TYPE_BINARY
    * @n        @ref OCIG_PARAMETER_TYPE_RECORD
    * @n        @ref OCIG_PARAMETER_TYPE_ARRAY
    * @n        @ref OCIG_PARAMETER_TYPE_UNKNOWN
    * @sa       @ref ANCHOR_OCIG_PARAMETER_TYPE_VALUES
    *           "OCIG Configuration Parameter Type Values"
    */
    
    uint32 type;

    /**
    * Defines what parameters must or may be set.
    * @flags    A combination of the following flags is possible:
    * @n        @ref OCIG_CONFIG_PARAMETER_READABLE
    * @n        @ref OCIG_CONFIG_PARAMETER_WRITEABLE
    * @n        @ref OCIG_CONFIG_PARAMETER_OPTIONAL
    * @sa       @ref ANCHOR_OCIG_CONFIG_PARAMETER_FLAGS
    *           "OCIG Configuration Parameter Flags"
    */
    
    uint32 flags;

    /**
    * Index within the data dictionary that contains the sub-type
    * definition. For @ref type @ref OCIG_PARAMETER_TYPE_RECORD this
    * points to the first member definition of that record. For
    * @a type @ref OCIG_PARAMETER_TYPE_ARRAY this points to the
    * element type definition (name will be ignored).
    * For other values of @a type, this is ignored and should
    * be set to @ref OCI_NO_VALUE.
    */
    
    uint32 structure;

    /**
    * Size of the sub-structure. 
    * For @ref type @ref OCIG_PARAMETER_TYPE_RECORD this is the number 
    * of member definitions of that record. They form a consecutive 
    * section of the data dictionary. 
    * For @ref type @ref OCIG_PARAMETER_TYPE_ARRAY this is the size of 
    * the array (in elements).
    * For @ref type @ref OCIG_PARAMETER_TYPE_INT and 
    * @ref OCIG_PARAMETER_TYPE_UNSIGNED this is the size of the cardninal
    * in bytes. Only powers of two are allowed.
    * For other values of @a type, this is ignored and should
    * be set to 0.
    */

    uint32 size;

} OCIG_DataDictionaryEntry;

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#include "..\Common\poppack.h"

#endif
