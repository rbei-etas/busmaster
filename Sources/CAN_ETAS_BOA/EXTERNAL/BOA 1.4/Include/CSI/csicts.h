#ifndef ETAS_CSI_CTS_H
#define ETAS_CSI_CTS_H

/**
* @file       csicts.h
* @brief      Public declaration of Connect To Service (CTS) which is 
*             a part of the Connection Service Interface (CSI).
* @copyright  Copyright (c) 2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4851 $
*/

#include <stdlib.h> //size_t
#include "..\Common\boaservice.h"
#include "csidefs.h"
#include "csitypes.h"
#include "..\OCI\ocierror.h"

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
                           this handle to functions like @ref OCI_CreateCANControllerBind to perform
                           the API binding. The owner of the interface stack is
                           obliged to call @ref CSI_DestroyInterfaceStack once the
                           stack is no longer needed.

    @return If the operation was successful @ref OCI_SUCCESS will be returned. */

CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_CreateInterfaceStack
(
    const char* uriLocation, const BOA_UuidVersion* interfaceId, BOA_ProtocolStack** stack
);


/** Destroy (close, if necessary, remove and clean up) a service interface stack instance. 
    The handle is no longer valid. Resources required for the InterfaceStack are released.

    @param[in]   handle  interface instance to be destroyed.
    
    @return If the operation was successful @ref OCI_SUCCESS will be returned. 

    @remark     Depending structures will be closed and destroyed as well. */
CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_DestroyInterfaceStack(BOA_ProtocolStack* stack);


/** signature used by microsoft for the non typesafe interface binding */ 
typedef int (*CSI_ProcAddress)();

typedef void* CSI_VTableHandle;

CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_GetVTable(const BOA_ProtocolStack* stack, CSI_ProcAddress **table, const size_t tableSize, CSI_VTableHandle* handle);

CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_ReleaseVTable(CSI_VTableHandle handle);

/** 
* @} 
*/

#ifdef __cplusplus
}
#endif

#endif
