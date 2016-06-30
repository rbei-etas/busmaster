#include "../stdafx.h"
#include "BusmasterMenuItem.h"


BusmasterMenuItem::BusmasterMenuItem(CCmdUI* mfcMenu)
{
    mMfcMenu = mfcMenu;
}


BusmasterMenuItem::~BusmasterMenuItem()
{
}

void BusmasterMenuItem::enableItem(bool enable)
{
    mMfcMenu->Enable(enable);
}
void BusmasterMenuItem::checkItem(bool enable)
{
    mMfcMenu->SetCheck(enable);
}

void BusmasterMenuItem::setName(char*)
{

}
void BusmasterMenuItem::getName(char* name, int maxlength)
{

}
