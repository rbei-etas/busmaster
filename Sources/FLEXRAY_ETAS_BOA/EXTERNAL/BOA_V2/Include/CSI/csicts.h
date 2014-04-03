#ifndef ETAS_CSI_CTS_H
#define ETAS_CSI_CTS_H

/**
* @file       csicts.h
* @brief      Public declaration of Connect To Service (CTS) which is
*             a part of the Connection Service Interface (CSI).
* @copyright  Copyright (c) 2008 ETAS GmbH. All rights reserved.
*
* $Revision: 6202 $
*/

#include <stdlib.h> //size_t
#include "..\Common\boaservice.h"
#include "csidefs.h"
#include "csitypes.h"
#include "csisfs.h"
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
    * @{
    */

    /**
        This function creates an interface stack, which contains all the information required by the BOA framework
        to create an API session. The API session itself can be created by a subsequent call to a function such
        as @ref OCI_CreateCANControllerBind() (for OCI_CAN; similar functions exist for other BOA APIs), using the
        interface stack created by @ref CSI_CreateInterfaceStack().

        The use of @ref CSI_CreateInterfaceStack() together with (e.g.) @ref OCI_CreateCANControllerBind() has an
        important advantage over the use of (e.g.) @ref OCI_CreateCANController(), namely that the former approach
        allows the caller to choose the version of the OCI_CAN API to which he binds. See @ref BackwardsCompatibilityPage
        for more information about the role of the API version.

        @param[in] uriLocation  A URI of the form "ETAS://...", which identifies the location of the requested
                                API. Typically, this will identify the hardware device or controller which the
                                API is to use.
        @param[in] interfaceId  A UUID identifying the required API (e.g. @ref UUID_OCICAN for OCI_CAN), and a version
                                number identifying the required version of the API. See @ref BackwardsCompatibilityPage
                                for more information about the role of the API version.
        @param[out] stack       An interface stack which contains all the information required by the BOA framework
                                to create a session of the specified API version, at the specified location. This may
                                be used as input to a subsequent call to (e.g.) @ref OCI_CreateCANController(). It
                                should be destroyed by calling @ref CSI_DestroyInterfaceStack().

        @return
        <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
        <tr> <td> @ref BOA_SUCCESS </td>
            <td> The stack was built. The application must call CSI_DestroyInterfaceStack() when it no longer requires
            the stack, to release the resources used by the stack. </td>
        </tr>
        <tr> <td> @ref BOA_ERR_INVALID_PARAMETER </td>
            <td> @a uriLocation does not begin with "ETAS://", or @a interfaceId is NULL, or @a stack is NULL. </td>
        </tr>
        <tr> <td> @ref BOA_ERR_HW_NOT_PRESENT </td>
            <td> The URI identified by @a uriLocation cannot be reached. Probably the hardware specified by the URI is
            not connected.</td>
        </tr>
        <tr> <td> @ref BOA_ERR_NO_INTERFACE </td>
            <td> The requested API is not supported by the location identified by @a uriLocation. </td>
        </tr>
        <tr> <td> @ref BOA_ERR_NONUNIQUE_BIND_TARGET </td>
            <td> @a uriLocation does not identify a unique location, so a single interface stack cannot be calculated. </td>
        </tr>
        <tr> <td> @ref BOA_WARN_OLDER_VERSION_IS_BETTER </td>
            <td> This warning indicates that, although the outputted interface stack is valid, there exists an earlier version
            of the API identified by @a interfaceId, such that if the caller were to modify @a interfaceId to request that
            earlier version, the interface stack which would be calculated would be "better" (e.g., "more efficient",
            or "faster", or "more accurate"), than the interface stack outputted by this current invocation of the function. </td>
        </tr>
        </TABLE>
    */
    CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_CreateInterfaceStack
    (
        const char* uriLocation, const BOA_UuidVersion* interfaceId, BOA_ProtocolStack** stack
    );

    /**
        This function creates an interface stack, which contains all the information required by the BOA framework
        to create an API session. See @ref CSI_CreateInterfaceStack() for more information about the role of an
        interface stack. This function fulfils a similar purpose to @ref CSI_CreateInterfaceStack(), but allows
        the client to solve the following problem:

        When the client calls @ref CSI_CreateInterfaceStack(), the BOA framework performs a "seach for hardware" to
        discover whether the URI which was supplied to @ref CSI_CreateInterfaceStack() refers to a connected device.
        This search process is slow, and must be performed every time that @ref CSI_CreateInterfaceStack() is called.
        This can cause performance problems if the client needs to call @ref CSI_CreateInterfaceStack() several times
        in quick succession.

        To solve this problem, the client should follow these steps:
         -# Call @ref CSI_CreateProtocolTree() to perform a "search for hardware", creating a protocol tree to represent
            the currently-connected hardware. This is a slow operation.
         -# Call @ref CSI_CreateInterfaceStackFromTree() to create an interface stack, using the hardware tree obtained in
            the first step. This is a fast operation.
         -# If necessary, call @ref CSI_CreateInterfaceStackFromTree() again to create another interface stack, again
            using the hardware tree obtained in the first step.
         -# ...
         -# Call @ref CSI_DestroyProtocolTree() to destroy the protocol tree.

         Some notes:

         When calling @ref CSI_CreateProtocolTree(), the client must specify a node range filter via an instance of
         @ref CSI_NodeRange. This filter must be chosen so that it does not exclude any nodes from the protocol tree.

         When calling @ref CSI_CreateProtocolTree(), the client must specify a URI (@c URI1), and when calling
         @ref CSI_CreateInterfaceStackFromTree() the client must also specify a URI (@c URI2). It must be the case
         that:
          - either @c URI1 and @c URI2 are identical, e.g. @c URI1 is "ETAS://ETH/ES595:12345/CAN:1" and URI2 is
            "ETAS://ETH/ES595:12345/CAN:1";
          - or @c URI1 must refer to a superset of the hardware referred to by @c URI2, e.g. @c URI1 is
            "ETAS://ETH" and @c URI2 is "ETAS://ETH/ES595:12345/CAN:1".
         Using the latter option (@c URI1 refers to a superset of @c URI2) allows the caller even more flexibility:
         he can perform a "search for hardware" once, e.g. using the URI "ETAS://", then use the resulting protocol
         tree for @em any subsequent call to @ref CSI_CreateInterfaceStackFromTree(). For example:
    @verbatim
        CSI_NodeRange       nodeRange = {CSI_NODE_MIN, CSI_NODE_MAX};
        CSI_Tree*           pTree;
        BOA_ProtocolStack*  pStack1;
        BOA_ProtocolStack*  pStack2;
        BOA_UuidVersion     ociCanV1_0 = {UUID_OCICAN, {1,0,0,0}};  // v1.0.0.0 of OCI_CAN.
        BOA_UuidVersion     ociLinV1_0 = {UUID_OCILIN, {1,0,0,0}};  // v1.0.0.0 of OCI_LIN.

        // Search for all connected hardware. This is slow, but it is only done once.
        CSI_CreateProtocolTree( "ETAS://", nodeRange, &pTree );

        // Try to create an interface stack for OCI_CAN v1.0 on an ES595 with serial number 12345. This is quick.
        CSI_CreateInterfaceStackFromTree( "ETAS://ETH/ES595:12345/CAN:1", &ociCanV1_0, pTree, &pStack1 );

        // Try to create an interface stack for OCI_LIN v1.0 on a CanCardXL with serial number 67890. This is quick.
        CSI_CreateInterfaceStackFromTree( "ETAS://PCMCIA/CANCARDXL:67890/LIN:1", &ociLinV1_0, pTree, &pStack2 );

        // Create more interface stacks
        ...

        // Destroy the protocol tree. We do not want to keep it for too long, because it will become invalid as soon
        // as any hardware is connected or disconnected.
        CSI_DestroyProtocolTree( pTree );

        // Use the interface stacks which we have created.
        ...
    @endverbatim

        @param[in] uriLocation  A URI of the form "ETAS://...", which identifies the location of the requested
                                API. Typically, this will identify the hardware device or controller which the
                                API is to use.
        @param[in] interfaceId  A UUID identifying the required API (e.g. @ref UUID_OCICAN for OCI_CAN), and a version
                                number identifying the required version of the API. See @ref BackwardsCompatibilityPage
                                for more information about the role of the API version.
        @param[in] pTree        A protocol tree, created via @ref CSI_CreateProtocolTree(), which includes at least the
                                hardware device identified by @a uriLocation.
        @param[out] stack       An interface stack which contains all the information required by the BOA framework
                                to create a session of the specified API version, at the specified location. This may
                                be used as input to a subsequent call to (e.g.) @ref OCI_CreateCANController(). It
                                should be destroyed by calling @ref CSI_DestroyInterfaceStack().

        @return
        <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
        <tr> <td> @ref BOA_SUCCESS </td>
            <td> The stack was built. The application must call CSI_DestroyInterfaceStack() when it no longer requires
            the stack, to release the resources used by the stack. </td>
        </tr>
        <tr> <td> @ref BOA_ERR_INVALID_PARAMETER </td>
            <td> @a uriLocation does not begin with "ETAS://", or @a interfaceId is NULL, or @a stack is NULL. </td>
        </tr>
        <tr> <td> @ref BOA_ERR_HW_NOT_PRESENT </td>
            <td> The URI identified by @a uriLocation cannot be reached. Probably the hardware specified by the URI is
            not connected.</td>
        </tr>
        <tr> <td> @ref BOA_ERR_NO_INTERFACE </td>
            <td> The requested API is not supported by the location identified by @a uriLocation, or this location is not
            present in the hardware tree identified by @a pTree. The latter may be the case if the URI used to create
            @a pTree did not refer to a superset of the hardware identified by @a uriLocation (see documentation
            above).</td>
        </tr>
        <tr> <td> @ref BOA_ERR_NONUNIQUE_BIND_TARGET </td>
            <td> @a uriLocation does not identify a unique location, so a single interface stack cannot be calculated. </td>
        </tr>
        <tr> <td> @ref BOA_WARN_OLDER_VERSION_IS_BETTER </td>
            <td> This warning indicates that, although the outputted interface stack is valid, there exists an earlier version
            of the API identified by @a interfaceId, such that if the caller were to modify @a interfaceId to request that
            earlier version, the interface stack which would be calculated would be "better" (e.g., "more efficient",
            or "faster", or "more accurate"), than the interface stack outputted by this current invocation of the function. </td>
        </tr>
        </TABLE>
    */
    CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_CreateInterfaceStackFromTree
    (
        const char* uriLocation, const BOA_UuidVersion* interfaceId, CSI_Tree* pTree, BOA_ProtocolStack** stack
    );


    /** Destroy an interface stack previously created with either @ref CSI_CreateInterfaceStack() or
        @ref CSI_CreateInterfaceStackFromTree().

        @param[in] stack    The interface stack to be destroyed.

        @return
        <TABLE border="0" rules="rows" cellspacing="3" cellpadding="3">
        <tr> <td> @ref BOA_SUCCESS </td>
            <td> The stack was destroyed. The memory referred to by @a stack must no longer be used. </td>
        </tr>
        <tr> <td> @ref BOA_ERR_UNEXPECTED_NULL </td>
            <td> @a stack is NULL. </td>
        </tr>
        </TABLE>
        */
    CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_DestroyInterfaceStack(BOA_ProtocolStack* stack);


    /** signature used by microsoft for the non typesafe interface binding */
    typedef int (*CSI_ProcAddress)();

    typedef void* CSI_VTableHandle;

    CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_GetVTable(const BOA_ProtocolStack* stack, CSI_ProcAddress** table, const size_t tableSize, CSI_VTableHandle* handle);

    CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_ReleaseVTable(CSI_VTableHandle handle);

    /**
     * This struct contains information about the authentication status of an OCD DLL.
     */
    typedef struct CSI_OcdAuthInfo
    {
        const char* szOcdDllName;   /**< The name of the OCD DLL. */
        int         authResult;     /**< The result of calling EtasDoChallengeResponse() to authenticate the OCD DLL. */
    } CSI_OcdAuthInfo;

    /**
     * This function obtains authentication information about each local OCD DLL which is used by a given protocol stack.
     * Note that this function restricts itself to local OCD DLLs: authentication information about OCD DLLs located in remote
     * processes or machines will not be reported.
     *
     * See @ref DllAuthentication for a description of how OCD DLLs are authenticated.
     *
     * @param[in]      pStack       Any BOA protocol stack, created with CSI_CreateInterfaceStack().
     * @param[in, out] ocdAuthInfos An array supplied by the caller, which will be populated by this function.
     * @param[in]      count        The number of elements available in @p ocdAuthInfos.
     * @param[out]     pUsed        The number of elements used in @p ocdAuthInfos. The caller should expect a non-zero value to
     *                              be outputted.
     *
     * @return
     *  - OCI_ERR_INVALID_PARAMETER     @p ocdAuthInfos is NULL, or @p pUsed is NULL.
     *  - OCI_ERR_BUFFER_OVERFLOW       @p ocdAuthInfos is too small to receive all the available information.
     *  - OCI_ERR_INVALID_ACCESS_PARAM  @p pStack specifies an OCD DLL which is not known to the CSI.
     *  - OCI_SUCCESS                   Otherwise.
     */
    CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_GetOcdAuthInfo( const BOA_ProtocolStack* pStack, CSI_OcdAuthInfo ocdAuthInfos[], const uint32 count, uint32* pUsed );

    /**
     * This function indicates whether a given public key has been revoked (the CSI library maintains a list of
     * revoked public keys).
     *
     * A pointer to this function is intended to be used as an argument to EtasDoChallengeResponse().
     *
     * @param[in] pKey      A public key, which is expected to contain PUB_KEY_SIZE bytes.
     *
     * @return An indication of the whether @p pKey has been revoked.
     */
    CSI_DECLSPEC int CSI_CALL CSI_IsKeyRevoked( const unsigned char* pKey );

    /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif
