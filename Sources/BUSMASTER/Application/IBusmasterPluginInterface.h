#pragma once
#include <string>
#include "IBusMasterKernel.h"
#include "BusmasterEvents.h"
#include "IBusmasterPluginConnection.h"
#include "IVariableLayer.h"

using namespace VariableManager;

#define EXT_INFO_PARAM      UINT_PTR
#define OUT_INFO_PARAM      UINT_PTR
////Info required any the plugins from busmaster//////
#define busmaster_version       0x1         //extended param - nullptr, expected INFO param: char[25]
#define busmaster_window        0x2         //extended param - nullptr, expected INFO param: &HWND

#define busmaster_install_path  0x3         //extended param - nullptr, expected INFO param: char[1024]
#define busmaster_default_path  0x4         //extended param - nullptr, expected INFO param: char[1024]

#define bus_status              0x10        //extended param - nullptr, expected INFO param: 
#define numeric_mode            0x11        //extended param - nullptr, expected INFO param: &eNumeric_mode



class IBusmasterPluginInterface
{
public:
    virtual int getDbService( IBMNetWorkGetService** )=0;
    virtual int getDilService( ETYPE_BUS, IBusService** ) = 0;
    virtual int displayMessage( char* message )=0;
    virtual int getInfo(int infoType, EXT_INFO_PARAM, /*IN, OUT*/ OUT_INFO_PARAM infoData) = 0;
    virtual int postMessage() = 0;
    virtual int getPluginConnectionPoint(const char* pluginId, IBusmasterPluginConnection**) = 0;    //TODO::
	virtual int getVariableCommunicationLayer(IVariableLayer** variableLayer) = 0;
};

class IBusmasterBusPluginInterface : public IBusmasterPluginInterface
{
public:
    virtual ~IBusmasterBusPluginInterface(){};
    virtual int getDbSetService(IBMNetWorkService**) = 0;
    virtual int notifyPlugins(eBusmaster_Event, void*) = 0;
};