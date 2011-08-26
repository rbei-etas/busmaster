///////////////////////////////////////////////////////////
//  IFlexRayRxFrame.h
//  Definition of the IFlexRayRxFrame interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_IFLEXRAYRXFRAME_INCLUDED__)
#define __OLI_IFLEXRAYRXFRAME_INCLUDED__

// include used interface and constants

#include "FlexRayBase.h"
#include "../Common/IRxFrame.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_FLEXRAY_MESSAGES

/// @{

/** The only frame type available on FlexRay.
*/

OLI_INTERFACE IFlexRayRxFrame : public IRxFrame
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~IFlexRayRxFrame() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = FLEXRAY_TYPE_FRAME};

    /// \name FlexRay-specific r/o interface
	/// @{

    virtual FlexRayFrameType OLI_CALL GetFrameType() const OLI_NOTHROW = 0;
    virtual FlexRayChannel OLI_CALL GetChannel() const OLI_NOTHROW = 0;
    virtual uint16 OLI_CALL GetSlot() const OLI_NOTHROW = 0;
    virtual uint8 OLI_CALL GetCycleCount() const OLI_NOTHROW = 0;
        
    virtual uint16 OLI_CALL GetHeaderCRC() const OLI_NOTHROW = 0;
    
    virtual uint8 OLI_CALL GetPayloadLength() const OLI_NOTHROW = 0;

	/// @}
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_IFLEXRAYRXFRAME_INCLUDED__)
