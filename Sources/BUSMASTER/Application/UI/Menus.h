#pragma once
#include<iostream>
#include<list>


enum eMenuTypes
{
    PopUp,
    MenuItem,
    Separator
};

class ToolButton
{
public:
    bool isToolButtonReq = false;
    std::string iconPathEnable;
    std::string iconPathDisable;
    std::string iconPathHot;
    /*int toolButtonID;
    std::list<ToolButton> toolbuttonlist;
    */

};

class Menu
{
public:
    eMenuTypes mMenuType;
    std::string mId;
    bool isToolTipReq = false;
    std::string mName;
    std::string tooltip;
    std::list<Menu> submenulist;
    ToolButton mToolbar;
};

class PluginMenu
{
public:
    std::string mParentMenu;
    std::string mBeforeMenu;
    std::list<Menu> menuList;
};