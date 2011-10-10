/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4833 $
*/

/** 
* @file
* @brief  ICANTxFrame definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ICANTXFRAME_INCLUDED__)
#define __OLI_ICANTXFRAME_INCLUDED__

// include used interface and constants

#include "CANBase.h"
#include "../Common/ITxFrame.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// forward declarations

class ICANRxFrame;
class ICANTxFrame;

/**
* @ingroup GROUP_OLI_CAN_MESSAGES
* @brief This function instantiates an object supporting @ref ICANTxFrame. 
*
*        See @ref BinaryCompatibility "binary compatibility" 
*        for an explanation of why it is needed.
*
*        NOTE that clients are encouraged to access this function 
*        via the wrapper @ref ICANTxFrame::Create().
*
* @param[in]  id               
*         The CAN message ID of the new TX frame.
* @param[in]  extendedID       
*         Indicates whether @a id is an extended CAN ID (29 bits) or 
*         a standard CAN ID (11 bits). The resulting frame instance
*         will return this value in @ref ICANTxFrame::IsExtendedID.
* @param[in]  data             
*         The TX frame payload, containing up to 8 bytes.
* @param[in]  dlc             
*         The data length code (DLC) of the CAN frame. This usually indicates the number of bytes at @a data, but
*         can sometimes be > 8: see @ref ICANTxFrame::GetDLC for more details.
* @param[out] ppCanTxFrame     
*         A pointer to an object supporting @ref ICANTxFrame. The object 
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
*         ICANTxFrame
*/
OLL_API IError* OLI_CALL ICANTxFrame_Create1( 
    uint32 id, 
    bool extendedID, 
    const byte* data, 
    uint8 dlc, 
    ICANTxFrame** ppCanTxFrame);

/**
* @ingroup GROUP_OLI_CAN_MESSAGES
* @brief  This function instantiates an object supporting @ref ICANTxFrame. 
*
*         The resulting CAN frame will have the same playload and properties
*         as the received frame. This factory can thus be used to implement 
*         a bridge between two CAN busses, for instance.
*
*         See @ref BinaryCompatibility "binary compatibility" 
*         for an explanation of why it is needed.
*
*         NOTE that clients are encouraged to access this function 
*         via the wrapper @ref ICANTxFrame::Create().
*
* @param[in]  received         
*         A RX CAN frame which will be used to populate the TX frame instance.
*         Must not be @c NULL.
* @param[out] ppCanTxFrame     
*         A pointer to an object supporting @ref ICANTxFrame. The object 
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
*         ICANTxFrame
*/
OLL_API IError* OLI_CALL ICANTxFrame_Create2( 
    const ICANRxFrame* received, 
    ICANTxFrame** ppCanTxFrame);


/** 
* @ingroup GROUP_OLI_CAN_MESSAGES
* @brief  Interface for CAN transmit frames.
*
* Since frame ID and payload have already been made available in the base
* classes, this interface only adds the @ref IsExtendedID "extended ID"
* indicator flag as well as the class-specific @ref TYPE "type identifier".
*
* @ref IMessage::GetID returns the frame's CAN ID.
*
* @remark All public methods except @ref ITxMessage::Destroy are thread-safe.
* @remark The OLI @ref ITxQueue "transmit queues" will never assume
*         ownership over the message instances added to them.
*         Instead, the client is responsible of destroying the
*         message instances after usage.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    ITxQueue, ICANLink
*/

OLI_INTERFACE ICANTxFrame : public ITxFrame
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class may need explicit destruction through @ref Destroy.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ICANTxFrame() OLI_NOTHROW {};

