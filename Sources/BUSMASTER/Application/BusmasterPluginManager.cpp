#include "stdafx.h"
#include "BusmasterPluginManager.h"
#include "UI/MenuCreator.h"
#include "UI/IdGenerator.h"
#include "UI/BusmasterMenuItem.h"
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include <libxml/xmlschemastypes.h>
#include "Utility/XMLUtils.h"
#include "Utility/Utility.h"
#define GET_BUSMASTER_PLUGIN_INTERFACE      "GetPlugInInterface"
#define FREE_PLUGIN_INTERFCE                "FreePlugInInterface"
#define defPLUGINEXTENSION                 "*.xml"
#define defDEFAULTPLUGINFOLDER              "BusmasterPlugins"
#define defSCHEMAFILENAME                   "BusMasterPluginSchema.xsd"

#define nodepath "//MenuConfiguration/parent"




typedef int (*GetPlugInInterface)(IBusmasterPlugin**);
typedef void (*FreePlugInInterface)(IBusmasterPlugin*);

BusmasterPluginManager::BusmasterPluginManager()
{
    mMenuCreator = nullptr;
    mIdgenerator = new IdGenerator();
    mIdgenerator->id = mIdgenerator->setStartId(20000);

    //SchemaPath
    std::string appPath;
    GetBusmasterInstalledFolder(appPath);
    char xsdFilePath[2 * MAX_PATH];
    PathCombine(xsdFilePath, appPath.c_str(), defSCHEMAFILENAME);
    mXsdSchemaPath = xsdFilePath;
}


BusmasterPluginManager::~BusmasterPluginManager()
{
    if (nullptr != mIdgenerator)
    {
        delete mIdgenerator;
    }
    if (nullptr != mMenuCreator)
    {
        delete mMenuCreator;
    }
    if (nullptr != mUiCreator)
    {
        delete mUiCreator;
    }
}


int BusmasterPluginManager::init( IBusmasterPluginInterface* busmasterInterface)
{
    mBusmasterInterface = busmasterInterface;

    //createPluginMenuMap();
    return S_OK;
}
int BusmasterPluginManager::loadPlugin( const char* pluginFilePath )
{
    xmlNodeSetPtr nodeSet;
    xmlXPathObjectPtr xmlpathptr;
    xmlDocPtr xmlDocptr;
    std::string  xmlfile = pluginFilePath;
    Menu*  menu = nullptr;
    xmlDocptr = xmlReadFile(xmlfile.c_str(), "utf-8", 0);

    if (nullptr == xmlDocptr)
    {
        return S_FALSE;
    }

    if (PLUGIN_ERR_LOAD_SUCCESS != isValidateBusmasterPluginXml(xmlDocptr))
    {
        return S_FALSE;
    }

    std::list<PluginMenu> pluginmenus;

    BusmasterPluginInfo busMasterPlugin;
    busMasterPlugin.mPluginId = GetNodeValue(xmlDocptr, (xmlChar*) "//BusMasterPlugin/id", "pluginId");
    busMasterPlugin.mDescription = GetNodeValue(xmlDocptr, (xmlChar*) "//BusMasterPlugin/description", "description");
    busMasterPlugin.mPluginDllpath = GetNodeValue(xmlDocptr, (xmlChar*) "//BusMasterPlugin/dllpath", "plugindllpath");

    auto tempPath = CheckforRelativePath((char*)busMasterPlugin.mPluginDllpath.c_str(), xmlfile);
    busMasterPlugin.mPluginDllpath = tempPath;
    busMasterPlugin.mMinBusMasterVer = GetNodeValue(xmlDocptr, (xmlChar*)"//BusMasterPlugin/minBusMasterVer", "minBusMasterVer");

    delete[]tempPath;

    xmlChar* pchPath = (xmlChar*)nodepath;
    xmlpathptr = xmlUtils::pGetNodes(xmlDocptr, pchPath);
    if (xmlpathptr != nullptr)
    {
        nodeSet = xmlpathptr->nodesetval;

        for (int cmdindex = 0; cmdindex < nodeSet->nodeNr; cmdindex++)
        {
            PluginMenu pluginMenu;
            pluginMenu.mParentMenu = (char*)xmlGetProp(nodeSet->nodeTab[cmdindex], (xmlChar*)"name");
            pluginMenu.mBeforeMenu = (char*)xmlGetProp(nodeSet->nodeTab[cmdindex], (xmlChar*)"beforemenu");
            CreateList(nodeSet->nodeTab[cmdindex], &pluginMenu, menu, xmlfile);
            pluginmenus.push_back(pluginMenu);
        }

        busMasterPlugin.menuList = pluginmenus;
    }
    xmlXPathFreeObject(xmlpathptr);
    xmlFreeDoc(xmlDocptr);

    //Add the plugin-Info irresepective of the return value
    loadPluginInformation(busMasterPlugin);

    mPluginList.push_back(busMasterPlugin);
    return S_OK;
}

