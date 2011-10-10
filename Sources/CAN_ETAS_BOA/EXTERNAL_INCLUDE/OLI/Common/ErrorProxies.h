/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4806 $
*/

/** 
* @file
* @brief  Proxy classes for @ref IError objects.
* 
*         This file declares proxy classes, each of which wraps a 
*         heap-allocated error object created by the OLL to encapsulate 
*         information about an error which has occurred. Each proxy class 
*         is instantiated by the client and uses the "pimpl" design pattern 
*         to delegate to the wrapped error object. 
*
*         The existence of the proxy classes gives the client application 
*         of the OLL two advantages:
*           - The client can throw an instance of a proxy object, thereby 
*             using exception syntax to handle the associated error.
*           - Each proxy object automatically ensures that the corresponding 
*             heap-allocated error object is correctly deleted.
*
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*
* @see    @ref ErrorReporting "error reporting", IError, CError
*/

#if !defined(__OLI_ERRORPROXIES__INCLUDED__)
#define __OLI_ERRORPROXIES__INCLUDED__

#include "..\..\OCI\ocierror.h"     // COCIError allows the user to obtain OCI error codes, so we must include them here.

#include "ErrorInterfaces.h"

#include "BeginNamespace.h"

// The Windows SDK defines GetMessage as a preprocessor macro, 
// so we must undefine it before we can use it as a function name.
#ifdef GetMessage 
#undef GetMessage
#endif

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// suppress QA C++ warnings (they are invalid for non-public members)
// QACPP[3:2634] C++ Language Feature: 
//    The implicitly defined copy constructor would have the same definition.
// QACPP[3:2635] C++ Language Feature: 
//    The implicitly defined copy constructor would also shallow copy pointer members.
#if defined(PRL_001) || defined(PRL_002) || defined(PRL_005)
#pragma PRQA_MESSAGES_OFF 2634,2635
#endif

/** @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Base class of all OLI exception classes.
 *
 * The hierarchy of proxy classes matches the hierarchy of error interfaces,
 * which is based on @ref IError. This is done to allow the client to handle 
 * proxies (and thereby errors) in type-specific or type-independent ways as 
 * required. Each error proxy class wraps a corresponding interface derived 
 * from @ref IError.
 *
 * @a CError objects are suitable for being thrown and caught as exceptions.
 * They automatically manage the lifetime of the wrapped @ref IError instance.
 *
 * An error object created by the OLL will always occupy a leaf position in 
 * the hierarchy of error objects, and must always implement an error 
 * interface which occupies a leaf position in the hierarchy of error 
 * interfaces. For example, an error object can never "really" be of a type 
 * like @c COLIErrorImpl, implementing @ref IOLIError; it must in fact have 
 * a derived type such as @c CInvalidStateErrorImpl, implementing 
 * @ref IInvalidStateError.
 *
 * However, no such restriction exists for proxy classes. For example, 
 * the class @ref CError can be instantiated and used as a proxy to any type 
 * of error object; the class @ref COLIError can be instantiated and used as 
 * a proxy to any error object which implements @ref IOLIError. In this sense, 
 * the proxy classes are like pointers: a pointer (or proxy object) can have 
 * a different type from the error object to which it refers.
 *
 * In practice, the most specific error class will be instantiated by
 * @ref CheckAndThrow. That is, for every interface in the @ref IError 
 * hierarchy, a suitable counterpart derived from @a CError exists. Only if
 * a future BOA implementation introduces additional error interfaces, 
 * @a CheckAndThrow will fall back to the base class proxies in old client 
 * applications.
 *
 * @remark Prefer using @a CError (or derived) objects over plain @ref IError 
 *         instances.
 * @remark Instances of this class and its sub-classes are intended to be
 *         thrown as excepions.
 * @remark This class is not intended for derivation as its implementation
 *         details may change in future BOA versions. If you still derive
 *         from it, you may need to adapt or rewrite that code for newer
 *         versions of the API.
 * @remark All public methods are thread-safe.
 *
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", IError, CheckAndThrow
 */
class CError
{
private:
    /** @brief  Wrapped instance pointer.
                This is a pointer to the error object which is wrapped by 
                this proxy.

        @remark May not be @c NULL unless the compiler implicitly calls
                the copy constructor.
        @since  BOA 1.3 
     */
    mutable IError* m_pImpl;    

    /** @brief  Unavailable assigment operator.

        @remark @ref CError and derived instances may not be copied.
        @since  BOA 1.3 
     */
    CError& operator=( const CError& rhs );

protected:

