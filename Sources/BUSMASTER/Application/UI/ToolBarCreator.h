#pragma once
#include <string>
#include <map>
#include"Menus.h"
#include "IIdGenerator.h"
#define DEFAULT_IMAGE_PATH "D:\\BusMasterPlugin\\Sources\\UIcreator\\images\\ERROR_ACTIVE .bmp"



class ToolBarCreator
{
private:
    void populatePluginToolbarList(std::map <std::string, CToolBar*> wndtoolbar, std::list<PluginMenu>& list, IIdGenerator* idgenerator);
    void populatePluginToolbar(std::map <std::string, CToolBar*> wndtoolbar, PluginMenu item, IIdGenerator* idgenerator);
    void populate(std::map <std::string, CToolBar*> wndtoolbar, Menu itemmenu, std::string parentmenu, IIdGenerator* idgenerator);
    CImageList  m_imageList;
    bool FileExist(std::string );
    int index=1;
public:
    ToolBarCreator();
    ~ToolBarCreator();
    void populateToolBar(std::map<std::string, CToolBar*>wndtoolbar, std::map<std::string, std::list<PluginMenu>> populatelist, IIdGenerator* idgenerator);
};

