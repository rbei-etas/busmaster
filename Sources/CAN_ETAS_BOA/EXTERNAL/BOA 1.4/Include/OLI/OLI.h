/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4509 $
*/

/** 
* @file
* @brief  Include this for the complete OLI
* @remark The header structure of the OLI may change
*         in future releases. Instead of including
*         individual headers directly, clients should
*         include this single header.
*/

/**

* @page PAGE_OLI_OVERVIEW OLI - Overview
* @htmlinclude "page_oli_overview.html"

* @page PAGE_OLI_FIGURES OLI - Figures
* <HR>
* @image html "figure_oli_class_overview.gif"
* Figure 1: Overview class diagram
* <HR>
* @image html "figure_oli_pap.gif"
* Figure 2: Typical OLI call sequence

	\addtogroup GROUP_OLI_COMMON OLI Common
	@{
		\defgroup GROUP_OLI_COMMON_BASE Base
		\defgroup GROUP_OLI_COMMON_ERROR Error Handling
		\defgroup GROUP_OLI_COMMON_CONFIGURATION Configuration
		\defgroup GROUP_OLI_COMMON_LINK Link
		\defgroup GROUP_OLI_COMMON_QUEUES Queues
		\defgroup GROUP_OLI_COMMON_MESSAGES Messages
		\defgroup GROUP_OLI_COMMON_FILTERS Filters
		\defgroup GROUP_OLI_COMMON_EXCEPTIONS Exceptions
		\ingroup GROUP_OLI_COMMON
	@}

	\addtogroup GROUP_OLI_CAN OLI CAN
	@{
		\defgroup GROUP_OLI_CAN_LINKS Link
		\defgroup GROUP_OLI_CAN_MESSAGES Messages
		\defgroup GROUP_OLI_CAN_FILTERS Filters
		\ingroup GROUP_OLI_CAN
	@}

	\addtogroup GROUP_OLI_FLEXRAY OLI Flexray
	@{
		\defgroup GROUP_OLI_FLEXRAY_LINKS Link
		\defgroup GROUP_OLI_FLEXRAY_MESSAGES Messages
		\defgroup GROUP_OLI_FLEXRAY_FILTERS Filters
		\ingroup GROUP_OLI_FLEXRAY
	@}

	\addtogroup GROUP_OLI_LIN OLI LIN
	@{
		\defgroup GROUP_OLI_LIN_LINKS Link
		\defgroup GROUP_OLI_LIN_MESSAGES Messages
		\defgroup GROUP_OLI_LIN_FILTERS Filters
		\ingroup GROUP_OLI_LIN
	@}
*/

/**
 * \page BinaryCompatibility Binary Compatibility
 *
 * The BOA OLL is required to be compatible with a client application which was built with a different compiler, or one which was
 * built with the same compiler using a different heap manager.
 *
 * This leads to the following problems:
 *  - There is not a complete standardisation of C++ at a binary level on x86 Windows, so how is it possible to guarantee that a
 *    C++ call stack can cross from a client to the OLL DLL?
 *  - If the client application uses a different heap manager from the OLL, an attempt to allocate memory using one heap and free
 *    it on another will cause disaster.
 *
 * In general, these problems can be solved as follows:
 *
 *  - C++ features used by Microsoft COM \em are standardised at a binary level. Therefore, if a C++ DLL has an API which uses only
 *    features defined by COM, a C++ call stack is guaranteed to be able to cross from the client to the DLL:
 *      - Only virtual functions and C-style functions can be exposed to the client.
 *      - Only C datatypes can be used as arguments and return values.
 *      - No exceptions can be thrown from the DLL to the client.
 *      - The client cannot use C++ RTTI on objects implemented by the DLL.
 *      - The DLL cannot expose static or non-virtual member functions (e.g. "operator new").
 *
 *  - Objects defined by the DLL must be allocated and freed within the DLL.
 *      - Thus an object allocated on the DLL's heap will be freed on the same heap.
 *      - The client can \em request creation or destruction of such an object, but it must be \em performed by the DLL.
 *
 * In applying these solutions, the OLL also relies on the following observation:
 *  - A non-virtual inline member function, i.e. a non-virtual member function which is defined in one of the DLL's public header
 *    files, will be compiled by the client application.
 *
 * Therefore the OLL takes the following approach to solve the problem of binary compatibility:
 *
 * OLL objects which are exposed to the client are either owned by the OLL (so the client will never delete them) or implement
 * a reference-counting API, so that the client calls \c Release() when it is finished using the object. The smart pointer class
 * AutoPtr is supplied to assist the client in performing correct reference counting.
 *
 * C-style functions are exported from the OLL and used by the client to create objects within the OLL, instead of \c operator \c new.
 * Each such function is wrapped by a non-virtual inline static member function of an appropriate class. For example:
 * \code
 *  // IMyOLIInterface.h
 *
 *  // This function is defined within OLL.DLL
 *  OLL_API IMyOLIInterface* OLI_CALL IMyOLIInterface_Create();
 *
 *  OLI_INTERFACE IMyOLIInterface
 *  {
 *  public:
 *      static AutoPtr<IMyOLIInterface> OLI_CALL Create()
 *      {
 *          return IMyOLIInterface_Create();
 *      }
 *  };
 * \endcode
 * Note that, as explained above, \c IMyOLIInterface::Create() will be compiled by the client, so it is \c IMyOLIInterface_Create() which
 * actually crosses the boundary into the OLL DLL. \c IMyOLIInterface_Create() is a C-style function, so it is guaranteed to be binary-
 * compatible.
 *
 * The inline static function (such as \c IMyOLIInterface::Create()) has two purposes:
 *  - If the newly-created object is to be reference-counted, the static member function wraps the pointer to the newly-created
 *    object in an instance of the class AutoPtr to assist the client in reference-counting the object correctly.
 *  - The existence of the static member function allows the client to write template code which calls the correct type of \c Create()
 *    method.
 *
 * In a few cases, the OLL must give the client a pointer to a reference-counted object which is created by another object, not by
 * a static method.
 * For example, when a CAN TX queue is created the client executes  \c pMyCANLink->CreateTxQueue(). In such cases, the OLL follows
 * the approach in this example:
 * \code
 *  // IMyOLIInterface.h
 *
 *  OLI_INTERFACE IMyOLIInterface
 *  {
 *  protected:
 *      virtual IMyTxQueue* OLI_CALL CreateTxQueueRaw() = 0;
 *
 *  public:
 *      AutoPtr<IMyTxQueue> OLI_CALL CreateTxQueue()
 *      {
 *          return CreateTxQueueRaw();
 *      }
 *  };
 * \endcode
 * The client calls the non-virtual inline function \c CreateTxQueue() which, as explained above, is compiled by the client. It delegates
 * to the protected virtual function \c CreateTxQueueRaw() which actually crosses the boundary into the OLL DLL. This latter function
 * is binary-compatible.
 *
 * It is also necessary to ensure that the OLL does not throw exceptions to its client, and to implement an alternative mechanism
 * to allow the OLL to report errors to its client. The solution to these problems is discussed separately in \ref ErrorReporting.
 * Many OLI methods make use of both the mechanisms described in this section and those described in \ref ErrorReporting.
 */

