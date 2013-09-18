/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4532 $
*/

/** 
* @file
* @brief  AutoPtr<> template definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_AUTOPTR_INCLUDED__)
#define __OLI_AUTOPTR_INCLUDED__

#include "OLIDefs.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** @brief  Acquire internal @ref AutoPtr<> assignment mutex.
*    
*   @exception <none> This function must not throw exceptions.
*
*   @remark This is an OLI-internal function. Do not call it directly
*           from client application code.
*   @since  BOA 1.3 
*   @see    AutoPtr
*/
OLL_API void OLI_CALL __AutoPtr_assignment_lock() OLI_NOTHROW;

/** @brief  Release internal @ref AutoPtr<> assignment mutex.
*    
*   @exception <none> This function must not throw exceptions.
*
*   @remark This is an OLI-internal function. Do not call it directly
*           from client application code.
*   @since  BOA 1.3 
*   @see    AutoPtr
*/
OLL_API void OLI_CALL __AutoPtr_assignment_unlock() OLI_NOTHROW;

/** @ingroup GROUP_OLI_COMMON_BASE
* @brief  Utility class template for automatic reference counting. 
*
* This template class uses the RAII pattern to satisfy the constraints
* of reference counting OLI objects. All interfaces deriving from
* @ref IRefCountable should be accessed only through pointers wrapped 
* by AutoPtr<>.
*
* Typical pointer operators (assigment, dereferencing, conversion
* to boolean) are being provided. However, there is not comparison
* operator, particularly for @c NULL. Use the @ref get() method
* to compare two pointers. Also, calling the @ref reset method is 
* more efficient than assigning NULL.
*
* Since @a operator& has not been overloaded, AutoPtr<> objects are 
* compatbile with STL containers.  
*
* AutoPtr<> objects are not meant to be shared between threads.
* Every thread should have its own reference to the underlying 
* object. To allow duplication of AutoPtr<> among differnt threads, 
* copy construction, assigment and destruction have been made 
* thread-safe.
*
* @param  I  Type of the wrapped interface pointer. Must be derived
*            from @ref IRefCountable.
*
* @remark Only construction, destruction and assignment are thread-safe.
* @remark This is client-application-side code. Depending on the 
*         compiler used for the client application, some features 
*         may not be available.
* @coding This class is considered "final". Do not inherit from it.
*
* @since  BOA 1.3
* @see    IRefCountable
*/
template<class I>
class AutoPtr
{
private:

    /** @brief  Wrapped instance pointer.
         
        @remark May be @c NULL.
        @since  BOA 1.3 
        @see    IRefCountable
    */
    I* m_instance;

public:

    /** @brief  Default constructor.
        
        This will initialize the internal pointer with @c NULL.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    AutoPtr() OLI_NOTHROW
        : m_instance (NULL)
    {
    }

    /** @brief  Initialize with a new, counted reference.
        
        Initialize the internal pointer with the given @a instance.
        If it is not @c NULL, the reference count will be increased
        by 1.

        @param[in] instance Object that the new @ref AutoPtr<> instance
                            shall point to. May be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting AutoPtr<> object will be treated as
                a new reference to the object. 
        @since  BOA 1.3 
        @see    IRefCountable
     */
    AutoPtr (I* instance) OLI_NOTHROW
        : m_instance (instance)
    {
        if (m_instance)
            m_instance->AddRef();
    }

    /** @brief  Initialize with a new, counted reference.
        
        Initialize the internal pointer with the given @a instance.
        If it is not @c NULL, the reference count will be increased
        by 1.

        @param[in] instance Object that the new @ref AutoPtr<> instance
                            shall point to. May be @c NULL.
        @param[in] addRef   If @c true, create a new reference.
                            Otherwise, @ref IRefCountable::AddRef "AddRef"
                            must already have been called for @a instance.
                            Ignored if @a instance is @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark Client applications should not need to call this 
                constructor directly. 
        @since  BOA 1.3 
        @see    IRefCountable
     */
    AutoPtr (I* instance, bool addRef) OLI_NOTHROW
        : m_instance (instance)
    {
        if ((m_instance != NULL) && addRef)
            m_instance->AddRef();
    }