    /** @brief  Hidden copy constructor.
    
                This constructor is protected to prevent a client of 
                the OLI from copying this object. It is protected (and 
                not private) to allow the copy constructors of our 
                subclasses to access it.

        @remark @ref CError and derived instances may not be copied.
                However, the compiler may call this implicitly depending
                on its exception handling implementation. In that case,
                @a other will no longer own the wrapped pointer and must
                be destroyed immediately.

        @since  BOA 1.3 
     */
    CError( const CError& other ) OLI_NOTHROW
        : m_pImpl (other.m_pImpl) 
    {
        other.m_pImpl = NULL;
    }

public:

    /** @brief  Public constructor.
    
                Initialises this proxy object with a pointer to the error 
                object which it will wrap. 

        @param[in] pImpl  The error object which will be wrapped by this 
                          proxy. Must not be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting @a CError object owns the @ref IError 
                interface instance that the pointer refers to. Upon 
                wrapper @ref destructor "destruction", the underlying 
                object will be destroyed and all pointers refering to it
                become invalid.

        @since  BOA 1.3 
        @see    IError::Destroy
     */
    CError( IError* pImpl ) OLI_NOTHROW : m_pImpl( pImpl ) {}

    /** @brief  Destructor.
        @anchor CError_destructor
        
                This destructor ensures that the wrapped error object is 
                destroyed when the proxy is destroyed.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    IError::Destroy
     */
    virtual ~CError() OLI_NOTHROW
    {
        /* In case that the copy constructor got called. */
        if (m_pImpl)
            m_pImpl->Destroy();
    }

    /** @brief  Returns a human-readable string describing the error.
     
        @return A human-readable string describing the error.
                The returned pointer remains valid only for the lifetime
                of this @ref IError instance.
        @exception <none> This function must not throw exceptions.

        @remark This method delegates to the similarly-named method on the 
                wrapped @ref IError pointer.
        @since  BOA 1.3 
        @see    IError::GetMessage
     */
    const char* GetMessage() OLI_NOTHROW
    {
        return m_pImpl->GetMessage();
    }

    /** @brief  Check whether the wrapped object implements the 
                specified interface.

        @param[in] typeId   The @c TYPE value from IError or one of its 
                            derived interfaces.
        @return
        - true  The object implementing this interface is of the 
                specified type, or derives from that type. Use a
                @c static_cast<> to access that type.
        - false Otherwise.
        @exception <none> This function must not throw exceptions.

        @remark Use @c static_cast<> to for down-casting CError instances.
        @remark This method delegates to the similarly-named method on the 
                wrapped @ref IError pointer.
        @since  BOA 1.3 
        @see    IError::IsOfType, GetMostDerivedType
     */
    bool IsOfType( unsigned long typeId ) OLI_NOTHROW
    {
        return m_pImpl->IsOfType( typeId );
    }

    /** @brief  Interface type actually implemented by the wrapped object.

        @return The @c TYPE value of the most-derived error interface 
                implemented by the wrapped object.
        @exception <none> This function must not throw exceptions.

        @remark Use @c static_cast<> to for down-casting CError instances.
        @remark This method delegates to the similarly-named method on the 
                wrapped @ref IError pointer.
        @since  BOA 1.3 
        @see    IError::GetMostDerivedType, IsOfType
     */
    unsigned long GetMostDerivedType() OLI_NOTHROW
    {
        return m_pImpl->GetMostDerivedType();
    }
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Wraps an object which implements @ref IUnexpectedError.
 *
 * @remark Prefer using @a CUnexpectedError objects over plain 
 *         @ref IUnexpectedError instances.
 * @remark Instances of this class intended to be thrown as excepions.
 * @remark This class is not intended for derivation as its implementation
 *         details may change in future BOA versions. If you still derive
 *         from it, you may need to adapt or rewrite that code for newer
 *         versions of the API.
 * @remark All public methods are thread-safe.
 *
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         IUnexpectedError, CheckAndThrow
 */
class CUnexpectedError : public CError
{
private:
    /** @brief  Hidden copy constructor.
    
                This constructor is private to prevent a client application
                of the OLI from copying this object.

        @remark @ref CError and derived instances may not be copied.
                However, the compiler may call this implicitly depending
                on its exception handling implementation. In that case,
                @a other will no longer own the wrapped pointer and must
                be destroyed immediately.

        @since  BOA 1.3 
     */
    CUnexpectedError( const CUnexpectedError& other ) OLI_NOTHROW
        : CError( other ) 
    {}

