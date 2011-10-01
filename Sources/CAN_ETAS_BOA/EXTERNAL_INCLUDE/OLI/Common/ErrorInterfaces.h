/**
*
* \file
*
* \brief This file declares all the error interfaces which can be returned from OLI methods.
*
* See \ref ErrorReporting for an explanation of how errors are reported from OLI methods.
*
* Copyright ETAS GmbH, Stuttgart.
*
* \version $Id: ErrorInterfaces.h 216939 2010-05-05 07:43:19Z olcritch $
*
******************************************************************************/

#if !defined(__OLI_ERRORINTERFACES__INCLUDED__)
#define __OLI_ERRORINTERFACES__INCLUDED__

/**
 * \page ErrorReporting Error Reporting
 *
 * The strategy used to report errors from the OLL to its clients aims to meet these requirements:
 *  - Errors must be reported in a way which does not break binary compatibility (see \ref BinaryCompatibility).
 *  - A client of the OLL must be able to use exception syntax to handle errors reported by OLI methods.
 *  - There must be minimal impact on performance unless an error is actually reported.
 *  - It must be possible for the OLL to report additional information beyond a description of the type of error which
 *    has occurred.
 *
 * Internally, the OLL uses exceptions to handle errors. In order to satisfy the above requirements, the methods of the
 * OLI report errors as follows:
 *
 *  - Each exception which is raised within the OLL is caught before it leaves the OLL and is used to
 *    construct an error object of a corresponding type, such as \c COCIErrorImpl, \c CNotSupportedErrorImpl, etc. Each error
 *    object implements an interface derived from \c IError, such as \c IOCIError, \c INotSupportedError, etc. Each error object
 *    is constructed on the heap.
 *
 *  - When an OLI method needs to report an error, it returns a pointer to \c IError, as implemented by the object which was
 *    constructed on the heap to represent the error.
 *
 *  - Each OLI method which can report an error is wrapped by an inline non-virtual wrapper method, defined in the same interface
 *    as the method which it wraps. When the wrapped method returns a pointer to \c IError, the wrapper method passes the pointer
 *    to the helper function CheckAndThrow(). This helper function must be compiled as part of the client. The inline wrapper
 *    method will also be automatically compiled as part of the client: see \ref BinaryCompatibility.
 *
 *  - When a pointer to \c IError is passed to CheckAndThrow(), the function discovers the derived type of the error interface,
 *    then constructs an appropriately-typed proxy for the derived error interface. For example, if the \c IError interface's
 *    derived type is actually \c IInvalidStateError, CheckAndThrow() constructs an instance of \c CInvalidStateError as a proxy
 *    for \c IInvalidStateError.
 *
 *  - The proxy which is constructed by CheckAndThrow() delegates to the original error interface via the "pimpl" design
 *    pattern. The proxy's destructor destroys the original error object via \c IError::Destroy(). An instance of a proxy class
 *    cannot be copied or assigned to.
 *
 *  - Once CheckAndThrow() has constructed an appropriately-typed proxy, it throws it. This allows the client to handle OLI
 *    errors by catching (by reference) instances of the proxies constructed by CheckAndThrow().
 *
 * The following example may help to make this process clearer. It shows how the \c GetStatus() method would be implemented on
 * the interface \c ISomething:
 * \code
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
 *      // This wrapper will be compiled inline by the client. It fulfils two purposes:
 *      //  - It uses CheckAndThrow() to convert any error into an exception.
 *      //  - It converts the method's signature so that client can access the result via the return
 *      //    value, instead of via an output parameter.
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

 *          int status = pSomething->GetStatus();
 *      }
 *      catch( COCIError& ociError )    // Note we must catch by reference: COCIError cannot be copied.
 *      {
 *          // We have caught a derived error proxy, representing a specific error. We can access
 *          // the properties of ociError to discover more details.
 *          printf( "The OLL encountered an OCI error with code 0x%08x and message \"%s\"\n",
 *                  ociError.GetCode(), ociError.GetMessage() );
 *      }
 *      catch( CError& error )          // Note we must catch by reference: CError cannot be copied.
 *      {
 *          // We have caught a base error proxy, which we must can handle in a type-independent way.
 *          printf( "The OLL has encountered an error of type 0x%08x with message \"%s\"\n",
 *                  error.GetMostDerivedType(), error.GetMessage() );
 *      }
 *  }
 * \endcode
 *
 * Note that in this example, the public and protected implementations of \c GetStatus() have different argument lists and
 * therefore can have the same name. In some cases, both the public wrapper and the protected API method have the same
 * argument list; when this occurs, the protected API method is given the suffix "...Raw".
 *
 * There is one problem with the solution described above: what happens if the OLL must call an OLI method internally? In
 * the language of the example above, what happens if the OLL must call \c ISomething::GetStatus() internally?
 *
 * In such a situation, the caller (i.e. the OLL) wishes to allow an exception which is thrown within the OLI method to
 * propagate to the caller. Such an exception will eventually be caught elsewhere in the OLL and returned to the OLI client
 * as an error object, as described above.
 *
 * The solution is to define an OLL-internal "throwing" variant of the method in question. In the above example, this method
 * would be called \c CSomething::GetStatusThrowing(). \c GetStatus() would delegate to \c GetStatusThrowing(), which
 * would perform the work of the method and, perhaps, throw an exception. If the OLL needed to call \c GetStatus() internally,
 * it would call \c GetStatusThrowing().
 */

#include "OLIDefs.h"
#include "OLITypes.h"
#include "CommonBase.h"
#include "BeginNamespace.h"

