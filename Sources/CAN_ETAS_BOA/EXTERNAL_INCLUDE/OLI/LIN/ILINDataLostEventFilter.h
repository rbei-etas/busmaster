/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4794 $
*/

/** 
* @file
* @brief  ILINDataLostEventFilter definition
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#if !defined(__OLI_ILINDATALOSTEVENTFILTER_INCLUDED__)
#define __OLI_ILINDATALOSTEVENTFILTER_INCLUDED__

// include used interface

#include "../Common/AutoPtr.h"
#include "../Common/IEventFilter.h"
#include "../Common/ErrorInterfaces.h"
#include "../Common/ErrorProxies.h"

// open ETAS::OLI namespace

#include "../Common/BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

// forward declaration

class ILINDataLostEventFilter;

/**
* @ingroup GROUP_OLI_LIN_FILTERS
* @brief This function instantiates an object supporting 
*        @ref ILINDataLostEventFilter. 
*
*        See @ref BinaryCompatibility "binary compatibility" 
*        for an explanation of why it is needed.
*
*        NOTE that clients are encouraged to access this function 
*        via the wrapper @ref ILINDataLostEventFilter::Create().
*
* @param[in]  minCount   
*         To pass the filter, @ref ILINDataLostEvent::GetCount 
*         must be greater or equal this value.
* @param[out] ppLinDataLostEventFilter   
*         A pointer to an object supporting @ref ILINDataLostEventFilter, 
*         which has already been AddRef-ed. The object must be 
*         reference-counted by the caller, using the object's
*         methods @ref IRefCountable::AddRef "AddRef()" and 
*         @ref IRefCountable::Release "Release()". This is easily 
*         done by wrapping the object pointer in an instance of the 
*         @ref AutoPtr class, which will be done automatically if 
*         the caller accesses @ref ILINDataLostEventFilter_Create() 
*         via the wrapper @ref ILINDataLostEventFilter::Create().
*
* @return A pointer to an interface based on @ref IError, describing 
*         the error which occurred during this function. @c NULL if 
*         no error occurred. See @ref ErrorReporting "error reporting"
*         for more information on how errors are reported.
*
* @exception <none> This function must not throw exceptions.
*
* @since  BOA 1.3 
* @see    @ref BinaryCompatibility "binary compatibility", 
*         @ref ErrorReporting "error reporting",
*         ILINDataLostEventFilter
*/
OLL_API IError* OLI_CALL ILINDataLostEventFilter_Create( 
    uint32 minCount, 
    ILINDataLostEventFilter** ppLinDataLostEventFilter );


/** @ingroup GROUP_OLI_LIN_FILTERS
* @brief  Extension to @ref IEventFilter that also filters for
*         @ref ILINDataLostEvent -specific members.
*
* In addition to the base class conditions, this interface checks
* for the @ref ILINDataLostEvent::GetCount "number of lost frames".
* Events will only pass this filter if this count is at least
* @ref GetMinCount.
*
* This interface's implementation of IFilter::GetIDMask and IFilter::GetIDValue always return 0.
*
* The implementation is expected to match @ref ILINDataLostEvent 
* instances only.
*
* @remark All public methods are thread-safe.
* @coding Client applications may implement this interface.
* @since  BOA 1.3
* @see    @ref filterConcepts "Filter concepts", ILINDataLostEvent
*/

OLI_INTERFACE ILINDataLostEventFilter 
    : public IEventFilter
{
protected:

    /** @brief Destructor.

        This destructor has been hidden to force the client
        application to use proper reference counting.

        @exception <none> This function must not throw exceptions.

        @since  BOA 1.3 
     */
    virtual ~ILINDataLostEventFilter() OLI_NOTHROW {};

public:

    /** @brief  Returns the minimum number of lost frames reported in
                @ref ILINDataLostEvent::GetCount.

        @return Minimum number of lost frames reported in
                @ref ILINDataLostEvent::GetCount.
        @exception <none> This function must not throw exceptions.

        @coding The implemementation must be thread-safe.
        @since  BOA 1.3 
        @see    ILINDataLostEvent::GetCount
     */
    virtual uint32 OLI_CALL GetMinCount() const OLI_NOTHROW = 0;

    /** @brief  Create an @ref ILINDataLostEventFilter instance.

                The instance returned here will only match 
                @ref ILINDataLostEvent messages. All other
                types will not pass the filter.

        @param[in]  minCount   
                To pass the filter, @ref ILINDataLostEvent::GetCount 
                must be greater or equal this value.
        @return New @ref ILINDataLostEventFilter instance.
        @exception CError This function may throw an exception
                derived from @ref CError.

        @remark This is a helper method which wraps 
                @ref ILINDataLostEventFilter_Create(): 
                see @ref BinaryCompatibility "binary compatibility" 
                and @ref ErrorReporting "error reporting"
                for an explanation of why it is needed.
        @since  BOA 1.3 
        @see    @ref filterConcepts "Filter concepts", ILINDataLostEvent
     */
    static AutoPtr<ILINDataLostEventFilter> OLI_CALL 
    Create ( uint32 minCount )
    {
        ILINDataLostEventFilter* pLinDataLostEventFilter = NULL;
        CheckAndThrow( 
            ILINDataLostEventFilter_Create( minCount, 
                                            &pLinDataLostEventFilter ) );

        // The wrapped method has already AddRef-ed the pointer, 
        // so we tell AutoPtr to take ownership of the pointer 
        // without a further AddRef.
        return AutoPtr<ILINDataLostEventFilter>( pLinDataLostEventFilter, false );
    }
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "../Common/EndNamespace.h"

#endif // !defined(__OLI_ILINDATALOSTEVENTFILTER_INCLUDED__)
