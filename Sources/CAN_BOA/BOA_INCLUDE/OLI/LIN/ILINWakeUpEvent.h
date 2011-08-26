///////////////////////////////////////////////////////////
//  ILINWakeUpEvent.h
//  Defines the ILINWakeUpEvent interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINWAKEUPEVENT__INCLUDED__)
#define __OLI_ILINWAKEUPEVENT__INCLUDED__

// include used interface and constants

#include "LINBase.h"
#include "../Common/IEvent.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_MESSAGES

/// @{

/** Represents the OCI_LINWakeUpEventMessage event.
*/

OLI_INTERFACE ILINWakeUpEvent : public IEvent
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~ILINWakeUpEvent() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = LIN_TYPE_EVENT_BASE + 2};

    /// \ref ILINWakeUpEvent::GetFlags returns 0 or a combination of the following flags

    enum Flags
    {
        WAKEUP_INTERNAL = 1
    };

    /// \name LIN-specific interface
	/// @{

    virtual uint16 OLI_CALL GetFlags() const OLI_NOTHROW = 0;

	/// @}
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINWAKEUPEVENT__INCLUDED__)