// The Windows SDK defines GetMessage as a preprocessor macro, so we must undefine it before we can use it as a function name.
#ifdef GetMessage 
#undef GetMessage
#endif

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/**
 * This is the root in the inheritance hierarchy of error interfaces. Therefore it encapsulates methods which are common to
 * all types of error.
 *
 * Each OLI method which is able to report an error returns a pointer to an object which implements this interface. In fact,
 * the object will also implement one of the error-specific interfaces derived from IError.
 *
 * See \ref ErrorReporting for more information about how this interface is used.
 */
OLI_INTERFACE IError
{
public:
    /// This is a unique identifier for the type of this interface.
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 1 };

    /**
     * This method returns a human-readable string describing the error.
     *
     * \return A human-readable string describing the error.
     */
    virtual const char* GetMessage( void ) = 0;

    /**
     * This method determines whether the object implementing this interface is of a specified type, or derives from that type.
     * This method is required to allow a client of the OLI to perform RTTI on the IError interface which is returned from an
     * OLI method. (Recall that C++ RTTI is not binary compatible).
     *
     * \param[in] typeId    The TYPE value from IError or one of its derived interfaces.
     *
     * \return
     *  - true      The object implementing this interface is of the specified type, or derives from that type.
     *  - false     Otherwise.
     */
    virtual bool IsOfType( uint32 typeId ) = 0;

    /**
     * This method returns an identifier for the most-derived error interface implemented by this object. This is equivalent to
     * the most-derived type of the error object itself.
     *
     * \return  The TYPE value of the most-derived error interface implemented by this object.
     */
    virtual uint32 GetMostDerivedType( void ) = 0;

    /**
     * This method should be used instead of \c operator \c delete to destroy an object which implements this interface.
     */
    virtual void Destroy( void ) = 0;

protected:
    /**
     * This destructor should never be called by a client of this object. Instead use the method Destroy().
     */
    virtual ~IError( void ) {};
};

/**
 * This interface is implemented by an object which represents an unexpected error. The OLL is not able to classify unexpected errors.
 */
OLI_INTERFACE IUnexpectedError : public IError
{
public:
    /// This is a unique identifier for the type of this interface.
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 2 };
};

/**
 * This interface is implemented by an object which represents an OCI error which is being relayed by the OLL.
 */
OLI_INTERFACE IOCIError : public IError
{
public:
    /// This is a unique identifier for the type of this interface.
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 3 };

    /**
     * This method obtains the OCI error code which identifies the type of the OCI error.
     *
     * \return  The OCI error code which identifies the type of the OCI error.
     */
    virtual OCI_ErrorCode GetCode( void ) = 0;
};

/**
 * This interface acts as a base for any interface describing an OLI error.
 */
OLI_INTERFACE IOLIError : public IError
{
public:
    /// This is a unique identifier for the type of this interface.
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 4 };
};

/**
 * This interface is implemented by an object which represents an "unsupported" error. Such an error will be raised if the client
 * attempts to access unsupported functionality within the OLL.
 */
OLI_INTERFACE INotSupportedError : public IOLIError
{
public:
    /// This is a unique identifier for the type of this interface.
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 5 };
};

/**
 * This interface is implemented by an object which represents a "version" error. Such an error will be raised if the client tries to
 * create an unsupported version of a CAN, LIN or FlexRay link.
 */
OLI_INTERFACE IVersionError : public IOLIError
{
public:
    /// This is a unique identifier for the type of this interface.
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 6 };
};

/**
 * This interface is implemented by an object which represents an "invalid state" error. Such an error will be raised if the OLL
 * enters an invalid internal state.
 */
OLI_INTERFACE IInvalidStateError : public IOLIError
{
public:
    /// This is a unique identifier for the type of this interface.
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 7 };
};

/**
 * This interface is implemented by an object which represents a "control flow" error. Such an error will be raised if the control
 * flow within the OLL reaches a point which should be unreachable.
 */
OLI_INTERFACE IControlFlowError : public IOLIError
{
public:
    /// This is a unique identifier for the type of this interface.
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 8 };
};

/**
 * This interface is implemented by an object which represents an "invalid operation" error. Such an error will be raised if the
 * client asks the OLL to perform an invalid operation, such as trying to modify a read-only value.
 */
OLI_INTERFACE IInvalidOperationError : public IOLIError
{
public:
    /// This is a unique identifier for the type of this interface.
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 9 };
};

/**
 * This interface is implemented by an object which represents an "out of range" error. Such an error will be raised if the
 * client supplies an index or other parameter which has a value outside the permitted range.
 */
OLI_INTERFACE IOutOfRangeError : public IOLIError
{
public:
    /// This is a unique identifier for the type of this interface.
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 10 };
};

/**
 * This interface is implemented by an object which represents an "allocation" error. Such an error will be raised if the
 * OLL finds itself unable to allocate heap memory when required.
 */
OLI_INTERFACE IAllocError : public IOLIError
{
public:
    /// This is a unique identifier for the type of this interface.
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 11 };
};

/**
 * This interface is implemented by an object which represents an arithmetic overflow error. Such an error will be raised if the
 * OLL performs a checked arithmetic operation which overflows.
 */
OLI_INTERFACE IOverflowError : public IOLIError
{
public:
    /// This is a unique identifier for the type of this interface.
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 12 };
};

/**
 * This interface is implemented by an object which represents an "invalid argument" error. Such an error will be raised if an
 * OLI method is called with an invalid argument.
 */
OLI_INTERFACE IInvalidArgumentError : public IOLIError
{
public:
    /// This is a unique identifier for the type of this interface.
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 13 };
};

/**
 * This interface is implemented by an object which represents a "domain" error. The OLL uses this as a general-purpose error type.
 */
OLI_INTERFACE IDomainError : public IOLIError
{
public:
    /// This is a unique identifier for the type of this interface.
    enum { TYPE = COMMON_TYPE_ERROR_BASE + 14 };
};

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_ERRORINTERFACES__INCLUDED__)