// interfaces

#include "Common\OLIDefs.h"
#include "Common\OLITypes.h"
#include "Common\AutoPtr.h"
#include "Common\AutoDestroyPtr.h"
#include "Common\CommonBase.h"
#include "Common\ErrorInterfaces.h"
#include "Common\ErrorProxies.h"
#include "Common\IAPIConnector.h"
#include "Common\IConfigurationContainer.h"
#include "Common\IConfigurationObserver.h"
#include "Common\IConfigurationParameter.h"
#include "Common\IDictionary.h"
#include "Common\IDictionaryEntry.h"
#include "Common\IEvent.h"
#include "Common\IEventFilter.h"
#include "Common\IFilter.h"
#include "Common\IFrameFilter.h"
#include "Common\IInternalErrorEvent.h"
#include "Common\IInternalErrorEventFilter.h"
#include "Common\IHardwareFinder.h"
#include "Common\ILink.h"
#include "Common\IMessage.h"
#include "Common\IQueue.h"
#include "Common\IQueueEvent.h"
#include "Common\IQueueEventFilter.h"
#include "Common\IQueueObserver.h"
#include "Common\IQuickFilterSupport.h"
#include "Common\IRxFrame.h"
#include "Common\IRxMessage.h"
#include "Common\IRxQueue.h"
#include "Common\ITimer.h"
#include "Common\ITimerEvent.h"
#include "Common\ITimerEventFilter.h"
#include "Common\ITimerStatus.h"
#include "Common\ITxMessage.h"
#include "Common\ITxQueue.h"

#include "CAN\CANBase.h"
#include "CAN\ICANErrorFrame.h"
#include "CAN\ICANErrorFrameFilter.h"
#include "CAN\ICANEvent.h"
#include "CAN\ICANEventFilter.h"
#include "CAN\ICANFrameFilter.h"
#include "CAN\ICANLink.h"
#include "CAN\ICANRxFrame.h"
#include "CAN\ICANTxFrame.h"

#include "FlexRay\FlexRayBase.h"
#include "FlexRay\IFlexRayEvent.h"
#include "FlexRay\IFlexRayEventFilter.h"
#include "FlexRay\IFlexRayFrameFilter.h"
#include "FlexRay\IFlexRayLink.h"
#include "FlexRay\IFlexRayReconfigurationMessage.h"
#include "FlexRay\IFlexRayRxFrame.h"
#include "FlexRay\IFlexRaySwitchMessage.h"
#include "FlexRay\IFlexRayTxFrame.h"

#include "LIN\LINBase.h"
#include "LIN\ILINCRCDetectEvent.h"
#include "LIN\ILINCRCDetectEventFilter.h"
#include "LIN\ILINDLCDetectEvent.h"
#include "LIN\ILINDLCDetectEventFilter.h"
#include "LIN\ILINDataLostEvent.h"
#include "LIN\ILINDataLostEventFilter.h"
#include "LIN\ILINErrSyncEvent.h"
#include "LIN\ILINErrSyncEventFilter.h"
#include "LIN\ILINFrameFilter.h"
#include "LIN\ILINLink.h"
#include "LIN\ILINMasterRequest.h"
#include "LIN\ILINRxFrame.h"
#include "LIN\ILINSleepEvent.h"
#include "LIN\ILINSleepEventFilter.h"
#include "LIN\ILINTransmissionStatusEvent.h"
#include "LIN\ILINTransmissionStatusEventFilter.h"
#include "LIN\ILINTxFrame.h"
#include "LIN\ILINWakeUpEvent.h"
#include "LIN\ILINWakeUpEventFilter.h"
