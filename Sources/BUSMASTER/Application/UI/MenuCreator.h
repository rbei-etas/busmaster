#pragma once
#include<iostream>
#include <list>
#include "Menus.h"
#include "IMenuCreator.h"
#include <string>
#include <map>
#include "IIdGenerator.h"
struct pluginIdtoMenuId
{
    std::string mPluginId;
    std::string mMenuId;
};

class MenuCreator : public IMenuCreator
{
	CMFCRibbonBar* mMainRibbon;
	IIdGenerator* mIdGenerator;
    std::map < int, pluginIdtoMenuId> mMenuIdPluginInfo;
    std::map<std::string, pluginMenuList> mPluginList;
	std::map<std::string, std::vector<std::string>> mCategortToImage;
public:
	void populateRibbonBar(CMFCRibbonBar* mainMenu, std::map<std::string, pluginMenuList> menulist, IIdGenerator* idgenerator);
    void getPluginMenuInfo(unsigned int id, std::string& pluginId, std::string& actualMenuId);

private:
	void populateRibbon(std::string pluginId, RibbonButton menu, IIdGenerator* idgenerator);
	void populateRibbonForPlugin(std::string pluginId, pluginMenuList&, IIdGenerator* idgenerator);

	void drawRibbonItem(std::string& plugInId, CMFCRibbonPanel* panel, CMFCRibbonButton* btn, RibbonElement& ribbonItem);
	CMFCRibbonPanel* GetRibbonPanel(CMFCRibbonCategory* category, std::string& panelName);
    CMFCRibbonCategory* GetRibbonCategory(std::string name, std::string beforCategory= "");
    int GetRibbonCategoryIndex(std::string name);
};

