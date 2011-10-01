///////////////////////////////////////////////////////////
//  ILINSleepEvent.h
//  Defines the ILINSleepEvent interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINSLEEPEVENT__INCLUDED__)
#define __OLI_ILINSLEEPEVENT__INCLUDED__

// include used interface and constants

#include "LINBase.h"
#include "../Common/IEvent.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_MESSAGES

/// @{

/** Represents the OCI_LINSleepEventMessage event.
*/

OLI_INTERFACE ILINSleepEvent : public IEvent
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~ILINSleepEvent() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = LIN_TYPE_EVENT_BASE + 3};

    /// \name LIN-specific interface
	/// @{

    virtual LINSleepMode OLI_CALL GetMode() const OLI_NOTHROW = 0;

	/// @}
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINSLEEPEVENT__INCLUDED__)
