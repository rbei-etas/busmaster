#pragma once

#include "../ProtocolDefinitions/ProtocolsDefinitions.h"
#include "../BusmasterDBNetwork/Include/IBMNetWorkService.h"
#include "../BusmasterDriverInterface/Include/IBusService.h"

class IBusMasterKernel
{
public:
    virtual ~IBusMasterKernel() {};
    virtual HRESULT getBusService(ETYPE_BUS busType, IBusService** busService) = 0;
    virtual HRESULT getDatabaseService( IBMNetWorkService** dbService ) = 0;
};




#if defined BUSMASTERKERNEL_EXPORTS
#define KERNEL_USAGEMODE   __declspec(dllexport)
#else
#define KERNEL_USAGEMODE   __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

    /*  Exported function list */
    KERNEL_USAGEMODE HRESULT getBusmasterKernel( IBusMasterKernel** );
    typedef HRESULT( *pGetBusmasterKernel )( IBusMasterKernel** );
    KERNEL_USAGEMODE HRESULT DIL_GetInterface( ETYPE_BUS eBusType, void** ppvInterface );
#ifdef __cplusplus
}
#endif

