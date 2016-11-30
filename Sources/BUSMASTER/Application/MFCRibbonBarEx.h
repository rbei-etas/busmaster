#pragma once


// CMFCRibbonBarEx

class CMFCRibbonCategoryEx : public CMFCRibbonCategory
{
public:
	void setLayout(int width)
	{
		SetPanelsLayout(width);
	}
    void showElements(BOOL show)
    {
        ShowElements(show);
    }
};

class CMFCRibbonButtonIcon : public CMFCRibbonButton
{
public:
    void SetIcon(HICON Icon)
    {
        m_hIcon = Icon;
    }
};

class CMFCRibbonButtonEx : public CMFCRibbonButton
{
    int mStartImageIndex = 0;
    CMFCRibbonBar* mParent;
public:
    void SetParentRibbonBar(CMFCRibbonBar* pRibbonBar) 
    { 
        mParent = pRibbonBar;
    }
    void setStartImageIndex(int index)
    {
        mStartImageIndex = index;
    }
    int getStartImageIndex()
    {
        return mStartImageIndex;
    }
    CMFCRibbonBar* GetParent()
    {
        return mParent;
    }
    void enableItem(bool bOn)
    {
        const BOOL bIsDisabled = !bOn;

        if (IsDisabled() != bIsDisabled)
        {
            m_bIsDisabled = bIsDisabled;
            OnEnable(!bIsDisabled);
        }
    }
    void checkItem(bool checkItem)
    {
        OnCheck(checkItem);
    }
    
    void setImageIndex(int index, BOOL isLarge)
    {
        SetImageIndex(index, isLarge);
    }
	void EnableWindow(bool enable)
	{
		m_bIsDisabled = !enable;
	}
    
    
};
class CMFCRibbonBarEx : public CMFCRibbonBar
{
	DECLARE_DYNAMIC(CMFCRibbonBarEx)

public:
	CMFCRibbonBarEx();
    bool isWindowHidden();
    void ToggleWindowDisplay();
    void UpdateRibbonBarMinimisedIcon();
    BOOL OnShowRibbonContextMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit);
	virtual ~CMFCRibbonBarEx();
protected:
	//virtual BOOL OnShowRibbonQATMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit);
	//BOOL OnShowRibbonContextMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit);
protected:
	DECLARE_MESSAGE_MAP()
};


