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
* @brief  IMessage definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IMESSAGE_INCLUDED__)
#define __OLI_IMESSAGE_INCLUDED__

// open ETAS::OLI namespace

#include "OLIDefs.h"
#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_COMMON_MESSAGES
* @brief Base interface for all frames and events.
*
* The common abstraction is that every message has a
* @ref GetType "type" and an @ref GetID "ID". Together,
* they specify how the message content is to be interpreted.
*
* The type denotes the C++ class, i.e. equals the @a TYPE 
* member of the respective interface. Received CAN frames, 
* for example, will return @ref ICANRxFrame::TYPE.
* OLI has been designed to allow for static casts based
* on that information:
* 
* @code
* void OLI_CALL CMyObserver::Trigger (IRxQueue* queue) OLI_NOTHROW
* {
*     const IRxMessage* message = queue->GetOldest();
*     if (message->GetType() == ICANRxFrame::TYPE)
*     {
*         const ICANRxFrame* frame = static_cast<const ICANRxFrame*>(message);
*         std::cout << "recieved " << frame->GetSize() << " payload bytes\n";
*     }
*     queue->Pop();
* }
* @endcode
*
* The @ref GetID "ID" property is best understood in the case of data frames: here it is usually the frame ID
* in some bus-specific encoding. However @em any subclass of IMessage must implement the @ref IMessage::GetID
* method, even subclasses such as IQueueEvent which may not seem to have a property which can be interpreted as
* an "ID". Please refer to the documentation of the various subclasses of IMessage for information about how
* each implements @ref IMessage::GetID.
*
* @remark All public methods are thread-safe.
* @remark Receive messages are owned by the receiving queue. 
*         Send messages are owned by the client application.
*         Hence, there is no need for reference counting.
* @since  BOA 1.3
* @see    IQueue, IFilter
*/

OLI_INTERFACE IMessage
{
protected:

    /** @brief Destructor.

        The topmost interface requires a virtual destructor.
        This destructor has been hidden since objects implementing 
        this class are controlled by the OLL.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IMessage() OLI_NOTHROW {};

public:

    /** @brief Get the message class.
        
        Returns the @a TYPE member of the most specific C++ interface 
        class that is supported by this message. If it equals
        @ref ICANRxFrame::TYPE, for instance, you may cast the message
        to @ref ICANRxFrame or any of its base classes.

        @return @a TYPE of the C++ interface.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
	virtual uint32 OLI_CALL GetType() const OLI_NOTHROW = 0;

    /** @brief @ref GetType "Type-specific" format information.
        
        If IMessage is implemented by an object which represents a data frame, then usually this method returns
        the frame's bus-specific ID. However @em any subclass of IMessage must implement this method, even
        subclasses such as IQueueEvent which may not seem to have a property which can be interpreted as
        an "ID". Please refer to the documentation of the various subclasses of IMessage for information about how
        each implements @ref IMessage::GetID.

        @return The type-specific format ID.
        @exception <none> This function must not throw exceptions.

        @remark The use of this member is specific to the 
                respective interface type. Please see there
                for detailed information.
        @since  BOA 1.3 
     */
	virtual uint32 OLI_CALL GetID() const OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_IMESSAGE_INCLUDED__)