    /** @brief  Duplicate an interface reference.
        
        Initialize the internal pointer with a reference to the same
        interface as @a rhs. If it is not @c NULL, the reference count 
        will be increased by 1.

        @param[in] rhs  Source interface reference to be copied.
                        May be a @c NULL reference.
        @exception <none> This function must not throw exceptions.

        @remark This constructor is guarateed to work if @a rhs
                is managed by a different thread. If the source
                pointer gets @ref reset or destroyed by the other
                thread while this construction is being executed, 
                the copy may either result in a @em valid, counted 
                reference to the old interface. In that case, it is 
                safe to use. Or, it will contain a @a NULL reference.
        @since  BOA 1.3 
        @see    IRefCountable
     */
    AutoPtr (const AutoPtr& rhs) OLI_NOTHROW
    {
        // We are about to access rhs.m_instance. It is protected by a singleton mutex.
        // (We are also accessing m_instance, but it requires no protection during a constructor).
        __AutoPtr_assignment_lock();

        m_instance = rhs.m_instance;
        if (m_instance)
            m_instance->AddRef();

        __AutoPtr_assignment_unlock();
    }

#if defined(_MSC_VER)
    /** @brief  Duplicate an interface reference.
        
        Initialize the internal pointer with a reference to a different
        interface on the same object as @a rhs. If it is not @c NULL, 
        the reference count will be increased by 1.

        @param Other    Source interface type. This interface must be  
                        equal to @a I or an interface derived from it.
        @param[in] rhs  Source interface reference to be copied.
                        May be a @c NULL reference.
        @exception <none> This function must not throw exceptions.

        @remark This constructor is guarateed to work if @a rhs
                is managed by a different thread. If the source
                pointer gets @ref reset or destroyed by the other
                thread while this construction is being executed, 
                the copy may either result in a @em valid, counted 
                reference to the old interface. In that case, it is 
                safe to use. Or, it will contain a @a NULL reference.
        @remark This constructor is only available with Microsoft
                Visual C++ compilers.
        @since  BOA 1.3 
        @see    IRefCountable
     */
    template<class Other>
    AutoPtr (const AutoPtr<Other>& rhs) OLI_NOTHROW
    {
        // We are about to access rhs.m_instance. It is protected by a singleton mutex.
        // (We are also accessing m_instance, but it requires no protection during a constructor).
        __AutoPtr_assignment_lock();

        m_instance = rhs.get();
        if (m_instance)
            m_instance->AddRef();

        __AutoPtr_assignment_unlock();
    }

#if (_MSC_VER >= 1600)
    /** @brief  Move-construct from a temporary interface reference.
        
        Moves the reference from the temporary @a rhs object to the new
        wrapper instance. The source reference will be reset, i.e. the
        new object takes ownership of the moved reference.

        @param[in] rhs  Temporary source interface reference to be copied.
                        May be a @c NULL reference.
        @exception <none> This function must not throw exceptions.

        @remark This constructor is only available with Microsoft
                Visual C++ compilers version 16.00 or newer 
                (Visual Studio 2010+).
        @since  BOA 1.3 
        @see    IRefCountable
     */
    AutoPtr (AutoPtr&& rhs) OLI_NOTHROW
        : m_instance (rhs.m_instance)
    {
        rhs.m_instance = NULL;
    }
#endif // _MSC_VER > 1600

#endif // _MSC_VER

