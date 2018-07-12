#pragma once

#include "../IMenuInterface.h"
//#include "../UI/Menus.h"
#include "../MFCRibbonBarEx.h"
class BusmasterMenuItem : public IMenuItem
{
private:
    CMFCRibbonButtonEx* mRibbonButton;
    CCmdUI* mMfcMenu;
    /*std::string mPluginMenuId;
    RibbonButton mMenu;*/
public:
    BusmasterMenuItem(CMFCRibbonButtonEx*, CCmdUI* pMfcCmd);
    ~BusmasterMenuItem();
    void enableItem(bool);
    void checkItem(bool);
    void setName(char*);
    void getName(char*, int maxlength);
    void setImageIndex(int index, BOOL isLarge);
};