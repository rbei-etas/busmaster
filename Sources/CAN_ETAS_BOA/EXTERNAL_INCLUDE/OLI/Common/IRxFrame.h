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
* @brief  IRxFrame definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IRXFRAME_INCLUDED__)
#define __OLI_IRXFRAME_INCLUDED__

// include base interface

#include "IRxMessage.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_COMMON_MESSAGES
* @brief Base interface for all received frames.
*
* Extends the base interface by methods providing access to 
* the frame's payload. That @ref GetData "payload" is accessed 
* as a plain 0-indexed byte array. @ref GetSize returns the
* number of valid bytes in it. Boundaries will not be checked.
*
* @remark All public methods are thread-safe.
* @remark The lifetime of all objects implementing this interface 
*         is defined by the @ref IRxQueue "receive queue" instance
*         that contains them. Consequently, the same applies to the 
*         payload as well.
* @since  BOA 1.3
* @see    IRxQueue
*/

OLI_INTERFACE IRxFrame : public IRxMessage
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IRxFrame() OLI_NOTHROW {};

public:

    /** @brief  Retun the size of the payload in bytes.
        
        @return Number of valid bytes in the @ref GetData "payload".
                May be 0.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    GetData
     */
	virtual uint32 OLI_CALL GetSize() const OLI_NOTHROW = 0;

    /** @brief  Retun a reference to the first payload byte. 
        
        The number of valid bytes in that buffer is given
        by @ref GetSize().

        @return Reference to the first byte of the internal 
                payload buffer. Never @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark For maximum efficiency, there are no boundary 
                checks nor can you assume any underlying minimum
                buffer size exceeding @ref GetSize elements.
                Any access beyond the @ref GetSize()'th element
                results in undefined behavior.
        @since  BOA 1.3 
        @see    GetSize
     */
    virtual const byte* OLI_CALL GetData() const OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IRXFRAME_INCLUDED__)
