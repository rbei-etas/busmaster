#ifndef ETAS_BOA_H
#define ETAS_BOA_H

/**
* @file       boa.h
* @brief      Doxygen-Documentation of Basic OpenAPI.
* @copyright  Copyright (c) 2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4509 $
*
*
* @mainpage Basic OpenAPI Overview
* <HR>
* @htmlinclude "page_boa_overview.html"
*
* @page PAGE_BOA_CONCEPTS BOA - Concepts
* @htmlinclude "page_boa_concepts.html"
*
* @page PAGE_BOA_ERROR_MANAGEMENT BOA - Error Management
* @htmlinclude "page_boa_error_management.html"
*
* @page PAGE_BOA_TIMER_MANAGEMENT BOA - Timer Management
* @htmlinclude "page_boa_time_management.html"
*
* @page PAGE_BOA_VCI_LIFECYCLE BOA - Virtual Controller Instance Lifecycle
* @htmlinclude "page_boa_lifecycle.html"
*
* @page PAGE_BOA_DATA_IO BOA - Data I/O
* @htmlinclude "page_boa_data_io.html"
*
* @page PAGE_BOA_GENERIC BOA - Generic Configuration
* @htmlinclude "page_boa_generic.html"
*
* @page PAGE_BOA_EXTENSIONS_CAN BOA - CAN Specifics
* @htmlinclude "page_boa_extensions_CAN.html"
*
* @page PAGE_BOA_EXTENSIONS_LIN BOA - LIN Specifics
* @htmlinclude "page_boa_extensions_LIN.html"
*
* @page PAGE_BOA_EXTENSIONS_FLEXRAY BOA - FlexRay Specifics
* @htmlinclude "page_boa_extensions_FlexRay.html"
*
* @page PAGE_BOA_GLOSSARY BOA - Glossary
* @htmlinclude "page_boa_glossary.html"
*
* @page PAGE_BOA_PROXIES BOA - Proxy Libraries
* @htmlinclude "page_boa_proxies.html"
*
* @page PAGE_BOA_MULTITHREADING BOA - Multi-Threading
* @htmlinclude "page_boa_multithreading.html"
*
* @page PAGE_BOA_FIGURES BOA - Figures
* <HR>
* @image html "figure_boa_error_code.gif"
* Figure 1: Error code structure
* <HR>
* @image html "figure_boa_controller_states.gif"
* Figure 2: State machine for virtual controller instances
* <HR>
* @image html "figure_boa_layers.gif"
* Figure 3: Protocol layers
* <HR>
* @image html "figure_boa_oli.gif"
* Figure 4: OLI vs. OCI
* <HR>
* @image html "figure_boa_pap_oci.gif"
* Figure 5: Typical OCI calling sequence
* <HR>
* @image html "figure_boa_modules.gif"
* Figure 6: Modules
* <HR>
* @image html "figure_boa_abstraction.gif"
* Figure 7: BOA as a means of hardware abstraction
* <HR>
* @image html "figure_boa_proxy.gif"
* Figure 8: Handle and call mapping structures within proxy libaries 
*
* @addtogroup GROUP_BOA_COMMON Common (BOA)
* @{
*
* @defgroup GROUP_BOA_GLOBAL_TYPES Global Types
* @ingroup GROUP_BOA_COMMON
*
* @defgroup GROUP_BOA_ERROR_MANAGEMENT Error Management
* @ingroup GROUP_BOA_COMMON
*
* @}
*
*
* @addtogroup GROUP_BOA_OCI Open Controller Interface (OCI)
* @brief    The Open Controller Interface (OCI) offers an access to 
*           the Automotive Bus Systems (CAN, LIN, FlexRay) via ETAS 
*           systems.\ It is a low level, bus specific interface, that 
*           allows an access to the bus systems on a frame based 
*           level.
* @details  @htmlinclude "page_oci_overview.html"
* @{
*
* @defgroup GROUP_OCI_COMMON OCI Common
* @ingroup GROUP_BOA_OCI
*
* @defgroup GROUP_OCI_CAN_CONTROLLER OCI CAN
* @ingroup GROUP_BOA_OCI
*
* @defgroup GROUP_OCI_LIN_CONTROLLER OCI LIN
* @ingroup GROUP_BOA_OCI
*
* @defgroup GROUP_OCI_FLEXRAY_CONTROLLER OCI FlexRay
* @ingroup GROUP_BOA_OCI
*
* @}
*
*
* 
*
*
* @addtogroup GROUP_BOA_CSI Connection Service Interface (CSI)
* @brief    The Connection Service Interface (CSI) implements a framework for the different 
            Basic OpenAPI components. 

  @details
  
  It offers a way to detect all nodes and protocols of the Basic OpenAPI, implements support routines 
  to bind the APIs and enables different views to the BOA. The different views to the elements of the
  BOA are required for the different use cases that are supported by the CSI.

  The main uses cases are
  - Support the installation of new hardware / protocol driver
  - Generate a tree view of the connected hardware and supported protocols
  - Map the user's view of the hardware and protocol driver to the internal deployment
  - Build and optimize a protocol stack for a specific BOA-API
  - support the proxy libraries of the BOA to bind a specific protocol stack
            
  The first figure illustrates the different elements of a system that offers the APIs of the Basic
  OpenAPI. It represents the logical view of the different BOA layer. The different boxes represent
  an implementation of some API for a specific hardware. When a module offers several instances of the
  same API (possibly on different layer of the user view) only one API is illustrated. The specific
  instance must be selected by the binding process in this case.
  @image html "figure_Boa_api_view.gif"
  
  @htmlinclude "page_csi_addressing_scheme.html"
  @image html "figure_boa_example_tree.gif"

  @htmlinclude "page_csi_binding.html"
  @image html "figure_csi_libs_and_dlls.gif"

  @htmlinclude "page_csi_protocol_stack.html"

*
* @{
*
* @defgroup GROUP_CSI_COMMON Enumerate Sub Service (ESS)
* @ingroup GROUP_BOA_CSI
*
* @defgroup GROUP_CSI_COMPONENT Enumerate Component Service (ECS)
* @ingroup GROUP_BOA_CSI
*
* @defgroup GROUP_CSI_SFS Search For Service (SFS)
* @ingroup GROUP_BOA_CSI
*
* @defgroup GROUP_CSI_CTS Connect To Service (CTS)
* @ingroup GROUP_BOA_CSI
*
* @defgroup GROUP_CSI_URI URI Handling
* @ingroup GROUP_BOA_CSI
*
* @}
*
*
* @addtogroup GROUP_BOA_OLI Open Link Interface (OLI)
* @brief    The Open Link Interface (OLI) offers a more abstract 
*           access to the Automotive Bus Systems.\ Advanced feature 
*           like filtering and dispatching on payload data is 
*           supported.
* @details  @htmlinclude "page_oli_overview.html"
* @{
*
* @defgroup GROUP_OLI_COMMON OLI Common
* @ingroup GROUP_BOA_OLI
*
* @defgroup GROUP_OLI_INTERFACES OLI Interfaces
* @ingroup GROUP_BOA_OLI
*
* @defgroup GROUP_OLI_BASE OLI Base
* @ingroup GROUP_BOA_OLI
*
* @defgroup GROUP_OLI_CAN OLI CAN
* @ingroup GROUP_BOA_OLI
*
* @defgroup GROUP_OLI_LIN OLI LIN
* @ingroup GROUP_BOA_OLI
*
* @defgroup GROUP_OLI_FLEXRAY OLI FlexRay
* @ingroup GROUP_BOA_OLI
*
* @}
*/

#endif

