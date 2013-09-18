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
* @brief  This file declares all the error interfaces which 
*         can be returned from OLI methods
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ERRORINTERFACES__INCLUDED__)
#define __OLI_ERRORINTERFACES__INCLUDED__

/**
 * @page ErrorReporting Error Reporting
 *
 * The strategy used to report errors from the OLL to its clients aims 
 * to meet these requirements:
 *  - Errors must be reported in a way which does not break binary 
 *    compatibility (see @ref BinaryCompatibility "there" ).
 *  - A client of the OLL must be able to use exception syntax to handle 
 *    errors reported by OLI methods.
 *  - There must be minimal impact on performance unless an error is 
 *    actually reported.
 *  - It must be possible for the OLL to report additional information 
 *    beyond a description of the type of error which has occurred.
 *
 * Internally, the OLL uses exceptions to handle errors. In order to satisfy 
 * the above requirements, the methods of the OLI report errors as follows:
 *
 *  - Each exception which is raised within the OLL is caught before it 
 *    leaves the OLL and is used to construct an error object of a 
 *    corresponding type, such as @c COCIErrorImpl, @c CNotSupportedErrorImpl, 
 *    and do on. Each error object implements an interface derived from 
 *    @c IError, such as @c IOCIError, @c INotSupportedError, etc. 
 *    Each error object is constructed on the heap.
 *
 *  - When an OLI method needs to report an error, it returns a pointer to 
 *    @c IError, as implemented by the object which was constructed on the 
 *    heap to represent the error.
 *
 *  - Each OLI method which can report an error is wrapped by an inline 
 *    non-virtual wrapper method, defined in the same interface as the 
 *    method which it wraps. When the wrapped method returns a pointer 
 *    to @c IError, the wrapper method passes the pointer to the helper 
 *    function @ref CheckAndThrow(). This helper function must be compiled
 *    as part of the client. The inline wrapper method will also be 
 *    automatically compiled as part of the client: see 
 *    @ref BinaryCompatibility "binary compatibility".
 *
 *  - When a pointer to @c IError is passed to @ref CheckAndThrow(), 
 *    the function discovers the derived type of the error interface, 
 *    then constructs an appropriately-typed proxy for the derived error 
 *    interface. For example, if the @c IError interface's derived type 
 *    is actually @c IInvalidStateError, CheckAndThrow() constructs an 
 *    instance of @c CInvalidStateError as a proxy for @c IInvalidStateError.
 *
 *  - The proxy which is constructed by @ref CheckAndThrow() delegates 
 *    to the original error interface via the "pimpl" design pattern. 
 *    The proxy's destructor destroys the original error object via 
 *    @c IError::Destroy(). An instance of a proxy class cannot be copied 
 *    or assigned to.
 *
 *  - Once CheckAndThrow() has constructed an appropriately-typed proxy, 
 *    it throws it. This allows the client to handle OLI errors by catching 
 *    (by reference) instances of the proxies constructed by CheckAndThrow().
 *
 * The following example may help to make this process clearer. It shows 
 * how the @c GetStatus() method would be implemented on the interface 
 * @c ISomething:
 * @code
 *
 *  // OLI method definition, within Something.cpp in the OLL library.
 *  IError* CSomething::GetStatus( int* pStatus )
 *  {
 *      try
 *      {
 *          // Calculate status
 *          ...
 *          *pStatus = ...;
 *      }
 *      catch( OCIException& ociException )
 *      {
 *          // The OCI reported an error.
 *          return new COCIErrorImpl( ociException );
 *      }
 *      catch( InvalidStateException& invalidStateException )
 *      {
 *          // It is not valid to call this method at the moment.
 *          return new CInvalidStateErrorImpl( invalidStateException );
 *      }
 *      // Other catch() clauses as needed
 *      ...
 *      catch(...)
 *      {
 *          // Something unexpected went wrong.
 *          return new CUnexpectedErrorImpl();
 *      }
 *
 *      // Success
 *      return NULL;
 *  }
 *
 *  // Interface declaration, within public OLI file ISomething.h
 *  ISomething
 *  {
 *  protected:
 *      // This is the "real" API method.
 *      virtual IError* GetStatus( int* pStatus ) = 0;
 *  
 *  public:
 *      // This wrapper will be compiled inline by the client. 
 *      // It fulfils two purposes:
 *      //  - It uses CheckAndThrow() to convert any error into an exception.
 *      //  - It converts the method's signature so that client can access the
 *      //    result via the return value, instead of via an output parameter.
 *      int GetStatus()
 *      {
 *          int status = 0;
 *          CheckAndThrow( GetStatus( &status ) );
 *          return status;
 *      };
 *  };
 *  
 *  // Usage, within client file MyOLIClient.cpp
 *  {
 *      try
 *      {
 *          AutoPtr< ISomething > pSomething = ...;
 *
 *          int status = pSomething->GetStatus();
 *      }
 *      catch( COCIError& ociError )    // Note we must catch by reference: COCIError cannot be copied.
 *      {
 *          // We have caught a derived error proxy, representing a specific 
 *          // error. We can access the properties of ociError to discover 
 *          // more details.
 *          printf( "The OLL encountered an OCI error with code 0x%08x and message \"%s\"\n",
 *                  ociError.GetCode(), ociError.GetMessage() );
 *      }
 *      catch( CError& error )          // Note we must catch by reference: CError cannot be copied.
 *      {
 *          // We have caught a base error proxy, which we must can handle 
 *          // in a type-independent way.
 *          printf( "The OLL has encountered an error of type 0x%08x with message \"%s\"\n",
 *                  error.GetMostDerivedType(), error.GetMessage() );
 *      }
 *  }
 * @endcode
 *
 * Note that in this example, the public and protected implementations 
 * of @c GetStatus() have different argument lists and therefore can have 
 * the same name. In some cases, both the public wrapper and the protected 
 * API method have the same argument list; when this occurs, the protected 
 * API method is given the suffix "...Raw".
 *
 * There is one problem with the solution described above: what happens 
 * if the OLL must call an OLI method internally? In the language of the 
 * example above, what happens if the OLL must call @c ISomething::GetStatus 
 * internally?
 *
 * In such a situation, the caller (i.e. the OLL) wishes to allow an 
 * exception which is thrown within the OLI method to propagate to the 
 * caller. Such an exception will eventually be caught elsewhere in the 
 * OLL and returned to the OLI client as an error object, as described above.
 *
 * The solution is to define an OLL-internal "throwing" variant of the 
 * method in question. In the above example, this method would be called 
 * @c CSomething::GetStatusThrowing(). @c GetStatus() would delegate to 
 * @c GetStatusThrowing(), which would perform the work of the method and, 
 * perhaps, throw an exception. If the OLL needed to call @c GetStatus() 
 * internally, it would call @c GetStatusThrowing().
 */

