/**
*
* \file
*
* \brief This file is intended to be compiled by a client of the OLL. It defines the helper function CheckAndThrow().
*
* See \ref ErrorReporting for an explanation of how errors are reported from OLI methods.
*
* Copyright ETAS GmbH, Stuttgart.
*
* \version $Id: ErrorProxies.cpp 214417 2010-04-26 14:04:49Z olcritch $
*
******************************************************************************/

#include "ErrorProxies.h"
#include "ErrorInterfaces.h"

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/**
 * This helper function is intended to be called by an inline wrapper method defined in an OLI interface class. NOTE that
 * CheckAndThrow() must be compiled by the client.
 *
 * When an OLI method fails, it returns a pointer to an object which implements an interface derived from IError. The OLI method
 * is wrapped by an inline wrapper method, defined in the same interface, which passes the IError pointer to CheckAndThrow().
 * CheckAndThrow() will discover the derived type of the error interface, then construct an appropriately-typed proxy for the
 * derived error interface. For example, if the IError interface's derived type is actually IInvalidStateError, CheckAndThrow()
 * constructs an instance of CInvalidStateError as a proxy for IInvalidStateError.
 *
 * CheckAndThrow throws the proxy object which it constructs. The proxy class, or one of its base classes, can be caught by the client.
 * See the examples in \ref ErrorReporting for more details.
 *
 * \param[in] pError    A pointer to an error object returned from an OLI method.
 */
void CheckAndThrow( IError* pError )
{
    if( !pError )
    {
        // There is no error.
        return;
    }

    // Discover the type of the error and throw an appropriately-typed proxy.

    // First check for the most-derived error types which we know about.
    if( pError->IsOfType( INotSupportedError::TYPE ) )      throw CNotSupportedError    ( (INotSupportedError*)( pError ) );
    if( pError->IsOfType( IVersionError::TYPE ) )           throw CVersionError         ( (IVersionError*)( pError ) );
    if( pError->IsOfType( IInvalidStateError::TYPE ) )      throw CInvalidStateError    ( (IInvalidStateError*)( pError ) );
    if( pError->IsOfType( IControlFlowError::TYPE ) )       throw CControlFlowError     ( (IControlFlowError*)( pError ) );
    if( pError->IsOfType( IInvalidOperationError::TYPE ) )  throw CInvalidOperationError( (IInvalidOperationError*)( pError ) );
    if( pError->IsOfType( IOutOfRangeError::TYPE ) )        throw COutOfRangeError      ( (IOutOfRangeError*)( pError ) );
    if( pError->IsOfType( IAllocError::TYPE ) )             throw CAllocError           ( (IAllocError*)( pError ) );
    if( pError->IsOfType( IUnexpectedError::TYPE ) )        throw CUnexpectedError      ( (IUnexpectedError*)( pError ) );
    if( pError->IsOfType( IOCIError::TYPE ) )               throw COCIError             ( (IOCIError*)( pError ) );
    if( pError->IsOfType( IOverflowError::TYPE ) )          throw COverflowError        ( (IOverflowError*)( pError ) );
    if( pError->IsOfType( IInvalidArgumentError::TYPE ) )   throw CInvalidArgumentError ( (IInvalidArgumentError*)( pError ) );
    if( pError->IsOfType( IDomainError::TYPE ) )            throw CDomainError          ( (IDomainError*)( pError ) );

    // If we reach this point, the error is of a derived type of which we are ignorant. Therefore we now check to see
    // if the error is of a super-type which we know about.
    //
    // Note that although the "real" type of the error can never be a super-type, we can construct an instance of a
    // super-type proxy to represent the error.

    if( pError->IsOfType( IOLIError::TYPE ) )               throw COLIError             ( (IOLIError*)( pError ) );

    // If we reach this point, the error is not of any recognised derived type, nor any recognised super-type. Therefore we
    // use the base-type proxy to represent the error.
    throw CError( (IError*)( pError ) );
}

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"
