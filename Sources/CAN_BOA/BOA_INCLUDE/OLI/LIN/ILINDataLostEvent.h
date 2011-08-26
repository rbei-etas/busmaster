///////////////////////////////////////////////////////////
//  ILINDataLostEvent.h
//  Defines the ILINDataLostEvent interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINDATALOSTEVENT__INCLUDED__)
#define __OLI_ILINDATALOSTEVENT__INCLUDED__

// include used interface and constants

#include "LINBase.h"
#include "../Common/IEvent.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_MESSAGES

/// @{

/** Represents the OCI_LINWakeUpEventMessage event.
*/

OLI_INTERFACE ILINDataLostEvent : public IEvent
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~ILINDataLostEvent() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = LIN_TYPE_EVENT_BASE + 1};

    /// \name LIN-specific interface
	/// @{

    virtual uint32 OLI_CALL GetCount() const OLI_NOTHROW = 0;

	/// @}
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINDATALOSTEVENT__INCLUDED__)
