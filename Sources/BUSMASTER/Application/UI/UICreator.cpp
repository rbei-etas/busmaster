#include "stdafx.h"
#include "UICreator.h"
#include "MenuCreator.h"
#include "IdGenerator.h"
//#include "ToolBarCreator.h"

UICreator::~UICreator()
{
    if (nullptr != mMenucreator)
    {
        delete mMenucreator;
    }
}

UICreator::UICreator(UIElements uielements, IIdGenerator* idgenerator)
{
	mUiElements.mRibbonBar = uielements.mRibbonBar;
    mMenucreator = new MenuCreator();
    mIdgenerator = idgenerator;
}

int UICreator::Create(std::map<std::string, pluginMenuList> populatelist)
{
    mPluginMenuList = populatelist;
	mMenucreator->populateRibbonBar(mUiElements.mRibbonBar, mPluginMenuList, mIdgenerator);
    return 1;
}

int UICreator::getPluginMenuInfo(unsigned int id, std::string& pluginId, std::string& actualMenuId)
{
    mMenucreator->getPluginMenuInfo(id, pluginId, actualMenuId);
    return 0;
}
