#pragma once
#include "IBusmasterPlugInManager.h"
#include <list>
#include "resource.h"
#include "UI\MenuCreator.h"
#include "UI\IIdGenerator.h"
#include "UI\UICreator.h"

#define PLUGIN_ERR_LOAD_SUCCESS             0x0
#define PLUGIN_ERR_INVALID_LOAD_STATE       0x800000
#define PLUGIN_ERR_INVALID_XML              0x800001
#define PLUGIN_ERR_SCHEMA_FILE_NOT_FOUND    0x800002
#define PLUGIN_ERR_DLL_NOT_FOUND            0x800003
#define PLUGIN_ERR_INVALID_INTERFACE        0x800004
#define PLUGIN_ERR_INVALID_LICENCE          0x800005


struct BusmasterPluginInfo
{
    IBusmasterPlugin* mPluginInterface = nullptr;
    IMenuInterface* mMenuInterface = nullptr;
    IEvent* mNotifyEvent = nullptr;
    HMODULE mDllHandle = nullptr;
    pluginMenuList menuList;
    std::string mPluginId = "";
    std::string mDescription ="";
    std::string mPluginDllpath ="";
    std::string mMinBusMasterVer ="";
    int mPluginLoadState = PLUGIN_ERR_INVALID_LOAD_STATE;
};

class BusmasterPluginManager : public IBusmasterPluginManager
{
    IBusmasterPluginInterface* mBusmasterInterface;
    std::vector<BusmasterPluginInfo> mPluginList;
    IMenuCreator* mMenuCreator = nullptr;
    IIdGenerator* mIdgenerator = nullptr;
    IUICreator* mUiCreator = nullptr;
    std::string mXsdSchemaPath;
protected:
    virtual IMenuCreator* getMenuCreator();
public:
    BusmasterPluginManager();
    virtual ~BusmasterPluginManager();
public:
    int init( IBusmasterPluginInterface* );
    int loadPlugin( const char* pluginFilePath );
    int loadPlugins( const char* dir );
    int drawUI(UIElements uielements) ;
    int notifyPlugins( eBusmaster_Event, void* );
    int noifyMenuClick( int menuId );
    int noifyMenuUpdate(int menuId, IMenuItem* menuItem);
    int unLoadPlugins();
    int getPluginCount();
    int getPluginConfiguration(xmlNodePtr&);
    int setPluginConfiguration(const xmlDocPtr);
    bool getPluginForMenuId(int, BusmasterPluginInfo&, std::string& plugin);
    IBusmasterPluginConnection* getPluginConnectionPoint(const char* id);
private:
    int loadPluginInformation(BusmasterPluginInfo& pluginInfo);

    std::string BusmasterPluginManager::GetNodeValue(xmlDocPtr docptr, xmlChar* pchXpath, char* nodename);
    void BusmasterPluginManager::CreateList(xmlNodePtr& nodeptr, PluginMenu* pluginmenu, Menu* menu, std::string xmlFilePath);
    Menu CreateCommandList(xmlNodePtr& childnodeptr, std::string xmlFilePath);
    char* CheckforRelativePath(char* revpath, std::string xmlFilePath);
    Menu CreateCommandGroupList(xmlNodePtr& childnodeptr, std::string xmlFilePath);
    int isValidateBusmasterPluginXml(xmlDocPtr pluginDocPtr);
    int unLoadPlugin(BusmasterPluginInfo&);
};

