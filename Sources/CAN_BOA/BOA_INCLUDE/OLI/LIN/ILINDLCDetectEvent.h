///////////////////////////////////////////////////////////
//  ILINDLCDetectEvent.h
//  Defines the ILINDLCDetectEvent interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINDLCDETECTEVENT__INCLUDED__)
#define __OLI_ILINDLCDETECTEVENT__INCLUDED__

// include used interface and constants

#include "LINBase.h"
#include "../Common/IEvent.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_MESSAGES

/// @{

/** Represents the OCI_LINDLCDetectEventMessage event.
*/

OLI_INTERFACE ILINDLCDetectEvent : public IEvent
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~ILINDLCDetectEvent() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = LIN_TYPE_EVENT_BASE + 6};

    /// \name LIN-specific interface
	/// @{

    virtual uint8 OLI_CALL GetDLC() const OLI_NOTHROW = 0;

	/// @}
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINDLCDETECTEVENT__INCLUDED__)
