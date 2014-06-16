/*****************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  CAN_PEAK_USB_Extern.h
  Description   :  Exports API functions for PEAK CAN Hardware interface
  $Log:   X:/Archive/Sources/DIL/DIL_CAN/CAN_PEAK_USB_Extern.h_v  $
 *
 *    Rev 1.3   27 Apr 2011 19:08:44   CANMNTTM
 * GetTimeModeMapping is updated for QueryPerformance counter
 *
 *    Rev 1.2   15 Apr 2011 18:47:56   CANMNTTM
 * Added RBEI Copyright information.
 *
 *    Rev 1.1   23 Mar 2011 14:54:48   CANMNTTM
 * Support upto 32 channels
 *
 *    Rev 1.0   16 Aug 2010 21:56:32   rac2kor
 *

  Author(s)     :  Pradeep Kadoor
  Date Created  :  15/04/2010
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*****************************************************************************/
#if !defined CAN_PEAK_USB_EXTERN_H__INCLUDED_
#define CAN_PEAK_USB_EXTERN_H__INCLUDED_


#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

    /*  Exported function list */
    USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface);


#ifdef __cplusplus
}
#endif

#endif // CAN_PEAK_USB_EXTERN_H__INCLUDED_