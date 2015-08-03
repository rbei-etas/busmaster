/**
 * \file      LIN_ISOLAR_EVE_VLIN_Extern.h
 * \brief     Exports API functions for LIN Kvaser Hardware interface
 * \author    a1@uincore.net
 * \copyright Copyright (c) 2015, UinCore. All rights reserved.
 *
 * Exports API functions for LIN Kvaser Hardware interface
 */

#pragma once

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
    USAGEMODE HRESULT GetIDIL_LIN_Controller(void** ppvInterface);

#ifdef __cplusplus
}
#endif