#include "OLIDefs.h"
#include "OLITypes.h"
#include "CommonBase.h"
#include "BeginNamespace.h"

// The Windows SDK defines GetMessage as a preprocessor macro, so we must 
// undefine it before we can use it as a function name.
#ifdef GetMessage 
#undef GetMessage
#endif

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Base class for all error interfaces.
 *
 * This is the root in the inheritance hierarchy of error interfaces. 
 * Therefore it encapsulates methods which are common to all types of error.
 *
 * Since the C++ standard does not specify an ABI for run-time type 
 * information (RTTI), the @ref IsOfType method has been added as a
 * substitute. 
 *
 * Each OLI method which is able to report an error returns a pointer to an 
 * object which implements this interface. In fact, the object will also 
 * implement one of the error-specific interfaces derived from IError.
 *
 * To provide a natural C++ interface, the error objects returned by
 * the respective methods will be wrapped into @ref CError instances
 * and thrown as an exception. These wrappers also automatically manage 
 * the lifetime of the incapsulated IError object. Otherwise, you would
 * need to call @ref Destroy explicitly.
 *
 * @remark Prefer using wrapper classes derived from @ref CError instead
 *         of managing IError instances manually.
 * @remark All public methods are thread-safe.
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", CError, CheckAndThrow
 */