    /** @brief  Unavailable assigment operator.

        @remark @ref CError and derived instances may not be copied.
        @since  BOA 1.3 
     */
    CUnexpectedError& operator=( CUnexpectedError& rhs );

public:
    /** @brief  Public constructor.
    
                Initialises this proxy object with a pointer to the error 
                object which it will wrap. 

        @param[in] pImpl  The error object which will be wrapped by this 
                          proxy. Must not be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting @a CUnexpectedError object owns the 
                @ref IUnexpectedError interface instance that the pointer 
                refers to. Upon wrapper @ref CError_destructor "destruction", 
                the underlying object will be destroyed and all pointers 
                refering to it become invalid.

        @since  BOA 1.3 
        @see    IError::Destroy
     */
    CUnexpectedError( IUnexpectedError* pImpl ) OLI_NOTHROW 
        : CError( pImpl ) 
    {}
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Wraps an object which implements @ref IOCIError.
 *
 * @remark Prefer using @a COCIError objects over plain 
 *         @ref IOCIError instances.
 * @remark Instances of this class intended to be thrown as excepions.
 * @remark This class is not intended for derivation as its implementation
 *         details may change in future BOA versions. If you still derive
 *         from it, you may need to adapt or rewrite that code for newer
 *         versions of the API.
 * @remark All public methods are thread-safe.
 *
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         IOCIError, CheckAndThrow
 */
class COCIError : public CError
{
private:

    /** @brief  Wrapped instance pointer.

                This is a pointer to the specific error object interface
                which is wrapped by this proxy. It gives access to additional
                methods.

        @remark May not be @c NULL.
        @since  BOA 1.3 
        @see    CError::m_pImpl
     */
    IOCIError* m_pImpl; 

    /** @brief  Hidden copy constructor.
    
                This constructor is private to prevent a client application
                of the OLI from copying this object.

        @remark @ref CError and derived instances may not be copied.
                However, the compiler may call this implicitly depending
                on its exception handling implementation. In that case,
                @a other will no longer own the wrapped pointer and must
                be destroyed immediately.

        @since  BOA 1.3 
     */
    COCIError( const COCIError& other ) OLI_NOTHROW
        : CError( other ), m_pImpl( other.m_pImpl )
    {
    }

    /** @brief  Unavailable assigment operator.

        @remark @ref CError and derived instances may not be copied.
        @since  BOA 1.3 
     */
    COCIError& operator=( COCIError& rhs );

public:

    /** @brief  Public constructor.
    
                Initialises this proxy object with a pointer to the error 
                object which it will wrap. 

        @param[in] pImpl  The error object which will be wrapped by this 
                          proxy. Must not be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting @a COCIError object owns the 
                @ref IOCIError interface instance that the pointer 
                refers to. Upon wrapper @ref CError_destructor "destruction", 
                the underlying object will be destroyed and all pointers 
                refering to it become invalid.

        @since  BOA 1.3 
        @see    IError::Destroy
     */
    COCIError( IOCIError* pImpl ) OLI_NOTHROW 
        : CError( pImpl ), m_pImpl( pImpl ) 
    {}

    /** @brief  Obtains the OCI error code which identifies the type 
                of the OCI error.

        @return The OCI error code which identifies the type of the 
                OCI error.
        @exception <none> This function must not throw exceptions.

        @remark This method delegates to the similarly-named method on the 
                wrapped @ref IOCIError pointer.
        @since  BOA 1.3 
        @see    IOCIError::GetCode
     */
    OCI_ErrorCode GetCode() OLI_NOTHROW
    {
        return m_pImpl->GetCode();
    }
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Base class for all wrappers around @ref IOLIError and its 
 *         sub-classes.
 *
 * @remark Prefer using @a COLIError objects over plain 
 *         @ref IOLIError instances.
 * @remark Instances of this class intended to be thrown as excepions.
 * @remark This class is not intended for derivation as its implementation
 *         details may change in future BOA versions. If you still derive
 *         from it, you may need to adapt or rewrite that code for newer
 *         versions of the API.
 * @remark All public methods are thread-safe.
 *
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         IOLIError, CheckAndThrow
 */
class COLIError : public CError
{
private:

    /** @brief  Unavailable assigment operator.

        @remark @ref CError and derived instances may not be copied.
        @since  BOA 1.3 
     */
    COLIError& operator=( COLIError& rhs );

protected:

    /** @brief  Hidden copy constructor.
    
                This constructor is protected to prevent a client of 
                the OLI from copying this object. It is protected (and 
                not private) to allow the copy constructors of our 
                subclasses to access it.

        @remark @ref CError and derived instances may not be copied.
                However, the compiler may call this implicitly depending
                on its exception handling implementation. In that case,
                @a other will no longer own the wrapped pointer and must
                be destroyed immediately.

        @since  BOA 1.3 
     */
    COLIError( const COLIError& other ) OLI_NOTHROW : CError( other ) {}

public:

    /** @brief  Public constructor.
    
                Initialises this proxy object with a pointer to the error 
                object which it will wrap. 

        @param[in] pImpl  The error object which will be wrapped by this 
                          proxy. Must not be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting @a COLIError object owns the 
                @ref IOLIError interface instance that the pointer 
                refers to. Upon wrapper @ref CError_destructor "destruction", 
                the underlying object will be destroyed and all pointers 
                refering to it become invalid.

        @since  BOA 1.3 
        @see    IError::Destroy
     */
    COLIError( IOLIError* pImpl ) OLI_NOTHROW : CError( pImpl ) {}
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Wraps an object which implements @ref INotSupportedError.
 *
 * @remark Prefer using @a CNotSupportedError objects over plain 
 *         @ref INotSupportedError instances.
 * @remark Instances of this class intended to be thrown as excepions.
 * @remark This class is not intended for derivation as its implementation
 *         details may change in future BOA versions. If you still derive
 *         from it, you may need to adapt or rewrite that code for newer
 *         versions of the API.
 * @remark All public methods are thread-safe.
 *
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         INotSupportedError, CheckAndThrow
 */
class CNotSupportedError : public COLIError
{
private:
    /** @brief  Hidden copy constructor.
    
                This constructor is private to prevent a client application
                of the OLI from copying this object.

        @remark @ref CError and derived instances may not be copied.
                However, the compiler may call this implicitly depending
                on its exception handling implementation. In that case,
                @a other will no longer own the wrapped pointer and must
                be destroyed immediately.

        @since  BOA 1.3 
     */
    CNotSupportedError( const CNotSupportedError& other ) OLI_NOTHROW
        : COLIError( other ) 
    {}

    /** @brief  Unavailable assigment operator.

        @remark @ref CError and derived instances may not be copied.
        @since  BOA 1.3 
     */
    CNotSupportedError& operator=( CNotSupportedError& rhs );

public:

    /** @brief  Public constructor.
    
                Initialises this proxy object with a pointer to the error 
                object which it will wrap. 

        @param[in] pImpl  The error object which will be wrapped by this 
                          proxy. Must not be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting @a CNotSupportedError object owns the 
                @ref INotSupportedError interface instance that the pointer 
                refers to. Upon wrapper @ref CError_destructor "destruction", 
                the underlying object will be destroyed and all pointers 
                refering to it become invalid.

        @since  BOA 1.3 
        @see    IError::Destroy
     */
    CNotSupportedError( INotSupportedError* pImpl ) OLI_NOTHROW
        : COLIError( pImpl ) 
    {}
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Wraps an object which implements @ref IVersionError.
 *
 * @remark Prefer using @a CVersionError objects over plain 
 *         @ref IVersionError instances.
 * @remark Instances of this class intended to be thrown as excepions.
 * @remark This class is not intended for derivation as its implementation
 *         details may change in future BOA versions. If you still derive
 *         from it, you may need to adapt or rewrite that code for newer
 *         versions of the API.
 * @remark All public methods are thread-safe.
 *
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         IVersionError, CheckAndThrow
 */
class CVersionError : public COLIError
{
private:
    /** @brief  Hidden copy constructor.
    
                This constructor is private to prevent a client application
                of the OLI from copying this object.

        @remark @ref CError and derived instances may not be copied.
                However, the compiler may call this implicitly depending
                on its exception handling implementation. In that case,
                @a other will no longer own the wrapped pointer and must
                be destroyed immediately.

        @since  BOA 1.3 
     */
    CVersionError( const CVersionError& other ) OLI_NOTHROW
        : COLIError( other ) 
    {}

    /** @brief  Unavailable assigment operator.

        @remark @ref CError and derived instances may not be copied.
        @since  BOA 1.3 
     */
    CVersionError& operator=( CVersionError& rhs );

public:

