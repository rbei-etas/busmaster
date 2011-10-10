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
* @brief  AutoDestroyPtr<> definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/
#if !defined(__OLI_AUTODESTROYPTR_INCLUDED__)
#define __OLI_AUTODESTROYPTR_INCLUDED__

#include "OLIDefs.h"

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** @ingroup GROUP_OLI_COMMON_BASE
* @brief  Utility class template for automatic handling of interfaces
*         that require explicit destruction. 
*
* This template class uses the RAII pattern to manage interfaces
* with a public @ref ITxMessage::Destroy "Destroy" method. So, far
* there is only the @ref ITxMessage interface that requires explicit
* destruction of instances. The behavior resembles @c std::auto_ptr.
*
* Typical pointer operators (assigment, dereferencing, conversion
* to boolean) are being provided. However, there is not comparison
* operator, particularly for @c NULL. Use the @ref get() method
* to compare two pointers. Also, calling the @ref reset method is 
* more efficient than assigning NULL.
*
* AutoDestroyPtr<> objects are generally not compatbile with 
* STL containers.  
*
* @param  I  Type of the wrapped interface pointer. Must be provide
*            a non-throwing Destroy() method.
*
* @remark This class is not thread-safe. 
* @coding This class is considered "final". Do not inherit from it.
*
* @since  BOA 1.3
* @see    ITxMessage
*/
template<class I>
class AutoDestroyPtr
{
private:

    /** @brief  Wrapped pointer instance.
    *    
    *   @remark May be @c NULL.
    *   @since  BOA 1.3 
    */
    mutable const I* instance;

public:

    /** @brief  Default constructor.
        
        This will initialize the internal pointer with @c NULL.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    AutoDestroyPtr() OLI_NOTHROW
        : instance (NULL)
    {
    };

    /** @brief  Initialize with a pointer.
        
        Initialize the internal pointer with the given @a instance.

        @param[in] instance Object that the new @ref AutoDestroyPtr<> 
                            instance shall point to. May be @c NULL.
        @exception <none> This function must not throw exceptions.

        @remark The resulting AutoDestroyPtr<> object owns the interface 
                instance that the pointer refers to. Upon wrapper 
                @ref destructor "destruction", @ref reset or 
                @ref assignment "re-assignment", the underlying object 
                will be destroyed and all pointers refering to it
                become invalid.
        @since  BOA 1.3 
        @see    ITxMessage::Destroy
     */
    AutoDestroyPtr (const I* instance) OLI_NOTHROW
        : instance (instance)
    {
    };

    /** @brief  Move the instance owner to a new wrapper instance.
        
        Initialize the internal pointer with a reference to the same
        interface as @a rhs and reset the interface pointer @a rhs.

        @param[in, out] rhs  
                Source interface reference to be moved. May be 
                a @c NULL reference. Will be a @c NULL reference 
                after the copy constructor finished.
        @exception <none> This function must not throw exceptions.

        @remark The resulting AutoDestroyPtr<> object owns interface 
                instance that the pointer refers to. Upon wrapper 
                @ref destructor "destruction", @ref reset or 
                @ref assignment "re-assignment", the underlying object 
                will be destroyed and all pointers refering to it
                become invalid.
        @since  BOA 1.3 
        @see    ITxMessage::Destroy
     */
    AutoDestroyPtr (const AutoDestroyPtr& rhs) OLI_NOTHROW
        : instance (rhs.instance)
    {
        rhs.instance = NULL;
    };

    /** @brief  Destructor.
        @anchor destructor
        
        Resets the internal interface reference. If this reference is
        not @c NULL, the Destroy method of the underlying object will be 
        called, in turn destroying that object.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
        @see    ITxMessage::Destroy
     */
    ~AutoDestroyPtr() OLI_NOTHROW
    {
        if (instance)
            instance->Destroy();
    };

