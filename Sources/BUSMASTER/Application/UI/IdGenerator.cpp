#include "stdafx.h"
#include "IdGenerator.h"


int IdGenerator::generateId(std::string menuid, int& id)
{
    std::map<std::string, int>::iterator lastid;
    auto found = menuidlist.find(menuid);

    if (found != menuidlist.end())
    {
        id = found->second;
        return 0;
    }
    else
    {
        if (menuidlist.size() > 1)
        {
            lastid = menuidlist.end();
            --lastid;
            if (id < lastid->second)
            {
                id = lastid->second;
            }
        }

        menuidlist.insert(std::pair<std::string, int>(menuid, ++id));
        return 1;
    }
}

bool IdGenerator::removeid(std::string menuid)
{
    auto found = menuidlist.find(menuid);
    if (found != menuidlist.end())
    {
        menuidlist.erase(menuid);
        return true;
    }
    else
    {
        return false;
    }
}

int IdGenerator::setStartId(int mid)
{
    id = mid;
    return id;
}

std::string IdGenerator::GetMenuItem(UINT id, std::map<std::string, std::list<PluginMenu>> populatelist)
{
    std::string idname;
for (auto menuid : menuidlist)
    {
        if (id == menuid.second)
        {
            idname = menuid.first;
            break;
        }
    }
    if (idname.empty())
    {
        return idname;
    }
for (auto item : populatelist)
    {
        auto list = item.second;
for (auto pluginMenu : list)
        {
for (auto itemMenu : pluginMenu.menuList)
            {
                if (itemMenu.mMenuType == eMenuTypes::PopUp)
                {
for (auto item : itemMenu.submenulist)
                    {
                        if (idname == item.mId && !item.tooltip.empty())
                        {
                            return item.tooltip;
                        }
                    }
                }
                else if (idname == itemMenu.mId && !itemMenu.tooltip.empty())
                {
                    return itemMenu.tooltip;
                }
            }
        }
    }
}