    /** @brief  Public constructor.
    
                Initialises this proxy object with a pointer to the error 
                object which it will wrap. 

        @param[in] pImpl  The error object which will be wrapped by this 
                          proxy. Must not be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting @a CVersionError object owns the 
                @ref IVersionError interface instance that the pointer 
                refers to. Upon wrapper @ref CError_destructor "destruction", 
                the underlying object will be destroyed and all pointers 
                refering to it become invalid.

        @since  BOA 1.3 
        @see    IError::Destroy
     */
    CVersionError( IVersionError* pImpl ) OLI_NOTHROW
        : COLIError( pImpl ) 
    {}
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Wraps an object which implements @ref IInvalidStateError.
 *
 * @remark Prefer using @a CInvalidStateError objects over plain 
 *         @ref IInvalidStateError instances.
 * @remark Instances of this class intended to be thrown as excepions.
 * @remark This class is not intended for derivation as its implementation
 *         details may change in future BOA versions. If you still derive
 *         from it, you may need to adapt or rewrite that code for newer
 *         versions of the API.
 * @remark All public methods are thread-safe.
 *
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         IInvalidStateError, CheckAndThrow
 */
class CInvalidStateError : public COLIError
{
private:
    /** @brief  Hidden copy constructor.
    
                This constructor is private to prevent a client application
                of the OLI from copying this object.

        @remark @ref CError and derived instances may not be copied.
                However, the compiler may call this implicitly depending
                on its exception handling implementation. In that case,
                @a other will no longer own the wrapped pointer and must
                be destroyed immediately.

        @since  BOA 1.3 
     */
    CInvalidStateError( const CInvalidStateError& other ) OLI_NOTHROW
        : COLIError( other ) 
    {}

    /** @brief  Unavailable assigment operator.

        @remark @ref CError and derived instances may not be copied.
        @since  BOA 1.3 
     */
    CInvalidStateError& operator=( CInvalidStateError& rhs );

public:

    /** @brief  Public constructor.
    
                Initialises this proxy object with a pointer to the error 
                object which it will wrap. 

        @param[in] pImpl  The error object which will be wrapped by this 
                          proxy. Must not be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting @a CInvalidStateError object owns the 
                @ref IInvalidStateError interface instance that the pointer 
                refers to. Upon wrapper @ref CError_destructor "destruction", 
                the underlying object will be destroyed and all pointers 
                refering to it become invalid.

        @since  BOA 1.3 
        @see    IError::Destroy
     */
    CInvalidStateError( IInvalidStateError* pImpl ) OLI_NOTHROW
        : COLIError( pImpl ) 
    {}
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Wraps an object which implements @ref IControlFlowError.
 *
 * @remark Prefer using @a CControlFlowError objects over plain 
 *         @ref IControlFlowError instances.
 * @remark Instances of this class intended to be thrown as excepions.
 * @remark This class is not intended for derivation as its implementation
 *         details may change in future BOA versions. If you still derive
 *         from it, you may need to adapt or rewrite that code for newer
 *         versions of the API.
 * @remark All public methods are thread-safe.
 *
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         IControlFlowError, CheckAndThrow
 */
class CControlFlowError : public COLIError
{
private:
    /** @brief  Hidden copy constructor.
    
                This constructor is private to prevent a client application
                of the OLI from copying this object.

        @remark @ref CError and derived instances may not be copied.
                However, the compiler may call this implicitly depending
                on its exception handling implementation. In that case,
                @a other will no longer own the wrapped pointer and must
                be destroyed immediately.

        @since  BOA 1.3 
     */
    CControlFlowError( const CControlFlowError& other ) OLI_NOTHROW
        : COLIError( other ) 
    {}

    /** @brief  Unavailable assigment operator.

        @remark @ref CError and derived instances may not be copied.
        @since  BOA 1.3 
     */
    CControlFlowError& operator=( CControlFlowError& rhs );

public:

