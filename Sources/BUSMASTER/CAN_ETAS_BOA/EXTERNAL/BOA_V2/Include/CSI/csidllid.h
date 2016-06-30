/**
* @file       csidllid.h
* @brief      Interface and node IDs for the Connection Service Interface (CSI).
* @copyright  Copyright (c) 2008 ETAS GmbH. All rights reserved.
*
* $Revision: 5976 $
*/

#ifndef ETAS_CSI_DLL_ID_H
#define ETAS_CSI_DLL_ID_H

#include "csidefs.h"
#include "..\OCI\ocitypes.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
    * @addtogroup GROUP_CSI_COMPONENT Enumerate Component Service (ECS)
    * @brief      Common declaration of a service required to find all BOA-APIs implemented
                  in a DLL for a specific parent node.
    @details

     This service is required for any DLL that implements some sub-service (node or protocol) of the root node.

    * @{
    */

    /** definition of the uuid of the one and only root node. This definition is used by the
        CSI_IdentifyComponent() call to identify the requesting node (parentNodeId) as the root
        node. Any implementation that reuses the mechanism used by the root must define its own
        parent UUID. */
#define UUID_NODE_ROOT {0xd3,0x68,0xef,0x7d,0x25,0xdf,0x4a,0xbe,0xbe,0x62,0xdb,0x56,0x53,0xaa,0xd1,0x69}
    /*{0xd368ef7d, 0x25df, 0x4abe, { 0xbe, 0x62, 0xdb, 0x56, 0x53, 0xaa, 0xd1, 0x69 } };*/

    /**
     Find all sub-items of the accessed DLL for a specific parent. This mechanism allows the
     extension of the supported sub-services (nodes and protocols) by the installation of additional
     DLLs. This mechanism is used for the root node. Any different node may use a private mechanism
     to find its children. Espacially many DLLs may cover a complete sub-tree and not access
     any different DLL when the application browses inside of the sub-tree.

     @param[in]  parentNodeId   UUID of the parent node. This uuid limits the returned
                                result to the APIs, which are children of the requested node.
                                Any DLL, that implements a sub-node of the root system must response to
                                the UUID_NODE_ROOT and deliver at least one item describing the
                                @ref GROUP_CSI_SFS-API.

     @param[out] result         CSI-results for that node. The memory for the result is supplied
                                by the caller. It is large enough to receive @p size elements.
     @param[in]  size           The number of elements in @p result[].
     @param[out] count          The number of elements in @p result[] which were used.
     @param[in]  szUriNameHint  If this is non-NULL it provides a hint that the caller is looking for a sub-service
                                which has a particular URI name. The function may choose to ignore the hint and
                                output all sub-services regardless, but the hint allows the function to optimise its
                                implementation, if possible, by only outputting those sub-services whose URI name
                                matches @p szUriNameHint, or which can be used to reach the sub-service named
                                by @p szUriNameHint.
                                NOTE that an implementation of this function should ignore @p szUriNameHint unless it
                                is absolutely certain that the hint can be applied. If in doubt, do not apply the hint.
    */
    CSI_DECLSPEC OCI_ErrorCode
    CSI_CALL CSI_IdentifyComponent( BOA_UuidBin* parentNodeId, CSI_SubItem result[], uint32 size, uint32* count, const char* szUriNameHint );

    typedef OCI_ErrorCode (CSI_CALL* PF_CSI_IdentifyComponent)( BOA_UuidBin* parentNodeId, CSI_SubItem result[], uint32 size, uint32* count, const char* szUriNameHint );

#define UUID_CSI_ECS {0xf0,0xfd,0x05,0x0b,0x2c,0xbc,0x42,0xac,\
        0xac,0x38,0x70,0x5e,0x8a,0x48,0xdc,0x77}

    typedef struct CSI_ECS_VTable
    {
        PF_CSI_IdentifyComponent identifyComponent;
    } CSI_ECS_VTable;

    /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif /* ETAS_CSI_DLL_ID_H */
