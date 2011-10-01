///////////////////////////////////////////////////////////
//  ILINMasterRequest.h
//  Defines the ILINMasterRequest interface
///////////////////////////////////////////////////////////

/// @todo update doxygen comments

#if !defined(__OLI_ILINMASTERREQUEST_INCLUDED__)
#define __OLI_ILINMASTERREQUEST_INCLUDED__

#include "LINBase.h"
#include "../Common/ITxMessage.h"

#include "../Common/BeginNamespace.h"

/// \addtogroup GROUP_OLI_LIN_MESSAGES

/// @{

/// Message type for LIN master tx queue
OLI_INTERFACE ILINMasterRequest : public ITxMessage
{
public:

    enum {TYPE = LIN_TYPE_FRAME_BASE + 2};
};

/// @}

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINMASTERREQUEST_INCLUDED__)