    /** @brief  Move interface pointer from source and destroy
                current target interface instance.
        @anchor assignment
        
        Set the internal pointer to the instance managed by @a rhs
        and reset the source pointer. The previously held reference 
        will be destroyed, if it wasn't @c NULL before.

        @param[in, out] rhs  
                Source interface reference to be moved. May be 
                a @c NULL reference. Will be a @c NULL reference 
                after the copy constructor finished.
        @return Reference to this wrapper instance.
        @exception <none> This function must not throw exceptions.

        @remark This AutoDestroyPtr<> object owns interface 
                instance that the pointer refers to. Upon wrapper 
                @ref destructor "destruction", @ref reset or 
                @ref assignment "re-assignment", the underlying object 
                will be destroyed and all pointers refering to it
                become invalid.
        @since  BOA 1.3 
        @see    ITxMessage::Destroy
     */
    AutoDestroyPtr<I>& operator=(const AutoDestroyPtr<I>& rhs) OLI_NOTHROW
    {
        if (this != &rhs)
        {
            if (instance)
                instance->Destroy();
            
            instance = rhs.instance;
            rhs.instance = NULL;
        }

        return *this;
    }

    /** @brief  Destroy the referenced object.
        
        Resets the internal interface pointer. If this pointer is
        not @c NULL, the Destroy method gets called, in turn destroying
        the underlying object.

        @exception <none> This function must not throw exceptions.

        @remark May be called even if the internal pointer is @c NULL. 
        @since  BOA 1.3 
        @see    IRefCountable
     */
    void reset() const OLI_NOTHROW
    {
        if (instance)
        {
            instance->Destroy();
            instance = NULL;
        }
    }

    /** @brief  Get the interface pointer.
        
        @return The interface pointer managed by this object.
                May be @c NULL.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    const I* get() const OLI_NOTHROW
    {
        return instance;
    }

    /** @brief  Check for a non-NULL interface pointer.
        @anchor operator_bool
        
        @return @c true, if the wrapped pointer is not @c NULL.
                @c false, otherwise.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    operator bool() const OLI_NOTHROW
    {
        return get() != NULL;
    }

    /** @brief  Check for a NULL interface pointer.
        @anchor operator_not
        
        @return @c true, if the wrapped pointer is @c NULL.
                @c false, otherwise.
        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    bool operator!() const OLI_NOTHROW
    {
        return get() == NULL;
    }

    /** @brief  Dereference a constant interface pointer.
        
        @return The interface being referenced by this object.
        @exception <none> This function must not throw exceptions.

        @remark Calling this function for @c NULL pointers is illegal
                and will result in undefined behavior.
        @since  BOA 1.3 
        @see    @ref operator_not "operator!", 
                @ref operator_bool "operator bool"
     */
    const I* operator->() const OLI_NOTHROW
    {
        return instance;
    }

    /** @brief  Dereference an interface pointer.
        
        @return The interface being referenced by this object.
        @exception <none> This function must not throw exceptions.

        @remark Calling this function for @c NULL pointers is illegal
                and will result in undefined behavior.
        @since  BOA 1.3 
        @see    @ref operator_not "operator!", 
                @ref operator_bool "operator bool"
     */
    I* operator->() OLI_NOTHROW
    {
        return instance;
    }

    /** @brief  Dereference a constant interface pointer.
        
        @return The interface being referenced by this object.
        @exception <none> This function must not throw exceptions.

        @remark Calling this function for @c NULL pointers is illegal
                and will result in undefined behavior.
        @since  BOA 1.3 
        @see    @ref operator_not "operator!", 
                @ref operator_bool "operator bool"
     */
    const I& operator*() const OLI_NOTHROW
    {
        return *instance;
    }

    /** @brief  Dereference an interface pointer.
        
        @return The interface being referenced by this object.
        @exception <none> This function must not throw exceptions.

        @remark Calling this function for @c NULL pointers is illegal
                and will result in undefined behavior.
        @since  BOA 1.3 
        @see    @ref operator_not "operator!", 
                @ref operator_bool "operator bool"
     */
    I& operator*() OLI_NOTHROW
    {
        return *instance;
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

#endif // !defined(__OLI_AUTODESTROYPTR_INCLUDED__)