int BusmasterPluginManager::loadPluginInformation(BusmasterPluginInfo& pluginInfo)
{
    pluginInfo.mDllHandle = LoadLibrary(pluginInfo.mPluginDllpath.c_str());
    if (nullptr == pluginInfo.mDllHandle)
    {
        pluginInfo.mPluginLoadState = PLUGIN_ERR_DLL_NOT_FOUND;
    }
    else
    {
        GetPlugInInterface fpluginInterface = (GetPlugInInterface)GetProcAddress(pluginInfo.mDllHandle, GET_BUSMASTER_PLUGIN_INTERFACE);

        //Not a BUSMASTER Plugin
        if (fpluginInterface == nullptr)
        {
            pluginInfo.mPluginLoadState = PLUGIN_ERR_INVALID_INTERFACE;
        }
        else
        {
            fpluginInterface(&pluginInfo.mPluginInterface);
            if (nullptr != pluginInfo.mPluginInterface)
            {
                pluginInfo.mPluginInterface->setBusmasterInterface(mBusmasterInterface);
                pluginInfo.mPluginInterface->getNotifySink(&pluginInfo.mNotifyEvent);
                pluginInfo.mPluginInterface->getMenuInterface(&pluginInfo.mMenuInterface);
                pluginInfo.mPluginLoadState = PLUGIN_ERR_LOAD_SUCCESS;
            }
            else
            {
                pluginInfo.mPluginLoadState = PLUGIN_ERR_INVALID_INTERFACE;
            }
        }
    }
    return pluginInfo.mPluginLoadState;
}
void BusmasterPluginManager::CreateList(xmlNodePtr& nodeptr, PluginMenu* pluginmenu, Menu* menu, std::string xmlFilePath)
{
    xmlNodePtr childnodeptr = nodeptr->children;
    while (childnodeptr != NULL)
    {
        // If Command under Parent
        if (!xmlStrcmp(childnodeptr->name, (const xmlChar*)"command"))
        {
            Menu submenu;
            submenu = CreateCommandList(childnodeptr, xmlFilePath);
            pluginmenu->menuList.push_back(submenu);
        }
        // If Command Group under Parent
        else if (!xmlStrcmp(childnodeptr->name, (const xmlChar*)"commandgroup"))
        {
            Menu menu;
            menu = CreateCommandGroupList(childnodeptr, xmlFilePath);
            pluginmenu->menuList.push_back(menu);
        }
        //if seperator under Parent
        else if (!xmlStrcmp(childnodeptr->name, (const xmlChar*)"seperator"))
        {
            Menu submenu;
            submenu.mMenuType = eMenuTypes::Separator;
            pluginmenu->menuList.push_back(submenu);
        }
        childnodeptr = childnodeptr->next;
    }
}

Menu BusmasterPluginManager::CreateCommandGroupList(xmlNodePtr& childnodeptr, std::string xmlFilePath)
{
    Menu popmenu;
    xmlChar* chName;
    chName = xmlGetProp(childnodeptr, (xmlChar*)"name");
    popmenu.mName = (char*)chName;
    xmlFree(chName);
    popmenu.mMenuType = eMenuTypes::PopUp;
    xmlNodePtr subchildnodeptr = childnodeptr->children;
    while (subchildnodeptr != NULL)
    {

        //If Command under Command Group
        if (!xmlStrcmp(subchildnodeptr->name, (const xmlChar*)"command"))
        {
            Menu submenu;
            submenu = CreateCommandList(subchildnodeptr, xmlFilePath);
            popmenu.submenulist.push_back(submenu);

        }
        //if seperator under Command Group
        else if (!xmlStrcmp(subchildnodeptr->name, (const xmlChar*)"seperator"))
        {
            Menu submenu;
            submenu.mMenuType = eMenuTypes::Separator;
            popmenu.submenulist.push_back(submenu);
        }
        // a commandgroup under commandgroup
        else if (!xmlStrcmp(subchildnodeptr->name, (const xmlChar*)"commandgroup"))
        {
            Menu menu = CreateCommandGroupList(subchildnodeptr, xmlFilePath);
            popmenu.submenulist.push_back(menu);
        }
        subchildnodeptr = subchildnodeptr->next;
    }
    return popmenu;
}

