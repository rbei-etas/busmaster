#pragma once
#include <Windows.h>
#include "IBusMasterKernel.h"
#include "../BusmasterDriverInterface/Include/BusmasterDriverInterface.h"
#include "../BusmasterDBNetwork/Include/IBMNetWorkService.h"
class BusMasterKernel : public IBusMasterKernel
{
    pDIL_GetInterface mDIL_GetInterface;
    HMODULE mDriverLibrary;
    static BusMasterKernel* mKernel;
    IBMNetWorkService* mBmNetworkService;
public:
    BusMasterKernel();
    virtual ~BusMasterKernel();
    HRESULT getBusService( ETYPE_BUS busType, IBusService** busService );
    HRESULT getDatabaseService( IBMNetWorkService** dbService );
    static BusMasterKernel* create();
private:
    bool loadDilInterface();
};

