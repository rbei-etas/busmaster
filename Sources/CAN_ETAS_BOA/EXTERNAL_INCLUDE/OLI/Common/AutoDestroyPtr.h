///////////////////////////////////////////////////////////
//  AutoDestroyPtr.h
///////////////////////////////////////////////////////////

#if !defined(__OLI_AUTODESTROYPTR_INCLUDED__)
#define __OLI_AUTODESTROYPTR_INCLUDED__

/// @todo update doxygen comments

#include "BeginNamespace.h"

/** 
*/

template<class I>
class AutoDestroyPtr
{
private:

    mutable const I* instance;

public:

    AutoDestroyPtr() 
        : instance (NULL)
    {
    };

    AutoDestroyPtr (const I* instance) 
        : instance (instance)
    {
    };

    AutoDestroyPtr (const AutoDestroyPtr& rhs) 
        : instance (rhs.instance)
    {
        rhs.instance = NULL;
    };

    ~AutoDestroyPtr() 
    {
        if (instance)
            instance->Destroy();
    };

    AutoDestroyPtr<I>& operator=(const AutoDestroyPtr<I>& rhs)
    {
        instance = rhs.instance;
        rhs.instance = NULL;

        return *this;
    }

    const I* get() const
    {
        return instance;
    }

    operator bool() const
    {
        return get() != NULL;
    }

    bool operator!() const
    {
        return get() == NULL;
    }

    const I* operator->() const
    {
        return instance;
    }

    const I& operator*() const
    {
        return *instance;
    }
};

#include "EndNamespace.h"

#endif // !defined(__OLI_AUTODESTROYPTR_INCLUDED__)
