///////////////////////////////////////////////////////////
//  ILINCRCDetectEvent.h
//  Defines the ILINCRCDetectEvent interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINCRCDETECTEVENT__INCLUDED__)
#define __OLI_ILINCRCDETECTEVENT__INCLUDED__

// include used interface and constants

#include "LINBase.h"
#include "../Common/IEvent.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_MESSAGES

/// @{

/** Represents the OCI_LINCRCDetectEventMessage event.
*/

OLI_INTERFACE ILINCRCDetectEvent : public IEvent
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~ILINCRCDetectEvent() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = LIN_TYPE_EVENT_BASE + 7};

    /// \name LIN-specific interface
	/// @{

    virtual LINCRCType OLI_CALL GetCRCType() const OLI_NOTHROW = 0;

	/// @}
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINCRCDETECTEVENT__INCLUDED__)
