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
* @brief  ILINRxFrame definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ILINRXFRAME_INCLUDED__)
#define __OLI_ILINRXFRAME_INCLUDED__

// include used interface and constants

#include "LINBase.h"
#include "../Common/IRxFrame.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_LIN_MESSAGES
* @brief  A received LIN bus data frame.
*
* LIN frames only add the @ref GetCRCType "CRC calculation method"
* to the common frame abstraction.
*
* This interface's implementation of @ref IMessage::GetID returns the message ID of the LIN frame.
*
* @remark All public methods are thread-safe.
* @remark The lifetime of all objects implementing this interface 
*         is defined by the @ref IRxQueue "receive queue" instance
*         that contains them. Consequently, the same applies to the 
*         payload as well.
* @since  BOA 1.3
* @see    IRxQueue, ILINLink
*/

OLI_INTERFACE ILINRxFrame : public IRxFrame
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINRxFrame() OLI_NOTHROW {};

public:

    /** The unique identifier for the type of this interface
        and will be returned by @ref IMessage::GetType.
     */
    enum {TYPE = LIN_TYPE_FRAME_BASE + 1};

    /** @brief  Get the CRC calculation method.
        
        @return CRC calculation method. 
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual LINCRCType OLI_CALL GetCRCType() const OLI_NOTHROW = 0;
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINRXFRAME_INCLUDED__)
