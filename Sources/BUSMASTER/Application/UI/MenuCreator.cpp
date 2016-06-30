#include "..\stdafx.h"
#include "Menus.h"
#include "MenuCreator.h"

#pragma once

#define ID_POPUP_CMD_ID 20000

void MenuCreator::populateMenuList(CMenu* mainMenu, std::map<std::string, pluginMenuList> populatelist, IIdGenerator* idgenerator)
{

    mPluginList = populatelist;
    if (mPluginList.size() > 0)
    {
        //int count = mainMenu->GetMenuItemCount();
        mMainMenu = mainMenu;
for (auto item : populatelist)
        {
            populatePluginMenuList(item.first, item.second, idgenerator);
        }
    }

}

void MenuCreator::getPluginMenuInfo(unsigned int id, std::string& pluginId, std::string& actualMenuId)
{
    auto itr = mMenuIdPluginInfo.find(id);
    if (itr != mMenuIdPluginInfo.end())
    {
        pluginId = itr->second.mPluginId;
        actualMenuId = itr->second.mMenuId;
    }
}
void MenuCreator::populatePluginMenuList(std::string pluginId, pluginMenuList& list, IIdGenerator*  idgenerator)
{
for (auto pluginMenu : list)
    {
        populatePluginMenu(pluginId, pluginMenu, idgenerator);
    }
}
void MenuCreator::populatePluginMenu(std::string pluginId, PluginMenu item, IIdGenerator* idgenerator)
{
    int index = getParentMenuIndex(item.mParentMenu);
    if (-1 != index)
    {
        CMenu* menu = mMainMenu->GetSubMenu(index); //TODO::nullpointer check
        INT COUNT = menu->GetMenuItemCount();
for (auto itemMenu : item.menuList)
        {
            populateMenu(pluginId, menu, itemMenu, idgenerator);
        }
        COUNT = menu->GetMenuItemCount();
        CMenu* subMenu = menu->GetSubMenu(19);
        // COUNT = subMenu->GetMenuItemCount();
        COUNT++;
    }
}

int MenuCreator::getParentMenuIndex(std::string name)
{
    if (name == "busmaster.commandgroup.can")
    {
        return 1;
    }
    else if (name == "busmaster.commandgroup.flexray")
    {
        return 3;
    }
    return -1;
}

void MenuCreator::populateMenu(std::string pluginId, CMenu* menu, Menu itemMenu, IIdGenerator* idgenerator)
{

    switch (itemMenu.mMenuType)
    {
        case PopUp:
        {
            CMenu* popupmenu = new CMenu();
            popupmenu->CreatePopupMenu();
for (auto subitem : itemMenu.submenulist)
            {
                populateMenu(pluginId, popupmenu, subitem, idgenerator);
            }
            int submenucount = popupmenu->GetMenuItemCount();
            menu->AppendMenuA(MF_POPUP, (UINT)popupmenu->GetSafeHmenu(), itemMenu.mName.c_str());
            break;
        }


        case MenuItem:
        {
            //int id;
            idgenerator->generateId(itemMenu.mId, idgenerator->id);
            pluginIdtoMenuId pluginMenuId;
            pluginMenuId.mMenuId = itemMenu.mId;
            pluginMenuId.mPluginId = pluginId;
            mMenuIdPluginInfo[idgenerator->id] = pluginMenuId;
            menu->AppendMenu(MF_BYCOMMAND, idgenerator->id, itemMenu.mName.c_str());
            menu->EnableMenuItem(0, MF_ENABLED);
            break;
        }

        case Separator:
        {
            menu->InsertMenu(menu->GetMenuItemCount(), MF_BYPOSITION | MF_SEPARATOR);
            break;
        }

    }

}

