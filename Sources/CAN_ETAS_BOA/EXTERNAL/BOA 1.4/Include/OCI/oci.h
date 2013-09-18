#ifndef ETAS_OCI_H
#define ETAS_OCI_H

/**
* @file       oci.h
* @brief      Public declarations of the Open Contoller Interface (OCI) API
*             for all supported I/O media.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4547 $
*
* @remarks    All strings are UNICODE UTF8 encoded.
*
* @coding     All pointer parameters can be @c NULL.
*             An implementation must explicitly check for @c NULL 
*             pointers and react appropriately.
*/

#include "ocican.h"
#include "ocilin.h"
#include "ociflx.h"

/**
* @page PAGE_OCI_OVERVIEW OCI - Overview
* @htmlinclude "page_oci_overview.html"
* @todo Add Figure with objects and their relation.
*
*
* @page PAGE_OCI_RECEIVE_MESSAGES OCI - Receive Messages
* Depending on callback functions and filters, there are different
* ways to receive events and data frames.
* 
* The following pseudo code shows the internal handling of incoming 
* messages:
* @code
* if (MessageIsFrame(message))
* {
*     if (MatchFrameFilter(message))
*     {            
*         if (onFrame.function != NULL)
*         {
*             // Call <onFrame.function> for receiving a frame.                    
*         }
*         else
*         {
*             // Write frame to receive queue.                   
*         }
*     }
* }
* else if (MessageIsEvent(message))
* {
*     if (onEvent.function != NULL)
*     {
*         if (MatchEventFilter(message, OCI_EVENT_DESTINATION_CALLBACK))
*         {
*             // Call <onEvent.function> for receiving an event.                    
*         }
*     }
*     if (MatchEventFilter(message, OCI_EVENT_DESTINATION_INBAND))
*     {
*         if (onFrame.function != NULL)
*         {
*             // Call <onFrame.function> for receiving an event.                    
*         }
*         else
*         {
*             // Write event to receive queue.
*         }
*     }
* }       
* @endcode
* @todo Verify pseudo code.
* @todo Add additional documentation.
*
*
* @page PAGE_BOA_ADDRESSINGSCHEME Addressing Scheme
* @htmlinclude "page_boa_addressingscheme.html"
*/

#endif
