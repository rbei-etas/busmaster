#pragma once
#include <Windows.h>
#include "IBusmasterPluginInterface.h"
#include "IBusmasterPlugin.h"
#include "UI/UICreator.h"

class IBusmasterPluginManager
{
public:
    virtual ~IBusmasterPluginManager() {};

    virtual int init( IBusmasterPluginInterface* ) = 0;
    virtual int loadPlugin( const char* pluginFilePath ) = 0;
    virtual int loadPlugins( const char* dir) = 0;
    virtual int drawUI( UIElements uielements ) = 0;
    virtual int notifyPlugins( eBusmaster_Event, void* ) = 0;
    virtual int noifyMenuClick( int menuId ) = 0;
    virtual int noifyMenuUpdate(int menuId, IMenuItem* menuItem) = 0;
    virtual int unLoadPlugins() = 0;
    virtual int getPluginCount() = 0;
    virtual int getPluginConfiguration(xmlNodePtr&)=0;
    virtual IBusmasterPluginConnection* getPluginConnectionPoint(const char* id) = 0;
    virtual int setPluginConfiguration(const xmlDocPtr)=0;
};

typedef int( *GetBusMaterPluginInterface )( IBusmasterPlugin* );