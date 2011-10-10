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
* @brief  IRefCountable definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IREFCOUNTABLE_INCLUDED__)
#define __OLI_IREFCOUNTABLE_INCLUDED__

// open ETAS::OLI namespace

#include "OLIDefs.h"
#include "OLITypes.h"
#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif


/** 
* @ingroup GROUP_OLI_COMMON_BASE
* @brief Base interface for life-time control.
*
* This is the base interface for all OLI objects that
* require life-time control to be shared between OLL
* and client application.
*
* A client application should not call the methods of 
* this interface directly. Instead it should use the
* @ref AutoPtr wrapper template to manage them implicitly.
*
* @remark All public methods are thread-safe.
* @since  BOA 1.3
* @see    AutoPtr
*/

OLI_INTERFACE IRefCountable
{
protected:

    /** @brief Destructor.

        The topmost interface requires a virtual destructor.
        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since BOA 1.3 
     */
    virtual ~IRefCountable() OLI_NOTHROW {};

public:

    /** @brief Increments the object's reference count.
        
        This method must be called when another reference has been 
        added to this instance.

        @return New reference count.
        @exception <none> This function must not throw exceptions.

        @remark Avoid calling @a AddRef manually. 
                Use the @ref AutoPtr wrapper template instead. 
        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    Release
     */
    virtual int32 OLI_CALL AddRef() const OLI_NOTHROW = 0;

    /** @brief Decrements the object's reference count. 
    
        If the reference count reaches 0, the object will be destroyed.
        This method must be called when another reference has been 
        added to this instance.

        @return New reference count.
        @exception <none> This function must not throw exceptions.

        @remark Avoid calling @a Release manually. 
                Use the @ref AutoPtr wrapper template instead. 
        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    AddRef
     */
    virtual int32 OLI_CALL Release() const OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IREFCOUNTABLE_INCLUDED__)