    /** @brief  Public constructor.
    
                Initialises this proxy object with a pointer to the error 
                object which it will wrap. 

        @param[in] pImpl  The error object which will be wrapped by this 
                          proxy. Must not be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting @a CControlFlowError object owns the 
                @ref IControlFlowError interface instance that the pointer 
                refers to. Upon wrapper @ref CError_destructor "destruction", 
                the underlying object will be destroyed and all pointers 
                refering to it become invalid.

        @since  BOA 1.3 
        @see    IError::Destroy
     */
    CControlFlowError( IControlFlowError* pImpl ) OLI_NOTHROW
        : COLIError( pImpl ) 
    {}
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Wraps an object which implements @ref IInvalidOperationError.
 *
 * @remark Prefer using @a CInvalidOperationError objects over plain 
 *         @ref IInvalidOperationError instances.
 * @remark Instances of this class intended to be thrown as excepions.
 * @remark This class is not intended for derivation as its implementation
 *         details may change in future BOA versions. If you still derive
 *         from it, you may need to adapt or rewrite that code for newer
 *         versions of the API.
 * @remark All public methods are thread-safe.
 *
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         IInvalidOperationError, CheckAndThrow
 */
class CInvalidOperationError : public COLIError
{
private:
    /** @brief  Hidden copy constructor.
    
                This constructor is private to prevent a client application
                of the OLI from copying this object.

        @remark @ref CError and derived instances may not be copied.
                However, the compiler may call this implicitly depending
                on its exception handling implementation. In that case,
                @a other will no longer own the wrapped pointer and must
                be destroyed immediately.

        @since  BOA 1.3 
     */
    CInvalidOperationError( const CInvalidOperationError& other ) OLI_NOTHROW
        : COLIError( other ) 
    {}

    /** @brief  Unavailable assigment operator.

        @remark @ref CError and derived instances may not be copied.
        @since  BOA 1.3 
     */
    CInvalidOperationError& operator=( CInvalidOperationError& rhs );

public:

    /** @brief  Public constructor.
    
                Initialises this proxy object with a pointer to the error 
                object which it will wrap. 

        @param[in] pImpl  The error object which will be wrapped by this 
                          proxy. Must not be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting @a CInvalidOperationError object owns the 
                @ref IInvalidOperationError interface instance that the pointer 
                refers to. Upon wrapper @ref CError_destructor "destruction", 
                the underlying object will be destroyed and all pointers 
                refering to it become invalid.

        @since  BOA 1.3 
        @see    IError::Destroy
     */
    CInvalidOperationError( IInvalidOperationError* pImpl ) OLI_NOTHROW
        : COLIError( pImpl ) 
    {}
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Wraps an object which implements @ref IOutOfRangeError.
 *
 * @remark Prefer using @a COutOfRangeError objects over plain 
 *         @ref IOutOfRangeError instances.
 * @remark Instances of this class intended to be thrown as excepions.
 * @remark This class is not intended for derivation as its implementation
 *         details may change in future BOA versions. If you still derive
 *         from it, you may need to adapt or rewrite that code for newer
 *         versions of the API.
 * @remark All public methods are thread-safe.
 *
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         IOutOfRangeError, CheckAndThrow
 */
class COutOfRangeError : public COLIError
{
private:
    /** @brief  Hidden copy constructor.
    
                This constructor is private to prevent a client application
                of the OLI from copying this object.

        @remark @ref CError and derived instances may not be copied.
                However, the compiler may call this implicitly depending
                on its exception handling implementation. In that case,
                @a other will no longer own the wrapped pointer and must
                be destroyed immediately.

        @since  BOA 1.3 
     */
    COutOfRangeError( const COutOfRangeError& other ) OLI_NOTHROW
        : COLIError( other ) 
    {}

    /** @brief  Unavailable assigment operator.

        @remark @ref CError and derived instances may not be copied.
        @since  BOA 1.3 
     */
    COutOfRangeError& operator=( COutOfRangeError& rhs );

public:

    /** @brief  Public constructor.
    
                Initialises this proxy object with a pointer to the error 
                object which it will wrap. 

        @param[in] pImpl  The error object which will be wrapped by this 
                          proxy. Must not be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting @a COutOfRangeError object owns the 
                @ref IOutOfRangeError interface instance that the pointer 
                refers to. Upon wrapper @ref CError_destructor "destruction", 
                the underlying object will be destroyed and all pointers 
                refering to it become invalid.

        @since  BOA 1.3 
        @see    IError::Destroy
     */
    COutOfRangeError( IOutOfRangeError* pImpl ) OLI_NOTHROW
        : COLIError( pImpl ) 
    {}
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Wraps an object which implements @ref IAllocError.
 *
 * @remark Prefer using @a CAllocError objects over plain 
 *         @ref IAllocError instances.
 * @remark Instances of this class intended to be thrown as excepions.
 * @remark This class is not intended for derivation as its implementation
 *         details may change in future BOA versions. If you still derive
 *         from it, you may need to adapt or rewrite that code for newer
 *         versions of the API.
 * @remark All public methods are thread-safe.
 *
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         IAllocError, CheckAndThrow
 */
class CAllocError : public COLIError
{
private:
    /** @brief  Hidden copy constructor.
    
                This constructor is private to prevent a client application
                of the OLI from copying this object.

        @remark @ref CError and derived instances may not be copied.
                However, the compiler may call this implicitly depending
                on its exception handling implementation. In that case,
                @a other will no longer own the wrapped pointer and must
                be destroyed immediately.

        @since  BOA 1.3 
     */
    CAllocError( const CAllocError& other ) OLI_NOTHROW
        : COLIError( other ) 
    {}

