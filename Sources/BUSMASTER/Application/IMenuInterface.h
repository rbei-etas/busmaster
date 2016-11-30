#pragma once


class IMenuItem
{
public:
	virtual ~IMenuItem() {}
	virtual void enableItem(bool) = 0;
	virtual void checkItem(bool) = 0;
	virtual void setName(char*) = 0;
	virtual void getName(char*, int maxlength) = 0;
    virtual void setImageIndex(int index, BOOL isLarge) = 0;
};
class IMenuInterface
{
public:
	virtual void OnMenuItemClicked(const char* menuId) = 0;
	virtual void updateMenuItem(const char* menuId, IMenuItem*) = 0;
};