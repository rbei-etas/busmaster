#include "stdafx.h"
#include "ToolBarCreator.h"
#include <map>
#include <string>
#include<fstream>
#include <afxwin.h>
#include<afxext.h>
#include<filesystem>


ToolBarCreator::ToolBarCreator() {}

ToolBarCreator::~ToolBarCreator() {}

void ToolBarCreator::populateToolBar(std::map <std::string, CToolBar*> wndtoolbar, std::map<std::string, std::list<PluginMenu>> populatelist, IIdGenerator* idgenerator)
{

    if (populatelist.size() > 0)
    {
for (auto item : populatelist)
        {
            populatePluginToolbarList(wndtoolbar, item.second, idgenerator);
        }
    }
}

void ToolBarCreator::populatePluginToolbarList(std::map <std::string, CToolBar*> wndtoolbar, std::list<PluginMenu>& list, IIdGenerator* idgenerator)
{
for (auto pluginMenu : list)
    {
        populatePluginToolbar(wndtoolbar, pluginMenu, idgenerator);
    }
}

void ToolBarCreator::populatePluginToolbar(std::map <std::string, CToolBar*> wndtoolbar, PluginMenu item, IIdGenerator* idgenerator)
{
    std::string parent = item.mParentMenu;
for (auto itemmenu : item.menuList)
    {
        populate(wndtoolbar, itemmenu, parent, idgenerator);
    }
}

void ToolBarCreator::populate(std::map <std::string, CToolBar*> wndtoolbar, Menu itemmenu, std::string parentmenu, IIdGenerator* idgenerator)
{

    TBBUTTON    btn;

    CImageList* imageList = nullptr;
for (auto toolbar : wndtoolbar)
    {
        int btnCount = toolbar.second->GetToolBarCtrl().GetButtonCount();
        if (toolbar.first == parentmenu)
        {
            switch (itemmenu.mMenuType)
            {
                case eMenuTypes::PopUp:
                {
for (auto submenu : itemmenu.submenulist)
                    {
                        populate(wndtoolbar, submenu, parentmenu, idgenerator);
                    }
                    break;
                }
                /*if (itemmenu.mMenuType == eMenuTypes::PopUp)
                {
                for (auto submenu : itemmenu.submenulist)
                {
                populate(wndtoolbar, submenu, parentmenu, idgenerator);
                }
                }*/
                case eMenuTypes::Separator:
                {
                    ZeroMemory(&btn, sizeof(btn));
                    btn.fsStyle = TBBS_SEPARATOR;
                    toolbar.second->GetToolBarCtrl().InsertButton(btnCount, &btn);
                    break;
                }
                /*else if (itemmenu.mMenuType == eMenuTypes::Separator)
                {
                ZeroMemory(&btn, sizeof(btn));
                btn.fsStyle = TBBS_SEPARATOR;
                toolbar.second->GetToolBarCtrl().InsertButton(btnCount, &btn);
                }*/
                case eMenuTypes::MenuItem:
                {
                    if (itemmenu.mToolbar.isToolButtonReq)
                    {
                        ZeroMemory(&btn, sizeof(btn));
                        toolbar.second->EnableDocking(CBRS_ALIGN_ANY);
                        imageList = toolbar.second->GetToolBarCtrl().GetImageList();
                        index = imageList->GetImageCount();

                        idgenerator->generateId(itemmenu.mId, idgenerator->id);
                        CBitmap* bmp = new CBitmap();

                        //Enabled image list
                        if (!FileExist(itemmenu.mToolbar.iconPathEnable.c_str()))
                        {
                            bmp->Attach(LoadImage(AfxGetInstanceHandle(), DEFAULT_IMAGE_PATH, IMAGE_BITMAP, 32, 32, (LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_LOADFROMFILE)));
                        }
                        else
                        {
                            bmp->Attach(LoadImage(AfxGetInstanceHandle(), itemmenu.mToolbar.iconPathEnable.c_str(), IMAGE_BITMAP, 32, 32, (LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_LOADFROMFILE)));
                        }

                        imageList->Add(bmp, RGB(255, 255, 255));
                        bmp->Detach();
                        btn.idCommand = idgenerator->id;
                        btn.iBitmap = index;
                        btn.fsStyle = TBSTYLE_BUTTON;



                        //Disabled image list
                        imageList = toolbar.second->GetToolBarCtrl().GetDisabledImageList();
                        index = imageList->GetImageCount();
                        if (!FileExist(itemmenu.mToolbar.iconPathDisable.c_str()))
                        {
                            bmp->Attach(LoadImage(AfxGetInstanceHandle(), itemmenu.mToolbar.iconPathEnable.c_str(), IMAGE_BITMAP, 32, 32, (LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_LOADFROMFILE)));
                        }
                        else
                        {
                            bmp->Attach(LoadImage(AfxGetInstanceHandle(), itemmenu.mToolbar.iconPathDisable.c_str(), IMAGE_BITMAP, 32, 32, (LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_LOADFROMFILE)));
                        }
                        imageList->Add(bmp, RGB(255, 255, 255));
                        bmp->Detach();
                        toolbar.second->GetToolBarCtrl().SetDisabledImageList(imageList);
                        btn.idCommand = idgenerator->id;
                        btn.iBitmap = index;
                        btn.fsStyle = TBSTYLE_BUTTON;


                        //Hotimage Image List
                        imageList = toolbar.second->GetToolBarCtrl().GetHotImageList();
                        index = imageList->GetImageCount();
                        if (!FileExist(itemmenu.mToolbar.iconPathHot.c_str()))
                        {
                            bmp->Attach(LoadImage(AfxGetInstanceHandle(), itemmenu.mToolbar.iconPathEnable.c_str(), IMAGE_BITMAP, 32, 32, (LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_LOADFROMFILE)));
                        }
                        else
                        {
                            bmp->Attach(LoadImage(AfxGetInstanceHandle(), itemmenu.mToolbar.iconPathHot.c_str(), IMAGE_BITMAP, 32, 32, (LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_LOADFROMFILE)));
                        }
                        imageList->Add(bmp, RGB(255, 255, 255));
                        bmp->Detach();
                        toolbar.second->GetToolBarCtrl().SetHotImageList(imageList);
                        btn.idCommand = idgenerator->id;
                        btn.iBitmap = index;
                        btn.fsStyle = TBSTYLE_BUTTON;

                        toolbar.second->GetToolBarCtrl().InsertButton(btnCount, &btn);
                        break;
                    }
                }

            }
            break;
        }


    }

}

bool ToolBarCreator::FileExist(std::string path)
{
    int exists = PathFileExists(path.c_str());
    if (exists == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}