    /** @brief  Destructor.
        
        Resets the internal interface reference. If this reference is
        not @c NULL, the reference count will be decreased by 1. If the
        last reference has been released, the underlying object will be 
        destroyed automatically.

        @exception <none> This function must not throw exceptions.

        @remark This method is guarateed to work correctly even while a copy
                of this reference is being created by a different thread . 
        @since  BOA 1.3 
        @see    IRefCountable
     */
    ~AutoPtr() OLI_NOTHROW
    {
        reset();
    }

    /** @brief  Replace an interface reference.
        
        Assign the internal pointer with a reference to the same
        interface as @a rhs. If it is not @c NULL, the reference count 
        will be increased by 1. Similarly, the previously held reference 
        will be released and the underlying object destroyed as soon
        as the last reference has been @ref IRefCountable::Release "removed".

        @param[in] rhs  Source interface reference to be copied.
                        May be a @c NULL reference.
        @return Reference to this wrapper instance.
        @exception <none> This function must not throw exceptions.

        @remark Assignment is guarateed to work correctly even if @a rhs
                is managed by a different thread. If the source
                pointer gets @ref reset or destroyed by the other
                thread while this construction is being executed, 
                the copy may either result in a @em valid, counted 
                reference to the old interface. In that case, it is 
                safe to use. Or, it will contain a @a NULL reference.
                These guarantees are valid for source and destination alike.
        @remark Self-assignment is legal and thread-safe.
        @since  BOA 1.3 
        @see    IRefCountable
     */
    AutoPtr<I>& operator=(const AutoPtr<I>& rhs) OLI_NOTHROW
    {
        // We are about to access m_instance and rhs.m_instance. They are both protected by a singleton mutex.
        __AutoPtr_assignment_lock();

        // Take a temporary reference to *m_instance. Doing this allows us to release our final reference to
        // *m_instance outside any mutex, thereby avoiding unnecessary serialisation of a possible destructor call.
        if( m_instance )
        {
            m_instance->AddRef();
        }
        I* instance = m_instance;

        // Overwrite m_instance with rhs.m_instance. This requires us to take an additional reference to *rhs.m_instance.
        if( rhs.m_instance )
        {
            rhs.m_instance->AddRef();
        }
        if (m_instance)
        {
            m_instance->Release();
        }
        m_instance = rhs.m_instance;

        // We have finished modifying m_instance and rhs.m_instance.
        __AutoPtr_assignment_unlock();

        // Release our final reference to the previous *m_instance.
        if (instance)
        {
            instance->Release();
        }

        return *this;
    }

#if defined(_MSC_VER)
    /** @brief  Replace the interface reference.
        
        Assign the internal pointer with a reference to the same
        interface as @a rhs. If it is not @c NULL, the reference count 
        will be increased by 1. Similarly, the previously held reference 
        will be released and the underlying object destroyed as soon
        as the last reference has been @ref IRefCountable::Release "removed".

        @param Other    Source interface type. This interface must be  
                        equal to @a I or an interface derived from it.
        @param[in] rhs  Source interface reference to be copied.
                        May be a @c NULL reference.
        @return Reference to this wrapper instance.
        @exception <none> This function must not throw exceptions.

        @remark Assignment is guarateed to work correctly even if @a rhs
                is managed by a different thread. If the source
                pointer gets @ref reset or destroyed by the other
                thread while this construction is being executed, 
                the copy may either result in a @em valid, counted 
                reference to the old interface. In that case, it is 
                safe to use. Or, it will contain a @a NULL reference.
                These guarantees are valid for source and destination alike.
        @remark Self-assignment is legal and thread-safe even if
                @a I and @a Other differ but refer to the same object.
        @remark This constructor is only available with Microsoft
                Visual C++ compilers.
        @since  BOA 1.3 
        @see    IRefCountable
     */
    template<class Other>
    AutoPtr<I>& operator=(const AutoPtr<Other>& rhs) OLI_NOTHROW
    {
        // We are about to access m_instance and rhs.m_instance. They are both protected by a singleton mutex.
        __AutoPtr_assignment_lock();

        // Take a temporary reference to *m_instance. Doing this allows us to release our final reference to
        // *m_instance outside any mutex, thereby avoiding unnecessary serialisation of a possible destructor call.
        if( m_instance )
        {
            m_instance->AddRef();
        }
        I* instance = m_instance;

        // Overwrite m_instance with rhs.m_instance. This requires us to take an additional reference to *rhs.m_instance.
        if( rhs.get() )
        {
            rhs.get()->AddRef();
        }
        if (m_instance)
        {
            m_instance->Release();
        }
        m_instance = rhs.get();

        // We have finished modifying m_instance and rhs.m_instance.
        __AutoPtr_assignment_unlock();

        // Release our final reference to the previous *m_instance.
        if (instance)
        {
            instance->Release();
        }

        return *this;
    }

#if (_MSC_VER >= 1600)
    /** @brief  Replace from a temporary interface reference.
        
        Move the reference held by the temporary wrapper instance to
        this instance and reset the tempoary. The previously held reference 
        will be released and the underlying object destroyed as soon
        as the last reference has been @ref IRefCountable::Release "removed".

        @param[in] rhs  Source interface reference to be copied.
                        May be a @c NULL reference.
        @return Reference to this wrapper instance.
        @exception <none> This function must not throw exceptions.

        @remark Assignment is guarateed to work correctly even if @a rhs
                is managed by a different thread. If the source
                pointer gets @ref reset or destroyed by the other
                thread while this construction is being executed, 
                the copy may either result in a @em valid, counted 
                reference to the old interface. In that case, it is 
                safe to use. Or, it will contain a @a NULL reference.
                These guarantees are valid for source and destination alike.
        @remark Self-assignment is legal and thread-safe.
        @remark This constructor is only available with Microsoft
                Visual C++ compilers version 16.00 or newer 
                (Visual Studio 2010+).

        @since  BOA 1.3 
        @see    IRefCountable
     */
    AutoPtr<I>& operator=(AutoPtr<I>&& rhs) OLI_NOTHROW
    {
        I* instance = NULL;

        // We are about to access m_instance and rhs.m_instance. They are both protected by a singleton mutex.
        __AutoPtr_assignment_lock();

        if (m_instance != rhs.m_instance)
        {
            // Take an additional (temporary) reference to *m_instance. Doing this allows us to release our final
            // reference to *m_instance outside any mutex, thereby avoiding unnecessary serialisation of a possible
            // destructor call.
            if( m_instance )
            {
                m_instance->AddRef();
            }
            instance = m_instance;

            // Overwrite m_instance. We implicitly transfer the reference held by rhs.m_instance to m_instance.
            if (m_instance)
            {
                m_instance->Release();
            }
            m_instance     = rhs.m_instance;
            rhs.m_instance = NULL;
        }

        // We have finished modifying m_instance and rhs.m_instance.
        __AutoPtr_assignment_unlock();

        // Release our final reference to the previous *m_instance.
        if (instance)
        {
            instance->Release();
        }

        return *this;
    }
#endif // _MSC_VER > 1600

#endif // _MSC_VER

