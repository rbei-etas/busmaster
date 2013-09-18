/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4509 $
*/

/** 
* @file
* @brief  IEventFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IEVENTFILTER_INCLUDED__)
#define __OLI_IEVENTFILTER_INCLUDED__

// include base interface

#include "IFilter.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** @ingroup GROUP_OLI_COMMON_FILTERS
* @brief  Base interface for all @ref IEvent "event" filters. 
*
* This interface has been introduced for symmetry and structural
* reasons only. Just like the @ref IEvent, it does not add any
* public methods to the base interface.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    IEvent
*/

OLI_INTERFACE IEventFilter : public IFilter
{
protected:

    /** @brief  Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IEventFilter() OLI_NOTHROW {};
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IEVENTFILTER_INCLUDED__)
