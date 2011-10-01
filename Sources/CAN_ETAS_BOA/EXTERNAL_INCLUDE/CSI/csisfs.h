#ifndef ETAS_CSI_SFS_H
#define ETAS_CSI_SFS_H

/**
* @file       csisfs.h
* @brief      Public declaration of Search For Service (SFS) which is 
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
@page VersionHandling Version Handling

@section VersionInformation Version Information

Any version consists of four independent parts:

@li   majorVersion. 
- The major Version identifies the feature set of an Interface. This version is incremented
when some incompatible changes in the @ref BOA_ServiceId are made. When a server offers a higher
major version, than the client can access an error is generated. A server may offer several
different major versions at the same time.

@li   minorVersion.
- The minor Version identifies compatible extensions of an Interface. When a server offers additional
feature, but can still serve an older client this number is increased. 

@li   bugfix.
- The bugfix identifies corrections made in the implementation of the interface. The bugfix is
implementation specific and therefore it may have different values for different implementations of
the same feature set. The bugfix must not be used for any version check, but can only be used for
informational and debugging purposes on a specific hardware.

@li   build.
- The build revision allows the verification of a specific version during the development phase.

@section VersionNegotiation Version negotiation

During the binding of a protocol a negotiation of the versions of a service are done. The client requests a
version and the server delivers a version. In general the negotiation is done in three steps:

-# The client selects the major and minor version it wants to use. This version is transmitted as requested 
version to the server.  
-# The server receives the request and offers a proposed version. The major version of the proposed version
must not be identical to the the major version of the requested version. When the server does not offer
the requested major version it generates an error. The minor version may be not equal to the requested 
version. A higher minor version than the requested version identifies an server with additional feature.
The client can ignore this information. A lower version than the requested version identifies an older
interface. The client may not be able to use any service that is offered in the requested version. 
-# The client verifies the offered version. When the offered minor version is lower than the requested minor
version the client may adapt itself to the limited feature set of the older interface and continue operation 
or may stop the binding process and exit with an error.
*/

/**
* @addtogroup GROUP_CSI_SFS Search For Service (SFS)
* @brief      Search For Service (SFS) offers a way to detect any protocol or 
              hardware connected to the local system.
              It uses the @ref GROUP_CSI_COMMON API implemented on any node to 
              to generate a tree view of the available protocols and hardware
              systems. The root node requires additionally the @ref GROUP_CSI_COMPONENT
              interface to allow the detection of all installed BOA components.

              @image html "figure_csi_tree_view.gif"
              @htmlinclude "page_csi_search_for_service.html"
* @{
*/

#define CSI_MAX_NAME 128
#define MAX_REQUIRED_APIS 4

/** The uuid of the CSI_SFS API */
#define UUID_CSI      {0x85,0xC4,0x11,0xF4,0x1E,0x2E,0x4c,0x3f,0xA1,0x9E,0x57,0xF9,0xF2,0x38,0x87,0xB6}
/* {85C411F4-1E2E-4c3f-A19E-57F9F23887B6} */

/** Different node types and node classes delivered by CSI. The node types in 
    conjunction with the visibleNames of a node shall allow the setup of a view 
    to the hardware, which is helpful for the user.
    Internal structures like master and slave structures, used protocol types, etc.
    are not transparent to the most user. Physical visible objects, like interface
    ports in the host (Ethernet, USB, ExpressCard), separate systems (a physical box)
    and connectors (ports) inside of the Box (CAN, LIN, FlexRay) are more familiar
    to the user. */
