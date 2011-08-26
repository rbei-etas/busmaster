/**
*
* \file
*
* \brief This file declares proxy classes, each of which wraps a heap-allocated error object created by the OLL to encapsulate
* information about an error which has occurred. Each proxy class is instantiated by the client and uses the "pimpl" design
* pattern to delegate to the wrapped error object. The existence of the proxy classes gives the client of the OLL two advantages:
*   - The client can throw an instance of a proxy object, thereby using exception syntax to handle the associated error.
*   - Each proxy object automatically ensures that the corresponding heap-allocated error object is correctly deleted.
*
* See \ref ErrorReporting for an explanation of how errors are reported from OLI methods.
*
* Copyright ETAS GmbH, Stuttgart.
*
* \version $Id: ErrorProxies.h 218029 2010-05-10 07:13:08Z guwirth $
*
******************************************************************************/

#if !defined(__OLI_ERRORPROXIES__INCLUDED__)
#define __OLI_ERRORPROXIES__INCLUDED__

#include "ErrorInterfaces.h"

#include "BeginNamespace.h"

// The Windows SDK defines GetMessage as a preprocessor macro, so we must undefine it before we can use it as a function name.
#ifdef GetMessage 
#undef GetMessage
#endif

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
void CheckAndThrow( IError* pError );

/**
 * This is the base class for all error proxy classes. The hierarchy of proxy classes matches the hierarchy of error interfaces,
 * which is based on IError. This is done to allow the client to handle proxies (and thereby errors) in type-specific or
 * type-independent ways as required. Each error proxy class wraps a corresponding interface derived from IError, which in turn is
 * implemented by a corresponding error object derived from CErrorImpl.
 *
 * An error object created by the OLL must always occupy a leaf position in the hierarchy of error objects, and must always implement
 * an error interface which occupies a leaf position in the hierarchy of error interfaces. For example, an error object can never
 * "really" be of type COLIErrorImpl, implementing IOLIError; it must in fact have a derived type such as CInvalidStateErrorImpl,
 * implementing IInvalidStateError.
 *
 * However, no such restriction exists for proxy classes. For example, the class CError can be instantiated and used as a proxy to
 * any type of error object; the class COLIError can be instantiated and used as a proxy to any error object which implements IOLIError.
 * In this sense, the proxy classes are like pointers: a pointer (or proxy object) can have a different type from the error object
 * to which it refers.
 */
class CError
//TODO QACPP[5:2112] Design Problem: '::ETAS::OLI::CError' has member pointers but no copy assignment operator.
//TODO QACPP[5:2616] Design Problem: This class has declared a copy constructor and a destructor but no copy assignment operator.
{
public:
    /**
     * This constructor initialises this proxy object with a pointer to the error object which it will wrap.
     *
     * \param[in] pImpl     The error object which will be wrapped by this proxy.
     */
    CError( IError* pImpl ) : m_pImpl( pImpl ) {};

    /**
     * This destructor ensures that the wrapped error object is destroyed when the proxy is destroyed.
     */
    virtual ~CError( void )
    {
        m_pImpl->Destroy();
    }

    /// This method delegates to the similarly-named method on the wrapped IError pointer.
    virtual const char* GetMessage( void )
    {
        return m_pImpl->GetMessage();
    }
    /// This method delegates to the similarly-named method on the wrapped IError pointer.
    virtual bool IsOfType( unsigned long typeId )
    {
        return m_pImpl->IsOfType( typeId );
    }
    /// This method delegates to the similarly-named method on the wrapped IError pointer.
    virtual unsigned long GetMostDerivedType( void )
    {
        return m_pImpl->GetMostDerivedType();
    }

protected:
    /// This constructor is protected to prevent a client of the OLL from copying this object. It is protected (and not private)
    /// to allow the copy constructors of our subclasses to access it.
    CError( const CError& other ) {};
//TODO QACPP[5:4265] Design Problem: POD member object 'm_pImpl' is not copied or initialised by this copy constructor.

private:
    /// This operator is private to prevent a client of the OLL from making an assignment to this object.
    CError& operator=( CError* rhs );

private:
    IError* m_pImpl;    ///< This is a pointer to the error object which is wrapped by this proxy.
};