OLI_INTERFACE IError
{
protected:

    /** @brief Destructor.

        This destructor should never be called by a client of this object. 
        Instead use the method @ref Destroy().

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~IError( void ) {};

public:
    /** This is a unique identifier for the type of this interface. */
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 1 };

    /** @brief  Returns a human-readable string describing the error.
     
        @return A human-readable string describing the error.
                The returned pointer remains valid only for the lifetime
                of this @ref IError instance.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual const char* GetMessage() OLI_NOTHROW = 0;

    /** @brief  Check whether the object implements the specified interface.

                This method determines whether the object implementing this 
                interface is of a specified type, or derives from that type.
                This method is required to allow a client of the OLI to 
                perform RTTI on the IError interface which is returned from 
                an OLI method. (Recall that C++ RTTI is not binary compatible
                across different compilers).
     
        @param[in] typeId   The @c TYPE value from IError or one of its 
                            derived interfaces.
        @return
        - true  The object implementing this interface is of the 
                specified type, or derives from that type. Use a
                @c static_cast<> to access that type.
        - false Otherwise.
        @exception <none> This function must not throw exceptions.

        @remark Use @c static_cast<> to for down-casting OLI interfaces.
        @since  BOA 1.3 
        @see    GetMostDerivedType
     */
    virtual bool IsOfType( uint32 typeId ) OLI_NOTHROW = 0;

    /** @brief  Returns the interface type actually implemented.

                This method returns an identifier for the most-derived 
                error interface implemented by this object. This is 
                equivalent to the most-derived type of the error object 
                itself.

        @return The @c TYPE value of the most-derived error interface 
                implemented by this object.
        @exception <none> This function must not throw exceptions.

        @remark Use @c static_cast<> to for down-casting OLI interfaces.
        @since  BOA 1.3 
        @see    IsOfType
     */
    virtual uint32 GetMostDerivedType() OLI_NOTHROW = 0;

    /** @brief  Destroy the object.

                This method must be used instead of @c operator @c delete 
                to destroy an object which implements this interface.

        @exception <none> This function must not throw exceptions.

        @remark The wrapper classes derived from @ref CError wrapper 
                will call this method automatically.
        @since  BOA 1.3
        @see    CError, CheckAndThrow
     */
    virtual void Destroy() OLI_NOTHROW = 0;
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Represents an unexpected error.
 *
 * This interface is implemented by an object which represents an unexpected 
 * error. The OLL is not able to classify unexpected errors.
 *
 * @remark Prefer using the @ref CUnexpectedError wrapper class instead
 *         of managing IUnexpectedError instances manually.
 * @remark All public methods are thread-safe.
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         CUnexpectedError, CheckAndThrow
 */
OLI_INTERFACE IUnexpectedError : public IError
{
public:
    /** This is a unique identifier for the type of this interface. */
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 2 };
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Representation of an OCI-level error.
 *
 * This interface is implemented by an object which represents an OCI error 
 * which is being relayed by the OLL.
 *
 * @remark Prefer using the @ref COCIError wrapper class instead
 *         of managing IOCIError instances manually.
 * @remark All public methods are thread-safe.
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         COCIError, CheckAndThrow
 */
OLI_INTERFACE IOCIError : public IError
{
public:
    /** This is a unique identifier for the type of this interface. */
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 3 };

    /** @brief  Obtains the OCI error code which identifies the type 
                of the OCI error.

        @return The OCI error code which identifies the type of the 
                OCI error.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual OCI_ErrorCode GetCode( void ) = 0;
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Base class for all OLI-specific error interfaces.
 *
 * @remark Prefer using the @ref COLIError wrapper class instead
 *         of managing IOLIError instances manually.
 * @remark All public methods are thread-safe.
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         COLIError, CheckAndThrow
 */
OLI_INTERFACE IOLIError : public IError
{
public:
    /** This is a unique identifier for the type of this interface. */
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 4 };
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Returned for unsuppored operations.
 *
 * This interface is implemented by an object which represents an 
 * "unsupported" error. Such an error will be raised if the client
 * attempts to access unsupported functionality within the OLL.
 *
 * @remark Prefer using the @ref CNotSupportedError wrapper class instead
 *         of managing INotSupportedError instances manually.
 * @remark All public methods are thread-safe.
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         CNotSupportedError, CheckAndThrow
 */
OLI_INTERFACE INotSupportedError : public IOLIError
{
public:
    /** This is a unique identifier for the type of this interface. */
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 5 };
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Represents a version incompatibility error.
 *
 * This interface is implemented by an object which represents a 
 * "version" error. Such an error will be raised if the client tries to
 * create an unsupported version of a CAN, LIN or FlexRay link.
 *
 * @remark Prefer using the @ref CVersionError wrapper class instead
 *         of managing IVersionError instances manually.
 * @remark All public methods are thread-safe.
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         CVersionError, CheckAndThrow
 */
OLI_INTERFACE IVersionError : public IOLIError
{
public:
    /** This is a unique identifier for the type of this interface. */
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 6 };
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Represents an "invalid state" error.
 *
 * This interface is implemented by an object which represents an 
 * "invalid state" error. Such an error will be raised if the OLL
 * enters an invalid internal state.
 *
 * @remark Prefer using the @ref CInvalidStateError wrapper class instead
 *         of managing IInvalidStateError instances manually.
 * @remark All public methods are thread-safe.
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         CInvalidStateError, CheckAndThrow
 */
