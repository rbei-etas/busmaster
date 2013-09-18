/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4412 $
*/

/** 
* @file
* @brief  IFlexRaySwitchMessage definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

/// @todo update doxygen comments

#if !defined(__OLI_IFLEXRAYSWITCHMESSAGE_INCLUDED__)
#define __OLI_IFLEXRAYSWITCHMESSAGE_INCLUDED__

// include used interface and constants

#include "FlexRayBase.h"
#include "../Common/ITxMessage.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// forward declarations

class IFlexRaySwitchMessage;

/**
* @ingroup GROUP_OLI_FLEXRAY_MESSAGES
* @brief  This function instantiates an object supporting 
*         @ref IFlexRaySwitchMessage. 
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref IFlexRaySwitchMessage::Create().
*
* @param[in]  txBufferIndex
*         Index of the controller-level send buffer to enable or disable.
*         Use @ref IFlexRayLink::GetTxBufferIndex to find a specific buffer.
* @param[in]  enable
*         If @c true, the send buffer will be enabled for the configured
*         LPDU-ID and the controller cannot receive frames from the bus
*         in that particular channel, slot and cycle number combination. 
* @param[out] ppFlexRaySwitchMsg
*         A pointer to an object supporting @ref IFlexRaySwitchMessage. 
*         The object is owned by the caller, i.e. the client application 
*         is expected to eventually call the @ref ITxMessage::Destroy method.
*         This can be ensured by wrapping the object pointer in an instance 
*         of the @ref AutoDestroyPtr class.
*
* @return A pointer to an interface based on @ref IError, describing 
*         the error which occurred during this function. @c NULL if 
*         no error occurred. See @ref ErrorReporting "error reporting"
*         for more information on how errors are reported.
*
* @exception <none> This function must not throw exceptions.
*
* @since  BOA 1.3 
* @see    @ref BinaryCompatibility "binary compatibility", 
*         @ref ErrorReporting "error reporting",
*         IFlexRaySwitchMessage
*/
OLL_API IError* OLI_CALL IFlexRaySwitchMessage_Create( 
    uint32 txBufferIndex, 
    bool enable, 
    IFlexRaySwitchMessage** ppFlexRaySwitchMsg );

/** 
* @ingroup GROUP_OLI_FLEXRAY_MESSAGES
* @brief  Interface for transmit buffer (de-)activation pseudo-messages.
*
* Certain application protocols, e.g. XCP on FlexRay, require LPDU-IDs
* to be dynamically allocated amongst the partipating nodes. Furthermore,
* typical FlexRay controller hardware is not capable of receing data
* if there as long as there is transmit buffer for the same LPDU-ID.
*
* Sending an instance of this pseudo-message to a transmit queue will
* activate or deactivate an existing transmit buffer.
*
* Its proccessing will be synchronized with all other messages in the
* queue: The buffer will be reconfigured after all previous messages
* got processed / sent to the bus and for all following messages, the
* new transmit buffer configuration will apply.
*
* @ref IMessage::GetID returns the same value as @ref GetTxBufferIndex.
*
* @remark All public methods except @ref ITxMessage::Destroy are thread-safe.
* @remark The OLI @ref ITxQueue "transmit queues" will never assume
*         ownership over the message instances added to them.
*         Instead, the client is responsible of destroying the
*         message instances after usage.
* @remark Pseudo-messages will not be sent to the bus but consumed by
*         the driver stack itself.
* @remark A queue will only be able to modify transmit buffers assigned to 
*         that queue. Pseudo-messages pertaining to other buffers will be
*         ignored.
* @remark Please note that deactivating transmit buffers may cause following
*         messages in the same queue to get "stuck", i.e. never be processed
*         if no suitable active transmit buffer remains. The only way to
*         resolve that situation would be either to call the @ref 
*         ITxQueue::Clear method or to close the queue entirely.
* @remark Changing the transmit buffer configuration may be unsafe in the
*         static segment.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    ITxQueue, IFlexRayLink, IFlexRayReconfigurationMessage
*
* @todo   verify that unsuitable reconf messages get ignored.
* @todo   can this be used to re-assign buffers from other queues?
*/

OLI_INTERFACE IFlexRaySwitchMessage : public ITxMessage
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class may need explicit destruction through @ref Destroy.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IFlexRaySwitchMessage() OLI_NOTHROW {};

public:

    /** The unique identifier for the type of this interface. 
        and will be returned by @ref IMessage::GetType.
     */
    enum {TYPE = FLEXRAY_TYPE_FRAME_BASE + 4};

    /** @brief  Transmit buffer to (de-)activate.
        
        @return Index of the transmit buffer to (de-)activate.
        @exception <none> This function must not throw exceptions.

        @remark The transmit buffer should be assigned to the 
                queue to which this message gets added. Otherwise,
                this message will be ignored.
        @since  BOA 1.3 
        @see    @ref IFlexRayLink::GetTxBufferIndex
     */
    virtual uint32 OLI_CALL GetTxBufferIndex() const OLI_NOTHROW = 0;

    /** @brief  New activation state.

                When this message has been processed by the @ref 
                ITxQueue "transmit queue", the @ref GetTxBufferIndex 
                "respective buffer" will be in the specified state.
                Only a disabled transmit buffer will allow for 
                frames to be received for the respective LPDU-ID.

        @return @c true, if the buffer is to be enabled; 
                @c false, if it will be disabled.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual bool OLI_CALL GetEnable() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref IFlexRaySwitchMessage instance.

        @param[in]  txBufferIndex
                Index of the controller-level send buffer to reconfigure.
                Use @ref IFlexRayLink::GetTxBufferIndex to find a specific
                buffer.
        @param[in]  txBufferIndex
                Index of the controller-level send buffer to enable or 
                disable. Use @ref IFlexRayLink::GetTxBufferIndex to find 
                a specific buffer.
        @param[in]  enable
                If @c true, the send buffer will be enabled for the 
                configured LPDU-ID and the controller hardware cannot 
                receive frames from the bus in that particular channel, 
                slot and cycle number combination. 

        @return New @ref IFlexRaySwitchMessage instance. The object 
                is owned by the caller, i.e. the client application is 
                expected to eventually call the @ref ITxMessage::Destroy 
                method. This can be ensured by wrapping the object pointer 
                in an instance of the @ref AutoDestroyPtr class.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref IFlexRaySwitchMessage_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    AutoDestroyPtr, IFlexRaySwitchMessage_Create
     */
    static IFlexRaySwitchMessage* OLI_CALL Create( 
        uint32 txBufferIndex, 
        bool enable )
    {
        IFlexRaySwitchMessage* pFlexRaySwitchMsg = NULL;
        CheckAndThrow( IFlexRaySwitchMessage_Create( txBufferIndex, 
                                                     enable, 
                                                     &pFlexRaySwitchMsg ) );
        return pFlexRaySwitchMsg;
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_IFLEXRAYSWITCHMESSAGE_INCLUDED__)
