#pragma once
#include "IBusmasterPlugInManager.h"
#include <list>
#include "resource.h"
#include "UI\MenuCreator.h"
#include "UI\IIdGenerator.h"
#include "UI\UICreator.h"
#include <concurrent_vector.h>





class BusmasterPluginManager : public IBusmasterPluginManager
{
    bool mUnloadPlugins = false;
    IBusmasterBusPluginInterface* mBusmasterInterface;
	std::map<std::string, CLicenseDetails> mMapLicenseDetails;
    Concurrency::concurrent_vector<BusmasterPluginConfiguration> mPluginList;
    Concurrency::concurrent_vector<BusmasterBusPluginConfiguration> mBusPluginList;
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
    int init(IBusmasterBusPluginInterface*);
	int loadBusPlugins(const char* dir);
	int loadBusPlugin(const char* pluginFilePath);
    int loadPlugin( const char* pluginFilePath );
    int loadPlugins( const char* dir );
	int addPlugin(BusmasterPluginConfiguration& pluginInfo);
    int drawUI(UIElements uielements) ;
    int notifyPlugins( eBusmaster_Event, void* );
    int noifyMenuClick( int menuId );
	int notifyAppClose();
	int getLicenseDetails(std::string strAddOnName, CLicenseDetails &obj);
    int noifyMenuUpdate(int menuId, IMenuItem* menuItem);
    int unLoadPlugins();
    int getPluginCount();
    int getPluginConfiguration(xmlNodePtr&);
    int setPluginConfiguration(const xmlDocPtr);
    bool getPluginForMenuId(int, BaseBusmasterPlugin&, std::string& plugin);
    IBusmasterPluginConnection* getPluginConnectionPoint(const char* id);
private:
    int parsePluginInformation(const char* pluginFile, BaseBusmasterPlugin& pluginInfo);
    int loadPluginInformation(BusmasterPluginConfiguration& pluginInfo);
	int loadBusPluginInformation(BusmasterBusPluginConfiguration& pluginInfo);
	
    std::string BusmasterPluginManager::GetNodeValue(xmlDocPtr docptr, xmlChar* pchXpath, char* nodename);
	void ParseMenuItem(xmlNodePtr, RibbonButton&, std::string&);
    void CreateList(xmlNodePtr& nodeptr, RibbonButton* pluginmenu, std::string xmlFilePath);
	RibbonElement CreateCommandList(xmlNodePtr& childnodeptr, std::string xmlFilePath);
    char* CheckforRelativePath(const char* revpath, std::string xmlFilePath);
	RibbonElement CreateCommandGroupList(xmlNodePtr& childnodeptr, std::string xmlFilePath);
    int isValidateBusmasterPluginXml(xmlDocPtr pluginDocPtr);
    int unLoadPlugin(BusmasterPluginConfiguration&);
    void parseIcons(xmlNodePtr, RibbonButton& button, bool isLarge, std::string& xmlFilePath);
};

