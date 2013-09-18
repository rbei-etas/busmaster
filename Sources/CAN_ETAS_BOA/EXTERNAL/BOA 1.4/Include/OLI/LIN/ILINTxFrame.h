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
* @brief  ILINTxFrame definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ILINTXFRAME_INCLUDED__)
#define __OLI_ILINTXFRAME_INCLUDED__

// include used interface and constants

#include "LINBase.h"
#include "../Common/ITxFrame.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// forward declarations

class ILINRxFrame;
class ILINTxFrame;

/**
* @ingroup GROUP_OLI_LIN_MESSAGES
* @brief  This function instantiates an object supporting @ref ILINTxFrame. 
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref ILINTxFrame::Create().
*
* @param[in]  id                
*         LIN frame ID.
* @param[in]  data             
*         The TX frame payload.
* @param[in]  dlc
*         The number of valid bytes at @a data.
* @param[in]  crcType
*         CRC calculation method to be used.
* @param[out] ppLinTxFrame     
*         A pointer to an object supporting @ref ILINTxFrame. The object 
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
* @since  BOA 1.3 
* @see    @ref BinaryCompatibility "binary compatibility", 
*         @ref ErrorReporting "error reporting",
*         ILINTxFrame
*/
OLL_API IError* OLI_CALL ILINTxFrame_Create1( 
    uint8 id, 
    const byte* data, 
    uint8 dlc, 
    LINCRCType crcType, 
    ILINTxFrame** ppLinTxFrame );

/**
* @ingroup GROUP_OLI_LIN_MESSAGES
* @brief  This function instantiates an object supporting @ref ILINTxFrame. 
*
*         The resulting LIN frame will have the same playload and properties
*         as the received frame. This factory can thus be used to implement 
*         a bridge between two LIN busses, for instance.
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref ILINTxFrame::Create().
*
* @param[in]  received         
*         A RX LIN frame which will be used to populate the TX frame instance.
*         Must not be @c NULL.
* @param[out] ppLinTxFrame     
*         A pointer to an object supporting @ref ILINTxFrame. The object 
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
* @since  BOA 1.3 
* @see    @ref BinaryCompatibility "binary compatibility", 
*         @ref ErrorReporting "error reporting",
*         ILINTxFrame
*/
OLL_API IError* OLI_CALL ILINTxFrame_Create2( 
    const ILINRxFrame* received, 
    ILINTxFrame** ppLinTxFrame );


/** 
* @ingroup GROUP_OLI_LIN_MESSAGES
* @brief  Interface for LIN transmit frames.
*
* Since frame ID and payload have already been made available in the base
* classes, this interface only adds the @ref GetCRCType "CRC method" to
* be used as well as the class-specific @ref TYPE "type identifier".
*
* @ref IMessage::GetID returns the frame's LIN ID.
*
* @remark All public methods except @ref ITxMessage::Destroy are thread-safe.
* @remark The OLI @ref ITxQueue "transmit queues" will never assume
*         ownership over the message instances added to them.
*         Instead, the client is responsible of destroying the
*         message instances after usage.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    ITxQueue, ILINLink
*/

OLI_INTERFACE ILINTxFrame : public ITxFrame
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class may need explicit destruction through @ref Destroy.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINTxFrame() OLI_NOTHROW {};

public:

    /** The unique identifier for the type of this interface. 
        and will be returned by @ref IMessage::GetType.
     */
    enum {TYPE = LIN_TYPE_FRAME_BASE + 2};

    /** @brief  The CRC calculation method to be used.

        @return The CRC calculation method to be used for this frame.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual LINCRCType OLI_CALL GetCRCType() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref ILINTxFrame instance.

        @param[in]  id                
                LIN frame ID.
        @param[in]  data             
                The TX frame payload.
        @param[in]  dlc
                The number of valid bytes at @a data.
        @param[in]  crcType
                CRC calculation method to be used.
        @return New @ref ILINTxFrame instance. The object is owned by the 
                caller, i.e. the client application is expected to eventually 
                call the @ref ITxMessage::Destroy method. This can be ensured 
                by wrapping the object pointer in an instance of the 
                @ref AutoDestroyPtr class.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ILINTxFrame_Create1(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    AutoDestroyPtr, ILINTxFrame_Create1
     */
    static ILINTxFrame* OLI_CALL Create 
        ( uint8         id
        , const byte*   data
        , uint8         dlc
        , LINCRCType    crcType )
    {
        ILINTxFrame* pLinTxFrame = NULL;
        CheckAndThrow( ILINTxFrame_Create1( id, 
                                            data, 
                                            dlc, 
                                            crcType, 
                                            &pLinTxFrame ) );
        return pLinTxFrame;
    }

    /** @brief  Create an @ref ILINTxFrame instance.

                The resulting LIN frame will have the same playload and 
                properties as the received frame. This factory can thus 
                be used to implement a bridge between two LIN busses, 
                for instance.

        @param[in]  received         
                A RX LIN frame which will be used to populate the TX frame 
                instance. Must not be @c NULL.
        @return New @ref ILINTxFrame instance. The object is owned by the 
                caller, i.e. the client application is expected to eventually 
                call the @ref ITxMessage::Destroy method. This can be ensured 
                by wrapping the object pointer in an instance of the 
                @ref AutoDestroyPtr class.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ILINTxFrame_Create2(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    AutoDestroyPtr, ILINTxFrame_Create2
     */
    static ILINTxFrame* OLI_CALL Create (const ILINRxFrame* received)
    {
        ILINTxFrame* pLinTxFrame = NULL;
        CheckAndThrow( ILINTxFrame_Create2( received, &pLinTxFrame ) );
        return pLinTxFrame;
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINTXFRAME_INCLUDED__)