Menu BusmasterPluginManager::CreateCommandList(xmlNodePtr& childnodeptr, std::string xmlFilePath)
{
    Menu submenu;
    xmlChar* cName, *cId, *cToolTip, *cEnabled, *cDisabled, *cHot;
    cName = xmlGetProp(childnodeptr, (xmlChar*)"name");
    submenu.mName = (char*)cName;
    xmlFree(cName);
    cId = xmlGetProp(childnodeptr, (xmlChar*)"id");
    submenu.mId = (char*)cId;
    xmlFree(cId);
    submenu.mMenuType = eMenuTypes::MenuItem;
    auto tooltipptr = (char*)xmlHasProp(childnodeptr, (xmlChar*)"tooltip");
    if (tooltipptr != NULL)
    {
        submenu.isToolTipReq = true;
    }
    if (submenu.isToolTipReq)
    {
        cToolTip = xmlGetProp(childnodeptr, (xmlChar*)"tooltip");
        submenu.tooltip = (char*)cToolTip;
        xmlFree(cToolTip);
    }

    xmlNodePtr toolbutton = childnodeptr->children;
    while (toolbutton != NULL)
    {
        char* abspathptr;
        std::string pathvalue;
        if ((!xmlStrcmp(toolbutton->name, (const xmlChar*)"toolbutton")))
        {
            submenu.mToolbar.isToolButtonReq = true;
            cEnabled = xmlGetProp(toolbutton, (xmlChar*)"enabledpath");
            abspathptr = CheckforRelativePath((char*)cEnabled, xmlFilePath);
            submenu.mToolbar.iconPathEnable = abspathptr;
            delete[]abspathptr;
            xmlFree(cEnabled);
            cDisabled = xmlGetProp(toolbutton, (xmlChar*)"disabledpath");
            abspathptr = CheckforRelativePath((char*)cDisabled, xmlFilePath);
            submenu.mToolbar.iconPathDisable = abspathptr;
            delete[]abspathptr;
            xmlFree(cDisabled);
            cHot = xmlGetProp(toolbutton, (xmlChar*)"hotpath");
            abspathptr = CheckforRelativePath((char*)cHot, xmlFilePath);
            submenu.mToolbar.iconPathHot = abspathptr;
            delete[]abspathptr;
            xmlFree(cHot);
            break;
        }
        toolbutton = childnodeptr->children->next;
    }
    return submenu;
}
char* BusmasterPluginManager::CheckforRelativePath(char* revpath, std::string xmlFilePath)
{
    char* returnPath = new char[1024];
    //returnPath = "";
    bool relativepath = false;
    relativepath = PathIsRelative(revpath);
    if (revpath != nullptr && strlen(revpath) !=0)
    {
        if (relativepath)
        {
            PathRemoveFileSpec((LPSTR)xmlFilePath.c_str());
            PathCombine(returnPath, (LPSTR)xmlFilePath.c_str(), revpath);
            return returnPath;
        }
        strncpy_s(returnPath, strlen(revpath) + 1, revpath, 1024);
    }
    return returnPath;
}

std::string BusmasterPluginManager::GetNodeValue(xmlDocPtr docptr, xmlChar* pchXpath, char* nodename)
{
    xmlXPathObjectPtr xmlpath;
    xmlChar* strData;
    xmlpath = xmlUtils::pGetNodes(docptr, pchXpath);
    xmlChar* xmlNodeName = (xmlChar*)nodename;
    xmlNodeSetPtr nodeptr;
    std::string data;
    nodeptr = xmlpath->nodesetval;
    if (nodeptr != nullptr)
    {
        strData = xmlNodeGetContent(nodeptr->nodeTab[0]);
        data = (char*)strData;
        xmlFree(strData);
    }
    xmlXPathFreeObject(xmlpath);
    return data;
}


