///////////////////////////////////////////////////////////
//  ILINRxFrame.h
//  Defines the ILINRxFrame interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINRXFRAME_INCLUDED__)
#define __OLI_ILINRXFRAME_INCLUDED__

// include used interface and constants

#include "LINBase.h"
#include "../Common/IRxFrame.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_MESSAGES

/// @{

/** The only frame type available on LIN.
* Interfacewise, it adds flags for extended ids and remote frames.
*/

OLI_INTERFACE ILINRxFrame : public IRxFrame
{
protected:

    /// Instances of this interface are owned by the OLL.

    virtual ~ILINRxFrame() OLI_NOTHROW {};

public:

    /// returned by @ref GetType

    enum {TYPE = LIN_TYPE_FRAME_BASE + 1};

    /// \name LIN-specific interface
	/// @{

    virtual LINCRCType OLI_CALL GetCRCType() const OLI_NOTHROW = 0;

	/// @}
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINRXFRAME_INCLUDED__)