/**
 * This proxy wraps an object which implements IUnexpectedError.
 */
class CUnexpectedError : public CError
//TODO QACPP[5:2618] Design Problem: This class has declared a copy constructor but no copy assignment operator.
{
public:
    /**
     * This constructor initialises this proxy object with a pointer to the error object which it will wrap.
     *
     * \param[in] pImpl     The error object which will be wrapped by this proxy.
     */
    CUnexpectedError( IUnexpectedError* pImpl ) : CError( pImpl ) {};

private:
    /// This constructor is private to prevent a client of the OLL from copying this object. It nevertheless requires a
    /// definition because it is referenced (but not called) by code which throws an instance of this class as an exception.
    CUnexpectedError( const CUnexpectedError& other ) : CError( other ) {};
    /// This operator is private to prevent a client of the OLL from making an assignment to this object.
    CUnexpectedError& operator=( CUnexpectedError* rhs );
};

/**
 * This proxy wraps an object which implements IOCIError.
 */
class COCIError : public CError
//TODO QACPP[5:2112] Design Problem: '::ETAS::OLI::COCIError' has member pointers but no copy assignment operator.
//TODO QACPP[5:2618] Design Problem: This class has declared a copy constructor but no copy assignment operator.
{
public:
    /**
     * This constructor initialises this proxy object with a pointer to the error object which it will wrap.
     *
     * \param[in] pImpl     The error object which will be wrapped by this proxy.
     */
    COCIError( IOCIError* pImpl ) : CError( pImpl ), m_pImpl( pImpl ) {};

    /// This method delegates to the similarly-named method on the wrapped IOCIError pointer.
    virtual OCI_ErrorCode GetCode( void )
    {
        return m_pImpl->GetCode();
    }

private:
    /// This constructor is private to prevent a client of the OLL from copying this object. It nevertheless requires a
    /// definition because it is referenced (but not called) by code which throws an instance of this class as an exception.
    COCIError( const COCIError& other ) : CError( other ) {};
//TODO QACPP[5:4265] Design Problem: POD member object 'm_pImpl' is not copied or initialised by this copy constructor.
    /// This operator is private to prevent a client of the OLL from making an assignment to this object.
    COCIError& operator=( COCIError* rhs );

private:    
    IOCIError* m_pImpl; ///< CError contains a pointer to the base implementation, but we need a pointer to the derived implementation also.
};

/**
 * This proxy wraps an object which implements IOCIError. It also acts as a base for all proxies which wrap error interfaces
 * derived from IOCIError.
 */
class COLIError : public CError
//TODO QACPP[5:2618] Design Problem: This class has declared a copy constructor but no copy assignment operator.
{
public:
    /**
     * This constructor initialises this proxy object with a pointer to the error object which it will wrap.
     *
     * \param[in] pImpl     The error object which will be wrapped by this proxy.
     */
    COLIError( IOLIError* pImpl ) : CError( pImpl ) {};

protected:
    /// This constructor is protected to prevent a client of the OLL from copying this object. It is protected (and not private)
    /// to allow the copy constructors of our subclasses to access it.
    COLIError( const COLIError& other ) : CError( other ) {};
private:
    /// This operator is private to prevent a client of the OLL from making an assignment to this object.
    COLIError& operator=( COLIError* rhs );
};

/**
 * This proxy wraps an object which implements INotSupportedError.
 */
class CNotSupportedError : public COLIError
//TODO QACPP[5:2618] Design Problem: This class has declared a copy constructor but no copy assignment operator.
{
public:
    /**
     * This constructor initialises this proxy object with a pointer to the error object which it will wrap.
     *
     * \param[in] pImpl     The error object which will be wrapped by this proxy.
     */
    CNotSupportedError( INotSupportedError* pImpl ) : COLIError( pImpl ) {};

private:
    /// This constructor is private to prevent a client of the OLL from copying this object. It nevertheless requires a
    /// definition because it is referenced (but not called) by code which throws an instance of this class as an exception.
    CNotSupportedError( const CNotSupportedError& other ) : COLIError( other ) {};
    /// This operator is private to prevent a client of the OLL from making an assignment to this object.
    CNotSupportedError& operator=( CNotSupportedError* rhs );
};