int BusmasterPluginManager::loadPlugins( const char* dir )
{
    char filePath[MAX_PATH];
    std::string pluginPath;
    if ( dir == NULL )
    {
        char tempPluginpath[MAX_PATH];
        GetModuleFileName( NULL, filePath, MAX_PATH );
        PathRemoveFileSpec( filePath );
        //filePath = filePath + defDEFAULTPLUGINFOLDER;
        PathCombine( tempPluginpath, filePath, defDEFAULTPLUGINFOLDER );
        pluginPath = tempPluginpath;
    }
    else
    {
        pluginPath = dir;
    }
    if ( PathFileExists( pluginPath.c_str() ) == TRUE )
    {
        SetCurrentDirectory( pluginPath.c_str() );

        CFileFind omFileFinder;
        CString strWildCard = defPLUGINEXTENSION; //look for the plugin files

        BOOL bWorking = omFileFinder.FindFile( strWildCard );
        while ( bWorking )
        {
            bWorking = omFileFinder.FindNextFile();
            if ( omFileFinder.IsDots() || omFileFinder.IsDirectory() )
            {
                continue;
            }
            loadPlugin( omFileFinder.GetFilePath() );
        }
        return S_OK;
    }
    return S_FALSE;
}
int BusmasterPluginManager::drawUI(UIElements uielements)
{

    std::map<std::string, pluginMenuList> pluginMenuList;
for ( auto info : mPluginList )
    {
        pluginMenuList[info.mPluginId] = info.menuList;
    }

    if (nullptr != mUiCreator)
    {
        delete mUiCreator;
        mUiCreator = nullptr;
    }
    mUiCreator = new UICreator(uielements, mIdgenerator);
    mUiCreator->Create(pluginMenuList);

    return S_OK;
}
int BusmasterPluginManager::notifyPlugins( eBusmaster_Event eventType, void* eventData)
{
for ( auto plugin : mPluginList )
    {
        if (nullptr != plugin.mNotifyEvent)
        {
            plugin.mNotifyEvent->onBusmasterEvent(eventType, eventData);
        }
    }
    return S_FALSE;
}
int BusmasterPluginManager::noifyMenuClick( int menuId )
{
    int retVal = S_FALSE;
    BusmasterPluginInfo pluginInfo;
    std::string pluginId;
    if(true == getPluginForMenuId( menuId, pluginInfo, pluginId ))
    {
        if ( nullptr != pluginInfo.mMenuInterface )
        {
            pluginInfo.mMenuInterface->OnMenuItemClicked( pluginId.c_str() );
            retVal = S_OK;
        }
    }
    return retVal;
}
int BusmasterPluginManager::noifyMenuUpdate(int menuId, IMenuItem* menuItem)
{
    BusmasterPluginInfo pluginInfo;
    std::string pluginId;
    if (true == getPluginForMenuId(menuId, pluginInfo, pluginId))
    {
        if (nullptr != pluginInfo.mMenuInterface)
        {
            pluginInfo.mMenuInterface->updateMenuItem(pluginId.c_str(), menuItem);
            return S_OK;
        }
    }
    return S_FALSE;
}
int BusmasterPluginManager::unLoadPlugins()
{
for (auto plugin : mPluginList)
    {
        unLoadPlugin(plugin);
    }
    mPluginList.clear();
    return S_FALSE;
}
int BusmasterPluginManager::unLoadPlugin(BusmasterPluginInfo& plugin)
{
    eBusmaster_Event eventType = eBusmaster_Event::busmaster_exit;
    if (nullptr != plugin.mNotifyEvent)
    {
        //plugin.mNotifyEvent->onBusmasterEvent(eventType, nullptr);
    }
    if (nullptr != plugin.mDllHandle)
    {
        auto freePluginInterface = (FreePlugInInterface)GetProcAddress(plugin.mDllHandle, FREE_PLUGIN_INTERFCE);
        if (nullptr != freePluginInterface)
        {
            freePluginInterface(plugin.mPluginInterface);
        }

        FreeLibrary(plugin.mDllHandle);

        plugin.mPluginInterface = nullptr;
        plugin.mMenuInterface = nullptr;
        plugin.mNotifyEvent = nullptr;
        plugin.mDllHandle = nullptr;
        plugin.menuList.clear();
    }
    plugin.mPluginLoadState = PLUGIN_ERR_INVALID_LOAD_STATE;

    return S_OK;
}
int BusmasterPluginManager::getPluginCount()
{
    return S_FALSE;
}
int BusmasterPluginManager::getPluginConfiguration(xmlNodePtr& parentNode)
{
for (auto plugin : mPluginList)
    {
        if(nullptr != plugin.mPluginInterface)
        {
            xmlNodePtr pluginNodePtr = nullptr;
            plugin.mPluginInterface->getConfiguration(pluginNodePtr);
            xmlAddChild(parentNode, pluginNodePtr);
        }
    }
    return 0;
}
int BusmasterPluginManager::setPluginConfiguration(const xmlDocPtr configData)
{
for (auto plugin : mPluginList)
    {
        if(nullptr != plugin.mPluginInterface)
        {
            plugin.mPluginInterface->setConfiguration(configData);
        }
    }
    return 0;
}
IMenuCreator* BusmasterPluginManager::getMenuCreator()
{
    if ( nullptr == mMenuCreator )
    {
        mMenuCreator = new MenuCreator();
    }
    return mMenuCreator;
}

