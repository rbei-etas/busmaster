/**
* @file       csidllid.h
* @brief      Interface and node IDs for the Connection Service Interface (CSI).
* @copyright  Copyright (c) 2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4636 $
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
 This service may be used by any different parent, when the childeren of the parent are implemented in 
 different DLLs, too. In this case a UUID for the parent node has to be defined. The following figure 
 illustrates the component view and the relation of the different APIs of the Connection Service Interface.

    @image html "figure_csi_component.gif"
    @htmlinclude "page_csi_root.html"

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

 @param[in]  parentNodeId UUID of the parent node. This uuid limits the returned
                          result to the APIs, which are children of the requested node.
                          Any DLL, that implements a sub-node of the root system must response to
                          the UUID_NODE_ROOT and deliver at least one item describing the 
                          @ref GROUP_CSI_SFS-API. 

 @param[out] result CSI-results for that node. The memory for the result is supplied
                    by the caller. It is large enough to receive size elements.
 @param[in]  size   size of the result[] buffer in number of elements.
 @param[out] count  number of valid elements filled by the routine. 
*/
CSI_DECLSPEC OCI_ErrorCode 
CSI_CALL CSI_IdentifyComponent(BOA_UuidBin* parentNodeId, CSI_SubItem result[], uint32 size, uint32* count);

typedef OCI_ErrorCode (CSI_CALL *PF_CSI_IdentifyComponent)(BOA_UuidBin* parentNodeId, CSI_SubItem result[], uint32 size, uint32* count);

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