typedef enum CSI_NodeType
{
   /** lower bound of any node. */
   CSI_NODE_MIN = 0,
   /** lower bound of any node with a physical representation - this class is used 
       for visible hardware nodes. */
   CSI_NODE_MIN_PHYSICAL_NODE = 0x2000,
   /** The component is a Port of a (Automotive Bus) Controller. */
   CSI_NODE_CONTROLLER_PORT,
   /** The component is a Sub-System - A Sub-System is a logical structural
       element, which can be used to visualize components of a modular hardware 
       system (different Interface Cards in a VME system etc) */
   CSI_NODE_SUBSYSTEM,
   /** The component is a system - A system is in normal cases identical to a
       physical pice of hardware with a separate housing. */
   CSI_NODE_SYSTEM,
   /** The component is a intermediate interface - A intermediate interface is
       used to connect different systems (or subsystems) with each other, A general
       rule for modeling a intermediate interface between two (sub-)systems is the
       expected knowledge and importance of the interface for the USER. */
   CSI_NODE_INTERMEDIATE_INTERFACE,
   /** Upper bound of nodes with a physical representation - Additional node types 
       may be inserted before this element, when they represent a physical visible component - 
       The generation of a tree-view should use the following constants to differ 
       between physical and logical components.*/ 
   CSI_NODE_MAX_PHYSICAL_NODE = 0x2fff,
   /** Lower bound of logical components like protocols, which are located on the URI
       endpoint or which are dependent on APIs which must be located on the endpoint.
       
       The generation of a tree-view should use this constant to limit the view to 
       logical components. The protocols can use the different values in the range 
       [CSI_NODE_MIN_LOGICAL_NODE..CSI_NODE_MAX_LOGICAL_NODE] to express the abstraction 
       layer of the protocol. Low Level protocols shall use values near the lower bound,
       high level protocols use values naer the upper bound. This mechanism allows the
       definitition of a range to filter for specific abstraction layer. */
   CSI_NODE_MIN_LOGICAL_NODE = 0x4000,
   /** The component is the CSI-Enumerate Component-Service. This is a protocol used by the
       CSI implementation itself to generate the different views. So it is a very low
       level protocol.*/ 
   CSI_NODE_PROTOCOL_CSI_ECS = 0x4100,
   /** The protocol represents the virtualization of a CAN, LIN or FlexRay Controller 
       (or is equivalent to this abstraction layer) */
   CSI_NODE_PROTOCOL_OCI = 0x4200,
   CSI_NODE_PROTOCOL_OCIG = 0x4280,
   /** The protocol represents the OLL abstraction Layer (or equivalent) */
   CSI_NODE_PROTOCOL_OLI = 0x4800,
   /** The protocol represents an Automotive API (examples: XCP, CCP or UDS protocol) */
   CSI_NODE_PROTOCOL_AUTOMOTIVE_API = 0x4c00,
   /** Upper bound of logical components. */
   CSI_NODE_MAX_LOGICAL_NODE= 0x4fff,
   /** lower bound of an abstract logical component, which offers an abstract service. In 
       difference to the protocol range the location of the service is not important for 
       the execution. The binding process may access the service somewhere in the protocol
       stack. The service may be independent of lower APIs and not located on the endpoint
       of the URI path or may require lower APIs, which may be located not on the endpoint. 
       (Any service that does not interact with physical interfaces. e.g. some kind of data 
       conversion)*/
   CSI_NODE_MIN_SERVICE = 0x6000,
   /** upper bound of an abstract logical component. */
   CSI_NODE_MAX_SERVICE = 0x6fff,
   /** Upper bound of any node. */
   CSI_NODE_MAX = 0x7fff
} CSI_NodeType;

/** A range of nodes. This component can be used to filter search results for
    a specific class (range). When min is set to the same value as max the interval
    will deliver an exact match only. */
typedef struct CSI_NodeRange
{
   /** The lower bound of the interval. */
   CSI_NodeType min;
   /** The upper bound of the interval. */
   CSI_NodeType max;
} CSI_NodeRange;

/** Elements delivered by the the CSI_ServiceGetSubItems() call. */
typedef struct CSI_SubItem
{
   /**
      @brief Identification of the server API and server type.

      Any sub-item may require some parameters that are delivered by the @ref
      CSI_SubItem structure in the CSI_ServiceGetSubItems() call at the parent.
      The parameters are handled as a block of binary data by the identification
      routines.
   */
   BOA_ServiceIdParam server;
   /** Type of the node. */
   CSI_NodeType nodeType;
   /** URI compatible name for addressing and identifying the API. The URI name
       must be unique inside a specific layer of the path. Any sub-tree manages
       its own namespace. So it is allowed that names in different sub-trees and
       names on different layer are identical. */
   char uriName[CSI_MAX_NAME];
   /** Visible name used for GUI representation. This name (or a translation) is
       visible to the user. So it should be identical for the same functions and
       layer in different sub-trees. When this name is empty (visibleName[0] = 0x00)
       the uriName is used for data representation. */
   char visibleName[CSI_MAX_NAME];
   /** Quality of service / cost declaration for different requirements
       (latency, throughput, processing power, etc.). Reserved fields must
       set to 0x00. This element helps to rate different stacks which offer the same
       functionality, when a different deployment or a different number of intermediate
       layer can resolve the required protcol */
   sint32 reserved[8];
   /** number of valid elements in the requiredAPI array. */
   uint32 count;
   /** Array of server APIs required on the lower layer to offer the service. The server 
       must find all requiredAPI with an access protocol, that the server can access. 
       When a server supports different access methods to lower level APIs it is represented 
       by different entries. For each supported (combination of) requiredAPI->access a new 
       entry is generated. */
   BOA_ServiceId requiredAPI[MAX_REQUIRED_APIS];

} CSI_SubItem;

/** tree representation of the connected hardware */
typedef struct CSI_Tree
{
   /** information of the item. */
   CSI_SubItem item;
   /** linked list of all items in this hierarchy layer */
   struct CSI_Tree *sibling;
   /** reference to a sub-tree with all children of the node */
   struct CSI_Tree *child;
} CSI_Tree;