/**
 * This proxy wraps an object which implements IVersionError.
 */
class CVersionError : public COLIError
//TODO QACPP[5:2618] Design Problem: This class has declared a copy constructor but no copy assignment operator.
{
public:
    /**
     * This constructor initialises this proxy object with a pointer to the error object which it will wrap.
     *
     * \param[in] pImpl     The error object which will be wrapped by this proxy.
     */
    CVersionError( IVersionError* pImpl ) : COLIError( pImpl ) {};

private:
    /// This constructor is private to prevent a client of the OLL from copying this object. It nevertheless requires a
    /// definition because it is referenced (but not called) by code which throws an instance of this class as an exception.
    CVersionError( const CVersionError& other ) : COLIError( other ) {};
    /// This operator is private to prevent a client of the OLL from making an assignment to this object.
    CVersionError& operator=( CVersionError* rhs );
};

/**
 * This proxy wraps an object which implements IInvalidStateError.
 */
class CInvalidStateError : public COLIError
//TODO QACPP[5:2618] Design Problem: This class has declared a copy constructor but no copy assignment operator.
{
public:
    /**
     * This constructor initialises this proxy object with a pointer to the error object which it will wrap.
     *
     * \param[in] pImpl     The error object which will be wrapped by this proxy.
     */
    CInvalidStateError( IInvalidStateError* pImpl ) : COLIError( pImpl ) {};

private:
    /// This constructor is private to prevent a client of the OLL from copying this object. It nevertheless requires a
    /// definition because it is referenced (but not called) by code which throws an instance of this class as an exception.
    CInvalidStateError( const CInvalidStateError& other ) : COLIError( other ) {};
    /// This operator is private to prevent a client of the OLL from making an assignment to this object.
    CInvalidStateError& operator=( CInvalidStateError* rhs );
};

/**
 * This proxy wraps an object which implements IControlFlowError.
 */
class CControlFlowError : public COLIError
//TODO QACPP[5:2618] Design Problem: This class has declared a copy constructor but no copy assignment operator.
{
public:
    /**
     * This constructor initialises this proxy object with a pointer to the error object which it will wrap.
     *
     * \param[in] pImpl     The error object which will be wrapped by this proxy.
     */
    CControlFlowError( IControlFlowError* pImpl ) : COLIError( pImpl ) {};

private:
    /// This constructor is private to prevent a client of the OLL from copying this object. It nevertheless requires a
    /// definition because it is referenced (but not called) by code which throws an instance of this class as an exception.
    CControlFlowError( const CControlFlowError& other ) : COLIError( other ) {};
    /// This operator is private to prevent a client of the OLL from making an assignment to this object.
    CControlFlowError& operator=( CControlFlowError* rhs );
};

/**
 * This proxy wraps an object which implements IInvalidOperationError.
 */
class CInvalidOperationError : public COLIError
//TODO QACPP[5:2618] Design Problem: This class has declared a copy constructor but no copy assignment operator.
{
public:
    /**
     * This constructor initialises this proxy object with a pointer to the error object which it will wrap.
     *
     * \param[in] pImpl     The error object which will be wrapped by this proxy.
     */
    CInvalidOperationError( IInvalidOperationError* pImpl ) : COLIError( pImpl ) {};

private:
    /// This constructor is private to prevent a client of the OLL from copying this object. It nevertheless requires a
    /// definition because it is referenced (but not called) by code which throws an instance of this class as an exception.
    CInvalidOperationError( const CInvalidOperationError& other ) : COLIError( other ) {};
    /// This operator is private to prevent a client of the OLL from making an assignment to this object.
    CInvalidOperationError& operator=( CInvalidOperationError* rhs );
};

/**
 * This proxy wraps an object which implements IOutOfRangeError.
 */
