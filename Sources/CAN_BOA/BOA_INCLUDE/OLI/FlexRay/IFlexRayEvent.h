///////////////////////////////////////////////////////////
//  IFlexRayEvent.h
//  Definition of the IFlexRayEvent interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_IFLEXRAYEVENT__INCLUDED__)
#define __OLI_IFLEXRAYEVENT__INCLUDED__

// include used interface and constants

#include "FlexRayBase.h"
#include "../Common/IEvent.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_FLEXRAY_MESSAGES

/// @{

/** The only event type available on FlexRay. */

OLI_INTERFACE IFlexRayEvent : public IEvent
{
protected:

    /// Instances of this interface are owned by the OLL. No public destructor is accessible.

    virtual ~IFlexRayEvent() OLI_NOTHROW {};

public:

   /// IFlexRayEvent::GetType() will return FLEXRAY_TYPE_EVENT_BASE + 1

   enum {TYPE = FLEXRAY_TYPE_EVENT_BASE + 1};

   /// IFlexRayEvent::GetID() will return a @ref FlexRayEventCode

   /// \name FlexRay-specific interface
	/// @{
   virtual FlexRayControllerProtocolOperationControlStatus OLI_CALL GetProtocolOperationControlStatus() const OLI_NOTHROW = 0;
   /// @}
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_IFLEXRAYEVENT__INCLUDED__)
