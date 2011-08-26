#ifndef ETAS_OCIG_CTRL_H
#define ETAS_OCIG_CTRL_H

/**
* @file       ocigctrl.h
* @brief      Controller configuration functions section of the 
*             Genric Open Controller Interface (OCIG) API.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*/

#include "ocigcfg.h"

#include "..\Common\pshpack1.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @addtogroup GROUP_OCIG_COMMON OCIG Common
* @{
*/

/**
* @addtogroup GROUP_OCIG_CONTROLLER Controller Handling
* @{
*/

/**
* OCIG argument values for selecting the data dictionary in 
* @ref OCIG_GetControllerDataStructureParameter as well as in
* @ref OCIG_GetControllerDataStructure and 
* @ref OCIG_SetControllerDataStructure.
* Please note that not all values are valid for all controllers
* and functions.
*/

typedef enum OCIG_DataStructureSelector
{
    /** 
    * Selects the controller configuration data structure. 
    */
    OCIG_STRUCTURE_CONFIGURATION = 0,
    
    /** 
    * Selects the controller properties structure. 
    */
    OCIG_STRUCTURE_PROPERTIES    = 1,

    /** 
    * Selects the controller capabilities structure. 
    */
    OCIG_STRUCTURE_CAPABILITIES  = 2,

    /** 
    * Selects the controller status structure. 
    */
    OCIG_STRUCTURE_STATUS        = 3

} OCIG_DataStructureSelector;


/** 
* Get the data dictionary describing the controller's configuration
* structure delivered by @ref OCIG_GetControllerDataStructure(). The 
* entry at index 0 will describe the root structure.
* @param[in]  controller  is the interface instance of the controller 
*                         to query. 
* @param[in]  selector    specifies for which structure the data
*                         dictionary shall be returned. Any value 
*                         of @ref OCIG_DataStructureSelector is allowed.
* @param[out] dictionary  is a pointer to the first of @a size entries
*                         in an array of @ref OCIG_DataDictionaryEntry 
*                         structures. The data dictionary will be 
*                         stored here. Must not be @c NULL, if 
*                         @a size > 0 and may only be @c NULL, if 
*                         @a size = 0.
* @param[in]  size        is the size of the @a dictionary buffer in 
*                         entries. If @a count > @a size, no entries 
*                         of the @a dictionary will be filled. 
*                         May be 0 to simply receive @a count.
* @param[out] count       receives the total number of 
*                         @ref OCIG_DataDictionaryEntry records 
*                         required to hold the configuration structure
*                         description. If @a count > @a size, no 
*                         entries of the @a dictionary will be filled.
*                         Must not be @c NULL.
* @return     If the operation was successful, @ref OCI_SUCCESS will 
*             be returned. If @a count > @a size and @a size > 0, 
*             @ref OCI_ERR_BUFFER_OVERFLOW will returned. Otherwise 
*             a specific error code will be returned. Error details 
*             will be stored for the @a controller handle. If no valid 
*             handle exists, details will be stored for handle 
*             @ref OCI_NO_HANDLE.
*
* @remark
* @n
* @code
* struct configuration 
* {
*     int32 value;
*     char  text[32];
* }
* @endcode
* @par 
* The dictionary entries of the structure @a dataStructure 
* could be (id, name, type, flags, structure, size, defaultValue):
* @li 0, "configuration", @ref OCIG_PARAMETER_TYPE_RECORD, 0, 1, 2, ""
* @li 1, "value", @ref OCIG_PARAMETER_TYPE_INT, 
*     @ref OCIG_CONFIG_PARAMETER_OPTIONAL, @ref OCI_NO_VALUE, 0, "0"
* @li 2, "text", @ref OCIG_PARAMETER_TYPE_BINARY, 
*     @ref OCIG_CONFIG_PARAMETER_OPTIONAL, @ref OCI_NO_VALUE, 32, ""
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
* identifier laws defined in programming language C. Identifiers can represent 
* instances of array elements or instances of elements of complex C types or 
* nested combinations of these. An instance of the element of a struct type 
* would be represented by the concatenation of the instance name, a point and 
* the element name. An instance of an array element would be represented by 
* an instance name followed by a pair of brackets which contain either a 
* numeric value or a symbolic string which is defined as an enumerator of an 
* ENUM definition of the C program. Identifiers are random names which may 
* contain characters A through Z, a through z, underscore (_), numerals 0 
* through 9, points ('.') and brackets ( '[',']' ) . However, the following 
* limitations apply: the first character must be a letter or an underscore, 
* brackets must occur in pairs at the end of a partial string and must contain 
* a number or an alphabetic string (description of the index of an array 
* element).
* @par
* <b>Examples of complete identifiers:</b>
* @li  MyIdentifier1
* @li  _MyIdentifier2
* @li  MyIdentifier3[1]
* @li  MyIdentifier4[high]
* @li  MyIdentifier5.MyIdentifier6
* @li  MyIdentifier7.MyIdentifier8[2]
* @li  MyIdentifier9[0].MyIdentifier10[100].MyIdentifier11
*
* @todo 2009-05-27, kuh: parameter description is inconsistent: dictionary is allowed to be null if size < count!
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCIG_GetControllerDataStructureParameter(
    OCI_ControllerHandle controller, 
    OCIG_DataStructureSelector selector,
    OCIG_DataDictionaryEntry* dictionary,
    uint32 size,
    uint32* count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCIG_GetControllerDataStructureParameter)(
    OCI_ControllerHandle controller, 
    OCIG_DataStructureSelector selector,
    OCIG_DataDictionaryEntry* dataDictionary,
    uint32 size,
    uint32* count);


/** 
* Get the current driver configuration. Use the information returned
* by @ref OCIG_GetControllerDataStructureParameter() to interpret the 
* content of that structure.
* @param[in]  controller     is the interface instance of the 
*                            controller to query. 
* @param[in]  selector       specifies for which structure shall be
*                            returned. Any value of 
*                            @ref OCIG_DataStructureSelector is allowed.
* @param[out] dataStructure  is a pointer to the buffer that will 
*                            receive the data structure.
*                            Must not be @c NULL, if @a size > 0 and
*                            may only be @c NULL, if @a size = 0.
* @param[in]  size           is the size of the @a data structure 
*                            buffer in bytes. If @a count > @a size, 
*                            no data will be written to this buffer. 
*                            May be 0 to simply receive @a count.
* @param[out] count          receives the number of bytes required
*                            to hold the data structure. 
*                            If @a count > @a size, no data will be 
*                            written. Must not be @c NULL.
* @return     If the operation was successful, @ref OCI_SUCCESS will 
*             be returned. If @a count > @a size and @a size > 0, 
*             @ref OCI_ERR_BUFFER_OVERFLOW will returned. Otherwise 
*             a specific error code will be returned. Error details 
*             will be stored for the @a controller handle. If no valid 
*             handle exists, details will be stored for handle 
*             @ref OCI_NO_HANDLE.
* @coding     Internally, this function should call the specialized
*             controller configuration API.
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCIG_GetControllerDataStructure(
    OCI_ControllerHandle controller, 
    OCIG_DataStructureSelector selector,
    void* dataStructure,
    uint32 size,
    uint32* count);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCIG_GetControllerDataStructure)(
    OCI_ControllerHandle controller, 
    OCIG_DataStructureSelector selector,
    void* dataStructure,
    uint32 size,
    uint32* count);


/** 
* Set the current driver configuration. The information passed as
* @a dataStructure must match the description returned by 
* @ref OCIG_GetControllerDataStructureParameter().
* Since reconfiguration support is very limited, the only
* valid value for @a selector is @ref OCIG_STRUCTURE_PROPERTIES.
* @param[in]  controller     is the interface instance of the 
*                            controller to query. 
* @param[in]  selector       specifies for which structure shall be
*                            returned. @ref OCIG_STRUCTURE_PROPERTIES
*                            is the only value allowed here.
* @param[in]  dataStructure  is a pointer to the buffer that contains 
*                            the configuration structure.
*                            Must not be @c NULL.
* @param[in]  size           is the size of the @a dataStructure 
*                            buffer in bytes. Must not be 0.
* @return     If the operation was successful, @ref OCI_SUCCESS will 
*             be returned. @ref OCI_ERR_INVALID_PARAMETER will be
*             returned if the configuration or properties buffer
*             do not meet the size requirements of the controller
*             associated with the @a controller handle. Otherwise
*             a specific error code will be returned.
* @coding     Internally, this function should call the specialized
*             controller configuration API.
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCIG_SetControllerDataStructure(
    OCI_ControllerHandle controller, 
    OCIG_DataStructureSelector selector,
    void* dataStructure,
    uint32 size);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCIG_SetControllerDataStructure)(
    OCI_ControllerHandle controller, 
    OCIG_DataStructureSelector selector,
    void* dataStructure,
    uint32 size);


/** 
* Open (activate / begin use of) a controller interface instance. 
* The configuration provided must either be compatible with the 
* controllers' current setup or this must be the only open controller 
* instance. Moreover it must match the controller's configuration 
* structure specification, which could be find out by a call of 
* @ref OCIG_GetControllerDataStructureParameter().
* Opening a controller interface instance fails, if the required 
* access cannot be granted, the configuration does not match or if 
* some other requirement cannot be met.
* @param[in]  controller         is the interface instance of the 
*                                controller to query. 
* @param[in]  configuration      is a pointer to the buffer that 
*                                contains the configuration structure.
* @param[in]  configurationSize  is the size of the configuration 
*                                buffer in bytes.
* @param[in]  properties         is a pointer to the buffer that 
*                                contains the properties.
* @param[in]  propertiesSize     is the size of the properties buffer 
*                                in bytes.
* @return     If the operation was successful, @ref OCI_SUCCESS will 
*             be returned. @ref OCI_ERR_INVALID_PARAMETER will be
*             returned if the configuration or properties buffer
*             do not meet the size requirements of the controller
*             associated with the @a controller handle. Otherwise
*             a specific error code will be returned.
* @coding     Internally, this function should call the specialized
*             controller configuration API.
* @sa         @ref GROUP_OCIG_CONTROLLER "Controller Handling"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCIG_OpenController(
    OCI_ControllerHandle controller, 
    void* configuration, 
    uint32 configurationSize,
    void* properties, 
    uint32 propertiesSize); 
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCIG_OpenController)(
    OCI_ControllerHandle controller, 
    void* configuration, 
    uint32 configurationSize,
    void* properties, 
    uint32 propertiesSize); 


/**
* Release the resources of the controller. The RX and TX 
* queues are released. It is not possible to access them any more. 
* Status, configuration and properties can be read. Nevertheless a 
* different instance may modify the configuration at any time.
* @param[in]  controller     is the interface instance of the 
*                            controller to close.
* @return     If the operation was successful, @ref OCI_SUCCESS will 
*             be returned. Otherwise an error code will be returned.
* @coding     Internally, this function should call the specialized
*             controller API.
* @sa         @ref GROUP_OCIG_CONTROLLER "Controller Handling"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode
OCI_CALL OCIG_CloseController(
    OCI_ControllerHandle controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCIG_CloseController)(
    OCI_ControllerHandle controller);

/**
* Destroy an interface instance of the controller. This 
* function implicitly call @ref OCIG_CloseController() if the 
* controller is not already in the closed state. After this call 
* the handle @a controller is invalid.
* @param[in] controller  is the handle of the controller 
*                        interface instance to destroy.
* @return    If the operation was successful and the controller 
*            interface instance was destroyed, @ref OCI_SUCCESS will 
*            be returned. Otherwise, an error code will be returned. 
* @sa        @ref GROUP_OCIG_CONTROLLER "Controller Handling"
*/

#ifndef OCI_DYNAMICDLL
OCI_DECLSPEC OCI_ErrorCode 
OCI_CALL OCIG_DestroyController(
     OCI_ControllerHandle controller);
#endif

typedef OCI_ErrorCode
(OCI_CALL *PF_OCIG_DestroyController)(
    OCI_ControllerHandle controller);


typedef struct OCIG_Ctrl_VTable
{
    PF_OCIG_GetControllerDataStructureParameter getControllerDataStructureParameter;
    PF_OCIG_GetControllerDataStructure getControllerDataStructure;
    PF_OCIG_SetControllerDataStructure setControllerDataStructure;

    PF_OCIG_OpenController openController;
    PF_OCIG_CloseController closeController;
    PF_OCIG_DestroyController destroyController;
} OCIG_Ctrl_VTable;

/** @} */

/** @} */


#ifdef __cplusplus
}
#endif

#include "..\Common\poppack.h"

#endif
