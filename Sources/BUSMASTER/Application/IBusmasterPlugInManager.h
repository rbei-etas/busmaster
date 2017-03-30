#pragma once
#include <Windows.h>
#include "IBusmasterPluginInterface.h"
#include "IBusmasterPlugin.h"
#include "IBusmasterBusPlugin.h"
#include "UI/UICreator.h"

#define PLUGIN_ERR_LOAD_SUCCESS             0x0
#define PLUGIN_ERR_INVALID_LOAD_STATE       0x800000
#define PLUGIN_ERR_INVALID_XML              0x800001
#define PLUGIN_ERR_SCHEMA_FILE_NOT_FOUND    0x800002
#define PLUGIN_ERR_DLL_NOT_FOUND            0x800003
#define PLUGIN_ERR_INVALID_INTERFACE        0x800004
#define PLUGIN_ERR_INVALID_LICENCE          0x800005

struct BaseBusmasterPlugin
{
    HMODULE mDllHandle = nullptr;
    pluginMenuList menuList;
    std::string mPluginId = "";
    std::string mDescription = "";
    std::string mPluginDllpath = "";
    std::string mMinBusMasterVer = "";
    IMenuInterface* mMenuInterface = nullptr;
    int mPluginLoadState = PLUGIN_ERR_INVALID_LOAD_STATE;
};
struct BusmasterBusPluginConfiguration : public BaseBusmasterPlugin
{
    ETYPE_BUS mBusType;
    IBusmasterBusPlugin* mBusPluginInterface = nullptr;
};
struct BusmasterPluginConfiguration : public BaseBusmasterPlugin
{
	IBusmasterPlugin* mPluginInterface = nullptr;
	IEvent* mNotifyEvent = nullptr;
};
class IBusmasterPluginManager
{
public:
    virtual ~IBusmasterPluginManager() {};

    virtual int init(IBusmasterBusPluginInterface*) = 0;
	virtual int loadBusPlugins(const char* pluginFilePath) = 0;
    virtual int loadPlugin( const char* pluginFilePath ) = 0;
    virtual int loadPlugins( const char* dir) = 0;
	virtual int addPlugin(BusmasterPluginConfiguration& pluginInfo) = 0;
    virtual int drawUI( UIElements uielements ) = 0;
    virtual int notifyPlugins( eBusmaster_Event, void* ) = 0;
    virtual int noifyMenuClick( int menuId ) = 0;
    virtual int noifyMenuUpdate(int menuId, IMenuItem* menuItem) = 0;
    virtual int unLoadPlugins() = 0;
    virtual int getPluginCount() = 0;
    virtual int getPluginConfiguration(xmlNodePtr&)=0;
    virtual IBusmasterPluginConnection* getPluginConnectionPoint(const char* id) = 0;
    virtual int setPluginConfiguration(const xmlDocPtr)=0;
	virtual int notifyAppClose()= 0 ;
	virtual int getLicenseDetails(std::string strAddOnName, CLicenseDetails &) = 0;
};

typedef int( *GetBusMaterPluginInterface )( IBusmasterPlugin* );