#ifndef ETAS_CSI_CTS_H
#define ETAS_CSI_CTS_H

/**
* @file       csicts.h
* @brief      Public declaration of Connect To Service (CTS) which is 
*             a part of the Connection Service Interface (CSI).
* @copyright  Copyright (c) 2008 ETAS GmbH. All rights reserved.
*/

#include "..\Common\boaservice.h"
#include "csidefs.h"
#include "csitypes.h"
#include "..\OCI\ocierror.h"
#include "..\Common\pshpack1.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @addtogroup GROUP_CSI_CTS Connect To Service (CTS)
* @brief      Connect To Service (CTS) offers functions to resolve the uri-path
              to a protocol stack, load the driver for a specific API and support 
              the binding of the static library part to the dynamically found and 
              loaded parts of the API.

              @image html "figure_csi_binding.gif"
              @image html "figure_resolve_uri.gif"
              @htmlinclude "page_csi_connect_to_service.html"

*
* @remark     The implementation of this functionality may be 
*             dependent on the used Operating System (OS). 
*             Nevertheless the currently supported Operation Systems 
*             QNX and Microsoft Windows offer similar mechanism to 
*             load and bind a DLL dynamically.
* @n
* @n          MS-Windows:
*             @code
*             HMODULE LoadLibrary(LPCTSTR lpFileName);
*             FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName); 
*             @endcode
* @n
* @n          QNX:
*             @code
*             void* dlopen(const char* pathname, int mode);
*             void* dlsym(void* handle, const char* name);
*             @endcode
*


* @{
*/

/** Deliver a handle which offers an interface compatible to the
    requested interfaceId and accessing a location defined in uriLocation.
    This hModule-Instance can be used by a (statically linked library) to 
    map the APIinterface routines to the (dynamically loaded) implementation
    of the interface. The static linked library part will map additionally
    handles, which are created by the dynamically loaded part and which may
    be not unique within the API, when different modules are loaded and more
    instances are active.
     
    @param[in] uriLocation A string conform to the URI-addressing scheme, which
                           references the location and name of the requested service.
    @param[in] interfaceId A uuid and required version of the API and the required
                           access protocol and version .
                           Must be UUID_DYNAMIC_LOADED_DLL, V1.x for this function.
    @param[out] handle     A handle to the new interface stack. The caller can pass
                           this handle to functions like @ref CSI_GetProcAddresses to perform
                           the API binding. The owner of the interface stack is
                           obliged to call @ref CSI_DestroyInterfaceStack once the
                           stack is no longer needed.

    @return If the operation was successful @ref OCI_SUCCESS will be returned. */

CSI_DECLSPEC OCI_ErrorCode 
CSI_CALL CSI_CreateInterfaceStack
    (const char* uriLocation, BOA_UuidVersion* interfaceId, CSI_Handle *handle);

/** Destroy (close, if necessary, remove and clean up) a service interface stack instance. 
    The handle is no longer valid. Resources required for the InterfaceStack are released.

    @param[in]   handle  interface instance to be destroyed.
    
    @return If the operation was successful @ref OCI_SUCCESS will be returned. 

    @remark     Depending structures will be closed and destroyed as well. */
CSI_DECLSPEC OCI_ErrorCode 
CSI_CALL CSI_DestroyInterfaceStack(CSI_Handle handle);

/** pointer to a string of the function names used by the CSI_GetProcAddresses() call
    to bind an API.*/
typedef const char *CSI_ProcIdent;

/** signature used by microsoft for the non typesafe interface binding */ 
typedef int (*CSI_ProcAddress)();

/** @brief Obtains pointers to the functions that implement the top-level API in an interface stack.

    @ref CSI_CreateInterfaceStack identifies and loads interface drivers as necessary.
    Before the driver can be used, an interface-specific binding operation has to be
    performed using the resulting handle and CSI_GetProcAddresses.

    @param[in]   handle  interface instance to bind
    @param[in]   procIdents An array of @a count procedure identifiers that will be used
                 to bind the corresponding functions
    @param[out]  addresses A buffer capable of holding at least @a count procedure
                           addresses. For each procIdents[i], the corresponding address
                           is written to addresses[i]. If an identifier could not be resolved,
                           the corresponding entry will be set to NULL. The caller needs
                           to be prepared to generate an error condition if mandatory
                           procedures are missing. The procedure addresses need to be
                           cast to the correct function pointer type (according to the API),
                           including the correct calling convention, so they can be safely
                           used.
    @param[in]   count The number of procedures to bind, and the minimum number of addresses
                       that can be written.
*/
CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_GetProcAddresses(CSI_Handle handle,
    const CSI_ProcIdent *procIdents, CSI_ProcAddress *addresses, size_t count);

/** @brief Retrieves a protocol stack that contains information for binding an
           API

    The URI passed to CSI_CreateInterfaceStack contains information for
    connecting to a specific service. OCI_GetProtocolStack on the other hand
    provides the caller with access parameters that are necessary to bind the
    API such as IP addresses and other low-level information available as a
    result of the Search For Service mechanism.

    This information is gathered during explicit calls to Search For Service
    functions, or implicitely during the execution of CSI_CreateInterfaceStack.

    If the API to be bound cannot interpret the protocol stack, it is the
    API proxy's responsibility to translate the information contained therein
    to a format the API understands.

    @param[in]  handle          interface instance to bind
    @param[out] protocolStack   pointer to the protocol stack;
                                owned by the callee
*/
CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_GetProtocolStack(CSI_Handle handle,
    const BOA_ProtocolStack **protocolStack);

/** 
* @} 
*/

#ifdef __cplusplus
}
#endif

#include "..\Common\poppack.h"

#endif
