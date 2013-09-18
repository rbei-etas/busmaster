/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4412 $
*/

/** 
* @file
* @brief  Implementation of the @ref CheckAndThrow function.
* 
*         By default, this code will be included into the @ref ErrorProxies.h
*         header file and the function will be @c inline.
*
*         Define @ref OLI_DONT_INLINE_CHECKANDTHROW and compile this source
*         source file as part of your client application, if you don't want
*         @a CheckAndThrow to be inline.
*
* @remark The header structure of the OLI may change in future releases. 
*         Don't include this file directly. Use @ref OLI.h instead.
*
* @remark You may compile and link this file as part of your client 
*         application even if @ref OLI_DONT_INLINE_CHECKANDTHROW has not
*         been defined. The function will still be inlined in that case
*         and this file will contribute no further object code. Thus, you
*         may get a linker warning.
*
* @see    @ref ErrorReporting "error reporting", IError, CError, CheckAndThrow
*/

/** @ingroup GROUP_OLI_COMMON_ERROR
 * @def   OLI_DONT_INLINE_CHECKANDTHROW
 * @brief Prevent @ref CheckAndThrow from being inlined.
 *
 * Define this macro before you include OLI.h in your application.
 * Then, you also need to compile and link this file (ErrorProxies.CPP)
 * as part of your client application.
 */

/* Detect whether this CPP file is being compiled directly or if it has 
 * been included into ErrorProxies.h
 */
#if defined(__OLI_ERRORPROXIES__INCLUDED__)
#define __IN_HEADER__
#define AUTO_INLINE inline
#else
#define __IN_SOURCE__
#define AUTO_INLINE 
#endif

/* Include other headers and the namespace only if we are in a source file, 
 * i.e. we didn't #include this file as for inlining
 */
#if defined(__IN_SOURCE__)
#include "stddef.h"
#include "ErrorProxies.h"
#include "ErrorInterfaces.h"

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif
#endif 

/* Implement CheckAndThrow().
 * Compile the code only if
 * - we are inlining and this has been included into a header
 * - we are not inlining and this is an ordinary source file
 */
#if defined(OLI_DONT_INLINE_CHECKANDTHROW) == defined(__IN_SOURCE__)
AUTO_INLINE void CheckAndThrow( IError* pError )
{
    if( !pError )
    {
        /* There is no error. */
        return;
    }

    /* Discover the type of the error and throw an appropriately-typed proxy.
     *
     * First check for the most-derived error types which we know about. 
     */
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

    /* If we reach this point, the error is of a derived type of which we 
     * are ignorant. Therefore we now check to see if the error is of a 
     * super-type which we know about.
     *
     * Note that although the "real" type of the error can never be a 
     * super-type, we can construct an instance of a super-type proxy to 
     * represent the error.
     */
    if( pError->IsOfType( IOLIError::TYPE ) )               throw COLIError             ( (IOLIError*)( pError ) );

    /* If we reach this point, the error is not of any recognised derived 
     * type, nor any recognised super-type. Therefore we use the base-type 
     * proxy to represent the error.
     */
    throw CError( (IError*)( pError ) );
}
#endif

/* Close the namespace only if we are in a source file, 
 * i.e. we didn't #include this file as for inlining
 */
#if defined(__IN_SOURCE__)
#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"
#endif

/* Preprocessor cleanup.
 */
#if defined(__IN_SOURCE__)
#undef __IN_SOURCE__
#else
#undef __IN_HEADER__
#endif
#undef AUTO_INLINE 
