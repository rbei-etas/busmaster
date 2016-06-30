#pragma once

#include "../IBusmasterPlugin.h"
class BusmasterMenuItem : public IMenuItem
{
private:
    CCmdUI* mMfcMenu;
    std::string mPluginMenuId;
public:
    BusmasterMenuItem(CCmdUI*);
    ~BusmasterMenuItem();
    void enableItem(bool);
    void checkItem(bool);
    void setName(char*);
    void getName(char*, int maxlength);
};