    /** @brief  Get the interface pointer.
        
        @return The interface pointer managed by this object.
                May be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark This function is not thread-safe with respect to
                assignment or destruction. Concurrent read-only
                access is legal.
        @since  BOA 1.3 
     */
    I* get() const OLI_NOTHROW
    {
        return m_instance;
    }

    /** @brief  Release the interface reference.
        
        Resets the internal interface reference. If this reference is
        not @c NULL, the reference count will be decreased by 1. Once the
        last reference has been @ref IRefCountable::Release "released", 
        the underlying object will be destroyed automatically.

        @exception <none> This function must not throw exceptions.

        @remark This method is guarateed to work correctly even while a copy
                of this reference is being created by a different thread. 
        @remark May be called even if the internal pointer is @c NULL. 
        @since  BOA 1.3 
        @see    IRefCountable
     */
    void reset() OLI_NOTHROW
    {
        // We are about to access m_instance. It is protected by a singleton mutex.
        __AutoPtr_assignment_lock();

        // Take an additional (temporary) reference to *m_instance. Doing this allows us to release our final
        // reference to *m_instance outside any mutex, thereby avoiding unnecessary serialisation of a possible
        // destructor call.
        if( m_instance )
        {
            m_instance->AddRef();
        }
        I* instance = m_instance;

        // Release m_instance.
        if (m_instance)
        {
            m_instance->Release();
        }
        m_instance = NULL;

        // We have finished modifying m_instance.
        __AutoPtr_assignment_unlock();

        // Release our final reference to the previous *m_instance.
        if (instance)
        {
            instance->Release();
        }
    }

