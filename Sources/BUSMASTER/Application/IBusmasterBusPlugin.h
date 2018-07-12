#pragma once

#include "../ProtocolDefinitions/ProtocolsDefinitions.h"
#include "../BusmasterDBNetwork/Include/IBMNetWorkService.h"
#include "../BusmasterDriverInterface/Include/IBusService.h"
#include "../Include/BaseDefs.h"
#include "IMenuInterface.h"
#include "IBusmasterPlugin.h"
#include "ILicenseProvider.h"
class IBusmasterBusPlugin
{
public:
	IBusmasterBusPlugin(){};
	virtual ETYPE_BUS getBusType() = 0;
	virtual IBusService* getBusService() = 0;
    virtual void setBusmasterInterface(IBusmasterBusPluginInterface*) = 0;
    virtual ESTATUS_BUS getBusStatus() = 0;
    virtual void getMenuInterface(IMenuInterface**) = 0;
    virtual void getConfiguration(xmlNodePtr& pNodePtr) = 0;            //TODO
    virtual void setConfiguration(const xmlDocPtr) = 0;            //TODO
	virtual void closureOperations() = 0;
    
	virtual ILicenseProvider *getLicenseProvider() = 0;

	virtual ~IBusmasterBusPlugin(){};
};