public:

    /** The unique identifier for the type of this interface. 
        and will be returned by @ref IMessage::GetType.
     */
    enum {TYPE = CAN_TYPE_FRAME_BASE + 2};

    /** @brief  Returns whether the @ref IMessage::GetID "frame ID" is 
                an extended ID.

        @return @c true, if the @ref IMessage::GetID "frame ID" is and
                extended CAN ID (29 bits). Otherwise, it is an 11 bit
                standard CAN ID.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual bool OLI_CALL IsExtendedID() const OLI_NOTHROW = 0;

    /** @brief  Returns the data length code (DLC) of the CAN frame.

                Usually, the DLC indicates the number of data bytes in the frame, (i.e. the number of valid bytes
                returned by @ref ITxFrame::GetData) and is therefore <= 8 and equal to the value returned by
                @ref ITxFrame::GetSize.
                
                However, the CAN specification implicitly permits DLC values which are > 8 and <= 15, though in such a
                case the frame will still contain only 8 data bytes. Consequently, some CAN controllers are able to
                transmit a frame with DLC > 8 (though such a frame still only contains 8 data bytes). The OLL allows a
                caller to request the transmission of such a frame, though the underlying OCD may reject the frame with
                an error. When processing such a frame:
                    - @ref ICANTxFrame::GetDLC returns the frame's DLC (> 8), as supplied to ICANTxFrame::Create;
                    - @ref ITxFrame::GetData returns the frame's payload (= 8 bytes), as supplied to ICANTxFrame::Create;
                    - @ref ITxFrame::GetSize returns the number of bytes in the frame's payload (= 8);

        @return The data length code (DLC) of the CAN frame, as supplied to ICANTxFrame::Create.
        @exception <none> This function must not throw exceptions.

        @see    ITxFrame::GetData, ITxFrame::GetSize, ICANTxFrame::Create
     */
    virtual uint8 OLI_CALL GetDLC() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref ICANTxFrame instance.

        @param[in]  id               
                The CAN message ID of the new TX frame.
        @param[in]  extendedID       
                Indicates whether @a id is an extended CAN ID (29 bits) or 
                a standard CAN ID (11 bits). The resulting frame instance
                will return this value in @ref IsExtendedID.
        @param[in]  data             
                The TX frame payload, containing up to 8 bytes.
        @param[in]  dlc             
                The data length code (DLC) of the CAN frame. This usually indicates the number of bytes at @a data,
                but can sometimes be > 8: see @ref ICANTxFrame::GetDLC for more details.
        @return New @ref ICANTxFrame instance. The object is owned by the 
                caller, i.e. the client application is expected to eventually 
                call the @ref ITxMessage::Destroy method. This can be ensured 
                by wrapping the object pointer in an instance of the 
                @ref AutoDestroyPtr class.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ICANTxFrame_Create1(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    AutoDestroyPtr, ICANTxFrame_Create1
     */
    static ICANTxFrame* OLI_CALL Create 
        ( uint32 id
        , bool extendedID
        , const byte* data
        , uint8 dlc )
    {
        ICANTxFrame* pCanTxFrame = NULL;
        CheckAndThrow( ICANTxFrame_Create1( id, 
                                            extendedID, 
                                            data, 
                                            dlc, 
                                            &pCanTxFrame ) );
        return pCanTxFrame;
    }

    /** @brief  Create an @ref ICANTxFrame instance.

                The resulting CAN frame will have the same playload and 
                properties as the received frame. This factory can thus 
                be used to implement a bridge between two CAN busses, 
                for instance.

        @param[in]  received         
                A RX CAN frame which will be used to populate the TX frame 
                instance. Must not be @c NULL.
        @return New @ref ICANTxFrame instance. The object is owned by the 
                caller, i.e. the client application is expected to eventually 
                call the @ref ITxMessage::Destroy method. This can be ensured 
                by wrapping the object pointer in an instance of the 
                @ref AutoDestroyPtr class.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ICANTxFrame_Create2(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    AutoDestroyPtr, ICANTxFrame_Create2
     */
    static ICANTxFrame* OLI_CALL Create( const ICANRxFrame* received )
    {
        ICANTxFrame* pCanTxFrame = NULL;
        CheckAndThrow( ICANTxFrame_Create2( received, &pCanTxFrame ) );
        return pCanTxFrame;
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ICANTXFRAME_INCLUDED__)
