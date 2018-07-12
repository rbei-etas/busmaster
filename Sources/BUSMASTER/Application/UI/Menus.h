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
};

class RibbonElement
{
public:
    eMenuTypes mMenuType;
    std::string mId;
	std::string mName;
    std::string tooltip;
	std::list<RibbonElement> submenulist;
	std::string mAcceleratorKey;

	RibbonElement()
    {
    }
};

class RibbonButton
{
public:
	std::string mCategory;
	std::string mName;
	std::string mId;
	std::string mParentPanel;
	
    std::vector<std::string> mImagePath;
    std::vector<std::string> mSmallImagePath;
    int mImageIndex;

	std::string mBeforeCategory;
	std::string mAcceleratorKey;
	std::list<RibbonElement> submenulist;
};
