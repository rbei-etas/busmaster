/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4794 $
*/

/** 
* @file
* @brief  IInternalErrorEvent definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/
#if !defined(__OLI_IINTERNALERROREVENT__INCLUDED__)
#define __OLI_IINTERNALERROREVENT__INCLUDED__

// include used interface and constants

#include "CommonBase.h"
#include "IEvent.h"
#include "OLITypes.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_COMMON_MESSAGES
* @brief Interface for internal error events.
*
* Extends the base interface by adding a method to @ref
* GetErrorCode "get the specific error code". Internal error
* messages are caused by failures in the driver stack itself
* such as losing the connection between PC and bus controller
* device. 
*
* Once such events occur, you will need to close the respective 
* @ref ILink instance and open a new one. The OLI implementation 
* will automatically disconnect the old link instance and treat 
* it as defunc as soon as it detects such an event.
*
* This interface's implementation of @ref IMessage::GetID returns an @ref InternalErrorEventCode value.
*
* @remark All public methods are thread-safe.
* @remark The lifetime of all objects implementing this interface 
*         is defined by the @ref IRxQueue "receive queue" instance
*         that contains them.
* @since  BOA 1.3
* @see    IRxQueue, ILink, IInternalErrorEventFilter
*/

OLI_INTERFACE IInternalErrorEvent : public IEvent
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IInternalErrorEvent() OLI_NOTHROW {};

public:

    /** The unique identifier for the type of this interface
        and will be returned by @ref IMessage::GetType.
     */
    enum {TYPE = COMMON_TYPE_EVENT_BASE + 1};

    /** @brief  Specific OCI error code.
        
        @return The errorCode field of the associated @ref OCI_InternalErrorEventMessage. May be 0, depending on the
        point of failure.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual OCI_ErrorCode OLI_CALL GetErrorCode() const OLI_NOTHROW = 0;

    /** @brief  Human readable error message.
        
        @return Error message. Never NULL.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual const char* OLI_CALL GetErrorMessage() const  OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IINTERNALERROREVENT__INCLUDED__)