bool BusmasterPluginManager::getPluginForMenuId( int menuId, BusmasterPluginInfo& pluginInfo, std::string& pluginActualId )
{
    bool found = false;
    if (getMenuCreator() != nullptr && nullptr != mUiCreator)
    {
        std::string id;
        mUiCreator->getPluginMenuInfo(menuId, id, pluginActualId);
for ( auto plugin : mPluginList )
        {
            if ( id == plugin.mPluginId )
            {
                pluginInfo = plugin;
                found = true;
                break;
            }
        }
    }
    return found;
}

IBusmasterPluginConnection* BusmasterPluginManager::getPluginConnectionPoint(const char* id)
{
    IBusmasterPluginConnection* pluginConnection = nullptr;
for (auto plugin : mPluginList)
    {
        if (plugin.mPluginId == id && nullptr != plugin.mPluginInterface )
        {
            plugin.mPluginInterface->getConnectPoint(&pluginConnection);
            break;
        }
    }
    return pluginConnection;
}


int BusmasterPluginManager::isValidateBusmasterPluginXml(xmlDocPtr pluginDocPtr)
{
    if (pluginDocPtr == nullptr)
    {
        return PLUGIN_ERR_INVALID_XML;
    }
    int isValid = PLUGIN_ERR_INVALID_XML;

    xmlSchemaPtr schemaPtr = nullptr;
    xmlSchemaParserCtxtPtr ctxtParser;
    xmlSchemaValidCtxtPtr ctxtSchema;

    xmlLineNumbersDefault(1);
    ctxtParser = xmlSchemaNewParserCtxt(mXsdSchemaPath.c_str());
    if (nullptr == ctxtParser)
    {
        return PLUGIN_ERR_SCHEMA_FILE_NOT_FOUND;
    }

    xmlSchemaSetParserErrors(ctxtParser, (xmlSchemaValidityErrorFunc)fprintf, (xmlSchemaValidityWarningFunc)fprintf, stderr);
    schemaPtr = xmlSchemaParse(ctxtParser);
    xmlSchemaFreeParserCtxt(ctxtParser);


    int ret;

    ctxtSchema = xmlSchemaNewValidCtxt(schemaPtr);
    xmlSchemaSetValidErrors(ctxtSchema, (xmlSchemaValidityErrorFunc)fprintf, (xmlSchemaValidityWarningFunc)fprintf, stderr);
    ret = xmlSchemaValidateDoc(ctxtSchema, pluginDocPtr);
    if (ret == 0)
    {
        isValid = PLUGIN_ERR_LOAD_SUCCESS;
    }

    xmlSchemaFreeValidCtxt(ctxtSchema);


    if (schemaPtr != nullptr)
    {
        xmlSchemaFree(schemaPtr);
    }

    xmlCleanupParser();
    xmlSchemaCleanupTypes();
    xmlMemoryDump();

    return isValid;
}