    /** @brief  Check for a non-NULL interface reference.
        @anchor operator_bool
        
        @return @c true, if the wrapped pointer is not @c NULL.
                @c false, otherwise.
        @exception <none> This function must not throw exceptions.

        @remark This function is not thread-safe with respect to
                assignment or destruction. Concurrent read-only
                access is legal.
        @since  BOA 1.3 
     */
    operator bool() const OLI_NOTHROW
    {
        return get() != NULL;
    }

    /** @brief  Check for a NULL interface reference.
        @anchor operator_not
        
        @return @c true, if the wrapped pointer is @c NULL.
                @c false, otherwise.
        @exception <none> This function must not throw exceptions.

        @remark This function is not thread-safe with respect to
                assignment or destruction. Concurrent read-only
                access is legal.
        @since  BOA 1.3 
     */
    bool operator!() const OLI_NOTHROW
    {
        return get() == NULL;
    }

    /** @brief  Dereference a constant interface pointer.
        
        @return The interface being referenced by this object.
        @exception <none> This function must not throw exceptions.

        @remark Calling this function for @c NULL references is illegal
                and will result in undefined behavior.
        @remark This function is not thread-safe with respect to
                assignment or destruction. Concurrent read-only
                access is legal.
        @since  BOA 1.3 
        @see    @ref operator_not "operator!", 
                @ref operator_bool "operator bool"
     */
    const I* operator->() const OLI_NOTHROW
    {
        return m_instance;
    }

    /** @brief  Dereference an interface pointer.
        
        @return The interface being referenced by this object.
        @exception <none> This function must not throw exceptions.

        @remark Calling this function for @c NULL references is illegal
                and will result in undefined behavior.
        @remark This function is not thread-safe with respect to
                assignment or destruction. Concurrent read-only
                access is legal.
        @since  BOA 1.3 
        @see    @ref operator_not "operator!", 
                @ref operator_bool "operator bool"
     */
    I* operator->() OLI_NOTHROW
    {
        return m_instance;
    }

    /** @brief  Dereference a constant interface pointer.
        
        @return The interface being referenced by this object.
        @exception <none> This function must not throw exceptions.

        @remark Calling this function for @c NULL references is illegal
                and will result in undefined behavior.
        @remark This function is not thread-safe with respect to
                assignment or destruction. Concurrent read-only
                access is legal.
        @since  BOA 1.3 
        @see    @ref operator_not "operator!", 
                @ref operator_bool "operator bool"
     */
    const I& operator*() const OLI_NOTHROW
    {
        return *m_instance;
    }

    /** @brief  Dereference an interface pointer.
        
        @return The interface being referenced by this object.
        @exception <none> This function must not throw exceptions.

        @remark Calling this function for @c NULL references is illegal
                and will result in undefined behavior.
        @remark This function is not thread-safe with respect to
                assignment or destruction. Concurrent read-only
                access is legal.
        @since  BOA 1.3 
        @see    @ref operator_not "operator!", 
                @ref operator_bool "operator bool"
     */
    I& operator*() OLI_NOTHROW
    {
        return *m_instance;
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_AUTOPTR_INCLUDED__)