    /** @brief  Unavailable assigment operator.

        @remark @ref CError and derived instances may not be copied.
        @since  BOA 1.3 
     */
    CAllocError& operator=( CAllocError& rhs );

public:

    /** @brief  Public constructor.
    
                Initialises this proxy object with a pointer to the error 
                object which it will wrap. 

        @param[in] pImpl  The error object which will be wrapped by this 
                          proxy. Must not be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting @a CAllocError object owns the 
                @ref IAllocError interface instance that the pointer 
                refers to. Upon wrapper @ref CError_destructor "destruction", 
                the underlying object will be destroyed and all pointers 
                refering to it become invalid.

        @since  BOA 1.3 
        @see    IError::Destroy
     */
    CAllocError( IAllocError* pImpl ) OLI_NOTHROW
        : COLIError( pImpl ) 
    {}
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Wraps an object which implements @ref IOverflowError.
 *
 * @remark Prefer using @a COverflowError objects over plain 
 *         @ref IOverflowError instances.
 * @remark Instances of this class intended to be thrown as excepions.
 * @remark This class is not intended for derivation as its implementation
 *         details may change in future BOA versions. If you still derive
 *         from it, you may need to adapt or rewrite that code for newer
 *         versions of the API.
 * @remark All public methods are thread-safe.
 *
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         IOverflowError, CheckAndThrow
 */
class COverflowError : public COLIError
{
private:
    /** @brief  Hidden copy constructor.
    
                This constructor is private to prevent a client application
                of the OLI from copying this object.

        @remark @ref CError and derived instances may not be copied.
                However, the compiler may call this implicitly depending
                on its exception handling implementation. In that case,
                @a other will no longer own the wrapped pointer and must
                be destroyed immediately.

        @since  BOA 1.3 
     */
    COverflowError( const COverflowError& other ) OLI_NOTHROW
        : COLIError( other ) 
    {}

    /** @brief  Unavailable assigment operator.

        @remark @ref CError and derived instances may not be copied.
        @since  BOA 1.3 
     */
    COverflowError& operator=( COverflowError& rhs );

public:

    /** @brief  Public constructor.
    
                Initialises this proxy object with a pointer to the error 
                object which it will wrap. 

        @param[in] pImpl  The error object which will be wrapped by this 
                          proxy. Must not be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting @a COverflowError object owns the 
                @ref IOverflowError interface instance that the pointer 
                refers to. Upon wrapper @ref CError_destructor "destruction", 
                the underlying object will be destroyed and all pointers 
                refering to it become invalid.

        @since  BOA 1.3 
        @see    IError::Destroy
     */
    COverflowError( IOverflowError* pImpl ) OLI_NOTHROW
        : COLIError( pImpl ) 
    {}
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Wraps an object which implements @ref IInvalidArgumentError.
 *
 * @remark Prefer using @a CInvalidArgumentError objects over plain 
 *         @ref IInvalidArgumentError instances.
 * @remark Instances of this class intended to be thrown as excepions.
 * @remark This class is not intended for derivation as its implementation
 *         details may change in future BOA versions. If you still derive
 *         from it, you may need to adapt or rewrite that code for newer
 *         versions of the API.
 * @remark All public methods are thread-safe.
 *
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         IInvalidArgumentError, CheckAndThrow
 */
class CInvalidArgumentError : public COLIError
{
private:
    /** @brief  Hidden copy constructor.
    
                This constructor is private to prevent a client application
                of the OLI from copying this object.

        @remark @ref CError and derived instances may not be copied.
                However, the compiler may call this implicitly depending
                on its exception handling implementation. In that case,
                @a other will no longer own the wrapped pointer and must
                be destroyed immediately.

        @since  BOA 1.3 
     */
    CInvalidArgumentError( const CInvalidArgumentError& other ) OLI_NOTHROW
        : COLIError( other ) 
    {}

    /** @brief  Unavailable assigment operator.

        @remark @ref CError and derived instances may not be copied.
        @since  BOA 1.3 
     */
    CInvalidArgumentError& operator=( CInvalidArgumentError& rhs );

public:

