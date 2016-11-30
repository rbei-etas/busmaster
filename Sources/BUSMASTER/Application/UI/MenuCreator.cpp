#include "..\stdafx.h"
#include "Menus.h"
#include "MenuCreator.h"
#include "..\resource.h"
#include "..\MFCRibbonBarEx.h"
#pragma once

#define ID_POPUP_CMD_ID 20000


void MenuCreator::populateRibbonBar(CMFCRibbonBar* mainMenu, std::map<std::string, pluginMenuList> populatelist, IIdGenerator* idgenerator)
{
	mPluginList = populatelist;
	mIdGenerator = idgenerator;
	mMainRibbon = mainMenu;
	if (mPluginList.size() > 0)
	{
		for (auto item : populatelist)
		{
			populateRibbonForPlugin(item.first, item.second, idgenerator);
		}
	}
	for each(auto imageToolBar in mCategortToImage)
	{
		auto category = GetRibbonCategory(imageToolBar.first.c_str());
		if (nullptr != category)
		{
			int lastindex = category->GetLargeImages().GetCount();
			for each(std::string path in imageToolBar.second)
			{
				CImage imageMfc2;
                if (S_OK != imageMfc2.Load(path.c_str()))
                {
                    imageMfc2.LoadFromResource(AfxGetInstanceHandle(), IDB_PLUGIN_IMAGE_MISS);
                }
				lastindex = category->GetLargeImages().AddImage(imageMfc2, lastindex);
			}
		}
	}
}


void MenuCreator::populateRibbonForPlugin(std::string pluginId, pluginMenuList& list, IIdGenerator*  idgenerator)
{
	for (auto pluginMenu : list)
	{
		populateRibbon(pluginId, pluginMenu, idgenerator);
	}
}

void MenuCreator::populateRibbon(std::string pluginId, RibbonButton item, IIdGenerator* idgenerator)
{
	auto category = GetRibbonCategory(item.mCategory, item.mBeforeCategory);
	if (nullptr != category)
	{
		int id = 0;
		auto panel = GetRibbonPanel(category, item.mParentPanel);
        CMFCRibbonButtonEx* ribbonButton = new CMFCRibbonButtonEx();
        ribbonButton->SetParentRibbonBar(mMainRibbon);
		ribbonButton->SetText(item.mName.c_str());
		ribbonButton->SetVisible(TRUE);
		ribbonButton->SetAlwaysLargeImage(TRUE);
		ribbonButton->SetKeys(item.mAcceleratorKey.c_str());
		if ("" != item.mId)
		{
			mIdGenerator->generateId(item.mId, id);
			ribbonButton->SetID(id);

			pluginIdtoMenuId pluginMenuId;
			pluginMenuId.mMenuId = item.mId;
			pluginMenuId.mPluginId = pluginId;
			mMenuIdPluginInfo[id] = pluginMenuId;
		}

		for each (auto button in item.submenulist)
		{
			drawRibbonItem(pluginId, panel, ribbonButton, button);
		}
		int index = -1;
		auto resultIter = mCategortToImage.find(category->GetName());
		if (resultIter == mCategortToImage.end())
		{
			std::vector<std::string> stringList;
            stringList.insert(stringList.end(), item.mImagePath.begin(), item.mImagePath.end());
            index = 0;
			mCategortToImage[category->GetName()] = stringList;
		}
		else
		{
            index = resultIter->second.size();
            resultIter->second.insert(resultIter->second.end(), item.mImagePath.begin(), item.mImagePath.end());
		}
		int currentCount = category->GetLargeImages().GetCount();
        ribbonButton->SetImageIndex(currentCount + index, TRUE);
        ribbonButton->setStartImageIndex(currentCount + index);
        ribbonButton->SetParentRibbonBar(mMainRibbon);
		panel->Add(ribbonButton);
		panel->SetJustifyColumns(FALSE);
	}
}



void MenuCreator::drawRibbonItem(std::string& plugInId, CMFCRibbonPanel* panel, CMFCRibbonButton* btn, RibbonElement& ribbonItem)
{
	switch (ribbonItem.mMenuType)
	{
		case PopUp:
		{
			CMFCRibbonButtonEx* ribbonButton = new CMFCRibbonButtonEx();
            ribbonButton->SetParentRibbonBar(mMainRibbon);
			ribbonButton->SetText(ribbonItem.mName.c_str());
			ribbonButton->SetVisible(TRUE);
			for each(auto item in ribbonItem.submenulist)
			{
				drawRibbonItem(plugInId, panel, ribbonButton, item);
			}
			btn->AddSubItem(ribbonButton);
			break;
		}
		case MenuItem:
		{
			int id = 0;
            CMFCRibbonButtonEx* subItem = new CMFCRibbonButtonEx();
            subItem->SetParentRibbonBar(mMainRibbon);
			subItem->SetText(ribbonItem.mName.c_str());
			mIdGenerator->generateId(ribbonItem.mId, id);
			btn->AddSubItem(subItem);


			subItem->SetID(id);

			pluginIdtoMenuId pluginMenuId;
			pluginMenuId.mMenuId = ribbonItem.mId;
			pluginMenuId.mPluginId = plugInId;
			mMenuIdPluginInfo[id] = pluginMenuId;
			
			break;
		}
		case Separator:
		{
			CMFCRibbonSeparator* subItem = new CMFCRibbonSeparator(TRUE);
			btn->AddSubItem(subItem);
		}
	}
}
int MenuCreator::GetRibbonCategoryIndex(std::string name)
{
    int categoryCount = mMainRibbon->GetCategoryCount();
    for (int i = 0; i < categoryCount; i++)
    {
        CMFCRibbonCategory* category = mMainRibbon->GetCategory(i);
        if (nullptr != category)
        {
            if (name == category->GetName())
            {
                return i;
            }
        }
    }
    return -1;
}
CMFCRibbonCategory* MenuCreator::GetRibbonCategory(std::string name, std::string beforeCategory)
{
	int categoryCount = mMainRibbon->GetCategoryCount();
	for (int i = 0; i < categoryCount; i++)
	{   
		CMFCRibbonCategory* category = mMainRibbon->GetCategory(i);
		if (nullptr != category)
		{
			if (name == category->GetName())
			{
				return category;
			}
		}
	}
    return mMainRibbon->AddCategory(name.c_str(), IDB_TEMP_BITMAP_SMALL, IDB_TEMP_BITMAP, CSize(16, 16), CSize(32, 32), GetRibbonCategoryIndex(beforeCategory));
}

CMFCRibbonPanel* MenuCreator::GetRibbonPanel(CMFCRibbonCategory* category, std::string& panelName)
{
	//auto panel = category->AddPanel("Hello");
	//return panel;
	int panelCount = category->GetPanelCount();
	for (int i = 0; i < panelCount; i++)
	{
		auto panel = category->GetPanel(i);
		if (nullptr != panel)
		{
			auto name = panel->GetName();
			if ( name == panelName)
			{
				return panel;
			}
		}
	}
	//CMFCRibbonPanel* oldPanel = category->GetPanel(0);
	CMFCRibbonPanel* newPanel = category->AddPanel(panelName.c_str());

	return newPanel;
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
