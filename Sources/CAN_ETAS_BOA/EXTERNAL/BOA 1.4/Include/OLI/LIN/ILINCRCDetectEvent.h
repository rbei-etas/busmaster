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
* @brief  ILINCRCDetectEvent definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ILINCRCDETECTEVENT__INCLUDED__)
#define __OLI_ILINCRCDETECTEVENT__INCLUDED__

// include used interface and constants

#include "LINBase.h"
#include "../Common/IEvent.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_LIN_MESSAGES
* @brief  LIN frame CRC detection event.
*
* Specific structure for a message concerning checksum
* calculation method of an unconfigured receive LIN frame.
* If an unconfigured frame is received, the hardware
* detects the according checksum calculation method by itself.
* The message occurs only once for the according LIN ID.
*
* Extends the base interface by adding a method to access the @ref
* GetCRCType "CRC type being detected". 
*
* This interface's implementation of @ref IMessage::GetID returns the @ref IMessage::GetID "frame ID"
* of the frame that triggered the CRC detection event.
*
* @remark All public methods are thread-safe.
* @remark The lifetime of all objects implementing this interface 
*         is defined by the @ref IRxQueue "receive queue" instance
*         that contains them.
* @since  BOA 1.3
* @see    IRxQueue, ILINLink, ILINCRCDetectEventFilter
*/

OLI_INTERFACE ILINCRCDetectEvent : public IEvent
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class are controlled by the receiving queue.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINCRCDetectEvent() OLI_NOTHROW {};

public:

    /** The unique identifier for the type of this interface
        and will be returned by @ref IMessage::GetType.
     */
    enum {TYPE = LIN_TYPE_EVENT_BASE + 7};

    /** @brief  Get the detected CRC type.
        
        @return Detected CRC type. 
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

#endif // !defined(__OLI_ILINCRCDETECTEVENT__INCLUDED__)
