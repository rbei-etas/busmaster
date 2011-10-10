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
* @brief  IFlexRayTxFrame definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_IFLEXRAYTXFRAME_INCLUDED__)
#define __OLI_IFLEXRAYTXFRAME_INCLUDED__

// include used interface and constants

#include "FlexRayBase.h"
#include "../Common/ITxFrame.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// forward declarations

class IFlexRayRxFrame;
class IFlexRayTxFrame;

/**
* @ingroup GROUP_OLI_FLEXRAY_MESSAGES
* @brief  This function instantiates an object supporting @ref IFlexRayTxFrame. 
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref IFlexRayTxFrame::Create().
*
* @param[in]  flags                
*         Additional information and settings of the FlexRay data message. 
*         @todo: give more detailed information
* @param[in]  data             
*         The TX frame payload.
* @param[in]  size
*         The number of valid bytes at @a data.
* @param[out] ppFlexRayTxFrame     
*         A pointer to an object supporting @ref IFlexRayTxFrame. The object 
*         is owned by the caller, i.e. the client application is expected
*         to eventually call the @ref ITxMessage::Destroy method.
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
* @remark LPDU-ID information (slot, clycle etc.) will implicitly addded
*         by transmit queue that the frame gets enqueued in. However, this
*         process will not alter the @a IFlexRayTxFrame instance itself;
*         it can still be added to multiple transmit queues at the same time.
* @since  BOA 1.3 
* @see    @ref BinaryCompatibility "binary compatibility", 
*         @ref ErrorReporting "error reporting",
*         IFlexRayTxFrame
*/
OLL_API IError* OLI_CALL IFlexRayTxFrame_Create1( 
    uint16 flags, 
    const byte* data, 
    uint8 size, 
    IFlexRayTxFrame** ppFlexRayTxFrame);

/**
* @ingroup GROUP_OLI_FLEXRAY_MESSAGES
* @brief  This function instantiates an object supporting @ref IFlexRayTxFrame. 
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref IFlexRayTxFrame::Create().
*
* @param[in]  received             
*         A RX FlexRay frame which will be used to populate the TX frame 
*         instances payload. Must not be @c NULL.
* @param[in]  flags                
*         Additional information and settings of the FlexRay data message. 
*         @todo: give more detailed information
* @param[out] ppFlexRayTxFrame     
*         A pointer to an object supporting @ref IFlexRayTxFrame. The object 
*         is owned by the caller, i.e. the client application is expected
*         to eventually call the @ref ITxMessage::Destroy method.
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
* @remark LPDU-ID information (slot, clycle etc.) will implicitly addded
*         by transmit queue that the frame gets enqueued in. However, this
*         process will not alter the @a IFlexRayTxFrame instance itself;
*         it can still be added to multiple transmit queues at the same time.
* @since  BOA 1.3 
* @see    @ref BinaryCompatibility "binary compatibility", 
*         @ref ErrorReporting "error reporting",
*         IFlexRayTxFrame
*/
OLL_API IError* OLI_CALL IFlexRayTxFrame_Create2( 
    const IFlexRayRxFrame* received, 
    uint16 flags, 
    IFlexRayTxFrame** ppFlexRayTxFrame);

/** 
* @ingroup GROUP_OLI_FLEXRAY_MESSAGES
* @brief  Interface for FlexRay transmit frames.
*
* This interface only adds @ref GetFlags "FlexRay data message settings".
*
* @ref IMessage::GetID returns the 0.
*
* @remark All public methods except @ref ITxMessage::Destroy are thread-safe.
* @remark The OLI @ref ITxQueue "transmit queues" will never assume
*         ownership over the message instances added to them.
*         Instead, the client is responsible of destroying the
*         message instances after usage.
* @remark LPDU-ID information (slot, clycle etc.) will implicitly addded
*         by transmit queue that the frame gets enqueued in. However, this
*         this process will not alter the @a IFlexRayTxFrame instance itself;
*         it can still be added to multiple transmit queues at the same time.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    ITxQueue, IFlexRayLink
*/

OLI_INTERFACE IFlexRayTxFrame : public ITxFrame
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class may need explicit destruction through @ref Destroy.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IFlexRayTxFrame() OLI_NOTHROW {};

public:

    /** The unique identifier for the type of this interface. 
        and will be returned by @ref IMessage::GetType.
     */
    enum {TYPE = FLEXRAY_TYPE_FRAME_BASE + 2};

    /** @brief  Additional information and settings of the FlexRay data 
                message.
        @todo   give more detailed information

        @return Flag set of additional information and settings of the 
                FlexRay data message.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual uint16 OLI_CALL GetFlags() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref IFlexRayTxFrame instance.

        @param[in]  flags                
                Additional information and settings of the FlexRay data 
                message. @todo: give more detailed information
        @param[in]  data             
                The TX frame payload.
        @param[in]  size             
                The number of valid bytes at @a data.
        @return New @ref IFlexRayTxFrame instance. The object is owned by the 
                caller, i.e. the client application is expected to eventually 
                call the @ref ITxMessage::Destroy method. This can be ensured 
                by wrapping the object pointer in an instance of the 
                @ref AutoDestroyPtr class.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark LPDU-ID information (slot, clycle etc.) will implicitly addded
                by transmit queue that the frame gets enqueued in. However, 
                thisprocess will not alter the @a IFlexRayTxFrame instance 
                itself; it can still be added to multiple transmit queues at 
                the same time.
        @remark This is a helper method which wraps 
                @ref IFlexRayTxFrame_Create1(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    AutoDestroyPtr, IFlexRayTxFrame_Create1
     */
    static IFlexRayTxFrame* OLI_CALL Create(
        uint16      flags,
        const byte* data,
        uint8       size)
    {
        IFlexRayTxFrame* pFlexRayTxFrame = NULL;
        CheckAndThrow( IFlexRayTxFrame_Create1( flags, 
                                                data, 
                                                size, 
                                                &pFlexRayTxFrame ) );
        return pFlexRayTxFrame;
    }

    /** @brief  Create an @ref IFlexRayTxFrame instance.

        @param[in]  received             
                A RX FlexRay frame which will be used to populate the TX frame 
                instances payload. Must not be @c NULL.
        @param[in]  flags                
                Additional information and settings of the FlexRay data 
                message. @todo: give more detailed information
        @return New @ref IFlexRayTxFrame instance. The object is owned by the 
                caller, i.e. the client application is expected to eventually 
                call the @ref ITxMessage::Destroy method. This can be ensured 
                by wrapping the object pointer in an instance of the 
                @ref AutoDestroyPtr class.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark LPDU-ID information (slot, clycle etc.) will implicitly addded
                by transmit queue that the frame gets enqueued in. However, 
                thisprocess will not alter the @a IFlexRayTxFrame instance 
                itself; it can still be added to multiple transmit queues at 
                the same time.
        @remark This is a helper method which wraps 
                @ref IFlexRayTxFrame_Create2(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    AutoDestroyPtr, IFlexRayTxFrame_Create2
     */
    static IFlexRayTxFrame* OLI_CALL Create(
        const IFlexRayRxFrame*  received,
        uint16                  flags )
    {
        IFlexRayTxFrame* pFlexRayTxFrame = NULL;
        CheckAndThrow( IFlexRayTxFrame_Create2( received, 
                                                flags, 
                                                &pFlexRayTxFrame ) );
        return pFlexRayTxFrame;
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_IFLEXRAYTXFRAME_INCLUDED__)
