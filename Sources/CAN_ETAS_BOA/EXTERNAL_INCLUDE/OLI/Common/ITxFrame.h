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
* @brief  ITxFrame definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ITXFRAME_INCLUDED__)
#define __OLI_ITXFRAME_INCLUDED__

// include base interface

#include "ITxMessage.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_COMMON_MESSAGES
* @brief  Base interface for all transmit frames.
*
* Extends the base interface by methods providing access to 
* the frame's payload. That @ref GetData "payload" is accessed 
* as a plain 0-indexed byte array. @ref GetSize returns the
* number of valid bytes in it. Boundaries will not be checked.
*
* @remark All public methods except @ref ITxMessage::Destroy are thread-safe.
* @remark The OLI @ref ITxQueue "transmit queues" will never assume
*         ownership over the message instances added to them.
*         Instead, the client is responsible of destroying the
*         message instances after usage.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    ITxQueue
*/

OLI_INTERFACE ITxFrame : public ITxMessage
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class may need explicit destruction through @ref Destroy.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ITxFrame() OLI_NOTHROW {};

public:

    /** @brief  Retun the size of the payload in bytes.
        
        @return Number of valid bytes in the @ref GetData "payload".
                May be 0.
        @exception <none> This function must not throw exceptions.

        @coding This may be less than the size of the internal data buffer.

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

#endif // !defined(__OLI_ITXFRAME_INCLUDED__)
