///////////////////////////////////////////////////////////
//  IRxFrame.h
//  Definition of the IRxFrame interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ITXFRAME_INCLUDED__)
#define __OLI_ITXFRAME_INCLUDED__

// include base interface

#include "ITxMessage.h"

#include "BeginNamespace.h"

/// @ingroup GROUP_OLI_COMMON_MESSAGES

/** Common base interface for all frame message objects.
* It extends the generic @ref IMessage (header) interface
* by a generic notion of a limited-capacity buffer containing
* arbitrary payload.
*/

OLI_INTERFACE ITxFrame : public ITxMessage
{
protected:

    /// Instances implementing this interface are owned
    /// by the OLL and must not be destructed by the client.

    virtual ~ITxFrame() OLI_NOTHROW {};

public:

    /// actual payload size in bytes.

	virtual uint32 OLI_CALL GetSize() const OLI_NOTHROW = 0;

    /// Points to the first payload byte. Only @ref GetSize bytes
    /// are guaranteed to be valid and accessible.
    ///
    /// Return value must not be NULL.

    virtual const byte* OLI_CALL GetData() const OLI_NOTHROW = 0;
};

#include "EndNamespace.h"

#endif // !defined(__OLI_ITXFRAME_INCLUDED__)