/** Create a tree with all sub-nodes representing a physical instance from a specific location 
    in the tree.
    The endpoint of the uriName will be the root of the created tree. The tree includes no 
    protocols, but only physical representations (@ref CSI_NodeType in the range 
    [CSI_NODE_MIN_PHYSICAL_NODE, CSI_NODE_MAX_PHYSICAL_NODE]. The memory is allocated and 
    managed by the driver. It must not be manipulated or released by the application.
    An implementation may choose to deliver a reference to a statically build tree.*/  
CSI_DECLSPEC OCI_ErrorCode 
CSI_CALL CSI_CreateHwTree(const char* uriName, CSI_Tree* *tree);

/** Release the access to a Hardware Tree */
CSI_DECLSPEC OCI_ErrorCode 
CSI_CALL CSI_DestroyHwTree(CSI_Tree* tree);


/** Create a tree with all sub-nodes from a specific location in the tree.
    The endpoint of the uriName will be the root of the created tree. The tree includes  
    protocols (which never have children) and physical nodes (which have no requiredAPIs).
    The memory is allocated and managed by the driver. It must not be manipulated or 
    released by the application.An implementation may choose to deliver a reference to a 
    statically build tree.*/  
CSI_DECLSPEC OCI_ErrorCode 
CSI_CALL CSI_CreateProtocolTree(const char* uriName, CSI_NodeRange range, CSI_Tree* *tree);

/** Release the access to a Protocol Tree */
CSI_DECLSPEC OCI_ErrorCode 
CSI_CALL CSI_DestroyProtocolTree(CSI_Tree* tree);

/*-------------------------------------------------------------------------------------------------------------*/

#define MAX_URI 256

typedef void* CSI_SFS_Handle;
typedef char OCI_URIName[MAX_URI];


/**
 Search for any Hardware and driver, that supports the Connection Service Interface,
 offers some BOA service and latch the result. 
 
 @param [out] sfsHandle - handle to access the search result for further processing.

 @return OCI_SUCCESS

 @status - preliminary / proposal for BOA V1.2
 */
CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_CreateSearchResult(CSI_SFS_Handle* sfsHandle);

/**
 Release all resources of a search result. The sfsHandle must not be used after this call.

 @param [in] sfsHandle - handle to a search result created by CSI_CreateSearchResult().

 @return OCI_SUCCESS

 @status - preliminary / proposal for BOA V1.2
 */
CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_DestroySearchResult(CSI_SFS_Handle sfsHandle);

/**
 Modifiy a filter for finding virtual driver. Per default a filter will hide any virtual 
 driver component. This call is only needed, when the application wants to access virtual
 driver.
 
 @param [in] sfsHandle - handle to a search result created by CSI_CreateSearchResult().

 @return OCI_SUCCESS

 @status - preliminary / proposal for BOA V1.2
*/
CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_FindVirtualDriver (CSI_SFS_Handle handle, bool doNotIgnoreVirtualDriver);


/**
 Fill an array with URI-Paths for a specific BOA_ServiceId. This service extracts all URI-Paths
 that support the requested BOA_SeriveId from a search result created by CSI_CreateSearchResult().

 @param [in] sfsHandle  A handle to a search result created by CSI_CreateSearchResult().
 @param [in] id         An id that is used to filter the search result. 
                        The id consists of an UUID and Version of the interface and an UUID and Version
						of the binding mechanism.
 @param [out] uriName   Application supplied array of OCI_URIName with @a size elements that will be filled 
                        with the URIs.
 @param [in] size       Size of  the @a uriName array. The function will not deilver more than @a size elements.
						When more than @size elements are found the function will return with OCI_ERR_BUFFER_OVERFLOW
						and @a count will deliver the number of found elements.
 @param [out] count     number of valid elements in the @a uriName array.

 @return OCI_SUCCESS
		 OCI_ERR_BUFFER_OVERFLOW when more than @a size elements are found.

 @status - preliminary / proposal for BOA V1.2
*/
CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_GetUriForServiceId (CSI_SFS_Handle handle, 
                                      const BOA_ServiceId* id, 
                                      OCI_URIName uriName[], int size, int* count);


/** information about a BOA interface */
typedef struct BOA_ApiInfo
{
   /** uri path of the interface */
   char uriName[MAX_URI];
   /** service id of the interface */
   BOA_ServiceId service;
} BOA_ApiInfo;

CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_GetInterfaceByServiceId (CSI_SFS_Handle handle, 
                                           const BOA_ServiceId* id, 
                                           BOA_ApiInfo* found, int size, int* count);

CSI_DECLSPEC OCI_ErrorCode CSI_CALL CSI_GetInterfaceByNodeRange (CSI_SFS_Handle handle, 
                                           const CSI_NodeRange* range, 
                                           BOA_ApiInfo* found, int size, int* count);


/** 
* @} 
*/

#ifdef __cplusplus
}
#endif

#include "..\Common\poppack.h"

#endif
