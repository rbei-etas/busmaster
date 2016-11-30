#include "../stdafx.h"
#include "BusmasterMenuItem.h"
#include "../IBusmasterPlugInManager.h"
#include "../MFCRibbonBarEx.h"
BusmasterMenuItem::BusmasterMenuItem(CMFCRibbonButtonEx* ribbonButton, CCmdUI* pMfcCmd)
{
    mRibbonButton = ribbonButton;
    mMfcMenu = pMfcCmd;
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

void BusmasterMenuItem::setName( char* newText )
{
    CString currentText = mRibbonButton->GetText();
    if (currentText != newText)
    {
        mRibbonButton->SetText(newText);
        auto parent = mRibbonButton->GetParent();
        if (nullptr != parent)
        {
            parent->ForceRecalcLayout();
        }
    }
}
void BusmasterMenuItem::getName(char* /*name*/, int /*maxlength*/)
{

}
void BusmasterMenuItem::setImageIndex(int index, BOOL isLarge)
{
    int currentIndex = mRibbonButton->GetImageIndex(isLarge);
    if (currentIndex != mRibbonButton->getStartImageIndex() + index)
    {
        mRibbonButton->SetImageIndex(mRibbonButton->getStartImageIndex() + index, isLarge);
        auto parent = mRibbonButton->GetParent();
        if (nullptr != parent)
        {
            parent->ForceRecalcLayout();
        }
    }
}