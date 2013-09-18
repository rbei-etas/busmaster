/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4509 $
*/

/** 
* @file
* @brief  ILINMasterRequest definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

/// @todo update doxygen comments

#if !defined(__OLI_ILINMASTERREQUEST_INCLUDED__)
#define __OLI_ILINMASTERREQUEST_INCLUDED__

#include "LINBase.h"
#include "../Common/ITxMessage.h"

#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/// \addtogroup GROUP_OLI_LIN_MESSAGES

/// @{

class ILINMasterRequest;

/**
 * This function instantiates an object supporting ILINMasterRequest. See \ref BinaryCompatibility for an explanation of why it is needed.
 *
 * NOTE that clients are encouraged to access this function via the wrapper ILINMasterRequest::Create().
 *
 * \param[in]  frameID            The frame identifier.
 * \param[out] ppLinMasterRequest A pointer to an object supporting @ref ILINMasterRequest. The object 
 *                                is owned by the caller, i.e. the client application is expected
 *                                to eventually call the @ref ITxMessage::Destroy method.
 *                                This can be ensured by wrapping the object pointer in an instance 
 *                                of the @ref AutoDestroyPtr class.
 *
 * \return A pointer to an interface based on IError, describing the error which occurred during this function. NULL if no error
 * occurred. See \ref ErrorReporting for more information on how errors are reported.
 */
OLL_API IError* OLI_CALL ILINMasterRequest_Create( uint8 frameID, ILINMasterRequest** ppLinMasterRequest );

/// Message type for LIN master tx queue
OLI_INTERFACE ILINMasterRequest : public ITxMessage
{
protected:

    /** @brief Destructor.

        This destructor has been hidden since objects implementing 
        this class may need explicit destruction through @ref Destroy.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINMasterRequest() OLI_NOTHROW {};

public:

    enum {TYPE = LIN_TYPE_FRAME_BASE + 3};

    /**
     * This is a helper method which wraps ILINMasterRequest_Create(): see \ref BinaryCompatibility and \ref ErrorReporting for an
     * explanation of why it is needed.
     */
    static ILINMasterRequest* OLI_CALL 
    Create( uint8 frameID )
    {
        ILINMasterRequest* pLinMasterRequest = NULL;
        CheckAndThrow( ILINMasterRequest_Create( frameID, &pLinMasterRequest ) );

        return pLinMasterRequest;
    }

};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINMASTERREQUEST_INCLUDED__)