    /** @brief  Public constructor.
    
                Initialises this proxy object with a pointer to the error 
                object which it will wrap. 

        @param[in] pImpl  The error object which will be wrapped by this 
                          proxy. Must not be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting @a CInvalidArgumentError object owns the 
                @ref IInvalidArgumentError interface instance that the pointer 
                refers to. Upon wrapper @ref CError_destructor "destruction", 
                the underlying object will be destroyed and all pointers 
                refering to it become invalid.

        @since  BOA 1.3 
        @see    IError::Destroy
     */
    CInvalidArgumentError( IInvalidArgumentError* pImpl ) OLI_NOTHROW
        : COLIError( pImpl ) 
    {}
};

/**
 * @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Wraps an object which implements @ref IDomainError.
 *
 * @remark Prefer using @a CDomainError objects over plain 
 *         @ref IDomainError instances.
 * @remark Instances of this class intended to be thrown as excepions.
 * @remark This class is not intended for derivation as its implementation
 *         details may change in future BOA versions. If you still derive
 *         from it, you may need to adapt or rewrite that code for newer
 *         versions of the API.
 * @remark All public methods are thread-safe.
 *
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", 
 *         IDomainError, CheckAndThrow
 */
class CDomainError : public COLIError
{
private:
    /** @brief  Hidden copy constructor.
    
                This constructor is private to prevent a client application
                of the OLI from copying this object.

        @remark @ref CError and derived instances may not be copied.
                However, the compiler may call this implicitly depending
                on its exception handling implementation. In that case,
                @a other will no longer own the wrapped pointer and must
                be destroyed immediately.

        @since  BOA 1.3 
     */
    CDomainError( const CDomainError& other ) OLI_NOTHROW
        : COLIError( other ) 
    {}

    /** @brief  Unavailable assigment operator.

        @remark @ref CError and derived instances may not be copied.
        @since  BOA 1.3 
     */
    CDomainError& operator=( CDomainError& rhs );

public:

    /** @brief  Public constructor.
    
                Initialises this proxy object with a pointer to the error 
                object which it will wrap. 

        @param[in] pImpl  The error object which will be wrapped by this 
                          proxy. Must not be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting @a CDomainError object owns the 
                @ref IDomainError interface instance that the pointer 
                refers to. Upon wrapper @ref CError_destructor "destruction", 
                the underlying object will be destroyed and all pointers 
                refering to it become invalid.

        @since  BOA 1.3 
        @see    IError::Destroy
     */
    CDomainError( IDomainError* pImpl ) OLI_NOTHROW
        : COLIError( pImpl ) 
    {}
};

/** @ingroup GROUP_OLI_COMMON_ERROR
 * @brief  Converts OLI error objects into client-side exceptions.
 *
 * This helper function is intended to be called by an inline wrapper 
 * method defined in an OLI interface class. NOTE that @a  CheckAndThrow() 
 * must be compiled by the client.
 *
 * When an OLI method fails, it returns a pointer to an object which 
 * implements an interface derived from IError. The OLI method is wrapped 
 * by an inline wrapper method, defined in the same interface, which passes 
 * the @ref IError pointer to @a CheckAndThrow(). @a CheckAndThrow() will 
 * discover the derived type of the error interface, then construct an 
 * appropriately-typed proxy for the derived error interface. 
 *
 * For example, if the @ref IError interface's derived type is actually 
 * @ref IInvalidStateError, @a CheckAndThrow() constructs an instance of 
 * @ref CInvalidStateError as a proxy for @a IInvalidStateError.
 *
 * @a CheckAndThrow throws the proxy object which it constructs. The proxy 
 * class, or one of its base classes, can be caught by the client. See the 
 * examples in @ref ErrorReporting "error reporting" for more details.
 *
 * @param[in] pError  A pointer to an OLI error object.
 * @exception CError  This function must not throw exceptions.
 *
 * @remark To prevent this function from being inlined, define the
 *         @ref OLI_DONT_INLINE_CHECKANDTHROW.
 *
 * @since  BOA 1.3 
 * @see    @ref ErrorReporting "error reporting", IError, CError,
 *         OLI_DONT_INLINE_CHECKANDTHROW
 */
#if defined(OLI_DONT_INLINE_CHECKANDTHROW)
void CheckAndThrow( IError* pError );
#else
#if !defined(__IN_SOURCE__)
#include "ErrorProxies.cpp"
#endif
#endif

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_ERRORPROXIES__INCLUDED__)