class COutOfRangeError : public COLIError
//TODO QACPP[5:2618] Design Problem: This class has declared a copy constructor but no copy assignment operator.
{
public:
    /**
     * This constructor initialises this proxy object with a pointer to the error object which it will wrap.
     *
     * \param[in] pImpl     The error object which will be wrapped by this proxy.
     */
    COutOfRangeError( IOutOfRangeError* pImpl ) : COLIError( pImpl ) {};

private:
    /// This constructor is private to prevent a client of the OLL from copying this object. It nevertheless requires a
    /// definition because it is referenced (but not called) by code which throws an instance of this class as an exception.
    COutOfRangeError( const COutOfRangeError& other ) : COLIError( other ) {};
    /// This operator is private to prevent a client of the OLL from making an assignment to this object.
    COutOfRangeError& operator=( COutOfRangeError* rhs );
};

/**
 * This proxy wraps an object which implements IAllocError.
 */
class CAllocError : public COLIError
//TODO QACPP[5:2618] Design Problem: This class has declared a copy constructor but no copy assignment operator.
{
public:
    /**
     * This constructor initialises this proxy object with a pointer to the error object which it will wrap.
     *
     * \param[in] pImpl     The error object which will be wrapped by this proxy.
     */
    CAllocError( IAllocError* pImpl ) : COLIError( pImpl ) {};

private:
    /// This constructor is private to prevent a client of the OLL from copying this object. It nevertheless requires a
    /// definition because it is referenced (but not called) by code which throws an instance of this class as an exception.
    CAllocError( const CAllocError& other ) : COLIError( other ) {};
    /// This operator is private to prevent a client of the OLL from making an assignment to this object.
    CAllocError& operator=( CAllocError* rhs );
};

/**
 * This proxy wraps an object which implements IOverflowError.
 */
class COverflowError : public COLIError
//TODO QACPP[5:2618] Design Problem: This class has declared a copy constructor but no copy assignment operator.
{
public:
    /**
     * This constructor initialises this proxy object with a pointer to the error object which it will wrap.
     *
     * \param[in] pImpl     The error object which will be wrapped by this proxy.
     */
    COverflowError( IOverflowError* pImpl ) : COLIError( pImpl ) {};

private:
    /// This constructor is private to prevent a client of the OLL from copying this object. It nevertheless requires a
    /// definition because it is referenced (but not called) by code which throws an instance of this class as an exception.
    COverflowError( const COverflowError& other ) : COLIError( other ) {};
    /// This operator is private to prevent a client of the OLL from making an assignment to this object.
    COverflowError& operator=( COverflowError* rhs );
};

/**
 * This proxy wraps an object which implements IInvalidArgumentError.
 */
class CInvalidArgumentError : public COLIError
//TODO QACPP[5:2618] Design Problem: This class has declared a copy constructor but no copy assignment operator.
{
public:
    /**
     * This constructor initialises this proxy object with a pointer to the error object which it will wrap.
     *
     * \param[in] pImpl     The error object which will be wrapped by this proxy.
     */
    CInvalidArgumentError( IInvalidArgumentError* pImpl ) : COLIError( pImpl ) {};

private:
    /// This constructor is private to prevent a client of the OLL from copying this object. It nevertheless requires a
    /// definition because it is referenced (but not called) by code which throws an instance of this class as an exception.
    CInvalidArgumentError( const CInvalidArgumentError& other ) : COLIError( other ) {};
    /// This operator is private to prevent a client of the OLL from making an assignment to this object.
    CInvalidArgumentError& operator=( CInvalidArgumentError* rhs );
};

/**
 * This proxy wraps an object which implements IDomainError.
 */
class CDomainError : public COLIError
//TODO QACPP[5:2618] Design Problem: This class has declared a copy constructor but no copy assignment operator.
{
public:
    /**
     * This constructor initialises this proxy object with a pointer to the error object which it will wrap.
     *
     * \param[in] pImpl     The error object which will be wrapped by this proxy.
     */
    CDomainError( IDomainError* pImpl ) : COLIError( pImpl ) {};

private:
    /// This constructor is private to prevent a client of the OLL from copying this object. It nevertheless requires a
    /// definition because it is referenced (but not called) by code which throws an instance of this class as an exception.
    CDomainError( const CDomainError& other ) : COLIError( other ) {};
    /// This operator is private to prevent a client of the OLL from making an assignment to this object.
    CDomainError& operator=( CDomainError* rhs );
};

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_ERRORPROXIES__INCLUDED__)