OLI_INTERFACE IInvalidStateError : public IOLIError
{
public:
    /** This is a unique identifier for the type of this interface. */
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 7 };
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Represents an "control flow" error.
 *
 * This interface is implemented by an object which represents a 
 * "control flow" error. Such an error will be raised if the control
 * flow within the OLL reaches a point which should be unreachable.
 *
 * @remark Prefer using the @ref CControlFlowError wrapper class instead
 *         of managing IControlFlowError instances manually.
 * @remark All public methods are thread-safe.
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         CControlFlowError, CheckAndThrow
 */
OLI_INTERFACE IControlFlowError : public IOLIError
{
public:
    /** This is a unique identifier for the type of this interface. */
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 8 };
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Represents an "invalid operation" error.
 *
 * This interface is implemented by an object which represents an 
 * "invalid operation" error. Such an error will be raised if the
 * client asks the OLL to perform an invalid operation, such as trying 
 * to modify a read-only value.
 *
 * @remark Prefer using the @ref CInvalidOperationError wrapper class instead
 *         of managing IInvalidOperationError instances manually.
 * @remark All public methods are thread-safe.
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         CInvalidOperationError, CheckAndThrow
 */
OLI_INTERFACE IInvalidOperationError : public IOLIError
{
public:
    /** This is a unique identifier for the type of this interface. */
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 9 };
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Represents an "out of range" error.
 *
 * This interface is implemented by an object which represents an 
 * "out of range" error. Such an error will be raised if the client 
 * supplies an index or other parameter which has a value outside 
 * the permitted range.
 *
 * @remark Prefer using the @ref COutOfRangeError wrapper class instead
 *         of managing IOutOfRangeError instances manually.
 * @remark All public methods are thread-safe.
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         COutOfRangeError, CheckAndThrow
 */
OLI_INTERFACE IOutOfRangeError : public IOLIError
{
public:
    /** This is a unique identifier for the type of this interface. */
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 10 };
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Represents a memory allocation error.
 *
 * This interface is implemented by an object which represents an 
 * "allocation" error. Such an error will be raised if the
 * OLL finds itself unable to allocate heap memory when required.
 *
 * @remark Prefer using the @ref CAllocError wrapper class instead
 *         of managing IAllocError instances manually.
 * @remark All public methods are thread-safe.
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         CAllocError, CheckAndThrow
 */
OLI_INTERFACE IAllocError : public IOLIError
{
public:
    /** This is a unique identifier for the type of this interface. */
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 11 };
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Represents an arithmetic overflow error.
 *
 * This interface is implemented by an object which represents an 
 * arithmetic overflow error. Such an error will be raised if the
 * OLL performs a checked arithmetic operation which overflows.
 *
 * @remark Prefer using the @ref COverflowError wrapper class instead
 *         of managing IOverflowError instances manually.
 * @remark All public methods are thread-safe.
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         COverflowError, CheckAndThrow
 */
OLI_INTERFACE IOverflowError : public IOLIError
{
public:
    /** This is a unique identifier for the type of this interface. */
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 12 };
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Returned on invalid arguments.
 *
 * This interface is implemented by an object which represents an 
 * "invalid argument" error. Such an error will be raised if an
 * OLI method is called with an invalid argument.
 *
 * @remark Prefer using the @ref CInvalidArgumentError wrapper class instead
 *         of managing IInvalidArgumentError instances manually.
 * @remark All public methods are thread-safe.
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         CInvalidArgumentError, CheckAndThrow
 */
OLI_INTERFACE IInvalidArgumentError : public IOLIError
{
public:
    /** This is a unique identifier for the type of this interface. */
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 13 };
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Represents an "domain" error.
 *
 * This interface is implemented by an object which represents a "domain" 
 * error, i.e. a @c std::domain_error. The OLL uses this as a general-purpose
 * error type.
 *
 * @remark Prefer using the @ref CDomainError wrapper class instead
 *         of managing IDomainError instances manually.
 * @remark All public methods are thread-safe.
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         CDomainError, CheckAndThrow
 */
OLI_INTERFACE IDomainError : public IOLIError
{
public:
    /** This is a unique identifier for the type of this interface. */
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 14 };
};

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_ERRORINTERFACES__INCLUDED__)
